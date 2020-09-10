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
import json
from collections import namedtuple
from enum import Enum

from app.components.FitComponentFactory import LayoutComponentFactory
from app.components.Container import ContainerWrapper
from library.GuiComponentFactory import GuiComponentFactory
from library.tool.BinaryGenerator import BinaryGenerator
from library.tool.components.IComponent import IComponent
from library.tool.exceptions import MissingAttributeException, GeneralException
from library.tool.LibConfig import LibConfig
from xml.etree import ElementTree as et
from library.tool.MapGenerator import MapGenerator
from app.control.FitMapFormatter import FitMapFormatter


class FitBinaryGenerator(BinaryGenerator):
    buildOptsTag = 'build_options'

    build_opts = None

    class BuildOptions(Enum):
        FlashSize1 = 'syncFlashComponent1Size'
        FlashSize2 = 'syncFlashComponent2Size'
        FlashSpiComponents = 'syncNumberOfSpiComponents'

    def __init__(self, xml_config_file, path_resolver=None):
        super().__init__(xml_config_file, path_resolver, GuiComponentFactory)
        self.max_size = 32 * 1024 * 1024
        self.pre_build = None
        self.post_build = None
        self.map_gen = MapGenerator(FitMapFormatter)

    def process_build_opts(self):
        '''
        Parse Build Options to set maximum size of the binary based on Number of SPI Components.
        Note: Raise GeneralException if layout XML doesn't contain required Build Options.
        '''

        self.max_size = self.get_build_option_value(self.BuildOptions.FlashSize1.value)
        flash_comps = self.get_build_option_value(self.BuildOptions.FlashSpiComponents.value)

        if flash_comps == 1:  # 0 == 1 component; 1 == 2 components
            self.max_size += self.get_build_option_value(self.BuildOptions.FlashSize2.value)

    def parse_configuration(self):
        root = self.xml_tree.getroot()
        self.root_component = LayoutComponentFactory().create_root_component(root)
        self.layout_root = self.root_component.children_by_name.get(self.layoutTag, None)
        self.build_opts = self.root_component.children_by_name.get(self.buildOptsTag, None)
        self.pre_build = root.find(self.preBuildTag)
        self.post_build = root.find(self.postBuildTag)

        self.configuration_root = self.root_component.children_by_name.get(LibConfig.settingsTag, None)
        if self.configuration_root is None:
            raise MissingAttributeException(f'Missing {LibConfig.settingsTag} in parsed layout xml.')
        self.configuration_root.to_xml_node = self._to_xml_node

        if self.layout_root is None:
            raise MissingAttributeException(f'Missing {self.layoutTag} in parsed layout xml.')

    def _to_xml_node(self, parent):
        node = et.SubElement(parent, ContainerWrapper.nodeName, {ContainerWrapper.nameTag: 'layout'})
        if self.configuration_root.children:
            for child in self.configuration_root.children:
                child.to_xml_node(node)

    def clear(self):
        if self.layout_root:
            self.layout_root.value = None
            self.layout_root.size = None

    @property
    def layout_containers(self):
        if self.layout_root:
            return self.layout_root.children
        else:
            return []

    @property
    def settings(self):
        """ Property which facilitates access to settings read from layout xml.
         Supports read only access. Hence, any changes done to the returned object are not reflected in stored configuration """
        children = self.configuration_root.children
        data = {child.name: child.value for child in children}
        settings_object = namedtuple('LayoutSettings', data.keys())(*data.values())
        return settings_object

    @property
    def region_order(self):
        return getattr(self.settings, 'region_order', '')

    @property
    def region_order_dict(self):
        region_order_dict = getattr(self.settings, 'region_order_dict', '').replace("'", "\"")
        return json.loads(region_order_dict)

    def get_region_id(self, name):
        if name == 'descriptor':
            return '0'

        for region_id, region_name in self.region_order_dict.items():
            if region_name == name:
                return region_id

        raise GeneralException(f"Could not find following Region ID for region called {name}")

    def apply_region_order(self):
        import copy
        if getattr(self.configuration_root, 'children', None):
            region_order_dict_keys = "".join(self.region_order_dict.keys())
            self.validate_region_order(self.region_order)
            self.validate_region_order(region_order_dict_keys)

            if len(self.region_order) != len(region_order_dict_keys):
                raise GeneralException(f"Number of specified regions in region_order setting is not equal to number of"
                                       f" regions in region_order_dict")

            if self.region_order_dict and self.region_order:
                layout_children = copy.copy(self.layout_root.children)
                descriptor = self._find_descriptor(layout_children)
                new_region_order = [descriptor] if descriptor else []

                for i in self.region_order:
                    region_name = self.region_order_dict[i]
                    region = next((layout_children.pop(layout_children.index(c)) for c in layout_children
                                   if c.name == region_name), None)
                    if not region:
                        raise GeneralException(
                            f"There is no region named {region_name}. Please correct region_order_dict "
                            f"setting in layout file")
                    new_region_order.append(region)
                self.layout_root.children = new_region_order

    @staticmethod
    def validate_region_order(string):
        letters = {}
        for letter in string:
            try:
                int(letter, 16)
            except ValueError:
                raise GeneralException(
                    f"There is an illegal character in region order: {string}.")
            if not letters.get(letter):
                letters[letter] = 1
            else:
                raise GeneralException(
                    f"There are duplicates in region order: {string}.")

    @staticmethod
    def _find_descriptor(layout_items):
        return next((layout_items.pop(layout_items.index(c)) for c in layout_items
                     if c.name == 'descriptor'), None)

    # we dont want to save components for FITm
    def save_components(self, root, directory):  # pragma: no cover
        pass

    def get_build_option_value(self, opt_name):
        '''
        Tries to get value of build option and throws GeneralException when requested option doesn't exist.

        :param opt_name: requested build option name
        :return: value of requested build option
        '''

        try:
            opt = next(opt for opt in self.build_opts.children if opt.name == opt_name)
            if opt.value is None:
                if opt.value_formula:
                    opt.value = opt.calculate_value(opt.value_formula, allow_calculate=True)
                else:
                    raise GeneralException(f'Missing value of {opt_name} in parsed Build Options.')
        except StopIteration:
            raise GeneralException(f'Missing {opt_name} in parsed Build Options.')
        else:
            return opt.value

    # we dont want to save components for FITm
    def save_components(self, root, directory):  # pragma: no cover
        pass