/** @file
  Header file for the Base Memory Services library implementation.

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

#ifndef _SSA_LOADER_LIB_INTERNAL_H_
#define _SSA_LOADER_LIB_INTERNAL_H_

#include <SysHost.h>

/**

  Do EV commands while at a the RUN_EV_TOOL checkpoint

  @param[in] Host    -   Pointer to the system structure

  @retval None

**/
VOID
EvItpDownloadCmd (
  IN PSYSHOST  Host
  );

/**

  Execute BIOS SSA Target loaded tool

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/

VOID
ExecuteTargetOnlyCmd (
  IN PSYSHOST Host
  );

//
// Prototypes for functions used inside this implementation.
//

/**

  Copy ITP downloaded content from DCU into MLC

**/
VOID 
FlushBios (
  VOID
  );

/**

  Modify the image base field of the PE Header of the output file.

  @param baseAddr - Base Address for the file

  @retval None

**/
VOID
PlacePeImage (
  UINT8 *baseAddr
  );

/**

  Get the PE entry point

  @param baseAddress - Pointer to the base address

  @retval PE Entry Point Address

**/
UINT32 GetPeEntrypoint(
  UINT8 *baseAddress  // Pointer to the base address
  );

#endif  // _SSA_LOADER_LIB_INTERNAL_H_
