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
from typing import Dict
from subprocess import Popen, PIPE, TimeoutExpired
import os
import sys
import xml.etree.ElementTree as ET

from app.control.UserXmlParser import UserXmlParser
from library.XmlContainer import XmlContainer
from library.tool.ColorPrint import ColorPrint
from library.tool.UniqueKey import VersionedName
from library.tool.exceptions import ExternalProcessException, GeneralException
from library.tool.LibConfig import LibConfig


class ExecutableContainer(XmlContainer):
    """
    Container that calls an executable to build container binary and get list of settings.
    """

    def __init__(self, executable, xml_name='configuration.xml', exe_config_name='config.xml', main_dir='', **kwargs):
        """
        :param executable: executable path or just name (when you put it in the same place as in 'main_dir')
        :param xml_name: path/name to configuration file with all configurable settings that executable should provide
        :param exe_config_name: path/name to configuration file for executable, with all settings already configured
        """
        super().__init__(xml_name, main_dir, **kwargs)
        self.executable = executable
        self.timeout = kwargs.get('timeout', 60)
        self._exe_config_name = exe_config_name

    @property
    def exe_config_path(self):
        return os.path.join(self.main_dir, self._exe_config_name)

    def _call_executable(self, args):
        try:
            executable_path = os.path.join(self.main_dir, self.executable)
            if sys.platform in ('win32', 'cygwin'):
                executable_path += ".exe"
            process = Popen(executable=executable_path,
                            args=[executable_path] + args,
                            stdout=PIPE,
                            stderr=PIPE,
                            universal_newlines=True,
                            cwd=os.getcwd())
        except Exception as e:
            raise ExternalProcessException(f'Failed to run external application\n{e}',
                                           executable=self.executable,
                                           arguments=args,
                                           cwd=os.getcwd())

        try:
            output, error_output = process.communicate(timeout=self.timeout)
        except TimeoutExpired:
            process.kill()
            # Now getting output from the process should be really swift
            # but just in case of some strange error lets specify some timeout
            output, error_output = process.communicate(timeout=5)
            raise ExternalProcessException(f'Application timed out after {self.timeout} seconds.',
                                           executable=self.executable,
                                           arguments=args,
                                           cwd=os.getcwd(),
                                           output=output,
                                           error_output=error_output)
        except Exception as e:
            raise ExternalProcessException(f'Application failed\n{e}',
                                           executable=self.executable,
                                           arguments=args,
                                           cwd=os.getcwd())
        if process.returncode != 0:
            raise ExternalProcessException(error_output, self.name)

        if LibConfig.isVerbose or error_output:
            ColorPrint.info(f"OUTPUT:\n{output}")
        if error_output:
            ColorPrint.warning(f'{self.executable} returned {process.returncode} '
                               f'but printed some errors:\n{error_output}')

    def build(self, required_containers: Dict[VersionedName, str]):
        raise GeneralException('Not implemented - use different container type')

    def _save_configuration_for_build(self, config_path, root_name):
        root_node = ET.Element(root_name)
        for file_setting in self.configuration.settings_tree.values():
            file_setting.to_xml_node(root_node, create_groups=False)
        UserXmlParser.save_node_to_file(root_node, config_path, include_copyright=True)

