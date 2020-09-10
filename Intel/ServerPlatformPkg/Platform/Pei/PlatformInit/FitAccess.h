/** @file
  This file contains functions for accessing FIT and its entries

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification
**/
// APTIOV_SERVER_OVERRIDE_RC_START: This is defined in SecurityPkg which is not consumed
//#include <Ppi/FirmwareVolumeInfoStoredHashFv.h>
// APTIOV_SERVER_OVERRIDE_RC_START: This is defined in SecurityPkg which is not consumed
//
// FIT Entry type definitions
//
#define FIT_TYPE_00_HEADER                  0x00
#define FIT_TYPE_01_MICROCODE               0x01
#define FIT_TYPE_02_STARTUP_ACM             0x02
#define FIT_TYPE_07_BIOS_STARTUP_MODULE     0x07
#define FIT_TYPE_08_TPM_POLICY              0x08
#define FIT_TYPE_09_BIOS_POLICY             0x09
#define FIT_TYPE_0A_TXT_POLICY              0x0A
#define FIT_TYPE_0B_KEY_MANIFEST            0x0B
#define FIT_TYPE_0C_BOOT_POLICY_MANIFEST    0x0C
#define FIT_TYPE_10_CSE_SECURE_BOOT         0x10
#define FIT_TYPE_2D_TXTSX_POLICY            0x2D
#define FIT_TYPE_2F_JMP_DEBUG_POLICY        0x2F
#define FIT_TYPE_7F_SKIP                    0x7F

#define FIT_POINTER_ADDRESS                 0xFFFFFFC0 ///< Fixed address at 4G - 40h

#define FIT_TYPE_VERSION                    0x0100

#define FIT_TYPE_00_SIGNATURE  SIGNATURE_64 ('_', 'F', 'I', 'T', '_', ' ', ' ', ' ')

#define BPM_SIGNATURE SIGNATURE_64          ('_', '_', 'A', 'C', 'B', 'P', '_', '_')
#define IBB_SIGNATURE SIGNATURE_64          ('_', '_', 'I', 'B', 'B', 'S', '_', '_')

//
// Data structure definition
//
#pragma pack (1)
//
// Hash structure
//
typedef struct {
  UINT16              HashAlg;
  UINT16              Size;
} SHAX_HASH_STRUCTURE;

typedef struct {
  UINT16              Size;               //Total number of bytes of HASH_LIST structure
  UINT16              Count;              //Number of Digest elements
} HASH_LIST;

typedef struct {
  UINT16              HashAlg;
  UINT16              Size;
} HASH_STRUCTURE;

typedef struct {
  UINT64              StructureId;                                              /// 00
  UINT8               StructVersion;      // 0x20                               /// 08
  UINT8               Reserved0;                                                /// 09
  UINT16              ElementSize;        // Total # of bytes in the element    /// 0A
} BPM_ELEMENT_HEADER;

typedef struct {
  BPM_ELEMENT_HEADER  Header;                                                   /// 00
  UINT8               Reserved1;                                                /// 0C
  UINT8               SetType;                                                  /// 0D
  UINT8               Reserved;                                                 /// 0E
  UINT8               PbetValue;                                                /// 0F
  UINT32              Flags;                                                    /// 10
  UINT64              IbbMchBar;                                                /// 14
  UINT64              VtdBar;                                                   /// 1C
  UINT32              DmaProtBase0;                                             /// 24
  UINT32              DmaProtLimit0;                                            /// 28
  UINT64              DmaProtBase1;                                             /// 2C
  UINT64              DmaProtLimit1;                                            /// 34
  SHAX_HASH_STRUCTURE PostIbbHash;                                              /// 3C
} IBB_ELEMENT;

typedef struct {
  UINT64              StructureID;
  UINT8               StructVersion;      // value has been promoted to 2.1 (0x21)
  UINT8               HdrStructVersion;   // 0x02 or 0x20 ???
  UINT16              HdrSize;            // new for 2.x
  UINT16              KeySignatureOffset; // new for 2.x
  UINT8               BpmRevision;
  UINT8               BpmRevocation;
  UINT8               AcmRevocation;
  UINT8               Reserved;
  UINT16              NEMPages;
} BPM_HEADER;

typedef struct {
  /**
    Address is the base address of the firmware component
    must be aligned on 16 byte boundary
  **/
  UINT64              Address;
  UINT8               Size[3];   ///< Size is the span of the component in multiple of 16 bytes
  UINT8               Reserved;  ///< Reserved must be set to 0
  /**
    Component's version number in binary coded decimal (BCD) format.
    For the FIT header entry, the value in this field will indicate the revision
    number of the FIT data structure. The upper byte of the revision field
    indicates the major revision and the lower byte indicates the minor revision.
  **/
  UINT16              Version;
  UINT8               Type : 7;  ///< FIT types 0x00 to 0x7F
  ///
  /// Checksum Valid indicates whether component has valid checksum.
  ///
  UINT8               C_V : 1;
  /**
    Component's checksum. The modulo sum of all the bytes in the component and
    the value in this field (Chksum) must add up to zero. This field is only
    valid if the C_V flag is non-zero.
  **/
  UINT8               Chksum;
} FIRMWARE_INTERFACE_TABLE_ENTRY;
#pragma pack ()

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data

**/
VOID *
FindFitEntryData (
  IN UINT8                        Type
  );

/**
  Find BPM address

  @return BPM address

**/
VOID *
FindBpm (
  VOID
  );

/**
  This functions finds the OBB Hash stored inside the Boot Policy Manifest (BPM)

  @param  BootMode              Current boot mode.

  @retval OBB Hash              A struture pointer where the OBB Hash is located inside the BPM.
  @retval NULL                  BPM was not found or is invalid

**/
VOID *
FindObbHash (
  EFI_BOOT_MODE                   BootMode
  );

/**
  This functions determines if the Hashed FVs are intended for Verified Boot, Measured or Both.

  @param  None

  @retval EFI_SUCCESS           The applicable type of boot, whether Verified, Measured or Both.

**/
UINT64
GetBaseHashFvFlag (
  VOID
  );

/**
  This functions installs a PPI containing all Hashed FVs along with their Hash information.

  @param  StoredHashFvPpi       A structure pointer to PPI where hashed FVs along with their Hash information are stored.
  @param  HashInfo              A structure pointer to the Hash of all the FVs Hashes
  @param  BootMode              Current boot mode.

  @retval EFI_SUCCESS           PPI was successfully installed.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.

**/
// APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported  
/*
EFI_STATUS
InstallStoredHashFvPpi (
  IN EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI  *StoredHashFvPpi,
  IN HASH_STRUCTURE                                     *HashInfo,
  IN EFI_BOOT_MODE                                      BootMode
  );
*/
 // APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported  
