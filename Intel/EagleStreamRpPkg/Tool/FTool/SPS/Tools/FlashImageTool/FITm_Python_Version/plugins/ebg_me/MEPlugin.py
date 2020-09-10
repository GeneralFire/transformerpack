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
from library.XmlContainer import XmlContainer


class MeContainer(XmlContainer):

    def build(self, required_containers):
        self.update_fw_version_in_bpdt()
        super().build(required_containers)

    def update_fw_version_in_bpdt(self):
        me_bin = self.generator.configuration_root.get_child('input_file').data
        manifest_start = me_bin.find(b'\x24\x4D\x4E\x32')  # search for '$MN2'
        major = int.from_bytes(me_bin[manifest_start + 8: manifest_start + 8 + 2], 'little')
        minor = int.from_bytes(me_bin[manifest_start + 10: manifest_start + 10 + 2], 'little')
        hotfix = int.from_bytes(me_bin[manifest_start + 12: manifest_start + 12 + 2], 'little')
        build = int.from_bytes(me_bin[manifest_start + 14: manifest_start + 14 + 2], 'little')

        bpdt_major = self.generator.configuration_root.get_child('bpdtMajor')
        bpdt_minor = self.generator.configuration_root.get_child('bpdtMinor')
        bpdt_hotfix = self.generator.configuration_root.get_child('bpdtHotfix')
        bpdt_build = self.generator.configuration_root.get_child('bpdtBuild')

        bpdt_major.value = major
        bpdt_major.validate()
        bpdt_minor.value = minor
        bpdt_minor.validate()
        bpdt_hotfix.value = hotfix
        bpdt_hotfix.validate()
        bpdt_build.value = build
        bpdt_build.validate()


class MEPlugin(IFitPlugin):
    pluginPath = IFitPlugin.plugins_dir(__file__)
    name = "me_ebg"
    containers = [(MeContainer, {"xml_name": "me.xml", "main_dir": pluginPath})]
