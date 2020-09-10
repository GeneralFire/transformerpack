/** @file
  This file contains the Memory throttling table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1600, 0, 1, 7, 32},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1866, 0, 1, 8, 35},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2133, 0, 1, 9, 38},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2400, 0, 1, 10, 41},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2666, 0, 1, 11, 44},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 0, 1, 11, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1600, 1, 1, 7, 32},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1866, 1, 1, 8, 35},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2133, 1, 1, 9, 38},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2400, 1, 1, 10, 41},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2666, 1, 1, 11, 44},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 1, 1, 11, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1600, 1, 2, 4, 23},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_1866, 1, 2, 5, 24},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2133, 1, 2, 5, 26},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2400, 1, 2, 6, 28},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2666, 1, 2, 6, 29},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X8,        DDR_2933, 1, 2, 7, 31},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1600, 0, 1, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1866, 0, 1, 8, 43},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2133, 0, 1, 9, 46},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2400, 0, 1, 10, 49},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2666, 0, 1, 11, 52},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 0, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1600, 1, 1, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1866, 1, 1, 8, 43},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2133, 1, 1, 9, 46},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2400, 1, 1, 10, 49},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2666, 1, 1, 11, 52},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 1, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1600, 1, 2, 4, 30},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_1866, 1, 2, 5, 32},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2133, 1, 2, 6, 34},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2400, 1, 2, 6, 35},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2666, 1, 2, 6, 37},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_2_WIDTH_X8,        DDR_2933, 1, 2, 7, 39},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 0, 1, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 0, 1, 8, 43},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 0, 1, 9, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 0, 1, 10, 50},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 0, 1, 11, 53},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 1, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 1, 8, 43},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 1, 9, 47},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 1, 10, 50},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 1, 11, 53},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 12, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 2, 5, 31},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 2, 5, 33},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 2, 6, 34},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 2, 6, 36},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 2, 7, 38},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 7, 40},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1600, 0, 1, 3, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1866, 0, 1, 3, 60},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2133, 0, 1, 3, 63},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2400, 0, 1, 4, 66},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2666, 0, 1, 4, 70},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 0, 1, 4, 73},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1600, 1, 1, 3, 56},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1866, 1, 1, 3, 60},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2133, 1, 1, 3, 63},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2400, 1, 1, 4, 66},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2666, 1, 1, 4, 70},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 1, 1, 4, 73},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1600, 1, 2, 2, 46},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_1866, 1, 2, 2, 48},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2133, 1, 2, 2, 50},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2400, 1, 2, 2, 52},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2666, 1, 2, 3, 54},
  {SPD_RDIMM,  SPD_16Gb, NUM_RANKS_1_WIDTH_X4,        DDR_2933, 1, 2, 3, 55},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 0, 1, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 0, 1, 13, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 0, 1, 14, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 0, 1, 15, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 0, 1, 15, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 0, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 1, 1, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 1, 1, 13, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 1, 1, 14, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 1, 1, 15, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 1, 1, 15, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 1, 2, 9, 85},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 1, 2, 9, 88},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 1, 2, 10, 90},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 1, 2, 10, 93},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 1, 2, 11, 96},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 2, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 0, 1, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 0, 1, 0, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 0, 1, 0, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 0, 1, 0, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 0, 1, 0, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 0, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 1, 1, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 1, 1, 0, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 1, 1, 0, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 1, 1, 0, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 1, 1, 0, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1600, 1, 2, 0, 85},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_1866, 1, 2, 0, 88},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2133, 1, 2, 0, 90},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2400, 1, 2, 0, 93},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2666, 1, 2, 0, 96},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_4_WIDTH_X4,        DDR_2933, 1, 2, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 0, 1, 15, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 0, 1, 15, 145},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 0, 1, 15, 149},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 0, 1, 15, 154},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 0, 1, 15, 159},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 0, 1, 15, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 1, 1, 15, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 1, 1, 15, 145},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 1, 1, 15, 149},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 1, 1, 15, 154},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 1, 1, 15, 159},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 1, 15, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 1, 2, 15, 127},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 1, 2, 15, 129},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 1, 2, 15, 132},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 1, 2, 15, 135},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 1, 2, 15, 137},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 2, 15, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 0, 1, 0, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 0, 1, 0, 145},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 0, 1, 0, 149},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 0, 1, 0, 154},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 0, 1, 0, 159},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 0, 1, 0, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 1, 1, 0, 140},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 1, 1, 0, 145},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 1, 1, 0, 149},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 1, 1, 0, 154},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 1, 1, 0, 159},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 1, 0, 163},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1600, 1, 2, 0, 127},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_1866, 1, 2, 0, 129},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2133, 1, 2, 0, 132},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2400, 1, 2, 0, 135},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2666, 1, 2, 0, 137},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_8_WIDTH_X4,        DDR_2933, 1, 2, 0, 140},
  // AEP
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 0, 1, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 0, 1, 13, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 0, 1, 14, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 0, 1, 15, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 0, 1, 15, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 1, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 1, 13, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 1, 14, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 1, 15, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 1, 15, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 15, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 2, 9, 85},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 2, 9, 88},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 2, 10, 90},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 2, 10, 93},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 2, 11, 96},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 12, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 0, 1, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 0, 1, 0, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 0, 1, 0, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 0, 1, 0, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 0, 1, 0, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 0, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 1, 0, 99},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 1, 0, 104},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 1, 0, 109},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 1, 0, 114},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 1, 0, 119},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 1, 0, 124},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1600, 1, 2, 0, 85},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_1866, 1, 2, 0, 88},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2133, 1, 2, 0, 90},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2400, 1, 2, 0, 93},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2666, 1, 2, 0, 96},
  {SPD_LRDIMM, SPD_16Gb, NUM_RANKS_1_WIDTH_X4_OR_ANY, DDR_2933, 1, 2, 0, 99}
};

//
// Based on Whitley Platform_OLTT/CLTT_Implementation_Whitepaper(612245) Revision 0.7
// the DimmTThrottlingStruct table below assumes the following:
//
const DimmTThrottlingStruct DimmTTMODE[] = { //OLTT(0) and CLTT(1) values Performance Values ddr4/ddr3
  // Throttling Mode; DIMM Per Channel; Frequency; WINDOWS; BW_THROTTLE_TF; THRT_HI
  //OLTT 1DPC
  { 0, 1, DDR_800,  1, 0x32,  0x5 },
  { 0, 1, DDR_1066, 1, 0x43,  0x7 },
  { 0, 1, DDR_1333, 1, 0x53,  0x8 },
  { 0, 1, DDR_1600, 1, 0x64,  0xA },
  { 0, 1, DDR_1866, 1, 0x75,  0xC },
  { 0, 1, DDR_2133, 1, 0x85,  0xD },
  { 0, 1, DDR_2400, 1, 0x96,  0xF },
  { 0, 1, DDR_2666, 1, 0xA7,  0x11},
  { 0, 1, DDR_2933, 1, 0xB7,  0x12},
  { 0, 1, DDR_3200, 1, 0xC8,  0x14},
  //OLTT 2DPC
  { 0, 2, DDR_800,  2, 0x64,  0x5},
  { 0, 2, DDR_1066, 2, 0x85,  0x7},
  { 0, 2, DDR_1333, 2, 0xA7,  0x8},
  { 0, 2, DDR_1600, 2, 0xC8,  0xA},
  { 0, 2, DDR_1866, 2, 0xE9,  0xC},
  { 0, 2, DDR_2133, 2, 0x10B, 0xD},
  { 0, 2, DDR_2400, 2, 0x12C, 0xF},
  { 0, 2, DDR_2666, 2, 0x14D, 0x11},
  { 0, 2, DDR_2933, 2, 0x16F, 0x12},
  { 0, 2, DDR_3200, 2, 0x190, 0x14},
  //CLTT 1DPC
  { 1, 1, DDR_800,  3, 0x96,  0x2},
  { 1, 1, DDR_1066, 4, 0x10B, 0x3},
  { 1, 1, DDR_1333, 3, 0xFA,  0x3},
  { 1, 1, DDR_1600, 3, 0x12C, 0x4},
  { 1, 1, DDR_1866, 4, 0x1D3, 0x6},
  { 1, 1, DDR_2133, 4, 0x215, 0x7},
  { 1, 1, DDR_2400, 4, 0x258, 0x8},
  { 1, 1, DDR_2666, 3, 0x1F4, 0x6},
  { 1, 1, DDR_2933, 3, 0x226, 0x7},
  { 1, 1, DDR_3200, 3, 0x258, 0x8},
  //CLTT 2DPC
  { 1, 2, DDR_800,  3, 0x96,  0x1},
  { 1, 2, DDR_1066, 4, 0x10B, 0x2},
  { 1, 2, DDR_1333, 4, 0x14D, 0x2},
  { 1, 2, DDR_1600, 3, 0x12C, 0x2},
  { 1, 2, DDR_1866, 4, 0x1D3, 0x3},
  { 1, 2, DDR_2133, 4, 0x215, 0x3},
  { 1, 2, DDR_2400, 4, 0x258, 0x4},
  { 1, 2, DDR_2666, 4, 0x29B, 0x4},
  { 1, 2, DDR_2933, 4, 0x2DD, 0x5},
  { 1, 2, DDR_3200, 4, 0x320, 0x6},
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
