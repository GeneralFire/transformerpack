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

#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/PrintLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/TimerLib.h>
#include <Library/SpdAccessLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/MemRoutingLib.h>
#include <MemRegs.h>

MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[] = {MEMORY_MODULE_MANUFACTURERS};

const UINT16  MfgListLength = sizeof(MemoryModuleManufacturerList)/sizeof(MemoryModuleManufacturerList[0]);

// -------------------------------------
// Code section
// -------------------------------------

#ifdef DEBUG_CODE_BLOCK
//
// Local Prototypes
//
VOID DisplaySize (PSYSHOST  Host, UINT8 socket, UINT8 dimm);
VOID DisplayDimmType (PSYSHOST  Host, UINT8 socket, UINT8 dimm);
VOID DisplayDramMfgDate (PSYSHOST  Host, UINT8 socket, UINT8 dimm);
VOID DisplaySpeed (PSYSHOST  Host, UINT8 socket, UINT8 dimm);
VOID DisplayModulePartNum (PSYSHOST  Host, UINT8 socket, UINT8 dimm);
VOID DisplayIntelSN (IN PSYSHOST Host, IN UINT8 socket, IN UINT8 dimm);
VOID DisplayDimmSN (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Dimm);
VOID DisplayEye (PSYSHOST Host, UINT8 socket, UINT8 scope, MRC_LT level, VOID *results, UINT8 maxVrefOffset);
VOID DisplayMRS (PSYSHOST Host, UINT8 socket);
VOID DisplayNvdimmN (PSYSHOST  Host, UINT8 Socket, UINT8 Dimm);
#ifdef DDR5_SUPPORT
VOID DisplayModulePartNumDdr5 (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Dimm);
#endif //DDR5_SUPPORT
#define DimmInfoPageWidth 20
/**
  Prints out the per Strobe cached values of the input MRS registers

  @param[in]      Host         Pointer to SYSHOST
  @param[in]      Socket       Socket number
  @param[in]      Ch           Channel number
  @param[in]      Dimm         Dimm number
  @param[in]      Rank         Rank number
  @param[in]      MrsRegister  Desired MRS register to print (0 for MR0, 1 for MR1, etc...)

  @retval   EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MemDebugDumpPerStrobeMrs (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     MrsRegister
  );

//
// Memory training margin group selector strings
//
GROUP_STRING  mGroupString[] = {
  {RecEnDelay,          "RecEnDelay"               },
  {RxDqsDelay,          "RxDqsDelay"               },
  {RxDqDqsDelay,        "RxDqDqsDelay"             },
  {RxDqsPDelay,         "RxDqsPDelay"              },
  {RxDqsNDelay,         "RxDqsNDelay"              },
  {RxDqsBitDelay,       "RxDqsBitDelay"            },
  {RxDqsPBitDelay,      "RxDqsPBitDelay"           },
  {RxDqsNBitDelay,      "RxDqsNBitDelay"           },
  {TxDqBitDelay,        "TxDqBitDelay"             },
  {RxDqDelay,           "RxDqDelay"                },
  {WrLvlDelay,          "WrLvlDelay"               },
  {TxDqsDelay,          "TxDqsDelay"               },
  {TxDqDelay,           "TxDqDelay"                },
  {RxVref,              "RxVref"                   },
  {TxVref,              "TxVref"                   },
  {RxEq,                "RxEq"                     },
  {PxcDelta,            "PxcDelta"                 },
  {DdjcDelta,           "DdjcDelta"                },
  {CmdVref,             "CmdVref"                  },
  {CmdAll,              "CmdAll"                   },
  {CtlAll,              "CtlAll"                   },
  {TxEq,                "TxEq"                     },
  {TxEqTap2,            "TxEqTap2"                 },
  {RxDfeCoefficient,    "RxDfeCoefficient"         },
  {RxDfe,               "RxDfeCoefficient"         },
  {RxStrobeInvert,      "RxStrobeInversionDelay"   },
  {RxFlyBy,             "RxFlyBy"                  },
  {TxFlyBy,             "TxFlyBy"                  },
  {DramDrvStr,          "DramRon"                  },
  {RxOdt,               "McOdt"                    },
  {TxRon,               "McRon"                    },
  {CmdTxEq,             "CmdTxEq"                  },
  {TxImode,             "TxImode"                  },
  {RxCtleC,             "RxCtleC"                  },
  {RxCtleR,             "RxCtleR"                  },
  {TxRiseFallSlewRate,  "TxRiseFallSlewRate"       },
  {TxFallSlewRate,      "TxFallSlewRate"           },
  {TxTco,               "TxTco"                    },
  {DcaDelay,            "DcaDelay"                 },
  {DcaDfeVref,          "DcaDfeVref"               },
  {RcdDcaVref,          "RcdDcaVref"               },
  {DcaTco,              "DcaTco"                   },
  {LinearCntlDcaTco,    "LinearCntlDcaTco"         },
  {TcoDqDqs,            "TcoDqDqs"                 },
  {LinearCntlTcoDqDqs,  "LinearCntlTcoDqDqs"       },
  {ParkOdt,             "ParkOdt"                  },
  {WrOdt,               "WrOdt"                    },
  {NomOdt,              "NomOdt"                   },
  {RxSenseAmpOffset,    "RxSenseAmpOffset"         },
  {RxFifoDqsPBitSkew,   "RxFifoDqsPBitSkew"        },
  {RxFifoDqsNBitSkew,   "RxFifoDqsNBitSkew"        },
  {DramRxEq,            "DramRxEq"                 },
  {RxBias,              "RxBias"                   },
  {TxEchoCoef,          "TxEchoCoef"               },
  {TxEchoUiSel,         "TxEchoUiSel"              },
  {RxDfeCode,           "RxDfeCode"                },
  {RxDfeDelaySel,       "RxDfeDelaySel"            },
  {Cmd0All,             "Backside QCA"             },
  {Cmd1All,             "Backside QCA"             },
  {TxDqPerChDelayOffset,"Tx Dq Per CH"             },
  {RxDqsPerChDelayOffset,"Rx Dqs Per CH"           },
  {RxVrefPerChDelayOffset,"Rx Vref Per CH"         },
  {RxSamplerPerChDelayOffset,"Rx Sampler Per CH"   },
  {Ctl0Grp,             "Backside QCS"             },
  {Ctl1Grp,             "Backside QCS"             },
  {DbDfeTap1,           "Data Buffer Tap1"         },
  {DbDfeTap2,           "Data Buffer Tap2"         },
  {DbDfeTap3,           "Data Buffer Tap3"         },
  {DbDfeTap4,           "Data Buffer Tap4"         },
  {RxSampler,           "RxSampler"                },
  {RxSamplerEvenOdd,    "RxSamplerEvenOdd"         },
  {RxSamplerEven,       "RxSamplerEven"            },
  {RxSamplerOdd,        "RxSamplerOdd"             },
  {TxDqPieOffset,       "TxDqPieOffset"            },
  {TxDqsPieOffset,      "TxDqsPieOffset"           },
  {DcaDfeVref,          "DcaDfeVref"               },
  {DcsDelay,            "DcsDelay"                 },
  {DramDcaClk,          "DramDcaClk"               },
  {DramDcaQClk,         "DramDcaQClk"              },
  {DramDcaIbClk,        "DramDcaIbClk"             },
  {DramDcaQbClk,        "DramDcaQbClk"             },
  {DramCaVref,          "DramCaVref"               },
  {DramCsVref,          "DramCsVref"               },
  {QcaDelay,            "QcaDelay"                 },
  {QcsDelay,            "QcsDelay"                 },
  {BcomDelay,           "BcomDelay"                },
  {BcomVref,            "BcomVref"                 },
  {DbDfeVrefGroup,      "DataBufferDfeVref"        },
  {DbDfeVrefDq0,        "DataBufferDfeVrefDq0"     },
  {DbDfeVrefDq1,        "DataBufferDfeVrefDq1"     },
  {DbDfeVrefDq2,        "DataBufferDfeVrefDq2"     },
  {DbDfeVrefDq3,        "DataBufferDfeVrefDq3"     },
  {DbDfeVrefDq4,        "DataBufferDfeVrefDq4"     },
  {DbDfeVrefDq5,        "DataBufferDfeVrefDq5"     },
  {DbDfeVrefDq6,        "DataBufferDfeVrefDq6"     },
  {DbDfeVrefDq7,        "DataBufferDfeVrefDq7"     }
};

//
// *** The order of this array is important ***
// The correct order is highest to lowest speed
//
DDR_SPEED_STRINGS  mDdrSpeedStrings[] = {
#if FixedPcdGetBool(PcdMcrSupport)
  {DDR_8800, DDR_8800_TCK_MIN, "8800"},
#endif // FixedPcdGetBool(PcdMcrSupport)
  {DDR_8400, DDR_8400_TCK_MIN, "8400"},
  {DDR_8000, DDR_8000_TCK_MIN, "8000"},
  {DDR_7600, DDR_7600_TCK_MIN, "7600"},
  {DDR_7200, DDR_7200_TCK_MIN, "7200"},
  {DDR_6800, DDR_6800_TCK_MIN, "6800"},
  {DDR_6400, DDR_6400_TCK_MIN, "6400"},
  {DDR_6000, DDR_6000_TCK_MIN, "6000"},
  {DDR_5600, DDR_5600_TCK_MIN, "5600"},
  {DDR_5200, DDR_5200_TCK_MIN, "5200"},
  {DDR_4800, DDR_4800_TCK_MIN, "4800"},
  {DDR_4400, DDR_4400_TCK_MIN, "4400"},
  {DDR_4266, DDR_4266_TCK_MIN, "4266"},
  {DDR_4200, DDR_4200_TCK_MIN, "4200"},
  {DDR_4000, DDR_4000_TCK_MIN, "4000"},
  {DDR_3800, DDR_3800_TCK_MIN, "3800"},
  {DDR_3733, DDR_3733_TCK_MIN, "3733"},
  {DDR_3600, DDR_3600_TCK_MIN, "3600"},
  {DDR_3466, DDR_3466_TCK_MIN, "3466"},
  {DDR_3400, DDR_3400_TCK_MIN, "3400"},
  {DDR_3200, DDR_3200_TCK_MIN, "3200"},
  {DDR_3000, DDR_3000_TCK_MIN, "3000"},
  {DDR_2933, DDR_2933_TCK_MIN, "2933"},
  {DDR_2800, DDR_2800_TCK_MIN, "2800"},
  {DDR_2666, DDR_2666_TCK_MIN, "2666"},
  {DDR_2600, DDR_2600_TCK_MIN, "2600"},
  {DDR_2400, DDR_2400_TCK_MIN, "2400"},
  {DDR_2200, DDR_2200_TCK_MIN, "2200"},
  {DDR_2133, DDR_2133_TCK_MIN, "2133"},
  {DDR_2000, DDR_2000_TCK_MIN, "2000"},
  {DDR_1866, DDR_1866_TCK_MIN, "1866"},
  {DDR_1800, DDR_1800_TCK_MIN, "1800"},
  {DDR_1600, DDR_1600_TCK_MIN, "1600"},
  {DDR_1400, DDR_1400_TCK_MIN, "1400"},
  {DDR_1333, DDR_1333_TCK_MIN, "1333"},
  {DDR_1200, DDR_1200_TCK_MIN, "1200"},
  {DDR_1066, DDR_1066_TCK_MIN, "1066"},
  {DDR_1000, DDR_1000_TCK_MIN, "1000"},
  {DDR_800,  DDR_800_TCK_MIN,  " 800"}
};

//
// Memory signal name strings
//
SIGNAL_STRING  mSignalString[] = {
  {PAR,   "PAR"},
  {ACT_N, "ACT_N"},
  {RAS_N, "RAS_N"},
  {CAS_N, "CAS_N"},
  {WE_N,  "WE_N"},
  {BA0,   "BA0"},
  {BA1,   "BA1"},
  {BA2,   "BA2"},
  {BG0,   "BG0"},
  {BG1,   "BG1"},
  {A0,    "A0"},
  {A1,    "A1"},
  {A2,    "A2"},
  {A3,    "A3"},
  {A4,    "A4"},
  {A5,    "A5"},
  {A6,    "A6"},
  {A7,    "A7"},
  {A8,    "A8"},
  {A9,    "A9"},
  {A10,   "A10"},
  {A11,   "A11"},
  {A12,   "A12"},
  {A13,   "A13"},
  {A14,   "A14"},
  {A15,   "A15"},
  {A16,   "A16"},
  {A17,   "A17"},
  {CS0_N, "CS0_N"},
  {CS1_N, "CS1_N"},
  {CS2_N, "CS2_N/C0"},
  {CS3_N, "CS3_N/C1"},
  {CS4_N, "CS4_N"},
  {CS5_N, "CS5_N"},
  {CS6_N, "CS6_N/C0"},
  {CS7_N, "CS7_N/C1"},
  {CS8_N, "CS8_N"},
  {CS9_N, "CS9_N"},
  {CKE0,  "CKE0"},
  {CKE1,  "CKE1"},
  {CKE2,  "CKE2"},
  {CKE3,  "CKE3"},
  {CKE4,  "CKE4"},
  {CKE5,  "CKE5"},
  {ODT0,  "ODT0"},
  {ODT1,  "ODT1"},
  {ODT2,  "ODT2"},
  {ODT3,  "ODT3"},
  {ODT4,  "ODT4"},
  {ODT5,  "ODT5"},
  {CK0,   "CK0"},
  {CK1,   "CK1"},
  {CK2,   "CK2"},
  {CK3,   "CK3"},
  {CK4,   "CK4"},
  {CK5,   "CK5"},
  {C0,    "C0"},
  {C1,    "C1"},
  {C2,    "C2"}
};

#endif // DEBUG_CODE_BLOCK

CHAR8 *mUnknownGroup = "Unknown Group";

CHAR8 *mUnknownSignal = "Unknown signal";


/**
  Return pointer to string for a training group

  @param[in]  Group     - The enumerated group value
  @param[out] String    - Pointer to caller's string pointer to be updated

  @retval   EFI_SUCCESS if String is updated
            EFI_NOT_FOUND otherwise
**/
EFI_STATUS
EFIAPI
GetGroupString (
  IN    MRC_GT    Group,
  OUT   CHAR8     **String
  )
{
#if DEBUG_CODE_BLOCK
  UINT8     Index;

  for (Index = 0; Index < ARRAY_SIZE (mGroupString); Index++) {
    if (Group == mGroupString[Index].Group) {
      *String = mGroupString[Index].String;
      return EFI_SUCCESS;
    }
  }
#endif // DEBUG_CODE_BLOCK
  *String = mUnknownGroup;
  return EFI_NOT_FOUND;
}

/**
  Return pointer to training group string
  This version does not take a string pointer from the caller.
  It can be used inside debug print statements without using a local variable.

  @param[in]  Group   - Training group number

  @return  Pointer to training group string

**/
CHAR8 *
EFIAPI
GetGroupStr (
  IN    MRC_GT  Group
  )
{
#ifdef DEBUG_CODE_BLOCK
  CHAR8     *GroupString;

  if (EFI_ERROR (GetGroupString (Group, &GroupString))) {
    GroupString = "Unknown group";
  }

  return GroupString;
#else // DEBUG_CODE_BLOCK
  return NULL;
#endif // DEBUG_CODE_BLOCK
} // GetGroupStr

#ifdef DEBUG_CODE_BLOCK
/**

  Displays information about the DIMMs present

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
DIMMInfo (
  PSYSHOST Host
  )
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 dimm;
  UINT16                                userVdd = 0;
  UINT8                                 maxSPC;
  UINT8                                 channelActive;
  UINT8                                 dimmPresent;
  UINT8                                 reqType = 0;
  UINT8                                 reqData = 0;
  UINT8                                 QClkRatio = 0;
  UINT8                                 MaxCh;
  UINT8                                 ChInc;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
  UINT8                                 MaxChDdr;
  QCLK_RATIO_TO_SPEED                   *QclkToSpeed;
  CHAR8                                 *SpeedString;
  UINT8                                 Index;
  UINT8                                 ListLength;
  QCLK_RATIO_TO_SPEED                   Bclk100[] = {{10, "1000"},
                                                     {12, "1200"},
                                                     {14, "1400"},
                                                     {16, "1600"},
                                                     {18, "1800"},
                                                     {20, "2000"},
                                                     {22, "2200"},
                                                     {24, "2400"},
                                                     {26, "2600"},
                                                     {28, "2800"},
                                                     {30, "3000"},
                                                     {32, "3200"},
                                                     {34, "3400"},
                                                     {36, "3600"},
                                                     {38, "3800"},
                                                     {40, "4000"},
                                                     {42, "4200"},
                                                     {44, "4400"},
                                                     {48, "4800"}};
  QCLK_RATIO_TO_SPEED                   Bclk133[] = {{6, "800"},
                                                     {8, "1066"},
                                                     {10, "1333"},
                                                     {12, "1600"},
                                                     {14, "1866"},
                                                     {16, "2133"},
                                                     {18, "2400"},
                                                     {20, "2666"},
                                                     {22, "2933"},
                                                     {24, "3200"},
                                                     {26, "3466"},
                                                     {27, "3600"},
                                                     {28, "3733"},
                                                     {30, "4000"},
                                                     {32, "4266"}};


  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  if (IsBrsPresent (Host, socket)) {
    Host->ChannelModeOperation = ModeDdr5FullChannel;
  }
  //
  // Return if this socket is disabled
  //
  ChInc = GetChInc (Host);
  MaxCh = MAX_CH / ChInc;

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  AcquirePrintControl ();
  if (Host->var.mem.memSize == 0) {
    RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DIMM_INFO, "START_SOCKET_%d_DIMMINFO_TABLE\n", socket);

    DimmInfoDisplayProc (Host, socket);

    RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DIMM_INFO, "S|");
    maxSPC = 0;
    for (ch = 0; ch < MaxChDdr; ch += ChInc) {
      RcDebugPrint (SDBG_DIMM_INFO, "     Channel %d", ch / ChInc);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 6);

      if (maxSPC < Host->var.mem.socket[socket].channelList[ch].numDimmSlots) {
        maxSPC = Host->var.mem.socket[socket].channelList[ch].numDimmSlots;
      }
    }
    RcDebugPrint (SDBG_DIMM_INFO, "\n");
    RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    for (dimm = 0; dimm < maxSPC; dimm++) {
      RcDebugPrint (SDBG_DIMM_INFO, "%d|", dimm);

      //
      // Indicate this DIMM isn't present on any channel. Will be updated in channel loop.
      //
      dimmPresent = 0;

      channelNvList = GetChannelNvList (Host, socket);
      for (ch = 0; ch < MaxChDdr; ch += ChInc) {

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent) {
          RcDebugPrint (SDBG_DIMM_INFO, "   DIMM: ");
          DisplayManf (Host, (*dimmNvList)[dimm].SPDMMfgId);
          dimmPresent = 1;
        } else {
          RcDebugPrint (SDBG_DIMM_INFO, "   Not installed");
          RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 4);
        }
      } // ch loop

      //
      // Skip all this other stuff if this DIMM is not present on this channel
      //
      if (dimmPresent == 0) {
        RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
        continue;
      }

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      for (ch = 0; ch < MaxChDdr; ch += ChInc) {

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent) {
          RcDebugPrint (SDBG_DIMM_INFO, "   DRAM: ");
          DisplayManf (Host, (*dimmNvList)[dimm].SPDDramMfgId);
        } else {
          RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
        }
      } // ch loop

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        for (ch = 0; ch < MaxChDdr; ch += ChInc) {

          dimmNvList = GetDimmNvList (Host, socket, ch);

          if ((*dimmNvList)[dimm].dimmPresent) {
            RcDebugPrint (SDBG_DIMM_INFO, "    RCD: ");
            DisplayManf (Host, (*dimmNvList)[dimm].SPDRegVen);
          } else {
            RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
          }
        } // ch loop

        // New line
        RcDebugPrint (SDBG_DIMM_INFO, "\n");
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
      } // if RDIMM
      else {
        for (ch = 0; ch < MaxChDdr; ch += ChInc) {
          RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
        } // ch loop
        // New line
        RcDebugPrint (SDBG_DIMM_INFO, "\n |");
      }

      DisplaySize (Host, socket, dimm);

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      DisplayDimmType (Host, socket, dimm);

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      DisplaySpeed (Host, socket, dimm);

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      // DRAM Manufacture Date
      DisplayDramMfgDate (Host, socket, dimm);

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

#ifdef DDR5_SUPPORT
      // Module Part Num
      DisplayModulePartNumDdr5 (Host, socket, dimm);
#else
      // Module Part Num
      DisplayModulePartNum (Host, socket, dimm);
#endif //DDR5_SUPPORT
      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      // Display Dimm Module Serial Number
      DisplayDimmSN (Host, socket, dimm);

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");
      DisplayIntelSN (Host, socket, dimm);

      if (PcdGetBool (PcdNvDimmEn)) {
        // New line
        RcDebugPrint (SDBG_DIMM_INFO, "\n |");

        // Display Type 1 (NVDIMM-N) information.
        DisplayNvdimmN (Host, socket, dimm);
      }

      // New line
      RcDebugPrint (SDBG_DIMM_INFO, "\n |");

      RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    } // dimm loop
    RcDebugPrint (SDBG_DIMM_INFO, "STOP_SOCKET_%d_DIMMINFO_TABLE\n", socket);
    RcDebugPrintLine (SDBG_DIMM_INFO, 2 + 21 * MaxCh, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    } else {
    RcDebugPrintLine (SDBG_DIMM_INFO, 21 + (13 * (MAX_SOCKET + 1)), DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DIMM_INFO, "START_DIMMINFO_SYSTEM_TABLE\n");
    RcDebugPrintLine (SDBG_DIMM_INFO, 21 + (13 * (MAX_SOCKET + 1)), DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      RcDebugPrint (SDBG_DIMM_INFO, "  Socket %d", socket);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 2);
    } // socket loop
    RcDebugPrint (SDBG_DIMM_INFO, "   System");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");
    RcDebugPrintLine (SDBG_DIMM_INFO, 21 + (13 * (MAX_SOCKET + 1)), DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    // Display Active Memory Size
    RcDebugPrint (SDBG_DIMM_INFO, "Active Memory");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 7);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      //
      // Continue if this socket is not present
      //
      if (SocketPresent (socket)) {
        if ((Host->var.mem.socket[socket].memSize >> 4) < 10000) {
          RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.socket[socket].memSize >> 4);
        }
        if ((Host->var.mem.socket[socket].memSize >> 4) < 1000) {
          RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.socket[socket].memSize >> 4);
        }
        if ((Host->var.mem.socket[socket].memSize >> 4) < 100) {
          RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.socket[socket].memSize >> 4);
        }
        if ((Host->var.mem.socket[socket].memSize >> 4) < 10) {
          RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.socket[socket].memSize >> 4);
        }
        RcDebugPrint (SDBG_DIMM_INFO, "  %dGB", Host->var.mem.socket[socket].memSize >> 4);
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
      } else {
        RcDebugPrint (SDBG_DIMM_INFO, "     N/A");
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 4);
      }
    } // socket

    if ((Host->var.mem.memSize >> 4) < 10000) {
      RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.memSize >> 4);
    }
    if ((Host->var.mem.memSize >> 4) < 1000) {
      RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.memSize >> 4);
    }
    if ((Host->var.mem.memSize >> 4) < 100) {
      RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.memSize >> 4);
    }
    if ((Host->var.mem.memSize >> 4) < 10) {
      RcDebugPrint (SDBG_DIMM_INFO, " ", Host->var.mem.memSize >> 4);
    }
    RcDebugPrint (SDBG_DIMM_INFO, "  %dGB", Host->var.mem.memSize >> 4);
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");
    // Display Active Channels

    // DDR Freq
    RcDebugPrint (SDBG_DIMM_INFO, "DDR Freq");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
    }
    //
    // Get current DCLK ratio
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {

      //
      // Continue if this socket is not present
      //

      if (IsSocketDimmPopulated (Host, socket) != TRUE) {
        continue;
      }

      GetQclkRatio (Host, socket, &reqType, &reqData);
      break;
    } // socket loop

    if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      RcDebugPrint (SDBG_DIMM_INFO, "  DDR4-");
    }
#ifdef DDR5_SUPPORT
    else if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
      RcDebugPrint (SDBG_DIMM_INFO, "  DDR5-");
    }
#endif // DDR5_SUPPORT

    QClkRatio = reqData;

    if (reqType == 0) {
      QclkToSpeed = Bclk133;
      ListLength = ARRAY_SIZE (Bclk133);
    } else {
      QclkToSpeed = Bclk100;
      ListLength = ARRAY_SIZE (Bclk100);
    }
    SpeedString = "UNKNOWN";
    for (Index = 0; Index < ListLength; Index++) {
      if (QclkToSpeed[Index].QclkRatio == QClkRatio) {
        SpeedString = QclkToSpeed[Index].SpeedString;
        break;
      }
    }
    RcDebugPrint (SDBG_DIMM_INFO, "%4a", SpeedString);
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");

    for (ch = 0; ch < MaxCh; ch += ChInc) {
      //
      // Check if this channel is active on any socket
      //
      channelActive = 0;
      for (socket = 0; socket < MAX_SOCKET; socket++) {

        channelNvList = GetChannelNvList (Host, socket);
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        channelActive = 1;
      } // socket loop

      if (channelActive == 0) {
        continue;
      }

      // Ch Timing
      RcDebugPrint (SDBG_DIMM_INFO, "Ch%d CL-RCD-RP-CMD", ch);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        //
        // Continue if this socket is not present
        //
        channelNvList = GetChannelNvList (Host, socket);

        if ((*channelNvList)[ch].enabled) {

          // CAS latency (tCL)
          if ((*channelNvList)[ch].common.nCL < 10) {
            RcDebugPrint (SDBG_DIMM_INFO, " ");
          }
          RcDebugPrint (SDBG_DIMM_INFO, "%d-", (*channelNvList)[ch].common.nCL);

          // Row Address to Column Address Delay (tRCD)
          RcDebugPrint (SDBG_DIMM_INFO, "%d-", (*channelNvList)[ch].common.nRCD);

          // Row Active Time (tRP)
          RcDebugPrint (SDBG_DIMM_INFO, "%d-", (*channelNvList)[ch].common.nRP);

          // Command timing
          if (!(*channelNvList)[ch].timingMode) {
            RcDebugPrint (SDBG_DIMM_INFO, "%dn", (*channelNvList)[ch].timingMode + 1);
          } else {
            RcDebugPrint (SDBG_DIMM_INFO, "%dn", (*channelNvList)[ch].timingMode);
          }

          if ((*channelNvList)[ch].common.nRCD < 10) {
            RcDebugPrint (SDBG_DIMM_INFO, " ");
          }
          if ((*channelNvList)[ch].common.nRP < 10) {
            RcDebugPrint (SDBG_DIMM_INFO, " ");
          }
          RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
        } else {
          RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
        }
      } // socket loop

      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
      RcDebugPrint (SDBG_DIMM_INFO, "\n");
    } // ch loop

    // DDR Vdd
    RcDebugPrint (SDBG_DIMM_INFO, "DDR Vdd");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 13);

    if (PcdGetBool (PcdMrcXmpSupport)) {

      SYS_SETUP *Setup;

      Setup = GetSysSetupPointer ();

      //
      // Get current Vdd setting
      //
      userVdd = Setup->mem.inputMemTime.vdd;
      if (userVdd != 0) {
        if ((userVdd < 900) || (userVdd > 1850)) {
          userVdd = 0;
        }
      }

    }

    for (socket = 0; socket < MAX_SOCKET; socket++) {

      //
      // Continue if this socket is not present
      //

      if (IsSocketDimmPopulated (Host, socket) != TRUE) {
        RcDebugPrint (SDBG_DIMM_INFO, "     N/A");
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 4);
        continue;
      }

      if (userVdd) {
        RcDebugPrint (SDBG_DIMM_INFO, "   %d.%dV", userVdd / 1000, userVdd % 1000);
      } else {
        switch (GetLvmode (Host, socket)) {
        case 0:
          RcDebugPrint (SDBG_DIMM_INFO, "   1.500V");
          break;
        case 1:
          RcDebugPrint (SDBG_DIMM_INFO, "   1.350V");
          break;
        case 3:
          RcDebugPrint (SDBG_DIMM_INFO, "   1.200V");
          break;
        default:
          //Automated add of default case. Please review.
          break;
        }
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 2);
        break;
      }
    } // socket loop
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");


    DimmInfoDisplayProcFeatures (Host);

    // CCMRC Revision
    RcDebugPrint (SDBG_DIMM_INFO, "CCMRC Revision");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 6);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
    }
    RcDebugPrint (SDBG_DIMM_INFO, "  %02x.%02x.%02x", (CCMRC_REVISION >> 24) & 0xff, (CCMRC_REVISION >> 16) & 0xff, (CCMRC_REVISION >> 8) & 0xff);
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 2);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");

    //
    // RC Version
    //
    RcDebugPrint (SDBG_DIMM_INFO, "RC Version");
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 10);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 12);
    }
    RcDebugPrint (
      SDBG_DIMM_INFO,
      " %d.%d.%d.%04x",
      Host->var.common.RcVersion.Major,
      Host->var.common.RcVersion.Minor,
      Host->var.common.RcVersion.Revision,
      Host->var.common.RcVersion.BuildNumber
    );
    RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
    RcDebugPrint (SDBG_DIMM_INFO, "\n");

    RcDebugPrintLine (SDBG_DIMM_INFO, 21 + (13 * (MAX_SOCKET + 1)), DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DIMM_INFO, "STOP_DIMMINFO_SYSTEM_TABLE\n");
    RcDebugPrintLine (SDBG_DIMM_INFO, 21 + (13 * (MAX_SOCKET + 1)), DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    PrintDimmConfig (Host);
  }

  ReleasePrintControl ();
  if (IsBrsPresent (Host, socket)) {
    Host->ChannelModeOperation = ModeDdr5SubChannel;
  }

  return SUCCESS;
} // DIMMInfo

/**
  Displays information about the DIMMs present

  @param Host:         Pointer to sysHost
  @param Socket:       Processor socket
  @param Dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplaySize (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm
  )
{
  UINT8 Ch;
  UINT8 Index;
  UINT16 MemSize = 0;
  UINT8 ChInc;
  struct dimmDevice (*ChdimmList)[MAX_DIMM];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  struct ddrRank (*RankList)[MAX_RANK_DIMM];
  UINT8             MaxChDdr;
  CHAR8             *RanksString;

  MaxChDdr = GetMaxChDdr ();

  ChInc = GetChInc (Host);

  // Reinit MemSize for each channel
  for (Ch = 0; Ch < MaxChDdr; Ch += ChInc, MemSize = 0) {

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    ChdimmList = &Host->var.mem.socket[Socket].channelList[Ch].dimmList;

#ifdef DDRT_SUPPORT
    if ((*DimmNvList)[Dimm].DcpmmPresent) {

      // Display size
      RcDebugPrint (SDBG_DIMM_INFO, "        %4dGB      |", GetDcPmmDimmSize (Socket, Ch, Dimm));

    }
#endif // DDRT_SUPPORT
    if ((*DimmNvList)[Dimm].dimmPresent && ((*DimmNvList)[Dimm].DcpmmPresent == 0)) {
      // Display size
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      if (!(*ChdimmList)[Dimm].memSize)  { //Check before calculation
        for (Index = 0; Index < (*DimmNvList)[Dimm].numDramRanks; Index++) {
          if ((*RankList)[Index].enabled == 0) {
            continue; // Continue to the next rank
          }
          MemSize = MemSize + (*RankList)[Index].rankSize;
        }
      }
      RcDebugPrint (SDBG_DIMM_INFO, "%3dGB(", MemSize >> 4);

      // Display DRAM tech
      RcDebugPrint (SDBG_DIMM_INFO, "%3dGb", EncodedSpdToDramDensity ((*DimmNvList)[Dimm].sdramCapacity));

      // Display DRAM width
      RcDebugPrint (SDBG_DIMM_INFO, "x%d", (4 << (*DimmNvList)[Dimm].dramIOWidth));

      // Display DRAM stack
      if (IsLrdimmPresent (Socket, Ch, Dimm) || ((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
        RcDebugPrint (SDBG_DIMM_INFO, " %dH", (*DimmNvList)[Dimm].dieCount);
      } else {
        RcDebugPrint (SDBG_DIMM_INFO, "  ");
      }

      //
      // Display number or ranks
      //
      switch ((*DimmNvList)[Dimm].numRanks) {
        case 1:
          RanksString =  "SR";
          break;
        case 2:
          RanksString =  "DR";
          break;
        case 3:
          RanksString =   "TR";
          break;
        case 4:
          RanksString =  "QR";
          break;
        case 8:
          RanksString =  "OR";
          break;
        default:
          RanksString = "";
          break;
      }
      RcDebugPrint (SDBG_DIMM_INFO, " %a)", RanksString);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
    } else if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // Ch loop
} // DisplaySize


/**

  Displays information about the DIMMs present

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayDimmType (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
{
  UINT8             ch;
  UINT8             ChInc;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8              MaxChDdr;
  UINT8             Index;
  CHAR8             *DimmTypeString;
  CHAR8             *DimmTechnologyString;
  CHAR8             *RawCardString;
  RAW_CARD_STRINGS  RawCards[] = {{RAW_CARD_A, "A "},
                                  {RAW_CARD_B, "B "},
                                  {RAW_CARD_C, "C "},
                                  {RAW_CARD_D, "D "},
                                  {RAW_CARD_E, "E "},
                                  {RAW_CARD_F, "F "},
                                  {RAW_CARD_G, "G "},
                                  {RAW_CARD_H, "H "},
                                  {RAW_CARD_J, "J "},
                                  {RAW_CARD_K, "K "},
                                  {RAW_CARD_L, "L "},
                                  {RAW_CARD_M, "M "},
                                  {RAW_CARD_N, "N "},
                                  {RAW_CARD_P, "P "},
                                  {RAW_CARD_R, "R "},
                                  {RAW_CARD_T, "T "},
                                  {RAW_CARD_U, "U "},
                                  {RAW_CARD_W, "W "},
                                  {RAW_CARD_Y, "Y "},
                                  {RAW_CARD_AA, "AA"},
                                  {RAW_CARD_AB, "AB"},
                                  {RAW_CARD_AC, "AC"},
                                  {RAW_CARD_AD, "AD"},
                                  {RAW_CARD_AE, "AE"},
                                  {RAW_CARD_AF, "AF"},
                                  {RAW_CARD_AG, "AG"},
                                  {RAW_CARD_AH, "AH"},
                                  {RAW_CARD_AJ, "AJ"},
                                  {RAW_CARD_AK, "AK"},
                                  {RAW_CARD_AL, "AL"}};

  MaxChDdr = GetMaxChDdr ();
  ChInc = GetChInc (Host);

  for (ch = 0; ch < MaxChDdr; ch += ChInc) {

    dimmNvList = GetDimmNvList (Host, socket, ch);

    if ((*dimmNvList)[dimm].dimmPresent) {
      // Display DIMM type
      switch ((*dimmNvList)[dimm].keyByte) {
      case SPD_TYPE_DDR4:
        DimmTechnologyString = "DDR4";
        break;
#ifdef DDR5_SUPPORT
      case SPD_TYPE_DDR5:
        DimmTechnologyString = "DDR5";
        break;
#endif // DDR5_SUPPORT
      case SPD_TYPE_AEP:
        DimmTechnologyString = "PMem";
        break;
      default:
        DimmTechnologyString = "";
        break;
      }
      RcDebugPrint (SDBG_DIMM_INFO, " %a ", DimmTechnologyString);

      // Display DIMM type
      switch ((*dimmNvList)[dimm].actKeyByte2) {
#if FixedPcdGetBool(PcdMcrSupport)
      case SPD_MCRDIMM_DDR5:
        DimmTypeString = "MCR-DIMM";
        break;
#endif // FixedPcdGetBool(PcdMcrSupport)
      case SPD_UDIMM:
        DimmTypeString = "UDIMM ";
        break;
      case SPD_RDIMM:
        DimmTypeString = "RDIMM ";
        break;
      case SPD_LRDIMM_DDR4:
        DimmTypeString = "LRDIMM";
        break;
      case SPD_NVM_DIMM:
        DimmTypeString = "NVDIMM";
        break;
      case SPD_SODIMM:
        DimmTypeString = "SODIMM";
        break;
      default:
        DimmTypeString = "Unknown";
        break;
      }
      RcDebugPrint (SDBG_DIMM_INFO, "%a ", DimmTypeString);

      //
      // Display Raw Card
      //
      RawCardString = "? ";
      for (Index = 0; Index < ARRAY_SIZE (RawCards); Index++) {
        if (((*dimmNvList)[dimm].SPDRawCard & 0x1F) == RawCards[Index].RawCardNumber) {
          RawCardString = RawCards[Index].String;
          break;
        }
      }
      RcDebugPrint (SDBG_DIMM_INFO, "R/C-%a", RawCardString);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 1);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // ch loop
} // DisplayDimmType


/**

  Displays information about the DIMMs present

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayDramMfgDate (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
{
  UINT8 ch;
  UINT8 ChInc;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8            MaxChDdr;

  ChInc = GetChInc (Host);
  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch += ChInc) {

    dimmNvList = GetDimmNvList (Host, socket, ch);

    if ((*dimmNvList)[dimm].dimmPresent) {

      //
      // Module date code
      //
      RcDebugPrint (SDBG_DIMM_INFO, "     ww%02d ", (UINT8) BCDtoDecimal ((UINT8) ((*dimmNvList)[dimm].SPDModDate >> 8)));
      RcDebugPrint (SDBG_DIMM_INFO, "20%02d", BCDtoDecimal ((UINT8) (*dimmNvList)[dimm].SPDModDate));
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 6);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // ch loop
} // DisplayDramMfgDate

/**

  Displays information about the DIMMs present

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplaySpeed (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
{
  UINT8             ch;
  UINT8             ChInc;
  UINT8             TckIndex;
  CHAR8             *SpeedString;
  //  UINT8 bclk = 133;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChInc = GetChInc (Host);

  for (ch = 0; ch < MaxChDdr; ch += ChInc) {

    dimmNvList = GetDimmNvList (Host, socket, ch);

    if ((*dimmNvList)[dimm].dimmPresent) {
      RcDebugPrint (SDBG_DIMM_INFO, "   ");
      SpeedString = "????";
      for (TckIndex = 0; TckIndex < ARRAY_SIZE (mDdrSpeedStrings); TckIndex++) {
        if ((*dimmNvList)[dimm].minTCK <= mDdrSpeedStrings[TckIndex].TckMin) {
          SpeedString = mDdrSpeedStrings[TckIndex].SpeedString;
          break;
        }
      }

      RcDebugPrint (SDBG_DIMM_INFO, "%4a ", SpeedString);

      if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
        // CAS latency (tCL)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d-", (UINT8) PicosecondToClocks (socket, (*dimmNvList)[dimm].tCL));
        // Row Address to Column Address Delay (tRCD)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d-", (UINT8) PicosecondToClocks (socket, (*dimmNvList)[dimm].tRCD));
        // Row Active Time (tRP)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d", (UINT8) PicosecondToClocks (socket, (*dimmNvList)[dimm].tRP));
      } else {
        // CAS latency (tCL)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d-", (UINT8) TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (*dimmNvList)[dimm].tCL, (*dimmNvList)[dimm].ftbTAA));
        // Row Address to Column Address Delay (tRCD)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d-", (UINT8) TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (*dimmNvList)[dimm].tRCD, (*dimmNvList)[dimm].ftbTRCD));
        // Row Active Time (tRP)
        RcDebugPrint (SDBG_DIMM_INFO, "%2d", (UINT8) TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (*dimmNvList)[dimm].tRP, (*dimmNvList)[dimm].ftbTRP));
      }

      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 4);

    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // ch loop
} // DisplaySpeed


/**

Displays information about the DIMMs present

@param Host:         Pointer to sysHost
@param socket:       Processor socket
@param dimm:         Specific DIMM on this Channel

@retval N/A

**/
VOID
DisplayNvdimmN (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm
  )
{
  UINT8            Channel;
  UINT8            ChInc;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  UINT8            MaxChDdr;

  ChInc = GetChInc (Host);
  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += ChInc) {

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    if (((*DimmNvList)[Dimm].dimmPresent == 1) && ((*DimmNvList)[Dimm].NvDimmNPresent)) {
      RcDebugPrint (SDBG_DIMM_INFO, "   JEDEC NVDIMM-N");
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  }
}

/**

  Displays information about the DIMMs present

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayModulePartNum (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
{
#ifndef DDR5_SUPPORT
  UINT8 i;
  UINT8 ch;
  UINT8 ChInc;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChInc = GetChInc (Host);

  for (ch = 0; ch < MaxChDdr; ch += ChInc) {

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if ((*dimmNvList)[dimm].dimmPresent) {
      //
      // Module part number
      //
      for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
        if ((*dimmNvList)[dimm].SPDModPartDDR4[i]) {
          RcDebugPrint (SDBG_DIMM_INFO, "%c", (*dimmNvList)[dimm].SPDModPartDDR4[i]);
        } else {
          RcDebugPrint (SDBG_DIMM_INFO, " ");
        }
      }
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 0);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // ch loop
#endif // DDR5_SUPPORT
} // DisplayModulePartNum

#ifdef DDR5_SUPPORT
/**

  Displays information about the DDR5 DIMMs present

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayModulePartNumDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm
  )
{
  BOOLEAN           ExtendNextLine = FALSE;
  UINT8             Index;
  UINT8             Channel;
  UINT8             ChInc;
  UINT8             MaxChDdr;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  MaxChDdr = GetMaxChDdr ();
  ChInc = GetChInc (Host);

  if (SPD_MODULE_PART_DDR5 > DimmInfoPageWidth) {
    ExtendNextLine = TRUE;
  }

  for (Channel = 0; Channel < MaxChDdr; Channel += ChInc) {
    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    if ((*DimmNvList)[Dimm].dimmPresent) {
      //
      // Module part number
      //
      for (Index = 0; Index < DimmInfoPageWidth; Index++) {
        if ((*DimmNvList)[Dimm].SPDModPartDDR5[Index]) {
          RcDebugPrint (SDBG_DIMM_INFO, "%c", (*DimmNvList)[Dimm].SPDModPartDDR5[Index]);
        } else {
          RcDebugPrint (SDBG_DIMM_INFO, " ");
        }
      }
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 0);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // Channel loop

  //
  // Extend one line if the number of part number byte is greater than the width of each dimm info
  //
  if (ExtendNextLine) {
    RcDebugPrint (SDBG_DIMM_INFO, "\n |");
    for (Channel = 0; Channel < MaxChDdr; Channel += ChInc) {
      DimmNvList = GetDimmNvList (Host, Socket, Channel);
      if ((*DimmNvList)[Dimm].dimmPresent) {
        for (Index = DimmInfoPageWidth; Index < SPD_MODULE_PART_DDR5; Index++) {
          if ((*DimmNvList)[Dimm].SPDModPartDDR5[Index]) {
            RcDebugPrint (SDBG_DIMM_INFO, "%c", (*DimmNvList)[Dimm].SPDModPartDDR5[Index]);
          } else {
            RcDebugPrint (SDBG_DIMM_INFO, " ");
          }
        }
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 10);
      } else {
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
      }
    } // Channel loop
  }
} // DisplayModulePartNumDdr5
#endif //DDR5_SUPPORT

/**
  Displays the DRAM manufacturer

  @param Host  - Pointer to sysHost
  @param MfgId - DRAM Manufacturer Id

  @retval N/A
**/
VOID
DisplayManf (
  PSYSHOST Host,
  UINT16   MfgId
  )
{
  UINT16  Index;
  UINT8   StrIndex;

  for (Index = 0; Index < MfgListLength; Index++) {
    if ((MemoryModuleManufacturerList[Index].MfgId == MfgId)){
      StrIndex = 0;
      while (MemoryModuleManufacturerList[Index].String[StrIndex] != 0) {
        StrIndex++;
      }
      RcDebugPrint (SDBG_DIMM_INFO, "%s", MemoryModuleManufacturerList[Index].String);
      while (StrIndex < 8) {
        RcDebugPrint (SDBG_DIMM_INFO, " ");
        StrIndex++;
      }
      break;
    }
  }

  if (Index == MfgListLength) {
    RcDebugPrint (SDBG_DIMM_INFO, "%04x    ", MfgId);
  }

  RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 3);
} // DisplayManf

/**

  Displays the system DDR frequency

  @param Host:         Pointer to sysHost
  @param Socket:       Processor socket

  @retval N/A

**/
VOID
DisplayDDRFreq (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                       Index;
  CHAR8                       *SpeedString;

  if (GetDebugLevel () & SDBG_MINMAX) {

    SpeedString = "";
    for (Index = 0; Index < ARRAY_SIZE (mDdrSpeedStrings); Index++) {
      if (mDdrSpeedStrings[Index].DdrFreq == GetHostFrequency (Socket)) {
        SpeedString = mDdrSpeedStrings[Index].SpeedString;
        break;
      }
    }
    RcDebugPrint (SDBG_DEFAULT, "DDR-%a\n", SpeedString);
  }
} // DisplayDDRFreq

/**

  Displays DIMM serial number from SPD 320-328

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayDimmSN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm
)
{
  UINT8             i;
  UINT8             Ch;
  UINT8             ChInc;
  UINT32            SerialNum;
  UINT16            MmfgId;
  UINT16            MmfgDate;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8             MaxChDdr;

  ChInc = GetChInc (Host);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += ChInc) {
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if ((*DimmNvList)[Dimm].dimmPresent) {
      MmfgId = (((*DimmNvList)[Dimm].SPDMMfgId & 0xFF) << 8) | (((*DimmNvList)[Dimm].SPDMMfgId & 0xFF00) >> 8);
      MmfgDate = (((*DimmNvList)[Dimm].SPDModDate & 0xFF) << 8) | (((*DimmNvList)[Dimm].SPDModDate & 0xFF00) >> 8);
      SerialNum = 0;

      for (i = 0; i < SPD_MODULE_SERIAL; i++) {
        SerialNum |= (UINT32) (*DimmNvList)[Dimm].SPDModSN[i] << ((3 - i) * 8);
      }
      RcDebugPrint (SDBG_DIMM_INFO, "0x%04x%02x%04x%08x", MmfgId, (*DimmNvList)[Dimm].SPDMMfLoc, MmfgDate, SerialNum);
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 0);
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  } // ch loop
} // DisplayDimmSN

/**

  Displays Intel SN from SPD 507-511

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Dimm:         Specific DIMM on this Channel

  @retval N/A

**/
VOID
DisplayIntelSN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm
)
{
  UINT8             i;
  UINT8             Ch;
  UINT8             ChInc;
  UINT64_STRUCT     SerialNum;
  UINT32            Remainder;

  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8             MaxChDdr;

  ChInc = GetChInc (Host);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += ChInc) {
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if ((*DimmNvList)[Dimm].dimmPresent) {
      SerialNum.lo = 0;
      SerialNum.hi = 0;

      SerialNum.hi= (*DimmNvList)[Dimm].SPDIntelSN[0];
      for (i = 1; i < 5; i++) {
        SerialNum.lo |= (*DimmNvList)[Dimm].SPDIntelSN[i] << ((4 - i) * 8);
      }

      if ((SerialNum.lo == 0) && (SerialNum.hi == 0)) {
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
      } else {
        SerialNum = DivUINT64x32 ( SerialNum, 1000, &Remainder);

        RcDebugPrint (SDBG_DIMM_INFO, "%d-%.3d", SerialNum.lo, Remainder);
        RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 8);
      }
    } else {
      RcDebugPrintEmptyBlock (SDBG_DIMM_INFO, 20);
    }
  }
}

#endif //DEBUG_CODE_BLOCK


UINT8
BCDtoDecimal (
  UINT8 BcdValue
  )
/**

  Function converts a BCD to a decimal value.

  @param BcdValue         - An 8 bit BCD value

  @retval UINT8            - Returns the decimal value of the BcdValue

**/
{
  UINT8 High;
  UINT8 Low;

  High = BcdValue >> 4;
  Low = BcdValue - (High << 4);

  return (UINT8) (Low + (High * 10));
}


#ifdef DEBUG_CODE_BLOCK
VOID
EyeDiagram (
  PSYSHOST        Host,
  UINT8           socket,
  MRC_LT          level,
  MRC_GT          group,
  UINT16          mode,
  UINT8           scope,
  UINT32          patternLength,
  UINT8           chMask,
  UINT8           rankMask,
  UINT8           dataMask[MAX_STROBE/2]
  )
/**
  Calculates positive and negative margin for the group provided

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check
  @param level:        DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group:        Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param mode:         Test mode to use
                         MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                         MODE_VIC_AGG          BIT1: Enable victim/aggressor
                         MODE_START_ZERO       BIT2: Start at margin 0
                         MODE_POS_ONLY         BIT3: Margin only the positive side
                         MODE_NEG_ONLY         BIT4: Margin only the negative side
                         MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                         MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                         MODE_DATA_LFSR        BIT7: Enable Data LFSR
                         MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                         MODE_ADDR_CMD0_SETUP  BIT9
                         MODE_CHECK_PARITY     BIT10: Enable parity checking
                         MODE_DESELECT         BIT11: Enable deselect patterns
                         MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns
  @param scope:        Margin granularity
                         SCOPE_SOCKET      0: Margin per processor socket
                         SCOPE_CH        1: Margin per channel
                         SCOPE_DIMM      2: Margin per DIMM
                         SCOPE_RANK      3: Margin per rank
                         SCOPE_STROBE    4: Margin per strobe group
                         SCOPE_BIT       5: Margin per bit

**/
{
  UINT8               ch;
  UINT8               rank;
  INT32               vIndex;
  UINT8               maxVrefOffset = 0;
  UINT64              startTsc = 0;
  UINT64              endTsc = 0;
  UINT32              mrcLatency = 0;
  struct bitMask      filter;
  struct rankMargin   (*results)[MAX_VREF_OFFSET_RANGE];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  results = RcAllocatePool (sizeof (*results));
  if (results == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }
  if (level == LrbufLevel) {
    RcDebugPrint (SDBG_DEFAULT, "\nBackside ");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\n");
  }

  if (group == RxDqsDelay) {
    RcDebugPrint (SDBG_DEFAULT, "Read Eye Diagrams\n");
    if (level == LrbufLevel) {
      maxVrefOffset = 45;
    } else {
      maxVrefOffset = MAX_RD_VREF_OFFSET;
    }
  } else if (group == RxDqsPDelay) {
    RcDebugPrint (SDBG_DEFAULT, "Read Eye(P) Diagrams\n");
    maxVrefOffset = MAX_RD_VREF_OFFSET;
  } else if (group == RxDqsNDelay) {
    RcDebugPrint (SDBG_DEFAULT, "Read Eye(N) Diagrams\n");
    maxVrefOffset = MAX_RD_VREF_OFFSET;
  } else if (group == TxDqDelay) {
    RcDebugPrint (SDBG_DEFAULT, "Write Eye Diagrams\n");
    if (level == LrbufLevel) {
      maxVrefOffset = 50;
    } else {
      maxVrefOffset = MAX_WR_VREF_OFFSET_BACKSIDE;
    }
  } else if (group == CmdAll) {
    RcDebugPrint (SDBG_DEFAULT, "Cmd Eye Diagrams\n");
    if (level == LrbufLevel) {
      maxVrefOffset = 20;
    } else {
      maxVrefOffset = 50;
    }
  }
  if (GetDebugLevel () & SDBG_MINMAX) {
    //
    // Save initial timestamp for latency measurements
    //
    startTsc = GetPerformanceCounter ();
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      //
      // Set all filter bits
      //
      CopyMem (filter.bits[ch][rank], dataMask, MSVx8);

      //
      // Clear margin data
      //
      for (vIndex = 0; vIndex < MAX_VREF_OFFSET_RANGE; vIndex++) {
        (*results)[vIndex].rank[ch][rank].n = 0;
        (*results)[vIndex].rank[ch][rank].p = 0;
      } // vIndex
    } // rank loop
  } // ch loop

  GetMultiVref (Host, socket, level, group, mode, scope, &filter, (VOID *) results, patternLength, 0, 0, 1, (maxVrefOffset * 2) + 1);

  if (group == RxDqsDelay) {
    RcDebugPrint (SDBG_DEFAULT, "\nRead Eye\n");
  } else if (group == RxDqsPDelay) {
    RcDebugPrint (SDBG_DEFAULT, "\nRead (P) Eye\n");
  } else if (group == RxDqsNDelay) {
    RcDebugPrint (SDBG_DEFAULT, "\nRead (N) Eye\n");
  } else if (group == TxDqDelay) {
    RcDebugPrint (SDBG_DEFAULT, "\nWrite Eye\n");
  } else if (group == CmdAll) {
    RcDebugPrint (SDBG_DEFAULT, "\nCmd Eye\n");
  }
  DisplayEye (Host, socket, scope, level, (VOID *) results, maxVrefOffset);

  endTsc = GetPerformanceCounter ();
  mrcLatency = TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS);
  RcDebugPrint (SDBG_DEFAULT, "\nDisplayEye time            %d ms\n", mrcLatency);
  RcFreePool (results);
} // EyeDiagram

VOID
DisplayEye (
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           scope,
  MRC_LT          level,
  VOID            *results,
  UINT8           maxVrefOffset
  )
/**
  Calculates positive and negative margin for the group provided

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param scope
  @param VOID:         VOID pointer for the results
  @param maxVrefOffset

  @retval N/A

**/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               strobeStart = 0;
  UINT8               maxStrobe = 0;
  UINT8               bit;
  UINT8               bitStart = 0;
  UINT8               maxBit = 0;
  INT16               tmpMargin;
  INT16               margin;
  INT32               vIndex;
  INT16               neg = 0;
  INT16               pos = 0;
  struct rankMargin   (*rankM)[MAX_VREF_OFFSET_RANGE];
  struct strobeMargin (*strobeM)[MAX_VREF_OFFSET_RANGE];
  struct bitMargin    (*bitM)[MAX_VREF_OFFSET_RANGE];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  if (scope == SCOPE_RANK) {
    strobeStart = 0;
    maxStrobe = 1;
    bitStart = 0;
    maxBit = 1;
  } else if (scope == SCOPE_STROBE) {
    strobeStart = 0;
    maxStrobe = MSVx4;
    bitStart = 0;
    maxBit = 1;
  } else if (scope == SCOPE_BIT) {
    strobeStart = 0;
    maxStrobe = MSVx4;
    bitStart = 0;
    maxBit = 72;
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    // Check if this channel is masked off

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList (Host, socket, ch, dimm);

      AcquirePrintControl ();

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (strobe = strobeStart; strobe < maxStrobe; strobe++) {
          for (bit = bitStart; bit < maxBit; bit++) {

            RcDebugPrintWithDevice (SDBG_DEFAULT, socket, ch, dimm, NO_SUBCH, rank, strobe, bit, "\n");
            RcDebugPrint (SDBG_DEFAULT, "\n   3322222222221111111111                   1111111111222222222233\n");
            RcDebugPrint (SDBG_DEFAULT, "   109876543210987654321098765432101234567890123456789012345678901\n");
            for (vIndex = (maxVrefOffset * 2) + 1; vIndex >= 0; vIndex--) {
              if ((level == LrbufLevel) && ((vIndex % 2) == 0)) {
                continue;
              }
              if (scope == SCOPE_RANK) {
                rankM = results;
                if ((*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].n >
                    (*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].p) {

                  (*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].n = 0;
                  (*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].p = 0;
                }
                neg = (*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].n;
                pos = (*rankM)[vIndex].rank[ch][ (*rankList)[rank].rankIndex].p;
              } else if (scope == SCOPE_STROBE) {
                strobeM = results;
                if ((*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n >
                    (*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {

                  (*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
                  (*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
                }
                neg = (*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n;
                pos = (*strobeM)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p;
              } else if (scope == SCOPE_BIT) {
                bitM = results;
                if ((*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].n >
                    (*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].p) {

                  (*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].n = 0;
                  (*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].p = 0;
                }
                neg = (*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].n;
                pos = (*bitM)[vIndex].bits[ch][ (*rankList)[rank].rankIndex][bit].p;
              }
              if (neg == pos) {
                continue;
              }
              if ((neg > MAX_TIMING_OFFSET) && (pos < -MAX_TIMING_OFFSET)) {
                continue;
              }
              RcDebugPrint (SDBG_DEFAULT, "%3d ", vIndex - maxVrefOffset);
              //
              // Calculate the negative margin
              //
              tmpMargin = neg;
              for (margin = -MAX_TIMING_OFFSET; margin <= MAX_TIMING_OFFSET; margin++) {
                if (margin == tmpMargin) {
                  RcDebugPrint (SDBG_DEFAULT, "#");
                  //
                  // Calculate the positive margin
                  //
                  tmpMargin = pos;
                } else if ((vIndex == maxVrefOffset) && (margin == 0)) {
                  RcDebugPrint (SDBG_DEFAULT, "+");
                } else {
                  RcDebugPrint (SDBG_DEFAULT, " ");
                }
              }
              RcDebugPrint (SDBG_DEFAULT, "\n");
            } // vIndex loop

          } // bit loop
        } // strobe loop
      } // rank loop
      ReleasePrintControl ();
    } // dimm loop
  } // ch loop
} // DisplayEye

VOID
DisplayTxDqDqsBitResults (
  PSYSHOST  Host,
  UINT8     socket
  )
/*++

  Displays the training results for Wr Dq/Dqs

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               bit;
  INT16               delay;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                "START_DATA_TX_DQ_BIT\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          for (bit = 0; bit < 4; bit++) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &delay);
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
                          "TxDq: Pi = %d\n", delay);
          } // bit loop
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                "STOP_DATA_TX_DQ\n");
}

/**
  Prints out the per Strobe cached values of the input MRS registers

  @param[in]      Host         Pointer to SYSHOST
  @param[in]      Socket       Socket number
  @param[in]      Ch           Channel number
  @param[in]      Dimm         Dimm number
  @param[in]      Rank         Rank number
  @param[in]      MrsRegister  Desired MRS register to print (0 for MR0, 1 for MR1, etc...)

  @retval   EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MemDebugDumpPerStrobeMrs (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     MrsRegister
  )
{
#ifndef DDR5_SUPPORT
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT16              PerDramValue = 0;
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];

  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

  if (IsLrdimmPresent (Socket, Ch, Dimm)) {
    MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, TxVref, LrbufLevel);
  } else {
    MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, TxVref, DdrLevel);
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                "MR%d:", MrsRegister);

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    //
    // Skip if this is an ECC dram when ECC is disabled
    //
    if (IsStrobeNotValid (Host, Strobe)) {
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "     ");
    } else {
      PerDramValue = GetPerStrobeMrsValue (Socket, Ch, Dimm, Rank, Strobe, MrsRegister);
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "%5x", PerDramValue);
    }
  } // strobe loop
  RcDebugPrint (SDBG_MEM_TRAIN,
                "\n");
#endif // DDR5_SUPPORT
  return EFI_SUCCESS;
}

VOID
DisplayMRS (
  PSYSHOST  Host,
  UINT8     socket
  )
/*++
  Displays the mode register settings

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A

--*/
{
#ifndef DDR5_SUPPORT
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\nSTART_DATA_MRS\n                  ");

  for (strobe = 0; strobe < MSVx4; strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MEM_TRAIN,
                  "  %2d ", strobe);
  } // strobe loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      for (rank = 0; rank < (*dimmNvList)[dimm].numDramRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK0);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK1);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK2);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK3);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK4);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK5);
        MemDebugDumpPerStrobeMrs (Host, socket, ch, dimm, rank, BANK6);
      } // rank loop
    } // dimm loop
  } // ch loop
#endif // DDR5_SUPPORT
} // DisplayMRS
#endif  // DEBUG_CODE_BLOCK

/**
  Prints a heading for per-bit results displays.

  @param [in]     Host        - Pointer to SysHost
  @param [in]     Socket      - Socket number
  @param [in]     Bit         - Specific bit number being displayed
  @param [in]     MaxBits     - Maximum number of bits to display

  @retval N/A
**/
VOID
PrintPerBitHeading (
  IN     PSYSHOST      Host,
  IN     UINT8         Socket,
  IN     UINT8         Bit,
  IN     UINT8         MaxBits
  )
{
  UINT8     BitNum;

  if (Bit == (MaxBits / 2)) {
    RcDebugPrint (SDBG_MEM_TRAIN, "\n\n");
  }
  for (BitNum = Bit; BitNum < (Bit + (MaxBits / 2)); BitNum++) {
    RcDebugPrint (SDBG_MEM_TRAIN, "  %2d", BitNum);
  }
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
}

VOID
DisplayResults (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
/*++
  Displays the training results

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param group:        MRC_GT group

  @retval N/A

--*/
{
  UINT8               ch;
#ifdef DDR5_SUPPORT
  UINT8               SubCh = 0;
#endif
  UINT8               dimm;
  UINT8               FirstDimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               bit;
  UINT8               b;
  UINT8               maxBits;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  INT16               delay = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct RankCh       (*rankPerCh)[MAX_RANK_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT32              IoComp = 0;
  CHAR8               *GroupString;
  UINT8               MaxChDdr;
  READ_TO_READ_TURNAROUND_STRUCT RdtoRdData;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  if (EFI_ERROR (GetGroupString (group, &GroupString))) {
    GroupString = "Unknown group";
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\nSTART_DATA_%a\n              ", GroupString);

  if (Host->nvram.mem.eccEn) {
    maxBits = MBV;
  } else {
    maxBits = MBV - 8;
  }

  if ((group != RxDqsBitDelay) && (group != TxDqBitDelay) && (group != RxVref) && (group != RxDqsPBitDelay) && (group != RxDqsNBitDelay)) {
    for (strobe = 0; strobe < MSVx4; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %2d ", strobe);
    } // strobe loop
  } // if not per bit
  RcDebugPrint (SDBG_MEM_TRAIN,
                "\n");

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    DisplayTcwlAdj (Host, socket, ch, group);

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankPerCh = &(*channelNvList)[ch].rankPerCh;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, DdrLevel);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }

        if (!FindFirstDimmInChannel (socket, ch, &FirstDimm)) {
          RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
        }

        if ((group == RxVref) && ((dimm != FirstDimm) | (rank > 0))) {
          continue;
        }

        if ((group == TxVref) && IsLrdimmPresent (socket, ch, dimm)&& (rank > 0)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                      "");

        if ((group == RxDqsBitDelay) || (group == RxDqsPBitDelay) || (group == RxDqsNBitDelay) || (group == TxDqBitDelay) || (group == RxVref)) {
          for (bit = 0; bit < maxBits; bit++) {
            if ((bit/4) %2) {
              strobe = bit/8 + MSVx8;
            } else {
              strobe = bit/8;
            }
            if ((bit == 0) || (bit == (maxBits / 2))) {
              RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");
              if (bit == (maxBits / 2)) {
                RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");
              }
              for (b = bit; b < (bit + (maxBits / 2)); b++) {
                RcDebugPrint (SDBG_MEM_TRAIN,
                              "  %2d", b);
              }
              RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");
            }

            // call SetGDCR to update the appropriate register field
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, (bit%4), DdrLevel, group, GSM_READ_ONLY,
              &delay);
            RcDebugPrint (SDBG_MEM_TRAIN,
                        " %3d", delay);

          } // bit loop
          RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");

        } else {
          for (strobe = 0; strobe < maxStrobe; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, group, GSM_READ_ONLY, &delay);
            RcDebugPrint (SDBG_MEM_TRAIN,
                          "%5d", delay);
          } // strobe loop

          RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");

          if (group == RecEnDelay) {
            GetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), GSM_READ_ONLY, &RoundTrip);
            GetIoLatency (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), GSM_READ_ONLY, &IoLatency);
#ifdef DDR5_SUPPORT
            if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
              for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
                GetIoLatencyIoComp (Host, socket, ch, SubCh, &IoComp, GSM_READ_ONLY);
                RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, SubCh, rank, NO_STROBE, NO_BIT,
                  "IO Latency = %d, Round Trip = %d, IO Comp = %d\n", IoLatency, RoundTrip, IoComp);
              } // SubCh
            }
#else
            GetIoLatencyIoComp (Host, socket, ch, SUB_CH_A, &IoComp, GSM_READ_ONLY);
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
              "IO Latency = %d, Round Trip = %d, IO Comp = %d\n", IoLatency, RoundTrip, IoComp);
#endif  // #ifdef DDR5_SUPPORT
          }
        } // if per bit
      } // rank loop
    } // dimm loop

    if (group == RecEnDelay) {
      GetReadToReadTiming (socket, ch, TRrdrType | TRrddType, &RdtoRdData);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "tRRDR = %d, tRRDD = %d\n", RdtoRdData.TRrdr, RdtoRdData.TRrdd);
    }
  } // ch loop
} // DisplayResults

VOID
DisplayBwSerr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT32    bwSerr[3],
  INT16     margin
  )
/**
  Displays the bitwise error status

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket
  @param ch:           Memory Channel
  @param bwSerr[3]     BitWise error status

  @retval N/A

**/
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   bit;
  UINT8   maxBits;
  UINT8   bitIndex;
  UINT32  errResult = 0;

  AcquirePrintControl ();

  maxBits = GetMaxBitsValid (Host);

  RcDebugPrintWithDevice (SDBG_DEFAULT, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "%3d", margin);

  for (bit = 0; bit < maxBits; bit++) {

    if (IsBitNotValid (Host, bit)) {
      continue;
    }
    if ((bit % 8) == 0) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    switch (bit) {
    case 0:
      errResult = bwSerr[0];
      break;

    case 32:
      errResult = bwSerr[1];
      break;

    case 64:
      errResult = bwSerr[2];
      break;
      default:
      //Automated add of default case. Please review.
      break;
    } // switch

    bitIndex = bit % 32;

    if (errResult & (1 << bitIndex)) {
      RcDebugPrint (SDBG_DEFAULT, "#");
    } else {
      RcDebugPrint (SDBG_DEFAULT, ".");
    }

  } // bit loop

  RcDebugPrint (SDBG_DEFAULT, "\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayBwSerr

#ifdef DEBUG_CODE_BLOCK
#ifdef LRDIMM_SUPPORT
/**
  Displays the LRDIMM backside training results

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Group:        Signal group

  @retval N/A
**/

VOID
DisplayDdr4LrdimmTrainingResults (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  MRC_GT    Group
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               MaxStrobe;
  INT16               Offset;
  CHAR8               *SignalString;
  UINT8               Index;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MaxChDdr;
  GROUP_STRING  LrdimmGroupString[] = {{RxVref,          "RXVREF"},
                                       {RecEnDelay,      "RX_RCVEN"},
                                       {RecEnDelayCycle, "RX_CYCLE"},
                                       {RxDqsDelay,      "RX_DQSDELAY"},
                                       {TxDqsDelay,      "WRITE_LEVELING"},
                                       {TxDqsDelayCycle, "WRITE_CYCLE"},
                                       {TxDqDelay,       "TX_DQDELAY"},
                                       {TxVref,          "TXVREF"}
                                      };

  MaxChDdr = GetMaxChDdr ();

  //
  // Find the string for the signal
  //
  SignalString = "UNKNOWN_SIGNAL";
  for (Index = 0; Index < ARRAY_SIZE (LrdimmGroupString); Index++) {
    if (Group == LrdimmGroupString[Index].Group) {
      SignalString = LrdimmGroupString[Index].String;
      break;
    }
  }
  //
  // Insert the signal string in the middle of the standard "LRDIMM_BACKSIDE_xxxxx_TRAINING_RESULTS"
  //
  RcDebugPrint (SDBG_MEM_TRAIN, "\nLRDIMM_BACKSIDE_%a_TRAINING_RESULTS\n             ", SignalString);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MEM_TRAIN, "  %3d", Strobe);
  }
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].lrDimmPresent == 0) {
        continue;
      }
      MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, LrbufLevel);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numDramRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "");
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, LrbufLevel, Group,
            GSM_READ_ONLY, &Offset);

          // subtract 16 so values are -15 to + 15 instead of 0 to 31
          if ((Group == RxDqsDelay) || (Group == TxDqDelay)) {
            Offset -= 15;
          }

          if (Group == RecEnDelayCycle) {
            if (Offset & BIT2) {
              Offset &= ~BIT2;
              Offset *= -1;
            }
          }

          RcDebugPrint (SDBG_MEM_TRAIN, "  %3d",  Offset);
        } // Strobe loop
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
} //DisplayDdr4LrdimmTrainingResults
#endif // LRDIMM_SUPPORT

/**
  Displays the training results for all parameters

  @param Host:         Pointer to sysHost

  @retval N/A
**/
UINT32
DisplayTrainResults (
  PSYSHOST  Host
  )
{
  UINT8  socket;
  UINT32 serialDebugMsgLvlsave = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->common.debugJumper) {
    serialDebugMsgLvlsave = GetDebugLevel ();
    if (serialDebugMsgLvlsave != 0) {
      SetDebugLevel (serialDebugMsgLvlsave | SDBG_INFO | SDBG_MEM_TRAIN);
    }
  }

  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  AcquirePrintControl ();

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "START_TRAINING_REGISTER_DUMP\n");

  DisplayXoverResults (Host, socket);

  if (Setup->mem.enableNgnBcomMargining) {
    DisplayXoverResultsFnv (Host, socket);
  }

  DisplayResults (Host, socket, RecEnDelay);
  if (GetTrainingStepEnableChip (socket, CHECKPOINT_MINOR_T_RX_DFE)) {
    DisplayResults (Host, socket, RxStrobeInvert);
  }
  DisplayResults (Host, socket, RxFlyBy);

  if (Setup->mem.optionsExt & DUTY_CYCLE_EN) {
    DisplayResults (Host, socket, RxDqsPDelay);
    DisplayResults (Host, socket, RxDqsNDelay);
  } else {
    DisplayResults (Host, socket, RxDqsDelay);
  }
  DisplayResults (Host, socket, RxDqDelay);
  DisplayResults (Host, socket, TxDqsDelay);
  DisplayResults (Host, socket, TxFlyBy);
  DisplayResults (Host, socket, TxDqDelay);
  DisplayResults (Host, socket, RxVref);
  DisplayResults (Host, socket, TxVref);
  DisplayCCCResults (Host, socket);
  DisplayMRS (Host, socket);
  if (DESKEW_DELAY == RxDqsBitDelay) {
    DisplayResults (Host, socket, RxDqsBitDelay);
  } else {
    DisplayResults (Host, socket, RxDqsPBitDelay);
    DisplayResults (Host, socket, RxDqsNBitDelay);
  }
  DisplayResults (Host, socket, TxDqBitDelay);

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  if ((Host->nvram.mem.dramType == SPD_TYPE_DDR4) && (Host->nvram.mem.socket[socket].lrDimmPresent)) {
    DisplayDdr4LrdimmTrainingResults (Host, socket, RecEnDelay);
    DisplayDdr4LrdimmTrainingResults (Host, socket, RecEnDelayCycle);
    DisplayDdr4LrdimmTrainingResults (Host, socket, RxDqsDelay);
    DisplayDdr4LrdimmTrainingResults (Host, socket, RxVref);
    DisplayDdr4LrdimmTrainingResults (Host, socket, TxDqsDelay);
    DisplayDdr4LrdimmTrainingResults (Host, socket, TxDqsDelayCycle);
    DisplayDdr4LrdimmTrainingResults (Host, socket, TxDqDelay);
    DisplayDdr4LrdimmTrainingResults (Host, socket, TxVref);
  }
#endif //LRDIMM_SUPPORT && !DDR5_SUPPORT

  DisplayResultsChip (Host, socket);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "STOP_TRAINING_REGISTER_DUMP\n");

  ReleasePrintControl ();

  if (serialDebugMsgLvlsave != 0) {
    SetDebugLevel (serialDebugMsgLvlsave);
  }

  //
  // Dump DRAM registers to serial log
  //
  DumpDimmState (socket);
  return SUCCESS;
} // DisplayTrainResults
#endif // DEBUG_CODE_BLOCK

/**
  This routine prints out memory configuration of each socket at channel level.

  @param[in] Host    Pointer to sysHost.

  @retval VOID
**/
VOID
PrintDimmConfig (
  IN PSYSHOST Host
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8 SocketId;
  UINT8 ChId;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  //
  // Display memory sizes in serial port for debugging.
  //
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "        Platform DIMM Configuration(num_chunks(chunk_size))\n");
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (Host->nvram.mem.socket[SocketId].enabled == 0) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "Socket  : %d\n", SocketId);
    for (ChId = 0; ChId < MaxChDdr; ChId++) {
      RcDebugPrint (SDBG_MAX, "\tChannel   : %d  DDR Size  : %d(64MB)  PMem volatile Size : %d(64MB)",
        ChId,
        Host->var.mem.socket[SocketId].channelList[ChId].memSize,
        Host->var.mem.socket[SocketId].channelList[ChId].volSize
        );

      if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_BIOS_SETUP) {
        RcDebugPrint (SDBG_MAX, "  PMem persistent Size : %d(64MB)  PMem persistent Wb Size : %d(64MB)",
          Host->var.mem.socket[SocketId].channelList[ChId].perSize,
          Host->var.mem.socket[SocketId].channelList[ChId].WbCachePerSize
          );
      } else {
        RcDebugPrint (SDBG_MAX, "  PMem non volatile Size : %d(64MB)",
          Host->var.mem.socket[SocketId].channelList[ChId].NonVolSize
          );
      }

      if (PcdGetBool (PcdNvDimmEn)) {
        RcDebugPrint (SDBG_MAX, "  NVDIMM-N Memory Size  : %d(64MB)",
          Host->var.mem.socket[SocketId].channelList[ChId].NVmemSize
          );
      }

      RcDebugPrint (SDBG_MAX, "\n\n");
    }
  }
#endif // DEBUG_CODE_BLOCK
}

#ifdef DEBUG_CODE_BLOCK
/**
  Displays the CMD Vref training results

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A
**/
VOID
DisplayCMDVrefResults (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  INT16     Offset[MAX_CH];
  UINT8     Ch;
  UINT8     Dimm;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  RcDebugPrint (SDBG_MEM_TRAIN,
                "\nSTART_DATA_CMD_VREF\n");

  for (Ch = 0; Ch < MaxChDdr; Ch ++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*dimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      GetSetCmdVref (Host, socket, Ch, 0, DdrLevel, GSM_READ_ONLY, &Offset[Ch]);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "%d\n", Offset[Ch] );
    } // Dimm loop
  } //Ch loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");
} //DisplayCMDVrefResults

/**

  Displays information about the SPD contents

  @param[in] Socket  - Processor Socket

  @retval N/A

**/
VOID
DisplaySpdContents (
  IN  UINT8  Socket
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               Dimm;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList) [MAX_CH];
  SYS_SETUP           *Setup;
  UINT16              Index;
  UINT8               SPDReg;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  // SPD_PRINT_EN
  if (Setup->mem.SpdPrintEn == SPD_PRINT_EN) {

    RcDebugPrint (SDBG_MINMAX, "START_PRINT_SPD\n");

    Host = GetSysHostPointer ();

    channelNvList = GetChannelNvList (Host, Socket);

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*channelNvList)[Ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, Socket, Ch);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if ((*dimmNvList)[Dimm].dimmPresent) {

          //
          // Print the Socket, Ch and Dimm information.
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n   ");

          //
          // Print the Column Number of the SPD data.
          //
          for (Index = 0; Index < 0x10; Index++) {
            RcDebugPrint (SDBG_MINMAX, " %02x", Index);
          }

          for (Index = 0; Index < Setup->mem.SpdPrintLength; Index++) {

            //
            // Print the Carriage Return and Byte Index of SPD data.
            //
            if ((Index % 0x10) == 0) {

              //
              // Split the SPD data for every 256 bytes
              //
              if (((Index % 0x100) == 0) && (Index != 0)) {
                RcDebugPrint (SDBG_MINMAX, "\n");
              }
              RcDebugPrint (SDBG_MINMAX, "\n%02x:", (UINT8) (Index & 0x00F0));
            }

            SpdReadByte (Socket, Ch, Dimm, Index, &SPDReg);
            RcDebugPrint (SDBG_MINMAX, " %02x", SPDReg);
          }

          RcDebugPrint (SDBG_MINMAX, "\n");

          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
        }

      } // Dimm loop
    } // Ch loop

    RcDebugPrint (SDBG_MINMAX, "STOP_PRINT_SPD\n");
  }
} // DisplaySpdContents
#endif  // DEBUG_CODE_BLOCK

/**
  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetSignalStr (
  IN  GSM_CSN   SignalName
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8     Index;

#ifdef DDR5_SUPPORT
  PSYSHOST                         Host;

  Host = GetSysHostPointer();
  if (IsDdr5Present(Host, GetSysSocketId ())) {
    return GetSignalStrDdr5(SignalName);
  }
#endif

  for (Index = 0; Index < ARRAY_SIZE (mSignalString); Index++) {
    if (SignalName == mSignalString[Index].Signal) {
      return mSignalString[Index].String;
    }
  }
#endif // DEBUG_CODE_BLOCK
   return mUnknownSignal;
}

/**
  This function is used to get a string associated with the given I/O level.

  @param[in]   Level   I/O level.

  @retval   String describing the I/O level.
**/
CHAR8*
EFIAPI
GetLevelStr (
  MRC_LT      level
  )
{
  switch (level) {
  case DdrLevel:
    return "DdrLevel";
    break;
  case LrbufLevel:
    return "LrbufLevel";
    break;
  case RegALevel:
    return "RegALevel";
    break;
  case RegBLevel:
    return "RegBLevel";
    break;
  case HbmLevel:
    return "HbmLevel";
    break;
  default:
    return "Unknown level";
    break;
  }
} // GetLevelStr

/**
  Return a pointer to a CMD group string

  @param[in]  Group   - MRC_GT Group value

  @retval   String pointer corresponding to Group
**/
CHAR8*
GetPlatformGroupStr (
  IN  MRC_GT  Group
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8                 GroupIndex;
  STATIC GROUP_STRING   CmdGroupString[] = {{CmdAll,     "CmdAll"},
                                            {CmdGrp0,    "CmdGrp0"},
                                            {CmdGrp1,    "CmdGrp1"},
                                            {CmdGrp2,    "CmdGrp2"},
                                            {CtlAll,     "CtlAll"},
                                            {CtlGrp0,    "CtlGrp0"},
                                            {CtlGrp1,    "CtlGrp1"},
                                            {CtlGrp2,    "CtlGrp2"},
                                            {CtlGrp3,    "CtlGrp3"},
                                            {CtlGrp4,    "CtlGrp4"},
                                            {CtlGrp5,    "CtlGrp5"},
                                            {CkAll,      "CkAll"},
                                            {CmdCtlAll,  "CmdCtlAll"},
                                           };

  for (GroupIndex = 0; GroupIndex < ARRAY_SIZE (CmdGroupString); GroupIndex++) {
    if (CmdGroupString[GroupIndex].Group == Group) {
      return CmdGroupString[GroupIndex].String;
    }
  }
#endif  // DEBUG_CODE_BLOCK
  return mUnknownGroup;
} // GetPlatformGroupStr

/**

  Displays Advanced Centering data

  @param[in] Host           Pointer to sysHost
  @param[in] Socket         Processor socket
  @param[in] Ch             Channel number
  @param[in] Sub-Ch         SubChannel number
  @param[in] Dimm           DIMM number
  @param[in] Rank           Rank number
  @param[in] Group          Parameter to center
  @param[in] Results        Pointer to the structure to store the margin results
  @param[in] Delay          Pointer to the delay value
  @param[in] OldDelay       Pointer to the old delay value
  @param[in] MaxVref        Max vref
  @param[in] StepSize       Setp size
  @param[in] NumPoints      Num points

  @retval N/A

**/
VOID
DisplayAdvancedCenteringResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Subchannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_GT    Group,
  IN VOID      *Results,
  IN INT16     *Delay,
  IN INT16     *OldDelay,
  IN UINT8     MaxVref,
  IN UINT8     StepSize,
  IN UINT8     NumPoints
  )
{
#ifndef DDR5_SUPPORT
  UINT8               Strobe;
  UINT8               VIndex;
  INT16               Margin;
  struct strobeMargin (*DisplayResults)[MAX_VREF_POINTS];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  DisplayResults = Results;

  if (GetDebugLevel () & SDBG_MAX) {

    RcDebugPrint (SDBG_DEFAULT, "Results for ");
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "\n");
    RcDebugPrint (SDBG_DEFAULT, "        ");

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValid(Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_DEFAULT, " %3d", Strobe);
    } // strobe loop
    RcDebugPrint (SDBG_DEFAULT, "\n");
    RcDebugPrintLine (SDBG_MAX, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    for (VIndex = NumPoints; VIndex > 0; VIndex--) {

      Margin = -MaxVref + ((VIndex - 1) * StepSize);
      RcDebugPrint (SDBG_DEFAULT, "Vref offset: %d\n", Margin);
      // Left Edge
      RcDebugPrint (SDBG_DEFAULT, "Left:  ");

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", (*DisplayResults)[(VIndex - 1)].strobe[Channel][(*RankList)[Rank].rankIndex][Strobe].n);
      } // strobe loop

      // Right Edge
      RcDebugPrint (SDBG_DEFAULT, "\nRight: ");

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", (*DisplayResults)[(VIndex - 1)].strobe[Channel][(*RankList)[Rank].rankIndex][Strobe].p);
      } // strobe loop
      RcDebugPrint (SDBG_DEFAULT, "\n");
    } // vIndex loop

    RcDebugPrintLine (SDBG_MAX, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    // Previous delay
    RcDebugPrint (SDBG_DEFAULT, "Prev Pi:");

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, " %3d", OldDelay[Strobe]);
    } // strobe loop

    // Print new delay
    RcDebugPrint (SDBG_DEFAULT, "\nNew Pi: ");

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, " %3d", Delay[Strobe]);
    } // strobe loop

    // Print difference
    RcDebugPrint (SDBG_DEFAULT, "\nDiff:   ");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, " %3d", Delay[Strobe] - OldDelay[Strobe]);
    } // strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n\n");
  }
#endif
}

/**

  Displays And Adjust Vref Centering data

  @param[in] Host            Pointer to sysHost
  @param[in] Socket          Processor socket
  @param[in] Group           Parameter to group
  @param[in] BitResults      Pointer to the bit results
  @param[in] StrobeResults   Pointer to the strobe results
  @param[in] PerBit          Vref training mode perbit

  @retval N/A

**/
VOID
DisplayAndAdjustVrefCenteringResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group,
  IN VOID      *BitResults,
  IN VOID      *StrobeResults,
  IN UINT8     PerBit
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               RankIndex;
  UINT8               Strobe;
  UINT8               Bit;
  UINT8               MaxChDdr;
  UINT8               MaxStrobe;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct bitMargin    (*VrefMarginBit)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin (*VrefMarginStrobe)[MAX_VREF_TIMING_NUMBER];
  UINT8               TimeIndex;
  UINT8               MBV = GetMaxBitsValid (Host);

  if ((BitResults == NULL) || (StrobeResults == NULL)) {
    RcDebugPrint (SDBG_MAX, "The result pointer is NULL!");
    ASSERT (FALSE);
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  VrefMarginBit = BitResults;
  VrefMarginStrobe = StrobeResults;

  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();

    if (Group == RxVref) {
      RcDebugPrint (SDBG_DEFAULT, "START_RX_VREF_CENTER\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "START_TX_VREF_CENTER\n");
    }

    for (Ch = 0;Ch < MaxChDdr; Ch++) {
      //
      // Skip if channel is disabled
      //
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if no DIMM present
        //
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
          //
          // Skip if no rank or if it is mapped out
          //

          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (IsLrdimmPresent (Socket, Ch, Dimm) && (Group == TxVref) && (Rank > 0)) {
            continue;
          }

          RankIndex = (*RankList)[Rank].rankIndex;

          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "vref Margins        -            0            +\n");

          if (PerBit == PER_BIT) {
            for (Bit = 0; Bit < MBV; Bit++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if (IsBitNotValid (Host, Bit)) {
                continue;
              }
              for (TimeIndex = 0; TimeIndex < MAX_VREF_TIMING_NUMBER; TimeIndex++) {
                if ((*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].n >
                    (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].p) {

                    (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].p = 0;
                    (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].n = 0;
                }
              } // timeIndex loop
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit, "  %3d : %3d,  %3d : %3d,  %3d : %3d\n",
                    (*VrefMarginBit)[0].bits[Ch][RankIndex][Bit].n,
                    (*VrefMarginBit)[0].bits[Ch][RankIndex][Bit].p,
                    (*VrefMarginBit)[2].bits[Ch][RankIndex][Bit].n,
                    (*VrefMarginBit)[2].bits[Ch][RankIndex][Bit].p,
                    (*VrefMarginBit)[1].bits[Ch][RankIndex][Bit].n,
                    (*VrefMarginBit)[1].bits[Ch][RankIndex][Bit].p
                    );
              } // bit loop
            } else {
              MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, DdrLevel);
              for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
                //
                // Skip if this is an ECC strobe when ECC is disabled
                //
                if (IsStrobeNotValid (Host, Strobe)) {
                  continue;
                }

                for (TimeIndex = 0; TimeIndex < MAX_VREF_TIMING_NUMBER; TimeIndex++) {
                  if ((*VrefMarginStrobe)[TimeIndex].strobe[Ch][RankIndex][Strobe].n >
                      (*VrefMarginStrobe)[TimeIndex].strobe[Ch][RankIndex][Strobe].p) {

                      (*VrefMarginStrobe)[TimeIndex].strobe[Ch][RankIndex][Strobe].p = 0;
                      (*VrefMarginStrobe)[TimeIndex].strobe[Ch][RankIndex][Strobe].n = 0;
                  }
                } // timeIndex loop
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT, "  %3d : %3d,  %3d : %3d,  %3d : %3d\n",
                    (*VrefMarginStrobe)[0].strobe[Ch][RankIndex][Strobe].n,
                    (*VrefMarginStrobe)[0].strobe[Ch][RankIndex][Strobe].p,
                    (*VrefMarginStrobe)[2].strobe[Ch][RankIndex][Strobe].n,
                    (*VrefMarginStrobe)[2].strobe[Ch][RankIndex][Strobe].p,
                    (*VrefMarginStrobe)[1].strobe[Ch][RankIndex][Strobe].n,
                    (*VrefMarginStrobe)[1].strobe[Ch][RankIndex][Strobe].p
                );
              } // strobe loop
            }
          RcDebugPrint (SDBG_MAX, "\n");
        } // rank loop
      } // dimm loop
    } // ch loop

    if (Group == RxVref) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_VREF_CENTER\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "STOP_TX_VREF_CENTER\n");
    }

    ReleasePrintControl ();
  }
}
