//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
 Verify signed DXE FVs (PEI after memory, DXE2 & BDS FVs)
 Locate FvDxeSig FFS in FV_DATA FV, if found - verify the signature of signed FV ranges
 by comparing decrypted hash against a hash value constructed using the ROM map layout
 and stored inside the FV_DATA FvDxeSig Certificate header
 Copy all signed FV Dxe ranges to RAM before performing hash calculation.
 Install a PPI to notify TCG module the good DXE Hash is ready to be extended to PCR
**/
#include <Token.h>  // FLASH_SIZE
#include <Ppi/AmiFvDxeValidate.h>

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <PPI/CryptoPPI.h>
#include "AmiCertificate.h"
#include <Protocol/AmiDigitalSignature.h> // EFI_CERT_X509_SHAXXX
#include <Library/AmiRomLayoutLib.h>

#include <Library/DebugLib.h>
#include <Library/PerformanceLib.h>

//------------------------------------------------------------------------
// External function prototypes
//------------------------------------------------------------------------
VOID ExecuteActionOnFailureToValidateFv(IN CONST EFI_PEI_SERVICES **PeiServices);

//------------------------------------------------------------------------
// Local defines
//------------------------------------------------------------------------
#define  MAX_FV_STORE_HEADER_SIZE 0x100

#ifndef AMI_FFS_COMMON_SECTION_HEADER
typedef struct _AMI_FFS_COMMON_SECTION_HEADER {
    EFI_FFS_FILE_HEADER FfsHdr;                         //< The FFS file header
    EFI_FREEFORM_SUBTYPE_GUID_SECTION GuidedSection;    //< The Guided Section of the FFS file
    APTIO_FW_CAPSULE_HEADER   pFvSigHeader[0];
} AMI_FFS_COMMON_SECTION_HEADER;
#endif

//------------------------------------------------------------------------
// Local prototypes
//------------------------------------------------------------------------
EFI_STATUS EFIAPI InstallFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *Ppi
);
EFI_STATUS EFIAPI LaunchFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *Ppi
);

static EFI_PEI_NOTIFY_DESCRIPTOR mReadyToInstallFvDxeVerify[] =
{
    {
        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
        &gAmiDigitalSignaturePPIGuid, 
        InstallFvDxeVerifyPpi
    },
};

static EFI_PEI_NOTIFY_DESCRIPTOR mReadyToLaunchFvDxeVerify[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gAmiPublishFvDxeValidatePpiGuid,
        LaunchFvDxeVerifyPpi
    },
};

//------------------------------------------------------------------------
// Notify, Install PPI
//------------------------------------------------------------------------
CONST EFI_PEI_PPI_DESCRIPTOR mAmiInstallCryptoPpi = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmiInstallCryptoPpiGuid, NULL
};

static EFI_PEI_PPI_DESCRIPTOR mFvDxeValidatePpiDesc = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gAmiFvDxeValidatePpiGuid, NULL
};

static EFI_PEI_PPI_DESCRIPTOR mFvDigestPpiDesc = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gAmiFvDigestPpiGuid, NULL
};

//------------------------------------------------------------------------
// Verify FvSig PPI
//------------------------------------------------------------------------
/**
Determine if the Rom Area attributes indicate the FV is measured

@param    Attributes

@retval   TRUE if FV is Measured
**/
BOOLEAN
IsFvMeasured (
    UINT32 Attributes
){
    return ( (Attributes & AMI_ROM_AREA_MEASURED) == AMI_ROM_AREA_MEASURED
           ||
           // TBD. Remove when AREA_MEASURED attribute becomes mainstream
           ( 
            // one of DXE/DXE2/BDS || PEI_AFTER_MEM
            (Attributes & AMI_ROM_AREA_FV_ACCESS_ATTRIBUTES) != 0
            &&
            // not FV_BB
            (Attributes & (AMI_ROM_AREA_FV_PEI | AMI_ROM_AREA_VITAL)) != (AMI_ROM_AREA_FV_PEI | AMI_ROM_AREA_VITAL)
           )
    );
}

/**
The Rom image hash is calculated according to Rom Area map

 @param[in]   ThisPpi Pointer to the FvDxeValidate Ppi data structure
 @param[in]   FvSigHeader - pointer to a FW Capsule Hdr
 @param[in]   HashAlgorithm
 @param[in]   HashDB

 @retval      Status 
              EFI_SUCCESS            Your hash is in the output buffer
              EFI_SECURITY_VIOLATION Something went wrong
**/
EFI_STATUS HashFwRomMapImage (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader,
    IN EFI_GUID                 *HashAlgorithm,
    IN OUT UINT8                *HashDB
){
    EFI_STATUS  Status;
    ROM_AREA    *RomAreaTbl;
    AMI_CRYPT_DIGITAL_SIGNATURE_PPI *AmiSigPpi;
    UINTN        RomSize = FLASH_SIZE;
    UINTN        i, RomMap_size, max_num_elem, num_elem;

    UINTN *AddrList;
    UINTN *LenList;

    AmiSigPpi = (AMI_CRYPT_DIGITAL_SIGNATURE_PPI *)ThisPpi->AmiSigPpi;

    RomAreaTbl = (ROM_AREA *)((UINTN)FvSigHeader+FvSigHeader->RomLayoutOffset);
    RomMap_size = FvSigHeader->RomImageOffset-FvSigHeader->RomLayoutOffset;
    // assume max size of RomMap array = RomMap_size/sizeof(ROM_AREA);
    max_num_elem = RomMap_size/sizeof(ROM_AREA);
    // at least one signed range must be included
    if(max_num_elem == 0) return EFI_SECURITY_VIOLATION;
    i = (max_num_elem+1)*sizeof(UINTN); // add 1 extra entry for FvSigHdr
    AddrList = (UINTN *)AllocateZeroPool(i*2);
    ASSERT (AddrList != NULL);    
    if(AddrList == NULL) return EFI_OUT_OF_RESOURCES;
    LenList = (UINTN*)((UINTN)AddrList + i);

    ThisPpi->FvDigestPpi->AddrList = AddrList;
    ThisPpi->FvDigestPpi->LenList = LenList;
    ThisPpi->FvDigestPpi->HashElemCnt = 0;

    num_elem = 0;
    for(i=0; i < max_num_elem && RomAreaTbl[i].Size != 0; i++)
    {
        if(!IsFvMeasured(RomAreaTbl[i].Attributes) ||
           !(RomAreaTbl[i].Attributes & AMI_ROM_AREA_SIGNED))
            continue;

        // sanity check for buffer overruns
        if(RomAreaTbl[i].Offset > RomSize ||
           ((UINT64)RomAreaTbl[i].Offset + RomAreaTbl[i].Size) > RomSize )
            return EFI_SECURITY_VIOLATION;

        AddrList[num_elem] = (UINTN)RomAreaTbl[i].Address;
        LenList[num_elem] = RomAreaTbl[i].Size;
        num_elem++;
    }
    // at least one signed range must be included
    if(num_elem == 0 || num_elem > max_num_elem) return EFI_SECURITY_VIOLATION;

    //  Hash of Capsule Hdr + FW Certificate Hdr
    if(FvSigHeader->CapHdr.Flags & CAPSULE_FLAGS_CAPHDR_IN_SIGNCERT) {
        AddrList[num_elem] = (UINTN)FvSigHeader;
        LenList[num_elem] = (UINTN)&FvSigHeader->FWCert.SignCert.CertData - (UINTN)FvSigHeader;
        num_elem++;
        if(num_elem > max_num_elem) return EFI_SECURITY_VIOLATION;
    }

    ThisPpi->FvDigestPpi->HashElemCnt = num_elem;

PERF_START (NULL, "Hash", NULL, 0);
    Status = AmiSigPpi->Hash(AmiSigPpi, HashAlgorithm, num_elem, (const UINT8**)AddrList,  (const UINTN*)LenList, HashDB );
PERF_END (NULL, "Hash", NULL, 0);

DEBUG_CODE (
    for (i=0; i<num_elem; i++) DEBUG ((DEBUG_INFO, "%d:Hash Range: %X-%X\n", i+1, AddrList[i], LenList[i]));
    for (i=0; i<16; i++) DEBUG ((DEBUG_INFO," %02X", (HashDB[i]) )); DEBUG ((DEBUG_INFO, "\nFvDxe Hash : %r\n", Status));
);

    return Status;
}

/**
This code verifies the signature for the measured FV DXE regions, 
 and performs following checks on the image:
  1. Signing certificate is signed with trusted Root Platform key
  2. Integrity check. Image Signature verification
Validate  Root Certificate 
   -Compare PubKey in Root Certificate Hdr with local FW Platform Key
   -Hash SignCert.PubKey 
   -VerifySig for RootCert.Signature and compare with SignCert.Key hash
 Validate  Sign Certificate 
   -Hash the ROM image inside the FW Capsule
   -VerifySig for SignCert.Signature and compare with calculated hash

 @param[in]   ThisPpi Pointer to the FvDxeValidate Ppi data structure
 @param[in]   FvSigHeader - pointer to a FW Capsule Hdr

 @retval     Status
             EFI_SUCCESS               The signature is successfully verified.
             EFI_SECURITY_VIOLATION    The signature does not match the given message.
**/
EFI_STATUS VerifyFwCertRsa2048Sha256 (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader
){
    EFI_STATUS              Status;
    PEI_CRYPT_HANDLE        PubKeyHndl;
    PEI_CRYPT_HANDLE        HashHndl;
    UINT8                   *pSig;
    UINT32                  Flags;
    UINT8                   *Addr;
    UINTN                   Size;
    EFI_CERT_BLOCK_RSA_2048_SHA256* pRootCert;
    AMI_CRYPT_DIGITAL_SIGNATURE_PPI *AmiSigPpi;

    AmiSigPpi = (AMI_CRYPT_DIGITAL_SIGNATURE_PPI *)ThisPpi->AmiSigPpi;

    Flags = (FvSigHeader->CapHdr.Flags & CAPSULE_FLAGS_RSA_PSS_PADDING_SCHEME)?EFI_CRYPT_RSASSA_PSS:EFI_CRYPT_RSASSA_PKCS1V15;

    HashHndl.AlgGuid  = gEfiHashAlgorithmSha256Guid;
    HashHndl.BlobSize = SHA256_DIGEST_SIZE;
    HashHndl.Blob = (UINT8*)&(ThisPpi->FvDigestPpi->FvHash[0]); // use the as a scratch Hash buffer

//
// 1. Compare Capsule's Sign Cert key with Platform Root Key
//
    PubKeyHndl.BlobSize =  DEFAULT_RSA_KEY_MODULUS_LEN;
    PubKeyHndl.AlgGuid = gEfiCertRsa2048Guid;
    PubKeyHndl.Blob = (UINT8*)FvSigHeader->FWCert.SignCert.CertData.PublicKey;
    Status = AmiSigPpi->VerifyKey(AmiSigPpi, &gPRKeyGuid, &PubKeyHndl); 
    DEBUG ((DEBUG_INFO, "Compare Platform and SignCert Keys : %r\n", Status));
    //  Skip the RootCert key checking if SignCert Key and PR Key are matching
    if(EFI_ERROR(Status)) {
    //
    // 1.1 Verify Signed Certificate key chain to a Platform Root key
    //
        for (pRootCert = &FvSigHeader->FWCert.RootCert; 
             (UINTN)pRootCert < ((UINT64)(UINTN)&FvSigHeader->FWCert+FvSigHeader->FWCert.SignCert.Hdr.Hdr.dwLength) &&
                 pRootCert->PublicKey[0]!=0;
             pRootCert++) 
        {
            PubKeyHndl.Blob = (UINT8*)pRootCert->PublicKey;
            Status = AmiSigPpi->VerifyKey(AmiSigPpi, &gPRKeyGuid, &PubKeyHndl); 
            DEBUG ((DEBUG_INFO, "Compare Platform and RootCert Keys : %r\n", Status));
            if (EFI_ERROR(Status)) continue;
            //
            // 2. Verify RootCert.Signature
            //
            // 2.1  Compute FWCert.SignCert.PublicKey Hash
            // 
            if(FvSigHeader->CapHdr.Flags & CAPSULE_FLAGS_SIGNKEY_IN_ROOTCERT)
            {
                Addr = (UINT8*)FvSigHeader->FWCert.SignCert.CertData.PublicKey;
                Size = DEFAULT_RSA_KEY_MODULUS_LEN;
            } else
            // 2.2  Compute FWCert.SignCert Hash
            {
                Addr = (UINT8*)&FvSigHeader->FWCert.SignCert;
                Size = sizeof(AMI_CERTIFICATE_RSA2048_SHA256);
            }
            Status = AmiSigPpi->Hash(AmiSigPpi,&gEfiHashAlgorithmSha256Guid, 1,(const UINT8**)&Addr,(const UINTN*)&Size, HashHndl.Blob);
            if (EFI_ERROR(Status)) break;

            pSig = (void*)pRootCert->Signature; 
            Status = AmiSigPpi->VerifySig(AmiSigPpi, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags ); 
            DEBUG ((DEBUG_INFO, "Verify Root Cert : %r\n", Status));
            break;
        }
    }
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
//
// 3. Verify Signing Cert
//
// 3.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(ThisPpi, FvSigHeader, &gEfiHashAlgorithmSha256Guid, HashHndl.Blob);
    if (EFI_ERROR(Status)) return Status;

    ThisPpi->FvDigestPpi->HashSize = SHA256_DIGEST_SIZE;
    ThisPpi->FvDigestPpi->HashAlgId = AMI_TDXE_SHA256;

    pSig = (void*)FvSigHeader->FWCert.SignCert.CertData.Signature; 
    PubKeyHndl.Blob = (UINT8*)FvSigHeader->FWCert.SignCert.CertData.PublicKey;

    PERF_START (NULL, "Pkcs1", NULL, 0);
    Status = AmiSigPpi->VerifySig(AmiSigPpi, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags);
    PERF_END (NULL, "Pkcs1", NULL, 0);
    return Status;
}

/**
This code verifies FW Capsule is genuine,
 and performs following checks on the image:
  1. Signing certificate is signed with trusted Root Platform key
  2. Integrity check. Image Signature verification

 @param[in]   ThisPpi Pointer to the FvDxeValidate Ppi data structure
 @param[in]   FvSigHeader - pointer to a FW Capsule Hdr

 @retval      Status 
              EFI_SUCCESS               The signature is successfully verified.
              EFI_SECURITY_VIOLATION    The signature does not match the given message.
**/
EFI_STATUS VerifyFwCertPkcs7 (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader
){
    EFI_STATUS              Status;
    UINT8                  *Pkcs7Cert, *pDigest, *TrustCert;
    UINTN                   Pkcs7Cert_len, DigestLen, CertSize;
    EFI_GUID               *HashAlgorithm;
    HASH_ALG                DigestAlgo=SHA256;
    EFI_CERT_X509_SHA256    HashCert;
    AMI_CRYPT_DIGITAL_SIGNATURE_PPI *AmiSigPpi;
    PEI_CRYPT_HANDLE        PubKeyHndl;

    AmiSigPpi = (AMI_CRYPT_DIGITAL_SIGNATURE_PPI *)ThisPpi->AmiSigPpi;

//
// 1. Validate Root Key
//
    PubKeyHndl.BlobSize =  0;
    PubKeyHndl.Blob = NULL;
    Status = AmiSigPpi->GetKey(AmiSigPpi, &gPRKeyGuid, &PubKeyHndl);
    // Skip further FV verify test if there is no or dummy FWkey
    DEBUG ((DEBUG_INFO, "Get FW Key (%r), [%X...] (%d bytes) %g\n", Status, (*(UINT32*)PubKeyHndl.Blob), PubKeyHndl.BlobSize, &PubKeyHndl.AlgGuid));
    if(EFI_ERROR(Status)) {
        return Status;
    }

    if(PubKeyHndl.Blob==NULL)
        return EFI_SECURITY_VIOLATION; 

    // FwKey is potentially a TimeStamped Hash of x509 tbs certificate data
    if(CompareGuid(&PubKeyHndl.AlgGuid, &gEfiCertSha256Guid)) {
        CopyMem((void*)HashCert.ToBeSignedHash, PubKeyHndl.Blob, SHA256_DIGEST_SIZE);
        TrustCert = (UINT8*)&HashCert;
        CertSize = sizeof(EFI_CERT_X509_SHA256);
    } else
        if(CompareGuid(&PubKeyHndl.AlgGuid, &gEfiCertX509Guid)) {
            TrustCert = PubKeyHndl.Blob;
            CertSize  = PubKeyHndl.BlobSize;
        } else
            return EFI_UNSUPPORTED;

    Pkcs7Cert = (UINT8*)&FvSigHeader->FWCert.SignCert.CertData; 
    Pkcs7Cert_len = FvSigHeader->FWCert.SignCert.Hdr.Hdr.dwLength-sizeof(WIN_CERTIFICATE_UEFI_GUID_1);
    // get Digest Algorithm
    Status = AmiSigPpi->Pkcs7GetDigestAlgorithm( AmiSigPpi,
                         Pkcs7Cert, Pkcs7Cert_len, &DigestAlgo);
    DEBUG ((DEBUG_INFO, "%r: Get Hash Type = %d\n", Status, DigestAlgo));
    if (EFI_ERROR(Status)) return Status;

    switch(DigestAlgo) {
        case AMI_TDXE_SHA256: 
            HashAlgorithm = &gEfiHashAlgorithmSha256Guid;
            DigestLen = SHA256_DIGEST_SIZE;
        break;
        case AMI_TDXE_SHA384: 
            HashAlgorithm = &gEfiHashAlgorithmSha384Guid;
            DigestLen = SHA384_DIGEST_SIZE;
        break;
        case AMI_TDXE_SHA512: 
            HashAlgorithm = &gEfiHashAlgorithmSha512Guid;
            DigestLen = SHA512_DIGEST_SIZE;
        break;
        default: 
            DEBUG ((DEBUG_ERROR, "Unsupported Hash Algorithm=%d. Only SHA256(1),SHA384(3) and SHA512(5) are valid. \n", DigestAlgo));
            return EFI_UNSUPPORTED;
    }
// 2. Verify Signing Cert Signature
//
// 2.1 The Rom image hash is calculated based on info from the Rom Area map
//
    pDigest = &(ThisPpi->FvDigestPpi->FvHash[0]);
    Status = HashFwRomMapImage(ThisPpi, FvSigHeader, HashAlgorithm, pDigest);
    if (EFI_ERROR(Status)) return Status;

    ThisPpi->FvDigestPpi->HashSize = DigestLen;
    ThisPpi->FvDigestPpi->HashAlgId = DigestAlgo;
// 2.2 Verify Fw Certificate
    PERF_START (NULL, "Pkcs7", NULL, 0);
    Status = AmiSigPpi->VerifyPkcs7Sig( AmiSigPpi,
                     Pkcs7Cert, Pkcs7Cert_len,           // Pkcs7Cert
                     TrustCert, CertSize,
                     &pDigest, &DigestLen // In/OutData
                     );
    PERF_END (NULL, "Pkcs7", NULL, 0);
    return Status;
}
/**
This code verifies the signature for the measured FV DXE regions,
 and performs following checks on the image:
  1.Signing certificate is signed with trusted Root Platform key
  2.Integrity check. Image Signature verification
   -Hash the ROM ranges listed in the embedded ROM map
   -VerifySig: decode digest and compare with calculated hash

 @param[in]   ThisPpi Pointer to the FvDxeValidate Ppi data structure
 @param[in]   FvSigHeader - pointer to a FW Capsule Hdr

 @retval     Status 
             EFI_SUCCESS               The signature is successfully verified.
             EFI_SECURITY_VIOLATION    The signature does not match the given message.
**/
EFI_STATUS VerifyFvDxeCert (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader
){
    EFI_STATUS           Status;

    if(ThisPpi == NULL || 
       ThisPpi->AmiSigPpi == NULL || 
       FvSigHeader == NULL 
       ) {
        return EFI_INVALID_PARAMETER;
    }

    if(CompareGuid((EFI_GUID*)&FvSigHeader->FWCert.SignCert.Hdr.CertType, &gEfiCertTypeRsa2048Sha256Guid)) {
        Status = VerifyFwCertRsa2048Sha256(ThisPpi, FvSigHeader);
    } else
       if(CompareGuid((EFI_GUID*)&FvSigHeader->FWCert.SignCert.Hdr.CertType, &gEfiCertPkcs7Guid)) {
            Status = VerifyFwCertPkcs7(ThisPpi, FvSigHeader);
        } else {
            Status = EFI_UNSUPPORTED;
            DEBUG((DEBUG_ERROR,"Invalid Certificate type %g\n",(EFI_GUID*)&FvSigHeader->FWCert.SignCert.Hdr.CertType));
        }
    return Status;
}

/**
 Return size of FV header, including ExtHeader

 @param[in] Address - Address of FV to search in

 @retval    Fv Hdr size
**/
UINT32 GetFvHeaderSize(VOID *Address){
    EFI_FIRMWARE_VOLUME_HEADER *FwVolHeader;
    EFI_FIRMWARE_VOLUME_EXT_HEADER *FwVolExtHeader;

    FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)Address;
    if (   FwVolHeader->Signature!=EFI_FVH_SIGNATURE//FV_SIGNATURE
        || FwVolHeader->HeaderLength != (UINT16)(sizeof(EFI_FIRMWARE_VOLUME_HEADER)+sizeof(EFI_FV_BLOCK_MAP_ENTRY))
    ) return 0;
    if (FwVolHeader->ExtHeaderOffset == 0){
        return (UINT32)FwVolHeader->HeaderLength;
    }else {
        UINT32 HeaderSize;
        UINT32 MaxOffset =  MAX_FV_STORE_HEADER_SIZE - sizeof(EFI_FFS_FILE_HEADER) - sizeof(EFI_FIRMWARE_VOLUME_EXT_HEADER);
        UINT32 MaxSize;

        if (   FwVolHeader->ExtHeaderOffset < FwVolHeader->HeaderLength
            || FwVolHeader->ExtHeaderOffset > MaxOffset
        ) return 0;
        
        MaxSize =   MAX_FV_STORE_HEADER_SIZE - sizeof(EFI_FFS_FILE_HEADER) - sizeof(EFI_FIRMWARE_VOLUME_HEADER) - sizeof(EFI_FV_BLOCK_MAP_ENTRY);
        FwVolExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINT8*)FwVolHeader + FwVolHeader->ExtHeaderOffset);
        if (   FwVolExtHeader->ExtHeaderSize < sizeof(EFI_FIRMWARE_VOLUME_EXT_HEADER)
            || FwVolExtHeader->ExtHeaderSize > MaxSize
        ) return 0;
        HeaderSize = FwVolHeader->ExtHeaderOffset + FwVolExtHeader->ExtHeaderSize;
        return ALIGN_VALUE(HeaderSize,8);
    }
}
/**
 This service will discover firmware files of FREEFORM type by a 
 File GUID and a Section GUID.


 @param[in] FvAddress - Address of FV to search in
 @param[in] FvSize - Size of FV to search in

 @param[out] Ffs - returns a pointer to FFs Hdr

 @retval     Status
**/
EFI_FFS_FILE_HEADER *
GetFvDxeSigFfs (
    IN UINT64    FvAddress,
    IN UINT32    FvSize
)
{
  EFI_FFS_FILE_HEADER  *ppFile;
  UINT32                FvHdrSize;
  UINTN                 Index;
  static EFI_GUID  gFvDxeFFSGuid   = {0x68d1fed0, 0x143e, 0x479c, {0xbf, 0x39, 0x45, 0x6e, 0x98, 0xb4, 0xfa, 0xc5}};
  static EFI_GUID gFvDxeFfsSecGuid = {0x72a45c4b, 0xf348, 0x408f, {0xb9, 0x13, 0xf7, 0x83, 0x9f, 0x24, 0x7c, 0xe2}};
  
  if (!FvSize || !FvAddress) 
    return NULL;

  FvHdrSize = GetFvHeaderSize((VOID*)(UINTN)FvAddress);
  ppFile = (EFI_FFS_FILE_HEADER*)((UINTN)FvAddress + FvHdrSize);
  Index = 0;
  while(((UINT64)(UINTN)ppFile > FvAddress) && 
        ((UINT64)(UINTN)ppFile < (FvAddress+FvSize))
        )
  {
      Index++;
      if(ppFile->Type == EFI_FV_FILETYPE_FREEFORM &&
         CompareGuid(&gFvDxeFFSGuid, &(ppFile->Name)) &&
         CompareGuid(&gFvDxeFfsSecGuid, &(((AMI_FFS_COMMON_SECTION_HEADER*)ppFile)->GuidedSection.SubTypeGuid))
      ){
          DEBUG ((DEBUG_INFO, "%d:Ffs %8X(%X)=%g, Type %2X\n", Index, (UINTN)ppFile, ((*(UINT32 *)ppFile->Size) & 0xffffff), &gFvDxeFFSGuid, ppFile->Type));
          DEBUG ((DEBUG_INFO, "===>FvDxeSig Ffs found\n"));
          return ppFile;
      }
      FvHdrSize = ALIGN_VALUE(((*(UINT32 *)ppFile->Size) & 0xffffff), 8);
      ppFile = (EFI_FFS_FILE_HEADER*)((UINT8*)ppFile+FvHdrSize);
  }
  return NULL;
}

/**
This function finds the FvSig Header inside FV, by traversing each Ffs within FV

 @param FvAddress     FV address
 @param Size          Size of the FV
 @param FvSigHeader   FvSig Header pointer

 @retval EFI_SUCCESS  Found guided encrypted section.
**/
EFI_STATUS
LocateFvDxeSigHeader(
  UINT64                     FvAddress,
  UINT32                     Size,
  APTIO_FW_CAPSULE_HEADER  **FvSigHeader
) {
  APTIO_FW_CAPSULE_HEADER   *pFvSigHeader;
  AMI_FFS_COMMON_SECTION_HEADER *FvSigFfs;

  FvSigFfs = (AMI_FFS_COMMON_SECTION_HEADER *)GetFvDxeSigFfs(FvAddress, Size);
  if( FvSigFfs ){
      pFvSigHeader = (APTIO_FW_CAPSULE_HEADER*)(FvSigFfs->pFvSigHeader);
      // just a sanity check - Cap Size must match the Section size
      if( (((*(UINT32 *)FvSigFfs->FfsHdr.Size) & 0xffffff) == 
             pFvSigHeader->CapHdr.HeaderSize + sizeof(AMI_FFS_COMMON_SECTION_HEADER)) 
          &&
          CompareGuid((EFI_GUID*)&pFvSigHeader->CapHdr.CapsuleGuid, &gFWCapsuleGuid)
      ){
          // More Hdr fields sanity checks for buffer overruns
          // Signed Aptio Capsule only supports WIN_CERT_TYPE_EFI_GUID 
          if(pFvSigHeader->FWCert.SignCert.Hdr.Hdr.wCertificateType != WIN_CERT_TYPE_EFI_GUID ||
             pFvSigHeader->RomImageOffset != pFvSigHeader->CapHdr.HeaderSize ||
             pFvSigHeader->RomImageOffset < (UINT64)pFvSigHeader->RomLayoutOffset + 2*sizeof(ROM_AREA) ||
             pFvSigHeader->RomLayoutOffset< (UINT64)pFvSigHeader->FWCert.SignCert.Hdr.Hdr.dwLength + offsetof(APTIO_FW_CAPSULE_HEADER, FWCert)
          ) {
              DEBUG ((DEBUG_ERROR, "FvDxeSig Hdr invalid\n"));
              return EFI_SECURITY_VIOLATION;
          }
          DEBUG ((DEBUG_INFO, "===>FvDxeSig Hdr found at %8X\n", (UINTN)pFvSigHeader));
          *FvSigHeader = pFvSigHeader;
          return EFI_SUCCESS;
      }
  }
  // header ffs is not found
  return EFI_NOT_FOUND;
}
#if 0
EFI_STATUS
LocateFvDxeSigHeader(
    UINT64                     FvAddress,
    UINT32                     Size,
    APTIO_FW_CAPSULE_HEADER  **FvSigHeader
) {
    APTIO_FW_CAPSULE_HEADER   *pFvSigHeader;
    static EFI_GUID  gFvDxeFFSGuid   = {0x68d1fed0, 0x143e, 0x479c, {0xbf, 0x39, 0x45, 0x6e, 0x98, 0xb4, 0xfa, 0xc5}};
    static EFI_GUID gFvDxeFfsSecGuid = {0x72a45c4b, 0xf348, 0x408f, {0xb9, 0x13, 0xf7, 0x83, 0x9f, 0x24, 0x7c, 0xe2}};

    UINT32 Signature;
    UINT32 *SearchPointer;
    AMI_FFS_COMMON_SECTION_HEADER *FileSection;

    Signature = gFvDxeFFSGuid.Data1;
    for(SearchPointer = (UINT32 *)FvAddress;
        SearchPointer < (UINT32 *)((UINTN)FvAddress + Size - sizeof(AMI_FFS_COMMON_SECTION_HEADER));
        SearchPointer++) {

        if(*SearchPointer == Signature) {
            FileSection = (AMI_FFS_COMMON_SECTION_HEADER *)SearchPointer;
            if(FileSection->FfsHdr.Type == EFI_FV_FILETYPE_FREEFORM &&
               CompareGuid(&gFvDxeFFSGuid, &(FileSection->FfsHdr.Name)) &&
               CompareGuid(&gFvDxeFfsSecGuid, &(FileSection->GuidedSection.SubTypeGuid))
            ){
                pFvSigHeader = (APTIO_FW_CAPSULE_HEADER*)(FileSection->pFvSigHeader);
                // just a sanity check - Cap Size must match the Section size
                if( (((*(UINT32 *)FileSection->FfsHdr.Size) & 0xffffff) == 
                       pFvSigHeader->CapHdr.HeaderSize + sizeof(AMI_FFS_COMMON_SECTION_HEADER)) 
                     &&
                    CompareGuid((EFI_GUID*)&pFvSigHeader->CapHdr.CapsuleGuid, &gFWCapsuleGuid)
                ){
                    // Signed Aptio Capsule only supports WIN_CERT_TYPE_EFI_GUID 
                    // More Hdr fields sanity checks for buffer overruns
                    if((pFvSigHeader->FWCert.SignCert.Hdr.Hdr.wCertificateType != WIN_CERT_TYPE_EFI_GUID) ||
                       (pFvSigHeader->RomLayoutOffset > pFvSigHeader->CapHdr.HeaderSize) ||
                       (pFvSigHeader->RomLayoutOffset < 
                         (UINT64)pFvSigHeader->FWCert.SignCert.Hdr.Hdr.dwLength + offsetof(APTIO_FW_CAPSULE_HEADER, FWCert))
                    ) {
                        DEBUG ((DEBUG_ERROR, "FvDxe Sig Hdr invalid\n"));
                        break;
                    }
                    DEBUG ((DEBUG_INFO, "FvDxe Sig Hdr found at FV Offs: 0x%0X\n", (UINTN)pFvSigHeader - (UINTN)FvAddress));
                    *FvSigHeader = pFvSigHeader;
                    return EFI_SUCCESS;
                }
            }
        }
    }
    // header ffs is not found
    return EFI_NOT_FOUND;
}
#endif
/**
 Function to relocate a firmware volume into system memory.

 @param PeiServices Pointer to the Pointer to the Pei Services Table
 @param Area Pointer to the AMI_ROM_AREA that should be moved into memory, and whose Address should be updated with the memory address

 @retval EFI_SUCCESS The firmware volume was relocated correctly
*/
EFI_STATUS RelocateFv(IN CONST EFI_PEI_SERVICES **PeiServices, IN OUT AMI_ROM_AREA *Area)
{
    EFI_STATUS Status;
    EFI_PHYSICAL_ADDRESS Dest;
    UINTN Pages;
    UINTN AlignSize = PcdGet32(AmiPcdTdxeShadowFvAlignSize);

    // Align to a page size
    Pages = EFI_SIZE_TO_PAGES(Area->Size + AlignSize);
    //
    Status = (*PeiServices)->AllocatePages(PeiServices, EfiBootServicesData, Pages, &Dest);
    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) {
        return Status;
    }
    // Align to a page size
    if(AlignSize != 0)
        Dest = (Dest & (EFI_PHYSICAL_ADDRESS)(0x100000000-AlignSize)) + AlignSize;

    CopyMem((VOID *)(UINTN)Dest, (VOID *)(UINTN)Area->Address, Area->Size);
    Area->Address = Dest;
    DEBUG((DEBUG_INFO, "-> %8lX [FvGuid]=%04X..., Aligned to %04X(%04X Pages) Attr %X\n", Area->Address, *(UINT32*)((UINT32)Area->Address+((EFI_FIRMWARE_VOLUME_HEADER*)(UINTN)(Area->Address))->ExtHeaderOffset), AlignSize, Pages, Area->Attributes));

    return Status;
}
/**
Verifies FVs with Measured attribute i.e., DXE, DXE2, BDS, PEI_MEM
Relocates FVs to RAM before verification

 @param ThisPpi Pointer to the Ppi data structure.
 @param PeiServices Pointer to the Pointer to the Pei Services Table.
 @param AmiRomArea Pointer to the AMI_ROM_AREA that should be moved into memory, and whose Address should be updated with the memory address

 @retval EFI_STATUS  Volume should be published
**/
EFI_STATUS EFIAPI
VerifyRelocateFvDxe (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN CONST EFI_PEI_SERVICES    **PeiServices,
    IN OUT AMI_ROM_AREA             *AmiRomArea
) {
    EFI_STATUS    Status;
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader;
    AMI_ROM_AREA *Area;
    ROM_AREA     *Area1;
    BOOLEAN      bShadow, bRangeValid;

    if(PeiServices == NULL || 
       ThisPpi     == NULL || 
       ThisPpi->FvDigestPpi == NULL ||
       ThisPpi->AmiSigPpi   == NULL )
        return EFI_NOT_READY;

    // to skip re-entry if previous test was successful
    if(ThisPpi->bFvDxeProcessed) {
        Status = (ThisPpi->bFvDxeStatus==TRUE)?EFI_SUCCESS:EFI_SECURITY_VIOLATION;
        DEBUG((DEBUG_INFO, "VerifyFvDxe : %r\n", Status));
        return Status;
    }
    PERF_START (NULL, "T-Dxe", NULL, 0);
    bShadow = PcdGetBool(AmiPcdTdxeShadowFvBeforePublish);
    FvSigHeader = (APTIO_FW_CAPSULE_HEADER*)NULL;
    PERF_START (NULL, "RelocFV", NULL, 0);
    for(Area = AmiGetFirstRomArea(); Area != NULL; Area = AmiGetNextRomArea(Area)) {
        DEBUG ((DEBUG_INFO, "FV %8lX(%8X)=%g, Attr %4X\n", Area->Address, Area->Size, (EFI_GUID*)((UINTN)Area->Address+((EFI_FIRMWARE_VOLUME_HEADER*)(UINTN)(Area->Address))->ExtHeaderOffset), Area->Attributes));
        if(!IsFvMeasured(Area->Attributes))
            continue;
        // Calling RelocateFv will copy the Current FV into memory
        // and set new address of the FV in Area descriptor
        if(bShadow) {
            RelocateFv(PeiServices, Area);
            // Update FV address if it was relocated
            if(AmiRomArea && AmiRomArea->Offset == Area->Offset)
                AmiRomArea->Address = Area->Address;
        }
        if(FvSigHeader == NULL) {
            if(!EFI_ERROR(LocateFvDxeSigHeader(Area->Address, Area->Size, &FvSigHeader)) && !bShadow)
                break;
        }
    }
    PERF_END (NULL, "RelocFV", NULL, 0);

    ThisPpi->bFvDxeProcessed = TRUE;
    ThisPpi->bFvDxeStatus = FALSE;

    if(FvSigHeader == NULL) {
        DEBUG((DEBUG_ERROR,"FvDxeSig Hdr not found\n"));
        PERF_END (NULL, "T-Dxe", NULL, 0);
        return EFI_NOT_FOUND; // go straight to recovery
    }
    /*
     * 1. Validate FvSigHdr RomArea ranges exist for each measured area in the global RomLoayout
     * The check is needed to catch a fake RomMap with empty or fixed range(s)
     * 
     * 2. Since the FvDxe Signed Certificate includes the RomMap 
     * with the FV ranges mapped to a Flash address space,
     * patch the RomMap FV start address to an address in memory.
     * 
     * Loop till Area1->Offset == Area->Offset
     *   in all entries with Area1->Offset >= Area->Offset && 
     *                       Area1->Offset <  Area->Offset+Area->Size
     * Replace all "Area1->Address" with Area->Address + Area1->Offset-Area->Offset
     * 
     */ 
    for(Area = AmiGetFirstRomArea(); Area != NULL; Area = AmiGetNextRomArea(Area)) {
        if(!IsFvMeasured(Area->Attributes) )
            continue;
        bRangeValid = FALSE;
        for(Area1 = (ROM_AREA*)((UINTN)FvSigHeader+FvSigHeader->RomLayoutOffset);
            Area1->Size != 0; Area1++) {
            if(!IsFvMeasured(Area1->Attributes) ||
               !(Area1->Attributes & AMI_ROM_AREA_SIGNED))
                continue;
            if(Area1->Offset >= Area->Offset &&
               Area1->Offset + Area1->Size <= Area->Offset + Area->Size ) {
                bRangeValid = TRUE;
                if(bShadow)
                    Area1->Address = Area->Address + (Area1->Offset - Area->Offset);
                DEBUG((DEBUG_INFO, "--->SgnRange: %8lX-%X\n", Area1->Address, Area1->Size));
            }
        }
        if(bRangeValid == FALSE) {
            DEBUG((DEBUG_ERROR,"Invalid FvDxe Hdr RomMap\n"));
            PERF_END (NULL, "T-Dxe", NULL, 0);
            return EFI_NOT_FOUND; // go straight to recovery
        }
    }

    // Begin Authentication
    Status = VerifyFvDxeCert(ThisPpi, FvSigHeader);
    DEBUG((DEBUG_INFO, "VerifyFvDxeCert: %r\n", Status));
    if(!EFI_ERROR(Status)) {
      ThisPpi->bFvDxeStatus = TRUE; // To avoid re-entry if validate was successful
      //-----------------------------------------------------------------------------------
      // Install FvDigestPPI
      //-----------------------------------------------------------------------------------
      Status = (*PeiServices)->InstallPpi (PeiServices, &mFvDigestPpiDesc);
      DEBUG((DEBUG_INFO, "Install FvDigestPpi(%p): %r\n", ThisPpi->FvDigestPpi, Status));
      ASSERT_EFI_ERROR(Status);
    }
    PERF_END (NULL, "T-Dxe", NULL, 0);

    return Status;
}
//---------------------------------------------------------------------------------------------------
// End PPI
//---------------------------------------------------------------------------------------------------
/**
 The function will :
  1. Publish a FvDxeValidate PPI after AmiDigitalSignature PPI is available.
  2. Execute FvDxeValidate PPI immediately after step1 for each pending FV to be published after memory

 @param PeiServices Pointer to the Pointer to the Pei Services Table.
 @param NotifyDescriptor The notification descriptor for this notification event.
 @param NotifyPpi Pointer to a  ppi.

 @retval EFI_SUCCESS If PPI is successfully installed.
*/
EFI_STATUS EFIAPI InstallFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES    **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *Ppi
) {
    EFI_STATUS               Status;
    AMI_FV_DXE_VALIDATE_PPI *FvDxeValidatePpi = NULL;
    AMI_PUBLISH_FV_DXE_PPI  *PublishFvDxePpi = NULL;
    VOID                    *AmiSigPpi = NULL;
    UINTN                    Instance = 0;
    
    //------------------------------------------------------------------------
    // AmiSigPPI must already be installed
    //------------------------------------------------------------------------
    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES **)PeiServices, &gAmiDigitalSignaturePPIGuid, 0, NULL, (void**)&AmiSigPpi);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;
    //------------------------------------------------------------------------
    //    Create PPI with ptr to FvDxe RomMap with the re-mapped addresses
    //    Use PPI inside PublishFV hook and update the Area->Address to RAM
    //------------------------------------------------------------------------
    FvDxeValidatePpi = (AMI_FV_DXE_VALIDATE_PPI *)AllocateZeroPool(sizeof(AMI_FV_DXE_VALIDATE_PPI));
    ASSERT (FvDxeValidatePpi != NULL);    
    if(FvDxeValidatePpi == NULL) return EFI_OUT_OF_RESOURCES;

    FvDxeValidatePpi->FvDxeValidate = VerifyRelocateFvDxe;
    FvDxeValidatePpi->AmiSigPpi     = AmiSigPpi;
    mFvDxeValidatePpiDesc.Ppi       = (VOID*) FvDxeValidatePpi;
    //------------------------------------------------------------------------
    //    Allocate dynamic placeholder for Valid FV Digest PPI
    //------------------------------------------------------------------------
    FvDxeValidatePpi->FvDigestPpi = (AMI_FV_DIGEST_PPI *)AllocateZeroPool(sizeof(AMI_FV_DIGEST_PPI));
    ASSERT (FvDxeValidatePpi->FvDigestPpi != NULL);    
    if(FvDxeValidatePpi->FvDigestPpi == NULL) return EFI_OUT_OF_RESOURCES;

    mFvDigestPpiDesc.Ppi = (VOID*) FvDxeValidatePpi->FvDigestPpi;
    //------------------------------------------------------------------------
    // Install FvDxeValidatePpi
    //------------------------------------------------------------------------
    Status = (*PeiServices)->InstallPpi ((CONST EFI_PEI_SERVICES **)PeiServices, &mFvDxeValidatePpiDesc);
    DEBUG((DEBUG_INFO, "Install FvDxeValidatePpi(%p): %r\n", FvDxeValidatePpi, Status));
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    // Launch FvDxeValidate for each instance of pending PublishFvDxePpi...
    Instance = 0;
    while ( !EFI_ERROR((*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES **)PeiServices, &gAmiPublishFvDxeValidatePpiGuid, Instance, NULL, (void**)&PublishFvDxePpi)) 
            && PublishFvDxePpi
    ) {
        DEBUG ((DEBUG_INFO, "%d:PublishFvDxePpi(%p)... Area %X(%l8X)\n", Instance, PublishFvDxePpi, (PublishFvDxePpi==NULL)?0:&PublishFvDxePpi->Area, (PublishFvDxePpi==NULL)?0:PublishFvDxePpi->Area.Address));
        Status = LaunchFvDxeVerifyPpi(PeiServices,NotifyDescriptor, PublishFvDxePpi);
        if(EFI_ERROR(Status))
            break;
        Instance++;
    }
    return Status;
}
/**
 The function will :
  Execute FvDxeValidate PPI for pending FV to be published after memory

 @param PeiServices Pointer to the Pointer to the Pei Services Table.
 @param NotifyDescriptor The notification descriptor for this notification event.
 @param NotifyPpi Pointer to a  ppi.

 @retval EFI_SUCCESS If PPI is successfully installed.
*/
EFI_STATUS EFIAPI LaunchFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES    **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *Ppi
) {
    EFI_STATUS              Status;
    AMI_FV_DXE_VALIDATE_PPI *FvDxeValidatePpi = NULL;
    AMI_PUBLISH_FV_DXE_PPI  *PublishFvDxePpi = NULL;
    AMI_ROM_AREA            *Area = NULL;
    
    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES **)PeiServices, &gAmiFvDxeValidatePpiGuid, 0, NULL, (void**)&FvDxeValidatePpi);
    DEBUG((DEBUG_INFO, "NotifyPpi (%X), FvDxeValidatePpi(%X) %r\n", Ppi, FvDxeValidatePpi, Status));
    if(Ppi == NULL || EFI_ERROR(Status))
        return EFI_NOT_READY;

    PublishFvDxePpi = (AMI_PUBLISH_FV_DXE_PPI *)Ppi;
    DEBUG ((DEBUG_INFO, "PublishFvDxePpi(%p)... Area %X(%l8X)\n", PublishFvDxePpi, (PublishFvDxePpi==NULL)?0:&PublishFvDxePpi->Area, (PublishFvDxePpi==NULL)?0:PublishFvDxePpi->Area.Address));
    Area = &PublishFvDxePpi->Area;
    Status = FvDxeValidatePpi->FvDxeValidate(FvDxeValidatePpi, (CONST EFI_PEI_SERVICES **)PeiServices, Area);
    if(!EFI_ERROR(Status)) {
        Status = AmiPublishFvArea(Area);
        DEBUG((DEBUG_INFO, "PublishFV %lX %r\n", Area->Address, Status));
        ASSERT_EFI_ERROR(Status);
    }        
    if (EFI_ERROR(Status)) {
    // Execute Action on failure to validate FV according to a TDXE Policy
//dbg: install FV_AFTER_MEM to test recovery
//            if (PcdGet32(AmiPcdTdxeFailedRecoveryPolicy) == 0 
//                && (Area->Attributes & (AMI_ROM_AREA_FV_PEI_MEM)) == AMI_ROM_AREA_FV_PEI_MEM
//                ) {
//                if(Area) AmiPublishFvArea(Area);
//            }
//dbg
        ExecuteActionOnFailureToValidateFv((CONST EFI_PEI_SERVICES **)PeiServices);
    }

    return Status;
}

/**
  Entry point to ValidatetFvDxeEntry PEIM.
  
  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval Status EFI_STATUS code
**/
EFI_STATUS
EFIAPI
ValidateFvDxeEntry(
    IN       EFI_PEI_FILE_HANDLE    FileHandle,
    IN CONST EFI_PEI_SERVICES       **PeiServices
) {
    VOID  *Ppi = NULL;
    // Must be invoked after MRC Memory initialized
    ASSERT_EFI_ERROR((*PeiServices)->LocatePpi(PeiServices, &gEfiPeiMemoryDiscoveredPpiGuid, 0, NULL, &Ppi));

    // Notify to install FwKey Hob and CryptoPpi. CryptoPPI has dependency on this PPI.
    (*PeiServices)->InstallPpi(PeiServices, &mAmiInstallCryptoPpi);
    // Publish/Launch FvDxeValidate PPI after CryptoPPI.gpAmiSigPPI as available
    (*PeiServices)->NotifyPpi(PeiServices, mReadyToInstallFvDxeVerify);
    // Launch FvDxeValidate on PublishFv notify
    (*PeiServices)->NotifyPpi(PeiServices, mReadyToLaunchFvDxeVerify);

    return EFI_SUCCESS;
}
