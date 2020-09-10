#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
INTEL CONFIDENTIAL
Copyright 2019-2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""
from library.tool.MapGenerator import IMapFormatter


class FitMapFormatter(IMapFormatter):
    intentSize = 4
    breakSize = 7

    def __init__(self):
        super().__init__()
        self._entries = ''

    def _create_header(self):
        return "Start (hex)    End (hex)    Length (hex)    Area Name\n" \
               "-----------    ---------    ------------    ---------\n"

    def _create_entries(self, map_root):
        self._entries = ''
        for single in sorted(map_root.map_data, key=lambda x: x[0]):
            self._create_entry(single)
        return self._entries

    def _create_entry(self, entry):
        offset, size, intent, map_name = entry
        self._entries += self._column(offset) + \
                         self._column(offset + size - 1) + \
                         self._column(size) + \
                         self._name_column(map_name, intent) + '\n'
        # TODO process required containers, now only containers directly included in layout are processed

    def _column(self, value):
        return "%08X" % value + self.breakSize * ' '

    def _name_column(self, name, intent):
        return intent * ' ' + name
