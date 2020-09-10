/** @file
  Implements Memory power and thermal csr access related functions

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

#include <Library/MemoryCoreLib.h>
#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiApi.h>
#include <Library/PcuMailBoxLib.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/UsraCsrLib.h>
#include <CpuPciAccess.h>
#include <Library/MemRcLib.h>
#include <Library/MemAccessLib.h>
#include <Library/SiliconWorkaroundLib.h>

CONST UINT32 PmCmdPwrRegOffsets[] = {
  PM_CMD_PWR_0_MCDDC_CTL_REG,
  PM_CMD_PWR_1_MCDDC_CTL_REG
};

/**
  Program MemHot Temperature range (Disable, High, Mid, Low)

  @param[in] MemTechType         - Memory technology type
  @param[in] Socket              - Socket Id, 0 based
  @param[in] Channel             - Channel number, 0 based
  @param[in] Dimm                - Dimm number, 0 based
  @param[in] MemHotOutputOnlyOpt - Option to select Memhot temperature range enable
                                 - Available Options are
                                     MEMHOT_OUTPUT_ONLY_DIS          = 0
                                     MEMHOT_OUTPUT_ONLY_HIGH         = 1
                                     MEMHOT_OUTPUT_ONLY_HIGH_MED     = 2
                                     MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW = 3

  retval N/A
**/
VOID
SetMemHotTemperatureRange (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          Channel,
  IN UINT8          Dimm,
  IN UINT8          MemHotOutputOnlyOpt
  )
{
  UINT32                               DimmTempEvOfsetRegOffset[] = {DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG,
                                                                     DIMM_TEMP_EV_OFST_1_MCDDC_CTL_REG};
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT DimmTempEvOfst;
  SYS_SETUP                            *Setup;

  Setup = GetSysSetupPointer ();
  DimmTempEvOfst.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempEvOfsetRegOffset[Dimm]);

  if (Setup->mem.ThrottlingMidOnTempLo == THRTMID_ON_TEMPLO_ENABLED) {
    DimmTempEvOfst.Bits.ev_thrtmid_templo = 1;
  } else {
    DimmTempEvOfst.Bits.ev_thrtmid_templo = 0;
  }

  switch (MemHotOutputOnlyOpt) {
  case MEMHOT_OUTPUT_ONLY_DIS:
    DimmTempEvOfst.Bits.ev_mh_temphi_en = 0;
    DimmTempEvOfst.Bits.ev_mh_tempmid_en = 0;
    DimmTempEvOfst.Bits.ev_mh_templo_en = 0;
    break;
  case MEMHOT_OUTPUT_ONLY_HIGH_MED:
    DimmTempEvOfst.Bits.ev_mh_temphi_en = 1;
    DimmTempEvOfst.Bits.ev_mh_tempmid_en = 1;
    break;
  case MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW:
    DimmTempEvOfst.Bits.ev_mh_temphi_en = 1;
    DimmTempEvOfst.Bits.ev_mh_tempmid_en = 1;
    DimmTempEvOfst.Bits.ev_mh_templo_en = 1;
    break;
  case MEMHOT_OUTPUT_ONLY_HIGH:
  default:
    DimmTempEvOfst.Bits.ev_mh_temphi_en = 1; //high by default
    break;
  }

  ChRegisterWrite (MemTechType, Socket, Channel, DimmTempEvOfsetRegOffset[Dimm], DimmTempEvOfst.Data);
}

/**
  Program MemHot IN\OUT direction

  @param[in] socket                           - Socket Id, 0 based
  @param[in] ThermalThrottlingOptions         - Option for in out direction

  retval N/A
**/
VOID
ProgramMemHotFeature (
  IN UINT8 Socket,
  IN UINT16 ThermalThrottlingOptions
  )
{
  MH_MAINCTL_PCU_FUN5_STRUCT      MhMainCntl;

  MhMainCntl.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, MH_MAINCTL_PCU_FUN5_REG);
  MhMainCntl.Bits.mh_pin_unidirection_mode = 1;
  MhMainCntl.Bits.unidir_memhot0_to_memhot_out_en = 1;
  if (IsHbmSku()) {
    MhMainCntl.Bits.unidir_memhot1_to_memhot_out_en = 0;
  } else {
    MhMainCntl.Bits.unidir_memhot1_to_memhot_out_en = 1;
  }
  if ((ThermalThrottlingOptions & MH_INPUT_EN) != 0) {
    MhMainCntl.Bits.unidir_memhot_in_en = 1;
  } else {
    MhMainCntl.Bits.unidir_memhot_in_en = 0;
  }
  if ((ThermalThrottlingOptions & MH_OUTPUT_EN) != 0) {
    MhMainCntl.Bits.mh_output_en = 1; //When 1, MEM_HOT output generation logic is enabled.
  } else {
    MhMainCntl.Bits.mh_output_en = 0;
  }

  if (IsSiliconWorkaroundEnabled ("S1909247944") && (ThermalThrottlingOptions & MH_INPUT_EN) != 0) {
    // BIOS WA - ICXR-PM: SMI is not getting triggered on memhot assertion
    // when memhot is configured in unidirectional mode
    MhMainCntl.Bits.mh_sense_en = 1;
  }

  UsraCsrWrite (Socket, PCU_INSTANCE_0, MH_MAINCTL_PCU_FUN5_REG, MhMainCntl.Data);
  return;
}

/**

  Configures registers for Memory hot feature

  @param[in] socket  - Socket Id, 0 based

  @retval N/A

**/
VOID
ConfigureMemHotFeature (
  IN UINT8 Socket
  )
{
  struct sysHost *Host = NULL;
  struct channelNvram(*ChannelNvList)[MAX_CH];
  struct dimmNvram(*DimmNvList)[MAX_DIMM];
  SYS_SETUP *Setup;

  UINT8 Channel = 0;
  UINT8 Dimm = 0;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Configure MEMHOT
  //
  ProgramMemHotFeature (Socket, Setup->mem.thermalThrottlingOptions);
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList(Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      SetMemHotTemperatureRange (MemTechDdr, Socket, Channel, Dimm, Setup->mem.memhotOutputOnlyOpt);
    }//Dimm
  }//Channel
  return;
} //ConfigureMemHotFeature

/**

  This function Configures registers for Memory Trip feature

  @param[in]  Socket         Socket Number

  @retval N/A

**/
VOID
ConfigureMemTripFeature (
  IN UINT8 Socket
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  ConfigureOffPkgBits (Setup->mem.thermalThrottlingOptions, Socket);
  ConfigureInPkgBits (Setup->mem.thermalThrottlingOptions, Socket);

  return;
} // ConfigureMemTripFeature


/**

  Checks if close page mapping is enabled

  @param[in] socket   - Socket number, 0 based
  @param[in] Channel  - Channel number, 0 based

  @retval  TRUE  if close page mapping
  @retval  FALSE.if open page mapping

**/
BOOLEAN
EFIAPI
IsMcMtrClosePageMapping (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  MCMTR_MC_MAIN_STRUCT          McMtr;
  BOOLEAN                       IsClosePage;

  McMtr.Data = MemReadPciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG);
  IsClosePage =  (McMtr.Bits.close_pg == 1) ? TRUE : FALSE;
  return IsClosePage;
}

/**

  Gets\Sets Memory throttle AllowIsoCh bit

  @param[in] Socket              - Socket Id, 0 based
  @param[in] ChannelIdex         - Channel Id, 0 based
  @Param[in] GetValue            - If True, then function gets value, else sets value
  @param[in,out] ThrotAllowIsoCh - Value to be set\get

  @retval N/A

**/
VOID
GetSetMemThrotAllowIsoch (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN BOOLEAN GetValue,
  IN OUT BOOLEAN *ThrotAllowIsoch
  )
{
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;
  ChnTempCfgReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
  if (GetValue == TRUE)  {
    if (ChnTempCfgReg.Bits.thrt_allow_isoch == 1) {
      *ThrotAllowIsoch = TRUE;
    } else {
      *ThrotAllowIsoch = FALSE;
    }
  } else {
    if (*ThrotAllowIsoch) {
      ChnTempCfgReg.Bits.thrt_allow_isoch = 1;
    } else {
      ChnTempCfgReg.Bits.thrt_allow_isoch = 0;
    }
  }
  MemWritePciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
}


/**

  Configures Input sense assertion period

  @param[in]  Socket             - Socket Number

  @retval N/A

**/
VOID
EFIAPI
ConfigureMemHotSenseAssertPeriod (
  IN UINT8 Socket
  )
{
  MH_SENSE_500NS_PCU_FUN5_STRUCT              MhSense500nsReg;
  //
  // Initialize MH_SENSE_500NS register to recommended values
  //
  MhSense500nsReg.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, MH_SENSE_500NS_PCU_FUN5_REG);
  MhSense500nsReg.Bits.cnfg_500_nanosec = (GetBclkFreq (Socket)/2);
  UsraCsrWrite (Socket, PCU_INSTANCE_0, MH_SENSE_500NS_PCU_FUN5_REG, MhSense500nsReg.Data);

} // ConfigureMemHotSenseAsserPeriod

/**

  Initializes PCU BIOS SPARE2 register

  @param[in]  Socket             - Socket Number

  @retval N/A

**/
VOID
ConfigurePcuBiosSpare2Reg (
  IN UINT8 Socket
  )
{
  PCU_BIOS_SPARE2_PCU_FUN5_STRUCT             PcuBiosSpare2Reg;
  UINT8 DimmTypePresent;
  UINT8 LrDimmPresent;
  PSYSHOST Host;

  //Get sysHost Pointer
  Host = GetSysHostPointer ();
  DimmTypePresent = Host->nvram.mem.dimmTypePresent;
  LrDimmPresent = Host->nvram.mem.socket[Socket].lrDimmPresent;

  PcuBiosSpare2Reg.Data = UsraCsrRead (Socket, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG);
  PcuBiosSpare2Reg.Data = PcuBiosSpare2Reg.Data & ~(BIT15 + BIT14 + BIT13 + BIT12);
  if (DimmTypePresent == UDIMM) {
    PcuBiosSpare2Reg.Data = PcuBiosSpare2Reg.Data | PCU_BIOS_SPARE2_UDIMM;
  } else if ((DimmTypePresent == RDIMM) && (LrDimmPresent == 1)) {
    PcuBiosSpare2Reg.Data = PcuBiosSpare2Reg.Data | PCU_BIOS_SPARE2_LRDIMM;
  } else {
    // default to RDIMM
    PcuBiosSpare2Reg.Data = PcuBiosSpare2Reg.Data | PCU_BIOS_SPARE2_RDIMM;
  }
  PcuBiosSpare2Reg.Data = PcuBiosSpare2Reg.Data | PCU_BIOS_SPARE2_125V_DIMM;

  UsraCsrWrite (Socket, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG, PcuBiosSpare2Reg.Data);
}  // ConfigurePcuBiosSpare2Reg

/**

  Disables Throttling for safely initializing Memory with
  no interference from MPM

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based

  @retval N/A

**/
VOID
EFIAPI
DisableThrottling (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
    CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;
    ChnTempCfgReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
    ChnTempCfgReg.Bits.oltt_en = 0;
    ChnTempCfgReg.Bits.bw_limit_thrt_en = 0;
    ChnTempCfgReg.Bits.thrt_allow_isoch = 1;
    MemWritePciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);

}

/**

  Gets\Sets Bandwidth Window size for throttling

  @param[in] Socket                  - Socket Id, 0 based
  @param[in] Channel                 - Channel Id, 0 based
  @param[in] OperationMode           - FALSE = Operation Write, TRUE = Operation Read
  @param[in,out] BandWidthWindowSize - Variable to set\get value

  @retval N/A

**/
VOID
GetSetBandWidthThrottleWindowSize (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN BOOLEAN OperationMode,
  IN OUT UINT32 *BandWidthWindowSize
  )
{
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;
  ChnTempCfgReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
  if (OperationMode == READ_THROTTLE_VALUES)  {
    *BandWidthWindowSize =  ChnTempCfgReg.Bits.bw_limit_tf;
  } else {
    ChnTempCfgReg.Bits.bw_limit_tf = *BandWidthWindowSize;
    MemWritePciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
  }
}

/**

  Gets\sets External throttle value

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in,out] ThrotExternal      - Variable to set\get value

  @retval N/A

**/
VOID
GetSetThrottleExternal (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN BOOLEAN OperationMode,
  IN OUT UINT8 *ThrottleExternal
  )
{
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;
  ChnTempCfgReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
  if (OperationMode == READ_THROTTLE_VALUES)  {
    *ThrottleExternal =  (UINT8)ChnTempCfgReg.Bits.thrt_ext;
  } else {
    ChnTempCfgReg.Bits.thrt_ext = *ThrottleExternal;
    MemWritePciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
  }
}

/**

  Gets\sets Dimm termperature offset value

  @param[in] Socket                  - Socket Id, 0 based
  @param[in] Channel                 - Channel Id, 0 based
  @param[in] Dimm                    - Dimm Id, 0 based
  @param[in] OperationMode           - FALSE = Operation Write, TRUE = Operation Read
  @param[in,out] DimmTempOffsetValue - Temperature Offset value in degree celcius.

  @retval N/A

**/
VOID
GetSetThrottlingDimmTempOffsetValue (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN BOOLEAN OperationMode,
  IN OUT UINT8 *DimmTempOffsetValue
  )
{
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT  DimmTempEvOfst;
  UINT32  DimmTempEvOfsetRegOffset[] = {
    DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG,
    DIMM_TEMP_EV_OFST_1_MCDDC_CTL_REG
  };
  DimmTempEvOfst.Data = MemReadPciCfgEp (Socket, Channel, DimmTempEvOfsetRegOffset[Dimm]);
  if (OperationMode == READ_THROTTLE_VALUES)  {
    *DimmTempOffsetValue = (UINT8) DimmTempEvOfst.Bits.dimm_temp_offset;
  } else {
    DimmTempEvOfst.Bits.dimm_temp_offset = *DimmTempOffsetValue;
    MemWritePciCfgEp (Socket, Channel, DimmTempEvOfsetRegOffset[Dimm], DimmTempEvOfst.Data);
  }
}

/**

  Gets\sets OLTT Enable bit

  @param[in] MemTechType            - Memory technology type
  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in,out] OlttEnable         - Variable to set\get value

  @retval N/A

**/
VOID
GetSetOlttEnable (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          Channel,
  IN BOOLEAN        OperationMode,
  IN OUT BOOLEAN    *OlttEnable
  )
{
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;

  ChnTempCfgReg.Data = ChRegisterRead (MemTechType, Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
  if (OperationMode == READ_THROTTLE_VALUES)  {
    *OlttEnable =  ChnTempCfgReg.Bits.oltt_en == 1 ? TRUE: FALSE;
  } else {
    ChnTempCfgReg.Bits.oltt_en = *OlttEnable ? 1 : 0;
    ChRegisterWrite (MemTechType, Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
  }
}

/**

  Gets\sets temperature-based refresh enable bits

  @param[in]     MemTechType        - Memory technology type
  @param[in]     Socket             - Socket Id, 0 based
  @param[in]     Channel            - Channel Id, 0 based
  @param[in]     Dimm               - Dimm Id, 0 based
  @param[in]     OperationMode      - FALSE = Operation Write, TRUE = Operation Read
  @param[in]     ThrottleValueType  - Throttle value type to configure - see THROTTLE_VALUE_TYPE
  @param[in,out] ThermRefreshEnable - Variable to set\get refresh enable bit

  @retval EFI_INVALID_PARAMTER if paramter passed in is invalid
          EFI_SUCCESS otherwise

**/
EFI_STATUS
GetSetThermalRefreshEnables (
  IN     MEM_TECH_TYPE        MemTechType,
  IN     UINT8                Socket,
  IN     UINT8                Channel,
  IN     UINT8                Dimm,
  IN     BOOLEAN              OperationMode,
  IN     THROTTLE_VALUE_TYPE  ThrottleValueType,
  IN OUT BOOLEAN              *ThermRefreshEnable
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT  DimmTempEvOfst;
  UINT32                                DimmTempEvOfsetRegOffset[] = {
                                          DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG,
                                          DIMM_TEMP_EV_OFST_1_MCDDC_CTL_REG
                                        };

  if ((ThermRefreshEnable == NULL) ||
      (Dimm >= sizeof (DimmTempEvOfsetRegOffset) / sizeof (DimmTempEvOfsetRegOffset[0]))) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  DimmTempEvOfst.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempEvOfsetRegOffset[Dimm]);

  if (OperationMode == READ_THROTTLE_VALUES)  {

    switch (ThrottleValueType) {
      case EnumHalfxRefresh:
        if (DimmTempEvOfst.Bits.ev_halfx_ref_en == 1)  {
          *ThermRefreshEnable = TRUE;
        } else {
          *ThermRefreshEnable = FALSE;
        }
        break;

      case EnumTwoxRefresh:
        if (DimmTempEvOfst.Bits.ev_2x_ref_en == 1)  {
          *ThermRefreshEnable = TRUE;
        } else {
          *ThermRefreshEnable = FALSE;
        }
        break;

      case EnumFourxRefresh:
        if (DimmTempEvOfst.Bits.ev_4x_ref_en == 1)  {
          *ThermRefreshEnable = TRUE;
        } else {
          *ThermRefreshEnable = FALSE;
        }
        break;

      default:
        Status = EFI_INVALID_PARAMETER;
        ASSERT (FALSE);
        break;
    }

  } else { // OperationMode

    switch (ThrottleValueType) {
      case EnumHalfxRefresh:
        if (*ThermRefreshEnable)  {
          DimmTempEvOfst.Bits.ev_halfx_ref_en = 1;
        } else {
          DimmTempEvOfst.Bits.ev_halfx_ref_en = 0;
        }
        break;

      case EnumTwoxRefresh:
        if (*ThermRefreshEnable)  {
          DimmTempEvOfst.Bits.ev_2x_ref_en = 1;
        } else {
          DimmTempEvOfst.Bits.ev_2x_ref_en = 0;
        }
        break;

      case EnumFourxRefresh:
        if (*ThermRefreshEnable)  {
          DimmTempEvOfst.Bits.ev_4x_ref_en = 1;
        } else {
          DimmTempEvOfst.Bits.ev_4x_ref_en = 0;
        }
        break;

      default:
        Status = EFI_INVALID_PARAMETER;
        ASSERT (FALSE);
        break;
    }
    ChRegisterWrite (MemTechType, Socket, Channel, DimmTempEvOfsetRegOffset[Dimm], DimmTempEvOfst.Data);

  }

  return Status;
}

/**

  Gets\sets temperature-base refresh threshold values

  @param[in]     MemTechType            - Memory technology type
  @param[in]     Socket                 - Socket Id, 0 based
  @param[in]     Channel                - Channel Id, 0 based
  @param[in]     Dimm                   - Dimm Id, 0 based
  @param[in]     OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in]     ThrottleValueType      - Throttle value type to configure - see THROTTLE_VALUE_TYPE
  @param[in,out] RefreshTemperature     - Variable to set\get the refresh threshold temperature in Celcius

  @retval EFI_INVALID_PARAMTER if paramter passed in is invalid
          EFI_SUCCESS otherwise

**/
EFI_STATUS
GetSetThermalRefreshThresholds (
  IN      MEM_TECH_TYPE         MemTechType,
  IN      UINT8                 Socket,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm,
  IN      BOOLEAN               OperationMode,
  IN      THROTTLE_VALUE_TYPE   ThrottleValueType,
  IN OUT  UINT8                 *RefreshTemperature
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  DIMM_TEMP_REFRESH_0_MCDDC_CTL_STRUCT  DimmTempRefresh;
  UINT32                                DimmTempRefreshRegOffset[] = {
                                          DIMM_TEMP_REFRESH_0_MCDDC_CTL_REG,
                                          DIMM_TEMP_REFRESH_1_MCDDC_CTL_REG
                                          };

  if ((RefreshTemperature == NULL) ||
      (Dimm >= sizeof (DimmTempRefreshRegOffset) / sizeof (DimmTempRefreshRegOffset[0])))
  {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DimmTempRefresh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempRefreshRegOffset[Dimm]);

  if (OperationMode == READ_THROTTLE_VALUES)  {

    switch (ThrottleValueType) {
      case EnumHalfxRefreshValue:
        *RefreshTemperature = (UINT8)DimmTempRefresh.Bits.temp_halfxref;
        break;
      case EnumTwoxRefreshValue:
        *RefreshTemperature = (UINT8)DimmTempRefresh.Bits.temp_twoxref;
        break;
      case EnumFourxRefreshValue:
        *RefreshTemperature = (UINT8)DimmTempRefresh.Bits.temp_fourxref;
        break;
      default:
        Status = EFI_INVALID_PARAMETER;
        ASSERT (FALSE);
        break;
    }

  } else { // OperationMode

    switch (ThrottleValueType) {
      case EnumHalfxRefreshValue:
        DimmTempRefresh.Bits.temp_halfxref = *RefreshTemperature;
        break;
      case EnumTwoxRefreshValue:
        DimmTempRefresh.Bits.temp_twoxref = *RefreshTemperature;
        break;
      case EnumFourxRefreshValue:
        DimmTempRefresh.Bits.temp_fourxref = *RefreshTemperature;
        break;
      default:
        Status = EFI_INVALID_PARAMETER;
        ASSERT (FALSE);
        break;
    }
    ChRegisterWrite (MemTechType, Socket, Channel, DimmTempRefreshRegOffset[Dimm], DimmTempRefresh.Data);

  } // OperationMode

  return Status;
}

/**

  Get the PM_CMD_PWR register offset for a particular DIMM.

  This additionally provides bounds checking for the DIMM value, comparing it against the number of members in the
  PmCmdPwrRegOffsets array.

  @param[in]  Dimm  The target DIMM number

  @retval The PM_CMD_PWR register offset for the target DIMM

**/
UINT32
GetPmCmdPwrRegOffset (
  IN UINT8 Dimm
  )
{
  if (Dimm >= ((sizeof PmCmdPwrRegOffsets) / (sizeof PmCmdPwrRegOffsets[0]))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_162);
  }
  return PmCmdPwrRegOffsets[Dimm];
}

/**

  Gets\Sets Bandwidth Throttle limit Enable bit

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in,out] BwlimitThrotEnable - Variable to set\get value

  @retval N/A

**/
VOID
GetSetBwLimitThrottleEnable (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN BOOLEAN OperationMode,
  IN OUT BOOLEAN *BwlimitThrotEnable
  )
{
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT ChnTempCfgReg;
  ChnTempCfgReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
  if (OperationMode == READ_THROTTLE_VALUES)  {
    *BwlimitThrotEnable =  ChnTempCfgReg.Bits.bw_limit_thrt_en == 1 ? TRUE: FALSE;
  } else {
    ChnTempCfgReg.Bits.bw_limit_thrt_en = *BwlimitThrotEnable ? 1 : 0;
    MemWritePciCfgEp (Socket, Channel, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
  }
}

/**

  Gets\Sets current memory temperature of given dimm

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] Dimm                   - Dimm Id, 0 based
  @param[in] ValueType              - Value type to program. Check enum THROTTLE_VALUE_TYPE for more info.
  @param[in out] Value              - Value to Get\set

  @retval EFI_UNSUPPORTED             Value Type not supported
  @retval EFI_SUCCESS                 Value Get\set successfully.

**/
EFI_STATUS
EFIAPI
GetSetMemTempStatusRegValues (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN BOOLEAN OperationMode,
  IN THROTTLE_VALUE_TYPE ValueType,
  IN OUT UINT8 *Value
  )
{
  UINT32  DimmTempStatRegOffset[] = {DIMMTEMPSTAT_0_MCDDC_CTL_REG, DIMMTEMPSTAT_1_MCDDC_CTL_REG};
  DIMMTEMPSTAT_0_MCDDC_CTL_STRUCT  DimmTempStat;

  if (ValueType == EnumTempStatusCurrentDimmTemp) {
    DimmTempStat.Data = MemReadPciCfgEp (Socket, Channel, DimmTempStatRegOffset[Dimm]);
    if (OperationMode == READ_THROTTLE_VALUES)  {
      *Value = (UINT8)DimmTempStat.Bits.dimm_temp;
    } else {
      DimmTempStat.Bits.dimm_temp = *Value;
      MemWritePciCfgEp (Socket, Channel, DimmTempStatRegOffset[Dimm], DimmTempStat.Data);
    }
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**

  Gets\Sets memory throttling registers

  @param[in] MemTechType            - Memory technology type
  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] Dimm                   - Dimm Id, 0 based
  @param[in] ValueType              - Value type to program. Check enum THROTTLE_VALUE_TYPE for more info.
  @param[in] Value                  - Value to Get\set

  @retval EFI_UNSUPPORTED             Value not supported
  @retval EFI_SUCCESS                 Value Get\set successfully.

**/
EFI_STATUS
GetSetMemThrottleRegValues (
  IN MEM_TECH_TYPE       MemTechType,
  IN UINT8               Socket,
  IN UINT8               Channel,
  IN UINT8               Dimm,
  IN BOOLEAN             OperationMode,
  IN THROTTLE_VALUE_TYPE ValueType,
  IN UINT8               *Value
  )
{
  DIMM_TEMP_TH_0_MCDDC_CTL_STRUCT       DimmTempTh;
  DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_STRUCT DimmTempThrtLmt;
  DIMM_TEMP_REFRESH_0_MCDDC_CTL_STRUCT  DimmTempRefresh;
  UINT32  DimmTempThrtLmtRegOffset[] = { DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_REG, DIMM_TEMP_THRT_LMT_1_MCDDC_CTL_REG };
  UINT32  DimmTempThRegOffset[] = { DIMM_TEMP_TH_0_MCDDC_CTL_REG, DIMM_TEMP_TH_1_MCDDC_CTL_REG };
  UINT32  DimmTempRefRegOffset[] = { DIMM_TEMP_REFRESH_0_MCDDC_CTL_REG, DIMM_TEMP_REFRESH_1_MCDDC_CTL_REG};

  switch (ValueType) {
    case EnumTempThersholdLow:
      DimmTempTh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES)  {
        *Value = (UINT8)DimmTempTh.Bits.temp_lo;
      } else {
        DimmTempTh.Bits.temp_lo = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm], DimmTempTh.Data);
      }
      break;
    case EnumTempThersholdMid:
      DimmTempTh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES)  {
        *Value = (UINT8)DimmTempTh.Bits.temp_mid;
      } else {
        DimmTempTh.Bits.temp_mid = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm], DimmTempTh.Data);
      }
      break;
    case EnumTempThersholdHigh:
      DimmTempTh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES) {
        *Value = (UINT8)DimmTempTh.Bits.temp_hi;
      } else {
        DimmTempTh.Bits.temp_hi = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm], DimmTempTh.Data);
      }
      break;

    case EnumTempMemTrip:
      DimmTempRefresh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempRefRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES) {
        *Value = (UINT8)DimmTempRefresh.Bits.temp_memtrip;
      } else {
        DimmTempRefresh.Bits.temp_memtrip = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempRefRegOffset[Dimm], DimmTempRefresh.Data);
      }
      break;

    case EnumTempThrottleMid:
      DimmTempThrtLmt.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES)  {
        *Value = (UINT8)DimmTempThrtLmt.Bits.thrt_mid;
      } else {
        DimmTempThrtLmt.Bits.thrt_mid = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm], DimmTempThrtLmt.Data);
      }
      break;
    case EnumTempThrottleHigh:
      DimmTempThrtLmt.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES)  {
        *Value = (UINT8)DimmTempThrtLmt.Bits.thrt_hi;
      } else {
        DimmTempThrtLmt.Bits.thrt_hi = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm], DimmTempThrtLmt.Data);
      }
      break;
    case EnumTempThrottleCritical:
      DimmTempThrtLmt.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES) {
        *Value = (UINT8)DimmTempThrtLmt.Bits.thrt_crit;
      } else {
        DimmTempThrtLmt.Bits.thrt_crit = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThrtLmtRegOffset[Dimm], DimmTempThrtLmt.Data);
      }
      break;
    case EnumTempThrottleHysteresis:
      DimmTempTh.Data = ChRegisterRead (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm]);
      if (OperationMode == READ_THROTTLE_VALUES)  {
        *Value = (UINT8)DimmTempTh.Bits.temp_thrt_hyst;
      } else {
        DimmTempTh.Bits.temp_thrt_hyst = *Value;
        ChRegisterWrite (MemTechType, Socket, Channel, DimmTempThRegOffset[Dimm], DimmTempTh.Data);
      }
      break;
    default:
      return EFI_UNSUPPORTED;
    }
  return EFI_SUCCESS;
}

/**

  This function configure thermal management feature

  @param[in]  Socket                 - Socket Number, 0 based
  @param[in]  CfgMemThermFeatureType - Feature type to change
                                       Takes value from enum CONFIGURE_MEM_THERMAL_FEATURE_TYPE.

  @retval   EFI_SUCCESS       Feature Configured Properly
  @retval   EFI_UNSUPPORTED   Feature not supported

**/
EFI_STATUS
EFIAPI
ConfigureMemoryThermalFeature (
  IN UINT8 Socket,
  IN CONFIGURE_MEM_THERMAL_FEATURE_TYPE CfgMemThermFeatureType
  )
{
  switch (CfgMemThermFeatureType) {
    case ConfigureMemHotFeatureType:
      ConfigureMemHotFeature (Socket);
      break;
    case ConfigureMemTripFeatureType:
      ConfigureMemTripFeature (Socket);
      break;
    case ConfigurePcuBiosSpare2RegType:
      ConfigurePcuBiosSpare2Reg (Socket);
      break;
    default:
      return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**

  Gets\Sets various memory thermal throttle values

  @param[in] MemTechType            - Memory technology type.
  @param[in] Socket                 - Socket number, 0 based
  @param[in] Channel                - Channel Number, 0 based
  @param[in] Dimm                   - Dimm Number, 0 based
  @param[in] OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in] ThrottleValueType      - Throttle value type to configure.
                                    - For more information on this parameter refer enum THROTTLE_VALUE_TYPE
  @param[in out] ThrottleValue      - Structure to hold value to write or read.

  @retval   EFI_SUCCESS              Feature Configured Properly
  @retval   EFI_UNSUPPORTED          Feature not supported

**/
EFI_STATUS
EFIAPI
GetSetThrottleValues (
  IN MEM_TECH_TYPE       MemTechType,
  IN UINT8               Socket,
  IN UINT8               Channel,
  IN UINT8               Dimm,
  IN BOOLEAN             OperationMode,
  IN THROTTLE_VALUE_TYPE ThrottleValueType,
  IN OUT THROTTLE_VALUE  *ThrottleValue
  )
{
  switch (ThrottleValueType) {
    case EnumBwLimitThrottleEnable:
      GetSetBwLimitThrottleEnable (Socket, Channel, OperationMode, &(ThrottleValue->BwLimitThrottleEnable));
      break;
    case EnumOlttEnable:
      GetSetOlttEnable (MemTechType, Socket, Channel, OperationMode, &(ThrottleValue->OlttEnable));
      break;
    case EnumThrottleExternal:
      GetSetThrottleExternal (Socket, Channel, OperationMode, &(ThrottleValue->ThrottleExternal));
      break;
    case EnumThrottlingDimmTempOffsetValue:
      GetSetThrottlingDimmTempOffsetValue (Socket, Channel, Dimm, OperationMode, &(ThrottleValue->ThrottlingDimmTempOffsetValue));
      break;
    case EnumTwoxRefresh:
      GetSetThermalRefreshEnables (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TwoxRefresh));
      break;
    case EnumTwoxRefreshValue:
      GetSetThermalRefreshThresholds (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TwoxRefreshValue));
      break;
    case EnumFourxRefresh:
      GetSetThermalRefreshEnables (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->FourxRefresh));
      break;
    case EnumFourxRefreshValue:
      GetSetThermalRefreshThresholds (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->FourxRefreshValue));
      break;
    case EnumHalfxRefresh:
      GetSetThermalRefreshEnables (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->HalfxRefresh));
      break;
    case EnumHalfxRefreshValue:
      GetSetThermalRefreshThresholds (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->HalfxRefreshValue));
      break;
    case EnumBandWidthThrottleWindowSize:
      GetSetBandWidthThrottleWindowSize (Socket, Channel, OperationMode, &(ThrottleValue->BandWidthThrottleWindowSize));
      break;
    case EnumMemThrotAllowIsoch:
      GetSetMemThrotAllowIsoch (Socket, Channel, OperationMode, &(ThrottleValue->MemThrotAllowIsoch));
      break;
    case EnumTempThersholdLow:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThersholdLow));
      break;
    case EnumTempThersholdMid:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThersholdMid));
      break;
    case EnumTempThersholdHigh:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThersholdHigh));
      break;
    case EnumTempMemTrip:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempMemTrip));
      break;
    case EnumTempThrottleMid:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThrottleMid));
      break;
    case EnumTempThrottleHigh:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThrottleHigh));
      break;
    case EnumTempThrottleCritical:
      GetSetMemThrottleRegValues (MemTechType, Socket, Channel, Dimm, OperationMode, ThrottleValueType, &(ThrottleValue->TempThrottleCritical));
      break;
    default:
      return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**

  Disable all the channel based and dimm based DRAM throttling for eADR

  @param[in] Socket   - Socket index

  @retval None

**/
VOID
DisableDramThrottling (
  IN UINT8   Socket
  )
{
  PSYSHOST                          Host;
  UINT8                             Mc;
  UINT8                             Ch;
  UINT8                             NodeCh;
  UINT8                             NumChPerMc;
  UINT8                             MaxImc;

  NumChPerMc = GetNumChannelPerMc ();
  MaxImc = GetMaxImc ();
  Host = GetSysHostPointer ();

  for (Mc = 0; Mc < MaxImc; Mc++) {
    // Skip if Mc is not enabled
    if (Host->var.mem.socket[Socket].imcEnabled[Mc] == 0) {
      continue;
    }
    for (NodeCh = 0; NodeCh < NumChPerMc; NodeCh++) {
      Ch = NODECH_TO_SKTCH (SKTMC_TO_NODE (Socket, Mc), NodeCh);
      //
      // Disables Channel based Throttling
      //
      DisableThrottling (Socket, Ch);
    }
  }
}
