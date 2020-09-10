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

from library.IFitPlugin import IFitPlugin
from typing import Dict
import os

from library.tool.LibException import LibException
from library.ExecutableContainer import ExecutableContainer
from library.tool.UniqueKey import VersionedName


class MfsContainer(ExecutableContainer):
    """
    Container that calls an executable to build mfs binary and get list of mfs settings.
    """

    def __init__(self, executable, xml_name='configuration.xml', exe_config_name='config.xml', main_dir='', **kwargs):
        super().__init__(executable, xml_name, exe_config_name, main_dir, **kwargs)

    def _create_generator(self):
        self._create_configuration()
        return super()._create_generator()

    def _create_configuration(self):
        arguments = ['--config_out', self.xml_path]
        arguments.extend(self._get_platform_arguments())
        self._call_executable(arguments)

    def build(self, required_containers: Dict[VersionedName, str]):
        self._save_configuration_for_build(self.exe_config_path, 'mfs')
        me_input = self.dependent_settings.get('me_input')
        if me_input is None:
            raise LibException('Missing path to setting: "me_input"')
        if not me_input.value:
            raise LibException('Path to ME Region not specified')
        me_path = os.path.join(os.getcwd(), me_input.value)
        arguments = ['--config_in', self.exe_config_path,
                     '--me_in', me_path,
                     '--mfs_out', self.output_path,
                     '--mfs_compressed_out', self.output_path + '.compressed',
                     '--mfs_compressed_size', '0x1FFE0',
                     '--mfsb_out', self.output_path + '.mfsb']
        arguments.extend(self._get_platform_arguments())
        self._call_executable(arguments)

    @staticmethod
    def _get_platform_arguments():
        return ['--pch', 'EBG',
                '--platform', 'Archer City',
                '--file_system', 'Eaglestream']


class FileSystemPlugin(IFitPlugin):
    pluginPath = IFitPlugin.plugins_dir(__file__)
    name = "ebg_mfs"
    containers = [(MfsContainer, {"executable": "FileSystemPlugin",
                                  "xml_name": "mfs.xml",
                                  "main_dir": pluginPath,
                                  'timeout': 60,
                                  'dependent_settings':
                                       {'me_input': 'me/input_file'}})]

