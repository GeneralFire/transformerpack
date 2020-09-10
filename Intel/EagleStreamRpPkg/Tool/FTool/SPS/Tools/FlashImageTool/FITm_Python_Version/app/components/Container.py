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
import xml.etree.ElementTree as ET
from typing import List, Union

from library.utils import is_proper_output_path
from library.tool.Converter import Converter
from library.tool.LibConfig import LibConfig
from library.tool.utils import check_file_path, XmlAttrType, XmlAttr
from library.tool.components.IComponent import IComponent
from library.tool.exceptions import InvalidAttributeException, MissingAttributeException
from library.tool.UniqueKey import UniqueKey, VersionedName
from library.tool.LibException import LibException, ComponentException
from library.XmlContainer import XmlContainer
from library.IContainer import IContainer, BuildPhaseEnum
from library.tool.components.DecompositionComponent import DecompositionComponent

from library.tool.ColorPrint import ColorPrint
from app.utils import Consts


class ContainerWrapper(IComponent):
    '''
        Represents each container in layout xml
    '''

    configVerTag = 'config_version'
    nodeName = 'container'
    nameTag = 'name'
    versionTag = 'version'
    plugNameTag = 'plugin_name'
    plugVerTag = 'plugin_version'
    binTag = 'binary'
    isReqTag = 'is_required'
    canBeBinTag = 'can_be_binary'
    orderChangeableTag = 'order_changeable'

    def __init__(self, xml_node, **kwargs):
        self._init_vars()
        self._parse_attributes(xml_node)
        super().__init__(xml_node, **kwargs)

    # base class implementation does not parse attributes of components that are disabled
    # there is no such case that disabled IComponent becomes enabled later - unless this is a ContainerWrapper
    # if we omit parsing disabled ContainerWrappers, than they would miss some properties that are essential
    # for enabled container - like 'padding', 'map_name'
    def _should_omit_parsing(self, xml_node):
        super()._should_omit_parsing(xml_node)
        return False

    def _parse_attributes(self, xml_node):
        name = XmlAttr(name=self.nameTag, xml_node=xml_node)
        version = XmlAttr(name=self.versionTag, is_required=False, default='', xml_node=xml_node)
        plug_name = XmlAttr(name=self.plugNameTag, is_required=False, default='', xml_node=xml_node)
        plug_ver = XmlAttr(name=self.plugVerTag, is_required=False, default='', xml_node=xml_node)
        can_be_bin = XmlAttr(name=self.canBeBinTag, is_required=False, default=False, attr_type=XmlAttrType.BOOL,
                             xml_node=xml_node)
        order_changeable = XmlAttr(name=self.orderChangeableTag, is_required=False, default=True, attr_type=XmlAttrType.BOOL,
                             xml_node=xml_node)
        try:
            self.can_be_binary = can_be_bin.value
            self.order_changeable = order_changeable.value
            ver_name = VersionedName(name.value, version.value)
            ver_name_plugin = VersionedName(plug_name.value, plug_ver.value)
            self.unique_key = UniqueKey(ver_name, ver_name_plugin)
            self._is_req = XmlAttr(name=self.isReqTag, is_required=False,
                                   attr_type=XmlAttrType.BOOL, default=True, xml_node=xml_node).value
        except (LibException, MissingAttributeException) as e:
            raise ComponentException(str(e), self.nodeName)

    def _init_vars(self):
        self.container_ref: Union[IContainer, None] = None
        self.is_present_in_layout = False
        self.is_external_bin = False
        self.order_changeable = True
        self.binary_path = ''
        self._enabled = None  # enabled property helper
        self._default_enabled = True

    def _get_name(self, xml_node):
        return self.unique_key.cont_key.name

    def build_layout(self, buffer, clear_build_settings=False):
        if self.should_build_layout():
            super().build_layout(buffer, clear_build_settings)

    def _build_layout(self):
        if self.is_binary_ready():
            with open(self.binary_path, 'rb') as binary:
                self.value = binary.read()
            self.size = self._calc_requested_region_size()
        elif self.build_phase == BuildPhaseEnum.post:
            if self.size_formula:  # size_formula is accessible for layout containers only
                self.size = self.calculate_value(self.size_formula)
                if self.size == 0:
                    raise ComponentException("Post build phase container size has to be predefined. It cannot be "
                                           "set to 0x0.")
            else:
                self.size = 0
        elif self.is_required:
            raise InvalidAttributeException(f"Binary does not exist: {self.binary_path}", 'Container ' + self.name)

    def _calc_requested_region_size(self):
        data_size = len(self.value)
        if self.size_formula is not None:
            requested_size = self.calculate_value(self.size_formula, allow_calculate=True)
            if requested_size:
                if requested_size < data_size:
                    raise ComponentException(f"Region size cannot be set to {hex(requested_size)} "
                                             f"Minimum size is {hex(data_size)}", self.name)
                if self.padding_formula is not None:
                    padding_value = self.calculate_value(self.padding_formula, allow_calculate=True)
                    if padding_value and requested_size % padding_value:
                        raise ComponentException(f"Region size must be a multiple of {hex(padding_value)} "
                                                 f"Requested region size is {hex(requested_size)}", self.name)
                return requested_size
        return data_size

    def build(self, buffer):
        if self.should_build():
            super().build(buffer)

    def _get_bytes(self):
        return self.value

    def set_external_binary(self, path, enabled=True):
        if self.can_be_binary:
            self.enabled = enabled
            self.is_present_in_layout = enabled
            self.binary_path = path
            self.is_external_bin = True
        else:
            ColorPrint.warning(f"Warning: Container {self.name} cannot be supplied by binary!\n"
                               f"Container {self.unique_key.to_format_str()} will be build instead.")

    def unset_external_binary(self):
        if self.is_external_bin and self.can_be_binary:
            self.enabled = self.is_required
            self.is_present_in_layout = self.is_required
            self.binary_path = ""
            self.is_external_bin = False
            self.size = None
            self.offset = None
        else:
            ColorPrint.warning(f"Cannot unset binary override for {self.unique_key.to_format_str()}."
                               f"This is applicable only to containers that can be binary and are already overridden")

    def is_binary_ready(self):
        return check_file_path(self.binary_path, True, False)

    def should_build(self):  # when we have no binary and container is not required, we can pass the build
        return self.is_binary_ready() or self.is_required

    def should_build_layout(self):
        return self.should_build() or (self._is_enabled() and self.build_phase == BuildPhaseEnum.post)

    @property
    def global_unique_key(self) -> UniqueKey:
        return self.unique_key

    @property
    def container_unique_key(self) -> VersionedName:
        return self.unique_key.cont_key

    @property
    def plugin_unique_key(self):
        return self.unique_key.plug_key

    @property
    def is_required(self):
        return self._is_req

    def is_from_plugin(self):
        return self.container_ref is not None

    def get_decomp_dependency(self):
        return next((comp for comp in self.container_ref.generator.root_component.children
                     if isinstance(comp, DecompositionComponent)), None)

    def has_decomp_dependency(self):
        if type(self.container_ref) == XmlContainer:
            return self.get_decomp_dependency() is not None
        return False

    @property
    def dependencies(self):
        if self.is_external_bin:
            return {}
        return self.container_ref.required_containers

    def get_config(self, platform=None):
        if self.container_ref:
            self.container_ref.platform = platform
            config_data = self.container_ref.configuration
            config_data.unique_key = self.unique_key
            return config_data

    def apply_cmd_overrides(self, param_overrides) -> List[IComponent]:
        if isinstance(self.container_ref, XmlContainer):
            return self.container_ref.generator.apply_cmd_overrides(param_overrides)
        else:
            raise InvalidAttributeException(f'Settings in this type of container cannot be overridden: {self.name}')

    def call_build(self, dependencies: dict):
        if self.is_external_bin:
            ColorPrint.debug(f"Using binary at '{self.binary_path}' for "
                             f"{self.container_unique_key.name} container...")
            return
        else:
            ColorPrint.debug(f"Building {self.container_unique_key.name} container...")

        if not is_proper_output_path(self.container_ref.output_path):
            raise ComponentException("Wrong output path '{}', should be inside '{}' folder"
                .format(self.container_ref.output_path, os.path.join(LibConfig.coreDir, Consts.INTERMEDIATE_DIRECTORY)))

        self.container_ref.build(dependencies)
        self.binary_path = self.container_ref.output_path

    def to_xml_node(self, parent, create_groups=False):
        # TODO: move creating node to some builder
        config = self.get_config()
        node = ET.SubElement(parent, self.nodeName, {self.nameTag: self.name})
        if not self.is_external_bin and not LibConfig.isDecompose:
            if self.container_unique_key.has_name_ver():
                node.set(self.versionTag, self.unique_key.cont_key.version)
            if not self.plugin_unique_key.is_empty():
                node.set(self.plugNameTag, self.unique_key.plug_key.name)
                node.set(self.plugVerTag, self.unique_key.plug_key.version)
            if config and config.settings_tree:
                node.set(self.configVerTag, str(config.config_version))
                node.set(self.enabledTag, str(self.enabled))
                if not LibConfig.isDecompose:
                    for setting in config.settings_tree.values():
                        setting.to_xml_node(node, create_groups)

        if (self.binary_path and self.is_external_bin) or \
                (not self.is_from_plugin() and not LibConfig.isDecompose):
            node.set(self.binTag, self.binary_path)
        if self.binary_path and self.is_external_bin:
            node.set(self.enabledTag, str(self.enabled))

    def get_property(self, property_name, allow_calculate=False):
        if self.is_present_in_layout:
            return super().get_property(property_name, allow_calculate)
        # if a ContainerWrapper is not present in layout, than it means that it is not enabled actually
        # this is necessary to satisfy FLREGs calculate formula
        elif property_name == self.enabledTag:
            self.enabled = False
            return self.enabled
        raise InvalidAttributeException(f"Attribute {self.name} is not enabled.")

    # override
    @property
    def enabled(self):
        if self._enabled is None:
            if self.enabled_formula is not None:
                self._enabled = self._parse_enabled()
            else:
                self._enabled = self._default_enabled
        return self._enabled

    @enabled.setter
    def enabled(self, value: bool):
        self._enabled = value

    # override
    def _is_enabled(self):
        return self.enabled

    def get_child(self, child_name):
        child = self.children_by_name.get(child_name, None)
        if child is None:
            configs = self.get_config()
            child = configs.setting(child_name)
        return child

    def clear(self, new_layout=True):
        self.offset = None
        self.size = None
        self.value = None
        if not self.is_external_bin:
            self.binary_path = None

        if new_layout:
            self.binary_path = None
            self.is_present_in_layout = self.is_from_plugin()
            self.is_external_bin = False
            self._enabled = None

    @property
    def map_data(self) -> ([int, int, int, str]):
        """Returns list of start offset, length, intent, area name"""
        data = super().map_data
        if self.container_ref and self.container_ref.map_data:
            cont_data = self.container_ref.map_data
            # append cont children with offset recalculation and intent:
            for child in cont_data:
                data.append((child[0] + self.offset, child[1], child[2] + 1, child[3]))
        return data

    @property
    def build_phase(self):
        if self.container_ref and not self.is_external_bin:
            return self.container_ref.build_phase
        else:
            return BuildPhaseEnum.main

    @property
    def image_data(self):
        return self.container_ref.image_data if self.container_ref else None

    @image_data.setter
    def image_data(self, value):
        if self.container_ref:
            self.container_ref.image_data = value


class DependContainerWrapper(ContainerWrapper):
    '''
        Represents each container from the plugin
    '''

    def __init__(self, **kwargs):
        self._init_vars()
        self.can_be_binary = False
        self.unique_key = kwargs.get('unique_key', VersionedName())
        self.container_ref = kwargs.get('container_ref')
        self._is_req = kwargs.get('is_required', True)
        self.offset = 0
        self.size = 0
        self.name = self.unique_key.cont_key.name
        self.children = []
        self.children_by_name = {}


class OverContainerWrapper(ContainerWrapper):
    '''
        Represents each container from the user xml.
        The configuration of the containers will be overridden with these values
    '''

    def __init__(self, xml_node, **kwargs):
        super().__init__(xml_node, **kwargs)

    def _init_vars(self):
        super()._init_vars()
        self._default_enabled = None
        self.config_version = ''

    def _parse_attributes(self, xml_node):
        name = XmlAttr(name=self.nameTag, xml_node=xml_node)
        version = XmlAttr(name=self.versionTag, xml_node=xml_node, is_required=False, )
        plug_name = XmlAttr(name=self.plugNameTag, is_required=False, xml_node=xml_node)
        plug_ver = XmlAttr(name=self.plugVerTag, is_required=False, xml_node=xml_node)
        config_ver = XmlAttr(name=self.configVerTag, attr_type=XmlAttrType.VERSION, is_required=False,
                             xml_node=xml_node)
        binary_path = XmlAttr(name=self.binTag, is_required=False, attr_type=XmlAttrType.PATH, xml_node=xml_node)
        ver_name = VersionedName(name.value, version.value)
        plug_ver_name = VersionedName(plug_name.value, plug_ver.value)
        self.unique_key = UniqueKey(ver_name, plug_ver_name)
        self.config_version = config_ver.value
        self.enabled_formula = self._parse_attribute(xml_node, self.enabledTag, False, None)
        if binary_path.value:
            self.binary_path = binary_path.value

    # override
    def _should_omit_parsing(self, xml_node):
        return False