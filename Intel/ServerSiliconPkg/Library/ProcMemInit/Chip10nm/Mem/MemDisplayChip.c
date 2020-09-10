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
#include "Chip/Include/MemProjectSpecific.h"
#include <Library/EmulationConfigurationLib.h>
#include "Chip10nm/Include/PerformanceValueTables.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SysHostPointerLib.h>

//
// Local Prototypes
//
STATIC VOID DisplaySwizzleTrainResults(PSYSHOST Host, UINT8 socket);

/**

  Displays tCwlAdj

  @param[in] Host:         Pointer to sysHost
  @param[in] socket:       Processor socket
  @param[in] ch:           Memory Channel
  @param[in] group:        MRC_GT group

  @retval N/A

**/
VOID
DisplayTcwlAdj(
  IN    PSYSHOST  Host,
  IN    UINT8     socket,
  IN    UINT8     ch,
  IN    MRC_GT    group
    )
{
#ifdef DEBUG_CODE_BLOCK
  INT16               cwlAdj;
  UINT8               Dimm;
  UINT8               DDR4Done = 0;
#ifdef DDRT_SUPPORT
  UINT8               AEPDone = 0;
#endif // DDRT_SUPPORT
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  if(group == TxDqsDelay) {
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*dimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if (!((*dimmNvList)[Dimm].DcpmmPresent) && !DDR4Done) {
        GetCwlAdj (Host, socket, ch, Dimm, &cwlAdj);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "10NM: t_cwl_adj = %d\n", cwlAdj);
        DDR4Done = 1;
      }

#ifdef DDRT_SUPPORT
      if ((*dimmNvList)[Dimm].DcpmmPresent && !AEPDone) {
        GetCwlAdj (Host, socket, ch, Dimm, &cwlAdj);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"10NM: t_cwl_adj_PMem = %d\n", cwlAdj);
        AEPDone = 1;
      }
#endif // DDRT_SUPPORT
    } // for Dimm
  }  // if TxDqsDelay
#endif // DEBUG_CODE_BLOCK
}

#ifdef DEBUG_CODE_BLOCK
#ifdef DDR5_SUPPORT
VOID
DisplayTcwlAdjDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     subch,
  MRC_GT    group
  )
/*++

Displays tCwlAdj

@param Host:         Pointer to sysHost
@param socket:       Processor socket
@param ch:           Memory Channel
@param group:        MRC_GT group

@retval N/A

--*/
{
  INT16               cwlAdj;
  UINT8               Dimm;
  UINT8               DDR5Done = 0;
  UINT8               AEPDone = 0;
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  if (group == TxDqsDelay) {
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*dimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      if (!((*dimmNvList) [Dimm].DcpmmPresent) && !DDR5Done) {
        GetCwlAdjDdr5 (Host, socket, ch, subch, Dimm, &cwlAdj);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, subch, NO_RANK, NO_STROBE, NO_BIT, "10NM: t_cwl_adj = %d\n", cwlAdj);
        DDR5Done = 1;
      }

#ifdef DDRT_SUPPORT
      if ((*dimmNvList) [Dimm].DcpmmPresent && !AEPDone) {
        GetCwlAdj (Host, socket, ch, Dimm, &cwlAdj);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, subch, NO_RANK, NO_STROBE, NO_BIT, "10NM: t_cwl_adj_PMem = %d\n", cwlAdj);
        AEPDone = 1;
      }
#endif // DDRT_SUPPORT
    } // for Dimm
  }  // if TxDqsDelay
}
#endif //DDR5_SUPPORT

/*++

  Displays the training results

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A

--*/
VOID
DisplayXoverResultsFnv (
  PSYSHOST  Host,
  UINT8     socket
)
{
#ifdef DDRT_SUPPORT
  UINT8               ch;
  UINT8               dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  RcDebugPrint (SDBG_MAX,
                "\nSTART_DATA_XOVER_FNV\n");

  RcDebugPrint (SDBG_MAX,
                     "          CMDS\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (!(*dimmNvList)[dimm].DcpmmPresent) {
        continue;
      }

      RcDebugPrint (SDBG_MAX,
        "\n");
    }
  }
#endif // DDRT_SUPPORT
} // DisplayXoverResults

/**
  Displays the Swizzling register settings

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A
**/
STATIC
VOID
DisplaySwizzleTrainResults(
  PSYSHOST      Host,
  UINT8         socket
)
{
  UINT8                      ch;
  UINT8                      dimm;
  UINT8                      i;
  struct channelNvram        (*channelNvList)[MAX_CH];
  struct dimmNvram           (*dimmNvList)[MAX_DIMM];
  UINT8  MBV = GetMaxBitsValid(Host);
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX,
                 "\nSTART_SWIZZLE_TRAINING_RESULTS\n");

  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      // Report results
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    ");
      for (i = 0; i < MBV; i++) {
        if (IsBitNotValid(Host, i)) {
          continue;
        }
        if (i && (i % 8 == 0)) {
          RcDebugPrint (SDBG_MAX,
                       " ");
        }
        RcDebugPrint (SDBG_MAX,
                   "%d", ((*dimmNvList)[dimm].dqSwz[i / 2] >> ((i % 2) * 4)) & 7);
      } // i loop
      RcDebugPrint (SDBG_MAX,
                 "\n");
    } // dimm loop
  } // ch loop
  RcDebugPrint (SDBG_MAX,"\n");

} //DisplaySwizzleTrainResults

// ICX_TODO

/**
  Displays training results specific to chip/project

  @param[in]  Host     Pointer to sysHost
  @param[in]  Socket   Processor socket
**/
VOID
DisplayResultsChip (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
#if !defined (DDR5_SUPPORT) || defined (ICX_HOST)
  UINT8       Index;
  UINT8       StepCount;
  MRC_STATUS  Status;
  MRC_GT      PowerTrainSupportedSteps[] = {
    TxImode,
    RxEq,
#if !(defined (ICX_HOST) && defined (DDR5_SUPPORT))
    //
    // DDR5 steps needs to be added.
    //
    DramDrvStr,
#endif
    TxRon,
    RxOdt,
    TxEq,
    TxEqTap2,
    TxTco,
    CmdTxEq,
    RxCtleC,
    RxCtleR,
    TxRiseFallSlewRate,
    TxFallSlewRate,
    PxcDelta,
    DdjcDelta,
#if !(defined (ICX_HOST) && defined (DDR5_SUPPORT))
    //
    // DDR5 steps needs to be added.
    //
    ParkOdt,
    WrOdt,
    NomOdt,
#endif
    RxDqDelay,
    RxDfeCoefficient
    };

  if (UbiosGenerationOrHsleEnabled ()) {
    return;
  }

  DisplaySenseAmpTrainingResults (Socket);
  DisplayCMDVrefResults (Host, Socket);
  DisplaySwizzleTrainResults (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\nSTART_POWER_TRAINING_DUMP\n");

  StepCount = sizeof (PowerTrainSupportedSteps) / sizeof (PowerTrainSupportedSteps[0]);
  for (Index = 0; Index < StepCount; Index++) {
    Status = DisplayPowerTrainingResults (Host, Socket, PowerTrainSupportedSteps[Index]);
    RC_FATAL_ERROR ((Status == MRC_STATUS_SUCCESS), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_130);
  }

  DisplayCompRegResults10nm (Socket);
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  return;
#endif // !defined (DDR5_SUPPORT) || defined (ICX_HOST)
}

MRC_MST_STRING  mMstStr[] = {{ReservedMemSs,  "Reserved"},
                            {Ddr4MemSs,       "DDR4"},
                            {Ddr5MemSs,       "DDR5"},
                            {LpDdr4MemSs,     "LPDDR4"},
                            {LpDdr5MemSs,     "LPDDR5"},
                            {Hbm2MemSs,       "HBM2"},
                            {MrcMstMax,       "Unknown"}
                           };

/**
  Given a memory subsystem type (MRC_MST), return a pointer to a string of the MRC_MST.

  @param[in]   MemSsType      - Memory subsystem type
  @param[out]  MstSTring      - Pointer to caller's string pointer

  @retval   N/A

**/
VOID
MemSsNameLookup (
  IN    MRC_MST     MemSsType,
  OUT   CHAR8       **MstString
  )
{
  UINT8           Index;

  for (Index = 0; Index < sizeof (mMstStr) / sizeof (mMstStr[0]); Index++) {
    if (MemSsType == mMstStr[Index].MemSsType) {
      *MstString = mMstStr[Index].String;
      return;
    }
  }

  //
  // Return "Uknown" if no match found
  //
  *MstString = mMstStr[Index - 1].String;
}
/**
  Prints the fields of the registers specified in RegIndex

  @param Host           - Pointer to sysHost structure
  @param Socket         - Socket value
  @param Value          - Value of the register
  @param RegList        - Array of registers
  @param RegIndex       - Register index to be displayed
  @param FieldList      - Array of fields for the IP specified
  @param FieldArraySize - Size of FieldList
  @param Ip             - String of the IP
  @param Instance       - Instance of the register

  @retval N/A
**/
VOID
PrintPerformanceRegisters (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        Value,
  IN REGISTER_LIST RegList[],
  IN UINT8         RegIndex,
  IN FIELD_LIST    FieldList[],
  IN UINT8         FieldArraySize,
  IN UINT8*        Ip,
  IN UINT8         Instance
  )
{
  if (PcdGetBool (PcdMrcDisplayPerformanceValues) == TRUE) {

    UINT8 FieldIndex = 0;
    UINT8 CharIndex;
    UINT8 String[MAX_PERFORMANCE_REG_CHAR];
    UINT8 Index;

    for (Index = 0; Index < FieldArraySize; Index++) {
      if (FieldList[Index].Index == RegIndex){
        FieldIndex = Index;
        break;
      }
    }

    while (FieldList[FieldIndex].Index == RegIndex) {
      RcDebugPrint (SDBG_DEFAULT, "%6d |", Socket);

      ZeroMem (String, sizeof (String));
      AsciiStrCpyS ((CHAR8*)String, sizeof (String), RegList[RegIndex].RegisterName);

      for (CharIndex = 0; CharIndex < MAX_PERFORMANCE_REG_CHAR; CharIndex++) {
        if (String[CharIndex] != 0) {
          RcDebugPrint (SDBG_DEFAULT, "%c", String[CharIndex]);
        } else {
          RcDebugPrint (SDBG_DEFAULT, " ");
        }
      }

      RcDebugPrint (SDBG_DEFAULT, "|");
      ZeroMem (String, sizeof (String));
      AsciiStrCpyS ((CHAR8*)String, sizeof (String), (CHAR8*)FieldList[FieldIndex].FieldName);

      for (CharIndex = 0; CharIndex < MAX_PERFORMANCE_FIELD_CHAR; CharIndex++) {
        if (String[CharIndex] != 0) {
          RcDebugPrint (SDBG_DEFAULT, "%c", String[CharIndex]);
        } else {
          RcDebugPrint (SDBG_DEFAULT, " ");
        }
      }

      RcDebugPrint (SDBG_DEFAULT, "| %8x | ",
                    (Value >> FieldList[FieldIndex].Offset) & ~(0xFFFFFFFF << FieldList[FieldIndex].Bits));

      ZeroMem (String, sizeof (String));
      AsciiStrCpyS ((CHAR8*)String, sizeof (String), (CHAR8*)Ip);

      for (CharIndex = 0; CharIndex < MAX_PERFORMANCE_IP_CHAR; CharIndex++) {
        if (String[CharIndex] != 0) {
          RcDebugPrint (SDBG_DEFAULT, "%c", String[CharIndex]);
        } else {
          RcDebugPrint (SDBG_DEFAULT, " ");
        }
      }

      RcDebugPrint (SDBG_DEFAULT, " | %d\n", Instance);

      if (FieldIndex < FieldArraySize) {
        FieldIndex++;
      } else {
        break;
      }
    }
  }

  return;

}
#endif // DEBUG_CODE_BLOCK

/**
  Print performance settings

  @param Host  - Pointer to sysHost

  @retval N/A
**/
UINT32
PrintPerformanceSettings (
  IN PSYSHOST Host
  )
{
#ifdef DEBUG_CODE_BLOCK

  if (PcdGetBool (PcdMrcDisplayPerformanceValues) == TRUE) {

    UINT8  Socket = Host->var.mem.currentSocket;
    UINT8  Index;
    UINT8  TotCha;
    UINT32 Address = 0;
    UINT32 Value = 0;

    if (Host->nvram.mem.socket[Socket].enabled == 0) {
      return SUCCESS;
    }

    AcquirePrintControl ();

  RcDebugPrint (SDBG_DEFAULT, "Print Performance Settings\n");
  RcDebugPrint (SDBG_DEFAULT, "SOCKET |REGISTER");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 39);
  RcDebugPrint (SDBG_DEFAULT, "FIELD");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 28);
  RcDebugPrint (SDBG_DEFAULT, "    VALUE | IP");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 10);
  RcDebugPrint (SDBG_DEFAULT, " INSTANCE\n");
  RcDebugPrintLine (SDBG_DEFAULT, 125, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    TotCha = GetTotChaCount (Socket);

    // CHA registers
    Index = 0;
    while (Index < (sizeof(ChaRegList) / sizeof(ChaRegList[0]))) {
      if (Address != ChaRegList[Index].Address) {
        Address = ChaRegList[Index].Address;
        Value = KtiReadPciCfg (Socket, 0, Address);
      }

      PrintPerformanceRegisters (Host, Socket, Value, ChaRegList, Index, ChaFieldList, sizeof(ChaFieldList)/sizeof(ChaFieldList[0]), (UINT8*)"CHA", 0);
      Index++;
    }

#if !defined(TNR_HOST)
    // IIO registers
    Index = 0;
    while (Index < (sizeof(IioRegList) / sizeof(IioRegList[0]))) {
      if (Address != IioRegList[Index].Address) {
        Address = IioRegList[Index].Address;
        Value = UsraCsrRead (Socket, 0, Address);
      }

      PrintPerformanceRegisters (Host, Socket, Value, IioRegList, Index, IioFieldList, sizeof(IioFieldList)/sizeof(IioFieldList[0]), (UINT8*)"IO", 0);
      Index++;
    }
#endif
    // MS2IDI registers
    Index = 0;
    while (Index < (sizeof(Ms2idiRegList) / sizeof(Ms2idiRegList[0]))) {
      if (Address != Ms2idiRegList[Index].Address) {
        Address = Ms2idiRegList[Index].Address;
        Value = KtiReadPciCfg (Socket, 0, Address);
      }

      PrintPerformanceRegisters (Host, Socket, Value, Ms2idiRegList, Index, Ms2idiFieldList, sizeof(Ms2idiFieldList)/sizeof(Ms2idiFieldList[0]), (UINT8*)"MS2IDI", 0);
      Index++;
    }

    PrintMemPerformanceSettings (Host, Socket);
    RcDebugPrintLine (SDBG_DEFAULT, 125, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

    ReleasePrintControl ();
  }

#endif //DEBUG_CODE_BLOCK

  return SUCCESS;
}


/**
  Diplay memory subsystem information

  @param[in]   Host             - Pointer to sysHost structure
  @param[in]   Socket           - Socket number

  @retval   EFI_SUCCESS

**/
EFI_STATUS
DisplayMemSsList (
  IN    PSYSHOST        Host,
  IN    UINT8           Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8                 Index;
  UINT8                 NumMemSs;
  CHAR8                 *MstString;
  UINT32                MemBar;
  MRC_MST               MemSsType;
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
  NumMemSs = (UINT8)Host->var.mem.socket[Socket].NumMemSs;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Memory Subsystems:\n");
  for (Index = 0; Index < NumMemSs; Index++) {
    MemSsType = Host->var.mem.socket[Socket].MemSsList[Index].MemSsType;
    MemSsNameLookup (MemSsType, &MstString);
    MemBar = CpuCsrAccessVar->MmioBar[Socket][Index + TYPE_MEM_BAR0];
    RcDebugPrint (SDBG_MAX, "        %d: %a, MMIO Base: 0x%08x\n", Index, MstString, MemBar);
  }
#endif // DEBUG_CODE_BLOCK

  return EFI_SUCCESS;
}

/*++
  Displays the comp register contents

  @param[in] Socket:       Processor socket

  @retval N/A

--*/
VOID
DisplayCompRegResults10nm (
  IN UINT8     Socket
)
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                      Host;
  UINT8                         Ch;
  UINT8                         Strobe;
  UINT8                         Index;
  UINT32                        DataCompValue;
  CMD_CTL_CLK_COMP              CompValues[CmdCtlClkCompGroupMax];
  UINT8                         MaxChDdr;

  //
  // Only enabled for SDBG_MAX
  //
  if ((GetDebugLevel () & SDBG_MAX) == 0) {
    return;
  }

  Host = GetSysHostPointer ();

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX,
                "START_COMP_REG_DUMP");

  //DQ dump
  RcDebugPrint (SDBG_MAX,
                "\nNibble:\t\t    ");
  for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
    RcDebugPrint (SDBG_MAX,
                  "  %3u ", Strobe);
  } // Strobe loop

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "\n");
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  DrvUp:\t    ");
    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      GetDataCompValue (Host, Socket, Ch, Strobe, DataCompRCompDrvUp, &DataCompValue);
      RcDebugPrint (SDBG_MAX,
                  "  %3u ", DataCompValue);
    }
    RcDebugPrint (SDBG_MAX, "\n");
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  DrvDn:\t    ");
    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      GetDataCompValue (Host, Socket, Ch, Strobe, DataCompRCompDrvDn, &DataCompValue);
      RcDebugPrint (SDBG_MAX,
                  "  %3u ", DataCompValue);
    }
  } //Ch loop

  RcDebugPrint (SDBG_MAX,
                "\n\nFub:\t  \t   CLK\t  CMD_CMDN    CMD_CKE    CMD_CTL   CMD_CMDS    CTL_CMDN    CTL_CKE    CTL_CTL    CTL_CMDS\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Index = ClkCompGroup; Index < CmdCtlClkCompGroupMax; Index++) {
      GetCmdCtlClkCompValue (Host, Socket, Ch, Index, &CompValues[Index]);
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  DrvUp:\t   %3u      %3u        %3u        %3u        %3u        %3u          %3u        %3u       %3u\n",
                   CompValues[ClkCompGroup].DrvUp,
                   CompValues[CmdNCompGroup].DrvUp,
                   CompValues[CmdCkeCompGroup].DrvUp,
                   CompValues[CmdCtlCompGroup].DrvUp,
                   CompValues[CmdSCompGroup].DrvUp,
                   CompValues[CtlNCompGroup].DrvUp,
                   CompValues[CtlCkeCompGroup].DrvUp,
                   CompValues[CtlCtlCompGroup].DrvUp,
                   CompValues[CtlSCompGroup].DrvUp);

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  DrvDn:\t   %3u      %3u        %3u        %3u        %3u        %3u          %3u        %3u       %3u\n",
                   CompValues[ClkCompGroup].DrvDn,
                   CompValues[CmdNCompGroup].DrvDn,
                   CompValues[CmdCkeCompGroup].DrvDn,
                   CompValues[CmdCtlCompGroup].DrvDn,
                   CompValues[CmdSCompGroup].DrvDn,
                   CompValues[CtlNCompGroup].DrvDn,
                   CompValues[CtlCkeCompGroup].DrvDn,
                   CompValues[CtlCtlCompGroup].DrvDn,
                   CompValues[CtlSCompGroup].DrvDn);

  }
#endif // DEBUG_CODE_BLOCK
}
