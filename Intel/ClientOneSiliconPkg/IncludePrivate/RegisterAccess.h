/** @file
  Header file for register access.

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

#ifndef _REGISTER_ACCESS_H_
#define _REGISTER_ACCESS_H_

typedef struct _REGISTER_ACCESS REGISTER_ACCESS;

/**
  Reads an 8-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 8-bit register value specified by Offset
**/
typedef
UINT8
(*REG_READ8) (
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
typedef
UINT8
(*REG_WRITE8) (
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
typedef
UINT8
(*REG_OR8) (
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
typedef
UINT8
(*REG_AND8) (
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
typedef
UINT8
(*REG_AND_THEN_OR8) (
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
typedef
UINT16
(*REG_READ16) (
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
typedef
UINT16
(*REG_WRITE16) (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           Value
  );

/**
  Performs a 16-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
typedef
UINT16
(*REG_OR16) (
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
typedef
UINT16
(*REG_AND16) (
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
typedef
UINT16
(*REG_AND_THEN_OR16) (
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
typedef
UINT32
(*REG_READ32) (
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
typedef
UINT32
(*REG_WRITE32) (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           Value
  );

/**
  Performs a 32-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
typedef
UINT32
(*REG_OR32) (
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
typedef
UINT32
(*REG_AND32) (
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
typedef
UINT32
(*REG_AND_THEN_OR32) (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData,
  IN UINT32           OrData
  );

struct _REGISTER_ACCESS {
  REG_READ8         Read8;
  REG_WRITE8        Write8;
  REG_OR8           Or8;
  REG_AND8          And8;
  REG_AND_THEN_OR8  AndThenOr8;

  REG_READ16         Read16;
  REG_WRITE16        Write16;
  REG_OR16           Or16;
  REG_AND16          And16;
  REG_AND_THEN_OR16  AndThenOr16;

  REG_READ32         Read32;
  REG_WRITE32        Write32;
  REG_OR32           Or32;
  REG_AND32          And32;
  REG_AND_THEN_OR32  AndThenOr32;
};

#endif
