/** @file Mesh2MemPerf.c
This file contains wrapper functions for auto-generated Mesh2Mem related functions
by RegFlow tool.

@copyright
INTEL CONFIDENTIAL
Copyright 2019 Intel Corporation. <BR>

The source code contained or described herein and all documents related to the
source code ("Material") are owned by Intel Corporation or its suppliers or
licensors. Title to the Material remains with Intel Corporation or its suppliers
and licensors. The Material may contain trade secrets and proprietary    and
confidential information of Intel Corporation and its suppliers and licensors,
and is protected by worldwide copyright and trade secret laws and treaty
provisions. No part of the Material may be used, copied, reproduced, modified,
published, uploaded, posted, transmitted, distributed, or disclosed in any way
without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual
property right is granted to or conferred upon you by disclosure or delivery
of the Materials, either expressly, by implication, inducement, estoppel or
otherwise. Any license under such intellectual property rights must be
express and approved by Intel in writing.

Unless otherwise agreed by Intel in writing, you may not remove or alter
this notice or any other notice embedded in Materials by Intel or
Intel's suppliers or licensors in any way.
**/

#include <Library/Mesh2MemIpLib.h>


/**

  This is a wrapper routine that calls auto-generated routine to configure QoS
  setting based on Recipe Profile setup knob value.

  @param[in] Socket   Processor to initialize

  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
EFIAPI
NvmdimmQosPerfSetting (
  IN UINT8    Socket
  )
{
  return EFI_UNSUPPORTED;
}
