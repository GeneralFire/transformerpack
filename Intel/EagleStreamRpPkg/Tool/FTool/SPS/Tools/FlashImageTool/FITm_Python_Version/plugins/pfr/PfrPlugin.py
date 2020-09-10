"""
INTEL CONFIDENTIAL
Copyright 2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""

import collections
from library.IFitPlugin import IFitPlugin
from library.XmlContainer import XmlContainer
from library.tool.components.IComponent import IComponent
from library.tool.exceptions import InvalidSizeException, InvalidAttributeException
from library.tool.structures import Buffer


class PfrContainerBase(XmlContainer):
    # xml name for pfm entries
    pfm_entries_name = "pfm_entries"
    pft_table_name = "pfm_spi_body"
    region_start_address = "region_start_address"
    image_size = "image_size"
    pfr_region_size = 0x2810000

    class PfmEntry:
        # xml pfm attributes
        include_name = "include"
        region_name = "name"
        start_offset_name = "start_offset"
        end_offset_name = "end_offset"

        def __init__(self, pfm_entry):
            self.region = ""
            self.start_offset = 0
            self.end_offset = 0
            self.include = False
            self.parse_pfm_entry(pfm_entry)
            self.include_range = tuple

        def parse_pfm_entry(self, pfm_entry):
            self.region = pfm_entry.get_child(self.region_name).value
            self.start_offset = pfm_entry.get_child(self.start_offset_name).value
            self.end_offset = pfm_entry.get_child(self.end_offset_name).value
            self.include = pfm_entry.get_child(self.include_name).value

    def get_config_setting(self, setting_name: str):
        return self.configuration.setting(setting_name)

    def get_layout_setting(self, setting_name: str):
        return next((setting for setting in self.generator.layout_root.descendants
                     if setting.name == setting_name), None)

    def get_global_offset(self, entry: PfmEntry) -> int:
        region_data = self.image_data[entry.region.lower()]
        return region_data[0] + entry.start_offset

    def get_container_data(self, entry: PfmEntry)-> bytearray:
        # required containers file in configuration should always have name 'ContainerBinary'
        # where Container should match value from pfm entry value_list
        setting_name = entry.region + 'Binary'
        container_file = self.configuration.setting(setting_name)
        container_data = container_file.get_property('data')
        start = entry.start_offset
        end = entry.end_offset
        if end == 0:
            region_data = self.image_data[entry.region.lower()]
            end = region_data[1]
            # in case of extended container we have to add padding
            if end != len(container_data):
                padding_length = end - len(container_data)
                container_data += bytearray(b'\xFF') * padding_length
        if end < start:
            raise InvalidAttributeException(f"Misconfiguration detected. Start offset: {start} is larger than"
                                            f" end offset: {end} in {entry.region} entry", self.name)
        return container_data[start: end]

    def get_entry_length(self, entry: PfmEntry):
        if entry.end_offset != 0:
            return entry.end_offset - entry.start_offset
        if entry.region == "Pfr":
            return self.pfr_region_size - entry.start_offset
        return len(self.get_container_data(entry))

    def sort_iterable_entries(self, pfm_entries):
        entries_offsets: dict = {}
        for entry in pfm_entries:
            entry_wrapper = self.PfmEntry(entry)
            entries_offsets[self.get_global_offset(entry_wrapper)] = entry
        ordered_entries = collections.OrderedDict(sorted(entries_offsets.items()))
        # add children in proper order
        pfm_entries.children = []
        pfm_entries.children_by_name = {}
        for sorted_entry in ordered_entries.values():
            pfm_entries.children.append(sorted_entry)
            pfm_entries.children_by_name[sorted_entry.name] = sorted_entry


class CapsuleContainer(PfrContainerBase):

    class PbcConfig:
        input_buffer = Buffer(-1, 1)
        output_buffer = Buffer(-1, 1)
        active_map = bytearray(b'\xFF')
        compress_map = bytearray(b'\xFF')
        input_length = 0
        output_length = 0
        page_size = 0
        page_pattern = 0
        payload_size = 0
        data_size = 0

    def __init__(self, xml_name, main_dir='', ** kwargs):
        super().__init__(xml_name, main_dir, ** kwargs)
        self.config = self.PbcConfig()

    def build(self, required_containers):
        super().build(required_containers)
        self.fill_pbc_config()
        self.fill_capsule()
        self.generator.save(self.output_path)

    def fill_pbc_config(self):
        self.config.page_size = self.get_config_setting("page_size").value
        self.config.output_buffer = self.generator.buffer
        self.config.output_length = len(self.generator.buffer)
        map_size = self.get_config_setting("size_of_bit_map").value
        self.config.active_map = bytearray(b'\x00') * map_size
        self.config.compress_map = bytearray(b'\x00') * map_size
        self.config.payload_offset = self.get_layout_setting("compressed_image").offset
        self.config.current_payload_offset = self.config.payload_offset

    def fill_capsule(self):
        pfm_entries = self.get_config_setting(self.pfm_entries_name)
        self.sort_iterable_entries(pfm_entries)
        pfm_entry_objects = (self.PfmEntry(entry) for entry in pfm_entries)
        for entry in pfm_entry_objects:
            if entry.include:
                self.pbc_compress(entry)
        self.update_payload_size()
        for entry in pfm_entry_objects:
            if not entry.include:
                self.pbc_erase(entry)
        self.update_bit_maps()
        self.generator.buffer.seek(self.config.current_payload_offset)

    def pbc_compress(self, entry: PfrContainerBase.PfmEntry):
        empty_buffer = bytearray(b'\xFF') * self.config.page_size
        self.validate_offsets(entry)
        input_buffer = self.get_container_data(entry)
        ptr_in = 0
        ptr_out = self.config.output_buffer
        global_entry_offset = self.get_global_offset(entry)
        pbc_offset = int(global_entry_offset / self.config.page_size)
        local_pbc_offset = 0
        current_offset = self.config.current_payload_offset
        while local_pbc_offset * self.config.page_size < len(input_buffer):
            ptr_out.seek(current_offset)
            if current_offset + self.config.page_size > self.config.output_length:
                raise InvalidSizeException(f"Data exceeds output buffer:{self.config.output_length}")
            if input_buffer[ptr_in: ptr_in + self.config.page_size] != empty_buffer:
                ptr_out.write(input_buffer[ptr_in: ptr_in + self.config.page_size])
                current_offset += self.config.page_size
                # 1 bit means the page should be taken, 0 means - it fits to the pattern
                self.config.compress_map[pbc_offset >> 3] |= 1 << (7 - (pbc_offset % 8))
            # 1 bit means it will erased - we should erase all pages, except the explicitly protected
            self.config.active_map[pbc_offset >> 3] |= 1 << (7 - (pbc_offset % 8))
            ptr_in += self.config.page_size
            local_pbc_offset += 1
            pbc_offset += 1
        self.config.payload_size = current_offset - self.config.payload_offset
        self.config.current_payload_offset = current_offset

    def validate_offsets(self, entry):
        if entry.start_offset % self.config.page_size != 0:
            raise InvalidAttributeException(f"Start offset: {entry.start_offset} is not aligned to page size: "
                                            f"{self.config.page_size} for PFM entry related to {entry.region}")
        if entry.end_offset % self.config.page_size != 0:
            raise InvalidAttributeException(f"End offset: {entry.end_offset} is not aligned to page size: "
                                            f"{self.config.page_size} for PFM entry related to {entry.region}")

    def pbc_erase(self, entry: PfrContainerBase.PfmEntry):
        global_entry_offset = self.get_global_offset(entry)
        pbc_offset = int(global_entry_offset / self.config.page_size)
        input_buffer = self.get_container_data(entry.region, entry.start_offset, entry.end_offset)
        local_pbc_offset = 0
        while local_pbc_offset * self.config.page_size < len(input_buffer):
            # set '0' bit for current page to exclude page from decompression
            self.config.compressMap[pbc_offset >> 3] &= not (1 << (7 - (pbc_offset % 8)))
            # set '0' bit for current page to exclude page from erasement
            self.config.activeMap[pbc_offset >> 3] &= not (1 << (7 - (pbc_offset % 8)))
            local_pbc_offset += 1
            pbc_offset += 1

    def update_bit_maps(self):
        active_bit_map = self.get_layout_setting("active_bit_map")
        compression_bit_map = self.get_layout_setting("compression_bit_map")
        self.config.output_buffer.seek(active_bit_map.offset)
        self.config.output_buffer.write(self.config.active_map)
        self.config.output_buffer.seek(compression_bit_map.offset)
        self.config.output_buffer.write(self.config.compress_map)

    def update_payload_size(self):
        payload_length = self.get_layout_setting("payload_length")
        self.config.output_buffer.seek(payload_length.offset)
        binary_value = self.config.payload_size.to_bytes(payload_length.size, IComponent.bigOrder)
        self.config.output_buffer.write(binary_value)


class PfrPlugin(IFitPlugin):
    """
        PFR (Platform Firmware Resilience) plugin creates binary from pfr.xml
    """
    name = "pfr"
    pluginPath = IFitPlugin.plugins_dir(__file__)
    containers = [(XmlContainer, {"xml_name": "signed_pfm.xml", "main_dir": pluginPath}),
                  (CapsuleContainer, {"xml_name": "capsule.xml", "main_dir": pluginPath}),
                  (XmlContainer, {"xml_name": "signed_capsule.xml", "main_dir": pluginPath}),
                  (XmlContainer, {"xml_name": "pfr.xml", "main_dir": pluginPath})]

    def __init__(self):
        super().__init__()

