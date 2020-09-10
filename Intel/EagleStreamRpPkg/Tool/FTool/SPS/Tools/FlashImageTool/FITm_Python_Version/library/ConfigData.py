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

from library.tool.components.IComponent import IComponent
from library.tool.LibException import ComponentException
from library.tool.exceptions import InvalidAttributeException, MissingAttributeException
from library.tool.UniqueKey import UniqueKey


class ConfigData:
    '''
        Container for the configuration data of the layout containers.
        These structure is DTO between FitEngine and the plugins.
        All settings from the xml are stored here as a list of settings
    '''

    def __init__(self, config: IComponent, config_version: str):
        """
        Constructor
        :param config: configuration wrapper, should contain all settings of container as its children
        :param config_version: configuration version as str. Version is checked by FIT in order to check
            that plugins use handled API by the core
        """
        self.config = config
        self._settings = {}
        self.config_version = config_version
        self.unique_key: UniqueKey = None

    @property
    def settings(self):
        if self.config and not self._settings:
            try:
                self.config.get_all_children(self._settings)
            except ComponentException as e:
                raise InvalidAttributeException(
                    f"Could not acquire settings from container: {self.unique_key.to_format_str()}\n{str(e)}")
        return self._settings

    @property
    def settings_tree(self):
        if self.config:
            return self.config.children_by_name
        return {}

    def setting(self, name):
        if name in self.settings:
            return self.settings[name]
        else:
            raise MissingAttributeException(f"Could not find given setting: {name}")
