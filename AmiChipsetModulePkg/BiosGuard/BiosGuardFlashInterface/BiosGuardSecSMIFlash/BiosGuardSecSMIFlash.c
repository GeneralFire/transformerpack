//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file BiosGuardSecSMIFlash.c
    This file contains code for Bios Guard SMM

**/

#include <Token.h>
#include <AmiDxeLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiBufferValidationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/SecSmiFlash.h>
#include <BiosGuard/Include/Protocol/AmiBiosGuardProtocol.h>
#include <Guid/CapsuleVendor.h>
#include <../../Include/PlatformDefinition.h>
#include <Capsule.h>
#include <Sb.h>

#include <BiosGuard/Include/Library/AmiBiosGuardMiscLib.h>

#include <BiosGuardDefinitions.h>

#define AP_SAFE_RETRY_LIMIT 1

//
//  Structure define
//
#pragma pack (1)
typedef struct {
    //
    // AmiBiosGuardRecoveryAddress for Tool Interface
    //
    UINT64                      Signature;                  // _AMIBSGD
    UINT16                      Version;                    // 0
    UINT16                      ReoveryMode;                // 1: Recovery, 2: Capsule
    UINT32                      ReoveryAddress;             // AP pointer;
} AMI_BIOS_GUARD_RECOVERY;
#pragma pack ()

typedef struct {
    EFI_FFS_FILE_HEADER         FfsHdr;
    EFI_COMMON_SECTION_HEADER   SecHdr;
    EFI_GUID                    SectionGuid;
    UINT8                       FwCapHdr[0];
} AMI_FFS_COMMON_SECTION_HEADER;

typedef enum
{
    BiosGuardRecoveryMode = 1,
    BiosGuardCapsuleMode = 2
} BIOS_GUARD_RECOVERY_TYPE;

//
//  GUID
//
static EFI_GUID     gAmiBiosGuardSecSmiFlashProtocolGuid = AMI_BIOSGUARD_SEC_SMI_FLASH_PROTOCOL_GUID;
EFI_GUID            BiosGuardCapsuleGuid = BIOS_GUARD_CAPSULE_GUID;
EFI_GUID            BiosGuardRecoveryGuid = BIOS_GUARD_RECOVERY_GUID;
EFI_GUID            BiosGuardRecoveryAddressGuid = BIOS_GUARD_RECOVERY_ADDRESS_GUID;
EFI_GUID            BiosGuardCapsuleVariableGuid = BIOS_GUARD_CAPSULE_VARIABLE_GUID;
extern EFI_GUID     gEfiCapsuleVendorGuid;

//
//  Global Variable
//
static EFI_PHYSICAL_ADDRESS gBiosGuardFwCapsuleBuffer = 0;
UINT32                      gBiosGuardFwCapsuleSize = 0; 
UINT32                      gBiosguardFwCapMaxSize = 0;
static EFI_PHYSICAL_ADDRESS gpFwBiosGuardCapsuleMailbox = 0;
static UINTN                gpFwBiosGuardCapsuleMailboxSize = 0;
static CHAR16               gCapsuleNameBuffer[30];
EFI_CAPSULE_HEADER          *gBiosGuardCapsuleHdr;
EFI_SMM_VARIABLE_PROTOCOL   *mSmmVariable;


//
//  Function Declaration
//
EFI_STATUS LoadBiosGuardFwImage(
    IN OUT FUNC_BLOCK   *pFuncBlock
);

EFI_STATUS SetBiosGuardFlUpdMethod(
    IN OUT FUNC_FLASH_SESSION_BLOCK  *pSessionBlock
);

AMI_BIOSGUARD_SEC_SMI_FLASH_PROTOCOL    BiosGuardSecSmiFlash = {
    LoadBiosGuardFwImage,
    SetBiosGuardFlUpdMethod
};

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// FUNCTION:  ReadRtcIndexedRegister
//
// DESCRIPTION: Used to read RTC register indexed by the argument
//
// Input:
//    IN UINT8    Index        Index of the register to read
//                                                         
//
// Output:
//    UINT8                    Current value of the register
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 ReadRtcIndexedRegister (IN UINT8 Index) {

    BOOLEAN       IntState = GetInterruptState ();
    UINT8         volatile Byte = 0;

    DisableInterrupts ();
    IoWrite8 (0x74, Index);
    Byte = IoRead8 (0x75);
    if (IntState) {
      EnableInterrupts ();
    }
    
    return Byte;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// FUNCTION:  ReadRtcIndexedRegister
//
// DESCRIPTION: Used to write to RTC register indexed by the argument
//
// Input:
//    IN UINT8    Index        Index of the register to write to 
//                                                         
//    IN UINT8  Value          Value to write to the RTC register
//
// Output:
//    VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID WriteRtcIndexedRegister (IN UINT8 Index, IN UINT8 Value) {

    BOOLEAN       IntState = GetInterruptState ();

    DisableInterrupts ();
    IoWrite8 (0x74, Index);
    IoWrite8 (0x75, Value);  
    if (IntState) {
      EnableInterrupts ();
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   S3RTCresume
//
// Description: This function puts system into ACPI S3 State.
//              if token ENABLE_RTC_ONE_SECOND_WAKEUP = 1, then it setups RTC
//              1 second alarm as well.
//
// Input:       None
//
// Output:      None, system will enter ACPI S3 State.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID S3RTCresume (VOID)
{
    EFI_STATUS  Status;
    UINT32      IoData;
    UINT8       Hour, Minute, Second;
    BOOLEAN     inBCD = TRUE;
    UINTN       Index;
    UINT8       RetryIteration;

    // determine if RTC is in BCD mode
    if( ReadRtcIndexedRegister(0xB) & 0x4 ) // bit 2
        inBCD = FALSE;
    // wait for time update to complete before reading the values
    while( ReadRtcIndexedRegister(0xA) & 0x80 ); // while bit 7 is set the 
                                                // time update is in progress
    //read current hour, minute, second
    Hour = ReadRtcIndexedRegister(0x4);
    Minute = ReadRtcIndexedRegister(0x2);
    Second = ReadRtcIndexedRegister(0x0);

    //convert second to decimal from BCD and increment by 1
    if(inBCD)
        Second = (Second >> 4) * 10 + (Second & 0x0F);
    Second += 2;
    
    if(Second > 59){
        Second -= 60;
        if(inBCD)
            Minute = (Minute >> 4) * 10 + (Minute & 0x0F);
        Minute++;
        if(Minute > 59){
            Minute = 0;
            if(inBCD)
                Hour = (Hour >> 4) * 10 + (Hour & 0x0F);
            Hour++;
            // check 24 hour mode/12 hour mode
            if( ReadRtcIndexedRegister(0xB) & 0x2 ) {// bit 1 1=24hour else 12 hour
                if(Hour > 23)
                    Hour = 0;
            } else {
                if(Hour > 11)
                    Hour = 0;
            }

            if(inBCD)
                Hour = Hour % 10 + ( (Hour / 10) << 4 ) ;
        }
        if(inBCD)
            Minute = Minute % 10 + ( (Minute / 10) << 4 ) ;
    }

    //convert from decimal to BCD
    if(inBCD)
        Second = Second % 10 + ( (Second / 10) << 4 ) ;
    
    //set the alarm
    WriteRtcIndexedRegister(0x5, Hour);
    WriteRtcIndexedRegister(0x3, Minute);
    WriteRtcIndexedRegister(0x1, Second);
    // Clear date alarm.
    WriteRtcIndexedRegister(0xD, ( ReadRtcIndexedRegister(0xD) & (1 << 7) ));
    //enable the alarm
    WriteRtcIndexedRegister(0xB, ( ReadRtcIndexedRegister(0xB) | ((UINT8)( 1 << 5 )) ));

// ========== PORTING REQUIRED ===========================================================
//  Current implementation to simulate the Warm Reboot may not be sufficient on some platforms. 
//  S3 transition may require additional Chipset/Platform coding.
//  If needed add any necessary OEM hooks to be able to put the system into S3 at the end of this handler
//========================================================================================

    //
    // Flush cache into memory before we go to sleep. It is necessary for S3 sleep
    // because the changes in gpFwCapsuleMailbox are only in cache now
    //
    for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
      Status = EFI_NOT_READY;
      for (RetryIteration = 0; (RetryIteration < AP_SAFE_RETRY_LIMIT) && (Status != EFI_SUCCESS); RetryIteration++) {
        Status = gSmst->SmmStartupThisAp (AsmWbinvd, Index, NULL);
      }
    }

    //
    // In S3 case, we give 0.1 second delay to make sure all the data are successfully moved from CPU cache to physical memory
    //
    MicroSecondDelay (100 * 1000);
    
    //set RTC_EN bit in PM1_EN to wake up from the alarm
    IoWrite16(PM_BASE_ADDRESS + 0x02, ( IoRead16(PM_BASE_ADDRESS + 0x02) | (1 << 10) ));
    
    //Disable Sleep SMI to avoid SMI re-entrance.
    IoWrite16(PM_BASE_ADDRESS + 0x30, ( IoRead16(PM_BASE_ADDRESS + 0x30) & (~BIT4) ));
    
    //modify power management control register to reflect S3
    IoData = IoRead32(PM_BASE_ADDRESS + 0x04);
    //following code is applicable to Intel PCH only. 
    IoData &= ~(0x1C00);
    IoData |= 0x1400; //Suspend to RAM

    IoWrite32(PM_BASE_ADDRESS + 0x04, IoData );
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// FUNCTION:  WarmResetSys
//
// DESCRIPTION: Use the warm reset before capsule flash 
//
// Input:
//    VOID
//
// Output:
//    VOID
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID WarmResetSys (VOID)
{
    UINTN       Index = 0;
    UINT8       RetryIteration = 0;
    EFI_STATUS  Status = EFI_SUCCESS;
    
#if !BIOS_GUARD_CAPSULE_RESET_TYPE
    //
    // Make the S3 if this Token is disabled
    //
    S3RTCresume ();
    return;
#endif
	
    IoWrite8 (0xcf9, 0);
    //
    // Flush cache into memory before we go to sleep. It is necessary for S3 sleep
    // because the changes in gpFwCapsuleMailbox are only in cache now
    //
    for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
      Status = EFI_NOT_READY;
      for (RetryIteration = 0; (RetryIteration < AP_SAFE_RETRY_LIMIT) && (Status != EFI_SUCCESS); RetryIteration++) {
        Status = gSmst->SmmStartupThisAp (AsmWbinvd, Index, NULL);
      }
    }

    //
    // Give 0.5 second delay to make sure all the data are successfully moved from CPU cache to physical memory
    //
    MicroSecondDelay (500 * 1000);

    //
    // Warm reset
    //
    IoWrite8 (0xcf9, 0x6);
    
    ///
    /// Waiting for system reset
    ///
    CpuDeadLoop ();
}

/**
 * Capsule NVRAM variables are owned by either the runtime Capsule Update service 
 * or by this driver. Each service must not override previously created instances 
 * of the variable and create a new one with n+1 index
 */
CHAR16* GetLastFwCapsuleVariable()
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN Index = 0;
    EFI_PHYSICAL_ADDRESS    IoData=0;
    UINTN                   Size;

    Swprintf_s(gCapsuleNameBuffer, 30, L"%s", EFI_CAPSULE_VARIABLE_NAME);
    // get any NVRAM variable of the format L"CapsuleUpdateDataN" where N is an integer
    while(!EFI_ERROR(Status)) {
        if(Index > 0 )
            Swprintf_s(gCapsuleNameBuffer, 30, L"%s%d", EFI_CAPSULE_VARIABLE_NAME, Index);
        Size = sizeof(EFI_PHYSICAL_ADDRESS);
        Status = mSmmVariable->SmmGetVariable( gCapsuleNameBuffer, &gEfiCapsuleVendorGuid,
                                    NULL,
                                    &Size, 
                                    &IoData);
        DEBUG((DEBUG_INFO, "Get '%S' %r, %lX\n", gCapsuleNameBuffer, Status, IoData));
        if(!EFI_ERROR(Status) && 
           (IoData == gpFwBiosGuardCapsuleMailbox && 
            !EFI_ERROR(AmiValidateSmramBuffer((VOID*)IoData, gpFwBiosGuardCapsuleMailboxSize)))
        ) {
            break;
        }
        Index++;
    }

    return gCapsuleNameBuffer;
}

/**    
    This function prepares Capsule Update EFI Variable

    @param pSessionBlock

    @retval  EFI_SUCCESS - Set Capsule Update EFI Variable success.
**/
EFI_STATUS 
UpdateCapsule (
    IN FUNC_FLASH_SESSION_BLOCK *pSessionBlock
)
{
    EFI_STATUS                      Status;
    EFI_CAPSULE_BLOCK_DESCRIPTOR    *pCapsuleMailboxPtr;
    UINT32                          Attributes;
    CHAR16                          *EfiFwCapsuleVarName;
    AMI_BIOS_GUARD_RECOVERY         *ApRecoveryAddress;
    UINTN                           BiosGuardRecoveryAddressVarSize = sizeof(UINT32);

    pCapsuleMailboxPtr = (EFI_CAPSULE_BLOCK_DESCRIPTOR*)(UINTN)gpFwBiosGuardCapsuleMailbox;
    
    // Initial gBiosGuardCapsuleHdr
    CopyGuid(&gBiosGuardCapsuleHdr->CapsuleGuid, &BiosGuardCapsuleGuid);
    gBiosGuardCapsuleHdr->HeaderSize = sizeof(EFI_CAPSULE_HEADER);
    gBiosGuardCapsuleHdr->CapsuleImageSize = gBiosGuardFwCapsuleSize + sizeof(EFI_CAPSULE_HEADER);
    gBiosGuardCapsuleHdr->Flags = CAPSULE_FLAGS_PERSIST_ACROSS_RESET;

    pCapsuleMailboxPtr[0].Length = gBiosGuardCapsuleHdr->HeaderSize;
    pCapsuleMailboxPtr[0].Union.DataBlock = (EFI_PHYSICAL_ADDRESS)(UINTN)gBiosGuardCapsuleHdr;
    
    pCapsuleMailboxPtr[1].Length = gBiosGuardFwCapsuleSize;
    pCapsuleMailboxPtr[1].Union.DataBlock = (EFI_PHYSICAL_ADDRESS)(UINTN)gBiosGuardFwCapsuleBuffer;
    
    pCapsuleMailboxPtr[2].Length = 0;
    pCapsuleMailboxPtr[2].Union.DataBlock = 0;

    EfiFwCapsuleVarName = GetLastFwCapsuleVariable();
    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS;

    //Clear old Capsule Update EFI Variable
    mSmmVariable->SmmSetVariable( EfiFwCapsuleVarName, &gEfiCapsuleVendorGuid, 0, 0, NULL);

    //Set Capsule Update EFI Variable
    Status = mSmmVariable->SmmSetVariable( EfiFwCapsuleVarName, 
                                           &gEfiCapsuleVendorGuid, 
                                           Attributes, 
                                           sizeof(EFI_PHYSICAL_ADDRESS), 
                                           (VOID*)&gpFwBiosGuardCapsuleMailbox);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = mSmmVariable->SmmGetVariable( L"BiosGuardRecoveryAddressVariable",
                                           &BiosGuardRecoveryAddressGuid,
                                           NULL,
                                           &BiosGuardRecoveryAddressVarSize,
                                           &ApRecoveryAddress);
    if ( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR (Status);
        return Status;
    }

    ApRecoveryAddress->ReoveryMode = BiosGuardCapsuleMode;
    ApRecoveryAddress->ReoveryAddress = (UINT32)gpFwBiosGuardCapsuleMailbox;
    
    return Status;
}

/**    
    This function save update Bios Image to gBiosGuardFwCapsuleBuffer

    @param pFuncBlock

    @retval  EFI_SUCCESS - Load image success.
    @retval  EFI_DEVICE_ERROR - Load image fail.
**/
EFI_STATUS LoadBiosGuardFwImage(
    IN OUT FUNC_BLOCK   *pFuncBlock
)
{
#if BIOS_GUARD_DEBUG_MODE
    DEBUG((DEBUG_INFO, "LoadBiosGuardFwImage enter\n"));
#endif    
    
    if(gBiosGuardFwCapsuleBuffer == 0 || pFuncBlock == NULL)
        return EFI_DEVICE_ERROR;

    pFuncBlock->ErrorCode = 1;
    
#if BIOS_GUARD_DEBUG_MODE    
    DEBUG((DEBUG_INFO,"BiosGuard SecSMI. LoadImage at %lX, Size is 0x%X\n",(UINTN)gBiosGuardFwCapsuleBuffer + pFuncBlock->BlockAddr, pFuncBlock->BlockSize));
#endif    
    if(EFI_ERROR(AmiValidateMemoryBuffer(pFuncBlock, sizeof(FUNC_BLOCK))))
        return EFI_DEVICE_ERROR;

    // prevent leaking of the SMM code to the external buffer
    if(EFI_ERROR(AmiValidateMemoryBuffer((VOID*)(pFuncBlock->BufAddr), pFuncBlock->BlockSize)))
        return EFI_DEVICE_ERROR;

    // assuming the address in 0 based offset in new ROM image
    if(((UINT64)pFuncBlock->BlockAddr + pFuncBlock->BlockSize) > (UINT64)gBiosguardFwCapMaxSize) {
        DEBUG((DEBUG_INFO, "Error : pFuncBlock->BlockAddr + pFuncBlock->BlockSize > gBiosguardFwCapMaxSize(%lx)\n", gBiosguardFwCapMaxSize));
        return EFI_DEVICE_ERROR;
    }

    MemCpy((VOID*)((UINTN)gBiosGuardFwCapsuleBuffer + pFuncBlock->BlockAddr), 
            (VOID*)pFuncBlock->BufAddr, pFuncBlock->BlockSize);

    pFuncBlock->ErrorCode = (UINT8)MemCmp(
        (VOID*)((UINTN)gBiosGuardFwCapsuleBuffer + pFuncBlock->BlockAddr), 
        (VOID*)pFuncBlock->BufAddr, pFuncBlock->BlockSize)==0?0:1;
    
    gBiosGuardFwCapsuleSize += pFuncBlock->BlockSize;

    return (pFuncBlock->ErrorCode == 0) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}

/**    
    This function will set FLASH_UPDATE_VAR

    @param pSessionBlock

    @retval EFI_SUCCESS - FLASH_UPDATE_VAR set success.
    @retval EFI_DEVICE_ERROR - FLASH_UPDATE_VAR set fail.
**/
EFI_STATUS SetBiosGuardFlashUpdateVar (
    IN FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
){
    EFI_STATUS                      Status;
    AMI_FLASH_UPDATE_BLOCK          FlashUpdDesc;
    UINTN                           FlashUpdDescSize = sizeof(AMI_FLASH_UPDATE_BLOCK);
    EFI_GUID                        FlashUpdGuid  = FLASH_UPDATE_GUID;
    UINT32                          Attributes;
    EFI_CAPSULE_BLOCK_DESCRIPTOR    *pCapsuleMailboxPtr;
    AMI_BIOS_GUARD_RECOVERY         *ApRecoveryAddress;
    UINTN                           BiosGuardRecoveryAddressVarSize = sizeof(UINT32);
    
    if(pSessionBlock->FlUpdBlock.FlashOpType == FlRecovery &&
        pSessionBlock->FlUpdBlock.FwImage.AmiRomFileName[0] == 0) {
        return EFI_DEVICE_ERROR;
    }
    
    SetMem((void*)&FlashUpdDesc, FlashUpdDescSize, 0 );
    CopyMem(&FlashUpdDesc.FwImage.AmiRomFileName, &pSessionBlock->FlUpdBlock.FwImage.AmiRomFileName[0], 16); // AmiRomFileName size is 16
    DEBUG((DEBUG_INFO, "Image Name %s, Size %x\n", FlashUpdDesc.FwImage.AmiRomFileName, pSessionBlock->FlUpdBlock.ImageSize));
    FlashUpdDesc.ImageSize = pSessionBlock->FlUpdBlock.ImageSize;
    FlashUpdDesc.ROMSection = BIOS_GUARD_RECOVERY_ROM_SECTION;
    FlashUpdDesc.FlashOpType = FlRecovery;
    
    Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;    
    mSmmVariable->SmmSetVariable ( FLASH_UPDATE_VAR, &FlashUpdGuid, Attributes, 0, NULL);
    Status = mSmmVariable->SmmSetVariable ( FLASH_UPDATE_VAR,
                                            &FlashUpdGuid,
                                            Attributes,
                                            FlashUpdDescSize,
                                            &FlashUpdDesc);
    if ( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR (Status);
        return EFI_DEVICE_ERROR;
    }
    
    // Initial gBiosGuardCapsuleHdr
    CopyGuid(&gBiosGuardCapsuleHdr->CapsuleGuid, &BiosGuardRecoveryGuid);
    gBiosGuardCapsuleHdr->HeaderSize = sizeof(EFI_CAPSULE_HEADER);
    gBiosGuardCapsuleHdr->CapsuleImageSize = 0;
    gBiosGuardCapsuleHdr->Flags = 0;
    
    pCapsuleMailboxPtr = (EFI_CAPSULE_BLOCK_DESCRIPTOR*)(UINTN)gpFwBiosGuardCapsuleMailbox;       
    pCapsuleMailboxPtr[0].Length = gBiosGuardCapsuleHdr->HeaderSize;
    pCapsuleMailboxPtr[0].Union.DataBlock = (EFI_PHYSICAL_ADDRESS)(UINTN)gBiosGuardCapsuleHdr;    
    pCapsuleMailboxPtr[1].Length = 0;
    pCapsuleMailboxPtr[1].Union.DataBlock = 0;    
    
    Status = mSmmVariable->SmmGetVariable( L"BiosGuardRecoveryAddressVariable",
                                           &BiosGuardRecoveryAddressGuid,
                                           NULL,
                                           &BiosGuardRecoveryAddressVarSize,
                                           &ApRecoveryAddress);
    if ( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR (Status);
        return EFI_DEVICE_ERROR;
    }
    
    ApRecoveryAddress->ReoveryMode = BiosGuardRecoveryMode;
    ApRecoveryAddress->ReoveryAddress = (UINT32)gpFwBiosGuardCapsuleMailbox;
    
    return EFI_SUCCESS;
}

/**    
    This function set BiosGuard flash update mode(capsule or recovery).
    
    @param  pSessionBlock - 

    @retval  EFI_SUCCESS - set BiosGuard flash update method success
    @retval  EFI_DEVICE_ERROR - set BiosGuard flash update method fail
**/
EFI_STATUS SetBiosGuardFlUpdMethod(
    IN OUT FUNC_FLASH_SESSION_BLOCK    *pSessionBlock
)
{
    EFI_STATUS          Status;
#if BIOS_GUARD_DEBUG_MODE    
    DEBUG((DEBUG_INFO, "SetBiosGuardFlUpdMethod enter\n"));
#endif    
    
    Status = EFI_DEVICE_ERROR;
    
    if(EFI_ERROR(AmiValidateMemoryBuffer(pSessionBlock, sizeof(FUNC_FLASH_SESSION_BLOCK))))
        return Status;

#if BIOS_GUARD_DEBUG_MODE    
    DEBUG((DEBUG_INFO, "pSessionBlock->FlUpdBlock.FlashOpType = %x\n", pSessionBlock->FlUpdBlock.FlashOpType));
#endif    
    
    switch(pSessionBlock->FlUpdBlock.FlashOpType)
    {
        case FlCapsule:
            if(gBiosGuardFwCapsuleBuffer == 0 || gBiosGuardFwCapsuleSize == 0)
                break;
            
            Status = UpdateCapsule(pSessionBlock);
#if BIOS_GUARD_DEBUG_MODE            
            DEBUG((DEBUG_INFO, "UpdateCapsule (%r)\n", Status));
#endif            
            break;
        case FlRecovery:
            Status = SetBiosGuardFlashUpdateVar (pSessionBlock);
#if BIOS_GUARD_DEBUG_MODE            
            DEBUG((DEBUG_INFO, "SetBiosGuardFlashUpdateVar (%r)\n", Status));
#endif            
            break;
        default:
            Status = EFI_DEVICE_ERROR;
    }

    
    // Set Error Status
    if (Status != EFI_SUCCESS) { 
        //pSessionBlock->FSHandle  = 0;
        //pSessionBlock->ErrorCode = 1;
        return EFI_DEVICE_ERROR;
    }
    
    return EFI_SUCCESS;
}

/**    
  This function set related variable for BiosGuard capsule/recovery
  and change S5 to S3.

  @param[in] Handle    Handle of the callback
  @param[in] Context   The dispatch context

  @retval EFI_SUCCESS  
**/
EFI_STATUS
BiosGuardRecoveryS5SmiHandler (
    IN EFI_HANDLE                DispatchHandle,
    IN CONST VOID                *DispatchContext OPTIONAL,
    IN OUT VOID                  *CommBuffer OPTIONAL,
    IN OUT UINTN                 *CommBufferSize OPTIONAL
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    UINTN                           BiosGuardRecoveryAddressVarSize = sizeof(UINT32);
    AMI_BIOS_GUARD_RECOVERY         *ApRecoveryAddress;
    EFI_CAPSULE_BLOCK_DESCRIPTOR    *BiosGuardCapsuleBlockDesc;
    EFI_CAPSULE_HEADER              *BiosGuardCapsuleHeader;
    UINTN                           BiosGuardCapsuleAddressVarSize = sizeof(AMI_BIOS_GUARD_RECOVERY);
    
    DEBUG ((DEBUG_INFO, "BiosGuardRecoveryS5SmiHandler Start\n"));

    Status = mSmmVariable->SmmGetVariable( L"BiosGuardRecoveryAddressVariable",
                                           &BiosGuardRecoveryAddressGuid,
                                           NULL,
                                           &BiosGuardRecoveryAddressVarSize,
                                           &ApRecoveryAddress);
    if ( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR (Status);
        return EFI_SUCCESS;
    }
    
    DEBUG ((DEBUG_INFO, "ApRecoveryAddress->ReoveryAddress : %x\n", ApRecoveryAddress->ReoveryAddress));
    if ( (ApRecoveryAddress->ReoveryAddress == 0) || (ApRecoveryAddress->ReoveryAddress == 0xFFFFFFF) ) return EFI_SUCCESS;
    
    BiosGuardCapsuleBlockDesc = (EFI_CAPSULE_BLOCK_DESCRIPTOR*)ApRecoveryAddress->ReoveryAddress;
    DEBUG ((DEBUG_INFO, "Length[0] : %x\n", BiosGuardCapsuleBlockDesc->Length));
    DEBUG ((DEBUG_INFO, "Union[0].DataBlock : %x\n", BiosGuardCapsuleBlockDesc->Union.DataBlock));
    DEBUG ((DEBUG_INFO, "Union[0].ContinuationPointer : %x\n", BiosGuardCapsuleBlockDesc->Union.ContinuationPointer));
    
    DEBUG ((DEBUG_INFO, "Length[1] : %x\n", BiosGuardCapsuleBlockDesc[1].Length));
    DEBUG ((DEBUG_INFO, "Union[1].DataBlock : %x\n", BiosGuardCapsuleBlockDesc[1].Union.DataBlock));
    DEBUG ((DEBUG_INFO, "Union[1].ContinuationPointer : %x\n", BiosGuardCapsuleBlockDesc[1].Union.ContinuationPointer));
    
    BiosGuardCapsuleHeader = (EFI_CAPSULE_HEADER*)((UINTN)BiosGuardCapsuleBlockDesc->Union.DataBlock);
    
    DEBUG ((DEBUG_INFO, "BiosGuardCapsuleHeader->CapsuleGuid : %g\n", &BiosGuardCapsuleHeader->CapsuleGuid));
    DEBUG ((DEBUG_INFO, "BiosGuardCapsuleHeader->HeaderSize : %X\n", BiosGuardCapsuleHeader->HeaderSize));
    DEBUG ((DEBUG_INFO, "BiosGuardCapsuleHeader->Flags : %X\n", BiosGuardCapsuleHeader->Flags));
    DEBUG ((DEBUG_INFO, "BiosGuardCapsuleHeader->CapsuleImageSize : %X\n", BiosGuardCapsuleHeader->CapsuleImageSize));   
    
    if ( CompareGuid (&BiosGuardRecoveryGuid, &BiosGuardCapsuleHeader->CapsuleGuid) ||
         CompareGuid (&BiosGuardCapsuleGuid, &BiosGuardCapsuleHeader->CapsuleGuid) ) {
        DEBUG ((DEBUG_INFO, "Start to do BIOS Guard Capsule\n"));
        
        Status = mSmmVariable->SmmSetVariable ( L"BiosGuardCapsuleVariable",
                                                &BiosGuardCapsuleVariableGuid,
                                                (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE),
                                                BiosGuardCapsuleAddressVarSize,
                                                ApRecoveryAddress);
        if ( EFI_ERROR(Status) ) {
            ASSERT_EFI_ERROR (Status);
            return EFI_SUCCESS;
        }

        WarmResetSys ();
    }
    
    DEBUG ((DEBUG_INFO, "BiosGuardRecoveryS5SmiHandler End\n"));
    return EFI_SUCCESS;
}

/**    
    

    @param VOID

    @retval VOID
**/
EFI_STATUS EFIAPI BiosGuardSecSMIFlashDriverEntryPoint (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      Handle, SxDispatchHandle;
    VOID                            *Memory = NULL;
    EFI_SMM_SX_DISPATCH2_PROTOCOL   *SxDispatch;
    EFI_SMM_SX_REGISTER_CONTEXT     SxContext = {SxS5, SxEntry};
    ICX_MSR_PLATFORM_INFO_REGISTER    PlatformInfoMsr;
    MSR_PLAT_FRMW_PROT_CTRL_REGISTER  PlatFirmCtrlMsr;
    
    DEBUG((DEBUG_INFO, "BiosGuardSecSMIFlashDriverEntryPoint\n"));
    
    PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
    if (PlatformInfoMsr.Bits.PfatEnable == 0) {
        PlatFirmCtrlMsr.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
        if (PlatFirmCtrlMsr.Bits.PfatEnable == 0) {
            DEBUG ((DEBUG_INFO, "BIOS Guard is disabled or not supported \n"));
            return EFI_SUCCESS;
        }
     }
    
    //
    // Reserve memory for BiosGuard Capsule/Recovery
    //
    gBiosguardFwCapMaxSize = BIOSGUARD_RECOVERY_IMAGE_SIZE;    
    DEBUG((DEBUG_INFO, "gBiosguardFwCapMaxSize = %x\n", gBiosguardFwCapMaxSize));
    
    gBiosGuardFwCapsuleBuffer = 0xffffffff;
    Status = gBS->AllocatePages(AllocateMaxAddress, EfiReservedMemoryType, EFI_SIZE_TO_PAGES(gBiosguardFwCapMaxSize), &gBiosGuardFwCapsuleBuffer);
    DEBUG((DEBUG_INFO, "Allocate BiosGuardCapsuleBuffer %lX,0x%X %r\n",gBiosGuardFwCapsuleBuffer, gBiosguardFwCapMaxSize, Status));
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    
    SetMem((void*)gBiosGuardFwCapsuleBuffer, gBiosguardFwCapMaxSize, 0 );
    
    //
    // Install BiosGuard Secure SMI Flash Protocol 
    //
    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface(
                 &Handle,
                 &gAmiBiosGuardSecSmiFlashProtocolGuid,
                 EFI_NATIVE_INTERFACE,
                 &BiosGuardSecSmiFlash
             );
    ASSERT_EFI_ERROR(Status);
    
    //
    // Reserve pool in smm runtime memory for capsule's mailbox list
    //
    gpFwBiosGuardCapsuleMailboxSize = 4 * sizeof(EFI_CAPSULE_BLOCK_DESCRIPTOR) + sizeof(EFI_CAPSULE_HEADER); // (4*16)+28
    Status = gSmst->SmmAllocatePool(EfiRuntimeServicesData, gpFwBiosGuardCapsuleMailboxSize, &Memory);
    gpFwBiosGuardCapsuleMailbox = (EFI_PHYSICAL_ADDRESS)(UINTN)Memory;    
    
    DEBUG((DEBUG_INFO, "Allocate gpFwBiosGuardCapsuleMailbox %lX,0x%X %r\n", gpFwBiosGuardCapsuleMailbox, gpFwBiosGuardCapsuleMailboxSize, Status));
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    SetMem((void*)gpFwBiosGuardCapsuleMailbox, gpFwBiosGuardCapsuleMailboxSize, 0 );
    
    //
    // Reserve pool for BiosGuardCapsuleHeader
    //
    //Status = gSmst->SmmAllocatePool(EfiRuntimeServicesData, sizeof(EFI_CAPSULE_HEADER), &gBiosGuardCapsuleHdr);
    Status = gBS->AllocatePool(EfiRuntimeServicesData, sizeof(EFI_CAPSULE_HEADER), &gBiosGuardCapsuleHdr);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    SetMem((void*)gBiosGuardCapsuleHdr, sizeof(EFI_CAPSULE_HEADER), 0 );
    
    //
    // Locate SmmVariableProtocol
    //
    Status = gSmst->SmmLocateProtocol(
                 &gEfiSmmVariableProtocolGuid,
                 NULL,
                 (VOID**)&mSmmVariable
             );
    if ( EFI_ERROR(Status) ) {
        ASSERT_EFI_ERROR (Status);
        return Status;
    }
    
    //
    // Register S5 call back function
    //
    Status = gSmst->SmmLocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, &SxDispatch);
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR(Status)) {
        Status  = SxDispatch->Register (SxDispatch, \
                                        BiosGuardRecoveryS5SmiHandler, \
                                        &SxContext, \
                                        &SxDispatchHandle);
        ASSERT_EFI_ERROR (Status);
    }
    
    return Status;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
