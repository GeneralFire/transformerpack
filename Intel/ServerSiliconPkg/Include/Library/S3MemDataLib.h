/** @file
  Interface of S3 and address decode data  library.

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

#ifndef __S3_MEM_DATA_LIB_H__
#define __S3_MEM_DATA_LIB_H__

#include <SiliconSetting.h>

typedef struct {
  UINT8  SpareInUse[2];
  UINT8  SpareLogicalRank[2];  // max 2 spare rank per channel
  UINT8  OldLogicalRank[2];
} RANK_SPARE_SINGLE_CHANNEL;


//
//used for S3 and address decode
//
typedef struct  {
  RANK_SPARE_SINGLE_CHANNEL SpareRank[MC_MAX_NODE][MAX_MC_CH];
  UINT8 ChFailed[MC_MAX_NODE][MAX_MC_CH];
  UINT8 NgnChFailed[MC_MAX_NODE][MAX_MC_CH];
  UINT8 FullMirrorChFailed [MC_MAX_NODE][MAX_MC_CH];
  UINT8 DevTag[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH];
} MEMRAS_S3_PARAM;


/**
  Get imc S3 RAS data.

  @param   none.
  @retval  point of S3 RAS data.

**/
MEMRAS_S3_PARAM *
EFIAPI
GetS3RasData (
  VOID
  );

/**
  Set full mirror channel failed flag

  @param  Node            - Node ID
  @param  ChOnMc          - Channel number per Mc

**/
VOID
EFIAPI
SetFullMirrorChFailed (
  IN  UINT8         Node,
  IN  UINT8         ChOnMc
  );

/**
  Check whether the full mirror channel is failed

  @param  Node            - Node ID
  @param  ChOnMc          - Channel number per Mc

  @retval TRUE -- Failed; FALSE -- Not failed full mirror channel.

**/
BOOLEAN
EFIAPI
IsFullMirrorChFailed (
  IN  UINT8         Node,
  IN  UINT8         ChOnMc
  );
#endif
