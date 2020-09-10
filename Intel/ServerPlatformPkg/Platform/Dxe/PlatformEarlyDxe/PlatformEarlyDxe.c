/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include "PlatformEarlyDxe.h"
#include <IioUniversalData.h>
#include <Protocol/SiliconRegAccess.h>

#include <Protocol/ReportStatusCodeHandler.h>
#include <Library/SystemInfoLib.h>
#include <Protocol/PrintBootPrompt.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#if AMT_SUPPORT
#include "Library/AmtSupportLib.h"
#include "Library/MeTypeLib.h"
#include <Library/MeUtilsLib.h>
#endif // AMT_SUPPORT
#include <Library/UefiBootManagerLib.h>
#include <Library/SiliconWorkaroundLib.h>

USRA_PROTOCOL     *mUsraProtocol = NULL;

IIO_UDS           *mIioUds;
CPU_CSR_ACCESS_VAR *mCpuCsrAccessVarPtr;

/**
  Notification function of EVT_GROUP_READY_TO_BOOT event group.

  This is a notification function registered on EVT_GROUP_READY_TO_BOOT event group.
  When the Boot Manager is about to load and execute a boot option, it reclaims variable
  storage if free size is below the threshold.

  @param[in] Event        Event whose notification function is being invoked.
  @param[in] Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
OnReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{

  gBS->CloseEvent (Event);

}

/**

  This routine sets the Setup boot option to active so it will automatically
  launch without the need for an F2 key press during the hotkey window.

  @param VOID

  @retval Nothing

**/
VOID
ForceSetup (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION    BootOption;
  EFI_STATUS                      Status;

  // If ForceBoot set to Setup, or if we are coming from above boot to shell, boot to Setup.
  // Setup boot option must be overridden by MFG mode, even if we change the above logic to boot to Shell
  Status = EfiBootManagerGetBootManagerMenu (&BootOption);
  ASSERT_EFI_ERROR (Status);
  EfiBootManagerBoot(&BootOption);
  EfiBootManagerFreeLoadOption (&BootOption);
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
PrintBootPrompt (
  IN PRINT_BOOT_PROMPT_PROTOCOL     *This,
  IN EFI_BOOT_MODE                  BootMode
  )
{
  CHAR16            *BootPrompt;
  EFI_HII_HANDLE    mHiiHandle;

#if AMT_SUPPORT
  UINTN           Index = 0;
  EFI_STATUS      Status = EFI_SUCCESS;
  EFI_STRING      String;
#endif // AMT_SUPPORT

  mHiiHandle = HiiAddPackages (
                 &gEfiCallerIdGuid,
                 gImageHandle,
                 PlatformEarlyDxeStrings,
                 NULL
                 );
  if (mHiiHandle == NULL) {

    ASSERT (mHiiHandle != NULL);
    return;
  }

  switch (BootMode) {
    case BOOT_WITH_MFG_MODE_SETTINGS:
        BootPrompt = HiiGetString (mHiiHandle, STRING_TOKEN (STR_MFG_MODE_PROMPT), NULL);
        break;
    default:
        BootPrompt = HiiGetString (mHiiHandle, STRING_TOKEN (STR_BOOT_PROMPT), NULL);
  }

  if (BootPrompt == NULL) {
    return;
  }

  Print (BootPrompt);

  FreePool (BootPrompt);

#if AMT_SUPPORT
  if (MeClientIsCorporate () && MeClientIsReady ()) {
    while (Status == EFI_SUCCESS) {
      Status = AmtGetSetupPrompt (&Index, &String);
      if ((!EFI_ERROR(Status) && (String != NULL))) {
        Print (L"     %s\n", String);
      }
    }
  }
#endif // AMT_SUPPORT
}

PRINT_BOOT_PROMPT_PROTOCOL mPrintBootPrompt = {
  PRINT_BOOT_PROMPT_VERSION_1,
  PrintBootPrompt
};

/**

  This function will retrieve the DPR data from HOBs produced by MRC
  and will use it to program the DPR registers in IIO and in PCH

  @param VOID

  @retval VOID

**/
VOID
ProgramDprRegs (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PLATFORM_TXT_DEVICE_MEMORY  *PlatformTxtMemory;
  EFI_HOB_GUID_TYPE               *GuidHob      = NULL;
  VOID                            *HobData      = NULL;
  EFI_HANDLE                      Handle        = NULL;
  volatile UINT64                 *Ptr64;
  UINT64                          Value64;
  UINT32                          Value32;
  UINTN                           PciAddress;
  UINT32                          Temp32;
  UINT8                           Stack;
  UINT8                           Socket;
  USRA_ADDRESS                    Address;

  DEBUG((DEBUG_INFO, "ProgramDprRegs : Entry\n"));

  //
  // Get Txt Device Memory HOB if it was published
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);

  if (GuidHob != NULL) {

    HobData = GET_GUID_HOB_DATA (GuidHob);
    PlatformTxtMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY*)HobData;

    //
    // Loop thru all IIO stacks of all sockets that are present
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1<<Socket)) == 0) {
        continue;
      }
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) == 0) {
          continue;
        }
        DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Stack));
        //
        // Read IIO LTDPR Register current value
        //
        USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, LTDPR_IIO_VTD_REG);
        mUsraProtocol->RegRead(&Address, &Value32);
        if ((PlatformTxtMemory->DMAProtectionMemoryRegionAddress != 0) && ((Value32 & 0x0FF5) == 0)) {
          //
          // Program IIO LTDPR Register if not programmed already
          //
          Temp32 = ((UINT32)(PlatformTxtMemory->DMAProtectionMemoryRegionSize) >> 16) | SA_DPR_LOCK | SA_DPR_PRM;
          Value32 = (Value32 & (~(0x0FF5))) | Temp32;
          mUsraProtocol->RegWrite(&Address, &Value32);
          mUsraProtocol->RegRead(&Address, &Value32);
          DEBUG ((EFI_D_INFO, "\tLTDPR_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Value32));
          //
          // Save for S3 resume
          //
          PciAddress = mUsraProtocol->GetRegAddr(&Address);

          S3BootScriptSaveMemWrite (
             S3BootScriptWidthUint32,
             (UINTN)PciAddress,
             1,
             &Value32
             );

          DEBUG ((EFI_D_INFO, "\tLTDPR_IIO_VTD_REG_PCI_ADDRESS[0x%08x] written with 0x%08x, ", PciAddress, Value32));
          //
          // Verify register value by reading it back
          // need to double check DPR REG//ASSERT (((Value32 & 0xFF0) << 16) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize);
          //
          Value32 = *(UINT32*)PciAddress;
          DEBUG ((DEBUG_INFO, "read back as 0x%08x\n", Value32));
          if (((Value32 & 0xFF0) << 16) != PlatformTxtMemory->DMAProtectionMemoryRegionSize){
            DEBUG ((EFI_D_ERROR, "\tLTDPR_IIO_VTD_REG_PCI_ADDRESS[0x%02x] = 0x%08x not set correctly!!!\n", PciAddress, Value32));
          }
        }
      }
    }

    //
    // Program DPR register in LT Public Space on PCH
    //
    Ptr64   = (UINT64 *) (UINTN) (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_DMA_PROTECTED_RANGE_REG_OFF);
    Value64 = RShiftU64 (PlatformTxtMemory->DMAProtectionMemoryRegionSize, 16) | SA_DPR_LOCK | SA_DPR_PRM;
    //Add top of the DPR
    Value64 |= ((PlatformTxtMemory->DMAProtectionMemoryRegionAddress + PlatformTxtMemory->DMAProtectionMemoryRegionSize) & 0xFFF00000);
    *Ptr64  = Value64;
    Value64 = *Ptr64;
    //
    // Save for S3 resume if LT DPR Reg was programmed correctly and clear the protected region
    //
    if (((LShiftU64 ((Value64 & 0xFFE), 16)) == PlatformTxtMemory->DMAProtectionMemoryRegionSize) &&
      (PlatformTxtMemory->DMAProtectionMemoryRegionAddress != 0)) {
       DEBUG ((DEBUG_INFO, "\tTXT Public Space DPR reg (at FED3_0000 + 0x330) = 0x%08x\n", (UINT32) Value64));
       S3BootScriptSaveMemWrite (
           S3BootScriptWidthUint64,
           (UINT64) (UINTN) (Ptr64),
           2,
           &Value64
           );
       //
       // Clear out the DMA protected memory area for use.
       //
       ZeroMem (
         (VOID *)(UINTN)PlatformTxtMemory->DMAProtectionMemoryRegionAddress,
         (UINTN)PlatformTxtMemory->DMAProtectionMemoryRegionSize
       );
    }else {
      DEBUG ((EFI_D_ERROR, "\tTXT Public Space DPR reg programming error!!!\n"));
    }


  } else {
    DEBUG((EFI_D_WARN, "\tEfiPlatformTxtDeviceMemoryGuid HOB not found. DPR regs not programmed.\n"));
  }
  //
  // Install dummy protocol to signal DPR regs are programmed.
  // Note that SMM driver BiosGuardServices depends on this protocol, so we always install it here.
  //
  Status = gBS->InstallProtocolInterface (
        &Handle,
        &gEfiDprRegsProgrammedGuid,
        EFI_NATIVE_INTERFACE,
        NULL
        );
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO, "ProgramDprRegs : Exit\n"));

  return;
}

/**

  This function will program the legacy range and RCBA range into the
  General Protected Range registers in IIO.

  @param VOID

  @retval VOID

**/
VOID
ProgramGenProtRangeRegs (
  VOID
  )
{
  USRA_ADDRESS              Address;
  UINT32                    Data32;
  UINT8                     Socket;
  UINT8                     Stack;

  DEBUG ((EFI_D_INFO, "ProgramGenProtRangeRegs : Entry\n"));

  //
  // Loop thru all IIO stacks of all sockets that are present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1<<Socket)) == 0) {
      continue;
    }
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1<<Stack)) == 0) {
        continue;
      }
      DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Stack));
      //
      //  Legacy address ranges need to be protected from inbound memory reads to prevent CTOs.
      //  (HSD 3614978, 3876023, 3247123, 3876024, 3876008)
      //
      Data32 = 0x0000;   // Base address of PAM region [63:32]
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE2_BASE_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE2_BASE_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = 0xA0000;   // Base address of PAM region [31:16], [15:0] assumed zero
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE2_BASE_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE2_BASE_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = 0x0000;   // Limit address of PAM region [63:32]
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE2_LIMIT_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE2_LIMIT_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = 0xF0000;   // Limit address of PAM region [31:16], [15:0] assumed one
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE2_LIMIT_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE2_LIMIT_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      //
      // Cover SPI MMIO region with GenProtRange register
      //
      Data32 = 0x0000;   // Base address  [63:32] of SPI region
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE1_BASE_N1_IIO_VTD_REG);
// APTIOV_SERVER_OVERRIDE_RC_START : Restore SPI region for BIOSGuard.
      if (FeaturePcdGet (PcdUsraSupportS3))
        Address.Attribute.S3Enable = USRA_ENABLE;
// APTIOV_SERVER_OVERRIDE_RC_END : Restore SPI region for BIOSGuard.
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE1_BASE_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = PCH_SPI_BASE_ADDRESS & 0xFFFF0000;  // Base address of SPI MMIO region is 0xFE010000
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE1_BASE_N0_IIO_VTD_REG);
// APTIOV_SERVER_OVERRIDE_RC_START : Restore SPI region for BIOSGuard.
      if (FeaturePcdGet (PcdUsraSupportS3))
        Address.Attribute.S3Enable = USRA_ENABLE;
// APTIOV_SERVER_OVERRIDE_RC_END : Restore SPI region for BIOSGuard.
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE1_BASE_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = 0x0000;   // Limit address of SPI region
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE1_LIMIT_N1_IIO_VTD_REG);
// APTIOV_SERVER_OVERRIDE_RC_START : Restore SPI region for BIOSGuard.
      if (FeaturePcdGet (PcdUsraSupportS3))
        Address.Attribute.S3Enable = USRA_ENABLE;
// APTIOV_SERVER_OVERRIDE_RC_END : Restore SPI region for BIOSGuard.
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE1_LIMIT_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));

      Data32 = (PCH_SPI_BASE_ADDRESS + 0x1000) & 0xFFFFF000;  // End of SPI Base region is 0xFE011000
      USRA_CSR_OFFSET_ADDRESS(Address, Socket, Stack, GENPROTRANGE1_LIMIT_N0_IIO_VTD_REG);
// APTIOV_SERVER_OVERRIDE_RC_START : Restore SPI region for BIOSGuard.
      if (FeaturePcdGet (PcdUsraSupportS3))
        Address.Attribute.S3Enable = USRA_ENABLE;
// APTIOV_SERVER_OVERRIDE_RC_END : Restore SPI region for BIOSGuard.
      mUsraProtocol->RegWrite(&Address, &Data32);
      mUsraProtocol->RegRead(&Address, &Data32);
      DEBUG ((EFI_D_INFO, "\tGENPROTRANGE1_LIMIT_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Data32));
    }
  }

  DEBUG ((EFI_D_INFO, "ProgramGenProtRangeRegs : Exit\n"));
}

/**

  This function will set up the IMR according to PCD values.
  This is done by writing Isolated Memory Region registers in IIO.

  @param VOID

  @retval VOID

**/
VOID
ProgramImrRegs (
  VOID
  )
{
#if !defined (SKX_HOST)
  USRA_ADDRESS                     Address;
  UINT8                            Socket;
  UINT8                            Stack;
  UINT32                           Temp;
  IMR0BASE_N0_IIO_VTD_STRUCT Imr0Base0;
  IMR0BASE_N1_IIO_VTD_STRUCT Imr0Base1;
  IMR0MASK_N0_IIO_VTD_STRUCT Imr0Mask0;
  IMR0MASK_N1_IIO_VTD_STRUCT Imr0Mask1;
  IMR0RAC_N0_IIO_VTD_STRUCT  Imr0Rac0;
  IMR0RAC_N1_IIO_VTD_STRUCT  Imr0Rac1;
  IMR0WAC_N0_IIO_VTD_STRUCT  Imr0Wac0;
  IMR0WAC_N1_IIO_VTD_STRUCT  Imr0Wac1;

  DEBUG ((EFI_D_INFO, "ProgramImrRegs : Entry\n"));

  if (PcdGetBool (PcdImr0Enable) == FALSE) {
    DEBUG ((EFI_D_INFO, "ProgramImrRegs : Exit\n"));
    return; // IMR0 is not enabled - skip programming
  }

  //
  // Set up IMR0 values according to PCDs
  //
  Temp = PcdGet64 (PcdImr0Base) && 0xFFFFFFFF; // Bits 43:12 of IMR base
  Imr0Base0.Bits.imr_base = Temp;
  Temp = (PcdGet64 (PcdImr0Base) >> 32) && 0xFFFFFFFF; // Bits 52:44
  Imr0Base1.Bits.imr_base = Temp;
  Imr0Base1.Bits.imr_en = 1; // Enable IMR0

  Temp = PcdGet64 (PcdImr0Mask) && 0xFFFFFFFF; // Bits 43:12 of IMR mask
  Imr0Mask0.Bits.imr_mask = Temp;
  Temp = (PcdGet64 (PcdImr0Mask) >> 32) && 0xFFFFFFFF; // Bits 52:44
  Imr0Mask1.Bits.imr_mask = Temp;

  Temp = PcdGet64 (PcdImr0Rac) && 0xFFFFFFFF; // Lower bits of read policy
  Imr0Rac0.Bits.imr0_sai_pol = Temp;
  Temp = (PcdGet64 (PcdImr0Rac) >> 32) && 0xFFFFFFFF; // Upper bits
  Imr0Rac1.Bits.imr0_sai_pol = Temp;

  Temp = PcdGet64 (PcdImr0Wac) && 0xFFFFFFFF; // Lower bits of write policy
  Imr0Wac0.Bits.imr0_sai_pol = Temp;
  Temp = (PcdGet64 (PcdImr0Wac) >> 32) && 0xFFFFFFFF; // Upper bits
  Imr0Wac1.Bits.imr0_sai_pol = Temp;

  //
  // Loop through all IIO stacks of all sockets that are present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
    }
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) == 0) {
        continue;
      }

      //
      // Write registers for this IIO
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0BASE_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Base0.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0BASE_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Base1.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0MASK_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Mask0.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0MASK_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Mask1.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0RAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Rac0.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0RAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Rac1.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0WAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Wac0.Data);
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR0WAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegWrite (&Address, &Imr0Wac1.Data);
    }
  }

  DEBUG ((EFI_D_INFO, "ProgramImrRegs : Exit\n"));
#endif // #if !defined (SKX_HOST)
}

/**

  Setup IMR2 (CPM and nCPM Region).

  @param Start        - IMR2 starting address
  @param Length       - IMR2 size in bytes

  @retval EFI_SUCCESS - IMR2 configured

**/
VOID
EFIAPI
ProgramImr2Regs (
  VOID
  )
{
#if !defined(SKX_HOST)
  EFI_HOB_GUID_TYPE                       *GuidHob    = NULL;
  IMR2BASE_N1_IIO_VTD_STRUCT              Imr2baseN1;
  IMR2BASE_N0_IIO_VTD_STRUCT              Imr2baseN0;
  IMR2MASK_N1_IIO_VTD_STRUCT              Imr2maskN1;
  IMR2MASK_N0_IIO_VTD_STRUCT              Imr2maskN0;
  IMR2RAC_N1_IIO_VTD_STRUCT               Imr2racN1;
  IMR2RAC_N0_IIO_VTD_STRUCT               Imr2racN0;
  IMR2WAC_N1_IIO_VTD_STRUCT               Imr2wacN1;
  IMR2WAC_N0_IIO_VTD_STRUCT               Imr2wacN0;
  USRA_ADDRESS                            Address;
  UINT8                                   Imr2BaseAddressHigh;
  UINT32                                  Imr2BaseAddressLow;
  EFI_PHYSICAL_ADDRESS                    Imr2BaseAddress;
  UINT8                                   Imr2MaskHigh;
  UINT32                                  Imr2MaskLow;
  UINT64                                  Imr2Size;
  UINT8                                   Socket;
  UINT8                                   Stack;

  DEBUG ((EFI_D_INFO, "ProgramImr2Regs : Entry\n"));

  //
  // Verify if IMR2 is enabled
  //
  if (PcdGetBool (PcdImr2Enable) == FALSE) {
    DEBUG ((EFI_D_INFO, "\tIMR2 is not enabled\n"));
    DEBUG ((EFI_D_INFO, "ProgramImr2Regs : Exit\n"));
    return; // Skip programming
  }

  //
  // Verify if IMR2 size is valid
  //
  if (PcdGet64 (PcdImr2Size) == 0) {
    DEBUG ((EFI_D_ERROR, "\tError: IMR2 size is equal to 0\n"));
    DEBUG ((EFI_D_ERROR, "ProgramImr2Regs : Exit\n"));
    return; // Skip programming
  }

  //
  // Get IMR2 Base Address HOB if it was published
  //
  GuidHob = GetFirstGuidHob (&gImr2BaseAddressHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tError: Data not found\n"));
    DEBUG ((EFI_D_ERROR, "ProgramImr2Regs : Exit\n"));
    return; // Skip programming
  }
  Imr2BaseAddress = (EFI_PHYSICAL_ADDRESS)(*(EFI_PHYSICAL_ADDRESS *)GET_GUID_HOB_DATA (GuidHob));
  DEBUG ((EFI_D_INFO, "\tImr2BaseAddress = 0x%016lx\n", (UINT64)Imr2BaseAddress));

  //
  // Extracting High and Low address
  //
  Imr2BaseAddressLow  = (UINT32) RShiftU64 (Imr2BaseAddress, 12);
  Imr2BaseAddressHigh = (UINT8)  RShiftU64 (Imr2BaseAddress, 32);

  //
  // Getting IMR2 Mask from IMR2 Size
  //
  Imr2Size     = PcdGet64 (PcdImr2Size) - 1;
  DEBUG ((EFI_D_INFO, "\tImr2Size = 0x%016lx\n",PcdGet64 (PcdImr2Size)));
  Imr2Size     = ~Imr2Size;
  Imr2Size     = (UINT64) RShiftU64 (Imr2Size, 12);
  Imr2MaskLow  = (UINT32) Imr2Size;
  Imr2MaskHigh = (UINT8)  RShiftU64 (Imr2Size, 32);

  //
  // Loop through all IIO stacks of all sockets that are present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) == 0) {
        continue;
      }
      DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Stack));

      //
      // Configure IMR2 Base Address
      // Base address must be a power of 2, aligned to its size, not overlap any other IMR region or system reserved region
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2BASE_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2baseN0.Data);
      Imr2baseN0.Data = (UINT32) Imr2BaseAddressLow;
      mUsraProtocol->RegWrite (&Address, &Imr2baseN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr2baseN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2BASE_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2baseN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2BASE_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2baseN1.Data);
      Imr2baseN1.Bits.imr_base = (UINT8) Imr2BaseAddressHigh;
      Imr2baseN1.Bits.imr_en = 1;
      mUsraProtocol->RegWrite (&Address, &Imr2baseN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr2baseN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2BASE_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2baseN1.Data));

      //
      // Configure IMR2 Mask
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2MASK_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2maskN0.Data);
      Imr2maskN0.Data = (UINT32) Imr2MaskLow;
      mUsraProtocol->RegWrite (&Address, &Imr2maskN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr2maskN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2MASK_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2maskN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2MASK_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2maskN1.Data);
      Imr2maskN1.Bits.imr_mask = (UINT8) Imr2MaskHigh;
      mUsraProtocol->RegWrite (&Address, &Imr2maskN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr2maskN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2MASK_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2maskN1.Data));

      //
      // Allow nIQAT & IQAT Read & Write Access Only
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2RAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2racN0.Data);
      Imr2racN0.Bits.imr2_sai_pol |= BIT26; // BIT26 for nIQAT
      mUsraProtocol->RegWrite (&Address, &Imr2racN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr2racN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2RAC_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2racN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2RAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2racN1.Data);
      Imr2racN1.Bits.imr2_sai_pol |= BIT28; // BIT60 for IQAT
      mUsraProtocol->RegWrite (&Address, &Imr2racN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr2racN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2RAC_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2racN1.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2WAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2wacN0.Data);
      Imr2wacN0.Bits.imr2_sai_pol |= BIT26; // BIT26 for nIQAT
      mUsraProtocol->RegWrite (&Address, &Imr2wacN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr2wacN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2WAC_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2wacN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR2WAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr2wacN1.Data);
      Imr2wacN1.Bits.imr2_sai_pol |= BIT28; // BIT60 for IQAT
      mUsraProtocol->RegWrite (&Address, &Imr2wacN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr2wacN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR2WAC_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr2wacN1.Data));
    } // End for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
  } // End of for (Socket = 0; Socket < MAX_SOCKET; Socket++)

  DEBUG ((EFI_D_INFO, "ProgramImr2Regs : Exit\n"));
#endif // #if !defined(SKX_HOST)
}

/**

  Setup IMR3 (TXT Region).

  @param Start        - IMR3 starting address
  @param Length       - IMR3 size in bytes

  @retval EFI_SUCCESS - IMR3 configured

**/
VOID
EFIAPI
ProgramImr3Regs (
  VOID
  )
{
#if !defined(SKX_HOST)
  EFI_HOB_GUID_TYPE                       *GuidHob    = NULL;
  VOID                                    *HobData    = NULL;
  EFI_PLATFORM_TXT_DEVICE_MEMORY          *PlatformTxtMemory;
  IMR3BASE_N1_IIO_VTD_STRUCT              Imr3baseN1;
  IMR3BASE_N0_IIO_VTD_STRUCT              Imr3baseN0;
  IMR3MASK_N1_IIO_VTD_STRUCT              Imr3maskN1;
  IMR3MASK_N0_IIO_VTD_STRUCT              Imr3maskN0;
  IMR3RAC_N1_IIO_VTD_STRUCT               Imr3racN1;
  IMR3RAC_N0_IIO_VTD_STRUCT               Imr3racN0;
  IMR3WAC_N1_IIO_VTD_STRUCT               Imr3wacN1;
  IMR3WAC_N0_IIO_VTD_STRUCT               Imr3wacN0;
  USRA_ADDRESS                            Address;
  UINT8                                   Imr3BaseAddressHigh;
  UINT32                                  Imr3BaseAddressLow;
  EFI_PHYSICAL_ADDRESS                    Imr3BaseAddress;
  UINT8                                   Imr3MaskHigh;
  UINT32                                  Imr3MaskLow;
  UINT64                                  Imr3Size;
  UINT8                                   Socket;
  UINT8                                   Stack;

  DEBUG ((EFI_D_INFO, "ProgramImr3Regs : Entry\n"));

  //
  // Verify if IMR3 is enabled
  //
  if (PcdGetBool (PcdImr3Enable) == FALSE || !IsSiliconWorkaroundEnabled ("S2207292289")) {
    DEBUG ((EFI_D_INFO, "\tIMR3 is not enabled\n"));
    DEBUG ((EFI_D_INFO, "ProgramImr3Regs : Exit\n"));
    return; // Skip programming
  }

  //
  // Get TXT Device Memory HOB if it was published
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tError: Data not found\n"));
    DEBUG ((EFI_D_ERROR, "ProgramImr3Regs : Exit\n"));
    return; // Skip programming
  }
  HobData = GET_GUID_HOB_DATA (GuidHob);
  PlatformTxtMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY*)HobData;


  //
  // Verify if IMR3 address and size are valid
  //
  if (PlatformTxtMemory->DMAProtectionMemoryRegionAddress == 0 || PlatformTxtMemory->DMAProtectionMemoryRegionSize == 0) {
    DEBUG ((EFI_D_ERROR, "\tError: IMR3 address or/and size is/are equal to 0\n"));
    DEBUG ((EFI_D_ERROR, "ProgramImr3Regs : Exit\n"));
    return; // Skip programming
  }

  Imr3BaseAddress = PlatformTxtMemory->DMAProtectionMemoryRegionAddress;
  DEBUG ((EFI_D_INFO, "\tImr3BaseAddress = 0x%016lx\n", (UINT64)Imr3BaseAddress));

  //
  // Extracting High and Low address
  //
  Imr3BaseAddressLow  = (UINT32) RShiftU64 (Imr3BaseAddress, 12);
  Imr3BaseAddressHigh = (UINT8)  RShiftU64 (Imr3BaseAddress, 32);

  //
  // Getting IMR3 Mask from IMR3 Size
  //
  Imr3Size     = PlatformTxtMemory->DMAProtectionMemoryRegionSize - 1;
  DEBUG ((EFI_D_INFO, "\tImr3Size = 0x%016lx\n",PlatformTxtMemory->DMAProtectionMemoryRegionSize));
  Imr3Size     = ~Imr3Size;
  Imr3Size     = (UINT64) RShiftU64 (Imr3Size, 12);
  Imr3MaskLow  = (UINT32) Imr3Size;
  Imr3MaskHigh = (UINT8)  RShiftU64 (Imr3Size, 32);

  //
  // Loop through all IIO stacks of all sockets that are present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) == 0) {
        continue;
      }
      DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Stack));

      //
      // Configure IMR3 Base Address
      // Base address must be a power of 2, aligned to its size, not overlap any other IMR region or system reserved region
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3BASE_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3baseN0.Data);
      Imr3baseN0.Data = (UINT32) Imr3BaseAddressLow;
      mUsraProtocol->RegWrite (&Address, &Imr3baseN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr3baseN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3BASE_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3baseN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3BASE_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3baseN1.Data);
      Imr3baseN1.Bits.imr_base = (UINT8) Imr3BaseAddressHigh;
      Imr3baseN1.Bits.imr_en = 1;
      mUsraProtocol->RegWrite (&Address, &Imr3baseN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr3baseN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3BASE_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3baseN1.Data));

      //
      // Configure IMR3 Mask
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3MASK_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3maskN0.Data);
      Imr3maskN0.Data = (UINT32) Imr3MaskLow;
      mUsraProtocol->RegWrite (&Address, &Imr3maskN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr3maskN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3MASK_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3maskN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3MASK_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3maskN1.Data);
      Imr3maskN1.Bits.imr_mask = (UINT8) Imr3MaskHigh;
      mUsraProtocol->RegWrite (&Address, &Imr3maskN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr3maskN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3MASK_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3maskN1.Data));

      //
      // Allow Read & Write Access to:
      // DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI | PM_PCS | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI
      //
      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3RAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3racN0.Data);
      Imr3racN0.Bits.imr3_sai_pol |= (BIT24 | BIT9 | BIT3);
      mUsraProtocol->RegWrite (&Address, &Imr3racN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr3racN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3RAC_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3racN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3RAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3racN1.Data);
      Imr3racN1.Bits.imr3_sai_pol |= (BIT10); //BIT42
      mUsraProtocol->RegWrite (&Address, &Imr3racN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr3racN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3RAC_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3racN1.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3WAC_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3wacN0.Data);
      Imr3wacN0.Bits.imr3_sai_pol |= (BIT24 | BIT9 | BIT3);
      mUsraProtocol->RegWrite (&Address, &Imr3wacN0.Data);
      mUsraProtocol->RegRead(&Address, &Imr3wacN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3WAC_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3wacN0.Data));

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMR3WAC_N1_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &Imr3wacN1.Data);
      Imr3wacN1.Bits.imr3_sai_pol |= (BIT10); //BIT42
      mUsraProtocol->RegWrite (&Address, &Imr3wacN1.Data);
      mUsraProtocol->RegRead(&Address, &Imr3wacN1.Data);
      DEBUG ((EFI_D_INFO, "\tIMR3WAC_N1_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,Imr3wacN1.Data));

    } // End for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
  } // End of for (Socket = 0; Socket < MAX_SOCKET; Socket++)

  DEBUG ((EFI_D_INFO, "ProgramImr3Regs : Exit\n"));
#endif // #if !defined(SKX_HOST)
}

/**

  Remove BOOT BIOS from IMR Global  .

  @param Start        - None
  @param Length       - None

  @retval EFI_SUCCESS - None

**/
VOID
EFIAPI
ProgramImrGlobalRegs(
  VOID
  )
{
#if !defined(SKX_HOST)
  IMRGLOBAL_BM_CP_N0_IIO_VTD_STRUCT       ImrGlobalBmCpN0;
  IMRGLOBAL_BM_WAC_N0_IIO_VTD_STRUCT      ImrGlobalWacN0;
  IMRGLOBAL_CP_N0_IIO_VTD_STRUCT          ImrGlobalCpN0;
  USRA_ADDRESS                            Address;
  UINT8                                   Socket;
  UINT8                                   Stack;
  DEBUG ((EFI_D_INFO, "ProgramImr3Regs : Entry\n"));

  //
  // Loop through all IIO stacks of all sockets that are present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((mIioUds->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) == 0) {
        continue;
      }
      DEBUG ((EFI_D_INFO, "\tSocket = 0x%d\tStack = 0x%d\n",Socket,Stack));

      //
      // Removing Bios from the Boot Bios (HOSTIA_BOOT_SAI) mode from the IMR Global registers.
      //
      if (!IsSimicsEnvironment()) {
        USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMRGLOBAL_BM_WAC_N0_IIO_VTD_REG);
        mUsraProtocol->RegRead(&Address, &ImrGlobalWacN0.Data);
        ImrGlobalWacN0.Bits.imrglobal_bm_sai_pol &= 0xFFFFFFEF;// clear BIT4
        mUsraProtocol->RegWrite (&Address, &ImrGlobalWacN0.Data);
        mUsraProtocol->RegRead(&Address, &ImrGlobalWacN0.Data);
        DEBUG ((EFI_D_INFO, "\tIMRGLOBAL_BM_WAC_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,ImrGlobalWacN0.Data));

        USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMRGLOBAL_BM_CP_N0_IIO_VTD_REG);
        mUsraProtocol->RegRead(&Address, &ImrGlobalBmCpN0);
        ImrGlobalBmCpN0.Bits.imrglobal_bm_ctrl_pol &= 0xFFFFFFEF;// Clear BIT4
        mUsraProtocol->RegWrite (&Address, &ImrGlobalBmCpN0.Data);
        mUsraProtocol->RegRead(&Address, &ImrGlobalBmCpN0.Data);
        DEBUG ((EFI_D_INFO, "\tIMRGLOBAL_BM_CP_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,ImrGlobalBmCpN0.Data));
      }

      USRA_CSR_OFFSET_ADDRESS (Address, Socket, Stack, IMRGLOBAL_CP_N0_IIO_VTD_REG);
      mUsraProtocol->RegRead(&Address, &ImrGlobalCpN0.Data);
      ImrGlobalCpN0.Bits.imr_ctrl_pol &= 0xFFFFFFEF;// Clear BIT4
      mUsraProtocol->RegWrite (&Address, &ImrGlobalCpN0.Data);
      mUsraProtocol->RegRead(&Address, &ImrGlobalCpN0.Data);
      DEBUG ((EFI_D_INFO, "\tIMRGLOBAL_CP_N0_IIO_VTD_REG[0x%08x] = 0x%08x\n",Address.Csr.Offset,ImrGlobalCpN0.Data));
    }
  }
#endif // #if !defined(SKX_HOST)
}

VOID
SetBiosInfoFlagWpe (
  VOID
  )
/*++

Routine Description:

  Sets the WPE bit of the BIOS Info Flags MSR to enable Anti-Flash wearout protection
  within BIOS Guard

Arguments:

  None

Returns:

  None

--*/
{
  AsmWriteMsr64 (ICX_MSR_BIOS_INFO_FLAGS, B_MSR_BIOS_INFO_FLAGS_WPE);

  return;
}

VOID
EFIAPI
EnableAntiFlashWearout (
      EFI_EVENT                 Event,
      VOID                      *Context
  )
/*++

Routine Description:

  Function to set the WPE bit of the BIOS Info Flags MSR to enable Anti-Flash wearout
  protection within BIOS Guard before booting to the OS

Arguments:

  IN EFI_EVENT         Event
  IN VOID              *Context

Returns:

  None

--*/
{
  EFI_STATUS                    Status;
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  SYSTEM_CONFIGURATION          SetupData;

  Status = GetSpecificConfigGuid (&gEfiSetupVariableGuid, (VOID *)&SetupData);
  ASSERT_EFI_ERROR(Status);
  //
  // First check if Anti-flash wearout feature is supported by platform and Setup variable is enabled
  //
  if (SetupData.AntiFlashWearoutSupported == TRUE && SetupData.EnableAntiFlashWearout) {
    Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpServices
                  );
    ASSERT_EFI_ERROR(Status);

    //
    // Set WPE on BSP, then all other APs
    //
    SetBiosInfoFlagWpe();

    MpServices->StartupAllAPs (
                    MpServices,
                    (EFI_AP_PROCEDURE) SetBiosInfoFlagWpe,
                    FALSE,
                    NULL,
                    0,
                    NULL,
                    NULL
                    );
  }

}

EFI_STATUS
EFIAPI
PlatformEarlyDxeEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
/**

  This is the main entry point of the Platform Early DXE module.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Module launched successfully.

**/
{
  EFI_STATUS            Status = EFI_SUCCESS;
  EFI_EVENT             ReadyToBootEvent;
  UINT32                BspCpuidSignature;
  UINT32                RegEax, RegEbx, RegEcx, RegEdx;
  EFI_HOB_GUID_TYPE     *GuidHob;
  IIO_UDS               *UdsHobPtr;
  EFI_BOOT_MODE         BootMode;
  EFI_GUID              UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HANDLE            Handle;
  PRINT_BOOT_PROMPT_PROTOCOL  PrintBootPrompt;

  DEBUG((DEBUG_INFO, "PlatformEarlyDxeEntry \n"));
  //
  // Get the IIO_UDS data HOB
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  UdsHobPtr = GET_GUID_HOB_DATA(GuidHob);
  //
  // Allocate Memory Pool buffer for IIO_UDS data
  //
  Status = gBS->AllocatePool ( EfiBootServicesData, sizeof (IIO_UDS), (VOID **) &mIioUds );
  ASSERT_EFI_ERROR (Status);
  //
  // Initialize the Memory Pool buffer with the data from the Hand-Off-Block
  //
  CopyMem(mIioUds, UdsHobPtr, sizeof(IIO_UDS));

  mCpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  //
  // Locate USRA protocol  for common register access (this protocol is assumed to have been installed)
  //
  Status = gBS->LocateProtocol (&gDxeUsraProtocolGuid, NULL, &mUsraProtocol);
  ASSERT_EFI_ERROR (Status);
  //
  // Get the boot mode that we are currently in
  //
  BootMode = GetBootModeHob();

  //
  // Program DPR registers with the range from Memory Init
  //
  ProgramDprRegs ();

  //
  // Program the GenProtRange registers for BIOS Guard
  //
  ProgramGenProtRangeRegs ();

  //
  // Program the IMR registers for ME IMR region
  //
  ProgramImrRegs ();

  //
  // Program the IMR2 registers for CPM & nCPM IMR region
  //
  ProgramImr2Regs ();

  //
  // Program the IMR3 registers for TXT Device Memory IMR region
  //
  ProgramImr3Regs ();

  //
  // Program/Remove BOOT BIOS from the IMR Global registers
  //
  ProgramImrGlobalRegs ();

  //
  // Get BSP CPU ID
  // Shift out the stepping
  //
  AsmCpuid (0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;
  if ( (BspCpuidSignature == CPU_FAMILY_SKX) && (BootMode != BOOT_ON_FLASH_UPDATE)) {
    //
    // Register event to set WPE bit in Bios Info Flags MSR to enable Anti Flash wearout
    //
    Status = EfiCreateEventReadyToBootEx (
                TPL_CALLBACK,
                EnableAntiFlashWearout,
                NULL,
                &ReadyToBootEvent
                );
    ASSERT_EFI_ERROR (Status);
  }

  Handle = NULL;
  Status = gBS->LocateProtocol (&gPrintBootPromptProtocolGuid, NULL, (VOID **)&PrintBootPrompt);
  if (Status == EFI_NOT_FOUND) {
    Status = gBS->InstallProtocolInterface (
          &Handle,
          &gPrintBootPromptProtocolGuid,
          EFI_NATIVE_INTERFACE,
          &mPrintBootPrompt
          );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;

}
