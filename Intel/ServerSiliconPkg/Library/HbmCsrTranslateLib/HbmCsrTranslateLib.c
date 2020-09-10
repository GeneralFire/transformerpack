/** @file
  HbmCsrTranslateLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/HbmCsrTranslateLib.h>

#define FLAG_UNSUP 0
#define FLAG_MINUS 1
#define FLAG_ADD   2

#define BLOCK_ID_OFFSET 8

typedef struct {
  UINT8     BlockId;
  UINT8     SignFlag;
  UINT16    Offset;
} DDR_TO_HBM_MAPPING;

//
// -------------------------------------------------------------
// |  BlockId  - DDR Register offset left sheft 8 bits         |
// |  SignFlag - DDR offset to HBM offset add or minus flag    |
// |  Offset   - Offset used to convert DDR registers to HBM   |
// -------------------------------------------------------------
//ww09b
//                        DDR MC  HBM MC  SignFlag Offset
//  mc_2lmcntl            0x0008  0x0008  0
//  mcdecs                0x080c  0x080c  0
//  mc_dec                0x0c00  0x0c00  0
//  mc_2lmddrt            0x1400                   // Unmapped
//  mcscheds_bs           0x1800  0x1800  0
//  mcmisc                0x2000  0x8000  +0x6000
//  mcscheds_pq           0x2800  0x1c00  -0x0c00
//  mc_rrd                0x2c08  0x2808  -0x0400  // 0x2c08 sprsp_top_mchbmss_mc_reg[0]_mc_rrd_crnode1_MEM_MCDP_CSR.xml
//  mc_wdb                0x3000  0x3000  0        // 0x3400 sprsp_top_mchbmss_mc_reg[0]_mc_wdb_crnode1_MEM_MCDP_CSR.xml
//  mc_wp                 0x3400  0x1000  -0x2400  // 0x1400 sprsp_top_mchbmss_mc_reg[0]_mc_wp_crnode1_MEM_MCDP_CSR.xml
//  cpgc_shared_cpgc_p    0x9000  0x3800  -0x5800
//  cpgc_shared_cpgc_v    0x9018  0x3818  -0x5800
//  cpgc_reqgen0_cpgc_t   0x9030  0x3830  -0x5800
//  cpgc_channel0_cpgc_d  0x9120  0x3920  -0x5800
//  cpgc_buffer0_cpgc_b   0x92d0  0x3ad0  -0x5800
//  cpgc_reqgen0_cpgc_a   0x9320  0x3b20  -0x5800
//  cpgc_reqgen1_cpgc_t   0x9430  0x3c30  -0x5800
//  cpgc_channel1_cpgc_d  0x9520  0x3d20  -0x5800
//  cpgc_buffer1_cpgc_b   0x96d0  0x3ed0  -0x5800
//  cpgc_reqgen1_cpgc_a   0x9720  0x3f20  -0x5800

//
DDR_TO_HBM_MAPPING DdrToHbm[] = {
  {0x00, FLAG_MINUS, 0     }, // mc_2lmcntl, mcdecs, mc_dec
  {0x14, FLAG_UNSUP, 0     }, // mc_2lmddrt
  {0x18, FLAG_MINUS, 0     }, // mcscheds_bs
  {0x20, FLAG_ADD,   0x6000}, // mcmisc
  {0x28, FLAG_MINUS, 0x0c00}, // mcscheds_pq
  {0x2c, FLAG_MINUS, 0x0400}, // mc_rrd
  {0x30, FLAG_MINUS, 0     }, // mc_wdb
  {0x34, FLAG_MINUS, 0x2400}, // mc_wp
  {0x90, FLAG_MINUS, 0x5800}, // cpgc_shared_cpgc_p, cpgc_shared_cpgc_v, cpgc_reqgen0_cpgc_t, cpgc_channel0_cpgc_d, cpgc_buffer0_cpgc_b, cpgc_reqgen0_cpgc_a
                              // cpgc_reqgen1_cpgc_t, cpgc_channel1_cpgc_d, cpgc_buffer1_cpgc_b, cpgc_reqgen1_cpgc_a
};

/**
  Used to do a translation between DDR MC to HBM MC

  @param[in] MemTechType  - Memory Technology Type
  @param[in] RegOffset    - Data of CSR register offset

  @retval RegOffset       - CSR register offset

**/
UINT32
DdrToHbmCompatibility (
  IN  MEM_TECH_TYPE  MemTechType,
  IN  UINT32         RegOffset
  )
{
  CSR_OFFSET      CsrOffset;
  UINT8           BoxType;
  UINT8           FuncBlk;

  CsrOffset.Data = RegOffset;
  BoxType = (UINT8) CsrOffset.Bits.boxtype;
  FuncBlk = (UINT8) CsrOffset.Bits.funcblk;

  //
  // Do convert only if the Mem Technology Type is HBM and the Rc Header is Combine MC Header(DDR)
  //
  if ((MemTechType == MemTechHbm) && (BoxType != BOX_HBM && BoxType != BOX_HBM2E_MC)) {

    //
    // Not support pseudo to access HBM MC registers
    //
    if (CsrOffset.Bits.pseudo) {
      ASSERT(FALSE);
    }

    //
    // Convert Function Block from DDR to HBM
    //
    if (BoxType == BOX_MC) {
      if (FuncBlk == BOX_FUNC_MC_MAIN) {
        CsrOffset.Bits.funcblk = SPR_HBM_FUNC_MAIN;
      } else if (FuncBlk == BOX_FUNC_MC_2LM) {
        CsrOffset.Bits.funcblk = SPR_HBM_FUNC_2LM;
      } else if (FuncBlk == BOX_FUNC_MC_GLOBAL) {
        CsrOffset.Bits.funcblk = SPR_HBM_FUNC_MC_GLOBAL;
      } else {
        ASSERT(FALSE);
      }
    } else if (BoxType == BOX_MCDDC) {
      if (FuncBlk == BOX_FUNC_MCDDC_CTL) {
        CsrOffset.Bits.funcblk = SPR_HBM_FUNC_DDC_CTL;
      } else if (FuncBlk == BOX_FUNC_MCDDC_DP) {
        CsrOffset.Bits.funcblk = SPR_HBM_FUNC_DDC_DP;
      } else {
        ASSERT(FALSE);
      }
    } else if (BoxType == BOX_MCCPGC) {
      CsrOffset.Bits.funcblk = SPR_HBM_FUNC_CPGC;
    } else {
      ASSERT(FALSE);
    }

    //
    // Convert Box Type from DDR to HBM
    //
    CsrOffset.Bits.boxtype = BOX_HBM2E_MC;

    DdrToHbmOffset(&CsrOffset);

  }

  return CsrOffset.Data;
}

/**
  Used to calculate offset from DDR MC to HBM MC

  @param[in, out] CsrOffset    - Pointer to data of CSR register offset

  @retval NULL

**/
VOID
DdrToHbmOffset (
  IN  OUT  CSR_OFFSET  *CsrOffset
  )
{
  UINT8           Index;
  UINT8           BlockId;
  UINT8           Limit;

  //
  // Get DdrToHbm Offset Table
  //
  BlockId = (UINT8) (CsrOffset->Bits.offset >> BLOCK_ID_OFFSET);
  Limit = sizeof (DdrToHbm) / sizeof (DdrToHbm[0]);
  for (Index = 1; Index < Limit; Index++) {
    if (DdrToHbm[Index].BlockId > BlockId) {
        break;
    }
  }
  Index = Index - 1;

  //
  // Convert Offset from DDR to HBM
  //
  if (DdrToHbm[Index].SignFlag == FLAG_MINUS) {
    CsrOffset->Bits.offset -= DdrToHbm[Index].Offset;
  } else if (DdrToHbm[Index].SignFlag == FLAG_ADD) {
    CsrOffset->Bits.offset += DdrToHbm[Index].Offset;
  } else {
    ASSERT(FALSE);
  }
}
