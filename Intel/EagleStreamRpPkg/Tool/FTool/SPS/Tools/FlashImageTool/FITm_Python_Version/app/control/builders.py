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

from yapsy.PluginFileLocator import PluginFileLocator
from yapsy.PluginManager import PluginManager

from app.control.FitPluginInfo import FitPluginInfo
from app.control.PluginMetaXmlAnalyzer import PluginMetaXmlAnalyzer
from library.IFitPlugin import IFitPlugin

''' this module has all builders of the objects that are used in FIT'''


def create_plugin_manager(plugin_cat, plugin_ext):
    custom_plugin_locator = PluginFileLocator([PluginMetaXmlAnalyzer("fit_analyzer", plugin_ext)],
                                              plugin_info_cls=FitPluginInfo)
    custom_plugin_locator._default_plugin_info_cls = FitPluginInfo  # Older yapsy ver has bug, we have to override it ourselves
    plugin_mng = PluginManager(categories_filter={plugin_cat: IFitPlugin},
                               plugin_locator=custom_plugin_locator)
    return plugin_mng
