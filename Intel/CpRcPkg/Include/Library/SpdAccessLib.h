/** @file
  The SPD Access Library API provides the necessary functions to initiate SPD
  read/write transactions.

  This API is designed to function as an interface between an agent that needs
  to read/write to a DIMM SPD and a lower level library (such as an SMBus library)
  which handles the actual transactions.  The read/write functions accept DIMM
  location information as well as the SPD byte offset and should then handle
  the steps necessary to initiate (for example) a SMBus transaction to do the
  reading/writing.  Functions are also provided to initialize any data/setup
  steps needed before attempting a read/write transaction and to communicate to
  the library that DIMM detection is complete providing a way for the library
  to know that it can check for a DIMM's presence bofore initiating a transaction.

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

#ifndef _SPD_ACCESS_LIB_H_
#define _SPD_ACCESS_LIB_H_

//
// DDR Technology supported
//
typedef enum {
  Ddr4Type = 0,         // DDR4 Technology support
  Ddr5Type,             // DDR5 Technology support
  DdrMaxType            // Enum limit to check valid value
} DDR_TECHNOLOGY_TYPE;

/**
  Initializes any SPD related data structures and prepares the system for
  initiating SPD reads via the SMBus if necessary.  This must be called
  before attempting to read/write to SPD.

  @param[in] Socket   - Socket ID
  @param[in] DdrType  - Type of DDR Technology supported

  @retval EFI_SUCCESS       Successfully initialized SPD related functionality
  @retval EFI_DEVICE_ERROR  Error initializing devices
**/
EFI_STATUS
EFIAPI
InitializeSpd (
  IN UINT8                Socket,
  IN DDR_TECHNOLOGY_TYPE  DdrType
  );

/**
  Performs SPD byte read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  ByteOffset  - SPD Byte Offset to be read
  @param[out] Data        - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdReadByte (
  IN UINT8      Socket,
  IN UINT8      Chan,
  IN UINT8      Dimm,
  IN UINT16     ByteOffset,
  OUT UINT8     *Data
  );

/**
  Performs SPD byte write operations.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] ByteOffset  - SPD Byte Offset to be written
  @param[in] Data        - Pointer to data to write

  @retval RETURN_SUCCESS         The write completed successfully.
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdWriteByte (
  IN UINT8   Socket,
  IN UINT8   Chan,
  IN UINT8   Dimm,
  IN UINT16  ByteOffset,
  IN UINT8   *Data
  );

/**
  Indicates to the Library that DIMM detection is complete.

  @param  None

  @retval RETURN_SUCCESS  DIMM detection done successfully communicated.
  @retval EFI_NOT_READY   Cannot store DIMM detection done indicator.
**/
EFI_STATUS
EFIAPI
SpdDimmDetectDone (
  VOID
  );

/**
  Performs read operations to SPD Hub internal register (Integrated Thermal Sensor)

  @param[in]  Socket      - Socket ID
  @param[in]  Channel     - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  Register    - Internal register to be read
  @param[out] Data        - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdHubReadByte (
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Register,
  OUT UINT8     *Data
  );

/**
  Performs write operations to SPD Hub internal register (Integrated Thermal Sensor)

  @param[in] Socket      - Socket ID
  @param[in] Channel     - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] Register    - Internal register to be written
  @param[in] Data        - Pointer to data to write

  @retval RETURN_SUCCESS         The write completed successfully.
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdHubWriteByte (
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Register,
  IN UINT8     *Data
  );

#endif // #ifndef _SPD_ACCESS_LIB_H_
