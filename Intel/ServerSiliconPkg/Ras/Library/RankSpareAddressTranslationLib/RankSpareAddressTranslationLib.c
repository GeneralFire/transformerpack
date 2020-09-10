/** @file
  Implementation of rank spare memory decode library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <Library/AddressDecodeLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/DebugLib.h>
#include <Library/MemMapDataLib.h>

BOOLEAN mRankSpare = FALSE;

/**

   If Rank Sparing happened, set the RankSparing Bit in the RAS field
   update the bad Rank Id with the Spare Rank

  @param TRANSLATED_ADDRESS  - ptr to Address Translation structure

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  Row
  Col
  Bank

  @retval EFI_SUCCESS / Error code
CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_5

**/
VOID
EFIAPI
RankSparingSupportForAddressTranslation (
  IN  VOID   *InputTa,
  IN  UINT8   TranslationMode     //Forward = 1 , Reverse = 2
  ){

  UINT8 Socket;
  UINT8 Node;
  UINT8 Mc;
  UINT8 Channel;
  UINT8 Rank, PhysicalRank;
  MEMRAS_S3_PARAM *S3Data;
  PTRANSLATED_ADDRESS TA;


  TA = (PTRANSLATED_ADDRESS)InputTa;

  Socket = TA->SocketId;
  Node = TA->Node;
  Channel = TA->ChannelId;
  PhysicalRank = TA->PhysicalRankId;
  Mc = NODE_TO_MC (Node);

  Rank = (TA->DimmSlot * 4) + PhysicalRank; //Chip Select
  if (mRankSpare) {
    S3Data = GetS3RasData ();

    if (TranslationMode == FORWARD_ADDRESS_TRANSLATION) {

      if ((S3Data->SpareRank[Node][Channel].SpareInUse[0] == 1)
          && (S3Data->SpareRank[Node][Channel].OldLogicalRank[0] == Rank)) {

        TA->ChipSelect = S3Data->SpareRank[Node][Channel].SpareLogicalRank[0];

        //
        //handle cointinous sparing. A->B, B->C   ---> A->C
        //
        if ((S3Data->SpareRank[Node][Channel].SpareInUse[1] == 1)
            && (S3Data->SpareRank[Node][Channel].OldLogicalRank[1] == TA->ChipSelect)) {
          TA->ChipSelect = S3Data->SpareRank[Node][Channel].SpareLogicalRank[1];

        }
      }

      if ((S3Data->SpareRank[Node][Channel].SpareInUse[1] == 1)
          && (S3Data->SpareRank[Node][Channel].OldLogicalRank[1] == Rank)) {
        TA->ChipSelect = S3Data->SpareRank[Node][Channel].SpareLogicalRank[1];

      }

      TA->PhysicalRankId = (TA->ChipSelect % MAX_RANK_DIMM);
      TA->RasModesApplied |= BIT0;
    } else if (TranslationMode == REVERSE_ADDRESS_TRANSLATION) {

      if ((S3Data->SpareRank[Node][Channel].SpareInUse[1] == 1)
          && (S3Data->SpareRank[Node][Channel].SpareLogicalRank[1] == Rank)) {
        TA->ChipSelect = S3Data->SpareRank[Node][Channel].OldLogicalRank[1];

        //
        //handle cointinous sparing. A->B, B->C   ---> A->C
        //
        if ((S3Data->SpareRank[Node][Channel].SpareInUse[0] == 1)
            && (S3Data->SpareRank[Node][Channel].SpareLogicalRank[0] == TA->ChipSelect)) {
          TA->ChipSelect = S3Data->SpareRank[Node][Channel].OldLogicalRank[0];
        }

      }

      if ((S3Data->SpareRank[Node][Channel].SpareInUse[0] == 1)
          && (S3Data->SpareRank[Node][Channel].SpareLogicalRank[0] == Rank)) {
        TA->ChipSelect = S3Data->SpareRank[Node][Channel].OldLogicalRank[0];

      }

      TA->PhysicalRankId = (TA->ChipSelect % MAX_RANK_DIMM);
      TA->RasModesApplied |= BIT0;
    }
  }

  if (TA->RasModesApplied) {
    DEBUG ((EFI_D_ERROR, "\nRankSpareSupportForAddrTrans: TA->ChipSelect:0x%08lx",TA->ChipSelect));
    DEBUG ((EFI_D_ERROR, "\nRankSpareSupportForAddrTrans: TA->PhysicalRankId:0x%08lx",TA->PhysicalRankId));
    DEBUG ((EFI_D_ERROR, "\nRankSpareSupportForAddrTrans: TA->RasModesApplied:0x%08lx\n",TA->RasModesApplied));
  }

}


/**

  The constructor function initialize rank spare address translation lib.
  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitRankSpareAddressTranslationLib (
  VOID
  )
{
  struct SystemMemoryMapHob  *SystemMemoryMap = NULL;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    DEBUG ((EFI_D_ERROR, "  [Error]InitRankSpareAddressTranslationLib: Get memory map data failed!"));
    ASSERT (SystemMemoryMap != NULL);
    return EFI_SUCCESS;
  }

  mRankSpare = SystemMemoryMap->RasModesEnabled & RK_SPARE;

  return EFI_SUCCESS;
}

