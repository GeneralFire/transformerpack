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

#ifndef _UPI_HOST_DATA_LIB_H_
#define _UPI_HOST_DATA_LIB_H_

#include <Upi/KtiHost.h>

typedef enum {
  KtiHostIn = 0,
  KtiHostOut,
  KtiHostNvram,
  KtiHostMax
} KTI_HOST_TYPE;

/**
  Get the KTI Host data pointer.
  @return NULL if can't find the KTI Host data

 **/
VOID *
EFIAPI
GetKtiHostDataPtr(
  KTI_HOST_TYPE KtiHostType
  );

VOID
EFIAPI
SimSetKtiHostData (
  VOID
  );

#define KTI_HOST_IN_PTR ((KTI_HOST_IN *)GetKtiHostDataPtr (KtiHostIn))
#define KTI_HOST_OUT_PTR ((KTI_HOST_OUT *)GetKtiHostDataPtr (KtiHostOut))
#define KTI_HOST_NVRAM_PTR ((KTI_HOST_NVRAM *)GetKtiHostDataPtr (KtiHostNvram))

#endif // _UPI_HOST_DATA_LIB_H_
