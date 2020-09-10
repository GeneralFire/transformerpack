/** @file
  Routine for lookup table scan/search related.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/LookupTable.h>
#include <Library/CpuAndRevisionLib.h>
#include <IioRegs.h>

LOOKUP_TBL_ENTRY  *gLookupTable = NULL;

//
//CDF satellite IEH
//{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
//
#define CDF_IEH {IehLocalDevice, CDF_IEH_STACK, 0, 0x1d, 0, 0, 0, 0},\
                {IehEndPoint, CDF_IEH_STACK, 0, 9, 0, 0, 1, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 10, 0, 0, 2, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 11, 0, 0, 3, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 12, 0, 0, 4, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 16, 0, 0, 5, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 17, 0, 0, 6, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 18, 0, 0, 7, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 19, 0, 0, 8, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 20, 0, 0, 9, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 21, 0, 0, 10, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 22, 0, 0, 11, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 23, 0, 0, 12, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 6,  0, 0, 13, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 7,  0, 0, 14, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 8,  0, 0, 15, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 14, 0, 0, 16, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 30, 0, 0, 17, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 31, 0, 0, 18, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 31, 4, 0, 19, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 31, 1, 0, 20, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 31, 5, 0, 20, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 31, 7, 0, 21, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 26, 0, 0, 22, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 26, 1, 0, 22, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 26, 2, 0, 22, 0}, \
                {IehEndPoint, CDF_IEH_STACK, 0, 15, 0, 0, 23, 0},

LOOKUP_TBL_ENTRY  IcxSpLookupTbl[] = {
  //
  //global IEH
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0xff, 0, 0, 3, 0, 0, 0},
  {IehEndPoint, 0xff, 0, 26, 0, 0, 1, 0x00}, //IMC0 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 26, 0, 0, 2, 0x01}, //IMC0 DDR CH1 Unique BitIndex
  {IehEndPoint, 0xff, 0, 27, 0, 0, 3, 0x04}, //IMC1 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 27, 0, 0, 4, 0x05}, //IMC1 DDR CH1 Unique BitIndex
  {IehEndPoint, 0xff, 0, 28, 0, 0, 5, 0x08}, //IMC2 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 28, 0, 0, 6, 0x09}, //IMC2 DDR CH1 Unique BitIndex
  {IehEndPoint, 0xff, 0, 29, 0, 0, 7, 0x0c}, //IMC3 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 29, 0, 0, 8, 0x0d}, //IMC3 DDR CH1 Unique BitIndex
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 9, 0}, //North Satellite IEH on Stack 0
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 10, 1}, //North Satellite IEH on Stack 1
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 11, 2}, //North Satellite IEH on Stack 2
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 12, 3}, //North Satellite IEH on Stack 3
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 13, 4}, //North Satellite IEH on Stack 4
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 14, 5}, //North Satellite IEH on Stack 5
  //
  //satellite IEH 0
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //share devices should put together as incremental sequence.
  //
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 0, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 0, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 1, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 2, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 3, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 4, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 5, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 6, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 7, 0, 2, 0},

  {IehEndPoint, 0, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 1, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 2, 0, 3, 0},

  {IehEndPoint, 0, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 0, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 0, 0, 5, 0, 0, 6, 0},
  {IehEndPoint, 0, 0, 6, 0, 0, 7, 0},
  {IehEndPoint, 0, 0, 7, 0, 0, 8, 0},

  //
  //satellite IEH 1
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 1, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 1, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 1, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 1, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 1, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 1, 0, 5, 0, 0, 6, 0},

  //
  //satellite IEH 2
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 2, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 2, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 2, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 2, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 2, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 2, 0, 5, 0, 0, 6, 0},

  //
  //satellite IEH 3
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 3, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 3, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 3, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 3, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 3, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 3, 0, 5, 0, 0, 6, 0},

  //
  //satellite IEH 4
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 4, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 4, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 4, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 4, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 4, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 4, 0, 5, 0, 0, 6, 0},

  //
  //satellite IEH 5
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 5, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 5, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 5, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 5, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 5, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 5, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 5, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 5, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 5, 0, 5, 0, 0, 6, 0},

  //indictor on end of table.
  {IehDevTypeEnd, 0, 0, 0, 0, 0, 0, 0},

};


LOOKUP_TBL_ENTRY  IcxDLookupTbl[] = {
  //
  //global IEH
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0xff, 0, 0, 3, 0, 0, 0},
  {IehEndPoint, 0xff, 0, 26, 0, 0, 1, 0x00},   //IMC0 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 26, 0, 0, 2, 0x01},   //IMC0 DDR CH1 Unique BitIndex
  {IehEndPoint, 0xff, 0, 27, 0, 0, 3, 0x04},   //IMC1 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 27, 0, 0, 4, 0x05},   //IMC1 DDR CH1 Unique BitIndex
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 5, 0}, //North Satellite IEH on Stack 0
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 6, 1}, //North Satellite IEH on Stack 1
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 7, 2}, //North Satellite IEH on Stack 2
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 8, 3}, //North Satellite IEH on Stack 3
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 9, 4}, //North Satellite IEH on Stack 4

  //
  //satellite IEH 0
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 0, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 0, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 1, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 2, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 3, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 4, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 5, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 6, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 7, 0, 2, 0},

  {IehEndPoint, 0, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 1, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 2, 0, 3, 0},

  {IehEndPoint, 0, 0, 3, 0, 0, 4, 0},

  //
  //satellite IEH 1
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 1, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 1, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 1, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 1, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 1, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 1, 0, 5, 0, 0, 6, 0},

  //
  //satellite IEH 2
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 2, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 2, 0, 4, 0, 0, 2, 0},
  {IehEndPoint, 2, 0, 5, 0, 0, 3, 0},

  //
  //satellite IEH 3
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 3, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 3, 0, 4, 0, 0, 2, 0},

  //
  //satellite IEH 4
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 4, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 2, 0, 1, 0},

  //
  //satellite IEH 5
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 5, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 5, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 5, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 5, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 5, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 5, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 5, 0, 3, 0, 0, 4, 0},
  {IehEndPoint, 5, 0, 4, 0, 0, 5, 0},
  {IehEndPoint, 5, 0, 5, 0, 0, 6, 0},

  //
  //CDF satellite IEH
  //
  CDF_IEH

  //indictor on end of table.
  {IehDevTypeEnd, 0, 0, 0, 0, 0, 0, 0},
};

LOOKUP_TBL_ENTRY  SnrLookupTbl[] = {
  //
  //global IEH
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0xff, 0, 0, 3, 0, 0, 0},
  {IehEndPoint, 0xff, 0, 26, 0, 0, 1, 0x00}, //IMC0 DDR CH0 Unique BitIndex
  {IehEndPoint, 0xff, 0, 26, 0, 0, 2, 0x01}, //IMC0 DDR CH1 Unique BitIndex
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 3, 0}, //North Satellite IEH on Stack 0
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 4, 1}, //North Satellite IEH on Stack 1
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 5, 2}, //North Satellite IEH on Stack 2
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 6, 3}, //North Satellite IEH on Stack 3
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 7, 4}, //North Satellite IEH on Stack 4

  //
  //satellite IEH 0
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 0, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 0, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 0, 0, 1, 0, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 1, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 2, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 3, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 4, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 5, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 6, 0, 2, 0},
  {IehEndPoint, 0, 0, 1, 7, 0, 2, 0},

  {IehEndPoint, 0, 0, 2, 0, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 1, 0, 3, 0},
  {IehEndPoint, 0, 0, 2, 2, 0, 3, 0},

  {IehEndPoint, 0, 0, 3, 0, 0, 4, 0},

  //
  //satellite IEH 1
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 1, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 1, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 1, 0, 4, 0, 0, 2, 0},
  {IehEndPoint, 1, 0, 5, 0, 0, 3, 0},
  {IehEndPoint, 1, 0, 6, 0, 0, 4, 0},
  {IehEndPoint, 1, 0, 7, 0, 0, 5, 0},

  //
  //satellite IEH 4
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 4, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 4, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 4, 0, 4, 0, 0, 2, 0},
  {IehEndPoint, 4, 0, 5, 0, 0, 3, 0},
  //
  //satellite IEH 3
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 3, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 3, 0, 0, 2, 0, 1, 0},

  {IehEndPoint, 3, 0, 4, 0, 0, 2, 0},

  //
  //satellite IEH 2
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, 0},
  {IehEndPoint, 2, 0, 0, 0, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 1, 0, 1, 0},
  {IehEndPoint, 2, 0, 0, 2, 0, 1, 0},
  {IehEndPoint, 2, -V_IOV_RESERVED_BUS_NUMBER, 0, 0, 0, 2, 0}, // HqmBus = buslimit of the stack - V_IOV_RESERVED_BUS_NUMBER

  //
  //CDF satellite IEH
  //
  CDF_IEH

  //indictor on end of table.
  {IehDevTypeEnd, 0, 0, 0, 0, 0, 0, 0},

};


/**
  get lookup table per CPU type
  wave1/2 need lookup table.

  @param   none

  @retval  Lookup table pointer.

**/
LOOKUP_TBL_ENTRY  *
EFIAPI
GetLookupTable (
  VOID
  )
{
  if (gLookupTable != NULL) {
    return gLookupTable;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    return IcxSpLookupTbl;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return IcxDLookupTbl;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    return SnrLookupTbl;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] ERROR: Lookup table is not found\n"));
  return NULL;
}
