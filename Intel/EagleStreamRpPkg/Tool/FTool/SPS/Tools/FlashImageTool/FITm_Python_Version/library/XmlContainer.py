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
from json import JSONDecodeError

from library.CoreDataComponent import CoreDataComponent
from library.GuiComponentFactory import GuiComponentFactory
from library.tool.BinaryGenerator import BinaryGenerator, PathResolver
from library.tool.LibException import *
from library.ConfigData import ConfigData
from library.tool.MapGenerator import XmlInfoFormatter
from library.tool.utils import XmlAttrType, XmlAttr
from library.tool.exceptions import GeneralException, InvalidAttributeException
from library.IContainer import IContainer, BuildPhaseEnum
from library.tool.components.IComponent import IComponent
from library.tool.LibConfig import LibConfig
from typing import Dict

imageDataTag = "image_data"
requiredContainersTag = "required_containers"


class ApiComponent(IComponent):

    def __init__(self, **kwargs):
        self.name = kwargs.get("name")
        self.value = kwargs.get("value", None)
        self.enabled = kwargs.get("enabled", True)
        self.parent = kwargs.get("parent", None)
        self.children_by_name = {}
        self.children = []
        self._set_parent_child()

    # override
    def _is_enabled(self):
        return self.enabled


class XmlContainer(IContainer):
    """
        Fully ready Container class.
        The binary is created from xml config with the structure described in User Guide
        In order to use this class you should provide path to the xml,
        place it in containers list in your plugin and
        attached it to meta xml file of your plugin

        If applicable, feel free to inherit from this class
    """

    configVerTag = "version"

    def __init__(self, xml_name, main_dir='', **kwargs):
        """
        :param xml_name: Xml path or just name (when you put xml to the same place as in 'main_dir')
        :param main_dir: Provide the temporary current directory for the binary generator
                         This can be useful when you want to use the relative paths for the dependencies.
        """
        super().__init__()
        self._xml_name = xml_name
        self._xmls = kwargs.get("xmls", None)
        self.main_dir = main_dir
        self.config_comp = None
        self._configuration = None
        self.path_resolver = PathResolver(LibConfig.coreDir, self.main_dir)
        self._generator = None
        self.dependent_settings_paths = kwargs.get('dependent_settings', dict())
        self.dependent_settings = dict()
        self.save_info = False

    def prepare_dependent_container_nodes(self, required_containers):
        components = self._create_required_containers_components(required_containers)
        self.generator.parse_build_nodes()
        self.generator.add_child_to_root(components)

    def prepare_fit_core_node(self):
        self.generator.add_child_to_root(CoreDataComponent())

    def prepare_image_data_node(self):
        image_data_node = ApiComponent(name=imageDataTag)
        for cont_name, cont_data in self.image_data.items():
            cont_node = ApiComponent(name=cont_name, parent=image_data_node)
            offset, size, present_in_layout = cont_data
            ApiComponent(name="offset", value=offset, parent=cont_node)
            ApiComponent(name="size", value=size, parent=cont_node)
            ApiComponent(name="enabled", value=present_in_layout, parent=cont_node)
        self.generator.add_child_to_root(image_data_node)

    def build(self, required_containers):
        try:
            self.generator.parse_layout()
            self.prepare_dependent_container_nodes(required_containers)
            self.prepare_fit_core_node()
            if self.build_phase == BuildPhaseEnum.post:
                self.prepare_image_data_node()
            self.generator.build_layout()
            self.generator.build()
            self.generator.save(self.output_path)
            if self.save_info:
                self.generator.map_gen.formatter = XmlInfoFormatter
                self.generator.save_info(os.path.join(IContainer.intermediate_directory, self.name + '_info.xml'), True)
        except LibException as ex:
            path_split = os.path.split(self.xml_path)
            raise GeneralException(f"Failed to build image from xml: {path_split[-1]}, an error occurred: {ex}", self.name)
        return os.path.abspath(self.output_path)

    @property
    def generator(self):
        if self._generator is None:
            self._generator = self._create_generator()
        return self._generator

    def _create_generator(self):
        return BinaryGenerator(self.xml_path, self.path_resolver, GuiComponentFactory)

    @property
    def configuration(self):
        if not self._configuration:
            self._configuration = self._load_configuration()
        return self._configuration

    @property
    def xml_path(self):
        return os.path.join(self.main_dir, self._xml_name)

    @staticmethod
    def _collect_settings_recursive(settings: list, only_overidable_settings: bool) -> Dict[str, list]:
        grouped = {}
        ungrouped = []
        for sett in settings:
            if sett.tag in IComponent.optionTypeMap or sett.tag in IComponent.multiLevelComponentsTags:
                collected = XmlContainer._collect_settings_recursive(sett, only_overidable_settings)
                grouped[sett.attrib[IComponent.nameTag]] = collected
                if sett.tag not in IComponent.multiLevelComponentsTags and not (sett.tag == IComponent.groupTag and IComponent.enabledTag in sett.attrib):
                    continue

            if not only_overidable_settings or IComponent.is_node_overridable(sett):
                ungrouped.append(sett)

        merged = {}
        if len(ungrouped):
            merged.update({'': ungrouped})
        merged.update(grouped)
        return merged

    @property
    def _overridable_setting_nodes_grouped(self):
        settings = self.setting_nodes
        res = self._collect_settings_recursive(settings, True)
        return res

    @property
    def _all_setting_nodes_grouped(self):
        settings = self.setting_nodes
        res = self._collect_settings_recursive(settings, False)
        return res

    @staticmethod
    def _aggregate_groups_path(settings: dict, path: str = ''):
        sett_info = {}
        for k, v in settings.items():
            new_key = '/'.join(filter(None, [path, k]))
            if isinstance(v, dict):
                agg = XmlContainer._aggregate_groups_path(v, new_key)
                sett_info = {**sett_info, **agg}
            else:
                sett_info[new_key] = {sett.get(IComponent.nameTag): sett.tag for sett in v}
        return sett_info

    @property
    def overridable_settings_info(self) -> Dict[str, Dict[str, str]]:
        """
        Retrieves info for settings available to be overridden withe their respective groups
        :return: A non-nested dict where key represents settings group and the value contains list of settings descriptions
        """
        settings = self._overridable_setting_nodes_grouped
        sett_info = self._aggregate_groups_path(settings)
        return sett_info

    @property
    def all_settings_info(self) -> Dict[str, Dict[str, str]]:
        """
        Retrieves info for settings available to be overridden withe their respective groups
        :return: A non-nested dict where key represents settings group and the value contains list of settings descriptions
        """
        settings = self._all_setting_nodes_grouped
        sett_info = self._aggregate_groups_path(settings)
        return sett_info

    @property
    def overridable_settings_info_flat(self) -> Dict[str, str]:
        """
        Retrieves info for settings available to be overridden
        :return: Dictionary containing overridable settings with their type
        """
        sett_info = {}
        settings = self.overridable_settings_info
        for val in settings.values():
            sett_info = {**sett_info, **val}

        return sett_info

    @property
    def all_settings_info_flat(self) -> Dict[str, str]:
        """
        Retrieves info for settings available to be overridden
        :return: Dictionary containing overridable settings with their type
        """
        sett_info = {}
        settings = self.all_settings_info
        for val in settings.values():
            sett_info = {**sett_info, **val}

        return sett_info

    @property
    def setting_nodes(self):
        settings = self.generator.xml_tree.find(LibConfig.settingsTag)
        return list(settings)

    def _load_configuration(self):
        """
        Function is responsible for parsing and creating proper instances of
        configuration setting from container's xml
        """
        try:
            self._switch_xml_for_container()
            self.generator.parse_configuration()
            self.config_comp = self.generator.configuration_root
            config_node = self.generator.get_xml_root().find(self.generator.buildOptsTag)
            version_attr = XmlAttr(name=self.configVerTag, attr_type=XmlAttrType.VERSION,
                                   xml_node=config_node)
            return ConfigData(self.config_comp, version_attr.value)
        except InvalidAttributeException:
            raise
        except (GeneralException, LibException) as ex:
            raise GeneralException(f"Failed to parse {os.path.join(self.main_dir, self.xml_path)}, an error occurred: {ex} in '{self.name}' container")
        except JSONDecodeError as ex:
            raise GeneralException(f"Failed to parse {os.path.join(self.main_dir, self.xml_path)}, an error occurred: '{ex.msg}' while parsing '{ex.doc}'")

    def _switch_xml_for_container(self):
        if self._xmls and self.platform:
            name = self._xmls.get(self.platform, None)
            if name:
                self._xml_name = name
                self.generator.switch_xml(self.xml_path, self.path_resolver)
            else:
                raise InvalidAttributeException(f"Can't find platform name '{self.platform}' in available list"
                                                f" {list(self._xmls.keys())} for '{self.name}' container")

    @staticmethod
    def _create_required_containers_components(required_containers: dict):
        parent_node = ApiComponent(name=requiredContainersTag)
        for key, bin_path in required_containers.items():
            ApiComponent(name=key.cont_key.name, parent=parent_node, value=bin_path, enabled=bool(bin_path))
        return parent_node

    @property
    def map_data(self) -> ([int, int, int, str]):
        """Returns list of start offset, length, intent, area name"""
        if self.generator.layout_root:
            return self.generator.layout_root.map_data
        return None
