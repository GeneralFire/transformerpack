/** @file
  Header file for NV data structure definition.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

**/

#ifndef __TCG2_CONFIG_NV_DATA_H__
#define __TCG2_CONFIG_NV_DATA_H__

#include <Guid/HiiPlatformSetupFormset.h>
// APTIOV_SERVER_OVERRIDE_RC_START
//#include <Guid/Tcg2ConfigHii.h>
// APTIOV_SERVER_OVERRIDE_RC_END
#include <IndustryStandard/TcgPhysicalPresence.h>

#define TPM_DEVICE_NULL           0
#define TPM_DEVICE_1_2            1
#define TPM_DEVICE_2_0_DTPM       2
#define TPM_DEVICE_MIN            TPM_DEVICE_1_2
#define TPM_DEVICE_MAX            TPM_DEVICE_2_0_DTPM
#define TPM_DEVICE_DEFAULT        TPM_DEVICE_1_2

#define TPM2_ACPI_REVISION_3       3
#define TPM2_ACPI_REVISION_4       4

#define TPM_DEVICE_INTERFACE_TIS       0
#define TPM_DEVICE_INTERFACE_PTP_FIFO  1
#define TPM_DEVICE_INTERFACE_PTP_CRB   2
#define TPM_DEVICE_INTERFACE_MAX       TPM_DEVICE_INTERFACE_PTP_FIFO
#define TPM_DEVICE_INTERFACE_DEFAULT   TPM_DEVICE_INTERFACE_PTP_CRB

#define TCG2_PPI_VERSION_1_2                    0x322E31  // "1.2"
#define TCG2_PPI_VERSION_1_3                    0x332E31  // "1.3"

//
// Nv Data structure referenced by IFR, TPM device user desired
//
typedef struct {
  UINT8   TpmDevice;
} TCG2_CONFIGURATION;

typedef struct {
  UINT64  PpiVersion;
  UINT8   Tpm2AcpiTableRev;
} TCG2_VERSION;

typedef struct {
  BOOLEAN  Sha1Supported;
  BOOLEAN  Sha256Supported;
  BOOLEAN  Sha384Supported;
  BOOLEAN  Sha512Supported;
  BOOLEAN  Sm3Supported;
  UINT8    TpmDeviceInterfaceAttempt;
  BOOLEAN  TpmDeviceInterfacePtpFifoSupported;
  BOOLEAN  TpmDeviceInterfacePtpCrbSupported;
} TCG2_CONFIGURATION_INFO;

//
// Variable saved for S3, TPM detected, only valid in S3 path.
// This variable is ReadOnly.
//
typedef struct {
  UINT8   TpmDeviceDetected;
} TCG2_DEVICE_DETECTION;

#define TCG2_STORAGE_NAME           L"TCG2_CONFIGURATION"
#define TCG2_STORAGE_INFO_NAME      L"TCG2_CONFIGURATION_INFO"
#define TCG2_DEVICE_DETECTION_NAME  L"TCG2_DEVICE_DETECTION"
#define TCG2_VERSION_NAME           L"TCG2_VERSION"

#endif
