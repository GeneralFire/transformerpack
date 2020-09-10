/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _CLIENT_CORE_INTERNAL_H_
#define _CLIENT_CORE_INTERNAL_H_

/**
  Send ME the BIOS end of Post message.

  @param[out] RequestedActions    Action request returned by EOP ACK
                                    0x00 (HECI_EOP_STATUS_SUCCESS) - Continue to boot
                                    0x01 (HECI_EOP_PERFORM_GLOBAL_RESET) - Global reset

  @retval EFI_SUCCESS             Platform reached End of Post successfully
  @retval EFI_DEVICE_ERROR        An error has occured by EoP message
**/
EFI_STATUS
MeEndOfPostEvent (
  OUT UINT32  *RequestedActions
  );

/**
  Perform Cf9Gr Lock Config
      - PCH BIOS Spec Rev 0.9 Section 18.4  Additional Power Management Programming
        Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
        for production machine according to "RS - PCH Intel Management Engine
        (Intel(r) ME) BIOS Writer's Guide".
**/
VOID
LockConfig (
  VOID
  );

/*
  Installs HECI_FLOW_PROTOCOL and registers a callback to uninstall it on exit boot services

  @retval     EFI_SUCCESS if protocol was installed, error otherwise
*/
EFI_STATUS
EFIAPI
InitializeHeciFlow (
  VOID
  );

/**
  Create the FWSTS info to the SMBIOS table using the SMBIOS protocol
  Invoke this routine to add the table entry when all the FWSTS data is finalized.

  @param[in]  Event                 A pointer to the Event that triggered the callback.
  @param[in]  Context               A pointer to private data registered with the callback
                                    If the Context is TRUE, invoke DisableAllMeDevices () before exit
                                    because it is set TRUE when registered function failed to install HeciProtocol.

  @retval EFI_SUCCESS               if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES      if not able to get resources.
  @retval EFI_UNSUPPORTED           if required DataHub or SMBIOS protocol is not available.
**/
VOID
EFIAPI
AddFwStsSmbiosEntry (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  );

/**
  Register ME reset handler.
**/
VOID
RegisterMeReset (
  VOID
  );

#endif // _CLIENT_CORE_INTERNAL_H_
