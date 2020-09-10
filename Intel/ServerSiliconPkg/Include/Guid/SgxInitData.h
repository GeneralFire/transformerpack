/** @file
  Data structures used by SGX init flows

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef   __SGX_INIT_DATA__
#define   __SGX_INIT_DATA__

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include "SysHost.h"

#define SGX_D_ERROR   EFI_D_ERROR
#define SGX_D_INFO    EFI_D_VERBOSE
#define SGX_D_VERBOSE EFI_D_VERBOSE

#define SGX_DEBUG(Expression)   \
  do {                           \
      DEBUG (Expression);       \
  } while (FALSE)

//
// Below SGX structures are based on MCHECK 2.4 SAS Platform Attestation document
//

// Size Transformation MAcros
#define CONVERT_GB_TO_B(x) ((UINT64) ((UINT64)x << 30))
#define CONVERT_MB_TO_B 20
#define UEFI_CONVERT_B_TO_GB(x) ((UINT32) RShiftU64 (x, 30))

// MAX Values
#define VALID_PRM_PER_IMC_COUNT_LCC         3
#define VALID_PRM_PER_IMC_0_LCC             CONVERT_GB_TO_B(2)
#define VALID_PRM_PER_IMC_1_LCC             CONVERT_GB_TO_B(16)
#define VALID_PRM_PER_IMC_2_LCC             CONVERT_GB_TO_B(128)
#define MIN_VALID_PRM_PER_DOMAIN            CONVERT_GB_TO_B(2)
#define MAX_VALID_PRM_PER_IMC               CONVERT_GB_TO_B(128)
#define MAX_PRMRR_COUNT                     8
#define CURR_SUPPORTED_PRMRR_COUNT          8
#define MAX_CPU_SUPPORTED_SGX               4
#define SGX_MCHECK_MAX_PACKAGE              8
#define SGX_MCHECK_MAX_KEYS                 32
#define SGX_MCHECK_MAX_P2P_LINKS            7
#define SGX_SUPPORTED_MEM_TYPES             1
#define SGX_MCHECK_SUPPORTED_PACKAGES       4
// Other
#define PROCESSOR_AS_PBSP_BIT               BIT3
#define B_TME_ACTIVATE_ENABLE               BIT1
#define SHA256_DIGEST_SIZE                  32
#define SHA3072_DIGEST_SIZE                 384
#define AES128_KEY                          16
#define UUID_BYTES_SIZE                     16
#define EPOCH_SIZE                          32
#define SGX_4K                              4096
#define INDENTATION_OFFSET                  2
#define SECURITY_PROPERTIES_TEM_SUPPORTED   0x10000
#define EPC_ENABLE                          BIT0
#define LOW_EPC_SHIFT                       12
#define HIGH_EPC_SHIFT                      32
#define SGX_SERVER_URL_LENGTH               256
// Versions
#define SGX_STRUCTURE_VERSION_1                      1 // ICX, ICX-D
#define SGX_STRUCTURE_VERSION_2                      2 // SPR
#define SGX_REGISTRATION_HEADER_VERSION              1
#define SGX_REGISTRATION_MANIFEST_HEADER_VERSION     2 // 2 manifests CCB
// SAI - Security Atributes of the Initiator
#define SGX_SAI_RAC                         0
#define SGX_SAI_WAC                         1
#define SGX_SAI_CP                          2


// Other Guids
extern EFI_GUID gEfiHashAlgorithmSha256Guid;

// DEBUG
#define SGX_DEBUG_RESTORE_KEY_BLOBS_FROM_REGISTRATION_PACKAGE_INFO 0

#pragma pack(1)

typedef enum {
  McheckRegularBoot                       = 0,
  McheckFirstPlatformBinding              = 1,
  McheckTcbRecovery                       = 2,
  McheckAddingNewPackages                 = 3,
  McheckInvalid                           = 0xFF
} SGX_MCHECK_SCENARIO;

// Internal to BIOS / System Firmware
typedef enum {
  SgxNormalBoot,
  SgxFirstBinding,
  SgxTcbRecovery,
  SgxAddPackageBoot,
  SgxAddPackageReboot,
  SgxDisabled
} SGX_UEFI_BOOT_SCENARIO;

// HOB SGX internal interface
extern EFI_GUID gSgxInitDataHobGuid;
#define SGX_INIT_DATA_HOB_GUID                         { 0x4EF0BEDC, 0xC5C1, 0x4E69, { 0x82, 0x18, 0x7D, 0xBE, 0x8A, 0x60, 0x68, 0x8C } }
extern EFI_GUID gSgxPreMemInitHobGuid;
#define SGX_INIT_PREMEM_DATA_HOB                       { 0x9A9E2225, 0xE026, 0x4BFF, { 0x99, 0x17, 0xF1, 0x23, 0xE1, 0xD4, 0x3A, 0xD5 } }

//
// NVRAM SGX internal interface
// part of: SGX_MCHECK_BIOS_PARAM_INFO BiosParamInfo->PlatformManifest
// part of: SGX_MCHECK_BIOS_PARAM_INFO BiosParamInfo->SocketData[Index].KeyBlob
//
#define SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE_VARIABLE_NAME L"SgxRegistrationState"
#define SGX_UEFIFW_KEY_BLOB_VARIABLE_NAME                        L"SgxKeyBlobs"
#define SGX_UEFIFW_PLATFORM_MANIFEST_VARIABLE_NAME               L"SgxPlatformManifest"
extern EFI_GUID gSgxUefiFwRegistrationStateVariableGuid;
extern EFI_GUID gSgxUefiFwKeyBlobsVariableGuid;
extern EFI_GUID gSgxUefiFwPlatformManifestVariableGuid;

//
// SGX OS Agent external interface
//
#define SGX_REGISTRATION_CONFIG_VARIABLE_NAME                  L"SgxRegistrationConfiguration"
#define SGX_REGISTRATION_REQUEST_VARIABLE_NAME                 L"SgxRegistrationServerRequest"
#define SGX_REGISTRATION_PACKAGE_INFO_VARIABLE_NAME            L"SgxRegistrationPackageInfo"
#define SGX_REGISTRATION_RESPONSE_VARIABLE_NAME                L"SgxRegistrationServerResponse"
#define SGX_REGISTRATION_STATUS_VARIABLE_NAME                  L"SgxRegistrationStatus"
#define SGX_UEFI_REGISTRATION_CONFIG_VARIABLE_NAME             L"SgxUefiRegistrationConfiguration"
#define SGX_UEFI_REGISTRATION_REQUEST_VARIABLE_NAME            L"SgxUefiRegistrationServerRequest"
#define SGX_UEFI_REGISTRATION_RESPONSE_VARIABLE_NAME           L"SgxUefiRegistrationServerResponse"
#define SGX_UEFI_REGISTRATION_STATUS_VARIABLE_NAME             L"SgxUefiRegistrationStatus"
#define SGX_LEGACY_REGISTRATION_SW_GUARD_STATUS_VARIABLE_NAME  L"SOFTWAREGUARDSTATUS"
#define SGX_LEGACY_REGISTRATION_EPC_BIOS_VARIABLE_NAME         L"EPCBIOS"
#define SGX_LEGACY_REGISTRATION_EPC_SW_VARIABLE_NAME           L"EPCSW"

// Registration Protocol Variables UUID
extern EFI_GUID gSgxRegistrationConfigVariableGuid;
extern EFI_GUID gSgxRegistrationServerRequestVariableGuid;
extern EFI_GUID gSgxRegistrationPackageInfoVariableGuid;
extern EFI_GUID gSgxRegistrationServerResponseVariableGuid;
extern EFI_GUID gSgxRegistrationStatusVariableGuid;
// Backup Registration Protocol Variables UUID in Flash
extern EFI_GUID gSgxUefiRegistrationConfigVariableGuid;
extern EFI_GUID gSgxUefiRegistrationServerRequestVariableGuid;
extern EFI_GUID gSgxUefiRegistrationServerResponseVariableGuid;
extern EFI_GUID gSgxUefiRegistrationStatusVariableGuid;
// Other Registration Variables UUID
extern EFI_GUID gSgxRegistrationServerInfoGuid;
// Legacy Registration Variables UUID
extern EFI_GUID gSgxLegacyRegistrationSoftwareGuardStatusVariableGuid;
extern EFI_GUID gSgxLegacyRegistrationEpcBiosVariableGuid;
extern EFI_GUID gSgxLegacyRegistrationEpcSwVariableGuid;

#define SGX_MCHECK_BIOS_PARAM_INFO_UUID            { 0x13, 0x43, 0xF1, 0x5C, 0xAA, 0x00, 0x42, 0x07, 0xA2, 0x14, 0xA8, 0x4F, 0x35, 0x58, 0x22, 0xA2}

#define SGX_MCHECK_KEY_BLOB_UUID                   { 0x2E, 0xCF, 0x43, 0xFD, 0x61, 0x4E, 0x4F, 0x94, 0x98, 0x2C, 0xDF, 0x36, 0x10, 0xF4, 0x3A, 0x9D}

#define SGX_MCHECK_MEMBERSHIP_CERTIFICATE_UUID     { 0xEB, 0xBD, 0x00, 0xC0, 0xEF, 0x90, 0x49, 0x10, 0xA4, 0x80, 0xF7, 0x72, 0xE7, 0x35, 0x5D, 0xB4}

#define SGX_REGISTRATION_ADD_REQUEST_UUID          { 0x69, 0x65, 0x19, 0xCA, 0x73, 0xC1, 0x47, 0x85, 0xA0, 0xF6, 0x4D, 0x28, 0x9D, 0x37, 0xE9, 0x95}

//
//

typedef union
{
  UINT8               Uuid[16];
  UINT64              Uuid64[2];
} SGX_UUID;

typedef struct {
  SGX_UUID            Uuid;           //MSB
  UINT16              Size;           //LSB
  UINT16              Version;        //LSB
  UINT8               Reserved[12];
} SGX_HEADER;

typedef struct {
  UINT8               Modulus[SHA3072_DIGEST_SIZE];
  UINT32              PubExp;
} SGX_MCHECK_PUBKEY;

typedef struct {
  SGX_HEADER          Header;
  UINT8               CpuSvn;
  SGX_UUID            Prid;
  SGX_UUID            PlatformInstanceId;
  UINT8               PlatformEpoch[EPOCH_SIZE];
  SGX_MCHECK_PUBKEY   Prsk; //Public Key
  SGX_MCHECK_PUBKEY   Rsak; //Public Key
  UINT8               Signature[SHA3072_DIGEST_SIZE];
} SGX_MCHECK_PLATFORM_MEMBERSHIP_CERTIFICATE;

typedef struct {
  SGX_HEADER          Header;
  UINT8               RsName[SHA256_DIGEST_SIZE];
  SGX_MCHECK_PUBKEY   Rsak; //Public Key
  SGX_MCHECK_PUBKEY   Rsek; //Public Key
  UINT8               Signature[SHA3072_DIGEST_SIZE];
} SGX_MCHECK_REGISTRATION_SERVER_ID;

typedef struct {
  SGX_HEADER          Header;
  SGX_UUID            PlatformInstanceId;
  SGX_MCHECK_PUBKEY   Rsak; //Public Key
  UINT8               PlatformEpoch[EPOCH_SIZE];
  UINT32              SecurityProperties;
  UINT32              PackageCount;
} SGX_MCHECK_PLATFORM_INFO;

typedef struct {
  SGX_HEADER          Header;
  UINT32              KeySlotCount;
  UINT32              KeyCount;
  UINT8               CpuSvn;
  SGX_UUID            Prid;
  UINT8               PlatformInfoHash[SHA256_DIGEST_SIZE];
  UINT8               EncWrapKey[SHA3072_DIGEST_SIZE];
  UINT8               PprResetKey[SGX_MCHECK_MAX_KEYS][AES128_KEY];
  UINT8               Signature[SHA3072_DIGEST_SIZE];
} SGX_MCHECK_ENCRYPTED_PLATFORM_KEYS;

typedef struct {
  SGX_HEADER          Header;
  UINT8               CpuSvn;
  SGX_UUID            Prid;
  UINT8               PlatformInfoHash[SHA256_DIGEST_SIZE];
  SGX_UUID            TargetPrid[SGX_MCHECK_MAX_P2P_LINKS];
  SGX_MCHECK_PUBKEY   TargetPrsk[SGX_MCHECK_MAX_P2P_LINKS];
  UINT8               Signature[SHA3072_DIGEST_SIZE];
} SGX_MCHECK_PAIRING_RECEIPT;

typedef struct {
  SGX_HEADER                         Header;
  SGX_MCHECK_PLATFORM_INFO           PlatformInfo;
  SGX_MCHECK_ENCRYPTED_PLATFORM_KEYS EncryptedKeys;
  SGX_MCHECK_PAIRING_RECEIPT         Receipt[SGX_MCHECK_MAX_PACKAGE];
} MCHECK_SGX_PLATFORM_MANIFEST;

typedef struct {
  SGX_HEADER          Header;
  UINT8               CpuSvn;
  UINT32              SecurityProperties;
  UINT8               Reserved1[11];
  SGX_UUID            Prid;
  UINT8               PlatformEpoch[EPOCH_SIZE];
  UINT8               RsakHash[SHA256_DIGEST_SIZE];
  UINT8               Pfk1[AES128_KEY];
  UINT8               PprResetKey[SGX_MCHECK_MAX_KEYS][AES128_KEY];
  UINT8               LastUsedSvn;
  UINT8               Reserved2[15];
  SGX_UUID            TargetPrid[SGX_MCHECK_MAX_P2P_LINKS];
  SGX_UUID            MasterComms[SGX_MCHECK_MAX_P2P_LINKS];
  UINT8               Iv[SGX_MCHECK_MAX_KEYS][12];//IV for different components of the structure
  UINT8               Mac[SGX_MCHECK_MAX_KEYS][16];//MAC for different components of the structure
  UINT8               IvComm[12];//IV for MasterCommsKeys
  UINT8               MacComm[16];//MAC for MasterCommsKeys
} SGX_MCHECK_KEY_BLOB;

typedef struct {
  SGX_MCHECK_KEY_BLOB KeyBlob;
  EFI_TIME            TimeStamp;
  BOOLEAN             Present;
} SGX_UEFIFW_KEY_BLOB;

typedef struct {
  SGX_MCHECK_PLATFORM_MEMBERSHIP_CERTIFICATE PlatformMembershipCertificate;
  BOOLEAN             Present;
} SGX_UEFIFW_PLATFORM_MEMBERSHIP_CERTIFICATE;

typedef struct {
  VOID                *Address;
  UINT64              Size;
} SGX_MCHECK_BUFFER;


#define FLAGS_BLOB_IN_BIT  BIT0
#define FLAGS_BLOB_OUT_BIT BIT1

typedef struct {
  UINT64              Flags;
  SGX_MCHECK_BUFFER   KeyBlob;     // SGX_MCHECK_KEY_BLOB
  SGX_MCHECK_BUFFER   Certificate; // SGX_MCHECK_PLATFORM_MEMBERSHIP_CERTIFICATE
  UINT8               Reserved[24];
} SGX_MCHECK_SOCKET_DATA;

typedef struct {
  UINT8               CpuSvn;
  UINT8               MasterId;
  UINT8               NodeIds[8]; //  Node ID is used to identify the package in the UPI communication
  UINT8               Reserved[22];
  SGX_UUID            Prids[SGX_MCHECK_MAX_PACKAGE];
} SGX_MCHECK_TOPOLOGY_INFO;

typedef struct {
  SGX_HEADER          Header;
  SGX_MCHECK_TOPOLOGY_INFO TopologyInfo;
  UINT8               Scenario;
  UINT8               NumSockets;
  UINT32              SecurityProperties;
  UINT8               Reserved[10];
  SGX_MCHECK_BUFFER   RegistrationServer; // SGX_REGISTRATION_SERVER_INFO
  SGX_MCHECK_BUFFER   PlatformManifest;   // MCHECK_SGX_PLATFORM_MANIFEST
  SGX_MCHECK_BUFFER   WorkingArea;
  SGX_MCHECK_SOCKET_DATA SocketData[SGX_MCHECK_MAX_PACKAGE];
} SGX_MCHECK_BIOS_PARAM_INFO;

//
// SGX Multi-Package Registration UEFI Interface
//


typedef struct {
  SGX_HEADER                         Header;
  UINT16                             UrlSize;
  UINT8                              Url[SGX_SERVER_URL_LENGTH];
  SGX_MCHECK_REGISTRATION_SERVER_ID  RegServerId;
} SGX_REGISTRATION_SERVER_INFO;

typedef struct {
  UINT16                             Version;
  UINT16                             Size;
} SGX_REGISTRATION_HEADER;

#define RS_SAVES_ENCRYPTED_KEYS_TRUE 0
#define RS_SAVES_ENCRYPTED_KEYS_FALSE 1
typedef struct {
  SGX_REGISTRATION_HEADER            Header;
  struct {
    UINT16                           RSsavesEncryptedKeys : 1;
    UINT16                           Reserved : 15;
  } Flags;
  SGX_REGISTRATION_SERVER_INFO       SgxRegServerInfo;
} SGX_REGISTRATION_CONFIG;

//
//


// UEFIFW version of PLATFORM_MANIFEST for SgxRegistrationRequest with registration variable HEADER
typedef struct {
  SGX_REGISTRATION_HEADER            Header;
  MCHECK_SGX_PLATFORM_MANIFEST       McheckPlatformManifestIpe; // Initial Platform Establishment
  MCHECK_SGX_PLATFORM_MANIFEST       McheckPlatformManifestTr;  // TCB Recovery
} SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST;

typedef struct {
  SGX_HEADER                         Header;
  UINT8                              CpuSvn;
  UINT16                             NumPackages;
  SGX_UUID                           PlatformInstanceId;
  SGX_UUID                           Prid[SGX_MCHECK_MAX_PACKAGE];
  SGX_UUID                           Reserved;
} SGX_REGISTRATION_ADD_REQUEST;

// UEFIFW version of ADD_REQUEST for SgxRegistrationRequest with registration variable HEADER
typedef struct {
  SGX_REGISTRATION_HEADER            Header;
  SGX_REGISTRATION_ADD_REQUEST       AddRequest;
} SGX_UEFI_REGISTRATION_ADD_PACKAGE;

typedef union {
  SGX_UEFI_REGISTRATION_PLATFORM_MANIFEST PlatformManifest;
  SGX_UEFI_REGISTRATION_ADD_PACKAGE       AddPackage;
} SGX_REGISTRATION_REQUEST;

typedef struct {
  SGX_REGISTRATION_HEADER            Header;
  SGX_MCHECK_KEY_BLOB                KeyBlobs[SGX_MCHECK_MAX_PACKAGE];
} SGX_REGISTRATION_PACKAGE_INFO;

typedef struct {
  SGX_REGISTRATION_HEADER                      Header;
  SGX_MCHECK_PLATFORM_MEMBERSHIP_CERTIFICATE   PlatformMembershipCertificate[SGX_MCHECK_MAX_PACKAGE];
} SGX_REGISTRATION_RESPONSE;

typedef enum {
  // RS_SUCCESS
  RS_SUCCESS = 0x00,

  // RS_PREMEM_GROUP
  RS_PREMEM_OTHER                        = 0x10,
  RS_PREMEM_NOMEM                        = 0x11,
  RS_PREMEM_SYS_NOT_CAPABLE              = 0x12,
  RS_PREMEM_NO_VALID_PRMRR               = 0x13,
  RS_PREMEM_HW_NOT_CAPABLE               = 0x14,
  RS_PREMEM_TME_DISABLED                 = 0x15,
  RS_PREMEM_SGX_DISABLED                 = 0x16,
  RS_PREMEM_INVALID_PRRMR_SIZE           = 0x17,
  RS_PREMEM_PRMRR_NOT_SECURED            = 0x18,
  RS_PREMEM_MEM_TOPOLOGY_ERR             = 0x19,

  // RS_POSTMEM_GROUP
  RS_POSTMEM_OTHER                       = 0x20,
  RS_POSTMEM_NOMEM                       = 0x21,
  RS_POSTMEM_SYSHOST_NOTFOUND            = 0x22,
  RS_POSTMEM_MMAP_HOST_NOTFOUND          = 0x23,
  RS_POSTMEM_VSPPI_NOTFOUND              = 0x24,
  RS_POSTMEM_MRCHCSPPI_NOTFOUND          = 0x25,
  RS_POSTMEM_SVN_ERR                     = 0x26,
  RS_POSTMEM_REGVARS_ERR                 = 0x27,
  RS_POSTMEM_KEYBLOBS_RES_ERR            = 0x28,
  RS_POSTMEM_PRID_UNLOCK_ERR             = 0x29,
  RS_POSTMEM_DETERMINE_BOOT_ERR          = 0x2A,
  RS_POSTMEM_FIRSTBOOT_ERR               = 0x2B,
  RS_POSTMEM_WARMRESET_ERR               = 0x2C,

  // RS_LATEINIT_GROUP
  RS_LATEINIT_OTHER                      = 0x30,
  RS_LATEINIT_TRIGCALLBACK_ERR           = 0x31,
  RS_LATEINIT_HOBLIST_NOTFOUND           = 0x32,
  RS_LATEINIT_MPSVC_ERR                  = 0x33,
  RS_LATEINIT_INITDATAHOB_RES            = 0x34,
  RS_LATEINIT_UPDTCAPAB_ERR              = 0x35,
  RS_LATEINIT_UPDTPRMRR_ERR              = 0x36,
  RS_LATEINIT_CRDIMM_ERR                 = 0x37,
  RS_LATEINIT_UPDTLEWR_ERR               = 0x38,
  RS_LATEINIT_SYS_NOT_CAPABLE            = 0x39,
  RS_LATEINIT_SGX_DISABLED               = 0x3A,
  RS_LATEINIT_FACTORY_RESET_ERR          = 0x3B,
  RS_LATEINIT_NVSAREA_ERR                = 0x3C,
  RS_LATEINIT_GET_NVVAR_ERR              = 0x3D,
  RS_LATEINIT_EXPOSE_PROTO_ERR           = 0x3E,
  RS_LATEINIT_LOCKVARS_ERR               = 0x3F,
  RS_LATEINIT_VAR_ROTO_ERR               = 0x40,

  // RS_LATE_INIT_CALLBACK_GROUP
  RS_LATEINIT_CALLBACK_OTHER             = 0x50,
  RS_LATEINIT_CALLBACK_NOMEM             = 0x51,
  RS_LATEINIT_CALLBACK_BIOSPARAM_ERR     = 0x52,
  RS_LATEINIT_CALLBACK_UCODE_LAUNCH_ERR  = 0x53,
  RS_LATEINIT_CALLBACK_UPDT_TIMESTMP_ERR = 0x54,
  RS_LATEINIT_CALLBACK_UPDT_PKG_INFO_ERR = 0x55,
  RS_LATEINIT_CALLBACK_LAUNCHCTRL_ERR    = 0x56,
  RS_LATEINIT_CALLBACK_UPDT_KEYBLOBS_ERR = 0x57,
  RS_LATEINIT_CALLBACK_TCBRECOVERY_ERR   = 0x58,
  RS_LATEINIT_CALLBACK_STORPLATMANIF_ERR = 0x59,
  RS_LATEINIT_CALLBACK_LEGACYVARS_ERR    = 0x5A,
  RS_LATEINIT_CALLBACK_REGSTATE_VAR_ERR  = 0x5B,

  // RS_MCHECK_GROUP
  RS_MCHECK_BEGIN                        = 0x70,
  RS_MCHECK_END                          = 0x7F
} SGX_ERROR_CODE_ENUM;

typedef union {
  struct {
    UINT8              RegistrationErrorCode : 7;
    UINT8              SourceOfErrorCode : 1;
  } ErrorCodeFields;
  UINT8                ErrorCode;
} SGX_ERROR_CODE;

typedef struct {
  SGX_REGISTRATION_HEADER  Header;
  struct {
    UINT16                 SgxRegistrationComplete : 1;
    UINT16                 SgxPackageInfoReadComplete : 1;
    UINT16                 Reserved : 14;
  } Status;
  SGX_ERROR_CODE           ErrorCode;
} SGX_REGISTRATION_STATUS;

//
//
//

///
/// EPC data structure for BIOS Variable
///
typedef struct _EPC_BIOS_CONFIG {
    UINT32  SupportedPrmBins; ///< supported EPC bins; Bit 0- Support for 1 MB, 1- Support for 2 MB, 2- Support for 4 MB,
    UINT32  MaxEpcSize;       ///< Maximum EPC size supported by platform in MB
    UINT32  AllocatedEpcSize; ///< Allocated EPC size in MB
    UINT32  EpcMap[32];       ///< Mapping of PRM size to corresponding EPC size
} EPC_BIOS_CONFIG;

///
/// EPC data structure for OS Variable
///
typedef struct {
  UINT32 RequestedEpcSize; ///< Requested EPC size in MB
} EPC_OS_CONFIG;

///
/// Data structure for SGX Policy Status
/// The presence of this variable implies that SGX is supported
/// For backward compatibility:
/// Absence of this variable implies that SGX is supported and defaulted to 'Software Controlled'
///
typedef struct {
  /**
  Bitmap of SGX Policy setting
   - BIT[1:0]: SgxStatus
      - 00b = Disabled.
      - 01b = Enabled.
      - 10b = Software Controlled.
      - 11b = Reserved.
  **/
  UINT8 SgxStatus:2;
  UINT8 Reserved:6;  ///< Reserved 6 bits
} SOFTWARE_GUARD_STATUS;

#pragma pack()


//
//SGX_TODO: Remove all hardcoded registration server data when
//MCHECK populate it by default and after PowerOn [Begin]
//
#define SGX_MCHECK_REGISTRATION_SERVER_UUID { 0x31, 0xA1, 0x2A, 0xFE, 0x07, 0x20, 0x4E, 0xBC, 0xB6, 0x4E, 0xC4, 0xB3, 0xC7, 0xF8, 0xBC, 0x0F }

#define SGX_MCHECK_SERVER_URL "https://pre.api.trustedservices.intel.com:443"
#define SGX_MCHECK_SBX_SERVER_URL "https://sbx.api.trustedservices.intel.com:443"
#define SGX_MCHECK_SERVER_RSNAME \
      { \
         0x5E306161EA7257AF\
        ,0x337B416B89177A10\
        ,0x5D3ECB09435253C5\
        ,0xCF8F292FD10BD407\
      }

#define SGX_MCHECK_SERVER_RSAK\
      {\
         0x7FC1B9FDCA250CCB\
        ,0x72EF96645F6E7CC1\
        ,0xBBB59C19AF2C3876\
        ,0x5255F3E36D18E33C\
        ,0x717587CBAC1737ED\
        ,0xDE4A25A3F64A6326\
        ,0x35A4EEC1346CA4ED\
        ,0x7FE57CB012D42F6A\
        ,0xA29B3D5BBD493726\
        ,0xA19D9CEDBADB51ED\
        ,0xC12D43312DAD4E10\
        ,0x16C675DB82E7049B\
        ,0xCE9053DB862E0C85\
        ,0x5D887A6CDD726294\
        ,0x3D9C71CC1B7A4039\
        ,0x675AD89FA26BA305\
        ,0xB6B543B9B826CD0B\
        ,0x413559AE21FE92DF\
        ,0xD09DB753DAF1C33D\
        ,0x95C855F27DBB0A73\
        ,0x6D95F49032F878A5\
        ,0x8847EF7F362E0FD5\
        ,0xBE452C79744AF26E\
        ,0xC0474AC3FFC2A51F\
        ,0x5563B92FE57BF412\
        ,0x51AB6345BA3C9832\
        ,0xAB64AE51696AB37B\
        ,0x74A9838EDD1919A7\
        ,0xF689B82F5A785A64\
        ,0x1F1FB1D2AE6D05DC\
        ,0x9D51BB1D819E97DC\
        ,0x6A0384654A15C9A7\
        ,0x1286EBE126E2C644\
        ,0x252E2C248DA6EDD1\
        ,0xD7C4782F447444B6\
        ,0x7985E3901702AF29\
        ,0x83E4EC0B84E70147\
        ,0xB88864D36585B53B\
        ,0x9C5D74F9CBD94AA4\
        ,0x58A724BF0821A028\
        ,0x19D0EFBD4A9624B2\
        ,0xC4391E14E582DB41\
        ,0x13285048CAA58A3D\
        ,0x70C6363C17047EBB\
        ,0xC86AE2E1636B086\
        ,0xA299968C68F4A76D\
        ,0x853224F91725B9A\
        ,0xA2C071E50EC02B00\
    }


#define SGX_MCHECK_SERVER_RSAK_EXP 0x10001

#define SGX_MCHECK_SERVER_RSEK \
      {\
         0x12CF4B066EF611A9\
        ,0x56462E8BF6912264\
        ,0x37218434859C086A\
        ,0x791C47A05F19C3F4\
        ,0xC9EA2A38BD33ACB2\
        ,0x9ECF2D4E71D6C38D\
        ,0x24BDCB2FB6C1FB27\
        ,0xCCF89806554279B0\
        ,0x6AAEBCBC9FE41521\
        ,0x3662718AD9534060\
        ,0xD83D7A2E9372370B\
        ,0x710720FC74131841\
        ,0xC79D33248B09487B\
        ,0x98318536F3639EC8\
        ,0x40009B1219E3D32A\
        ,0x523687EE8FFF1F90\
        ,0xA6F123DE0C53997B\
        ,0xD25E1B3204C96CD1\
        ,0xDF888BFBD4390D17\
        ,0x59014905DEE99C30\
        ,0x5A392B13D4ACC983\
        ,0x113975B517A4FFDD\
        ,0xD7135199CC4D22F8\
        ,0x11260623F6456F19\
        ,0x141DEDB7635EA914\
        ,0xB0B75915ED9CCB42\
        ,0x2E69A336D7098A47\
        ,0xB94A269B91842E3\
        ,0xC7DCEB17BD92FE08\
        ,0x9A428C572046EE4A\
        ,0xCFE5F13CC1725885\
        ,0xD549B61E355DA2FB\
        ,0x21EAD913BB59D98A\
        ,0x81BD0273392E9F2A\
        ,0x64BC60DBCA1FF8E9\
        ,0x314A66385C04A935\
        ,0xB251BB2981DD11F0\
        ,0x53CEDDC7BA542A29\
        ,0xD080C2311D95DDBA\
        ,0x1F3845AD9AB7D91E\
        ,0xF321E5953990CAE\
        ,0x33AC6505AB7026CC\
        ,0x30F272974621EA0A\
        ,0x5FADDC2EA89DF681\
        ,0x9C41476AD41256A9\
        ,0xBF4F0B75144D69F6\
        ,0x2867973B3431D699\
        ,0xC01861FB28A1E859\
      }
#define SGX_MCHECK_SERVER_RSEK_EXP 0x10001

#define SGX_MCHECK_SERVER_SIGNATURE \
     { \
         0xE0FD8D87C3EB078\
        ,0x6C8767CC8A7D0DCF\
        ,0x9A3AE4B9B1436597\
        ,0x9DC26B8E35D9B4DC\
        ,0xF2AEAEE51D0CDC27\
        ,0x4B0AB935279A2C81\
        ,0x16601D155BBA7956\
        ,0x7AE9E766D76B7\
        ,0x40907CC6DE564AD3\
        ,0x1034972B21BAC8AC\
        ,0x1645DF710E6AC18A\
        ,0x54A50BB183E32F87\
        ,0x30030F7C0FD6005A\
        ,0x42E42F642F59AD2D\
        ,0x6584B2B27050E258\
        ,0x58535F029BE82A8C\
        ,0x45505EBBFAF20EEE\
        ,0x3ACD776D928771B3\
        ,0x4AEA3C39134BC6EE\
        ,0x35B41C423B25BABF\
        ,0x9A72012DBED8BF1E\
        ,0x50C07D858E4E0374\
        ,0xD2CCB2D252F662F0\
        ,0x26F3CA1927A0E4F4\
        ,0x135B334763266E04\
        ,0xBAE130B1DC1982C3\
        ,0x23E0A9034B537BF9\
        ,0x34FF383B9D14E9CA\
        ,0x71FAB815DB94127C\
        ,0xCD4B483DD7088754\
        ,0xA4AF26F482D59A77\
        ,0xD0A10A2913BBAEBC\
        ,0xC05BC3C6453DE885\
        ,0xE277FBB8A39DE7F3\
        ,0xBB70E93D1B24FE6E\
        ,0x8E3DFB6E51039B39\
        ,0x842B17D08A501219\
        ,0x28737CF0D9C498FC\
        ,0x1E3070FDA4D251EA\
        ,0x7605DB151FFDE715\
        ,0xA38BB55A6AC1435\
        ,0x4DE37643B0AC9F34\
        ,0x53A9D0FCA4DE67F\
        ,0x65C34DC42B63EA8E\
        ,0xA4041A31A8AAAED2\
        ,0xF1401C26A06CD7A1\
        ,0x5D19FC52C18CEAE2\
        ,0x13193555947F291C\
    }

#define SGX_SERVER_ID \
{\
  0x31, 0xa1, 0x2a, 0xfe, 0x07, 0x20, 0x4e, 0xbc, \
  0xb6, 0x4e, 0xc4, 0xb3, 0xc7, 0xf8, 0xbc, 0x0f, \
  0xa8, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x0f, 0xe4, 0x50, 0xdc, 0xfd, 0x28, 0x18, 0x26, \
  0x37, 0x6c, 0xe0, 0x5f, 0xde, 0x2c, 0xae, 0x99, \
  0x45, 0xdf, 0xf9, 0x5c, 0x9b, 0x3b, 0x32, 0x87, \
  0x9b, 0x98, 0xb2, 0x50, 0x85, 0x56, 0x4a, 0x88, \
  0x71, 0xd5, 0x42, 0xe3, 0x89, 0x3c, 0x9f, 0x6f, \
  0x88, 0xef, 0x99, 0x9a, 0x79, 0x69, 0xea, 0xcc, \
  0x38, 0xe1, 0x56, 0xd8, 0x23, 0x3e, 0x64, 0x79, \
  0x99, 0x7f, 0x6d, 0xaa, 0x35, 0x53, 0xd9, 0x02, \
  0xbb, 0x6a, 0x7f, 0xd6, 0xe6, 0xdb, 0x21, 0xc4, \
  0x3c, 0x13, 0x99, 0x3c, 0x91, 0xc2, 0xeb, 0x95, \
  0xb6, 0x65, 0x4b, 0x70, 0xd3, 0xe1, 0x60, 0x2d, \
  0x5c, 0x23, 0x6f, 0x9c, 0x5e, 0x82, 0x09, 0xa6, \
  0xa9, 0x92, 0x3f, 0x49, 0x62, 0x8e, 0xb7, 0xba, \
  0x93, 0x49, 0x13, 0xcc, 0xee, 0x4a, 0xe3, 0xdf, \
  0x1c, 0x9c, 0xe1, 0x1d, 0x0d, 0xac, 0x1c, 0x2e, \
  0x6e, 0xbb, 0xeb, 0x7b, 0x03, 0x6e, 0x12, 0x88, \
  0xad, 0x98, 0xd0, 0xaa, 0xc4, 0x4e, 0x5d, 0xbe, \
  0x3e, 0x01, 0xea, 0x40, 0xeb, 0x73, 0x01, 0xc5, \
  0x13, 0xc3, 0x88, 0xd7, 0xe8, 0x7b, 0x66, 0x30, \
  0xfc, 0xee, 0x23, 0xdc, 0xcc, 0x28, 0xe5, 0x46, \
  0x6a, 0x36, 0x69, 0x13, 0x7e, 0x79, 0x02, 0x1e, \
  0x38, 0x6d, 0xb7, 0x55, 0x69, 0x60, 0x6a, 0x48, \
  0x1a, 0xc8, 0x1b, 0xcd, 0x03, 0xfd, 0xc3, 0x0a, \
  0x14, 0x2b, 0xce, 0x8c, 0xca, 0x27, 0x4d, 0xc0, \
  0x44, 0xf2, 0xb4, 0x0d, 0xc9, 0xab, 0xae, 0xf9, \
  0x52, 0xbb, 0x4f, 0x0d, 0x01, 0x05, 0x8d, 0x59, \
  0x0d, 0x69, 0x50, 0xcd, 0x56, 0xbd, 0x03, 0x6c, \
  0x73, 0x85, 0x27, 0x27, 0x89, 0xe3, 0x8d, 0xe9, \
  0xb7, 0x30, 0x2f, 0xaf, 0xd5, 0x51, 0x42, 0x48, \
  0xde, 0x83, 0xc1, 0x8a, 0xed, 0xc9, 0xfb, 0x3c, \
  0x4a, 0x60, 0x75, 0x4b, 0x41, 0xd8, 0x03, 0x8b, \
  0xe2, 0xc8, 0x5d, 0x9e, 0x10, 0x97, 0x42, 0x45, \
  0x0b, 0x76, 0x98, 0x9f, 0xf2, 0x62, 0xcb, 0x0a, \
  0x7a, 0x97, 0x95, 0x46, 0x01, 0x8c, 0x7c, 0x76, \
  0x10, 0x9e, 0xc6, 0xdd, 0x79, 0x65, 0xee, 0xad, \
  0x46, 0x2b, 0xbd, 0x1e, 0xdf, 0x61, 0x24, 0xe4, \
  0xa7, 0xda, 0x00, 0xde, 0x49, 0x74, 0x08, 0xe4, \
  0x48, 0x69, 0x49, 0x61, 0x38, 0xcc, 0x13, 0x83, \
  0xf2, 0xca, 0xef, 0x7b, 0x6b, 0xf4, 0x56, 0xc1, \
  0xf4, 0x94, 0xc2, 0xb5, 0x39, 0xe1, 0x74, 0x1d, \
  0x90, 0x45, 0x15, 0x41, 0x48, 0x16, 0xa5, 0x09, \
  0x6e, 0x96, 0x35, 0x0b, 0x5d, 0xec, 0xc8, 0x4c, \
  0xdb, 0x9c, 0x29, 0xfe, 0x6f, 0xfa, 0x7a, 0x19, \
  0x82, 0xb5, 0x5f, 0xe6, 0xfb, 0x25, 0x8e, 0x18, \
  0xf0, 0x3c, 0x17, 0x24, 0xb2, 0x96, 0xaa, 0xb4, \
  0x46, 0xa9, 0xf4, 0xe1, 0x0d, 0xe1, 0xd4, 0x94, \
  0x85, 0xf4, 0x36, 0x0d, 0xfa, 0xde, 0x7b, 0xc6, \
  0xab, 0xf7, 0x0c, 0x0f, 0x1e, 0xd5, 0x9c, 0x15, \
  0xab, 0x30, 0xfa, 0xce, 0x19, 0xe3, 0xcf, 0xee, \
  0x43, 0xe1, 0xbe, 0xe2, 0xf2, 0x80, 0x00, 0x95, \
  0x57, 0x6a, 0xf5, 0x2b, 0x46, 0x34, 0x4e, 0xa4, \
  0xe7, 0xe0, 0x8c, 0x2b, 0xc1, 0xd5, 0x68, 0xdd, \
  0x01, 0x00, 0x01, 0x00, 0x53, 0x86, 0x5e, 0x87, \
  0x4b, 0x6c, 0x87, 0xac, 0x13, 0xa5, 0xc6, 0xfc, \
  0x78, 0x92, 0xd4, 0xd7, 0xf4, 0xeb, 0x09, 0x82, \
  0xd6, 0x26, 0xbb, 0xd5, 0xc6, 0x8b, 0xc8, 0xb3, \
  0xa5, 0x25, 0xa7, 0x98, 0x81, 0x5d, 0xe4, 0x07, \
  0x07, 0xd7, 0xbf, 0x43, 0x25, 0xfc, 0x10, 0x54, \
  0xe1, 0x21, 0x5b, 0x35, 0xae, 0xa8, 0xd0, 0x4e, \
  0x3a, 0xb6, 0x95, 0xd1, 0x23, 0xfb, 0x11, 0xa2, \
  0x15, 0xcd, 0x3b, 0x17, 0x40, 0x20, 0xb3, 0xdc, \
  0xab, 0xa8, 0x69, 0x92, 0x48, 0x77, 0x2e, 0xd5, \
  0x8c, 0xcd, 0x2f, 0x59, 0x15, 0x20, 0xbc, 0x74, \
  0xa6, 0xb3, 0xd7, 0x23, 0x46, 0xfb, 0x10, 0x3b, \
  0x1c, 0xe5, 0x4b, 0xc3, 0x31, 0xd2, 0x9c, 0xa6, \
  0x1d, 0x10, 0xa5, 0xe0, 0x27, 0x89, 0x36, 0x43, \
  0xe4, 0x1e, 0xd8, 0x23, 0x36, 0xc5, 0x5c, 0x3c, \
  0x73, 0x2f, 0x10, 0xfb, 0x4f, 0x1c, 0xd7, 0xd5, \
  0x6f, 0x25, 0x40, 0xf0, 0xa9, 0x74, 0xe9, 0xd4, \
  0xc1, 0xec, 0x3f, 0x99, 0x9b, 0x18, 0xfe, 0x61, \
  0x82, 0x37, 0xb6, 0xe6, 0x16, 0x33, 0x9f, 0x7f, \
  0x52, 0xcf, 0xa6, 0x83, 0xe9, 0xf9, 0x8b, 0xb6, \
  0x39, 0x42, 0xdb, 0x5b, 0xf5, 0x05, 0x11, 0x74, \
  0xa2, 0xde, 0x73, 0x0e, 0xcc, 0xba, 0x31, 0xbb, \
  0x7e, 0xfa, 0xfa, 0x5e, 0x75, 0x8c, 0x28, 0x33, \
  0x4b, 0xcc, 0x8a, 0x99, 0x07, 0x89, 0xa4, 0xd1, \
  0xf6, 0x7f, 0x92, 0xa2, 0x13, 0x49, 0x5e, 0x60, \
  0x7b, 0xf7, 0x60, 0xa3, 0x56, 0xa9, 0x37, 0xd5, \
  0xf3, 0xd7, 0xa9, 0x3d, 0x17, 0x8c, 0x93, 0xac, \
  0xa4, 0xfd, 0x02, 0x4d, 0x0e, 0x69, 0xdb, 0xf2, \
  0xab, 0x0a, 0xe8, 0x5d, 0x3b, 0xf7, 0x38, 0xe8, \
  0x29, 0xbd, 0x7d, 0xa6, 0xd0, 0xa2, 0xce, 0x59, \
  0x5c, 0x8a, 0x10, 0xe1, 0x86, 0x9b, 0x42, 0x32, \
  0xf6, 0x4d, 0xcc, 0x1b, 0xaa, 0x31, 0xde, 0x86, \
  0x4b, 0x18, 0xbc, 0x1e, 0x27, 0x13, 0x5e, 0x84, \
  0x8e, 0xf3, 0xeb, 0x24, 0xca, 0x35, 0xcd, 0xf8, \
  0xe2, 0xc5, 0x5c, 0x37, 0x30, 0x36, 0x1a, 0xba, \
  0x50, 0x65, 0xa2, 0xeb, 0x20, 0xf5, 0xeb, 0xc2, \
  0xf0, 0xc7, 0xfd, 0xd4, 0x7a, 0xa2, 0x1c, 0x31, \
  0xbb, 0xc6, 0x9e, 0x82, 0xcb, 0x09, 0x49, 0x5a, \
  0xa0, 0xdc, 0xff, 0xb3, 0x83, 0x1e, 0x5f, 0xa7, \
  0x9f, 0x35, 0xd6, 0x6a, 0xd9, 0x6a, 0xce, 0x75, \
  0x7e, 0x75, 0xed, 0x06, 0x86, 0x1b, 0xed, 0x0b, \
  0x35, 0xa3, 0x31, 0x35, 0x1b, 0x2f, 0xea, 0x04, \
  0x23, 0x0b, 0x0d, 0xf1, 0xd5, 0x61, 0x56, 0xea, \
  0xa1, 0x9a, 0xd0, 0x0c, 0xb4, 0xb2, 0x8b, 0xb7, \
  0x16, 0x93, 0xfd, 0x0b, 0xed, 0x35, 0x76, 0x7e, \
  0x3e, 0xf9, 0x40, 0x37, 0x81, 0x90, 0xce, 0x58, \
  0x26, 0x39, 0x03, 0x3f, 0xab, 0x28, 0x49, 0xee, \
  0x70, 0xeb, 0x9e, 0x97, 0x24, 0x6c, 0xab, 0x79, \
  0x38, 0xbb, 0x7e, 0x93, 0x01, 0x00, 0x01, 0x00, \
  0x44, 0x91, 0x62, 0xe4, 0x3c, 0xe4, 0xb9, 0x57, \
  0xb8, 0x70, 0x95, 0xe4, 0x84, 0x56, 0x2b, 0xfb, \
  0xa1, 0x8e, 0x80, 0x51, 0xcb, 0x47, 0xf4, 0xf2, \
  0x89, 0xcd, 0x67, 0xf1, 0x66, 0x8a, 0xbd, 0x66, \
  0x99, 0xef, 0x4b, 0xa9, 0x20, 0xd4, 0x8c, 0xd9, \
  0xf2, 0x1b, 0xfa, 0x94, 0x49, 0x90, 0x69, 0x52, \
  0x02, 0x92, 0xda, 0x8a, 0xe0, 0xe7, 0x24, 0x0f, \
  0x7f, 0xca, 0x73, 0x86, 0xf4, 0x0e, 0x45, 0x24, \
  0x13, 0xdc, 0xbe, 0xf8, 0x50, 0xee, 0x08, 0xf9, \
  0xb6, 0xab, 0x86, 0x93, 0x29, 0x5b, 0x0b, 0x2e, \
  0x12, 0x99, 0x8c, 0x77, 0x4b, 0x75, 0xa4, 0xc5, \
  0xa8, 0x4d, 0x56, 0xd0, 0xb6, 0x09, 0x9d, 0xef, \
  0x3e, 0x01, 0x92, 0x91, 0xa9, 0x5e, 0xaa, 0x7e, \
  0x94, 0x50, 0xb3, 0xcc, 0x19, 0xe0, 0x28, 0x6a, \
  0xd9, 0x2f, 0x2c, 0xbf, 0x40, 0xda, 0x8c, 0x12, \
  0x75, 0x4f, 0x95, 0x4f, 0x16, 0x2f, 0xed, 0xcc, \
  0xc7, 0x3f, 0x63, 0x75, 0xf9, 0x06, 0xa4, 0x7a, \
  0x3f, 0xc9, 0xf4, 0x7c, 0x2e, 0x7d, 0x02, 0xe5, \
  0xa0, 0x2c, 0xab, 0x3c, 0x5e, 0xd0, 0x20, 0xcb, \
  0x99, 0xd4, 0x41, 0x03, 0xab, 0x27, 0xf5, 0xa1, \
  0x70, 0xa1, 0xea, 0x73, 0x82, 0xd3, 0x04, 0xbd, \
  0xa6, 0x98, 0x5f, 0x21, 0x74, 0xee, 0xa0, 0xa2, \
  0x58, 0xdf, 0xd6, 0x82, 0x55, 0x5d, 0x32, 0x42, \
  0x0d, 0x5c, 0x90, 0xde, 0x3b, 0xcd, 0x68, 0xe9, \
  0x4f, 0xe6, 0xf7, 0x47, 0x4d, 0xaf, 0xfb, 0xd8, \
  0xad, 0xb5, 0x9f, 0xae, 0x2e, 0xd1, 0xb7, 0x59, \
  0x2e, 0x22, 0x43, 0xab, 0xfe, 0x44, 0x9a, 0xea, \
  0x27, 0x48, 0x9b, 0x1f, 0x5e, 0x3c, 0xf1, 0x54, \
  0xe7, 0xe7, 0x23, 0x85, 0x50, 0x7b, 0xa8, 0x26, \
  0xb4, 0x80, 0x34, 0xbf, 0x0f, 0xa3, 0xfa, 0xe8, \
  0x1f, 0x0f, 0x9e, 0x93, 0xad, 0x33, 0x71, 0x5a, \
  0x3d, 0xa6, 0x80, 0xdd, 0x0c, 0x81, 0x3f, 0x83, \
  0x56, 0x90, 0x78, 0x86, 0xb5, 0x04, 0x2a, 0x27, \
  0xb6, 0x56, 0x10, 0x6e, 0xa6, 0xe3, 0xe1, 0xb2, \
  0xad, 0x6b, 0x88, 0x3d, 0xa6, 0xa2, 0x2f, 0x20, \
  0x49, 0xc9, 0x3f, 0xfe, 0x1d, 0xd8, 0x68, 0xc1, \
  0xa2, 0xbc, 0xff, 0x65, 0x7f, 0x90, 0xa6, 0x96, \
  0x32, 0xe4, 0x1a, 0x51, 0x7f, 0xd3, 0x66, 0xb3, \
  0xa4, 0x95, 0x34, 0x0d, 0xc1, 0xd7, 0x8c, 0x75, \
  0x76, 0xd9, 0x1d, 0xed, 0x0b, 0x41, 0x40, 0x95, \
  0x75, 0x8f, 0x72, 0x6a, 0xf0, 0x8f, 0xb1, 0x63, \
  0xd2, 0xa8, 0x39, 0x03, 0x17, 0x71, 0x4d, 0x80, \
  0x17, 0x01, 0xef, 0x1e, 0x6b, 0x69, 0xaa, 0x73, \
  0x49, 0x24, 0xa6, 0xc1, 0xab, 0x97, 0x35, 0x96, \
  0xf1, 0xcb, 0x66, 0x30, 0x4d, 0x64, 0x15, 0xd5, \
  0x2a, 0x6c, 0x66, 0x34, 0x3d, 0x17, 0x17, 0x4e, \
  0xc2, 0xa0, 0x8f, 0x77, 0x86, 0x14, 0x20, 0x17, \
  0xfb, 0xed, 0x41, 0xed, 0x24, 0x96, 0xc8, 0x68 \
}
//
//SGX_TODO: Remove all hardcoded registration server data when
//MCHECK populate it by default and after PowerOn [END]
//


typedef EFI_STATUS
(EFIAPI *GET_REGISTRATION_CONFIG) (
  SGX_REGISTRATION_CONFIG *SgxRegistrationConfig
);

typedef EFI_STATUS
(EFIAPI *SET_REGISTRATION_CONFIG) (
  SGX_REGISTRATION_CONFIG *SgxRegistrationConfig
);

typedef EFI_STATUS
(EFIAPI *GET_REGISTRATION_REQUEST) (
  SGX_REGISTRATION_REQUEST *SgxRegistrationRequest
);

typedef EFI_STATUS
(EFIAPI *GET_REGISTRATION_PACKAGE_INFO) (
  SGX_REGISTRATION_PACKAGE_INFO *SgxRegistrationPackageInfo
);

typedef EFI_STATUS
(EFIAPI *SET_REGISTRATION_RESPONSE) (
  SGX_REGISTRATION_RESPONSE *SgxRegistrationResponse
);

typedef EFI_STATUS
(EFIAPI *GET_REGISTRATION_STATUS) (
  SGX_REGISTRATION_STATUS *SgxRegistrationStatus
);

typedef EFI_STATUS
(EFIAPI *SET_REGISTRATION_STATUS) (
  SGX_REGISTRATION_STATUS *SgxRegistrationStatus
);



typedef enum {
  SgxRequestNone,
  SgxRequestPlatformManifest,
  SgxRequestAddRequest
} SGX_REGISTRATION_SERVER_REQUEST_TYPE;

typedef struct {
  // Registration Request
  UINT64                               RegistrationRequestSize;
  SGX_REGISTRATION_SERVER_REQUEST_TYPE RegistrationRequestType;
  SGX_REGISTRATION_SERVER_REQUEST_TYPE PreviousRegistrationRequestType;
  BOOLEAN                              PreviousRequestPending;

  // Registration Config
  BOOLEAN                   RegistrationConfigHashPresent;
  UINT8                     RegistrationConfigHash[SHA256_DIGEST_SIZE]; // SHA256 hash

  BOOLEAN                   SwAgentFirstBindingComplete;
} SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE;

typedef struct {
  SGX_UEFIFW_NONVOLATILVE_REGISTRATION_STATE NonVolatile;
  // Variables exposure states
  BOOLEAN                   SgxTcbRecoveryPending;
  BOOLEAN                   SgxAddPackagePending;
  // Helpers - Response Variable
  BOOLEAN                   RegistrationResponseVariablePresent;
  // Helpers - Status Variable
  BOOLEAN                   RegistrationStatusVariablePresent;
  BOOLEAN                   RequestPending;
  BOOLEAN                   PackageInfoPending;
  // Helpers - Config Variable
  BOOLEAN                   RegistrationConfigVariablePresent;
  BOOLEAN                   ForceFirstBindingOnRegConfChanged;
} SGX_UEFIFW_REGISTRATION_STATE;

typedef struct{
  UINT64                    Prids_0;
  UINT64                    Prids_1;
  // Socket is new in AddPackage Flow
  BOOLEAN                   AddPackageNew;
} SGX_UEFIFW_SOCKET;

typedef struct {
  UINT64              PrmrrBase[MAX_PRMRR_COUNT];
  UINT64              MaxSadRuleSize;
  UINT64              ValidPrmrrBitMap;
  UINT64              PrmrrSize;
  BOOLEAN             IsSgxCapable;
  UINT8               CrDimmsPresent;
  UINT8               IsHwCongifSupportedBySgx;
  UINT8               ClusterValidForSgx [MAX_SOCKET][MAX_IMC];
  UINT8               SocketCount;
  UINT8               PrmrrCount;
  UINT8               MaxSadRuleImcWays;
  UINT8               TargetPrmrrCount;
  UINT8               PrmrrCountPerPackage;
  SGX_ERROR_CODE_ENUM ErrorCode;
} SGX_PREMEM_INIT_HOB;

//
// SGX UEFI FW Internal HOB
//
typedef struct {
  SGX_PREMEM_INIT_HOB           SgxPreMemInitHob;
  SGX_UEFIFW_SOCKET             Socket[SGX_MCHECK_MAX_PACKAGE];
  UINT64                        Prids_QualifiedForAddPkg[SGX_MCHECK_MAX_PACKAGE];
  UINT8                         PrResetSvn[SGX_MCHECK_MAX_PACKAGE];
  UINT8                         BootingSvn;
  BOOLEAN                       SgxFirstBoot;
  SGX_UEFI_BOOT_SCENARIO        SgxBootScenario;
  UINT32                        ErrorCode;
  SGX_UEFIFW_KEY_BLOB           UefiFwKeyBlobs[SGX_MCHECK_MAX_PACKAGE];
  SGX_UEFIFW_PLATFORM_MEMBERSHIP_CERTIFICATE UefiFwPlatMemCert[SGX_MCHECK_MAX_PACKAGE];
  BOOLEAN                       KeyBlobsExistInNvram;
  BOOLEAN                       KeyBlobsCreatedOrModifiedByMcheck;
  BOOLEAN                       KeyBlobsRestoredFromPackageInfo;
  UINT8                         SgxLeWr;
  // Registration state
  SGX_UEFIFW_REGISTRATION_STATE RegistrationState;
} SGX_INIT_DATA_HOB;

//
// ACM HEADER
//
// Intel (R) Trusted Execution Technology (Intel (R) Txt) November 2017 Revision 015
// Appendix A Intel(R) TXT Execution Technology Authenticated Code Modules
// https://www.intel.com/content/dam/www/public/us/en/documents/guides/intel-txt-software-development-guide.pdf
//
typedef struct _ACM_HEADER_V30 {
  UINT16 ModuleType;      ///<  0 2 Module type
  UINT16 ModuleSubType;   ///<  2 2 Module sub-type
  UINT32 HeaderLen;       ///<  4 4 Header length (in multiples of four bytes) (161 for version 0.0)
  UINT32 HeaderVersion;   ///<  8 4 Module format version
  UINT16 ChipsetId;       ///< 12 2 Module release identifier
  UINT16 Flags;           ///< 14 2 Module-specific flags
  UINT32 ModuleVendor;    ///< 16 4 Module vendor identifier
  UINT32 Date;            ///< 20 4 Creation date (BCD format: year.month.day)
  UINT32 Size;            ///< 24 4 Module size (in multiples of four bytes)
  UINT16 TxtSvn;          ///< 28 2 TXT Security Vesrsion Number
  UINT16 SeSvn;           ///< 30 2 Software Guard Extension (Secure Enclaves) Security Version Number
  UINT32 CodeControl;     ///< 32 4 Authenticated code control flags
  UINT32 ErrorEntryPoint; ///< 36 4 Error response entry point offset (bytes)
  UINT32 GdtLimit;        ///< 40 4 GDT limit (defines last byte of GDT)
  UINT32 GdtBasePtr;      ///< 44 4 GDT base pointer offset (bytes)
  UINT32 SegSel;          ///< 48 4 Segment selector initializer
  UINT32 EntryPoint;      ///< 52 4 Authenticated code entry point offset (bytes)
  UINT32 Reserved2[16];   ///< 56 64 Reserved for future extensions
  UINT32 KeySize;         ///< 120 4 Module public key size less the exponent  (in multiples of four bytes - 64 for version 0.0)
  UINT32 ScratchSize;     ///< 124 4 Scratch field size (in multiples of four bytes - 2 * KeySize + 15 for version 0.0)
} ACM_HEADER_V30;

typedef struct _ACM_INFO_TABLE_V30 {
  UINT32 Uuid;            ///<  0 4 UUID of the chipset AC module
  UINT16 ChipsetAcmType;  ///<  4 2 UUID of the chipset AC module
} ACM_INFO_TABLE_V30;

typedef struct {
  //
  // ACM information
  //
  EFI_PHYSICAL_ADDRESS     BiosAcmAddress;
  EFI_PHYSICAL_ADDRESS     StartupAcmAddressInFit;
  EFI_PHYSICAL_ADDRESS     StartupAcmAddress;
  //
  // Misc Policy
  //
  UINT32                   MiscPolicy;
  //
  // BiosOs region. It should be at least 2 for LCP PD.
  //
  UINT32                   BiosOsDataRegionRevision;
  EFI_PHYSICAL_ADDRESS     LcpPolicyDataBase;
  UINT64                   LcpPolicyDataSize;
  //
  // 64-bits TXT Scratch Register
  //
  EFI_PHYSICAL_ADDRESS     TxtScratchAddress;
  //
  // Bios ACM Policy
  //
  UINT32                   BiosAcmPolicy;
  //
  // BiosOsData (SINIT consumption, chipset specific)
  //
  UINT64                   FlagVariable;
  //
  // DPR Lock
  //
  //  UINT32                   DprLock;
  //
  // Clear TPM Aux Index
  //
  UINT8                    TxtClearAuxIndex;
} EFI_PLATFORM_TXT_POLICY_DATA;

#endif // __SGX_INIT_DATA__
