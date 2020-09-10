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


class LayoutComponent(IComponent):
    """
     LayoutComponent - represent component in xml <layout></layout>
     Node "layout" in layout.xml is overridden,
     due to handle many different cases of invocation to containers such as:
      -by name
      -by VersionedName
      -by UniqueName
    """
    nodeName = "layout"

    def get_child(self, child_name):
        for key_child, child in self.children_by_name.items():
            if child_name == str(key_child):
                return child
        keys_str = [str(key) for key in self.children_by_name.keys()]
        raise ComponentException("No '%s' child. Choose one of: %s" % (child_name, ', '.join(keys_str)),
                                 self.name)

    @property
    def map_data(self) -> ([int, int, int, str]):
        """Returns list of start offset, length, intent, area name"""
        self.set_map_names_for_layout()
        return super().map_data
