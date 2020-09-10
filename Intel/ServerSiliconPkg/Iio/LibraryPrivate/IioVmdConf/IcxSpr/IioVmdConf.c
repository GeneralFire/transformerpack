/** @file
  This file initialize VMD 2.0 for ICX|SNR|ICXD|SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/DebugLib.h>

#include <RcRegs.h>

#include <IioSetupDefinitions.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>

#include <Library/IioVmdInit.h>
#include <LibraryPrivate/IioVmdConf.h>

#include <Library/KtiApi.h>

/**
  Return TRUE if VMD is present on given stack (VMAssign register is readable)

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack

  @return True if VMD device is present, FALSE otherwise
 */
BOOLEAN
IsVmdDevicePresent (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  )
{
  VMASSIGN_IIO_VMD_STRUCT         VmAssign;

  VmAssign.Data = IioReadCpuCsr32 (IioIndex, StackIndex, VMASSIGN_IIO_VMD_REG);

  // Skip VMD configuration if config space is not valid.
  return VmAssign.Data != 0xFFFFFFFF;
}

/**
  Clear all VMD assigment - all devices are visible
 */
VOID
ClearVmdAssigning (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  )
{
  VMASSIGN_IIO_VMD_STRUCT         VmAssign;
  FN0_ASSIGN_IIO_VMD_STRUCT       FnAssign;

  //
  // Clear VmAssign
  //
  VmAssign.Bits.port_assign = 0;
  IioWriteCpuCsr32 (IioIndex, StackIndex, VMASSIGN_IIO_VMD_REG, VmAssign.Data);

  //
  // Clear all FnAssign regs
  //
  FnAssign.Bits.device = 0;
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN0_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN1_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN2_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN3_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN4_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN5_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN6_ASSIGN_IIO_VMD_REG, FnAssign.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, FN7_ASSIGN_IIO_VMD_REG, FnAssign.Data);
}


/**
  Program VMD FnAssign register of current stack for given Device/Function

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
  @param Device     - device number of the rootport
  @param Function   - function number of the rootport
  @param AssignToVmd - if TRUE assign device to VMD, unassigns otherwise
 */
VOID
AssigningDeviceToVmd (
  IN  UINT8    IioIndex,
  IN  UINT8    StackIndex,
  IN  UINT8    Device,
  IN  UINT8    Function,
  IN  BOOLEAN  AssignToVmd
  )
{
  FN0_ASSIGN_IIO_VMD_STRUCT       FnAssign;
  UINT32                          CsrRegister;

  //
  // Assign/unassign function
  //
  switch (Function) {
    case 0:
      CsrRegister = FN0_ASSIGN_IIO_VMD_REG;
      break;
    case 1:
      CsrRegister = FN1_ASSIGN_IIO_VMD_REG;
      break;
    case 2:
      CsrRegister = FN2_ASSIGN_IIO_VMD_REG;
      break;
    case 3:
      CsrRegister = FN3_ASSIGN_IIO_VMD_REG;
      break;
    case 4:
      CsrRegister = FN4_ASSIGN_IIO_VMD_REG;
      break;
    case 5:
      CsrRegister = FN5_ASSIGN_IIO_VMD_REG;
      break;
    case 6:
      CsrRegister = FN6_ASSIGN_IIO_VMD_REG;
      break;
    case 7:
      CsrRegister = FN7_ASSIGN_IIO_VMD_REG;
      break;
    default:
      DEBUG ((EFI_D_ERROR, "[IIO] VMD Invalid Function = %x\n", Function));
      return;
  } // Switch ..

  FnAssign.Data = IioReadCpuCsr32 (IioIndex, StackIndex, CsrRegister);
  if (AssignToVmd) {
    FnAssign.Bits.device |= (1 << Device);
  } else {
    FnAssign.Bits.device &= ~(1 << Device);
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, CsrRegister, FnAssign.Data);

  DEBUG ((EFI_D_ERROR, "[IIO] %a: Socket/Stack=%d/%d Dev/Fun=%X/%X FnAssign=%X\n",
      __FUNCTION__, IioIndex, StackIndex, Device, Function, FnAssign.Data));
}


/**
  Configure VMD BARs for given stack

  @param[in] IioIndex   - index of the socket
  @param[in] StackIndex - index of the stack
 */
VOID
ConfigureVmdBars (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  )
{
  EFI_STATUS                      Status;
  IIO_GLOBALS                     *IioGlobalData;

  CFGBARSZ_IIO_VMD_STRUCT         CfgBarSz;
  MEMBAR1SZ_IIO_VMD_STRUCT        MemBar1Sz;
  MEMBAR2SZ_IIO_VMD_STRUCT        MemBar2Sz;
  CFGBAR_N0_IIO_VMD_STRUCT        CfgBar;
  MEMBAR1_N0_IIO_VMD_STRUCT       MemBar1;
  MEMBAR1_N0_IIO_VMD_STRUCT       MemBar2;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get IioGlobalData. \n"));
    return;
  }

  //
  // Program CfgBarSz
  //
  CfgBarSz.Data  = IioReadCpuCsr8 (IioIndex, StackIndex, CFGBARSZ_IIO_VMD_REG);
  CfgBarSz.Bits.size = IioGlobalData->SetupData.VMDCfgBarSz[IioIndex][StackIndex];
  IioWriteCpuCsr32 (IioIndex, StackIndex, CFGBARSZ_IIO_VMD_REG, CfgBarSz.Data);

  //
  // Program MemBarSz 1/2
  //
  MemBar1Sz.Data = IioReadCpuCsr8 (IioIndex, StackIndex, MEMBAR1SZ_IIO_VMD_REG);
  MemBar1Sz.Bits.size = IioGlobalData->SetupData.VMDMemBarSz1[IioIndex][StackIndex];
  IioWriteCpuCsr32 (IioIndex, StackIndex, MEMBAR1SZ_IIO_VMD_REG, MemBar1Sz.Data);

  MemBar2Sz.Data = IioReadCpuCsr8 (IioIndex, StackIndex, MEMBAR2SZ_IIO_VMD_REG);
  MemBar2Sz.Bits.size = IioGlobalData->SetupData.VMDMemBarSz2[IioIndex][StackIndex];
  IioWriteCpuCsr32 (IioIndex, StackIndex, MEMBAR2SZ_IIO_VMD_REG, MemBar2Sz.Data);

  DEBUG ((EFI_D_INFO, "[IIO] VMD CfgBarSz:%02d MemBar1Sz:%02d MemBar2Size:%02d\n",
      CfgBarSz.Data, MemBar1Sz.Data, MemBar2Sz.Data));

  //
  // Program CfgBar attribute
  //
  CfgBar.Data = IioReadCpuCsr32 (IioIndex, StackIndex, CFGBAR_N0_IIO_VMD_REG);
  switch (IioGlobalData->SetupData.VMDCfgBarAttr[IioIndex][StackIndex]) {
    case VMD_32BIT_NONPREFETCH: // 32-bit non-prefetchable
      CfgBar.Bits.prefetchable = 0;
      CfgBar.Bits.type = 0;
      break;
    case VMD_64BIT_NONPREFETCH: // 64-bit non-prefetcheble
      CfgBar.Bits.prefetchable = 0;
      CfgBar.Bits.type = 2;
      break;
    case VMD_64BIT_PREFETCH:    // 64-bit prefetchable
      CfgBar.Bits.prefetchable = 1;
      CfgBar.Bits.type = 2;
      break;
    default:
      break;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, CFGBAR_N0_IIO_VMD_REG, CfgBar.Data);

  //
  // Program MemBar1 attribute
  //
  MemBar1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MEMBAR1_N0_IIO_VMD_REG);
  switch (IioGlobalData->SetupData.VMDMemBar1Attr[IioIndex][StackIndex]) {
    case VMD_32BIT_NONPREFETCH: // 32-bit non-prefetchable
      MemBar1.Bits.prefetchable = 0;
      MemBar1.Bits.type = 0;
      break;
    case VMD_64BIT_NONPREFETCH: // 64-bit non-prefetchable
      MemBar1.Bits.prefetchable = 0;
      MemBar1.Bits.type = 2;
      break;
    case VMD_64BIT_PREFETCH:    // 64-bit prefetchable
      MemBar1.Bits.prefetchable = 1;
      MemBar1.Bits.type = 2;
      break;
    default:
      break;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, MEMBAR1_N0_IIO_VMD_REG, MemBar1.Data);

  //
  // Program MemBar2 attribute
  //
  MemBar2.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MEMBAR2_N0_IIO_VMD_REG);
  switch (IioGlobalData->SetupData.VMDMemBar2Attr[IioIndex][StackIndex]) {
    case VMD_32BIT_NONPREFETCH: // 32-bit non-prefetchable
      MemBar2.Bits.prefetchable = 0;
      MemBar2.Bits.type = 0;
      break;
    case VMD_64BIT_NONPREFETCH: // 64-bit non-prefetchable
      MemBar2.Bits.prefetchable = 0;
      MemBar2.Bits.type = 2;
      break;
    case VMD_64BIT_PREFETCH:    // 64-bit prefetchable
      MemBar2.Bits.prefetchable = 1;
      MemBar2.Bits.type = 2;
      break;
    default:
      break;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, MEMBAR2_N0_IIO_VMD_REG, MemBar2.Data);

  DEBUG ((EFI_D_INFO, "[IIO] VMD CfgBar:%X MemBar1:%X MemBar2:%X\n", CfgBar.Data, MemBar1.Data, MemBar2.Data));
}


/**
  Program the VMD VmConfig register

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
SetVmdConfig (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  )
{
  EFI_STATUS                      Status;
  IIO_GLOBALS                     *IioGlobalData;

  VMCONFIG_IIO_VMD_STRUCT         VmConfig;
  VMLOCK_IIO_VMD_STRUCT           VmLock;
  VMCAP_IIO_VMD_STRUCT            VmCap;
  UINT16                          VmdDid;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get IioGlobalData. \n"));
    return;
  }

  // Get the device ID of VMD
  VmdDid = IioReadCpuCsr16 (IioIndex, StackIndex, DID_IIO_VMD_REG);

  //
  // Program the device ID that appears in the Device ID register and apply lock
  //
  VmConfig.Data = IioReadCpuCsr32 (IioIndex, StackIndex, VMCONFIG_IIO_VMD_REG);
  VmConfig.Bits.devid_select = VmdDid & 0x000F;

  // Read Bus Restr. capabilities
  VmCap.Data = IioReadCpuCsr32 (IioIndex, StackIndex, VMCAP_IIO_VMD_REG);
  if (VmCap.Bits.bus_restrict_cap == 1) {
    if (IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][StackIndex] > 127 ) {
      //
      // This socket uses buses > 127 => bus restr.: 0 ( VMD uses buses 1-127 )
      //
      VmConfig.Bits.bus_restrictions = 0;
    } else {
      //
      // This socket uses buses < 127 => bus restr.: 1 ( VMD uses buses 129-255 )
      //
      VmConfig.Bits.bus_restrictions = 1;
    }
    DEBUG ((DEBUG_INFO, "[Iio] VMD IioIndex=%x StackIndex=%x bus restriction=%d\n",
        IioIndex, StackIndex, VmConfig.Bits.bus_restrictions));
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, VMCONFIG_IIO_VMD_REG, VmConfig.Data);
  DEBUG ((EFI_D_INFO, "[IIO] VMD Config = %X\n", VmConfig.Data));


  VmLock.Data = IioReadCpuCsr32 (IioIndex, StackIndex, VMLOCK_IIO_VMD_REG);
  VmLock.Bits.vmdlock = 1;
  IioWriteCpuCsr32(IioIndex, StackIndex, VMLOCK_IIO_VMD_REG, VmLock.Data);
  DEBUG ((EFI_D_INFO, "[IIO] VMD Lock = %X\n", VmLock.Data));
}

/**
  Apply VMD workarounds for given stack

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
ApplyVmdWorkarounds (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  )
{
  // There are no ICX specific workarounds yet
}
