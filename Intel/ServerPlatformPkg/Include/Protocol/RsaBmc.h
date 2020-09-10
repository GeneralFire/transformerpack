/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _RSA_BMC_COMMANDS_PROTOCOL_H_
#define _RSA_BMC_COMMANDS_PROTOCOL_H_

//
// "KCS_MAX_DATASIZE" defines the maximum # of data bytes that are sent in a IPMI messgae via KCS.
// PORT this value to IPMI message size supported in BMC implementation.
//
#define KCS_MAX_DATASIZE      0x80

#define MDR_MAX_DATA          0x5C
#define MDR_MAX_ACPITABLE     64
#define MDR_UNLOCKED          0x00
#define MDR_STRICT_LOCK       0x01
#define MDR_PREEMPT_LOCK      0x02

#define MDR_SMBIOS_REGION     0x01
#define MDR_ACPI_REGION       0x02

#define ACPI_SIG_LENGTH         4

#pragma pack(1)

typedef struct _EFI_RSA_BMC_COMMANDS_PROTOCOL EFI_RSA_BMC_COMMANDS_PROTOCOL;

/*----------------------------------------------------------------------------------------
    Definitions for BMC Data Region commands
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_RSA_BMC_MDR_REGION_STATUS           0x20
#define EFI_INTEL_RSA_BMC_MDR_REGION_UPDATE_COMPLETE  0x21
#define EFI_INTEL_RSA_BMC_MDR_REGION_READ             0x22
#define EFI_INTEL_RSA_BMC_MDR_REGION_WRITE            0x23
#define EFI_INTEL_RSA_BMC_MDR_REGION_LOCK             0x24

//------------------------------------------------------------------------------------------------
//  Constants and Structure definitions for "RSA BMC Data Region Lock" command to follow here
//------------------------------------------------------------------------------------------------
// Code 25h
typedef struct _RSA_BMC_GET_MDR_REGION_STATUS_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 DataRegion;             // 0x01 = SMBIOS data region
                                // 0x02 = ACPI tables region
} RSA_BMC_GET_MDR_REGION_STATUS_REQUEST;

typedef struct _RSA_BMC_GET_MDR_REGION_STATUS_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 MdrVersion;
  UINT8 DataRegion;             // 0x01 = SMBIOS data region
                                // 0x02 = ACPI tables region
  UINT8 DataValidation;         // 0x00 = Invalid dataSMBIOS data region
                                // 0x01 = Valid data
  UINT8 UpdateCount;
  UINT8 LockStatus;             // 0x00 = Unlocked
                                // 0x01 = Strict lock
                                // 0x02 = Preemptable lock
  UINT16 MaxSizeOfRegionInBytes;
  UINT16 UsedSizefRegionInBytes;
  UINT8 RegionCheckSum;
} RSA_BMC_GET_MDR_REGION_STATUS_RESPONSE;


typedef struct _RSA_BMC_MDR_REGION_LOCK_REQUEST {
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
  UINT8 SessionLockHandle;  // Handle returned from lock operation
  UINT8 DataRegion;         // 0x01 = SMBIOS data region
                            // 0x02 = ACPI tables region
  UINT8 LockType;           // 0x00 Abort, unlock without completing operation.
                            // 0x01 Strict Lock, only writes by the locker are allowed.
                            // 0x02 Preemptable lock, a strict lock attempt will "break" this lock.
  UINT16 Timeout;           // Number of milliseconds allowed between write accesses before the lock is terminated.
} RSA_BMC_MDR_REGION_LOCK_REQUEST;

typedef struct _RSA_BMC_MDR_REGION_LOCK_RESPONSE {
  UINT8 CompletionCode;     // 0x00 = Normal
                            // 0x81 = Region is in use by another user
                            // 0xC9 = Invalid Region Specified
                            // 0xD5 = Region is not locked
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
  UINT8 SessionLockHandle;  // Only Valid if byte 1 is a successful code
} RSA_BMC_MDR_REGION_LOCK_RESPONSE;


//---------------------------------------------------------------------------------------------------
//  Constants and Structure definitions for "RSA BMC Data Update Complete" command to follow here
//----------------------------------------------------------------------------------------------------
typedef struct _RSA_BMC_MDR_REGION_UPDATE_COMPLETE_REQUEST {
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
  UINT8 SessionLockHandle;  // Handle returned from lock operation
  UINT8 DataRegion;         // 0x01 = SMBIOS data region
                            // 0x02 = ACPI tables region
} RSA_BMC_MDR_REGION_UPDATE_COMPLETE_REQUEST;

typedef struct _RSA_BMC_MDR_REGION_UPDATE_COMPLETE_RESPONSE {
  UINT8 CompletionCode;     // 0x00 = Normal
                            // 0x81 = Region is in use by another user
                            // 0xC9 = Invalid Region Specified
                            // 0xD5 = Region is not locked
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
} RSA_BMC_MDR_REGION_UPDATE_COMPLETE_RESPONSE;



/*----------------------------------------------------------------------------------------
    Definitions for RSA BMC Data Region Read command
----------------------------------------------------------------------------------------*/

//
//  Constants and Structure definitions for "RSA BMC Data Region Read" command to follow here
//
typedef struct _RSA_BMC_MDR_REGION_READ_REQUEST {
  UINT8 IntelRsdSpec;   // IntelRsdSpec = 0x04
  UINT8 DataRegion;     // 0x01 = SMBIOS data region
                        // 0x02 = ACPI tables region
  UINT8 DataLength;     // 0x00 - 0xFF: Length of data write requested.
                        // Note: Subject to transport size limitations.
  UINT16 DataOffset;    // 0 (Region size - 1): Byte offset of target data.
} RSA_BMC_MDR_REGION_READ_REQUEST;

typedef struct _RSA_BMC_MDR_REGION_READ_RESPONSE {
  UINT8 CompletionCode;     // 0x00 = Normal
                            // 0x81 = Region is in use by another user
                            // 0xC4 = Request is larger than maximum allowed
                            // 0xC7 = Request data extends beyond length of region
                            // 0xC9 = Invalid Region Specified
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
  UINT8 ReadLength;
  UINT8 UpdateCount;
  UINT8 DataArray[MDR_MAX_DATA];   //Data to be written
} RSA_BMC_MDR_REGION_READ_RESPONSE;


/*----------------------------------------------------------------------------------------
    Definitions for RSA BMC Data Region Write command
----------------------------------------------------------------------------------------*/
//
//  Constants and Structure definitions for "BMC Data Region Write" command to follow here
//
typedef struct _RSA_BMC_MDR_REGION_WRITE_REQUEST {
  UINT8 IntelRsdSpec;       // IntelRsdSpec = 0x04
  UINT8 SessionLockHandle;  // Handle returned from lock operation
  UINT8 DataRegion;         // 0x01 = SMBIOS data region
                            // 0x02 = ACPI tables region
  UINT8 DataLength;         // 0x00 - 0xFF: Length of data write requested.
                            // Note: Subject to transport size limitations.
  UINT16 DataOffset;        // 0 (Region size - 1): Byte offset of target data.
  UINT8 DataArray[MDR_MAX_DATA];  // Data to be written.
} RSA_BMC_MDR_REGION_WRITE_REQUEST;

typedef struct _RSA_BMC_MDR_REGION_WRITE_RESPONSE {
   UINT8 CompletionCode;         // 0x00 = Normal
                                 // 0x81 = Region is in use by another user
                                 // 0xC4 = Request is larger than maximum allowed
                                 // 0xC7 = Request data extends beyond length of region
                                 // 0xC9 = Invalid Region Specified
                                 // 0xD5 = Data Region Not Locked
  UINT8 IntelRsdSpec;            // IntelRsdSpec = 0x04
  UINT8 DataRegion;              // 0x01 = SMBIOS data region
                                 // 0x02 = ACPI tables region
  UINT8 DataValidation;          // 0x00 = Invalid
                                 // 0x01 = Valid
  UINT8 LockStatus;              // 0x00 = Unlocked
                                 // 0x01 = Strict Lock
                                 // 0x02 = Preemptible Lock
  UINT16 MaxRegionLength;
  UINT16 SizeRegionUsed;
} RSA_BMC_MDR_REGION_WRITE_RESPONSE;


/*----------------------------------------------------------------------------------------
    Definitions for TPM Configuration commands
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_RSA_BMC_GET_SYSTEM_MODE  0x25
#define EFI_INTEL_RSA_BMC_SET_SYSTEM_MODE  0x26

// Code 25h
typedef struct _RSA_BMC_GET_SYSTEM_MODE_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
} RSA_BMC_GET_SYSTEM_MODE_REQUEST;

typedef struct _RSA_BMC_GET_SYSTEM_MODE_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 AdminMode;              // 0x00 = Admin Mode (Default)
                                // 0x01 = User Mode
  UINT8 RackScaleDesignMode;    // 0x00 = Rack Scale Mode Off (Default)
                                // 0x01 = Rack Scale Mode On
} RSA_BMC_GET_SYSTEM_MODE_RESPONSE;

// Code 26h
typedef struct _RSA_BMC_SET_SYSTEM_MODE_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 AdminMode;              // 0x00 = Admin Mode (Default)
                                // 0x01 = User Mode
  UINT8 RackScaleDesignMode;    // 0x00 = Rack Scale Mode Off (Default)
                                // 0x01 = Rack Scale Mode On
} RSA_BMC_SET_SYSTEM_MODE_REQUEST;

typedef struct _RSA_BMC_SET_SYSTEM_MODE_RESPONSE {
  UINT8 CompletionCode;         // 0x00  = Normal/Command Successful
                                // 0xD4h = Insufficient privilege level
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 AdminMode;              // 0x00 = Admin Mode (Default)
                                // 0x01 = User Mode
  UINT8 RackScaleDesignMode;    // 0x00 = Rack Scale Mode Off (Default)
                                // 0x01 = Rack Scale Mode On
} RSA_BMC_SET_SYSTEM_MODE_RESPONSE;




/*----------------------------------------------------------------------------------------
    Definitions for TPM Configuration commands
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_RSA_BMC_GET_TPM_CONFIG  0x29
#define EFI_INTEL_RSA_BMC_SET_TPM_CONFIG  0x30
#define EFI_INTEL_RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE  0x31

//
//Get TPM Configuration
//
typedef struct _RSA_BMC_GET_TPM_CONFIG_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
} RSA_BMC_GET_TPM_CONFIG_REQUEST;


typedef struct _RSA_BMC_GET_TPM_CONFIG_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 TpmCommandStatus;       // 0x00 = Command Not Valid
                                // 0x01 = Command Valid. BIOS must act on this
  UINT8 TpmVersionSelection;    // 0x00 = Disable TPM (Default)
                                // 0x01 and above = Desired TPM version value as identified in the TPM Configuration Index
  UINT8 ClearTpmAction;         // 0x00 = Preserve User Tpm
                                // 0x01 = Clear User Tpm Ownership
} RSA_BMC_GET_TPM_CONFIG_RESPONSE;

//
//Set TPM Configuration
//
typedef struct _RSA_BMC_SET_TPM_CONFIG_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 TpmCommandStatus;       // 0x00 = Command Not Valid or BIOS completed setting up TPM
                                // 0x01 = Command Valid.
  UINT8 TpmVersionSelection;    // 0x00 = Disabled TPM
                                // 0x01 or above = Desired TPM version value as identified in TPM Config Index
  UINT8 ClearTpmAction;         // 0x00 = Preserve User TPM Ownership
                                // 0x01 = Clear User TPM Ownership
} RSA_BMC_SET_TPM_CONFIG_REQUEST;

typedef struct _RSA_BMC_SET_TPM_CONFIG_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
                                // 0xD4 = Insufficient Privilege Level
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 TpmCommandStatus;       // 0x00 = Command Not Valid
                                // 0x01 = Command Valid. BIOS must act on this
  UINT8 TpmVersionSelection;    // 0x00 = Disable TPM (Default)
                                // 0x01 and above = Desired TPM version value as identified in the TPM Configuration Index
  UINT8 ClearTpmAction;         // 0x00 = Preserve User Tpm
                                // 0x01 = Clear User Tpm Ownership
} RSA_BMC_SET_TPM_CONFIG_RESPONSE;

//
//TPM Update Complete
//
typedef struct _RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
  UINT8 TpmCommandStatus;       // 0x00 = BIOS Completed Setting up TPM
  UINT8 TpmVersionSelection;    // 0x00 = Disabled TPM
                                // 0x01 or above = Desired TPM version value as identified in TPM Config Index
  UINT8 ClearTpmAction;         // 0x00 = Preserve User TPM Ownership
                                // 0x01 = Clear User TPM Ownership
} RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_REQUEST;

typedef struct _RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
                                // 0xD4 = Insufficient Privilege
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04
} RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_RESPONSE;

/*----------------------------------------------------------------------------------------
    Definitions for Get ACPI Table Names commands
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_RSA_BMC_GET_ACPI_TABLE_NAMES  0x32
#define EFI_INTEL_RSA_BMC_SET_ACPI_TABLE_NAMES  0x33

//
//Get ACPI_TABLE_NAMES
//
typedef struct _RSA_BMC_GET_ACPI_TABLE_NAMES_REQUEST {
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04 
} RSA_BMC_GET_ACPI_TABLE_NAMES_REQUEST;


typedef struct _RSA_BMC_GET_ACPI_TABLE_NAMES_RESPONSE {
  UINT8 CompletionCode;         // 0x00 = Normal/Command Successful
  UINT8 IntelRsdSpec;           // IntelRsdSpec = 0x04 
  UINT8 DataLength;             // DataLength is 4*N+3
  UINT32 DataArray[MDR_MAX_ACPITABLE];           // DataArray is 4 byte ACPI names ASCII encoding format
} RSA_BMC_GET_ACPI_TABLE_NAMES_RESPONSE;

typedef
EFI_STATUS
(EFIAPI *MDR_REGION_READ) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL       *This,
  IN     UINT8                               RequestRegion,
  IN     UINT8                               RequestLength,
  IN     UINT16                              RequestOffset,
  IN OUT RSA_BMC_MDR_REGION_READ_RESPONSE    *ResponseData,
  IN     UINTN                               ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *MDR_REGION_WRITE) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL      *This,
  IN     UINT8                              SessionLockHandle,
  IN     UINT8                              RequestRegion,
  IN     UINT8                              *RequestData,
  IN     UINT8                              RequestLength,
  IN     UINT16                             RequestOffset,
  IN OUT RSA_BMC_MDR_REGION_WRITE_RESPONSE  *ResponseData,
  IN     UINTN                              ResponseSize
  );


typedef
EFI_STATUS
(EFIAPI *GET_MDR_REGION_LOCK) (
  IN EFI_RSA_BMC_COMMANDS_PROTOCOL               *This,
  IN     UINT8                                   LockType, 
  IN     UINT8                                   RequestRegion,
  IN OUT RSA_BMC_MDR_REGION_LOCK_RESPONSE        *ResponseData,
  IN     UINTN                                   ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *GET_MDR_REGION_UPDATE_COMPLETE) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL                *This,
  IN     UINT8                                        SessionLockHandle,
  IN     UINT8                                        RequestRegion,
  IN OUT RSA_BMC_MDR_REGION_UPDATE_COMPLETE_RESPONSE  *ResponseData,
  IN     UINTN                                        ResponseSize
  );


typedef
EFI_STATUS
(EFIAPI *GET_MDR_REGION_STATUS) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL              *This,
  IN     UINT8                                      RequestRegion,
  IN OUT RSA_BMC_GET_MDR_REGION_STATUS_RESPONSE     *ResponseData,
  IN     UINTN                                      ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *GET_SYSTEM_MODE) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL             *This,
  IN OUT RSA_BMC_GET_SYSTEM_MODE_RESPONSE          *ResponseData,
  IN     UINTN                                     ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *SET_SYSTEM_MODE) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL              *This,
  IN     RSA_BMC_SET_SYSTEM_MODE_REQUEST            RequestData,
  IN OUT RSA_BMC_SET_SYSTEM_MODE_RESPONSE           *ResponseData,
  IN     UINTN                                      ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *GET_TPM_CONFIGURATION) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL                 *This,
  IN OUT RSA_BMC_GET_TPM_CONFIG_RESPONSE               *ResponseData,
  IN     UINTN                                         ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *SET_TPM_CONFIGURATION) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL                 *This,
  IN     RSA_BMC_SET_TPM_CONFIG_REQUEST                RequestData,
  IN OUT RSA_BMC_SET_TPM_CONFIG_RESPONSE               *ResponseData,
  IN     UINTN                                         ResponseSize
  );

typedef
EFI_STATUS
(EFIAPI *TPM_CONFIGURATION_UPDATE_COMPLETE) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL                        *This,
  IN     RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_REQUEST           RequestData,
  IN OUT RSA_BMC_TPM_CONFIG_UPDATE_COMPLETE_RESPONSE          *ResponseData,
  IN     UINTN                                                ResponseSize
  );

typedef
EFI_STATUS 
(EFIAPI *GET_ACPI_TABLE_NAMES) (
  IN     EFI_RSA_BMC_COMMANDS_PROTOCOL                       *This,
  IN OUT RSA_BMC_GET_ACPI_TABLE_NAMES_RESPONSE               *ResponseData,
  IN     UINTN                                               ResponseSize
  );


//
// RSA BMC Commands Protocol
//

struct _EFI_RSA_BMC_COMMANDS_PROTOCOL {
  GET_MDR_REGION_STATUS              GetMdrRegionStatus;
  GET_MDR_REGION_UPDATE_COMPLETE     GetMdrRegionUpdateComplete;
  MDR_REGION_READ                    MdrRegionRead;
  MDR_REGION_WRITE                   MdrRegionWrite;
  GET_MDR_REGION_LOCK                GetMdrRegionLock;
  GET_SYSTEM_MODE                    GetSystemMode;
  SET_SYSTEM_MODE                    SetSystemMode;
  GET_TPM_CONFIGURATION              GetTpmConfiguration;
  SET_TPM_CONFIGURATION              SetTpmConfiguration;
  TPM_CONFIGURATION_UPDATE_COMPLETE  TpmConfigurationUpdateComplete;
  GET_ACPI_TABLE_NAMES               GetAcpiTableNames;
  };

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiRsaBmcCommandsProtocolGuid;

#pragma pack()
#endif  // _RSA_BMC_COMMANDS_PROTOCOL_H_
