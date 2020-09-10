/** @file
  The PEI Library Implements ME Init.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#ifndef _ME_INIT_POST_MEM_H_
#define _ME_INIT_POST_MEM_H_

/**
  Configure MCTP when needed

  @param[in] MbpHob                   Pointer to MbpHob
  @param[in] MePeiConfig              Pointer to ME PEI Config
**/
VOID
MctpConfigure (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob,
  IN ME_PEI_CONFIG       *MePeiConfig
  );

/**
  Disable ME Devices when needed

  @param[in] IsHeci3Supported   Determines if HECI3 is supported and should be enabled

  @retval EFI_STATUS            Operation status
**/
EFI_STATUS
MeDeviceConfigure (
  IN BOOLEAN IsHeci3Supported
  );

/**
  Configure HECI devices on End Of Pei

  @param[in]  PeiServices        Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor   Pointer to the descriptor for the Notification event that caused this function to execute.
  @param[in]  Ppi                Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS        The function completes successfully

**/
EFI_STATUS
EFIAPI
MeOnEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Query if ME operation mode is Temp Disable mode.

  @retval TRUE            ME is in Temp Disable mode.
  @retval FALSE           ME is not in Temp Disable mode.
**/
BOOLEAN
IsMeInTempDisabledMode (
  VOID
  );

/**
  Send Optional Heci Command.
**/
VOID
SendOptionalHeciCommand (
  VOID
  );

/**
  Save all policies in PEI phase to HOB.
**/
VOID
SavePolicyToHob (
  VOID
  );

/**
  Save FWSTS to ME FWSTS HOB, if the HOB is not existing, the HOB will be created and publish.
  If the HOB is existing, the data will be overrided.
**/
VOID
SaveFwStsToHob (
  VOID
  );

/**
  Build Sps Guid Data Hob

  @param[in] SiPreMemPolicyPpi     Pointer to PreMem policy PPI
**/
VOID
BuildSpsGuidDataHob (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Check if MCTP (Management Component Transport) Protocol is supported by CSME and BIOS.

  @param[in] MbpHob                   Pointer to MbpHob
  @param[in] MePeiConfig              Pointer to ME PEI Config

  @retval    TRUE                     MCTP is supported by CSME or BIOS
  @retval    FALSE                    MCTP is not supported by CSME or BIOS
**/
BOOLEAN
IsMctpConfigurationSupported (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob,
  IN ME_PEI_CONFIG       *MePeiConfig
  );

/**
  Check if HECI3 is supported and should be enabled.

  @param[in] MbpHob                   Pointer to MbpHob

  @retval    TRUE                     HECI3 is supported an should be enabled
  @retval    FALSE                    HECI3 is supported an should be disabled
**/
BOOLEAN
IsHeci3Supported (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob
  );

/**
  Create (if not exists) ME_FIRMWARE_VERSION_HOB

  @retval EFI_SUCCESS        HOB has been created/updated successfully
  @retval EFI_DEVICE_ERROR   Can't create HOB
  @retval EFI_UNSUPPORTED    ME doesn't support MkhiVersion
**/
EFI_STATUS
CreateMkhiVersionHob (
  VOID
  );

#endif // _ME_INIT_POST_MEM_H_
