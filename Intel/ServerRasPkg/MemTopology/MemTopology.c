/** @file
  Implementation of ensuring PlatMemTopologyLib initialization code be invoked at boot phase. 

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#include <Library/PlatMemTopologyLib.h>

/**
  Entry point for MemTopology driver.

  This function initializes, and installs RAS Misc error handlers.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.
**/
EFI_STATUS
EFIAPI
InitializeMemtopology (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  MEM_TOPOLOGY      *MemTopology;
  //
  //Due to EDK2 build tool design limitation, it could not generated right sequence of constructor list at corner case(discrete node 
  //library dependency tree). It causes a couple of hang issues in drivers which use MemTopologyLib.
  //Hence it requires library should put as less initialization code at constructor as possible. This is why moving InitializeMemTopology 
  //out of constructor.
  //
  //this driver is to make sure InitializeMemTopology is invoked at boot phase due to above reason. This is initialization for SMM copy.
  //

  MemTopology = GetMemTopology ();

  return  EFI_SUCCESS;
}
