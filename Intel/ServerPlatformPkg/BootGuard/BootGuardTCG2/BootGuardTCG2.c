//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file BootGuardTCG2.c
    TCG2 Initialization Flow for Boot Guard

**/


#include <Efi.h>
#include <Pei.h>
#include <Library/HobLib.h>
#include <AmiPeiLib.h>
#include <AmiTcg/TCGMisc.h>
#include <Token.h>
#include <AmiCspLibInc.h>
#include <Library/DebugLib.h>
#include "BootGuardTCG2.h"

EFI_GUID gIntEfiPeiTcgLogHobGuid            =   EFI_TCG_LOG_HOB_GUID;         // TCG TPM 1.2 HOB
EFI_GUID gIntEfiTcgTreeLogHobGuid           =   EFI_TCG_TREE_LOG_HOB_GUID;    // TCG TPM 2.0 HOB

EFI_GUID gIntTpm20HobGuid                   =   TPM20_HOB_GUID;
EFI_GUID gIntAmiTreePpiGuid                 =   AMI_TREE_PPI_GUID;


CONST CHAR8 DetailPCRStr[] = "Boot Guard Measured S-CRTM";
CONST CHAR16 AuthoritySCRTMStr[] = L"Boot Guard Measured S-CRTM";

void sha1_vector(UINTN num_elem, const UINT8* addr[], const UINTN* len, UINT8* mac);
void sha256_vector(UINTN num_elem, const UINT8* addr[], const UINTN* len, UINT8* mac);
UINT64  ReadMsr (UINT32 Msr);

/**
  Check if this is MeasuredBoot

  @retval TRUE  This is MeasuredBoot
  @retval FALSE This is NOT MeasuredBoot
**/
BOOLEAN
IsMeasuredBoot (
    VOID
)
{
    if ((AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
BOOLEAN
IsBootGuardSupported (
  VOID
  )
{
  UINT64          BootGuardBootStatus;
  UINT32          BootGuardAcmStatus;
  UINT64          BootGuardCapability;

  BootGuardBootStatus = *(UINT64 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS);
  BootGuardAcmStatus  = *(UINT32 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_ACM_STATUS);
  BootGuardCapability = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_CAPABILITY;

    if (BootGuardCapability != 0)
    {
    DEBUG ((DEBUG_INFO, "Processor supports Boot Guard.\n"));
    return TRUE;
    }
    else
    {
    DEBUG ((DEBUG_ERROR, "Processor does not support Boot Guard.\n"));
    return FALSE;
  }
}

VOID PrintBuf(
    UINTN   unBufSize,
    UINT8*  _buf
)
{
    UINTN   unIdx;
    for( unIdx = 0; unIdx<unBufSize; ++unIdx )
    {
        if( unIdx % 0x10 == 0 )
            DEBUG(( DEBUG_INFO, "\n" ));

        DEBUG(( DEBUG_INFO, " %02x", _buf[unIdx] ));
    }

    DEBUG(( DEBUG_INFO, "\n" ));
}

VOID PrintAryHashBuf(
    UINTN           unBufCount,
    CONST UINT8     *_buf[],
    CONST UINTN     bufSize[]
)
{
    UINTN   unIdx;
    DEBUG(( DEBUG_INFO, "\n" ));
    for( unIdx = 0; unIdx<unBufCount; ++unIdx )
    {
        DEBUG(( DEBUG_INFO, "Index[0x%04x], Size[0x%04x]:", unIdx, bufSize[unIdx]));
        PrintBuf (bufSize[unIdx], _buf[unIdx]);
    }
}

VOID ShowStruID( UINT8* Tbl )
{
    UINTN   unIdx;
    
    DEBUG(( DEBUG_INFO, "StructureID:"));
    for( unIdx=0; unIdx<8; ++unIdx)
    {
        DEBUG(( DEBUG_INFO, "%c", Tbl[unIdx]));
    }
    DEBUG(( DEBUG_INFO, "\nStructVersion[%02x]\n", Tbl[8]));
}

EFI_STATUS ACM_TpmStsChk(
    IN EFI_PEI_SERVICES **PeiServices
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32      AcmStatus = MmioRead32 (MMIO_ACM_STATUS);
    UINT32      AcmErrClass;
    
    AcmErrClass = AcmStatus >> 4;
    AcmErrClass &= 0x3F;
    
    DEBUG(( DEBUG_INFO, "[%d]: Check ACM_STATUS[%x]\n", __LINE__, AcmStatus));
    
    switch ( AcmErrClass )
    {
        case 0x06 :
        case 0x0B :
        case 0x0C :
        case 0x15 :
            Status = EFI_NOT_READY;
            break;
    }
    
    return Status;
}

EFI_STATUS ACM_PostSuccess(
)
{
    UINT32  MsrValue;

    MsrValue = (UINT32) ReadMsr (MSR_BOOT_GUARD_SACM_INFO);
    if ( ((MsrValue & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) == B_BOOT_GUARD_SACM_INFO_NEM_ENABLED)  &&
         ((MsrValue & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) == B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT)  &&         
         ((MsrValue & B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS) == B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS) ) {
        DEBUG(( DEBUG_INFO, "[BootGuardTCG2.c] : ACM_PostSuccess : EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    DEBUG(( DEBUG_INFO, "[BootGuardTCG2.c] : %a :  EFI_UNSUPPORTED\n",__FUNCTION__));
    return EFI_UNSUPPORTED;
}

//#pragma pack(push, 1)
//typedef struct {
//    UINT32  PCRIndex;
//    UINT32  EventType;
//    UINT8[] Digest;
//    UINT32  EventSize;
//    UINT8[] Event;    
//} EVENT_COMMON_HEADER;
//#pragma pack(pop)

EFI_STATUS CreateBootGuardTPM12Event(
    EFI_PEI_SERVICES **PeiServices,
    UINT32  PcrIndex,
    UINT32  EventType,
    UINT8*  Sha1Digest,
    UINT8*  EventData,
    UINT32  EventDataLen
)
{
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT8               *ptrEvent;
    TCG_LOG_HOB         *TcgLog;
    VOID                *HobStart;
    UINTN               DigestSize = SHA1_DIGEST_SIZE;
    UINTN               Len;
    
    DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d] : Enter  %a (...)\n", __LINE__,__FUNCTION__));

    Status = (*PeiServices)->GetHobList( PeiServices, &HobStart );

    if ( EFI_ERROR( Status ))
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Can not locate PEI HOB Logs\n", __LINE__));
        return EFI_NOT_FOUND;
    }

    TcgLog = (TCG_LOG_HOB*)GetNextGuidHob( &gIntEfiPeiTcgLogHobGuid, HobStart );
    if( NULL == TcgLog)
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Can not Find TCG HOB Logs\n", __LINE__));
        return EFI_NOT_FOUND;        
    }
    
    TcgLog = (TCG_LOG_HOB *)((UINTN)TcgLog + sizeof(EFI_HOB_GUID_TYPE));

    ptrEvent = (UINT8*)(UINTN)(TcgLog+1);
    
    // PCRIndex
    *(UINT32*)ptrEvent = PcrIndex;
    ptrEvent += sizeof(UINT32);
    // EventSize
    *(UINT32*)ptrEvent = EventType;
    ptrEvent += sizeof(UINT32);
    // Digest
    (*PeiServices)->CopyMem(ptrEvent, Sha1Digest, DigestSize);
    ptrEvent += DigestSize;
    // EventSize
    *(UINT32*)ptrEvent = EventDataLen;
    ptrEvent += sizeof(UINT32);
    // EventData
    (*PeiServices)->CopyMem(ptrEvent, EventData, EventDataLen);
    ptrEvent += EventDataLen;
    
    Len = (UINTN)ptrEvent - ((UINTN)(TcgLog+1) + TcgLog->TableSize);
    TcgLog->TableSize += Len;
    ++TcgLog->EventNum;

    return EFI_SUCCESS;
}

EFI_STATUS  CreateBootGuardTPM20Event (
    EFI_PEI_SERVICES **PeiServices,
    UINT32  PcrIndex,
    UINT32  EventType,
    UINT8*  Sha1Digest,
    UINT8*  Sha256Digest,
    UINT8*  EventData,
    UINT32  EventDataLen
)
{
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT8               *ptrEvent;
    TCG_LOG_HOB         *TcgLog;
    VOID                *HobStart;
    UINTN               Len;
    UINT32              u32HashCount = 0;
    
    DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d] : Enter  %a (...)\n", __LINE__,__FUNCTION__));

    Status = (*PeiServices)->GetHobList( PeiServices, &HobStart );

    if ( EFI_ERROR( Status ))
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Can not locate PEI HOB Logs\n", __LINE__));
        return EFI_NOT_FOUND;
    }

    TcgLog = (TCG_LOG_HOB*)GetNextGuidHob( &gIntEfiTcgTreeLogHobGuid, HobStart );
    
    if( NULL == TcgLog)
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Can not Find TCG HOB Logs\n", __LINE__));
        return EFI_NOT_FOUND;        
    }
    
    TcgLog = (TCG_LOG_HOB*)((UINTN)TcgLog + sizeof(EFI_HOB_GUID_TYPE));
    DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d] : TcgLog Addr 0x%x\n", __LINE__, TcgLog));

    if( 0 == ( TcgLog->ActPcrBanks & (TREE_BOOT_HASH_ALG_SHA1|TREE_BOOT_HASH_ALG_SHA256) ) )
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Err ActPcrBanks, No SHA1/SHA256 banks\n", __LINE__));
        return EFI_INVALID_PARAMETER;
    }

    // if ( 0 == TcgLog->Tcg2SpecVersion ) means it is TPM 12 SHA1 Digest Event
    if( TCG2_PROTOCOL_SPEC_VERSION_1_0 == TcgLog->Tcg2SpecVersion || 0 == TcgLog->Tcg2SpecVersion )
    {
    }
    else if( TCG2_PROTOCOL_SPEC_VERSION_1_X == TcgLog->Tcg2SpecVersion )
    {
        if( TcgLog->ActPcrBanks & TREE_BOOT_HASH_ALG_SHA1 )
        {
            ++u32HashCount;
        }
        if( TcgLog->ActPcrBanks & TREE_BOOT_HASH_ALG_SHA256 )
        {
            ++u32HashCount;
        }
    }

    ptrEvent = (UINT8*)(UINTN)TcgLog->LastEntry;
    
    // PCRIndex
    *(UINT32*)ptrEvent = PcrIndex;
    ptrEvent += sizeof(UINT32);
    // EventType
    *(UINT32*)ptrEvent = EventType;
    ptrEvent += sizeof(UINT32);
    // if ( 0 == TcgLog->Tcg2SpecVersion ) means it is TPM 12 SHA1 Digest Event
    if( TCG2_PROTOCOL_SPEC_VERSION_1_0 == TcgLog->Tcg2SpecVersion || 0 == TcgLog->Tcg2SpecVersion )
    {
        // Digest
        (*PeiServices)->CopyMem(ptrEvent, Sha1Digest, SHA1_DIGEST_SIZE);
        ptrEvent += SHA1_DIGEST_SIZE;
    }
    else if( TCG2_PROTOCOL_SPEC_VERSION_1_X == TcgLog->Tcg2SpecVersion )
    {
        *(UINT32*)ptrEvent = u32HashCount;     // Count
        ptrEvent += sizeof(UINT32);
        
        if( TcgLog->ActPcrBanks & TREE_BOOT_HASH_ALG_SHA1 )
        {
            *(UINT16*)ptrEvent = 0x0004;    // AlgorithmId
            ptrEvent += sizeof(UINT16);

            (*PeiServices)->CopyMem(ptrEvent, Sha1Digest, SHA1_DIGEST_SIZE);
            ptrEvent += SHA1_DIGEST_SIZE;
        }

        if( TcgLog->ActPcrBanks & TREE_BOOT_HASH_ALG_SHA256 )
        {
            *(UINT16*)ptrEvent = 0x000B;    // AlgorithmId
            ptrEvent += sizeof(UINT16);

            (*PeiServices)->CopyMem(ptrEvent, Sha256Digest, SHA256_DIGEST_SIZE);
            ptrEvent += SHA256_DIGEST_SIZE;
        }
    }
    // EventSize
    *(UINT32*)ptrEvent = EventDataLen;
    ptrEvent += sizeof(UINT32);
    // EventData
    (*PeiServices)->CopyMem(ptrEvent, EventData, EventDataLen);
    ptrEvent += EventDataLen;
    
    Len = (UINTN)ptrEvent - (UINTN)TcgLog->LastEntry;
    TcgLog->LastEventPtr = TcgLog->LastEntry;
    TcgLog->LastEntry = (UINT64)(UINTN)ptrEvent;
    TcgLog->TableSize += Len;
    ++TcgLog->EventNum;

    return EFI_SUCCESS;
}

EFI_STATUS
Tpm20MeasureCRTM(
    IN CONST EFI_PEI_SERVICES **PeiServices, 
    AMI_TREE_PPI *TrEEPeiPpi)
{
    tdTpm2Event         *TrEEEventData;
    UINT32              EventSize = 0;
    EFI_GUID            CrtmVersion = CRTM_GUID;
    UINT8               *EventDataPtr;
    UINTN               Len;
    UINT8               InBuf[0x100];

    if( NULL == TrEEPeiPpi )
    {
//        ASSERT_EFI_ERROR( Status );
        return EFI_NOT_FOUND;
    }

    EventSize  = sizeof(EFI_GUID);
    
    Len =  sizeof(tdTpm2EventHeader) +  sizeof(UINT32) + EventSize;

    TrEEEventData = (tdTpm2Event*)&InBuf[0];
    
    TrEEEventData->Size = Len;

    TrEEEventData->Header.EventType  = 0x00000008;
    TrEEEventData->Header.HeaderSize = sizeof(tdTpm2EventHeader);
    TrEEEventData->Header.HeaderVersion = 1; 
    TrEEEventData->Header.PCRIndex      = 0;
    
    EventDataPtr = (UINT8 *)TrEEEventData;

    EventDataPtr += sizeof(tdTpm2EventHeader) + sizeof(UINT32);

    (*PeiServices)->CopyMem(
        EventDataPtr,
        &CrtmVersion,
        EventSize
        );
    
    return TrEEPeiPpi->HashLogExtendEvent(TrEEPeiPpi,
            0, (EFI_PHYSICAL_ADDRESS)EventDataPtr, EventSize, TrEEEventData);
}

EFI_STATUS
Tpm12MeasureCRTMVersion(
    IN EFI_PEI_SERVICES **PeiServices )
{
    UINT8                   *pBufTcgEvent;
    UINT8                   InBuf[0x100];
    UINT32                  EventNum;
    EFI_GUID                CrtmVersion   = CRTM_GUID;         
    PEI_TCG_PPI             *TcgPpi       = NULL;
    EFI_STATUS              Status;
    EFI_GUID gTcgPpiguid    =  PEI_TCG_PPI_GUID;
    
    pBufTcgEvent = &InBuf[0];
    
    *(UINT32*)pBufTcgEvent      = 0;                // PCRIndex
    pBufTcgEvent += sizeof(UINT32);
    *(UINT32*)pBufTcgEvent      = 0x00000008;       // EventType
    pBufTcgEvent += sizeof(UINT32);    
    pBufTcgEvent += 20;                             // Digest
    *(UINT32*)pBufTcgEvent      = sizeof(EFI_GUID); // EventDataSize
    pBufTcgEvent += sizeof(UINT32);
    // Event Data
    (*PeiServices)->CopyMem (
            pBufTcgEvent,
            &CrtmVersion,
            sizeof(EFI_GUID)
            );

    Status =  (*PeiServices)->LocatePpi(
                           PeiServices,
                           &gTcgPpiguid,
                           0, NULL,
                           &TcgPpi);

    if(EFI_ERROR(Status)){
        DEBUG(( DEBUG_ERROR, "[%d]: gTcgPpiguid NOT found %r \n", __LINE__, Status)); 
        return Status;
    }

    return TcgPpi->TCGHashLogExtendEvent(
               TcgPpi,
               PeiServices,
               (UINT8*)&CrtmVersion,
               sizeof(EFI_GUID),
               &InBuf[0],
               &EventNum
               );
}

// 0: No TPM
// 1: TPM 1.2
// 2: TPM 2.0
// 3: PTT on HCI
UINTN CheckCpuMsrTpmDevice()
{
    UINT64      u64Msr13A;
    
    u64Msr13A = ReadMsr (MSR_BOOT_GUARD_SACM_INFO);
    
    u64Msr13A >>= 1;
    u64Msr13A &= 3;
    
    return (UINTN)u64Msr13A;
}

// 0: No TPM
// 1: TPM 1.2
// 2: TPM 2.0
// 3: PTT on HCI
UINTN CheckTreeHobTpmDevice(
    CONST EFI_PEI_SERVICES **PeiServices
)
{
    Tpm20DeviceHob      *TrEEDeviceHob;
    VOID                *HobStart;
    EFI_STATUS          Status;

    Status = (*PeiServices)->GetHobList( PeiServices, &HobStart );

    if ( EFI_ERROR( Status ))
    {
        DEBUG(( DEBUG_ERROR, "BootGuardTCG2.c[%d] : Can not locate PEI HOB Logs, It may TPM 1.2 Device\n", __LINE__));
        return 1;
    }

    TrEEDeviceHob = (Tpm20DeviceHob*)GetNextGuidHob( &gIntTpm20HobGuid, HobStart );
    TrEEDeviceHob = (Tpm20DeviceHob*)((UINTN)TrEEDeviceHob + sizeof(EFI_HOB_GUID_TYPE));
    if( 1 == TrEEDeviceHob->Tpm20DeviceState )
        return 2;

    return 1;
}

EFI_STATUS FindEntryInFit(
    IN UINT8    FitType,
    VOID        **pFitTable
)
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINT32          FitEntryPointer = 0, FitEntryNumber = 0, i = 0;
    FIT_ENTRY       *FitEntry = NULL;
    BOOLEAN         bFoundTbl = FALSE;

    do
    {
        FitEntryPointer = MmioRead32(IBB_ENTRYPOINT_M);
        FitEntry = (FIT_ENTRY*)(UINTN)FitEntryPointer;
        FitEntryNumber = FitEntry->TblSIZE;

        if ( ( 0xFFFFFFFF == (UINTN)FitEntry ) || ( NULL == FitEntry ) )
        {
            Status = EFI_NOT_FOUND;
            DEBUG(( DEBUG_ERROR, "[BootGuardTCG2.c] : in %a (%lx) is Invalid\n", __FUNCTION__,FitEntryPointer));
            break;
        }

        FitEntry = (FIT_ENTRY*)FitEntryPointer;

//        DEBUG(( DEBUG_INFO, "[BootGuardTCG2.c] : FitEntryPoint[0x%08x], TableCount[%x]\n", FitEntryPointer, FitEntryNumber ));

        for(i=1; i<FitEntryNumber; i++)
        {
            FitEntry = (FIT_ENTRY*)(FitEntryPointer + i*16);
//            DEBUG(( DEBUG_INFO, "[BootGuardTCG2.c] : FitEntry->TblType = %x\n", FitEntry->TblType));
            if ( FitEntry->TblType == FitType )
            {
                bFoundTbl =1;
                break;
            }
        }

        if( FALSE == bFoundTbl )
        {
            Status = EFI_NOT_FOUND;
            DEBUG(( DEBUG_INFO, "[BootGuardTCG2.c] : Error, Did not find Tbl type = %x\n", FitType));
            break;
        }

        *pFitTable = (VOID*)(UINTN)FitEntry->TblAddress;
    } while( FALSE );

    if( EFI_ERROR(Status) )
    {
        // Do something
    }
    return Status;
}

BOOLEAN NeedLocalityEvent (
  VOID
)
{
// For Whitley Platform, we need report the startup event is locality 3
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINT32                      u32IBBElementFlag = 0;
    BpmStruct                   *BpmStructure = NULL;
    
    Status = FindEntryInFit( FIT_BPM_TYPE, &BpmStructure );
    if( EFI_ERROR(Status) )
    {
      DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
      return FALSE;
    }
    
    u32IBBElementFlag = BpmStructure->SE[0].Flags;

    // Check the Startup Locality bit set or not?
    if( 0x02 != (u32IBBElementFlag & 0x02) )
    {
      DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: Skip Log Authority Event\n", __LINE__ ));
      return FALSE;
    }

   return TRUE;
}

EFI_STATUS LogLocalityStartupEvent(
    IN EFI_PEI_SERVICES **PeiServices
)
{
    EFI_STATUS                          Status;
    TCG_EFI_STARTUP_LOCALITY_EVENT      LocalityEventData;
    UINT8                               Sha1Digest[SHA1_DIGEST_SIZE];
    UINT8                               Sha256Digest[SHA256_DIGEST_SIZE];

    (*PeiServices)->SetMem( Sha1Digest, sizeof(Sha1Digest), 0 );
    (*PeiServices)->SetMem( Sha256Digest, sizeof(Sha256Digest), 0 );

    if (NeedLocalityEvent())
    {
        // Make sure the Startup Event must be null terminal. Form TCG PC Client Platform Spec Define.
        (*PeiServices)->SetMem( LocalityEventData.Signature, sizeof(LocalityEventData.Signature), 0 );
        (*PeiServices)->CopyMem (LocalityEventData.Signature, (UINT8 *) "StartupLocality", sizeof ("StartupLocality"));
        LocalityEventData.StartupLocality = 0x03;

        if( 1 == CheckCpuMsrTpmDevice() )
        {
            // For TPM 1.2, did not need the startup event locality event
        }
        else if ( (2 == CheckCpuMsrTpmDevice() ) || ( 3 == CheckCpuMsrTpmDevice() ) )
        {
            Status =  CreateBootGuardTPM20Event(
                                    PeiServices,
                                    0,                      // PcrIndex
                                    0x00000003,             // EventType : EV_NO_ACTION
                                    Sha1Digest,             // HashData
                                    Sha256Digest,           // HashDataLen
                                    (UINT8*)&LocalityEventData,
                                    sizeof(LocalityEventData)
                                    );

            if( EFI_ERROR(Status) )
            {
               // Log Event Error
            }

            return EFI_SUCCESS;
        }
    }
    return EFI_SUCCESS;
}

VOID SwapByteReverse(
    UINT8           *Key,
    UINTN           unKeySize
)
{
//    UINTN       unKeySize = 0x100;
    UINTN       unIdx, unLastOfst;
    UINT8       u8TmpVal;

    for( unIdx=0, unLastOfst=0xff; unIdx < unKeySize/2; ++unIdx, --unLastOfst )
    {
        u8TmpVal = Key[unIdx];
        Key[unIdx] = Key[unLastOfst];
        Key[unLastOfst] = u8TmpVal;
    }
}

EFI_STATUS LogAuthorityPCREvent(
    IN EFI_PEI_SERVICES **PeiServices
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINTN                       unIdx = 0;
    PCR7_DATA                   *Pcr7Data;
    UINT8                       PcrDataBuffer[MAX_PCR7_DATA_LEN];
    KEY_MANIFEST_STRAUCTURE     *KmStructure = NULL;
    BpmStruct                   *BpmStructure = NULL;
    UINT8                       *AcmTable = NULL;
    BPM_COMMON_HEADER           *BpmCommHdr = NULL;
    CONST UINT8*                HashDataAddr[6];
    UINTN                       HashDataAddrLen[6];
    UINT32                      u32IBBElementFlag = 0;
    UINT8                       Sha1Digest[SHA1_DIGEST_SIZE];
    UINT8                       Sha256Digest[SHA256_DIGEST_SIZE];
    UINT64                      u64TmpVal = 0;
    UINT32                      u32TmpVal = 0;
    UINT8                       BpKeyHashBuffer[RSA2048_KEY_BYTES + sizeof(UINT32)];

    DEBUG(( DEBUG_INFO,  "[BootGuardTCG2.c] : Enter %a (...)\n",__FUNCTION__));

    Pcr7Data = (PCR7_DATA*)&PcrDataBuffer[0];
    (*PeiServices)->SetMem (Pcr7Data, sizeof(PcrDataBuffer), 0);

    do
    {
        Status = FindEntryInFit( FIT_BPM_TYPE, &BpmStructure );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        u32IBBElementFlag = BpmStructure->SE[0].Flags;
        // Check the AuthorityMeasure bit set or not?
        if( 0x04 != (u32IBBElementFlag & 0x04) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: Skip Log Authority Event\n", __LINE__ ));
            Status = EFI_SUCCESS;
            break;
        }

        // (1). AcmPolicyStatus
        (*PeiServices)->CopyMem (&Pcr7Data->AcmPolicyStatus, (VOID*)MMIO_ACM_POLICY_STATUS, sizeof(Pcr7Data->AcmPolicyStatus));
        DEBUG(( DEBUG_INFO, "Pcr7Data->AcmPolicyStatus = [0x%08x]\n", Pcr7Data->AcmPolicyStatus ));

        Status = FindEntryInFit( FIT_ACM_TYPE, &AcmTable );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nAcmTable: Addr[0x%08x]\n", (UINTN)AcmTable));

        // (2). Get ACM SVN
        Pcr7Data->AcmHeaderSvn = *(UINT16*)((UINTN)AcmTable + 0x1C);
        DEBUG(( DEBUG_INFO, "ACM_SVN locate at AcmTable Offset[0x1C], Addr[%x]: %x\n", (UINTN)AcmTable + 0x1C ));
        DEBUG(( DEBUG_INFO, "Pcr0Data->ACM_SVN = [0x%04x]\n", Pcr7Data->AcmHeaderSvn ));

        // (3). Get ACM KeyHash from MSR 20..23
        u64TmpVal = ReadMsr (MSR_ACM_CPU_KEY_HASH_0);
        (*PeiServices)->CopyMem (&Pcr7Data->AcmKeyHash[0], &u64TmpVal, sizeof(u64TmpVal));
        u64TmpVal = ReadMsr (MSR_ACM_CPU_KEY_HASH_1);
        (*PeiServices)->CopyMem (&Pcr7Data->AcmKeyHash[8], &u64TmpVal, sizeof(u64TmpVal));
        u64TmpVal = ReadMsr (MSR_ACM_CPU_KEY_HASH_2);
        (*PeiServices)->CopyMem (&Pcr7Data->AcmKeyHash[16], &u64TmpVal, sizeof(u64TmpVal));
        u64TmpVal = ReadMsr (MSR_ACM_CPU_KEY_HASH_3);
        (*PeiServices)->CopyMem (&Pcr7Data->AcmKeyHash[24], &u64TmpVal, sizeof(u64TmpVal));
        DEBUG(( DEBUG_INFO, "\nMSR ACM HASH"));
        PrintBuf( sizeof(Pcr7Data->AcmKeyHash), Pcr7Data->AcmKeyHash );

        Status = FindEntryInFit( FIT_BPM_TYPE, &BpmStructure );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nBpmStructure: Addr[0x%08x]\n", (UINTN)BpmStructure));
        ShowStruID( (UINT8*)BpmStructure );

        // Find the BPM (PMSE) Signature
        BpmCommHdr = (BPM_COMMON_HEADER*)BpmStructure;
        for (unIdx = 0; unIdx < 6; ++unIdx)
        {
            DEBUG(( DEBUG_INFO, "Current: BpmCommHdr Addr[0x%08x]\n", (UINTN)BpmCommHdr));
            ShowStruID( (UINT8*)BpmCommHdr );
            if (BpmCommHdr->StructureID == BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID )
            {
                break;
            }
            BpmCommHdr = (BPM_COMMON_HEADER*)((UINTN)BpmCommHdr + (UINTN)BpmCommHdr->ElementSize);
        }

        if( 6 == unIdx)
        {
            Status = EFI_NOT_FOUND;
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        // Follow spec, The Key must be RSA2048 Key.
        if ( 2048 != ((BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT*)BpmCommHdr)->KeySignature.Key.KeySize )
        {
            Status = EFI_INVALID_PARAMETER;
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        // (4) Calculate BP.Key
        // For Intel FIT RSA format is Little Endian (LE) format,
        // OpenSSL treats all series of bytes as Big Endian.
        (*PeiServices)->CopyMem (&BpKeyHashBuffer[0], ((BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT*)BpmCommHdr)->KeySignature.Key.Modulus , RSA2048_KEY_BYTES);
//        SwapByteReverse ((UINT8*)&BpKeyHashBuffer[0], RSA2048_KEY_BYTES);
        // Default exponent.
        u32TmpVal = 0x00010001;
        (*PeiServices)->CopyMem (&BpKeyHashBuffer[RSA2048_KEY_BYTES], &u32TmpVal , sizeof(UINT32));
        DEBUG(( DEBUG_INFO, "\nBP PubKey Hash Buffer, Size[0x%08x]", sizeof(BpKeyHashBuffer)));
        PrintBuf( sizeof(BpKeyHashBuffer), &BpKeyHashBuffer[0] );
        HashDataAddr[0] = &BpKeyHashBuffer[0];
        HashDataAddrLen[0] = sizeof(BpKeyHashBuffer);
        // TO CHECK: Current use the SHA256 Hash
        sha256_vector( 1, HashDataAddr, HashDataAddrLen, Pcr7Data->BpKeyHash);
        DEBUG(( DEBUG_INFO, "\nBP PubKey Hash, Size[0x%04x]:", sizeof(Pcr7Data->BpKeyHash) ));
        PrintBuf( sizeof(Pcr7Data->BpKeyHash), Pcr7Data->BpKeyHash );

        Status = FindEntryInFit( FIT_KM_TYPE, &KmStructure );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }

        // (5). Get KM.BPKey.hashBuffer[BRM pubkey digeest size]
        // TO CHECK: Current BpmPubKeyHash is SHA256.
        (*PeiServices)->CopyMem (Pcr7Data->BpmPubkeyHashBuffer, KmStructure->KmHash[0].Digest.HashBuffer , sizeof(Pcr7Data->BpmPubkeyHashBuffer));
        DEBUG(( DEBUG_INFO, "\nKM.BPKey.hashBuffer, size[0x%04x]:", sizeof(Pcr7Data->BpmPubkeyHashBuffer) ));
        PrintBuf( sizeof(Pcr7Data->BpmPubkeyHashBuffer), Pcr7Data->BpmPubkeyHashBuffer );

        DEBUG(( DEBUG_INFO, "\nPCR7_DATA, Size[0x%08x]", sizeof(*Pcr7Data) ));
        PrintBuf( sizeof(*Pcr7Data), (UINT8*)Pcr7Data );

        HashDataAddr[0] = (UINT8*)Pcr7Data;
        HashDataAddrLen[0] = sizeof(*Pcr7Data);

        sha1_vector( 1, HashDataAddr, HashDataAddrLen, &Sha1Digest[0]);
        sha256_vector( 1, HashDataAddr, HashDataAddrLen, &Sha256Digest[0]);

        DEBUG(( DEBUG_INFO, "\nPCR7_DATA SHA1 Digest" ));
        PrintBuf( sizeof(Sha1Digest), &Sha1Digest[0] );

        DEBUG(( DEBUG_INFO, "\nPCR7_DATA SHA256 Digest" ));
        PrintBuf( sizeof(Sha256Digest), &Sha256Digest[0] );

        if( 1 == CheckCpuMsrTpmDevice() )
        {
            // TPM 1.2 did not support the Authority Measure.
        }
        else if ( (2 == CheckCpuMsrTpmDevice() ) || ( 3 == CheckCpuMsrTpmDevice() ) )
        {
            Status =  CreateBootGuardTPM20Event(
                                                    PeiServices,
                                                    7,                      // PcrIndex
                                                    0x80000001,             // EV_EFI_VARIABLE_DRIVER_CONFIG
                                                    &Sha1Digest[0],
                                                    &Sha256Digest[0],
                                                    AuthoritySCRTMStr,
                                                    sizeof(AuthoritySCRTMStr)
                                                    );
            if( EFI_ERROR(Status) )
            {
                // Log Event Error
                break;
            }
        }
    } while( FALSE );

    if( EFI_ERROR(Status) )
    {
        // Do something
    }
    return Status;

}

EFI_STATUS LogDetailPCREvent(
    IN EFI_PEI_SERVICES **PeiServices
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINTN                       unIdx = 0;
    PCR0_DATA                   *Pcr0Data;
    UINT8                       PcrDataBuffer[MAX_PCR0_DATA_LEN];
    KEY_MANIFEST_STRAUCTURE     *KmStructure = NULL;
    BpmStruct                   *BpmStructure = NULL;
    UINT8                       *AcmTable = NULL;
    RSA2048_KEY_SIGNATURE_STRUCT    *RsaKss = NULL;
    BPM_COMMON_HEADER           *BpmCommHdr = NULL;
    UINTN                       IBBSegmentOffset = 0;
//    UINTN                       DigestHashBufferSize = 0;
    HASH_STRUCTURE              *pCommHashStru = NULL;
    UINT8                       *Buffer = NULL;
    UINT8                       Sha1Digest[SHA1_DIGEST_SIZE];
    UINT8                       Sha256Digest[SHA256_DIGEST_SIZE];
    CONST UINT8*                HashDataAddr[10];
    UINTN                       HashDataAddrLen[10];
    UINTN                       HashDataCount = 0;
    UINTN                       unTmpVal = 0;

    DEBUG(( DEBUG_INFO,  "[BootGuardTCG2.c] : Enter %a (...)\n",__FUNCTION__));

    Pcr0Data = (PCR0_DATA*)&PcrDataBuffer[0];
    (*PeiServices)->SetMem (Pcr0Data, sizeof(PcrDataBuffer), 0);

    do
    {
        DEBUG(( DEBUG_INFO, "MSR[0x%x]:[%08x]\n", MSR_BOOT_GUARD_SACM_INFO, ReadMsr (MSR_BOOT_GUARD_SACM_INFO) ));
        DEBUG(( DEBUG_INFO, "ACM_STATUS Addr::[%08x]\n", MMIO_ACM_STATUS, ACM_STATUS ));
        DEBUG(( DEBUG_INFO, "ACM_POLICY_STATUS Addr: %x:[%08x]\n", MMIO_ACM_POLICY_STATUS, ACM_POLICY_STATUS ));

        //
        HashDataAddr[HashDataCount] = (UINT8*)MMIO_ACM_POLICY_STATUS;
        HashDataAddrLen[HashDataCount] = sizeof(Pcr0Data->AcmPolicyStatus);
        ++HashDataCount;
        DEBUG(( DEBUG_INFO, "Pcr0Data->AcmPolicyStatus = [0x%08x]\n", ACM_POLICY_STATUS ));

        Status = FindEntryInFit( FIT_ACM_TYPE, &AcmTable );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nAcmTable: Addr[0x%08x]\n", (UINTN)AcmTable));

        // Get ACM SVN
        HashDataAddr[HashDataCount] = (UINT8*)((UINTN)AcmTable + 0x1C);
        HashDataAddrLen[HashDataCount] = sizeof(Pcr0Data->AcmHeaderSvn);
        ++HashDataCount;
        DEBUG(( DEBUG_INFO, "ACM_SVN locate at AcmTable Offset[0x1C], Addr[%x]: %x\n", (UINTN)AcmTable + 0x1C ));
        DEBUG(( DEBUG_INFO, "Pcr0Data->ACM_SVN = [0x%04x]\n", MmioRead16( (UINTN)AcmTable + 0x1C) ));

        // Check ACM Key Size
        unTmpVal = *(UINT32*)((UINTN)AcmTable + 120);
        if ( 64 == unTmpVal || 96 == unTmpVal ) // RSA2048 or RSA3072
        {
            // Success for detect the correct ACM version and RSA Sig Size
        }
        else
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        HashDataAddr[HashDataCount] = (UINT8*)((UINTN)AcmTable + 388);
        HashDataAddrLen[HashDataCount] = unTmpVal * 4;
        ++HashDataCount;
        DEBUG(( DEBUG_INFO, "\nACM Sig locate at AcmTable Offset[0x184], Addr[0x%08x]", (UINTN)AcmTable + 0x184 ));
        PrintBuf( HashDataAddrLen[HashDataCount-1], HashDataAddr[HashDataCount-1] );

        Status = FindEntryInFit( FIT_KM_TYPE, &KmStructure );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nKmStructure: Addr[0x%08x]\n", (UINTN)KmStructure));
        ShowStruID( (UINT8*)KmStructure );

        DEBUG(( DEBUG_INFO, "\nPcr0Data->KmSignature"));
        RsaKss = (RSA2048_KEY_SIGNATURE_STRUCT*)((UINTN)KmStructure + (UINTN)KmStructure->KeySignatureOffset);

        HashDataAddr[HashDataCount] = (UINT8*)RsaKss->Signature.Signature;
        HashDataAddrLen[HashDataCount] = sizeof(RsaKss->Signature.Signature);
        ++HashDataCount;
        PrintBuf( HashDataAddrLen[HashDataCount-1], HashDataAddr[HashDataCount-1] );

        Status = FindEntryInFit( FIT_BPM_TYPE, &BpmStructure );
        if( EFI_ERROR(Status) )
        {
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nBpmStructure: Addr[0x%08x]\n", (UINTN)BpmStructure));
        ShowStruID( (UINT8*)BpmStructure );

        // Find the BPM (PMSE) Signature
        BpmCommHdr = (BPM_COMMON_HEADER*)BpmStructure;
        for (unIdx = 0; unIdx < 6; ++unIdx)
        {
            DEBUG(( DEBUG_INFO, "Current: BpmCommHdr Addr[0x%08x]\n", (UINTN)BpmCommHdr));
            ShowStruID( (UINT8*)BpmCommHdr );
            if (BpmCommHdr->StructureID == BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID )
            {
                break;
            }
            BpmCommHdr = (BPM_COMMON_HEADER*)((UINTN)BpmCommHdr + (UINTN)BpmCommHdr->ElementSize);
        }

        if( 6 == unIdx)
        {
            Status = EFI_NOT_FOUND;
            DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
            break;
        }
        DEBUG(( DEBUG_INFO, "\nPcr0Data->BpmSignature"));
        HashDataAddr[HashDataCount] = (UINT8*)((BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT*)BpmCommHdr)->KeySignature.Signature.Signature;
        HashDataAddrLen[HashDataCount] = sizeof(((BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT*)0)->KeySignature.Signature.Signature);
        ++HashDataCount;
        PrintBuf( HashDataAddrLen[HashDataCount-1], HashDataAddr[HashDataCount-1] );

        DEBUG(( DEBUG_INFO, "\nIBB Segment Element: Addr[0x%08x]\n", (UINTN)&BpmStructure->SE[0]));
        ShowStruID( (UINT8*)&BpmStructure->SE[0] );

        Status = EFI_SUCCESS;
        pCommHashStru = (HASH_STRUCTURE*)&BpmStructure->SE[0].DigestList.Digest[0];
        for (unIdx=0; unIdx < BpmStructure->SE[0].DigestList.Count; ++unIdx)
        {
            if (TPM_ALG_SHA256 == pCommHashStru->HashAlg)
            {
                HashDataAddr[HashDataCount] = pCommHashStru->HashBuffer;
                HashDataAddrLen[HashDataCount] = SHA256_DIGEST_SIZE;
                sha256_vector( HashDataCount + 1, HashDataAddr, HashDataAddrLen, (UINT8*)&Sha256Digest[0] );
                DEBUG(( DEBUG_INFO, "\nIBB SHA256 Digest"));
                PrintBuf( HashDataAddrLen[HashDataCount], HashDataAddr[HashDataCount] );
            }
            else if (TPM_ALG_SHA1 == pCommHashStru->HashAlg)
            {
                HashDataAddr[HashDataCount] = pCommHashStru->HashBuffer;
                HashDataAddrLen[HashDataCount] = SHA1_DIGEST_SIZE;
                sha1_vector( HashDataCount + 1, HashDataAddr, HashDataAddrLen, (UINT8*)&Sha1Digest[0] );
                DEBUG(( DEBUG_INFO, "\nIBB SHA1 Digest"));
                PrintBuf( HashDataAddrLen[HashDataCount], HashDataAddr[HashDataCount] );
            }
            else if (TPM_ALG_SHA384 == pCommHashStru->HashAlg)
            {
                HashDataAddr[HashDataCount] = pCommHashStru->HashBuffer;
                HashDataAddrLen[HashDataCount] = SHA384_DIGEST_SIZE;
                // TODO: To Support the SHA384 Algorithm
                DEBUG(( DEBUG_INFO, "\nIBB SHA384 Digest"));
                PrintBuf( HashDataAddrLen[HashDataCount], HashDataAddr[HashDataCount] );
            }
            else if (TPM_ALG_SM3_256 == pCommHashStru->HashAlg)
            {
                HashDataAddr[HashDataCount] = pCommHashStru->HashBuffer;
                HashDataAddrLen[HashDataCount] = SM3_256_DIGEST_SIZE;
                // TODO: To Support the SM3 Algorithm
                DEBUG(( DEBUG_INFO, "\nIBB SM3 Digest"));
                PrintBuf( HashDataAddrLen[HashDataCount], HashDataAddr[HashDataCount] );
            }
            else
            {
                Status = EFI_NOT_FOUND;
                DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: TRACE ERROR - %r\n", __LINE__, Status ));
                break;
            }
            pCommHashStru = (HASH_STRUCTURE*)((UINTN)pCommHashStru->HashBuffer + (UINTN)pCommHashStru->Size);
        }

        if (EFI_ERROR (Status))
        {
            break;
        }
        // Increase the Hash Data Count
        ++HashDataCount;

        DEBUG(( DEBUG_INFO, "\nDump PCR0_DATA Common Hash Structure" ));
        PrintAryHashBuf( HashDataCount - 1, HashDataAddr, HashDataAddrLen);

        DEBUG(( DEBUG_INFO, "\nPCR0_DATA SHA1 Digest" ));
        PrintBuf( sizeof(Sha1Digest), &Sha1Digest[0] );
        
        DEBUG(( DEBUG_INFO, "\nPCR0_DATA SHA256 Digest" ));
        PrintBuf( sizeof(Sha256Digest), &Sha256Digest[0] );

        if( 1 == CheckCpuMsrTpmDevice() )
        {
            Status =  CreateBootGuardTPM12Event(
                                                PeiServices, 
                                                0,                      // PcrIndex
                                                0x00000007,             // EventType
                                                &Sha1Digest[0],
                                                DetailPCRStr,
                                                sizeof(DetailPCRStr)
                                                );
            if( EFI_ERROR(Status) )
            {
                // Log Event Error
                break;
            }
        }
        else if ( (2 == CheckCpuMsrTpmDevice() ) || ( 3 == CheckCpuMsrTpmDevice() ) )
        {
            Status =  CreateBootGuardTPM20Event(
                                                    PeiServices,
                                                    0,                      // PcrIndex
                                                    0x00000007,             // EventType
                                                    &Sha1Digest[0],
                                                    &Sha256Digest[0],
                                                    DetailPCRStr,
                                                    sizeof(DetailPCRStr)
                                                    );
            if( EFI_ERROR(Status) )
            {
                // Log Event Error
                break;
            }
        }
    } while( FALSE );

    if( EFI_ERROR(Status) )
    {
        // Do something
    }
    
    return EFI_SUCCESS;
}

EFI_STATUS
BootGuardTcg2MeasureCRTMVersion (
    CONST EFI_PEI_SERVICES **PeiServices, 
    VOID *_TrEEPeiPpi )
{
    EFI_STATUS Status = EFI_NOT_READY;
    VOID        *TrEEPeiPpi = NULL;
    DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: Enter %a\n", __LINE__,__FUNCTION__));

    Status = (*PeiServices)->LocatePpi(
                 PeiServices,
                 &gIntAmiTreePpiGuid,
                 0, NULL,
                 &TrEEPeiPpi);
    if( EFI_ERROR(Status) )
    {
        TrEEPeiPpi = NULL;
    }

    if ( IsBootGuardSupported() == FALSE ) {
        goto TCG_TPM_CRTM_L;
    }
    
    Status = ACM_PostSuccess();
    if( !EFI_ERROR(Status) )
    {
        DEBUG((EFI_D_INFO, "[BootGuardTCG2.c] Start the LogDetailPCREvent\n"));
        if(  IsMeasuredBoot( ) ) {
            Status = LogLocalityStartupEvent( PeiServices );
            ASSERT_EFI_ERROR ( Status );
            Status = LogDetailPCREvent( PeiServices );
            ASSERT_EFI_ERROR ( Status );
            Status = LogAuthorityPCREvent( PeiServices );
            ASSERT_EFI_ERROR ( Status );
        } else {
            goto TCG_TPM_CRTM_L;   	    
        }
    }
    else
    {
        DEBUG(( DEBUG_INFO, "[%d]: The BootGuard ACM did not Enable\n", __LINE__));
        Status = EFI_SUCCESS;
    }
    
    DEBUG(( DEBUG_INFO, "BootGuardTCG2.c[%d]: End of BootGuardTcg2MeasureCRTMVersion\n", __LINE__));
    
TCG_TPM_CRTM_L: ;
    if( 1 == CheckCpuMsrTpmDevice() )
    {
        Tpm12MeasureCRTMVersion( PeiServices );
    }
    else if ( (2 == CheckCpuMsrTpmDevice() ) || ( 3 == CheckCpuMsrTpmDevice() ) )
    {
        Tpm20MeasureCRTM( PeiServices, TrEEPeiPpi );
    }
    else
    {
        if( 2 == CheckTreeHobTpmDevice( PeiServices ) )
        {
            Tpm20MeasureCRTM( PeiServices, TrEEPeiPpi );
        }
        else
        {
            Tpm12MeasureCRTMVersion( PeiServices );
        }
    }
    
    return EFI_SUCCESS;
}
