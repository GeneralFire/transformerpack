/** @file
  Implementation of IEH device detection.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include "IehRasLibInternal.h"


extern NODE_DEVICE   *mGlobalIEH;


/**
  This routine returns whether global IEH exist.

  @param -- none
  @retval   TRUE  -- IEH exist; FALSE -- IEH not exist

**/
BOOLEAN
EFIAPI
IsGlobalIehExist (
  VOID
  )
{
  BOOLEAN  Flag = FALSE;
  UINT8    i;

  for (i = 0; i < MAX_SOCKET; i++) {
    if (mGlobalIEH[i].Common.Type == GlobalIeh) {
      Flag = TRUE;
      break;
    }
  }

  return Flag;
}

/**
  This routine returns whether IioRas Device exist.
  For 10nm will check whether Global IEH exist.

  @param -- none
  @retval   TRUE  -- Iio device exist; FALSE -- Iio device not exist

**/
BOOLEAN
EFIAPI
IioRasDeviceExist (
  VOID
  )
{
  return IsGlobalIehExist ();
}

/**
 This routine 's purpose is to search IEH devices from whole system.

 @param [IN,OUT]  IEH pointer to save IEH device information
 @retval  IEH device number. If none, return 0.
**/
UINT8
EFIAPI
SearchIehDevice (
  IN OUT  NODE_DEVICE *TempIEH
  )
{
  UINT8                       Socket;
  UINT8                       IioStack;
  UINT8                       Bus;
  UINT8                       Dev;
  UINT8                       Func;
  UINT8                       Index;
  UINT8                       PchIehDevice;
  UINT8                       PchIehFunction;
  UINT8                       IehDevType;
  UINT8                       IehVersion;
  UINT8                       IEHDeviceCounter = 0;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[IEH] Search all IEH device in the system \n"));
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent(Socket) == FALSE) {
      continue;
    }

    for (IioStack = 0; IioStack < MAX_LOGIC_IIO_STACK; IioStack ++ ) {

      //
      // Validate stack and Get stack bus number
      //
      if (IioStack < MAX_IIO_STACK) {
        if (!GetStackStartBus (Socket, IioStack, &Bus)) {
          continue;
        }
      } else if ((IioStack == UBOX_STACK)) {
        if (!GetSocket10nmUboxBus (Socket, &Bus)) {
          continue;
        }
      } else {
        continue;
      }
      //
      // Start to detect IEH device
      //
      if (GetIeh (Socket, IioStack, &Bus, &Dev, &Func, &IehDevType, &IehVersion)) {
          TempIEH[IEHDeviceCounter].Common.Socket = Socket;
          TempIEH[IEHDeviceCounter].Common.Bus = Bus;
          TempIEH[IEHDeviceCounter].Common.Dev = Dev;
          TempIEH[IEHDeviceCounter].Common.Func = Func;
          TempIEH[IEHDeviceCounter].Common.Type = IehDevType;
          if (TempIEH[IEHDeviceCounter].Common.Type == GlobalIeh) {
            TempIEH[IEHDeviceCounter].Common.Stack = 0xff;
            TempIEH[IEHDeviceCounter].GlobalIEH.IehVer = IehVersion;
          } else {
            TempIEH[IEHDeviceCounter].Common.Stack = IioStack;
            TempIEH[IEHDeviceCounter].SatelliteIEH.IehVer = IehVersion;
          }
          RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH] found ieh device: Socket:0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x ieh  stack:0x%x ",
                      Socket, Bus, TempIEH[IEHDeviceCounter].Common.Dev, TempIEH[IEHDeviceCounter].Common.Func,
                                                                          TempIEH[IEHDeviceCounter].Common.Stack));
          DisplayIehDevType (TempIEH[IEHDeviceCounter].Common.Type, TRUE);
          IEHDeviceCounter ++;
        }


      //
      // Looking for South IEH
      //
      if (IioStack == 0) {
        if (PchRasGetIehDevice (GetPcieSegment (Socket), Bus, &PchIehDevice, &PchIehFunction, NULL, &IehDevType)) {
          TempIEH[IEHDeviceCounter].Common.Socket = Socket;
          TempIEH[IEHDeviceCounter].Common.Bus = Bus;
          TempIEH[IEHDeviceCounter].Common.Dev = PchIehDevice;
          TempIEH[IEHDeviceCounter].Common.Func = PchIehFunction;
          TempIEH[IEHDeviceCounter].Common.Type = SatelliteIehSouth;
          TempIEH[IEHDeviceCounter].Common.Stack = CDF_IEH_STACK;
          TempIEH[IEHDeviceCounter].SatelliteIEH.IehVer = (IehDevType & 0xf0) >> 4;
          RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH] found PCH south ieh device: Socket:0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x ieh   ",
                          Socket, Bus, TempIEH[IEHDeviceCounter].Common.Dev, TempIEH[IEHDeviceCounter].Common.Func));

          DisplayIehDevType (TempIEH[IEHDeviceCounter].Common.Type, TRUE);
          IEHDeviceCounter ++;
        }
      }
    }

  }

  //
  //Fill dummy data.
  //
  for (Index = IEHDeviceCounter; Index < MAX_IEH_DEVICE; Index ++) {
    TempIEH[Index].Common.Socket = 0xff;
    TempIEH[Index].Common.Type = IehDevTypeEnd;
  }
  RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH] total IEH devices:0x%x on system \n",IEHDeviceCounter));
  return   IEHDeviceCounter;
}


