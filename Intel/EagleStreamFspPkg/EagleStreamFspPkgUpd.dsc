## @file
# VPD DSC file of 10nmSoc FSP project.
# This package provides 10nmSoc FSP specific modules.
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2019 Intel Corporation. <BR>
#
# The source code contained or described herein and all documents related to the
# source code ("Material") are owned by Intel Corporation or its suppliers or
# licensors. Title to the Material remains with Intel Corporation or its suppliers
# and licensors. The Material may contain trade secrets and proprietary    and
# confidential information of Intel Corporation and its suppliers and licensors,
# and is protected by worldwide copyright and trade secret laws and treaty
# provisions. No part of the Material may be used, copied, reproduced, modified,
# published, uploaded, posted, transmitted, distributed, or disclosed in any way
# without Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or delivery
# of the Materials, either expressly, by implication, inducement, estoppel or
# otherwise. Any license under such intellectual property rights must be
# express and approved by Intel in writing.
#
# Unless otherwise agreed by Intel in writing, you may not remove or alter
# this notice or any other notice embedded in Materials by Intel or
# Intel's suppliers or licensors in any way.
##

[Defines]
  DEFINE FSP_PKG                 = EagleStreamFspPkg
  #
  # UPD tool definition
  #
  FSP_T_UPD_TOOL_GUID            = 34686CA3-34F9-4901-B82A-BA630F0714C6
  FSP_M_UPD_TOOL_GUID            = 39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
  FSP_S_UPD_TOOL_GUID            = CAE3605B-5B34-4C85-B3D7-27D54273C40F
  FSP_T_UPD_FFS_GUID             = 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA
  FSP_M_UPD_FFS_GUID             = D5B86AEA-6AF7-40D4-8014-982301BC3D89
  FSP_S_UPD_FFS_GUID             = E3CD9B18-998C-4F76-B65E-98B154E5446F

  UPD_TOOL_GUID                  = 8C3D856A-9BE6-468E-850A-24F7A8D38E08

[PcdsDynamicVpd.Upd]
  #
  # This section is not used by the normal build process
  # Howerver, FSP will use dedicated tool to handle it and generate a
  # VPD simliar binary block (User Configuration Data). This block will
  # be accessed through a generated data structure directly rather than
  # PCD services. This is for size consideration.
  # Format:
  #   gPlatformFspPkgTokenSpaceGuid.Updxxxxxxxxxxxxn        | OFFSET | LENGTH | VALUE
  # Only simple data type is supported
  #

  # offset 0000 ~ 00B0
  !include $(FSP_PKG)/Upd/FsptUpd.dsc

  # offset 0000 ~ 06AE
  !include $(FSP_PKG)/Upd/FspmUpd.dsc

  # offset 0000 ~ 0D10
  !include $(FSP_PKG)/Upd/FspsUpd.dsc