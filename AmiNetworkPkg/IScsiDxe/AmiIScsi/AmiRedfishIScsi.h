//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiRedfishIScsi.h
Header file for the AMI Override functions for the RedFish-IScsi feature

**/

#ifndef _AMI_REDFISH_ISCSI_H_
#define _AMI_REDFISH_ISCSI_H_

#include "IScsiDxe/IScsiImpl.h"
#include "Protocol/RedFishIScsiConfig.h"

#define REDFISH_ISCSI_CONFIG_PROTOCOL_VERSION 2
#define REDFISH_NIC_SIZE                 0x06

// ISCSI CHAP TYPES
#define REDFISH_ISCSI_NO_CHAP            0x00
#define REDFISH_ISCSI_UNI_CHAP           0x01
#define REDFISH_ISCSI_MUTUAL_CHAP        0x02

CHAR8                   gRedfishIScsiInitiatorName[ISCSI_NAME_MAX_SIZE];
BOOLEAN                 gRedfishIScsiDataAvailable;

/**
  Extract nvram data and respective initiator name specific to the current MAC and attempt.
    
  @param[in]  Controller        Controller Handle
  @param[in]  AttemptConfigData ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

**/
EFI_STATUS
RedfishMultipleInitiatorName (
    EFI_HANDLE                  Controller,
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData
);

/**
  Constructs one copy of ISCSI_ATTEMPT_CONFIG_NVDATA from Redfish attempt details.

  @param[in]    IScsiAttemptData                      The iSCSI attempt parameters from Redfish structure.
  @param[OUT]    RedfishIScsiAttemptConfigData        Parsed ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

  @retval EFI_SUCCESS           ISCSI_ATTEMPT_CONFIG_NVDATA has been framed from the input arguments.

**/
EFI_STATUS
EFIAPI
RedfishIScsiFrameAttemptConfigData (
    IN REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *IScsiAttemptData,
    IN ISCSI_ATTEMPT_CONFIG_NVDATA         *RedfishIScsiAttemptConfigData
);

/**
  Convert Hex decimal MAC address to Ascii String
    
  @param[in]  MacAddr     The mac address  Ex: {0x00 , 0x50, 0x56, 0x0C0, 0X00, 0X02}
  @param[in]  Len         Length in bytes of the mac address Ex: 6 bytes.
  @param[in]  MacString8  The Mac String. Ex: "00:50:56:C0:00:02"

**/
VOID
EFIAPI
RedfishIscsiConvertMacAddrToAsciiStr (
    IN CHAR8 *MacAddr,
    IN UINT8 Len, 
    OUT CHAR8 *MacString8
);

/**
  Initialize the iSCSI configuration form.

  @param[in]  DriverBindingHandle The iSCSI driverbinding handle.

  @retval EFI_SUCCESS             The iSCSI configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
RedfishIScsiInit (
    IN EFI_HANDLE  ImageHandle
);

/**
  Overrides the Redfish Attempt with same MAC.

  @param[in]  IScsiAttemptData ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

  @retval EFI_SUCCESS             The Redfish Attempt is overrided.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
IScsiRedfishDeleteAttempts (
    IN  ISCSI_ATTEMPT_CONFIG_NVDATA *IScsiAttemptData
);

/**
  Convert IsId from binary to string format.

  @param[out]      String  The buffer to store the converted string.
  @param[in]       IsId    The buffer to store IsId.

  @retval EFI_SUCCESS              The string converted successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.

**/

extern EFI_GUID   gEfiRedfishIScsiConfigProtocolGuid;
extern EFI_GUID   gEfiRedfishIScsiAttemptConfigGuid;
extern EFI_GUID   gEfiRedfishIscsiDataGuid;
#endif

