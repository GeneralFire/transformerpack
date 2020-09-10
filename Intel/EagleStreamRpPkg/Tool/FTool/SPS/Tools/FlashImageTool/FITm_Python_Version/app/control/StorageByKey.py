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
from bisect import insort
from enum import Enum

from library.tool.exceptions import MissingAttributeException, GeneralException
from library.tool.UniqueKey import VersionedName
from library.tool.ColorPrint import ColorPrint


class StorageByVersion:
    def __init__(self, key: VersionedName, item):
        self._ver_dict = {}
        self._ver_sorted = []
        self.add_item(key, item)

    def add_item(self, key: VersionedName, item):
        if key.version not in self._ver_dict:
            insort(self._ver_sorted, key.strict_ver)
        else:
            prev_item = self._ver_dict[key.version][1]
            if item != prev_item:
                ColorPrint.warning(f"Warning: Container with the key: {str(key)} already exists!")
                ColorPrint.warning(f"         Plugin '{str(item)}' will replace previous one.")
        self._ver_dict[key.version] = (key, item)

    def get_item(self, key: VersionedName, with_real_key=False):
        tup = self._ver_dict.get(key.version, None)
        if tup:
            return tup if with_real_key else tup[1]
        raise MissingAttributeException(f"Could not find proper version for {key}")

    def get_all_items_desc(self, with_real_key=False):
        items = []
        for s_item in reversed(self._ver_sorted):
            if with_real_key:
                items.append(self._ver_dict[str(s_item)])
            else:
                items.append(self._ver_dict[str(s_item)][1])
        return items

    def get_highest_item(self, layout_name=None, with_real_key=False):
        highest_item_tuple = None
        for version in reversed(self._ver_sorted):
            item_tuple = self._ver_dict[str(version)]
            item = item_tuple[1]
            if type(item) is StorageByName:
                layout_iter = item.get_items_supporting_layout(layout_name)
                if next(layout_iter, None) is None:
                    continue
                highest_item_tuple = item_tuple
                break
            elif layout_name is None or item.is_layout_supported(layout_name):
                highest_item_tuple = item_tuple
                break
        # return None if there is no item that supports given layout
        if highest_item_tuple is None:
            return None
        return highest_item_tuple if with_real_key else highest_item_tuple[1]

    def get_items_supporting_layout(self, layout_name):
        for item in self.get_all_items_desc():
            if type(item) is StorageByName:
                for subitem in item.get_items_supporting_layout(layout_name):
                    yield subitem
            else:
                if layout_name is None or item.is_layout_supported(layout_name):
                    yield item

    def is_present(self, key: VersionedName):
        return key.version in self._ver_dict

    def __getitem__(self, item):  # pragma: no cover
        return self.get_item(item)

    def __setitem__(self, key, value):  # pragma: no cover
        self.add_item(key, value)

    def __iter__(self):  # pragma: no cover
        return iter(self._ver_sorted)

    def __next__(self):  # pragma: no cover
        return next(iter(self._ver_sorted))

    def __len__(self):  # pragma: no cover
        return len(self._ver_sorted)


class StorageByName:
    class Name(Enum):
        Containers = 'containers'
        Plugins = 'plugins'

    def __init__(self, storage_name, items_name):
        self.storage_name = storage_name
        self.items_name = items_name
        self._names_dict = {}

    def add_item(self, key: VersionedName, item):
        if key.name in self._names_dict:
            self._names_dict[key.name].add_item(key, item)
        else:
            self._names_dict[key.name] = StorageByVersion(key, item)

    def get_item(self, key: VersionedName, layout_name=None, with_real_key=False):
        ver_store = self._names_dict.get(key.name, None)
        if ver_store:
            if key.has_name_ver():
                return ver_store.get_item(key, with_real_key=with_real_key)
            else:
                return ver_store.get_highest_item(layout_name=layout_name, with_real_key=with_real_key)
        elif key.is_empty() and len(self._names_dict) == 1:
            return self._get_only_item()
        elif key.is_empty() and len(self._names_dict) > 1:
            items_supporting_layout = list(self.get_items_supporting_layout(layout_name))
            if not items_supporting_layout:
                raise GeneralException(
                    f"There is no {key.to_format_str()} {self.items_name} that supports {layout_name} layout")
            if len(items_supporting_layout) > 1:
                raise GeneralException(f"Ambiguous key: {len(items_supporting_layout)} matches in"
                                       f"{self.storage_name}s: " + ", ".join([p.unique_key.to_format_str()
                                                                              for p in items_supporting_layout]))
            return items_supporting_layout[0]
        raise MissingAttributeException(f"Could not find key: {str(key)}")

    def get_items_supporting_layout(self, layout_name):
        for item in self._names_dict.values():
            for subitem in item.get_items_supporting_layout(layout_name=layout_name):
                yield subitem

    def get_names(self):
        return list(self._names_dict.keys())

    def get_all_with_name(self, key: VersionedName, with_real_key=False):
        ver_store = self._names_dict.get(key.name, None)
        if ver_store:
            return ver_store.get_all_items_desc(with_real_key)
        else:
            return []

    def get_all(self, with_real_key=False):
        all_items = []
        for ver_store in self._names_dict.values():
            items = ver_store.get_all_items_desc(with_real_key)
            all_items.extend(items)
        return all_items

    def is_present(self, key: VersionedName):
        if key.has_name_ver():
            return self._is_present_any_ver(key) and self._is_present_with_ver(key)
        elif not key.is_empty():
            return self._is_present_any_ver(key)
        else:
            return self._is_present_any()

    def _is_present_with_ver(self, key: VersionedName):
        return self._names_dict[key.name].is_present(key)

    def _is_present_any_ver(self, key: VersionedName):
        return key.name in self._names_dict

    def _is_present_any(self):
        return len(self._names_dict) > 0

    def _get_only_item(self):
        ver_store = next(iter(self._names_dict.values()))
        return ver_store.get_highest_item()

    def __getitem__(self, item):
        if type(item) is tuple:
            return self.get_item(*item)
        else:
            return self.get_item(item)

    def __setitem__(self, key, value):  # pragma: no cover
        self.add_item(key, value)

    def __iter__(self):  # pragma: no cover
        return iter(self._names_dict)

    def __next__(self):  # pragma: no cover
        return next(iter(self._names_dict))

    def __len__(self):  # pragma: no cover
        return len(self._names_dict)
