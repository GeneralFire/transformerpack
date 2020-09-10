#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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
import re
import string
from enum import Enum
from xml.etree import ElementTree as ET

from .IComponent import IComponent
from ..Converter import Converter
from ..dependencies.Dependency import Dependency
from ..LibException import ComponentException
from ..structures import Buffer


# TODO property dynamically returning number of edited entries might be useful
class IterableComponent(IComponent):
    defaultTag = 'default'
    entryTag = 'entry'
    maxSizeTag = 'max_entry_count'
    indexTag = 'index'

    class ComponentProperty(Enum):
        Size = "size"
        Data = "data"
        Indices = "indices"

    def __init__(self, xml_node, **kwargs):
        self.default_element = None
        self.defaultEntriesList = []
        self.indices = []
        self._data = None
        self.max_size = None
        super().__init__(xml_node, **kwargs)

    def _parse_additional_attributes(self, xml_node):
        max_size_formula = self._parse_attribute(xml_node, self.maxSizeTag, False, None)
        if max_size_formula is not None:
            self.max_size = self.calculate_value(max_size_formula)

    def build_layout(self, buffer, clear_build_settings: bool = False):
        raise ComponentException('Trying to build GUI only element', self.name)

    def build(self, buffer):
        raise ComponentException('Trying to build GUI only element', self.name)

    @property
    def data(self):
        self._load_data()
        return self._data

    @property
    def entries(self):
        return [entry for entry in self.children if entry.node_tag != self.defaultTag]

    @property
    def entries_count(self):
        return len(self.entries)

    @property
    def descendants(self):
        return []

    def _load_data(self):
        buffer = Buffer(-1, 100000)
        for entry in self.entries:
            entry.build_layout(buffer)
        buffer.flush()
        for entry in self.entries:
            entry.build(buffer)
        self._data = buffer[0:buffer.tell()]
        buffer.flush()

    def _get_property(self, component_property, _=False):
        if component_property == self.ComponentProperty.Data:
            return self.data
        if component_property == self.ComponentProperty.Size:
            return self.entries_count
        if component_property == self.ComponentProperty.Indices:
            return self.indices

    def _copy_to(self, dst):
        super()._copy_to(dst)
        dst.children = self.children
        dst.children_by_name = self.children_by_name

    def _parse_children(self, xml_node, buffer=None):
        self.children = []
        self.children_by_name = {}
        index = 0
        if self.dependency_formula and Dependency.Tags.getTag in self.dependency_formula:
            return
        try:
            self._parse_default_node(xml_node)
            for child_node in xml_node:
                if child_node.tag == self.defaultTag:
                    continue
                if child_node.tag == self.entryTag:
                    self._parse_entry_node(child_node, index)
                else:
                    raise ComponentException(
                        f'Iterable children must be either "{self.entryTag}" or "{self.defaultTag}" '
                        f'- {child_node.tag} provided')
                index += 1
        except ComponentException as ex:
            self.trace_exception(ex)

    def _parse_default_node(self, xml_node):
        defaults = xml_node.findall(self.defaultTag)
        if len(defaults) == 0:
            raise ComponentException(f'No default definition for {self.name} iterable', self.name)
        if len(defaults) > 1:
            raise ComponentException(f'There is more than one default definition for {self.name} iterable',
                                     self.name)
        default_node = defaults[0]
        self._fill_default_structure(default_node)
        self.default_element = self.componentFactory.create_component(default_node)

    def _parse_entry_node(self, child_node, index):
        if self.indexTag in child_node.attrib:
            index = child_node.attrib[self.indexTag]
            if index.isdigit():
                index = Converter.string_to_int(index)
                if index in self.indices:
                    raise ComponentException(f'Index with value {index} already used')
            else:
                raise ComponentException('Iterable entry index must be numeric value')
            if index >= self.max_size:
                raise ComponentException(f'Number of entries exceeds max_size {self.max_size}')
        self._validate_entry_structure(child_node, index)
        self.indices.append(index)
        self.componentFactory.create_component(child_node, parent=self, index=index)

    # override
    def _get_child(self, child_name):
        if self.children_by_name is None:
            raise ComponentException("'{}' has no children".format(self.name), self.name)
        if child_name in self.children_by_name:
            return self.children_by_name[child_name]
        reg = re.search(r'^entry(\d+)$', child_name)
        if reg and self.max_size is not None and int(reg.group(1)) >= self.max_size:
            raise ComponentException(
                f'Trying to get configuration for non-existing index. Exceeded max_size: {reg.group(1)}')
        if reg and self.default_element:
            return self._add_new_entry(reg)
        raise ComponentException(
            f"No '{child_name}' child. Choose one of: {', '.join(self.children_by_name.keys())}", self.name)

    def _add_new_entry(self, reg: re):
        index = int(reg.group(1))
        entry = self.default_element.semideepcopy()
        entry.parent = self
        entry.name = f'{self.entryTag}{index}'
        entry._set_parent_child()
        self.indices.append(index)
        return entry

    def to_xml_node(self, parent, create_groups: bool = False):
        if not self.is_setting_saveable or (self.xml_save_formula and not self.calculate_value(self.xml_save_formula)):
            return
        for entry in self.entries:
            setting_childes = [child for child in entry.descendants if child.node_tag != self.groupTag]
            for child in setting_childes:
                if not child.is_setting_saveable:
                    continue
                if child.xml_save_formula and not child.calculate_value(child.xml_save_formula):
                    continue
                name = self.name + "[" + str(entry.index) + "]." + child.name
                node = ET.SubElement(parent, "setting", {self.nameTag: name})
                node.set(self.valueTag, child.get_value_string())

    def _fill_default_structure(self, default_node):
        self.defaultEntriesList = []
        for node in default_node.iter():
            if node.tag == self.groupTag or node.tag == self.defaultTag:
                continue
            setting_name = self.get_name(node)
            # names generated can have additional number at the end which can vary between entries
            setting_name = setting_name.rstrip(string.digits)
            self.defaultEntriesList.append(setting_name)

    def _validate_entry_structure(self, entry_node, index):
        node_settings = [self.get_name(setting).rstrip(string.digits) for setting in entry_node.iter()
                         if setting.tag != self.groupTag and setting.tag != self.entryTag]
        if len(node_settings) != len(self.defaultEntriesList):
            raise ComponentException(f"Number of settings in {index} entry doesn't match number of default entries'",
                                     self.name)
        for mandatory_default_name in self.defaultEntriesList:
            if mandatory_default_name not in node_settings:
                raise ComponentException(f"Mandatory setting name: {mandatory_default_name} missing in"
                                         f" {index} entry of {self.name} iterable")
        return

    @property
    def visible(self):
        return False

    @visible.setter
    def visible(self, val):
        pass

