/** @file
  AdvMemTestLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef  _ADV_MEM_TEST_LIB_H_
#define  _ADV_MEM_TEST_LIB_H_

#include "SysHost.h"
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <MemDecodeCommonIncludes.h>


//
// Advanced MemTest interface
//

/**

  Runs Advanced Memory Test algorithms with CPGC and logs any failures

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval - SUCCESS

**/
UINT32
AdvancedMemTest (
  PSYSHOST Host               // Pointer to the system Host (root) structure
  );


/**

  Runs MemTest

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
DdrMemTest (
  PSYSHOST Host               // Pointer to the system Host (root) structure
  );


/**

  Runs Memory Initialization

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
DdrMemInit (
  PSYSHOST Host               // Pointer to the system Host (root) structure
  );


/**

  Returns whether memory error injection is enabled

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param Socket - Socket number (0-based)
  @param Ch     - Ddr channel within the socket

  @retval 1 = Error injection enabled; 0 = disabled

**/
UINT8
GetInjectErrorMATS (
  PSYSHOST  Host,
  UINT8     Ch
  );

/**
  This function checks if the DDR memory initialization with all 0's pattern is done.

  @retval TRUE        DDR memory initialization is done.
  @retval FALSE       DDR memory initialization is not done.
**/
BOOLEAN
IsDdrMemInitDone (
  VOID
  );

/**

  Runs optimized Memory Init to 0 data pattern with DDR scrambler enabled

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be initialized

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemInitOpt (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   ddrChEnabled
  );

#endif // #ifndef  _ADV_MEM_TEST_LIB_H_
