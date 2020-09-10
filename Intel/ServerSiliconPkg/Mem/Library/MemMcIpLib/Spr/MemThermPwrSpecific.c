/** @file
  Implements Memory power and thermal csr access related functions specific for Spr

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

#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <RcRegs.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/UsraCsrLib.h>
#include <CpuPciAccess.h>

/**

  Configures Off pkg Memtrip fields

  @param [in]  ThermalThrottlingOptions      Throttling Options
  @param [in]  Socket                        Socket Number

  @retval N/A

**/
VOID
EFIAPI
ConfigureOffPkgBits (
  IN UINT16 ThermalThrottlingOptions,
  IN UINT8  Socket
  )
{

  THERMTRIP_CONFIG_PCU_FUN2_STRUCT  ThermTripCfgReg;

  ThermTripCfgReg.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, THERMTRIP_CONFIG_PCU_FUN2_REG);

  //
  // Configure MemTrip to ThermTrip
  //
  ThermTripCfgReg.Bits.memtrip1_to_xxthermtrip_n_en = 0;
  if ((ThermalThrottlingOptions & OFFPKG_MEM_TO_THERMTRIP_EN) && (ThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN))) {
    ThermTripCfgReg.Bits.memtrip1_to_xxthermtrip_n_en = 1;
  }

  //
  // Configure MemTrip to MemTrip
  //
  ThermTripCfgReg.Bits.memtrip1_to_xxmemtrip_n_en = 0;
  if ((ThermalThrottlingOptions & OFFPKG_MEM_TO_MEMTRIP_EN) && (ThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN))) {
    ThermTripCfgReg.Bits.memtrip1_to_xxmemtrip_n_en = 1;
  }

  UsraCsrWrite (Socket, PCU_INSTANCE_0, THERMTRIP_CONFIG_PCU_FUN2_REG, ThermTripCfgReg.Data);
}

/**

  Configures in pkg Memtrip fields

  @param [in]  ThermalThrottlingOptions      Throttling Options
  @param [in]  Socket                        Socket Number

  @retval N/A

**/
VOID
EFIAPI
ConfigureInPkgBits (
  IN UINT16 ThermalThrottlingOptions,
  IN UINT8  Socket
  )
{

  THERMTRIP_CONFIG_PCU_FUN2_STRUCT  ThermTripCfgReg;

  //
  // Configure MemTrip to ThermTrip
  //
  ThermTripCfgReg.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, THERMTRIP_CONFIG_PCU_FUN2_REG);

  ThermTripCfgReg.Bits.memtrip0_to_xxthermtrip_n_en = 0;
  if ((ThermalThrottlingOptions & INPKG_MEM_TO_THERMTRIP_EN) && (ThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN))) {
    ThermTripCfgReg.Bits.memtrip0_to_xxthermtrip_n_en = 1;
  }

  //
  // Configure MemTrip to MemTrip
  //
  ThermTripCfgReg.Bits.memtrip0_to_xxmemtrip_n_en = 0;
  if ((ThermalThrottlingOptions & INPKG_MEM_TO_MEMTRIP_EN) && (ThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN))) {
    ThermTripCfgReg.Bits.memtrip0_to_xxmemtrip_n_en = 1;
  }

  UsraCsrWrite (Socket, PCU_INSTANCE_0, THERMTRIP_CONFIG_PCU_FUN2_REG, ThermTripCfgReg.Data);
}

/**

  Configures power per command for given DIMM

  @param[in] Socket            - Socket number, 0 based
  @param[in] Channel           - Channel Number, 0 based
  @param[in] Dimm              - Dimm Number, 0 based
  @param[in] WTPmCmdPwrData    - DimmWTRowEntryStruct structure that passes the values
                                 Parameter is an entry from DIMM Weight Table
                                 0-3 => PwrcasrDimmVal, 4-7 =>PwrcaswDimmVal

  @retval    N/A

**/
VOID
EFIAPI
ConfigurePwrPerCmd (
  IN UINT8  Socket,
  IN UINT8  Channel,
  IN UINT8  Dimm,
  IN UINT32 WTPmCmdPwrData
  )
{
  struct sysHost                     *Host = NULL;
  PM_CMD_PWR_0_MCDDC_CTL_STRUCT      PmCmdPwrReg;
  UINT32                             PmCmdPwrRegOffset;
  struct dimmNvram                   (*DimmNvList)[MAX_DIMM];

  //Get sysHost Pointer
  Host              = GetSysHostPointer ();
  DimmNvList        = GetDimmNvList (Host, Socket, Channel);
  PmCmdPwrRegOffset = GetPmCmdPwrRegOffset (Dimm);
  PmCmdPwrReg.Data  = MemReadPciCfgEp (Socket, Channel, PmCmdPwrRegOffset);

  if (WTPmCmdPwrData != 0) {
    if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP)) {

      //
      // look up PmCmdPwrReg value from the fixed table
      //
      PmCmdPwrReg.Bits.active_idle_dimm = (WTPmCmdPwrData >> 22) & 0x1F;  // get ActiveIdleDimmVal
    } else {
      //
      // look up PmCmdPwrReg value from the fixed table
      //
      PmCmdPwrReg.Data = WTPmCmdPwrData;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "::MPM::  get table value PmCmdPwrReg.Data = %x\n\n", PmCmdPwrReg.Data);
  } else {
    //
    // default DRAM Power setting
    //
    if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP)) {

      PmCmdPwrReg.Bits.active_idle_dimm = DEFAULT_ACTIVE_IDLE_DIMM_VALUE;  // get ActiveIdleDimmVal

    } else {
      PmCmdPwrReg.Bits.pwrodt_cnt_dimm = DEFAULT_PWR_ODT_CNT_DIMM_VALUE;

      if ((*DimmNvList)[Dimm].keyByte2 == SPD_UDIMM) {
        PmCmdPwrReg.Bits.active_idle_dimm = DEFAULT_UDIMM_ACTIVE_IDLE_DIMM_VALUE;
        PmCmdPwrReg.Bits.pwrref_dimm = DEFAULT_UDIMM_PWR_REF_DIMM;
        PmCmdPwrReg.Bits.pwract_dimm = DEFAULT_UDIMM_PWR_CONTRIBUTION_ACT_COMMAND;
      } else if ((*DimmNvList)[Dimm].actKeyByte2 == SPD_LRDIMM) {
        PmCmdPwrReg.Bits.active_idle_dimm = DEFAULT_LRDIMM_ACTIVE_IDLE_DIMM_VALUE;
        PmCmdPwrReg.Bits.pwrref_dimm = DEFAULT_LRDIMM_PWR_REF_DIMM;
        PmCmdPwrReg.Bits.pwract_dimm = DEFAULT_LRDIMM_PWR_CONTRIBUTION_ACT_COMMAND;
      } else {  // assume RDIMM
        PmCmdPwrReg.Bits.active_idle_dimm = DEFAULT_RDIMM_ACTIVE_IDLE_DIMM_VALUE;
        PmCmdPwrReg.Bits.pwrref_dimm = DEFAULT_RDIMM_PWR_REF_DIMM;
        PmCmdPwrReg.Bits.pwract_dimm = DEFAULT_RDIMM_PWR_CONTRIBUTION_ACT_COMMAND;
      }

      PmCmdPwrReg.Bits.pwrcasw_dimm = DEFAULT_PWR_CONTRIBUTION_CAS_WR;
      PmCmdPwrReg.Bits.pwrcasr_dimm = DEFAULT_PWR_CONTRIBUTION_CAS_RD;
    }
  }

  PmCmdPwrReg.Bits.pwrref_dimm = 0;

  MemWritePciCfgEp (Socket, Channel, PmCmdPwrRegOffset, PmCmdPwrReg.Data);

  return;
} // ConfigurePwrPerCmd

/**

  Configure CKE and ODT contributions for RAPL.

  @param[in]  Socket  The target socket number
  @param[in]  Channel The target channel number

  @retval N/A

**/
VOID
EFIAPI
ConfigureCkeAndOdtContributions (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  // ICX only
}

/**

  Configure min OPS.

  @param[in]  Socket  The target socket number

  @retval N/A

**/
VOID
EFIAPI
ConfigureMinOps (
  IN UINT8 Socket
  )
{
  // ICX only
}
