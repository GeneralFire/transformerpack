/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2019 Intel Corporation. <BR>

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

#include "BdsPlatform.h"
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include "Library/AmtSupportLib.h"
#include "Library/MeTypeLib.h"
#include <Library/MeUtilsLib.h>
#endif // AMT_SUPPORT
#include <Library/PlatformBootTimeOutLib.h>

#define STRING_WIDTH_80    80
#define STRING_WIDTH_128  128

//
// BDS Platform Functions
//

EFI_HII_HANDLE    mHiiHandle = NULL;

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
InitializeStringSupport (
  VOID
  )
{
  mHiiHandle = HiiAddPackages (
                 &gEfiCallerIdGuid,
                 gImageHandle,
                 PlatformBootManagerLibStrings,
                 NULL
                 );
  ASSERT (mHiiHandle != NULL);
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
PrintBootPrompt (
  VOID
  )
{
  CHAR16          *BootPrompt;

  BootPrompt = HiiGetString (mHiiHandle, STRING_TOKEN (STR_BOOT_PROMPT), NULL);
  if (BootPrompt != NULL) {
    Print (BootPrompt);
    FreePool (BootPrompt);

#if defined(AMT_SUPPORT) && AMT_SUPPORT
    if (MeClientIsCorporate () && MeClientIsReady ()) {
      UINTN      Index = 0;
      EFI_STATUS Status = EFI_SUCCESS;
      EFI_STRING String;

      while (Status == EFI_SUCCESS) {
        Status = AmtGetSetupPrompt (&Index, &String);
        if ((!EFI_ERROR(Status) && (String != NULL))) {
          Print (L"     %s\n", String);
        }
      }
    }
#endif // AMT_SUPPORT
  }
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
PrintMfgModePrompt (
  VOID
  )
{
  CHAR16          *MfgPrompt;

  MfgPrompt = HiiGetString (mHiiHandle, STRING_TOKEN (STR_MFG_MODE_PROMPT), NULL);
  if (MfgPrompt != NULL) {
    Print (MfgPrompt);
    FreePool (MfgPrompt);
  }
}

/**

    GC_TODO: add routine description

    @param TimeoutRemain - GC_TODO: add arg description

    @retval None

**/
VOID
EFIAPI
PlatformBootManagerWaitCallback (
  UINT16          TimeoutRemain
  )
{
  CHAR16                        *TmpStr;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Foreground;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Background;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;
  UINT16                        Timeout;

  Timeout = PcdGet16 (PcdPlatformBootTimeOut);
  //
  // Show progress
  //
  SetMem (&Foreground, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);
  SetMem (&Background, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);
  SetMem (&Color,      sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);

  TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_START_BOOT_OPTION), NULL);
  if (TmpStr != NULL && Timeout != 0) {
    UpdatePlatformBootTimeOutStr(&TmpStr, TimeoutRemain);

    ShowProgress (
      Foreground,
      Background,
      TmpStr,
      Color,
      (Timeout - TimeoutRemain) * 100 / Timeout,
      0
      );
    FreePool (TmpStr);
  }
}

/**
  Perform the memory test base on the memory test intensive level,
  and update the memory resource.

  @param  Level         The memory test intensive level.

  @retval EFI_STATUS    Success test all the system memory and update
                        the memory resource

**/
EFI_STATUS
MemoryTest (
  IN EXTENDMEM_COVERAGE_LEVEL Level
  )
{
  EFI_STATUS                        Status;
  EFI_STATUS                        KeyStatus;
  EFI_STATUS                        InitStatus;
  BOOLEAN                           RequireSoftECCInit;
  EFI_GENERIC_MEMORY_TEST_PROTOCOL  *GenMemoryTest;
  UINT64                            TestedMemorySize;
  UINT64                            TotalMemorySize;
  UINTN                             TestPercent;
  UINT64                            PreviousValue;
  BOOLEAN                           ErrorOut;
  BOOLEAN                           TestAbort;
  EFI_INPUT_KEY                     Key;
  CHAR16                            StrPercent[80];
  CHAR16                            *StrTotalMemory; //128bytes
  CHAR16                            *Pos;
  CHAR16                            *TmpStr;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     Foreground;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     Background;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     Color;
  BOOLEAN                           IsFirstBoot;

  //
  // Use a DynamicHii type pcd to save the boot status, which is used to
  // control configuration mode, such as FULL/MINIMAL/NO_CHANGES configuration.
  //
  IsFirstBoot = PcdGetBool(PcdBootState);

  ZeroMem (&Key, sizeof (EFI_INPUT_KEY));

  Pos = AllocatePool (128);

  if (Pos == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  StrTotalMemory    = Pos;

  TestedMemorySize  = 0;
  TotalMemorySize   = 0;
  PreviousValue     = 0;
  ErrorOut          = FALSE;
  TestAbort         = FALSE;
  TestPercent       = 0;

  SetMem (&Foreground, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);
  SetMem (&Background, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);
  SetMem (&Color, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);

  RequireSoftECCInit = FALSE;

  Status = gBS->LocateProtocol (
                  &gEfiGenericMemTestProtocolGuid,
                  NULL,
                  (VOID **) &GenMemoryTest
                  );
  if (EFI_ERROR (Status)) {
    FreePool (Pos);
    return Status;
  }

  InitStatus = GenMemoryTest->MemoryTestInit (
                                GenMemoryTest,
                                Level,
                                &RequireSoftECCInit
                                );
  if (InitStatus == EFI_NO_MEDIA) {
    //
    // The PEI codes also have the relevant memory test code to check the memory,
    // it can select to test some range of the memory or all of them. If PEI code
    // checks all the memory, this BDS memory test will has no not-test memory to
    // do the test, and then the status of EFI_NO_MEDIA will be returned by
    // "MemoryTestInit". So it does not need to test memory again, just return.
    //
    FreePool (Pos);
    return EFI_SUCCESS;
  }

  TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_ESC_TO_SKIP_MEM_TEST), NULL);

  if (TmpStr != NULL) {
    PrintXY (10, 10, NULL, NULL, TmpStr);
    FreePool (TmpStr);
  }

  do {
    Status = GenMemoryTest->PerformMemoryTest (
                              GenMemoryTest,
                              &TestedMemorySize,
                              &TotalMemorySize,
                              &ErrorOut,
                              TestAbort
                              );
    if (ErrorOut && (Status == EFI_DEVICE_ERROR)) {
      TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_SYSTEM_MEM_ERROR), NULL);
      if (TmpStr != NULL) {
        PrintXY (10, 10, NULL, NULL, TmpStr);
        FreePool (TmpStr);
      }

      ASSERT_EFI_ERROR (Status);
      FreePool (Pos);
      return Status;
    }

    if (TotalMemorySize == 0) {
      DEBUG ((DEBUG_WARN, "WARNING: Memory Test total memory size is 0!\n"));
      TestPercent = 100;
    } else {
      TestPercent = (UINTN) DivU64x32 (
                              DivU64x32 (MultU64x32 (TestedMemorySize, 100), 16),
                              (UINT32) DivU64x32 (TotalMemorySize, 16)
                              );
    }
    if (TestPercent != PreviousValue) {
      UnicodeValueToStringS(StrPercent, sizeof(CHAR16) * STRING_WIDTH_80, 0, TestPercent, 0);
      TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_MEMORY_TEST_PERCENT), NULL);
      if (TmpStr != NULL) {
        //
        // TmpStr size is 64, StrPercent is reserved to 16.
        //
        StrCatS (StrPercent, STRING_WIDTH_80, TmpStr);

        PrintXY (10, 10, NULL, NULL, StrPercent);
        FreePool (TmpStr);
      }

      TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PERFORM_MEM_TEST), NULL);
      if (TmpStr != NULL) {
        ShowProgress (
          Foreground,
          Background,
          TmpStr,
          Color,
          TestPercent,
          (UINTN) PreviousValue
          );
        FreePool (TmpStr);
      }
    }

    PreviousValue = TestPercent;

    KeyStatus     = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    if (!EFI_ERROR (KeyStatus) && (Key.ScanCode == SCAN_ESC)) {
      if (!RequireSoftECCInit) {
        TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PERFORM_MEM_TEST), NULL);
        if (TmpStr != NULL) {
          ShowProgress (
            Foreground,
            Background,
            TmpStr,
            Color,
            100,
            (UINTN) PreviousValue
            );
          FreePool (TmpStr);
        }

        PrintXY (10, 10, NULL, NULL, L"100");
        Status = GenMemoryTest->Finished (GenMemoryTest);
        goto Done;
      }

      TestAbort = TRUE;
    }
  } while (Status != EFI_NOT_FOUND);

  Status = GenMemoryTest->Finished (GenMemoryTest);

Done:
  UnicodeValueToStringS (StrTotalMemory, (STRING_WIDTH_128 - (StrTotalMemory == Pos ? 0 : 1)), COMMA_TYPE, TotalMemorySize, 0);
  if (StrTotalMemory[0] == L',') {
    StrTotalMemory++;
  }

  TmpStr = HiiGetString (mHiiHandle, STRING_TOKEN (STR_MEM_TEST_COMPLETED), NULL);
  if (TmpStr != NULL) {
    //StrTotalMemory will be > *Pos if StrTotalMemory[0] == L','
    StrCatS (StrTotalMemory, (STRING_WIDTH_128 - (StrTotalMemory == Pos ? 0 : 1))/sizeof(CHAR16), TmpStr);

    FreePool (TmpStr);
  }

  PrintXY (10, 10, NULL, NULL, StrTotalMemory);
  ShowProgress (
    Foreground,
    Background,
    StrTotalMemory,
    Color,
    100,
    (UINTN) PreviousValue
    );
  FreePool (Pos);

  return EFI_SUCCESS;
}
