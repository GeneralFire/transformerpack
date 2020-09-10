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
import sys

from app.ArgsParser import ArgsParser
from app.BinaryDecomposer import BinaryDecomposer
from app.control.FitEngine import FitEngine
from app.utils import get_extension, Consts
from library.tool.ColorPrint import ColorPrint
from library.tool.exceptions import GeneralException
from library.tool.utils import print_header, is_python_ver_satisfying
from library.utils import check_dir, real_path_dir_for, set_fit_LibConfig, set_fit_LibConfig_coreDir
from library.version import FIT_VERSION
from library.tool.LibConfig import LibConfig
from library.AccessManager import AccessManager


def check_python_ver():
    if not is_python_ver_satisfying(required_python=(3, 6)):
        sys.exit(1)


def parse_args():
    name = 'Intel (R) Flash Image Tool'
    print_header(name=name, version=FIT_VERSION)
    check_python_ver()
    args = ArgsParser(name)
    check_dir(args.plugins_path)
    return args


def main():
    is_exe = getattr(sys, 'frozen', False)
    app_file = sys.executable if is_exe else __file__
    run_from = os.getcwd()

    core_dir = real_path_dir_for(app_file)
    os.chdir(core_dir)
    set_fit_LibConfig_coreDir(core_dir)
    args = parse_args()

    accessManager = AccessManager(True)
    if LibConfig.isDirAclSet:
        accessManager.set_dir_acl(core_dir)
        return

    if not accessManager.check_all_nested(core_dir):
        return

    is_decompose = get_extension(args.input_path) == Consts.BIN_EXT
    set_fit_LibConfig(app_file, run_from, is_decompose, args.is_verbose)

    engine = FitEngine(args.plugins_path, args.in_layout_path, args.is_gui, bool(args.help))

    if args.is_gui:
        try:
            from gui_server.gui_api import gui_api
        except ImportError:
            raise GeneralException("Could not load GUI API. This version of FITm does not support GUI.")
        try:
            gui_api(engine, args)
        except ImportError as ie:
            raise GeneralException(f"FITm requirements for python modules are not met. {str(ie)}."
                                   f" Please review FITm user guide and install all requirements.")

    if args.help is not None:
        if args.help != '':
            engine.print_container_help(args.help)
        else:
            args.print_help()
        return

    if args.license is not None:
        args.print_licenses()
        return

    if not args.is_gui and get_extension(args.input_path) == Consts.XML_EXT:
        engine.load_user_xml(args.input_path)

    engine.init(args.param_overrides, args.layout_overrides)
    engine.check_containers_verions()

    if is_decompose:
        run_decomposition(engine, args)

    if not args.is_gui:
        if args.is_build:
            engine.build_image()
        if args.out_xml_path and not is_decompose:
            engine.save_user_xml(args.out_xml_path)


def run_decomposition(engine, args):
    containers = engine.layout_engine.containers_list
    decomposer = BinaryDecomposer(containers)
    out_xml_path = 'decomp_config.xml' if not args.out_xml_path else args.out_xml_path

    decomposer.binary_decomposition(args.input_path)
    engine.save_user_xml(out_xml_path)


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        ColorPrint.error("Error occurred: \n" + str(e))
        LibConfig.exitCode = 1
    sys.exit(LibConfig.exitCode)
