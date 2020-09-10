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
from enum import Enum
from typing import Dict

from app.utils import Consts
from library.utils import join_real_path
from library.tool.UniqueKey import VersionedName
from library.ConfigData import ConfigData
import os.path


BuildPhaseEnum = Enum('BuildPhaseEnum', ['main', 'post'], start=0)


class IContainer:
    """
     API Interface - All containers should inherit from that class in order to be used as a container in the plugin
    """

    intermediate_directory = Consts.INTERMEDIATE_DIRECTORY

    def __init__(self):
        self.required_containers = {}
        self.name = ''
        self.version = ''
        self.platform = ''
        self.dependent_settings_paths = dict()
        self.dependent_settings = dict()
        self.image_data = None

    @property
    def unique_name(self) -> VersionedName:
        """
        Unique key for container. It is used in Fit to unambiguously identify each container
        :param self: No params
        :return: concatenation of name and version (Unique key for container) : str
        """
        return VersionedName(self.name, self.version)

    @property
    def configuration(self) -> ConfigData:
        """
        Configuration is optional property/ member of container. If any, the container's settings should be passed here.
        You can override here as a property with @property tag or just as a class member defining self.configuration variable
        :param self: No params
        :return: returns ConfigData object. It is DTO between plugin and FIT.
        """
        return ConfigData(None, '')

    @property
    @join_real_path()
    def output_path(self) -> str:
        """
        Fit calls this method in order to locate created binary by the container.
        Please assure that the binary is created in the same place.
        By default the path is concatenation unique name with .bin extension
        :param self:
        :return: path to produced container: str
        """
        return os.path.join(IContainer.intermediate_directory, str(VersionedName(self.name, self.version, separator='_')) + '.bin')

    def build(self, required_containers: Dict[VersionedName, str]):
        """
        This method should build the container  binary and write it to the same path as indicated in output_path method
        :param self:
        :param required_containers: dictionary- container's unique_name as VersionedName object, path to the container's binary
                Fit passes the other containers that was required by this container in meta xml.
        :return: Nothing
        """
        raise NotImplementedError("Build is not implemented", type(self))

    @property
    def map_data(self) -> ([int, int, int, str]):
        """Returns list of start offset, length, intent, area name"""
        return None
