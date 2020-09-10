#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
INTEL CONFIDENTIAL
Copyright 2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""

from library.tool.components.IComponent import IComponent
from library.version import FIT_VERSION


class CoreDataComponentChild(IComponent):

    def __init__(self, **kwargs):
        self.name = kwargs.get("name")
        self.parse_string_value(kwargs.get("value", None))
        self.parent = kwargs.get("parent", None)
        self.children_by_name = {}
        self.children = []
        self._set_parent_child()


class CoreDataComponent(IComponent):
    CoreDataComponentTag = 'core'

    def __init__(self):
        self.name = self.CoreDataComponentTag
        self.children_by_name = {}
        self.children = []
        props = dict(zip(['fitm_version_major', 'fitm_version_minor', 'fitm_version_build'], FIT_VERSION.split('.')))
        for key, val in props.items():
            CoreDataComponentChild(name=key, value=val, parent=self)
