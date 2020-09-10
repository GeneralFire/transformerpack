/** @file
  Cpu Initialization Common Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "CpuInit.h"
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <ReferenceCodeFatalErrors.h>
/**

  Write a CONFIG_TDP subcommand to pcode
  Return data returned from command (if any)
   and status code

  @param socket:              CPU socket ID
  @param SubCommand:          Subcommand of CONFIG_TDP to send
  @param CmdBits:             Optional data for command
  @param *TempData32:         Pointer to data read, if any

  @retval UINT32:             Status from command write
                              (not EFI_STATUS format)

**/
UINT32
SendConfigTdpSubCommand (
  IN      UINT8   Socket,
  IN      UINT8   SubCommand,
  IN      UINT32  CmdBits,
  IN OUT  UINT32  *TempData32
  )
{
  UINT32  PcodeMailboxStatus = 1;
  UINT32  MailboxData;
  B2P_SEND_CONFIG_TDP_SUBCOMMAND MailboxCmd;

  MailboxData = CmdBits;
  MailboxCmd.Data = 0;
  MailboxCmd.Bits.Command = MAILBOX_BIOS_CMD_CONFIG_TDP;
  MailboxCmd.Bits.SubCommand = SubCommand;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MailboxCmd.Data,
                         MAILBOX_BIOS_CMD_ZERO,
                         &MailboxData,
                         0,
                         0
                         );
  if ((PcodeMailboxStatus == 0) && (TempData32 != NULL)) {
    *TempData32 = MailboxData;
  }

  return PcodeMailboxStatus;
}

/**
  Active PBF.

**/
VOID
ActivePbf (
  VOID
  )
{
  UINT8       Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent(Socket)) {
      SetB2pConfigTdpControlPbfEnable (Socket);
    }
  }
}

/**
  Get PBF system level capabilty.

  @param ConfLevelInfoPtr   Pointer to CPU_CONFIG_LEVEL_INFO Structure.

  @return PBF system level capability.

**/
UINT8
GetPbfSystemStatus (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  )
{
  UINT8       Level;
  UINT8       Socket;
  UINT8       PbfCapableSystem;

  PbfCapableSystem = 1;

  //
  // [NOTE]: Current PBF only supports level 0.
  // This is for future support.
  // Level = ConfLevelInfoPtr->CurrentTdpLevel[Socket];
  //
  Level = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent(Socket)) {
      if (ConfLevelInfoPtr->PbfCapable[Socket][Level] == 0) {
        PbfCapableSystem = 0;
      }
    }
  }

  return PbfCapableSystem;
}

/**

  Find MAX Ratio for one socket.

  @param Socket             CPU socket ID.
  @param ConfLevelInfoPtr   Pointer to CPU_CONFIG_LEVEL_INFO Structure.
  @param IssLevel           ISS level.
  @param AvxLevel           AVX level.

  @retval MaxRatio

**/
UINT8
FindP1Ratio (
  IN UINT8                  Socket,
  IN CPU_CONFIG_LEVEL_INFO  *ConfLevelInfoPtr,
  IN UINT8                  IssLevel,
  IN UINT8                  AvxLevel
  )
{
  UINT8     P1Ratio;

  P1Ratio = 0;

  switch (AvxLevel) {
    case 1:
      P1Ratio = ConfLevelInfoPtr->Avx2P1Ratio[Socket][IssLevel];
      break;
    case 2:
      P1Ratio = ConfLevelInfoPtr->Avx3P1Ratio[Socket][IssLevel];
      break;
    default:
      P1Ratio = ConfLevelInfoPtr->SseP1Ratio[Socket][IssLevel];
      break;
  }

  //
  // Code will be here when GET_P1_INFO B2P mailbox command got
  // error or 0 data in GetB2PFuseP1Ratio().
  //
  if (P1Ratio == 0) {
    if (IssLevel > 0) {
      //
      // ISS
      //
      P1Ratio = ConfLevelInfoPtr->TdpRatio[Socket][IssLevel];
    } else if (AvxLevel > 0) {
      //
      // AVX
      //
      P1Ratio = ConfLevelInfoPtr->TdpRatio[Socket][AvxLevel];
    } else {
      //
      // Both IssLevel and AvxLevel are 0.
      // Max Non-Turbo
      //
      P1Ratio = ConfLevelInfoPtr->MaxCoreToBusRatio[Socket];
    }
  }

  return P1Ratio;
}

/**

  Find Common MAX Ratio for system (all sockets).

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval CommonMaxRatio

**/
UINT8
FindCommonP1Ratio (
  IN CPU_CONFIG_LEVEL_INFO  *ConfLevelInfoPtr
  )
{

  UINT8         Socket;
  UINT8         AvxLevel;
  UINT8         IssLevel;
  UINT8         CommonMaxRatio;
  UINT8         MaxRatio;

  IssLevel = ConfLevelInfoPtr->IssSystemStatus;
  AvxLevel = ConfLevelInfoPtr->AvxSystemStatus;

  CommonMaxRatio = 0xFF;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    MaxRatio = FindP1Ratio (Socket, ConfLevelInfoPtr, IssLevel, AvxLevel);

    if (CommonMaxRatio > MaxRatio) {
      CommonMaxRatio = MaxRatio;
    }
  }

  return CommonMaxRatio;

}

/**

  Get Cpu Nvram Data
  @Result: Point to the Cpu Nvram data;

 **/
CPU_NVRAM *
GetCpuNvramData(
  )
{
  VOID* GuidHob;
  GuidHob = GetFirstGuidHob (&gEfiCpuNvramDataGuid);
  if (GuidHob != NULL) {
    return GET_GUID_HOB_DATA(GuidHob);
  }

  return NULL;
}


/**

  Clear specific bit in 64-bit mask

  @param  *BitMask:     64-bit mask of bits
  @param  BitPosition:  Position of bit to clear

  @retval VOID

**/
VOID
ClearBit64 (
  IN OUT   UINT64  *BitMask,
  IN CONST UINT8        BitPosition
  )
{
  if (BitMask != NULL) {
    if (BitPosition < 64)
    *BitMask &= (UINT64) (~ LShiftU64 (1, BitPosition));
  }

  return;
}

/**

  Convert a mask of bits to a count of bits

  @param MaskToCheck: 64-bit mask of bits

  @retval BitCount:   8-bit count of bits in input mask

**/
UINT8
CountMaskBits64 (
  IN CONST UINT64 MaskToCheck
  )
{
  UINT8 i;
  UINT8 BitCount;

  for (i = 0, BitCount = 0; i < 64; i++) {
    if (RShiftU64 (MaskToCheck, i) & 1) {
      BitCount++;
    }
  }

  return BitCount;
}



/**
  Debug print ConfigTdp level info.

  @param Socket              CPU socket ID.
  @param ConfLevelInfoPtr    Pointer to CPU_CONFIG_LEVEL_INFO Structure.

**/
VOID
PrintConfigTdpLevelInfo (
  IN UINT8                  Socket,
  IN CPU_CONFIG_LEVEL_INFO  *ConfLevelInfoPtr
  )
{
  UINT8     Level;

  RcDebugPrint (SDBG_DEFAULT, "\n\n  :: S%d MaxRatio : MinRatio : MinOpRatio : Config TDP Capable? : Iss Capable?\n", Socket);
  RcDebugPrint (SDBG_DEFAULT, "           %02d         %02d          %02d                %d                  %d\n",
                ConfLevelInfoPtr->MaxCoreToBusRatio[Socket],
                ConfLevelInfoPtr->MinCoreToBusRatio[Socket],
                ConfLevelInfoPtr->MinOperaingRatio[Socket],
                (ConfLevelInfoPtr->AvxSystemStatus & (1 << Socket)),
                (ConfLevelInfoPtr->IssSystemStatus & (1 << Socket)));

  RcDebugPrint (SDBG_DEFAULT, "\n  :: S%d Current TDP Level : ConfigTdpMaxLevel : IssMaxLevel\n", Socket);
  RcDebugPrint (SDBG_DEFAULT, "              %02d                   %02d               %02d\n",
                ConfLevelInfoPtr->CurrentTdpLevel[Socket],
                ConfLevelInfoPtr->ConfigTdpMaxLevel[Socket],
                ConfLevelInfoPtr->IssMaxLevel[Socket]);

  RcDebugPrint (SDBG_DEFAULT, "\nLevel : PkgTDP : TdpRatio : Core # : SSE P1 : AVX2 P1 : AVX3 P1\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   : %06d :  %06d  : %06d : %06d :  %06d : %06d\n",
                Level,
                ConfLevelInfoPtr->PkgTdp[Socket][Level],
                ConfLevelInfoPtr->TdpRatio[Socket][Level],
                ConfLevelInfoPtr->CoreCount[Socket][Level],
                ConfLevelInfoPtr->SseP1Ratio[Socket][Level],
                ConfLevelInfoPtr->Avx2P1Ratio[Socket][Level],
                ConfLevelInfoPtr->Avx3P1Ratio[Socket][Level]);

  }

  RcDebugPrint (SDBG_DEFAULT, "\nLevel : Turbo Ratio Limit -        SSE P1      :       AVX2 P1     :     AVX3 P1\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   :                   - : %016lx :  %016lx : ",
                Level,
                ConfLevelInfoPtr->SseTurboRatio[Socket][Level].Data,
                ConfLevelInfoPtr->Avx2TurboRatio[Socket][Level].Data);

    RcDebugPrint (SDBG_DEFAULT, "%016lx\n",
                ConfLevelInfoPtr->Avx3TurboRatio[Socket][Level].Data);

  }

  RcDebugPrint (SDBG_DEFAULT, "\nLevel :   Power  : TJMax : CoreMaskLo : CoreMaskHi\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   : %8x : %04d  :  %016lx\n\n",
                Level,
                ConfLevelInfoPtr->PowerMinMax[Socket][Level],
                ConfLevelInfoPtr->Tjmax[Socket][Level],
                ConfLevelInfoPtr->IssEnabledCoreMask[Socket][Level]);
  }

  RcDebugPrint (SDBG_DEFAULT, "\nLevel : PbfCap : P1_Hi : P1_Lo : CoreMaskLo : CoreMaskHi\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   :   %d    : %04d  : %04d  :  %016lx\n\n",
                Level,
                ConfLevelInfoPtr->PbfCapable[Socket][Level],
                ConfLevelInfoPtr->PbfP1HighRatio[Socket][Level],
                ConfLevelInfoPtr->PbfP1LowRatio[Socket][Level],
                ConfLevelInfoPtr->PbfP1HiCoreMap[Socket][Level]);
  }

  RcDebugPrint (SDBG_DEFAULT, "\nCore Ratio Info\nLevel :   P0   :    P1   :   Pn   :   Pm\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   : %06d :  %06d   : %06d : %06d\n",
                Level,
                ConfLevelInfoPtr->CoreP0Ratio[Socket][Level],
                ConfLevelInfoPtr->CoreP1Ratio[Socket][Level],
                ConfLevelInfoPtr->CorePnRatio[Socket][Level],
                ConfLevelInfoPtr->CorePmRatio[Socket][Level]);

  }

  RcDebugPrint (SDBG_DEFAULT, "\nUncore Ratio Info\nLevel :   P0   :    P1   :   Pn   :   Pm\n");
  for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {

    RcDebugPrint (SDBG_DEFAULT, "  %d   : %06d :  %06d   : %06d : %06d\n",
                Level,
                ConfLevelInfoPtr->UncoreP0Ratio[Socket][Level],
                ConfLevelInfoPtr->UncoreP1Ratio[Socket][Level],
                ConfLevelInfoPtr->UncorePnRatio[Socket][Level],
                ConfLevelInfoPtr->UncorePmRatio[Socket][Level]);

  }
}

/**

  Executed by SBSP only.
  Configure all CPU sockets to a common Max Non-turbo Ratio by programming FLEX_RATIO CSR.

  @retval FLEX_RATIO CSRs programmed
  @retval SetSysResetRequired() updated (if reset is needed)

**/
EFI_STATUS
EFIAPI
ProgramProcessorFlexRatio (
  )
{
  UINT8   Socket;
  UINT8   SbspSktId;
  UINT8   Level;
  UINT8   SocketBitMap;
  UINT8   AvxCapableSystem = 0;
  UINT8   IssCapableSystem = 0;
  UINT8   TargetRatio;
  UINT8   RatioChangeFlag = 0;
  UINT8   ResetNeeded;
  UINT8   CommonMaxRatio = 0;
  UINT8   CommonMinRatio;

  PLATFORM_INFO_N0_PCU_FUN0_STRUCT      PlatformInfoN0Csr;
  PLATFORM_INFO_N1_PCU_FUN0_STRUCT      PlatformInfoN1Csr;
  FLEX_RATIO_N0_PCU_FUN3_STRUCT         FlexRatioCsr;

  CPU_PRE_MEM_POLICY       *CpuPolicy;
  CPU_NVRAM                *CpuNvramData;
  CPU_VAR_DATA             *CpuVarDataPtr;

  CPU_CONFIG_LEVEL_INFO	   CpuConfLevelInfo;

  EFI_STATUS  ReturnData = EFI_SUCCESS;

  RcDebugPrint (SDBG_DEFAULT, "\n:: ProgramProcessorFlexRatio()\n");

  CpuPolicy = GetCpuPreMemPolicy();
  CpuNvramData = GetCpuNvramData();
  CpuVarDataPtr = GetCpuVarData();
  SbspSktId = GetSysSbspSocketIdNv ();

  //
  // Check data structure ptrs
  //
  if (CpuPolicy == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "GetCpuPreMemPolicy Returned NULL pointer\n");
    RC_FATAL_ERROR ((CpuPolicy != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    ReturnData = EFI_UNSUPPORTED;
  }

  if (CpuNvramData == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "CpuNvramData Returned NULL pointer\n");
    RC_FATAL_ERROR ((CpuPolicy != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    ReturnData = EFI_UNSUPPORTED;
  }

  if (CpuVarDataPtr == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "CpuVarDataPtr Returned NULL pointer\n");
    RC_FATAL_ERROR ((CpuPolicy != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    ReturnData = EFI_UNSUPPORTED;
  }

  if (ReturnData != EFI_SUCCESS) {
    return ReturnData;
  }

  //
  // Initialize various data
  //
  TargetRatio = 0;
  ResetNeeded = 0;
  SocketBitMap = 0;

  ZeroMem (&CpuConfLevelInfo, sizeof (CPU_CONFIG_LEVEL_INFO));

  //
  //  For Intel PPV test only: Do not touch power-on default ratio of all CPU sockets if selected in BIOS Setup.
  //  This allows each CPU socket to run at different default ratio/freq as-is.
  //  CAUTION:  This usage is out-of-spec, so use at your own risk.
  //
  if (CpuPolicy->AllowMixedPowerOnCpuRatio) {
    RcDebugPrint (SDBG_DEFAULT, "  :: setup.cpu.AllowMixedPowerOnCpuRatio = Enabled. Skip.\n");
    return ReturnData;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((!RasInProgress() && SocketPresent(Socket)) ||
      (RasInProgress() && GetRasSocId() == Socket)) {

      SocketBitMap |= (1 << Socket);

      //
      // Get the max non-turbo ratio and the max efficiency ratio from CSRs
      // pcode has initialized them at reset
      //
      PlatformInfoN0Csr.Data = UsraCsrRead (Socket, 0, PLATFORM_INFO_N0_PCU_FUN0_REG);
      CpuConfLevelInfo.MaxCoreToBusRatio[Socket] = (UINT8) (PlatformInfoN0Csr.Bits.max_non_turbo_lim_ratio);
      CpuConfLevelInfo.TdpRatio[Socket][0] = CpuConfLevelInfo.MaxCoreToBusRatio[Socket];

      PlatformInfoN1Csr.Data = UsraCsrRead (Socket, 0, PLATFORM_INFO_N1_PCU_FUN0_REG);
      CpuConfLevelInfo.PlatformInfoN1Csr[Socket] = PlatformInfoN1Csr.Data;
      CpuConfLevelInfo.MinCoreToBusRatio[Socket] = (UINT8) (PlatformInfoN1Csr.Bits.max_efficiency_ratio);
      CpuConfLevelInfo.MinOperaingRatio[Socket]  = (UINT8) (PlatformInfoN1Csr.Bits.min_operating_ratio);

      CpuConfLevelInfo.ConfigTdpMaxLevel[Socket] = (UINT8) (PlatformInfoN1Csr.Bits.config_tdp_levels);

      CpuVarDataPtr->MaxCoreToBusRatio[Socket] = CpuConfLevelInfo.MaxCoreToBusRatio[Socket];
      CpuVarDataPtr->MinCoreToBusRatio[Socket] = CpuConfLevelInfo.MinCoreToBusRatio[Socket];

      //
      // Collect all level TDP info
      //
      GetConfigTdpLevelInfo (Socket, &CpuConfLevelInfo);

      PrintConfigTdpLevelInfo (Socket, &CpuConfLevelInfo);
    }
  }

  //
  // Get user setting, then determine system status
  //
  if (SocketBitMap == CpuConfLevelInfo.IssSystemStatus) {

    GetCpuVarData()->IssCapableSystem = 1;
    GetCpuVarData()->IssConfigTdpMaxLevel = CpuConfLevelInfo.IssMaxLevel[SbspSktId];

    if (CpuPolicy->EnableGv && (CpuPolicy->IssTdpLevel > 0)) {
      IssCapableSystem = CpuPolicy->IssTdpLevel;
      CpuConfLevelInfo.DynamicIssEnableFlag = CpuPolicy->DynamicIss;
    }
  }

  if (SocketBitMap == CpuConfLevelInfo.AvxSystemStatus) {

    GetCpuVarData()->ConfigTdpCapableSystem = 1;

    if (CpuPolicy->EnableGv && (CpuPolicy->ConfigTdpLevel > 0)) {
      AvxCapableSystem = CpuPolicy->ConfigTdpLevel;
    }
  }

  CpuConfLevelInfo.IssSystemStatus = IssCapableSystem;
  CpuConfLevelInfo.AvxSystemStatus = AvxCapableSystem;

  //
  //  ISS Level     >0  | 0  |  0
  //  AVX P1        any | >0 |  0
  //  RatioChange   1   | 1  |  iterate sockets to determine
  //
  if ( (CpuPolicy->IssTdpLevel > 0) ||
       ((CpuPolicy->IssTdpLevel == 0) && (CpuPolicy->ConfigTdpLevel > 0)) ) {
    RatioChangeFlag = 1;
  } else {
    // Iterate sockets to determine if asymmertrical P1 ratio among sockets
    CommonMaxRatio = 0;
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent(Socket)) {
        if (CommonMaxRatio == 0) {
          CommonMaxRatio = CpuConfLevelInfo.MaxCoreToBusRatio[Socket];
        } else if (CommonMaxRatio != CpuConfLevelInfo.MaxCoreToBusRatio[Socket]) {
          RatioChangeFlag = 1;
          break;
        }
      } // socket present
    }
  }

  //
  // Find Common MAX Ratio for system
  //
  CommonMaxRatio = FindCommonP1Ratio (&CpuConfLevelInfo);
  CommonMinRatio = CpuConfLevelInfo.MinCoreToBusRatio[0];

  TargetRatio = CommonMaxRatio;

  if (CpuPolicy->flexRatioEn != 0) {
    //
    // If user wants to set a specific flex ratio,
    //  this overrides everything
    // Rest of code will see TargetRatio != 0
    //  and will not change it
    //
    TargetRatio = CpuPolicy->flexRatioNext;

    // If the override ratio is out of range, normalize it
    if (TargetRatio < CommonMinRatio) {
      TargetRatio = CommonMinRatio;
      RcDebugPrint (SDBG_ERROR, "FlexRationNext forced to be CommonMinRatio:%d\n", TargetRatio);
    } else if (TargetRatio > CommonMaxRatio) {
      TargetRatio = CommonMaxRatio;
      RcDebugPrint (SDBG_ERROR, "FlexRatioNext forced to be CommonMaxRatio:%d\n", TargetRatio);
    }

    RatioChangeFlag = 1;
  }

  //
  // Print out config TDP and ISS data so far
  //
  RcDebugPrint (SDBG_DEFAULT, "  :: CommonMaxRatio : CommonMinRatio : Target Ratio\n");
  RcDebugPrint (SDBG_DEFAULT, "          %02d              %02d             %02d\n", CommonMaxRatio, CommonMinRatio, TargetRatio);

  RcDebugPrint (SDBG_DEFAULT, "  ::   IssTdpLevel  : ConfigTdpLevel\n");
  RcDebugPrint (SDBG_DEFAULT, "          %02d            %02d\n", CpuPolicy->IssTdpLevel, CpuPolicy->ConfigTdpLevel);

  // Skip FLEX_RATIO override if Dynamic ISS is enabled
  if ((CpuPolicy->DynamicIss > 0) && (IssCapableSystem > 0)) {
    RatioChangeFlag = 0;
    RcDebugPrint (SDBG_DEFAULT, "  :: Dynamic SST-PP is enabled, skip FLEX_RATIO program!\n");
  }

  //
  // Save data into CPU_VAR_DATA
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent(Socket)) {
      continue;
    }

    for (Level = 0; Level < CONFIG_TDP_TOTAL_LEVEL; Level++) {
      GetCpuVarData()->IssConfigTdpRatio[Socket][Level]       = FindP1Ratio (Socket, &CpuConfLevelInfo, Level, CpuConfLevelInfo.AvxSystemStatus);
      GetCpuVarData()->IssConfigTdpPower[Socket][Level]       = CpuConfLevelInfo.PkgTdp[Socket][Level];
      GetCpuVarData()->IssConfigTdpPowerMinMax[Socket][Level] = CpuConfLevelInfo.PowerMinMax[Socket][Level];
      GetCpuVarData()->IssConfigTdpTjmax[Socket][Level]       = CpuConfLevelInfo.Tjmax[Socket][Level];
      GetCpuVarData()->IssConfigTdpCoreCount[Socket][Level]   = CpuConfLevelInfo.CoreCount[Socket][Level];
      GetCpuVarData()->IssConfigTdpEnabledCoreMask[Socket][Level]  = CpuConfLevelInfo.IssEnabledCoreMask[Socket][Level];


      GetCpuVarData()->PbfP1HiCoreMap[Socket][Level]  = CpuConfLevelInfo.PbfP1HiCoreMap[Socket][Level];

      GetCpuVarData()->PbfP1HighRatio[Socket][Level]  = CpuConfLevelInfo.PbfP1HighRatio[Socket][Level];
      GetCpuVarData()->PbfP1LowRatio[Socket][Level]  = CpuConfLevelInfo.PbfP1LowRatio[Socket][Level];
    }
  }

  //
  // Active TDP Level
  //
  SetIssConfigTdpLevel (&CpuConfLevelInfo);

  GetCpuVarData()->IssConfigTdpCurrentLevel = CpuConfLevelInfo.CurrentTdpLevel[SbspSktId];
  RcDebugPrint (SDBG_DEFAULT, "\n  ::  TargetRatio: %d is ready (RatioChangeFlag: %d),   CurrentTdpLevel: %d\n\n",
                              TargetRatio, RatioChangeFlag, GetCpuVarData()->IssConfigTdpCurrentLevel);

  //
  // Check PBF system level capability and active PBF based on policy.
  //
  GetCpuVarData()->PbfCapableSystem = GetPbfSystemStatus (&CpuConfLevelInfo);
  if (GetCpuVarData()->PbfCapableSystem && CpuPolicy->ActivePbf) {
    ActivePbf ();
  }

  //
  // Check SST-CP system level capability
  //
  GetCpuVarData()->SstCpSystemStatus = GetSstCpSystemStatus ();

  // Program FLEX_RATIO
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((!RasInProgress() && (SocketPresent(Socket))) ||
      (RasInProgress() && GetRasSocId() == Socket)) {

      if (RatioChangeFlag) {
        //
        // Read FLEX_RATIO CSR
        // csrReg contains current FlexRatio
        //
        FlexRatioCsr.Data = UsraCsrRead (Socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
        RcDebugPrint (SDBG_DEFAULT, "  :: Read FLEX_RATIO_N0_PCU_FUN3_REG (0x%x) = %d,  TargetRatio = %d\n", FLEX_RATIO_N0_PCU_FUN3_REG, FlexRatioCsr.Bits.flex_ratio, TargetRatio);

        //
        // Check to see if the target ratio is already set.
        //
        RcDebugPrint (SDBG_DEFAULT, "  :: FlexRatioCsr.Bits.flex_ratio = %d,   TargetRatio = %d\n", FlexRatioCsr.Bits.flex_ratio, TargetRatio);

        if (FlexRatioCsr.Bits.flex_ratio == TargetRatio) {
          RcDebugPrint (SDBG_DEFAULT, "  :: TargetRatio previously set:  S%d FLEX_RATIO_N0_PCU_FUN3_REG = %08X\n", Socket, FlexRatioCsr.Data);
          continue;
        }

        FlexRatioCsr.Bits.flex_ratio = TargetRatio;
        FlexRatioCsr.Bits.enable = 1;
        RcDebugPrint (SDBG_DEFAULT, "  :: S%d Write FLEX_RATIO_N0_PCU_FUN3_REG = %08X\n", Socket, FlexRatioCsr.Data);

        //
        // Program target flex ratio
        //
        UsraCsrWrite (Socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG, FlexRatioCsr.Data);

        //
        // Update PMC set_strap message data register witht the new flex ratio setting
        //
        if (PcdGetBool (PcdPmcStrapSetVdmSupported)) {
          UpdatePmcSetStrapMsgFlexRatio ((UINT8) FlexRatioCsr.Bits.flex_ratio, NULL);
        }

        //
        // Indicate need a reset
        //
        ResetNeeded |= POST_RESET_WARM;
        FlexRatioCsr.Data = UsraCsrRead (Socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
        RcDebugPrint (SDBG_DEFAULT, "  :: S%d Read FLEX_RATIO_N0_PCU_FUN3_REG = %08X\n", Socket, FlexRatioCsr.Data);
      }
    }
  }

  //
  // Save current FLEX_RATIO CSR for S3 resume
  //
  FlexRatioCsr.Data = UsraCsrRead (SbspSktId, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
  CpuNvramData->flexRatioCsr = FlexRatioCsr.Data;

  if (ResetNeeded) {
    SetSysResetRequired(GetSysResetRequired() | ResetNeeded);
  }

  return EFI_SUCCESS;
}

/**
  Per ACP s2207539915 request, set C1E and ACP disable before CPL2

  @param None

  @retval None

**/
VOID
SetEarlyC1e (
  )
{
  CPU_PRE_MEM_POLICY                *CpuPolicy;
  UINT8                             Socket;
  ICX_MSR_POWER_CTL1_REGISTER       PwrCtrlMsr;

  CpuPolicy = GetCpuPreMemPolicy ();
  if (CpuPolicy->EarlyC1eEnable != 0) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent(Socket)) {
        PwrCtrlMsr.Uint64 = ReadMsrPipeHL64 (Socket, ICX_MSR_POWER_CTL1);
        PwrCtrlMsr.Bits.C1eEnable = 1;
        WriteMsrPipeHL64 (Socket, ICX_MSR_POWER_CTL1, PwrCtrlMsr.Uint64);
      }
    }
  }
}

/**

  Get B2P CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO

  @param socket:              CPU socket ID
  @param TdpLevel:            CONFIG_TDP Level

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpLevelInfo (
  IN      UINT8   Socket,
  IN      UINT8   TdpLevel
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO,
                                             TdpLevel,
                                             &PcodeDataReturn);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet B2P Level Info = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn    = 0;
  }

  return PcodeDataReturn;
}

/**

  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO

  @param socket:              CPU socket ID
  @param TdpLevel:            CONFIG_TDP Level

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpPowerInfo (
  IN      UINT8   Socket,
  IN      UINT8   TdpLevel
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO,
                                             TdpLevel,
                                             &PcodeDataReturn);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn    = 0;
  }

  return PcodeDataReturn;
}

/**

  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO

  @param socket:              CPU socket ID
  @param TdpLevel:            CONFIG_TDP Level

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpTjMax (
  IN      UINT8   Socket,
  IN      UINT8   TdpLevel
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO,
                                             TdpLevel,
                                             &PcodeDataReturn);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn    = 0;
  }

  return PcodeDataReturn;
}

/**

  Get Config Tdp Information from the CSRs

  @param socket:              CPU socket ID
  @param TdpLevel:            CONFIG_TDP Level

  @retval GetTdpInfoReturn:   Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetConfigTdpInfoFromCsr (
  IN      UINT8   Socket,
  IN      UINT8   TdpLevel
  )
{
  PACKAGE_POWER_SKU_UNIT_PCU_FUN0_STRUCT  PackagePowerSKUUnit;
  PACKAGE_POWER_SKU_N0_PCU_FUN0_STRUCT    PackagePowerSKU;
  PLATFORM_INFO_N0_PCU_FUN0_STRUCT        PlatformInfoN0Csr;
  PLATFORM_INFO_N1_PCU_FUN0_STRUCT        PlatformInfoN1Csr;
  CONFIG_TDP_LEVEL1_N0_PCU_FUN3_STRUCT    ConfigTdpLevelN0Csr;
  CONFIG_TDP_LEVEL2_N0_PCU_FUN3_STRUCT    ConfigTdpLeve2N0Csr;
  B2P_GET_TDP_INFO_OUTPUT_STRUCT          GetTdpInfoReturn;

  GetTdpInfoReturn.Data    = 0;

  PackagePowerSKUUnit.Data = UsraCsrRead(Socket, 0, PACKAGE_POWER_SKU_UNIT_PCU_FUN0_REG);
  PackagePowerSKU.Data = UsraCsrRead(Socket, 0, PACKAGE_POWER_SKU_N0_PCU_FUN0_REG);
  PlatformInfoN0Csr.Data = UsraCsrRead (Socket, 0, PLATFORM_INFO_N0_PCU_FUN0_REG);
  PlatformInfoN1Csr.Data = UsraCsrRead (Socket, 0, PLATFORM_INFO_N1_PCU_FUN0_REG);

  if (TdpLevel == 0) {
    //
    // max_non_turbo_lim_ratio from PLATFORM_INFO_N0_PCU_FUN0_REG should be equal to
    // tdp_ratio from CONFIG_TDP_NOMINAL_PCU_FUN3_REG.
    //
    GetTdpInfoReturn.Bits.RatioThisLevel = PlatformInfoN0Csr.Bits.max_non_turbo_lim_ratio;
    GetTdpInfoReturn.Bits.PkgTdpThisLevel = (PackagePowerSKU.Bits.pkg_tdp >> PackagePowerSKUUnit.Bits.pwr_unit);
  }

  if ((PlatformInfoN1Csr.Bits.config_tdp_levels > 0) && (TdpLevel == 1)) {
    ConfigTdpLevelN0Csr.Data = UsraCsrRead (Socket, 0, CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG);
    GetTdpInfoReturn.Bits.RatioThisLevel = ConfigTdpLevelN0Csr.Bits.tdp_ratio;
    GetTdpInfoReturn.Bits.PkgTdpThisLevel = (ConfigTdpLevelN0Csr.Bits.pkg_tdp >> PackagePowerSKUUnit.Bits.pwr_unit);
  }

  if ((PlatformInfoN1Csr.Bits.config_tdp_levels > 1) && (TdpLevel == 2)) {
    ConfigTdpLeve2N0Csr.Data = UsraCsrRead (Socket, 0, CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG);
    GetTdpInfoReturn.Bits.RatioThisLevel = ConfigTdpLeve2N0Csr.Bits.tdp_ratio;
    GetTdpInfoReturn.Bits.PkgTdpThisLevel = (ConfigTdpLeve2N0Csr.Bits.pkg_tdp >> PackagePowerSKUUnit.Bits.pwr_unit);
  }

  RcDebugPrint (SDBG_DEFAULT, "  Get TDP info from CSRs = 0x%x\n", GetTdpInfoReturn.Data);

  return GetTdpInfoReturn.Data;
}

/**

  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO

  @param socket:              CPU socket ID
  @param TdpLevel:            CONFIG_TDP Level

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpInfo (
  IN      UINT8   Socket,
  IN      UINT8   TdpLevel
  )
{
  UINT32  PcodeMailboxStatus = 0;
  B2P_GET_TDP_INFO_OUTPUT_STRUCT          GetTdpInfoReturn;

  GetTdpInfoReturn.Data    = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO,
                                             TdpLevel,
                                             &GetTdpInfoReturn.Data);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO (0x%x)\n",
                  GetTdpInfoReturn.Data, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO, GetTdpInfoReturn.Data, PcodeMailboxStatus);
  }

  //
  // Cover for Pcode issue or Data returned from Pcode is "0" case
  //
  if ((GetTdpInfoReturn.Data == 0) || PcodeMailboxStatus) {
    GetTdpInfoReturn.Data = GetConfigTdpInfoFromCsr (Socket, TdpLevel);
  }

  return GetTdpInfoReturn.Data;
}

/**

  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK

  @param socket:              CPU socket ID
  @param Index:               CONFIG_TDP Level Index + Core Mask Index

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpCoreMask (
  IN      UINT8   Socket,
  IN      UINT32  Index
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK,
                                             Index,
                                             &PcodeDataReturn);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn    = 0;
  }

  return PcodeDataReturn;
}

/**

  Set a given TDP level on all sockets

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
ActiveIssLevel (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  )
{
  UINT8          Socket;
  UINT32         PcodeMailboxStatus;
  UINT32         MailboxData;
  UINT32         IssLevel;
  UINT32         AvxLevel;

  IssLevel = ConfLevelInfoPtr->IssSystemStatus;
  AvxLevel = ConfLevelInfoPtr->AvxSystemStatus;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((!RasInProgress() && (SocketPresent(Socket))) ||
      (RasInProgress() && GetRasSocId() == Socket)) {

     if ((ConfLevelInfoPtr->DynamicIssEnableFlag == 0) && (ConfLevelInfoPtr->IssMaxLevel[Socket] > 0)) {
       RcDebugPrint (SDBG_DEFAULT, "  ::  S%d: Active SST-PP Level: %d \n", Socket, IssLevel);

       if (ConfLevelInfoPtr->CurrentTdpLevel[Socket] != (UINT8) (IssLevel & 0xFF)) {
         ConfLevelInfoPtr->CurrentTdpLevel[Socket] = (UINT8) (IssLevel & 0xFF);
         // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_LEVEL
         PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                                       MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_LEVEL,
                                                       IssLevel,
                                                       NULL);
       }
     }

     // Set MAILBOX_BIOS_CMD_SET_MIN_ICCP_LICENSE
     // AVX license mapping for ICX :  SSE = 2, AVX2 = 4, AVX512 = 5
     switch (AvxLevel) {
        case 1:
          MailboxData = 4;
          break;
        case 2:
          MailboxData = 5;
          break;
        default:
          MailboxData = 2;
          break;
      }

      if (AvxLevel > 0) {
        SetB2PAvxIccpLicense (Socket, MailboxData);
      }
    }
  }
}

/**

  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT

  @param socket:              CPU socket ID
  @param Index:               CONFIG_TDP Level Index + Word Index + AVX Level

  @retval PcodeDataReturn:    B2P Return Data (NOTE: "0", no value is avaliable)

**/
UINT32
GetB2pConfigTdpTurboLimit (
  IN      UINT8   Socket,
  IN      UINT32  Index
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                             MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT,
                                             Index,
                                             &PcodeDataReturn);

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT (0x%x - 0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT, Index);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT (0x%x - 0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT, Index, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn    = 0;
  }

  return PcodeDataReturn;
}

/**
  Convert NumberOfCores2Disable value to core disable mask.

  @param Socket                     socket index
  @param CpuPolicy                  pointer of platform policy
  @param AvailCoresMask             pointer of AvailCoresMask
  @param CoreDisableMask            pointer of core disable bit mask
  @param NumberOfCoresDisabledMask  pointer of NumberOfCoresDisabledMask

  @retval VOID

**/
VOID
ConvertNumberOfCores2DisableToMask (
  IN      UINT8               Socket,
  IN      CPU_PRE_MEM_POLICY  *CpuPolicy,
  IN      UINT64_DATA         *AvailCoresMask,
  IN      UINT64_DATA         *CoreDisableReqMask,
  OUT     UINT64_DATA         *NumberOfCoresDisabledMask
  )
{
  UINT16                NumberOfCoresDisabled;
  UINT64                Mask;
  UINT64                CoreDisableReqBitMap;
  UINT64                AvailableCoresBitMap;
  UINT64                NumberOfCoresDisabledMap;
  UINT8                 Index;

  NumberOfCoresDisabledMap = 0;

  if (CpuPolicy->NumberOfCores2Disable[Socket] > 0) {
    CoreDisableReqBitMap = CoreDisableReqMask->Data;
    AvailableCoresBitMap = AvailCoresMask->Data;

    //
    // Count the setup input disable cores number.
    //
    NumberOfCoresDisabled = CountMaskBits64 (AvailableCoresBitMap & CoreDisableReqBitMap);

    //
    // If ME disable more cores than input.
    //
    if (CpuPolicy->NumberOfCores2Disable[Socket] > NumberOfCoresDisabled) {
      AvailableCoresBitMap &= ~CoreDisableReqBitMap;
      for (Index = 0, Mask = BIT0; Index < (CpuPolicy->NumberOfCores2Disable[Socket] - NumberOfCoresDisabled); Mask = LShiftU64 (Mask, 1)) {
        //
        // Disable cores from low bit set.
        //
        if ((AvailableCoresBitMap & Mask)) {
          NumberOfCoresDisabledMap |= Mask;
          Index++;
        }
      }
    }
    NumberOfCoresDisabledMask->Data = NumberOfCoresDisabledMap;
  }
}

/**
  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL.

  @param Socket CPU socket ID.
  @param Index  CONFIG_TDP Level Index.

  @retval B2P Return Data (NOTE: "0", no value is avaliable).

**/
UINT32
GetB2pConfigTdpControl (
  IN      UINT8   Socket,
  IN      UINT32  Index
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL,
                         Index,
                         &PcodeDataReturn
                         );

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn = 0;
  }

  return PcodeDataReturn;
}

/**
  Set B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL to enable PBF.

  @param Socket CPU socket ID

**/
VOID
SetB2pConfigTdpControlPbfEnable (
  IN      UINT8   Socket
  )
{
  UINT32  PcodeMailboxStatus = 0;
  B2P_CONFIG_TDP_SET_CONFIG_TDP_CONTROL_DATA_STRUCT  B2pSetConfigTdpCtrl;

  B2pSetConfigTdpCtrl.Data = 0;
  B2pSetConfigTdpCtrl.Bits.PbfEnable = 1;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL,
                         B2pSetConfigTdpCtrl.Data,
                         NULL
                         );

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nSet MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL (0x%x) to Value = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL, B2pSetConfigTdpCtrl.Data);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to set MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL, B2pSetConfigTdpCtrl.Data, PcodeMailboxStatus);
  }
}

/**
  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO.

  @param Socket CPU socket ID.
  @param Index  CONFIG_TDP Level Index + Word Index of core mask.

  @retval B2P Return Data (NOTE: "0", no value is avaliable).

**/
UINT32
GetB2pConfigTdpPbfGetCoreMask (
  IN      UINT8   Socket,
  IN      UINT32  Index
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO,
                         Index,
                         &PcodeDataReturn
                         );

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn = 0;
  }

  return PcodeDataReturn;
}

/**
  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO.

  @param Socket CPU socket ID.
  @param Index  CONFIG_TDP Level Index.

  @retval B2P Return Data (NOTE: "0", no value is avaliable).

**/
UINT32
GetB2pConfigTdpPbfGetP1HiP1Lo (
  IN      UINT8   Socket,
  IN      UINT32  Index
  )
{
  UINT32  PcodeDataReturn = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO,
                         Index,
                         &PcodeDataReturn
                         );

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO, PcodeDataReturn, PcodeMailboxStatus);

    PcodeDataReturn = 0;
  }

  return PcodeDataReturn;
}

/**

  Get smt capability for the given CPU package

  @param socket:  CPU socket ID

  @retval SmtCapability - UINT8      if non-zero, then CPU is smt-capable

**/
UINT8
GetSmtCapability (
  IN      UINT8   Socket
  )
{
  UINT8                   SmtCapability = 0;
  CAPID0_PCU_FUN3_STRUCT  Capid0Func3;

  Capid0Func3.Data = UsraCsrRead (Socket, 0, CAPID0_PCU_FUN3_REG);
  SmtCapability = (UINT8) !Capid0Func3.Bits.ht_dis;

  return SmtCapability;
}

/**
  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO.

  @param Socket         CPU socket ID
  @param TdpLevel       CONFIG_TDP Level

  @retval B2P Return Data (NOTE: "0", no value is avaliable).

**/
UINT32
GetB2pConfigTdpGetUncoreP0P1 (
  IN      UINT8   Socket,
  IN      UINT32  TdpLevel
  )
{
  UINT32  PcodeDataReturn    = 0;
  UINT32  PcodeMailboxStatus = 0;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO,
                         TdpLevel,
                         &PcodeDataReturn
                         );
  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO, PcodeDataReturn, PcodeMailboxStatus);
    PcodeDataReturn = 0;
  }
  return PcodeDataReturn;
}

/**
  Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO.

  @param Socket          CPU socket ID
  @param ConfigTdpLevel  CONFIG_TDP Level
  @param Uncore          0 - Core;  1 - Uncore

  @retval B2P Return Data (NOTE: "0", no value is avaliable).

**/
UINT32
GetB2pConfigTdpGetRatioInfo (
  IN      UINT8   Socket,
  IN      UINT32  ConfigTdpLevel,
  IN      UINT8   Uncore
  )
{
  UINT32  PcodeDataReturn    = 0;
  UINT32  PcodeMailboxStatus = 0;

  B2P_CONFIG_TDP_GET_RATIO_INFO_CMD_DATA_STRUCT   GetRatioInfoCmdData;
  GetRatioInfoCmdData.Data             = 0;
  GetRatioInfoCmdData.Bits.ConfigLevel = ConfigTdpLevel;
  GetRatioInfoCmdData.Bits.Uncore      = Uncore;

  PcodeMailboxStatus = SendConfigTdpSubCommand (
                         Socket,
                         MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO,
                         GetRatioInfoCmdData.Data,
                         &PcodeDataReturn
                         );
  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet Value = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO (0x%x)\n",
                  PcodeDataReturn, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                  MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO, PcodeDataReturn, PcodeMailboxStatus);
    PcodeDataReturn = 0;
  }
  return PcodeDataReturn;
}
