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

#include <RcRegs.h>
#include "IioEvDebug.h"
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <LibraryPrivate/IioDebug.h>

const struct iio_monbusParameter  EvParameterList[] = {
    {EVMONBUS,0x3D,11,13},     // 0 AFE SUMR OC
    {EVMONBUS,0x3C,0,3},     // 1 RVREF
    {EVMONBUS,0x3B,0,13},     // 2 CTOC
    {EVMONBUS,0x3A,0,3},     // 3 OC START
    {EVMONBUS,0x39,4,5},     // 4 OC DONE
    {EVMONBUS,0x37,0,5},     // 5 SUMR
    {EVMONBUS,0x36,0,7},     // 6 ESM1
    {EVMONBUS,0x35,0,7},     // 7 ESM0
    {EVMONBUS,0x34,0,7},     // 8 ESP1
    {EVMONBUS,0x33,0,7},     // 9 ESP0
    {EVMONBUS,0x32,0,7},     // 10 DS1
    {EVMONBUS,0x31,0,7},     // 11 DS0
    {EVMONBUS,0x30,0,4},     // 12 SQLCH
    {EVMONBUS,0x2D,0,13},     // 13 VREF ADAPT CTRL
    {EVMONBUS,0x2C,7,13},     // 14 DFE1 STEP ACC
    {EVMONBUS,0x2B,8,13},     // 15 DFE2 STEP ACC
    {EVMONBUS,0x2A,9,13},     // 16 DFE3 STEP ACC
    {EVMONBUS,0x29,9,13},     // 17 DFE4 STEP ACC
    {EVMONBUS,0x28,7,12},     // 18 DFE1
    {EVMONBUS,0x27,8,13},     // 19 DFE2
    {EVMONBUS,0x26,9,13},     // 20 DFE3
    {EVMONBUS,0x25,9,13},     // 21 DFE4
    {EVMONBUS,0x24,4,13},     // 22 AGC STEP
    {EVMONBUS,0x23,9,13},     // 23 AGC
    {EVMONBUS,0x22,7,13},     // 24 CDR STEP
    {EVMONBUS,0x21,7,13},     // 25 CDR
    {EVMONBUS,0x20,7,13},     // 26 CDR_ACC
    {EVMONBUS,0x19,7,13},     // 27 RXDCC
    {EVMONBUS,0x03,8,13},     // 28 ICOMP
    {EVMONBUS,0x01,8,13},     // 29 TX ICOMP OVRD
    {EVTRACEBUS,0x03,16,21},     // 30 ICOMP
    {EVTRACEBUS,0x21,25,31},     // 31 CDR
    {EVTRACEBUS,0x22,21,25},     // 32 AGC
    {EVTRACEBUS,0x23,25,30},     // 33 DFE1
    {EVTRACEBUS,0x24,26,31},     // 34 DFE2
    {EVTRACEBUS,0x25,27,31},     // 35 DFE3
    {EVTRACEBUS,0x26,27,31},     // 36 DFE4
    {EVTRACEBUS,0x27,24,31},     // 37 CTOC1
    {EVTRACEBUS,0x27,11,16},     // 38 CTOC2
    {EVTRACEBUS,0x3D,11,17},     // 39 TXDCC
    {EVTRACEBUS,0x3D,25,31},     // 40 RX0DCC
    {EVTRACEBUS,0x3D,18,24},     // 41 RX1DCC
};
const UINT32 PortLinkSelect[] = { 0, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3};

/**
    This routine will dump Arc state of the link.

    @param[in] L0ExitArc - L0 Exit Arc value.

    @return String describing L0ExitArc is returned.
**/
CONST CHAR8*
DecodeLtssmArc (
  IN  UINT32       L0ExitArc
  )
{
    static CHAR8 *StrPrint[] = {"RxL0s",
                                "REC",
                                "FERR",
                                "REDOEQ",
                                "PSMI",
                                "PROLOG",
                                "SENT",
                                "SLD",
                                "UNKNOWN"};

    if (L0ExitArc >= NELEMENTS (StrPrint)) {
        L0ExitArc  = NELEMENTS (StrPrint) - 1;
    }

    return StrPrint[L0ExitArc];
}

/**
    This routine converts LTSSM state & substate to a textual form.

    @param[in]  State          - Value of the Main LTSSM State.
    @param[in]  SubState       - Value of the Sub LTSSM State.
    @param[out] StateStrPtr    - Pointer to string of the Main LTSSM State.
    @param[out] SubstateStrPtr - Pointer to string of the Sub LTSSM State.
**/
VOID
DecodeLtssm (
    IN  UINT32      State,
    IN  UINT32      Substate,
    OUT CHAR8     **StateStrPtr,
    OUT CHAR8     **SubstateStrPtr
    )
{
    static CHAR8 *StateNames[] = {"DETECT.",
                                  "POLLING/L0_EXT.",
                                  "CONFIG.",
                                  "L0.",
                                  "RECOVERY.",
                                  "LOOPBACK.",
                                  "HOTRESET/PREP_DET.",
                                  "DISABLED.",
                                  "EQRECOVERY.",
                                  "UNKNOWN."};

    static CHAR8 *State0SubstateNames[] = {"QUIET_RST",
                                           "QUIET_ENTER",
                                           "QUIET",
                                           "ACT_128US",
                                           "ACTIVE",
                                           "ACT2_128US",
                                           "POL",
                                           "EXIT_SQUELCH",
                                           "RST_ASSERT",
                                           "RECOMBINE",
                                           "TRANS_GEN1",
                                           "STGR",
                                           "SEND_TS1_POLL",
                                           "UNKNOWN"};

    static CHAR8 *State1SubstateNames[] = {"ACTIVE",
                                           "CONFIG",
                                           "COMP_G1/POL_COMPLIANCE",
                                           "Z_NO_EIOS",
                                           "COMP_G2_ENTRY",
                                           "COMP_G2_PREP",
                                           "COMP_G2",
                                           "PDP",
                                           "COMP_G2_EXIT",
                                           "COMP_G2_WAIT",
                                           "COMP_G2_PREP_EXIT",
                                           "COMP_G1_EIOS",
                                           "COMP_G2_IDLE_WAIT",
                                           "CHECK_COMPL",
                                           "SQ_EXIT",
                                           "UNKNOWN"};

    static CHAR8 *State2SubstateNames[] = {"LNKWID_START",
                                           "LNKWID_ACCEPT",
                                           "LANENUM_WAIT",
                                           "LANENUM_ACCEPT",
                                           "COMPLETE",
                                           "IDLE",
                                           "LNKWID_START_REC",
                                           "IDLE_OLD",
                                           "LWS_WAIT_FOR_TS",
                                           "LNKWID_START_UPCFG",
                                           "LNKWID_START_UPCFG_PREP_EXIT_MASTER",
                                           "LNKWID_START_UPCFG_PREP_EXIT_SLAVE ",
                                           "LWS_WAIT4_BITLOCK",
                                           "UNKNOWN"};

    static CHAR8 *State3SubstateNames[] = {"L0",
                                           "TXL0S_WAIT4EIOS",
                                           "TXL0S_IDLE",
                                           "TXL0S_PREP_EXIT",
                                           "L1_EXIT",
                                           "L1_IDLE",
                                           "L2_IDLE",
                                           "L1L2_ENTRY",
                                           "TXL0S_FTS",
                                           "L2_IDLE_WAIT",
                                           "UPSTREAM_L1L2_EIOS",
                                           "L1_IDLE_WAIT",
                                           "UNKNOWN"};

    static CHAR8 *State4SubstateNames[] = {"COMPLETE",
                                           "RCVRCFG",
                                           "RCVRLOCK",
                                           "IDLE",
                                           "SPEED",
                                           "RCVRCFG_SPEED",
                                           "WAIT_FOR_GEN_TRANS",
                                           "W4RX2COMPLETE",
                                           "SPEED_IDLE",
                                           "SPEED_WAIT_1US_PREP",
                                           "SPEED_WAIT_1US",
                                           "WAIT_EXIT_EI",
                                           "WAIT_BITLOCK_TO",
                                           "UNKNOWN"};

    static CHAR8 *State5SubstateNames[] = {"ENTRY_S",
                                           "ACTIVE_S",
                                           "EXIT_S",
                                           "EXIT1",
                                           "ENTRY_M",
                                           "ACTIVE_M",
                                           "EXIT_M",
                                           "SPEED_CHANGE",
                                           "SEND_EIOS",
                                           "SPEED_PREP_EXIT",
                                           "SPEED_IDLE_WAIT",
                                           "UNKNOWN"};

    static CHAR8 *State6SubstateNames[] = {"HR_ENTRY/HOTRESET",
                                           "MAS",
                                           "SLV",
                                           "TS1",
                                           "TS2",
                                           "EIOS",
                                           "UNKNOWN"};

    static CHAR8 *State7SubstateNames[] = {"TS1",
                                           "EIOS",
                                           "EI",
                                           "SUCC",
                                           "SUCC_WAIT",
                                           "UNKNOWN"};

    static CHAR8 *State8SubstateNames[] = {"PH0",
                                           "PH1",
                                           "ADAPTMYTX",
                                           "ADAPTMYRXS1_W4REQALLLANES",
                                           "ADAPTMYRXS2_W4500NSTO",
                                           "ADAPTMYRXS3_W464KUITO",
                                           "ADAPTMYRXS4_W416KUITO",
                                           "ADAPTMYRXS5_CHK4DONEALLLANES",
                                           "ADAPTMYRXS6_W4FINALRXTRAIN",
                                           "ADAPTMYRXS7_FINALRXTRAINTO",
                                           "UNKNOWN"};

    if (State >= NELEMENTS (StateNames)) {
        State  = NELEMENTS (StateNames) - 1;
    }
    *StateStrPtr = StateNames[State];

    switch (State) {
    case 0:
        if (Substate >= NELEMENTS (State0SubstateNames)) {
            Substate  = NELEMENTS (State0SubstateNames) - 1;
        }
        *SubstateStrPtr = State0SubstateNames[Substate];
        break;
    case 1:
        if (Substate >= NELEMENTS (State1SubstateNames)) {
            Substate  = NELEMENTS (State1SubstateNames) - 1;
        }
        *SubstateStrPtr = State1SubstateNames[Substate];
        break;
    case 2:
        if (Substate >= NELEMENTS (State2SubstateNames)) {
            Substate  = NELEMENTS (State2SubstateNames) - 1;
        }
        *SubstateStrPtr = State2SubstateNames[Substate];
        break;
    case 3:
        if (Substate >= NELEMENTS (State3SubstateNames)) {
            Substate  = NELEMENTS (State3SubstateNames) - 1;
        }
        *SubstateStrPtr = State3SubstateNames[Substate];
        break;
    case 4:
        if (Substate >= NELEMENTS (State4SubstateNames)) {
            Substate  = NELEMENTS (State4SubstateNames) - 1;
        }
        *SubstateStrPtr = State4SubstateNames[Substate];
        break;
    case 5:
        if (Substate >= NELEMENTS (State5SubstateNames)) {
            Substate  = NELEMENTS (State5SubstateNames) - 1;
        }
        *SubstateStrPtr = State5SubstateNames[Substate];
        break;
    case 6:
        if (Substate >= NELEMENTS (State6SubstateNames)) {
            Substate  = NELEMENTS (State6SubstateNames) - 1;
        }
        *SubstateStrPtr = State6SubstateNames[Substate];
        break;
    case 7:
        if (Substate >= NELEMENTS (State7SubstateNames)) {
            Substate  = NELEMENTS (State7SubstateNames) - 1;
        }
        *SubstateStrPtr = State7SubstateNames[Substate];
        break;
    case 8:
        if (Substate >= NELEMENTS (State8SubstateNames)) {
            Substate  = NELEMENTS (State8SubstateNames) - 1;
        }
        *SubstateStrPtr = State8SubstateNames[Substate];
        break;
    default:
        break;
    }
}
