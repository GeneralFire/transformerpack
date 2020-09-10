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
from library.IContainer import IContainer, BuildPhaseEnum
from library.IFitPlugin import ContainerTupleOrder
from library.tool.exceptions import InvalidAttributeException, MissingAttributeException, WrongTypeException, \
    GeneralException
from library.tool.UniqueKey import UniqueKey, VersionedName
from app.control.FitPluginInfo import FitPluginInfo
from typing import Tuple, List


class ContainersManager:
    '''
        The ContainersManager is responsible for storing all external containers (from plugins), that are loaded to FIT.
    '''

    def __init__(self):
        self._plugins_storage = StorageByName(storage_name=StorageByName.Name.Plugins,
                                              items_name=StorageByName.Name.Containers)  # sorted by cont_key/ plug_key
        self._layout_name = None

    @property
    def layout_name(self):
        return self._layout_name

    @layout_name.setter
    def layout_name(self, new_layout_name):
        self._layout_name = new_layout_name

    def add_plugin_object(self, container_meta: Tuple[VersionedName, List[UniqueKey]], yapsy_plugin):
        container_unq_name = container_meta[0]

        if self._plugins_storage.is_present(container_unq_name):
            self._plugins_storage[container_unq_name, self.layout_name][yapsy_plugin.unique_key] = yapsy_plugin
        else:
            store = StorageByName(storage_name=StorageByName.Name.Containers,
                                  items_name=StorageByName.Name.Plugins)
            store[yapsy_plugin.unique_key] = yapsy_plugin
            self._plugins_storage[container_unq_name] = store

    def get_container_with_key(self, unique_key: UniqueKey):
        real_cont_key, yapsy_plugin = self._get_plugin_obj_with_key(unique_key)
        return self._get_container_from_plugin(real_cont_key, yapsy_plugin)

    def get_container(self, unique_key: UniqueKey):
        _, container = self.get_container_with_key(unique_key)
        return container

    def get_real_container_key(self, unique_key: UniqueKey):
        real_glb_key, _ = self.get_container_with_key(unique_key)
        return real_glb_key

    def is_container_present(self, unique_key: UniqueKey):
        cont_key = unique_key.cont_key
        plug_key = unique_key.plug_key
        if self._plugins_storage.is_present(cont_key):
            plug_store = self._plugins_storage.get_item(cont_key, layout_name=self.layout_name)
            return plug_store.is_present(plug_key) if plug_store else False
        return False

    def _get_plugin_obj_with_key(self, unique_key):
        cont_key = unique_key.cont_key
        plug_key = unique_key.plug_key
        try:
            if not plug_key.is_empty() and not cont_key.has_name_ver():  # special cases, when we have to search plugins, among container's StorageByName
                real_cont_key, yapsy_plugin = self._search_for_container(unique_key)
            else:
                real_cont_key, plug_store = self._plugins_storage.get_item(cont_key, layout_name=self.layout_name,
                                                                           with_real_key=True)
                yapsy_plugin = plug_store.get_item(plug_key, layout_name=self.layout_name)
        except GeneralException as e:
            raise MissingAttributeException("Container: " + unique_key.to_format_str() + "\n" + str(e))
        return real_cont_key, yapsy_plugin

    def get_plugin_object(self, unique_key: UniqueKey):
        _, cont = self._get_plugin_obj_with_key(unique_key)
        return cont.plugin_object

    def _search_for_container(self, unique_key: UniqueKey):
        all_plug_stores = self._plugins_storage.get_all_with_name(unique_key.cont_key, True)
        for key, plug_store in all_plug_stores:
            if plug_store.is_present(unique_key.plug_key):
                return key, plug_store[unique_key.plug_key]
        raise MissingAttributeException(f"Could not find item with key: {unique_key}")

    @staticmethod
    def _add_container(container_meta: Tuple[VersionedName, List[UniqueKey], BuildPhaseEnum], yapsy_plugin: FitPluginInfo):
        """
        It creates objects of the containers
        :param container_meta:
        :param yapsy_plugin:
        :return: nothing
        """
        containers = yapsy_plugin.plugin_object.containers
        containers_meta = yapsy_plugin.containers_meta

        if len(containers_meta) != len(containers):
            MissingAttributeException(f"Containers indicated in meta xml does not fit with containers"
                                      f" list in plugin {yapsy_plugin.unique_key.to_format_str()}")

        container_unq_name = container_meta[0]

        results = list(filter(lambda cont_tup: cont_tup[0] == container_unq_name, containers_meta))

        if len(results) != 1:
            raise InvalidAttributeException(f"There are {len(results)} containers with name {container_unq_name.to_format_str()} "
                                      f"in plugin: {yapsy_plugin.unique_key.to_format_str()}")

        cont_index = containers_meta.index(container_meta)
        info_tup = containers[cont_index]

        kwargs = {}

        if len(info_tup) != len(ContainerTupleOrder) - 1 and len(info_tup) != len(ContainerTupleOrder):
            raise InvalidAttributeException("Incorrect number of one of the container tuples",
                                            container_unq_name.name)

        if len(info_tup) == len(ContainerTupleOrder) and type(info_tup[ContainerTupleOrder.Kwargs.value]) == dict:
            kwargs = info_tup[ContainerTupleOrder.Kwargs.value]

        container_type = info_tup[ContainerTupleOrder.Class.value]
        if not issubclass(container_type, IContainer):
            raise WrongTypeException(f"Container should inherit from IContainer, got type: {type(container_type)}",
                                     container_unq_name.name)
        try:
            container_obj = container_type(**kwargs)
            container_obj.name = container_unq_name.name
            container_obj.build_phase = container_meta[2]
            container_obj.version = container_unq_name.version
            container_obj.required_containers = container_meta[1]
            if container_unq_name not in yapsy_plugin.plugin_object.used_containers:
                yapsy_plugin.plugin_object.used_containers[container_unq_name] = container_obj
            else:
                raise InvalidAttributeException(f"Container {container_unq_name.to_format_str()} "
                                                "is already added")
        except Exception as e:
            raise GeneralException("Error occurred while trying to create container %s from plugin %s: '%s'" %
                                   (container_unq_name.name, yapsy_plugin.unique_key.to_format_str(), str(e)))

    @staticmethod
    def _get_container_from_plugin(cont_key: VersionedName, plugin: FitPluginInfo):
        if cont_key not in plugin.plugin_object.used_containers:
            cont_meta = next((c for c in plugin.containers_meta if c[0] == cont_key), None)
            if cont_meta:
                ContainersManager._add_container(cont_meta, plugin)
            else:
                raise MissingAttributeException(f"Could not find container with key: {cont_key}")
        return UniqueKey(cont_key, plugin.unique_key), plugin.plugin_object.get_container(cont_key)
