/** @file
  FPGA Configuration library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2016 Intel Corporation. <BR>

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

#include <Library/FpgaConfigurationLib.h>

//
// Functions implemented in the Library
//


/**
  Set values in FPGA Configuration

  @param[ in ] FpgaConfigurationPointer      Pointer to the FPGA Configuration with new values

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The FPGA is not supported / Global enable is FALSE
  @retval EFI_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaConfigurationSetValues (
  IN FPGA_CONFIGURATION *FpgaConfigurationPointer
  )
{
  return EFI_SUCCESS;
}




/**
  Get FPGA Configuration into

  @param[ out] FpgaConfigurationPointer     Pointer to save the FPGA Configuration data to

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The FPGA is not supported / Global enable is FALSE
  @retval EFI_INVALID_PARAMETER   The Global Enable Value changed, it's read only for this function.
  @retval EFI_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaConfigurationGetValues (
  IN FPGA_CONFIGURATION *FpgaConfigurationPointer
  )
{
  return EFI_UNSUPPORTED;
}




/**
  Get FPGA Global Enable, convenience function to get value in Configuration

  @param[ out] FpgaGlobalEnable   Pointer to FPGA Global enable value to return

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND     The function could not locate the FPGA data
**/

EFI_STATUS
EFIAPI
FpgaGlobalEnableGet (
  IN  FPGA_FLOW_STAGE   FpgaFlowStage,
  OUT BOOLEAN *FpgaGlobalEnable
  )
{
  return EFI_SUCCESS;
}



