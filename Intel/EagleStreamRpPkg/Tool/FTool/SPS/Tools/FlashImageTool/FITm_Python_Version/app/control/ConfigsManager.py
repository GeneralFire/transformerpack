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
import re
from collections import namedtuple
from typing import List, Tuple

from app.components.Container import ContainerWrapper
from app.control.ContainersManager import ContainersManager
from app.utils import Consts
from library.tool.ColorPrint import ColorPrint
from library.ConfigData import ConfigData
from library.XmlContainer import XmlContainer
from library.tool.LibConfig import LibConfig
from library.tool.LibException import LibException
from library.tool.UniqueKey import UniqueKey, VersionedName
from library.tool.components.IComponent import IComponent
from library.tool.components.IterableComponent import IterableComponent
from library.tool.dependencies.SetDependency import SetDependency
from library.tool.exceptions import InvalidAttributeException
from library.tool.exceptions import MissingAttributeException, DependencyException
from library.tool.components.FileComponent import FileComponent
from .WrappersManager import WrappersManager
from library.tool.utils import validate_path_to_file
from library.tool.Converter import Converter


class ConfigsManager:
    '''
     Managing configuration (settings) of the containers
    '''

    def __init__(self, containers_mng: ContainersManager):
        self.wrapper_mng = WrappersManager()
        self.xml_overrides: List[ContainerWrapper] = []
        self.containers_mng = containers_mng
        self.dependencies = []
        self.duplicates = []
        self._layout_name = None

    @property
    def layout_name(self):  # pragma: no cover
        return self._layout_name

    @layout_name.setter
    def layout_name(self, new_layout_name):  # pragma: no cover
        self._layout_name = new_layout_name
        self.wrapper_mng.layout_name = new_layout_name

    @staticmethod
    def _check_params_overridable(cont_wrapper, cont_overrides):
        if isinstance(cont_wrapper.container_ref, XmlContainer):
            setting_names = set(cont_wrapper.container_ref.overridable_settings_info_flat)
            override_names = set([ovr_name.split('=')[0].split('/')[0] for ovr_name in cont_overrides])
            if len(setting_names & override_names) != len(override_names):
                all_setting_names = set(cont_wrapper.container_ref.all_settings_info_flat)
                err_msg = []
                settings_non_overridable = override_names - setting_names & all_setting_names
                if len(settings_non_overridable):
                    err_msg.append(f'Specified parameter(s) cannot be overridden: {settings_non_overridable}')

                settings_missing = override_names - all_setting_names
                if len(settings_missing):
                    err_msg.append(
                        f'Specified parameter(s) are not a part of container {cont_wrapper.name}: {settings_missing}')

                raise InvalidAttributeException('\n'.join(err_msg))
        else:
            raise InvalidAttributeException('Settings in this type of container cannot be overridden')

    def load_config(self, cont_wrapper: ContainerWrapper, platform=None):
        if cont_wrapper.is_from_plugin():
            ColorPrint.debug(f"Loading {cont_wrapper} config")
            self.wrapper_mng.add_wrapper(cont_wrapper)
            cont_wrapper.get_config(platform)

    def load_configs(self, containers: List[ContainerWrapper], platform: str = None):
        for cont_wrapper in containers:
            if not cont_wrapper.is_external_bin:
                self.load_config(cont_wrapper, platform)

        self._load_dependencies()
        self._load_decomp_dependencies()
        self.resolve_all_dependencies()
        self._resolve_dependent_settings_paths()

    def reload_configs(self, containers: List[ContainerWrapper], platform: str):
        for cont_wrapper in containers:
            if not cont_wrapper.is_external_bin and cont_wrapper.is_from_plugin():
                cont_wrapper.container_ref._configuration = None
                cont_wrapper.get_config(platform)

        self._load_dependencies()
        self._load_decomp_dependencies()
        self.resolve_all_dependencies()
        self._resolve_dependent_settings_paths()

    def set_xml_overrides(self, xml_overrides: List[ContainerWrapper]):
        self.xml_overrides = xml_overrides

    def apply_cmd_overrides(self, containers: List[ContainerWrapper], param_overrides: dict):
        overriden_settings: List[Tuple[ContainerWrapper, List[IComponent]]] = []
        import copy
        missing = copy.copy(param_overrides)
        for cont_wrapper in containers:
            if not cont_wrapper.is_external_bin:
                cont_overrides = param_overrides.get(cont_wrapper.name, None) if param_overrides else None
                if cont_overrides is not None:
                    del missing[cont_wrapper.name]
                if cont_overrides:
                    self._check_params_overridable(cont_wrapper, cont_overrides)
                    overriden_settings_in_container = cont_wrapper.apply_cmd_overrides(cont_overrides)
                    overriden_settings.append((cont_wrapper, overriden_settings_in_container))

        if missing is not None:
            if LibConfig.enableRegions in missing:
                del missing[LibConfig.enableRegions]
            if len(missing):
                raise InvalidAttributeException(
                    'Could not override all the specified parameters. Following containers are missing: ' +
                    ', '.join(missing.keys()))

        set_deps = [x for x in self.dependencies + self.duplicates if isinstance(x, SetDependency)]
        rest_deps = [x for x in self.dependencies + self.duplicates if not isinstance(x, SetDependency)]
        # process set_dependencies first, than process the rest of the dependencies
        for dependencies in [set_deps, rest_deps]:
            for cont_wrapper, settings in overriden_settings:
                for setting in settings:
                    self.resolve_dependencies_for(cont_wrapper.unique_key, setting.name, dependencies,
                                                  modified_settings=settings)
                    for child in setting.children:
                        self.resolve_dependencies_for(cont_wrapper.unique_key, child.name, dependencies,
                                                      modified_settings=settings)

    def set_container_setting(self, cont_key: UniqueKey, setting_name, setting_value):
        setting = self.find_setting(cont_key, setting_name)
        if setting_value != '' and isinstance(setting, FileComponent):
            validate_path_to_file(setting_name, setting_value)
        setting.parent_container_key = cont_key
        # parsing a string value for bit_register or its bit affects whole bit_register and at least one bit.
        # So the parse_string_value method is overridden to return the list of modified settings.
        modified_settings = setting.parse_string_value(setting_value)
        affected_by_dependencies = []
        for sett in modified_settings:
            sett.parent_container_key = cont_key
            settings_affected = self.resolve_dependencies_for(cont_key, sett.name, self.dependencies + self.duplicates)
            affected_by_dependencies.extend(settings_affected)
        modified_settings.extend(affected_by_dependencies)
        return modified_settings

    def override_all_configs(self, param_overrides=None):
        modified = False
        for overridden_cont in self.xml_overrides:
            unq_name = overridden_cont.global_unique_key
            if unq_name in self.wrapper_mng.all_containers:
                orig_cont = self.wrapper_mng.all_containers[unq_name]
                if not orig_cont.is_external_bin:
                    orig_config = orig_cont.get_config()
                    settings = {}
                    overridden_cont.get_all_children(settings)
                    cont_overrides = param_overrides.get(overridden_cont.name, None) if param_overrides else None
                    if cont_overrides:
                        self._check_params_overridable(orig_cont, cont_overrides)
                        for override in cont_overrides:
                            path, value = self._parse_override_entry(override)
                            if path in settings:
                                setting = settings[path]
                                setting.value = value
                    self.override_config_for(orig_config, list(settings.values()))
                    modified = True
        return modified

    @staticmethod
    def container_enabler(orig_cont, param_overrides):
        if param_overrides:
            params = param_overrides.get(LibConfig.enableRegions, [])
            for cont in params:
                cont_name, enabled = ConfigsManager._parse_override_entry(cont, "region_name")
                try:
                    enabled = Converter.string_to_bool(enabled, can_be_int=True)
                except ValueError:
                    raise LibException(f"Invalid option '{cont}', use syntax: <region_name>=<bool_value>")
                if orig_cont.name == cont_name:
                    orig_cont.is_present_in_layout = enabled
                    orig_cont.enabled = enabled

    def override_binaries(self, configs_to_override: List[ContainerWrapper]):
        for overrdn_cont in self.xml_overrides:
            cont_key = overrdn_cont.container_unique_key
            # So far we have no info about real key for layout containers, matching only by names
            destined_cont = list(
                filter(lambda cont: cont_key.name == cont.container_unique_key.name, configs_to_override))
            if destined_cont:
                destined_cont = next(iter(destined_cont))
                if overrdn_cont.binary_path:
                    destined_cont.set_external_binary(overrdn_cont.binary_path, overrdn_cont.enabled)
                elif not overrdn_cont.binary_path and not overrdn_cont.enabled and destined_cont.is_external_bin:
                    destined_cont.unset_external_binary()
                elif (not overrdn_cont.binary_path and overrdn_cont.enabled) or \
                     (not overrdn_cont.binary_path and not overrdn_cont.enabled and not destined_cont.is_required):
                    destined_cont.enabled = overrdn_cont.enabled
                    destined_cont.is_present_in_layout = overrdn_cont.enabled
            elif not self.containers_mng.is_container_present(overrdn_cont.global_unique_key):
                ColorPrint.warning(f"Warning: Container {cont_key.name} does not exist.")

    def resolve_all_dependencies(self):
        self.resolve_dependencies(self.dependencies + self.duplicates)

    def resolve_dependencies(self, dependencies_and_duplicates):
        ColorPrint.debug("Resolving dependencies")
        for dependency in dependencies_and_duplicates:
            changed = dependency.execute()
            for component in changed:
                self.resolve_dependencies_for(dependency.unique_key, component.name, dependencies_and_duplicates)

    def resolve_dependencies_for(self, cont_key: UniqueKey, setting_name: str, dependencies_and_duplicates,
                                 modified_settings=None):
        modified_settings = [] if modified_settings is None else modified_settings
        for dependency in dependencies_and_duplicates:
            real_key = self.containers_mng.get_real_container_key(dependency.unique_key)
            if real_key == cont_key and dependency.src_setting_ref.name == setting_name:
                changed = dependency.execute()
                anything_was_changed_before = bool(set(changed) & set(modified_settings))
                if anything_was_changed_before:
                    ColorPrint.warning(dependency.inconsistency_warn())
                modified_settings.extend(changed)

                for component in changed:
                    component.parent_container_key = dependency.unique_key
                    self.resolve_dependencies_for(dependency.unique_key, component.name, dependencies_and_duplicates,
                                                  modified_settings)
        return modified_settings

    def _resolve_dependent_settings_paths(self):
        # TODO: To be removed - MfsContainer refectoring needed
        for container in self.wrapper_mng.all_containers.values():
            settings = dict()
            for key, path in container.container_ref.dependent_settings_paths.items():
                container_key, plugin_key, setting_name = self._parse_dependent_setting_path(path)
                unique_key = UniqueKey(cont_key=container_key, plug_key=plugin_key)
                settings[key] = self.find_setting(unq_key=unique_key, set_name=setting_name)
                container.container_ref.dependent_settings = settings

    def config(self, real_key: UniqueKey):
        if real_key in self.wrapper_mng.all_containers:
            return self.wrapper_mng.all_containers[real_key].get_config()
        else:
            return None

    def find_container_wrapper(self, cont_key: UniqueKey) -> ContainerWrapper:
        return self.wrapper_mng.find_wrapper(cont_key)

    def get_container_setting(self, cont_key: UniqueKey, setting_name):
        wrapper = self.find_container_wrapper(cont_key)
        config = wrapper.get_config()
        if setting_name in config.settings:
            return config.settings[setting_name]
        else:
            raise MissingAttributeException(
                f"Setting could not be acquired: {setting_name} from {cont_key.to_format_str()}")

    def get_binary_inputs_for_region(self, name, only_required: bool = True):
        binary_inputs = []
        for container in self.wrapper_mng.all_containers.values():
            binary_inputs.extend(setting for setting in container.get_config().settings.values()
                                 if setting.binary_input and
                                 (not setting.region_option and container.name == name or setting.region_option == name)
                                 and (not only_required or setting.required_binary is not False))
        return binary_inputs

    def get_all_region_options(self) -> []:
        gui_options = []
        for container in self.wrapper_mng.all_containers.values():
            setting_info = namedtuple('SettingsInfo', ['setting', 'container_key'])
            gui_options.extend(
                setting_info(setting, container.unique_key) for setting in container.get_config().settings.values()
                if setting.region_option is not None or setting.binary_input)
        return gui_options

    def _load_dependencies(self):
        for container in self.wrapper_mng.all_containers.values():
            dst_config = container.get_config()
            for dst_setting in dst_config.settings.values():
                if dst_setting.has_dependencies():
                    self._append_dependency(dst_setting, dst_config.unique_key, container, self.dependencies)
                if dst_setting.has_duplicates():
                    self._append_dependency(dst_setting, dst_config.unique_key, container, self.duplicates,
                                            duplicates=True)

    def _load_decomp_dependencies(self):
        # TODO: move logic to BinaryGenerator
        for container in self.wrapper_mng.all_containers.values():
            if container.has_decomp_dependency():
                decomp = container.get_decomp_dependency()
                sett = decomp.expr_engine.calculate_component_from_path(decomp.file_dep_path)
                sett.add_decomp_dependency(decomp)

    def _append_dependency(self, dst_setting: IComponent, dst_key: UniqueKey, container, cont, duplicates=False):
        dependencies = dst_setting.get_dependencies(duplicates)
        for dependency in dependencies:
            if dependency.unique_key.is_empty():
                dependency.unique_key = container.global_unique_key
            try:
                referenced_setting = self.find_setting(dependency.unique_key, dependency.referenced_set_name)
                dependency.set_referenced_setting(referenced_setting)
                cont.append(dependency)
            except (DependencyException, MissingAttributeException) as e:
                raise DependencyException("Dependency error:\n" +
                                          str(e) +
                                          f"Requestor setting: {dst_setting.name} "
                                          f"in {dst_key.to_format_str()}")

    def find_setting(self, unq_key: UniqueKey, set_name) -> IComponent:
        real_key = self.containers_mng.get_real_container_key(unq_key)
        if real_key not in self.wrapper_mng.all_containers:
            raise MissingAttributeException(
                f"Missing {real_key} container that owns {real_key.cont_key.name}:{set_name} setting\n")
        wrapper = self.wrapper_mng.all_containers[real_key]
        config = wrapper.get_config()
        try:
            return config.setting(set_name)
        except MissingAttributeException as e:
            raise MissingAttributeException(str(e) + f" in {unq_key.to_format_str()}.\n")

    @staticmethod
    def _parse_dependent_setting_path(path):
        parts = path.split(LibConfig.pathSeparator)
        if len(parts) == 2:
            plugin_key = VersionedName()
            container_key = VersionedName(name=parts[0])
            setting_name = parts[1]
        elif len(parts) == 3:
            plugin_key = VersionedName(name=parts[0])
            container_key = VersionedName(name=parts[1])
            setting_name = parts[2]
        else:
            raise LibException(f'Invalid path to dependent settings: {path}'
                               f'should be: "<plugin_name>(optional){LibConfig.pathSeparator}'
                               f'<container_name>{LibConfig.pathSeparator}'
                               f'<setting_name>"')
        return container_key, plugin_key, setting_name

    @staticmethod
    def override_config_for(orig_config: ConfigData, settings: List[IComponent]):
        cont_name_str = orig_config.unique_key.to_format_str()
        for over_setting in settings:
            # search for iterable settings iterable[index].setting
            reg = re.match(Consts.ITERABLE_SETTING_REGEX, over_setting.name)
            if reg:
                ConfigsManager._override_iterable(reg, orig_config, cont_name_str, over_setting)
            elif over_setting.name in orig_config.settings:
                orig_setting = orig_config.settings[over_setting.name]
                if orig_setting.read_only:
                    ConfigsManager._print_warn_cont_header(cont_name_str)
                    ColorPrint.warning(f": setting '{orig_setting}' is read only and will not be overridden")
                else:
                    if over_setting.node_tag not in orig_setting.optionTypeMap:
                        ConfigsManager._parse_setting_value(over_setting.value, orig_setting, cont_name_str)
            else:
                ConfigsManager._print_warn_sett_not_found(cont_name_str, over_setting.name)

    @staticmethod
    def _override_iterable(reg: re, orig_config: ConfigData, cont_name_str: str, over_setting: IComponent):
        iterable_name = reg.group(1)
        if iterable_name in orig_config.settings:
            iterable = orig_config.settings[iterable_name]
            # find iterable entry with proper index
            index = reg.group(2)
            entry = iterable.get_child(IterableComponent.entryTag + index)
            # find setting inside entry node
            setting_name = reg.group(3)
            settings = [setting for setting in entry.descendants if setting.name == setting_name]
            if len(settings) != 1:
                ConfigsManager._print_warn_sett_not_found(cont_name_str, over_setting.name)
                ColorPrint.warning(f'there is no setting with name {setting_name}')
            else:
                ConfigsManager._parse_setting_value(over_setting.value, settings[0], cont_name_str)
        else:
            ConfigsManager._print_warn_sett_not_found(cont_name_str, over_setting.name)
            ColorPrint.warning(f'there is no iterable structure with name {iterable_name}')

    @staticmethod
    def _parse_setting_value(new_value: str, orig_setting: IComponent, cont_name: str):
        try:
            orig_setting.parse_string_value(new_value)
        except (ValueError, LibException) as e:
            ConfigsManager._print_warn_cont_header(cont_name)
            ColorPrint.warning(f"- setting {orig_setting} with value '{new_value}' "
                               f"could not be parsed:\n{str(e)}")

    @staticmethod
    def _print_warn_cont_header(cont_name):
        return ColorPrint.warning(f"Warning: Container {cont_name}", "")

    @staticmethod
    def _print_warn_sett_not_found(cont_name: str, sett_name: str):
        ConfigsManager._print_warn_cont_header(cont_name)
        ColorPrint.warning(f" does not have setting with name: {sett_name} - passing")

    @staticmethod
    def _parse_override_entry(override, entry_name="setting"):
        parts = override.split("=")
        if len(parts) != 2:
            raise LibException(f"Invalid option '{override}', use syntax: <{entry_name}>=<value>")
        path = parts[0]
        value = parts[1]
        return path, value
