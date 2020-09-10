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
from yapsy.IPlugin import IPlugin
from typing import List, Tuple
from enum import Enum

from library.ActionExecutor import ActionExecutor
from library.tool.exceptions import MissingAttributeException
from library.tool.UniqueKey import VersionedName
from library.IContainer import IContainer


class ContainerTupleOrder(Enum):
    """
    Enum helper, it indicates proper order of tuple in "containers" property
    """
    Class = 0
    Kwargs = 1


class IFitPlugin(ActionExecutor, IPlugin):
    """
        Interface for plugins. It is used in communication between FitEngine and the plugins

        :var name: The member is required to recognize the plugin by the API.
               It should match with the name in the meta xml
    """
    name = "Plugin"

    def __init__(self):
        super().__init__()
        self.used_containers = {}

    @property
    def containers(self) -> List[Tuple[IContainer, dict]]:
        """
            Each plugin should provide containers that will be included in image binary
            or that used by the other containers
            In the name of programmers' comfort, this can be property (@property tag) or object member (self.containers).
            From API perspective it is no matter

            :return: List of tuples (container_class_name, Kwargs):
                    :container: IContainer: name of the container class that inherit from IContainer
                    :kwargs: dict[str,str]: Kwargs are passed to the containers object constructor
                                            It is common pythonic way to map constructor arguments
                                            Dictionary should look like: key: argument_name
                                                                         value: argument
                                            Kwargs is not required part of the tuple (If constructor with no args needed)
        """
        return []

    def get_container(self, unique_key: VersionedName) -> IContainer:
        """
        API method. It should not be overridden in most cases.
        This method creates containers lazily.
        In the name of performance the containers are created only if they are used in Fit.
        :param unique_key: Fit uses container unique key to request specific container
        :return: Plugin's container that inherit from IContainer
        """
        if unique_key in self.used_containers:
            return self.used_containers[unique_key]
        else:
            raise MissingAttributeException(
                "Internal Error: Could not find container {} in plugin {}".format(unique_key.name, self.name))

    @staticmethod
    def plugins_dir(file__) -> str:
        """
        Helper method to obtain the plugins directory
        :param file__: provide __file__ variable of your plugin module
        :return: absolute plugins directory path: str
        """
        return os.path.dirname(os.path.realpath(file__))

    @staticmethod
    def abs_join_plugins_dir(filename, file__) -> str:
        """
        Helper method to create absolute path of the file located in plugins directory
        :param filename: name of the desired file to be joined with plugins directory
        :param file__: provide __file__ variable of your plugin module
        :return: absolute path to the file with 'filename' located in plugins directory: str
        """
        return os.path.join(IFitPlugin.plugins_dir(file__), filename)
