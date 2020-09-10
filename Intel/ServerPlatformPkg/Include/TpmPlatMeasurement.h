/** @file
  Tpm platform measurement definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#ifndef _TPM_PLATFORM_MEASUREMENT_H_
#define _TPM_PLATFORM_MEASUREMENT_H_

#define EV_OEM_TYPE_OPROM  6
#define EV_ID_OEM_OPROM    7

//
// EFI_TCG_EV_OPTION_ROM
//
typedef struct _EFI_TCG_EV_OPROM_HASH {
  UINT16                            Reserved;
  UINT16                            PFA;
  TCG_DIGEST                        Hash;
} EFI_TCG_EV_OPROM_HASH;

//
// EFI_TCG_EV_TAG
//
typedef struct _EFI_TCG_EV_OPTION_ROM {
  UINT32                             EventID;
  UINT32                             EventSize;
  EFI_TCG_EV_OPROM_HASH              EventData;
} EFI_TCG_EV_OPTION_ROM;

#endif
