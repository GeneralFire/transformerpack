/** @file
  EMCA common definitions.

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

#ifndef __EMCA_H__
#define __EMCA_H__

#define MODE_UMC      0
#define MODE_EMCAGEN1 1 // DEPRECATED
#define MODE_EMCA     2

#define MC_SCOPE_TD    0x0
#define MC_SCOPE_CORE  0x1
#define MC_SCOPE_PKG   0x2
#define MCSCOPE_MASK  (BIT0 | BIT1)
#define MC_SCOPE_ERROR 0xFFFF

#define EMCA_SECTYPE_NONE       0x0
#define EMCA_SECTYPE_PROCGEN    0x1
#define EMCA_SECTYPE_PROCIA32   0x2
#define EMCA_SECTYPE_PROCIPF    0x3
#define EMCA_SECTYPE_PLATMEM    0x4
#define EMCA_SECTYPE_PCIE       0x5
#define EMCA_SECTYPE_FWERR      0x6
#define EMCA_SECTYPE_PCIBUS     0x7
#define EMCA_SECTYPE_PCIDEV     0x8
#define EMCA_SECTYPE_DMAR       0x9
#define EMCA_SECTYPE_VTIO       0xA
#define EMCA_SECTYPE_IOMMU      0xB
#define EMCA_SECTYPE_RAW        0xC
#define EMCA_SECTYPE_INVALID    0xFF

#define MCA_UNIT_TYPE_IFU   (BIT0)
#define MCA_UNIT_TYPE_DCU   (BIT1)
#define MCA_UNIT_TYPE_DTLB  (BIT2)
#define MCA_UNIT_TYPE_MLC   (BIT3)
#define MCA_UNIT_TYPE_PCU   (BIT4)
#define MCA_UNIT_TYPE_KTI   (BIT5)
#define MCA_UNIT_TYPE_IIO   (BIT6)
#define MCA_UNIT_TYPE_CHA   (BIT7)
#define MCA_UNIT_TYPE_M2MEM (BIT8)
#define MCA_UNIT_TYPE_IMC   (BIT9)
#define MCA_UNIT_TYPE_MEE   (BIT10)
#define MCA_UNIT_TYPE_MEC   (BIT11)
#define MCA_UNIT_TYPE_FEC   (BIT12)
#define MCA_UNIT_TYPE_CH    (BIT13)
#define MCA_UNIT_TYPE_BUNIT (BIT14)
#define MCA_UNIT_TYPE_PUNIT (BIT15)
#define MCA_UNIT_TYPE_DUNIT (BIT16)
#define MCA_UNIT_TYPE_BUS   (BIT17)
#define MCA_UNIT_TYPE_L2    (BIT18)
#define MCA_UNIT_TYPE_MDF   (BIT19)
#define MCA_UNIT_TYPE_IMB   (BIT21)
#define MCA_UNIT_TYPE_LLC   (BIT22)
#define MCA_UNIT_TYPE_MSE   (BIT23)
#define MCA_UNIT_TYPE_NONE  (BIT24)

#define MCA_UNIT_TYPE_ERROR  0xFFFFFFFF

#define GET_BUS_LOG_MC6_MISC(a) (((a) & 0xFF000000) >> 24)
#define GET_DEVICE_LOG_MC6_MISC(a) (((a) & 0xF80000) >> 19)
#define GET_FUNCTION_LOG_MC6_MISC(a) (((a) & 0x70000) >> 16)
#define GET_SEGMENT_LOG_MC6_MISC(a) (((a) & 0xE000) >> 13)

typedef struct {
  UINT64 McSts;
  UINT64 McAddr;
  UINT64 McMisc;
} EMCA_MC_BANK_SIGNATURE;

typedef struct {
  UINT32 ApicId;
  UINT32 BankIndex;
  EMCA_MC_BANK_SIGNATURE Signature;
} EMCA_MC_SIGNATURE_ERR_RECORD;

#endif
