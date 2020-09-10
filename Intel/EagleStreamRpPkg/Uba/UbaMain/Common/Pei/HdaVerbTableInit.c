/** @file
  Install Hda Verb table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary  and
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

#include "PeiCommonBoardInitLib.h"
#include <HdAudioConfig.h>

HDAUDIO_VERB_TABLE HdaVerbTableAlc711 = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: (Realtek ALC711)
  //  Revision ID = 0xff
  //  Codec Verb Table
  //  Codec Address: CAd value (0/1/2)
  //  Codec Vendor: 0x10EC0700
  //
  0x10EC, 0x0711,
  0xFF, 0xFF,
  //===================================================================================================
  //
  //                                   Realtek Semiconductor Corp.
  //
  //===================================================================================================

  // Realtek High Definition Audio Configuration - Version : 5.0.3.1
  // Realtek HD Audio Codec : ALC711
  // PCI PnP ID : PCI\VEN_8086&DEV_2668&SUBSYS_72708086
  // HDA Codec PnP ID : INTELAUDIO\FUNC_01&VEN_10EC&DEV_0711&SUBSYS_10EC11C2
  // The number of verb command block : 17

  //  NID 0x12 : 0x40000000
  //  NID 0x13 : 0x411111F0
  //  NID 0x14 : 0x411111F0
  //  NID 0x15 : 0x411111F0
  //  NID 0x16 : 0x411111F0
  //  NID 0x17 : 0x411111F0
  //  NID 0x18 : 0x411111F0
  //  NID 0x19 : 0x04A11010
  //  NID 0x1A : 0x411111F0
  //  NID 0x1B : 0x411111F0
  //  NID 0x1D : 0x40400001
  //  NID 0x1E : 0x411111F0
  //  NID 0x1F : 0x411111F0
  //  NID 0x21 : 0x04211020
  //  NID 0x29 : 0x411111F0

  //===== HDA Codec Subsystem ID Verb-table =====
  // HDA Codec Subsystem ID  : 0x10EC11C2
  0x001720C2,
  0x00172111,
  0x001722EC,
  0x00172310,

  //===== Pin Widget Verb-table =====
  // Widget node 0x01 :
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  // Pin widget 0x12 - DMIC
  0x01271C00,
  0x01271D00,
  0x01271E00,
  0x01271F40,
  // Pin widget 0x13 - DMIC
  0x01371CF0,
  0x01371D11,
  0x01371E11,
  0x01371F41,
  // Pin widget 0x14 - FRONT (Port-D)
  0x01471CF0,
  0x01471D11,
  0x01471E11,
  0x01471F41,
  // Pin widget 0x15 - I2S-OUT
  0x01571CF0,
  0x01571D11,
  0x01571E11,
  0x01571F41,
  // Pin widget 0x16 - LINE3 (Port-B)
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  // Pin widget 0x17 - I2S-OUT
  0x01771CF0,
  0x01771D11,
  0x01771E11,
  0x01771F41,
  // Pin widget 0x18 - I2S-IN
  0x01871CF0,
  0x01871D11,
  0x01871E11,
  0x01871F41,
  // Pin widget 0x19 - MIC2 (Port-F)
  0x01971C10,
  0x01971D10,
  0x01971EA1,
  0x01971F04,
  // Pin widget 0x1A - LINE1 (Port-C)
  0x01A71CF0,
  0x01A71D11,
  0x01A71E11,
  0x01A71F41,
  // Pin widget 0x1B - LINE2 (Port-E)
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  // Pin widget 0x1D - PC-BEEP
  0x01D71C01,
  0x01D71D00,
  0x01D71E40,
  0x01D71F40,
  // Pin widget 0x1E - S/PDIF-OUT
  0x01E71CF0,
  0x01E71D11,
  0x01E71E11,
  0x01E71F41,
  // Pin widget 0x1F - S/PDIF-IN
  0x01F71CF0,
  0x01F71D11,
  0x01F71E11,
  0x01F71F41,
  // Pin widget 0x21 - HP-OUT (Port-I)
  0x02171C20,
  0x02171D10,
  0x02171E21,
  0x02171F04,
  // Pin widget 0x29 - I2S-IN
  0x02971CF0,
  0x02971D11,
  0x02971E11,
  0x02971F41,

  // Widget node 0x20 - 1 : //remove NID 58 realted setting for ALC711  bypass DAC02 DRE(NID5B bit14)
  0x05B50010,
  0x05B4581D,
  0x0205006F,
  0x0204058B,

  // Widget node 0x20 - 2 : //2 MIC Vrefo-L/R for headset
  0x02050045,
  0x02045289,
  0x0205004A,
  0x0204201B,
  // Widget node 0x20 - 3 : /To set 1 pin detect JD for headset
  0x02050008,
  0x0204A806,
  0x02050008,
  0x0204A806
);


EFI_STATUS
InstallHdaVerbTable (
  VOID
)
{
  DEBUG ((DEBUG_INFO, "HDA: Init default verb tables (Realtek ALC711)\n"));

  PcdSet32S (PcdHdaVerbTable, (UINT32) &HdaVerbTableAlc711);

  return EFI_SUCCESS;
}