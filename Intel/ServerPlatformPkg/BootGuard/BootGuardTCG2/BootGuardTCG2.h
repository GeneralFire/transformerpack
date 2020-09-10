//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file BootGuardTCG2.h
    TPM Initialization Flow for Boot Guard

**/
//**********************************************************************

#ifndef _BOOT_GUARD_TCG2_H_
#define _BOOT_GUARD_TCG2_H_

//
// Define macros to build data structure signatures from characters.
//

#define IBB_ENTRYPOINT_M  0xFFFFFFC0

#define M_KEYHASH_ADDR      0xFED30400

#define MSR_ACM_CPU_KEY_HASH_0              0x20
#define MSR_ACM_CPU_KEY_HASH_1              0x21
#define MSR_ACM_CPU_KEY_HASH_2              0x22
#define MSR_ACM_CPU_KEY_HASH_3              0x23

// Define the FIT Entry Type
#define FIT_HEADER_TYPE             0x00
#define FIT_MICROCODE_TYPE          0x01
#define FIT_ACM_TYPE                0x02
#define FIT_KM_TYPE                 0x0B
#define FIT_BPM_TYPE                0x0C

//
// BtG related definitions
//
#define MSR_BOOT_GUARD_SACM_INFO                                      0x13A
#define B_BOOT_GUARD_SACM_INFO_NEM_ENABLED                            BIT0
#define V_TPM_PRESENT_MASK                                            0x06
#define V_TPM_PRESENT_NO_TPM                                          0
#define V_TPM_PRESENT_DTPM_12                                         1
#define V_TPM_PRESENT_DTPM_20                                         2
#define V_TPM_PRESENT_PTT                                             3
#define B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS                            BIT3
#define B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT                          BIT5
#define B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT                          BIT6
#define B_BOOT_GUARD_SACM_INFO_CAPABILITY                             BIT32
#define TXT_PUBLIC_BASE                                               0xFED30000
#define R_CPU_BOOT_GUARD_ERRORCODE                                    0x30
#define R_CPU_BOOT_GUARD_BOOTSTATUS                                   0xA0
#define R_CPU_BOOT_GUARD_ACM_STATUS                                   0x328
#define V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS                             0x8000000000000000
#define B_BOOT_GUARD_ACM_ERRORCODE_MASK                               0x00007FF0
#define MMIO_ACM_STATUS         0xFED30328
#define MMIO_ACM_POLICY_STATUS  0xFED30378
#define ACM_STATUS          (*(UINT32*)MMIO_ACM_STATUS)
#define ACM_POLICY_STATUS   (*(UINT32*)MMIO_ACM_POLICY_STATUS)
//
// Manifest definition
//
//#define TPM_ALG_NULL                0x0010
#define TPM_ALG_SHA1                0x0004
#define TPM_ALG_SHA256              0x000B
#define TPM_ALG_SHA384              0x000C
#define TPM_ALG_SM3_256             0x0012
#define SHA1_DIGEST_SIZE            20
#define SHA256_DIGEST_SIZE          32
#define SHA384_DIGEST_SIZE          48
#define SM3_256_DIGEST_SIZE         32
//#define TPM_ALG_RSA                 0x0001

#pragma pack(push, 1)

typedef struct _FIT_ENTRY
{
    UINT64           TblAddress;
    UINT32           TblSIZE;
    UINT16           TblVer;
    UINT8            TblType;
    UINT8            TblChkSum;
} FIT_ENTRY;

typedef struct {
    UINT16      HashAlg;
    UINT16      Size;
    UINT8       HashBuffer[1];  // various size from 'Size'
} HASH_STRUCTURE;

typedef struct {
    UINT16      HashAlg;
    UINT16      Size;
    UINT8       HashBuffer[SHA256_DIGEST_SIZE];
} HASH256_STRUCTURE;

typedef struct {
    UINT64              Usage;
    HASH256_STRUCTURE   Digest;
} KM_HASH256_STRUCTURE;

typedef struct {
    UINT16              Size;
    UINT16              Count;
    HASH256_STRUCTURE   Digest[1];  // various Digest from Count
} HASH256_LIST_STRUCTURE;

#define RSA2048_KEY_BITS  2048
#define RSA2048_KEY_BYTES ((RSA2048_KEY_BITS + 7) / 8)
#define RSA2048_PUBLIC_KEY_EXPONENT_DEFAULT  0x10001 // NOT 0x10001

#define RSA3072_KEY_BITS  3072
#define RSA3072_KEY_BYTES ((RSA3072_KEY_BITS + 7) / 8)

typedef struct {
    UINT8    Version;
    UINT16   KeySize;
    UINT32   Exponent;
    UINT8    Modulus[RSA2048_KEY_BYTES];
} RSA2048_PUBLIC_KEY_STRUCT;

typedef struct {
    UINT8    Version;
    UINT16   KeySize;
    UINT16   HashAlg;
    UINT8    Signature[RSA2048_KEY_BYTES];
} RSASSA2048_SIGNATURE_STRUCT;

typedef struct {
    UINT8                       Version;    // 0x10
    UINT16                      KeyAlg;     // TPM_ALG_RSA
    RSA2048_PUBLIC_KEY_STRUCT   Key;
    UINT16                      SigScheme;  // TPM_ALG_RSASSA
    RSASSA2048_SIGNATURE_STRUCT Signature;
} RSA2048_KEY_SIGNATURE_STRUCT;

typedef struct {
    UINT16      Reserved;       // 0x00
    UINT16      Flags;          // IBB Segment flags, bit 0, 0: This is a Hashed IBB Segment. 1: non-Hashed.
    UINT32      Base;
    UINT32      Size;
} IBB_SEGMENT_ELEMENT;

typedef struct {
    UINT64                  StructureID;
    UINT8                   StructVersion;
    UINT8                   Reserved;
    UINT16                  ElementSize;
} BPM_COMMON_HEADER;

#define BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID  (*(UINT64 *)"__ACBP__")
#define BOOT_POLICY_MANIFEST_STRUCTURE_VERSION              0x21
#define BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_VERSION       0x20
typedef struct {
    UINT64          StructureID;            // BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID
    UINT8           StructVersion;          // BOOT_POLICY_MANIFEST_STRUCTURE_VERSION
    UINT8           HdrStructVersion;       // BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_VERSION
    UINT16          HdrSize;
    UINT16          KeySignatureOffset;
    UINT8           BpmRevision;
    UINT8           BPMSVN;
    UINT8           ACMSVN_Auth;
    UINT8           Reserved;
    UINT16          NEMDataStack;
} BOOT_POLICY_MANIFEST_HEADER;

#define BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__IBBS__")
#define BOOT_POLICY_MANIFEST_IBB_ELEMENT_VERSION       0x20
typedef struct {
    UINT64          StructureID;        // BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID
    UINT8           StructVersion;      // BOOT_POLICY_MANIFEST_IBB_ELEMENT_VERSION
    UINT8           Reserved1;
    UINT16          ElementSize;
    UINT8           Reserved2;
    UINT8           SetNumber;
    UINT8           Reserved3;
    UINT8           PBETValue;
    UINT32          Flags;              // IBB Segment Element Flags,
                                        // Bit 1, 0: TPM2 Startup from Locality 0. 1: Locality 3
                                        // Bit 2, 0: Do not extend Authority PCR[7]. 1: PCR[7] Authority Measurement.
    UINT64          IBB_MCHBAR;
    UINT64          Vtd_BAR;
    UINT32          DmaProtBase0;
    UINT32          DmaProtLimit0;
    UINT64          DmaProtBase1;
    UINT64          DmaProtLimit1;
    UINT16          PostIbbHashHashAlg;     // Be careful, must be TPM_ALG_NULL
    UINT16          PostIbbHashSize;        // Be careful, must be 0
    UINT32          IbbEntryPoint;
    HASH256_LIST_STRUCTURE  DigestList;     // (?) variable of hash digest
    UINT16          PostObbHashHashAlg;     // Be careful, must be TPM_ALG_NULL
    UINT16          PostObbHashSize;        // Be careful, must be 0
    UINT8           Reserved4[3];
    UINT8           SegmentCount;           // 1 ~ 8
//    IBB_SEGMENT_ELEMENT IBBSegment[1];      // (?) variable of IBB_SEGMENT_ELEMENT
} IBB_ELEMENT;

#define BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__PMSG__")
#define BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_VERSION       0x20
typedef struct {
    UINT64                  StructureID;            // Must be BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID
    UINT8                   StructVersion;          // BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_VERSION
    UINT8                   Reserved[3];
    RSA2048_KEY_SIGNATURE_STRUCT    KeySignature;
} BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT;

#define TXT_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__TXTS__")
#define TXT_ELEMENT_VERSION       0x20
typedef struct {
    UINT64                  StructureID;        // TXT_ELEMENT_STRUCTURE_ID
    UINT8                   StructVersion;      // TXT_ELEMENT_VERSION
    UINT8                   Reserved;
    UINT16                  ElementSize;
    UINT8                   Reserved1;
    UINT8                   SetNumber;
    UINT8                   Reserved2[2];
    UINT32                  ControlFlags;
    UINT16                  PowerDownInterval;
    UINT8                   PttCmosOffset0;     // Default: 0xFE(126)
    UINT8                   PttCmosOffset1;     // Default: 0xFF(127)
    UINT16                  AcpiBaseOffset;     // Default: 0x400
    UINT8                   Reserved3[2];
    UINT32                  PwrmBaseOffset;     // Default: 0xFE000000
    UINT16                  DigestSize;         // careful must be 0x04
    UINT16                  DigestCount;        // careful must be 0x00
    UINT8                   Reserved4[3];
    UINT8                   SegmentCount;       // Must be 0x00
} TXT_ELEMENT_STRAUCTURE;

#define PLATFORM_CONFIGURATION_DATA_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__PCDS__")
#define PLATFORM_CONFIGURATION_DATA_ELEMENT_VERSION       0x10
typedef struct {
    UINT64                      StructureID;        // PLATFORM_CONFIGURATION_DATA_ELEMENT_STRUCTURE_ID
    UINT8                       StructVersion;      // PLATFORM_CONFIGURATION_DATA_ELEMENT_VERSION
    UINT8                       Reserved1;
    UINT16                      ElementSize;
    UINT8                       Reserved2[2];
    UINT16                      SizeofData;
//    UINT8                       Data[1];          // (?) various sizeofData UINT8
} PLATFORM_CONFIGURATION_DATA_ELEMENT;

typedef struct {
    BOOT_POLICY_MANIFEST_HEADER                     BPMH;
    IBB_ELEMENT                                     SE[1];
//    TXT_ELEMENT_STRAUCTURE                          TXTE;
//    PLATFORM_CONFIGURATION_DATA_ELEMENT             PCDE;
//    PLATFORM_MANUFACT_ELEM                          PME;    // Current non-defined
//    BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT          PMSE;
} BpmStruct;

#define KEY_MANIFEST_STRUCTURE_ID  (*(UINT64 *)"__KEYM__")
#define KEY_MANIFEST_VERSION       0x21
typedef struct {
    UINT64                  StructureID;            // KEY_MANIFEST_STRUCTURE_ID
    UINT8                   StructVersion;          // KEY_MANIFEST_VERSION
    UINT8                   Reserved[3];
    UINT16                  KeySignatureOffset;
    UINT8                   Reserved2[3];
    UINT8                   KeyManifestRevision;    // Define by Platform Manufacture
    UINT8                   KMSVN;
    UINT8                   KeyManifestID;
    UINT16                  KmPubKeyHashAlg;        // careful, current support TPM_ALG_SHA256
    UINT16                  KeyCount;
    KM_HASH256_STRUCTURE    KmHash[1];              // careful, various KeyCount
    RSA2048_KEY_SIGNATURE_STRUCT    KeyManifestSignature;
} KEY_MANIFEST_STRAUCTURE;

#define MAX_PCR0_DATA_LEN   (sizeof(UINT64) + sizeof(UINT16) + RSA3072_KEY_BYTES * 3 + 64)

typedef struct {
    UINT64          AcmPolicyStatus;
    UINT16          AcmHeaderSvn;
//    UINT8           AcmSignature[RSA2048_KEY_BYTES];
//    UINT8           KmSignature[RSA2048_KEY_BYTES];
//    UINT8           BpmSignature[RSA2048_KEY_BYTES];
//    UINT8           IbbDigest[];        // various hash buffer
} PCR0_DATA;

#define MAX_PCR7_DATA_LEN   (sizeof(UINT64) + sizeof(UINT16) + SHA256_DIGEST_SIZE * 2 + 64)
typedef struct {
    UINT64          AcmPolicyStatus;
    UINT16          AcmHeaderSvn;
    UINT8           AcmKeyHash[32];
    UINT8           BpKeyHash[32];
    UINT8           BpmPubkeyHashBuffer[32];
}PCR7_DATA;

//
// Efi Startup Locality Event Data
//
typedef struct {
  UINT8   Signature[16];
  UINT8   StartupLocality;
} TCG_EFI_STARTUP_LOCALITY_EVENT;

// For TPM20 Define +>
typedef struct _tdTpm2EventHeader {
    UINT32      HeaderSize;
    UINT16      HeaderVersion;
    UINT32      PCRIndex;
    UINT32      EventType;
} tdTpm2EventHeader;

#pragma pack(pop)

// For TCG Protocol Define

// For TPM12 Define +>

#define PEI_TCG_PPI_GUID  \
    {0x177d39d2, 0x43b8, 0x40c8, 0x9a, 0xe1, 0x3c, 0x51, 0x98, 0xd6, 0x94, 0x1e}

typedef
EFI_STATUS
(EFIAPI * PEI_TCG_HASH_LOG_EXTEND_EVENT)(
    IN VOID             *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN UINT8            *HashData,
    IN UINT32           HashDataLen,
    IN UINT8            *NewEvent,
    OUT UINT32          *EventNum
);

typedef struct _PEI_TCG_PPI
{
    PEI_TCG_HASH_LOG_EXTEND_EVENT   TCGHashLogExtendEvent;
    VOID                            *TCGLogEvent;
    VOID                            *TCGPassThroughToTpm;
} PEI_TCG_PPI;

typedef struct _tdTpm2Event {
    UINT32              Size;
    tdTpm2EventHeader   Header;
    UINT8               Event[1];
} tdTpm2Event;

typedef
EFI_STATUS
(EFIAPI * AMI_TREE_HASH_LOG_EXTEND_EVENT) (
    VOID                    *This,
    UINT64                  Flags,
    EFI_PHYSICAL_ADDRESS    DataToHash,
    UINT64                  DataToHashLen,
    tdTpm2Event             *Event
);

typedef struct _AMI_TREE_PPI {
    VOID*                           GetCapability;
    VOID*                           GetEventLog;
    AMI_TREE_HASH_LOG_EXTEND_EVENT  HashLogExtendEvent;
    VOID*                           SubmitCommand;
} AMI_TREE_PPI;

#define TREE_BOOT_HASH_ALG_SHA1   0x00000001
#define TREE_BOOT_HASH_ALG_SHA256 0x00000002
#define TREE_BOOT_HASH_ALG_SHA384 0x00000004
#define TREE_BOOT_HASH_ALG_SHA512 0x00000008
#define TREE_BOOT_HASH_ALG_SM3    0x00000010

#define TCG2_PROTOCOL_SPEC_VERSION_1_0  0x01
#define TCG2_PROTOCOL_SPEC_VERSION_1_X  0x02


#ifndef EFI_TCG_LOG_HOB_GUID
#define EFI_TCG_LOG_HOB_GUID \
    {0x5f7d4e0e, 0x3d6d, 0x42bc, 0xa9, 0x42, 0xe, 0x91, 0xe8, 0x3e, 0x3c, 0x31}
#endif

#ifndef EFI_TCG_TREE_LOG_HOB_GUID
#define EFI_TCG_TREE_LOG_HOB_GUID \
    {0x711e7d64, 0x1131, 0x494b, 0xa2, 0xd1, 0x84, 0x80, 0x6c, 0xd7, 0x2d, 0x53}
#endif

#ifndef TPM20_HOB_GUID
#define TPM20_HOB_GUID \
  { \
    0x7cea4f7e, 0x2052, 0x46ed, 0xbe, 0xfd, 0xe2, 0x2a, 0x44, 0xdc, 0x65, 0xe7 \
  }
#endif

#ifndef AMI_TREE_PPI_GUID
#define AMI_TREE_PPI_GUID\
    {0x86f5680a, 0x155c, 0x4bc8,  { 0xac, 0x77, 0x57, 0x38, 0x48, 0xe2, 0xad, 0x3d } }
#endif



#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
