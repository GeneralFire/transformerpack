/** @file
  DdrioPinMap.c
  Ddrio Pin Map file for SPR DDR4

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

#include <SysHostChip.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include <Include/MemDdrioSpecific.h>

PinRegister PinRegisterSprSa[] = {
  { ALERT_N, 0 },
  { A0, 1 },
  { A1, 2 },
  { A2, 3 },
  { A3, 4 },
  { A4, 5 },
  { A5, 6 },
  { A6, 7 },
  { A7, 8 },
  { A8, 9 },
  { A9, 10 },
  { A10, 11 },
  { A11, 12 },
  { A12, 13 },
  { A13, 14 },
  { CKE3, 15 },
  { CKE1, 16 },
  { ACT_N, 17 },
  { BG1, 18 },
  { BG0, 19 },
  { CKE2, 20 },
  { CKE0, 21 }
};

PinRegister PinRegisterSprSb[] = {
  { ODT2, 0 },
  { A14, 1 },
  { WE_N, 1},  // Mux with A14
  { A15, 2 },
  { CAS_N, 2}, // Mux with A15
  { A16, 3 },
  { RAS_N, 3}, // Mux with A16
  { A17, 4 },
  { BA0, 5 },
  { BA1, 6 },
//not used  { Spare, 7 },
  { CS2_N, 8 },
  { CS1_N, 9 },
  { ODT0, 10 },
  { CS3_N, 11 },
  { CS0_N, 12 },
  { C2, 13 },
 //not used { Spare, 14 },
  { ODT3, 15 },
  { ODT1, 16 },
  { PAR, 17 },
  { CS5_N, 18 },
  { CS4_N, 19 },
  { CS7_N, 20 },
  { CS6_N, 21 }
};

/**
  Gets the silicon register index from the signal name

  @param[in]     Host:      Pointer to SYSHOST
  @param[in]     Socket:    Socket number
  @param[in]     Ch:        Channel number
  @param[in]     Signal:    Enumerated signal name
  @param[in,out] SubCh:     Sub Channel Number
  @param[in,out] RegIndex:  Register Index

  @retval MRC_STATUS

**/
MRC_STATUS
SignalToRegIndex (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN GSM_CSN               Signal,
  IN OUT UINT8             *SubCh,
  IN OUT UINT8             *RegIndex
  )
{
  UINT8        SigLoop;
  UINT8        MaxSignals;
  PinRegister  *PinSignal = NULL;
  MRC_STATUS   Status;
  UINT8        SocketId;

  SocketId = GetSysSocketId ();

  if (((Signal >= A0) && (Signal <= A13)) ||
      ((Signal >= CKE0) && (Signal <= CKE3)) ||
      ((Signal == BG0) || (Signal == BG1)) ||
      (Signal == ALERT_N) || (Signal == ACT_N)) {
    *SubCh = SUB_CH_A;
    GetPinRegisterValue (Host, Socket, Ch, SUB_CH_A, &PinSignal, &MaxSignals);
  } else {
    GetPinRegisterValue (Host, Socket, Ch, SUB_CH_B, &PinSignal, &MaxSignals);
    *SubCh = SUB_CH_B;
  }

  for (SigLoop = 0; SigLoop < MaxSignals; SigLoop++) {
    if (PinSignal[SigLoop].Signal == Signal) {
      *RegIndex = PinSignal[SigLoop].RegisterIndex;
      break;
    }
  } // sigLoop loop

  if (SigLoop == MaxSignals) {
#ifdef DEBUG_CODE_BLOCK
    RcDebugPrint (SDBG_MAX,
                    "Unable to find signal %a\n", GetSignalStr (Signal));
#endif
    Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  } else {
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;
} // SignalToRegIndex

/**
  Get the Pin Register structure for given Side

  @param[in]  Host         Pointer to SYSHOST
  @param[in]  Socket       Socket number
  @param[in]  Ch           Channel number
  @param[in]  SubCh        Either SUB_CH_A or SUB_CH_B
  @param[out] Signal       Pointer to the Signal Structure (If SignalType is valid)
  @param[out] Size         Size of structure (If SignalType is valid)

  retval MRC_SUCCESS - Value returned successfully
  retval MRC_STATUS_FAILURE - Failed to return structure pointer(Wrong Input parameter)
**/
MRC_STATUS
GetPinRegisterValue (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN  PinSubCh      SubCh,
  OUT PinRegister   **Signal,
  OUT UINT8         *Size
  )
{
  switch (SubCh) {

  case PinSubChA:
    *Signal = PinRegisterSprSa;
    *Size = sizeof (PinRegisterSprSa) / sizeof (PinRegister);
  break;

  case PinSubChB:
    *Signal = PinRegisterSprSb;
    *Size = sizeof (PinRegisterSprSb) / sizeof (PinRegister);
  break;

  default:
    return MRC_STATUS_FAILURE;
  }
  return MRC_STATUS_SUCCESS;
}

/**

  This function updates sub-channel number,
  taking DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number

  @retval CC Register Address

**/
UINT8
EFIAPI
UpdateDdrIoCcRegisterSubCh (
 IN       PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh
  )
{

  UINT8     SubChannel; // This is the sub-channel number for register access.

  if (IsLaneReversalEnabled(Host, Socket, Ch) == TRUE) {
    //
    // Lane reversal enabled. Swap Sub Channel A and Sub Channel B.
    //
    if (SubCh == SUB_CH_A) {
      SubChannel = SUB_CH_B;
    } else {
      SubChannel = SUB_CH_A;
    }
  } else {
    //
    // Lane reversal disabled. Do not swap Sub Channel A and Sub Channel B.
    //
    SubChannel = SubCh;
  }

  return SubCh;
}