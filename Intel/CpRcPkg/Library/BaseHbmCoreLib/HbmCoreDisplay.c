/** @file
  Interface source file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include "HbmCore.h"
#include "Library/SysHostPointerLib.h"

#define LINE_SIZE 120

/**

  Display HBM Table Header information

  @param[in]                Socket              Current Socket (0-Based)

**/
VOID
HbmDisplayHeader(
  IN        UINT8           Socket
  )
{
  HbmDebugPrint (HBM_SDBG_MINMAX,"|                                        HBM Socket:%d Display Device Info                                              |\n", Socket);
  HbmDebugPrintLine ("|    Field     |      HBMIO MODULE 1     |      HBMIO MODULE 2     |      HBMIO MODULE 3     |      HBMIO MODULE 4     |\n");

} // HbmDisplayHeader

/**

  Display HBM Memory Stack hight information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayStackInfo (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{
  if (!(*ChDevId).Bits.hbm_stack_height) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          2 or 4         ");
  } else {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "             8           ");
  } // if stack hight
} // HbmDisplayStackInfo


/**

  Display HBM Memory Channel enabled information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayChEnable (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{
  UINT8   Bit;
  CHAR8   BinaryStr[9];

  BinaryStr[8] = 0;
  for (Bit = 0; Bit < 8; Bit++) {
    BinaryStr[Bit] = (((*ChDevId).Bits.channel_available >> Bit) & 0x1) ?  '1' : '0';
  }
  HbmDebugPrint (HBM_SDBG_MINMAX, "|         %a        ", BinaryStr);
} // HbmDisplayChEnable

/**

  Display HBM Memory Addressing Mode information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayAddrMode (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{
  if (1 == (*ChDevId).Bits.addresing_mode) {
    HbmDebugPrint (HBM_SDBG_MINMAX,"|%25a", "        Pseudo Ch        ");
  } else {
    HbmDebugPrint (HBM_SDBG_MINMAX,"|%25a", "         Illegal         ");
  }
} // HbmDisplayAddrMode

/**

  Display HBM Memory Addressing Mode information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayVid (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{
  if (HBM_VID_SAMSUNG == (*ChDevId).Bits.manufacturer_id) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         SAMSUNG         ");
  } else if (HBM_VID_SKHYNIX == (*ChDevId).Bits.manufacturer_id) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         SK HYNIX        ");
  } else if (HBM_VID_MICRON == (*ChDevId).Bits.manufacturer_id) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          MICRON         ");
  } else {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         Reserved        ");
  } // if manufacturer id
} // HbmDisplayVid

/**

  Display HBM Memory Density information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayDensity (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{

  switch ((*ChDevId).Bits.density) {

  case Hbm1Gb:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          1 Gbit         ");
    break;

  case Hbm2Gb:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          2 Gbit         ");
    break;

  case Hbm4Gb:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          4 Gbit         ");
    break;

  case Hbm6Gb:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          6 Gbit         ");
    break;

  case Hbm8Gb8H:
  case Hbm8Gb:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          8 Gbit         ");
    break;

  case Hbm12Gb8H:
  case Hbm8Gb12H:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         12 Gbit         ");
    break;

  case Hbm16Gb8H:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         16 Gbit         ");
    break;

  case Hbm12Gb12H:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         18 Gbit         ");
    break;

  case Hbm16Gb12H:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         24 Gbit         ");
    break;

  default:
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         Reserved        ");
    break;
  } // switch density
} // HbmDisplayDensity

/**

  Display HBM Memory ECC Support information

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayEcc (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
  )
{
  if (1 == (*ChDevId).Bits.ecc) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "           ECC           ");
  }
  else {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "          NO ECC         ");
  }
} // HbmDisplayEcc

/**

  Display HBM Memory Gen2 Test Support

  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                ChDevId             Pointer to Device ID information

**/
VOID
HbmDisplayGen2 (
  IN        UINT8           Ch,
  IN        DEVICE_ID_WDR   *ChDevId
)
{
  if (1 == (*ChDevId).Bits.gen2_test) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         Supported       ");
  }
  else {
    HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "        No Support       ");
  }
} // HbmDisplayGen2

/**

  Display HBM Memory information

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/

UINT32
EFIAPI
HbmMemDisplay (
  IN        UINT8           Socket
  )
{
  UINT8           Ch       = 0;
  UINT8           IoModule = 0;
  UINT32          TmpVar32 = 0;
  DEVICE_ID_WDR   *ChDevId;
  HBM_HOST        *HbmHost;

  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  if (!IsHbmFlowEnabled (HF_DISPLAY)) {
    return EFI_SUCCESS;
  }

  HbmDisplayHeader (Socket);

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Module Enable ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    if (IsHbmMemSsEnabled (Socket, IoModule)) {
      HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         ENABLED         ");
    } else {
      HbmDebugPrint (HBM_SDBG_MINMAX, "|%25a", "         DISABLED        ");
    } // if module enable
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Model num     ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDebugPrint (HBM_SDBG_MAX,"|            %02X           ", (*ChDevId).Bits.model_part_num);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Stack High    ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayStackInfo (0, ChDevId);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Channel enable");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayChEnable (0, ChDevId);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Address Mode  ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayAddrMode (Ch, ChDevId);
  } // for IoModule
  HbmDebugPrint (HBM_SDBG_MAX,"|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Serial number ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    //
    // serial_number_low (width = 14)
    //
    TmpVar32  = ChDevId->Bits.serial_number_low;
    //
    // Serial_number_hi (width = 34), only 32 bits will be concatenated, the remaining
    // 2 bits will be printed directly from serial_number_hi [19:18]
    //
    TmpVar32 |= ChDevId->Bits.serial_number_hi << 14;
    HbmDebugPrint (HBM_SDBG_MAX,"|        %02x%08x       ", ((*ChDevId).Bits.serial_number_hi >> 18) & 0x3, TmpVar32);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX, "|%14a", "Manuf. Week   ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDebugPrint (HBM_SDBG_MAX,"|           WW%02d          ", (*ChDevId).Bits.manufacturing_week);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Manuf. Year   ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    // TmpVar32[7:0] = {manufacturing_year_hi[3:0],manufacturing_year_low[3:0]}
    TmpVar32  = ChDevId->Bits.manufacturing_year_low | (ChDevId->Bits.manufacturing_year_hi << 4);

      HbmDebugPrint (HBM_SDBG_MAX,"|           %04d          ", TmpVar32 + 2011);
    } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX, "|%14a", "Manuf. loc    ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDebugPrint (HBM_SDBG_MAX,"|           %04x          ", (*ChDevId).Bits.manufacturing_loc);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX, "|%14a", "Manuf. ID     ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayVid (Ch, ChDevId);
  } // for Ch
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "Density       ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayDensity (Ch, ChDevId);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX,"|%14a", "ECC           ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayEcc (Ch, ChDevId);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  HbmDebugPrint (HBM_SDBG_MAX, "|%14a", "Gen2 Test     ");
  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    ChDevId = &HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId;
    HbmDisplayGen2 (Ch, ChDevId);
  } // for IoModule
  HbmDebugPrintLine ("|\n");

  return EFI_SUCCESS;
} // HbmMemDisplay

/**

  Function to print out the final training Results for HBM training steps.


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                TrainType           Training margin Group selector.
  @param[in]                NumRows             Row size for 2-dimensional array PrevData.
  @param[in]                NumCols             Column size for 2-dimensional array PrevData.
  @param[in]                PrevData            2-dimentional array that stores settings before
                                                training begins.
                                                NULL indicates only display current values.
  @param[in]                MaxStrobe           Number of strobes for this signal.
  @param[in]                HbmDisplayType      1 indicates displaying Strobe based Results;
                                                0 indicates channel based Results.

**/
VOID
HbmPrintResults (
  IN        UINT8           Socket,
  IN        UINT16          TrainType,
  IN        UINT8           NumRows,
  IN        UINT8           NumCols,
  IN        INT16           *PrevData,
  IN        UINT8           MaxStrobe,
  IN        UINT8           HbmDisplayType
  )
{
  UINT8               Strobe;
  UINT8               IoModule;
  UINT8               StartCh;
  UINT8               EndCh;
  UINT8               Ch;
  UINT8               PrintLineCount;
  INT16               NewData[MAX_HBM_CH][MAX_HBM_STROBE];

  if (NumRows != MAX_HBM_CH) {
    return;
  }

  if (NumCols < MaxStrobe) {
    return;
  }

  for (IoModule = 0; IoModule < HbmGetMaxMemSs (Socket); IoModule++) {
    if (!IsHbmMemSsEnabled (Socket, IoModule)) {
      continue;
    }

    StartCh = IoModule * HbmGetMaxChannelMemSs (Socket, IoModule);
    EndCh   = StartCh + HbmGetMaxChannelMemSs (Socket, IoModule);

    if (HbmDisplayType == HBM_DISPLAY_CHANNEL) {
      HbmDebugPrintLine ("Channel         ");
      PrintLineCount = 50;
    } else {
      HbmDebugPrintLine ("Words           ");
      PrintLineCount = 145;
    }

    for (Ch = StartCh; Ch < EndCh; Ch++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        HbmDebugPrint (HBM_SDBG_MAX, "%3d ", Strobe + ((Ch-StartCh) * MaxStrobe));
      }
    }
    if (PrevData != NULL) {
      HbmDebugPrintDevice (HBM_SDBG_MAX, Socket, HBM_IO_REPLACE | IoModule, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Prev:  ");
      for (Ch = StartCh; Ch < EndCh; Ch++) {
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          if (IsHbmChannelEnabled (Socket, Ch)) {
            HbmDebugPrint (HBM_SDBG_MAX, "%03d ", *(PrevData + Ch * NumCols + Strobe));
          } else {
            HbmDebugPrintLine ("--- ");
          }
        }
      }
      HbmDebugPrintLine ("\n");
    }
    HbmDebugPrintDevice (HBM_SDBG_MAX, Socket, HBM_IO_REPLACE | IoModule, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  New:  ");
    for (Ch = StartCh; Ch < EndCh; Ch++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //TODO //HbmGetSetDataGroup (Socket, Ch, Strobe, TrainType, HBM_GSM_READ_UC, &NewData[Ch][Strobe]);
        if (IsHbmChannelEnabled (Socket, Ch)) {
          HbmDebugPrint(HBM_SDBG_MAX, "%03d ", NewData[Ch][Strobe]);
        } else {
          HbmDebugPrintLine ( "--- ");
        }
      }
    }
    HbmDebugPrintLine ("\n");
    if (PrevData != NULL) {
      HbmDebugPrintDevice (HBM_SDBG_MAX, Socket, HBM_IO_REPLACE | IoModule, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Diff:  ");
      for (Ch = StartCh; Ch < EndCh; Ch++) {
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          if (IsHbmChannelEnabled (Socket, Ch)) {
            HbmDebugPrint (HBM_SDBG_MAX, "%03d ", NewData[Ch][Strobe] - *(PrevData + Ch * NumCols + Strobe));
          } else {
            HbmDebugPrintLine ("--- ");
          }
        }
      }
    }
  }
}

