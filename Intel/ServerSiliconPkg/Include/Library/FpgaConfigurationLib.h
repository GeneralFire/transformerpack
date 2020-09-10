/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _FPGA_CONFIGURATION_LIB_H_
#define _FPGA_CONFIGURATION_LIB_H_

#include <Uefi.h>
#include <Library/FpgaFvDataLib.h>

typedef enum {
  FPGA_LOAD_SUCCESS = 0,
  FPGA_LOAD_STATUS_GUID_NOT_FOUND_ERROR,
  FPGA_LOAD_STATUS_DEVICE_NO_RESPONCE_ERROR,
  FPGA_LOAD_STATUS_DEVICE_RETURNED_ERROR,
  FPGA_LOAD_STATUS_DEVICE_NOT_CONFIGURED,
  FPGA_LOAD_STATUS_DEVICE_BITSTREAM_NOT_FOUND,
  FPGA_LOAD_STATUS_DEVICE_PARM_REGISTER_FAILED,
  FPGA_LOAD_STATUS_DEVICE_LOADER_NOT_FOUND,
  FPGA_LOAD_STATUS_DEVICE_BINARY_SIZE_MISMATCH,
  FPGA_LOAD_STATUS_DEVICE_PRESENT_NOT_ACTIVE,
  FPGA_LOAD_STATUS_GENERAL,

  // Always the last one
  FPGA_LOAD_STATUS_MAX
} FPGA_BITSTREAM_LOAD_STATUS;

typedef enum {
  //
  // KTI stage is TRUE if FpgaPcdValue         is enabled and
  //                  if FpgaPlatformEnabled  is enabled
  //           it effects FpgaSktPresent
  //
  FPGA_FLOW_STAGE_KTI = 0,
  //
  // BS Load stage is TRUE if FpgaPcdValue         is enabled and
  //                      if FpgaPlatformEnabled  is enabled and
  //                       if FpgaSktPresent       is != 0
  //           it effects FpgaBitStreamStatus, FpgaSktActive, FpgaSktError, and FpgaSktProgrammed
  //
  FPGA_FLOW_STAGE_BS_LOAD,
  //
  // Setup stage is TRUE if FpgaPcdValue         is enabled
  //           it effects FpgaPlatformEnabled, FpgaFvFound,
  //
  FPGA_FLOW_STAGE_SETUP,

  // Always the last one
  FPGA_FLOW_STAGE_MAX=0xff

} FPGA_FLOW_STAGE;


#pragma pack(1)

typedef struct {

  //
  // These are the the Global enables, .
  // For instance setup, all it cares about is the PCD value, as it may be be the first to look for the FV.
  //
  BOOLEAN      FpgaPcdValue;           //ReadOnly
  BOOLEAN      FpgaFvFound;            // system has found an FPGA FV in the system

  //
  // These are all socket bit position flags, with Bit 0 being Socket 0
  //

  // These are runtime status entries
  UINT8          FpgaSktPresent;         // FPGA Is present in the socket, set in UPI code
  UINT8          FpgaSktActive;          // FPGA Is active in the socket, set in UPI code
  UINT8          FpgaSktError;           // FPGA has error in the socket, set in Loader code
  UINT8          FpgaSktProgrammed;      // FPGA has been successful programmed with BitStream, set in Loader code

  // Bit values from Setup selection to select active sockets;

  UINT8          FpgaPlatformEnabled;    // FPGA in socket is requested enabled by Platform PPI, possibly VIA setup values.


  //
  // Array of values with FPGA Socket as index
  //

  //
  // FPGA BitStram GUID index from Platform setup
  // invalid value will be "FPGA_NULL_GUID_INDEX"
  // FV Loader will use index to find GUID BS data from FV
  //
  UINT8          FpgaBitStreamGuid[FPGA_MAX_SOCKET];

  //
  //Status of BitStream load for error reporting if needed.
  //
  UINT8          FpgaBitStreamStatus[FPGA_MAX_SOCKET]; // FPGA BitStream load Status value

  //
  // Loader error return, only valid if the FpgaBitStreamStatus == FPGA_LOAD_STATUS_DEVICE_RETURNED_ERROR
  //
  UINT8          FpgaSocketErrorReturn[FPGA_MAX_SOCKET];

  // FPGA Temperature Threshold 1/2: Max value clamped at 100 C;
  // i.e. if the SW tries to write value greater than 100 C, HW will automatically default to 100 C.
  UINT8    FpgaSktThermalTH1[FPGA_MAX_SOCKET];
  UINT8    FpgaSktThermalTH2[FPGA_MAX_SOCKET];

} FPGA_CONFIGURATION;

#pragma pack()



//
// Functions implemented in the Library
//
/**
  Set values in FPGA Configuration

  @param[ in ] FpgaConfigurationPointer      Pointer to the FPGA Configuration with new values

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The FPGA is not supported / Global enable is FALSE
  @retval EFI_ERROR_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaConfigurationSetValues (
  IN FPGA_CONFIGURATION *FpgaConfigurationPointer
  );



/**
  Get FPGA Configuration, return pointer to CB

  @param[ out] FpgaConfigurationPointer     Pointer to save the FPGA Configuration pointer to

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The FPGA is not supported / Global enable is FALSE
  @retval EFI_INVALID_PARAMETER   The Global Enable Value changed, it's read only for this function.
  @retval EFI_ERROR_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaConfigurationGetValues (
  OUT FPGA_CONFIGURATION *FpgaConfigurationPointer
  );


/**
  Get FPGA Global Enable based on Flow stage

  @param[ in ] FpgaFlowStage      FPGA_FLOW_STAGE to determine if global is active
  @param[ out] FpgaGlobalEnable   Pointer to FPGA Global enable value to return

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ERROR_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaGlobalEnableGet (
  IN  FPGA_FLOW_STAGE   FpgaFlowStage,
  OUT BOOLEAN           *FpgaGlobalEnable
  );




#endif
