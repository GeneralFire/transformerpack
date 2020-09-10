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

import xml.etree.ElementTree as ET

from library.IContainer import BuildPhaseEnum
from library.tool.exceptions import GeneralException
from library.tool.utils import XmlAttr, XmlAttrType
from library.tool.UniqueKey import UniqueKey, VersionedName
from typing import Tuple, List, Dict


class ContainersInfoBuilder:
    """
    The class is responsible for parsing node: 'container' in meta xml (.mmxml)
    """
    containerTag = 'container'
    dependencyTag = 'dependency'


    @staticmethod
    def create_container_info_list(node: ET.ElementTree) -> List[Tuple[VersionedName, List[UniqueKey], BuildPhaseEnum]]:
        """
        Parsing <containers> node in a plugin's meta xml.
        :param node: XmlNode for the <containers> node
        :return: Dictionary of the all available containers in the plugin and theirs dependencies as List of UniqueKeys
        """
        containers = []
        containers_set = set()
        children = node.findall(ContainersInfoBuilder.containerTag)
        for cont_node in children:
            produced, required, build_phase = ContainersInfoBuilder.create_container_info(cont_node)
            if produced not in containers_set:
                containers.append((produced, required, build_phase))
            containers_set.add(produced)
        return containers

    @staticmethod
    def create_container_info(node: ET.ElementTree) -> (VersionedName, Dict[UniqueKey, bool], BuildPhaseEnum):
        """
        Parsing <container> node in a plugin's meta xml.
        :param node: XmlNode for the <container> node
        :return: Tuple of the given container as VersionedName, its dependencies as Dictionary of UniqueKeys
        and related required state, given container build phase
        """
        required_conts = {}
        name = XmlAttr(name='name', xml_node=node).value
        version = XmlAttr(name='version', xml_node=node).value
        versioned_name = VersionedName(name, version)
        build_phase_str = XmlAttr(name='build_phase', xml_node=node, is_required=False, default=BuildPhaseEnum.main.name).value
        accepted_build_phase_literals = [phase.name for phase in BuildPhaseEnum]
        if build_phase_str in accepted_build_phase_literals:
            build_phase = BuildPhaseEnum[build_phase_str]
        else:
            raise GeneralException(f"Error while loading {versioned_name} container. "
                                   f"Cannot parse build phase attribute value: '{build_phase_str}' Expected "
                                   f"values for build phase attribute are: {', '.join(accepted_build_phase_literals)}")
        for depend in node.findall(ContainersInfoBuilder.dependencyTag):
            depend_name = XmlAttr(name='name', xml_node=depend).value
            depend_version = XmlAttr(name='version', xml_node=depend, is_required=False, default='').value
            depend_plug_name = XmlAttr(name='plugin_name', xml_node=depend, is_required=False, default='').value
            depend_plug_version = XmlAttr(name='plugin_version', xml_node=depend, is_required=False, default='').value
            is_required = XmlAttr(name='is_required', xml_node=depend, is_required=False, attr_type=XmlAttrType.BOOL, default=False).value
            depend_container_key = VersionedName(depend_name, depend_version)
            depend_plugin_key = VersionedName(depend_plug_name, depend_plug_version)
            required_conts[UniqueKey(depend_container_key, depend_plugin_key)] = is_required
        return versioned_name, required_conts, build_phase

