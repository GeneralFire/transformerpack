/** @file
  This file contains the Memory throttling table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <MemRegs.h>
#include <MemCommon.h>
#include <Library/MemPwrTempTableLib.h>
#include <Library/SpdDecodeLib.h>

const DimmTTRowEntryStruct DimmTT[] = {
  // DimmType;Density;  TTRankNumIoWidth;             Frequency; AdjDimmInstalled;  Dimms Per Channel;  Temperature Offset value in Celcius;  Power in Watts
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 0, 1, 11, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 1, 1, 11, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 1, 2, 7, 31},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 0, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 1, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 1, 2, 7, 39},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 0, 1, 4, 73},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 1, 1, 4, 73},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 1, 2, 3, 55},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 0, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 2, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 0, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 2, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 0, 1, 15, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 1, 15, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 2, 15, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 0, 1, 0, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 1, 0, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 2, 0, 140},
  // AEP
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 0, 99}
};

//
// Based on Eagle Stream Platform_OLTT/CLTT_Implementation_Whitepaper() Revision 0.4
// the DimmTThrottlingStruct table below assumes the following:
//
const DimmTThrottlingStruct DimmTTMODE[] = { //OLTT(0) and CLTT(1) values Performance Values ddr5
  // Throttling Mode; DIMM Per Channel; Frequency; WINDOWS; BW_THROTTLE_TF; THRT_HI
  //OLTT 1DPC
  { 0, 1, DDR_3200, 1, 0xC8,  0x14},
  { 0, 1, DDR_3600, 1, 0xE1,  0x17},
  { 0, 1, DDR_4000, 1, 0xFA,  0x19},
  { 0, 1, DDR_4400, 1, 0x113, 0x1C},
  { 0, 1, DDR_4800, 1, 0x12C, 0x1E},
  { 0, 1, DDR_5200, 1, 0x145, 0x21},
  { 0, 1, DDR_5600, 1, 0x15E, 0x23},
  { 0, 1, DDR_6000, 1, 0x177, 0x26},
  { 0, 1, DDR_6400, 1, 0x190, 0x28},
  //OLTT 2DPC
  { 0, 2, DDR_3200, 2, 0x190, 0x14},
  { 0, 2, DDR_3600, 2, 0x1C2, 0x17},
  { 0, 2, DDR_4000, 2, 0x1F4, 0x19},
  { 0, 2, DDR_4400, 2, 0x226, 0x1C},
  { 0, 2, DDR_4800, 2, 0x258, 0x1E},
  { 0, 2, DDR_5200, 2, 0x28A, 0x21},
  { 0, 2, DDR_5600, 2, 0x2BC, 0x23},
  { 0, 2, DDR_6000, 2, 0x2EE, 0x26},
  { 0, 2, DDR_6400, 2, 0x320, 0x28},
  //CLTT 1DPC
  { 1, 1, DDR_3200, 3, 0x258, 0x8},
  { 1, 1, DDR_3600, 3, 0x2A3, 0x9},
  { 1, 1, DDR_4000, 3, 0x2EE, 0xA},
  { 1, 1, DDR_4400, 3, 0x339, 0xB},
  { 1, 1, DDR_4800, 3, 0x384, 0xC},
  { 1, 1, DDR_5200, 3, 0x3CF, 0xD},
  { 1, 1, DDR_5600, 3, 0x41A, 0xE},
  { 1, 1, DDR_6000, 3, 0x465, 0xF},
  { 1, 1, DDR_6400, 3, 0x4B0, 0xF},
  //CLTT 2DPC
  { 1, 2, DDR_3200, 3, 0x258, 0x4},
  { 1, 2, DDR_3600, 3, 0x2A3, 0x4},
  { 1, 2, DDR_4000, 3, 0x2EE, 0x5},
  { 1, 2, DDR_4400, 3, 0x339, 0x5},
  { 1, 2, DDR_4800, 3, 0x384, 0x6},
  { 1, 2, DDR_5200, 3, 0x3CF, 0x6},
  { 1, 2, DDR_5600, 3, 0x41A, 0x7},
  { 1, 2, DDR_6000, 3, 0x465, 0x8},
  { 1, 2, DDR_6400, 3, 0x4B0, 0x8},
};

/**

  This function gets size of Dimm thermal Throttling table

  @retval UINT16   size of Dimm thermal Throttling Table

**/
UINT16
GetDimmTTTableSize (
  VOID
  )
{
  return (sizeof (DimmTT) / sizeof (DimmTTRowEntryStruct));
} // GetDimmTTTableSize

/**

  This function gets size of Dimm thermal throttling mode table

  @retval UINT16   size of Dimm thermal throttling mode Table

**/
UINT16
GetDimmTTModeTableSize (
  VOID
  )
{
  return (sizeof (DimmTTMODE) / sizeof (DimmTThrottlingStruct));
} // GetDimmTTModeTableSize

/**

  This function gets Thermal Throttling Mode table pointer

  @param[out]  DimmTThrottlingStruct               Pointer to DimmTtMode Table
  @param[in]   TTMRow                              Row number to get

  @retval N/A

**/
VOID
GetDimmTtModeTable (
  OUT const DimmTThrottlingStruct           **PtrDimmThermalThrottlingMode,
  IN  UINT16 ThermalThrottlingRow
  )
{
  if (ThermalThrottlingRow < GetDimmTTModeTableSize ()) {
    *PtrDimmThermalThrottlingMode = &DimmTTMODE[ThermalThrottlingRow];
  }
} // GetDimmTtModeTable

/**

  This function gets Dimm thermal Throttling table pointer

  @param[out]  DimmTTRowEntryStruct                Pointer to DimmTT Table
  @param[in]   TTRow                               Row number to get

  @retval N/A

**/
VOID
GetDimmTTTable (
  OUT const DimmTTRowEntryStruct           **PtrDimmThermalThrottling,
  IN  UINT16 ThermalThrottlingRow
  )
{
  *PtrDimmThermalThrottling = &DimmTT[ThermalThrottlingRow];
} // GetDimmTTTable
