/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef __USRA_REGISTER_FILTER_LIB_API_H__
#define __USRA_REGISTER_FILTER_LIB_API_H__


#include <UsraAccessType.h>

//////////////////////////////////////////////////////////////////////////
//
// USRA Register Filter Library
//
//////////////////////////////////////////////////////////////////////////

/**
  Perform register read.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be read out.
  @param[in]  AlignedAddress       An address to be read out.
  @param[out] Buffer               A pointer of buffer contains the data to be read out.

  @retval  0                       Register access is not issued.
  @return  Non-zero                Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterFilterRead (
  IN  USRA_ADDRESS             *Address,
  IN  UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );

/**
  Perform register write.

  @param[in]  Address              A pointer of the address of the USRA Address Structure to be written.
  @param[in]  AlignedAddress       An address to be written.
  @param[in]  Buffer               A pointer of buffer contains the data to be written.

  @retval  0                       Register access is not issued.
  @return  Non-zero                Register access is issued.
**/
RETURN_STATUS
EFIAPI
UsraRegisterFilterWrite (
  IN  USRA_ADDRESS             *Address,
  IN  UINTN                    AlignedAddress,
  IN  VOID                     *Buffer
  );

#endif
