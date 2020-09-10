/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#ifndef _CRASH_LOG_PCH_LIB_H_
#define _CRASH_LOG_PCH_LIB_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>
#include <Guid/CrashLog.h>

#pragma pack(1)

//
// Structure of PCH CrashLog Header
//
typedef union {
  struct {
    UINT32 CrashlogCapability    : 1;
    UINT32 DisableCommand        : 1;
    UINT32 DiscoveryMech         : 2;
    UINT32 ManualTriggerSupp     : 1;
    UINT32 ClearCommand          : 1;
    UINT32 TriggerOnAllReset     : 1;
    UINT32 ReARMSupported        : 1;
    UINT32 ColdResetReqSupported : 1;
    UINT32 Reserved              : 18;
    UINT32 ColdResetRequest      : 1;
    UINT32 CrashlogInProgress    : 1;
    UINT32 TriggerARMStatus      : 1;
    UINT32 TriggerOnAllResets    : 1;
    UINT32 CrashLogDisabled      : 1;

    UINT16 PchSpecificOrReserved : 16;
    UINT16 DescriptorTableOffset : 16;
  } Fields;
  UINT32 Data[2];
} PCH_CRASHLOG_DISCOVERY_HEADER;

//
// Structure of PCH CrashLog Descriptor Table
//
typedef union {
  struct {
    UINT16 RegionOffset;
    UINT16 RegionSize;
  } Fields;
  UINT32 Data;
} PCH_CRASHLOG_DESCRIPTOR_ENTRY;

typedef struct {
  UINT32                        NumOfRegions;
  PCH_CRASHLOG_DESCRIPTOR_ENTRY Entry[MAX_PCH_CRASHLOG_REGIONS];
} PCH_CRASHLOG_DESCRIPTOR_TABLE;

#pragma pack()

/**
  Get PCH CrashLog Discovery Header

  @param[OUT] CrashLogDiscHdr   Structure of CrashLog Discovery Header

  @retval EFI_UNSUPPORTED          Get PCH Crashlog discovery header fail
  @retval EFI_SUCCESS              PCH Crashlog discovery header found
**/
EFI_STATUS
GetPchCrashLogDiscHdr (
  OUT PCH_CRASHLOG_DISCOVERY_HEADER           *CrashLogDiscHdr
  );

/**
  This function discovers PCH crashLog region

  @param[out] PchCrashLogRecRegs  - Stucture of PCH CrashLog Regions

  @retval EFI_UNSUPPORTED         PCH CrashLog is not supported
  @retval EFI_SUCCESS             PCH CrashLog is supported
**/
EFI_STATUS
PchCrashLogDiscovery (
  OUT PCH_CRASHLOG_RECORD_REGIONS_STRUCT  *PchCrashLogRecRegs
  );

/**
  This function clears cpu crash log

  @retval EFI_SUCCESS

**/
EFI_STATUS
PchCrashLogClear (
  VOID
  );

/**
  This function re-arm PCH crash log

  @retval EFI_SUCCESS

**/
EFI_STATUS
PchCrashLogReArm (
  VOID
  );

/**
  This function enable/disables PCH crash log

  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
PchCrashLogEnableDisable (
  BOOLEAN     Enable
  );

/**
  This function enable/disables PCH crash log collect on all resets

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
PchCrashLogOnAllReset (
  VOID
  );

/**
  Check if PMC SSRAM device disabled

  @retval FALSE - device not exist
  @retval TRUE -  device exist
**/
BOOLEAN
IsPmcSsramDeviceDisabled (
  VOID
  );

/**
  Init PMC SSRAM MMIO Bar

  @param[in] SavedStsCmd               - the saved value of PMC SSRAM device status and command register

  @retval FALSE - PMC SSRAM MMIO Bar already asigned
  @retval TRUE -  PMC SSRAM MMIO Bar asigned by this routine
**/
BOOLEAN
InitPmcSsramMmioBar (
  OUT UINT16              *SavedStsCmd
  );

/**
  Clear PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd               - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
VOID
ClearPmcSsramMmioBar (
  IN UINT16                     SavedStsCmd,
  IN BOOLEAN                    ClearBar
  );

/**
  Clear PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd               - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
UINT32
GetPmcSsramMmioBar (
  VOID
  );
#endif //_CRASH_LOG_PCH_LIB_H_

