/** @file
  Debug routines for dumping training states, values or margins.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioDataHubLib.h>
#include "../Common/IioEvDebug.h"
#include <LibraryPrivate/IioDebug.h>

/**
  Find the actual lane incase of lane reversal.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to IIO Port
  @param[in] Lane                 - Current Lane
**/
UINT8
GetAbsLane (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8           IioIndex,
  IN  UINT8           PortIndex,
  IN  UINT8           Lane
  )
{
  UINT8               LaneOffset = 0;

  if (PortIndex == 1) {
    LaneOffset = 4;
  }

  if (PortIndex > 1) {
    LaneOffset = ((PortIndex - 1) % 4) * 4;
  }

  return LaneOffset + Lane;
}

/**
  Gets and returns CTLE peak value.

  @param[in] IioGlobalData  - Pointer to IIO_GLOBALS
  @param[in] IioIndex       - Index to IIO
  @param[in] PortIndex      - Index to IIO Port
  @param[in] Lane           - Lane
  @param[in] Speed          - PortSpeed

  @retval CTLE peaking code
**/
UINT32
GetPcieCtle (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT8         Lane,
  IN  UINT16        Speed
  )
{
  UINT8             AbsLane;
  UINT32            Ctlep = 0;

  AbsLane = GetAbsLane (IioGlobalData, IioIndex, PortIndex, Lane);

  switch (Speed) {
  case 1:
  case 2:
  case 3:
    break;
  default:
    Ctlep = 0;
    break;
  }
  //
  // Place holder for reading CTLE peaking code
  //
  return Ctlep;
}

/**
  Gets and returns specified monbus parameter.

  @param[in] IioGlobalData          - Pointer to IIO_GLOBALS
  @param[in] IioIndex               - Index to Iio
  @param[in] PortIndex              - Index to IIO port
  @param[in] Lane                   - Current Lane
  @param[in] Param                  - Param index

  @retval ReadData
**/
UINT32
GetPcieMonBus (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT8         Lane,
  IN  UINT32        Param
  )
{
  UINT32            ReadData = 0;
  return ReadData;
}

/**
  Gets and returns specified tracebus parameter.

  @param[in] IioGlobalData          - Pointer to IIO_GLOBALS
  @param[in] IioIndex               - Index to Iio
  @param[in] PortIndex              - Index to IIO port
  @param[in] Lane                   - Current Lane
  @param[in] Param                  - Param index

  @retval ReadData
**/
UINT32
GetPcieTraceBus (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT8         Lane,
  IN  UINT32        Param
  )
{
  UINT32            ReadData = 0;
  return ReadData;
}

/**
  The purpose is dump information about the errors found in
  the receiver if any.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to the Iio Port
**/
VOID
ReceiverErrors (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  )
{
  //
  // TODO:
  // Verify when 0p8 register drop is avaliable.
  //
}

/**
  The purpose is to clear all the errors in the receiver.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to the Iio Port
**/
VOID
ReceiverErrrorsClear (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  //
  // TODO:
  // Verify when 0p8 register drop is avaliable.
  //
}

/**
  Gets and returns specified monbus parameter.

  @param[in] IioGlobaData           - Pointer to IIO_GLOBALS
  @param[in] IioIndex               - Index to Iio
  @param[in] PortIndex              - Index to Iio port
  @param[in] LinkWidth              - Link Width of current Port

  @retval - ReadData
**/
VOID
GetPcieJitterModeMargin (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               PortIndex,
  IN  UINT16              LinkWidth
  )
{
  UINT8                   Lane;
  UINT8                   StopOnError;
  UINT16                  Peak = 0;
  UINT16                  Step = 0;
  UINT16                  Loop = 0;
  UINT32                  ErrCnt = 0;

  IIO_D_PCIDBG ("Jitter Margins\n");

  for (Lane = 0; Lane < LinkWidth; Lane++) {
    Peak = 0;
    StopOnError = 0;
    while ((Peak < 64) & (StopOnError == 0 )) {
      Peak += 1;
      //
      // set peak/step/enable
      //
      Step = Peak * 8;
      if (Step > 255) {
        Step = 255;
      }
      //
      // clear errors
      //
      for (Loop = 0; Loop < 10; Loop++) {
        IioStall (1000); // dwell time

        if (ErrCnt > 0) {
          StopOnError = 1;
          break;
        } // ErrCnt check
      } // Loopcount
    } // Peak loop
    IIO_D_PCIDBG ("S%d:P%d:Lane%2d, Peak = %2d, Step = %3d, Loop= %2d\n",
        IioIndex, PortIndex, Lane, Peak, Step, Loop - 100);
  }
}

/**
  The purpose is to setup the LTSSM logger.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to the Iio Port
  @param[in] Speed                - PortSpeed
  @param[in] Stop                 - Flag to Stop or Start logger
  @param[in] Mask                 - Mask
**/
VOID
LoggerStartStop (
  IN  IIO_GLOBALS                        *IioGlobalData,
  IN  UINT8                               IioIndex,
  IN  UINT8                               PortIndex,
  IN  UINT8                               Speed,
  IN  UINT8                               Stop,
  IN  UINT8                               Mask
  )
{
  //
  // TODO:
  // Verify when 0p8 register drop is avaliable.
  //
}

/**
  The purpose is to dump the LTSSM logger results.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to Iio Port
**/
VOID
LogSnapShot (
  IN  IIO_GLOBALS                        *IioGlobalData,
  IN  UINT8                               IioIndex,
  IN  UINT8                               PortIndex
  )
{
  //
  // TODO:
  // Verify when 0p8 register drop is avaliable.
  //
}

/**
  Gets and print rx stats per lane.

  @param[in] IioGlobalData - Pointer to IioGlobalData
**/
VOID
ShowRxStatsPrivate (
  IN  IIO_GLOBALS             *IioGlobalData
  )
{
  UINT8                        IioIndex;
  UINT8                        PortIndex;
  UINT8                        Lane;
  UINT8                        MaxPortNumberPerSocket;
  IIO_V_DATA                  *IioVData;
  LINKSTS_IIO_PCIE_G5_STRUCT   LnkSts;

  LnkSts.Data = 0;

  IioVData = GetIioVData ();

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioVData->SocketPresent[IioIndex]) {
      continue;
    }

    MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
    for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
      if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
        continue;
      }

      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
        LnkSts.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIEDMI_REG);
      } else {
        LnkSts.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIE_G5_REG);
      }

      if (LnkSts.Bits.nlw == 1 || LnkSts.Bits.nlw == 2 ||
          LnkSts.Bits.nlw == 4 || LnkSts.Bits.nlw == 8 ||
          LnkSts.Bits.nlw == 16) {
        IIO_D_PCIDBG ("\n Socket:%d, Port%d ", IioIndex, PortIndex);
        IIO_D_PCIDBG (" Link up as x%d Gen%d", LnkSts.Bits.nlw, LnkSts.Bits.cls);
        IIO_D_PCIDBG ("\n------------------------------------------");
        if ((LnkSts.Bits.nlw > 0) & (PortIndex > 0)) {
          GetPcieJitterModeMargin (IioGlobalData, IioIndex, PortIndex, LnkSts.Bits.nlw);
        }
        IIO_D_PCIDBG ("\n Lane CTLEP   AGC   CDR CTOC1 CTOC2  DFE1  DFE2  DFE3  DFE4   ");
        IIO_D_PCIDBG ("DS0   DS1  ESM0  ESM1  ESP0  ESP1 ICOMP RVREF RXDCC SQLCH  SUMR");

        for (Lane = 0; Lane < LnkSts.Bits.nlw; Lane++) {
          IIO_D_PCIDBG ("\n   %2d", Lane);
        } // lane loop
      } // if link up check
    }
  }
}
