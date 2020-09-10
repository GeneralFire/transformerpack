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

from distutils.version import StrictVersion
from enum import Enum
from xml.etree import ElementTree as ET
from typing import Tuple, List
from app.control.ContainersInfoBuilder import ContainersInfoBuilder
from library.IContainer import BuildPhaseEnum
from library.tool.exceptions import MissingAttributeException
from library.tool.UniqueKey import VersionedName, UniqueKey

from yapsy.PluginInfo import PluginInfo


class FitPluginInfo(PluginInfo):
    class MetaProperties(Enum):
        Name = 'name'
        Module = 'module'
        Version = 'version'
        MetaVersion = 'meta_version'

    layoutTag = 'supported_layouts'
    pluginInfoSection = 'Plugin_Info'
    containersTag = 'containers'

    def __init__(self, plugin_name, plugin_path):
        super().__init__(plugin_name, plugin_path)
        self._parsed_containers = {}

    @property
    def unique_key(self):
        return VersionedName(self.name, str(self.version))

    @property
    def meta_version(self):
        return StrictVersion(self.details.get(self.pluginInfoSection, self.MetaProperties.MetaVersion.value))

    @property
    def version(self):
        return StrictVersion(self.details.get(self.pluginInfoSection, self.MetaProperties.Version.value))

    @property
    def supported_layouts(self):
        if self.details.has_option(self.pluginInfoSection, self.layoutTag):
            meta_xml_path = self.details.get(self.pluginInfoSection, self.layoutTag)
            tree = ET.parse(meta_xml_path)
            root = tree.getroot()
            supported_layouts = root.find(self.layoutTag)
            if supported_layouts is not None:
                return supported_layouts.text.split(';')
        return []

    def is_layout_supported(self, layout_name):
        supported_layouts = self.supported_layouts
        is_supported = False
        for lname in supported_layouts:
            if lname.lower() == layout_name.lower():
                is_supported = True
                break
        return is_supported or supported_layouts == []

    @property
    def containers_meta(self) -> List[Tuple[VersionedName, List[UniqueKey], BuildPhaseEnum]]:
        if not self._parsed_containers and \
                self.details.has_option(self.pluginInfoSection, self.containersTag):
            meta_xml_path = self.details.get(self.pluginInfoSection, "Containers")
            tree = ET.parse(meta_xml_path)
            root = tree.getroot()
            self._parsed_containers = ContainersInfoBuilder.create_container_info_list(root.find(self.containersTag))
        return self._parsed_containers

    def _ensureDetailsDefaultsAreBackwardCompatible(self):
        pass
