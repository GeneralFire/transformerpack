/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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
#ifndef _SMBIOS_STRING_H_
#define _SMBIOS_STRING_H_

#include <Library/PlatMemTopologyLib.h>

#define DIMMINFO_SIZE          0x200
#define STRING_BUFFER_SIZE     14


/**
  Set SMBIOS 16, 17 Handler in MemTopology.

  @param  IN OUT MemTopology  MEM_TOPOLOGY instance

  @retval none

**/
VOID
EFIAPI
SetMemTopoSmbios (
  IN OUT MEM_TOPOLOGY  *MemTopology
  );
#endif
