/** @file
  Heci driver core for PEI & DXE phases, determines the HECI device and
  initializes it.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/ReportStatusCodeLib.h>
#include "HeciCore.h"
#include "HeciHpet.h"
#include <BupMsgs.h>
#include <MeState.h>
#include <Register/HeciRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciInitLib.h>
#include <Protocol/HeciProtocol.h>
#include <PcieRegs.h>
#include <Library/MeTypeLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/MeUtilsLib.h>


/**
  Checks ME Boot path. The function provides ME BIOS boot path based on current
  HECI1 FW Status Register.  HECI1 must be enabled before invoking the function.
  The FW registers may report multiple statuses to reflect Me Bios boot path,
  BIOS will follow the prioritized Me Bios boot path to continue boot.
  If the priority changes, then BOOT_PATH enumerated type shall be
  adjusted as well to reflect the actual priority.

  @retval ME_BIOS_BOOT_PATH       Me-Bios Path taken based on FWSTS registers
                                    @see ME_BIOS_BOOT_PATH - for available paths
**/
ME_BIOS_BOOT_PATH
CheckMeBootPath (
  VOID
  )
{
  HECI_FWS_REGISTER     Hfsts1;
  HECI_FW_STS4_REGISTER Hfsts4;

  Hfsts1.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS));
  Hfsts4.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS_4));

  DEBUG ((DEBUG_INFO, "HECI1 Hfsts1 = %x, Hfsts4 = %x \n", Hfsts1.ul, Hfsts4.ul));

  if (MeTypeIsSps ()) {
    return (Hfsts1.r.CurrentState > ME_STATE_RECOVERY) ? SpsPath : SpsRcvPath;
  }

  if (!SpiIsFlashDescriptorOverrideEnabled ()) {
    return SecoverJmprPath;
  }

  if (Hfsts4.r.FwInEnfFlow == 0x01) {
    return EnforcementWithoutDidMsgPath;
  }

  if (Hfsts1.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    return DebugModePath;
  } else if (Hfsts1.r.MeOperationMode == ME_OPERATION_MODE_ENHANCED_DEBUG) {
    return EnhancedDebugModePath;
  } else if (Hfsts1.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR) {
    return SecoverJmprPath;
  } else if (Hfsts1.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE) {
    return SwTempDisablePath;
  } else if (Hfsts1.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_HECI_MSG) {
    return SecoverMeiMsgPath;
  }

  if (Hfsts1.r.ErrorCode == ME_ERROR_CODE_IMAGE_FAILURE) {
    return ErrorWithoutDidMsgPath;
  }

  if (Hfsts1.r.CurrentState == ME_STATE_RECOVERY) {
    return RecoveryPath;
  }

  if (Hfsts1.r.FptBad == 0x01 || Hfsts1.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) {
    return ErrorPath;
  }

  return NormalPath;
}

/**
  Returns the mapping of CSME devices that need to be hidden.
  The mapping is based only on ME-Bios boot path taken. Additional devices
  might be hidden later based on other platform policies.

  @retval ME_DEV_EXPOSURE   Mapping of CSME devices to be hidden
                              @see ME_DEV_EXPOSURE for bit assignment
**/
ME_DEV_EXPOSURE
GetBootPathMeDevHidePolicy (
  VOID
  )
{
  ME_BIOS_BOOT_PATH MeBiosPath;
  ME_DEV_EXPOSURE   MeDevHidePolicy;

  MeBiosPath = CheckMeBootPath ();

  DEBUG ((DEBUG_INFO, "Current MeBiosBootPath - %d\n", MeBiosPath));

  switch (MeBiosPath) {
    case NormalPath:
      MeDevHidePolicy = 0;
      break;
    case SecoverMeiMsgPath:
    case SwTempDisablePath:
    case EnhancedDebugModePath:
      MeDevHidePolicy = (HIDE_MEI2 | HIDE_MEI3 | HIDE_AMT_DEVICE);
      break;
    case ErrorPath:
    case RecoveryPath:
      MeDevHidePolicy = (HIDE_MEI2 | HIDE_AMT_DEVICE);
      break;
    case SpsPath:
    case SpsRcvPath:
      MeDevHidePolicy = HIDE_SPS_DEVICE;
      break;
    case ErrorWithoutDidMsgPath:
    case SecoverJmprPath:
    case DebugModePath:
    case EnforcementWithoutDidMsgPath:
    default:
      MeDevHidePolicy = HIDE_ALL_ME_DEVICE;
  }

  return MeDevHidePolicy;
}

/**
  Determines if the HECI device is present.
  If present, initializes it to be used by the BIOS.

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        No HECI device
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         HECI MSG is unsupported
**/
EFI_STATUS
EFIAPI
HeciInitialize (
  IN HECI_DEVICE                      HeciDev
  )
{
  return EFI_SUCCESS;
}

/**
  Heci Re-initialization for Host

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             Heci Re-initialization successful
  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
**/
EFI_STATUS
EFIAPI
HeciReInitialize (
  IN HECI_DEVICE                  HeciDev
  )
{
  return EFI_SUCCESS;
}

/**
  Reads a message from CSME through HECI.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Blocking             Used to determine if the read is BLOCKING or NON_BLOCKING.
  @param[in, out] MessageBody     Pointer to a buffer used to receive a message.
  @param[in, out] Length          Pointer to the length of the buffer on input and the length
                                  of the message on return. (in bytes)

  @retval EFI_SUCCESS             One message packet read.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             Failed to receive a full message on time
  @retval EFI_NO_RESPONSE         No response from CSME
  @retval EFI_BUFFER_TOO_SMALL    The caller's buffer was not large enough
**/
EFI_STATUS
EFIAPI
HeciReceive (
  IN      HECI_DEVICE HeciDev,
  IN      UINT32      Blocking,
  IN OUT  UINT32      *MessageBody,
  IN OUT  UINT32      *Length
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Function sends one message (of any length) through the HECI circular buffer.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Message              Pointer to the message data to be sent.
  @param[in] Length               Length of the message in bytes.
  @param[in] HostAddress          The address of the host processor.
  @param[in] MeAddress            Address of the ME subsystem the message is being sent to.

  @retval EFI_SUCCESS             One message packet sent.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             CSME failed to empty the circular buffer
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSend (
  IN HECI_DEVICE               HeciDev,
  IN UINT32                    *Message,
  IN UINT32                    Length,
  IN UINT8                     HostAddress,
  IN UINT8                     MeAddress
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Function sends one message through the HECI circular buffer and waits
  for the corresponding ACK message with retry mechanism.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in][out] Message         Pointer to the message buffer.
  @param[in] Length               Length of the message in bytes.
  @param[in][out] RecLength       Length of the message response in bytes.
  @param[in] HostAddress          Address of the sending entity.
  @param[in] MeAddress            Address of the ME entity that should receive the message.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             CSME failed to empty or fill the circular buffer
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSendwAck (
  IN      HECI_DEVICE HeciDev,
  IN OUT  UINT32      *Message,
  IN      UINT32      Length,
  IN OUT  UINT32      *RecLength,
  IN      UINT8       HostAddress,
  IN      UINT8       MeAddress
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Me reset and waiting for ready

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Delay                The biggest waiting time

  @retval EFI_SUCCESS             Host Ready bit cleared
  @retval EFI_TIMEOUT             Host Ready bit not cleared
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
**/
EFI_STATUS
EFIAPI
MeResetWait (
  IN  HECI_DEVICE              HeciDev,
  IN  UINT32                   Delay
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Function forces a reinit of the heci interface by following the reset heci interface via host algorithm
  in HPS section 4.1.1.1

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             Interface reset successful
  @retval EFI_TIMEOUT             ME is not ready
**/
EFI_STATUS
EFIAPI
ResetHeciInterface (
  IN  HECI_DEVICE              HeciDev
  )
{
  if (HeciDev != HECI1_DEVICE) {
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  return ResetHeci1Interface ();
}

/**
  Get an abstract Intel HECI1 State from Firmware Status Register.
  This is used to control BIOS flow for different Intel ME functions.

  @param[out] MeStatus            Pointer for status report
                                    @see MeState.h - Abstract ME status definitions.

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeStatus is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
HeciGetMeStatus (
  OUT UINT32        *MeStatus
  )
{
 return EFI_DEVICE_ERROR;
}

/**
  Get HECI1 Mode

  @param[out] MeMode              Pointer for HECI1 Mode report
                                    @see MeState.h - Abstract ME Mode definitions.

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeMode is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
HeciGetMeMode (
  OUT UINT32        *MeMode
  )
{
  return EFI_DEVICE_ERROR;
}
