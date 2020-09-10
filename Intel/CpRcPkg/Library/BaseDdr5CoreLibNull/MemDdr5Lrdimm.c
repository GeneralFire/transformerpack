/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryCoreLib.h>
#include <Memory/JedecDefinitions.h>

//
// Local Function prototypes
//


/**

  Perform DDR5 LRDIMM Backside Read Training (RX)

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
Ddr5LrdimmMrePhaseCycleTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} //Ddr5LrdimmMrePhaseCycleTraining

/**

  Retrieves the Ddr5LrdimmTrainingData structure pointer from the static pointer table.  If
  it has not yet been initialized, resources are allocated and the pointer is set.

  @param[in,out] Ddr5LrdimmTrainingData  - Pointer to the DDR5_LRDIMM_TRAINING_DATA structure

  @retval SUCCESS

**/
MRC_STATUS
EFIAPI
GetDdr5LrdimmTrainingData (
  IN OUT DDR5_LRDIMM_TRAINING_DATA  **Ddr5LrdimmTrainingData
  )
{
  return SUCCESS;
} //GetDdr5LrdimmTrainingData

/**

Set PG[x]RWE6 and PG[x]RWE7 to the respective settings that are in the center of the passing region.

@param[in] Socket            - Socket number
@param[in] Ch                - Channel number
@param[in] Dimm              - Dimm number
@param[in] Rank              - Rank number
@param[in] CenterPoint       - Strobe number

@retval Status

**/
UINT32
EFIAPI
SetLrdimmMwdResultsDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT16    CenterPoint[MAX_STROBE]
  )
{
  return SUCCESS;
} //SetLrdimmMwdResultsDdr5

/**

Set PG[x]RWE4 and PG[x]RWE5 to the respective settings that are in the center of the passing region.

@param[in] Socket            - Socket number
@param[in] Ch                - Channel number
@param[in] Dimm              - Dimm number
@param[in] Rank              - Rank number
@param[in] CenterPoint       - Center Point

@retval Status

**/
UINT32
EFIAPI
SetLrdimmMrdResultsDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT16    CenterPoint[MAX_STROBE]
  )
{
  return SUCCESS;
} //SetLrdimmMrdResultsDdr5

/**

  Initialize CPGC for LRDIMM Write Training


  @param[in] Socket    - Socket number
  @param[in] ChBitmask - mask of channels to be used in this CPGC test
  @param[in] NumCL     - CL number
  @param[in] LoopCount - Loop Count

**/
VOID
EFIAPI
SetupLrdimmReadWriteTestDdr5 (
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     NumCL,
  IN UINT8     LoopCount
  )
{
  return;
} //SetupLrdimmReadWriteTestDdr5

/**

DDR5 backside training DB LFSR setup.

@retval Status

**/
UINT32
EFIAPI
DbLfsrDdr5 (
  VOID
  )
{
  return SUCCESS;
}

/**

  Perform DDR5 LRDIMM Backside Read Training (RX)

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
Ddr5LrdimmBacksideRxDelay (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} //Ddr5LrdimmBacksideRxDelay

/**

Perform LRDIMM Backside Training (TX)

@param Host  - Pointer to sysHost

@retval Status

**/
UINT32
EFIAPI
Ddr5LrdimmBacksideTxDelay (
  PSYSHOST  Host
  )
{
  return SUCCESS;
} //Ddr5LrdimmBacksideTxDelay

/**

  Initialize LRDIMM MDQ Receive Enable Cycle Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank to sysHost
  @param[in] Step       -

  @retval N/A

**/
STATIC
VOID
EFIAPI
LrdimmCycleTrainingInit (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Step
  )
{
  return;
} // LrdimmCycleTrainingInit

/**

  Initialize LRDIMM MDQ Receive Enable Cycle Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank number

  @retval N/A

**/
STATIC
VOID
EFIAPI
LrdimmMrdInit (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  return;
} // LrdimmMrdInit


/**

  Initialize LRDIMM MDQS Receive Enable Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank to sysHostr

  @retval N/A

**/
STATIC
VOID
EFIAPI
LrdimmMdqsRecInit (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  return;
} // LrdimmMdqsRecInit

/**

  Initialize LRDIMM MDQ-MDQS Write Delay Training Parameters

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] Dimm       - Dimm number
  @param[in] Rank       - Rank to sysHostr

  @retval N/A

**/
STATIC
VOID
EFIAPI
LrdimmMwdInit (
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  return;
} // LrdimmMwdInit


/**

  Perform LRDIMM MDQ Receive Enable Phase Training (MREP)

  @param[in] Socket - Socket number

  @retval Status

**/
UINT32
EFIAPI
MDQSReceiveEnablePhaseCycleTraining (
  IN UINT8     Socket
  )
{
  return SUCCESS;
} //MDQSReceiveEnablePhaseCycleTraining

/**

  Restore Data Buffer Rank Presence Settings:

  Restore the Rank presence settings for data buffers associated with a
  particular DIMM.

  @param[in] Socket - Socket number
  @param[in] Dimm   - DIMM number

  @retval Status

**/
UINT32
EFIAPI
RestoreDimmRankPresence (
  IN UINT8     Socket,
  IN UINT8     Dimm
  )
{
  return SUCCESS;
} //RestoreDimmRankPresence


