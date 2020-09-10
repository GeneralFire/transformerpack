/** @file
  SsaMemoryHbmConfig.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#ifndef _SsaMemoryHbmConfig_h_
#define _SsaMemoryHbmConfig_h_

// BIOS-SSA Memory HBM API revision
#define SSA_REVISION_MEMORY_HBM ((0x01 << 24) | (0x00 << 16) | (0x00 << 8) | 0x00)

#pragma pack (push, 1)

///
/// Pseudo channel configuration setting
///
typedef enum {
  PSEUDO_CH_0      = 0,
  PSEUDO_CH_1      = 1,
  PSEUDO_CH_BOTH   = 2,
  PseudoChSelMax,
  PseudoChSelDelim = MAX_INT32
} PSEUDO_CH_SEL;

/**
  Function used to set the DqDB byte group pattern select.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based memory controller number.
  @param[in]      Channel    - Zero based memory channel number.
  @param[in]      SelBitmask - Specifies the byte group pattern selection options.

  Bits 7:0 of the bitmask correspond to pseudo channel 0 "byte groups" and bits
  15:8 correspond to pseudo channel 1 "byte groups".  "Byte groups" is in
  quotes because, for HBM, it consists of 8 Dq signals and the corresponding
  ECC signal.  A bit value of 0 indicates byte group option 0 which maps the
  byte group bit lanes [ECC+DQ7:DQ0] to the WDB bit lanes [DQ8:DQ0].  A bit
  value of 1 indicates byte group option 1 which maps the byte group bit lanes
  [ECC+DQ7:DQ0] to the WDB bit lanes [DQ17:DQ9].

  @retval Nothing.
**/
typedef
VOID
(EFIAPI * HBM_SET_DQDB_BYTE_GROUP_PAT_SEL) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN UINT16                    SelBitmask
  );

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
typedef
VOID
(EFIAPI * HBM_SET_DQDB_DBI_DATA_SOURCE) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   SourceFromDqBit,
  OUT UINT8                    DqBit
  );

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
typedef
VOID
(EFIAPI * HBM_SET_DQDB_DATA_PARITY_DATA_SOURCE) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   SourceFromDqBit,
  OUT UINT8                    DqBit
  );

/**
  Function used to configure whether duplicate the same cacheline transaction on both pseudo channels.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket        - Zero based CPU socket number.
  @param[in]      Controller    - Zero based memory controller number.
  @param[in]      Channel       - Zero based memory channel number.
  @param[in]      PseudoChannel - Pseudo channel configuration setting.

  @retval Nothing.
**/
typedef
VOID
(EFIAPI * HBM_SET_PSEUDO_CHANNEL_CONFIG) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN PSEUDO_CH_SEL             PseudoChannel
  );

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
typedef
SSA_STATUS
(EFIAPI * HBM_SET_PARITY_ERROR_CONFIG) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   IgnoreAwordParityError,
  IN BOOLEAN                   IgnoreDwordParityError
  );

/**
  Function used to get the parity error event capture status.

  @param[in, out] This                     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket                   - Zero based CPU socket number.
  @param[in]      Controller               - Zero based memory controller number.
  @param[in]      Channel                  - Zero based memory channel number.
  @param[out]     CurrentAwordParityError  - Pointer to where the current Aword parity error status for the given channel will be stored.
  @param[out]     ObservedAwordParityError - Pointer to where the observed Aword parity error status for the given channel will be stored.
  @param[out]     CurrentDwordParityError  - Pointer to where the current Dword parity error status for the given channel will be stored.
  @param[out]     ObservedDwordParityError - Pointer to where the observed Dword parity error status for the given channel will be stored.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.
**/
typedef
SSA_STATUS
(EFIAPI * HBM_GET_PARITY_ERROR_STATUS) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  OUT BOOLEAN                  *CurrentAwordParityError,
  OUT BOOLEAN                  *ObservedAwordParityError,
  OUT BOOLEAN                  *CurrentDwordParityError,
  OUT BOOLEAN                  *ObservedDwordParityError
  );

/**
  Function used to get the per-strobe error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based memory controller number.
  @param[in]      Channel    - Zero based memory channel number.
  @param[out]     Status     - Pointer to where the strobe error status will be stored.  Only the bits [3:0] are valid because HBM has a 32-bit strobe group.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 0 = strobe group 0; bit position 1 = strobe group 1, etc.

  @retval NotAvailable if the system does not provide strobe error status.  Else Success.
**/
typedef
SSA_STATUS
(EFIAPI * HBM_GET_STROBE_ERROR_STATUS) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  OUT UINT8                    *Status
  );

/**
  Function used to get or set the IO level flag.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      IsGet      - Specifies whether the operation is a get or set.  TRUE = get; FALSE = set.
  @param[in, out] MemSsType  - Pointer to where the MemSsType value will be/is stored. Set the value pointed to by MemSsType if IsGet is FALSE, while get the MemSsType if IsGet is TRUE

  @retval This function returns Success status codes
**/
typedef
SSA_STATUS
(EFIAPI * HBM_GET_SET_MEM_SS_TYPE) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN BOOLEAN                   IsGet,
  IN OUT MRC_LT                *MemSsType
  );

/**
  BIOS-SSA HBM Memory Configuration
**/
typedef struct _SSA_MEMORY_HBM_CONFIG {
  UINT32                               Revision;                 ///< Incremented when a backwards compatible binary change is made to the PPI.
  HBM_SET_DQDB_BYTE_GROUP_PAT_SEL      SetDqdbByteGroupPatSel;   ///< Function used to select the two byte group pattern options.
  HBM_SET_DQDB_DBI_DATA_SOURCE         SetDqdbDbiDataSource;         ///< Function used to set the DqDB source for the DBI signals.
  HBM_SET_DQDB_DATA_PARITY_DATA_SOURCE SetDqdbDataParityDataSource;  ///< Function used to set the DqDB source for the data parity signals.
  HBM_SET_PSEUDO_CHANNEL_CONFIG        SetPseudoChannelConfig;   ///< Function used to configure whether duplicate the same cacheline transaction on both pseudo channels.
  HBM_SET_PARITY_ERROR_CONFIG          SetParityErrorConfig;     ///< Function used to set the parity error event capture.
  HBM_GET_PARITY_ERROR_STATUS          GetParityErrorStatus;     ///< Function used to get the parity error status.
  HBM_GET_STROBE_ERROR_STATUS          GetStrobeErrorStatus;     ///< Function used to get the per-strobe error status.
  HBM_GET_SET_MEM_SS_TYPE              GetSetMemSsType;          ///< Function used to to get or set the IO level flag.
} SSA_MEMORY_HBM_CONFIG;

#pragma pack (pop)

#endif // _SsaMemoryHbmConfig_h_
