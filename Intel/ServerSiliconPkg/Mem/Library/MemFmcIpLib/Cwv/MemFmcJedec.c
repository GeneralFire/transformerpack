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

#include "Include/MemFmcRegs.h"
#include "Include/MemFmcJedec.h"
#include <Library/MemFmcIpLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Library/MemRcLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemoryServicesLib.h>
#include <Library/IoAccessLib.h>

struct MraRemapTable CwvMraMap [MAX_MRA_REMAP] = {
  // Jedec Address    Cw Bit    Cwv Address
  {   0x00,            0x1,        0x14}, // RW00
  {   0x01,            0x1,        0x15}, // RW01
  {   0x02,            0x1,        0x16}, // RW02
  {   0x03,            0x1,        0x17}, // RW03
  {   0x04,            0x1,        0x18}, // RW04
  {   0x02,            0x0,        0x19}, // MR02
  {   0x19,            0x0,        0x1A}, // MR25
  {   0x1A,            0x0,        0x1B}, // MR26
  {   0x1B,            0x0,        0x1C}, // MR27
  {   0x1C,            0x0,        0x1D}, // MR28
  {   0x1E,            0x0,        0x1E}, // MR30
  {   0x08,            0x0,        0x1F}, // MR08
  {   0x28,            0x0,        0x77}, // MR40
  {   0xFF,            0xF,        0x76}  // HSIO Training
};

struct SnoopMapTable CwvSnoopMapTable [MAX_SNOOP_SETTING] = {
  // DRAM Address                   DB Address
  // The data format for BL and CL of MR0 in DDR5 DRAM and CWV is different, which cannot be handled in CwvSnoopMapTable.
  // So program that in ProgramMr0Ddrt2
  //  {   DDR5_MODE_REGISTER_0_REG,     LRDIMM_RWE0},
  {   DDR5_MODE_REGISTER_8_REG,     LRDIMM_RWE1},
  {   DDR5_MODE_REGISTER_25_REG,    LRDIMM_RWE2},
  {   DDR5_MODE_REGISTER_26_REG,    LRDIMM_RWE3},
  {   DDR5_MODE_REGISTER_27_REG,    LRDIMM_RWE4},
  {   DDR5_MODE_REGISTER_28_REG,    LRDIMM_RWE5},
  {   DDR5_MODE_REGISTER_30_REG,    LRDIMM_RWE6},
  {   DDR5_MODE_REGISTER_45_REG,    LRDIMM_RWE7},
  {   DDR5_MODE_REGISTER_50_REG,    LRDIMM_RWE8},
};

/**

  Programs CAS Latency for DDR5 ranks

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number
  @param[in] Dimm    - Dimm number (zero-based)
  @param[in] Rank    - Rank number (zero-based)

  @retval N/A

**/
VOID
ProgramMr0Ddrt2 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                                         CASLatency;
  UINT8                                         CASTemp;
  UINT8                                         CWLTemp;
  INT16                                         BufferCas;
  DDRT_HSIO_TRAINING_MR0_CWV_DDRIO_HSIO_STRUCT  Ddrt2HsioMr0;
  DDRT_HSIO_TRAINING_MR2_CWV_DDRIO_HSIO_STRUCT  Ddrt2HsioMr2;
  DDR5_MODE_REGISTER_0_STRUCT                   Mr0Struct;

  //
  // Grabbing Read and Write Latencies from table
  //
  GetDdrtCasLatencies (Socket, Ch, Dimm, &CASLatency, NULL, &CWLTemp, NULL);
  //
  // CAS latency has a +22 offset
  //
  CASTemp = (CASLatency - DDRT_CAS_LATENCY_OFFSET) / DDRT_CAS_LATENCY_RL_STEP_SIZE;

  Ddrt2HsioMr0.Data = 0;
  //
  // CAS latency is split into 2 fields. tcl_gnt_data_0 is BIT0
  // tcl_gnt_data is BIT4:BIT1
  //
  Ddrt2HsioMr0.Bits.tcl_gnt_data_0 = CASTemp & 0x1;
  Ddrt2HsioMr0.Bits.tcl_gnt_data   = CASTemp >> 1;

  //
  // CWL latency has a +20 offset
  //
  CWLTemp = (CWLTemp - DDRT_CWL_LATENCY_OFFSET) / DDRT_CWL_LATENCY_RL_STEP_SIZE;
  Ddrt2HsioMr2.Data = 0;
  Ddrt2HsioMr2.Bits.tcwl = CWLTemp;

  DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, (UINT8) Ddrt2HsioMr0.Data);
  //
  // Program direct snoop settings to DQ buffer for BL and CL
  //
  BufferCas = (CASLatency - DDRT_CAS_LATENCY_OFFSET - 10) / DDRT_CAS_LATENCY_RL_STEP_SIZE;
  if (BufferCas < 0) {
    BufferCas = 0;
  }

  Mr0Struct.Data = 0;
  Mr0Struct.Bits.burst_length = 0;  // DDRT2 BL is 16, encoding 0
  Mr0Struct.Bits.cas_latency_rl = (UINT8)BufferCas;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "ProgramSnoopSettingsToDb: DRAM Address 0x%02X, DB Address 0x%02X, Data 0x%02X\n", DDR5_MODE_REGISTER_0_REG, LRDIMM_RWE0, Mr0Struct.Data);
  DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);
  DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWE0, Mr0Struct.Data);

  DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, (UINT8) Ddrt2HsioMr2.Data);
}

/**
  Enables Write Leveling Mode for DDRT2 ranks

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] Ch                - Ch number
  @param[in] Dimm              - Dimm number (zero-based)
  @param[in] Rank              - Rank number (zero-based)
  @param[in] Mode              - Write Leveling Mode

  @retval N/A

**/
VOID
ProgramWlMr2Ddrt2 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
  )
{

  switch (Mode) {
    case EXTERNAL_WRITE_LEVELING_MODE:
      //
      // Program External Write Leveling
      //
      ProgramCwvWriteLevel(Host, Socket, Ch, Dimm, 1);
    break;

    case INTERNAL_WRITE_LEVELING_MODE:
      RcDebugPrintWithDevice(SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "CWV does not support Internal Write Leveling Mode \n");
      break;

    case DISABLE_WRITE_LEVELING_MODE:
      //
      // Clear External Write Leveling
      //
      ProgramCwvWriteLevel(Host, Socket, Ch, Dimm, 0);
      break;

    default:
      break;
  }
}

/**
  Programs CCC Settings for CWV

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT
  picode1 BCLK_A
  picode2 BCOM_A[0], BCS_A
  
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_STRUCT
  picode1 BRST_A, BCOM_A[2:1]

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT
  picode1 BCLK_B
  picode2 BCOM_B[0], BCS_B

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT
  picode1 BRST_B, BCOM_B[2:1]

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket Id
  @param[in] Ch        - Ch number
  @param[in] SubchMask - SubchMask number
  @param[in] Dimm      - Dimm number (zero-based)
  @param[in] Data      - Data to write

  @retval N/A
**/
VOID
WriteCwvBufferDelay(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubchMask,
  IN UINT8     Dimm,
  IN UINT16    Data
  )
{
  UINT8 SubCh;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_STRUCT DdrCrPicoding3;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT DdrCrPicoding2;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT DdrCrPicoding1;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT DdrCrPicoding0;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (SubchMask)) == 0) {
      continue;
    }
    if (SubCh == 0) {
      DdrCrPicoding2.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
      DdrCrPicoding3.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG);

      DdrCrPicoding2.Bits.picode2 = Data;
      DdrCrPicoding3.Bits.picode1 = Data;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, DdrCrPicoding2.Data);
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG, DdrCrPicoding3.Data);
    } else {
      DdrCrPicoding0.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
      DdrCrPicoding1.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG);

      DdrCrPicoding0.Bits.picode2 = Data;
      DdrCrPicoding1.Bits.picode1 = Data;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, DdrCrPicoding0.Data);
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG, DdrCrPicoding1.Data);
    }

  }
}

/**
  Programs Bclk Latency for CPS

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT
  picode1 BCLK_A

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT
  picode1 BCLK_B

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Ch number
  @param[in] Subch      - Subch
  @param[in] Dimm       - Dimm number (zero-based)
  @param[in] InputDelay - Desired Delay

  @retval N/A
**/
VOID
ProgramDdrt2BclkDelay(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Subch,
  IN UINT8     Dimm,
  IN INT16     InputDelay
  )
{
  INT16 RegDelay;
  INT8  AdjustValue = 1;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT DdrCrPicoding2;
  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT DdrCrPicoding0;

  if (Subch == 0) {
    DdrCrPicoding2.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
    RegDelay = (INT16) DdrCrPicoding2.Bits.picode1;

    if (InputDelay < RegDelay) {
      AdjustValue = -1;
    }

    while (RegDelay != InputDelay) {
      RegDelay += AdjustValue;
      DdrCrPicoding2.Bits.picode1 = RegDelay;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, DdrCrPicoding2.Data);
      FixedDelayMicroSecond (1);
    }
  } else {
    DdrCrPicoding0.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
    RegDelay = (INT16) DdrCrPicoding0.Bits.picode1;

    if (InputDelay < RegDelay) {
      AdjustValue = -1;
    }

    while (RegDelay != InputDelay) {
      RegDelay += AdjustValue;
      DdrCrPicoding0.Bits.picode1 = RegDelay;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, DdrCrPicoding0.Data);
      FixedDelayMicroSecond (1);
    }
  }
}

/**
General function for Get/Set CS Vref for CWV

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CsVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcsVrefDdrt2 (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubCh,
  IN UINT8      Signal,
  IN UINT8      Mode,
  IN OUT UINT8  *CsVrefValue
)
{
  UINT32      PercentageOfVddq;
  UINT32      CwvDcsVref;
  UINT32      ReadCwvDcsVref;
  MRC_STATUS  Status;
  DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_STRUCT  SubchannelAChipSelectVref;
  DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT  SubchannelBChipSelectVref;

  if ((Mode != GSM_READ_CSR) && (Mode != GSM_FORCE_WRITE)) {
    Status = MRC_STATUS_UNSUPPORTED;
    return Status;
  }

  if (Mode & GSM_FORCE_WRITE) {
    PercentageOfVddq = DecodeRcdVref((UINT32) (UINT8) *CsVrefValue);
    RcDebugPrintWithDevice(SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CWV DCS PercentageOfVddq: %d \n", PercentageOfVddq);

    CwvDcsVref = (((PercentageOfVddq * CWV_DCS_VREF_MAX_ENCODING) + (ONE_HUNDRED_PERCENTAGE_VREF - 5000))/ ONE_HUNDRED_PERCENTAGE_VREF);
  }

  Status = MRC_STATUS_SUCCESS;
  ReadCwvDcsVref = CWV_DCS_VREF_MAX_ENCODING;

  if (SubCh) {
    switch (Signal) {
      case CS0_A_N:
      case CS2_A_N:
        SubchannelAChipSelectVref.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcsVref = SubchannelAChipSelectVref.Bits.vrefctl_rx5;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubchannelAChipSelectVref.Bits.vrefctl_rx5 = CwvDcsVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG, SubchannelAChipSelectVref.Data);
        }
        break;
      case CS1_A_N:
      case CS3_A_N:
        SubchannelAChipSelectVref.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcsVref = SubchannelAChipSelectVref.Bits.vrefctl_rx6;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubchannelAChipSelectVref.Bits.vrefctl_rx6 = CwvDcsVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_REG, SubchannelAChipSelectVref.Data);
        }
        break;
      default:
        Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
        break;
    }
  } else {
    switch (Signal) {
      case CS0_B_N:
      case CS2_B_N:
        SubchannelBChipSelectVref.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcsVref = SubchannelBChipSelectVref.Bits.vrefctl_rx11;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubchannelBChipSelectVref.Bits.vrefctl_rx11 = CwvDcsVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG, SubchannelBChipSelectVref.Data);
        }
        break;
      case CS1_B_N:
      case CS3_B_N:
        SubchannelBChipSelectVref.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcsVref = SubchannelBChipSelectVref.Bits.vrefctl_rx12;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubchannelBChipSelectVref.Bits.vrefctl_rx12 = CwvDcsVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG, SubchannelBChipSelectVref.Data);
        }
        break;
      default:
        Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
        break;
    }
  }

  //
  // CWV Dcs range from 0 (0V) to 127 (VDDQ) in steps of VDDQ/128
  //
  if ((Mode & GSM_READ_CSR) && (Status != MRC_STATUS_SIGNAL_NOT_SUPPORTED)) {
    if (ReadCwvDcsVref < CWV_DCS_VREF_MAX_ENCODING) {
      PercentageOfVddq = EncodeRcdVref (ReadCwvDcsVref);
      *CsVrefValue =  (UINT8) ((MAX_DCS_VREF_PERCENTAGE - PercentageOfVddq) / VREF_STEP_OF_0P5PERCENTAGE);
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT,
           "Read CWV Rcd DcsVref Out of range!! TempCwvDcsVref = 0x%x", ReadCwvDcsVref);
      Status = MRC_STATUS_FAILURE;
    }
  }

  return Status;
}

/**
General function for Get/Set CA Vref for FMC

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CaVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcaVrefDdrt2 (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm,
  IN UINT8        SubCh,
  IN GSM_CSN      Signal,
  IN UINT8        Mode,
  IN OUT UINT8    *CaVrefValue
)
{
  UINT32      PercentageOfVddq;
  UINT32      CwvDcaVref;
  UINT32      ReadCwvDcaVref;
  MRC_STATUS  Status;
  DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT SubChannelACommandAddressVrefLower;
  DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT SubChannelACommandAddressVrefUpper;
  DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT SubChannelBCommandAddressVrefLower;
  DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT SubChannelBCommandAddressVrefUpper;

  if ((Mode != GSM_READ_CSR) && (Mode != GSM_FORCE_WRITE)) {
    Status = MRC_STATUS_UNSUPPORTED;
    return Status;
  }

  if (Mode & GSM_READ_CSR) {
    ReadCwvDcaVref = CWV_DCA_VREF_MAX_ENCODING;
  } else if (Mode & GSM_FORCE_WRITE) {
    PercentageOfVddq = DecodeRcdVref((UINT32) (UINT8) *CaVrefValue);
    RcDebugPrintWithDevice(SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CWV DCA PercentageOfVddq: %d \n", PercentageOfVddq);

    CwvDcaVref = (((PercentageOfVddq * CWV_DCA_VREF_MAX_ENCODING) + (ONE_HUNDRED_PERCENTAGE_VREF - 5000))/ ONE_HUNDRED_PERCENTAGE_VREF);
  }

  Status = MRC_STATUS_SUCCESS;

  if (SubCh == 0) {
    switch (Signal) {
      case A0A:
        SubChannelACommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefLower.Bits.vrefctl_rx6;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefLower.Bits.vrefctl_rx6 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefLower.Data);
        } 
        break;
      case A1A:
        SubChannelACommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefLower.Bits.vrefctl_rx5;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefLower.Bits.vrefctl_rx5 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefLower.Data);
        }
        break;
      case A2A:
        SubChannelACommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefLower.Bits.vrefctl_rx8;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefLower.Bits.vrefctl_rx8 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefLower.Data);
        }
        break;
      case A3A:
        SubChannelACommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefLower.Bits.vrefctl_rx7;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefLower.Bits.vrefctl_rx7 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefLower.Data);
        }
        break;
      case A4A:
        SubChannelACommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx10;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx10 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefUpper.Data);
        }
        break;
      case A5A:
        SubChannelACommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx9;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx9 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefUpper.Data);
        }
        break;
      case A6A:
        SubChannelACommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx12;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx12 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefUpper.Data);
        }
        break;
      case PAR:
        SubChannelACommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx11;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelACommandAddressVrefUpper.Bits.vrefctl_rx11 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubChannelACommandAddressVrefUpper.Data);
        }
        break;
      default:
        Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
        break;
      }
    } else {
    switch (Signal) {
      case A0A:
        SubChannelBCommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx11;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx11 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefLower.Data);
        }
        break;
      case A1A:
        SubChannelBCommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx12;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx12 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefLower.Data);
        }
        break;
      case A2A:
        SubChannelBCommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx9;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx9 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefLower.Data);
        }
        break;
      case A3A:
        SubChannelBCommandAddressVrefLower.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx10;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefLower.Bits.vrefctl_rx10 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefLower.Data);
        }
        break;
      case A4A:
        SubChannelBCommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx7;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx7 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefUpper.Data);
        }
        break;
      case A5A:
        SubChannelBCommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx8;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx8 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefUpper.Data);
        }
        break;
      case A6A:
        SubChannelBCommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx5;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx5 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefUpper.Data);
        }
        break;
      case PAR:
        SubChannelBCommandAddressVrefUpper.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);
        if (Mode & GSM_READ_CSR) {
          ReadCwvDcaVref = SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx6;
        } else if (Mode & GSM_FORCE_WRITE) {
          SubChannelBCommandAddressVrefUpper.Bits.vrefctl_rx6 = CwvDcaVref;
          WriteFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubChannelBCommandAddressVrefUpper.Data);
        }
        break;
      default:
        Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
        break;
    }
  }

  //
  // CWV range from 0 (0V) to 127 (VDDQ) in steps of VDDQ/128
  //
  if ((Mode & GSM_READ_CSR) && (Status != MRC_STATUS_SIGNAL_NOT_SUPPORTED)) {
    if ((ReadCwvDcaVref >= CWV_DCA_VREF_MIN_VALUE) && (ReadCwvDcaVref <= CWV_DCA_VREF_MAX_VALUE)) {
      PercentageOfVddq = EncodeRcdVref (ReadCwvDcaVref);
      *CaVrefValue =  (UINT8) ((MAX_DCS_VREF_PERCENTAGE - PercentageOfVddq) / VREF_STEP_OF_0P5PERCENTAGE);
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT,
           "Read CWV Rcd DcaVref Out of range!! TempCwvDcaVref = 0x%x", ReadCwvDcaVref);
      Status = MRC_STATUS_FAILURE;
    }
  }

  return Status;
}

/**

Programs rw01_parity_control.blk_cmd_dq_buffer to block/unblock BCOM commands during DCS or DCA

@param[in] Host        - Pointer to sysHost
@param[in] Socket      - Socket Id
@param[in] Enable      - TRUE == DISABLE

@retval N/A

**/

VOID
EnableDisableBcom(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BOOLEAN   Enable
)
{
  UINT8                                       Ch;
  UINT8                                       Dimm;
  BOOLEAN                                     ddrtEnabled = FALSE;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_STRUCT   Rw01;
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr();
  ChannelNvList = GetChannelNvList(Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if ((*ChannelNvList)[Ch].ddrtEnabled) {
      ddrtEnabled = TRUE;
      break;
    }
  } // Ch
  if (ddrtEnabled == FALSE) {
    return;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      Rw01.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_REG);
      if (Enable == TRUE) {
        Rw01.Bits.blk_cmd_dq_buffer = 0;
      } else {
        Rw01.Bits.blk_cmd_dq_buffer = 1;
      }
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_REG, Rw01.Data);

    } // Dimm
  } // Ch
} // EnableDisableBcom

/**
 Remap MRA to CWV address

@param[in]      Host        - Pointer to sysHost
@param[in]      Socket      - Socket Id
@param[in]      Ch          - Ch number
@param[in]      Dimm        - Dimm number (zero-based)
@param[in out]  Address     - Address for the MRS write
@param[in out]  Cw          - CW flag

@retval none
**/
VOID
RemapMraAddress (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN OUT UINT16 *Address,
  IN OUT UINT8  *Cw
)
{
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  UINT8            MraIndex;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);

  if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
    for (MraIndex = 0; MraIndex < MAX_MRA_REMAP; MraIndex++) {
      if ((CwvMraMap[MraIndex].JedecMraAddress == *Address) && (CwvMraMap[MraIndex].Cw == *Cw)) {
        *Address = CwvMraMap[MraIndex].CrowValleyMraAddress;
        //
        // CW has to be 0 in order for cfg logic to accept it
        //
        *Cw      = 0;
        break;
      }
    } // MraIndex
  } // Aep Present
}
/**
Decode RCD programming into percentage of VDDQ

@param[in] Data        - encoding of DCS Vref


@retval percentage of DCS VDDQ

**/
UINT32
DecodeRcdVref(
  IN UINT32 Data
)
{
  return (MAX_DCS_VREF_PERCENTAGE - (Data * 50));
}

/**
Eecode RCD programming into percentage of VDDQ

  @param[in] Data        - encoding of Vref

  @retval percentage of Vref VDDQ

**/
UINT32
EncodeRcdVref (
  IN UINT32 Data
)
{
  return (UINT32) ((Data * ONE_HUNDRED_PERCENTAGE_VREF) - (ONE_HUNDRED_PERCENTAGE_VREF - 5000)) / CWV_DCA_VREF_MAX_ENCODING;
}

/**

Programs ddrt_hsio_func_defeature0.clear_ddrt_err to 1 then 0 to de-assert ERR# and Req# after DCS training

@param[in] Host        - Pointer to sysHost
@param[in] Socket      - Socket Id

@retval N/A

**/
VOID
DeassertDdrtErrAndReq (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                           MaxChDdr;
  UINT8                                           Ch;
  UINT8                                           Dimm;
  struct channelNvram                             (*ChannelNvList)[MAX_CH];
  struct dimmNvram                                (*DimmNvList)[MAX_DIMM];
  DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_STRUCT Hsiofuncdefeature0;

  MaxChDdr = GetMaxChDdr();
  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      Hsiofuncdefeature0.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_REG);
      Hsiofuncdefeature0.Bits.clear_ddrt_err = 1;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_REG, Hsiofuncdefeature0.Data);
      Hsiofuncdefeature0.Bits.clear_ddrt_err = 0;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_REG, Hsiofuncdefeature0.Data);
    } // Dimm
  } // Ch
}

/**
  Program direct snoop settings to DQ buffer

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] SubChMask   - Mask of subchannel numbers to be updated
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] Rank        - Rank number (zero-based)
  @param[in] Address     - Address for the MRS write
  @param[in] Cw          - Data to write

  @retval none
**/
VOID
EFIAPI
ProgramSnoopSettingsToDb (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubChMask,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Address,
  IN UINT8      Data
  )
{
  UINT8            SnoopIndex;
  UINT8            DbAddress;

  for (SnoopIndex = 0; SnoopIndex < MAX_SNOOP_SETTING; SnoopIndex++) {
    if (CwvSnoopMapTable[SnoopIndex].DramAddress == Address) {

      DbAddress = CwvSnoopMapTable[SnoopIndex].DbAddress;

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                              "ProgramSnoopSettingsToDb: DRAM Address 0x%02X, DB Address 0x%02X, Data 0x%02X\n", Address, DbAddress, Data);

      //
      // Choose Page 8 and send the Snoop Settings
      //
      DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE8);
      DbControlWordWrite (Host, Socket, Ch, SubChMask, Dimm, Rank, DbAddress, Data);
      break;
    }
  } // SnoopIndex
}

/*
  Programs DFE DCA LFSR Seed for Training Accelerator

  @param[in] Host - Pointer to sysHost
  @param[in] Socket - Socket Id
  @param[in] Ch - Ch number
  @param[in] Dimm - Dimm number(zero - based)
  @param[in] Signal - DCA Signal

  @retval N / A
*/
VOID
ProgramCaLfsrSeed(
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN GSM_CSN    Signal
)
{
  RW34_LFSR_DFETM_CWV_DDRIO_HSIO_STRUCT    RW34;
  RW63_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW63;
  RW67_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW67;
  RW6B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW6B;
  RW6F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW6F;
  RW73_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW73;
  RW77_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW77;
  RW7B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW7B;
  RW7F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT RW7F;

  if (IsDcpmmPresentDimm(Socket, Ch, Dimm) == TRUE) {
    switch (Signal) {
    case A0A:
    case A0B:
      RW63.Data = 0;
      RW63.Bits.ca0_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW63_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW63.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A1A:
    case A1B:
      RW67.Data = 0;
      RW67.Bits.ca1_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW67_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW67.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A2A:
    case A2B:
      RW6B.Data = 0;
      RW6B.Bits.ca2_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW6B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW6B.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A3A:
    case A3B:
      RW6F.Data = 0;
      RW6F.Bits.ca3_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW6F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW6F.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A4A:
    case A4B:
      RW73.Data = 0;
      RW73.Bits.ca4_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW73_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW73.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A5A:
    case A5B:
      RW77.Data = 0;
      RW77.Bits.ca5_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW77_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW77.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case A6A:
    case A6B:
      RW7B.Data = 0;
      RW7B.Bits.ca6_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW7B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW7B.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    case PARA:
    case PARB:
      RW7F.Data = 0;
      RW7F.Bits.ca7_lfsr_seed = 0x5A;
      WriteFnvCfg(Host, Socket, Ch, Dimm, RW7F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG, RW7F.Data);
      ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8*)&RW34.Data);
      RW34.Bits.lfsr_load = 1;
      RcdControlWordWrite(Socket, Ch, 1, Dimm, 0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, (UINT8)RW34.Data);
      break;
    default:
      break;
    }
  }
}