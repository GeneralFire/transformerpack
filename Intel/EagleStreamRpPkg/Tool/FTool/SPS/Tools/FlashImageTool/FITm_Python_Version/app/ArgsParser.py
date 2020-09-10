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
import argparse
import os
import pprint
import re
import sys

from app.licensing import Licensing
from app.utils import get_extension, Consts, check_strings_for_illegal_characters
from library.AccessManager import AccessManager, WrongAccessException
from library.utils import set_fit_LibConfig_isAccessCheckSkipped, set_fit_LibConfig_isDirAclSet
from library.tool.ColorPrint import ColorPrint
from library.tool.LibConfig import LibConfig
from library.tool.exceptions import GeneralException, InvalidAttributeException
from library.tool.utils import check_file_path


# Line below suppress useless PyCharm
# noinspection PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit,PyAttributeOutsideInit


class ArgsParser:
    is_exe = getattr(sys, 'frozen', False)
    plugins_folder = 'plugins'
    layouts_folder = 'layouts'

    platform = ''
    port = 4243
    input_path = ''
    out_xml_path = ''
    is_gui = False
    is_build = False
    help = None
    param_overrides = None
    is_verbose = False
    license = None
    layout_overrides = None

    def __init__(self, app_name):
        self.plugins_path = os.path.join(LibConfig.coreDir, self.plugins_folder)
        self.in_layout_path = os.path.join(LibConfig.coreDir, self.layouts_folder)

        ColorPrint.setup_colorama()

        self._parser = argparse.ArgumentParser(app_name,
                                               epilog=f"Possible arguments combinations: \n {self.possible_args_combinations_formatted}",
                                               formatter_class=argparse.RawDescriptionHelpFormatter,
                                               allow_abbrev=False,
                                               add_help=False)
        self._add_arguments_to_parser()
        args = self._parser.parse_args()

        self.check_if_args_not_empty()
        self.validate_args_combination(args)
        self.check_args(args)
        self.check_args_for_duplicates()
        check_strings_for_illegal_characters([self.out_xml_path, self.input_path, self.in_layout_path, self.plugins_path])

    def check_if_args_not_empty(self):
        if not len(sys.argv) > 1:
            self.print_help()
            raise GeneralException("Running FITm without arguments has no effect. Please choose one of the possible "
                                   "combinations from the above list.")

    def check_args(self, args):
        if args.build is not None:
            self.is_build = args.build

        if args.set_dir_acl is not None:
            set_fit_LibConfig_isDirAclSet(args.set_dir_acl)

        accessManager = AccessManager(False)

        if args.skip_access_check is not None and args.skip_access_check:
            set_fit_LibConfig_isAccessCheckSkipped(args.skip_access_check)
            ColorPrint.warning("Warning: accepting relative plugins dir and skipping checking access rights for any directory.\n")

        if args.plugins_dir is not None:
            if not LibConfig.isAccessCheckSkipped and not os.path.isabs(args.plugins_dir):
                raise GeneralException(f"Plugins directory must be an absolute path, but given relative: '{args.plugins_dir}'")
            if not accessManager.check_single(args.plugins_dir):
                raise WrongAccessException(args.plugins_dir)
            self.plugins_path = args.plugins_dir

        if args.layout_xml is not None:
            if not accessManager.check_single(args.layout_xml):
                raise WrongAccessException(args.layout_xml)
            self.in_layout_path = args.layout_xml

        if args.input is not None:
            if get_extension(args.input) != Consts.XML_EXT and args.build:
                raise GeneralException("Only xml input files are accepted when building an image.")
            self.input_path = args.input
            if get_extension(args.input) == Consts.BIN_EXT:
                self._validate_binary_file()

        if args.save_xml is not None:
            ext = get_extension(args.save_xml)
            if not ext:
                name_with_ext = f"{args.save_xml}{Consts.XML_EXT}"
                ColorPrint.warning(f"Warning: configuration file will be saved with {Consts.XML_EXT} extension\n{args.save_xml} --> {name_with_ext}")
                args.save_xml = name_with_ext
            elif ext != Consts.XML_EXT:
                raise GeneralException(f"Wrong extension '{ext}' for configuration file: {args.save_xml}\nOnly xml format is allowed.")
            self.out_xml_path = args.save_xml

        if args.gui is not None:
            self.is_gui = args.gui
        ColorPrint.is_gui = self.is_gui

        if args.port is not None:
            if not args.port.isdigit() or not 1024 <= int(args.port) <= 65535:
                raise GeneralException('Port must be number between 1024 and 65535.')
            self.port = int(args.port)

        if args.help is not None:
            self.help = args.help

        if args.license is not None:
            self.license = args.license

        if args.params is not None:
            try:
                self.param_overrides = self._reformat_override_params(args.params)
            except InvalidAttributeException:
                raise InvalidAttributeException('Incorrect format for settings override. Please use the following form:\n' +
                                   '  <container>:<setting>=<value> <another>=<value>')

        if args.verbose is not None:
            self.is_verbose = args.verbose

        if args.layout is not None:
            self.layout_overrides = self._reformat_layout_overrides(args.layout)
            if self.is_build:
                for override in self.layout_overrides:
                    check_file_path(self.layout_overrides[override], False, True)

    def _validate_binary_file(self):
        spi_descriptor = b'\x0F\xF0\xA5\x5A'
        with open(self.input_path, 'rb') as f:
            s = f.read()
            if not s:
                raise GeneralException("Binary file is empty")
            if -1 == s.find(spi_descriptor):
                raise GeneralException("There is no SPI descriptor in the binary file. The image is broken.")

    @property
    def possible_args_combinations(self):
        return {
            'build': ['input', f'{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}', 'plugins_dir', 'layout_xml', 'save_xml', 'params', 'layout', 'verbose'],
            'save_xml': ['input', f'{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}', 'plugins_dir', 'layout_xml', 'params', 'layout', 'verbose'],
            'input': ['verbose', f'{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}', 'save_xml', 'layout'],
            'license': [],
            'gui': ['port', f'{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}'],
            'help': [],
            f'{Consts.SET_DIR_ACL_ARG}': []
        }

    @property
    def possible_args_combinations_formatted(self):
        excluded_help_items = ['gui']
        possibilities = {k: self.possible_args_combinations[k] for k in self.possible_args_combinations if k not in excluded_help_items}
        possibilities.update({'--' + k: ['--' + arg for arg in v] for k, v in possibilities.items()})
        for key in list(possibilities):
            possibilities.pop(key) if '--' not in key else ''
        return pprint.pformat(possibilities)

    def validate_args_combination(self, args):
        possible_combinations = self.possible_args_combinations
        arg_names = [k for k, v in vars(args).items() if v or v == '']
        seq_matched = False
        for key, args_list in possible_combinations.items():
            invalid_args = [c for c in arg_names if c not in args_list + [key]]
            if not invalid_args and key in arg_names:
                seq_matched = True
                break
        if not seq_matched:
            self.print_help()
            raise GeneralException("Did not find matching argument combination.")

    def check_args_for_duplicates(self):
        actions = [a.option_strings for a in self._parser._actions]
        arg_strings = [arg for arg in sys.argv[1:] if arg[0] == '-']
        for arg in arg_strings:
            a = next((action for action in actions if arg in action), None)
            if not a:
                raise GeneralException(f"Argument {arg} was duplicated.")
            actions.remove(a)

    def _add_arguments_to_parser(self):
        self._parser.add_argument('-b', '--build', action='store_true', help='Triggers building a binary image')
        self._parser.add_argument('-d', '--plugins_dir', help='Specifies plugins directory')
        self._parser.add_argument('-l', '--layout_xml', help='Specifies kind of platform layout to be chosen')
        self._parser.add_argument('-i', '--input', help='Specifies path for the user configuration xml or a binary file to decompose')
        self._parser.add_argument('-s', '--save_xml', help='Specifies path where to save the configuration xml')
        self._parser.add_argument('--gui', action='store_true', help=argparse.SUPPRESS)
        self._parser.add_argument('--port', help=argparse.SUPPRESS)
        self._parser.add_argument('-h', '--help', help='Shows this help or overridable setting list for the specified container', nargs='?', const='')
        self._parser.add_argument('--license', help='Shows license agreement', nargs='?', const='')
        self._parser.add_argument('--params', help='Specifies settings to be overridden. Sample definition: container:Setting=0x1 FilePath=input.bin', nargs='+')
        self._parser.add_argument('--verbose', action='store_true', help='Display verbose messages')
        self._parser.add_argument('--layout', help='Override region. Sample definition: bios=bios.bin', nargs='+')
        self._parser.add_argument(f'--{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}', action='store_true', help='Allows user to provide plugins directory as a relative path and omit directories access rights check')
        self._parser.add_argument(f'--{Consts.SET_DIR_ACL_ARG}', action='store_true', help='Sets required ACL for each item in root directory')

    @staticmethod
    def _reformat_override_params(params):
        sett_per_container = {}
        colon_pattern = re.compile(r':(?![\\\/])')
        equal_char = '='
        cont_name = LibConfig.enableRegions
        for param in params:
            matched = re.findall(colon_pattern, param)
            if matched:
                parts = list(filter(None, colon_pattern.split(param)))
                if len(parts) != 2 or len(matched) != 1:
                    raise GeneralException(f'Container specification incorrect. [{param}]')

                cont_name = parts[0]
                sett_ovr = parts[1]
            else:
                sett_ovr = param

            iterable_match = re.match(Consts.ITERABLE_CLI_REGEX, sett_ovr)
            if iterable_match:
                sett_ovr = f'{iterable_match.group(1)}/entry{iterable_match.group(2)}/{iterable_match.group(3)}'

            if param.count(equal_char) != 1:
                raise GeneralException(f'Setting override missing assignment? [{param}]')

            if cont_name in sett_per_container:
                sett_per_container[cont_name].append(sett_ovr)
            else:
                sett_per_container[cont_name] = [sett_ovr]

        return sett_per_container

    @staticmethod
    def _reformat_layout_overrides(params):
        region_paths = {}
        equal_pattern = re.compile(r'=')
        for param in params:
            matched = re.findall(equal_pattern, param)
            if matched:
                parts = list(filter(None, equal_pattern.split(param)))
                if len(parts) == 1 and len(matched) == 1 and param.endswith('='):
                    raise GeneralException(f'Binary not specified. [{param}]')
                if len(parts) != 2 or len(matched) != 1:
                    raise GeneralException(f'Container specification incorrect. [{param}]')
                cont_name = parts[0]
                region_paths[cont_name] = parts[1]
            else:
                raise GeneralException(f'Container name or value missing? [{param}]')
        return region_paths

    def print_help(self):
        self._parser.print_help()

    @staticmethod
    def print_licenses():
        for name, license_txt in Licensing.get_licenses().items():
            print('\n' + name + '\n')
            print(license_txt + '\n')
