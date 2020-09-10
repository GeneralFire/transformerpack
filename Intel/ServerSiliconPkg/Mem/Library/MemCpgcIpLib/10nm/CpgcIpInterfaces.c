/** @file
  Interface source file for the HBM library class.

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

#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "CpgcChip.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemCpgcIpTargetLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRcLib.h>



/**

  Configure the CPGC Start/Wrap addresses

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                AddressSettings     Pointer to address settings
  @param[in]                ScaleRateSettings   Pointer to scale, rate and order settings

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupAddress (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        ADDRESS_SETTINGS    *AddressSettings,
  IN        SCALE_RATE_SETTINGS *ScaleRateSettings
  )
{
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     SeqBaseAddressStartLo;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     SeqBaseAddressStartHi;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      SeqBaseAddressWrapLo;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      SeqBaseAddressWrapHi;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            SeqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            SeqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               SeqBaoCic;
  SYSHOST                                       *Host;

  Host = GetSysHostPointer ();

  SeqBaseAddressStartLo.Data  = 0;
  SeqBaseAddressStartHi.Data  = 0;
  SeqBaseAddressWrapLo.Data   = 0;
  SeqBaseAddressWrapHi.Data   = 0;
  SeqBaincCtl00.Data          = 0;
  SeqBaincCtl10.Data          = 0;
  SeqBaoCic.Data              = 0;

  //
  // Start address configuration
  //
  SeqBaseAddressStartLo.Bits.bank_addr    = AddressSettings->StartList.Bits.BankAddr;
  SeqBaseAddressStartLo.Bits.column_addr  = AddressSettings->StartList.Bits.ColumnAddr;
  SeqBaseAddressStartHi.Bits.rank_addr    = AddressSettings->StartList2.Bits.RankAddr;
  SeqBaseAddressStartHi.Bits.row_addr     = AddressSettings->StartList2.Bits.RowAddr;
  //
  // Wrap address configuration
  //
  SeqBaseAddressWrapLo.Bits.bank_addr     = AddressSettings->WrapList.Bits.BankAddr;
  SeqBaseAddressWrapLo.Bits.column_addr   = AddressSettings->WrapList.Bits.ColumnAddr;
  SeqBaseAddressWrapHi.Bits.rank_addr     = AddressSettings->WrapList2.Bits.RankAddr;
  SeqBaseAddressWrapHi.Bits.row_addr      = AddressSettings->WrapList2.Bits.RowAddr;
  //
  // Scale and rate control
  //
  SeqBaincCtl00.Bits.column_ba_inc        = ScaleRateSettings->IncList2.Bits.Column;
  SeqBaincCtl00.Bits.column_ba_updrate    = ScaleRateSettings->RateList.Bits.Column;
  SeqBaincCtl00.Bits.column_ba_updscale   = ScaleRateSettings->ScaleList.Bits.Column;

  SeqBaincCtl00.Bits.row_ba_inc           = ScaleRateSettings->IncList3.Bits.Row;
  SeqBaincCtl00.Bits.row_ba_updrate       = ScaleRateSettings->RateList.Bits.Row;
  SeqBaincCtl00.Bits.row_ba_updscale      = ScaleRateSettings->ScaleList.Bits.Row;

  SeqBaincCtl10.Bits.bank_ba_inc          = ScaleRateSettings->IncList.Bits.Bank;
  SeqBaincCtl10.Bits.bank_ba_updrate      = ScaleRateSettings->RateList.Bits.Bank;
  SeqBaincCtl10.Bits.bank_ba_updscale     = ScaleRateSettings->ScaleList.Bits.Bank;

  SeqBaincCtl10.Bits.column_ba_inc_msb    = ScaleRateSettings->IncList2.Bits.ColumnMsb;

  SeqBaincCtl10.Bits.rank_ba_inc          = ScaleRateSettings->IncList.Bits.Rank;
  SeqBaincCtl10.Bits.rank_ba_updrate      = ScaleRateSettings->RateList.Bits.Rank;
  SeqBaincCtl10.Bits.rank_ba_updscale     = ScaleRateSettings->ScaleList.Bits.Rank;
  //
  // Control order
  //
  SeqBaoCic.Bits.bank_addr_order          = ScaleRateSettings->OrderList.Bits.Bank;
  SeqBaoCic.Bits.column_addr_order        = ScaleRateSettings->OrderList.Bits.Column;
  SeqBaoCic.Bits.rank_addr_order          = ScaleRateSettings->OrderList.Bits.Rank;
  SeqBaoCic.Bits.row_addr_order           = ScaleRateSettings->OrderList.Bits.Row;
  //
  // CPGC Register accesses
  //
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_REG, 4, SeqBaseAddressStartLo.Data);
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, 4, SeqBaseAddressStartHi.Data);
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_REG, 4, SeqBaseAddressWrapLo.Data);
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 4, SeqBaseAddressWrapHi.Data);

  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAINCCTL00_MC_MAIN_REG, 8, SeqBaincCtl00.Data);
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAINCCTL10_MC_MAIN_REG, 8, SeqBaincCtl10.Data);

  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAOCIC0_MC_MAIN_REG, 4, SeqBaoCic.Data);

  return EFI_SUCCESS;
} // SetupAddress

/**
  Setups the CADB Write Pointer

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Value               Value to setup the CADB write pointer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupCadbWritePointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Value
  )
{
  SYSHOST     *Host;

  Host = GetSysHostPointer ();

  //
  // Set the write pointer
  //
  MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, Value);

  return EFI_SUCCESS;
}

/**
  Appends a CADB line

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                CadbPattern         Value to setup the CADB write pointer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
AppendCadbLine (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        CADB_CMD_PATTERN  CadbPattern
  )
{
  SYSHOST     *Host;
  CPGC_PATCADBPROG0_MCDDC_CTL_STRUCT  PatternCadbProg0;

  Host = GetSysHostPointer ();

  PatternCadbProg0.Data           = 0;
#ifndef DDR5_SUPPORT
  //
  // Cke
  //
  PatternCadbProg0.Bits.hbm_data2 |= CadbPattern.List.Bits.Cke & 0x1;
  //
  // Row address failing
  //
  PatternCadbProg0.Bits.cid       |= (CadbPattern.List.Bits.RowAddrF & 0x1) << 2;
  PatternCadbProg0.Bits.bank      |= (CadbPattern.List.Bits.RowAddrF) >> 1;
#endif
  //
  // Row address rising
  //
  PatternCadbProg0.Bits.address   |= (CadbPattern.List.Bits.RowAddrR & 0x03) << 16;
#ifndef DDR5_SUPPORT
  PatternCadbProg0.Bits.hbm_data  |= (CadbPattern.List.Bits.RowAddrR & 0x0C) >> 2;
  PatternCadbProg0.Bits.cid       |= (CadbPattern.List.Bits.RowAddrR & 0x30) >> 4;
#endif
  //
  // Column address failing
  //
  PatternCadbProg0.Bits.address   |= CadbPattern.List.Bits.ColAddrF << 8;
  //
  // Column address rising
  //
  PatternCadbProg0.Bits.address   |= CadbPattern.List.Bits.ColAddrR;

  MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBPROG0_MCDDC_CTL_REG, PatternCadbProg0.Data);

  return EFI_SUCCESS;
}

/**
  Enables or disables CADB mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) CADB mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableCadb (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  )
{
  SYSHOST     *Host;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT    CpgcPatCadbCtlReg;

  Host = GetSysHostPointer ();

  CpgcPatCadbCtlReg.Data = 0;
  if (Enable) {
    CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG);;
    CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect      = 1;
  }

  MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

  return EFI_SUCCESS;
}

/**
  Get errors from CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Strobe              Current Strobe (0-Based)
  @param[out]               CpgcErrorStatus     Pointer to Cpgc error Status per channel/strobe

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
  @retval                   EFI_UNSUPPORTED     Strobe index is not supported

**/
EFI_STATUS
EFIAPI
GetCpgcErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
      OUT   CPGC_ERRORS     *CpgcErrorStatus
  )
{

  SYSHOST     *Host;
  UINT32      EccCpgcError;
  UINT32      OddCpgcError;
  UINT32      EveCpgcError;

  Host = GetSysHostPointer ();

  EccCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_ECC_S_MCDDC_CTL_REG);
  switch (Strobe) {
  case 0:
    EveCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA0_S_MCDDC_CTL_REG);
    OddCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA2_S_MCDDC_CTL_REG);
    EccCpgcError = (EccCpgcError >> 0)  & 0xF;
    break;
  case 1:
    EveCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA1_S_MCDDC_CTL_REG);
    OddCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA3_S_MCDDC_CTL_REG);
    EccCpgcError = (EccCpgcError >> 4)  & 0xF;
    break;
  case 2:
    EveCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA4_S_MCDDC_DP_REG);
    OddCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA6_S_MCDDC_DP_REG);
    EccCpgcError = (EccCpgcError >> 8)  & 0xF;
    break;
  case 3:
    EveCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA5_S_MCDDC_DP_REG);
    OddCpgcError = MemReadPciCfgEp (Socket, Channel, CPGC_ERR_DATA7_S_MCDDC_DP_REG);
    EccCpgcError = (EccCpgcError >> 12) & 0xF;
    break;
  default:
    return EFI_UNSUPPORTED;
  }

  CpgcErrorStatus->CpgcEvenErrors  = EveCpgcError;
  CpgcErrorStatus->CpgcOddErrors   = OddCpgcError;
  CpgcErrorStatus->CpgcEccErrors   = (UINT8) EccCpgcError;


  return EFI_SUCCESS;
}

/**
  Clear errors from CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
ClearCpgcErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  SYSHOST     *Host;

  Host = GetSysHostPointer ();

  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA0_S_MCDDC_CTL_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA1_S_MCDDC_CTL_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA2_S_MCDDC_CTL_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA3_S_MCDDC_CTL_REG, 0);

  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA4_S_MCDDC_DP_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA5_S_MCDDC_DP_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA6_S_MCDDC_DP_REG, 0);
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA7_S_MCDDC_DP_REG, 0);

  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_ECC_S_MCDDC_CTL_REG,  0);

  return EFI_SUCCESS;
}

/**
  Setup the error mask in the CPGC registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Strobe              Current Strobe (0-Based)
  @param[in]                Mask                Bits to be masked

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
  @retval                   EFI_UNSUPPORTED     Strobe index is not supported

**/
EFI_STATUS
EFIAPI
SetupErrorMask (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           Strobe,
  IN        UINT32          Mask
  )
{
  SYSHOST     *Host;
  UINT32      RegisterOffset [4] = {CPGC_ERR_DATA0_MCDDC_CTL_REG, CPGC_ERR_DATA1_MCDDC_CTL_REG, CPGC_ERR_DATA2_MCDDC_DP_REG, CPGC_ERR_DATA3_MCDDC_DP_REG};

  Host = GetSysHostPointer ();

  if (Strobe >= 4) {
    return EFI_UNSUPPORTED;
  }

  MemWritePciCfgEp (Socket, Channel, RegisterOffset[Strobe], Mask);

  return EFI_SUCCESS;
}

/**
  Mask ECC errors

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) CADB mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
MaskEccErrors (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         EnableMask
  )
{
  SYSHOST     *Host;
  UINT32      Mask;

  Host = GetSysHostPointer ();

  if (EnableMask) {
    Mask = 0xFFFFFFFF;
  } else {
    Mask = 0;
  }

  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_ECC_MCDDC_CTL_REG, Mask);

  return EFI_SUCCESS;
}

/**
  Get or set Cpgc BL4

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                GetSet              Specify (GET or SET)_CPGC_LIB command
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetSetCpgcBl4 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{
  SYSHOST     *Host;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT  CpgcGlobalCtrlReg;

  Host = GetSysHostPointer ();

  CpgcGlobalCtrlReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
  if (GetSet == GET_CPGC_LIB) {
    Global->SettingList.Bits.Bl4Support  = CpgcGlobalCtrlReg.Bits.use_bl4_col_addr;
  } else {
    CpgcGlobalCtrlReg.Bits.use_bl4_col_addr = Global->SettingList.Bits.Bl4Support;
    MemWritePciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, CpgcGlobalCtrlReg.Data);
  }

  return EFI_SUCCESS;
}

/**
  Get or set cpgc status (start test and clear errors)

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                GetSet              Specify (GET or SET)_CPGC_LIB command
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetSetCpgcTestStatus (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           GetSet,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{
  SYSHOST     *Host;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT  CpgcGlobalCtrlReg;

  Host = GetSysHostPointer ();

  CpgcGlobalCtrlReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
  if (GetSet == GET_CPGC_LIB) {
    Global->SettingList.Bits.StartTest    = CpgcGlobalCtrlReg.Bits.start_test;
    Global->SettingList.Bits.ClearErrors  = CpgcGlobalCtrlReg.Bits.clear_errs;
  } else {
    CpgcGlobalCtrlReg.Bits.start_test = Global->SettingList.Bits.StartTest;
    CpgcGlobalCtrlReg.Bits.clear_errs = Global->SettingList.Bits.ClearErrors;

    //
    // slight delay to allow everything to settle before starting.
    //

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (10);

    MemWritePciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, CpgcGlobalCtrlReg.Data);
  }

  return EFI_SUCCESS;
}

/**
  Stops any cpgc test

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
StopCpgcTests (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  SYSHOST     *Host;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT  CpgcGlobalCtrlReg;

  Host = GetSysHostPointer ();

  CpgcGlobalCtrlReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
  CpgcGlobalCtrlReg.Bits.stop_test  = 1;
  CpgcGlobalCtrlReg.Bits.start_test = 0;
  MemWritePciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, CpgcGlobalCtrlReg.Data);

  return EFI_SUCCESS;

}
/**
  Get all the test done available bits

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in,out]            Global              Pointer to Get/Set value for global features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
GetCpgcTestDone (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN  OUT   GLOBAL_FEATURE  *Global
  )
{
  SYSHOST     *Host;
  ALIAS_CPGC_GLOBALERR_MC_MAIN_STRUCT  CpgcGlobalErrReg;

  Host = GetSysHostPointer ();

  CpgcGlobalErrReg.Data = MemReadPciCfgEp (Socket, Channel, ALIAS_CPGC_GLOBALERR_MC_MAIN_REG);

  Global->SettingList.Bits.TestDone0  = CpgcGlobalErrReg.Bits.channel_test_done_status_0;
  Global->SettingList.Bits.TestDone1  = CpgcGlobalErrReg.Bits.channel_test_done_status_1;
  Global->SettingList.Bits.TestDone2  = CpgcGlobalErrReg.Bits.channel_test_done_status_2;

  return EFI_SUCCESS;
}


/**
  Setup Subsequence

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature
  @param[in]                SubsequenceOffset   Offset for specified subsequence

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/

EFI_STATUS
SetupSubsequence (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures,
  IN        UINT32          SubsequenceOffset
  )
{
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT  CpgcSubseqCtl;
  SYSHOST                           *Host;


  Host = GetSysHostPointer ();

  CpgcSubseqCtl.Data = MemReadPciCfgEp (Socket, Channel, SubsequenceOffset);
  CpgcSubseqCtl.Bits.num_cachelines       = SubsequenceFeatures->DataList.Bits.Cachelines;
  CpgcSubseqCtl.Bits.rstcurr_ba_start     = SubsequenceFeatures->AddressList.Bits.ResetAddress;
  CpgcSubseqCtl.Bits.savecurr_ba_start    = SubsequenceFeatures->AddressList.Bits.SaveAddress;
  CpgcSubseqCtl.Bits.num_cachelines_scale = 0;
  CpgcSubseqCtl.Bits.subseq_wait          = 0;
  CpgcSubseqCtl.Bits.inv_dataecc          = 0;
  if (SubsequenceFeatures->ControlList.Bits.Type == READ_SUBSEQ_TYPE_CPGC_LIB) {
    CpgcSubseqCtl.Bits.subseq_type        = 0;
  } else if (SubsequenceFeatures->ControlList.Bits.Type == WRITE_SUBSEQ_TYPE_CPGC_LIB) {
    CpgcSubseqCtl.Bits.subseq_type        = 1;
  }


  MemWritePciCfgEp (Socket, Channel, SubsequenceOffset, CpgcSubseqCtl.Data);

  return EFI_SUCCESS;
}

/**
  Setup Subsequence 0

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupSubsequence0 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  )
{
  return SetupSubsequence (Socket, Channel, SubsequenceFeatures, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG);
}

/**
  Setup Subsequence 1

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SubsequenceFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupSubsequence1 (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_FEATURE *SubsequenceFeatures
  )
{
  return SetupSubsequence (Socket, Channel, SubsequenceFeatures, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG);
}

/**
  Setup Loopcount and Update Rate Mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SequenceGlobalFeatures Pointer to Subsequence feature

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupLoopCount (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  )
{
  SYSHOST                         *Host;
  CPGC_SEQCTL0_MC_MAIN_STRUCT     CpgcSeqCtl0Reg;

  Host = GetSysHostPointer ();

  CpgcSeqCtl0Reg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG);
  CpgcSeqCtl0Reg.Bits.loopcount                 = SequenceChannelFeatures->TestList.Bits.Loopcount + 1;
  CpgcSeqCtl0Reg.Bits.address_update_rate_mode  = SequenceChannelFeatures->AddressList.Bits.UpdateRateMode;
  MemWritePciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG, CpgcSeqCtl0Reg.Data);

  return EFI_SUCCESS;
}

/**
  Enable / Disable CPGC in Native Mode

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                MemSs               MC ID
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) to operate in native mode

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableCpgcInNativeMode (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           MemSs,
  IN        BOOLEAN         Enable
  )
{
  SetCpgcTrainMode (Socket, MemSs, Channel, Enable);

  return EFI_SUCCESS;
}


/**
  Enable / Disable Global Support

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Enable(TRUE) / Disable(FALSE) support for global execution

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
EnableGlobalSupport (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  )
{
  SYSHOST                         *Host;
  CPGC_SEQCTL0_MC_MAIN_STRUCT     CpgcSeqCtl0Reg;

  Host = GetSysHostPointer ();

  CpgcSeqCtl0Reg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG);
  if (Enable) {
    CpgcSeqCtl0Reg.Bits.global_control  = 1;
  } else {
    CpgcSeqCtl0Reg.Bits.global_control  = 0;
  }

  MemWritePciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG, CpgcSeqCtl0Reg.Data);

  return EFI_SUCCESS;
}

/**
  Setup channel sequence configuration

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                SequenceChannelFeatures Pointer to Subsequence per channel features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupChannelSequence (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        SUBSEQUENCE_CHANNEL_FEATURE *SequenceChannelFeatures
  )
{
  SYSHOST                         *Host;
  CPGC_SEQCTL0_MC_MAIN_STRUCT     CpgcSeqCtl0Reg;

  Host = GetSysHostPointer ();

  CpgcSeqCtl0Reg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG);
  CpgcSeqCtl0Reg.Bits.global_control            = SequenceChannelFeatures->TestList.Bits.GlobalControl;
  if (SequenceChannelFeatures->TestList.Bits.Mode == TESTING_CPGC_LIB) {
    CpgcSeqCtl0Reg.Bits.initialization_mode     = 1;
  }

  CpgcSeqCtl0Reg.Bits.loopcount                 = SequenceChannelFeatures->TestList.Bits.Loopcount;

  CpgcSeqCtl0Reg.Bits.address_update_rate_mode  = SequenceChannelFeatures->AddressList.Bits.UpdateRateMode;
  CpgcSeqCtl0Reg.Bits.subseq_start_pointer      = SequenceChannelFeatures->AddressList.Bits.StartSequence;
  CpgcSeqCtl0Reg.Bits.subseq_end_pointer        = SequenceChannelFeatures->AddressList.Bits.EndSequence;
  CpgcSeqCtl0Reg.Bits.start_test_delay          = 0;
  CpgcSeqCtl0Reg.Bits.link_wrtrn_enb            = 0;

  MemWritePciCfgEp (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG, CpgcSeqCtl0Reg.Data);

  return EFI_SUCCESS;
}

/**
  Configure patterns, pattern type and others

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                PatternBuffers      Pointer to Pattern Buffer

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetupPatternWdbMux (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        PATTERN_BUFFERS *PatternBuffers
  )
{
  SYSHOST                                 *Host;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT    CpgcPatWdbClMuxCtl;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT       CpgcPatWdbClCtl;

  Host = GetSysHostPointer ();

  CpgcPatWdbClMuxCtl.Data                       = 0;
  if (PatternBuffers->MuxList.Bits.MuxSelect == PATBUF_CPGC_LIB) {
    CpgcPatWdbClMuxCtl.Bits.mux0  = 1;
    CpgcPatWdbClMuxCtl.Bits.mux1  = 1;
    CpgcPatWdbClMuxCtl.Bits.mux2  = 1;
  } else if (PatternBuffers->MuxList.Bits.MuxSelect == LFSR24_CPGC_LIB) {
    CpgcPatWdbClMuxCtl.Bits.mux0  = 2;
    CpgcPatWdbClMuxCtl.Bits.mux1  = 2;
    CpgcPatWdbClMuxCtl.Bits.mux2  = 2;
  }
  CpgcPatWdbClMuxCtl.Bits.ecc_replace_byte      = 1;
  CpgcPatWdbClMuxCtl.Bits.ecc_datasrc_sel       = 0;
  CpgcPatWdbClMuxCtl.Bits.save_lfsr_seedrate    = 0;
  CpgcPatWdbClMuxCtl.Bits.rld_lfsr_seedrate     = 0;
  CpgcPatWdbClMuxCtl.Bits.cpgc_lfsr_2code_mode  = 0;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, CpgcPatWdbClMuxCtl.Data);

  CpgcPatWdbClCtl.Data              = 0;
  CpgcPatWdbClCtl.Bits.incr_rate    = PatternBuffers->MuxList.Bits.IncRate;
  CpgcPatWdbClCtl.Bits.incr_scale   = PatternBuffers->MuxList.Bits.IncScale;
  CpgcPatWdbClCtl.Bits.start_pntr   = PatternBuffers->MuxList.Bits.StartPointer;
  CpgcPatWdbClCtl.Bits.end_pntr     = PatternBuffers->MuxList.Bits.EndPointer;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, CpgcPatWdbClCtl.Data);

  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, PatternBuffers->WriteList0.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, PatternBuffers->WriteList1.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, PatternBuffers->WriteList2.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, PatternBuffers->ReadList0.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, PatternBuffers->ReadList1.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, PatternBuffers->ReadList2.Data);

  return EFI_SUCCESS;
}

/**

  Configures the WDB Rd/Wr pointer and Write pointers in fast Mode

  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                WdbRdWrPointer      Pointer to pointer and subpointers
  @param[in]                Hi                  Hi 32 bits
  @param[in]                Lo                  Low 32 bits

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
FastProgramWdb (
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer,
  IN        UINT32          Hi,
  IN        UINT32          Lo
  )
{
  FastProgramWdbTarget (Channel, WdbRdWrPointer, Hi, Lo);

  return EFI_SUCCESS;
}

/**

  Configures the WDB Rd/Wr pointer

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                WdbRdWrPointer      Pointer to pointer and subpointers

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
WdbRdWrPointer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        RDWR_POINTER    *WdbRdWrPointer
  )
{
  SYSHOST                               *Host;
  CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_STRUCT PatWdbRdWrPntr;

  Host = GetSysHostPointer ();

  PatWdbRdWrPntr.Data               = 0;
  PatWdbRdWrPntr.Bits.rdwr_pntr     = WdbRdWrPointer->List.Bits.Pointer;
  PatWdbRdWrPntr.Bits.rdwr_subpntr  = WdbRdWrPointer->List.Bits.SubPointer;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG, PatWdbRdWrPntr.Data);

  return EFI_SUCCESS;
}


/**

  Configures the WDB Write buffers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Lo                  Low 32 bits
  @param[in]                Hi                  Hi 32 bits

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
WdbBuffer (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Lo,
  IN        UINT32          Hi
  )
{
  SYSHOST     *Host;

  Host = GetSysHostPointer ();

  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_WR0_MCDDC_DP_REG, Lo);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_WR1_MCDDC_DP_REG, Hi);

  return EFI_SUCCESS;
}

/**

  Configures the rank logical mapping

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                RankLogicalMapping  Pointer to Rank logical mapping

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupRankLogicalMap (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        RANK_LOGICAL_MAPPING    *RankLogicalMapping
  )
{
  SYSHOST     *Host;
  CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_STRUCT  RankLogicalMap;

  Host = GetSysHostPointer ();

  RankLogicalMap.Data = 0;
  RankLogicalMap.Bits.rank0_mapping = RankLogicalMapping->List.Bits.Rank0Map;
  MemWritePciCfgEp (Socket, Channel, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, RankLogicalMap.Data);

  return EFI_SUCCESS;
}


/**

  Configures the error logging registers

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                ErrorLoggingFeature Pointer to Error logging features

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupErrorLogging (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        ERROR_LOGGING_FEATURE    *ErrorLoggingFeature
  )
{
  SYSHOST     *Host;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT  CpgcErrCtlReg;

  Host = GetSysHostPointer ();

  CpgcErrCtlReg.Data = 0;
  CpgcErrCtlReg.Bits.stop_on_ntherr           = 1;
  CpgcErrCtlReg.Bits.selective_err_enb_cl     = 0xFF;
  CpgcErrCtlReg.Bits.selective_err_enb_chunk  = 0xFF;
  if (ErrorLoggingFeature->List.Bits.StopOnErrorCmd ==  ERROR_STOP_ON_ALL) {
    CpgcErrCtlReg.Bits.stop_on_errcontrol     = 3;
  } else if (ErrorLoggingFeature->List.Bits.StopOnErrorCmd ==  ERROR_STOP_ON_SINGLE) {
    CpgcErrCtlReg.Bits.stop_on_errcontrol     = 1;
  }

  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_CTL_MCDDC_CTL_REG, CpgcErrCtlReg.Data);

  return EFI_SUCCESS;
}


/**

  Configures Misc Cpgc options

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                RefMask             Refresh Mask
  @param[in]                ZqMask              Zq Calibration Mask

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           RefMask,
  IN        UINT8           ZqMask
  )
{
  SYSHOST     *Host;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT    CpgcMiscCkeCtlReg;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    CpgcMiscRefCtlReg;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT     CpgcMiscZqCtlReg;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT    CpgcMiscOdtCtlReg;

  Host = GetSysHostPointer ();

  //
  // MC Controller Init - disable miscellaneous events
  // functional CKE logic, do not disable CKE powerdown,
  // disable refreshes for all ranks
  // disable ZQCal for all ranks
  //
  CpgcMiscCkeCtlReg.Data  = MemReadPciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  CpgcMiscRefCtlReg.Data  = MemReadPciCfgEp (Socket, Channel, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  CpgcMiscZqCtlReg.Data   = MemReadPciCfgEp (Socket, Channel, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  CpgcMiscOdtCtlReg.Data  = MemReadPciCfgEp (Socket, Channel, CPGC_MISCODTCTL_MCDDC_CTL_REG);

  CpgcMiscCkeCtlReg.Bits.cke_override     = 0;
  CpgcMiscRefCtlReg.Bits.ref_rankmask     = RefMask;
  CpgcMiscZqCtlReg.Bits.zq_rankmask       = ZqMask;
  CpgcMiscOdtCtlReg.Bits.odt_override     = 0;
  CpgcMiscOdtCtlReg.Bits.mpr_train_ddr_on = 0;

  if (!IsBrsPresent(Host, Socket)) {
  MemWritePciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG, CpgcMiscCkeCtlReg.Data);
  }
  MemWritePciCfgEp (Socket, Channel, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_MISCZQCTL_MCDDC_CTL_REG, CpgcMiscZqCtlReg.Data);
  MemWritePciCfgEp (Socket, Channel, CPGC_MISCODTCTL_MCDDC_CTL_REG, CpgcMiscOdtCtlReg.Data);

  return EFI_SUCCESS;
}

/**

  Configures HBM Misc Cpgc options

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
SetupHbmMiscCpgc (
  IN        UINT8           Socket,
  IN        UINT8           Channel
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT   CpgcHbmMisc;
  SYSHOST                         *Host;

  Host = GetSysHostPointer ();
  CpgcHbmMisc.Data  = MemReadPciCfgEp (Socket, Channel, CPGC_HBM_MISC_MCDDC_DP_REG);
  CpgcHbmMisc.Bits.hbm_invert_pseudo_chan  = 1;
  CpgcHbmMisc.Bits.hbm_dparity_datasrc_sel = 0;
  CpgcHbmMisc.Bits.hbm_dbi_datasrc_sel     = 0;
  CpgcHbmMisc.Bits.ignore_hbm_derr         = 1;
  MemWritePciCfgEp (Socket, Channel, CPGC_HBM_MISC_MCDDC_DP_REG, CpgcHbmMisc.Data);

  return EFI_SUCCESS;
}

/**

  Get Write Leveling Fly by Loop Count.

  @param[out]  LoopCount     Write Leveling Fly by Loop Count.

  @retval      EFI_SUCCESS   This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
GetWlFlyByLoopCount (
  OUT  UINT32  *LoopCount
  )
{
  *LoopCount = WL_FLY_BY_LC_DDR5;

  return EFI_SUCCESS;
}
