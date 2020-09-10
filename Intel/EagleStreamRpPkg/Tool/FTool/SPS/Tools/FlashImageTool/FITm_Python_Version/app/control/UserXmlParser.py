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
from xml.dom import minidom

from library.tool.exceptions import GeneralException, MissingAttributeException
from library.tool.utils import LibException, XmlAttr
from library.tool.LibConfig import LibConfig
from app.components.FitComponentFactory import UserXmlComponentFactory
from library.version import FIT_VERSION

copyrightComment = """<!--
INTEL CONFIDENTIAL
Copyright 2019-2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
-->

"""


class UserXmlParser:
    '''
        Parsing user xml which includes configuration for all containers
    '''
    rootTag = 'user_settings'
    containerTag = 'container'
    setVerTag = 'config_version'
    nameTag = 'name'
    layoutTag = 'layout'
    layoutVersionTag = 'layout_version'
    coreVerTag = 'core_version'

    def __init__(self, xml_path):
        self.xml_path = xml_path

    def parse_all(self):
        try:
            return self._parse_all_throwing()
        except (GeneralException, LibException, ET.ParseError) as e:
            raise GeneralException((e.args[0] + '. Could not parse the file: %s :\n' % self.xml_path), 'Xml parser')

    def save_all(self, containers: iter, layout_name, layout_version, layout_settings):
        root = ET.Element(self.rootTag, {self.layoutTag: layout_name, self.layoutVersionTag: layout_version, self.coreVerTag: FIT_VERSION})
        layout_settings.to_xml_node(root)
        for cont in containers:
            cont.to_xml_node(root)
        self.save_node_to_file(root, self.xml_path)

    @staticmethod
    def save_node_to_file(node, xml_path, include_copyright=False):
        if not os.access(xml_path, os.W_OK) and os.path.isfile(xml_path):
            raise GeneralException("Could not save to read-only file")
        raw_xml_string = ET.tostring(node, 'utf-8').decode("utf-8")
        if include_copyright:
            raw_xml_string = copyrightComment + '\n' + raw_xml_string
        reparsed = minidom.parseString(raw_xml_string)
        final_xml = reparsed.toprettyxml(indent=' ' * 4)
        with open(xml_path, 'w') as f:
            f.write(final_xml)

    def _parse_all_throwing(self):
        overridden_containers = []
        tree = ET.parse(self.xml_path)
        root = tree.getroot()
        layout_attr = XmlAttr(name='layout', is_required=True, xml_node=root)
        containers = root.findall(self.containerTag)
        layout_settings = None
        layout_settings_node = next((cont for cont in containers if XmlAttr(name=self.nameTag, xml_node=cont).value == self.layoutTag), None)
        if layout_settings_node:
            containers.remove(layout_settings_node)
            layout_settings_node.tag = LibConfig.settingsTag
            layout_settings = UserXmlComponentFactory().create_root_component(layout_settings_node)

        for cont_node in containers:
            container = UserXmlComponentFactory().create_component(cont_node)
            overridden_containers.append(container)
        return overridden_containers, layout_attr.value, layout_settings
