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

from app.utils import Consts
from functools import wraps
from .tool.LibConfig import LibConfig
from .tool.components.IComponent import IComponent
from .tool.exceptions import InvalidAttributeException, GeneralException


def set_fit_LibConfig_coreDir(path):
    LibConfig.coreDir = path


def set_fit_LibConfig_isAccessCheckSkipped(skip):
    LibConfig.isAccessCheckSkipped = skip


def set_fit_LibConfig_isDirAclSet(set):
    LibConfig.isDirAclSet = set


def set_fit_LibConfig(app_file, run_from, is_decompose: bool = False, is_verbose: bool = False):
    LibConfig.runFrom = run_from
    LibConfig.settingsTag = 'configuration'
    LibConfig.overridesTag = 'container_overrides'
    LibConfig.defaultPaddingValue = IComponent.AlignByte.ByteFF
    LibConfig.rootTag = 'container'
    LibConfig.schemaPath = os.path.join(os.path.dirname(app_file), 'schema.xsd')
    LibConfig.isVerbose = is_verbose
    LibConfig.isDecompose = is_decompose
    LibConfig.maxBufferSize = 64 * 1024 * 1024


def check_dir(directory):
    if not os.path.isdir(directory):
        raise InvalidAttributeException("Given directory does not exist: {}".format(directory))


def real_path_dir_for(filename: str):
    return os.path.dirname(os.path.realpath(filename))


def join_real_path():
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            folder = LibConfig.coreDir
            return os.path.join(folder, func(*args, **kwargs))

        return wrapper

    return decorator


def get_relative_path(path):
    path = os.path.join(LibConfig.runFrom, path)
    return os.path.relpath(path, LibConfig.coreDir)


def check_write_access(path):
    if os.path.exists(path) and not os.access(path, os.W_OK):
        raise GeneralException(f"Could not save to {path}. No write access")


def is_proper_output_path(path):
    '''
        Checks that given output path of Container points to Int directory.
        Can be omitted with FIT argument '--{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}'
    '''

    if LibConfig.isAccessCheckSkipped:
        return True

    relative_path = os.path.relpath(os.path.realpath(path), os.path.join(LibConfig.coreDir, Consts.INTERMEDIATE_DIRECTORY))
    return not relative_path.startswith('..')