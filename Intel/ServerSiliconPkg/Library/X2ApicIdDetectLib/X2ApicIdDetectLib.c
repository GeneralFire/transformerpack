/** @file

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

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Cpu/CpuCoreRegs.h>
#ifndef IA32
#include <Library/UefiBootServicesTableLib.h>
#endif //IA32
#include <Library/DebugLib.h>
#include <Library/UsraAccessApi.h>
#include <SysHost.h>
#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include "X2ApicIdDetectLib.h"
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Register/Cpuid.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Detect if system needs to use x2APIC ID

  @param[in] *Host  - Pointer to syshost struture, only need the parm after KTI Init in PEI phase.

  @retval TRUE  - x2APIC ID
  @retval FALSE - Legacy APIC ID

**/
BOOLEAN
X2ApicIdDetect (
  IN PSYSHOST  Host
  )
{
  UINT32                      MaxSocketId;
  UINT32                      ApicId;
  UINT32                      IntraPackageIdBits;
  UINT32                      MaxThreadsPerPkg;
  UINT32                      SocketPresentBitMap;
#ifndef IA32
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
#else //IA32
  UINT32                      UboxMiscBus;
  UINT32                      UboxMiscDev = 0x00;
  UINT32                      UboxMiscFun = 0x00;
  UINT32                      BiosScratchpad2;
  USRA_ADDRESS                Address;
  UINT32                      MsrCpuBusNo;
  UINT32                      RegEax;
  UINT32                      FamilyId;
  UINT32                      ModelId;
  UINT32                      ExtendedFamilyId;
  UINT32                      ExtendedModelId;
#endif //IA32

#ifdef IA32
  if (Host != NULL) {
    SocketPresentBitMap = GetSocketPresentBitMap ();
  } else {
    // Get CPUID(1).EAX
    //
    AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

    //
    // The Extended Family ID needs to be examined only when the Family ID is 0FH.
    // If Family ID is 0FH, Displayed Family ID = Family ID + Extended Family ID.
    // Otherwise, Displayed Family ID is Family ID
    //
    FamilyId = BitFieldRead32 (RegEax, 8, 11);
    if (FamilyId == 0x0f) {
      ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
      FamilyId += ExtendedFamilyId;
    }

    //
    // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
    // If Family ID is 06H or 0FH, Displayed Model ID = Model ID + (Extended Model ID << 4).
    // Otherwise, Displayed Model ID is Model ID.
    //
    ModelId = BitFieldRead32 (RegEax, 4, 7);
    if (FamilyId == 0x06 || FamilyId == 0x0f) {
      ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
      ModelId += (ExtendedModelId << 4);
    }

    if (IS_ICELAKE_SERVER_PROC (FamilyId, ModelId) ||
        IS_ICELAKE_D_SERVER_PROC(FamilyId, ModelId) ||
        IS_SNOWRIDGE_SERVER_PROC(FamilyId, ModelId) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {

      MsrCpuBusNo = AsmReadMsr32 (MSR_CPU_BUSNUMBER);
    } else {
      MsrCpuBusNo = AsmReadMsr32 (SKX_MSR_CPU_BUSNUMBER);
    }

    UboxMiscBus = MsrCpuBusNo & 0xff;
    BiosScratchpad2 = (UINT32) (BIOSSCRATCHPAD2_UBOX_MISC_REG & 0xffff);

    if (IS_ICELAKE_SERVER_PROC (FamilyId, ModelId) ||
        IS_ICELAKE_D_SERVER_PROC(FamilyId, ModelId) ||
        IS_SNOWRIDGE_SERVER_PROC(FamilyId, ModelId)) {
      UboxMiscDev = ICX_UBOX_MISC_DEV;
      UboxMiscFun = ICX_UBOX_MISC_FUN;
    } else if (IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) {
      UboxMiscDev = SKX_UBOX_MISC_DEV;
      UboxMiscFun = SKX_UBOX_MISC_FUN;
    }

    //
    //Get socketPresentBitMap from Sticky BIOS Scratchpad2, KTI saves socketPresentBitMap in Scratchpad2.
    //
    if (IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId) || IS_ICELAKE_SERVER_PROC (FamilyId, ModelId) ||
        IS_ICELAKE_D_SERVER_PROC(FamilyId, ModelId) || IS_SNOWRIDGE_SERVER_PROC(FamilyId, ModelId)) {
      USRA_PCIE_ADDRESS (Address, UsraWidth32, UboxMiscBus, UboxMiscDev, UboxMiscFun, BiosScratchpad2);
      RegisterRead(&Address, &SocketPresentBitMap);

      if (SocketPresentBitMap == 0x0) {
        DEBUG((DEBUG_INFO, "socketPresentBitMap in Scratchpad2 is not ready\n"));
        return FALSE;
      }
      SocketPresentBitMap = (SocketPresentBitMap & 0xff);
    } else {
      SocketPresentBitMap = 0x01;
    }
  }
#else  //IA32
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  SocketPresentBitMap = CpuCsrAccessVarPtr->socketPresentBitMap;
#endif //IA32
  //
  // CPUID.(EAX=0Bh, ECX=1h):EAX[4:0] - shift bits to get Socket level of APIC ID
  //
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &IntraPackageIdBits, &MaxThreadsPerPkg, NULL, NULL);
  IntraPackageIdBits = ((CPUID_EXTENDED_TOPOLOGY_EAX*)&IntraPackageIdBits)->Bits.ApicIdShift;

  //
  // NBSP Max APIC ID = MaxSocketId << Socket level bitshift
  //
  MaxSocketId = (UINT32)HighBitSet32(SocketPresentBitMap);
  ApicId = MaxSocketId << IntraPackageIdBits;

  DEBUG((DEBUG_INFO, "Detected NBSP Max APIC ID = 0x%x \n", ApicId));

  if ((ApicId > 0xff) || ((SocketPresentBitMap == 1) && (MaxThreadsPerPkg > 255))) {
    DEBUG((DEBUG_ERROR, "System is detected to require X2APIC Support!\n"));
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      return FALSE; //force return FALSE as this caller code is too earlier that some scratchpad2 register is not set correclty. This routine need to be refactor later.
    }
    return TRUE;    //X2APIC ID, APID length > 8 bits
   
  } else {
    DEBUG((DEBUG_INFO, "Legacy APIC ID detected!\n"));
    return FALSE;   //Legacy ID, APID length <= 8 bits
  }
}
