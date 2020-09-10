/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include "GetCpuInfo.h"


#define ALL_CPU_INFO_GUID \
  { \
    0xdbf338fe, 0x93be, 0x46d6, 0xad, 0xf5, 0xc1, 0x7c, 0x22, 0x36, 0xd8, 0x38 \
  }

#define FORMSET_GUID \
  { \
    0xdbf338fe, 0x93be, 0x46d6, 0xad, 0xf5, 0xc1, 0x7c, 0x22, 0x36, 0xd8, 0x38 \
  }

#define STRING_WIDTH 50


EFI_MP_SERVICES_PROTOCOL            *mMpService;
STATIC CHAR16                       *mCpuSerialName;
STATIC CHAR16                       *mPlatformIdString;
STATIC CHAR16                       *mCpuSteppingString;
STATIC CHAR16                       *mCpuMicroCodeRevString;
STATIC CHAR16                       *mCpuIdString;
STATIC CHAR16                       *mCpuCoreFreqString;
STATIC CHAR16                       *mTotalCpuNumString;
STATIC CHAR16                       *mCpuActualFreqString;
STATIC UINTN                        mNumberofCpus;
STATIC UINT64                       MaxFreqCount = 0;
IIO_UDS                             *IioUds;


/**

  Returns the CPU Core Frequency of the processor installed in the system.

  @param None

  @retval Processor Core Frequency in MHz

**/
UINT32
GetCpuCoreFreq (
  VOID
  )
{
  ICX_MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;
  PlatformInfoMsr.Uint32 = (UINT32)AsmReadMsr32 (ICX_MSR_PLATFORM_INFO);
  return (UINT32)((10000 * (PlatformInfoMsr.Bits.MaxNonTurboLimRatio )) /100);
}


/**

  Returns the actual CPU Frequency of the processor installed in the system.

  @param None

  @retval Actual Processor Frequency in MHz

**/
UINT64
GetActualCpuFreq (
  VOID
  )
{
  UINT64      ActualFreqCount;
  UINT64      ActualRatio;

  //
  // Calculate global max frequency cycles for 100 microseconds
  //
  if( MaxFreqCount == 0) {
    AsmWriteMsr64(MSR_IA32_MPERF, 0);
    MicroSecondDelay(100);
    MaxFreqCount = AsmReadMsr64(MSR_IA32_MPERF);
  }

  //
  // Calculate actual frequency cycles for 100 microseconds
  //
  AsmWriteMsr64(MSR_IA32_APERF, 0);
  MicroSecondDelay(100);
  ActualFreqCount = AsmReadMsr64(MSR_IA32_APERF);

  //
  // If actual frequency is greater than max frequency, set to max frequency
  //
  if(ActualFreqCount > MaxFreqCount) {
    ActualFreqCount = MaxFreqCount;
  }

  ActualRatio = (((ActualFreqCount * 100) / MaxFreqCount) * 10000);
  return ((GetCpuCoreFreq() * (ActualRatio / 10000)) / 100);
}


/*

  Get all CPU (BSP+APs) information.

  @param None

  @retval None

*/
VOID
GetCpuInformation (
  VOID
  )
{
  UINTN               CpuIndex;
  CHAR16              NumString[STRING_WIDTH];
  CHAR16              SteppingString[STRING_WIDTH];
  CHAR16              IdString[STRING_WIDTH];
  CHAR16              MicroCodeRevString[STRING_WIDTH];
  CHAR16              CpuIdString[STRING_WIDTH];
  CHAR16              CoreFreqString[STRING_WIDTH];
  CHAR16              ActualFreqString[STRING_WIDTH];
  EFI_STATUS          Status;
  UINT64              PlatformIdQword;
  UINT32              RegEax;
  UINT32              CoreFreq;
  UINT64              ActualFreq;

  Status = mMpService->WhoAmI (
                  mMpService,
                  &CpuIndex
                  );
  ASSERT (!EFI_ERROR (Status));

  //
  // Cpu name: CPU1 ~ CPUn
  //
  UnicodeValueToStringS (
    NumString,
    sizeof(CHAR16) * STRING_WIDTH,
    LEFT_JUSTIFY,
    CpuIndex,
    4
    );
  StrCatS (mCpuSerialName + CpuIndex * STRING_WIDTH, STRING_WIDTH, NumString);

  //
  // Processor Stepping
  //
  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  // Mask off all but the stepping nibble
  UnicodeValueToStringS (
    SteppingString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO | RADIX_HEX,
    RegEax & 0x0F,
    1
    );

  StrCatS (mCpuSteppingString + CpuIndex * STRING_WIDTH, STRING_WIDTH, SteppingString);
  //
  // mCpuIdString
  //
  // Reuse previous CPUID fetch data, but use all of it this time.
  UnicodeValueToStringS (
    CpuIdString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO | RADIX_HEX,
    RegEax,
    8
    );

  StrCatS (mCpuIdString + CpuIndex * STRING_WIDTH, STRING_WIDTH, CpuIdString);
  //
  // Platform ID
  //
  PlatformIdQword = AsmReadMsr64 (MSR_IA32_PLATFORM_ID);
  UnicodeValueToStringS (
    IdString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO | RADIX_HEX,
    PlatformIdQword,
    16
  );

  StrCatS (mPlatformIdString + CpuIndex * STRING_WIDTH, STRING_WIDTH, IdString);
  //
  // Microcode Revision
  //
  UnicodeValueToStringS (
    MicroCodeRevString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO | RADIX_HEX,
    GetCurrentMicrocodeSignature (),
    8
    );

  StrCatS (mCpuMicroCodeRevString + CpuIndex * STRING_WIDTH, STRING_WIDTH, MicroCodeRevString);
  //
  // mCpuCoreFreqString
  //
  CoreFreq = GetCpuCoreFreq();
  UnicodeValueToStringS (
    CoreFreqString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO,
    CoreFreq,
    8
    );

  StrCatS (mCpuCoreFreqString + CpuIndex * STRING_WIDTH, STRING_WIDTH, CoreFreqString);

  //
  // mCpuActualFreqString
  //
  ActualFreq = GetActualCpuFreq();
  UnicodeValueToStringS (
    ActualFreqString,
    sizeof(CHAR16) * STRING_WIDTH,
    PREFIX_ZERO,
    ActualFreq,
    8
    );

  StrCatS (mCpuActualFreqString + CpuIndex * STRING_WIDTH, STRING_WIDTH, ActualFreqString);

}

/**

    The driver Entry Point.

    @param ImageHandle - EFI_HANDLE
    @param SystemTable - EFI_SYSTEM_TABLE

    @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
GetCpuInfoInit (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HII_HANDLE      HiiHandle;
  GET_CPU_INFO        *GetCpuInfo;
  UINTN               Count;
  CHAR16              TempString[STRING_WIDTH];
  EFI_GUID            GetCpuInfoGuid = ALL_CPU_INFO_GUID;
  UINTN               NumberOfEnabledProcessors;
  EFI_GUID            UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE   *GuidHob;

  GetCpuInfo = NULL;

  //
  // Get the IIO_UDS HOB data stored in the PEI driver.
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
      return EFI_NOT_FOUND;
  }
  IioUds = GET_GUID_HOB_DATA(GuidHob);

  Status = gBS->LocateProtocol (
      &gEfiMpServiceProtocolGuid,
      NULL,
      &mMpService
      );
  if (EFI_ERROR (Status)) {
    return Status ;
  }

  Status = mMpService->GetNumberOfProcessors (
                       mMpService,
                       &mNumberofCpus,
                       &NumberOfEnabledProcessors
                       );


  //
  // Allocate space for total number string
  //
  mTotalCpuNumString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * STRING_WIDTH);
  if (mTotalCpuNumString == NULL) {
    goto Error;
  }
  //
  // Allocate space for CPU serial number (CPU0 ~ CPUn)
  //
  mCpuSerialName = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuSerialName == NULL) {
    goto Error;
  }
  //
  // Allocate space for CPU platform id
  //
  mPlatformIdString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mPlatformIdString == NULL) {
    goto Error;
  }
  //
  // Allocate space for CPU stepping
  //
  mCpuSteppingString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuSteppingString == NULL) {
    goto Error;
  }
  //
  // Allocate space for CPU microcode revision
  //
  mCpuMicroCodeRevString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuMicroCodeRevString == NULL) {
    goto Error;
  }
  //
  // Allocate space for mCpuIdString
  //
  mCpuIdString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuIdString == NULL) {
    goto Error;
  }
  //
  // Allocate space for mCpuCoreFreqString
  //
  mCpuCoreFreqString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuCoreFreqString == NULL) {
    goto Error;
  }
  //
  // Allocate space for mCpuActualFreqString
  //
  mCpuActualFreqString = (CHAR16 *)AllocateZeroPool (sizeof(CHAR16) * mNumberofCpus * STRING_WIDTH);
  if (mCpuActualFreqString == NULL) {
    goto Error;
  }

  StrCpyS(mTotalCpuNumString, STRING_WIDTH, L"Total CPU Number: ");
  UnicodeValueToStringS (
      TempString,
      sizeof(CHAR16) * STRING_WIDTH,
      LEFT_JUSTIFY,
      mNumberofCpus,
      5);
  StrCatS (mTotalCpuNumString, STRING_WIDTH, TempString);

  for(Count=0; Count<mNumberofCpus; Count++) {
    StrCpyS(mCpuSerialName+Count*STRING_WIDTH, STRING_WIDTH, L"CPU");
    StrCpyS(mPlatformIdString+Count*STRING_WIDTH, STRING_WIDTH, L"PlatformID: ");
    StrCpyS(mCpuSteppingString+Count*STRING_WIDTH, STRING_WIDTH, L"Stepping: ");
    StrCpyS(mCpuMicroCodeRevString+Count*STRING_WIDTH, STRING_WIDTH, L"MicroCodeRev: ");
    StrCpyS(mCpuIdString+Count*STRING_WIDTH, STRING_WIDTH, L"CPUID: ");
    StrCpyS(mCpuCoreFreqString+Count*STRING_WIDTH, STRING_WIDTH, L"CpuCoreFreq (MHz): ");
    StrCpyS(mCpuActualFreqString+Count*STRING_WIDTH, STRING_WIDTH, L"ActualCpuFreq (MHz): ");
  }

  //
  // Save BSP information
  //
  GetCpuInformation();
  //
  // Save all APs information
  //
  mMpService->StartupAllAPs (
         mMpService,
         (EFI_AP_PROCEDURE)GetCpuInformation,
         FALSE,
         NULL,
         0,
         NULL,
         NULL
         );

  GetCpuInfo = AllocateZeroPool (sizeof (GET_CPU_INFO));
  if (GetCpuInfo == NULL) {
    goto Error;
  }


  HiiHandle = HiiAddPackages (
                   &GetCpuInfoGuid,
                   NULL,
                   GetCpuInfoStrings,
                   VfrBin,
                   NULL
                   );
  GetCpuInfo->RegisteredHandle = HiiHandle;

  //
  // Display CPU information
  //
  RegisterAllCpuInfo (GetCpuInfo);

Error:
  if (mTotalCpuNumString != NULL) {
    gBS->FreePool (mTotalCpuNumString);
  }
  if (mCpuSerialName != NULL) {
    gBS->FreePool (mCpuSerialName);
  }
  if (mPlatformIdString != NULL) {
    gBS->FreePool (mPlatformIdString);
  }
  if (mCpuSteppingString != NULL) {
    gBS->FreePool (mCpuSteppingString);
  }
  if (mCpuMicroCodeRevString != NULL) {
    gBS->FreePool (mCpuMicroCodeRevString);
  }
  if (mCpuIdString != NULL) {
    gBS->FreePool (mCpuIdString);
  }
  if (mCpuCoreFreqString != NULL) {
    gBS->FreePool (mCpuCoreFreqString);
  }
  if (mCpuActualFreqString != NULL) {
    gBS->FreePool (mCpuActualFreqString);
  }
  if (GetCpuInfo != NULL) {
    gBS->FreePool (GetCpuInfo);
  }
  return EFI_SUCCESS;
}

/**


  This function converts CPU related data as ASCII and put in provided string

    @param String            - pointer for CPU Info string
    @param Lang              - pointer to the value of the PlatformLang variable
    @param StartOpCodeHandle - pointer for StartOpCodeHandle
    @param Private           - Used to display cpu info

    @retval None

**/
VOID
UpdateTokenValue (
  CHAR16              *String,
  CHAR8               *Lang,
  VOID                *StartOpCodeHandle,
  GET_CPU_INFO        *Private
  )
{
    EFI_STRING_ID   TokenToUpdate;

    TokenToUpdate = 0;
    TokenToUpdate = HiiSetString (
                      Private->RegisteredHandle,
                      0,
                      String,
                      NULL
                      );

    HiiCreateTextOpCode (
      StartOpCodeHandle,
      TokenToUpdate,
      STRING_TOKEN (STR_NULL_STRING),
      STRING_TOKEN (STR_NULL_STRING)
      );
}


#define LABEL_BEGIN  0x1234
#define LABEL_END    0x1235
EFI_GUID mFormSetGuid = FORMSET_GUID;

/**

  This is the function that update the information in page.

  @param Private -    Used to display cpu info

  Returns -    Always successful

**/
EFI_STATUS
EFIAPI
RegisterAllCpuInfo (
  IN GET_CPU_INFO       *Private
  )
{
  UINTN                            Index;
  VOID                            *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL              *StartLabel;
  VOID                            *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL              *EndLabel;
  CHAR8                           *Lang;
  EFI_STATUS                       Status;

  Lang   = NULL;
  //
  // Get the current platform language setting
  //
  Status = GetEfiGlobalVariable2 (L"PlatformLang", &Lang, NULL);
  //This variable is not found on first boot.
  //However, code below doesn't use it anyway.
  //Commenting error check code for now.
  /*
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) return Status;
  */

  //
  // Initialize the container for dynamic opcodes
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);

  //
  // If we failed to allocate handle, skip creating the opcodes
  //
  if((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL)){
    if(StartOpCodeHandle != NULL){
      HiiFreeOpCodeHandle(StartOpCodeHandle);
    }
    if(EndOpCodeHandle != NULL){
      HiiFreeOpCodeHandle (EndOpCodeHandle);
    }
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LABEL_BEGIN;

  //
  // Create Hii Extend Label OpCode as the end opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = LABEL_END;

  //
  // Total CPU number
  //
  HiiSetString (
    Private->RegisteredHandle,
    STRING_TOKEN (STR_TOTAL_CPU_NUM),
    mTotalCpuNumString,
    NULL
    );


  //
  // Create a item for the driver in CPU info page
  //
  for (Index = 0; Index<mNumberofCpus; Index++) {
    //
    // CPU0 ~ CPUn
    //
    UpdateTokenValue(mCpuSerialName+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
    //
    // CPU ID
    //
    UpdateTokenValue(mCpuIdString+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
    //
    // Stepping
    //
    UpdateTokenValue(mCpuSteppingString+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
    //
    // Microcode Revision
    //
    UpdateTokenValue(mCpuMicroCodeRevString+Index*STRING_WIDTH, Lang, StartOpCodeHandle,  Private);
    //
    // Platform ID
    //
    UpdateTokenValue(mPlatformIdString+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
    //
    // CPU Core Freq
    //
    UpdateTokenValue(mCpuCoreFreqString+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
    //
    // Actual CPU Freq
    //
    UpdateTokenValue(mCpuActualFreqString+Index*STRING_WIDTH, Lang, StartOpCodeHandle, Private);
  }
  //
  // Update CPU info page form
  //
  HiiUpdateForm (
    Private->RegisteredHandle,  // HII handle
    &mFormSetGuid,              // Formset GUID
    0x1234,                     // Form ID
    StartOpCodeHandle,          // Label for where to insert opcodes
    EndOpCodeHandle             // Replace data
    );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  if (Lang != NULL) {
    FreePool (Lang);
  }
  return EFI_SUCCESS;
}
