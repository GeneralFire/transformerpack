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

from app.components.Container import ContainerWrapper
from app.utils import reverse_string, Consts
from library.tool.ColorPrint import ColorPrint
from library.tool.Converter import Converter
from library.tool.ExpressionEngine import ExpressionEngine
from library.tool.components.BitFieldComponent import BitFieldComponent
from library.tool.exceptions import GeneralException
from library.tool.utils import validate_file
from library.utils import check_write_access


class BinaryDecomposer:

    def __init__(self, containers_list: list, save_path: str = 'decomp'):
        self.containers_list: list[ContainerWrapper] = containers_list
        self.descriptor_end_offset: int = 0x1000
        self.descriptor_start_offset: int = 0
        self.descriptor_region_name: str = 'descriptor'
        self.save_path: str = save_path

    @staticmethod
    def get_flreg_offsets(flreg: BitFieldComponent, bin_content: bytes):
        region_base = next((c for c in flreg.bit_fields if c.name == 'DescFlashRegionBase'), None)
        region_limit = next((c for c in flreg.bit_fields if c.name == 'DescFlashRegionLimit'), None)
        if not region_base or not region_limit:
            raise GeneralException(f"Could not find offset information in {flreg.region_name} in descriptor.")

        flreg_hex = bin_content[flreg.offset:flreg.offset + flreg.size]
        if not flreg_hex:
            raise GeneralException("Flregs are either empty or non-existent in passed binary file. The image is broken.")

        flreg_bin = Converter.hex_to_bin(reverse_string(flreg_hex) if flreg.byte_order == flreg.littleOrder else flreg_hex)
        flreg_bin = reverse_string(flreg_bin)
        return BinaryDecomposer._get_offsets(flreg_bin, region_base, region_limit)

    @staticmethod
    def _get_offsets(flreg_bin, region_base, region_limit):
        start_offset = int(reverse_string(flreg_bin[region_base.bit_low:region_base.bit_high + 1]), base=2)
        start_offset = ExpressionEngine.get_logical_shift(start_offset, region_base.value_formula)
        end_offset = int(reverse_string(flreg_bin[region_limit.bit_low:region_limit.bit_high + 1]), base=2)
        end_offset = ExpressionEngine.get_logical_shift(end_offset, region_limit.value_formula)
        end_offset = end_offset + 0x1000 if end_offset else end_offset
        return start_offset, end_offset

    def _save_region_to_file(self, region, bin_content, start_offset, end_offset, set_binary_path=True):
        region_name = getattr(region, 'region_name', None) or getattr(region, 'name', None)
        file_name = region_name + Consts.BIN_EXT
        file_path = os.path.join(self.save_path, file_name)
        check_write_access(file_path)

        with open(file_path, 'wb+') as file:
            region_content = bin_content[start_offset: end_offset]
            file.write(region_content)
            ColorPrint.debug(f"Region {region_name} saved to {file.name}")

        container_to_save = next(
            (c for c in self.containers_list if c.name == region_name), None)
        if not container_to_save:
            raise GeneralException(f"Could not find container matching flreg in layout")

        if set_binary_path:
            container_to_save.set_external_binary(file_path)
        container_to_save.enabled = True

    def binary_decomposition(self, input_path):
        validate_file(input_path)

        bin_content = open(input_path, 'rb').read()
        descriptor = next((c for c in self.containers_list if c.name == self.descriptor_region_name), None)

        # disable all containers, so that they could be enabled back when used during binary decomposition and saved to xml
        for c in self.containers_list:
            c.enabled = False

        if not os.path.exists(self.save_path):
            os.mkdir(self.save_path)

        if not descriptor:
            raise GeneralException("Could not decompose binary file - descriptor was not found.")
        self._save_region_to_file(descriptor, bin_content, self.descriptor_start_offset, self.descriptor_end_offset, set_binary_path=False)
        flregs = [c for c in descriptor.children if not c.region_name == self.descriptor_region_name]

        last_enabled_end_offset = 0

        for flreg in flregs:
            ColorPrint.debug(f"Decomposing {flreg.region_name} region")

            start_offset, end_offset = self.get_flreg_offsets(flreg, bin_content)

            if end_offset > len(bin_content):
                raise GeneralException(f"End offset of the {flreg.region_name} exceeds input binary file size")

            if end_offset:
                self._save_region_to_file(flreg, bin_content, start_offset, end_offset)

            if end_offset > last_enabled_end_offset:
                last_enabled_end_offset = end_offset

        if last_enabled_end_offset != len(bin_content):
            raise GeneralException(f"End offset of the last flreg is not equal to input binary file size")
        ColorPrint.success("Binary file successfully decomposed")
