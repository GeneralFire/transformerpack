/** @file
  MrcCommonTypes.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#ifndef _MrcCommonTypes_h_
#define _MrcCommonTypes_h_

#include <MemCommon.h>



///
/// DIMM memory package
///
typedef enum {
  RDimmMemoryPackage      = 1,
  UDimmMemoryPackage      = 2,
  SoDimmMemoryPackage     = 3,
  MicroDimmMemoryPackage  = 4,
  MiniRDimmMemoryPackage  = 5,
  MiniUDimmMemoryPackage  = 6,
  MiniCDimmMemoryPackage  = 7,
  SoUDimmEccMemoryPackage = 8,
  SoRDimmEccMemoryPackage = 9,
  SoCDimmEccMemoryPackage = 10,
  LrDimmMemoryPackage     = 11,
  SoDimm16bMemoryPackage  = 12,
  SoDimm32bMemoryPackage  = 13,
  MemoryPackageMax,                       ///< MEMORY_PACKAGE enumeration maximum value.
  MemoryPackageDelim = MAX_INT32          ///< This value ensures the enum size is consistent on both sides of the PPI.
} MEMORY_PACKAGE;

typedef struct {
  MRC_MST       MemSsType;
  UINT32        NumDataCh;      // Total number of physical data channels in the MemSS
  UINT32        NumDataIoFubsPerCh;  // Total number of IO fubs in a data channel
  UINT32        NumDataIoFubsPerSubCh;  // Total number of IO fubs in a data sub channel
  UINT32        NumDqLanesPerCh;  // Number of active DQ lanes in a data channel (bus width)
} MRC_MSM;

typedef enum {
  DdrLevel = 0,                         ///< Refers to frontside of DIMM
  LrbufLevel = 1,                         ///< Refers to data level at backside of LRDIMM or NVMDIMM buffer
  RegALevel = 2,                         ///< Refers to cmd level at backside of register, side A
  RegBLevel = 3,                         ///< Refers to cmd level at backside of register, side B
  HbmLevel = 4,                         ///< Refers to HBM
  MrcLtMax,
  MrcLtDelim = MAX_INT32
  } MRC_LT;

///
/// Memory training margin group selectors.
///
typedef enum {
  RecEnDelay       = 0,                   ///< Linear delay (PI ticks), where the positive increment moves the RCVEN sampling window later in time relative to the RX DQS strobes.
  RxDqsDelay       = 1,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe later in time relative to the RX DQ signal (i.e. toward the hold side of the eye).
  RxDqDelay        = 2,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQ byte/nibble/bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxDqsPDelay      = 3,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "even" chunks later in time relative to the RX DQ signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNDelay      = 4,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "odd" chunks later in time relative to the RX DQ signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxVref           = 5,                   ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane RX Vref to a higher voltage.
  RxEq             = 6,                   ///< RX CTLE setting indicating a set of possible resistances, capacitance, current steering, etc. values, which may be a different set of values per product. The setting combinations are indexed by integer values.
  RxDqBitDelay     = 7,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQ bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxVoc            = 8,                   ///< Monotonic increment (Sense Amp setting), where the positive increment moves the byte/nibble/bitlane's effective switching point to a lower Vref value.
  RxOdt            = 9,                   ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtUp          = 10,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtDn          = 11,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  DramDrvStr       = 12,                  ///< Drive strength setting resistance setting within a set of possible resistances (or currents), which may be a different set of values per product. Indexed by integer values.
  McOdtDelay       = 13,                  ///<
  McOdtDuration    = 14,                  ///<
  SenseAmpDelay    = 15,                  ///<
  SenseAmpDuration = 16,                  ///<
  RoundTripDelay   = 17,                  ///< Relative time from CMD to data valid in RX FIFO, in increments of 1 DQ UI (logic delays).
  RxDqsBitDelay    = 18,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS within the bitlane later in time relative to the RX DQ signal (i.e.closing the gap between DQ and DQS in the hold side of the eye).
  RxDqDqsDelay     = 19,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS per strobe later in time relative to the RX DQ signal (i.e. closing the gap between DQS and DQ in the hold side of the eye. The difference between this parameter and RxDqsDelay is that both the DQ and DQS timings may be moved in order to increase the total range of DQDQS timings.

  WrLvlDelay       = 20,                  ///< Linear delay (PI ticks), where the positive increment moves both the TX DQS and TX DQ signals later in time relative to all other bus signals.
  TxDqsDelay       = 21,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQS strobe later in time relative to all other bus signals.
  TxDqDelay        = 22,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQ byte/nibble/bitlane later in time relative to all other bus signals.
  TxVref           = 23,                  ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane TX Vref to a higher voltage. (Assuming this will abstract away from the range specifics for DDR4, for example.)
  TxEq             = 24,                  ///< TX EQ setting indicating a set of possible equalization levels, which may be a different set of values per product. The setting combinations are indexed by integer values.
  TxDqBitDelay     = 25,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQ bitlane later in time relative to all other bus signals.
  TxRon            = 26,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonUp          = 27,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonDn          = 28,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxSlewRate       = 29,                  ///< Monotonic increment, where the positive increment moves the byte/nibble/bitlane's effective slew rate to a higher slope.
  TxImode          = 30,                  ///< TX I-Mode Boost setting indicating a set of possible current boost levels, which may be a different set of values per product. The setting combinations are indexed by integer values.
  WrOdt            = 31,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  NomOdt           = 32,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  ParkOdt          = 33,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxTco            = 34,                  ///<
  TxRiseFallSlewRate = 35,                ///< TX RISE FALL SLEW RATE setting
  RxCtleR          = 36,                  ///< RX CTLE setting indicating a set of possible resistance values, which may be a different set of values per product. The setting combinations are indexed by integer values.
  RxCtleC          = 37,                  ///< RX CTLE setting indicating a set of possible capacitance values, which may be a different set of values per product. The setting combinations are indexed by integer values.
  RxDqsPBitDelay   = 38,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "even" chunks later in time relative to the RX DQ bitlane signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNBitDelay   = 39,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "odd" chunks later in time relative to the RX DQ bitlane signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  ClientCmdAll     = 40,
  ClientCmdGrp0    = 41,
  ClientCmdGrp1    = 42,
  ClientCmdGrp2    = 43,
  CtlAll           = 44,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_ALL category later in time relative to all other signals on the bus.
  CtlGrp0          = 45,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP0 category later in time relative to all other signals on the bus.
  CtlGrp1          = 46,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP1 category later in time relative to all other signals on the bus.
  CtlGrp2          = 47,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP2 category later in time relative to all other signals on the bus.
  CtlGrp3          = 48,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP3 category later in time relative to all other signals on the bus.
  CtlGrp4          = 49,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP4 category later in time relative to all other signals on the bus.
  CtlGrp5          = 50,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP5 category later in time relative to all other signals on the bus.
  CmdCtlAll        = 51,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_CTL_ALL category later in time relative to all other signals on the bus.
  CkAll            = 52,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CK_ALL category later in time relative to all other signals on the bus.
  CmdVref          = 53,                  ///< Linear increment (Vref ticks), where the positive increment moves the CMD Vref to a higher voltage.
  AlertVref        = 54,                  ///< Linear increment (Vref ticks), where the positive increment moves the ALERT Vref to a higher voltage.
  CmdRon           = 55,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  CS               = 56,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  CA               = 57,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  QCS              = 58,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  QCA              = 59,                  ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  EridDelay        = 60,                  ///< Linear delay (PI ticks), where the positive increment moves the ERID signals later in time relative to the internal sampling clock (i.e.closing the gap between ERID and internal sampling clock in the setup side of the eye). This group is applicable for DDRT DIMMs.
  EridVref         = 61,                  ///< Linear increment (Vref ticks), where the positive increment moves the ERID Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ErrorVref        = 62,                  ///< Linear increment (Vref ticks), where the positive increment moves the ERROR Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ReqVref          = 63,                  ///< Linear increment (Vref ticks), where the positive increment moves the REQ Vref to a higher voltage. This group is applicable for DDRT DIMMs.

  RecEnDelayCycle  = 67,
  TxDqsDelayCycle  = 68,

  CmdAll           = 70,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdAll category later in time relative to all other signals on the bus.
  CmdGrp0          = 71,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp0 category later in time relative to all other signals on the bus.
  CmdGrp1          = 72,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp1 category later in time relative to all other signals on the bus.
  CmdGrp2          = 73,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp3          = 74,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp4          = 75,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp5          = 76,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp6          = 77,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp7          = 78,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp8          = 79,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp9          = 80,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp10         = 81,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp11         = 82,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp12         = 83,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp13         = 84,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp14         = 85,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp15         = 86,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp16         = 87,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp17         = 88,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp18         = 89,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp19         = 90,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
  CmdGrp20         = 91,                  ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CmdGrp2 category later in time relative to all other signals on the bus.
 // HBM
  TxDsPu           = 92,                  ///< Drive strength setting for pull up legs that are turned on.
  TxDsPd           = 93,                  ///< Drive strength setting for pull down legs that are turned on.
  CmdDsPu          = 94,                  ///< Drive strength setting for pull up legs that are turned on.
  CmdDsPd          = 95,                  ///< Drive strength setting for pull down legs that are turned on.
  CmdTco           = 96,                  ///< TX Duty Cycle correction
  CkTco            = 97,                  ///< TX Duty Cycle correction
                                          ///
  TxFallSlewRate   = 98,                  ///< TX FALL SLEW RATE setting
  CmdTxEq          = 99,                  ///< CMD TX EQ
  Cmd0All          = 100,                 ///< Subchannel  0 command address signals
  Cmd1All          = 101,                 ///< Subchannel  1 command address signals
  Ctl0Grp          = 102,                 ///< Subchannel  0 CS# indexed by dimm, rank
  Ctl1Grp          = 103,                 ///< Subchannel  1 CS# indexed by dimm, rank
  PxcDelta         = 104,                 ///< TX XTalk delta setting
  TxEqTap2         = 105,                 ///< TX EQ extra tap setting indicating a set of possible equalization levels, which may be a different set of values per product.
  PxcControl       = 106,                 ///< TX XTalk enable/disable
  DdjcDelta        = 107,                 ///< DDJ Cancellation delta setting
  RxDfeCoefficient = 108,                 ///< Rx DFE Coefficient setting
  RxStrobeInvert   = 109,                 ///< Rx Strobe Inversion
  RxDfe            = 110,                 ///< General Rx DFE group
  TxFlyBy          = 111,                 ///< Write fly-by delay
  RxFlyBy          = 112,                 ///< Read fly-by delay
  RxSenseAmpOffset = 113,                 ///< Rx Sense Amp Offset
  RxFifoDqsPBitSkew = 114,                ///< selects which skewed DQSP (to sample even bits) is used in the RX FIFO.
  RxFifoDqsNBitSkew = 115,                ///< selects which skewed DQSN (to sample odd bits) is used in the RX FIFO.
  RxDqsPerChDelayOffset = 116,            ///< RxDqs delay offset multicast (1 per channel) - offset is added to unicast RxDqs delays
  TxDqPerChDelayOffset  = 117,            ///< TxDq delay offset multicast (1 per channel) - offset is added to unicast TxDq delays
  DramRxEq          = 118,                ///< DRAM Rx Eq / CTLE setting
  PhysicalDelay     = 119,                ///< Force Physical Swizzling
  LogicalDelay      = 120,                ///< Force Logical Swizzling
  RxBias            = 121,
  TxEchoCoef        = 122,                ///< Tx Echo Cancellation Coef
  TxEchoUiSel       = 123,                ///< Tx Echo Ui Select
  RxDfeCode         = 124,                 ///< Rx Dfe Code value
  RxDfeDelaySel     = 125,                 ///< Rx Dfe delay Select
  Req               = 126,
  RttNomWr          = 127,                ///< DDR5 DRAM ODT RTT_NOM_WR
  RttNomRd          = 128,                ///< DDR5 DRAM ODT RTT_NOM_RD
  RttWr             = 129,                ///< DDR5 DRAM ODT RTT_WR
  RttPark           = 130,                ///< DDR5 DRAM ODT RTT_PARK
  DqsRttPark        = 131,                ///< DDR5 DRAM ODT DQS_RTT_PARK
  DramRonPullUp     = 132,                ///< DDR5 DRAM Ron Pull Up
  DramRonPullDown   = 133,                ///< DDR5 DRAM Ron Pull Down
  RttCkGroupA       = 134,
  RttCkGroupB       = 135,
  RttCsGroupA       = 136,
  RttCsGroupB       = 137,
  RttCaGroupA       = 138,
  RttCaGroupB       = 139,
  DbDfeTap1         = 140,                ///< DB DFE coefficient Tap 1
  DbDfeTap2         = 141,                ///< DB DFE coefficient Tap 2
  DbDfeTap3         = 142,                ///< DB DFE coefficient Tap 3
  DbDfeTap4         = 143,                ///< DB DFE coefficient Tap 4
  DbDfeTapAll       = 158,                ///< DB DFE All Taps
  DbDfe             = 159,                ///< DB DFE
  TxDfe             = 160,                ///< Tx DFE
  TxDfeTapAll       = 161,                ///< Tx DFE All Taps.
  TxDfeTap1         = 162,                ///< Tx DFE Tap 1
  TxDfeTap2         = 163,                ///< Tx DFE Tap 2
  TxDfeTap3         = 164,                ///< Tx DFE Tap 3
  TxDfeTap4         = 165,                ///< Tx DFE Tap 4

  RxDfeTapAll       = 166,                ///< Tx DFE All Taps.
  RxDfeTap1         = 167,                ///< Tx DFE Tap 1
  RxDfeTap2         = 168,                ///< Tx DFE Tap 2
  RxDfeTap3         = 169,                ///< Tx DFE Tap 3
  RxDfeTap4         = 170,                ///< Tx DFE Tap 4
  RxSampler         = 171,                ///< Rx Sampler Offset
  RxSamplerEvenOdd  = 172,                ///< Rx Sampler Even and Odd (Per Bit)
  RxSamplerEven     = 173,                ///< Rx Sampler Even (Per Bit)
  RxSamplerOdd      = 174,                ///< Rx Sampler Odd (Per Bit)

  DcaDfe            = 175,                ///< General Dca DFE group
  DcaDfeTapAll      = 176,                ///< Dca DFE All Taps.
  DcaDfeTap1        = 177,                ///< Dca DFE Tap 1
  DcaDfeTap2        = 178,                ///< Dca DFE Tap 2
  DcaDfeTap3        = 179,                ///< Dca DFE Tap 3
  DcaDfeTap4        = 180,                ///< Dca DFE Tap 4
  // available      = 181,                ///<
  TxDqDelayEven     = 182,                ///< Tx Even UI
  TxDqDelayOdd      = 183,                ///< Tx Odd UI
  TxDqPieOffset     = 184,                ///< Tx Dq Pie Offset
  TxDqsPieOffset    = 185,                ///< Tx Dqs Pie Offset
  DcaTco            = 186,                ///< Dca Tco Control, adjust the duty cycle of command/address
  LinearCntlDcaTco  = 187,                ///< Dca Tco Control, Normalized to 0 with monotonically increasing discrete steps
  DcaDelay          = 188,
  DcaDfeVref        = 189,                ///< DCA monitor DFE Vref
  RcdDcsVref        = 190,                ///< DCS monitor Vref
  RcdDcaVref        = 191,                ///< DCA monitor Vref
  TxDqFallSlewRate  = 192,
  TxDqRiseSlewRate  = 193,
  DcsDelay          = 194,

  RxSummer          = 195,                ///< Rx Summer
  RcdCmdAll         = 196,                ///< CMD signals group with RCD
  TcoDqDqs          = 197,                ///< Tco Dq/Dqs Control, adjust the duty cycle of data lines
  LinearCntlTcoDqDqs = 198,               ///< Tco Dq/Dqs Control, Normalized to 0 with monotonically increasing discrete steps

  DramDcaClk        = 208,                ///< DRAM Duty Cycle Adjuster for single/two-phase internal clock(s)
  DramDcaQClk       = 209,                ///< DRAM Duty Cycle Adjuster for QCLK in 4-phase internal clocks
  DramDcaIbClk      = 210,                ///< DRAM Duty Cycle Adjuster for IBCLK in 4-phase internal clocks
  DramDcaQbClk      = 211,                ///< DRAM Duty Cycle Adjuster for QBCLK in 4-phase internal clocks
  TxDqScomp         = 212,
  RxDfeCoefficientTap1PerBit = 200,
  RxDfeCoefficientTap2PerBit = 201,
  RxDfeCoefficientTap3PerBit = 202,
  RxDfeCoefficientTap4PerBit = 203,
  RxDfeCoefficientTap1 = 204,
  RxDfeCoefficientTap2 = 205,
  RxDfeCoefficientTap3 = 206,
  RxDfeCoefficientTap4 = 207,
  DbDfeVrefGroup    = 213,                ///< DB DFE VREF

  RxVrefPerChDelayOffset     = 214,            ///< RxVref delay offset multicast (1 per channel) - offset is added to unicast RxVref delays
  RxSamplerPerChDelayOffset  = 215,            ///< RxSampler delay offset multicast (1 per channel) - offset is added to unicast RxSampler delays
  DramCsVref        = 216,                ///< DRAM Vref CS
  DramCaVref        = 217,                ///< DRAM Vref CA

  QcaDelay          = 218,
  QcsDelay          = 219,

  RxCoarseDelay     = 220,                ///< RoundTripLatency, IoLatency and IoCompLatency group.
  BcomDelay         = 221,
  DbHostRonPullUp   = 222,                ///< DDR5 LRDIMM Host Ron Pull Up
  DbHostRonPullDown = 223,                ///< DDR5 LRDIMM Host Ron Pull Down

  DbDfeVrefDq0      = 224,                /// DB DFE TX VREF DQ0
  DbDfeVrefDq1      = 225,                /// DB DFE TX VREF DQ1
  DbDfeVrefDq2      = 226,                /// DB DFE TX VREF DQ2
  DbDfeVrefDq3      = 227,                /// DB DFE TX VREF DQ3
  DbDfeVrefDq4      = 228,                /// DB DFE TX VREF DQ4
  DbDfeVrefDq5      = 229,                /// DB DFE TX VREF DQ5
  DbDfeVrefDq6      = 230,                /// DB DFE TX VREF DQ6
  DbDfeVrefDq7      = 231,                /// DB DFE TX VREF DQ7
  BcomVref          = 232,
  MrcGtMax,
  MrcGtDelim = MAX_INT32
  } MRC_GT;

//
// Number of Legacy RxDfe Parameters
//
#define LEGACY_RXDFE_PARAM_COUNT  2

typedef enum {
  SigRasN = 0,
  SigCasN = 1,
  SigWeN = 2,
  SigBa0 = 3,
  SigBa1 = 4,
  SigBa2 = 5,
  SigA0 = 6,
  SigA1 = 7,
  SigA2 = 8,
  SigA3 = 9,
  SigA4 = 10,
  SigA5 = 11,
  SigA6 = 12,
  SigA7 = 13,
  SigA8 = 14,
  SigA9 = 15,
  SigA10 = 16,
  SigA11 = 17,
  SigA12 = 18,
  SigA13 = 19,
  SigA14 = 20,
  SigA15 = 21,
  SigA16 = 22,
  SigA17 = 23,
  SigCs0N = 24,
  SigCs1N = 25,
  SigCs2N = 26,
  SigCs3N = 27,
  SigCs4N = 28,
  SigCs5N = 29,
  SigCs6N = 30,
  SigCs7N = 31,
  SigCs8N = 32,
  SigCs9N = 33,
  SigCke0 = 34,
  SigCke1 = 35,
  SigCke2 = 36,
  SigCke3 = 37,
  SigCke4 = 38,
  SigCke5 = 39,
  SigOdt0 = 40,     //could also be used for CA-ODT for LP4
  SigOdt1 = 41,     //could also be used for CA-ODT for LP4
  SigOdt2 = 42,
  SigOdt3 = 43,
  SigOdt4 = 44,
  SigOdt5 = 45,
  SigPar = 46,
  SigAlertN = 47,
  SigBg0 = 48,
  SigBg1 = 49,
  SigActN = 50,
  SigCid0 = 51,
  SigCid1 = 52,
  SigCid2 = 53,
  SigCk0 = 54,
  SigCk1 = 55,
  SigCk2 = 56,
  SigCk3 = 57,
  SigCk4 = 58,
  SigCk5 = 59,
  SigGnt0 = 60,
  SigGnt1 = 61,
  SigErid00 = 62,
  SigErid01 = 63,
  SigErid10 = 64,
  SigErid11 = 65,
  SigErr0 = 66,
  SigErr1 = 67,
  Sig0Ca0 = 68,    // Subchannel 0
  Sig0Ca1 = 69,
  Sig0Ca2 = 70,
  Sig0Ca3 = 71,
  Sig0Ca4 = 72,
  Sig0Ca5 = 73,
  Sig0Ca6 = 74,
  Sig0Ca7 = 75,
  Sig0Ca8 = 76,
  Sig0Ca9 = 77,
  Sig0Ca10 = 78,
  Sig0Ca11 = 79,
  Sig0Ca12 = 80,
  Sig0Ca13 = 81,
  Sig1Ca0 = 82,    // Subchannel 1
  Sig1Ca1 = 83,
  Sig1Ca2 = 84,
  Sig1Ca3 = 85,
  Sig1Ca4 = 86,
  Sig1Ca5 = 87,
  Sig1Ca6 = 88,
  Sig1Ca7 = 89,
  Sig1Ca8 = 90,
  Sig1Ca9 = 91,
  Sig1Ca10 = 92,
  Sig1Ca11 = 93,
  Sig1Ca12 = 94,
  Sig1Ca13 = 95,
  Sig0Cs0n = 96,    // Subchannel 0
  Sig0Cs1n = 97,
  Sig0Cs2n = 98,
  Sig0Cs3n = 99,
  Sig1Cs0n = 100,    // Subchannel 1
  Sig1Cs1n = 101,
  Sig1Cs2n = 102,
  Sig1Cs3n = 103,

  MrcCsnMax,
  MrcCsnDelim = MAX_INT32
  } MRC_CSN;

typedef struct  {
  MRC_CSN  Signal;    // Command signal name
  INT32  Value;     // Offset selected by mode GsmWriteOffset
} MRC_CSVAL;

typedef enum {
  DefaultTest = 0,
  XoverTest = 1,
  SenseAmpTest = 2,
  EarlyCmdMPRTest = 10,
  EarlyCmdParTest = 11,
  EarlyCmdLoopbackTest = 12,   // re-use this one for DDR5.
  EarlyCtlRecEnTest = 13,
  EarlyCtlLoopbackTest = 14,
  EarlyCsLoopbackTest = 15,    //DDR5 Early CS CLK training
  EarlyRcdCsLoopbackTest = 16, //DDR5 Early CS CLK training
  EarlyCsBacksideLoopbackTest = 17, //DDR5 Early CS CLK Backside training
  EarlyCaLoopbackTest = 18, //DDR5 Early CA CLK training
  EarlyCaBacksideLoopbackTest = 19, //DDR5 Early Ca CLK Backside training
  RcvenFineTest = 20,  // re-use this one for DDR5.
  RcvenCoarseTest = 21, // re-use this one for DDR5 (is this really a different test, or can we combine fine and coarse?)
  RdDqTest = 22,  // re-use this one for DDR5, for a serial mode pattern that is similar to DDR4 clock pattern.
  ReadPreDfe2DCenteringTest = 23, //Read DqDqs Pre Dfe 2D Centering for DDR5
  ReadDfeTest = 24, // new for DDR5
  ReadPostDfe2DCenteringTest = 25, //Read DqDqs Post Dfe 2D Centering for DDR5
  EarlyRxVrefTest = 26,  // re-use for DDR5
  DqSwzDiscTestDdr5 = 27,
  BcomTrainingTest = 28,
  PeriodicTxReTrainTest = 29, //Periodic Tx Retraining Test for DDR5
  WrLvlFineTest = 30,
  WrLvlCoarseTest = 31,
  WrLvlCombinedTest = 32, // new for DDR5?
  WrDqTest = 33, //re-use this one for DDR5
  WritePreDfe2DCenteringTest = 34, //Write DqDqs Pre Dfe 2D Centering
  WriteDfeTest = 35, // new for DDR5
  WritePostDfe2DCenteringTest = 36, // new for DDR5
  EarlyTxVrefTest = 37,  // re-use for DDR5
  DcaDckDutyCycleTest = 38, // DCA DCK Duty Cycle Test
  DcaDfeDdr5Test = 39, // new for DDR5
  LateCmdTest = 40,  // re-use for DDR5
  AdvDqRxPwrTrainTest = 51, //re-use for DDR5
  AdvDqTurnAroundTest = 52, //re-use for DDR5
  RmtCmdTest = 60, //DDR5
  RmtDqTest = 61, //DDR5
  MemInitFunction = 70, //DDR5
  MemTest0 = 71, //DDR5
  MemTest1 = 72,
  MemTest2 = 73, // TODO: need to define more test types for Advanced Memtest
  EarlyEridTest = 80,
  AdvEridTest = 81,
  CmdVrefTxEq = 82,
  EarlyRcdCaLoopbackTest = 83,
  WLDdr5 = 84,
  WlFineSweep = 85,
  InternalWlFineSweepTest = 86,
  ExternalWlFineSweepTest = 87,
  ReqClkTest = 88,
  InternalWlCoarseSweepTest = 89,
  MdqsReceiveEnablePhaseTraining = 90,     //MDQS receive enable phase training
  MdqsReceiveEnableCycleTraining = 91,     //MDQS receive enable cycle training
  DramRxEqTest = 92, //DRAM Rx Eq training test
  MdqsCoarseReadDelayTraining = 94,        //MDQS read delay training
  BacksideDqSwizzleDiscovery = 95,         //LRDIMM Backside swizzle discovery
  MdqsExternalWriteLevelingCoarseTraining = 96,    //MDQS External Write Leveling Coarse training
  MdqsExternalWriteLevelingFineTraining   = 97,    //MDQS External Write Leveling Fine training
  MdqsInternalWriteLevelingCoarseTraining = 98,    //MDQS Internal Write Leveling Coarse training
  MdqsInternalWriteLevelingFineTraining   = 99,    //MDQS Internal Write Leveling Fine training
  CaSlewRateTest                          = 100,   //Ca Slew Rate Test
  FrontsideDqSwizzleDiscovery             = 101,   //LRDIMM Frontside swizzle discovery
  MdqsReadDelayDeskew                     = 102,   //LRDIMM Per-bit read delay deskew
  TurnAroundSgTest                        = 103,   // Turn Around, Same Rank, Same Bank
  TurnAroundSrTest                        = 104,   // Turn Around, Same Rank, Different Bank
  TurnAroundDrDdWwRrWrTest                = 105,   // Turn Around, Different Rank, Wr-Wr, Rd-Rd, Wr-Rd
  TurnAroundDrDdRwTest                    = 106,   // Turn Around, Different Rank, Rd-Wr
  MdqsWriteDelay                          = 107,   // LRDIMM write delay training
  DqSlewRateTest                          = 108,
  DramDutyCycleAdjusterTest               = 109,
  WriteDbDfeTest                          = 110,   // Write Data Buffer DFE training
  MrcTtMax,
  MrcTtDelim = MAX_INT32
} MRC_TT;

typedef struct {
  UINT8       Dimm;    // DIMM number within a DDR channel (0-based, farthest slot == 0)
  UINT8       Rank;    // Logical rank number within a DIMM (0-based)
} MRC_RT;

typedef struct {
  UINT8   odtRdLeading;
  UINT8   odtRdTrailing;
  UINT8   odtWrLeading;
  UINT8   odtWrTrailing;
  } ODT_TIMING, *PODT_TIMING;

typedef enum {
  Even = 0,
  Odd = 1,
  All = 3,
  MrcCtMax,
  MrcCtDelim = MAX_INT32
  } MRC_CT;

///
/// External signal names
///
typedef enum {
  RAS_N, CAS_N, WE_N,
  BA0, BA1, BA2,
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17,
  CS0_N, CS1_N, CS2_N, CS3_N, CS4_N, CS5_N, CS6_N, CS7_N, CS8_N, CS9_N,
  CKE0, CKE1, CKE2, CKE3, CKE4, CKE5,
  ODT0, ODT1, ODT2, ODT3, ODT4, ODT5,
  PAR, ALERT_N,
  BG0, BG1, ACT_N, C0, C1, C2,
  CK0, CK1, CK2, CK3, CK4, CK5,
  FNV_GOOD_PARITY, DESELECT, PRECHARGE, GNT,   // these are actually commands as opposed to signals
  A0A, A1A, A2A, A3A, A4A, A5A, A6A, A7A, A8A, A9A, A10A, A11A, A12A, A13A, PARA,
  A0B, A1B, A2B, A3B, A4B, A5B, A6B, A7B, A8B, A9B, A10B, A11B, A12B, A13B, PARB,
  CS0_A_N, CS1_A_N, CS2_A_N, CS3_A_N, CS0_B_N, CS1_B_N, CS2_B_N, CS3_B_N,
  QnCSm, // All QCS signals: QCS[0:3][SUB CH A: SUB CH B]
  QCS0_A_n, QCS1_A_n,  // D0 R0 SUB CH A; D0 R1 SUB CH A
  QCS2_A_n, QCS3_A_n,  // D1 R0 SUB CH A; D1 R1 SUB CH A
  QCS0_B_n, QCS1_B_n,  // D0 R0 SUB CH B; D0 R1 SUB CH B
  QCS2_B_n, QCS3_B_n,  // D1 R0 SUB CH B; D1 R1 SUB CH B
  ALERT_N_DDR5, RSPA_SA, RSPB_SA, RSPA_SB, RSPB_SB,
  QnxCAm, // All QCA signals: Q[0:1][A:B]CA[0:13]
  Q0ACA0, Q0ACA1, Q0ACA2, Q0ACA3, Q0ACA4, Q0ACA5, Q0ACA6, Q0ACA7, Q0ACA8, Q0ACA9, Q0ACA10, Q0ACA11, Q0ACA12, Q0ACA13,
  Q0BCA0, Q0BCA1, Q0BCA2, Q0BCA3, Q0BCA4, Q0BCA5, Q0BCA6, Q0BCA7, Q0BCA8, Q0BCA9, Q0BCA10, Q0BCA11, Q0BCA12, Q0BCA13,
  Q1ACA0, Q1ACA1, Q1ACA2, Q1ACA3, Q1ACA4, Q1ACA5, Q1ACA6, Q1ACA7, Q1ACA8, Q1ACA9, Q1ACA10, Q1ACA11, Q1ACA12, Q1ACA13,
  Q1BCA0, Q1BCA1, Q1BCA2, Q1BCA3, Q1BCA4, Q1BCA5, Q1BCA6, Q1BCA7, Q1BCA8, Q1BCA9, Q1BCA10, Q1BCA11, Q1BCA12, Q1BCA13,
  Q0ACK, Q0BCK, Q0CCK, Q0DCK,
  Q1ACK, Q1BCK, Q1CCK, Q1DCK,
  BCOM_A, BCOM_B,
  NC,
  gsmCsnDelim = MAX_INT32
} GSM_CSN;

#pragma pack(push, 1)
//
// Provide an association between MRC_GT values and signal group type strings
//
typedef struct {
  MRC_GT    Group;
  CHAR8     *String;
} GROUP_STRING;

//
// Provide an association between GSM_CSN values and signal type strings
//
typedef struct {
  GSM_CSN   Signal;
  CHAR8     *String;
} SIGNAL_STRING;

typedef struct {
  GSM_CSN   Rank;
  UINT8     Copy;
  CHAR8     *String;
} BACKSIDE_QCS_SIGNAL_STRING;

#pragma pack(pop)

#endif // _MrcCommonTypes_h_
