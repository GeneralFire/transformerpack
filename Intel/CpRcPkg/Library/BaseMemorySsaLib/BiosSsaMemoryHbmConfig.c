/** @file
  This file contains the BIOS implementation of the BIOS-SSA Memory HBM API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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
#include <Library/SysHostPointerLib.h>

/**
  Function used to set the DqDB Byte group pattern select.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based memory controller number.
  @param[in]      Channel    - Zero based memory channel number.
  @param[in]      SelBitmask - Specifies the Byte group pattern selection options.

  Bits 7:0 of the bitmask correspond to pseudo channel 0 "Byte groups" and bits
  15:8 correspond to pseudo channel 1 "Byte groups".  "Byte groups" is in
  quotes because, for HBM, it consists of 8 Dq signals and the corresponding
  ECC signal.  A bit value of 0 indicates Byte group option 0 which maps the
  Byte group bit lanes [ECC+DQ7:DQ0] to the WDB bit lanes [DQ8:DQ0].  A bit
  value of 1 indicates Byte group option 1 which maps the Byte group bit lanes
  [ECC+DQ7:DQ0] to the WDB bit lanes [DQ17:DQ9].

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetDqdbByteGroupPatSel (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN UINT16                    SelBitmask
  )
{
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetDqdbByteGroupPatSel (Socket:%d, Controller:%d)\n", Socket, Controller);
  RcDebugPrint (SDBG_BSSA,"End: BiosSetDqdbByteGroupPatSel\n");
 
}

/**
  Function used to set the DqDB source for the DBI signals.

  @param[in, out] This            - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket          - Zero based CPU socket number.
  @param[in]      Controller      - Zero based memory controller number.
  @param[in]      Channel         - Zero based memory channel number.
  @param[in]      SourceFromDqBit - Specifies whether the DBI is sourced from DBI calculation in memory controller (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out]     DqBit           - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetDqdbDbiDataSource (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   SourceFromDqBit,
  OUT UINT8                    DqBit
  )
{

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetDqdbDbiDataSource (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosSetDqdbDbiDataSource\n");
}

/**
  Function used to set the DqDB source for the data parity signals.

  @param[in, out] This            - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket          - Zero based CPU socket number.
  @param[in]      Controller      - Zero based memory controller number.
  @param[in]      Channel         - Zero based memory channel number.
  @param[in]      SourceFromDqBit - Specifies whether the data parity is sourced from data parity calculation in memory controller (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out]     DqBit           - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetDqdbDataParityDataSource (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   SourceFromDqBit,
  OUT UINT8                    DqBit
  )
{

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetDqdbDataParityDataSource (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosSetDqdbDataParityDataSource\n");
}

/**
  Function used to configure whether duplicate the same cacheline transaction on both pseudo channels.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket        - Zero based CPU socket number.
  @param[in]      Controller    - Zero based memory controller number.
  @param[in]      Channel       - Zero based memory channel number.
  @param[in]      PseudoChannel - Pseudo channel configuration setting.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetPseudoChannelConfig (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN PSEUDO_CH_SEL             PseudoChannel
  )
{

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetPseudoChannelConfig (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosSetPseudoChannelConfig\n");
}

/**
  Function used to configure the parity error event capture.

  @param[in, out] This                   - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket                 - Zero based CPU socket number.
  @param[in]      Controller             - Zero based memory controller number.
  @param[in]      Channel                - Zero based memory channel number.
  @param[in]      IgnoreAwordParityError - Specifies whether to ignore Aword parity for the given channel.
  @param[in]      IgnoreDwordParityError - Specifies whether to ignore Dword parity for the given channel.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.
**/
SSA_STATUS
EFIAPI 
BiosSetParityErrorConfig (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   IgnoreAwordParityError,
  IN BOOLEAN                   IgnoreDwordParityError
  )
{

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetParityErrorConfig (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosSetParityErrorConfig\n");
  return Success;
}

/**
  Function used to get the parity error event capture Status.

  @param[in, out] This                     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket                   - Zero based CPU socket number.
  @param[in]      Controller               - Zero based memory controller number.
  @param[in]      Channel                  - Zero based memory channel number.
  @param[out]     CurrentAwordParityError  - Pointer to where the current Aword parity error Status for the given channel will be stored.
  @param[out]     ObservedAwordParityError - Pointer to where the observed Aword parity error Status for the given channel will be stored.
  @param[out]     CurrentDwordParityError  - Pointer to where the current Dword parity error Status for the given channel will be stored.
  @param[out]     ObservedDwordParityError - Pointer to where the observed Dword parity error Status for the given channel will be stored.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.
**/
SSA_STATUS
EFIAPI 
BiosGetParityErrorStatus (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  OUT BOOLEAN                  *CurrentAwordParityError,
  OUT BOOLEAN                  *ObservedAwordParityError,
  OUT BOOLEAN                  *CurrentDwordParityError,
  OUT BOOLEAN                  *ObservedDwordParityError
  )
{

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetParityErrorStatus (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosGetParityErrorStatus\n");
  return Success;
}

/**
  Function used to get the per-strobe error Status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based memory controller number.
  @param[in]      Channel    - Zero based memory channel number.
  @param[out]     Status     - Pointer to where the strobe error Status will be stored.  Only the bits [3:0] are valid because HBM has a 32-bit strobe group.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 0 = strobe group 0; bit position 1 = strobe group 1, etc.

  @retval NotAvailable if the system does not provide strobe error Status.  Else Success.
**/
SSA_STATUS
EFIAPI 
BiosGetStrobeErrorStatus (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  OUT UINT8                    *Status
  )
{
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetStrobeErrorStatus (Socket:%d, Controller:%d)\n", Socket, Controller);

  RcDebugPrint (SDBG_BSSA,"End: BiosGetStrobeErrorStatus\n");
  return Success;
}

/**
  Function used to get or set the IO level flag.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      IsGet      - Specifies whether the operation is a get or set.  TRUE = get; FALSE = set.
  @param[in, out] MemSsType  - Pointer to where the MemSsType value will be/is stored. Set the value pointed to by MemSsType if IsGet is FALSE, while get the MemSsType if IsGet is TRUE

  @retval This function returns Success Status codes
**/

SSA_STATUS
EFIAPI 
BiosGetSetMemSsType (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN BOOLEAN                   IsGet,
  IN OUT MRC_LT                *MemSsType
  )
{
  
  return Success;
}

// end file BiosSsaMemoryHbmConfig.c
