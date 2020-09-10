/** @file
  Implementation of PCLS library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <RcRegs.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/ImcRoutingLib.h>
#include <Library/PclsLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/WorkaroundLib.h>

PCLS_STRUCT              *mPclsData  = NULL;  //Track all 16 PCLS registers value through this variable

/**
  Get the PCLS global data structure

  @param none

  @retval PCLS_MODE_STRUCT
          EFI_FAILURE
**/
PCLS_STRUCT *
EFIAPI
GetPclsData (
  VOID
  )
{
  return mPclsData;
}

/**
  Get the PCLS global data structure

  @param none

  @retval PCLS_MODE_STRUCT Pcls Data structure

**/
PCLS_STRUCT *
EFIAPI
SetPclsValue (
  IN PCLS_STRUCT *PclsData
  )
{
  mPclsData = PclsData;
  return mPclsData;
}

/**

  Identify the index of the PCLS register to be used to perform
  a PCLS action on the current failure nibble

  @param  SparingRegionPtr   pointer to sparing region information
  @param  PclsIndexPtr  pcls index value

  @retval PCLS Index number

**/
BOOLEAN
EFIAPI
GetPclsIndex (
  IN  SPARING_REGION_STRUCT   *SparingRegionPtr,
  OUT UINT16                  *PclsIndexPtr
  )
{
  UINT8             Node;
  UINT8             ChOnNode;
  UINT16            PclsIndex;
  BOOLEAN           PclsIndexFound;

  Node            =  SparingRegionPtr->NodeId;
  ChOnNode        =  SparingRegionPtr->ChOnNode;
  PclsIndexFound  = FALSE;

  for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION ; PclsIndex++) {
    if (mPclsData->PclsBitMapPerChannel[Node][ChOnNode] & (1 << PclsIndex)) { //PCLS register already taken
      continue;
    }
    *PclsIndexPtr = PclsIndex;
    PclsIndexFound = TRUE;
    break;
  }
  return PclsIndexFound;
}

/**
  Take PCLS action on target index of channel on particular memory controller.
   Note to make sure internal data and CSR are consistent.

  @param[in]  SparingRegionPtr  pointer to sparing region information

  @retval TRUE    PCLS action successful
  @retval FALSE   PCLS action not successful
**/
BOOLEAN
EFIAPI
TakePclsAction (
  IN  SPARING_REGION_STRUCT   *SparingRegionPtr
  )
{
  UINT8     Node;
  UINT8     ChOnNode;
  UINT8     Socket;
  UINT8     ChOnSkt;
  UINT16    PclsIndex;
  BOOLEAN   Status;
  BOOLEAN   PclsEnable = TRUE;
  UINT32    ChipSelect;
  UINT32    Cbit;
  UINT32    Bg;
  UINT32    Ba;
  UINT32    Row;
  UINT32    Col;
  UINT32    DataOffset;

  Status    = FALSE;
  PclsIndex = 0;
  Node      = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;
  Socket    = NODE_TO_SKT (Node);
  ChOnSkt   = NODECHA_TO_SKTCHA (Node, ChOnNode);

  Status = GetPclsIndex (SparingRegionPtr, &PclsIndex);

  RAS_ASSERT (PclsIndex < MAX_PCLS_REGION);
  if (PclsIndex >= MAX_PCLS_REGION) {
    Status = FALSE;
  }

  if (Status) {
    UpdatePclsDataStruct (PclsIndex, PclsEnable, SparingRegionPtr);

    ChipSelect    = SparingRegionPtr->FailedRank;
    Cbit          = SparingRegionPtr->FailedSubRank;
    Bg            = (SparingRegionPtr->FailedBank >> 2);
    Ba            = (SparingRegionPtr->FailedBank & 0x3);
    Row           = SparingRegionPtr->FailedRow;
    Col           = SparingRegionPtr->FailedCol;
    DataOffset    = (SparingRegionPtr->Transfer << 5) | (SparingRegionPtr->FailedDevice & 0x1f);

    RAS_DEBUG ((LEVEL_REG, "[PCLS]: S%d Ch%d index %d:\n", Socket, ChOnSkt, PclsIndex));
    RAS_DEBUG ((LEVEL_REG, "  Call pcls enable \n"));
    RAS_DEBUG ((LEVEL_REG, "  chip_select = %d\n", ChipSelect));
    RAS_DEBUG ((LEVEL_REG, "  cbit        = %d\n", Cbit));
    RAS_DEBUG ((LEVEL_REG, "  bg          = %d\n", Bg));
    RAS_DEBUG ((LEVEL_REG, "  ba          = %d\n", Ba));
    RAS_DEBUG ((LEVEL_REG, "  row         = %d\n", Row));
    RAS_DEBUG ((LEVEL_REG, "  col         = %d\n", Col));
    RAS_DEBUG ((LEVEL_REG, "  data_offset = 0x%02x\n", DataOffset));

    EnablePcls (Socket, ChOnSkt, PclsIndex, ChipSelect, Cbit, Bg, Ba, Row, Col, DataOffset);
    mPclsData->PclsBitMapPerChannel[Node][ChOnNode] |= (1 << PclsIndex); //Save the PCLS entries used in a bitmap
  }

  return Status;
}

/**

  Verify that the current failure is single bit and qualifies for a PCLS

  @param  SparingRegionPtr   pointer to sparing region information

  @retval   TRUE  -- Current failure is single bit
            FALSE –- current failure not single bit

 **/
BOOLEAN
EFIAPI
IsCurrentFailureSingleBit (
  IN SPARING_REGION_STRUCT *SparingRegionPtr
  )
{
  return SparingRegionPtr->SingleBit;
}

/**

  Verify that the current failure is single bit Intermediate Locator Syndrome and qualifies for a PCLS

  @param  SparingRegionPtr   pointer to sparing region information

  @retval   TRUE  -- Current failure is single bit Intermediate Locator Syndrome
            FALSE –- current failure not single bit Intermediate Locator Syndrome

 **/
BOOLEAN
EFIAPI
IsCurrentFailureSingleBitInterLocSyn (
  IN OUT SPARING_REGION_STRUCT *SparingRegionPtr
  )
{
  if (SparingRegionPtr->InterLocSyn && (HighBitSet32 (SparingRegionPtr->InterLocSyn) == LowBitSet32 (SparingRegionPtr->InterLocSyn))) {
    //
    // IL 0-5 in Transfer 2, IL 6-11 in Transfer 6.
    //
    if (HighBitSet32 (SparingRegionPtr->InterLocSyn) >= 6) {
      SparingRegionPtr->Transfer = 6;
    } else {
      SparingRegionPtr->Transfer = 2;
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  function to find if PCLS policy enabled

  @retval   TRUE  -- PCLS has happened on target rank.
            FALSE –- NO PCLS happened on target rank.
 **/
BOOLEAN
EFIAPI
IsPclsEnabled (
  VOID
  )
{
  MEM_TOPOLOGY            *MemTopology;

  MemTopology = GetMemTopology ();
  if (MemTopology != NULL) {
    if (MemTopology->ExRasModesEnabled & PCLS_EN) {
      return TRUE;
    }
  }
  return FALSE;
}

/**

  Do you have one of the PCLS resources out of the 16 per channel

  @param  SparingRegionPtr   pointer to sparing region information

  @retval   TRUE  -- PCLS resource available
            FALSE –- PCLS resource not available

 **/
BOOLEAN
EFIAPI
IsPclsResourceAvailable (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  UINT8 Node;
  UINT8 ChOnMc;

  Node    = SparingRegionPtr->NodeId;
  ChOnMc  = SparingRegionPtr->ChOnNode;
  //
  // ~ PclsBitMapPerChannel if Non-Zero indicates available pcls resource in channel
  //
  if ( ( ~(mPclsData->PclsBitMapPerChannel[Node][ChOnMc])) & PCLS_BITMAP_MASK) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Is Pcls possible for this error

  @param  SparingRegionPtr   pointer to sparing region information

  @retval   TRUE  -- PCLS is possible
            FALSE –- PCLS not possible

 **/

BOOLEAN
EFIAPI
IsPclsPossible (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  return ( (IsPclsEnabled()) & (IsCurrentFailureSingleBit(SparingRegionPtr)) & (IsPclsResourceAvailable(SparingRegionPtr)) );
}

/**
  Clear all pre-existing PCLS regions on this Rank

  @param[in]    NodeId,
  @param[in]    ChOnNode,
  @param[in]    Rank

  @retval  NONE
**/
VOID
EFIAPI
ClearPclsOnRank (
  UINT8   NodeId,
  UINT8   ChOnNode,
  UINT8   Rank
  )
{
  UINT8   Socket;
  UINT8   ChOnSkt;
  UINT16  PclsIndex;

  Socket  = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION; PclsIndex++) {
    if (mPclsData->PclsBitMapPerChannel[NodeId][ChOnNode] & (1 << PclsIndex)) { //Valid Pcls entry
      if ( (mPclsData->PclsData[NodeId][ChOnNode][PclsIndex].Rank == Rank)) {
        DisablePcls (Socket, ChOnSkt, PclsIndex);
      }
    }
  }
  return;
}

/**

  In preparation for doing an ADDDC operation on the bank region, clear all
  pre-existing PCLS regions on this bank

  @param[in]    NodeId,
  @param[in]    ChOnNode,
  @param[in]    Rank,
  @param[in]    Bank

  @retval   None
**/
VOID
EFIAPI
ClearPclsOnBank (
    UINT8   NodeId,
    UINT8   ChOnNode,
    UINT8   Rank,
    UINT8   Bank
  )
{
  UINT8   Socket;
  UINT8   ChOnSkt;
  UINT16  PclsIndex;

  Socket  = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION; PclsIndex++) {
    if (mPclsData->PclsBitMapPerChannel[NodeId][ChOnNode] & (1 << PclsIndex)) { //Valid Pcls entry
      if ((mPclsData->PclsData[NodeId][ChOnNode][PclsIndex].Rank == Rank) &&
          (mPclsData->PclsData[NodeId][ChOnNode][PclsIndex].Bank == Bank)) {
        DisablePcls (Socket, ChOnSkt, PclsIndex);
      }
    }
  }
  return;
}

/**

  In preparation for doing an ADDDC operation on the bank region, clear all
  pre-existing PCLS regions on the channel.

  @param[in]    NodeId,
  @param[in]    ChOnNode,

  @retval   None
**/
VOID
EFIAPI
ClearPclsOnCh (
  UINT8   NodeId,
  UINT8   ChOnNode
  )
{
  UINT8   Socket;
  UINT8   ChOnSkt;
  UINT16  PclsIndex;

  Socket  = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION; PclsIndex++) {
    if (mPclsData->PclsBitMapPerChannel[NodeId][ChOnNode] & (1 << PclsIndex)) { //Valid Pcls entry
        DisablePcls (Socket, ChOnSkt, PclsIndex);
    }
  }
  return;
}

/**

  PCLS internal data structure is used for quick data processing rather than
  reading the registers every time. This internal data structure should be updated
  on every new PCLS entry or PCLS reverse

  @param   IN SPARING_REGION_STRUCT  *SparingRegionPtr - Pointer for sparing region information

  @retval   None
**/
VOID
EFIAPI
UpdatePclsDataStruct (
  IN UINT16                   PclsIndex,
  IN BOOLEAN                  PclsEnable,
  IN SPARING_REGION_STRUCT    *SparingRegionPtr
  )
{

  UINT8 Node;
  UINT8 ChOnMc;

  Node   = SparingRegionPtr->NodeId;
  ChOnMc = SparingRegionPtr->ChOnNode;

  if (PclsEnable) {
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].PclsEnable   = TRUE;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].Rank         = SparingRegionPtr->FailedRank;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].Bank         = SparingRegionPtr->FailedBank;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].Row          = SparingRegionPtr->FailedRow;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].Col          = SparingRegionPtr->FailedCol;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].DataOffset   = SparingRegionPtr->Transfer;
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].Device       = SparingRegionPtr->FailedDevice;
  } else {
    mPclsData->PclsData[Node][ChOnMc][PclsIndex].PclsEnable = FALSE;
  }
}

/**
  Initialize PCLS global data structure

  @param None

  @retval EFI_SUCCESS
          EFI_FAILURE

**/
EFI_STATUS
EFIAPI
InitPclsData (
  VOID
  )
{
  BOOLEAN             NewAllocation;
  UINT8               NodeId;
  UINT8               ChOnNode;
  UINT8               PclsIndex;

  mPclsData = GetRasGlobalData (
                 L"PCLSValue",
                 sizeof (PCLS_STRUCT),
                 &NewAllocation
                 );

  if (mPclsData == NULL) {
    return EFI_UNSUPPORTED;
  }

  for (NodeId = 0; NodeId < MC_MAX_NODE; NodeId++) {
    for (ChOnNode = 0; ChOnNode< MAX_MC_CH; ChOnNode++) {
      mPclsData->PclsBitMapPerChannel[NodeId][ChOnNode] = 0;
      mPclsData->AdddcRegion[NodeId][ChOnNode].Present = FALSE;
      for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION; PclsIndex++) {
        mPclsData->PclsData[NodeId][ChOnNode][PclsIndex].PclsEnable = 0;
      }
    }
  }

  return EFI_SUCCESS;
}


/**

  The constructor function initialize PCLS library.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitPclsLibrary (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  InitPclsData ();

  return EFI_SUCCESS;
}

