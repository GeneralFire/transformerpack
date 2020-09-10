/** @file
  Data format for Universal Data Structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef   __SOCKET_COMMONRC_CONFIG_DATA_H__
#define   __SOCKET_COMMONRC_CONFIG_DATA_H__


#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

extern EFI_GUID gEfiSocketCommonRcVariableGuid;
#define SOCKET_COMMONRC_CONFIGURATION_NAME L"SocketCommonRcConfig"

#define UMABASEDCLUSTERING_DISABLED   0
#define UMABASEDCLUSTERING_HEMISPHERE 2
#define UMABASEDCLUSTERING_QUADRANT   4

#pragma pack(1)
typedef struct {
    //
    //  Common Section of RC
    //
    UINT32   MmiohBase;
    UINT16   MmiohSize;
    UINT8    MmcfgBase;
    UINT8    MmcfgSize;
    UINT8    IsocEn;
    UINT8    NumaEn;
    UINT8    UmaBasedClustering;
    UINT8    MirrorMode;
    UINT8    CpuType;
    UINT8    CpuChop;
    UINT8    X2ApicForceEn;
    UINT8    SystemRasType;
    UINT8    NumCpus;
    UINT8    DfxRstCplBitsEn;
    UINT8    DfxB2PMailboxCmdEnMask[MAX_B2P_MAILBOX_GROUPS];
    UINT8    OcCap;
    UINT8    IssMaxLevel;
    UINT8    DcpmmEnable;
    UINT8    DfxB2PErrAssertAtCommand;
    UINT8    HbmSku;
    UINT8    PbfCapableSystem;
    UINT8    DcuRtlWaEn;
    UINT8    SstCpCapableSystem;
} SOCKET_COMMONRC_CONFIGURATION;
#pragma pack()

#endif


