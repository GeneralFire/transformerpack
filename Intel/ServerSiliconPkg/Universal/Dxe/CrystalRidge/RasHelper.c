/** @file RasHelper.c
  RAS helper for CR driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include <Guid/LastBootErrorLogHob.h>
#include <Cpu/Msr/McaMsr.h>
#include "CrystalRidge.h"
#include <Library/MemTypeLib.h>
#include "Ars/ShortArs.h"

// Needed RAS defines (temporary solution)

#define DCU_POISON_DETECTED 0x10
#define IFU_POISON_DETECTED 0xC

#define CORE_IFU_BANK       0
#define CORE_DCU_BANK       1

#define MEM_IMC0_CH0_BANK   13
#define MEM_IMC1_CH2_BANK   18

#define MEM_IMC_MCBANK_NUM_START  MEM_IMC0_CH0_BANK
#define MEM_IMC_MCBANK_NUM_END    MEM_IMC1_CH2_BANK

#define MEM_M2M_0_BANK            7
#define MEM_M2M_1_BANK            8

//Memory mcbank offsets
#define MEM_M2M_MCBANK_NUM_START  MEM_M2M_0_BANK
#define MEM_M2M_MCBANK_NUM_END    MEM_M2M_1_BANK

/**
  Checks if specified MCBANK_ERR_INFO structure contains a cache poison error.

  @param[in]  McBankInfoPtr  Pointer to MCBANK_ERR_INFO structure.

  @retval     TRUE           Cache poison error identified.
  @retval     FALSE          No cache poison error.

**/
STATIC
BOOLEAN
IsCachePoisonError (
  IN MCBANK_ERR_INFO * McBankInfoPtr
  )
{
  IA32_MCI_STATUS_MSR_STRUCT                  McBankSts;

  if (McBankInfoPtr == NULL) {
    return FALSE;
  }

  //Only cover IFU and DCU poison detection case here.
  if (McBankInfoPtr->McBankNum != CORE_IFU_BANK && McBankInfoPtr->McBankNum != CORE_DCU_BANK) {
    return FALSE;
  }

  McBankSts.Data = McBankInfoPtr->McBankStatus;

  if (McBankInfoPtr->McBankNum == CORE_IFU_BANK && McBankSts.Bits.ms_code == IFU_POISON_DETECTED) {
    return TRUE;
  }

  if (McBankInfoPtr->McBankNum == CORE_DCU_BANK && McBankSts.Bits.ms_code == DCU_POISON_DETECTED) {
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if gPrevBootErrSrcHobGuid HOB contains fatal errors within PMEM regions.
  Any errors found are added to Short ARS scrub list.

  @param[out]  Found                 Pointer to a BOOLEAN to store result in.
                                     TRUE:  fatal error in PMEM region found.
                                     FALSE: no error in PMEM regions found.

  @retval      EFI_SUCCESS           HOB examined completely.
  @retval      EFI_NOT_FOUND         HOB not found.
  @retval      EFI_COMPROMISED_DATA  HOB corrupted.

**/
EFI_STATUS
CheckPreviousDcpmmError (
  OUT BOOLEAN * Found
  )
{

  EFI_HOB_GUID_TYPE          *GuidHob;
  UINT16                     HobSize;
  PREV_BOOT_ERR_SRC_HOB      *PrevBootErrSrcHob;
  UINT16                     Index;
  UINT16                     Left;
  UINT16                     ItemLength;
  BOOLEAN                    IntFound = FALSE;
  MCBANK_ERR_INFO            *McBankErrInfo;
  IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatusReg;
  EFI_STATUS                 Status;
  UINT64                     SpaBase;
  UINT8                      Socket;
  UINT8                      Channel;
  UINT8                      Dimm;
  UINT16                     SadType;

  GuidHob = GetFirstGuidHob (&gPrevBootErrSrcHobGuid);
  if (GuidHob == NULL) {
    CRDEBUG ((DEBUG_INFO, "CheckPreviousPMemError: gPrevBootErrSrcHobGuid not found!\n"));
    return EFI_NOT_FOUND;
  }
  HobSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
  if (HobSize < 2) {
    return EFI_COMPROMISED_DATA;
  }
  PrevBootErrSrcHob = (PREV_BOOT_ERR_SRC_HOB *) GET_GUID_HOB_DATA (GuidHob);
  CRDEBUG ((DEBUG_INFO, "CheckPreviousPMemError: Hob Length: %d\n", PrevBootErrSrcHob->Length));
  if (HobSize < PrevBootErrSrcHob->Length) {
    return EFI_COMPROMISED_DATA;
  }

  Left = PrevBootErrSrcHob->Length - 2;

  Index = 0;

  while (Left) {
    switch (PrevBootErrSrcHob->HobData[Index]) {
    case McBankType:
      ItemLength = sizeof (MCBANK_ERR_INFO);
      break;
    case PciExType:
      ItemLength = sizeof (PCI_EX_ERR_INFO);
      break;
    default:
      ItemLength = 0;
      CRDEBUG ((DEBUG_ERROR, "CheckPreviousPMemError: unknown entry (type %d) in gPrevBootErrSrcHobGuid!\n", PrevBootErrSrcHob->HobData[Index]));
      break;
    }
    if (ItemLength == 0) {
      break;
    }
    if (Left < ItemLength) {
      CRDEBUG ((DEBUG_ERROR, "CheckPreviousPMemError: gPrevBootErrSrcHobGuid corrupted!\n"));
      break;
    }
    if (PrevBootErrSrcHob->HobData[Index] == McBankType) {
      McBankErrInfo = (MCBANK_ERR_INFO *) &PrevBootErrSrcHob->HobData[Index];
      // Check for memory Mcbank or cache poisoned errors
      if ((McBankErrInfo->McBankNum <= MEM_IMC_MCBANK_NUM_END && McBankErrInfo->McBankNum >= MEM_IMC_MCBANK_NUM_START) ||
          (McBankErrInfo->McBankNum <= MEM_M2M_MCBANK_NUM_END && McBankErrInfo->McBankNum >= MEM_M2M_MCBANK_NUM_START) ||
          IsCachePoisonError (McBankErrInfo)) {
        MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;

        //check for fatal error.
        if (MsrIa32MCiStatusReg.Bits.uc == 1 && MsrIa32MCiStatusReg.Bits.addrv) {
          CRDEBUG ((DEBUG_INFO, "CheckPreviousPMemError: found fatal error at SPA 0x%16llx\n", McBankErrInfo->McbankAddr));
          Status = GetDimmInfoFromSpa (McBankErrInfo->McbankAddr, &SpaBase, &Socket, &Channel, &Dimm, &SadType);
          if (EFI_ERROR (Status)) {
            if (Status != EFI_NOT_FOUND) {
              CRDEBUG ((DEBUG_ERROR, "CheckPreviousPMemError: GetDimmInfoFromSpa (0x%16llX): %r\n",  McBankErrInfo->McbankAddr, Status));
            }
          } else {
            CRDEBUG ((DEBUG_INFO, "CheckPreviousPMemError: error at S%d.C%d.D.%d, SadType: %d\n", Socket, Channel, Dimm, SadType));
            if (SadType == MemType1lmAppDirect) {
              IntFound = TRUE;
              Status = ShortArsAddError (McBankErrInfo->McbankAddr);
              if (EFI_ERROR (Status)) {
                CRDEBUG ((DEBUG_ERROR, "CheckPreviousPMemError: ShortArsAddError (0x%llx) failed: %r\n", McBankErrInfo->McbankAddr, Status));
                break;
              }
            }
          }
        }
      }
    }
    Index += ItemLength;
    Left -= ItemLength;
  }
  CRDEBUG ((DEBUG_INFO, "CheckPreviousPMemError: Left %d bytes in HobData\n", Left));
  if (!IntFound && Left) {
    return EFI_COMPROMISED_DATA;
  }
  if (Found) {
    *Found = IntFound;
  }
  return EFI_SUCCESS;
}
