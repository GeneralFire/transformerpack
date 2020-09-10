/** @file
Interface of PSMI Policy library.

@copyright
INTEL CONFIDENTIAL
Copyright 2018 - 2018 Intel Corporation. <BR>

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

#ifndef __PSMI_POLICY_LIB_H__
#define __PSMI_POLICY_LIB_H__

typedef struct {
    UINT8  PsmiTraceRegion[5];
    UINT8  PsmiTraceBufferSizeRegion[5];
    UINT8  PsmiTraceMemTypeRegion[5];
} TRACE_INFO;

struct psmiSetup{
    UINT8  GlobalPsmiEnable;
    UINT8  PsmiTrace[MAX_SOCKET];
    UINT8  PsmiHandlerSize[MAX_SOCKET];
    TRACE_INFO trace[MAX_SOCKET];
};

#define EFI_PSMI_POLICY_DATA_HOB_GUID \
    { \
  0x42ed3781, 0x19a, 0x28ae, { 0x16, 0x3b, 0x9a, 0x50, 0x7, 0xf, 0x93, 0x6b }\
    }

extern EFI_GUID gEfiPsmiPolicyDataHobGuid;


/**
Set PSMI policy info into system.

@param   PsmiPolicy       Pointer to Psmi Policy Info.
@param   DataSize         The size of PSMI Policy.

@retval  EFI_SUCCESS  Set PSMI policy success.
@retval EFI_OUT_OF_RESOURCES Out of resource;

**/
EFI_STATUS
EFIAPI
SetPsmiPolicy (
  IN VOID *PsmiPolicy,
  IN UINTN DataSize
  );


/**
Get PSMI policy info into system.


@retval  pointer to psmiSetup  Get PSMI policy success.
@retval  NULL                        Can't find the PSMI Policy.


**/
struct psmiSetup *
EFIAPI
GetPsmiPolicy (
  );

#endif
