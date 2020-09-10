#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
INTEL CONFIDENTIAL
Copyright 2017-2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""
import os
from collections import namedtuple
from typing import List

from app.components.Container import ContainerWrapper
from app.control.ConfigsManager import ConfigsManager
from app.control.FitBinaryGenerator import FitBinaryGenerator
from app.utils import LayoutXml
from library.ActionExecutor import ActionExecutor
from library.ConfigData import ConfigData
from library.tool.ColorPrint import ColorPrint
from library.tool.UniqueKey import UniqueKey, VersionedName
from library.tool.components.FileComponent import FileComponent
from library.tool.components.IComponent import IComponent
from library.tool.exceptions import GeneralException, MissingAttributeException, InvalidSizeException
from library.tool.utils import LibException
from library.tool.utils import validate_path_to_file

PlatformMap = {}


class LayoutEngine(ActionExecutor):
    """
        LayoutEngine class is responsible for building final image from the xml layout.
        It also loads layout from xml with LayoutParser help
    """

    def __init__(self, input_layout: str):
        super().__init__()
        self.containers_list: [ContainerWrapper] = []
        self.containers_by_unique_name: dict[str, ContainerWrapper] = {}
        self.input_layout: str = input_layout
        self.platform_name: str = ''
        self.layout_name: str = ''
        self.layout_version: str = ''
        self.name: str = 'image'
        self.bin_generator: FitBinaryGenerator = None
        self.xml_overrides = []
        self.map_path: str = ""
        self.build_info_avail: bool = False
        self.image_data = {}
        self.two_flash_comps = False
        self.flash_1_size = 0
        self.flash_2_size = 0

    def fill_platform_map(self):
        if os.path.isdir(self.input_layout):
            self._fill_platform_map_from_directory()

        elif LayoutXml.check_if_file_is_layout(self.input_layout):
            self._update_platform_map(LayoutXml.get_platform_attrib_from_layout(self.input_layout), self.input_layout)

    @staticmethod
    def _clean_platform_map():
        temp = {}
        for name, entry in PlatformMap.items():
            temp[name] = entry.path

        PlatformMap.update(temp)

    def _fill_platform_map_from_directory(self):
        for file in sorted(os.listdir(self.input_layout)):
            layout_path = os.path.join(self.input_layout, file)
            Entry = namedtuple('PlatformMapEntry', 'version path')

            if LayoutXml.check_if_file_is_layout(layout_path):
                layout_name = LayoutXml.get_platform_attrib_from_layout(layout_path)
                layout_version = LayoutXml.get_platform_attrib_from_layout(layout_path, attrib='layout_version')
                layout_entry = Entry(layout_version, layout_path)

                if layout_name:
                    self._update_platform_map(layout_name, layout_entry)

        self._clean_platform_map()

    @staticmethod
    def _update_platform_map(layout_name, layout_entry):
        existing_entry = PlatformMap.get(layout_name, None)
        if existing_entry:
            ColorPrint.warning(f"Layout '{layout_name}' already exists '{PlatformMap[layout_name].path}' "
                               f"(version: {PlatformMap[layout_name].version}).")
            if existing_entry.version < layout_entry.version:
                ColorPrint.warning(f"Overwriting path with '{layout_entry.path}' "
                                   f"(version: {layout_entry.version}).")
            else:
                ColorPrint.warning(f"Duplicate layout '{layout_entry.path}' "
                                   f"(version: {layout_entry.version}) will not be used.")
                return

        PlatformMap[layout_name] = layout_entry

    def pick_layout_from_platform_map(self):
        if self.platform_name:
            try:
                self.input_layout = PlatformMap[self.platform_name]
            except KeyError:
                raise GeneralException(f'Could not load layout for chosen configuration: {self.platform_name}')
        else:
            self.platform_name = self.platform_names[0]
            self.input_layout = self.platform_paths[0]

    @property
    def platform_paths(self):
        platform_paths = list(PlatformMap.values())
        if not platform_paths:
            raise GeneralException(f'Could not load any layouts from directory: {self.input_layout}')
        return platform_paths

    @property
    def platform_names(self):
        platform_names = list(PlatformMap.keys())
        if not platform_names:
            raise GeneralException(f'Could not load any layouts from directory: {self.input_layout}')
        return platform_names

    def set_layout_parameters_after_choosing_new_platform(self, platform):
        try:
            self.input_layout = PlatformMap[platform]
            self.platform_name = platform
        except KeyError:
            raise GeneralException(f'Could not find layout for specified configuration with layout name: {platform}')

    def load_layout(self):
        """ Loading layout from xml. LayoutEngine stores the containers from layout """
        self.layout_name = LayoutXml.get_platform_attrib_from_layout(self.input_layout)
        self.layout_version = LayoutXml.get_platform_attrib_from_layout(self.input_layout, attrib='layout_version')
        if not self.layout_version:
            raise GeneralException(f'Could not load layout version.')
        self.bin_generator = FitBinaryGenerator(self.input_layout)
        try:
            ColorPrint.info(f'Loading {self.input_layout} layout')
            self.bin_generator.parse_configuration()
        except LibException as e:
            raise GeneralException(str(e))
        self.containers_list = self.bin_generator.layout_containers

    def update_containers(self):
        self.containers_by_unique_name = {}
        for container in self.containers_list:
            self.containers_by_unique_name[container.global_unique_key] = container

    def apply_layout_overrides(self, layout_overrides=None):
        if layout_overrides:
            for container_name in layout_overrides:
                containers = {c for c in self.containers_list if c.name == container_name}
                if not containers:
                    raise GeneralException(f"Cannot find container to override: {container_name}")
                for container in containers:
                    container.set_external_binary(layout_overrides[container_name])

    def apply_region_enabling_cmd(self, param_overrides=None):
        if param_overrides:
            for container in self.containers_list:
                ConfigsManager.container_enabler(container, param_overrides)

    @property
    def flash_size(self):
        self.flash_1_size = self.bin_generator.get_build_option_value(FitBinaryGenerator.BuildOptions.FlashSize1.value)
        flash_comp_count = self.bin_generator.get_build_option_value(FitBinaryGenerator.BuildOptions.FlashSpiComponents.value)

        flash_size = self.flash_1_size
        # 0 == 1 component; 1 == 2 components
        self.two_flash_comps = flash_comp_count == 1
        if self.two_flash_comps:
            self.flash_2_size = self.bin_generator.get_build_option_value(FitBinaryGenerator.BuildOptions.FlashSize2.value)
            flash_size += self.flash_2_size
        return flash_size

    def _calculate_free_image_space(self) -> int:
        containers = filter(lambda cont: cont.is_present_in_layout, self.containers_list)
        containers_size = sum(cont.size for cont in containers)

        if containers_size > self.flash_size:
            raise InvalidSizeException('Containers size exceeds the available flash space.'
                                       ' Consider setting higher flash size.')
        return self.flash_size - containers_size

    def _get_extended_container(self):
        # the feature is not accessible for specified layout if there is no such param
        # the feature is disabled if param value is empty
        if not 'extended_region' in self.settings_root.children_by_name or not self.settings.extended_region:
            return None
        container = next((cont for cont in self.containers_list if cont.name == self.settings.extended_region), None)
        if not container or container.is_present_in_layout is False:
            raise MissingAttributeException('Extended container could not be found: ' + self.settings.extended_region)
        return container

    def _extend_container(self, extend_size: int, print_log: bool = True):
        extended_cont = self._get_extended_container()
        if extended_cont is None:
            return
        containers = filter(lambda cont: cont.is_present_in_layout, self.containers_list)
        if print_log:
            ColorPrint.debug(
                f'Extending [{extended_cont.name}] size from {hex(extended_cont.size)} to'
                f' {hex(extended_cont.size + extend_size)}')
        extended_cont.size += extend_size
        extended_cont.padding = b'\xFF' * extend_size
        for cont in containers:
            if cont.offset > extended_cont.offset:
                cont.offset += extend_size
        self.bin_generator.layout_root.size = self.flash_size

    def build_main(self):
        """ Build main phase containers and lock containers offsets and sizes """
        try:
            self.bin_generator.clear()
            self.image_data = None
            self.bin_generator.process_build_opts()
            self.bin_generator.build_layout(clear_build_settings=True)
            extend_size = self._calculate_free_image_space()
            if self._enable_conditional_region_if_needed(extend_size):
                # region was enabled - we need to reevaluate the layout
                self.bin_generator.layout_root.size = None
                self.bin_generator.build_layout(clear_build_settings=True)
                extend_size = self._calculate_free_image_space()
            self._extend_container(extend_size, False)
            self.image_data = {cont.name: (cont.offset, cont.size, cont.is_present_in_layout) for cont in self.containers_list}
        except LibException as e:
            raise GeneralException(str(e)) from e

    def build_post(self):
        """ Concatenating binaries from containers to whole image """
        ColorPrint.info('Building image...')
        try:
            #  the method below loads container binaries so it has to be called again to load post phase conts
            self.bin_generator.build_layout()
            #  Unfortunately, build_layout() method removes extended_cont  expansion - it needs to be reapplied
            extended_cont = self._get_extended_container()
            if extended_cont:
                _, desired_size, __ = self.image_data[extended_cont.name]
                self._extend_container(desired_size - extended_cont.size)
            self.bin_generator.build()
            self._verify_offsets_and_sizes()
            self.build_info_avail = True
            ColorPrint.success("Image successfully built")
        except LibException as e:
            raise GeneralException(str(e)) from e

    def _verify_offsets_and_sizes(self):
        for cont in self.containers_list:
            offset, size, _ = self.image_data[cont.name]
            if cont.size != size:
                raise GeneralException(
                    f"Container {cont.name} has change its size {size} -> {cont.size} after main build phase what is forbidden.")
            if cont.offset != offset:
                raise GeneralException(
                    f"Container {cont.name} has change its offset {offset} -> {cont.offset} after main build phase what is forbidden.")

    def _enable_conditional_region_if_needed(self, extend_size):
        # the action is available for specified layout if both parameters are provided
        # the action is disabled when conditional region is set to ""
        if not all(param in self.settings_root.children_by_name for param in
                   ['extended_region_limit_size', 'conditional_region'])\
                or not self.settings.conditional_region:
            return False  # action should not be executed
        else:
            container_to_extend = self._get_extended_container()
            if container_to_extend is None or self.settings.extended_region_limit_size is None:
                return False
            if container_to_extend.size + extend_size <= self.settings.extended_region_limit_size:
                return False

            conditional_container = next((i for i in self.containers_list if i.name == self.settings.conditional_region), None)
            if conditional_container is None:
                ColorPrint.warning(f"Missing '{self.settings.conditional_region}' region - "
                                   f"cannot enable it to limit {container_to_extend.name} size.")
                return False

            if conditional_container.enabled:
                # Don't do anything if conditional_container is already enabled
                return False

            conditional_container._enabled = True
            conditional_container.is_present_in_layout = True
            conditional_container.call_build({})
            ColorPrint.info(f'{conditional_container.label} region has been enabled in order to fill unused flash space.',
                            gui_visible=True)

            return True

    @property
    def settings_root(self) -> IComponent:
        return self.bin_generator.configuration_root

    @property
    def settings(self):
        """ Property which facilitates access to settings read from layout xml.
         Supports read only access. Hence, any changes done to the returned object are not reflected in stored configuration """
        children = self.settings_root.children if self.settings_root.children else ''
        data = {child.name: child.value for child in children}
        settings_object = namedtuple('LayoutSettings', data.keys())(*data.values())
        return settings_object

    def get_setting(self, setting_name: str):
        try:
            return next(filter(lambda s: setting_name == s.name, self.settings_root.descendants))
        except StopIteration:
            raise MissingAttributeException(
                f"Setting could not be acquired: {setting_name} from 'layout'")

    def override_layout_settings(self, params):
        if params:
            for param in params:
                param_name, param_value = param.split('=')
                self.override_layout_setting(param_name, param_value)

    def override_layout_setting(self, name, value):
        child = next((c for c in self.settings_root.children if c.name == name), None)
        if not child:
            raise GeneralException(f"Could not find {name} in layout's settings node", self.name)
        if isinstance(child, FileComponent):
            validate_path_to_file(self.name, value, True)
        child.parse_string_value(value)
        self.bin_generator.apply_region_order()
        return child

    def unload_all_external_binaries(self):
        for cont in self.containers_list:
            if cont.is_external_bin:
                cont.unset_external_binary()

    def save_built_image(self):
        if not hasattr(self.settings, "output_file"):
            raise GeneralException("Could not find output_file in layout's settings node")
        try:
            out_path = self.settings.output_file
            self.map_path = os.path.splitext(out_path)[0]

            # write full binary
            self.bin_generator.save(out_path)

            # write flash components if needed
            if self.two_flash_comps:
                flash_comps_paths = self.get_flash_comps_out_paths(out_path)
                self.bin_generator.save(flash_comps_paths[0], 0, self.flash_1_size)
                self.bin_generator.save(flash_comps_paths[1], self.flash_1_size, self.bin_generator.buffer.tell())

            self.map_path = self.bin_generator.save_info(self.map_path)
            abs_path = os.path.abspath(out_path)
            ColorPrint.info("Map created at: {}".format(self.map_path), gui_visible=True)
            ColorPrint.success("Binary created at: {}".format(abs_path), gui_visible=True)
            return abs_path
        except LibException as e:
            raise GeneralException(str(e))

    def set_xml_overrides(self, xml_settings: List[IComponent]):
        self.xml_overrides = xml_settings

    def apply_setting_xml_overrides(self):
        config = ConfigData(self.settings_root, '')
        config.unique_key = UniqueKey(VersionedName(self.platform_name))
        ConfigsManager.override_config_for(config, self.xml_overrides)
        self.xml_overrides.clear()

    def overridable_settings_info(self):
        return {setting.name: setting.node_tag for setting in self.settings_root.children}

    @staticmethod
    def get_flash_comps_out_paths(out_path: str):
        out_path_parts = out_path.split('.')
        if -1 != out_path.find('.'):
            return [out_path_parts[0] + "(1)." + out_path_parts[1], out_path_parts[0] + "(2)." + out_path_parts[1]]
        else:
            return [out_path_parts[0] + "(1)", out_path_parts[0] + "(2)"]
