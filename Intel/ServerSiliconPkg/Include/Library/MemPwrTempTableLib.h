/** @file
  Interface for getting Power and Temperature Tables

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_PWR_TEMP_TABLE_LIB_H_
#define _MEM_PWR_TEMP_TABLE_LIB_H_

//
// TTRankNumIoWidth with format
// Bits[0:2] -> SDRAM Device/IO Width
// Bits[3:5] -> number_of_package_ranks per dimm
//
#define NUM_RANKS_1_WIDTH_X4_OR_ANY 0x0
#define NUM_RANKS_1_WIDTH_X8        0x1
#define NUM_RANKS_1_WIDTH_X4        0x8
#define NUM_RANKS_2_WIDTH_X8        0x9
#define NUM_RANKS_4_WIDTH_X4        0x18
#define NUM_RANKS_8_WIDTH_X4        0x38

#pragma pack (push, 1)

///
/// Common MemThrot structure
///
typedef struct {
  UINT8  TTDimmType;          /// Dimm Type RDIMM = 1,  LRDIMM = 11d
  UINT8  TTDramDensity;       /// DRAM Density 1Gb = 2, 2Gb = 3, 4Gb = 4, 8Gb = 5, 16Gb = 6, 32Gb = 7
  UINT8  TTRankNumIoWidth;    /// Number of number per dimm in Bits[3:5] + SDRAM Device/IO Width Bits[0:2]
  UINT8  TTFrequency;         ///     800 = 0
                              ///     1000 = 1
                              ///     1066 = 2
                              ///     1200 = 3
                              ///     1333 = 4
                              ///     1400 = 5
                              ///     1600 = 6
                              ///     1800 = 7
                              ///     1866 = 8
                              ///     2000 = 9
                              ///     2133 = 0xA
                              ///     2200 = 0xB
                              ///     2400 = 0xC
                              ///     2600 = 0xD
                              ///     2666 = 0xE
                              ///     2800 = 0xF
                              ///     2933 = 0x10
                              ///     3000 = 0x11
                              ///     3200 = 0x12
                              ///     3400 = 0x13
                              ///     3466 = 0x14
                              ///     3600 = 0x15
                              ///     3733 = 0x16
                              ///     3800 = 0x17
                              ///     4000 = 0x18
                              ///     4200 = 0x19
  UINT8  TTAdjDimmInstalled;  ///     1 = yes, 0 = No
  UINT8  TTDPC;               /// Dimms Per Channel
  UINT8  TTStaticData;        /// Temperature Offset value in Celcius
  UINT8  TTPower;             /// Power in Watts
} DimmTTRowEntryStruct;

//
// Based on Platform_OLTT/CLTT_Implementation_Whitepaper(563306) Revision 1.0 (NOV 2015)
// the DimmTThrottlingStruct table below assumes the following:
//
// TTMode = Throttling Mode
//      OLTT = 0
//      CLTT = 1
//
// TTDIMMPC = Num of DIMM Per Channel
//      1DPC = 1
//      2DPC = 2
//      3DPC = 3
//
// TTFREQ = Frequency
//      800 = 0
//      1066 = 2
//      1333 = 4
//      1600 = 6
//      1866 = 8
//      2133 = 0xA
//      2400 = 0xC
//      2666 = 0xE
//      2933 = 0x10
//      3200 = 0x12
//
// TTWINDOWS = Windows (us)
//      1us = 1
//      2us = 2
//      ...
//
// TTBW_THROTTLE_TF = BW_THROTTLE_TF (hex)
//
// TTTHRT_HI = THRT_HI (hex)
//
typedef struct {
  UINT8   TTMode;
  UINT8   TTDIMMPC;
  UINT8   TTFREQ;
  UINT8   TTWINDOWS;
  UINT16  TTBW_THROTTLE_TF;
  UINT8   TTTHRT_HI;
} DimmTThrottlingStruct;

///
/// Common MemWeight Structure as used in the Weight Table xls
///
// The definition of DimmWTAEP is as follows;
// WTDimmType: NVMDIMM = 0xFD
// WTLvAndDramDensity: LV bit and DRAM density combined in 1 byte
//   - DRAM Density: 64Gb = 8, 128Gb = 9, 256Gb = a, 512Gb = b
//   - Voltage supported: 1.2v = 0x40
//   => 128Gb and 1.2V = 0x9 + 0x40 = 0x49
// WTRankNumIoWidth: #Ranks and Width combined into 1 byte
//   - Ranks: SR(1) = 0, DR(2) = 0x8, QR(4) = 0x18
//   - Width: x4 = 0, x8 = 1, x16 = 2
//   => DR and x8  = 8 + 1 = 9
// WTSignalLoading: SPD Byte 6[1:0] - Signal Loading
// WTDieCount: SPD Byte6[6:4] - Die Count
// WTDimmPos: DIMM position in channel, 1 = 0x0, 2 = 0x1
// WTFrequency - 2400 = 12, 2666 = 14
// WTPmCmdPwrData
//   Bits[7:0],  PwrcasrDimmVal will be filled into rd_cmd_x
//   Bits[15:8], PwrcaswDimmVal will be filled into wr_cmd_x
//   Bits[21:16], reserved
//   Bits[26:22], active_idle_dimm
//   Bits[31:27], pwrodt_cnt_dimm
// DramPmWritePowerCoeff: refer to DRAM_PM_WRITE_POWER_COEFF in spreadsheet
// DramMaxPwr: DRAM_MAX_PWR (1/8W) closed page
// DramTdp: DRAM_TDP_PWR (1/8W) closed page
// DramMinPwrClosedPage: DRAM_MIN_PWR (closed page 1/8W)
// DramWritePwrScaleClosedPage: WRITE_PWR_BW_SCALE:  scale factor closed page
// DramMaxPwrOpenPage: DRAM_MAX_PWR (1/8W) open page
// DramTdpOpenPage: DRAM_TDP_PWR (1/8W) open page
// DramMinPwrOpenPage: RAM_MIN_PWR (open page 1/8W)
// DramWritePwrScaleOpenPage: WRITE_PWR_BW_SCALE: scale factor open page
// DramRefreshRateSlope: 1to4x refresh rate slope + 1to4x refresh rate constant
// DramSelfRefreshPower: DIMM Self Refresh Power (milliWatts)
typedef struct {
  UINT8  WTDimmType;
  UINT8  WTLvAndDramDensity;
  UINT8  WTRankNumIoWidth;
  UINT8  WTSignalLoading;
  UINT8  WTDieCount;
  UINT8  WTDimmPos;
  UINT8  WTFrequency;
  UINT32 WTPmCmdPwrData;
  UINT16 DramPmWritePowerCoeff;
  UINT8  DramMaxPwr;
  UINT8  DramTdp;
  UINT8  DramMinPwrClosedPage;
  UINT16 DramWritePwrScaleClosedPage;
  UINT8  DramMaxPwrOpenPage;
  UINT8  DramTdpOpenPage;
  UINT8  DramMinPwrOpenPage;
  UINT16 DramWritePwrScaleOpenPage;
  UINT32 DramRefreshRateSlope;
  UINT16 DramSelfRefreshPower;
} DimmWTRowEntryStruct;

#pragma pack (pop)

/**

  This function gets DimmWeight table pointer

  @param[out]  DimmWTRowEntryStruct               Pointer to DimmWeight Table
  @param[in]   WeightTableRow                     Row number to get

  @retval N/A

**/
VOID
EFIAPI
GetDimmWeightTable (
  IN const DimmWTRowEntryStruct           **PtrDimmWeightTable,
  IN UINT16 WeightTableRow
  );

/**

  This function gets DimmWtaep table pointer

  @param[out]  DimmWTRowEntryStruct               Pointer to DimmWtaep Table
  @param[in]   WeightTableRow                     Row number to get

  @retval N/A

**/
VOID
EFIAPI
GetDimmWtaepTable (
  IN const DimmWTRowEntryStruct           **PtrDimmWT,
  IN UINT16 WeightTableRow
  );

/**

  This function gets size of Dimm weight table for DDRT

  @retval UINT16   size of Dimm weight table for DDRT

**/
UINT16
EFIAPI
GetDimmWtaepTableSize (
  VOID
  );

/**

  This function gets size Dimm weight table

  @retval UINT16   size of Dimm Weight Table

**/
UINT16
EFIAPI
GetDimmWeightTableSize (
  VOID
  );

/**

  This function gets Dimm thermal Throttling table pointer

  @param[out]  DimmTTRowEntryStruct                Pointer to DimmTT Table
  @param[in]   TTRow                               Row number to get

  @retval N/A

**/
VOID
EFIAPI
GetDimmTTTable (
  OUT const DimmTTRowEntryStruct           **ptrDimmTT,
  IN UINT16 TTRow
  );

/**

  This function gets Thermal Throttling Mode table pointer

  @param[out]  DimmTThrottlingStruct               Pointer to DimmTtMode Table
  @param[in]   TTMRow                              Row number to get

  @retval N/A

**/
VOID
EFIAPI
GetDimmTtModeTable (
  OUT const DimmTThrottlingStruct           **ptrDimmTTMODE,
  IN UINT16 TTMRow
  );

/**

  This function gets size of Dimm thermal Throttling table

  @retval UINT16   size of Dimm thermal Throttling Table

**/
UINT16
EFIAPI
GetDimmTTTableSize (
  VOID
  );

/**

  This function gets size of Dimm thermal throttling mode table

  @retval UINT16   size of Dimm thermal throttling mode Table

**/
UINT16
EFIAPI
GetDimmTTModeTableSize (
  VOID
  );

#endif // _MEM_PWR_TEMP_TABLE_LIB_H_
