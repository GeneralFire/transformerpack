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
import mimetypes

from library.tool.ColorPrint import ColorPrint


class Licensing:
    license_dir = 'doc/'
    license_sufix = '_LICENSE.txt'
    max_size = 262144

    @staticmethod
    def get_licenses():
        licenses = {}
        if os.path.exists(Licensing.license_dir):
            # sorting for cryptography licenses do display in the right order
            for license_name in sorted([(Licensing._remove_sufix(l).casefold(), Licensing._remove_sufix(l)) for l in
                                        os.listdir(Licensing.license_dir) if l.endswith(Licensing.license_sufix)]):
                Licensing._load_license(licenses, license_name[1])
        return licenses

    @staticmethod
    def _load_license(licenses: dict, license_name: str, path: str = None):
        if not path:
            path = Licensing._compose_path(license_name)
        stat = os.stat(path)
        if stat.st_size > Licensing.max_size:
            ColorPrint.warning(f'Skipping {license_name}{Licensing.license_sufix}. File too big.')
            return
        mime = mimetypes.guess_type(path)
        if mime[0] != 'text/plain':
            ColorPrint.warning(f'Skipping {license_name}{Licensing.license_sufix}. Must be text file.')
            return
        try:
            with open(path, 'r', encoding='utf-8') as license_file:
                licenses[license_name] = license_file.read()
        except UnicodeDecodeError:
            ColorPrint.warning(f'Skipping {license_name}{Licensing.license_sufix}. Incorrect encoding.')

    @staticmethod
    def _remove_sufix(name):
        return name[:-len(Licensing.license_sufix)]

    @staticmethod
    def _compose_path(license_name):
        return os.path.join(Licensing.license_dir, license_name + Licensing.license_sufix)
