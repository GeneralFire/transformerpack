/** @file

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

#ifndef __MEM_RC_LIB_H__
#define __MEM_RC_LIB_H__

#include "SysHost.h"

/**
  Get memory NUMA support option

  @retval TRUE          NUMA support is turned on
  @retval FALSE         NUMA support is turned off

**/
BOOLEAN
EFIAPI
IsNumaAware (
  VOID
  );

/**
  Get cluster XOR support option

  @retval TRUE          Cluster XOR support is turned on
  @retval FALSE         Cluster XOR support is turned off

**/
BOOLEAN
EFIAPI
IsClusterXorEnable (
  VOID
  );

/**
  Get prefetch option based on system memory size

  @return prefetch option

**/
UINT8
EFIAPI
GetPrefetchOption (
  VOID
  );

/**
  Get total memory size in 64MB unit for a specific socket

  @param[in] SocketId         The socket id

  @return the total memory size in 64MB unit for the whole system

**/
UINT32
EFIAPI
GetSocketMemSize (
  IN UINT8      SocketId
  );

/**
  Get total memory size in 64MB unit for a specific socket

  @param[in] SocketId         The socket id
  @param[in] ChannelId        The channel id

  @return the total memory size in 64MB unit for the whole system

**/
UINT32
EFIAPI
GetChannelMemSize (
  IN UINT8      SocketId,
  IN UINT8      ChannelId
  );

/**
  Get DDR channel interleaved memory size in 64MB unit for a specified channel

  @param[in] SocketId         The socket id
  @param[in] ChannelId        The channel id on socket

  @return DDR channel interleaved memory size in 64MB unit

**/
UINT32
EFIAPI
GetChannelVolMemInterleaveSize (
  IN UINT8      SocketId,
  IN UINT8      ChannelId
  );

/**
  Get Socket Xor Mode

  @param[in] SocketIndex     - Socket ID.

  @return Xor Mode

**/
UINT8
EFIAPI
GetSocketXorMode (
  IN  UINT8    SocketIndex
  );

/**
  This function checks if the biased 2-way near memory cache is enabled on system.

  @retval TRUE    Biased 2-way near memory cache is enabled.
  @retval FALSE   Biased 2-way near memory cache is disabled.
**/
BOOLEAN
EFIAPI
IsTwoWayNmCacheEnabled (
  VOID
  );

/**
  This function checks if the fADR feature is enabled on system.

  @retval TRUE      fADR is enabled
  @retval FALSE     fADR is disabled.

**/
BOOLEAN
EFIAPI
IsFadrEnabled (
  VOID
  );

/**
  Check if the mc in a socket is enabled

  @param[in] SocketId         The socket id
  @param[in] ImcId            The iMC id

  @return whether the mc in a socket is enabled

**/
BOOLEAN
EFIAPI
CheckSocketMcEnabled (
  IN UINT8     SocketId,
  IN UINT8     ImcId
  );

/**
  Check if the mc in a socket is enabled

  @param[in] SocketId         The socket id

  @return whether the socket is enabled

**/
BOOLEAN
EFIAPI
CheckSocketEnabled (
  IN UINT8     SocketId
  );

/**
  Check if AEP DIMM in a socket is present

  @param[in] SocketId         The socket id

  @return whether AEP DIMM is present in a socket

**/
BOOLEAN
EFIAPI
CheckSocketDcpmmPresent (
  IN UINT8     SocketId
  );

/**
  Get RAS mode

  @return system RAS mode

**/
UINT8
EFIAPI
GetRasMode (
  VOID
  );

/**
  Get Dram type

  @return the dram type in a system

**/
UINT16
EFIAPI
GetDramType (
  VOID
  );

/**
  Get the caching memory type.

  @retval 0  DDR is caching DDRT (CACHE_TYPE_DDR_CACHE_PMEM)
  @retval 1  HBM is caching DDR  (CACHE_TYPE_HBM_CACHE_DDR)

**/
UINT8
EFIAPI
GetCacheMemType (
  VOID
  );

/**
  Get the effective volatile memory mode.

  @retval 0  The memory mode is 1LM        (VOL_MEM_MODE_1LM)
  @retval 1  The memory mode is 2LM        (VOL_MEM_MODE_2LM)
  @retval 2  The memory mode is 1LM or 2LM (VOL_MEM_MODE_MIX_1LM2LM)

**/
UINT8
EFIAPI
GetVolMemMode (
  VOID
  );

typedef enum {
  All2All,
  HemiClusteringMode,
  QuadClusteringMode,
  Snc2ClusteringMode,
  Snc4ClusteringMode,
  MaxClusteringMode
} CLUSTERING_MODE;

/**
  Returns system clustering mode for DDR memory after memory decode has been executed.

  @return the ddr clustering mode.

**/
CLUSTERING_MODE
EFIAPI
GetDdrClusteringMode (
  VOID
  );

/**
  Get the CSR offset value for SBSP to check in.

  @return the CSR offset value.

**/
UINT32
EFIAPI
GetSrPbspCheckinCsr (
  VOID
  );

/**
  Indicate if an AEP DIMM is installed in any slot on a channel
  Provides the index of the first AEP DIMM if one is found.

  @param Socket   - Socket number
  @param Ch       - Channel number
  @param Dimm     - Pointer to dimm number - updated by this routine
                    with the 0-based index of the first AEP DIMM found.
                    This parameter is optional and may be NULL.

  @retVal   TRUE  if an AEP DIMM is installed and "dimm" updated
            FALSE otherwise, "dimm" not modified
**/
BOOLEAN
AepDimmPresent (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     *Dimm   OPTIONAL
  );

/**
  Indicate if an NVDIMM-N is installed in any slot on a channel
  Provides the index of the first NVDIMM-N if one is found.

  @param[in]  Socket   - Socket number
  @param[in]  Ch       - Channel number
  @param[out] Dimm     - Pointer to dimm number - updated by this routine
                         with the 0-based index of the first NVDIMM-N DIMM found.
                         This parameter is optional and may be NULL.

  @retVal   TRUE  if an NVDIMM-N DIMM is installed and "dimm" optionally updated if Dimm is not NULL
            FALSE otherwise, "dimm" not modified
**/
BOOLEAN
NvdimmNPresent (
  IN  UINT8 Socket,
  IN  UINT8 Ch,
  OUT UINT8 *Dimm OPTIONAL
  );

/**

  Chip specific code path to halt the system

  @param[in[ HaltOnError - To halt or not on error

  @retval None

**/

VOID
RcDeadLoop (
  UINT8 HaltOnError
  );

/**
  Gets the DDR freqency in MHz using the macro encoding and
  updates the Host

  @param Socket           - Socket number

  @retval                - Data value for Ddr Freq in Mhz
**/
UINT16
EFIAPI
GetDdrFreqInMhz (
  IN     UINT8    Socket
  );

/**
  Get the effective NVM CR QOS setting.

  @retval TRUE   NVM CR QOS is either 1 of the recipes chosen
  @retval FALSE  NVM CR QOS is disabled

**/
UINT8
EFIAPI
GetNvmCrQos (
  VOID
  );

/**
  Checks if a DDRT DIMM is present on the MC specified

  @param[in] Socket  Socket ID
  @param[in] McId    Mc Controller Id

  @retval TRUE if DDRT is present in the MC
          FALSE if DDRT is not present in the MC
**/
BOOLEAN
EFIAPI
IsDdrtPresentMC (
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**
  Check if there is any 9x4 DDR5 DIMM present on system.

  @retval TRUE    9x4 DDR5 DIMM is present on system.
  @retval FALSE   9x4 DDR5 DIMM is not present on system.
**/
BOOLEAN
EFIAPI
Check9x4Ddr5DimmPresent (
  VOID
  );



/**
  Check if there is any 9x4 DDR5 DIMM present on channel.

  @param[in]   Socket  -- socket index
  @param[in]   Ch  --  channel index

  @retval TRUE    9x4 DDR5 DIMM is present on system.
  @retval FALSE   9x4 DDR5 DIMM is not present on system.
**/
BOOLEAN
EFIAPI
Check9x4Ddr5DimmPresentPerChn (
  IN       UINT8       Socket,
  IN       UINT8       Ch
  );


/**
  Enable CSR prints to log

  @param[in] True or False

**/
VOID
EFIAPI
SetCsrPrintEnabling (
  IN  BOOLEAN      PrintCsr
  );

/**
  Get and return Host PrintCsr value

  @retval TRUE  CSR prints to log is enabled
          FALSE CSR prints to log is not enabled
**/
BOOLEAN
EFIAPI
GetCsrPrintEnabling (
  VOID
  );

/**
  Detects whether the Memory Subsystem is DDR5 type

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number

  @retval TRUE  if DDR5 memory subsystem is present
  @retval FALSE if DDR5 memory subsytem is not present
**/
BOOLEAN
EFIAPI
IsDdr5MemSsPresent (
  PSYSHOST        Host,
  UINT32          Socket
  );

/**
Detects whether Bretton Sound DDR5 POC is present

@param[in]    Host        - Pointer to sysHost structure
@param[in]    Socket      - Socket number

@retval 1 if DDR5 memory subsystem is present
@retval 0 if DDR5 memory subsytem is not present
**/
BOOLEAN
EFIAPI
IsBrsPresent (
  IN PSYSHOST  Host,
  IN UINT32    Socket
  );

/**
  Detects whether Denton Creek (DNT) is present

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number

  @retval TRUE  DNT is present
  @retval FALSE DNT is not present
**/
BOOLEAN
EFIAPI
IsDntPresent (
  IN        PSYSHOST            Host,
  IN        UINT32              Socket
  );
#endif // __MEM_RC_LIB_H__
