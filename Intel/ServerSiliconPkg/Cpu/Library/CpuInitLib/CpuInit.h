/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_INIT_H
#define _CPU_INIT_H

#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>
#include "SysHostChipCommon.h"
#include <Chip/Include/SysHostChip.h>
#include <KtiSetupDefinitions.h>

#include <RcRegs.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryCoreLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <UncoreCommonIncludes.h>
#include <Library/CpuInitLib.h>
#include <PchReservedResources.h>
#include <Library/KtiApi.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <Guid/CpuNvramData.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuEarlyDataLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemRcLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/PcuIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <ReferenceCodeFatalErrors.h>

#define PACKAGE_TDP_MASK      0X7FFF    //Bit[14:0]
#define RATIO_MASK            0XFF      //Bit[7:0]



typedef struct {
    UINT8                   MaxCoreToBusRatio[MAX_SOCKET];                     // Package Max Non-turbo Ratio (per socket).
    UINT8                   MinCoreToBusRatio[MAX_SOCKET];                     // Package Maximum Efficiency Ratio (per socket).
    UINT8                   MinOperaingRatio[MAX_SOCKET];                      // Package Minimum Operating Ratio (per socket).
    UINT8                   IssSystemStatus;                                   // Usage 1: 1 = All sockets ISS capable status; Usage 2: customer selection
    UINT8                   AvxSystemStatus;                                   // Usage 1: 1 = All sockets AVX capable status; Usage 2: customer selection
    UINT8                   CurrentTdpLevel[MAX_SOCKET];                       // B2P CONFIG_TDP_GET_LEVELS_INFO
    UINT8                   ConfigTdpMaxLevel[MAX_SOCKET];                     // B2P CONFIG_TDP_GET_LEVELS_INFO
    UINT8                   IssMaxLevel[MAX_SOCKET];                           // B2P CONFIG_TDP_GET_LEVELS_INFO
    UINT16                  PkgTdp[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];          // get B2P CONFIG_TDP_GET_TDP_INFO
    UINT8                   TdpRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // get B2P CONFIG_TDP_GET_TDP_INFO
    UINT8                   SseP1Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // get B2P CONFIG_TDP_GET_P1_INFO
    UINT8                   Avx2P1Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // get B2P CONFIG_TDP_GET_P1_INFO
    UINT8                   Avx3P1Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // get B2P CONFIG_TDP_GET_P1_INFO
    UINT32                  PowerMinMax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // get B2P CONFIG_TDP_GET_POWER_INFO
    UINT8                   Tjmax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];           // B2P CONFIG_TDP_GET_ICCP_TJMAX_INFO
    UINT8                   CoreCount[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];       // get B2P CONFIG_TDP_GET_CORE_COUNT
    UINT64                  IssEnabledCoreMask[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];   // get B2P CONFIG_TDP_GET_CORE_MASK
    UINT32                  PlatformInfoN1Csr[MAX_SOCKET];
    UINT64_DATA             SseTurboRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];   // get B2P CONFIG_TDP_GET_TURBO_LIMIT
    UINT64_DATA             Avx2TurboRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];  // get B2P CONFIG_TDP_GET_TURBO_LIMIT
    UINT64_DATA             Avx3TurboRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];  // get B2P CONFIG_TDP_GET_TURBO_LIMIT
    UINT8                   PbfCapableSystem;                                       // 1 = All sockets PBF Capable
    UINT8                   PbfCapable[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];         // B2P GET_CONFIG_TDP_CONTROL PBF_SUPPORT Bit[1]
    UINT64                  PbfP1HiCoreMap[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];   // B2P PBF_GET_CORE_MASK_INFO 64Bits
    UINT8                   PbfP1HighRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // B2P PBF_GET_P1HI_P1LO_INFO P1_HI
    UINT8                   PbfP1LowRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // B2P PBF_GET_P1HI_P1LO_INFO P1_LO
    UINT8                   DynamicIssEnableFlag;
    UINT8                   CoreP0Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   CoreP1Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   CorePnRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   CorePmRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   UncoreP0Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   UncoreP1Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   UncorePnRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // B2P CONFIG_TDP_GET_RATIO_INFO
    UINT8                   UncorePmRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];      // B2P CONFIG_TDP_GET_RATIO_INFO
} CPU_CONFIG_LEVEL_INFO;

#define CORE_RATIO_P1           0x3f


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
  );


/**

  Convert a mask of bits to a count of bits

  @param MaskToCheck: 64-bit mask of bits

  @retval BitCount:   8-bit count of bits in input mask

**/
UINT8
CountMaskBits64 (
  IN CONST UINT64 MaskToCheck
  );


/**

  Clear specific bit in 64-bit mask

  @param  *BitMask:     64-bit mask of bits
  @param  BitPosition:  Position of bit to clear

  @retval VOID

**/
VOID
ClearBit64 (
  IN OUT UINT64    *BitMask,
  IN CONST UINT8        BitPosition
  );

/**

  Executed by SBSP only. cLEAR dmictrl.AUTO_COMPLETE_PM

  @retval VOID
  @retval Related CPU CSRs are programmed as needed.
**/
VOID
ClearDmictrlAutoPmCplt (
    UINT8          socket
  );

/**

  Executed by SBSP only. Configure CPU registers that are required to be configured for PCODE usage prior to setting BIOS_INIT_Done.

  @retval VOID
  @retval Related CPU CSRs are programmed as needed.
  @retval Host->var.common.resetRequired is updated as needed

**/
VOID
CpuInitMISC(
  );

/**
  Get Cpu Nvram Data
  @Result: Point to the Cpu Nvram data;
 **/
CPU_NVRAM *
GetCpuNvramData(
  );

/**
  Init the CPU Nvram Data;
  @Result: EFI_SUCCESS the Nvram data init success;
                EFI_OUT_OF_RESOURCES Can't create CPU Nvram Data hob.
**/
EFI_STATUS
InitCpuNvram(
  );

/**

  Set a given TDP level on all sockets

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
SetIssConfigTdpLevel (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  );

/**

  Executed by SBSP only. Decouple CLM frequency from core PBM

  @retval VOID

**/
VOID
DeCoupleCLMFrequency (
  );

/**

  Restore DESIRED_CORES CSRS for all CPUs on S3 resume path.

  @param socket: socket number

  @retval Type of reset required

**/
UINT8
RestoreDesiredCores (
  UINT8                      socket
  );

/**

 Executed by SBSP only. Program IOT/OCLA config based on Setup data.

 @retval VOID

**/
VOID
Config_IOT_LLC (
  );

/**

  Executed by SBSP only. cLEAR dmictrl.AUTO_COMPLETE_PM


  @retval VOID
  @retval Related CPU CSRs are programmed as needed.
**/
VOID
ClearDmictrlAutoPmCplt (
  UINT8               socket
  );

/**

    Get B2P UNCORE_P0_P1_INFO

    @param Socket:    Socket # being configured
    @param MsrValue:  MSR Low 32bits value

    @retval:        Return data from B2P mailbox command

**/
UINT32
CpuUncoreRatioInit (
  UINT8         Socket,
  UINT32        MsrValue
  );

/**

  Get TDP information for TDP levels supported

  @param Socket:              Socket to get info for
  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
GetConfigTdpLevelInfo (
  IN  UINT8                    Socket,
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  );

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
  );

/**

    Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO

    @param Socket:        Socket # being configured
    @param ConfigIndex:   ConfigTdp Level

    @retval          FuseP1 value

**/
UINT32
GetB2PFuseP1Ratio (
  UINT8         Socket,
  UINT8         ConfigIndex
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**

  Find Common MAX Ratio for system

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval CommonMaxRatio

**/
UINT8
FindCommonP1Ratio (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  );

/**

  Set a given TDP level on all sockets

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
ActiveIssLevel (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  );

/**

  Set B2P SET_MIN_ICCP_LICENSE

  @param socket:         CPU socket ID
  @param MailBoxData:    Value to B2P

  @retval VOID

**/
VOID
SetB2PAvxIccpLicense (
  IN      UINT8     Socket,
  IN      UINT32    MailBoxData
  );

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
  );

/**

  Get B2P GET_TDP_INFO

  @param Socket:          Socket to get info for
  @param ConfigTdpLevel:  TDP level to get info for

  @retval PcodeDataReturn

**/
UINT32
GetB2PConfigTdpInfo (
  IN UINT8      Socket,
  IN UINT8      ConfigTdpLevel
  );

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
  );

EFI_STATUS
CpuPhysicalAddressLimit (
);

/**
  Per ACP s2207539915 request, set C1E and ACP disable before CPL2

  @param None

  @retval None

**/
VOID
SetEarlyC1e (
);


/**
  Send MAILBOX_BIOS_CMD_TSC_RESET_ENABLE to pcode, to enable reset TSC during warm reboot.

  @retval N/A
**/
VOID
EnableTscReset (
  VOID
  );

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
  );

/**
  Set B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL to enable PBF.

  @param Socket CPU socket ID

**/
VOID
SetB2pConfigTdpControlPbfEnable (
  IN      UINT8   Socket
  );

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
  );

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
  );

/**

  Enable or disable SMT per platform request

  @param Socket           socket index
  @param CpuPolicy        pointer of platform policy
  @param CpuNvramData     pointer of NVRAM data

  @retval VOID

**/
VOID
ConfigSocketSmt (
  IN    UINT8              Socket,
  IN    CPU_PRE_MEM_POLICY *CpuPolicy,
  OUT   CPU_NVRAM          *CpuNvramData
  );

/**

  Get smt capability for the given CPU package

  @param socket:  CPU socket ID

  @retval SmtCapability - UINT8      if non-zero, then CPU is smt-capable

**/
UINT8
GetSmtCapability (
  IN    UINT8         Socket
  );

/**
  Get SST-CP system level Status.

  @return SST-CP system level Status.

**/
UINT32
GetSstCpSystemStatus (
  VOID
  );

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
  );

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
  );
#endif // _CPU_INIT_H
