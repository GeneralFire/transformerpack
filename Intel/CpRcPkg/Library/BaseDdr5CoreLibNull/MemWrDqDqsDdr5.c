/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include "Ddr5Core.h"
#include <Library/MemoryCoreLib.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>

//
// Local function prototypes
//
STATIC
VOID
WrDqDqsInit (
  PSYSHOST        Host,
  UINT8           socket
  );

STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST     Host,
  IN  UINT8        Socket,
  IN  UINT8        Dimm,
  IN  UINT8        Rank,
  OUT BErrorResult (*ErrorResult) [MAX_CH][SUB_CH]
  );

//
// Internal definitions
//
// Define the max number of sample in this algorithm
//
#define WRITE_DQ_DQS_MAX_SAMPLE (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)

/**

  Perform Write DQDQS training for DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteDqDqsPerBitDdr5 (
  IN PSYSHOST Host
  )
{

  return SUCCESS;

} //WriteDqDqsPerBitDdr5

/**

  Initialize Wr DQ/DQs parameters

  @param Host                - Pointer to sysHost
  @param socket                - Socket number

  @retval N/A

**/
STATIC
VOID
WrDqDqsInit (
  PSYSHOST        Host,
  UINT8           socket
  )
{
  return;

} // WrDqDqsInit



/*++

  This function collects all the data from DqDqs algorithm

  @param Host                - Pointer to sysHost
  @param socket                - Socket number
  @param dimm                - DIMM number
  @param rank                - Rank number
  ErrorResult         - Error result return data

  @retval 0 - successful
  @retval Other - failure

--*/
STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST     Host,
  IN  UINT8        Socket,
  IN  UINT8        Dimm,
  IN  UINT8        Rank,
  OUT BErrorResult (*ErrorResult) [MAX_CH][SUB_CH]
  )
{
  return SUCCESS;
} // FindPerBitResults

