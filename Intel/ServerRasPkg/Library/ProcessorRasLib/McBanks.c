/** @file
  Implementation of Cloak function for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "ProcessorRasLibInternal.h"
#include <Library/CpuAndRevisionLib.h>
#include <Library/Mesh2MemIpLib.h>

#define EMCA_LOG              BIT2

#define MCA_UNIT_IFU0         (MCA_UNIT_TYPE_IFU)
#define MCA_UNIT_DCU0         (MCA_UNIT_TYPE_DCU)
#define MCA_UNIT_DTLB0        (MCA_UNIT_TYPE_DTLB)
#define MCA_UNIT_MLC0         (MCA_UNIT_TYPE_MLC)

#define MCA_UNIT_PCU0         (MCA_UNIT_TYPE_PCU)
#define MCA_UNIT_KTI0         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_KTI1         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_KTI2         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_KTI3         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_KTI4         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_KTI5         (MCA_UNIT_TYPE_KTI)
#define MCA_UNIT_CHAM0        (MCA_UNIT_TYPE_CHA)
#define MCA_UNIT_CHAM1        (MCA_UNIT_TYPE_CHA)
#define MCA_UNIT_CHAM2        (MCA_UNIT_TYPE_CHA)

#define MCA_UNIT_IIO0         (MCA_UNIT_TYPE_IIO)
#define MCA_UNIT_M2M0         (MCA_UNIT_TYPE_M2MEM)
#define MCA_UNIT_M2M1         (MCA_UNIT_TYPE_M2MEM)
#define MCA_UNIT_M2M2         (MCA_UNIT_TYPE_M2MEM)
#define MCA_UNIT_M2M3         (MCA_UNIT_TYPE_M2MEM)
#define MCA_UNIT_IMC00        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC01        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC02        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC10        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC11        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC12        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC20        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC21        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC22        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC30        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC31        (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC32        (MCA_UNIT_TYPE_IMC)
#define MCA_UINT_MEE0         (MCA_UNIT_TYPE_MEE)
#define MCA_UINT_MEE1         (MCA_UNIT_TYPE_MEE)
#define MCA_UINT_MEE2         (MCA_UNIT_TYPE_MEE)
#define MCA_UINT_MEE3         (MCA_UNIT_TYPE_MEE)
#define MCA_UINT_MEE4         (MCA_UNIT_TYPE_MEE)
#define MCA_UINT_MEE5         (MCA_UNIT_TYPE_MEE)
#define MCA_UNIT_CH0          (MCA_UNIT_TYPE_CH)
#define MCA_UNIT_CH1          (MCA_UNIT_TYPE_CH)
#define MCA_UNIT_CH2          (MCA_UNIT_TYPE_CH)
#define MCA_UNIT_DUNIT0       (MCA_UNIT_TYPE_DUNIT)
#define MCA_UNIT_DUNIT1       (MCA_UNIT_TYPE_DUNIT)
#define MCA_UNIT_DUNIT2       (MCA_UNIT_TYPE_DUNIT)
#define MCA_UNIT_MDF0         (MCA_UNIT_TYPE_MDF)
#define MCA_UNIT_MDF1         (MCA_UNIT_TYPE_MDF)
#define MCA_UNIT_IMB0         (MCA_UNIT_TYPE_IMB)
#define MCA_UNIT_HBM_M2MEM    (MCA_UNIT_TYPE_M2MEM)
#define MCA_UNIT_HBM_IMC_DP0  (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_HBM_IMC_DP1  (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_LLC0         (MCA_UNIT_TYPE_LLC)
#define MCA_UNIT_LLC1         (MCA_UNIT_TYPE_LLC)
#define MCA_UNIT_MSE          (MCA_UNIT_TYPE_MSE)

#define MSR_IA32_MC29_CTL     0x00000474
#define MSR_IA32_MC30_CTL     0x00000478
#define MSR_IA32_MC31_CTL     0x0000047C

#define M2M0_BANK_OFFSET_07   7
#define M2M0_BANK_OFFSET_12   12
#define M2M0_BANK_GAP_01        1
#define M2M0_BANK_GAP_04        4

#define NA                    0xFF        // Instance is not applicable

/**
  Fill MC signature as defined in the EMCA SAS.

  @param[in]  McBankIdx   The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
  @param[in]  Socket      Socket ID.
  @param[in]  Instance    Unit instance number.
  @param[out] McSig       A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
**/
typedef
VOID
(EFIAPI *MCA_READ_MCBANK_SIG_HOOK) (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  );

VOID
EFIAPI
ReadMcBankSignature (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  );

VOID
EFIAPI
ReadM2MemMcBankSignature (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  );

//
// Type Definitions
//
typedef struct {
  UINT32 CtlMsr;
  UINT32 Ctl2Msr;
  UINT16 Attrib;
  UINT16 SecType;
  UINT32 UnitType;
  UINT8  Instance;  ///< unit instance number, if applicable.
  MCA_READ_MCBANK_SIG_HOOK ReadMcBankSig;
} EMCA_MCBANK_DESC;

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListSkx [] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_IFU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_DCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_DTLB0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_MLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_PCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI0,       0,  ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M1,       1,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM2,      NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI1,       1,  ReadMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC10,      1,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC11,      1,  ReadMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC02,      0,  ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC12,      1,  ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI2,       2,  ReadMcBankSignature},

  //
  // CPX-only UPI links
  //
  {MSR_IA32_MC20_CTL, MSR_IA32_MC20_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI3,       3, ReadMcBankSignature},
  {MSR_IA32_MC21_CTL, MSR_IA32_MC21_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI4,       4, ReadMcBankSignature},
  {MSR_IA32_MC22_CTL, MSR_IA32_MC22_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI5,       5, ReadMcBankSignature}
};

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListIcxSp[] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_IFU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_DCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_DTLB0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_MLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_PCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI0,       0,  ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI1,       1,  ReadMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI2,       2,  ReadMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM2,      NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC02,      0,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M1,       1,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC10,      1,  ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC11,      1,  ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC12,      1,  ReadMcBankSignature},
  {MSR_IA32_MC20_CTL, MSR_IA32_MC20_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M2,       2,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC21_CTL, MSR_IA32_MC21_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC20,      2,  ReadMcBankSignature},
  {MSR_IA32_MC22_CTL, MSR_IA32_MC22_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC21,      2,  ReadMcBankSignature},
  {MSR_IA32_MC23_CTL, MSR_IA32_MC23_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC22,      2,  ReadMcBankSignature},
  {MSR_IA32_MC24_CTL, MSR_IA32_MC24_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M3,       3,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC25_CTL, MSR_IA32_MC25_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC30,      3,  ReadMcBankSignature},
  {MSR_IA32_MC26_CTL, MSR_IA32_MC26_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC31,      3,  ReadMcBankSignature},
  {MSR_IA32_MC27_CTL, MSR_IA32_MC27_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC32,      3,  ReadMcBankSignature}
  };

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListIcxD[] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_IFU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_DCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_DTLB,  NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_MLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_PCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM2,      NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC02,      0,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M1,       1,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC10,      1,  ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC11,      1,  ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC12,      1,  ReadMcBankSignature}
};

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListSnr[] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_BUS,   NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_L2,    NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_FEC,   NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_MEC,   NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_PUNIT, NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE, NA, ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM2,      NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC02,      0,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature}
};

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListSprSp[] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_IFU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_DCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_DTLB0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_MLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_PCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI0,       0,  ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_MDF0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_MDF1,       NA, ReadMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM2,      NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC10,      1,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC11,      1,  ReadMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC20,      2,  ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC21,      2,  ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC30,      3,  ReadMcBankSignature},
  {MSR_IA32_MC20_CTL, MSR_IA32_MC20_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC31,      3,  ReadMcBankSignature},
  {MSR_IA32_MC21_CTL, MSR_IA32_MC21_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC22_CTL, MSR_IA32_MC22_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC23_CTL, MSR_IA32_MC23_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC24_CTL, MSR_IA32_MC24_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC25_CTL, MSR_IA32_MC25_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC26_CTL, MSR_IA32_MC26_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC27_CTL, MSR_IA32_MC27_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC28_CTL, MSR_IA32_MC28_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC29_CTL, MSR_IA32_MC29_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_HBM_M2MEM,  0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC30_CTL, MSR_IA32_MC30_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_HBM_IMC_DP0,0,  ReadMcBankSignature},
  {MSR_IA32_MC31_CTL, MSR_IA32_MC31_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_HBM_IMC_DP1,1,  ReadMcBankSignature}
  };

GLOBAL_REMOVE_IF_UNREFERENCED EMCA_MCBANK_DESC gMcBankListGnrSp[] = {
  {MSR_IA32_MC0_CTL,  MSR_IA32_MC0_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_IFU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC1_CTL,  MSR_IA32_MC1_CTL2,  MC_SCOPE_TD,             EMCA_SECTYPE_NONE,    MCA_UNIT_DCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC2_CTL,  MSR_IA32_MC2_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_DTLB0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC3_CTL,  MSR_IA32_MC3_CTL2,  MC_SCOPE_CORE,           EMCA_SECTYPE_NONE,    MCA_UNIT_MLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC4_CTL,  MSR_IA32_MC4_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_PCU0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC5_CTL,  MSR_IA32_MC5_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_KTI0,       0,  ReadMcBankSignature},
  {MSR_IA32_MC6_CTL,  MSR_IA32_MC6_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_IIO0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC7_CTL,  MSR_IA32_MC7_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM0,      NA, ReadMcBankSignature},
  {MSR_IA32_MC8_CTL,  MSR_IA32_MC8_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_CHAM1,      NA, ReadMcBankSignature},
  {MSR_IA32_MC9_CTL,  MSR_IA32_MC9_CTL2,  MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_LLC0,       NA, ReadMcBankSignature},
  {MSR_IA32_MC10_CTL, MSR_IA32_MC10_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_LLC1,       NA, ReadMcBankSignature},
  {MSR_IA32_MC11_CTL, MSR_IA32_MC11_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_MSE,        NA, ReadMcBankSignature},
  {MSR_IA32_MC12_CTL, MSR_IA32_MC12_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_M2M0,       0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC13_CTL, MSR_IA32_MC13_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC00,      0,  ReadMcBankSignature},
  {MSR_IA32_MC14_CTL, MSR_IA32_MC14_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC01,      0,  ReadMcBankSignature},
  {MSR_IA32_MC15_CTL, MSR_IA32_MC15_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC02,      0,  ReadMcBankSignature},
  {MSR_IA32_MC16_CTL, MSR_IA32_MC16_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC10,      0,  ReadMcBankSignature},
  {MSR_IA32_MC17_CTL, MSR_IA32_MC17_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC11,      0,  ReadMcBankSignature},
  {MSR_IA32_MC18_CTL, MSR_IA32_MC18_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC12,      0,  ReadMcBankSignature},
  {MSR_IA32_MC19_CTL, MSR_IA32_MC19_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC20,      0,  ReadMcBankSignature},
  {MSR_IA32_MC20_CTL, MSR_IA32_MC20_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC21,      0,  ReadMcBankSignature},
  {MSR_IA32_MC21_CTL, MSR_IA32_MC21_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC22,      0,  ReadMcBankSignature},
  {MSR_IA32_MC22_CTL, MSR_IA32_MC22_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC30,      0,  ReadMcBankSignature},
  {MSR_IA32_MC23_CTL, MSR_IA32_MC23_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC31,      0,  ReadMcBankSignature},
  {MSR_IA32_MC24_CTL, MSR_IA32_MC24_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_IMC32,      0,  ReadMcBankSignature},
  {MSR_IA32_MC25_CTL, MSR_IA32_MC25_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC26_CTL, MSR_IA32_MC26_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC27_CTL, MSR_IA32_MC27_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC28_CTL, MSR_IA32_MC28_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC29_CTL, MSR_IA32_MC29_CTL2, MC_SCOPE_PKG,            EMCA_SECTYPE_NONE,    MCA_UNIT_TYPE_NONE,  NA, ReadMcBankSignature},
  {MSR_IA32_MC30_CTL, MSR_IA32_MC30_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_HBM_M2MEM,  0,  ReadM2MemMcBankSignature},
  {MSR_IA32_MC31_CTL, MSR_IA32_MC31_CTL2, MC_SCOPE_PKG | EMCA_LOG, EMCA_SECTYPE_PLATMEM, MCA_UNIT_HBM_IMC_DP0,0,  ReadMcBankSignature}
  };

/**
  Get the number of MCBANKS_PER_LP.

  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  Assumption of implementation is that all sockets are symmetric, they all have the same number of MCBanks Per LP

  @return MCBANKS_PER_LP

**/
UINT32
EFIAPI
GetNumMcBanksPerLp (
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);

  return (UINT32) (McgCapReg.Bits.Count);
}

/**

    Return bimap of MC Banks that support EMCA gen 2 morphing

    @param None

    @retval BitMap - Bitmap of McBanks which support morphing

**/
UINT32
EFIAPI
GetEmcaBankSupp (
  VOID
  )
{
  MSR_SMM_MCA_CAP_REGISTER SmmMcaCapReg;

  SmmMcaCapReg.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);

  return (UINT32) (SmmMcaCapReg.Bits.BankSupport);
}

EMCA_MCBANK_DESC *
GetMachineCheckBank (
  IN UINTN        Index
  )
{
  EMCA_MCBANK_DESC *Desc;

  if (Index >= GetNumMcBanksPerLp () ){
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= GetNumMcBanksPerLp ()\n"));
      RAS_ASSERT (FALSE);
  }

  Desc = NULL;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListIcxD)) {
      Desc = &gMcBankListIcxD[Index];
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListIcxD) \n"));
      RAS_ASSERT (FALSE);
    }
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListIcxSp)) {
      Desc = &gMcBankListIcxSp[Index];
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListIcxSp) \n"));
      RAS_ASSERT (FALSE);
    }
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListSnr)) {
      Desc = &gMcBankListSnr[Index];
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListSnr) \n"));
      RAS_ASSERT (FALSE);
    }
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListSkx)) {
      Desc = &gMcBankListSkx[Index];
    } else{
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListSkx) \n"));
      RAS_ASSERT (FALSE);
    }
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListSprSp)) {
      Desc = &gMcBankListSprSp[Index];
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListSprSp) \n"));
      RAS_ASSERT (FALSE);
    }
  }else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
    if (Index < ARRAY_SIZE (gMcBankListGnrSp)) {
      Desc = &gMcBankListGnrSp[Index];
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Index >= ARRAY_SIZE (gMcBankListGnrSp) \n"));
      RAS_ASSERT (FALSE);
    }
  }

  return Desc;
}

/**

    Get Scope for the Bank. The scope can be thread level, core level and package level.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval  0x0  - Thread Scope
    @retval  0x1  - Core Scope
    @retval  0x2  - Package Scope

**/
UINT16
EFIAPI
GetMcBankScope (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return (Desc->Attrib & MCSCOPE_MASK);
  }
  return MC_SCOPE_ERROR;
}


/**
    Checks whether the machine check bank support log.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval TRUE         The machine check bank support Emca log.
    @retval FALSE        The machine check bank does not support Emca log.

**/
BOOLEAN
EFIAPI
IsMcBankSupportEmcaLog (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return ((BOOLEAN) (Desc->Attrib & EMCA_LOG) != 0);
  }
  return FALSE;
}


/**

    Get Section Type for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval SecType - Return the section type related to the McBank Unit

**/
UINT16
EFIAPI
GetMcBankSecType (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->SecType;
  }
  return EMCA_SECTYPE_INVALID;
}


/**

    Return whether the bank unit is Memory

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval UNIT TYPE  - Silicon Specific Bank Unit Type

**/
UINT32
EFIAPI
GetMcBankUnitType (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->UnitType;
  }
  return MCA_UNIT_TYPE_ERROR;
}

UINT32
GetAddrMcBankCtl2 (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->Ctl2Msr;
  }
  return MSR_IA32_INVALID;
}

/**

    Get Bank Index for Mci_status for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval Mci_status Index

**/
UINT32
EFIAPI
GetAddrMcBankSts (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->CtlMsr + 0x1;
  }
  return MSR_IA32_INVALID;
}

/**

    Get Bank Index for Mci_addr for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.


    @retval Mci_addr Index

**/
UINT32
GetAddrMcBankAddr (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->CtlMsr + 0x2;
  }
  return MSR_IA32_INVALID;
}

/**

    Get Bank Index for Mci_Misc for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval Mci_Misc Index

**/
UINT32
GetAddrMcBankMisc (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->CtlMsr + 0x3;
  }
  return MSR_IA32_INVALID;
}



/**
  Disables CSMI at MC bank

  @param McBank - McBank number

**/
VOID
EFIAPI
DisableCsmiAtMcBank (
  IN UINT8 McBank
  )
{
  UINT32 McBankIdx;

  McBankIdx = GetAddrMcBankCtl2 (McBank);

  if (McBankIdx == MSR_IA32_INVALID) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "McBankIdx == MSR_IA32_INVALID\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  AsmMsrAnd64 (McBankIdx, ~BIT32);    // disable CMCI to SMI CTL
}

/**
  Access MC Bank MSRs to fill MC signature as defined in the EMCA SAS.

  @param[in]  McBankIdx   The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
  @param[in]  Socket      Socket ID.
  @param[in]  Instance    Unit instance number (unused).
  @param[out] McSig       A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
**/
VOID
EFIAPI
ReadMcBankSignature (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  )
{
  UINT32 MsrAddr;

  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;

  MsrAddr = GetAddrMcBankSts (McBankIdx);
  if (MsrAddr == MSR_IA32_INVALID) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID \n"));
    RAS_ASSERT (FALSE);
    return;
  }
  MciStsMsr.Data = AsmReadMsr64 (MsrAddr);

  //
  // Fill Error Signature
  //
  McSig->Signature.McSts = MciStsMsr.Data;

  if (MciStsMsr.Bits.addrv == 1) {
    MsrAddr = GetAddrMcBankAddr (McBankIdx);
    if (MsrAddr == MSR_IA32_INVALID) {
      RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID \n"));
      RAS_ASSERT (FALSE);
      return;
    }
    McSig->Signature.McAddr = AsmReadMsr64 (MsrAddr);
  } else {
    McSig->Signature.McAddr = 0;
  }

  if (MciStsMsr.Bits.miscv == 1) {
    MsrAddr = GetAddrMcBankMisc (McBankIdx);
    if (MsrAddr == MSR_IA32_INVALID) {
      RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID\n"));
      RAS_ASSERT (FALSE);
      return;
    }
    McSig->Signature.McMisc = AsmReadMsr64 (MsrAddr);
  } else {
    McSig->Signature.McMisc = 0;
  }
  McSig->ApicId = GetApicId ();

  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]ReadMcBankSignature 0x%x, Socket 0x%x: ApicId 0x%x, McSts 0x%lx McAddr 0x%lx McMisc 0x%lx\n",
              McBankIdx, Socket, McSig->ApicId, McSig->Signature.McSts, McSig->Signature.McAddr, McSig->Signature.McMisc));
}

/**
  Access MC Bank MSRs to fill MC signature as defined in the EMCA SAS.

  If MC Bank information is invalid use the Shadow CSRs.

  @param[in]  McBankIdx   The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
  @param[in]  Socket      Socket ID.
  @param[in]  Instance    Unit instance number.
  @param[out] McSig       A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
**/
VOID
EFIAPI
ReadM2MemMcBankSignature (
  IN  UINT32                        McBankIdx,
  IN  UINT8                         Socket,
  IN  UINT8                         Instance,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD  *McSig
  )
{
  UINT32                      Mc5Shadow0Status;
  UINT32                      Mc5Shadow1Status;
  UINT32                      Mc5Shadow0Addr;
  UINT32                      Mc5Shadow1Addr;
  UINT32                      Mc5Shadow0Misc;
  UINT32                      Mc5Shadow1Misc;
  IA32_MCI_STATUS_MSR_STRUCT  MciStsMsr;

  ReadMcBankSignature (McBankIdx, Socket, Instance, McSig);
  MciStsMsr.Data = McSig->Signature.McSts;

  if (MciStsMsr.Bits.val == 0) {

    Mc5Shadow1Status = GetM2mMciStatusShadowN1 (Socket, Instance);
    Mc5Shadow0Status = GetM2mMciStatusShadowN0 (Socket, Instance);

    McSig->Signature.McSts = ((UINT64) Mc5Shadow1Status << 32) | Mc5Shadow0Status;
    MciStsMsr.Data = McSig->Signature.McSts;

    if (MciStsMsr.Bits.val && MciStsMsr.Bits.addrv) {
      Mc5Shadow0Addr = GetM2mMciAddrShadow0 (Socket, Instance);
      Mc5Shadow1Addr = GetM2mMciAddrShadow1 (Socket, Instance);

      McSig->Signature.McAddr = (LShiftU64((UINT64) Mc5Shadow1Addr, 32)) | (Mc5Shadow0Addr);

    } else {
      McSig->Signature.McAddr = 0;
    }

    if (MciStsMsr.Bits.val && MciStsMsr.Bits.miscv) {
      Mc5Shadow0Misc = GetM2mMciMiscShadowN0 (Socket, Instance);
      Mc5Shadow1Misc = GetM2mMciMiscShadowN1 (Socket, Instance);

      McSig->Signature.McMisc = (LShiftU64((UINT64) Mc5Shadow1Misc, 32)) | (Mc5Shadow0Misc);

    } else {
      McSig->Signature.McMisc = 0;
    }

    McSig->ApicId = GetApicId ();

    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]ReadM2MemMcBankSignature 0x%x, Socket 0x%x: ApicId 0x%x, McSts 0x%lx McAddr 0x%lx McMisc 0x%lx\n",
                McBankIdx, Socket, McSig->ApicId, McSig->Signature.McSts, McSig->Signature.McAddr, McSig->Signature.McMisc));
  }
}

/**

    Call Bank specific Signature hook.

    @param McBankIdx -  The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param Socket    -  Socket Id
    @param McSig     -  A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.

**/
VOID
EFIAPI
ReadMcBankSigHook (
  IN UINT32 McBankIdx,
  IN UINT8  Socket,
  OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
  )
{
  MCA_READ_MCBANK_SIG_HOOK ReadMcBankSig = NULL;
  EMCA_MCBANK_DESC* Desc;

  Desc = GetMachineCheckBank (McBankIdx);

  if (McSig == NULL) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "McSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (Socket >= MAX_SOCKET) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "Socket >= MAX_SOCKET \n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (Desc != NULL) {
    ReadMcBankSig = Desc->ReadMcBankSig;
  }

  if (ReadMcBankSig == NULL) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "ReadMcBankSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  ReadMcBankSig (McBankIdx, Socket, Desc->Instance, McSig);
}


/**

    Write updated values for corrected error in the McBank. The Firmware updated bit must be set.

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param McSig     - A pointer to Machine Check Signature Error Record. See Related definitions and EMCA SAS.
                               The values in the signature will be written to the banks.

**/
VOID
EFIAPI
WriteMcBank (
  IN UINT32                       McBankIdx,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig
  )
{
  IA32_MCI_STATUS_MSR_STRUCT McSts;
  UINT32                     MsrAddr;

  if (McSig == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return;
  }
  McSts.Data = McSig->Signature.McSts;

  if (McSts.Bits.val == 1 && (McSts.Data & MCA_FIRMWARE_UPDATED_BIT) != 0) {
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]WriteMcBank 0x%x, ApicId 0x%x, McSts 0x%lx McAddr 0x%lx McMisc 0x%lx\n",
                McBankIdx, McSig->ApicId, McSig->Signature.McSts, McSig->Signature.McAddr, McSig->Signature.McMisc));

    MsrAddr = GetAddrMcBankSts (McBankIdx);
    if (MsrAddr == MSR_IA32_INVALID) {
      RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID\n"));
      RAS_ASSERT (FALSE);
      return;
    }
    AsmWriteMsr64 (MsrAddr, McSts.Data);

    if (McSts.Bits.miscv == 1) {
      MsrAddr = GetAddrMcBankMisc (McBankIdx);
      if (MsrAddr == MSR_IA32_INVALID) {
        RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID\n"));
        RAS_ASSERT (FALSE);
        return;
      }
      AsmWriteMsr64 (MsrAddr, McSig->Signature.McMisc);
    }

    if (McSts.Bits.addrv == 1) {
      MsrAddr = GetAddrMcBankAddr (McBankIdx);
      if (MsrAddr == MSR_IA32_INVALID) {
        RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID\n"));
        RAS_ASSERT (FALSE);
        return;
      }
      AsmWriteMsr64 (MsrAddr, McSig->Signature.McAddr);
    }
  }
}

/**

  Clear the value in a given McBank.

  @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

  @retval None
**/
VOID
EFIAPI
ClearMcBank (
  IN UINT32 McBankIdx
  )
{
  UINT32 MsrAddr;

  MsrAddr = GetAddrMcBankSts(McBankIdx);
  AsmWriteMsr64(MsrAddr, 0);

  MsrAddr = GetAddrMcBankMisc(McBankIdx);
  AsmWriteMsr64(MsrAddr, 0);

  MsrAddr = GetAddrMcBankAddr(McBankIdx);
  AsmWriteMsr64(MsrAddr, 0);

}

/**

  Sets firmware updated bit values for corrected error in the McBank.

  @param McBankInx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

**/
VOID
EFIAPI
SetMcBankFirmwareUpdatedBit (
  IN UINT32                 McBankIdx
  )
{
  IA32_MCI_STATUS_MSR_STRUCT        MciStsMsr;
  UINT32                            MsrAddr;

  MsrAddr = GetAddrMcBankSts (McBankIdx);
  if (MsrAddr == MSR_IA32_INVALID) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "MsrAddr == MSR_IA32_INVALID\n"));
    RAS_ASSERT (FALSE);
    return;
  }
  MciStsMsr.Data = AsmReadMsr64 (MsrAddr);
  MciStsMsr.Bits.uc = 1;
  MciStsMsr.Bits.val = 1;
  MciStsMsr.Bits.pcc = 1;
  MciStsMsr.Bits.over = 1;
  MciStsMsr.Data |= MCA_FIRMWARE_UPDATED_BIT;
  AsmWriteMsr64 (MsrAddr,MciStsMsr.Data);

  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Set Mc bank 0x%x firmware update bit MSR(0x%x) = 0x%lx\n",
              McBankIdx, MsrAddr, MciStsMsr.Data));
}


/**
  Get MC class

  @param   McErrSig     Signature structure

  @return MC exception class

**/
MCE_CLASS
EFIAPI
GetMceClass (
  IN EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  )
{
  IA32_MCI_STATUS_MSR_STRUCT        MciStsMsr;
  MSR_IA32_MCG_CAP_REGISTER         McgCapReg;

  MciStsMsr.Data = McErrSig->Signature.McSts;

  if (MciStsMsr.Bits.val == 1) {
    if (MciStsMsr.Bits.uc == 1) {
      if (MciStsMsr.Bits.pcc == 0) {

        McgCapReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);

        if (McgCapReg.Bits.MCG_SER_P) {
          if (MciStsMsr.Bits.s == 1) {
            if (MciStsMsr.Bits.ar == 1) {
              RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class SRAR\n", MciStsMsr.Data));
              return SRAR;
            } else {
              RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class SRAO\n", MciStsMsr.Data));
              return SRAO;
            }
          } else {
            RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class UCNA\n", MciStsMsr.Data));
            return UCNA;
          }
        } else {
          RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class FATAL\n", MciStsMsr.Data));
          return FATAL;
        }
      } else {
        RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class FATAL\n", MciStsMsr.Data));
        return FATAL;
      }
    } else {
      RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class CORRECTED\n", MciStsMsr.Data));
      return CORRECTED;
    }
  } else {
    RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]MC status 0x%lx, class INVALID\n", MciStsMsr.Data));
    return INVALID;
  }
}

/**
  Get MC bank physical address

  @param   McErrSig     Signature structure

  @return  physical address of machine check bank
  @retval  MAX_ADDRESS  The physical address in machine check bank is not valid

**/
PHYSICAL_ADDRESS
EFIAPI
GetMcAddress (
  IN EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  )
{
  IA32_MCI_STATUS_MSR_STRUCT  MciStsMsr;
  UINT64                      Mask;
  UINT64                      McAddress;
  UINT8                       Lsb;

  MciStsMsr.Data = McErrSig->Signature.McSts;
  if (MciStsMsr.Bits.addrv == 1 && MciStsMsr.Bits.miscv == 1) {
    McAddress = McErrSig->Signature.McAddr;

    Lsb = McErrSig->Signature.McMisc & MCA_MISC_LSB_MASK;
    Mask = ~((1 << Lsb) - 1);

    McAddress &= Mask;

    return McAddress;
  } else {
    return MAX_ADDRESS;
  }
}

/**
  Get machine check bank instance

  @param   BankIndex     Machine check bank index

  @return  machine check bank instance
  @retval  NA  The instance is not valid
**/
UINT8
EFIAPI
GetMcBankInstance (
  IN UINT32 McBankIdx
  )
{
  EMCA_MCBANK_DESC* Desc;
  Desc = GetMachineCheckBank (McBankIdx);

  if (Desc != NULL) {
    return Desc->Instance;
  }
  return NA;
}

/**
  Get machine M2Mem McBank Index

  @param   MC_Index     memory controller index

  @return  McBank Index
  @retval  McBank Index
**/
UINT32
EFIAPI
GetM2MemMcBankIndex (
  IN        UINT8   MC_Index
  )
{

  if (MC_Index >= MAX_IMC ){
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: MC_Index >= MAX_IMC\n"));
    RAS_ASSERT (FALSE);
  }

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return (M2M0_BANK_OFFSET_07 + MC_Index*M2M0_BANK_GAP_01);
  }
  else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)  \
               || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
             return (M2M0_BANK_OFFSET_12 + MC_Index*M2M0_BANK_GAP_04);
  }
  else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL))  {
             return (M2M0_BANK_OFFSET_12);
  }
  else {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: DONT KNOW THIS CPU!\n"));
  }

  return (M2M0_BANK_OFFSET_12 + MC_Index*M2M0_BANK_GAP_04);

}
