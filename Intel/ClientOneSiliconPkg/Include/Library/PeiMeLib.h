/** @file
  Header file for PEI Me functionality

@copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation.

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
#ifndef _PEI_ME_LIB_H_
#define _PEI_ME_LIB_H_

#include <BupMsgs.h>
#include <MkhiMsgs.h>
#include <Register/MeRegs.h>
#include <MeBiosPayloadData.h>
#include <IccMsgs.h>

#define ME_STATE_STALL_1_SECOND 1000000
#define MSG_MAX_WAIT_TIMEOUT    5

//
// Prototype for HECI message PEI phase
//

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin         Reset source
  @param[in] ResetType           Global or Host reset

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_NOT_FOUND          No ME present
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  );

//
// MKHI_FWCAPS_GROUP_ID = 0x03
//

/**
  Send Get Firmware SKU Request to ME

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU               *RuleData
  );

/**
  This message is sent by the BIOS or IntelR MEBX. One of usages is to utilize
  this command to determine if the platform runs in Consumer or Corporate SKU
  size firmware.

  @param[out] RuleData              Including PlatformBrand, IntelMeFwImageType,
                                    SuperSku, and PlatformTargetUsageType

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  );

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS/IntelR MEBX to get firmware runtime status.
  The GET FW RUNTIME STATUS message doesn't need to check the HFS.FWInitComplete
  value before sending the command.
  It means this message can be sent regardless of HFS.FWInitComplete.

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  );

/**
  This message is sent by the BIOS in PEI phase to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is enabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigOnRtcClearDisableMsg (
  OUT UINT32                *RuleData
  );

/**
  This message is sent by the BIOS on the boot where the host wants to get the firmware provisioning state.
  The firmware will respond to AMT BIOS SYNC INFO message even after the End of Post.

  @param[out] RuleData            @see AMT_BIOS_SYNC_INFO

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  );

/**
  Send Set Rule Data Request to CSME

  @param[in]   RuleId             Identifies the rule which data needs to be changed.
  @param[in]   RuleDataLength     Rule Data Length.
  @param[in]   RuleData           Pointer to new rule data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  );

/**
  This message is sent by the BIOS in PEI phase to set
  ME Unconfig on RTC Clear Disable state.

  @param[in] RuleData             1 - Disable Unconfig on RTC clear
                                  0 - Enable Unconfig on RTC clear

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  );

//
// MKHI_MCA_GROUP_ID = 0x0A
//

/**
  Sends the NVM request device ownership message.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciRequestDeviceOwnership (
  VOID
  );

//
// MKHI_DNX_GROUP_ID = 0x0D
//

/**
  Send DNX Request Set message.

  @param[out] BiosAction          Requested Bios Action:
                                    0 - Continue Post
                                    1 - Reset Required

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciDnxReqSet (
  OUT UINT32         *BiosAction
  );

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get ME Unconfiguration Status.

  @param[out] UnconfigStatus      Unconfiguration Status:
                                    0x01 - Unconfiguration in progress
                                    0x02 - No unconfigure w/o password to be processed
                                    0x03 - CSME finished processing unconfigure w/o password
                                    0x80 - Error encountered by CSME during unconfigure w/o password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigureStatus (
  OUT UINT8* UnconfigStatus
  );

//
// BUP_COMMON_GROUP_ID = 0xF0
//

/**
  Send DRAM init done message through HECI to inform ME of memory initialization done.

  @param[in]  ImrBaseLow                Base address for IMR region (Low DWORD)
  @param[in]  ImrBaseHigh               Base address for IMR region (High DWORD)
  @param[in]  MemStatus                 Memory init status
  @param[out] MkhiResult                MKHI Error Code
  @param[out] Flags                     Flags
  @param[out] BiosAction                ME response to DID
  @param[out] PciImrBaseLow            Base address for PCI IMR (Low DWORD)
  @param[out] PciImrBaseHigh           Base address for PCI IMR (High DWORD)
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciSendDid (
  IN  UINT32  ImrBaseLow,
  IN  UINT32  ImrBaseHigh,
  IN  UINT8   MemStatus,
  OUT UINT8   *MkhiResult,
  OUT UINT8   *Flags,
  OUT UINT8   *BiosAction,
  OUT UINT32  *PciImrBaseLow,
  OUT UINT32  *PciImrBaseHigh
  );

/**
  Send Get MBP from FW

  @param[out]     MbpEntriesNumber  Number of MBP entries
  @param[in, out] MbpItems          MBP items of the response
  @paran[in]      SkipMbp           Skip MBP

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetMbpMsg (
  OUT UINT32                   *MbpEntriesNumber,
  IN OUT UINT32                *MbpItems,
  IN BOOLEAN                   SkibMbp
  );

/**
  This message is sent by the BIOS to retrieve from CSME total size of IMRs.
  BIOS needs to provide mask of disabled IMRs and requested size for PCIe IMR.

  @param[in]  BiosImrDisableMask0  Low DWORD of BIOS IMR Disable mask
  @param[in]  BiosImrDisableMask1  High DWORD of BIOS IMR Disable mask
  @param[in]  PciImrSize           Requested IMR size for PCI
  @param[out] MkhiResult           MKHI Error Code
  @param[out] ImrsSize             Total IMR size
  @param[out] Alignment            Required address alignment

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciGetImrSizeMsg (
  IN  UINT32     BiosImrDisableMask0,
  IN  UINT32     BiosImrDisableMask1,
  IN  UINT32     PciImrSize,
  OUT UINT8      *MkhiResult,
  OUT UINT32     *ImrsSize,
  OUT UINT32     *Alignment
  );

/**
  Send Manufacturing Reset and Halt

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciBupManufacturingResetAndHalt (
  VOID
  );

/**
  Send Set eDebug Mode State.

  @param[in]  State                 EDebug Mode state to be set:
                                      TRUE  - Enable  eDebug State
                                      FALSE - Disable eDebug State

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciSetEDebugModeState (
  IN BOOLEAN     State
  );

/**
  This command is sent by the BIOS to switch clear CSME data to image creation state.
  After the command success, platform reset is expected.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Get ME mode fail.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
PeiHeciDataClear (
  VOID
  );

/**
  This message is sent by the BIOS to tell CSME that Buffer has been allocated for payload to be copied.

  @param[in]      LoadRequest            IP information for which firmware has to be loaded
  @param[in]      BufferAddressLow       Lower DRAM address of the buffer
  @param[in]      BufferAddressHigh      Upper DRAM address of the buffer
  @param[in, out] BufferSize             On input, BufferSize is size of Buffer allocated by Bios.On output, actual PayloadSize copied by ME
  @param[out]     IpLoaderStatus         IP Loading status from CSME
  @param[out]     HashType               Hash algorithm used for message digest
  @param[out]     Hash                   Hash Value

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support sending this message through this HECI or Input parameters are not valid.
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER          Parameter invalid
**/
EFI_STATUS
PeiHeciGetFwPayload (
  IN     UINT32        LoadRequest,
  IN     UINT32        BufferAddressLow,
  IN     UINT32        BufferAddressHigh,
  IN OUT UINT32        *BufferSize,
  OUT    UINT32        *IpLoaderStatus,
  OUT    UINT8         *HashType,
  OUT    UINT8         *Hash
  );

/**
  This message is sent by the BIOS to retrieve debut token data from CSME.

  @param[out] KnobData              Debug Token Knob Data

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetDebugTokenData (
  OUT DEBUG_TOKEN_KNOB_DATA    *KnobData
  );

//
// BUP_ICC_GROUP_ID = 0xF1
//

/**
  Send ICC request through HECI to query if CSME FW requires the warm reset flow from a previous boot.

  @param[out] WarmResetRequired   1 - CSME requires a warm reset to complete BCLK ramp en flow

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciIccBclkMsg (
  OUT UINT8    *WarmResetRequired
  );

//
// BUP_MPHY_GROUP_ID = 0xF2
//

/**
  Get the Mphy data from CSME

  @param[out] Data                Pointer to the data buffer.
  @param[in]  DataSize            Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciReadFromMphy (
  OUT UINT8  *Data,
  IN  UINT32 DataSize
  );

//
// BUP_PM_GROUP_ID = 0xF3
//

/**
  Send Host Reset Notification Message to determine if warm reset is required.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSendHostResetNotificationMsg (
  VOID
  );

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Can't localize HECI protocol
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMkhiVersion (
  OUT MKHI_VERSION        *MkhiVersion
  );

//
// Interface functions of HeciMsgLib
//

//
// ME Client - Integrated Clock Chip (ICC)
//

/**
  Reads the ChipsetInit table from the CSME FW.

  @param[in, out] ChipsetInitTable     Pointer to buffer for the required system ChipsetInit Table.
  @param[in, out] ChipsetInitTableLen  Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciReadChipsetInitMsg (
  IN OUT UINT8                   *ChipsetInitTable,
  IN OUT UINT32                  *ChipsetInitTableLen
  );

/**
  Send the required system ChipsetInit Table to CSME FW.

  @param[in] ChipsetInitTable     The ChipsetInitSyncMsg HDR and the required system ChipsetInit Table.
  @param[in] ChipsetInitTableLen  Length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
**/
EFI_STATUS
PeiHeciWriteChipsetInitMsg (
  IN  UINT8                 *ChipsetInitTable,
  IN  UINT32                ChipsetInitTableLen
  );

//
// Prototype for ME Policy PEI phase
//

/**
  Read the ME policy to see if HECI timeouts are enabled.

  @retval TRUE             Timeout is enabled
  @retval FALSE            Timeout is disabled
**/
BOOLEAN
MeHeciTimeoutsEnabled (
  VOID
  );

/**
  Read the ME policy to see if Heci Full Trace are enabled.

  @retval TRUE             HECI full trace in the log is enabled
  @retval FALSE            HECI full trace in the log is disabled
**/
BOOLEAN
MeHeciFullTrace (
  VOID
  );

/**
  Check if HECI2 should be removed from PCI space or not

  @retval FALSE            HECI2 should be Disabled and removed from PCI space
  @retval TRUE             HECI2 should be enabled and not removed from PCI space
**/
BOOLEAN
MeHeci2Enabled (
  VOID
  );

/**
  Read the ME policy to see if HECI message check is enabled for Bios Boot Path.

  @retval TRUE             HECI message check is enabled
  @retval FALSE            HECI message check is disabled
**/
BOOLEAN
MeHeciMessageCheckEnabled (
  VOID
  );

/**
  Read the ME policy to see if MBP HOB is skipped.

  @retval TRUE             MBP HOB is skipped
  @retval FALSE            MBP HOB is not skipped
**/
BOOLEAN
MeMbpHobSkipped (
  VOID
  );

/**
  Get default Heci1 BAR address from policy.

  @param[in] MeDev         The ME device to be accessed.

  @retval                  32-bit BAR address
**/
UINT32
MeGetHeciBarAddress (
  IN ME_DEVICE             MeDev
  );

/**
  Read the ME policy to see if D0I3 for HECI devices are enabled.

  @retval TRUE             D0I3 for HECI devices are enabled
  @retval FALSE            D0I3 for HECI devices are disabled
**/
BOOLEAN
MeHeciD0I3Enabled (
  VOID
  );

/**
  Read the ME policy to see if Enforcing Enhanced Debug Mode is enabled.

  @retval TRUE             ME should enter Enhanced Debug Mode
  @retval FALSE            ME should not enter Enhanced Debug Mode
**/
BOOLEAN
MeIsEnforceEDebugModeEnabled (
  VOID
  );

/**
  Check if End of Post Message is enabled in setup options.

  @retval FALSE            EndOfPost is disabled.
  @retval TRUE             EndOfPost is enabled.
**/
BOOLEAN
MeEndOfPostEnabled (
  VOID
  );

/**
  Checks if EndOfPost event already happened

  @retval TRUE             End of post already happened
  @retval FALSE            End of post did not happen yet
**/
BOOLEAN
MeIsAfterEndOfPost (
  VOID
  );

/**
  Set EndOfPost done
**/
VOID
MeSetEndOfPostDone (
  VOID
  );

/**
  Install ME call back function for reset2 ppi.

**/
VOID
RegisterMeReset (
  VOID
  );

/**
  Install Heci PPIs needed in PEI phase

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        ME FPT is bad
**/
EFI_STATUS
InstallHeciPpi (
  VOID
  );


/**
  Retrieves information on whether Pcie root port configuration SoftStrap override is being done or not

  @param[in]      NumberOfControllers    On input, it is the number of controllers caller expects.
  @param[in, out] ControllerConfig       Pointer to the controller config message.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate required resources
**/
EFI_STATUS
PeiHeciGetSoftStrpConfigMsg (
  IN      UINT16     NumberOfControllers,
  IN OUT  UINT8      *ControllerConfig
  );

/**
  Set Pcie Root port configuration SoftStrap override for the specified Pcie Root Port controller

  @param[in] NumberOfControllers         On input, it is the number of controllers caller expects.
  @param[in] OverrideMask                Mask with RpController where Hybrid Storage is present and softstraps need to be overridden

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES           Unable to allocate required resources
**/
EFI_STATUS
PeiHeciOverrideSoftStrapMsg (
  IN UINT16     NumberOfControllers,
  IN UINT8      OverrideMask
  );

#endif
