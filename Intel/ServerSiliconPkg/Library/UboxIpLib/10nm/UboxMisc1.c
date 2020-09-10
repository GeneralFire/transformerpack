/** @file
.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include <Uefi.h>
#include "Include/UboxRegs.h"
#include <Library/UsraCsrLib.h>
#include "SysHost.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/RcDebugLib.h>
#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>

/**
  UBOX: Hide the requested devices on requested socket

  @param[in] SocId          - CPU Socket Node number (Socket ID)
  @param[in] BoxInst        - Box Instance, 0 based
  @param[in] BusNum         - Bus Number for devices to devhide, 0 based.
  @param[in] DevHideBitmask - Bitmask of devices to hide

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxHideUboxDevice (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    BusNum,
  IN UINT32   DevHideBitmask
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: configure MMCFG rule, such as MmcfgLengthUbox

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param MmCfgSize        - Mmcfg size

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureMmcfgRule (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgSize
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Enable/Disable current CPUBUSNO Valid bit

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param CpuBusNoProgrammed - 1: programmed; 0: not programmed

  @retval                   - EFI_UNSUPPORTED: the function not implemented
                              EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetCpuBusNoValid (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    CpuBusNoProgrammed
  )
{
  CPUBUSNO_VALID_UBOX_MISC_STRUCT CpuBusNoValid;

  //
  // Enable/Disable current CPUBUSNO Valid bit
  //
  CpuBusNoValid.Data = UsraCsrRead (SocId, BoxInst, CPUBUSNO_VALID_UBOX_MISC_REG);
  CpuBusNoValid.Bits.busno_programmed = CpuBusNoProgrammed;
  UsraCsrWrite (SocId, BoxInst, CPUBUSNO_VALID_UBOX_MISC_REG, CpuBusNoValid.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Get SBREG bar Page Table Entry Registers' flat address

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegAddress       - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetSbRegPte0RegisterFlatAddr (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINTN*   RegAddress
  )
{
  *RegAddress = UsraGetCsrRegisterAddress (SocId, BoxInst, SBREG_PTE0_UBOX_MISC_REG);

  return EFI_SUCCESS;
}

/**
  UBOX: Configure CPU BUS NO.

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuBusNo (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  )
{
  KTI_HOST_OUT              *KtiHostOutPtr;
  CPUBUSNO_UBOX_MISC_STRUCT CpuBusNo;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  //
  // Write new value for CPUBUSNO0/1
  //
  CpuBusNo.Data = 0;
  CpuBusNo.Bits.cpubusno0 = (UINT32) KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusBase;
  CpuBusNo.Bits.cpubusno1 = (UINT32) KtiHostOutPtr->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusLimit;
  CpuBusNo.Bits.segment   = KtiHostOutPtr->CpuInfo[SocId].CpuRes.PciSegment;

  UsraCsrWrite (SocId, BoxInst, CPUBUSNO_UBOX_MISC_REG, CpuBusNo.Data);

  return EFI_SUCCESS;
}