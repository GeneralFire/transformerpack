/** @file
  Interface of SSR library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef __PLAT_SSR_LIB_H__
#define __PLAT_SSR_LIB_H__

#define PATROL_SAVE    0
#define PATROL_RESTORE 1
#define GV_C6FLAG   0x80

/**
  PatrolSPAMode = 1,           --- Patrol runs at SPA mode.
  PatrolLegacyScrubbingMode,   --- Patrol legacy mode, scrubbing
  PatrolLegacyCPLMode,         --- Patrol legacy mode, complete.
  PatrolLegacyStoppedMode,     --- Patrol legacy mode, stopped due to error.
  SpareCopyingMode,            --- sparing mode, copying
  SpareCopyCPLMode,            --- sparing mode, complete.
  SSRStopMode                  --- neither patrol or sparing working.
                                   stop case other than PatrolLegacyCPLMode/PatrolLegacyStoppedMode/SpareCopyCPLMode
**/
typedef enum{
  SSRStopMode = 1,
  PatrolSPAMode,
  PatrolLegacyCPLMode,
  PatrolLegacyScrubbingMode,
  PatrolLegacyStoppedMode,
  SpareCopyCPLMode,
  SpareCopyingMode,
  MaxSsrState
} SSR_STATE;

typedef enum {
  RankSparing,       // Rank Sparing
  BankAdddcSparingMR,    // ADDDC Sparing
  RankAdddcSparingMR,    // ADDDC Sparing
  SddcPlusOne,
} SPARING_MODE;

typedef struct  {
  UINT8   Valid; // Initialize to 0
  UINT8   NodeId;
  UINT8   ChOnNode;
  UINT8   RegionNum;
  UINT8   FailedRank;
  UINT8   FailedSubRank;
  UINT8   FailedBank;    // Bit3~Bit2: BankGrp, Bit1~Bit0: BankInGrp
  UINT8   NonFailedRank;
  UINT8   NonFailedSubRank;
  UINT8   NonFailedBank;      // Bit3~Bit2: BankGrp, Bit1~Bit0: BankInGrp
  UINT8   FailedDevice;
  BOOLEAN ReverseRegion;
  UINT32  FailedRow;  //Required for PCLS
  UINT32  FailedCol;  //Required for PCLS
  UINT8   Transfer; //burst index in cacheline
  UINT8   SingleBit;
  UINT16  InterLocSyn;
} SPARING_REGION_STRUCT;

/**
  This implementation is to scrub specic memory rank to prepare for rank sparing.
After scrub complete, it will trigger SMI, or if any correctable or uncorrectable error happens,
it will stop and trigger SMI.

  Caller should ensure restore patrol scrub, and check if patrol scrub is enabled by MRC.

  @param   NodeId     Mc index in whole system
  @param   ChOnMc     Channal index on memory controller
  @param   SparePhysicalRank   rank index to be scrub. Logical rank index is the same as physical rank index on SKX.

**/
VOID
EFIAPI
PatrolScrubSpecificAddress (
  IN  UINT8     NodeId,
  IN  UINT8     ChOnMc,
  IN  UINT8     SparePhysicalRank
  );



VOID
EFIAPI
PatrolScrubGoBack (
  IN  UINT8     NodeId,
  IN  UINT8     ChOnMc,
  IN  UINT8     FailedRank,
  IN  UINT8     ScrubbedRank
  );

/**
  Save and restore patrol scrub register.

  @param  Flag   PATROL_SAVE  -- save; PATROL_RESTORE -- restore.
  @param  Node   node index in whole system
  @retval none.
**/
VOID
EFIAPI
PatrolScrubInfoSaveOrRestore (
  UINT8   Node,
  UINT8   Flag
  );


VOID
RankSparingCopy (
  IN UINT8 NodeId,
  IN UINT8 Ch,
  IN UINT8 SrcRank,
  IN UINT8 DstRank
  );


/**
  get SSR state of specific Node.

  @param   Node   node index in whole system
  @retval  SSR state of specific node.
**/
SSR_STATE
EFIAPI
GetMcSsrState (
  IN UINT8         Node
  );



BOOLEAN
EFIAPI
IsPatrolEnabled (
  );


/**
  spare copy for VLS(ADDDC, SDDC)

  @param   pAdddcRegion ADDDC region parameters.
  @param   SparingMode  sparing mode
**/
EFI_STATUS
EFIAPI
VlsSparingCopy (
  IN SPARING_REGION_STRUCT  *SparingRegion,
  IN SPARING_MODE          SparingMode
  );

/**
  Get the failed rank/bank/device information

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval Status

**/
EFI_STATUS
GetFailedDeviceInfo (
  IN   UINT8                 NodeId,
  IN   UINT8                 ChOnNode,
  OUT  SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**

  At the end of 2LM sparing (rank/adddc/+1) we have to clear Work around bits
  nomcstoponddr4err,Link_Cfg_read and this function is used for that purpose

  @param Mc - Memory controller ID

  @retval EFI_SUCCESS if sparing engine is released
  @retval EFI_UNSUPPORTED any other error

**/
VOID
EFIAPI
ClearSparing2LmWa(
    IN UINT8 NodeId,
    IN UINT8 Ch
 );

#endif
