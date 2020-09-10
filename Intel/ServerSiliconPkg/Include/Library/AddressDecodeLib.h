/** @file
  Interface of Address decode library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef __ADRESS_DECODE_LIB_H__
#define __ADRESS_DECODE_LIB_H__

#include <Library/MemTypeLib.h>

#define   RANK_INTL_MIN_CLOSE     6
#define   RANK_INTL_MIN_OPEN      13

#define   FORWARD_ADDRESS_TRANSLATION  1
#define   REVERSE_ADDRESS_TRANSLATION  2

#define   SOCKET_VALID            BIT0
#define   MEMTYPE_VALID           BIT1
#define   DIMMTYPE_VALID          BIT2
#define   MC_VALID                BIT3
#define   CHN_ADDR_VALID          BIT4
#define   CHN_VALID               BIT5
#define   DPA_VALID               BIT6
#define   CHIP_SELECT_VALID       BIT7
#define   PYSICAL_RANK_VALID      BIT8
#define   DIMM_VALID              BIT9
#define   COLUMN_VALID            BIT10
#define   ROW_VALID               BIT11
#define   BANK_VALID              BIT12
#define   BANK_GROUP_VALID        BIT13
#define   SUB_RANK_VALID          BIT14
#define   NM_MC_VALID             BIT15
#define   NM_CHN_VALID            BIT16
#define   NM_CHN_ADDR_VALID       BIT17
#define   NM_CHIP_SELECT_VALID    BIT18
#define   NM_PYSICAL_RANK_VALID   BIT19
#define   NM_DIMM_VALID           BIT20
#define   NM_COLUMN_VALID         BIT21
#define   NM_ROW_VALID            BIT22
#define   NM_BANK_VALID           BIT23
#define   NM_BANK_GROUP_VALID     BIT24
#define   RANK_ADDRESS_VALID      BIT25
#define   NM_RANK_ADDRESS_VALID   BIT26
#define   TARGET_WAY_VALID        BIT27
#define   CHANNEL_WAY_VALID       BIT28


typedef enum {
  ddr3dimmType = 0,
  ddr4dimmType = 1,
  ddrtdimmType = 2,
  HbmdimmType = 3,
  dimmTypeUnknown
} TRANSLATED_DIMM_TYPE;

typedef struct _TRANSLATED_ADDRESS {
  UINT64               Flag;
  UINT64               SystemAddress;
  UINT64               NmSystemAddress;
  UINT64               SpareSystemAddress;
  UINT64               DPA;
  UINT8                SocketId;
  UINT8                SadId; //internal
  UINT8                TadId; //internal
  UINT8                MemoryControllerId;
  UINT8                NmMemoryControllerId;
  UINT8                TargetId; //internal
  UINT8                LogChannelId; //internal
  UINT8                TargetWays; //internal
  UINT8                ChannelWays; //internal
  UINT64               ChannelAddress; //internal
  UINT64               SecChannelAddress; //internal
  UINT64               NmChannelAddress; //internal
  UINT8                ChannelId;
  UINT8                SecChannelId;
  UINT8                NmChannelId;
  UINT64               RankAddress; //internal
  UINT64               SecRankAddress; //internal
  UINT64               NmRankAddress; //internal
  UINT8                PhysicalRankId; //rank on the DIMM  - external
  UINT8                SecPhysicalRankId; //rank on the DIMM  - external
  UINT8                NmPhysicalRankId;
  UINT8                DimmSlot;
  UINT8                SecDimmSlot;
  UINT8                NmDimmSlot;
  UINT8                DimmRank; // internal
  UINT32               Row;
  UINT32               SecRow;
  UINT32               NmRow;
  UINT32               Col;
  UINT32               SecCol;
  UINT32               NmCol;
  UINT8                Bank;
  UINT8                SecBank;
  UINT8                NmBank;
  UINT8                BankGroup;
  UINT8                SecBankGroup;
  UINT8                NmBankGroup;
  UINT8                LockStepRank;
  UINT8                LockStepPhysicalRank;
  UINT8                LockStepBank;
  UINT8                LockStepBG;
  UINT8                ChipSelect; //rank on the chn - external
  UINT8                NmChipSelect;
  UINT8                SecChipSelect;
  UINT8                Node; //internal
  UINT8                ChipId; //sub-rank
  UINT8                NmChipId;
  UINT8                SecChipId;
  UINT8                RasModesApplied;  //BIT0 = Rank sparing, BIT1 = RANK VLS, BIT2 = BANK VLS BIT3 = MIRROR
  MEM_TYPE             MemType; //need to add persistent Memmory/block decode/
  TRANSLATED_DIMM_TYPE DimmType; //for FM
  UINT32               StatusToDsmInterface;
} TRANSLATED_ADDRESS, *PTRANSLATED_ADDRESS;

/**
  Translate a system address to a DIMM address.

  @param[in]      SystemAddress       System address to be translated
  @param[out]     TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       System address was invalid
**/
EFI_STATUS
EFIAPI
SystemAddressToDimmAddress (
  IN      UINTN               SystemAddress,
  OUT     PTRANSLATED_ADDRESS TranslatedAddress
  );

/**
  Translate a DIMM address to system address.

  @param[in,out]  TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       DIMM address was null or invalid
**/
EFI_STATUS
EFIAPI
DimmAddressToSystemAddress (
  IN OUT  PTRANSLATED_ADDRESS TranslatedAddress
  );


/**
  check whether this system address belong to CXL bus.

  @param[in]       SystemAddress    System address to be translated
  @param[in, out]  Socket           Point to socket. If not NULL, it tells this CXL bus belong to which socket.

  @retval          TRUE  ---  this address belong to CXL bus.
                   FALSE ---  this address not belong to CXL bus.

**/
BOOLEAN
EFIAPI
IsCxlAddress (
  IN        UINTN               SystemAddress,
  IN OUT    UINT8              *Socket
  );
#endif
