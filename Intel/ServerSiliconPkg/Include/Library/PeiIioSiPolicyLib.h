/** @file
  Prototype of the IioSiPolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef _PEI_IIO_SI_POLICY_LIB_H_
#define _PEI_IIO_SI_POLICY_LIB_H_

#include <ConfigBlock.h>

#define IIO_POLICY_PPI_REVISION  0x1
#define IIO_PCIE_CONFIG_REVISION  0x1

extern EFI_GUID gIioPolicyPpiGuid;
extern EFI_GUID gIioPcieConfigGuid;

/**
  IIO Policy PPI

  Initial version
**/
typedef struct  {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
  /*
    Individual Config Block Structures are added here in memory as part of AddConfigBlock()
  */
} IIO_POLICY_STRUCT;

/**
  The Silicon Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header; ///< Offset 0 - 27 Config Block Header
  //
  // TO_DO: Define IIO PCIE Config Block
  //

  UINT8  RsvdBytes0[4];
} IIO_PCIE_CONFIG;

/**
  CreateDefaultIioSiPolicy generates the default Iio Pei Policy.

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateDefaultIioSiPolicy (
  VOID
  );

/**
  Initialize IIO policies in config block table and installs PPI

  @retval EFI_SUCCESS          If IIO policy PPI installed successfully
  @retval EFI_ERROR            Failed to create and install IIO policy PPI
**/
EFI_STATUS
EFIAPI
InitializeIioPolicyPpi (
  VOID
  );

/**
  IioCreateConfigBlocks creates the config blocks of IIO Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in,out] IioPolicyPpi         The pointer to get IIO Policy PPI instance

  @return EFI_STATUS
**/
EFI_STATUS
EFIAPI
IioCreateConfigBlocks (
  IIO_POLICY_STRUCT **IioPolicyPpi
  );

/**
  This routine installs IioPolicyPpi.

  @param[in] IioPolicyPpi        The pointer to Iio Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IioInstallPolicyPpi (
  IIO_POLICY_STRUCT *IioPolicyPpi
  );
#endif // _PEI_IIO_SI_POLICY_LIB_H_
