/** @file
  Implementation of memory function library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <Library/RasDebugLib.h>
#include <Library/MemFuncLib.h>
#include <SiliconSetting.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Include/Ras/RasCommon.h>

/**
  Get channel error info.

  This implementation is to get failed DIMM info from retry register.
  It doesn't provide buddy info as it related to feature policy.

  @param[in]  Node            Memory controller index per whole system.
  @param[in]  ChOnNode        Channel index on memory controller.
  @param[in]  UcError         1 for uncorrectable error, 0 for correctable error.
  @param[out] FailedDimmInfo  Error info.

  @retval EFI_SUCCESS         DIMM error found, error info returned.
  @retval EFI_NOT_FOUND       No DIMM error found.
**/
EFI_STATUS
EFIAPI
GetChannelErrorInfo (
  IN  UINT8             Node,
  IN  UINT8             ChOnNode,
  IN  UINT64            SystemAddress,
  OUT FAILED_DIMM_INFO  *FailedDimmInfo
  )
{
  UINT8                                     AdddcRegionFaildevice;
  UINT8                                     Socket = NODE_TO_SKT (Node);
  UINT8                                     SktCh = NODECHA_TO_SKTCHA (Node,ChOnNode);
  UINT8                                     AddcRegNum = 00;
  UINT32                                    EccMode;
  BOOLEAN                                   Valid;
  UINT8                                     Uc;
  UINT8                                     Rank;
  UINT8                                     SubRank;
  UINT8                                     Bank;
  UINT8                                     Dev;
  UINT8                                     SingleBit;
  UINT8                                     Transfer;
  UINT16                                    InterLocSyn = 0;
  UINT32                                    Col;
  UINT32                                    Row;
  UINT8                                     FailedBankGroupInRetryReg;
  UINT8                                     FailedBankInRetryReg;
  UINT8                                     AdddcRegionEnable;
  UINT8                                     AdddcFailRank = 0;
  UINT8                                     AdddcFailSubRank = 0;
  UINT8                                     AdddcNonFailRank = 0;
  UINT8                                     AddddcNonFailSubRank = 0;
  UINT8                                     AddddcRegionSize = 0;
  UINT8                                     AddddcFailedBank = 0;
  UINT8                                     AddddcNonFailedBank = 0;
  BOOLEAN                                   Is14nmCpu;
  BOOLEAN                                   BankErrorInfoFound;

  Valid = GetRetryReadInfo (
            Socket,
            SktCh,
            SystemAddress,
            &Uc,
            &EccMode,
            &Rank,
            &SubRank,
            &Bank,
            &Dev,
            &Col,
            &Row,
            &SingleBit,
            &Transfer,
            &InterLocSyn
            );
  if (!Valid) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "RETRY_RD_ERR_LOG_MCDDC_DP_REG is not valid\n"));
    return EFI_NOT_FOUND;
  }

  RAS_DEBUG ((
    LEVEL_BASIC_FLOW,
    "[GetChannelErrorInfo]:raw data EccMode is 0x%x. Uncorrect is 0x%x  "
    "Rank:0x%x SubRank:0x%x Bank:0x%x Dev:0x%x Col:0x%x  Row:0x%x  SingleBit:0x%x  Transfer:0x%x  InterLocSyn:0x%x\n",
    EccMode,
    Uc,
    Rank,
    SubRank,
    Bank,
    Dev,
    Col,
    Row,
    SingleBit,
    Transfer,
    InterLocSyn
    ));

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    FailedBankGroupInRetryReg = Bank & (BIT0 | BIT1);
    FailedBankInRetryReg      = Bank >> 02;
  } else {
    FailedBankGroupInRetryReg = (Bank & (BIT0 | BIT1)) | ((Bank & BIT4) >> 2);
    FailedBankInRetryReg      = (Bank >> 02) & (BIT0 | BIT1);
  }

  FailedDimmInfo->Node    = Node;
  FailedDimmInfo->NodeCh  = ChOnNode;

  //
  // SDDC 2LM/1LM, DDRT
  // error in non adddc region
  //
  FailedDimmInfo->FailedRank = Rank;
  FailedDimmInfo->FailedSubRank = SubRank;
  FailedDimmInfo->FailedDev = Dev;
  FailedDimmInfo->FailedBank = FailedBankInRetryReg;
  FailedDimmInfo->FailedBG = FailedBankGroupInRetryReg;
  FailedDimmInfo->FailedCol = Col;
  FailedDimmInfo->FailedRow = Row;
  FailedDimmInfo->FailedDimm = FailedDimmInfo->FailedRank / 4;
  FailedDimmInfo->SingleBit = SingleBit;
  FailedDimmInfo->Transfer = Transfer;
  FailedDimmInfo->InterLocSyn = InterLocSyn;

  BankErrorInfoFound = FALSE;

  //
  //SDDC 2LM/1LM, DDRT
  //
  if ((EccMode == 0) || (EccMode == 1) || (EccMode == 8)) {
    FailedDimmInfo->RegionType = ErrInIndReg;
    return EFI_SUCCESS;
  } else {
    if (FailedDimmInfo->FailedDev >= ADDDC_DEVICE_SWITCH_NUMBER) {
      FailedDimmInfo->FailedDev -= ADDDC_DEVICE_SWITCH_NUMBER;

      for (AddcRegNum = 0; AddcRegNum < MAX_ADDDC_REGION; AddcRegNum++) {
        GetAdddcRegion (Socket, SktCh, AddcRegNum, &AdddcRegionEnable, &AdddcFailRank, &AdddcFailSubRank,
        &AdddcNonFailRank, &AddddcNonFailSubRank, &AddddcRegionSize, &AddddcFailedBank, &AddddcNonFailedBank);

        if (AdddcRegionEnable == 0) {
          continue;
        }

        if (FailedDimmInfo->FailedRank == (UINT8) AdddcFailRank) {
          FailedDimmInfo->PeerRank =(UINT8) AdddcNonFailRank;

          if (AddddcRegionSize == 0) {
            FailedDimmInfo->RegionType = ErrInAdddcBankFailureReg;
          } else {
            FailedDimmInfo->RegionType = ErrInAdddcRankFailureReg;
          }
          break;
        } else {
          FailedDimmInfo->PeerRank =(UINT8) AdddcFailRank;
          if (AddddcRegionSize == 0) {
            FailedDimmInfo->RegionType = ErrInAdddcBankBuddyReg;
          } else {
            FailedDimmInfo->RegionType = ErrInAdddcRankBuddyReg;
          }
          break;
        }

      }
      return EFI_SUCCESS;
    } else {

      for (AddcRegNum = 0; AddcRegNum < MAX_ADDDC_REGION; AddcRegNum++) {
        GetAdddcRegion (Socket, SktCh, AddcRegNum, &AdddcRegionEnable, &AdddcFailRank, &AdddcFailSubRank,
          &AdddcNonFailRank, &AddddcNonFailSubRank, &AddddcRegionSize, &AddddcFailedBank, &AddddcNonFailedBank);

        AdddcRegionFaildevice = GetAdddcRegionFaildevice (Socket, SktCh, AddcRegNum);

        if (AdddcRegionEnable == 0) {
          continue;
        }

        if ((AdddcFailRank == Rank) || ( AdddcNonFailRank == Rank)) {

          if (AddcRegNum < MAX_ADDDC_REGION) {

            if (AddddcRegionSize == 1) {
              //
              //if this is RANK level VLS then Bank information is not valid. So set the flag to true.
              //
              BankErrorInfoFound = TRUE;
            }
            Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);
            if (Is14nmCpu){
              //
              // due to https://vthsd.sc.intel.com/hsd/skylake_server/default.aspx#bugeco/default.aspx?bugeco_id=312014
              //        In case its ECC mode is  ADDDC or ADDDC + 1, and dev < 18, scan ADDDC region to figure out the one whoes failed_cs/non_failed_cs matches retry_rd_err.cs
              //  a.    If region is bank VLS, use ADDDC peer cs/subrank/bank.
              //  b.    If region is rank VLS,  use ADDDC peer cs.
              //
              if (AdddcFailRank == Rank) {
                FailedDimmInfo->FailedRank = AdddcNonFailRank;
                FailedDimmInfo->PeerRank = AdddcFailRank;
                if (AddddcRegionSize == 0) {
                  //
                  //bank VLS
                  //
                  FailedDimmInfo->FailedSubRank = AddddcNonFailSubRank;
                  FailedDimmInfo->FailedBank = BANK_IN_GROUP(AddddcNonFailedBank);
                  FailedDimmInfo->FailedBG = BANK_TO_GROUP(AddddcNonFailedBank);
                  BankErrorInfoFound = TRUE;
                }
              }

              if (AdddcNonFailRank == Rank) {
                FailedDimmInfo->FailedRank = AdddcFailRank;
                FailedDimmInfo->PeerRank = AdddcNonFailRank;
                if (AddddcRegionSize == 0) {
                  //
                  //bank VLS
                  //
                  FailedDimmInfo->FailedSubRank = AdddcFailSubRank;
                  FailedDimmInfo->FailedBank = BANK_IN_GROUP(AddddcFailedBank);
                  FailedDimmInfo->FailedBG = BANK_TO_GROUP(AddddcFailedBank);
                  BankErrorInfoFound = TRUE;
                }
              }
            }

            //
            //On ICX, the logged rank will the correct rank where the error is. But subrank and bank will always be the buddy value, regardless where the error is actually in.
            //
            if (Is14nmCpu == FALSE){
              if (AdddcFailRank == Rank) {
                if ((AddddcRegionSize == 0) && (FailedDimmInfo->FailedBank == BANK_IN_GROUP(AddddcNonFailedBank))) {
                  //
                  //bank VLS
                  //
                  FailedDimmInfo->FailedSubRank = AdddcFailSubRank;
                  FailedDimmInfo->FailedBank = BANK_IN_GROUP(AddddcFailedBank);
                  FailedDimmInfo->FailedBG = BANK_TO_GROUP(AddddcFailedBank);
                  BankErrorInfoFound = TRUE;
                }
              }

              if (AdddcNonFailRank == Rank) {
                if ((AddddcRegionSize == 0) && (FailedDimmInfo->FailedBank == BANK_IN_GROUP(AddddcFailedBank))) {
                  //
                  //bank VLS
                  //
                  FailedDimmInfo->FailedSubRank = AddddcNonFailSubRank;
                  FailedDimmInfo->FailedBank = BANK_IN_GROUP(AddddcNonFailedBank);
                  FailedDimmInfo->FailedBG = BANK_TO_GROUP(AddddcNonFailedBank);
                  BankErrorInfoFound = TRUE;
                }
              }
            }

            FailedDimmInfo->FailedDimm = FailedDimmInfo->FailedRank / (MAX_RANK_DIMM * SUB_CH);
          }


          if (FailedDimmInfo->FailedRank == (UINT8) AdddcFailRank) {
            FailedDimmInfo->PeerRank =(UINT8) AdddcNonFailRank;

            if (AddddcRegionSize == 0) {
              FailedDimmInfo->RegionType = ErrInAdddcBankFailureReg;
            } else {
              FailedDimmInfo->RegionType = ErrInAdddcRankFailureReg;
            }
          } else {
            FailedDimmInfo->PeerRank =(UINT8) AdddcFailRank;

            if (AddddcRegionSize == 0) {
              FailedDimmInfo->RegionType = ErrInAdddcBankBuddyReg;
            } else {
              FailedDimmInfo->RegionType = ErrInAdddcRankBuddyReg;
            }
          }

          if ((AdddcRegionFaildevice == Dev) &&
             ((FailedDimmInfo->RegionType == ErrInAdddcBankFailureReg) || (FailedDimmInfo->RegionType == ErrInAdddcRankFailureReg))) {
            FailedDimmInfo->FailedDev = ADDDC_SPARE_DEVICE;
          }
          if (BankErrorInfoFound == TRUE) {
            break;
          }
        }
      }
    }
  }

  //
  //change needed for CPX/ICX/SPR given current RTL implementation for the failed dev (for SPR, it is device 8 instead of device 16 since 
  //DDR5 has only 10 devices total) if the rank is in adddc mode and
  //and correction_debug_log.adddc_meta_bit_failed is set
  //
  if (FailedDimmInfo->RegionType != ErrInIndReg) {
    if (CheckADDDCMetaBit(Socket, SktCh)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetChannelErrorInfo : adddc_meta_bit_failed \n"));
      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
        FailedDimmInfo->FailedDev = DEVICE_NUMBER16;
      }
    }
  }
  return EFI_SUCCESS;
}
