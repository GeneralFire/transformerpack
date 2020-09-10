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
from configparser import ConfigParser
from xml.etree import ElementTree as ET

from yapsy.PluginFileLocator import IPluginFileAnalyzer

from app.control.FitPluginInfo import FitPluginInfo
from library.tool.exceptions import MissingAttributeException


class PluginMetaXmlAnalyzer(IPluginFileAnalyzer):
    """
        The class override default meta file parsing (Yapsy lib) and handles the xml way.
        It requires: name,version,meta_version,containers nodes
    """

    def __init__(self, name, extensions="mxml"):
        IPluginFileAnalyzer.__init__(self, name)
        self.setPluginInfoExtension(extensions)

    def setPluginInfoExtension(self, extensions):
        # Make sure extension is a tuple
        if not isinstance(extensions, tuple):
            extensions = (extensions,)
        self.expectedExtensions = extensions

    def isValidPlugin(self, filename):
        for ext in self.expectedExtensions:
            if filename.endswith(".%s" % ext):
                return True
        return False

    def getInfosDictFromPlugin(self, dirpath, filename):
        meta_path = os.path.join(dirpath, filename)
        tree = ET.parse(meta_path)
        root = tree.getroot()
        cf_parser = ConfigParser()
        cf_parser.add_section(FitPluginInfo.pluginInfoSection)
        for meta_enum in FitPluginInfo.MetaProperties:
            node = root.find(meta_enum.value)
            if node is not None:
                cf_parser.set(FitPluginInfo.pluginInfoSection, meta_enum.value, node.text)
            else:
                raise MissingAttributeException(f"Cannot find required tag '{meta_enum.value}' in meta xml", filename)
        cf_parser.set(FitPluginInfo.pluginInfoSection, FitPluginInfo.containersTag,
                      meta_path)
        cf_parser.set(FitPluginInfo.pluginInfoSection, FitPluginInfo.layoutTag,
                      meta_path)
        module_path = os.path.join(dirpath, cf_parser.get(FitPluginInfo.pluginInfoSection,
                                                          FitPluginInfo.MetaProperties.Module.value))
        name = cf_parser.get(FitPluginInfo.pluginInfoSection,
                             FitPluginInfo.MetaProperties.Name.value)

        infos = {"path": module_path, "name": name}
        cf_parser.add_section("Core")
        cf_parser.set("Core", "Name", name)
        cf_parser.set("Core", "Module", module_path)
        return infos, cf_parser
