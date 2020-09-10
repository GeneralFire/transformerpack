/** @file
  Routine for lookup table scan/search related.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

LOOKUP_TBL_ENTRY  *gLookupTable = NULL;

//
//EBG satellite IEH
//{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
// Following are the devices in the sequential order for EBG IEH
// IEH
// XHCI
// ESPISPI
// SPAP0 (PCIe controller 4Px8)
// SPAP1 (PCIe controller 4Px8)
// SPAP2 (PCIe controller 4Px8)
// SPAP3 (PCIe controller 4Px8)
// SPBP0 (PCIe controller 4Px4)
// SPBP1 (PCIe controller 4Px4)
// SPBP2 (PCIe controller 4Px4)
// SPBP3 (PCIe controller 4Px4)
// SPCP0 (PCIe controller 4Px4)
// SPCP1 (PCIe controller 4Px4)
// SPCP2 (PCIe controller 4Px4)
// SPCP3 (PCIe controller 4Px4)
// SPDP0 (PCIe controller 4Px8)
// SPDP1 (PCIe controller 4Px8)
// SPDP2 (PCIe controller 4Px8)
// SPDP3 (PCIe controller 4Px8)
// NPK
// SMB (legacy SMBUS host controller)
// LPC
// SATA 1 (Sata controller 1 Port P0-P7)
// SATA 2 (Sata controller 2 Port P8-P15)
// SATA 3 (Sata controller 2 Port P16-P23)
// GbE
// cAVS
// CSME
// pDMI
// MCSMB (PCIe controller hot plug)
// RLINK
// TAP2SB
//

#define EBG_IEH {IehLocalDevice, EBG_IEH_STACK, 0, 20, 4, 0, 0, 0},\
                {IehEndPoint, EBG_IEH_STACK, 0, 20, 0, 0, 1, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 0, 0, 2, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  8, 0, 0, 3, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  9, 0, 0, 4, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 10, 0, 0, 5, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 11, 0, 0, 6, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 12, 0, 0, 7, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 13, 0, 0, 8, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 14, 0, 0, 9, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 15, 0, 0, 10, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 16, 0, 0, 11, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 17, 0, 0, 12, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 18, 0, 0, 13, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 19, 0, 0, 14, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 26, 0, 0, 15, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 27, 0, 0, 16, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 28, 0, 0, 17, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 29, 0, 0, 18, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 7, 0, 19, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 4, 0, 20, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 0, 0, 21, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 23, 0, 0, 22, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 24, 0, 0, 23, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 25, 0, 0, 24, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 6, 0, 25, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 31, 3, 0, 26, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0, 22, 0, 0, 27, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  0, 0, 0, 28, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  1, 0, 0, 29, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  0, 0, 0, 30, 0}, \
                {IehEndPoint, EBG_IEH_STACK, 0,  0, 0, 0, 31, 0},

LOOKUP_TBL_ENTRY  SprSpLookupTbl[] = {

  //
  //global IEH
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 0xff, 0, 0, 3, 0, 0, 0},
  {SatelliteIehSouth, 0xff, 0, 0x14, 4, 0, 1, EBG_IEH_STACK}, //EBG South IEH
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 2, 0x08}, //North Satellite IEH on Stack 8
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 3, 0x00}, //North Satellite IEH on Stack 0
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 4, 0x01}, //North Satellite IEH on Stack 1
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 5, 0x09}, //North Satellite IEH on Stack 9
  {NullDevice, 0xff, 0, 0, 4, 0, 6, 0x06}, //North Satellite IEH on Stack 6
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 7, 0x02}, //North Satellite IEH on Stack 2
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 8, 0x0A}, //North Satellite IEH on Stack 10
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 9, 0x03}, //North Satellite IEH on Stack 3
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 10, 0x04}, //North Satellite IEH on Stack 4
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 11, 0x0B}, //North Satellite IEH on Stack 11
  {NullDevice, 0xff, 0, 0, 4, 0, 12, 0x07}, //North Satellite IEH on Stack 7
  {SatelliteIehNorth, 0xff, 0, 0, 4, 0, 13, 0x05}, //North Satellite IEH on Stack 5
  //
  //satellite IEH 0
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //share devices should put together as incremental sequence.
  //
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 0, 0, 0, 4, 572, 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehLocalDevice, 0, 0, 0, 4, 466, 0, (SpdI3cBus << 8) | 4},          //Local Error Source 4
  {IehLocalDevice, 0, 0, 0, 4, 467, 0, (SpdI3cBus << 8) | 5},          //Local Error Source 5
  {IehLocalDevice, 0, 0, 0, 4, 0, 0, (CxpSmBus << 8) | 6},           //Local Error Source 6
  {IehEndPoint, 0, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 0, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 0, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 0, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 0, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 0, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 0, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 0, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 0, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 0, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 0, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 1
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 1, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 1, 0, 0, 4, 592 + (0x28 << 24), 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 1, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 1, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 1, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 1, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 1, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 1, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 1, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 1, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 1, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 1, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 1, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 2
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 2, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 2, 0, 0, 4, 612 + (0x28 << 24), 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 2, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 2, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 2, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 2, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 2, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 2, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 2, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 2, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 2, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 2, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 2, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 3
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 3, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 3, 0, 0, 4, 652 + (0x28 << 24), 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 3, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 3, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 3, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 3, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 3, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 3, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 3, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 3, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 3, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 3, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 3, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 4
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 4, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 4, 0, 0, 4, 632 + (0x28 << 24), 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 4, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 4, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 4, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 4, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 4, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 4, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 4, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 4, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 4, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 4, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 4, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 5
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 5, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 5, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 5, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 5, 0, 0, 4, 672 + (0x28 << 24), 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 5, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 5, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 5, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 5, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 5, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 5, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 5, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 5, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 5, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 5, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 5, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x
/*
  //
  //satellite IEH 6
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 6, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 6, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 6, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 6, 0, 0, 4, 0, 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 6, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 6, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 6, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 6, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 6, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 6, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 6, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 6, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 6, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 6, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 6, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x

  //
  //satellite IEH 7
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 7, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 7, 0, 0, 4, 0, 0, (PsfPciePi5 << 8) | 1},         //Local Error Source 1
  {IehLocalDevice, 7, 0, 0, 4, 0, 0, (PcieGen4DmiPi5 << 8) | 2},     //Local Error Source 2
  {IehLocalDevice, 7, 0, 0, 4, 0, 0, (PcieGen5IalPi5 << 8) | 3},     //Local Error Source 3
  {IehEndPoint, 7, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 7, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 7, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 7, 0, 8, 0, 0, 2, 0},        //a.PCIEGen4_RP.PI5.x
  {IehEndPoint, 7, 0, 6, 0, 0, 3, 0},        //b.PCIEGen4_RP.PI5.x
  {IehEndPoint, 7, 0, 4, 0, 0, 4, 0},        //c.PCIEGen4_RP.PI5.x
  {IehEndPoint, 7, 0, 2, 0, 0, 5, 0},        //d.PCIEGen4_RP.PI5.x
  {IehEndPoint, 7, 0, 1, 0, 0, 6, 0},        //a.PCIEGen5_RP.PI5.x
  {IehEndPoint, 7, 0, 3, 0, 0, 7, 0},        //b.PCIEGen5_RP.PI5.x
  {IehEndPoint, 7, 0, 5, 0, 0, 8, 0},        //c.PCIEGen5_RP.PI5.x
  {IehEndPoint, 7, 0, 7, 0, 0, 9, 0},        //d.PCIEGen5_RP.PI5.x
*/

  //
  //satellite IEH 8
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 8, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 8, 0, 0, 4, 0, 0, (PsfDino << 8) | 1},    //Local Error Source 1
  {IehLocalDevice, 8, 0, 0, 4, 0, 0, (PsfHcX << 8) | 2},     //Local Error Source 2
  {RCiEP, 8, 0, 1, 0, 0, 0, 0},              //DSA.DINO.x
  {RCiEP, 8, 0, 2, 0, 0, 0, 0},              //IAX.DINO.x
  {RCiEP, 8, 0, 3, 0, 0, 0, 0},              //MSM.OOB_MSM.DINO.x
  {RCiEP, 8, 0, 3, 1, 0, 0, 0},              //PMON.OOB_MSM.DINO.x
  {RCiEP, 8, 0, 3, 2, 0, 0, 0},              //DFx.OOB_MSM.DINO.x
  {RCiEP, 8, 1, 0, 0, 0, 0, 0},              //PF.CPM.HCx.x
  {RCiEP, 8, 2, 0, 0, 0, 0, 0},              //PF.HQM0.HCx.x
  {IehEndPoint, 8, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 8, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 8, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x

  //
  //satellite IEH 9
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 9, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 9, 0, 0, 4, 0, 0, (PsfDino << 8) | 1},    //Local Error Source 1
  {IehLocalDevice, 9, 0, 0, 4, 0, 0, (PsfHcX << 8) | 2},     //Local Error Source 2
  {RCiEP, 9, 0, 1, 0, 0, 0, 0},              //DSA.DINO.x
  {RCiEP, 9, 0, 2, 0, 0, 0, 0},              //IAX.DINO.x
  {RCiEP, 9, 0, 3, 0, 0, 0, 0},              //MSM.OOB_MSM.DINO.x
  {RCiEP, 9, 0, 3, 1, 0, 0, 0},              //PMON.OOB_MSM.DINO.x
  {RCiEP, 9, 0, 3, 2, 0, 0, 0},              //DFx.OOB_MSM.DINO.x
  {RCiEP, 9, 1, 0, 0, 0, 0, 0},              //PF.CPM.HCx.x
  {RCiEP, 9, 2, 0, 0, 0, 0, 0},              //PF.HQM0.HCx.x
  {IehEndPoint, 9, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 9, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 9, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x

  //
  //satellite IEH 10
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 10, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 10, 0, 0, 4, 0, 0, (PsfDino << 8) | 1},    //Local Error Source 1
  {IehLocalDevice, 10, 0, 0, 4, 0, 0, (PsfHcX << 8) | 2},     //Local Error Source 2
  {RCiEP, 10, 0, 1, 0, 0, 0, 0},              //DSA.DINO.x
  {RCiEP, 10, 0, 2, 0, 0, 0, 0},              //IAX.DINO.x
  {RCiEP, 10, 0, 3, 0, 0, 0, 0},              //MSM.OOB_MSM.DINO.x
  {RCiEP, 10, 0, 3, 1, 0, 0, 0},              //PMON.OOB_MSM.DINO.x
  {RCiEP, 10, 0, 3, 2, 0, 0, 0},              //DFx.OOB_MSM.DINO.x
  {RCiEP, 10, 1, 0, 0, 0, 0, 0},              //PF.CPM.HCx.x
  {RCiEP, 10, 2, 0, 0, 0, 0, 0},              //PF.HQM0.HCx.x
  {IehEndPoint, 10, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 10, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 10, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x

  //
  //satellite IEH 11
  //{IehDevType, Stack, BusOffset, Device, Function, SbPortId, BitIndex, DevSpecInfo}
  //
  {IehLocalDevice, 11, 0, 0, 4, 0, 0, 0},     //Satellite IEH
  {IehLocalDevice, 11, 0, 0, 4, 0, 0, (PsfDino << 8) | 1},    //Local Error Source 1
  {IehLocalDevice, 11, 0, 0, 4, 0, 0, (PsfHcX << 8) | 2},     //Local Error Source 2
  {RCiEP, 11, 0, 1, 0, 0, 0, 0},              //DSA.DINO.x
  {RCiEP, 11, 0, 2, 0, 0, 0, 0},              //IAX.DINO.x
  {RCiEP, 11, 0, 3, 0, 0, 0, 0},              //MSM.OOB_MSM.DINO.x
  {RCiEP, 11, 0, 3, 1, 0, 0, 0},              //PMON.OOB_MSM.DINO.x
  {RCiEP, 11, 0, 3, 2, 0, 0, 0},              //DFx.OOB_MSM.DINO.x
  {RCiEP, 11, 1, 0, 0, 0, 0, 0},              //PF.CPM.HCx.x
  {RCiEP, 11, 2, 0, 0, 0, 0, 0},              //PF.HQM0.HCx.x
  {IehEndPoint, 11, 0, 0, 0, 0, 1, 0},        //MMap/VT-d.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 11, 0, 0, 1, 0, 1, 0},        //M2IAL.MS2IOSF.SCF_IOCOH.x
  {IehEndPoint, 11, 0, 0, 2, 0, 1, 0},        //RAS.MS2IOSF.SCF_IOCOH.x

  //
  // EBG satellite IEH
  //
  EBG_IEH

  //indictor on end of table.
  {IehDevTypeEnd, 0, 0, 0, 0, 0, 0, 0},
};

/**
  get lookup table per CPU type
  wave3 need lookup table.

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
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return SprSpLookupTbl;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] ERROR: Lookup table is not found\n"));
  return NULL;
}
