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
from enum import Enum

from app.control.ContainersManager import ContainersManager
from app.utils import Consts
from library.IContainer import BuildPhaseEnum
from library.XmlContainer import XmlContainer
from library.tool.ColorPrint import ColorPrint
from library.tool.LibConfig import LibConfig
from library.tool.UniqueKey import UniqueKey, VersionedName
from library.tool.exceptions import InvalidSizeException, GeneralException, MissingAttributeException, ActionException
from library.tool.utils import parse_json_str, check_file_path
from library.utils import get_relative_path
from .ConfigsManager import ConfigsManager
from .ExecTree import ExecTree
from .LayoutEngine import LayoutEngine
from .UserXmlParser import UserXmlParser
from .WrappersManager import WrappersManager
from .builders import create_plugin_manager


class FitEngine:
    '''
        FitEngine - core of the ModularFit, integrate plugins, gui, LayoutEngine, cmd calls its methods
    '''
    pluginCategory = "Plugin"
    pluginExt = "mxml"

    class ActionEnum(Enum):
        pre_build = 1
        post_build = 2

        def __str__(self):
            if self.value == 1:
                return "pre build"
            else:
                return "post build"

    def __init__(self, plugins_dir: str, input_layout: str, is_gui: bool, is_help: bool):
        LibConfig.isGui = is_gui
        self.is_gui = is_gui
        self.is_help = is_help
        self.plugins_dir = plugins_dir
        self.exec_tree = ExecTree()
        self.containers_mng = ContainersManager()
        self.layout_engine: LayoutEngine = LayoutEngine(input_layout)
        self.yapsy_plug_mng = create_plugin_manager(self.pluginCategory, self.pluginExt)
        self.configs_manager = ConfigsManager(self.containers_mng)
        self.binary_paths = {}
        self.initialized = False

    def init(self, param_overrides=None, layout_overrides=None):
        layout_settings = param_overrides.pop(Consts.LAYOUT_CONTAINER, None) if param_overrides else None
        self._collect_available_containers()
        self.layout_engine.fill_platform_map()
        self.layout_engine.pick_layout_from_platform_map()
        self.layout_engine.load_layout()
        # After loading layout layout_engine know name of the current layout - we have to spread it to other clases
        # that deal with storages.
        self.containers_mng.layout_name = self.layout_engine.layout_name
        self.configs_manager.layout_name = self.layout_engine.layout_name
        self.override_layout_by_binaries()
        self.layout_engine.update_containers()
        self.layout_engine.apply_setting_xml_overrides()
        self.layout_engine.override_layout_settings(layout_settings)
        self.layout_engine.apply_layout_overrides(layout_overrides)
        self.update_layout_containers_wrappers()
        self.layout_engine.bin_generator.apply_region_order()
        self.layout_engine.apply_region_enabling_cmd(param_overrides)
        self._create_execution_queue(new_layout=True)

        self.configs_manager.load_configs(self.exec_tree.all_nodes,
                                          platform=self.layout_engine.settings.default_platform)
        self.override_configs(param_overrides)
        self.configs_manager.apply_cmd_overrides(self.exec_tree.all_nodes, param_overrides)
        self.print_container_tree()
        self.initialized = True

    def update_layout_containers_wrappers(self):
        """This method updates layout's containers to have current global unique key info and container ref"""
        for layout_cont in self.layout_engine.containers_list:
            is_in_plugin = self.containers_mng.is_container_present(layout_cont.unique_key)
            is_binary = layout_cont.is_external_bin
            if is_in_plugin and not is_binary:
                global_unq_key, layout_cont.container_ref = self.containers_mng.get_container_with_key(
                    layout_cont.global_unique_key)
                layout_cont.unique_key.update(global_unq_key)
        self.layout_engine.update_containers()

    def collect_container_overridable_settings_info(self, cont_name: str):
        self._collect_available_containers()
        self.layout_engine.fill_platform_map()
        self.layout_engine.pick_layout_from_platform_map()
        self.layout_engine.load_layout()
        self.containers_mng.layout_name = self.layout_engine.layout_name
        self.configs_manager.layout_name = self.layout_engine.layout_name
        self.update_layout_containers_wrappers()
        self._create_execution_queue()
        settings = {}
        if cont_name == self.layout_engine.bin_generator.layoutTag:
            settings = {'': self.layout_engine.overridable_settings_info()}
        else:
            cont = self.containers_mng.get_container(UniqueKey(VersionedName(cont_name)))
            if isinstance(cont, XmlContainer):
                settings = cont.overridable_settings_info
        return settings

    def print_container_tree(self):
        if not self.is_gui and not self.is_help:
            self.exec_tree.print_tree()

    def print_container_help(self, container: str):
        sett_grouped = self.collect_container_overridable_settings_info(container)
        layout_containers = self.exec_tree.all_nodes
        if container == 'layout' or container in (c.name for c in layout_containers):
            cont_key = None if container == Consts.LAYOUT_CONTAINER else \
                self.containers_mng.get_container_with_key(UniqueKey(VersionedName(container)))[0]
            cont_desc = 'layout' if cont_key is None else cont_key.to_format_str()
            print(f'Settings allowed to be overridden for {cont_desc}')
            for group, settings in sett_grouped.items():
                print(group)
                for name, tag in settings.items():
                    print(''.ljust(2, ' ') + name.ljust(35, ' ') + f'<{tag}>'.ljust(25, ' '))
        else:
            ColorPrint.error(f"Container {container} is not a part of {self.layout_engine.layout_name} layout")

    def load_user_xml(self, user_xml_path: str):
        ColorPrint.info(f"Opening xml: {os.path.abspath(user_xml_path)} ...")
        parser = UserXmlParser(get_relative_path(user_xml_path))
        overr_containers, platform_name_config, layout_settings = parser.parse_all()
        self.layout_engine.platform_name = platform_name_config
        self.configs_manager.set_xml_overrides(overr_containers)
        if layout_settings:
            settings = {}
            layout_settings.get_all_children(settings)
            self.layout_engine.set_xml_overrides(list(settings.values()))
        ColorPrint.success('User xml loaded successfully', gui_visible=True)

    def override_layout_by_binaries(self):
        self.configs_manager.override_binaries(self.layout_engine.containers_list)

    def _get_filter_function(self):
        if LibConfig.isDecompose:
            return lambda cont: cont.can_be_binary and cont.enabled
        return lambda cont: cont.can_be_binary or cont in self.exec_tree.all_nodes

    def _prepare_saved_container_list(self):
        to_be_saved = list(filter(self._get_filter_function(),
                                  self.layout_engine.containers_list))
        cont_with_depend = list(filter(lambda cont: self.exec_tree.has_dependent_containers(cont.unique_key), to_be_saved))
        self._add_dependent_container_list(to_be_saved, cont_with_depend) if not LibConfig.isDecompose else ''
        return to_be_saved

    def _add_dependent_container_list(self, container_list, cont_with_depend):
        if not cont_with_depend:
            return container_list
        cont_names = [container.name for container in container_list]
        for cont in cont_with_depend:
            deps = self.exec_tree.get_dependent_containers(cont.unique_key)
            idx = container_list.index(cont)
            for dep in deps:
                # containers might be dependent for different plugins and we don't want to save them multiple times
                if dep.name in cont_names:
                    continue
                idx += 1
                container_list.insert(idx, dep)
                cont_names.append(dep.name)
        return container_list

    def save_user_xml(self, user_xml_path: str):
        if self.is_gui:
            self._create_execution_queue(new_layout=False)
        parser = UserXmlParser(user_xml_path)
        all_conts_to_save = self._prepare_saved_container_list()
        parser.save_all(all_conts_to_save, self.layout_engine.platform_name, self.layout_engine.layout_version, self.layout_engine.bin_generator.configuration_root)
        ColorPrint.success(f'Configuration file saved successfully: {os.path.abspath(user_xml_path)}', gui_visible=True)

    def build_image(self):
        ColorPrint.info("Building main phase containers")
        self._create_execution_queue(new_layout=False)
        self.execute_pre_build_actions()
        self._build_containers(BuildPhaseEnum.main)
        self.layout_engine.build_main()
        ColorPrint.info("Building post phase containers")
        self._build_containers(BuildPhaseEnum.post)
        self.layout_engine.build_post()
        self.execute_post_build_actions()
        return self.layout_engine.save_built_image()

    def _prepare_buffer(self, real_key: UniqueKey):
        if real_key is None:
            return bytearray(self.layout_engine.bin_generator.buffer)
        cont = self.configs_manager.wrapper_mng.find_wrapper(real_key)
        return bytearray(self.layout_engine.bin_generator.buffer[cont.offset:cont.offset + cont.size])

    def _prepare_params(self, real_key: UniqueKey, action_params: str):
        parsed = parse_json_str(action_params)
        params = {}
        if real_key is None:
            for sett_name in parsed['params']:
                params[sett_name] = self.layout_engine.get_setting(sett_name).value
        else:
            for sett_name in parsed['params']:
                params[sett_name] = self.configs_manager.get_container_setting(real_key, sett_name).value
        return params

    def _update_buffer(self, cont_name: str, buffer: bytearray):
        uk = UniqueKey(VersionedName(cont_name))
        rk = self.containers_mng.get_real_container_key(uk)
        cont = self.configs_manager.wrapper_mng.find_wrapper(rk)
        buff_len = len(buffer)
        if buff_len != cont.size:
            raise InvalidSizeException(f'Given buffer size doesn\'t match container size: {buff_len}. Expected: {cont.size}')

        self.layout_engine.bin_generator.buffer[cont.offset:cont.offset + cont.size] = buffer

    def execute_pre_build_actions(self):
        self._execute_action(self.layout_engine.bin_generator.pre_build, FitEngine.ActionEnum.pre_build)

    def execute_post_build_actions(self):
        self._execute_action(self.layout_engine.bin_generator.post_build, FitEngine.ActionEnum.post_build)

    def _execute_action(self, actions_element, action_type: ActionEnum):
        if actions_element is None:
            return

        ColorPrint.info(f"Executing {str(action_type)} actions")
        for action_node in list(actions_element):
            action_name = action_node.get("name")
            action_params = action_node.get("action_params")
            cont_name = action_node.get("container")

            # Actions on layout are treated differently - we don't need any UniqueKey or plugin reference,
            if cont_name == Consts.LAYOUT_CONTAINER:
                real_key = None
                action_executor = self.layout_engine
            else:
                uk = UniqueKey(VersionedName(cont_name))
                try:
                    real_key = self.containers_mng.get_real_container_key(uk)
                    action_executor = self.containers_mng.get_plugin_object(uk)
                except GeneralException:
                    raise MissingAttributeException(f'Plugin for the specified container could not be found: {cont_name}.\n' +
                                                    f'Verify post_build action [{action_name}] xml definition.')
                containers_by_name = [container for container in self.layout_engine.containers_list if
                                      container.name == cont_name]
                # Making sure there's only one container with specified name
                if len(containers_by_name) > 1:
                    raise ActionException(f'Multiple containers with the same name: [{cont_name}].')
                if any(containers_by_name) and containers_by_name[0].is_external_bin:
                    ColorPrint.debug(
                        f'Skipping action [{action_name}]: container [{cont_name}] loaded from external file.')
                    continue

            params = self._prepare_params(real_key, action_params)

            try:
                if action_type == FitEngine.ActionEnum.post_build:
                    buffer = self._prepare_buffer(real_key)
                    buffer = action_executor.execute_post_build_action(action_name, buffer, params)
                    if buffer is not None:
                        self._update_buffer(cont_name, buffer)
                else:
                    action_executor.execute_pre_build_action(action_name, params)
                ColorPrint.debug(f'Action: {action_name} executed successfully')
            except Exception as e:
                ColorPrint.error(f'Exception occurred while executing action [{action_name}] from [{cont_name}]')
                raise GeneralException(str(e))

    def _create_execution_queue(self, new_layout: bool = True):
        '''
            create tree and list of  list of independent containers:
            Example:
                Tree:
                  ┌descriptor:0.1
                  ├bios:0.1
                  ├ie:0.1
            image┤
                │       ┌pmc:0.1
                └me:0.1┤
                      └oem_key:0.1

                    is mapped to:

                 List of list:
                 [
                    [descriptor:0.1,bios:0.1,ie:0.1,pmc:0.1,oem_key:0.1], <- 1
                    [me:0.1], <- 2
                    [image] <- 3  each list can be executed in parallel in the future
                 ]
        '''
        self.exec_tree.clear(new_layout=new_layout)
        self.exec_tree.create_tree(self.layout_engine, self.containers_mng)
        self.exec_tree.create_ordered_queue()

    def override_configs(self, param_overrides=None):
        modified = self.configs_manager.override_all_configs(param_overrides)
        if modified:
            self.configs_manager.resolve_all_dependencies()

    def _collect_available_containers(self):
        self.yapsy_plug_mng.setPluginPlaces([self.plugins_dir])
        self.yapsy_plug_mng.collectPlugins()

        ColorPrint.info('Plugins detection')
        plugins = self.yapsy_plug_mng.getPluginsOfCategory(self.pluginCategory)
        for plugin in plugins:
            if len([p for p in plugins if p.unique_key == plugin.unique_key]) > 1:
                raise GeneralException(f"Plugin {plugin.name} is duplicated (plugins have the same name and version).")

            containers_to_produce = plugin.containers_meta
            for container_name in containers_to_produce:
                self.containers_mng.add_plugin_object(container_name, plugin)
                ColorPrint.debug(f'{container_name[0]} container detected')
        ColorPrint.info(f'{len(self.containers_mng._plugins_storage.get_names())} containers detected')

    def _build_containers(self, phase):
        if phase == BuildPhaseEnum.main:
            self.binary_paths = {}
        depends_gen = self.exec_tree.depends_generator(phase)
        for i, stage in enumerate(depends_gen):
            ColorPrint.debug(f"Dependency stage {i}")
            for container in stage:  # can be parallel
                if container.enabled is not False:
                    depends_keys = container.dependencies  # container can have partial cont key, we have to obtain real one
                    depends_paths = {}
                    for unq_key in depends_keys:
                        self.set_path_for_dependent_container(unq_key, depends_paths, depends_keys)
                    if phase == BuildPhaseEnum.post:
                        container.image_data = self.layout_engine.image_data
                    container.call_build(depends_paths)
                    self.binary_paths[container.global_unique_key] = container.binary_path

    def set_path_for_dependent_container(self, unq_key: UniqueKey, depends_paths: dict, depends_keys: dict):
        # set path for layout binary container
        if unq_key in self.layout_engine.containers_by_unique_name and \
                self.set_path_for_binary_container(unq_key, depends_paths):
            return
        real_key = self.containers_mng.get_real_container_key(unq_key)
        # set empty path if container is not required and is disabled
        if not depends_keys[unq_key] and self.set_empty_path_for_disabled_container(real_key, depends_paths):
            return
        if real_key in self.binary_paths:
            depends_paths[real_key] = self.binary_paths[real_key]
        else:
            raise GeneralException(f"Can not retrieve data from dependency container: '{real_key}', "
                                   f"please check if it is enabled and properly configured.")

    def set_path_for_binary_container(self, unq_key: UniqueKey, depends_paths: dict) -> bool:
        layout_container = self.layout_engine.containers_by_unique_name[unq_key]
        if not layout_container.is_from_plugin():
            if unq_key in self.binary_paths:
                depends_paths[unq_key] = self.binary_paths[unq_key]
            else:
                depends_paths[unq_key] = ''
            return True
        return False

    def set_empty_path_for_disabled_container(self, real_key: UniqueKey, depends_paths: dict) -> bool:
        container_wrapper = self.configs_manager.find_container_wrapper(real_key)
        if not container_wrapper.enabled:
            depends_paths[real_key] = ''
            return True
        return False

    def override_with_binary(self, unq_key: UniqueKey, path):
        check_file_path(path)
        if unq_key in self.layout_engine.containers_by_unique_name:
            layout_cont = self.layout_engine.containers_by_unique_name[unq_key]
            layout_cont.set_external_binary(path)

    def remove_binary_override(self, unq_key: UniqueKey):
        layout_cont = self.layout_engine.containers_by_unique_name[unq_key]
        layout_cont.unset_external_binary()

    def is_binary_overridden(self, unq_key: UniqueKey):
        if unq_key in self.layout_engine.containers_by_unique_name:
            cont = self.layout_engine.containers_by_unique_name[unq_key]
            return cont.is_external_bin
        raise GeneralException(f'Could not find {unq_key} container.')

    def reload_layout(self, platform):
        self.configs_manager.wrapper_mng = WrappersManager()
        self.configs_manager.xml_overrides.clear()
        self.configs_manager.dependencies.clear()
        self.configs_manager.duplicates.clear()
        self.layout_engine.set_layout_parameters_after_choosing_new_platform(platform)
        self.layout_engine.load_layout()
        # After loading layout layout_engine know name of the current layout - we have to spread it to other clases
        # that deal with storages.
        self.containers_mng.layout_name = self.layout_engine.layout_name
        self.configs_manager.layout_name = self.layout_engine.layout_name
        self.override_layout_by_binaries()
        self.update_layout_containers_wrappers()
        self._create_execution_queue(new_layout=True)
        self.layout_engine.update_containers()
        self.configs_manager.load_configs(self.exec_tree.all_nodes)

    def change_platform(self, platform_name: str = None):
        if not platform_name:
            platform_name = self.layout_engine.settings.default_platform
        self.layout_engine.unload_all_external_binaries()
        self.configs_manager.reload_configs(self.exec_tree.all_nodes, platform_name)
        self.layout_engine.override_layout_setting('default_platform', platform_name)

    def reload_user_xml(self, user_xml_path: str):
        parser = UserXmlParser(user_xml_path)
        overr_containers, platform_name_config, layout_settings = parser.parse_all()
        # platform_name_config = layout_name
        if platform_name_config != self.layout_engine.platform_name:
            self.reload_layout(platform_name_config)
        if layout_settings:
            if 'default_platform' in layout_settings.children_by_name:
                # platform_name_in_config = platform_name.The issue with double usage of 'platform' is about to be
                # fixed soon
                platform_name_in_config = layout_settings.children_by_name['default_platform'].value
                if platform_name_in_config != self.layout_engine.settings_root.children_by_name['default_platform'].value:
                    self.change_platform(platform_name_in_config)
            self.layout_engine.set_xml_overrides(layout_settings.children)
            self.layout_engine.apply_setting_xml_overrides()
            self.layout_engine.bin_generator.apply_region_order()
        self.configs_manager.set_xml_overrides(overr_containers)
        self.override_configs()
        self.override_layout_by_binaries()
        self._enable_container_for_binary_input()
        ColorPrint.success('User xml loaded successfully', gui_visible=True)

    def _enable_container_for_binary_input(self):
        for container in self.configs_manager.wrapper_mng.all_containers.values():
            binary_inputs = self.configs_manager.get_binary_inputs_for_region(container.name)
            if len(binary_inputs) > 0 and all((i.value for i in binary_inputs)):
                self.enable_container(container.unique_key)

    def _get_container_by_name(self, unq_key):
        container_name = unq_key.cont_key.name
        cont = next((c for c in self.layout_engine.containers_list if c.name == container_name), None)
        if not cont:
            raise GeneralException(f"Could not find container named {container_name}")
        return cont

    def enable_container(self, unq_key):
        cont = self._get_container_by_name(unq_key)
        cont.is_present_in_layout = True
        cont.enabled = True

    def disable_container(self, unq_key):
        cont = self._get_container_by_name(unq_key)
        cont.is_present_in_layout = False
        cont.enabled = False

    def is_container_required(self, unq_key):
        cont = self._get_container_by_name(unq_key)
        return cont.is_required

    def check_containers_verions(self):
        for overr_container in self.configs_manager.xml_overrides:
            for container in self.layout_engine.containers_list:
                if overr_container.name == container.name and overr_container.unique_key.plug_key.name != '' \
                        and container.unique_key.plug_key.name != '' \
                        and overr_container.unique_key != container.unique_key:
                    raise GeneralException(f'Incorrect container/plugin version: {container.name}. '
                                           f'Is {overr_container.unique_key}, should be: {container.unique_key}')
