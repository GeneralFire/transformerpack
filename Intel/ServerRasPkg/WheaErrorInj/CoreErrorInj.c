/** @file
  This file contaisn the functionality for the ME Error Injection.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/RasDebugLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <Protocol/RasPolicy.h>
#include <Library/PlatPolicyLib.h>
#include <Library/RasSiliconLib.h>

#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#include <SiliconSetting.h>

#include "WheaErrorInj.h"

#include <Cpu/Msr/CommonMsr.h>
#include <Cpu/Msr/IcxMsr.h>
#include <Library/ChaIpLib/Include/ChaRegs.h>

extern  MEM_TOPOLOGY                                  *mMemTopologyData;
extern  WHEA_EINJ_PARAM_BUFFER                        *mEinjParam ;
extern  WHEA_EINJ_TRIGGER_ACTION_TABLE                *mEinjAction;

/**

    As part of LLC error injection. BIOS needs to disable cache prefetch. (DCU IP, DCU Streamer, MLC spatial, MLC streamer).
    Also, start a PSMI event to start quiesce handshake.
    Quiesce is needed to ensure that DMA/DCA accesses do not interfere with the error injection.
    For more information see MCA HAS LLC Error injection and RAS HAS for Quiesce flow.

    @param node - Node Id

    @retval None

**/
VOID
DisablePsmiAndPrefetch (
    UINT8 *node
)
{
  //
  // Disable PSMI
  //
  MSR_QUIESCE_CTL2_REGISTER QuiesceCtl2Reg;
  ICX_MSR_MISC_FEATURE_CONTROL_REGISTER IcxMiscFeatureControlReg;

  QuiesceCtl2Reg.Uint64 = 0;
  QuiesceCtl2Reg.Bits.SignalHandshake = 1;
  AsmWriteMsr64 (MSR_QUIESCE_CTL2, QuiesceCtl2Reg.Uint64);

  //
  //  Disable Prefetch
  //
  IcxMiscFeatureControlReg.Uint64 = AsmReadMsr64 (ICX_MSR_MISC_FEATURE_CONTROL);
  IcxMiscFeatureControlReg.Bits.MlcStreamerPrefetchDisable = 1;
  IcxMiscFeatureControlReg.Bits.MlcSpatialPrefetchDisable = 1;
  IcxMiscFeatureControlReg.Bits.DcuStreamerPrefetchDisable = 1;
  IcxMiscFeatureControlReg.Bits.DcuIpPrefetchDisable = 1;
  AsmWriteMsr64 (ICX_MSR_MISC_FEATURE_CONTROL, IcxMiscFeatureControlReg.Uint64);
}


/**
  Setup the SADDBGMM0/1/2 registers for the LLC error injection.

  @param EinjParam - Error Injection arguments.

  @retval None
**/
VOID
SetupLlcInjRegister(
  WHEA_EINJ_PARAM_BUFFER *EinjParam)
{
  UINT8   Cha;
  UINT8   SocketId = 0;
  UINT32  Data32;
  UINT32  InjAddrValue;
  UINT64  PhyAddr;

  LLCERRINJ_CHA_MISC_STRUCT     LLCErrInj;
  MSR_QUIESCE_CTL1_REGISTER     QuiesceCtl1Reg;

  PhyAddr      = EinjParam->EinjAddr;
  SocketId     = EinjParam->Skt;
  InjAddrValue = INJECTION_ADDRESS_VALUE;

  //
  // Bring the cache line to M state
  //
  Data32  = *(volatile  UINT32 *)(UINTN)(PhyAddr);
  *(volatile  UINT32 *) (UINTN) (PhyAddr) = 0x12345678;
  AsmFlushCacheLine ((VOID*)(UINTN)PhyAddr);

  //
  // Memory Fence
  //
  MemoryFence();

  //
  // Turn On Quiesce
  //
  QuiesceCtl1Reg.Uint64 = AsmReadMsr64(MSR_QUIESCE_CTL1);
  if (QuiesceCtl1Reg.Bits.QuiesceCapability == 1) {
    QuiesceCtl1Reg.Bits.Quiesce = 1;
    AsmWriteMsr64 (MSR_QUIESCE_CTL1, QuiesceCtl1Reg.Uint64);
  }

  Data32  = *(volatile  UINT32 *)(UINTN)(PhyAddr);
  *(volatile  UINT32 *)(UINTN)(PhyAddr) = InjAddrValue;

  //
  // Turn Off Quiesce
  //
  if (QuiesceCtl1Reg.Bits.QuiesceCapability == 1) {
    QuiesceCtl1Reg.Bits.Quiesce = 0;
    QuiesceCtl1Reg.Bits.Unquiesce = 1;
    AsmWriteMsr64 (MSR_QUIESCE_CTL1, QuiesceCtl1Reg.Uint64);
  }


  for (Cha = 0; Cha < mMemTopologyData->Socket[SocketId].TotalCha; Cha ++) {
    if ((mMemTopologyData->Socket[SocketId].LLCMap & (LShiftU64 (1,Cha))) == 0) {
      continue;
    }
    //
    // Set ErrInjMode bit to inject once only
    // Set inj3bdataerr bit for triple bit data error in LLC when Address match happens
    //
    LLCErrInj.Data = ReadCpuCsr( SocketId, Cha, LLCERRINJ_CHA_MISC_REG );
    LLCErrInj.Bits.errinjmode = 01;
    LLCErrInj.Bits.inj3bdataerr = 01;
    RAS_DEBUG ((LEVEL_REG,"LLC Debug data %x\n",LLCErrInj.Data));
    WriteCpuCsr (SocketId, Cha, LLCERRINJ_CHA_MISC_REG, LLCErrInj.Data);

    //
    // Set address Match and Mask bits
    //
    SetLlcErrorinjection (SocketId, Cha, PhyAddr);
  }
}


VOID
SetupLlcInjRegisterAp(
  IN      UINT8     *Buffer) //Empty Buffer
{
  SetupLlcInjRegister(mEinjParam);
}

/**

Routine Description: This function injects LLC error in the cache

    @param None

    @retval none

**/
VOID
InjectLLCError(
    UINT8 SocketId
)
{

  //UINT16   thread = 0;
  UINT64  PhyAddr;
  mEinjParam->Skt = SocketId;

  // Unlock the error injection logic
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, UnLockInjLogic, NULL);
  RAS_DEBUG ((LEVEL_REG, "InjectLLCError :  Unlock Completed \n"));
  PhyAddr = mEinjParam->EinjAddr;


  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, TRUE, DisablePsmiAndPrefetch, NULL);

  RAS_DEBUG ((LEVEL_REG, "InjectLLCError :  EWB Inj Regs setup on Skt %d start : \n", SocketId));

  if(SocketId == 0) {
    SetupLlcInjRegister(mEinjParam);
  } else {
    MultiProcessorExecute (OneApPerPackage, SocketId, ALL_CORE, TRUE, SetupLlcInjRegisterAp, NULL);
  }

  RAS_DEBUG((LEVEL_REG,"Err inj bits set \n"));
  mEinjAction->Trigger0.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
  mEinjAction->Trigger0.Instruction                      = WHEA_INSTRUCTION_CFLUSH;    // CLFLUSH
  mEinjAction->Trigger0.Flags                            = 0;
  mEinjAction->Trigger0.Reserved0                        = 00;
  mEinjAction->Trigger0.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
  mEinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
  mEinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
  mEinjAction->Trigger0.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
  mEinjAction->Trigger0.RegisterRegion.Address           = PhyAddr;
  mEinjAction->Trigger0.Value                            = 0x12345678;
  mEinjAction->Trigger0.Mask                             = 0xffffffff;
}

