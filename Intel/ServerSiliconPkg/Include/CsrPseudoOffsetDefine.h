/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef __CSR_PSEUDO_OFFSET_DEFINE_H__
#define __CSR_PSEUDO_OFFSET_DEFINE_H__
#include <RegPath.h>
#include REG_PATH(PSEUDO_OFFSET_INFO.h)
//////////////////////////////////////
// Construct a pseudo offset table  //
//////////////////////////////////////

// -----------------------------------
// |    PSEUDO_OFFSET_TABLE_HEADER   |
// |---------------------------------|
// |         DATA BLOCK 0            |
// |---------------------------------|
// |                                 |
// |         DATA BLOCK 1            |
// |                                 |
// |---------------------------------|
// |         DATA BLOCK 2            |
// |---------------------------------|
// |                                 |
// |         DATA BLOCK 3            |
// |                                 |
// |---------------------------------|
// |        ......                   |

#define MAX_BOX_TYPE_IN_TABLE_ICXDH0 32
#define MAX_BOX_TYPE_IN_TABLE_ICXDL0 0

#pragma pack (push,1)

typedef struct _PSEUDO_OFFSET_TABLE_HEADER {
  UINT16    TableSize;                                                           //Total size of pseudo offset table(Header + Data Blocks).
#if defined (ICXDE_HCC_HOST)
  UINT16    SubTableLocation[MAX_BOX_TYPE_IN_TABLE_ICXDH0 + 1][MAX_FUNC_BLK_IN_TABLE + 1];  //Provides sub table location info from the starting of pseudo. Created for supporting ICXDE HCC table 
#elif  defined (ICXDE_LCC_HOST)
  UINT16    SubTableLocation[MAX_BOX_TYPE_IN_TABLE_ICXDL0 + 1][MAX_FUNC_BLK_IN_TABLE + 1];  //Provides sub table location info from the starting of pseudo. Created for supporting ICXDE LCC table 
#else
  UINT16    SubTableLocation[MAX_BOX_TYPE_IN_TABLE + 1][MAX_FUNC_BLK_IN_TABLE + 1];  //Provides sub table location info from the starting of pseudo.
#endif
} PSEUDO_OFFSET_TABLE_HEADER;
#pragma pack (pop)

#endif //#ifndef __CSR_PSEUDO_OFFSET_DEFINE_H_
