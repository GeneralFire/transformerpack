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

from app.control.StorageByKey import StorageByName
from library.tool.UniqueKey import VersionedName, UniqueKey
from app.components.Container import ContainerWrapper
from itertools import chain
from library.tool.exceptions import MissingAttributeException

from typing import Dict


class WrappersManager:
    '''
        The WrappersManager is responsible for storing all containers' wrappers, that are present in current layout.
    '''

    def __init__(self):
        self._containers_by_keys = StorageByName(storage_name=StorageByName.Name.Containers,
                                                 items_name=StorageByName.Name.Plugins)  # sorted by plug_key / cont_key
        self.all_containers: Dict[UniqueKey, ContainerWrapper] = {}
        self._layout_name = None

    @property
    def layout_name(self):
        return self._layout_name

    @layout_name.setter
    def layout_name(self, new_layout_name):
        self._layout_name = new_layout_name

    def add_wrapper(self, wrapper: ContainerWrapper):
        self.all_containers[wrapper.unique_key] = wrapper
        self._add_sorted_wrapper(wrapper)

    def _add_sorted_wrapper(self, wrapper: ContainerWrapper):
        if self._containers_by_keys.is_present(wrapper.plugin_unique_key):
            self._containers_by_keys[wrapper.plugin_unique_key, self.layout_name][wrapper.container_unique_key] = wrapper
        else:
            sorted_cont = StorageByName("containers", "plugins")
            sorted_cont[wrapper.container_unique_key] = wrapper
            self._containers_by_keys[wrapper.plugin_unique_key] = sorted_cont

    def get_all_plugin_names(self):
        return self._containers_by_keys.get_names()

    def get_all_wrappers_for(self, plugin_name: str):
        cont_stores = self._containers_by_keys.get_all_with_name(VersionedName(plugin_name))
        return list(chain(*[conts.get_all() for conts in cont_stores]))  # efficient list of list -> list reduction

    def find_wrapper(self, unique_key: UniqueKey):
        if self._containers_by_keys.is_present(unique_key.plug_key) and \
                self._containers_by_keys[unique_key.plug_key].is_present(unique_key.cont_key):
            return self._containers_by_keys[unique_key.plug_key][unique_key.cont_key]
        raise MissingAttributeException(f"Could not find given container: {str(unique_key)}")
