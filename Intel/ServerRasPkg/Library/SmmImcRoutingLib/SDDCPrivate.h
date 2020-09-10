/** @file
  Interface of SDDC routine library.
 
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>
  
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

#ifndef __SDDC_PRIVATE_H__
#define __SDDC_PRIVATE_H__

//
//!!don't change SddcPlusOneAvailable value, keept it as 0.
//
typedef enum {
  SddcPlusOneAvailable,         // SDDC Sparing available
  SddcPlusOneInProgress,        // SDDC Sparing in progress
  SddcPlusOneBuddyInProgress ,  // SDDC buddy in progress
  SddcPlusOneDone,              // SDDC Sparing is done
} SDDC_PLUSONE_STATUS2;


#pragma  pack(1)
typedef struct {
  SDDC_PLUSONE_STATUS2       SddcPlusOneStatus[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH * SUB_CH];
} SDDC_PLUS_ONE_STRUCTURE;
#pragma  pack()

SDDC_PLUSONE_STATUS2
EFIAPI
GetSDDCState (
  IN  UINT8   Node,
  IN  UINT8   ChOnNode,
  IN  UINT8   Rank
  );

#endif
