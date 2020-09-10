/** @file
  DdrioPinMap.c
  DDRIO Pin map for SPR DDR5

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

//
// DDR5 signal to CR register mapping when lane reversal is disabled.
//

//
// SUB_CH_A
//
PinRegister PinRegisterSprSaNormal[] = {
  { ALERT_N_DDR5, 0 },
  { A0A, 1 },
  { A1A, 2 },
  { A2A, 3 },
  { A3A, 4 },
  { A4A, 5 },
  { A5A, 6 },
  { A6A, 7 },
  { A7A, 8 },
  { A8A, 9 },
  { A9A, 10 },
  { A10A, 11 },
  { A11A, 12 },
  { A12A, 13 },
  { A13A, 14 },
  { RSPB_SA, 15 },
  { RSPA_SA, 16 },
  { PARA, 17 },
  { CS1_A_N, 18 },
  { CS0_A_N, 19 },
  { CS3_A_N, 20 },
  { CS2_A_N, 21 }
};

//
// SUB_CH_B
//
PinRegister PinRegisterSprSbNormal[] = {
  { A0B, 1 },
  { A1B, 2 },
  { A2B, 3 },
  { A3B, 4 },
  { A4B, 5 },
  { A5B, 6 },
  { A6B, 7 },
  { A7B, 8 },
  { A8B, 9 },
  { A9B, 10 },
  { A10B, 11 },
  { A11B, 12 },
  { A12B, 13 },
  { A13B, 14 },
  { RSPB_SB, 15 },
  { RSPA_SB, 16 },
  { PARB, 17 },
  { CS1_B_N, 18 },
  { CS0_B_N, 19 },
  { CS3_B_N, 20 },
  { CS2_B_N, 21 }
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

  //
  //  Choose subchannel assuming no lane reversal.
  //
  if (((Signal >= A0A) && (Signal <= PARA)) ||
      ((Signal >= CS0_A_N) && (Signal <= CS3_A_N)) ||
      ((Signal >= RSPA_SA) && (Signal <= RSPB_SA)) ||
      (Signal == ALERT_N_DDR5)) {

    *SubCh = SUB_CH_A;
  } else {
    *SubCh = SUB_CH_B;
  }

  GetPinRegisterValue (Host, Socket, Ch, *SubCh, &PinSignal, &MaxSignals);

  for (SigLoop = 0; SigLoop < MaxSignals; SigLoop++) {
    if (PinSignal[SigLoop].Signal == Signal) {
      *RegIndex = PinSignal[SigLoop].RegisterIndex;
      break;
    }
  } // sigLoop loop

  if (SigLoop == MaxSignals) {
#ifdef DEBUG_CODE_BLOCK
    RcDebugPrint (SDBG_MAX, "Unable to find signal %a\n", GetSignalStr (Signal));
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

    *Signal = PinRegisterSprSaNormal;
    *Size = sizeof (PinRegisterSprSaNormal) / sizeof (PinRegister);

  break;

  case PinSubChB:
    *Signal = PinRegisterSprSbNormal;
    *Size = sizeof (PinRegisterSprSbNormal) / sizeof (PinRegister);

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

  return SubChannel;
}