/** @file
  MS2IDI (B2IDI in Gen3) internal functions declaration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worlDWORDide copyright and trade secret laws and treaty
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

#ifndef __MS2IDI_LIB_INTERNAL_H__
#define __MS2IDI_LIB_INTERNAL_H__

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature and program the required registers

  @param[in] SocId            - Socket ID
  @param[in] DdrtQosMode      - DDRT QoS Mode

**/
VOID
EFIAPI
Ms2IdiSetEnDdrtQoSChip (
  IN UINT8  SocId,
  IN UINT8  DdrtQosMode
  );

#endif // __MS2IDI_LIB_INTERNAL_H__
