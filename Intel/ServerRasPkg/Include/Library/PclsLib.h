/** @file
  PCLS library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __PCLS_LIB_H__
#define __PCLS_LIB_H__

#include <Library/MemFuncLib.h>
#include <Library/PlatSSRLib.h>
#include <SiliconSettingRAS.h>

#define   PCLS_BITMAP_MASK 0xFFFF

typedef struct {
     BOOLEAN  PclsEnable;
     UINT8    Rank;
     UINT8    Cbit;
     UINT8    Bank;  // b0b1 = bank address b2b3 = bank group
     UINT32   Row;
     UINT32   Col;
     UINT8    DataOffset; //   Burst * 16 + device = data_offset
     UINT8    Device;    //used to search for specific device
     UINT8    State;  // manipulate data state.
} PCLS_DATA_STRUCT;

typedef struct {
  BOOLEAN  Present;  // Is there ADDDC region for given node/channel?
} PCLS_ADDDC_STRUCT;

typedef struct {
  PCLS_ADDDC_STRUCT AdddcRegion[MC_MAX_NODE][MAX_MC_CH];
  PCLS_DATA_STRUCT PclsData[MC_MAX_NODE][MAX_MC_CH][MAX_PCLS_REGION];
  UINT16  PclsBitMapPerChannel[MC_MAX_NODE][MAX_MC_CH];
} PCLS_STRUCT;

/**

  Reverse the Pcls on the channel and Pcls index passed. Also update the per channel global structure
  used to track used up pcls regions

  @param  IN NodeId
  @param  IN ChOnNode
  @param  IN PclsIndex

  @retval   none.

**/
VOID
EFIAPI
ReversePcls (
  IN  UINT8   NodeId,
  IN  UINT8   ChOnNode,
  IN  UINT16  PclsIndex
  );

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
  );

/**
  Clear all pre-existing PCLS regions on this Rank

  @param   NodeId,
  @param   ChOnNode,
  @param   Rank

  @retval  NONE
**/
VOID
EFIAPI
ClearPclsOnRank (
  UINT8   NodeId,
  UINT8   ChOnNode,
  UINT8   Rank
  );

/**

  In preparation for doing an ADDDC operation on the bank region, clear all
  pre-existing PCLS regions on this bank

  @param[in]  NodeId,
  @param[in]   ChOnNode,
  @param[in]   Rank,
  @param[in]   Bank

  @retval   None
**/
VOID
EFIAPI
ClearPclsOnBank (
  UINT8   NodeId,
  UINT8   ChOnNode,
  UINT8   Rank,
  UINT8   Bank
  );

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
  );

/**

  PCLS Basic requirements are PCLS policy enabled, dimms are in independent mode (no Mirror or Rank sparing)
  S3 is disabled. S3 and PCLS cannot work in conjunction

  @param[in]  Node       memory controller index in system.
  @param[in]  ChOnMc  Channel index in Mc.
  @param[in]  Rank       Rank index in channel

  @retval   TRUE
            FALSE

 **/
BOOLEAN
EFIAPI
CheckPclsBasicRestrictions (
  VOID
  );


/**

  Verify that the current failure is single bit and qualifies for a PCLS

  @param  Node       memory controller index in system.
  @param  ChOnMc  Channel index in Mc.

  @retval   TRUE  -- PCLS has happened on target rank.
            FALSE –- NO PCLS happened on target rank.

 **/
BOOLEAN
EFIAPI
IsCurrentFailureSingleBit (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

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
  );

/**

  PCLS Basic requirements are PCLS policy enabled, dimms are in independent mode (no Mirror or Rank sparing)
  S3 is disabled. S3 and PCLS cannot work in conjunction

  @param  Node       memory controller index in system.
  @param  ChOnMc  Channel index in Mc.
  @param  Rank       Rank index in channel

  @retval   TRUE  -- PCLS has happened on target rank.
            FALSE –- NO PCLS happened on target rank.

 **/
BOOLEAN
EFIAPI
IsPclsPossible (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**

  Do you have one of the PCLS resources out of the 16 per channel

  @param  Node       memory controller index in system.
  @param  ChOnMc  Channel index in Mc.
  @param  Rank       Rank index in channel

  @retval   TRUE  -- PCLS has happened on target rank.
            FALSE –- NO PCLS happened on target rank.

 **/
BOOLEAN
EFIAPI
IsPclsResourceAvailable (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**

   Check if any PCLS happened on target bank.

  @param  Node       memory controller index in system.
  @param  ChOnMc  Channel index in Mc.
  @param  Rank       Rank index in channel.
  @param  Bank       Bank index in Rank.

  @retval   TRUE  -- PCLS happened on target bank.
            FALSE – NO PCLS happened on target bank.
**/

BOOLEAN
EFIAPI
IsPCLSExistOnBank (
  IN  UINT8 Node,
  IN  UINT8 Ch,
  IN  UINT8 Rank,
  IN  UINT8 Bank
  );

/**

  PCLS internal data structure is used for quick data processing rather than
  reading the registers every time. This internal data structure should be updated
  on every new PCLS entry or PCLS reverse

  @param  Node       memory controller index in system.
  @param  ChOnMc  Channel index in Mc.
  @param  Rank       Rank index in channel.
  @param  Bank       Bank index in Rank.

  @retval   TRUE  -- PCLS happened on target bank.
            FALSE – NO PCLS happened on target bank.
**/
VOID
EFIAPI
UpdatePclsDataStruct (
  IN UINT16             PclsIndex,
  IN BOOLEAN            PclsEnable,
  IN SPARING_REGION_STRUCT   *SparingRegionPtr
  );



/**
  Handle new PCLS event

  @param  NodeId          - Memory controller ID
  @param  IsEventHandled  - ptr to buffer to hold event handled status

  @retval None

  @retval IsEventHandled  - TRUE  -  event was handled
                          - FALSE - event not handled

**/
VOID
EFIAPI
HandleNewPclsEvent (
  IN  UINT8             NodeId,
  OUT BOOLEAN          *IsEventHandled
  );

/**
  The rank is not in VLS/SDDC + 1, try to handle the error within bank level

  @retval Status   - TRUE - Error existed
                   - FALSE- No error

**/
EFI_STATUS
HandleNonRankPcls (
  IN UINT8        NodeId,
  IN UINT8        ChOnNode,
  FAILED_DIMM_INFO  *FailedDimmInfo
  );

/**
  Handle event to reverse adddc region and create a PCLS for
  current failure

  @param  FailedDimmInfo  - Pointer to the failed dimm information

  @retval Status          - EFI_SUCCESS/EFI_ERROR

**/
EFI_STATUS
EFIAPI
HandleRankVlsRevsPclsEvent (
  IN FAILED_DIMM_INFO  *pFailedDimmInfo
  );

/**

  Identify the index of the PCLS register to be used to perform
  a PCLS action on the current failure nibble

  @param  FailedDimmInfoPtr   pointer to failed dimm information

  @retval PCLS Index number

**/
BOOLEAN
EFIAPI
GetPclsIndex (
  IN  SPARING_REGION_STRUCT   *SparingRegionPtr,
  OUT UINT16                  *PclsIndexPtr
  );

/**
  Check and handle PCLS event

  @param  NodeId          - Memory controller ID
  @param  EventType       - NEW_EVENTS or EVENTS_IN_PROGRESS

  @retval IsEventHandled  - TRUE -  event was handled
  @retval                 - FALSE - event not handled

**/

BOOLEAN
EFIAPI
CheckAndHandlePclsSparing (
  IN  UINT8         NodeId,
  IN  EVENT_TYPE    EventType
  );


#endif //__PCLS_LIB_H__
