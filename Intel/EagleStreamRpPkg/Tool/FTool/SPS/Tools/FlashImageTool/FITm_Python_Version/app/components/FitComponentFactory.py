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

from app.components.Container import ContainerWrapper, OverContainerWrapper
from app.components.LayoutComponent import LayoutComponent
from library.tool.components.ComponentFactory import ComponentFactory
from library.tool.components.IComponent import IComponent


class LayoutComponentFactory(ComponentFactory):

    def __init__(self):
        super().__init__()
        self._class_map[ContainerWrapper.nodeName] = ContainerWrapper
        self._class_map[LayoutComponent.nodeName] = LayoutComponent


class UserXmlComponentFactory(ComponentFactory):

    def __init__(self):
        super().__init__()
        self._class_map[OverContainerWrapper.nodeName] = OverContainerWrapper
        self._class_map[Setting.nodeName] = Setting


class Setting(IComponent):
    '''
        @class: Setting is used as handler for xml user nodes,
          which values would be placed instead of default ones. To override value we don't care about setting type,
          but only new value
    '''
    nodeName = 'setting'

    def _parse_string_value(self, value):
        return value