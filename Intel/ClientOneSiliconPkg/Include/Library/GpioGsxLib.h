/** @file
  Header file for GpioGsxLib.
  All function in this library is available for PEI, DXE, and SMM

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _GPIO_GSX_LIB_H_
#define _GPIO_GSX_LIB_H_

/**
    GSX initialization code to configure input and output numbers.

    @param GpioCom    GPIO Community where GSX lives. Refer to GSX_GROUP_INFO in GpioGsxLib.h.
    @param NIn        Number of input expanders.
    @param NOut       Number of output expanders.

**/
EFI_STATUS
InitGSX (
  UINT16 GpioCom,
  UINT16 NIn,
  UINT16 NOut
  );

/**
    Returns the value of the CxGPI received over the GSX channel.

    @param GpioCom    GPIO Community where GSX lives. Refer to GSX_GROUP_INFO in GpioGsxLib.h.
    @param *Data      Address where Dword0 and Dword1 from CxGPI will be returned.

    @retval Status EFI_SUCCESS       GSX read success.
    @retval Status EFI_UNSUPPORTED   GSX is busy and cannot be accessed.

**/
EFI_STATUS
GsxRead (
  UINT16                GpioCom,
  UINT32                *Data
  );

#endif // _GPIO_GSX_LIB_H_
