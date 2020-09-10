/** @file
  Definition for a struture sharing information for CPU hot plug.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2014 Intel Corporation. <BR>

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

#ifndef _CPU_HOT_PLUG_DATA_H_
#define _CPU_HOT_PLUG_DATA_H_

#define  CPU_HOT_PLUG_DATA_REVISION_1      0x00000001

typedef struct {
  UINT32    Revision;          // Used for version identification for this structure
  UINT32    ArrayLength;       // The entries number of the following ApicId array and SmBase array
  //
  // Data required for SMBASE relocation
  //
  UINT64    *ApicId;           // Pointer to ApicId array
  UINTN     *SmBase;           // Pointer to SmBase array
  UINT32    IEDBase;
  UINT32    SmrrBase;
  UINT32    SmrrSize;
} CPU_HOT_PLUG_DATA;

#endif

