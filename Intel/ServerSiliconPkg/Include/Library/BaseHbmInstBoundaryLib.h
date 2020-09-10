/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2020 Intel Corporation.

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

#define SYS_TO_HBM_IMC(SysMcId) (SysMcId - MAX_IMC)
#define SYS_TO_HBM_CH(SysChId) (SysChId - (MAX_IMC * MAX_MC_CH))

/*
  This function identifies the memory controller id is located in HBM scope

  @param[in] McId       The system memory controller id within socket

  @retval TRUE/FALSE    Return TRUE if McId is HBM MC. Otherwise FALSE.
*/
BOOLEAN
EFIAPI
IsHbmMc (
  IN UINT8             McId
  );

/*
  This function identifies the memory channel id is located in HBM scope

  @param[in] ChId       The system memory channle id within socket

  @retval TRUE/FALSE    Return TRUE if ChId is HBM Channel. Otherwise FALSE.
*/
BOOLEAN
EFIAPI
IsHbmCh (
  IN UINT8             ChId
  );
