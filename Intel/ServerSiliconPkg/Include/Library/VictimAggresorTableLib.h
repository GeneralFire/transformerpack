/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2018 Intel Corporation. <BR>

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

#ifndef __VICTIM_AGGRESSOR_TABLE_LIB_H__
#define __VICTIM_AGGRESSOR_TABLE_LIB_H__

typedef struct {
  UINT8 vicAggrLookup[MAX_CH][MAX_BITS][2]; // [x][y][0]= primary aggressor for that bit (index 0 based); [x][y][1]= secondary aggressor for that bit (index 0 based)
  UINT8 bitOrder[MAX_CH][MAX_BITS]; // order in which worst margins are treated as potential victim
  UINT8 MT[MAX_CH][MAX_BITS][7]; //[x][y][0] = DQ lane; [x][y][1]= victim; [x][y][2]= aggr0; [x][y][3]= aggr1; [x][y][4]=victim status; [x][y][5]=aggressor 0 status;[x][y][6]=aggressor 1 status;
  INT16 marginValue[MAX_CH][MAX_BITS]; // margin value for that bit (index 0 based)
  INT16 skewValue[MAX_CH][MAX_BITS];   // skew value for that bit (index 0 based)
  INT16 nibbleMaxSkew[MAX_CH][MAX_BITS / 4][2];  // [x][z][0] = maxNibbleSkewBit(skew bit value); [x][z][1] = maxNibbleSkew(skew value)
} PXC_VIC_AGGR;

typedef struct {
  UINT8 CapIndLookup[MAX_CH][MAX_STROBE];
} PXC_CAP_IND_SETTING;

typedef enum {
  PxcAttacker0 = 0,
  PxcAttacker1,
  MaxPxcAttacker
} PXC_ATTACKER_ENUM;

typedef union {
  struct {
    UINT32 AttackerBit0 : 3; // xtalk aggressor for bit0
    UINT32 AttackerBit1 : 3; // xtalk aggressor for bit1
    UINT32 AttackerBit2 : 3; // xtalk aggressor for bit2
    UINT32 AttackerBit3 : 3; // xtalk aggressor for bit3
    UINT32 Reserved : 20;
  } Bits;
  UINT32 Data;
} PXC_ATTACKER_SEL;

#define DQ0 0
#define DQ1 1
#define DQ2 2
#define DQ3 3
#define DQ4 4
#define DQ5 5
#define DQ6 6
#define DQ7 7
#define DQ8 8
#define DQ9 9
#define DQ10 10
#define DQ11 11
#define DQ12 12
#define DQ13 13
#define DQ14 14
#define DQ15 15
#define DQ16 16
#define DQ17 17
#define DQ18 18
#define DQ19 19
#define DQ20 20
#define DQ21 21
#define DQ22 22
#define DQ23 23
#define DQ24 24
#define DQ25 25
#define DQ26 26
#define DQ27 27
#define DQ28 28
#define DQ29 29
#define DQ30 30
#define DQ31 31
#define DQ32 32
#define DQ33 33
#define DQ34 34
#define DQ35 35
#define DQ36 36
#define DQ37 37
#define DQ38 38
#define DQ39 39
#define DQ40 40
#define DQ41 41
#define DQ42 42
#define DQ43 43
#define DQ44 44
#define DQ45 45
#define DQ46 46
#define DQ47 47
#define DQ48 48
#define DQ49 49
#define DQ50 50
#define DQ51 51
#define DQ52 52
#define DQ53 53
#define DQ54 54
#define DQ55 55
#define DQ56 56
#define DQ57 57
#define DQ58 58
#define DQ59 59
#define DQ60 60
#define DQ61 61
#define DQ62 62
#define DQ63 63
#define DQ64 64
#define DQ65 65
#define DQ66 66
#define DQ67 67
#define DQ68 68
#define DQ69 69
#define DQ70 70
#define DQ71 71

/**
  This function returns a pointer to the SoC victim aggressor DQ mapping.

  @param[out] VicAggrTablePtr - Pointer to victim aggressor table

  @retval EFI_STATUS to to indicate the execution status of the routine
**/
EFI_STATUS
EFIAPI
GetVictimAggressorTable (
  OUT PXC_VIC_AGGR **VicAggrTablePtr
  );

/**
  This function returns a pointer to the SoC capacitive/inductive values.

  @param[out] CapIndTablePtr - Pointer to Capacitive/Inductive table

  @retval EFI_STATUS to to indicate the execution status of the routine
**/
EFI_STATUS
EFIAPI
GetCapacitiveInductiveTable (
  OUT PXC_CAP_IND_SETTING **CapIndTablePtr
  );

#endif // __VICTIM_AGGRESSOR_TABLE_LIB_H__
