/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _ME_SPS_FIA_MUX_H_
#define _ME_SPS_FIA_MUX_H_

#include <Guid/SpsInfoHobGuid.h>

/**
  This function updates ME related strings for FiaMux in BIOS setup database.

  Some of the strings contain dummy values to be update with run-time data.
  These strings are updated by this function before any setup screen is
  printed. For some reason the strings from various screens,
  although identified with unique ids, are stored in separate databases.
  Class identifies the database. To avoid multiple requests to ME
  and multiple traces parts of this function are executed for particular
  forms.

  @param[out] HiiAdvancedHandle BIOS setup database handle
  @param[in]  SpsInfo           SpsInfo Hob pointer

  @retval EFI_SUCCESS         Always return EFI_SUCCESS
**/
EFI_STATUS
FiaMuxUpdateSetupInformation (
  OUT    EFI_HII_HANDLE HiiAdvancedHandle,
      IN SPS_INFO_HOB   *SpsInfo
  );
#endif // _ME_SPS_FIA_MUX_H_
