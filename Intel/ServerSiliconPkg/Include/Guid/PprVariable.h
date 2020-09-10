/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 Intel Corporation. <BR>

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

#ifndef __PPR_VARIABLE_H__
#define __PPR_VARIABLE_H__

extern EFI_GUID gEfiPprVariableGuid;

#define PPR_VARIABLE_NAME L"PprAddress"

//
// PPR max entries per CH
//
#define MAX_PPR_ADDR_ENTRIES           20


typedef struct {
  UINT8     pprAddrStatus;
  UINT8     socket;
  UINT8     mc;
  UINT8     ch;
  UINT8     dimm;
  UINT8     rank;
  UINT8     subRank;
  UINT32    nibbleMask;
  UINT8     bank;
  UINT32    row;
} PPR_ADDR_SETUP;

typedef struct {
  UINT16          crc16;
  PPR_ADDR_SETUP  pprAddrSetup[MAX_PPR_ADDR_ENTRIES];
} PPR_ADDR_VARIABLE;

#endif // #ifndef __PPR_VARIABLE_H__
