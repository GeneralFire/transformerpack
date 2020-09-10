#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
INTEL CONFIDENTIAL
Copyright 2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""
import os
from stat import S_ISUID, S_ISGID, S_IWOTH

from app.utils import Consts
from library.tool.LibConfig import LibConfig
from library.tool.ColorPrint import ColorPrint
from library.tool.exceptions import GeneralException


def is_windows():
    '''
    Checks whether current OS is Windows
    :return: True if current OS is Windows
    '''

    return os.name == "nt"


if is_windows():
    import win32api
    import win32con
    import win32security
    import ntsecuritycon


class Constants:
    '''
    Access Management constants only.
    '''
    GENERIC_ACCESS_ERROR_MSG    = 'Wrong access rights for: '
    USER_GUIDE_ACL_REFERENCE    = '\nFor further information, please refer to User Guide ' + \
                                 f"or run application as Administrator/root with '{Consts.SET_DIR_ACL_ARG}' argument only to set required Access Control List.\n" + \
                                 f" Otherwise, run application with '{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}' argument to skip access rights check.\n"
    # Windows ACL constants
    AUTHENTICATED_USERS_SID         = 'S-1-5-32'
    DOMAIN_ADMINS_SID               = 'S-1-5-21'
    BUILTIN_ADMINISTRATORS_SID      = 'S-1-5-11'
    WHITELISTED_ACL                 = [AUTHENTICATED_USERS_SID, DOMAIN_ADMINS_SID, BUILTIN_ADMINISTRATORS_SID]
    AUTHENTICATED_USERS_SID_NAME    = 'Authenticated Users'
    DOMAIN_ADMINS_SID_NAME          = 'Domain Admins'
    BUILTIN_ADMINISTRATORS_SID_NAME = 'Administrators'
    WHITELISTED_NAMES_ACL           = [AUTHENTICATED_USERS_SID_NAME, DOMAIN_ADMINS_SID_NAME, BUILTIN_ADMINISTRATORS_SID_NAME]
    SID_STRING_LEN                  = len(DOMAIN_ADMINS_SID)
    ALLOWED_ACE_TYPE                = 'ACCESS_ALLOWED_ACE_TYPE'
    # Linux ACL constants
    OTHERS_WRITE_BIT_MASK       = S_IWOTH
    USER_GROUP_STICKY_BIT_MASK  = S_ISUID | S_ISGID


class WrongAccessException(Exception):
    def __init__(self, path):
        super().__init__(f"Wrong access rights: {path}")


class AccessControlEntry:
    '''
        Represents Windows Access Control Entry object.
    '''
    def __init__(self, ace):
        '''
        Initializes ACE fields.
        :param ace: raw PyACE object
        '''
        self.type = ace[0][0]
        self.flags = ace[0][1]
        self.sid = ace[2]
        self.sid_name = win32security.LookupAccountSid(None, ace[2])[0]
        self.mask = ace[1]
        # get only SID string from 'PySID:S-1-5-XX-xxx'
        sid_string = str(self.sid).replace('PySID:', '')
        # and leave only non-varied part (group membership)
        sid_string = sid_string[:Constants.SID_STRING_LEN]
        self.is_whitelisted = sid_string in Constants.WHITELISTED_ACL


class AccessManager:
    '''
    Handles directories access rights validation and configuration.
    '''

    def __init__(self, main):
        '''
        Creates new AccessManager object.
        :param main: defines whether object has been instantiated on startup.
        If True, {Consts.USER_GUIDE_ACL_REFERENCE} will be printed only on fail of self.check_all_nested,
        otherwise every failure of self.check_single will print {Consts.USER_GUIDE_ACL_REFERENCE}.
        '''

        self._fit_main = main

    def check_single(self, path):
        '''
        Checks single directory access rights. Can be omitted with FIT argument '{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}'.
        :param path: input directory to be tested against proper access rights
        :return: True if current Access Control List is proper
        '''

        if LibConfig.isAccessCheckSkipped:
            return True

        # check whether directory exists
        if not os.path.exists(path):
            raise GeneralException(f"Path does not exists: {path}")

        result = self._is_proper_windows_access(path) if is_windows() else self._is_proper_linux_access(path)

        if not result and not self._fit_main:
            self._print_generic_access_error(Constants.USER_GUIDE_ACL_REFERENCE)

        return result

    def check_all_nested(self, path):
        '''
        Checks given directory and it's all nested access rights.
        Can be omitted with FIT argument '--{Consts.SKIP_ACCESS_RIGHTS_CHECK_ARG}'.
        :param path: input directory to be tested against proper access rights
        :return: True if current Access Control List is proper
        '''

        if LibConfig.isAccessCheckSkipped:
            return True

        for subpath in self._get_all_nested(path):
            if not self.check_single(subpath):
                self._print_generic_access_error(Constants.USER_GUIDE_ACL_REFERENCE)
                return False

        return True

    def set_dir_acl(self, path):
        '''
        Sets Access Control List.
        After setting ACL, any future generated folder/file should inherit proper ACL.
        :param path: root directory
        :return:
        '''
        ColorPrint.warning(
            f"Setting ACL for '{path}' and any nested item.\n\nPlease wait a moment, this will take a while..")

        if is_windows():
            self._set_windows_acl(path)
        else:
            self._set_linux_acl(path)

        ColorPrint.success(f"\nACL has been set successfully\n")

    def _set_windows_acl(self, path):
        '''
        Sets whitelisted Access Control List to:
            - given path without any inheritance from parent path
            - all nested child folders/files with inheritance from given path
        After setting ACL, any future generated folder/file should inherit ACL.
        :param path: root directory
        '''
        try:
            # check whether run FITm as Administrator
            import pythoncom
            import pywintypes
            from win32com.shell import shell
            if not shell.IsUserAnAdmin():
                raise GeneralException("Must be run as a Administrator")

            # SE_SECURITY_NAME needed to access DACL and SACL, SE_RESTORE_NAME needed to change owner to someone other than yourself
            new_privs = (
            (win32security.LookupPrivilegeValue('', ntsecuritycon.SE_SECURITY_NAME), win32con.SE_PRIVILEGE_ENABLED),
            (win32security.LookupPrivilegeValue('', ntsecuritycon.SE_RESTORE_NAME), win32con.SE_PRIVILEGE_ENABLED),)

            enable_all_privileges = 0
            proc_hnd = win32api.GetCurrentProcess()
            token_hnd = win32security.OpenProcessToken(proc_hnd, win32security.TOKEN_ALL_ACCESS | win32con.TOKEN_ADJUST_PRIVILEGES)
            modified_privs = win32security.AdjustTokenPrivileges(token_hnd, enable_all_privileges, new_privs)

            # set ACL for root dir implicitly as this must be done at the beginning
            self._set_single_win_acl(path, True)
            paths = self._get_all_nested(path)
            # set new permissions for each child object (folder/file)
            for subpath in paths:
                if subpath == path:
                    continue
                self._set_single_win_acl(subpath, False)

            win32security.AdjustTokenPrivileges(token_hnd, 0, modified_privs)
        except ImportError:
            raise WrongAccessException('Missing pywin32 library!')

    def _set_single_win_acl(self, path, is_root):
        '''
        Sets whitelisted Access Control List. If path is a root directory,
        sets ACL without any inheritance from parent path.
        Otherwise with inheritance from root directory.
        :param path: path to folder or file on which ACL will be set
        :param is_root: defines whether input path is a root directory
        '''
        # lookup whitelisted SIDs and set default owner
        not_whitelisted_sids = []
        whitelisted_sids = []
        default_sid = win32security.LookupAccountName('', Constants.DOMAIN_ADMINS_SID_NAME)[0]
        for sid_name in Constants.WHITELISTED_NAMES_ACL:
            sid = win32security.LookupAccountName('', sid_name)[0]
            whitelisted_sids.append(sid)

        if default_sid is None:
            raise GeneralException('Cannot set proper access rights')

        path_dacl = win32security.ACL()

        security_descriptor = win32security.GetFileSecurity(path, win32security.DACL_SECURITY_INFORMATION)

        path_dacl_descriptor = security_descriptor.GetSecurityDescriptorDacl()

        if path_dacl_descriptor is not None:
            # get current Access Control List
            for ace in self._get_access_control_list(path_dacl_descriptor):
                # store all non-whitelisted sids to modify permissions, if needed
                if not ace.is_whitelisted:
                    not_whitelisted_sids.append(ace.sid)

        if is_root or LibConfig.coreDir == path:
            # clean any Access Control Entries from current ACL
            # which will disable any inheritance from parent folder as well
            win32security.SetNamedSecurityInfo(path, win32security.SE_FILE_OBJECT,
                                               win32security.DACL_SECURITY_INFORMATION | win32security.SACL_SECURITY_INFORMATION,
                                               default_sid, default_sid, None, None)

        # enable inheritance for all child objects
        inheritance = win32security.CONTAINER_INHERIT_ACE | win32security.OBJECT_INHERIT_ACE


        # populate with new ACEs according
        for sid in whitelisted_sids:
            path_dacl.AddAccessAllowedAceEx(win32security.ACL_REVISION_DS,
                                            inheritance, win32con.GENERIC_ALL, sid)

        for sid in not_whitelisted_sids:
            access_mask = self._set_proper_access_mask(ace.mask)
            path_dacl.AddAccessAllowedAceEx(win32security.ACL_REVISION_DS,
                                inheritance, access_mask, sid)

        # set new Access Control List Descriptor (DACL)
        win32security.SetNamedSecurityInfo(path, win32security.SE_FILE_OBJECT,
                                           win32security.DACL_SECURITY_INFORMATION | win32security.SACL_SECURITY_INFORMATION,
                                           default_sid, default_sid, path_dacl, path_dacl)

    @staticmethod
    def _set_proper_access_mask(access_mask):
        '''
        Clears any "modify-like" permissions.
        :param access_mask: Access Control Entry permissions
        :return: modified permissions
        '''
        modify_like_permission_mask = ntsecuritycon.GENERIC_ALL     | ntsecuritycon.GENERIC_WRITE | \
                                      ntsecuritycon.FILE_WRITE_DATA | ntsecuritycon.FILE_APPEND_DATA

        return AccessManager._clear_bit(access_mask, modify_like_permission_mask)

    @staticmethod
    def _set_bit(value, mask):
        '''
        Sets specific bits by given mask.
        :param value: input value to be modified
        :param mask: bit mask defining bits to be modified
        :return: modified value
        '''

        value |= mask
        return value

    @staticmethod
    def _clear_bit(value, mask):
        '''
        Sets specific bits by given mask.
        :param value: input value to be modified
        :param mask: bit mask defining bits to be modified
        :return: modified value
        '''
        value &= ~mask
        return value

    def _set_linux_acl(self, path):
        '''
        Sets ACL by clearing WRITE permission bit for OTHERS
        to given path and any child folder/file.
        Additionally, the sticky-bits are set as well for USER & GROUP
        to enable inheritance of permissions.
        :param path: root directory
        '''
        # check whether run FITm as root
        if not 'SUDO_UID' in os.environ.keys():
            raise GeneralException("Must be run as a sudo")

        # get current permission mask
        path_st_mode = os.stat(path).st_mode

        # prepare new permissions with WRITE & EXECUTE bits cleared for OTHERS
        # and sticky bit for USER and GROUP to enable permission inheritance
        path_st_mode = self._clear_bit(path_st_mode, Constants.OTHERS_WRITE_BIT_MASK)
        path_st_mode = self._set_bit(path_st_mode, Constants.USER_GROUP_STICKY_BIT_MASK)

        # set new permissions for each item starting from path
        for subpath in self._get_all_nested(path):
            os.chmod(subpath, path_st_mode)

    def _is_proper_windows_access(self, path):
        '''
        Check path access rights.
        :param path: existing directory
        :return: Returns True if Access Control Entries are consistent with {Consts.WHITELISTED_ACL}
        and ACE type is {Consts.ALLOWED_ACE_TYPE}, otherwise False.
        '''

        security_descriptor = win32security.GetFileSecurity(path, win32security.DACL_SECURITY_INFORMATION)

        path_dacl_descriptor = security_descriptor.GetSecurityDescriptorDacl()

        if path_dacl_descriptor is None:
            self._print_generic_access_error(f"{Constants.GENERIC_ACCESS_ERROR_MSG} '{path}'", f"No Access Control List found for path: {path}")
            return False

        for ace in self._get_access_control_list(path_dacl_descriptor):
            # check whether ACE is on the whitelist
            # if not, then access mask should not contain any "modify-like" permissions
            if not ace.is_whitelisted and self._is_access_mask_modify_like(ace.mask):
                self._print_generic_access_error(f"{Constants.GENERIC_ACCESS_ERROR_MSG} '{path}'",
                    f"Wrong permissions (access mask={ace.mask}) for Access Control Entry with SID name '{ace.sid_name}'")
                return False

            # if it is whitelisted, check whether ACE type is proper
            if ace.is_whitelisted and not self._is_type_allowed(ace.type):
                self._print_generic_access_error(f"{Constants.GENERIC_ACCESS_ERROR_MSG} '{path}'",
                    f"Wrong ACE (type={ace.type}) Access Control Entry with SID name ='{ace.sid_name}'")
                return False

        return True

    @staticmethod
    def _get_access_control_list(dacl):
        '''
        Gets list of current Access Control Entries.
        :param dacl: Access Control List descriptor
        :return: list of Access Control Entries
        '''
        for ace_no in range(0, dacl.GetAceCount()):
            yield AccessControlEntry(dacl.GetAce(ace_no))

    @staticmethod
    def _is_access_mask_modify_like(mask):
        '''
        Checks whether given value have "modify-like" bits set.
        :param mask: Access Control Entry permissions as access mask
        :return: True if specific bits are set
        '''
        modify_like_permission_mask = ntsecuritycon.GENERIC_ALL     | ntsecuritycon.GENERIC_WRITE | \
                                      ntsecuritycon.FILE_WRITE_DATA | ntsecuritycon.FILE_APPEND_DATA

        return modify_like_permission_mask & mask != 0

    @staticmethod
    def _is_type_allowed(ace_type):
        '''
        Checks whether Access Control Entry is of type Allowed
        :param ace_type: input ACE type
        :return: True if input ACE type is of type Allowed
        '''
        return getattr(ntsecuritycon, Constants.ALLOWED_ACE_TYPE) == ace_type

    @staticmethod
    def _is_proper_linux_access(path):
        '''
        Checks if given path have WRITE permission bit set for OTHERS
        and whether sticky-bits are set fro USER & GROUP.
        :param path: existing directory
        :return: True if WRITE permission bit is clearer for OTHERS
        and sticky-bits are set for USER & GROUP, otherwise False
        '''
        path_st_mode = os.stat(path).st_mode

        # check whether WRITE & EXECUTE permission bits are cleared
        w_exe_bits = path_st_mode & Constants.OTHERS_WRITE_BIT_MASK
        if w_exe_bits != 0:
            AccessManager._print_generic_access_error(f"{Constants.GENERIC_ACCESS_ERROR_MSG} '{path}'",
                f"Wrong WRITE & EXECUTE permission bits for path: {path}")
            return False

        # check whether sticky-bit is set for USER & GROUP
        sticky_bits = path_st_mode & Constants.USER_GROUP_STICKY_BIT_MASK
        if sticky_bits != Constants.USER_GROUP_STICKY_BIT_MASK:
            AccessManager._print_generic_access_error(f"{Constants.GENERIC_ACCESS_ERROR_MSG} '{path}'",
                f"Permission sticky-bit not set for USER & GROUP for path: {path}")
            return False

        return True

    @staticmethod
    def _get_all_nested(path):
        '''
        Gets list of all nested folders, files and root directory.
        :param path: root directory
        :return: list of directories as absolute paths
        '''
        yield path
        for root, dirs, files in os.walk(path):
            for dir in dirs:
                yield os.path.join(root, dir)
            for file in files:
                yield os.path.join(root, file)

    @staticmethod
    def _print_generic_access_error(error_msg, debug_msg = None):
        '''
        Prints generic error message with verbose message if given.
        :param error_msg: required error message
        :param debug_msg: verbose message (optional)
        '''
        if debug_msg is not None:
            ColorPrint.debug(debug_msg)

        ColorPrint.error(error_msg)

