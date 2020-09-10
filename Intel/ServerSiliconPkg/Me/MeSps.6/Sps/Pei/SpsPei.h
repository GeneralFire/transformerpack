/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _SPSPEI_H_
#define _SPSPEI_H_

UINT64
EFIAPI
ReadTimeStampCounter (
 VOID
 );

#ifdef FSP_FLAG
EFI_STATUS
SpsFspInitComplete (
  IN HECI_CONTROL                  *PpiHeci,
  IN SPS_PEI_CONFIG                *pSpsPolicy,
  UINT8                            WorkFlow,
  UINT32                           FeatureSet,
  UINT32                           FeatureSet2,
  BOOLEAN                          PwrOptBoot,
  UINT8                            Cores2Disable
  );
#endif
#endif // _SPSPEI_H_
