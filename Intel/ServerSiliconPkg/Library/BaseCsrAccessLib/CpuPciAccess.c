/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <KtiMisc.h>
#include <Chip/Include/CpuPciAccess.h>
#include <UncoreCommonIncludes.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CpuAccessLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CsrSbPortIdLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PerformanceTrackerLib.h>

#define LEGACY_CAPID4_PCU_FUN3_REG                     PCI_REG_ADDR(1,30,3,0x94)

/**

    Count the CSR accesses.

    @param - None

**/
VOID
CpuCsrAccessCount (
  VOID
  )
{

#if defined(IA32)

    CountTrackingData (PTC_PCI_ACCESS, 1);

#endif //#if defined (IA32) || defined (SIM_BUILD)

  return;

} // CpuCsrAccessCount

/**

  Get MMCFG base and limit

  @retval 32-bit MMCFG base address

**/
UINT32
GetMmcfgAddress (
  VOID
  )
{
  UINT32 CsrLegacyAddr = 0;
  UINT32 Data32;
  UINT64_STRUCT  MsrData64;

  //
  // Pick the right MMCFG_RULE CSR address accordign to current CPU type
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    Data32 = SKX_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    Data32 = SPR_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG;
  } else {
    Data32 = ICX_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG;
  }

  //
  // Use CF8/CFC IO port  to read legacy socket's CHA MMCFG_RULE CSR
  //

  // Get current Bus# from MSR 0x300 or 0x128
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    MsrData64 = ReadMsr (SKX_MSR_CPU_BUSNUMBER);
  } else {
    MsrData64 = ReadMsr (MSR_CPU_BUSNUMBER);
  }

  // Use BusU1 for CHA
  CsrLegacyAddr =  (Data32 & 0xff00ffff) | ((MsrData64.lo & 0x0000ff00) << 8);
  //
  // Read CSR via CF8/CFC IO
  //
#if defined(IA32)
  OutPort32 (0x0cf8, (0x80000000 | CsrLegacyAddr));
  Data32 = InPort32 (0x0cfc);
#else
  IoWrite32 (0x0cf8, (0x80000000 | CsrLegacyAddr));
  Data32 = IoRead32 (0x0cfc);
#endif

  Data32 &= 0xfc000000;              // Mask Bits[25:0]
  return Data32;
}

/**

  Get CPU socket id for the system boot strap processor
  This function only works on SBSP prior to KTIRC completion.

  @retval system boot strap processor socket ID

**/
UINT8
GetSbspSktId (
  VOID
  )
{
  UINT32 sadTarget;
  UINT32 sadControl;
  UINT8 legacyPchTarget;
  UINT64_STRUCT  MsrData64;
#if defined(IA32)
  UINT8 BusNo;
#else //defined (IA32) || defined (SIM_BUILD)
  UINT32  Data32;
#endif //defined (IA32) || defined (SIM_BUILD)

#if defined(IA32)
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    MsrData64 = ReadMsr (SKX_MSR_CPU_BUSNUMBER);
  } else {
    MsrData64 = ReadMsr (MSR_CPU_BUSNUMBER);
  }
  //
  // Get current Bus# from MSR 0x300 or 0x128
  //
  BusNo = (MsrData64.lo & 0x0000FF00) >> 8;

  OutPort32 (0x0cf8, LEGACY_CSR_SAD_TARGET(BusNo));   // SAD_TARGET
  sadTarget = InPort32 (0x0cfc);
  OutPort32 (0x0cf8, LEGACY_CSR_SAD_CONTROL(BusNo));  // SAD_CONTROL
  sadControl = InPort32 (0x0cfc);
#else
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    IoWrite32(0x0cf8, LEGACY_CSR_CPUBUSNO_UBOX);         // only used for SKX CPUBUSNO_UBOX in DXE phase
    Data32 = IoRead32 (0x0cfc);
    Data32 = (Data32 >> 8) & 0xFF;
  } else {
    MsrData64 = ReadMsr (MSR_CPU_BUSNUMBER);
    Data32 = (MsrData64.lo >> 8) & 0xFF;
  }
  IoWrite32 (0x0cf8, LEGACY_CSR_SAD_TARGET(Data32));
  sadTarget = IoRead32 (0x0cfc);
  IoWrite32 (0x0cf8, LEGACY_CSR_SAD_CONTROL(Data32));
  sadControl = IoRead32 (0x0cfc);
#endif
  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    legacyPchTarget = (UINT8)((sadTarget >> 4) & 0xF);
    //
    // If LegacyPchTarget[3] = 1 that means this is for local so SADCONTROL[2:0] = NodeID else for remote so SADTARGET[6:4] = NodeId
    //
    if (legacyPchTarget & BIT3) {
      return (sadControl & 0x7);
    }
  } else {
    legacyPchTarget = (UINT8)((sadTarget >> 17) & 0x1F);
    //
    // If LegacyPchTarget[4] = 1 that means this is for local so SADCONTROL[3:0] = NodeID
    //
    if (legacyPchTarget & BIT4) {
      return (sadControl & 0xF);
    }
  }

  //
  // We choose SBSP based on legacy PCH location.   This can only be determined via register on the socket with the
  // legacy_pch prior to KTIRC programming this field on non-SBSP sockets.  Assert if this function  called from non-sbsp prior
  // to Host structure available
  //
  CpuDeadLoop ();
  return 0;
}

/**

  Enable the CSR address cache

**/
VOID
EnableCsrAddressCache (
  VOID
)
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
  CpuCsrAccessVar->CsrCachingEnable  = 1;
  CpuCsrAccessVar->LastCsrAddress[0] = 0;
  CpuCsrAccessVar->LastCsrAddress[1] = 0;
}

/**

  Disable the CSR address cache

**/
VOID
DisableCsrAddressCache (
  VOID
  )
{
  GetSysCpuCsrAccessVar()->CsrCachingEnable = 0;
}