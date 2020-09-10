/** @file
  Header file for GPIO GSX Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _GPIO_GSX_LIBRARY_H_
#define _GPIO_GSX_LIBRARY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/PchPcrLib.h>

typedef struct {
  PCH_SBI_PID  Community;
  UINT16       CapOffset;
  UINT16       C0CapDw0Offset;
  UINT16       C0CapDw1Offset;
  UINT16       C0GpiLvlDw0Offset;
  UINT16       C0GpiLvlDw1Offset;
  UINT16       C0GpoLvlDw0Offset;
  UINT16       C0GpoLvlDw1Offset;
  UINT16       C0CmdOffset;
  UINT16       C0TmOffset;
} GSX_GROUP_INFO;

#endif // _GPIO_GSX_LIBRARY_H_
