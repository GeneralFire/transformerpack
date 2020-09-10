/** @file

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

#include "MemorySubClass.h"
#include "Platform.h"
#include "UncoreCommonIncludes.h"
#include <Protocol/SmbiosMemInfo.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemTypeLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/SysHostPointerLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/HbmCoreLib.h>

/*****************************************************************************
 * Definitions
 *****************************************************************************/
#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif

#if FixedPcdGet16(PcdSmbiosVersion) == 0x0302
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x34;   // Version 3.2 size (from the SMBIOS Specification)
#elif FixedPcdGet16(PcdSmbiosVersion) == 0x0208
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x28;   // Version 2.8 size (from the SMBIOS Specification)
#elif FixedPcdGet16(PcdSmbiosVersion) == 0x0207
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x22;   // Version 2.7 size (from the SMBIOS Specification)
#elif FixedPcdGet16(PcdSmbiosVersion) == 0x0206
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x1c;   // Version 2.6 size (from the SMBIOS Specification)
#elif FixedPcdGet16(PcdSmbiosVersion) == 0x0203
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x1b;   // Version 2.3 size (from the SMBIOS Specification)
#else
#define SMBIOS_TYPE17_DEFAULT_SIZE  0x15;   // Version 2.1 size (from the SMBIOS Specification)
#endif

/*****************************************************************************
 * Function prototypes
 *****************************************************************************/
EFI_STATUS
SmbiosMemInfoAddDimm (
  IN UINT8             Socket,
  IN UINT8             Imc,
  IN UINT8             Ch,
  IN UINT8             Dimm,
  IN EFI_SMBIOS_HANDLE Type17Handle,
  IN BOOLEAN           IsNvDimm
  );
EFI_STATUS
SmbiosMemInfoClear (VOID);
EFI_STATUS
SmbiosMemInfoInstall (VOID);

UINT8
GetBitmapFor2LMType (
  IN SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap,
  IN UINT8 Socket,
  IN UINT8 Imc
  );


/*****************************************************************************
 * Variables
 *****************************************************************************/
SMBIOS_MEM_INFO_PROTOCOL mSmbiosMemInfo;
UINT16                   mSmbiosDimmNum = 0;
struct {
  EFI_SMBIOS_HANDLE Type17Handle;
  BOOLEAN           IsNvDimm;
} mSmbiosDimmInfo[MAX_SOCKET][MAX_IMC][MAX_MC_CH][MAX_DIMM];

EFI_STRING_ID mDimmToDevLocator[] = {
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_0), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_8), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_9),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_10), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_11),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_12), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_13),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_14), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_15),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_16), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_17),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_18), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_19),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_1F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_20), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_21),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_22), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_23),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_24), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_25),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_26), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_27),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_28), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_29),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_2F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_30), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_31),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_32), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_33),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_34), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_35),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_36), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_37),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_38), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_39),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_3F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_40), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_41),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_42), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_43),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_44), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_45),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_46), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_47),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_48), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_49),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_4F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_50), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_51),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_52), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_53),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_54), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_55),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_56), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_57),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_58), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_59),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_5F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_60), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_61),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_62), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_63),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_64), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_65),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_66), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_67),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_68), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_69),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_6F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_70), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_71),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_72), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_73),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_74), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_75),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_76), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_77),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_78), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_79),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7A), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7C), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7E), STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_7F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_DIMM_80)
};

EFI_STRING_ID mHbmToDevLocator[] = {
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_00), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_01),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_02), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_03),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_04), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_05),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_06), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_07),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_08), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_09),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0A), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0C), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0E), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_0F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_10), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_11),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_12), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_13),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_14), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_15),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_16), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_17),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_18), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_19),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1A), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1C), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1D),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1E), STRING_TOKEN(STR_MEMORY_SUBCLASS_HBM_1F),
};

EFI_STRING_ID mDimmToBankLocator[] = {
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_00), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_01), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_02), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_03),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_04), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_05), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_06), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_07),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_08), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_09), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0A), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0C), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0D), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0E), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_0F),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_10), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_11), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_12), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_13),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_14), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_15), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_16), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_17),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_18), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_19), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1A), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1B),
  STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1C), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1D), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1E), STRING_TOKEN(STR_MEMORY_SUBCLASS_NODE_1F),
};

EFI_STRING_ID  mAssetTagString[]  = {
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_A1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_A2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_B1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_B2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_C1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_C2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_D1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_D2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_E1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_E2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_F1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_F2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_G1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_G2),
  STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_H1), STRING_TOKEN (STR_CPU1_DIMM_ASSETTAG_H2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_A1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_A2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_B1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_B2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_C1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_C2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_D1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_D2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_E1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_E2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_F1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_F2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_G1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_G2),
  STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_H1), STRING_TOKEN (STR_CPU2_DIMM_ASSETTAG_H2),
};

EFI_GUID mEfiMemorySubClassDriverGuid = EFI_MEMORY_SUBCLASS_DRIVER_GUID;

MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[] = {MEMORY_MODULE_MANUFACTURERS};


/**
  @brief Return position of next bit set in a bitmap.

  @param[in] Bitmap   Bitmap of to look for bit
  @param[in] StartAt  Bit position to start at

  @return The position of bit found in Bitmap, or FFh if not found.
**/
UINT8
BitmapGetBit (
  IN UINT32 Bitmap,
  IN UINT8  StartAt
  )
{
  for (Bitmap >>= StartAt; Bitmap != 0; Bitmap >>= 1, StartAt++) {

    if (Bitmap & 1) {

      return StartAt;
    }
  }
  return 0xFF;
}


/**
  @brief Count bits set in a bitmap.

  @param[in] Bitmap   The bitmap to count bit.

  @return Number of bits set in the bitmap is retuned.
**/
UINT8
BitmapGetBitCount (
  IN UINT32 Bitmap
  )
{
  UINT8     BitCnt;

  for (BitCnt = 0; Bitmap != 0; Bitmap >>= 1) {

    if (Bitmap & 1) {
      BitCnt++;
    }
  }
  return BitCnt;
}


CHAR8
FilterCharacter (
  IN    CHAR8   Character
  )
{
  if( (Character >= 0x20 && Character <= 0x7E) ) {
    return Character;
  }
  return ' ';
}

// APTIOV_SERVER_OVERRIDE_RC_START: Get IMC index
UINT8
GetMcIndexFromBitmap (
  UINT8 imcInterBitmap
  )
{
  UINT8 mc;

  // if this entry is IMC0/IMC1 interleaved then use MC0 since number of channels have to be the same > 1 IMC way
  if ((imcInterBitmap & BIT0) && (imcInterBitmap & BIT1)) {
    mc = 0;
  } else if (imcInterBitmap & BIT1) { // if this entry for MC1 only
    mc = 1;
  } else { // if this entry for MC0 only
    mc = 0;
  }
  return mc;
}
// APTIOV_SERVER_OVERRIDE_RC_END

/**

Routine Description:
   Get Logical to Physical rank ID

Arguments:
    NodeId
    DdrCh
    Logical Rank

Returns:
  Physical Rank Id

--*/
UINT8
GetLogicalToPhysicalRankId (
  IN UINT8  NodeId,
  IN UINT8  DdrCh,
  IN UINT8  LogicalRank
  )
{
  UINT8   csMapping2DPC[] = {0, 1, 2, 3, 4, 5, 6, 7};
  UINT8   phyRank = 0xFF;

  if (LogicalRank < 8) {
    phyRank = csMapping2DPC[LogicalRank];
  }

  return phyRank;
}


/*++
Routine Description:
  This function is invoked when calculating spare DIMM size.
  Assuming that all Logical Rank size is the same on this Channel,
  because of cannot get spare rank size from the SystemMemoryMapHob.

Arguments:
  SystemMemoryMap
  NodeId
  DdrCh
  EnabledLogicalRanks

Returns:
  Rank Size.

--*/
UINT16
GetRankSize (
  IN SYSTEM_MEMORY_MAP_HOB      *SystemMemoryMap,
  IN UINT8                       NodeId,
  IN UINT8                       DdrCh,
  IN UINT8                       EnabledLogicalRanks
  )
{
  UINT8           Dimm = 0;
  UINT16          ChnnelSize = 0;
  UINT8           Index;
  UINT8           SpareRank = 0xFF;
  UINT8           SpareRankNum = 0;
  UINT8           EnableRankNum = 0;
  UINT16          RankSize = 0;
  //
  // Calculate total effective size on this Channel
  //
  for(Dimm=0; Dimm < MAX_DIMM; Dimm++){
    if(SystemMemoryMap->Socket[NodeId].ChannelInfo[DdrCh].DimmInfo[Dimm].Present != 0){
      ChnnelSize = ChnnelSize + (SystemMemoryMap->Socket[NodeId].ChannelInfo[DdrCh].DimmInfo[Dimm].DimmSize>>4);
    }
  }
  //
  // Calculate total spare rank number on this Channel
  //
  for(Index=0; Index< MAX_SPARE_RANK; Index++){
    SpareRank = SystemMemoryMap->Socket[NodeId].ChannelInfo[DdrCh].SpareLogicalRank[Index];
    if(SpareRank == 0xff){
      continue;
    }
    SpareRankNum++;
  }
  //
  // Calculate total enable rank number on this Channel
  //
  for(Index=0; Index<MAX_RANK_CH; Index++){
    if((EnabledLogicalRanks>>Index) == 0){
      break;
    }
    if(((EnabledLogicalRanks>>Index) & 0x01) == 0x01){
      EnableRankNum++;
    }
  }
  //
  // Calculate Rank size.
  //
  if(EnableRankNum > SpareRankNum ){
    RankSize = ChnnelSize/(EnableRankNum-SpareRankNum);
  }
  return RankSize;
}

STATIC
BOOLEAN
CheckSpareDimm (
  IN SYSTEM_MEMORY_MAP_HOB      *SystemMemoryMap,
  IN UINT8                       node,
  IN UINT8                       Chl,
  IN UINT8                       Dimm
  )
{
   BOOLEAN    ReturnValue = FALSE;
   UINT8      i;
   UINT8      LogicalRank;
   UINT8      PhysicalRank;

   if((SystemMemoryMap->RasModesEnabled & RK_SPARE) == 0){
     return ReturnValue;
   }

   for(i = 0; i < MAX_SPARE_RANK; i++){
     LogicalRank = SystemMemoryMap->Socket[node].ChannelInfo[Chl].SpareLogicalRank[i];
     if(LogicalRank == 0xff){
       continue;
     }

     PhysicalRank = GetLogicalToPhysicalRankId(node,Chl,LogicalRank);
     if(Dimm == (PhysicalRank/4)){
       ReturnValue = TRUE;
       break;
     }
   }
   return     ReturnValue;
}

UINT8
GetDimmSpareRankNum (
  IN SYSTEM_MEMORY_MAP_HOB      *SystemMemoryMap,
  IN UINT8                       NodeId,
  IN UINT8                       DdrCh,
  IN UINT8                       Dimm
  )
{
   UINT8      Index;
   UINT8      LogicalRank = 0xFF;
   UINT8      PhysicalRank;
   UINT8      SpareRankNum = 0;
   for(Index=0; Index< MAX_SPARE_RANK; Index++){
     LogicalRank = SystemMemoryMap->Socket[NodeId].ChannelInfo[DdrCh].SpareLogicalRank[Index];
     if(LogicalRank == 0xFF){
       continue;
     }

     PhysicalRank = GetLogicalToPhysicalRankId(NodeId,DdrCh,LogicalRank);

     if(Dimm == (PhysicalRank/4)){
       SpareRankNum++;
     }
   }
   return SpareRankNum;
}


/**

  This function will return the number of flat memory regions for HBM

  @param[in] SystemMemoryMapHob   Pointer to the System Memory Map
  @param[in] Socket               Current socket

  @return                         Number of HBM flat regions within an specified socket
**/
UINT8
DetermineHbmFlatElement (
  IN SYSTEM_MEMORY_MAP_HOB         *SystemMemoryMap,
  IN UINT8                         Socket
  )
{
  UINT32   StartOffset;
  UINT32   EndOffset;
  UINT8    HbmFlatElements;


  //
  // Only sweep thru MAX_SAD_RULES on the specific socket
  //
  StartOffset = (Socket + 0) * MAX_SAD_RULES;
  EndOffset   = (Socket + 1) * MAX_SAD_RULES;
  HbmFlatElements = 0;

  for (;StartOffset < EndOffset; StartOffset++) {
    if (SystemMemoryMap->Element[StartOffset].Type == MemType1lmHbm) {
      HbmFlatElements++;
    }
  }
  DEBUG ((
    EFI_D_INFO,
    "TotalClusters: %d HbmFlatElements: %d\n",
    SystemMemoryMap->TotalClusters,
    HbmFlatElements
    ));
  if (SystemMemoryMap->TotalClusters > HbmFlatElements) {
    DEBUG ((
      EFI_D_INFO,
      "SystemMemoryMap->TotalClusters > HbmFlatElements\n"
      ));
  }

  return HbmFlatElements;

}


/**

  This function will get the index to the next Element related with HBM

  @param[in] SystemMemoryMapHob   Pointer to the System Memory Map
  @param[in] Socket               Current socket
  @param[in] PrevElementIndex     Previous index found

  @return                         Index to the next HBM flat element

**/
UINT32
GetNextHbmFlatElement (
  IN SYSTEM_MEMORY_MAP_HOB         *SystemMemoryMap,
  IN UINT8                         Socket,
  IN UINT32                        PrevElementIndex
  )
{
  UINT32   StartIndex;
  UINT32   EndIndex;

  //
  // Only sweep thru MAX_SAD_RULES on the specific socket
  //
  StartIndex  = PrevElementIndex;
  EndIndex    = (Socket + 1) * (MAX_DRAM_CLUSTERS * MAX_SAD_RULES);

  for (;StartIndex < EndIndex; StartIndex++) {
    if (SystemMemoryMap->Element[StartIndex].Type == MemType1lmHbm) {
      return StartIndex;
    }
  }

  return 0xFFFFFFFF;
}

/**

  GC_TODO: add routine description

  @param ImageHandle - GC_TODO: add arg description
  @param SystemTable - GC_TODO: add arg description

  @retval EFI_NOT_FOUND - GC_TODO: add retval description
  @retval Status        - GC_TODO: add retval description

**/
EFI_STATUS
EFIAPI
MemorySubClassEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HII_HANDLE                HiiHandle;
  EFI_STATUS                    Status;
  SYSTEM_MEMORY_MAP_HOB        *SystemMemoryMap;
  SYSTEM_MEMORY_MAP_ELEMENT    *MemElemPtr;
  UINT8                         MemElemIdx;
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT8                         Chan;
  BOOLEAN                       DimmPresent;

  EFI_SMBIOS_PROTOCOL          *SmbiosProtocol;
  struct {
    union {
      SMBIOS_TABLE_TYPE16       Type16;
      SMBIOS_TABLE_TYPE19       Type19;
      SMBIOS_TABLE_TYPE20       Type20;
    } Data;
    UINT8                       TrailingZero[2]; // SMBIOS protocol expects two zero bytes behind added record
  } Record;
  SMBIOS_TABLE_TYPE17           *Type17Record;
  EFI_SMBIOS_HANDLE             MemArrayType16Handle;
  EFI_SMBIOS_HANDLE             MemArrayMappedType19Handle;
  EFI_SMBIOS_HANDLE             MemDevType17Handle;
  UINTN                         TableSize;
  UINT32                        MemoryCapacity;
  UINT32                        MemoryDeviceSize;
  UINT16                        MemorySpeed;
  UINT16                        MemoryTotalWidth;
  UINT16                        MemoryDataWidth;
  CHAR8                         *OptionalStrStart;
  UINTN                         DeviceLocatorStrLen;
  UINTN                         BankLocatorStrLen;
  UINTN                         ManufactureStrLen;
  UINTN                         SerialNumberStrLen;
  UINTN                         AssertTagStrLen;
  UINTN                         PartNumberStrLen;
  EFI_STRING_ID                 DeviceLocator;
  EFI_STRING_ID                 BankLocator;
  EFI_STRING_ID                 Manufacturer;
  EFI_STRING_ID                 SerialNumber;
  EFI_STRING_ID                 AssetTag;
  EFI_STRING_ID                 PartNumber;
  EFI_STRING                    DeviceLocatorStr;
  EFI_STRING                    BankLocatorStr;
  EFI_STRING                    ManufactureStr;
  EFI_STRING                    SerialNumberStr;
  EFI_STRING                    AssertTagStr;
  EFI_STRING                    PartNumberStr;
  UINTN                         StringBufferSize;
  EFI_STRING                    StringBuffer;
  UINT32                        MemInfoTotalMem;
  UINT16                        MemInfoMemFreq;
  UINT16                        MinimumVoltage;
  UINT16                        MaximumVoltage;
  UINT16                        ConfiguredVoltage;
  UINT8                         Ranks;
  UINT8                         MemoryType;
  UINT8                         BitmapOfCachedChannels;

  MEMORY_MODULE_MANUFACTURER_LIST *IdTblPtr;
  UINT8                         PlatformMaxSocket;
  UINT8                         PlatformMaxSktCh;
  UINT8                         PlatformMaxChDimm;
  UINT8                         PlatformMaxSktDimm;
  UINT8                         MfdSize;
  UINT8                         MfdIndex;
  CHAR16                        *TempString=NULL;
  UINT16                        MaxMemorySpeed;
  UINT8                         SpdPartNum;
  UINT8                         i;
  CHAR16                        StringBuffer2[SMBIOS_STRING_MAX_LENGTH];
  UINT8                         Node;
  UINT32                        SpareDimmSize;
  CPU_CSR_ACCESS_VAR            *CpuCsrAccessVarPtr;

  UINT8                         Type17StructureSize = SMBIOS_TYPE17_DEFAULT_SIZE; // Default is version 2.8 size
  UINT8                         DimmSku;
  UINT8                         MemoryTechnology, PlatformMemoryFormFactor;
  UINTN                         FirmwareVersionStrLen;
  EFI_STRING_ID                 FirmwareVersion;
  EFI_STRING                    FirmwareVersionStr;
  UINT16                        ModuleManufacturerId;
  UINT16                        ModuleProductId;
  UINT16                        MemorySubsystemControllerManufacturerId;
  UINT16                        MemorySubsystemControllerProductId;
  UINT64                        NonVolatileSize;
  UINT64                        VolatileSize;
  UINT64                        CacheSize;
  UINT64                        LogicalSize;
  UINT32                        RemainingDdrCacheSize[MAX_SOCKET][MAX_CH];
  UINT32                        PlatformMaxDimmSize;
  BOOLEAN                       IsHalfWidth = FALSE;
  UINT8                         DimmType = 0;


  IsHalfWidth = PcdGetBool (PcdHalfWidth);

  // APTIOV_SERVER_OVERRIDE_RC_START
   UINT8                         MemoryFormFactor;  // Added changes for NO DIMM populated.
   //
   // Update  Type 19,17 Structure.
   //
   UINT8                         PartitionWidth = 0;
   EFI_STRING                            StringBufferAsset;

   //UINT8                         Type;
   //EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord;
   EFI_SMBIOS_HANDLE                         SearchType19Handle = SMBIOS_HANDLE_PI_RESERVED;
//   UINT8                         SadIndex;
//   UINT8                         ChannelInterBitmap;
//   UINT8                         ChInterWays;
   // APTIOV_SERVER_OVERRIDE_RC_END

  SmbiosMemInfoClear ();
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {

    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }
  //
  // Check if MRC code has set the PCDs for platform or not
  //
  MemInfoTotalMem = (UINT32)(SystemMemoryMap->memSize << 6);
  MemInfoMemFreq  = SystemMemoryMap->memFreq;

  //
  // Use HBM frequency for HBM only configuration.
  //
  if ((MemInfoMemFreq == 0) && IsHbmSku ()) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (CheckSocketEnabled (Socket)) {
        MemInfoMemFreq = HbmGetFrequencyInMhz (Socket);
        DEBUG ((DEBUG_WARN, "[SMBIOS] Use HBM frequency for HBM only configuration\n"));
        break;
      }
    }
  }

  if (MemInfoTotalMem == 0 || MemInfoMemFreq == 0) {
    DEBUG ((DEBUG_ERROR, "[SMBIOS] Invalid memory size/frequency! MemInfoTotalMem = %d, MemInfoMemFreq = %d\n",
      MemInfoTotalMem,
      MemInfoMemFreq
      ));
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "[SMBIOS] Total memory size = %d GiB\n", MemInfoTotalMem >> 10));
  DEBUG ((DEBUG_INFO, "[SMBIOS] Memory speed = %d MHz\n", MemInfoMemFreq));

  //
  // Prepare local variable for CacheSize Calculation
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      //
      // Convert from 64MB to MB Granularity
      //
      RemainingDdrCacheSize[Socket][Ch] = SystemMemoryMap->DdrCacheSize[Socket][Ch] << CONVERT_64MB_TO_MB_GRAN;
    }
  }
  //
  // Allocate Buffers
  //
  StringBufferSize  = (sizeof (CHAR16)) * SMBIOS_STRING_MAX_LENGTH;
  StringBuffer      = AllocateZeroPool (StringBufferSize);
  ASSERT (StringBuffer != NULL);
  //
  // APTIOV_SERVER_OVERRIDE_RC_START : Allocate for Asset tag
  //
  StringBufferAsset = AllocatePool (StringBufferSize);
  ASSERT (StringBufferAsset != NULL);
  //
  // APTIOV_SERVER_OVERRIDE_RC_END : Allocate for Asset tag
  //

  //
  // Locate dependent protocols
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, &SmbiosProtocol);
  if (EFI_ERROR (Status)) {

    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  //
  // Add our default strings to the HII database. They will be modified later.
  //
  HiiHandle = HiiAddPackages (
                &mEfiMemorySubClassDriverGuid,
                NULL,
                MemorySubClassStrings,
                NULL,
                NULL
                );
  ASSERT_EFI_ERROR (Status);

  TempString = AllocatePool (StringBufferSize);
  ASSERT (TempString != NULL);

  PlatformMaxSocket = (UINT8)PcdGet32 (PcdOemSkuBoardSocketCount);

  //
  // If this board supports more than one socket
  //
  if (PlatformMaxSocket > 1) {
    //
    // Count the enabled sockets in the system by parsing SYSTEM_MEMORY_MAP_HOB
    //
    for (Socket = 0, PlatformMaxSocket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SystemMemoryMap->Socket[Socket].SocketEnabled == 1) {
        PlatformMaxSocket++;
      }
    }
  }

  PlatformMaxSktCh    = (UINT8)PcdGet32 (PcdOemSkuMaxChannel);
  PlatformMaxChDimm   = (UINT8)PcdGet32 (PcdOemSkuMaxDimmPerChannel);
  PlatformMaxSktDimm  = PlatformMaxSktCh * PlatformMaxChDimm;
  PlatformMaxDimmSize = PcdGet32 (PcdOemSkuMaxDimmSize);
  PlatformMemoryFormFactor  = PcdGet8 (PcdOemSkuMemDevFormFactor);

  if (!FeaturePcdGet (PcdCrystalRidgeEnable)) {

    MemoryCapacity = PlatformMaxSocket * PlatformMaxSktDimm * PlatformMaxDimmSize; // in GB

  } else {

    if (PlatformMaxChDimm == 1) {
      //
      // There should be at least 1 DDR4 DIMM per IMC and remaining can be (n - 1) DDRT DIMM per IMC
      //
      MemoryCapacity = PlatformMaxSocket * ((PlatformMaxSktCh - MAX_IMC) * MAX_APACHEPASS_DIMM_SIZE + MAX_IMC * PlatformMaxDimmSize);

    } else {
      //
      // There can only be one DDRT DIMM per channel so the max capacity is 1 DDRT DIMM per channel plus
      // (n - 1) DDR4 DIMMs per channel (where n is the total number of DIMM slots per channel).  In case
      // of 3 channels per iMC and 2 slots per channel, the max capacity per node (iMC) would be (x3 256GB
      // 16Gb 3DS DDR4 LRDIMM) and (x3 512GB Apache Pass mapped as memory).
      //
      MemoryCapacity = PlatformMaxSocket * (PlatformMaxSktCh * MAX_APACHEPASS_DIMM_SIZE + (PlatformMaxSktDimm - PlatformMaxSktCh) * PlatformMaxDimmSize);
    }
  }
  DEBUG ((DEBUG_INFO, "[SMBIOS] Memory capacity %d GiB (skts %d, skt channels %d, ch dimms %d, skt dimms %d)\n",
            MemoryCapacity, PlatformMaxSocket, PlatformMaxSktCh, PlatformMaxChDimm, PlatformMaxSktDimm));
  //
  // Convert DDR Voltage to millivolts
  //
  switch (SystemMemoryMap->DdrVoltage) {
    // Currently DDR4 only supports 1.2v & DDR5 only supports 1.1v
    case SPD_VDD_120 :
      MinimumVoltage = 1200;
      MaximumVoltage = 1200;
      ConfiguredVoltage = 1200;
      break;
    case SPD_VDD_110 :
      MinimumVoltage = 1100;
      MaximumVoltage = 1100;
      ConfiguredVoltage = 1100;
      break;
    default :
      MinimumVoltage = 0;
      MaximumVoltage = 0;
      ConfiguredVoltage = 0;
  }
  DEBUG ((DEBUG_INFO, "[SMBIOS] Configured DDR Voltage = %dmV\n", ConfiguredVoltage));
  //
  // Type 16 - Create one single Physycal Memory Array Type 16 record for the whole system.
  //
  ZeroMem (&Record, sizeof (Record));
  Record.Data.Type16.Hdr.Type = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
  Record.Data.Type16.Hdr.Length = sizeof (SMBIOS_TABLE_TYPE16);
  Record.Data.Type16.Hdr.Handle = 0;
  Record.Data.Type16.Location = MemoryArrayLocationSystemBoard;
  Record.Data.Type16.Use = MemoryArrayUseSystemMemory;
  switch (SystemMemoryMap->RasModesEnabled) {

    case CH_INDEPENDENT:
      Record.Data.Type16.MemoryErrorCorrection = MemoryErrorCorrectionSingleBitEcc;
      break;
    case FULL_MIRROR_1LM:
    case PARTIAL_MIRROR_1LM:
    case FULL_MIRROR_2LM:
    case PARTIAL_MIRROR_2LM:
      Record.Data.Type16.MemoryErrorCorrection = MemoryErrorCorrectionMultiBitEcc;;
      break;
    case RK_SPARE:
      Record.Data.Type16.MemoryErrorCorrection = MemoryErrorCorrectionSingleBitEcc;
      break;
    case CH_LOCKSTEP:
      Record.Data.Type16.MemoryErrorCorrection = MemoryErrorCorrectionSingleBitEcc;
      break;
    default:
      Record.Data.Type16.MemoryErrorCorrection = MemoryErrorCorrectionMultiBitEcc;
  }
  DEBUG ((DEBUG_INFO, "[SMBIOS] RasMode: 0x%x -> MemoryErrorCorrection: 0x%x\n",
          SystemMemoryMap->RasModesEnabled, Record.Data.Type16.MemoryErrorCorrection));
  //
  // If Max Capacity less than 2T, use MaximumCapacity to store the value, else use ExtendedMaximumCapacity to store it.
  //
  if (MemoryCapacity < 2048) {

    Record.Data.Type16.MaximumCapacity = MemoryCapacity << 20; // in KB

  } else {

    Record.Data.Type16.MaximumCapacity = 0x80000000;
    Record.Data.Type16.ExtendedMaximumCapacity = LShiftU64 (MemoryCapacity, 30); // in byte
  }
  Record.Data.Type16.MemoryErrorInformationHandle = 0xFFFE;
  Record.Data.Type16.NumberOfMemoryDevices = PlatformMaxSocket * PlatformMaxSktDimm;

  MemArrayType16Handle = SMBIOS_HANDLE_PI_RESERVED;
  Status = SmbiosProtocol->Add (SmbiosProtocol, NULL, &MemArrayType16Handle, &Record.Data.Type16.Hdr);
  DEBUG ((DEBUG_INFO, "[SMBIOS] Added Type16 (locaton %d, use %d, correction %d, capacity 0x%X/0x%llX",
          Record.Data.Type16.Location, Record.Data.Type16.Use, Record.Data.Type16.MemoryErrorCorrection,
          Record.Data.Type16.MaximumCapacity, Record.Data.Type16.ExtendedMaximumCapacity));
  DEBUG ((DEBUG_INFO, ", devices %d) - handle 0x%04X\n",
          Record.Data.Type16.NumberOfMemoryDevices, MemArrayType16Handle));
  ASSERT_EFI_ERROR (Status);
  // APTIOV_SERVER_OVERRIDE_RC_START Added changes to fill Extended Type 17 details
  SystemMemoryMap->MemSetup.ExtendedType17 = 1;
  // APTIOV_SERVER_OVERRIDE_RC_END Added changes to fill Extended Type 17 details

  for (Socket = 0; Socket < PlatformMaxSocket; Socket++) {

    for (Node = 0; Node < MAX_IMC; Node++) {

      BitmapOfCachedChannels = GetBitmapFor2LMType (SystemMemoryMap, Socket, Node);
      //
      // Building Type 17 structures
      // Loop for each memory node
      //
      MfdSize = sizeof (MemoryModuleManufacturerList) / sizeof (MemoryModuleManufacturerList[0]);

      MemorySpeed = 0;
      MaxMemorySpeed = 0;
      for (Ch = 0; Ch < MAX_MC_CH; Ch++) { // Detect DIMMs on each channel
        for (Dimm = 0; Dimm < PlatformMaxChDimm; Dimm++) {
          DimmPresent = TRUE;
          Chan = MEM_IMCCH_TO_SKTCH (Node, Ch);

          if (IsHalfWidth == TRUE) {
            if ((Ch == 1 && Dimm == 1) || (Ch ==2 && Dimm == 1))
            continue;
          }

          if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].Present== 0) {
            DimmPresent = FALSE;
          }
          DeviceLocator = mDimmToDevLocator[(Socket * MAXIMUM_CHANNEL_PER_SOCKET * MAX_DIMM) + (Chan * MAX_DIMM) + Dimm]; //Zero based index
          BankLocator   = mDimmToBankLocator[(Socket * MAXIMUM_IMC_PER_SOCKET) + Node]; //Zero based index

          if (IsHalfWidth == TRUE) {
            if (DimmPresent == TRUE) {
              AssetTag = mAssetTagString[Socket * MAX_CH * MAX_DIMM + Chan * MAX_DIMM + Dimm]; //Zero based index  //STRING_TOKEN (STR_MEMORY_SUBCLASS_SPACE);
            } else {
              AssetTag = STRING_TOKEN (STR_MEMORY_SUBCLASS_SPACE);
            }
          } else {
            AssetTag    = STRING_TOKEN (STR_MEMORY_SUBCLASS_SPACE);
          }

          SerialNumber  = STRING_TOKEN (STR_MEMORY_SUBCLASS_UNKNOWN);
          PartNumber    = STRING_TOKEN (STR_MEMORY_SUBCLASS_UNKNOWN);
          FirmwareVersion = STRING_TOKEN (STR_MEMORY_SUBCLASS_UNKNOWN);

          if (SystemMemoryMap->MemSetup.ExtendedType17) {
            Type17StructureSize = sizeof (SMBIOS_TABLE_TYPE17);
          }
          if (DimmPresent) {
            // Update Manufacturer
            IdTblPtr = MemoryModuleManufacturerList;
            ZeroMem(StringBuffer,StringBufferSize);
            for (MfdIndex = 0; MfdIndex < MfdSize; MfdIndex++) {
              if ((IdTblPtr[MfdIndex].MfgId == SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].SPDMMfgId)){
                CopyMem (StringBuffer, IdTblPtr[MfdIndex].String,(2*StrLen(IdTblPtr[MfdIndex].String)));
                break;
              }
            }
            Manufacturer = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            //
            // Update SerialNumber
            //
            ZeroMem(StringBuffer2,StringBufferSize);
            ZeroMem(StringBuffer,StringBufferSize);
            ZeroMem (TempString, StringBufferSize);
            if ((SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].manufacturingDate & 0xF0) == 0x00) {
              StrCpyS (TempString, SMBIOS_STRING_MAX_LENGTH, L"0");
              if ((SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].manufacturingDate & 0xFF) == 0x00) {
                StrCatS (TempString, SMBIOS_STRING_MAX_LENGTH, L"0");
                if ((SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].manufacturingDate & 0xFF00) <= 0x0F00) {
                  StrCatS (TempString, SMBIOS_STRING_MAX_LENGTH, L"0");
                }
              }
            }
            UnicodeValueToStringS (
              StringBuffer2,
              StringBufferSize,
              RADIX_HEX,
              ((SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].manufacturingDate & 0xFF) << 8 |
               (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].manufacturingDate & 0xFF00) >> 8),
              4
              );
            StrCatS (TempString, SMBIOS_STRING_MAX_LENGTH, StringBuffer2);
            StrCatS (TempString, SMBIOS_STRING_MAX_LENGTH, L"-");
            StrCatS (StringBuffer, SMBIOS_STRING_MAX_LENGTH, TempString);
            for (i = 0; i <  4; i++) {
              ZeroMem(TempString, StringBufferSize);
              if ( SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].serialNumber[i] <= 0x0F) {
                StrCpyS(TempString, SMBIOS_STRING_MAX_LENGTH, L"0");
              }
              UnicodeValueToStringS (
                  StringBuffer2,
                  StringBufferSize,
                  RADIX_HEX,
                  SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].serialNumber[i],
                  2
                  );

              StrCatS (TempString, SMBIOS_STRING_MAX_LENGTH, StringBuffer2);
              StrCatS (StringBuffer, SMBIOS_STRING_MAX_LENGTH, TempString);
            }
            SerialNumber = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            //
            // Update PartNumber
            //
            SpdPartNum = 0;
            ZeroMem(StringBuffer,StringBufferSize);
            ZeroMem(TempString, StringBufferSize);
            for (i = 0; i < 20; i++) {
              SpdPartNum = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].PartNumber[i];
              if (SpdPartNum == 0) {
                SpdPartNum = 0x20;
              }
              TempString[0] = FilterCharacter(SpdPartNum);
              StringBuffer[i] = TempString[0];
            }
            PartNumber = HiiSetString (HiiHandle, 0, StringBuffer, NULL);

            if (SystemMemoryMap->MemSetup.ExtendedType17) {
              // Update FirmwareVersion
              ZeroMem(StringBuffer,StringBufferSize);
              ZeroMem(TempString, StringBufferSize);
              for (i = 0; i < 4; i++) {
                TempString[0] = FilterCharacter(SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].FirmwareVersionStr[i]);
                StringBuffer[i] = TempString[0];
              }
              StringBuffer[4] = 0x20;
              FirmwareVersion = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            }

            if (CheckSpareDimm (SystemMemoryMap, Socket, Chan, Dimm)) {
              SpareDimmSize = GetDimmSpareRankNum (SystemMemoryMap, Socket, Chan, Dimm) * GetRankSize(SystemMemoryMap, Socket, Chan, SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].EnabledLogicalRanks);
              MemoryDeviceSize = (UINT32)((UINT16)SpareDimmSize << 4) + (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DimmSize);
            } else {
              MemoryDeviceSize = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DimmSize; //in 64MB
            }
            MemoryDeviceSize = MemoryDeviceSize << 6; // in MB

            if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DcpmmPresent) {
              //
              // Since NVMCTLR DIMMs have a single rank and since there is only one NVMCTLR DIMM per channel
              // Check SPD byte 178, which shows as 1,2,4 ranks for 128/256/512 GB NVMCTLR DIMMs
              //
              Ranks = 1;
              MemoryType = MemoryTypeLogicalNonVolatileDevice;
            } else {
              Ranks = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NumRanks;
              if (SystemMemoryMap->DramType == SPD_TYPE_DDR5) {
                MemoryType = MemoryTypeDdr5;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NvDimmNPresent) {
                MemoryType = MemoryTypeLogicalNonVolatileDevice;
              } else {
                MemoryType = MemoryTypeDdr4;
              }
            }
            // APTIOV_SERVER_OVERRIDE_RC_START : Added changes for NO DIMM populated.
            MemoryFormFactor = MemoryFormFactorDimm;
            // APTIOV_SERVER_OVERRIDE_RC_END : Added changes for NO DIMM populated.
            DEBUG ((DEBUG_INFO, "[SMBIOS] S%d.C%d.D%d: Size = %d GiB\n", Socket, Chan, Dimm, MemoryDeviceSize >> 10));
            MemorySpeed = MemInfoMemFreq;

              if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_8400_TCK_MIN) {
                MaxMemorySpeed = 8400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_6400_TCK_MIN) {
                MaxMemorySpeed = 6400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_6000_TCK_MIN) {
                MaxMemorySpeed = 6000;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_5600_TCK_MIN) {
                MaxMemorySpeed = 5600;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_5200_TCK_MIN) {
                MaxMemorySpeed = 5200;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_4800_TCK_MIN) {
                MaxMemorySpeed = 4800;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_4400_TCK_MIN) {
                MaxMemorySpeed = 4400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_4266_TCK_MIN) {
                MaxMemorySpeed = 4266;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_4200_TCK_MIN) {
                MaxMemorySpeed = 4200;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_4000_TCK_MIN) {
                MaxMemorySpeed = 4000;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3800_TCK_MIN) {
                MaxMemorySpeed = 3800;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3733_TCK_MIN) {
                MaxMemorySpeed = 3733;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3600_TCK_MIN) {
                MaxMemorySpeed = 3600;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3466_TCK_MIN) {
                MaxMemorySpeed = 3466;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3400_TCK_MIN) {
                MaxMemorySpeed = 3400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3200_TCK_MIN) {
                MaxMemorySpeed = 3200;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_3000_TCK_MIN) {
                MaxMemorySpeed = 3000;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2933_TCK_MIN) {
                MaxMemorySpeed = 2933;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2800_TCK_MIN) {
                MaxMemorySpeed = 2800;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2666_TCK_MIN) {
                MaxMemorySpeed = 2666;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2600_TCK_MIN) {
                MaxMemorySpeed = 2600;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2400_TCK_MIN) {
                MaxMemorySpeed = 2400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2200_TCK_MIN) {
                MaxMemorySpeed = 2200;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2133_TCK_MIN) {
                MaxMemorySpeed = 2133;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_2000_TCK_MIN) {
                MaxMemorySpeed = 2000;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1866_TCK_MIN) {
                MaxMemorySpeed = 1866;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1800_TCK_MIN) {
                MaxMemorySpeed = 1800;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1600_TCK_MIN) {
                MaxMemorySpeed = 1600;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1400_TCK_MIN) {
                MaxMemorySpeed = 1400;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1333_TCK_MIN) {
                MaxMemorySpeed = 1333;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1200_TCK_MIN) {
                MaxMemorySpeed = 1200;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1066_TCK_MIN) {
                MaxMemorySpeed = 1066;
              } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].commonTck <= DDR_1000_TCK_MIN) {
                MaxMemorySpeed = 1000;
              } else {
                MaxMemorySpeed = 800;
              }

              ///
              /// @todo Need a non-hard-coded way to get these
              ///
        if (SystemMemoryMap->DramType == SPD_TYPE_DDR5) {
          MemoryTotalWidth  = MemoryTotalWidth_DDR5;
          MemoryDataWidth   = MemoryDataWidth_DDR5;
        }else {
          MemoryTotalWidth  = MemoryTotalWidth_DDR4;
          MemoryDataWidth   = MemoryTotalWidth - 8;
        }

            // SMBIOS 3.2 fields
            if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DcpmmPresent) {
              MemoryTechnology = MemoryTechnologyIntelPersistentMemory;
            } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NvDimmNPresent) {
              MemoryTechnology = MemoryTechnologyNvdimmN;
            } else {
              MemoryTechnology = MemoryTechnologyDram;
            }
            DimmSku = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DimmSku;

            ModuleManufacturerId = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].VendorID;
            ModuleProductId = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DeviceID;
            MemorySubsystemControllerManufacturerId = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].SubsystemVendorID;
            MemorySubsystemControllerProductId = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].SubsystemDeviceID;

            // Logical arrays not supported
            if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DcpmmPresent) {
              NonVolatileSize = (UINT64)SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].nonVolCap << 26; // in Bytes
              VolatileSize = (UINT64)SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].VolCap << 26; // in Bytes
              CacheSize = 0;
              LogicalSize = 0;
            } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NvDimmNPresent) {
              NonVolatileSize = LShiftU64 (MemoryDeviceSize, CONVERT_MB_TO_B); // in Bytes 0;
              VolatileSize = 0;
              CacheSize = 0;
              LogicalSize = 0;
            } else {
              NonVolatileSize = 0;
              if ((SystemMemoryMap->volMemMode == VOL_MEM_MODE_1LM) || (SystemMemoryMap->CacheMemType != CACHE_TYPE_DDR_CACHE_PMEM)) {
                //
                // DDR is 1LM or far memory
                //
                VolatileSize = LShiftU64 (MemoryDeviceSize, CONVERT_MB_TO_B); // in Bytes
                CacheSize = 0;
              } else if (SystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
                //
                // RemainingDdrCacheSize is specified on a per-channel basis. DIMM devices on a channel
                // are first allocated as Cache and any remaining capacity is allocated as Volatile
                //
                if (RemainingDdrCacheSize[Socket][Ch] == 0) {
                  //
                  // The RemainingDdrCacheSize may be zero under these conditions:
                  // 1. 1LM+2LM is enabled and there is DDR present but no DDRT present on this channel. This would
                  //    only occur on platforms that do not not support xtile caching.
                  // 2. The channel contains more than one DDR device (i.e. DDR4 on both slots 0 and 1). If the
                  //    entire RemainingDdrCacheSize is assigned to the first DIMM slot, the remaining
                  //    RemainingDdrCacheSize will be zero for the second DIMM slot.
                  //
                  VolatileSize = LShiftU64 (MemoryDeviceSize, CONVERT_MB_TO_B); // in Bytes
                  CacheSize = 0;
                } else if (RemainingDdrCacheSize[Socket][Ch] <= MemoryDeviceSize) {
                  VolatileSize = LShiftU64 ((MemoryDeviceSize - RemainingDdrCacheSize[Socket][Ch]), CONVERT_MB_TO_B); // in Bytes
                  CacheSize = LShiftU64 (RemainingDdrCacheSize[Socket][Ch], CONVERT_MB_TO_B); // in Bytes
                  //
                  // When the RemainingDdrCacheSize is less than or equal to the Memory Device Size, then the entire
                  // RemainingDdrCacheSize will be allocated to the current DIMM index. Any additional DDR DIMMs that
                  // make up the channel will see a remaining RemainingDdrCacheSize of zero.
                  //
                  RemainingDdrCacheSize[Socket][Ch] = 0;
                } else {
                  //
                  // The RemainingDdrCacheSize may be greater than the MemoryDeviceSize if the channel contains more
                  // than one DDR Device. For example, if the channel contains an 8GB DDR4 DIMM on DIMM slot 0, a 16GB
                  // DDR4 DIMM on slot 1 and the RemainingDdrCacheSize is 16GB, then only 8GB of the
                  // RemainingDdrCacheSize is assigned to DIMM slot 0 and the remaining 8GB is assigned to DIMM slot 1
                  // on the next iteration.
                  //
                  VolatileSize = 0;
                  CacheSize = LShiftU64 (MemoryDeviceSize, CONVERT_MB_TO_B); // in Bytes
                  RemainingDdrCacheSize[Socket][Ch] -= MemoryDeviceSize;
                }
              } else {
                VolatileSize = 0;
                CacheSize = LShiftU64 (MemoryDeviceSize, CONVERT_MB_TO_B); // in Bytes
              }
              LogicalSize = 0;
            }
          } else {
            StrCpyS (StringBuffer, SMBIOS_STRING_MAX_LENGTH, L"NO DIMM");

            if (IsHalfWidth == TRUE) {
              StrCpyS (StringBuffer, SMBIOS_STRING_MAX_LENGTH, L"Empty/NO DIMM");
              Manufacturer = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
              StrCpyS (StringBuffer, SMBIOS_STRING_MAX_LENGTH, L"NO DIMM");
            } else {
              Manufacturer    = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            }
            SerialNumber      = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            PartNumber        = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            MemoryDeviceSize  = 0;
            MemorySpeed       = 0;
            MemoryTotalWidth  = 0;
            MemoryDataWidth   = 0;
            Ranks             = 0;
            MaxMemorySpeed    = 0;
            MemoryType        = MemoryTypeUnknown;
              // APTIOV_SERVER_OVERRIDE_RC_START : Added changes for NO DIMM populated.
              MemoryFormFactor = MemoryFormFactorUnknown;
              // APTIOV_SERVER_OVERRIDE_RC_END : Added changes for NO DIMM populated.

            // SMBIOS 3.2 fields
            DimmSku = 0;
            MemoryTechnology = MemoryTechnologyUnknown;
            FirmwareVersion = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
            ModuleManufacturerId = 0;
            ModuleProductId = 0;
            MemorySubsystemControllerManufacturerId = 0;
            MemorySubsystemControllerProductId = 0;
            NonVolatileSize = 0;
            VolatileSize = 0;
            CacheSize = 0;
            LogicalSize = 0;
          }
          //
          // Get strings from HII database
          //
          DeviceLocatorStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, DeviceLocator, NULL);
          if (DeviceLocatorStr == NULL) {
            return EFI_UNSUPPORTED;
          }
          DeviceLocatorStrLen = StrLen (DeviceLocatorStr);
          ASSERT (DeviceLocatorStrLen <= SMBIOS_STRING_MAX_LENGTH);
        //
        // APTIOV_SERVER_OVERRIDE_RC_START : Update Asset Tag
        //
         if (DimmPresent) {
           StrCpy (TempString, L"_AssetTag");
           ZeroMem(StringBufferAsset,StringBufferSize);
           StrCat (StringBufferAsset, DeviceLocatorStr);
           StrCat (StringBufferAsset, TempString);
           AssetTag = HiiSetString (HiiHandle, 0, StringBufferAsset, NULL);
         } else {     
           StrCpy (StringBuffer, L"NO DIMM");  
           AssetTag = HiiSetString (HiiHandle, 0, StringBuffer, NULL);
         }
        //
        // APTIOV_SERVER_OVERRIDE_RC_END : Update Asset Tag
        //

          BankLocatorStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, BankLocator, NULL);
          BankLocatorStrLen = StrLen (BankLocatorStr);
          ASSERT (BankLocatorStrLen <= SMBIOS_STRING_MAX_LENGTH);

          ManufactureStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, Manufacturer, NULL);
          ManufactureStrLen = StrLen (ManufactureStr);
          ASSERT (ManufactureStrLen <= SMBIOS_STRING_MAX_LENGTH);

          SerialNumberStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, SerialNumber, NULL);
          SerialNumberStrLen = StrLen (SerialNumberStr);
          ASSERT (SerialNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

          AssertTagStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, AssetTag, NULL);
          AssertTagStrLen = StrLen (AssertTagStr);
          ASSERT (AssertTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

          PartNumberStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, PartNumber, NULL);
          PartNumberStrLen = StrLen (PartNumberStr);
          ASSERT (PartNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

          // SMBIOS 3.2 string
          FirmwareVersionStr = HiiGetPackageString (&mEfiMemorySubClassDriverGuid, FirmwareVersion, NULL);
          FirmwareVersionStrLen = StrLen (FirmwareVersionStr);
          ASSERT (FirmwareVersionStrLen <= SMBIOS_STRING_MAX_LENGTH);

          //
          // Report Memory Device to Type 17 SMBIOS Record
          //
          TableSize = Type17StructureSize + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1 + AssertTagStrLen + 1 + PartNumberStrLen + 1 + 1;
          if (SystemMemoryMap->MemSetup.ExtendedType17) {
            TableSize += FirmwareVersionStrLen + 1;
          }

          Type17Record = AllocatePool (TableSize);
          if (Type17Record == NULL) {
            ASSERT (Type17Record != NULL);
            return EFI_OUT_OF_RESOURCES;
          }
          ZeroMem (Type17Record, TableSize);
          Type17Record->Hdr.Type    = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
          Type17Record->Hdr.Length  = Type17StructureSize;
          Type17Record->Hdr.Handle  = 0;
          Type17Record->MemoryArrayHandle = MemArrayType16Handle;
          Type17Record->MemoryErrorInformationHandle = 0xFFFE;
          Type17Record->TotalWidth  = MemoryTotalWidth;
          Type17Record->DataWidth   = MemoryDataWidth;
          if (MemoryDeviceSize < 0x7fff) {
            Type17Record->Size = (UINT16)MemoryDeviceSize; // in MiB
            Type17Record->ExtendedSize = 0;
          } else {
            Type17Record->Size = 0x7fff; // Specifies size is in ExtendedSize field
            Type17Record->ExtendedSize = MemoryDeviceSize; // in MiB
          }
          Type17Record->FormFactor    = PlatformMemoryFormFactor;
          Type17Record->DeviceLocator = 1;
          Type17Record->BankLocator   = 2;
          Type17Record->MemoryType    = MemoryType;
          Type17Record->TypeDetail.Synchronous    = 1;

          if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].Enabled) {
            if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DcpmmPresent ||
            SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NvDimmNPresent) {
              Type17Record->TypeDetail.Nonvolatile  = 1;
            } else if (BitmapOfCachedChannels & (1 << Ch)) {
              Type17Record->TypeDetail.CacheDram = 1;
            }
          }
          Type17Record->Speed = MaxMemorySpeed; // in MHZ
          Type17Record->ConfiguredMemoryClockSpeed = ((MemorySpeed == 2132) ? 2133 : MemorySpeed); // in MHZ
          Type17Record->Manufacturer      = 3;
          Type17Record->SerialNumber      = 4;
          Type17Record->AssetTag          = 5;
          Type17Record->PartNumber        = 6;
          if (SystemMemoryMap->MemSetup.ExtendedType17) {
            Type17Record->FirwareVersion = 7;
          }
          Type17Record->Attributes        = Ranks;
          Type17Record->MinimumVoltage    = MinimumVoltage;
          Type17Record->MaximumVoltage    = MaximumVoltage;
          Type17Record->ConfiguredVoltage = ConfiguredVoltage;

          // Fill SMBIOS 3.2 fields
          if (SystemMemoryMap->MemSetup.ExtendedType17) {
            Type17Record->MemoryTechnology = MemoryTechnology;
            if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].DcpmmPresent) {
              // Set MemoryOperatingModeCapability for AEP based on DIMM SKU capabilities
              // Bit0:MemorymodeEnabled 1: StoragemodeEnabled 2:AppDirectmodeEnabled
              if (DimmSku & BIT0) {
                Type17Record->MemoryOperatingModeCapability.Bits.VolatileMemory = 1;
              }
              if (DimmSku & BIT1) {
                Type17Record->MemoryOperatingModeCapability.Bits.BlockAccessiblePersistentMemory = 1;
              }
              if (DimmSku & BIT2) {
                Type17Record->MemoryOperatingModeCapability.Bits.ByteAccessiblePersistentMemory = 1;
              }
            } else if (SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].NvDimmNPresent) {
              Type17Record->MemoryOperatingModeCapability.Bits.ByteAccessiblePersistentMemory = 1;
            } else {
              Type17Record->MemoryOperatingModeCapability.Bits.VolatileMemory = 1;
            }
            // APTIOV_SERVER_OVERRIDE_RC_START : Added changes for NO DIMM populated.
            if (!DimmPresent) {
               Type17Record->MinimumVoltage    = 0;
               Type17Record->MaximumVoltage    = 0;
               Type17Record->ConfiguredVoltage = 0; 
               Type17Record->TypeDetail.Unknown = 1;
               Type17Record->TypeDetail.Synchronous = 0;
               Type17Record->MemoryOperatingModeCapability.Bits.VolatileMemory= 0;
            }
            // APTIOV_SERVER_OVERRIDE_RC_END : Added changes for NO DIMM populated.
            Type17Record->ModuleManufacturerID = ModuleManufacturerId;
            Type17Record->ModuleProductID = ModuleProductId;
            Type17Record->MemorySubsystemControllerManufacturerID = MemorySubsystemControllerManufacturerId;
            Type17Record->MemorySubsystemControllerProductID = MemorySubsystemControllerProductId;

            Type17Record->NonVolatileSize = NonVolatileSize;
            Type17Record->VolatileSize = VolatileSize;
            Type17Record->CacheSize = CacheSize;
            Type17Record->LogicalSize = LogicalSize;
            DEBUG ((DEBUG_INFO, "Memory Tech = %x FirmwareVersion = %x%x ModuleManufacturerId = %x ModuleProductId = %x\n",
                    MemoryTechnology, SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].FirmwareVersionStr[0],
                    SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].FirmwareVersionStr[1], ModuleManufacturerId, ModuleProductId));
            DEBUG ((DEBUG_INFO, "Sub Man Id = %x Sub Prod Id = %x NonVolSize = %llx VolSize = %llx CacheSize = %llx\n",
                    MemorySubsystemControllerManufacturerId, MemorySubsystemControllerProductId, NonVolatileSize, VolatileSize, CacheSize));
          }
            DimmType = SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[Dimm].actKeyByte2;
            switch (DimmType) {
            case SPD_RDIMM:
               Type17Record->TypeDetail.Registered = 1;
               break;
            case SPD_UDIMM:
               Type17Record->TypeDetail.Unbuffered = 1;
               break;
            case SPD_SODIMM:
               break;
            case SPD_LRDIMM_DDR4:
               Type17Record->TypeDetail.LrDimm = 1;
               break;
            case SPD_NVM_DIMM:
               break;
            default:
               Type17Record->TypeDetail.Unknown = 1;
               break;
            }
          DEBUG((DEBUG_INFO, "Synchronous = %x\n Registered = %x\n Unbuffered = %x\n LrDimm = %x\n Unknown = %x\n",
                 Type17Record->TypeDetail.Synchronous, Type17Record->TypeDetail.Registered, Type17Record->TypeDetail.Unbuffered,
                 Type17Record->TypeDetail.LrDimm, Type17Record->TypeDetail.Unknown));
          OptionalStrStart = (CHAR8 *) Type17Record + Type17StructureSize;
          UnicodeStrToAsciiStrS (DeviceLocatorStr, OptionalStrStart, DeviceLocatorStrLen + 1);

          OptionalStrStart = (CHAR8 *) (OptionalStrStart + DeviceLocatorStrLen + 1);
          UnicodeStrToAsciiStrS (BankLocatorStr, OptionalStrStart, BankLocatorStrLen + 1);

          OptionalStrStart = (CHAR8 *) (OptionalStrStart + BankLocatorStrLen + 1);
          UnicodeStrToAsciiStrS (ManufactureStr, OptionalStrStart, ManufactureStrLen + 1);

          OptionalStrStart = (CHAR8 *) (OptionalStrStart + ManufactureStrLen + 1);
          UnicodeStrToAsciiStrS (SerialNumberStr, OptionalStrStart, SerialNumberStrLen + 1);

          OptionalStrStart = (CHAR8 *) (OptionalStrStart + SerialNumberStrLen + 1);
          UnicodeStrToAsciiStrS (AssertTagStr, OptionalStrStart, AssertTagStrLen + 1);

          OptionalStrStart = (CHAR8 *) (OptionalStrStart + AssertTagStrLen + 1);
          UnicodeStrToAsciiStrS (PartNumberStr, OptionalStrStart, PartNumberStrLen + 1);

          if (SystemMemoryMap->MemSetup.ExtendedType17) {
            OptionalStrStart = (CHAR8 *) (OptionalStrStart + PartNumberStrLen + 1);
            UnicodeStrToAsciiStrS (FirmwareVersionStr, OptionalStrStart, FirmwareVersionStrLen + 1);
          }
          MemDevType17Handle = SMBIOS_HANDLE_PI_RESERVED;
          Status = SmbiosProtocol->Add (SmbiosProtocol, NULL, &MemDevType17Handle, &Type17Record->Hdr);
          DEBUG ((DEBUG_INFO, "[SMBIOS] Added Type17 (array handle 0x%04X, err info handle 0x%04X, tot width %d, data width %d ",
                  Type17Record->MemoryArrayHandle, Type17Record->MemoryErrorInformationHandle,
                  Type17Record->TotalWidth, Type17Record->DataWidth));
          DEBUG ((DEBUG_INFO, "size %d/%d, speed %d/%d) - handle 0x%04X\n",
                  Type17Record->Size, Type17Record->ExtendedSize,
                  Type17Record->Speed, Type17Record->ConfiguredMemoryClockSpeed, MemDevType17Handle));
          if (!EFI_ERROR (Status)) {
            SmbiosMemInfoAddDimm (Socket, Node, Ch, Dimm, MemDevType17Handle, (BOOLEAN)Type17Record->TypeDetail.Nonvolatile);
          } else {
            ASSERT_EFI_ERROR (Status);
          }
          FreePool (DeviceLocatorStr);
          FreePool (BankLocatorStr);
          FreePool (ManufactureStr);
          FreePool (SerialNumberStr);
          FreePool (AssertTagStr);
          FreePool (PartNumberStr);
          FreePool (Type17Record);
        } // for (Dimm...)
      } // for (Ch...)
    } // for (Node...)
  } // for (Socket...)
  FreePool (TempString);
  gBS->FreePool (StringBuffer);
  //
  // Type 19 - Create Memory Array Mapped Address Type 19 record for each memory SAD reported in HOB.
  //
  if (SystemMemoryMap->numberEntries == 0) {

    DEBUG ((DEBUG_INFO,  "[SMBIOS] Creating Type 19 for system memory of %d GiB\n", MEM_64MB_TO_GBYTES (SystemMemoryMap->memSize)));
    ZeroMem (&Record, sizeof (Record));
    Record.Data.Type19.Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
    Record.Data.Type19.Hdr.Length = sizeof (SMBIOS_TABLE_TYPE19);
    Record.Data.Type19.Hdr.Handle = 0;
    Record.Data.Type19.MemoryArrayHandle = MemArrayType16Handle;
    Record.Data.Type19.PartitionWidth = 1;
    if (SystemMemoryMap->memSize < MEM_KBYTES_TO_64MB (0xFFFFFFFF)) {

      Record.Data.Type19.StartingAddress = 0;
      Record.Data.Type19.EndingAddress = (UINT32)MEM_64MB_TO_KBYTES (SystemMemoryMap->memSize) - 1;

    } else {

      Record.Data.Type19.StartingAddress = 0xFFFFFFFF;
      Record.Data.Type19.EndingAddress = 0xFFFFFFFF;
      Record.Data.Type19.ExtendedStartingAddress = 0;
      Record.Data.Type19.ExtendedEndingAddress = MEM_64MB_TO_BYTES (SystemMemoryMap->memSize) - 1;
    }
    MemArrayMappedType19Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = SmbiosProtocol->Add (SmbiosProtocol, NULL, &MemArrayMappedType19Handle, &Record.Data.Type19.Hdr);
    DEBUG ((DEBUG_INFO, "[SMBIOS] Added Type19 (range 0x%X/%llX..0x%X/%llX, ",
            Record.Data.Type19.StartingAddress, Record.Data.Type19.ExtendedStartingAddress,
            Record.Data.Type19.EndingAddress, Record.Data.Type19.ExtendedEndingAddress));
    DEBUG ((DEBUG_INFO, "array handle 0x%04X, width %d) - handle 0x%04X\n",
            Record.Data.Type19.MemoryArrayHandle, Record.Data.Type19.PartitionWidth, MemArrayMappedType19Handle));
    ASSERT_EFI_ERROR (Status);

  } else {

    for (MemElemIdx = 0; MemElemIdx < SystemMemoryMap->numberEntries; MemElemIdx++) {

      MemElemPtr = &SystemMemoryMap->Element[MemElemIdx];
      Socket = MemElemPtr->SocketId;
      DEBUG ((DEBUG_INFO,  "[SMBIOS] Creating Type 19 for MemElem[%d] of type 0x%X, skt bitmap %X, IMC bitmap %X, ch bitmap",
              MemElemIdx, MemElemPtr->Type, MemElemPtr->SktInterBitmap, MemElemPtr->ImcInterBitmap));
      for (Node = 0; Node < NELEMENTS (MemElemPtr->ChInterBitmap); Node++) {
        DEBUG ((DEBUG_INFO, " %02X", MemElemPtr->ChInterBitmap[Node]));
      }
      DEBUG ((DEBUG_INFO, ", %d GiB\n", MEM_64MB_TO_GBYTES (MemElemPtr->ElementSize)));
      ZeroMem (&Record, sizeof (Record));
      Record.Data.Type19.Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
      Record.Data.Type19.Hdr.Length = sizeof (SMBIOS_TABLE_TYPE19);
      Record.Data.Type19.Hdr.Handle = 0;
      Record.Data.Type19.MemoryArrayHandle = MemArrayType16Handle;
      //
      // Now find partition width. It is a number of DIMMs interleaved in the region. We got bitmap of channels,
      // in each MC but in case of volatile DIMM each channel may have two DIMMs. We do not have DIMM bitmap in
      // memory map hob, so let's check if there are two volatile DIMMs in each channel. If so double the width.
      //
      IsHalfWidth = FALSE;
      for (Node = 0; Node < NELEMENTS (MemElemPtr->ChInterBitmap); Node++) {

        Record.Data.Type19.PartitionWidth += BitmapGetBitCount (MemElemPtr->ChInterBitmap[Node]);
        for (Ch = BitmapGetBit (MemElemPtr->ChInterBitmap[Node], 0);
             Ch < MAX_MC_CH; Ch = BitmapGetBit (MemElemPtr->ChInterBitmap[Node], Ch+1)) {

          Chan = MEM_IMCCH_TO_SKTCH (Node, Ch);
          if (!(Chan < NELEMENTS (SystemMemoryMap->Socket[Socket].ChannelInfo) &&
                SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[0].Enabled &&
                !SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[0].DcpmmPresent &&
                SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[0].NVmemSize == 0 &&
                SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[1].Enabled &&
                !SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[1].DcpmmPresent &&
                SystemMemoryMap->Socket[Socket].ChannelInfo[Chan].DimmInfo[1].NVmemSize == 0)) {
            //
            // There is just one volatile DIMM in this channel, so region is not double width.
            //
            IsHalfWidth = TRUE;
            break;
          }
        }
      }
      if (!IsHalfWidth) {
        Record.Data.Type19.PartitionWidth *= 2;
      }
      Record.Data.Type19.PartitionWidth *= BitmapGetBitCount (MemElemPtr->SktInterBitmap);
      //
      // If the address is overflow, use ExtendedStartingAddress/ExtendedEndingAddress store it.
      //
      if ((UINT64)MemElemPtr->BaseAddress + MemElemPtr->ElementSize < MEM_KBYTES_TO_64MB (0xFFFFFFFF)) {

        Record.Data.Type19.StartingAddress = (UINT32)MEM_64MB_TO_KBYTES ((UINT64)MemElemPtr->BaseAddress);
        Record.Data.Type19.EndingAddress = (UINT32)MEM_64MB_TO_KBYTES ((UINT64)MemElemPtr->BaseAddress + MemElemPtr->ElementSize) - 1;

      } else {

        Record.Data.Type19.StartingAddress = 0xFFFFFFFF;
        Record.Data.Type19.EndingAddress = 0xFFFFFFFF;
        Record.Data.Type19.ExtendedStartingAddress = MEM_64MB_TO_BYTES ((UINT64)MemElemPtr->BaseAddress);
        Record.Data.Type19.ExtendedEndingAddress = MEM_64MB_TO_BYTES ((UINT64)MemElemPtr->BaseAddress + MemElemPtr->ElementSize) - 1;
      }
      MemArrayMappedType19Handle = SMBIOS_HANDLE_PI_RESERVED;
      Status = SmbiosProtocol->Add (SmbiosProtocol, NULL, &MemArrayMappedType19Handle, &Record.Data.Type19.Hdr);
      DEBUG ((DEBUG_INFO, "[SMBIOS] Added Type19 (range 0x%X/%llX..0x%X/%llX, ",
              Record.Data.Type19.StartingAddress, Record.Data.Type19.ExtendedStartingAddress,
              Record.Data.Type19.EndingAddress, Record.Data.Type19.ExtendedEndingAddress));
      DEBUG ((DEBUG_INFO, "array handle 0x%04X, width %d) - handle 0x%04X\n",
              Record.Data.Type19.MemoryArrayHandle, Record.Data.Type19.PartitionWidth, MemArrayMappedType19Handle));
      ASSERT_EFI_ERROR (Status);
    } // for (MemElemIdx...)
  }
  //
  // Instal SMBIOS Memory Info protocol, so that other modules can know SMBIOS handle for DIMM location.
  //
  SmbiosMemInfoInstall ();

  return Status;
} // MemorySubClassEntryPoint()


/**
  @brief Clear memory info data used by SMBIOS_MEM_INFO_PROTOCOL.

  @return Alwas success is returned.
 **/
EFI_STATUS
SmbiosMemInfoClear (VOID)
{
  UINT8 Socket;
  UINT8 Imc;
  UINT8 Ch;
  UINT8 Dimm;

  mSmbiosDimmNum = 0;
  for (Socket = 0; Socket < NELEMENTS (mSmbiosDimmInfo); Socket++) {
    for (Imc = 0; Imc < NELEMENTS (mSmbiosDimmInfo[0]); Imc++) {
      for (Ch = 0; Ch < NELEMENTS (mSmbiosDimmInfo[0][0]); Ch++) {
       for (Dimm = 0; Dimm < NELEMENTS (mSmbiosDimmInfo[0][0][0]); Dimm++) {

         mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].Type17Handle = SMBIOS_INVALID_HANDLE;
         mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].IsNvDimm = FALSE;
       }
     }
   }
  }
  return EFI_SUCCESS;
}


/**
  @brief Add DIMM for SMBIOS_MEM_INFO_PROTOCOL.

  @param[in] Socket       - Index of socket where DIMM is connected
  @param[in] Imc          - Index of integrated memory controller
  @param[in] Ch           - Index of channel in IMC
  @param[in] Dimm         - Index of DIMM slot in channel
  @param[in] Type17Handle - SMBIOS Type17 record handle for this DIMM
  @param[in] IsNvDimm     - True if it is NVM DIMM

  @return Alwas success is returned.
 */
EFI_STATUS
SmbiosMemInfoAddDimm (
  IN UINT8             Socket,
  IN UINT8             Imc,
  IN UINT8             Ch,
  IN UINT8             Dimm,
  IN EFI_SMBIOS_HANDLE Type17Handle,
  IN BOOLEAN           IsNvDimm
  )
{
  ASSERT (mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].Type17Handle == SMBIOS_INVALID_HANDLE);
  mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].Type17Handle = Type17Handle;
  mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].IsNvDimm = IsNvDimm;
  DEBUG ((DEBUG_INFO, "[SMBIOS] S%d.MC%d.C%d.D%d: Added handle 0x%04X\n", Socket, Imc, Ch, Dimm, Type17Handle));
  return EFI_SUCCESS;
}


/**
  @brief This function finds SMBIOS Type17 entry for given SMBIOS handle.

  On input Info->Handle must be set.
  On output, unless error was returned, the rest of the structure is filled.

  @param[in]     This  - Pointer to the protocol
  @param[in,out] Info  - Pointer to DIMM info structure

  @return Standard status codes are returned.
**/
EFI_STATUS EFIAPI
SmbiosMemInfoGetDimmByHandle (
  IN     SMBIOS_MEM_INFO_PROTOCOL *This,
  IN OUT SMBIOS_DIMM_INFO         *Info
  )
{
  UINT8 Socket;
  UINT8 Imc;
  UINT8 Ch;
  UINT8 Dimm;

  for (Socket = 0; Socket < NELEMENTS (mSmbiosDimmInfo); Socket++) {
    for (Imc = 0; Imc < NELEMENTS (mSmbiosDimmInfo[0]); Imc++) {
      for (Ch = 0; Ch < NELEMENTS (mSmbiosDimmInfo[0][0]); Ch++) {
        for (Dimm = 0; Dimm < NELEMENTS (mSmbiosDimmInfo[0][0][0]); Dimm++) {

          if (mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].Type17Handle == Info->Type17Handle) {

            Info->IsNvDimm = mSmbiosDimmInfo[Socket][Imc][Ch][Dimm].IsNvDimm;
            Info->Dimm = Dimm;
            Info->Channel = Ch;
            Info->Imc = Imc;
            Info->Socket = Socket;
            return EFI_SUCCESS;
          }
        }
      }
    }
  }
  return EFI_NOT_FOUND;
}


/**
  @brief This function finds SMBIOS Type17 entry for given DIMM location.

  On input Info->Socket, Info->Imc, Info->Channel, Info->Dimm must be set.
  On output, unless error was returned, the rest of the structure is filled.

  @param[in]     This  - Pointer to the protocol
  @param[in,out] Info  - Pointer to DIMM info structure.

  @return Standard status codes are returned.
**/
EFI_STATUS EFIAPI
SmbiosMemInfoGetDimmByLocation (
  IN     SMBIOS_MEM_INFO_PROTOCOL *This,
  IN OUT SMBIOS_DIMM_INFO         *Info
  )
{
  if ((UINT8)Info->Socket  < NELEMENTS (mSmbiosDimmInfo) &&
      (UINT8)Info->Imc     < NELEMENTS (mSmbiosDimmInfo[0]) &&
      (UINT8)Info->Channel < NELEMENTS (mSmbiosDimmInfo[0][0]) &&
      (UINT8)Info->Dimm    < NELEMENTS (mSmbiosDimmInfo[0][0][0]) &&
      mSmbiosDimmInfo[Info->Socket][Info->Imc][Info->Channel][Info->Dimm].Type17Handle != SMBIOS_INVALID_HANDLE) {

    Info->Type17Handle = mSmbiosDimmInfo[Info->Socket][Info->Imc][Info->Channel][Info->Dimm].Type17Handle;
    Info->IsNvDimm = mSmbiosDimmInfo[Info->Socket][Info->Imc][Info->Channel][Info->Dimm].IsNvDimm;
    return EFI_SUCCESS;
  }
  return EFI_NOT_FOUND;
}


/**
  Instal SMBIOS_MEM_INFO_PROTOCOL service.

  @return Status of protocol installation is returned.
 **/
EFI_STATUS
SmbiosMemInfoInstall (VOID)
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  mSmbiosMemInfo.SmbiosMemInfoProtVersion = SMBIOS_MEMINFO_PROT_VERSION;
  mSmbiosMemInfo.SmbiosDimmNum = mSmbiosDimmNum;
  mSmbiosMemInfo.SmbiosGetDimmByHandle = SmbiosMemInfoGetDimmByHandle;
  mSmbiosMemInfo.SmbiosGetDimmByLocation = SmbiosMemInfoGetDimmByLocation;

  Status = gBS->InstallProtocolInterface (&Handle, &gSmbiosMemInfoProtocolGuid, EFI_NATIVE_INTERFACE, &mSmbiosMemInfo);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  @brief Find all memory controller channels that are marked as 2LM cached.

  @param[in] *SystemMemoryMap - system memory map struct
  @param[in] Socket           - index of socket
  @param[in] Imc              - index of memory controller

  @return Bitmap of channels that are 2LM cached, e.g. bits b101 means channel 0,2
**/
UINT8
GetBitmapFor2LMType (
  IN SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap,
  IN UINT8 Socket,
  IN UINT8 Imc
  )
{
  SAD_TABLE *SadPtr;
  UINT8      BitmapOfCh = 0;
  UINT8      SadInx;

  DEBUG ((DEBUG_INFO,  "[SMBIOS] 2LM SAD of S:%d iMC:%d channels bitmap: {", Socket, Imc));

  for (SadInx = 0; SadInx < (MAX_DRAM_CLUSTERS * MAX_SAD_RULES); SadInx++) {

    SadPtr = &SystemMemoryMap->Socket[Socket].SAD[SadInx];
    if (SadPtr->Enable &&
       (SadPtr->imcInterBitmap & (1 << Imc)) &&
       (IsMemType2lm (SadPtr->type))
       ) {
      DEBUG ((DEBUG_INFO, "%d:%02X, ", SadInx, SadPtr->channelInterBitmap[Imc]));
      //
      // Combind all 2LM SAD entry's iMC channel bit maps into a single map.
      //
      BitmapOfCh |= SadPtr->channelInterBitmap[Imc];
    }
  }
  DEBUG ((DEBUG_INFO, "} = %02X\n", BitmapOfCh));

  return BitmapOfCh;
}

