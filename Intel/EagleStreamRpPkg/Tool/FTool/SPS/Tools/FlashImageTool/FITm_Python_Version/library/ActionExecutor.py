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

from library.tool.exceptions import ActionException


class ActionExecutor:

    name = 'ActionExecutor'

    def __init__(self):
        super().__init__()
        self._post_build_actions = {}
        self._pre_build_actions = {}

    def register_pre_build_action(self, name: str, func):
        if name in self._pre_build_actions:
            raise ActionException(f"Defined action name [{name}] in already registered for this plugin [{self.name}]")
        self._pre_build_actions[name] = func

    def register_post_build_action(self, name: str, func):
        if name in self._post_build_actions:
            raise ActionException(f"Defined action name [{name}] in already registered for this plugin [{self.name}]")
        self._post_build_actions[name] = func

    def execute_pre_build_action(self, name: str, *args):
        if name in self._pre_build_actions:
            return self._pre_build_actions[name](*args)
        raise ActionException(f"Specified action {name} wasn't found in {self.name}")

    def execute_post_build_action(self, name: str, *args):
        if name in self._post_build_actions:
            return self._post_build_actions[name](*args)
        raise ActionException(f"Specified action {name} wasn't found in {self.name}")