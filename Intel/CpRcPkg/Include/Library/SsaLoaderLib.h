/** @file
  SsaLoaderLib.h

  Library header with API to support SSA Loader

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _SSA_LOADER_LIB_H_
#define _SSA_LOADER_LIB_H_

/**
  Enable SSA Loader functionality (Target\Host mode)

  @retval EFI_SUCCESS           SSA Loader method Supported
  @retVal EFI_UNSUPPORTED       SSA Loader method Un-Supported

**/
EFI_STATUS
EFIAPI
EnableSsaBiosLoader (
  VOID
  );

/**
  Load SSA Checkpoint for Target Loader.
  Reads Checkpoint written during Target loader, and loads it in Scratchpad register for processing
  during MRC Training execution.

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.

  @retval N/A
**/
VOID
EFIAPI
SsaLoader (
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

#endif //_SSA_LOADER_LIB_H_
