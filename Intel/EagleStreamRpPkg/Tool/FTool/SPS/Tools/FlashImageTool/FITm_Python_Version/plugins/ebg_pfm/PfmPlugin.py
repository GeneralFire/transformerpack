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
from library.tool.exceptions import InvalidAttributeException
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


class PfmContainer(PfrContainerBase):
    def build(self, required_containers):
        super().build(required_containers)
        self.sort_pfm_entries()
        self.validate_pfm_entries_cover()

    def sort_pfm_entries(self):
        pfm_entries = self.get_config_setting(self.pfm_entries_name)
        self.sort_iterable_entries(pfm_entries)
        self.sort_table_entries_in_image()

    def sort_table_entries_in_image(self):
        table_entries = self.get_layout_setting(self.pft_table_name)
        entries_buffer_offset = table_entries.offset
        entries_offsets: dict = {}
        for entry in table_entries:
            region_start = [setting for setting in entry.descendants if setting.name == self.region_start_address]
            start_settings_length = len(region_start)
            if start_settings_length != 1:
                raise InvalidAttributeException(f"There should be only one {self.region_start_address} setting in "
                                                f"{self.pft_table_name}, found {start_settings_length} settings",
                                                self.name)
            region_start_offset = region_start[0].value
            entries_offsets[region_start_offset] = entry
        ordered_entries = collections.OrderedDict(sorted(entries_offsets.items()))
        sorted_buffer = Buffer(-1, table_entries.size)
        for entry in ordered_entries.values():
            sorted_buffer.write(self.generator.buffer[entry.offset: entry.offset + entry.size])
        self.generator.buffer.seek(entries_buffer_offset)
        self.generator.buffer.write(sorted_buffer)

    def validate_pfm_entries_cover(self):
        # pfm entries should be already sorted by global offset
        pfm_entries = self.get_config_setting(self.pfm_entries_name)
        pfm_entry_objects = (self.PfmEntry(entry) for entry in pfm_entries)
        current_image_offset = 0
        for entry in pfm_entry_objects:
            image_offset = self.get_global_offset(entry)
            if image_offset < current_image_offset:
                raise InvalidAttributeException(f"Pfm entries overlap detected. Entry: {entry.region} 'start' offset: "
                                                f"{image_offset} overlaps previous entry 'end' offset: "
                                                f"{current_image_offset}", self.name)
            if image_offset > current_image_offset:
                raise InvalidAttributeException(f"Pfm does not cover all image. Entry: {entry.region} 'start' offset: "
                                                f"{image_offset} does not match previous entry 'end' offset: "
                                                f"{current_image_offset}", self.name)
            current_image_offset = image_offset + self.get_entry_length(entry)
        # check if last entry end offset doesn't match image size
        image_size = self.get_config_setting(self.image_size).value
        if current_image_offset != image_size:
            raise InvalidAttributeException(f"Last pfm entry 'end' offset: {current_image_offset} doesn't match "
                                            f"image length: {image_size}", self.name)


class PfmPlugin(IFitPlugin):
    """
        PFR (Platform Firmware Resilience) plugin creates binary from pfr.xml
    """
    name = "ebg_pfm"
    pluginPath = IFitPlugin.plugins_dir(__file__)
    containers = [(PfmContainer, {"xml_name": "pfm_egs.xml", "main_dir": pluginPath})]

    def __init__(self):
        super().__init__()

