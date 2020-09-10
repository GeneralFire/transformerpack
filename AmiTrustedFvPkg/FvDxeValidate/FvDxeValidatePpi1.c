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

#include <Token.h>
#include <AmiPeiLib.h>
#include <Ppi/AmiFvDxeValidate.h>

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <CryptLib.h>
#include "AmiCertificate.h"
#include <Library/AmiRomLayoutLib.h>

#include <Library/DebugLib.h>
#include <Library/PerformanceLib.h>

//------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------
/// The GUID of the HOB containing Hash address list.
// cf96ae83-7ce7-4e3f-903f-b0f431cb5746
#define AMI_FV_DIGEST_HOB_GUID \
    {0xcf96ae83, 0x7ce7, 0x4e3f, { 0x90, 0x3f, 0xb0, 0xf4, 0x31, 0xcb, 0x57, 0x46} }
static EFI_GUID gAddrListHobGuid = AMI_FV_DIGEST_HOB_GUID;

/// AMI FvDxe signature hashed address list HOB
typedef struct _AMI_FV_DIGEST_INFO_HOB {
    EFI_HOB_GUID_TYPE    Header;
    UINTN                LenListOffs;
//    UINTN               AddrList[]; // List of mem ranges used to calculate final FvHash digest
} AMI_FV_DIGEST_INFO_HOB;

//------------------------------------------------------------------------
// External function prototypes
//------------------------------------------------------------------------
VOID ExecuteActionOnFailureToValidateFv(IN CONST EFI_PEI_SERVICES **PeiServices);

//------------------------------------------------------------------------
// Local prototypes
//------------------------------------------------------------------------
EFI_STATUS InstallFvDigestPpi(
    IN CONST EFI_PEI_SERVICES    **PeiServices,
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi
);

EFI_STATUS EFIAPI InstallFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES    **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID                      *NullPpi);

//------------------------------------------------------------------------
// Notify, Install PPI
//------------------------------------------------------------------------
//static EFI_PEI_NOTIFY_DESCRIPTOR mReadyToInstallFvDxeVerify[] =
//{
//    {
//        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
//        &gEfiPeiMemoryDiscoveredPpiGuid,
//        (EFI_PEIM_NOTIFY_ENTRY_POINT)InstallFvDxeVerifyPpi
//    },
//};

static EFI_PEI_NOTIFY_DESCRIPTOR mReadyToLaunchFvDxeVerify[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gAmiPublishFvDxeValidatePpiGuid,
        (EFI_PEIM_NOTIFY_ENTRY_POINT)InstallFvDxeVerifyPpi
    },
};

//------------------------------------------------------------------------
// Verify FvSig
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
Loads a binary from a GUIDed section of FV

@param[in]  FileGuid - Ffs Guid
@param[in]  pFV - ptr to FV

@param[out] Buffer - returns a pointer to allocated memory. Caller must free it when done
@param[out] Size  - returns the size of the binary loaded into the buffer.

@retval     Status
**/
EFI_STATUS
GetFfsRawImage (
   IN EFI_GUID *FileGuid,
   IN EFI_FIRMWARE_VOLUME_HEADER    *pFV,
   IN OUT VOID **Buffer,
   IN OUT UINT32 *Size)
{
  EFI_STATUS                    Status;
  EFI_FFS_FILE_HEADER           *ppFile=NULL;

  if (!Buffer || !Size)
    return EFI_INVALID_PARAMETER;

  ppFile = NULL;

  while(TRUE)
  {
      // Must enable PcdFrameworkCompatibilitySupport in order for routine to look for Ffs inside a FV
      Status = PeiServicesFfsFindNextFile (EFI_FV_FILETYPE_FREEFORM, pFV, (void**)&ppFile);
//      DEBUG ((DEBUG_INFO, "===>File %g, type %d %r\n", (EFI_GUID*)&ppFile->Name, ppFile->Type, Status));
      if(Status == EFI_NOT_FOUND)
      {
          break;
      }
      if(CompareGuid(&ppFile->Name, FileGuid)) 
      {
          // hopefully we found the file...
          *Size = FVFILE_SIZE(ppFile)-sizeof(EFI_FFS_FILE_HEADER)-sizeof(EFI_COMMON_SECTION_HEADER)-sizeof(EFI_GUID);
          *Buffer = (void*)ppFile;
          DEBUG ((DEBUG_INFO, "===>Ffs %g found in FV(addr %X), Guid Section len: 0x%X\n", (EFI_GUID*)&ppFile->Name, (UINT32)*Buffer, *Size));
          break;
      }
  }
  return Status;
}

// ----------------------------------------------------------------------------------------
// CryptoPPI replacement functions
// ----------------------------------------------------------------------------------------
// SDL defined Public Exponent E of RSA Key.
//
// Const defined in AmiCryptoPkg SDL
const UINT8  KeyE[] = {E_CONST}; // 0x10001
const UINTN  LenE = sizeof(KeyE);
//    PKCS_1 PSS Signature constant. Size of the Salt (random data) field in PSS signature.
const INT32  PssSaltlen = PSS_SIG_SALTLEN; // 0 - saltlen matches the size of hash

typedef struct{
    EFI_GUID   AlgGuid;
    UINT32     BlobSize;
    UINT8      *Blob;
} PEI_CRYPT_HANDLE;

/**
  Loads a binary from RAW section of FV_BB

  @param[in]  FileGuid Points to the EFI_GUID which identifies the FwKey file.
  @param[out] Buffer returns a pointer to allocated memory. Caller must free it when done
  @param[out] Size  returns the size of the binary loaded into the buffer.
  
  @retval     Status
**/
EFI_STATUS
CryptoGetFwKeyRaw (
   IN EFI_GUID *FileGuid, 
   IN OUT VOID **Buffer, 
   IN OUT UINT32 *Size)
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME_HEADER    *pFV;
  UINTN                         FvNum=0;
  EFI_FFS_FILE_HEADER           *ppFile=NULL;
  static EFI_GUID               pFV_BB_Guid = { 0x61C0F511, 0xA691, 0x4F54, { 0x97, 0x4F, 0xB9, 0xA4, 0x21, 0x72, 0xCE, 0x53 }};
  
  if (!Buffer || !Size) 
    return EFI_INVALID_PARAMETER;

  while(TRUE) {
      Status = PeiServicesFfsFindNextVolume(FvNum++, (void**)&pFV);
      DEBUG ((DEBUG_INFO, "FvNum %d : %g %r\n", FvNum-1, (EFI_GUID*)((UINTN)pFV+pFV->ExtHeaderOffset),Status));
      if(EFI_ERROR(Status)) return Status;
      //
      // FwKey location is in FV_BB only
      //
      if(CompareGuid((EFI_GUID*)((UINTN)pFV+pFV->ExtHeaderOffset), &pFV_BB_Guid) == 0)
        continue;

      Status = GetFfsRawImage (FileGuid, pFV, (VOID **)&ppFile, Size);
      if(!EFI_ERROR(Status)) {
          break;
      }
  }
    // hopefully we found the file...now try to read raw data
    // !!! Keys are uncompressed. There is no much reason to run compression on prime numbers anyway
  Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, ppFile, Buffer);
  if(!EFI_ERROR(Status)) {
    // Size may need to subtract Section hdr size = 28 bytes sizeof(EFI_FFS_FILE_HEADER + EFI_COMMON_SECTION_HEADER)
    *Size = FVFILE_SIZE(ppFile)-sizeof(EFI_FFS_FILE_HEADER)-sizeof(EFI_COMMON_SECTION_HEADER);
  }

  DEBUG ((DEBUG_INFO, "===>FwKey Ffs %r\n(addr %X, len 0x%x) [%X...]\n", Status, (UINT32)*Buffer, *Size, *((UINT32*)*Buffer)));
  return Status;
}
/**
  The function locates a FwKey Hob pointing to a
  Platform Signing Key (PK) inside FV_BB ffs

  @param[out]  pFwKeyHob Pointer to pFwKeyHob

  @retval     Status
    EFI_SUCCESS               The Key Hob was found
    EFI_NOT_FOUND             The Key Hob not found
**/

EFI_STATUS
CryptoGetFwKeyHob (
    IN OUT FW_KEY_HOB **pFwKeyHob
){
    EFI_STATUS       Status = EFI_NOT_FOUND;
    CONST EFI_PEI_SERVICES **PeiServices;
    VOID            *FirstHob;

    PeiServices = GetPeiServicesTablePointer ();
    if (*PeiServices == NULL) ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    (*PeiServices)->GetHobList(PeiServices, &FirstHob);
    if (!FirstHob) ASSERT_EFI_ERROR (EFI_NOT_FOUND);

    while ( !EFI_ERROR(Status = FindNextHobByType(EFI_HOB_TYPE_GUID_EXTENSION, &FirstHob)) ) {
     if ( CompareGuid(&((EFI_HOB_GUID_TYPE*)FirstHob)->Name, &gPRKeyGuid) ) {
         break;
     }
    }
    if (!EFI_ERROR(Status)) 
        *pFwKeyHob = (FW_KEY_HOB *)FirstHob;

    return Status;
}

/**
  Function returns Ptr to a Platform Signing Key (PK) Ffs inside Recovery FV (FV_BB or similar)

  @param[in]   KeyAlgorithm Points to the EFI_GUID which identifies the PKpub algorithm to use.
  @param[out]  PublicKey    Handle to a key used to return a ptr to a Key. This handle must be identifying a public key.

  @retval     Status
    EFI_SUCCESS               The Key is successfully returned.
    EFI_NOT_FOUND             The Key not found
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
GetKey (
  IN CONST EFI_GUID          *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN OUT PEI_CRYPT_HANDLE        *PublicKey
  )
{
    EFI_STATUS      Status;
    FW_KEY_HOB *pFwKeyHob;

    if(PublicKey == NULL /*|| KeyAlgorithm == NULL*/)
        return EFI_INVALID_PARAMETER;

    // now only supporting PKpub key comparison
//    if(CompareGuid((EFI_GUID*)KeyAlgorithm, &gPRKeyGuid)==0)
//        return EFI_INVALID_PARAMETER;

    // Acquire FwKey hob
    pFwKeyHob = NULL;
    Status = CryptoGetFwKeyHob(&pFwKeyHob);
    if (!EFI_ERROR(Status) && pFwKeyHob) {
//        PublicKey->AlgGuid = pFwKeyHob->KeyGuid;
        CopyGuid (&PublicKey->AlgGuid, &pFwKeyHob->KeyGuid);
        PublicKey->Blob = (UINT8*)(UINTN)(pFwKeyHob->KeyAddress);
        PublicKey->BlobSize = pFwKeyHob->KeySize;

        return Status;
    } 
    Status = CryptoGetFwKeyRaw (&gPKeyFileSha256Guid, (VOID **)&(PublicKey->Blob), &(PublicKey->BlobSize));
    CopyGuid (&PublicKey->AlgGuid, &gEfiCertSha256Guid);
    return Status;
}
/**
  Function verifies that the specified signature matches the specified hash. 
  Verifies the RSA-SSA signature with EMSA-PKCS1-v1_5 encoding scheme defined in
  RSA PKCS#1.
  This function decrypts the signature with the provided key and then compares 
  the decrypted value to the specified hash value

  @param[in]  PublicKey     Handle to a key used for verifying signatures. This handle must be identifying a public key.
  @param[in]  Hash          Handle of the hash object to verify.
  @param[in]  Signature     Pointer to the signature data to be verified.
  @param[in]  SignatureSize The size, in bytes, of the signature data.
  @param[in]  Flags        Specifies additional flags to further customize the signing/verifying behavior.

  @retval     Status
    EFI_SUCCESS               The signature is successfully verified.
    EFI_SECURITY_VIOLATION    The signature does not match the given message.
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
VerifySig
(
  IN PEI_CRYPT_HANDLE            *PublicKey,
  IN PEI_CRYPT_HANDLE            *Hash,
  IN VOID                        *Signature,
  IN UINTN                       SignatureSize,
  IN UINT32                      Flags
  )
{
    EFI_STATUS      Status;
    INTN            err;
    struct          crypto_rsa_key *key = NULL;
    size_t         *sig_len=(size_t*)&SignatureSize;
    INT32           pss_modulus_bitlen;
    INT32           pss_saltlen;
    UINT32          HashLen;
    UINT8           DecriptedSig[DEFAULT_RSA_SIG_LEN];

// Only supporting RSASSA_PKCS1V15 signature types
    if(!((Flags & EFI_CRYPT_RSASSA_PKCS1V15) || 
         (Flags & EFI_CRYPT_RSASSA_PSS)) )
            return EFI_INVALID_PARAMETER;

    ResetCRmm();

// check Key handle if requested PubKey is a Platform FW Key
// In this case use the FW key from ffs image
// validity of the Key must be verified prior to calling VerifySig
// Guid must match one of the valid keys we can use in PEI Sig verification.
// For now Public Key is supported in 2 formats: RAW 256 bytes and ASN.1 Integer
// shall be extended to gPBkey_x509_Guid
    if(CompareGuid(&PublicKey->AlgGuid, &gEfiCertRsa2048Guid)==0)
        return EFI_INVALID_PARAMETER;

    key = crypto_import_rsa2048_public_key(PublicKey->Blob, PublicKey->BlobSize, (UINT8*)&KeyE, LenE);

    if(key == NULL )
        err = -1;
    else
        err = crypto_rsa_exptmod((const UINT8*)Signature, (size_t)SignatureSize, (UINT8*)&DecriptedSig, sig_len, key, 0);
// locate Hash inside the decrypted signature body and compare it with given Hash;
// Should be extended to handle sha1, sha256 hashes. use Hash->AlgGuid to determine the Hash type
    if(!err) 
    {
        HashLen = SHA256_DIGEST_SIZE;
        if(Flags & EFI_CRYPT_RSASSA_PKCS1V15)
        {
            // Validate PKCS#1v1.5 Padding
            err = pkcs_1_v1_5_decode((const UINT8 *)Hash->Blob, HashLen, (const UINT8 *)&DecriptedSig, (unsigned long)*sig_len);
        } else { //(Flags & EFI_CRYPT_RSASSA_PSS))
            // Validate PKCS#1 PSS Signature: padding & hash
            pss_saltlen = (PssSaltlen!=0)?(INT32)PssSaltlen:HashLen;
            pss_modulus_bitlen = DEFAULT_RSA_SIG_LEN << 3;
            err = pkcs_1_pss_decode(
                    Hash->Blob, HashLen, 
                    (const unsigned char *)&DecriptedSig, (unsigned long)*sig_len, 
                    pss_saltlen,  pss_modulus_bitlen);
        }
    }

    Status = !err ? EFI_SUCCESS:EFI_SECURITY_VIOLATION;

    // clear memory heap on exit
    ResetCRmm();

    return Status;
}
/**
  Function compares the input PublicKey against Platform Signing Key (PK) image in the flash.

  @param[in]   KeyAlgorithm    Points to the EFI_GUID which identifies the PKpub algorithm to use.
  @param[in]   PublicKey       Handle to a key used for verifying signatures.  This handle must be identifying a public key.

  @retval     Status
    EFI_SUCCESS               The Key is successfully verified.
    EFI_SECURITY_VIOLATION    The Key does not match current FW key.
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
VerifyKey
(
  IN CONST EFI_GUID          *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN PEI_CRYPT_HANDLE        *PublicKey
  )
{
    EFI_STATUS    Status;
    int           err;
    PEI_CRYPT_HANDLE KeyHndl;
    UINT8         Hash[SHA256_DIGEST_SIZE] = {0};
    UINT8         *KeyBuf, *PKpubBuffer=NULL;
    size_t        KeyLen;

    if(PublicKey == NULL || KeyAlgorithm == NULL || PublicKey->Blob == NULL)
        return EFI_INVALID_PARAMETER;

// check Key handle if requested PubKey is a Platform Key PKpub.
// In this case use PKpub key from ffs image
//    if(CompareGuid((EFI_GUID*)KeyAlgorithm, &gPRKeyGuid)==0)
//        return EFI_INVALID_PARAMETER;
    
    Status = GetKey(KeyAlgorithm, &KeyHndl);
    if(EFI_ERROR(Status))
        return Status;

    PKpubBuffer = KeyHndl.Blob;
    KeyLen = (size_t)KeyHndl.BlobSize; // always 256
    KeyBuf = PublicKey->Blob;
//  FWKey : hash
    if(CompareGuid(&KeyHndl.AlgGuid, &gEfiCertSha256Guid)==0){
        return EFI_SECURITY_VIOLATION;
    }
    KeyBuf = Hash;
    //    pubkey : Rsa2048 - hash it
//        if(CompareGuid(&PublicKey->AlgGuid, &gEfiCertRsa2048Guid) ){
    // SHA256 Hash of RSA Key
        KeyLen = (size_t)PublicKey->BlobSize;
        sha256_vector(1, (const UINT8**)&PublicKey->Blob, (const size_t*)&KeyLen, KeyBuf);
//        } 

//    KeyLen = SHA256_DIGEST_SIZE;
    err = MemCmp(PKpubBuffer, KeyBuf, SHA256_DIGEST_SIZE);

    return (!err ? EFI_SUCCESS:EFI_SECURITY_VIOLATION);
}

/**
The Rom image hash is calculated according to Rom Area map

 @param[in]   ThisPpi - pointer to a FvDxeValidatePpi struct
 @param[in]   FvSigHeader - pointer to a FW Capsule Hdr
 @param[in]   HashAlgorithm
 @param[in]   HashDB

 @retval      Status 
              EFI_SUCCESS            Your hash is in the output buffer
              EFI_SECURITY_VIOLATION Something went wrong
**/
#ifndef sha256_state
struct sha256_state {
    UINT64 length;
    UINT32 state[8], curlen;
    UINT8  buf[64];
};
#endif
void sha256_init(struct sha256_state *md);
int sha256_process(struct sha256_state *md, const unsigned char *in, unsigned long inlen);
int sha256_done(struct sha256_state *md, unsigned char *out);

EFI_STATUS HashFwRomMapImage (
    IN AMI_FV_DXE_VALIDATE_PPI  *ThisPpi,
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader,
    IN EFI_GUID                 *HashAlgorithm,
    IN OUT UINT8                *HashDB
){
    EFI_STATUS  Status;
    ROM_AREA    *RomAreaTbl;
    UINTN        RomSize = FLASH_SIZE;
    UINTN        i, RomMap_size, max_num_elem;

    struct sha256_state Sha2CtxStruct;
    VOID         *Sha2Ctx=&Sha2CtxStruct;

    UINTN *AddrList;
    UINTN *LenList;

    UINTN HobSize;
    AMI_FV_DIGEST_INFO_HOB *pHob;
    CONST EFI_PEI_SERVICES **PeiServices;

    PeiServices = GetPeiServicesTablePointer ();
    if (*PeiServices == NULL) ASSERT_EFI_ERROR (EFI_NOT_FOUND);

//    Sha256Init( Sha2Ctx );
    sha256_init((struct sha256_state *)Sha2Ctx);

    RomAreaTbl = (ROM_AREA *)((UINTN)FvSigHeader+FvSigHeader->RomLayoutOffset);
    RomMap_size = FvSigHeader->RomImageOffset-FvSigHeader->RomLayoutOffset;
    max_num_elem = RomMap_size/sizeof(ROM_AREA);
    // at least one signed range must be included
    if(max_num_elem == 0) return EFI_SECURITY_VIOLATION;

    i = (max_num_elem+1)*sizeof(UINTN); // add 1 extra entry for FvSigHdr
//    AddrList = (UINTN *)AllocateZeroPool(i*2);
    HobSize = sizeof(AMI_FV_DIGEST_INFO_HOB)+(i*2);
    Status = (*PeiServices)->CreateHob( PeiServices,
        EFI_HOB_TYPE_GUID_EXTENSION, 
        (UINT16)HobSize,
        (void**)&pHob);
//    DEBUG ((DEBUG_INFO, "AddrList Hob %X [sz %X, LenOffs %X]...\n", pHob, HobSize, i));
    if (EFI_ERROR(Status))
        return Status;
    CopyGuid (&(pHob->Header.Name), &gAddrListHobGuid);
    pHob->LenListOffs = i;
    AddrList = (UINTN*)((UINTN)pHob + sizeof(AMI_FV_DIGEST_INFO_HOB));
    ASSERT (AddrList != NULL);    
    if(AddrList == NULL) 
        return EFI_OUT_OF_RESOURCES;
    LenList = (UINTN*)((UINTN)AddrList + i);
//    DEBUG ((DEBUG_INFO, "AddrList %X, LenList %X (OffsFromAddrList %X)\n", AddrList, LenList, pHob->LenListOffs));

    ThisPpi->FvDigestPpi->AddrList = AddrList;
    ThisPpi->FvDigestPpi->LenList = LenList;
    ThisPpi->FvDigestPpi->HashElemCnt = 0;

    for(i=0; i < max_num_elem && RomAreaTbl[i].Size != 0; i++)
    {
        if(!IsFvMeasured(RomAreaTbl[i].Attributes) ||
           !(RomAreaTbl[i].Attributes & AMI_ROM_AREA_SIGNED))
            continue;

        // sanity check for buffer overruns
        if(RomAreaTbl[i].Offset > RomSize ||
           ((UINT64)RomAreaTbl[i].Offset + RomAreaTbl[i].Size) > RomSize )
            return EFI_SECURITY_VIOLATION;

        DEBUG ((DEBUG_INFO, "%2d(%2d):Hash Range: %X-%X\n", i+1,max_num_elem, (const VOID*)(UINTN)RomAreaTbl[i].Address, RomAreaTbl[i].Size));
//        Sha256Update(Sha2Ctx,(const VOID*)(UINTN)RomAreaTbl[i].Address, RomAreaTbl[i].Size);
        sha256_process(Sha2Ctx,(const VOID*)(UINTN)RomAreaTbl[i].Address, (unsigned long)RomAreaTbl[i].Size);
        AddrList[ThisPpi->FvDigestPpi->HashElemCnt] = (UINTN)RomAreaTbl[i].Address;
        LenList[ThisPpi->FvDigestPpi->HashElemCnt] = RomAreaTbl[i].Size;
        ThisPpi->FvDigestPpi->HashElemCnt++;
    }
    // at least one signed range must be included
    if(ThisPpi->FvDigestPpi->HashElemCnt == 0 || 
       ThisPpi->FvDigestPpi->HashElemCnt > max_num_elem) return EFI_SECURITY_VIOLATION;
    //
    // Does not apply for TcgMeasured mode
    // Hash of Capsule Hdr + FW Certificate Hdr
    //
    if(FvSigHeader->CapHdr.Flags & CAPSULE_FLAGS_CAPHDR_IN_SIGNCERT) {
        DEBUG ((DEBUG_INFO, "%2d(%2d):Hash Range: %X-%X\n", i+1,max_num_elem, (const VOID*)(UINTN)FvSigHeader, (UINTN)&(FvSigHeader->FWCert.SignCert.CertData) - (UINTN)FvSigHeader));
//        Sha256Update(Sha2Ctx, (const VOID*)(UINTN)FvSigHeader, (UINTN)&(FvSigHeader->FWCert.SignCert.CertData) - (UINTN)FvSigHeader);
        sha256_process(Sha2Ctx, (const VOID*)(UINTN)FvSigHeader, (unsigned long)((UINTN)&(FvSigHeader->FWCert.SignCert.CertData) - (UINTN)FvSigHeader));
        AddrList[ThisPpi->FvDigestPpi->HashElemCnt] = (UINTN)RomAreaTbl[i].Address;
        LenList[ThisPpi->FvDigestPpi->HashElemCnt] = RomAreaTbl[i].Size;
        ThisPpi->FvDigestPpi->HashElemCnt++;
        if(ThisPpi->FvDigestPpi->HashElemCnt > max_num_elem)
            return EFI_SECURITY_VIOLATION;
    }

//    Sha256Final( Sha2Ctx, HashDB );
    Status = (sha256_done(Sha2Ctx, HashDB )==0)?EFI_SUCCESS:EFI_SECURITY_VIOLATION;

    DEBUG_CODE ( for (i=0; i<16; i++) DEBUG ((DEBUG_INFO," %02X", (HashDB[i]) )); DEBUG ((DEBUG_INFO, "\nFvDxe Hash : %r\n", Status)); );

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

@param[in]   ThisPpi - pointer to a FvDxeValidatePpi struct
@param[in]   FvSigHeader - pointer to a FW Capsule Hdr

@retval      Status 
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

    if(CompareGuid((EFI_GUID*)&FvSigHeader->FWCert.SignCert.Hdr.CertType, &gEfiCertTypeRsa2048Sha256Guid)==0) {
        DEBUG((DEBUG_ERROR,"Invalid Certificate type %g\n",(EFI_GUID*)&FvSigHeader->FWCert.SignCert.Hdr.CertType));
        return EFI_UNSUPPORTED;
    }

    Flags = (FvSigHeader->CapHdr.Flags & CAPSULE_FLAGS_RSA_PSS_PADDING_SCHEME)?EFI_CRYPT_RSASSA_PSS:EFI_CRYPT_RSASSA_PKCS1V15;

    HashHndl.AlgGuid  = gEfiHashAlgorithmSha256Guid;
    HashHndl.BlobSize = SHA256_DIGEST_SIZE;
    HashHndl.Blob = &(ThisPpi->FvDigestPpi->FvHash[0]); // use the as a scratch Hash buffer

//
// 1. Compare Capsule's Sign Cert key with Platform Root Key
//
    PubKeyHndl.BlobSize =  DEFAULT_RSA_KEY_MODULUS_LEN;
    PubKeyHndl.AlgGuid = gEfiCertRsa2048Guid;
    PubKeyHndl.Blob = (UINT8*)FvSigHeader->FWCert.SignCert.CertData.PublicKey;
    Status = VerifyKey(&gPRKeyGuid, &PubKeyHndl); 
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
            Status = VerifyKey(&gPRKeyGuid, &PubKeyHndl); 
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
            sha256_vector(1, (const UINT8**)&Addr, (const size_t*)&Size, HashHndl.Blob);

            pSig = (void*)pRootCert->Signature; 
            Status = VerifySig(&PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags ); 
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
    PERF_START (NULL, "Hash", NULL, 0);
    Status = HashFwRomMapImage(ThisPpi, FvSigHeader, &gEfiHashAlgorithmSha256Guid,  HashHndl.Blob);
    PERF_END (NULL, "Hash", NULL, 0);
    if (EFI_ERROR(Status)) return Status;

    ThisPpi->FvDigestPpi->HashSize = SHA256_DIGEST_SIZE;
    ThisPpi->FvDigestPpi->HashAlgId = AMI_TDXE_SHA256;

    pSig = (void*)FvSigHeader->FWCert.SignCert.CertData.Signature; 
    PubKeyHndl.Blob = (UINT8*)FvSigHeader->FWCert.SignCert.CertData.PublicKey;

    PERF_START (NULL, "Pkcs1", NULL, 0);
    Status = VerifySig(&PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags);
    PERF_END (NULL, "Pkcs1", NULL, 0);
    return Status;
}

/**
This function finds the FvSig Header

@param FvAddress     FV address
@param Size          Size of the FV
@param FvSigHeader   FvSig Header pointer

@retval EFI_SUCCESS  Found guided encrypted section.
**/
EFI_STATUS
LocateFvDxeSigHeader(
    UINT64                      FvAddress,
    UINT32                      Size,
    APTIO_FW_CAPSULE_HEADER   **FvSigHeader
) {
    EFI_STATUS Status;
    EFI_FFS_FILE_HEADER       *FfsFileHeader;
    APTIO_FW_CAPSULE_HEADER   *pFvSigHeader;
    //optional ffs guid for the ROM hole :{0x05ca020a, 0x0fc1,0x11dc, {0x90, 0x11, 0x00, 0x17, 0x31, 0x53, 0xeb, 0xa8}};
    static EFI_GUID  gFvDxeFFSGuid    = {0x68d1fed0, 0x143e, 0x479c, {0xbf, 0x39, 0x45, 0x6e, 0x98, 0xb4, 0xfa, 0xc5}};

    Status = GetFfsRawImage (&gFvDxeFFSGuid, (EFI_FIRMWARE_VOLUME_HEADER*)(UINTN)FvAddress, (void**)&FfsFileHeader, &Size);
    if(!EFI_ERROR(Status)) {
        // Found FV_DATA FFS, let's locate the FvSigHdr
        pFvSigHeader = (APTIO_FW_CAPSULE_HEADER*)((UINT8*)FfsFileHeader+\
                            sizeof(EFI_FFS_FILE_HEADER)+\
                            sizeof(EFI_COMMON_SECTION_HEADER)+\
                            sizeof(EFI_GUID));
        if(CompareGuid(&gFWCapsuleGuid, &(pFvSigHeader->CapHdr.CapsuleGuid))) {
            // More Hdr fields sanity checks for buffer overruns
            // Signed Aptio Capsule only supports WIN_CERT_TYPE_EFI_GUID 
            if(((UINT16)*(UINT16*)FfsFileHeader->Size != pFvSigHeader->CapHdr.HeaderSize +\
                                                sizeof(EFI_FFS_FILE_HEADER)+\
                                                sizeof(EFI_COMMON_SECTION_HEADER)+\
                                                sizeof(EFI_GUID) ) || 
                pFvSigHeader->FWCert.SignCert.Hdr.Hdr.wCertificateType != WIN_CERT_TYPE_EFI_GUID ||
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
    IN CONST EFI_PEI_SERVICES   **PeiServices,
    IN OUT AMI_ROM_AREA         *AmiRomArea
) {
    EFI_STATUS    Status;
    IN APTIO_FW_CAPSULE_HEADER  *FvSigHeader;
    AMI_ROM_AREA *Area;
    ROM_AREA     *Area1;
    BOOLEAN      bRangeValid;

    if(PeiServices == NULL || 
       ThisPpi     == NULL || 
       ThisPpi->FvDigestPpi == NULL)
        return EFI_NOT_READY;

   // to skip re-entry if previous test was successful
    if(ThisPpi->bFvDxeProcessed) {
        Status = (ThisPpi->bFvDxeStatus==TRUE)?EFI_SUCCESS:EFI_SECURITY_VIOLATION;
        DEBUG((DEBUG_INFO, "VerifyFvDxe : %r\n", Status));
        return Status;
    }
/*
 * TBD
 * Each FV can be signed individually and have FvDxeSig.ffs for the single FV range
 * If input "Area" parameter is NULL -> verify all FVs in a loop, else 
 *  verify each FV area individually.
 * Common flow: if FV Measured attrib. -> relocate(PCD policy), locate FvDxesig.ffs, VerifySig, InstallDigestPPI 
 * 
 * for(Area0 = AmiGetFirstRomArea(); Area0 != NULL; Area0 = AmiGetNextRomArea(Area0)) {
 *  
 *  IsMeasured() -Shadow FV to mem.
 *      TBD:set MemShadow attribute (may need to defined a new attribute or use reserved bits)
 *      #define AMI_ROM_AREA_SHADOWED_IN_MEMORY  0x00010000
 *      Rom_Area->Attribute bits to indicate the FV was re-located or not
 *      
 *  Search for FvDxesig.ffs in Area 
 *    If Found -> 
 *     update RomRanges in FvSig rom map with addresses in MEM 
 *     1. run validation
 *     2. set ThisPpi->bFvDxeStatus to TRUE/FALSE
 * 
 *   Continue to next Area to search for signed FvDxeSig
 *  }
 *  Pros: Allow to sign each FV individually by ODM, OEM IBV etc., so they can be replaced in the BIOS image individually
 *  Cons: Only in a scenario when FVs are shadowed to RAM (PCD policy)
 *        and all FVs are signed as one image, only one FV containing FvSig ffs will be shadowed.
 *
 *        TCG must locate all instances of FvDigestPPI to extend each measured FV digest to the PCR0
 *
 */
    PERF_START (NULL, "T-Dxe", NULL, 0);
    FvSigHeader = NULL;
    for(Area = AmiGetFirstRomArea(); Area != NULL; Area = AmiGetNextRomArea(Area)) {
        DEBUG ((DEBUG_INFO, "FV %8lX(%8X)=%g, Attr %4X\n", Area->Address, Area->Size, (EFI_GUID*)((UINTN)Area->Address+((EFI_FIRMWARE_VOLUME_HEADER*)(UINTN)(Area->Address))->ExtHeaderOffset), Area->Attributes));
        if(!IsFvMeasured(Area->Attributes) )
            continue;
        if(FvSigHeader == NULL) {
            if(!EFI_ERROR(LocateFvDxeSigHeader(Area->Address, Area->Size, &FvSigHeader)))
                break;
        }
    }

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
//                    DEBUG((DEBUG_INFO, "--->SgnRange: %8lX-%X\n", Area1->Address, Area1->Size));
            }
        }
        if(bRangeValid == FALSE) {
            DEBUG((DEBUG_ERROR,"Invalid FvDxeSig Hdr.RomMap\n"));
            PERF_END (NULL, "T-Dxe", NULL, 0);
            return EFI_NOT_FOUND; // go straight to recovery
        }
    }
    // Begin Authentication
    Status = VerifyFwCertRsa2048Sha256(ThisPpi, FvSigHeader);
    DEBUG((DEBUG_INFO, "VerifyFvDxeCert: %r\n", Status));
    if(!EFI_ERROR(Status)) {
        ThisPpi->bFvDxeStatus = TRUE; // to avoid re-entry if validate was successful
        //-----------------------------------------------------------------------------------
        // Install FvDigestPPI
        //-----------------------------------------------------------------------------------
        Status = InstallFvDigestPpi(PeiServices, ThisPpi);
        ASSERT_EFI_ERROR(Status);
    }
    PERF_END (NULL, "T-Dxe", NULL, 0);

    return Status;
}
//---------------------------------------------------------------------------------------------------
// End PPI
//---------------------------------------------------------------------------------------------------
/**
 The function will publish a FvDigestPpi PPI after successful FV Dxe verification.

 @param ThisPpi Pointer to the FvDxeValidate Ppi data structure

 @retval EFI_SUCCESS If FV_MAIN is successfully verified.
*/
EFI_STATUS
InstallFvDigestPpi(
    IN CONST EFI_PEI_SERVICES    **PeiServices,
    IN AMI_FV_DXE_VALIDATE_PPI    *ThisPpi
) {
    EFI_STATUS              Status;
    EFI_PEI_PPI_DESCRIPTOR *pPpiDesc;

/*
 * Case for individually signed FVs. Multiple instances of FvDigestPPI will be installed
 * 
      AMI_FV_DIGEST_PPI       *FvDigestPpi; // Local
        //------------------------------------------------------------------------
        //    Allocate dynamic placeholder for Valid FV Digest PPI
        //------------------------------------------------------------------------
        Status = (**PeiServices).AllocatePool(PeiServices,
                                        sizeof(AMI_FV_DIGEST_PPI),
                                        (VOID**)&FvDigestPpi
                                        );
        if (EFI_ERROR(Status)) {
            return Status;
        }
        CopyMem(&FvDigestPpi, &ThisPpi->FvDigestPpi, sizeof(AMI_FV_DIGEST_PPI));
 * 
 */
    //--------------------------------------------------------------------------
    //   Allocate dynamic placeholder for Descriptor PPI
    //--------------------------------------------------------------------------
    pPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *)AllocateZeroPool(sizeof(EFI_PEI_PPI_DESCRIPTOR));
    ASSERT (pPpiDesc != NULL);    
    if(pPpiDesc == NULL) return EFI_OUT_OF_RESOURCES;
//    pPpiDesc->Ppi = (VOID*) FvDigestPpi;
    pPpiDesc->Ppi = (VOID*) ThisPpi->FvDigestPpi;
    pPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    pPpiDesc->Guid = &gAmiFvDigestPpiGuid;

    //------------------------------------------------------------------------
    // Install FvDxeValidatePpi
    //------------------------------------------------------------------------
    Status = (*PeiServices)->InstallPpi (PeiServices, pPpiDesc);
    DEBUG((DEBUG_INFO, "Install FvDigestPpi %X: %r\n", pPpiDesc->Ppi, Status));
    ASSERT_EFI_ERROR(Status);

    return Status;
}

/**
 This callback function will publish a FvDxeValidate PPI on gAmiPublishFvDxeValidatePpiGuid signal event

 @param PeiServices Pointer to the Pointer to the Pei Services Table.
 @param NotifyDescriptor The notification descriptor for this notification event.
 @param NotifyPpi Pointer to a ppi.

 @retval EFI_SUCCESS If PPI is successfully installed.
*/
EFI_STATUS EFIAPI
InstallFvDxeVerifyPpi(
    IN EFI_PEI_SERVICES    **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID                      *NotifyPpi
) {
    EFI_STATUS               Status;
    AMI_FV_DXE_VALIDATE_PPI *FvDxeValidatePpi = NULL;
    EFI_PEI_PPI_DESCRIPTOR  *pPpiDesc;
    AMI_PUBLISH_FV_DXE_PPI  *PublishFvDxePpi;
    AMI_ROM_AREA            *Area = NULL;

    AMI_FV_DIGEST_INFO_HOB *AddrListHob;
    AMI_FV_DIGEST_PPI *pFvDigestPpi;

    Status = (*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES**)PeiServices, &gAmiFvDxeValidatePpiGuid, 0, NULL, (void**)&FvDxeValidatePpi);
    DEBUG((DEBUG_INFO, "NotifyPpi (%X), FvDxeValidatePpi(%X) %r\n", NotifyPpi, FvDxeValidatePpi, Status));
    if(EFI_ERROR(Status)) {

        //------------------------------------------------------------------------
        //    Create PPI with ptr to FvDxe RomMap with the re-mapped addresses
        //    Use PPI inside PublishFV hook and update the Area->Address to RAM
        //------------------------------------------------------------------------
        FvDxeValidatePpi = (AMI_FV_DXE_VALIDATE_PPI *)AllocateZeroPool(sizeof(AMI_FV_DXE_VALIDATE_PPI));
        ASSERT (FvDxeValidatePpi != NULL);    
        if(FvDxeValidatePpi == NULL) return EFI_OUT_OF_RESOURCES;

        FvDxeValidatePpi->FvDxeValidate = VerifyRelocateFvDxe;
        //    mFvDxeValidatePpiDesc.Ppi = (VOID*) FvDxeValidatePpi;
        //------------------------------------------------------------------------
        //    Allocate dynamic placeholder for Valid FV Digest PPI
        //------------------------------------------------------------------------
        FvDxeValidatePpi->FvDigestPpi = (AMI_FV_DIGEST_PPI *)AllocateZeroPool(sizeof(AMI_FV_DIGEST_PPI));
        ASSERT (FvDxeValidatePpi->FvDigestPpi != NULL);    
        if(FvDxeValidatePpi->FvDigestPpi == NULL) return EFI_OUT_OF_RESOURCES;

        //mFvDigestPpiDesc.Ppi = (VOID*) FvDxeValidatePpi->FvDigestPpi;
        //--------------------------------------------------------------------------
        //   Allocate dynamic placeholder for Descriptor PPI
        //--------------------------------------------------------------------------
        pPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *)AllocateZeroPool(sizeof(EFI_PEI_PPI_DESCRIPTOR));
        ASSERT (pPpiDesc != NULL);    
        if(pPpiDesc == NULL) return EFI_OUT_OF_RESOURCES;

        pPpiDesc->Ppi = (VOID*) FvDxeValidatePpi;
        pPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
        pPpiDesc->Guid = &gAmiFvDxeValidatePpiGuid;
        //------------------------------------------------------------------------
        // Install FvDxeValidatePpi
        //------------------------------------------------------------------------
        Status = (*PeiServices)->InstallPpi ((CONST EFI_PEI_SERVICES**)PeiServices, pPpiDesc);
        DEBUG((DEBUG_INFO, "Install FvDxeValidatePpi(%X) PeiServices(%X): %r\n", FvDxeValidatePpi, PeiServices, Status));
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) 
            return Status;
    }
    // Run FvDxeValidate if PublishFvDxePpi pending or if launched from a PEI entry point...
    PublishFvDxePpi = (AMI_PUBLISH_FV_DXE_PPI *)NotifyPpi;
    DEBUG ((DEBUG_INFO, "PublishFvDxePpi(%X), Area %X(%l8X)\n", PublishFvDxePpi, (PublishFvDxePpi==NULL)?0:&PublishFvDxePpi->Area, (PublishFvDxePpi==NULL)?0:PublishFvDxePpi->Area.Address));
    if(PublishFvDxePpi)
        Area = &PublishFvDxePpi->Area;

    Status = FvDxeValidatePpi->FvDxeValidate(FvDxeValidatePpi, (CONST EFI_PEI_SERVICES **)PeiServices, Area);
    if(!EFI_ERROR(Status) && Area) {
        Status = AmiPublishFvArea(Area);
        DEBUG((DEBUG_INFO, "Publish FV %X(%lX) %r\n", Area, Area->Address, Status));

        if (!EFI_ERROR((*PeiServices)->LocatePpi((CONST EFI_PEI_SERVICES**)PeiServices, &gAmiFvDigestPpiGuid, 0, NULL, (VOID**)&pFvDigestPpi))) {
//            DEBUG((DEBUG_INFO, "FvDigestPpi:\nBefore %X\nAfter  %X\n", FvDxeValidatePpi->FvDigestPpi, pFvDigestPpi));
//            DEBUG_CODE ( for (int i=0; i<32; i++) DEBUG ((DEBUG_INFO," %02X", pFvDigestPpi->FvHash[i] )); DEBUG((DEBUG_INFO, "\nAddrList %X, HashElemCnt %d\n", pFvDigestPpi->AddrList, pFvDigestPpi->HashElemCnt)););
    // update address to AddrList/LenList in RAM
            FvDxeValidatePpi->FvDigestPpi = pFvDigestPpi;
    // Locate Hob
            (*PeiServices)->GetHobList((CONST EFI_PEI_SERVICES **)PeiServices, (VOID**)&AddrListHob);
            if (!AddrListHob) ASSERT_EFI_ERROR (EFI_NOT_FOUND);
            Status = FindNextHobByGuid (&gAddrListHobGuid, (VOID**)&AddrListHob);
//            DEBUG ((DEBUG_INFO, "AddrList Hob %X: %r\n", AddrListHob, Status));
            if (!EFI_ERROR(Status)) {
                pFvDigestPpi->AddrList = (UINTN*)((UINTN)AddrListHob + sizeof(AMI_FV_DIGEST_INFO_HOB));
                pFvDigestPpi->LenList = (UINTN*)((UINTN)pFvDigestPpi->AddrList + AddrListHob->LenListOffs);
//                DEBUG ((DEBUG_INFO, "AddrList %X, LenList %X (OffsFromAddrList %X)\n", pFvDigestPpi->AddrList, pFvDigestPpi->LenList, AddrListHob->LenListOffs));
            }
        }
        ASSERT_EFI_ERROR(Status);
    }
    // on Failure to validate FV DXE -> execute recovery action according to a TDXE Policy
    if (EFI_ERROR(Status)) {
        ExecuteActionOnFailureToValidateFv((CONST EFI_PEI_SERVICES**)PeiServices);
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
    // Verify before memory installed
    InstallFvDxeVerifyPpi((EFI_PEI_SERVICES **)PeiServices,NULL,NULL);

    // Launch on memory install or END_OF_PEI notify
    //(*PeiServices)->NotifyPpi(PeiServices, mReadyToInstallFvDxeVerify);
    // Launch on gAmiPublishFvDxeValidatePpiGuid
    (*PeiServices)->NotifyPpi(PeiServices, mReadyToLaunchFvDxeVerify);

    return EFI_SUCCESS;
}
