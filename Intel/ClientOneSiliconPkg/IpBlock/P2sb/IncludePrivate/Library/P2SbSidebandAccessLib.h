/** @file
  Header for P2SbSidebandAccessLib

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _P2SB_SIDEBAND_ACCESS_LIB_H_
#define _P2SB_SIDEBAND_ACCESS_LIB_H_

#include <RegisterAccess.h>
#include <Library/PchPcrLib.h> // For PCH_SBI_PID definition

typedef PCH_SBI_PID  P2SB_PID;

typedef enum {
  MemoryRead             = 0x0,
  MemoryWrite            = 0x1,
  PciConfigRead          = 0x4,
  PciConfigWrite         = 0x5,
  PrivateControlRead     = 0x6,
  PrivateControlWrite    = 0x7,
  GpioLockUnlock         = 0x13
} P2SB_SBI_OPCODE;

typedef enum {
  SBI_SUCCESSFUL          = 0,
  SBI_UNSUCCESSFUL        = 1,
  SBI_POWERDOWN           = 2,
  SBI_MIXED               = 3,
  SBI_INVALID_RESPONSE
} P2SB_SBI_RESPONSE;

typedef enum {
  P2SbMmioAccess = 0,
  P2SbMsgAccess
} P2SB_SIDEBAND_ACCESS_METHOD;

typedef struct {
  UINT64  PciConfig;
  UINT32  Mmio;
} P2SB_DEV;

/**
  REGISTER_ACCESS for P2SB device to support access to sideband registers.
  Be sure to keep first member of this structure as REGISTER_ACCESS to allow
  for correct casting between caller who sees this structure as REGISTER_ACCESS
  and calle who will cast it to P2SB_SIDEBAND_REGISTER_ACCESS.
**/
typedef struct {
  REGISTER_ACCESS              Access;
  P2SB_SIDEBAND_ACCESS_METHOD  AccessMethod;
  P2SB_PID                     P2SbPid;
  UINT16                       Fid;
  P2SB_SBI_OPCODE              Opcode;
  P2SB_DEV                     *P2SbDev;
} P2SB_SIDEBAND_REGISTER_ACCESS;

/**
  Reads an 8-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 8-bit register value specified by Offset
**/
UINT8
P2SbSidebandRead8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  );

/**
  Writes an 8-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 8-bit register value written to register
**/
UINT8
P2SbSidebandWrite8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            Value
  );

/**
  Performs an 8-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
P2SbSidebandOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            OrData
  );

/**
  Performs an 8-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 8-bit register value written to register
**/
UINT8
P2SbSidebandAnd8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData
  );

/**
  Performs an 8-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
P2SbSidebandAndThenOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData,
  IN UINT8            OrData
  );

/**
  Reads a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 16-bit register value specified by Offset
**/
UINT16
P2SbSidebandRead16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  );

/**
  Writes a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 16-bit register value written to register
**/
UINT16
P2SbSidebandWrite16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16            Value
  );

/**
  Performs a 16-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
P2SbSidebandOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           OrData
  );

/**
  Performs a 16-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 16-bit register value written to register
**/
UINT16
P2SbSidebandAnd16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData
  );

/**
  Performs a 16-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
P2SbSidebandAndThenOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData,
  IN UINT16           OrData
  );

/**
  Reads a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 32-bit register value specified by Offset
**/
UINT32
P2SbSidebandRead32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  );

/**
  Writes a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 32-bit register value written to register
**/
UINT32
P2SbSidebandWrite32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32            Value
  );

/**
  Performs a 32-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
P2SbSidebandOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           OrData
  );

/**
  Performs a 32-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 32-bit register value written to register
**/
UINT32
P2SbSidebandAnd32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData
  );

/**
  Performs a 32-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
P2SbSidebandAndThenOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData,
  IN UINT32           OrData
  );

#endif
