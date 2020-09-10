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
import re
import xml.etree.ElementTree as Et

from library.tool.UniqueKey import UniqueKey
from library.tool.ColorPrint import ColorPrint
from library.tool.exceptions import GeneralException


def print_tree(current_node, indent="", last='updown'):
    if isinstance(current_node.name, UniqueKey):
        node_name = current_node.name.to_format_str()
    else:
        node_name = current_node.name
    enabled_children = [child for child in current_node.children if child.container_wrapper.enabled is True]
    children_sum_func = lambda node: sum(children_sum_func(child) for child in node.children) + 1
    size_branch = {child: children_sum_func(child) for child in enabled_children}

    """ Creation of balanced lists for "up" branch and "down" branch. """
    up = list(enabled_children)
    down = []
    while up and sum(size_branch[node] for node in down) < sum(size_branch[node] for node in up):
        down.insert(0, up.pop())

    """ Printing of "up" branch. """
    for child in up:
        next_last = 'up' if up.index(child) == 0 else ''
        next_indent = '{0}{1}{2}'.format(indent, ' ' if 'up' in last else '│', " " * len(node_name))
        print_tree(child, indent=next_indent, last=next_last)

    """ Printing of current node. """
    if last == 'up':
        start_shape = '┌'
    elif last == 'down':
        start_shape = '└'
    elif last == 'updown':
        start_shape = ' '
    else:
        start_shape = '├'

    if up:
        end_shape = '┤'
    elif down:
        end_shape = '┐'
    else:
        end_shape = ''

    ColorPrint.debug(str('{0}{1}{2}{3}'.format(indent, start_shape, node_name, end_shape)
                         .encode('utf-8')
                         .decode('utf-8')))  # fix for incorrect encoding in .exe

    """ Printing of "down" branch. """
    for child in down:
        next_last = 'down' if down.index(child) is len(down) - 1 else ''
        next_indent = '{0}{1}{2}'.format(indent, ' ' if 'down' in last else '│', " " * len(node_name))
        print_tree(child, indent=next_indent, last=next_last)


class LayoutXml:

    @staticmethod
    def get_platform_attrib_from_layout(input_layout, attrib='layout_name'):
        try:
            xml_tree = Et.ElementTree(file=input_layout)
            root = xml_tree.getroot()
            layout_attrib = root.attrib[attrib]
        except(KeyError, Et.ParseError) as e:
            ColorPrint.warning(f'{input_layout} - incorrect layout xml file. There is no parameter called {attrib}')
            ColorPrint.debug(e)
            return False
        return layout_attrib

    @classmethod
    def check_if_file_is_layout(cls, file_path):
        if os.path.isfile(file_path) and file_path.endswith('.xml') and cls.get_platform_attrib_from_layout(file_path):
            return True
        return False


def get_extension(file_path):
    return os.path.splitext(file_path)[1]


def reverse_string(string):
    return string[::-1]


class Consts:
    BIN_EXT = '.bin'
    XML_EXT = '.xml'
    ILLEGAL_CHARACTERS = '[@!#$%^&*()<>?|}{~:]'
    LAYOUT_CONTAINER = 'layout'
    INTERMEDIATE_DIRECTORY = 'Int'
    ITERABLE_CLI_REGEX = r'^(.*)\[(\d+)\]\.(.*=.*)$'
    ITERABLE_SETTING_REGEX = r'^(.*?)\[(\d+)\]\.(.*?)$'
    SET_DIR_ACL_ARG = 'set_dir_acl'
    SKIP_ACCESS_RIGHTS_CHECK_ARG = 'skip_access_check'


def check_strings_for_illegal_characters(string_or_list):
    regex = re.compile(Consts.ILLEGAL_CHARACTERS)
    if type(string_or_list) == str:
        string_or_list = [string_or_list]

    for path in string_or_list:
        if regex.search(os.path.basename(path)):
            raise GeneralException(f'Used illegal character in passed path: {path}')


