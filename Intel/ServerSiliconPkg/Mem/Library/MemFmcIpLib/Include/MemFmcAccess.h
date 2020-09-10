/** @file
  MemFmcAccess.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef  _memfmcaccess_h
#define  _memcfmcaccess_h

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"

//
// FMC Access Interface
//
#define  FMC_SMB   0
#define  FMC_EMRS  1
#define  FMC_DPA   2

//
// DDRT Training Status
//
#define  DDRT_TRAINING_STATUS_NOT_COMPLETE  0x00
#define  DDRT_TRAINING_STATUS_COMPLETE      0x01
#define  DDRT_TRAINING_STATUS_FAILURE       0x02
#define  DDRT_TRAINING_STATUS_S3_COMPLETE   0x03
#define  DDRT_TRAINING_STATUS_NORMAL_MODE   0x04

//
// Function prototypes that are private to this lib
//

/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
UINT32
ReadFnvCfgDpa (
              PSYSHOST Host,
              UINT8  socket,
              UINT8  ch,
              UINT8  dimm,
              UINT32 reg,
              UINT64_STRUCT *data
              );

/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
UINT32 WriteFnvCfgDpa (
                      PSYSHOST Host,
                      UINT8  socket,
                      UINT8  ch,
                      UINT8  dimm,
                      UINT32 reg,
                      UINT64_STRUCT data
                      );

#endif   // _memfmcaccess_h
