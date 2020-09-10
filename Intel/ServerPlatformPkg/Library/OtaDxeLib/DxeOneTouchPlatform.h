/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#ifndef _DXE_ONE_TOUCH_PLATFORM_H_
#define _DXE_ONE_TOUCH_PLATFORM_H_
//APTIOV_SERVER_OVERRIDE_START
//#include <Library/Tpm2CommandLib.h>
//APTIOV_SERVER_OVERRIDE_END
#include <Protocol/Tcg2Protocol.h>
#include <Protocol/TcgService.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/SocketIioVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/MeRcVariable.h>
#include <Tcg2ConfigNvData.h>

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
// ****************************	PLATFORM SPECIFIC DATA STRUCTURE **************************************
//-----------------------------------------------------------------------------------------------------
// OT_PLATFORM_DATA: Platform specific data as implemented in BIOS.
typedef struct {
  SYSTEM_CONFIGURATION               *SystemConfig;            // SYSTEM_CONFIGURATION setup data
  SOCKET_PROCESSORCORE_CONFIGURATION Spcc;                     // SOCKET_PROCESSORCORE_CONFIGURATION setup data
  SOCKET_IIO_CONFIGURATION           Sic;                      // SOCKET_IIO_CONFIGURATION setup data
  TCG2_DEVICE_DETECTION              T2dd;                     // TCG2_DEVICE_DETECTION setup data
  TCG2_CONFIGURATION                 T2c;                      // TCG2_CONFIGURATION setup data
  ME_RC_CONFIGURATION                Merc;                     // ME_RC_CONFIGURATION setup data
  UINT32                             Sca;                      // SYSTEM_CONFIGURATION attribute
  UINT32                             Spcca;                    // SOCKET_PROCESSORCORE_CONFIGURATION attribute
  UINT32                             Sica;                     // SOCKET_IIO_CONFIGURATION attribute
  UINT32                             T2dda;                    // TCG2_DEVICE_DETECTION attribute
  UINT32                             T2ca;                     // TCG2_CONFIGURATION attribute
  UINT32                             Merca;                    // ME_RC_CONFIGURATION attribute
  BOOLEAN                            Scm;                      // TRUE: SYSTEM_CONFIGURATION setup data modified
  BOOLEAN                            Spccm;                    // TRUE: SOCKET_PROCESSORCORE_CONFIGURATION setup data modified
  BOOLEAN                            Sicm;                     // TRUE: SOCKET_IIO_CONFIGURATION setup data modified
  BOOLEAN                            T2cm;                     // TRUE: TCG2_CONFIGURATION setup data modified
  BOOLEAN                            Mercm;                    // TRUE: ME_RC_CONFIGURATION setup data modified
  EFI_TCG2_PROTOCOL                  *Tcg2Protocol;            // Ptr to EFI_TCG2_PROTOCOL (valid for TPM2.0)
  EFI_TCG_PROTOCOL                   *TcgProtocol;             // Ptr to EFI_TCG_PROTOCOL (valid for TPM1.2)
  TPM_PERMANENT_FLAGS                Tpm12PermanentFlags;      // TPM1.2 Permanent Flags (read on entry, updated during task execution) (valid for TPM1.2)
  BOOLEAN                            Tpm12PreparedForTask;     // TRUE: TPM1.2 prepared for task execution (valid for TPM1.2)
  BOOLEAN                            Tpm12ActivateStateChange; // TRUE: TPM1.2 Activate/Deactivate State Changed (valid for TPM1.2)
                                                               // TPM1.2 Activate <--> Deactivate requires reboot even before any TPM1.2 task can be performed
  UINT16                             PasswordAttribute;        // Bit-mapped Password Attribute (definition same as OXP_HEADER.PasswordAttribute)
  UINT16                             FeatureSupported;         // Bit-mapped Feature Supported (definition same as OXP_HEADER.FeatureSuported)
  UINT16                             FeatureEnabled;           // Bit-mapped Feature Enabled (definition same as OXP_HEADER.FeatureEnabled)
//
  UINT16                             TpmUsage;                 // Bit-mapped TPM Usage (definition same as OXP_HEADER.TpmUsage)
  UINT32                             TpmCommand;               // Last TPM command issued
  UINT32                             TpmRc;                    // TPM response code of last TPM command issued
//
  BOOLEAN                            TaskHasOutputData;        // Task has output data beyond OXP_HEADER
//
  UINT16                             MaxMkTmeKeys;             // Max #of MK-TME keys available for use
                                                               // This value is obtained from MSR 981h. This value may NOT be a power of 2.
                                                               // 0 = MK-TME not supported
  UINT8                              MaxMkTmeKeyIdBits;        // Max #of bits allowed for use as Key Identifiers for MK-TME
                                                               // This value is obtained from MSR 981h
                                                               // 0 = MK-TME not supported
  UINT8                              MkTmeKeyIdBits;           // #of bits allocated for use as Key Identifiers for MK-TME
                                                               // This value is obtained from MSR 982h
//
  UINT16                             PfrState;                 // Bit-mapped PFR State (same as OXP_DISCOVERY_INFO.PfrState)
  UINT8                              PfrRecoveryCount;         // #of Recovery since last AC power cycle  (same as OXP_DISCOVERY_INFO.PfrRecoveryCount)
  UINT8                              PfrLastRecoveryReason;    // Last Recovery Reason  (same as OXP_DISCOVERY_INFO.PfrLastRecoveryReason)
  UINT8                              PfrPanicEventCount;       // #of Panic Event since last AC power cycle  (same as OXP_DISCOVERY_INFO.PfrPanicEventCount)
  UINT8                              PfrLastPanicReason;       // Last Panic Reason  (same as OXP_DISCOVERY_INFO.PfrLastPanicReason)
//
} OT_PLATFORM_DATA;
//
// Bit-mapped definition of different password
#define OT_NO_PASSWORD     0x00
#define OT_ADMIN_PASSWORD  0x01
#define OT_USER_PASSWORD   0x02
//
//-----------------------------------------------------------------------------------------------------
// LT reqgister definitions
#define LTCS_EXISTS_REGISTER    0x00000000FED30010  // TXT EXISTS Register
#define LTCS_EMIF_REGISTER      0x00000000FED30200  // TXT Memory Interface Version and Capability
#define LTCS_FTIF_REGISTER      0x00000000FED30800  // TXT TPM Interface
//
//-----------------------------------------------------------------------------------------------------
// TME/MK-TME reqgister/MSR definitions
//
// TME Support information: CPUID Input EAX = 7, Input ECX = 0, Output ECX Bit-13
#define CPUID_LEAF_EAX_EXTENDED_FEATURE_FLAG    0x07 // CPUID Leaf
#define CPUID_SUBLEAF_ECX_EXTENDED_FEATURE_FLAG 0x00 // CPUID Subleaf
#define CPUID_OUTPUT_EBX_SGX_SUPPORT_BIT      0x0004 // EBX:Bit-2  = 1 -> SGX supported
#define CPUID_OUTPUT_ECX_TME_SUPPORT_BIT      0x2000 // ECX:Bit-13 = 1 -> TME supported
//
#define IA32_TME_CAPABILITY_MSR          0x981       // MSR 981h
typedef union {
  struct {
    UINT64  SupportAesXts128: 1;// Bit-0   : Supports AES-XTS-128 encryption algorithm
    UINT64  Reserved: 31;       // Bit31-1 : Reserved
    UINT64  MaxKeyIdBits: 4;    // Bit35-32: Max #of bits that are allowed for use as Key Identifiers for MK-TME (0 if MK-TME not supported)
    UINT64  MaxNumKeys: 15;     // Bit50-36: Max #of keys which are available for use (0 if MK-TME not supported)
    UINT64  Reserved2: 13;      // Bit63-51: Reserved
  } bits;
  UINT64  data;
} TME_CAPABILITY;
//
#define IA32_TME_ACTIVATE_MSR 0x982  // MSR 982h
typedef union {
  struct {
    UINT64  Lock: 1;          // Bit-0   : Lock RO, Set uopon successful WRMSR (or first SMI), written value ignored
    UINT64  TmeEnableRwl: 1;  // Bit-1   : TME Enable RWI, Also enables and locks MK-TME (MK-TME cannot be enabled without enabling TME)
    UINT64  KeySelect: 1;     // Bit-2   : Key Select, 0/1 = (Create a new TME Key)/(Restore TME Key from storage)
    UINT64  KeySave: 1;       // Bit-3   : Save TME key for standby, Save key in a storage to be used during resume from standby
    UINT64  TmeAlgo: 4;       // Bit7-4  : TME Policy/Encryption Algorithm (0000 = AES-XTS-128)
    UINT64  Reserved: 24;     // Bit31-8 : Reserved
    UINT64  KeyIdBits: 4;     // Bit35-32: #of bits allocated for use as Key Identifiers for MK-TME
    UINT64  Reserved2: 12;    // Bit47-36: Reserved
    UINT64  CryptoAlgs: 16;   // Bit63-48: Bitmask of encryption algorithms allowed for MK-TME (Reserved if MK-TME is not enumerated)
                              // Bit-48: AES-XTS-128
                              // Bit63:49: Reserved (#GP)
  } bits;
  UINT64  data;
} TME_ACTIVATE;
//
//-----------------------------------------------------------------------------------------------------
// **************************** FUNCTION PROTOTYPE ****************************************************
//-----------------------------------------------------------------------------------------------------
/**
  DisplayBinaryData(): Display binary data.

  @param  Data INPUT: UINT8*, Ptr to data
  @param  Size INPUT: UINTN, Data size in bytes

  @retval None
**/
VOID
DisplayBinaryData (
  UINT8      *Data,
  UINTN      Size
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_AllocatePlatformInformationArea(): Allocate platform information area.

  @param  None  INPUT: None
  @param  Opd   OUTPUT: Ptr to OT_PLATFORM_DATA, Success
    NULL, Error
**/
OT_PLATFORM_DATA*
OT_AllocatePlatformInformationArea (
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_ClearAndDeallocatePlatformInformationArea(): Clear and Deallocate platform information area.

  @param  Opd  INPUT: Ptr to OT_PLATFORM_DATA
  @param  None OUTPUT

  Note: Before deallocating, clear platform information area to prevent the data from being hacked.

**/
VOID
OT_ClearAndDeallocatePlatformInformationArea (
  IN  OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_ReadPlatformInformation(): Read platform information.

  @param  Opd     INPUT/OUTPUT: Ptr to OT_PLATFORM_DATA where platform information to be read
  @param  Result  OUTPUT: BOOLEAN, TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_ReadPlatformInformation (
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_WritePlatformInformation(): Write platform information, if necessary.

  @param  Opd    INPUT: Ptr to OT_PLATFORM_DATA having platform information
  @param  Result OUTPUT: BOOLEAN, TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_WritePlatformInformation (
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//-------------------------------------------------------------------------------------------------------
/**
  Returns the information of different installed passwords.

  @param[in] Data SYSTEM_CONFIGURATION*: Ptr to System configuration

  @retval PasswordBitmap UINT8: Bit-mapped information of installed passwords
    OT_NO_PASSWORD    No password installed
    OT_ADMIN_PASSWORD Administrative password installed
    OT_USER_PASSWORD  User password installed
**/
UINT8
OT_CheckIfPasswordInstalled (
  IN  SYSTEM_CONFIGURATION *Data
  );
//
//-------------------------------------------------------------------------------------------------------
/**
  Read system configuration.

  @param[in] Data     SYSTEM_CONFIGURATION*: Ptr to System configuration
  @param[out] Result  OUTPUT: BOOLEAN, TRUE/FALSE: Success/Error

**/
VOID*
OT_ReadSystemConfiguration (
  IN  SYSTEM_CONFIGURATION *Data
  );
//
//-------------------------------------------------------------------------------------------------------
/**
  Match the given UnicodeZ password with the BIOS Administrative Password.

  @param[in] *Scd      SYSTEM_CONFIGURATION*: Ptr to SYSTEM_CONFIGURATION
  @param[in] *Password CHAR16*: Ptr to given UnicodeZ password

  @retval BOOLEAN  TRUE/FALSE: Success/Error

**/
BOOLEAN
OT_MatchAdminPassword (
  IN  SYSTEM_CONFIGURATION *Scd,
  IN  CHAR16               *Password
  );
//
//------------------------------------------------------------------------------------------------------
// **************************** dTPM Function Prototypes ***********************************************
//-----------------------------------------------------------------------------------------------------
/**
  OT_PrepareTpmBeforeOobTask(): Prepare TPM before performing OOB Task.

  @param[in]  Opd     INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_PrepareTpmBeforeOobTask (
  IN     OT_PLATFORM_DATA *Opd
  );
//
//-----------------------------------------------------------------------------------------------------
/**
  OT_PrepareTpmAfterOobTask(): Prepare TPM after performing OOB Task.

  @param[in]  Opd    INPUT: Ptr to OT_PLATFORM_DATA

  @retval     Result OUTPUT: UINT8, Status code

**/
UINT8
OT_PrepareTpmAfterOobTask (
  IN     OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmClear(): Clear dTPM.

  @param  Auth     INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd      INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result   OUTPUT: UINT8, Status Code
                           OXP_SUCCESS, OXP_DTPM_CLEAR_ERROR
**/
UINT8
OT_DTpmClear (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmEnable(): Enable dTPM.

  @param  Auth     INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd      INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result   OUTPUT: UINT8, Status Code
                           OXP_SUCCESS, OXP_DTPM_ENABLE_ERROR, OXP_DTPM_ACTIVATE_ERROR
**/
UINT8
OT_DTpmEnable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_DTpmDisable(): Disable dTPM.

  @param  Auth    INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd     INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result  OUTPUT: UINT8, Status Code
                          OXP_SUCCESS, OXP_DTPM_DISABLE_ERROR
**/
UINT8
OT_DTpmDisable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
// **************************** fTPM Function Prototypes ***********************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmClear(): Clear fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_DTPM_CLEAR_ERROR
**/
UINT8
OT_FTpmClear (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmEnable(): Enable fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_FTPM_ENABLE_ERROR
**/
UINT8
OT_FTpmEnable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmDisable(): Disable fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_FTPM_DISABLE_ERROR
**/
UINT8
OT_FTpmDisable (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_FTpmSetPcrBanks(): Provision fTPM.

  @param  Auth   INPUT:  Ptr to TPM2B_AUTH, Platform auth value (NULL: no platform auth change)
  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Ci     INPUT:  BOOLEAN*, Ptr to Boolean Variable
                         OUTPUT: TRUE  -> Set PCR Banks command issued
                         OUTPUT: FALSE -> Set PCR Banks command NOT issued
  @param  Result OUTPUT: UINT8, Status Code

**/
UINT8
OT_FTpmSetPcrBanks (
  IN     TPM2B_AUTH       *Auth,
  IN OUT OT_PLATFORM_DATA *Opd,
  IN OUT BOOLEAN          *Ci
  );
//
//------------------------------------------------------------------------------------------------------
// **************************** TXT Function Prototypes ************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_TxtEnable(): Enable TXT.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_TXT_ENABLE_ERROR
**/
UINT8
OT_TxtEnable (
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
/**
  OT_TxtDisable(): Disable TXT.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS, OXP_TXT_ENABLE_ERROR
**/
UINT8
OT_TxtDisable (
  IN OUT OT_PLATFORM_DATA *Opd
  );
//
//------------------------------------------------------------------------------------------------------
// **************************** TME/MK-TME Function Prototypes *****************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_HandleMkTme(): Handle TME/MK-TME.

  @param  Opd    INPUT:  Ptr to OT_PLATFORM_DATA
  @param  Tme    INPUT:  BOOLEAN*, Ptr to area indicating TME Operation
                                If NULL, Do not touch TME settings
                                If Non-Null, TRUE/FALSE: Enable/Disable TME
  @param  MkTme  INPUT:  BOOLEAN*, Ptr to area indicating MK-TME Operation
                                If NULL, Do not touch MK-TME settings
                                If Non-Null, TRUE/FALSE: Enable/Disable MK-TME
  @param  Result OUTPUT: UINT8, Status Code
                                OXP_SUCCESS Success
                                Other  Error
**/
UINT8
OT_HandleMkTme (
  IN OUT OT_PLATFORM_DATA *Opd,
  IN     BOOLEAN          *Tme,
  IN     BOOLEAN          *MkTme
  );
//
//------------------------------------------------------------------------------------------------------
// **************************** PFR Function Prototypes ************************************************
//------------------------------------------------------------------------------------------------------
/**
  OT_HandlePfr(): Handle PFR.

  @param  Opd    INPUT : OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
          Task   INPUT : UINT16, PFR Task (Bit-mapped task information defined in OXP_PFR_INFO.Attribute)

  @param  Result OUTPUT: UINT8, Status Code
                         OXP_SUCCESS Success
                         Other  Error
**/
UINT8
OT_HandlePfr (
  IN OUT OT_PLATFORM_DATA  *Opd,
  IN     UINT16            Task
  );
//
//------------------------------------------------------------------------------------------------------
#pragma pack (pop)
//------------------------------------------------------------------------------------------------------

#endif
