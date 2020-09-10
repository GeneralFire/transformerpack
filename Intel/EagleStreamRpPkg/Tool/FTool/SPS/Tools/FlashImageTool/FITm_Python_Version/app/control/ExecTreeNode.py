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
from library.IContainer import BuildPhaseEnum
from library.tool.exceptions import MissingAttributeException, DependencyException
from library.tool.UniqueKey import UniqueKey
from app.components.Container import DependContainerWrapper, ContainerWrapper


class ExecTreeNode:
    """A single node in dependency tree"""

    comp_name = "Resolving containers' dependency"
    pathSeparator = " -> "

    def __init__(self, parent, container_wrapper: ContainerWrapper):
        self.parent = parent
        self.container_wrapper = container_wrapper
        if self.parent is None:  # root node is implicitly built at the last stage
            self.build_phase = BuildPhaseEnum(len(BuildPhaseEnum)-1)
        else:
            self.build_phase = container_wrapper.build_phase
        self.name = ''
        if container_wrapper:
            if container_wrapper.is_external_bin:
                self.name = f"{container_wrapper.name} binary: {container_wrapper.binary_path}"
            else:
                self.name = container_wrapper.global_unique_key
        self.children = []

    def semideepcopy(self):
        """
        Makes an independent copy of the tree structure but avoids copying the container wrappers
        :return:
        """
        from copy import copy
        copied = copy(self)
        copied.children = []

        if self.children is not None:
            for child in self.children:
                child_copy = child.semideepcopy()
                child_copy.parent = copied
                copied.children.append(child_copy)

        return copied

    def resolve_children(self, containers_manager, layout_containers):
        if self.container_wrapper.is_binary_ready():
            return
        for dependency_unq_key in self.container_wrapper.dependencies:
            layout_containers_names = [container.name for container in layout_containers]
            if dependency_unq_key.cont_key.name in layout_containers_names:
                continue
            self.check_existence(dependency_unq_key, containers_manager, self.container_wrapper)
            self.check_cycles(dependency_unq_key, [dependency_unq_key])
            glb_unq_key, cont_ref = containers_manager.get_container_with_key(dependency_unq_key)
            child_container = DependContainerWrapper(unique_key=glb_unq_key, container_ref=cont_ref)
            child = ExecTreeNode(self, child_container)
            child.resolve_children(containers_manager, layout_containers)
            self.children.append(child)

    def pop_leaves(self, all_nodes: list, leaves_list: list, cur_build_phase: BuildPhaseEnum):
        if not self.is_container_enabled():
            self.add_children_to_all_nodes(all_nodes)
            self.parent.remove_child(self)
            # if container is disabled we cut entire branch
            return
        if self.can_be_popped(leaves_list, cur_build_phase):
            if not self.is_already_included_in_leaves(leaves_list):
                leaves_list.append(self.container_wrapper)
            if not self.is_already_included_in_leaves(all_nodes):
                all_nodes.append(self.container_wrapper)
            self.parent.remove_child(self)
        else:
            children_full_list = list(self.children)
            for child in children_full_list:
                child.pop_leaves(all_nodes, leaves_list, cur_build_phase)

    def add_children_to_all_nodes(self, all_nodes: list):
        if not self.is_already_included_in_leaves(all_nodes):
            all_nodes.append(self.container_wrapper)
        children_full_list = list(self.children)
        for child in children_full_list:
            child.add_children_to_all_nodes(all_nodes)

    def can_be_popped(self, leaves_list: list, cur_build_phase: BuildPhaseEnum):
        return self.build_phase == cur_build_phase and \
               self.is_leaf() and \
               not self.is_from_same_plugin(leaves_list) and \
               not self.is_root() or self.is_already_included_in_leaves(leaves_list)

    def is_already_included_in_leaves(self, leaves_list):
        return self.name in [x.unique_key for x in leaves_list]

    def is_container_enabled(self) -> bool:
        if self.container_wrapper:
            return self.container_wrapper.enabled
        return True

    def remove_child(self, child):
        self.children.remove(child)

    def is_from_same_plugin(self, leaves: list):
        if not isinstance(self.name, UniqueKey):
            return False
        return any(self.name.plug_key == cont.plugin_unique_key for cont in leaves)

    def is_root(self):
        return self.parent is None

    def is_leaf(self):
        return len(self.children) == 0 or self.container_wrapper and self.container_wrapper.is_external_bin

    def check_existence(self, cont_key: UniqueKey, conts_manager, container):
        is_present = conts_manager.is_container_present(cont_key)
        if container.is_required and not is_present:
            path = self.dependency_path_str(str(cont_key))
            raise MissingAttributeException(f"Could not find container: {container.name}.\n Dependency path: {path} ",
                                            self.comp_name)
        elif not is_present:
            return False
        else:
            return True

    def dependency_path_str(self, path):
        path = str(self.name) + self.pathSeparator + path
        if self.parent:
            path = self.parent.dependency_path_str(path)
        return path

    def check_cycles(self, key_to_check, stack_logs):
        stack_logs.append(self.name)
        if key_to_check == self.name:
            raise DependencyException("Cyclic dependencies containers found: {}".format(' -> '.join(stack_logs)),
                                      self.comp_name)
        if not self.is_root():
            self.parent.check_cycles(key_to_check, stack_logs)

    def clear(self):  # pragma: no cover
        self.container_wrapper.clear()

    def get_all_descendants(self, desc_list=None):
        if desc_list is None:
            desc_list = []
        if self.children:
            desc_list.extend(self.children)
            for child in self.children:
                child.get_all_descendants(desc_list)
        return desc_list
