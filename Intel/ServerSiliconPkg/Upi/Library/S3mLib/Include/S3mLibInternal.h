/** @file
  S3M library internal header file.

  This file contains the internal definitions for data structure and so on.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef __S3M_LIB_INTERNAL_H__
#define __S3M_LIB_INTERNAL_H__

#include <MaxSocket.h>
#include <Upi/KtiSi.h>

#define RETRY_COUNT                 3

#define IEK_WRAP_SIZE               40
#define ENCRYPT_SIZE                16

//
// stringizing helper macros
//
#define STRINGIFY(x) #x

#pragma pack (push,1)

/*********************************************************************
                          S3M SoftStrap
*********************************************************************/

//
// Currently, the SPR_HOST and GNRSRF_HOST are both defined, which will cause build failure, so
// (1) comment out below HOST flag check
// (2) make SPR and GNRSRF use the same SoftStrapDefinision.h, need to separate the SoftStrapDefinition.h later.
//

#if defined(SPR_HOST) || defined(GNRSRF_HOST)
//#if defined(GNRSRF_HOST)
//#error "Only one HOST flag can be defined per build"
//#endif

#include STRINGIFY(Registers/Spr/SoftStrapDefinition.h)
#endif

//#if defined(GNRSRF_HOST)
//#if defined(SPR_HOST)
//#error "Only one HOST flag can be defined per build"
//#endif
//#include STRINGIFY(Registers/GnrSrf/SoftStrapDefinition.h)
//#endif

typedef union {
  UINT32              RawData32[S3M_CPLD_DATA_BLOCK_SIZE];
  S3M_DATA_FORMAT     DataBlob;
} S3M_CONFIG_SOFTSTRAP_PAYLOAD;

typedef struct {
  STRAP_SET_LIST      StrapSet[MAX_SOCKET];
} CPU_STRAP_SET;

/*********************************************************************
                          S3M CFR
*********************************************************************/

typedef struct {
UINT32     ModuleType;               //    Identifies the header type to CSS as Image Signing Header, value is 0x00000013

UINT32     HeaderLen;                //    Length of the entire signing structure in DWORDS, including this header, the public signing key and the signature, but excluding the image being signed.
                                     //    For example, for RSA3K, this field will be 225DW, which includes 32DW for this header + 96DW for the signing key modulus + 1DW for the signing key exponent + 96DW for the signature.

UINT32     HeaderVersion;            //    Version of this header. First version will be 0x00010000.

UINT32     ModuleID;                 //    Bit [31] - Debug: Indicates a debug signing. Firmware must refuse to load debug-signed images on production parts.
                                     //    Bit[30] - Patch: Indicates image is to be signed by a CSS Patch Project, which provides a symmetric encryption key. This renders the Debug bit of this field security-critical, since the debug and release keys are shared in a Patch project. If this bit is set, firmware must check the Debug bit and prevent verification on release parts. If this bit value mismatches the CSS Project type, the signing will fail and an error will be returned.
                                     //    Bits [29:0] - ImageType: Identifies the entity being signed. The value is arbitrary to the CSS, but significant to the business unit, which may use some or all bits to also identify the device for which this image is intended.
                                     //    Values:
                                     //    0x05 - OROM Image
                                     //    0x06 - ESS NVM Image
                                     //    0x07 - BaseIA IBB Image
                                     //    0x08 - Miniloader
                                     //    0x09 - Flash Descriptor
                                     //    0x0A - OEM co-signing manifest
                                     //    0x0B - PCODE/UCODE CFR
                                     //    0x0C - S3M FW CFR

UINT32    ModuleVendor;              //    Bits[31:16] - Reserved: Set to zero
                                     //    Bits[15:0] - VendorCode: Vendor which created and signed this entity. MEU signing tool will set this for OEMs to a different value. If OEM is signing and using ARO, then value of OEM must match AROVendorID field below.
                                     //    0x8086 - Intel
                                     //    0xNNNN - OEM

UINT32    Date;                      //    CSS requires a valid date in the following packed BCD format:
                                     //    0xYYYYMMDD
                                     //    Actual meaning is per use case, but typically would correspond to the build date of the object being signed

UINT32    Size;                      //    The length of the entire signing structure in DWORDs, thus HeaderLen + size of image being signed

UINT8     SigningType;               //    Identifies the cryptographic algorithm used for signing. For human readable purposes only:
                                     //    0 - RSA3K
                                     //    1 - ECDSA 384

UINT8     SigningMode;               //    Identifies the next level of detail of cryptographic algorithm used for signing. For human readable purposes only, and is dependent on the SigningType:
                                     //    RSA3K:
                                     //    0 - PKCS
                                     //    1 - PSS
                                     //    ECDSA 384:
                                     //    0 - P384
                                     //    1 - Brainpool384

UINT8     HashType;                  //    Identifies the hash algorithm and length for signing. For human readable purposes only:
                                     //    1 - SHA384
                                     //    2 - SHA512

UINT8    EncryptionType;             //    Identifies the cryptographic algorithm used for encrypting. For human readable purposes only:
                                     //    0 - None
                                     //    1 - AES256 CTR

UINT32   EntryOffset;                //    Offset from beginning of this header to the entry point of the signed image in words

UINT32   UniqueID;                   //    Unique per-signed object identifier, arbitrary to CSS but significant to the business unit

UINT32   SVN;                        //    Security version number for object

UINT32   SE_SVN;                     //    SE Security Version number for object

UINT8    IEKWrap[IEK_WRAP_SIZE];     //    Wrapped Image Encryption Key (IEK), if ModuleID:30 is set, otherwise zeros. A new IEK will be used for every image encryption. Note that LT-CSS retention of IEK usage history is TBD.

UINT8    ImageEncryptIV[ENCRYPT_SIZE];//   Encryption Initialization Vector for image encryption. On return, this field will be set to the low-order 96 bits of SHA256(header || image).
                                     //    Set to zero when submitting to LT-CSS. If encryption was not successful, IV will remain zero.

UINT8    Flags;                      //    Bits[7:3]: Reserved and set to zero
                                     //    Bit[2] - ExtHeaderPresent: Indicates that an extension header is present. The extension header is located at the beginnig of the image being signed. This bit is always set for the ESS NVM image and the OEM co-signing manifest associated with the ESS NVM image to reflect the fact that a root key revocation list is always present with these images.
                                     //    Bit[1] - UpdateFSVN: The corresponding FSVN is to be updated with the ISVN value found in the SVN field of this header.
                                     //    Bit[0] - ARO: Anti-Rollback Override (relevant only in FW manifests). Indicates that the FSVN for each validly co-signed image may be ignored.

UINT8    ReservedCSS[sizeof(UINT32) - sizeof(UINT8)];//    Maintain alignment

UINT16   AROVendorID;                //    Bits[15:0] - Indicates the OEM Vendor ID which has ARO enabled for it. VendorID codes will be decided later. If ARO =
                                     //    0: AROVendorID = 0;
                                     //    1: AROVendorID: 0xNNNN, Cannot = 0x8086 (Intel)

UINT32   CPUID;                      //    CPUID of the incoming content

UINT32   PlatformID;                 //    Platform ID of the incoming content

UINT16   ImageContentSpecifics;      //    Bits [15:8]:
                                     //        - 001: uCode placed first
                                     //        - 010: Pcode placed first
                                     //        - Rest are Not a valid setting for *Code CFR flows; else can be zero (including for S3M FW CFR)
                                     //    Bits [7:3]: Reserved & Set to zero
                                     //    Bits [2:0]:
                                     //        - 000: Not a valid setting for S3M & *Code CFR flows; else can be zero
                                     //        - 001: uCode only image
                                     //        - 010: Pcode only image
                                     //        - 011: Ucode+Pcode image
                                     //        - 100: S3M FW image

UINT16   FirstCFRImageLength;        //    The length of the first image placed (can be uCode or Pcode).

UINT16   SecondCFRImageLength;       //    The length of the second image placed (can be uCode or Pcode).

UINT32   ReservedUser;               //    Unused by CSS, may be used to carry cookies or other metadata
} S3M_COMMON_CSS_HEADER;

#pragma pack (pop)

#define S3M_IS_PUCODE_CFR_IMAGE(Image) ((((S3M_COMMON_CSS_HEADER*)Image)->ModuleID & 0xFF) == 0xB)
#define S3M_IS_S3M_FW_CFR_IMAGE(Image) ((((S3M_COMMON_CSS_HEADER*)Image)->ModuleID & 0xFF) == 0xC)

#endif // __S3M_LIB_INTERNAL_H__
