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
import codecs
import sys
from typing import List

from app.components.Container import ContainerWrapper
from app.control.ExecTreeNode import ExecTreeNode
from app.control.LayoutEngine import LayoutEngine
from app.utils import print_tree
from library.IContainer import BuildPhaseEnum
from library.tool.ColorPrint import ColorPrint
from library.tool.UniqueKey import UniqueKey
from library.tool.exceptions import DependencyException


class ExecTree:
    '''
    Responsible for creating the execution tree:
    1. Creating Tree using info about required containers from each container
    2. Creating List based queue (List of lists) each list in queue has independent containers and can
        be processed separately
        - it is done by popping leaves from the tree in while loop unless it is root
    '''

    def __init__(self):
        self.root = None
        # build_phase/dependency_stage/container
        self.exec_order: List[List[List[ContainerWrapper]]] = []
        self.all_nodes: List[ContainerWrapper] = []
        self.cur_build_phase = None

    def create_tree(self, engine: LayoutEngine, cont_manager):
        # Tree of containers considering theirs dependencies should be created
        self.root = ExecTreeNode(None, None)
        self.root.name = engine.name
        layout_containers = engine.containers_list
        for layout_cont in layout_containers:
            is_bin_ready = layout_cont.is_binary_ready()
            is_in_plugin = self.root.check_existence(layout_cont.global_unique_key, cont_manager, layout_cont)
            if is_in_plugin or is_bin_ready:
                self._create_node(cont_manager, layout_cont, layout_containers)

    def _create_node(self, cont_manager, layout_cont, layout_containers):
        layout_cont.is_present_in_layout = layout_cont.enabled
        node = ExecTreeNode(self.root, layout_cont)
        self.root.children.append(node)
        node.resolve_children(cont_manager, layout_containers)

    def create_ordered_queue(self):
        self.exec_order.clear()
        leaves = []
        nodes = self.root.semideepcopy()

        for build_phase in BuildPhaseEnum:
            self.exec_order.append([])
            self.cur_build_phase = build_phase
            self._add_dependency_level(nodes, leaves)  # do - while emulation
            while len(leaves) > 0:
                self._add_dependency_level(nodes, leaves)
        if len(nodes.children):
            ColorPrint.debug("Unable to build following part of the tree:")
            print_tree(nodes)
            raise DependencyException("Main phase container depends on a post phase container what is forbidden")
        self.cur_build_phase = None

    def _add_dependency_level(self, nodes, leaves: list):
        leaves.clear()
        nodes.pop_leaves(self.all_nodes, leaves, self.cur_build_phase)
        self.exec_order[self.cur_build_phase.value].append(list(leaves))

    def depends_generator(self, phase: BuildPhaseEnum):  # With above tree we should request build binary from each plugin
        for depends in self.exec_order[phase.value]:
            yield depends

    def clear(self, new_layout=True):
        for node in self.all_nodes:
            node.clear(new_layout=new_layout)
        self.exec_order.clear()
        self.all_nodes.clear()

    def print_tree(self):
        ColorPrint.debug('Image containers dependency tree:')
        previous_import_state = ColorPrint.can_import_colorama
        previous_stdout = sys.stdout

        try:
            if sys.stdout.encoding != 'UTF-8':
                ColorPrint.can_import_colorama = False
                sys.stdout = codecs.getwriter('utf-8')(sys.stdout.buffer, 'strict')
            print_tree(self.root)

        except (UnicodeDecodeError, UnicodeEncodeError) as e:
            ColorPrint.debug(f"An error occurred during printing tree: \n{e}")

        finally:
            ColorPrint.can_import_colorama = previous_import_state
            sys.stdout = previous_stdout

    def get_exec_tree_node(self, name: UniqueKey):
        """
        Searched the top level exec tree entries to find the proper exec tree node
        :param name: Unique key for the searched container
        :return: Exec tree node which contains the specified container
        """
        all = self.root.get_all_descendants()
        return next((node for node in all if node.name == name), None)

    def _get_dependent_containers_recursive(self, node: ExecTreeNode):
        dependent = [child.container_wrapper for child in node.children]
        for child in node.children:
            child_dep = self.get_dependent_containers(child.name)
            if child_dep is not None:
                dependent.extend(child_dep)

        return dependent

    def get_dependent_containers(self, name: UniqueKey):
        node = self.get_exec_tree_node(name)
        if node is None:
            return None

        raw = self._get_dependent_containers_recursive(node)
        return sorted(raw, key=lambda cont: cont.unique_key.cont_key.name)

    def has_dependent_containers(self, name: UniqueKey):
        res = self.get_dependent_containers(name)
        return res is not None and len(res) > 0
