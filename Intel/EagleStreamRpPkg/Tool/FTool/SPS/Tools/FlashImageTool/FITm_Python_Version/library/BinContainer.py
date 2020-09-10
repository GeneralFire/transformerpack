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

from library.IContainer import IContainer


class BinContainer(IContainer):
    '''
        This class represent case when Container wants to return ready binary.
        In such case only output path should be specified
    '''

    def __init__(self, path):
        super().__init__()
        self.path = path

    def build(self, required_containers):
        pass

    @property
    def output_path(self):
        return self.path
