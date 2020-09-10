/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module, such as
  SMRR, EMRR, IED.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include "SmmCpuFeatures.h"
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/RasRegisterClearLib.h>

//
// The CPUID mapping for SkyLake
//
CPUID_MAPPING  mSkylakeMap[] = {
  {(UINT32)(CPU_FAMILY_SKX << 4), CPUID_MASK_NO_STEPPING},    // Skylake Server
  };

//
// The CLASS for SkyLake
//
CPU_SMM_CLASS mSkylakeClass = {
  CpuSkylake,
  sizeof(mSkylakeMap)/sizeof(mSkylakeMap[0]),
  mSkylakeMap,
  };

//
// The CPUID mapping for IceLake
//
CPUID_MAPPING  mIcelakeMap[] = {
  {(UINT32)(CPU_FAMILY_ICX << 4),  CPUID_MASK_NO_STEPPING },    // Icelake Server
  {(UINT32)(CPU_FAMILY_ICXD << 4), CPUID_MASK_NO_STEPPING },    // IcelakeG Server
};

CPU_SMM_CLASS mIcelakeClass = {
  CpuIcelake,
  sizeof(mIcelakeMap) / sizeof(mIcelakeMap[0]),
  mIcelakeMap,
};

//
//
// The CPUID mapping for Snow Ridge
//
CPUID_MAPPING  mSnowRidgeMap[] = {
  {(UINT32)(CPU_FAMILY_SNR << 4), CPUID_MASK_NO_STEPPING},    // SNOWRIDGE server
  };

//
// The CLASS for Snow Ridge
//
CPU_SMM_CLASS mSnowridgeClass = {
  CpuSnowridge,
  sizeof(mSnowRidgeMap)/sizeof(mSnowRidgeMap[0]),
  mSnowRidgeMap,
  };

//
// The CPUID mapping for SPR
//
CPUID_MAPPING  mSprMap[] = {
  {(UINT32)(CPU_FAMILY_SPRSP << 4), CPUID_MASK_NO_STEPPING },    // SPR Server
};

CPU_SMM_CLASS mSprClass = {
  CpuSpr,
  sizeof(mSprMap) / sizeof(mSprMap[0]),
  mSprMap,
};

//
// This table defines supported CPU class
//
CPU_SMM_CLASS *mCpuClasstable[] = {
  &mSkylakeClass,
  &mIcelakeClass,
  &mSnowridgeClass,
  &mSprClass
  };

//
// Below section is common definition
//

//
// Assumes UP, or MP with identical feature set
//
CPU_SMM_FEATURE_CONTEXT  mFeatureContext;
CPU_SMM_CLASS            *mThisCpu;
BOOLEAN                  mSmmMsrSaveStateEnable = FALSE;
BOOLEAN                  mSaveStateInMsr = FALSE;
BOOLEAN                  mSmmInitSaveStateInMsr;
UINT32                   mSmrr2Base;
UINT32                   mSmrr2Size;
UINT32                   mSmmBaseNotCoveredBySmrr = 0;
UINT32                   mSmmSizeNotCoveredBySmrr = 0;
UINT8                    mSmrr2CacheType;
BOOLEAN                  mSmmUseDelayIndication;
BOOLEAN                  mSmmUseBlockIndication;
SPIN_LOCK                *mCsrAccessLock = NULL;
BOOLEAN                  mIEDEnabled;
BOOLEAN                  mIEDBaseInitialized = FALSE;
UINT32                   mIEDRamSize;
UINT32                   mIEDRamBase;
UINT8                    *mNonIioBusNum;
EFI_PROCESSOR_INFORMATION *mSmmProcessorInfo;
UINT64                   *mTstateMsr;
UINT32                   *mSmBase;
SMM_CPU_SYNC_FEATURE     *mSmmSyncFeature;
BOOLEAN                  mSmmRuntimeCtlHooksEnable;

//
// Protected Mode Entrypoint
//
BOOLEAN                  mSmmProtectedModeEnable;
SMM_PROT_MODE_CONTEXT    *mSmmProtModeContext;

//
// Set default value to assume MSR_SMM_FEATURE_CONTROL is not supported
//
BOOLEAN                  mSmmFeatureControlSupported = FALSE;
BOOLEAN                  mSmmSupovrStateLock = FALSE;

EFI_SMRAM_DESCRIPTOR     *mSmmFeatureSmramRanges;
UINTN                    mSmmFeatureSmramRangeCount;

extern IA32_DESCRIPTOR  gSmmFeatureSmiHandlerIdtr;

//
// Variables from Protected Mode SMI Entry Template
//
extern volatile UINT32                     gSmmFeatureSmiStack;
extern UINT32                              gSmmFeatureSmbase;
extern UINT32                              gSmmFeatureSmiCr3;
extern BOOLEAN                             gSmmFeatureXdSupported;
extern BOOLEAN                             gSmmFeatureEnable5LevelPaging;
extern UINT32                              gProtModeSmbase;
extern volatile UINT8                      gcSmmFeatureSmiHandlerTemplate[];
extern CONST UINT16                        gcSmmFeatureSmiHandlerSize;
extern volatile UINT8                      gcSmiPMHandlerTemplate[];
extern CONST UINT16                        gcSmiPMHandlerSize;
extern UINT32                              gProtModeIdtr;
extern UINT32                              gPMStackDesc[2];


/**
  Check if it is Skylake server processor.

  @retval TRUE  It is Skylake server
  @retval FALSE It is not Skylake server.
**/
BOOLEAN
IsSkylakeServer (
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if ((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_SKX << 4))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if it is Icelake server processor.

  @retval TRUE  It is Icelake server
  @retval FALSE It is not Icelake server.
**/
BOOLEAN
IsIcelakeServer (
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if (((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_ICX << 4))) || ((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_ICXD << 4)))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if it is Icelake-SP R stepping server processor.

  @retval TRUE  It is Icelake-SP R stepping server
  @retval FALSE It is not Icelake-SP R stepping server.
**/
BOOLEAN
IsIcelakeSpRServer (
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if (((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_ICX << 4))) &&
      (((CPUID_VERSION_INFO_EAX *) &RegEax)->Bits.SteppingId < ICXSP_L0_CPU_STEP)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if it is SPR server processor.

  @retval TRUE  It is SPR server
  @retval FALSE It is not SPR server.
**/
BOOLEAN
IsSprServer (
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if ((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_SPRSP << 4))) {
    return TRUE;
  }
  return FALSE;
}


/**
  Check if it is Snowridge server processor.

  @retval TRUE  It is Snowridge server
  @retval FALSE It is not Snowridge server.
**/
BOOLEAN
IsSnowridgeServer(
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if (((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_SNR << 4)))
     ) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if it is Snowridge A stepping server processor.

  @retval TRUE  It is Snowridge A stepping server
  @retval FALSE It is not Snowridge A stepping server.
**/
BOOLEAN
IsSnowridgeAServer (
  VOID
  )
{
  UINT32 RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  if (((RegEax & CPUID_MASK_NO_STEPPING) == ((UINT32)(CPU_FAMILY_SNR << 4))) &&
      (((CPUID_VERSION_INFO_EAX *) &RegEax)->Bits.SteppingId < SNR_B0_CPU_STEP)) {
    return TRUE;
  }
  return FALSE;
}

/**
  This function will return current CPU_SMM_CLASS accroding to CPUID mapping.

  @return The point to current CPU_SMM_CLASS

**/
CPU_SMM_CLASS *
GetCpuFamily (
  VOID
  )
{
  UINT32         ClassIndex;
  UINT32         Index;
  UINT32         Count;
  CPUID_MAPPING  *CpuMapping;
  UINT32         RegEax;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  for (ClassIndex = 0; ClassIndex < sizeof(mCpuClasstable)/sizeof(mCpuClasstable[0]); ClassIndex++) {
    CpuMapping = mCpuClasstable[ClassIndex]->MappingTable;
    Count = mCpuClasstable[ClassIndex]->MappingCount;
    for (Index = 0; Index < Count; Index++) {
      if ((CpuMapping[Index].Signature & CpuMapping[Index].Mask) == (RegEax & CpuMapping[Index].Mask)) {
        return mCpuClasstable[ClassIndex];
      }
    }
  }
  return NULL;
}

/**
  Allocate pages for code.

  @param  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePages (
  IN UINTN           Pages
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory;

  if (Pages == 0) {
    return NULL;
  }

  Status = gSmst->SmmAllocatePages (AllocateAnyPages, EfiRuntimeServicesCode, Pages, &Memory);
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  return (VOID *) (UINTN) Memory;
}

/**
  Find out SMRAM range information from SMM ACCESS2 Protocol.

**/
VOID
FindSmramRange (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             Size;
  EFI_SMM_ACCESS2_PROTOCOL          *SmmAccess;

  //
  // Get SMM Access Protocol
  //
  Status = gBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **)&SmmAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Get SMRAM information
  //
  Size = 0;
  Status = SmmAccess->GetCapabilities (SmmAccess, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  mSmmFeatureSmramRanges = (EFI_SMRAM_DESCRIPTOR *)AllocatePool (Size);
  ASSERT (mSmmFeatureSmramRanges != NULL);

  Status = SmmAccess->GetCapabilities (SmmAccess, &Size, mSmmFeatureSmramRanges);
  ASSERT_EFI_ERROR (Status);

  mSmmFeatureSmramRangeCount = Size / sizeof (EFI_SMRAM_DESCRIPTOR);
}

/**
  The routine returns TRUE when CPU supports it (CPUID[7,0].ECX.BIT[16] is set) and
  the max physical address bits is bigger than 48. Because 4-level paging can support
  to address physical address up to 2^48 - 1, there is no need to enable 5-level paging
  with max physical address bits <= 48.

  @retval TRUE  5-level paging enabling is needed.
  @retval FALSE 5-level paging enabling is not needed.
**/
static
BOOLEAN
Is5LevelPagingNeeded (
  VOID
  )
{
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX              VirPhyAddressSize;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_ECX ExtFeatureEcx;
  UINT32                                      MaxExtendedFunctionId;
  VOID                                        *Hob;

  Hob = GetFirstHob (EFI_HOB_TYPE_CPU);
  if (Hob != NULL) {
    VirPhyAddressSize.Bits.PhysicalAddressBits = ((EFI_HOB_CPU *) Hob)->SizeOfMemorySpace;
  } else {
    AsmCpuid (CPUID_EXTENDED_FUNCTION, &MaxExtendedFunctionId, NULL, NULL, NULL);
    if (MaxExtendedFunctionId >= CPUID_VIR_PHY_ADDRESS_SIZE) {
      AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &VirPhyAddressSize.Uint32, NULL, NULL, NULL);
    } else {
      VirPhyAddressSize.Bits.PhysicalAddressBits = 36;
    }
  }

  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
    NULL, NULL, &ExtFeatureEcx.Uint32, NULL
    );
  DEBUG ((
    DEBUG_INFO, "SmmCpuFeatureLib: PhysicalAddressBits = %d, 5LPageTable = %d.\n",
    VirPhyAddressSize.Bits.PhysicalAddressBits, ExtFeatureEcx.Bits.FiveLevelPage
    ));

  //
  // 4-level paging can support to address physical address up to 2^48 - 1.
  // 5-level paging is only needed when physical address size > 48.
  //
  if (VirPhyAddressSize.Bits.PhysicalAddressBits > 48) {
    ASSERT (ExtFeatureEcx.Bits.FiveLevelPage == 1);
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  The constructor function

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmCpuFeaturesLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT32  RegEax;
  UINT32  RegEdx;
  UINTN   FamilyId;
  UINTN   ModelId;
  UINTN   SpinLockSize;
  VOID    *SpinLockBuffer;

  //
  // Retrieve CPU Family and Model
  //
  AsmCpuid(CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  DEBUG ((EFI_D_INFO, "CPUID - 0x%08x\n", RegEax));
  FamilyId = (RegEax >> 8) & 0xf;
  ModelId  = (RegEax >> 4) & 0xf;
  if (FamilyId == 0x06 || FamilyId == 0x0f) {
    ModelId = ModelId | ((RegEax >> 12) & 0xf0);
  }
  DEBUG ((EFI_D_INFO, "FamilyId - 0x%08x\n", FamilyId));
  DEBUG ((EFI_D_INFO, "ModelId - 0x%08x\n", ModelId));

  RegEdx = 0;
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &RegEax, NULL, NULL, NULL);
  if (RegEax >= CPUID_EXTENDED_CPU_SIG) {
    AsmCpuid (CPUID_EXTENDED_CPU_SIG, NULL, NULL, NULL, &RegEdx);
  }
  //
  // Determine the mode of the CPU at the time an SMI occurs
  //   Intel(R) 64 and IA-32 Architectures Software Developer's Manual
  //   Volume 3C, Section 34.4.1.1
  //
  mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT;
  if ((RegEdx & BIT29) != 0) {
    mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_64BIT;
  }
  if (FamilyId == 0x06) {
    if (ModelId == 0x17 || ModelId == 0x0f || ModelId == 0x1c) {
      mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_64BIT;
    }
  }

  gSmmFeatureXdSupported = FALSE;
  if (((CPUID_EXTENDED_CPU_SIG_EDX*)&RegEdx)->Bits.NX != 0) {
    gSmmFeatureXdSupported = TRUE;
  }

  //
  // Enable 5-level paging in SMM env if CPU supports and max physical address bits > 48.
  //
  gSmmFeatureEnable5LevelPaging = Is5LevelPagingNeeded ();

  //
  // Allocate page for SPIN LOCK buffer
  //
  SpinLockSize            = GetSpinLockProperties ();
  SpinLockBuffer          = AllocatePages (EFI_SIZE_TO_PAGES (SpinLockSize * 2));

  if (SpinLockBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "AllocatePages Returned NULL pointer\n"));
    ASSERT (SpinLockBuffer != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  mMsrSpinLock            = (SPIN_LOCK *)SpinLockBuffer;
  mCsrAccessLock          = (SPIN_LOCK *)((UINTN)SpinLockBuffer + SpinLockSize);

  //
  // Initialize spin lock for MSR access
  //
  InitializeSpinLock (mMsrSpinLock);

  //
  // Use CPUID to determine CPU Family
  //
  mThisCpu = GetCpuFamily ();
  ASSERT (mThisCpu != NULL);

  //
  // Initialize spin lock for CSR access
  //
  InitializeSpinLock(mCsrAccessLock);

  mNonIioBusNum = (UINT8 *)AllocatePool(sizeof(UINT8) * PcdGet32(PcdCpuMaxLogicalProcessorNumber));
  ASSERT(mNonIioBusNum != NULL);

  mTstateMsr = (UINT64 *)AllocatePool(sizeof(UINT64) * PcdGet32(PcdCpuMaxLogicalProcessorNumber));
  ASSERT(mTstateMsr != NULL);

  mSmBase = (UINT32 *)AllocatePool(sizeof(UINT32) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  ASSERT(mSmBase != NULL);

  mSmmSyncFeature = (SMM_CPU_SYNC_FEATURE *)AllocatePool (sizeof (SMM_CPU_SYNC_FEATURE) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  ASSERT (mSmmSyncFeature != NULL);

  //
  // Related PCD values are retrieved into global variables.
  //
  mSmmMsrSaveStateEnable    = PcdGetBool (PcdCpuSmmMsrSaveStateEnable);
  mSmmProtectedModeEnable   = PcdGetBool (PcdCpuSmmProtectedModeEnable);
  mSmmUseDelayIndication    = PcdGetBool (PcdCpuSmmUseDelayIndication);
  mSmmUseBlockIndication    = PcdGetBool (PcdCpuSmmUseBlockIndication);
  mSmmRuntimeCtlHooksEnable = PcdGetBool (PcdCpuSmmRuntimeCtlHooks);

  //
  // Dump some SMM feature PCD values
  //
  DEBUG ((EFI_D_INFO, "PcdCpuSmmMsrSaveStateEnable     = %d\n", mSmmMsrSaveStateEnable));
  DEBUG ((EFI_D_INFO, "PcdCpuSmmProtectedModeEnable    = %d\n", mSmmProtectedModeEnable));
  DEBUG ((EFI_D_INFO, "PcdCpuSmmUseDelayIndication     = %d\n", mSmmUseDelayIndication));
  DEBUG ((EFI_D_INFO, "PcdCpuSmmUseBlockIndication     = %d\n", mSmmUseBlockIndication));
  DEBUG ((EFI_D_INFO, "PcdCpuSmmRuntimeCtlHooks        = %d\n", mSmmRuntimeCtlHooksEnable));


  if (mSmmProtectedModeEnable) {
    mSmmProtModeContext = SmmFeatureAllocateCodePages (EFI_SIZE_TO_PAGES(sizeof(SMM_PROT_MODE_CONTEXT) * PcdGet32(PcdCpuMaxLogicalProcessorNumber)));
    ASSERT(mSmmProtModeContext != NULL);
  }

  mSmrr2Base      = PcdGet32(PcdCpuSmmSmrr2Base);
  mSmrr2Size      = PcdGet32(PcdCpuSmmSmrr2Size);
  mSmrr2CacheType = PcdGet8 (PcdCpuSmmSmrr2CacheType);
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2Base      = %x\n", mSmrr2Base));
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2Size      = %x\n", mSmrr2Size));
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2CacheType = %d\n", mSmrr2CacheType));
  ASSERT((mSmrr2CacheType == MTRR_CACHE_WRITE_PROTECTED) || (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK));
  if (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK) {
    //
    // PcdCpuSmmSmrr2Base and PcdCpuSmmSmrr2Size should be zero if cache type is WB
    //
    ASSERT (mSmrr2Base == 0);
    ASSERT (mSmrr2Size == 0);
  }

  //
  // Cache IED configuration settings into module global variables.
  //
  mIEDEnabled = PcdGetBool (PcdCpuIEDEnabled);
  mIEDRamBase = PcdGet32 (PcdCpuIEDRamBase);
  mIEDRamSize = PcdGet32 (PcdCpuIEDRamSize);
  DEBUG((DEBUG_INFO, "PcdCpuIEDEnabled: %x\n", mIEDEnabled));
  DEBUG((DEBUG_INFO, "PcdCpuIEDRamBase: %x\n", mIEDRamBase));
  DEBUG((DEBUG_INFO, "PcdCpuIEDRamSize: %x\n", mIEDRamSize));

  FindSmramRange ();

  return EFI_SUCCESS;
}

/**
  Verifiy if SMRR base/size meet SMRR programming requirement.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.

  @retval TRUE       SMRR base/size is valid.
  @retval TRUE       SMRR base/size is not valid.
**/
BOOLEAN
VerifySmrrBaseSize (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize
  )
{
  //
  // SMRR size cannot be less than 4-KBytes
  // SMRR size must be of length 2^n
  // SMRR base alignment cannot be less than SMRR length
  //
  if ((SmrrSize < SIZE_4KB) ||
      (SmrrSize != GetPowerOfTwo32 (SmrrSize)) ||
      ((SmrrBase & ~(SmrrSize - 1)) != SmrrBase)) {
    return FALSE;
  }

  return TRUE;
}
/**
  Return if SMRR is supported

  @retval TRUE  SMRR is supported.
  @retval FALSE SMRR is not supported.

**/
BOOLEAN
IsSmrrSupported (
  VOID
  )
{
  MSR_IA32_MTRRCAP_REGISTER    MtrrCap;
  BOOLEAN                      ReturnValue;

  MtrrCap.Uint32 = (UINT32) AsmReadMsr64 (MSR_IA32_MTRRCAP);
  if (MtrrCap.Bits.SMRR == 0) {
    ReturnValue = FALSE;
  } else {
    ReturnValue = TRUE;
  }

  return ReturnValue;
}

/**
  Return if SMRR2 is supported

  @retval TRUE  SMRR2 is supported.
  @retval FALSE SMRR2 is not supported.

**/
BOOLEAN
IsSmrr2Supported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER    MtrrCap;
  BOOLEAN                 ReturnValue;

  MtrrCap.Uint32 = (UINT32) AsmReadMsr64 (MSR_IA32_MTRRCAP);
  if (MtrrCap.Bits.Smrr2 == 0) {
    ReturnValue = FALSE;
  } else {
    ReturnValue = TRUE;
  }

  return ReturnValue;
}

/**
  Return if EMRR is supported

  @retval TRUE  EMRR is supported.
  @retval FALSE EMRR is not supported.

**/
BOOLEAN
IsEmrrSupported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER    MtrrCap;
  BOOLEAN                 ReturnValue;


  MtrrCap.Uint32 = (UINT32) AsmReadMsr64(MSR_IA32_MTRRCAP);
  if (MtrrCap.Bits.Prmrr == 0) {
    ReturnValue = FALSE;
  } else {
    ReturnValue = TRUE;
  }

  return ReturnValue;
}

/**
  Initialize SMRR in SMM relocate.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
**/
VOID
InitSmrr (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize,
  IN BOOLEAN               IsMonarch
  )
{
  UINT32                   SmrrMask;
  UINT32                   Smrr2Base;
  UINT32                   Smrr2Size;
  UINT32                   FinalSmrrSize;
  UINT32                   Alignment;

  FinalSmrrSize = SmrrSize;
  if (!VerifySmrrBaseSize (SmrrBase, FinalSmrrSize)) {
    //
    // If SMRR base/size does not meet requirment, check if we could enable SMRR2
    //
    if (IsSmrr2Supported ()) {
      Smrr2Base = 0;
      Smrr2Size = 0;
      //
      // mSmrr2CacheType should be WB
      //
      ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK);
      //
      // Calculate the alignment of the SMM base address.
      //
      Alignment = (UINT32)LShiftU64 (1, LowBitSet32 (SmrrBase));
      if (Alignment >= SmrrSize) {
        Smrr2Size     = SmrrSize - GetPowerOfTwo32 (SmrrSize);
        FinalSmrrSize = SmrrSize - Smrr2Size;
      } else {
        Smrr2Size     = SmrrSize - Alignment;
        FinalSmrrSize = Alignment;
      }
      Smrr2Base = SmrrBase + FinalSmrrSize;
      if (IsMonarch) {
        DEBUG ((DEBUG_INFO, "SmrrBase  = %x, FinalSmrrSize = %x\n", SmrrBase, FinalSmrrSize));
        DEBUG ((DEBUG_INFO, "Smrr2Base = %x, Smrr2Size     = %x\n", Smrr2Base, Smrr2Size));
      }
      mSmmSizeNotCoveredBySmrr = Smrr2Size;
      mSmmBaseNotCoveredBySmrr = Smrr2Base;
    }
    if (FinalSmrrSize != SmrrSize) {
      if (!VerifySmrrBaseSize (SmrrBase, FinalSmrrSize)) {
        //
        // Verify SMRR base/size again if SMRR size is modified
        //
        if (IsMonarch) {
          //
          // Print message and halt if CPU is Monarch
          //
          DEBUG ((DEBUG_ERROR, "SMM Base/Size does not meet alignment/size requirement!\n"));
        }
        CpuDeadLoop ();
      }
    }
  }

  if ( IsSnowridgeServer() ) {
    SmrrMask = (UINT32) (-1) - ((UINT32) (1 << SNR_ADDR_SHIFT_BIT) - 1);
  } else {
    SmrrMask = (UINT32) (-1) - ((UINT32) (1 << ICX_ADDR_SHIFT_BIT) - 1);
  }

  AsmWriteMsr64 (MSR_SMRR_BASE, SmrrBase | CACHE_WRITE_BACK);
  AsmWriteMsr64 (MSR_SMRR_MASK, (~(FinalSmrrSize - 1) & SmrrMask)); // Valid bit will be set in ConfigSmrr() at first SMI
}

/**
  Initialize CHA SMM Reg in SMM relocate.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  CpuIndex           CPU index.
  @param  IsMonarch          TRUE if the current CPU is Monarch.
                             FALSE if the current CPU is not Monarch.
**/
VOID
InitChaSmmReg (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize,
  IN UINTN                 CpuIndex,
  IN BOOLEAN               IsMonarch
  )
{
  UINT64 SmmRegChaSadSmmBaseCsr;
  UINT64 SmmRegChaSadSmmLimitCsr;

  if (IsMonarch) {
    DEBUG ((DEBUG_INFO, "ChaSmmRegBase  = %x, ChaSmmRegLimit = %x\n", SmrrBase, (SmrrBase + SmrrSize)));
  }

  SmmRegChaSadSmmBaseCsr  = SmmReadReg64 (CpuIndex, SmmRegChaSadSmmBase);
  SmmRegChaSadSmmLimitCsr = SmmReadReg64 (CpuIndex, SmmRegChaSadSmmLimit);

  //
  // Only set the CHA SMM Reg if the new value is different than the current value
  //
  if (SmmRegChaSadSmmBaseCsr != SmrrBase) {
    SmmWriteReg64 (CpuIndex, SmmRegChaSadSmmBase, SmrrBase);
  }
  if (SmmRegChaSadSmmLimitCsr != ((SmrrBase + SmrrSize) | CHA_SMM_REG_EN)) {
    SmmWriteReg64 (CpuIndex, SmmRegChaSadSmmLimit, ((SmrrBase + SmrrSize) | CHA_SMM_REG_EN));
  }
}

/**
  Caculate SMRR2 base/size.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  IsMonarch          TRUE if the current CPU is Monarch.
                             FALSE if the current CPU is not Monarch.
**/
VOID
CalculateSmrr2 (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize,
  IN BOOLEAN               IsMonarch
  )
{
  UINTN                 Index;
  BOOLEAN               Found;
  UINT32                Smrr2Base;
  UINT32                Smrr2Size;
  UINT32                CpuStart;
  UINT32                PhysicalSize;

  Smrr2Base = 0;
  Smrr2Size = 0;
  do {
    Found = FALSE;
    for (Index = 0; Index < mSmmFeatureSmramRangeCount; Index++) {
      CpuStart     = (UINT32) mSmmFeatureSmramRanges[Index].CpuStart;
      PhysicalSize = (UINT32) mSmmFeatureSmramRanges[Index].PhysicalSize;
      if ((CpuStart < SmrrBase) || (CpuStart >= SmrrBase + SmrrSize)) {
        //
        // If one SMM range is out of current SMRR range
        //
        if (Smrr2Size == 0) {
          //
          // Assign the first SMRR2 range
          //
          Smrr2Base = CpuStart;
          Smrr2Size = PhysicalSize;
          Found = TRUE;
        } else if ((Smrr2Base + Smrr2Size) == CpuStart) {
          //
          // Append new SMM range at end of current SMRR2 range
          //
          Smrr2Size += PhysicalSize;
          Found = TRUE;
        } else if (Smrr2Base == (CpuStart + PhysicalSize)) {
          //
          // Insert new SMM range in front of current SMRR2 range
          //
          Smrr2Base -= PhysicalSize;
          Smrr2Size += PhysicalSize;
          Found = TRUE;
        }
      }
    }
  } while (Found);

  if (IsMonarch) {
    DEBUG ((DEBUG_INFO, "SMRR2 Base: 0x%x, SMRR2 Size: 0x%x\n", Smrr2Base, Smrr2Size));
  }

  if (Smrr2Size != 0 && mSmmSizeNotCoveredBySmrr != 0) {
    //
    // We cannot support 2 SMRR2 range
    //
    if (IsMonarch) {
      DEBUG ((DEBUG_ERROR, "Can not support 2 SMRR2 ranges!\n"));
    }
    CpuDeadLoop ();
  }

  if (Smrr2Size == 0 && mSmmSizeNotCoveredBySmrr == 0) {
    //
    // If there is no SMRR2 requirement for WB memory, the SMRR2 range
    // mSmrr2Base and mSmrr2Size should be from PCDs in SmmCpuFeaturesLibConstructor()
    //
    ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_PROTECTED);
  } else {
    ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK);
    if (mSmmSizeNotCoveredBySmrr != 0) {
      mSmrr2Base = mSmmBaseNotCoveredBySmrr;
      mSmrr2Size = mSmmSizeNotCoveredBySmrr;
    } else {
      //
      // Smrr2Size != 0
      //
      mSmrr2Base = Smrr2Base;
      mSmrr2Size = Smrr2Size;
    }
  }
  if (mSmrr2Size != 0) {
    if (!VerifySmrrBaseSize (mSmrr2Base, mSmrr2Size)) {
      if (IsMonarch) {
        DEBUG ((DEBUG_ERROR, "SMRR2 range does not meet SMRR2 requirement!\n"));
      }
      CpuDeadLoop ();
    }
  }
}

/**
  Configure SMRR register at each SMM entry.
**/
VOID
ConfigSmrr (
  VOID
  )
{
  MSR_SMRR_MASK_REGISTER SmrrMask;

  SmrrMask.Uint64 = AsmReadMsr64 (MSR_SMRR_MASK);
  if (!SmrrMask.Bits.Vld) {
    SmrrMask.Bits.Vld = 1;
    AsmWriteMsr64 (MSR_SMRR_MASK, SmrrMask.Uint64);
  }
}

/**
  Programming IED.

  @param  ProcessorNumber       The processor number.
  @param  IedBase               The base address of IED. -1 when IED is disabled.
  @param  IsMonarch             If this processor treated as monarch.
  @param  SmmInitSaveStateInMsr TRUE indicates SMM save state in MSR is supported.
                                FALSE indicates SMM save state in MSR is not supported.
**/
VOID
RelocateIedBase (
  IN UINTN                 ProcessorNumber,
  IN UINT32                IedBase,
  IN BOOLEAN               IsMonarch
  )
{
  UINT32 StrSize;
  UINT32 *IedOffset;
  UINT32 ScratchPad;

  ScratchPad = 0;
  //
  // IED must be enabled for Haswell processor.
  // A minimum of 4MB IDERAM is required for Haswell processors.
  //
  ASSERT (mIEDEnabled);
  ASSERT (mIEDRamSize >= 0x400000);

  if (mSmmInitSaveStateInMsr) {
    AsmWriteMsr64 (MSR_SMRAM_IEDBASE, LShiftU64 (IedBase, 32));
  } else {
    IedOffset = (UINT32 *)(UINTN)(SMM_DEFAULT_SMBASE + SMM_IEDBASE_OFFSET);
    *IedOffset = IedBase;
  }

  if ((IedBase != (UINT32)-1) && IsMonarch) {

    // Allocate 48 byte header
    //
    ZeroMem ((UINT8 *)(UINTN)IedBase, 48);

    // Copy signature to start of IED
    //
    StrSize = sizeof (IED_STRING) - 1;
    CopyMem ((UINT8 *)(UINTN)IedBase, IED_STRING, StrSize);

    // Write IED size to IED header
    //
    *(UINT64 *)(UINTN)(IedBase + 10) = (UINT64)mIEDRamSize;

    // Set IED trace
    //
    // Get base of IedTrace from BIOSNONSTICKYSCRATCHPAD3
    //
    if (IsSkylakeServer ()) {
      ScratchPad = PciRead32 (SKYLAKE_NON_STICKY_SCRATCHPAD3 (mNonIioBusNum[ProcessorNumber]));
    } else if ((IsIcelakeServer ()) || (IsSnowridgeServer()) || IsSprServer ()) {
      ScratchPad = PciRead32 (TEN_NM_NON_STICKY_SCRATCHPAD3 (mNonIioBusNum[ProcessorNumber]));
    }

    if (ScratchPad == 0) {

      // If scratchpad zero, just write FF's as trace ptr
      //
      *(UINT64 *)((UINTN)IedBase + 16) = (UINT64)-1;
    } else {

      // IED Trace Ptr = ScrtachPad * 1MB
      //
      *(UINT64 *)((UINTN)IedBase + 16) = LShiftU64 (ScratchPad, 20);
    }

    // Clear rest of IED
    //
    ZeroMem ((UINT8 *)(UINTN)IedBase + 0x100000, 32 * 0x1024);
  }
}

/**
  Initialize EMRR in SMM relocation for Haswell processors.

  @param  IedBase           IEDRAM base address.

**/
VOID
InitEmrr (
  IN UINT32          IedBase
  )
{
  UINT32                ApicId;
  UINT32                IntraPackageIdBits;
  UINT64                McSegBase;
  UINT64                McSegMask;
  UINT8                 PhysicalAddressBits;
  UINT64                ValidMtrrAddressMask;
  UINT32                RegEax;
  UINT32                IcxMsrPrmrrIndex;
  SKX_MSR_PRMRR_BASE_REGISTER             PrmrrBase;
  SKX_MSR_PRMRR_MASK_REGISTER             PrmrrMask;
  SNR_MSR_PMH_CR_PRMRR_BASE_REGISTER      SnrPrmrrBase;
  SNR_MSR_PMH_CR_PRMRR_MASK_REGISTER      SnrPrmrrMask;

  PrmrrBase.Uint64 = 0;
  PrmrrMask.Uint64 = 0;
  SnrPrmrrBase.Uint64 = 0;
  SnrPrmrrMask.Uint64 = 0;
  IcxMsrPrmrrIndex = ICX_MSR_PRMRR_BASE_0;

  ApicId = GetInitialApicId ();

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &IntraPackageIdBits, NULL, NULL, NULL);
  IntraPackageIdBits &= 0x1f;

  AsmCpuid (CPUID_EXTENDED_FUNCTION, &RegEax, NULL, NULL, NULL);
  PhysicalAddressBits = 36;
  if (RegEax >= CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  }
  ValidMtrrAddressMask = (LShiftU64 (1, PhysicalAddressBits) - 1) & 0xfffffffffffff000ull;

  if (IsSnowridgeServer()) {
    //
    // IEDBASE + 2MB for socket 0
    //
    McSegBase = IedBase + 0x200000;
  } else {
    //
    // IEDBASE + (2MB + (1MB * socket #))
    //
    McSegBase = IedBase + 0x200000 + (0x100000 * (ApicId >> IntraPackageIdBits));
  }

  McSegBase &= ValidMtrrAddressMask;
  McSegMask = (UINT64)(~(UINT64)(0x200000 - 1));
  McSegMask &= ValidMtrrAddressMask;

  if (IsSnowridgeServer ()) {
    SnrPrmrrMask.Uint64 = AsmReadMsr64 (SNR_MSR_PMH_CR_PRMRR_MASK);
    if (SnrPrmrrMask.Bits.L == 0) {
      SnrPrmrrBase.Bits.Memtype = CACHE_WRITE_BACK;
      SnrPrmrrBase.Bits.Base = (UINT32) (McSegBase >> SNR_ADDR_SHIFT_BIT);
      AsmWriteMsr64 (SNR_MSR_PMH_CR_PRMRR_BASE, SnrPrmrrBase.Uint64);

      SnrPrmrrMask.Bits.Mask = (UINT32) (McSegMask >> SNR_ADDR_SHIFT_BIT);
      SnrPrmrrMask.Bits.L = 1;
      AsmWriteMsr64 (SNR_MSR_PMH_CR_PRMRR_MASK, SnrPrmrrMask.Uint64);
    }
  } else if (!IsIcelakeServer () && !IsSprServer ()){
    PrmrrMask.Uint64 = AsmReadMsr64 (SKX_MSR_PRMRR_MASK);
    if (PrmrrMask.Bits.L == 0) {
      PrmrrBase.Bits.Memtype = CACHE_WRITE_BACK;
      PrmrrBase.Bits.Base0 = (UINT32) (McSegBase >> ICX_ADDR_SHIFT_BIT);
      PrmrrBase.Bits.Base1 = (UINT32) ((McSegBase >> ICX_ADDR_SHIFT_BIT) >> 20);
      AsmWriteMsr64 (SKX_MSR_PRMRR_BASE, PrmrrBase.Uint64);

      PrmrrMask.Bits.Mask0 = (UINT32) (McSegMask >> ICX_ADDR_SHIFT_BIT);
      PrmrrMask.Bits.Mask1 = (UINT32) ((McSegMask >> ICX_ADDR_SHIFT_BIT) >> 20);
      PrmrrMask.Bits.L = 1;
      AsmWriteMsr64 (SKX_MSR_PRMRR_MASK, PrmrrMask.Uint64);
    }
  }
}

/**
  Config MSR Save State feature for processors.

  @param[in]  ProcessorNumber        The processor number.

**/
VOID
ConfigMsrSaveState (
  IN UINTN        ProcessorNumber
  )
{
  UINT64                      SmmFeatureControlMsr;
  MSR_SMM_MCA_CAP_REGISTER    McaCap;

  //
  // Get the enhanced SMM capabilities MSR
  //
  McaCap.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP) ;

  //
  // Get the feature control MSR
  //
  SmmFeatureControlMsr = SmmReadReg64 (ProcessorNumber, SmmRegFeatureControl);

  //
  // If SMM code check is available on this SKU,
  //  set code check enable bit if not set already
  //
  if (McaCap.Bits.SmmCodeAccessChk != 0) {
    if ((SmmFeatureControlMsr & SMM_CODE_CHK_EN_BIT) != SMM_CODE_CHK_EN_BIT) {
      SmmFeatureControlMsr |= SMM_CODE_CHK_EN_BIT;
    }
  }

  //
  // If SMM save state control is available on this SKU
  //
  if (McaCap.Bits.SmmCpuSvrstr != 0) {
    SmmFeatureControlMsr = SmmReadReg64 (ProcessorNumber, SmmRegFeatureControl);

    //
    // SMM_CPU_SAVE_EN_BIT is package scope, so when a thead in a package changes this bit,
    //  MSR Save State feature will be changed for all threads in the package in subsequent SMIs.
    //
    // Lock bit is not set here. It is only set when PcdCpuSmmFeatureControlMsrLock = TRUE, which is never.
    //
    // If setting save state enable
    //
    if (mSmmMsrSaveStateEnable) {
      //
      // Setting save state enable
      // If it's not already set
      //
      if ((SmmFeatureControlMsr & SMM_CPU_SAVE_EN_BIT) == 0) {
        SmmFeatureControlMsr |= SMM_CPU_SAVE_EN_BIT;
        if ((SmmFeatureControlMsr & SMM_FEATURE_CONTROL_LOCK_BIT) == 0) {
          SmmWriteReg64 (ProcessorNumber, SmmRegFeatureControl, SmmFeatureControlMsr);
        }
        mSaveStateInMsr = TRUE;
      }
    } else {
      //
      // Clearing save state enable
      // If it's not already clear
      //
      if ((SmmFeatureControlMsr & SMM_CPU_SAVE_EN_BIT) == SMM_CPU_SAVE_EN_BIT) {
        SmmFeatureControlMsr &= (UINT64) ~SMM_CPU_SAVE_EN_BIT;
        if ((SmmFeatureControlMsr & SMM_FEATURE_CONTROL_LOCK_BIT) == 0) {
          SmmWriteReg64 (ProcessorNumber, SmmRegFeatureControl, SmmFeatureControlMsr);
        }
        mSaveStateInMsr = FALSE;
      }
    }
  }

  //
  // Set lock bit here if enabled
  //
  if (FeaturePcdGet (PcdCpuSmmFeatureControlMsrLock) == TRUE) {
    DEBUG ((EFI_D_INFO, "SMM Feature control locking deferred to \"SmmCpuFeaturesCompleteSmmReadyToLock\"\n"));

  }

  //
  // Write back the changed register
  //
  SmmWriteReg64 (ProcessorNumber, SmmRegFeatureControl, SmmFeatureControlMsr);
}

/**
  Initialize SMRR2 in SMM relocate.

  @param  Smrr2Base          The base address of SMRR2.
  @param  Smrr2Size          The size of SMRR2.
**/
VOID
InitSmrr2 (
  IN UINT32                Smrr2Base,
  IN UINT32                Smrr2Size
  )
{
  MSR_SMRR2_BASE_REGISTER Smrr2BaseMsr;
  MSR_SMRR2_MASK_REGISTER Smrr2MaskMsr;
  UINT32                  SmrrMask;

  if ( IsSnowridgeServer() ) {
    SmrrMask = (UINT32) (-1) - ((UINT32) (1 << SNR_ADDR_SHIFT_BIT) - 1);
  } else {
    SmrrMask = (UINT32) (-1) - ((UINT32) (1 << ICX_ADDR_SHIFT_BIT) - 1);
  }

  ASSERT (Smrr2Size != 0);

  Smrr2BaseMsr.Uint64 = 0;
  Smrr2MaskMsr.Uint64 = AsmReadMsr64 (MSR_SMRR2_MASK);

  if (!Smrr2MaskMsr.Bits.Vld) {
    //
    // Program SMRR2 Base and Mask
    //
    Smrr2BaseMsr.Bits.Memtype = mSmrr2CacheType;
    Smrr2BaseMsr.Bits.Base = Smrr2Base;
    AsmWriteMsr64 (MSR_SMRR2_BASE, Smrr2BaseMsr.Uint64);

    Smrr2MaskMsr.Bits.Vld = 1;
    Smrr2MaskMsr.Bits.Mask = (~(Smrr2Size - 1) & SmrrMask);
    AsmWriteMsr64 (MSR_SMRR2_MASK, Smrr2MaskMsr.Uint64);
  }

}

/**
  Enable the Save Floating Point Pointers feature on every logical processor.
  Refer to SNB/IVB BWG "SMM Handler Considerations".

**/
VOID
EnableSmmSaveControl (
  VOID
)
{
  ICX_MSR_SMM_SAVE_CONTROL_REGISTER   SaveControl;
  ICX_MSR_PLATFORM_INFO_REGISTER  PlatformInfoMsr;
  //
  // MSR_PLATFORM_INFO[16] = 1 indicates the Save Floating Point Pointers feature exists.
  //
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
  if (PlatformInfoMsr.Bits.SmmSaveCap != 0) {
    SaveControl.Uint32  = AsmReadMsr32 (ICX_MSR_SMM_SAVE_CONTROL);
    SaveControl.Bits.SaveFpState = 1;
    AsmWriteMsr32 (ICX_MSR_SMM_SAVE_CONTROL, SaveControl.Uint32);
  }
}

//
// Below section is external function
//

/**
  Read MSR or CSR based on the CPU type Register to read.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.

  @return 64-bit value read from register.

**/
UINT64
SmmReadReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName
  )
{
  UINT64      RetVal;

  RetVal = 0;
  if (IsSkylakeServer ()) {
    AcquireSpinLock (mCsrAccessLock);
    switch (RegName) {
    //
    // Server uses CSR
    //
    case SmmRegFeatureControl:
      RetVal = PciRead32 (SKYLAKE_SMM_FEATURE_CONTROL_REG (mNonIioBusNum[CpuIndex]));
      break;
    case SmmRegSmmDelayed:
      RetVal = PciRead32 (SKYLAKE_SMM_DELAYED1_REG (mNonIioBusNum[CpuIndex]));
      RetVal = LShiftU64 (RetVal, 32) | PciRead32 (SKYLAKE_SMM_DELAYED0_REG (mNonIioBusNum[CpuIndex]));
      break;
    case SmmRegSmmBlocked:
      RetVal = PciRead32 (SKYLAKE_SMM_BLOCKED1_REG (mNonIioBusNum[CpuIndex]));
      RetVal = LShiftU64 (RetVal, 32) | PciRead32 (SKYLAKE_SMM_BLOCKED0_REG (mNonIioBusNum[CpuIndex]));
      break;
    default:
      ASSERT (FALSE);
    }
    ReleaseSpinLock (mCsrAccessLock);
  } else if ((IsIcelakeServer ()) || (IsSnowridgeServer()) || IsSprServer ()) {
    AcquireSpinLock (mCsrAccessLock);
    switch (RegName) {
    //
    // Server uses CSR
    //
    case SmmRegFeatureControl:
      RetVal = PciRead32 (TEN_NM_SMM_FEATURE_CONTROL_REG (mNonIioBusNum[CpuIndex]));
      break;
    case SmmRegSmmDelayed:
      if (IsIcelakeSpRServer ()) {
        RetVal = PciRead32 (ICXSP_R_SMM_DELAYED_REG (mNonIioBusNum[CpuIndex]));
      } else if (IsSnowridgeAServer ()) {
        RetVal = PciRead32 (SNR_A_SMM_DELAYED_REG (mNonIioBusNum[CpuIndex]));
      } else {
        RetVal = PciRead32 (TEN_NM_SMM_DELAYED_REG (mNonIioBusNum[CpuIndex]));
      }
      break;
    case SmmRegSmmBlocked:
      if (IsIcelakeSpRServer ()) {
        RetVal = PciRead32 (ICXSP_R_SMM_BLOCKED_REG (mNonIioBusNum[CpuIndex]));
      } else if (IsSnowridgeAServer ()) {
        RetVal = PciRead32 (SNR_A_SMM_BLOCKED_REG (mNonIioBusNum[CpuIndex]));
      } else {
        RetVal = PciRead32 (TEN_NM_SMM_BLOCKED_REG (mNonIioBusNum[CpuIndex]));
      }
      break;
    case SmmRegChaSadSmmBase:
      RetVal = PciRead32 (TEN_NM_SMMSEG_BASE_N1_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1));
      RetVal = LShiftU64 (RetVal, 32) | PciRead32 (TEN_NM_SMMSEG_BASE_N0_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1));
      break;
    case SmmRegChaSadSmmLimit:
      RetVal = PciRead32 (TEN_NM_SMMSEG_LIMIT_N1_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1));
      RetVal = LShiftU64 (RetVal, 32) | PciRead32 (TEN_NM_SMMSEG_LIMIT_N0_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1));
      break;
    default:
      ASSERT (FALSE);
    }
    ReleaseSpinLock (mCsrAccessLock);
  } else {
    switch (RegName) {
    //
    // Client uses MSR
    //
    case  SmmRegFeatureControl:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_FEATURE_CONTROL);
      break;
    case  SmmRegSmmDelayed:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_DELAYED);
      break;
    case  SmmRegSmmBlocked:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_BLOCKED);
      break;
    default:
      ASSERT (FALSE);
    }
  }
  return  RetVal;
}

/**
  Write MSR or CSR based on the CPU type Register to write.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  @param[in]  RegValue  64-bit Register value.

**/
VOID
SmmWriteReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName,
  IN  UINT64          RegValue
  )
{
  if (IsSkylakeServer ()) {
    AcquireSpinLock (mCsrAccessLock);
    switch (RegName) {
    //
    // Server uses CSR
    //
    case SmmRegFeatureControl:
      PciWrite32 (SKYLAKE_SMM_FEATURE_CONTROL_REG (mNonIioBusNum[CpuIndex]), (UINT32)RegValue);
      break;
    default:
      ASSERT (FALSE);
    }
    ReleaseSpinLock (mCsrAccessLock);
  } else if ((IsIcelakeServer ()) || (IsSnowridgeServer()) || IsSprServer ()) {
    AcquireSpinLock (mCsrAccessLock);
    switch (RegName) {
    //
    // Server uses CSR
    //
    case SmmRegFeatureControl:
      PciWrite32 (TEN_NM_SMM_FEATURE_CONTROL_REG (mNonIioBusNum[CpuIndex]), (UINT32)RegValue);
      break;
    case SmmRegChaSadSmmBase:
      PciWrite32 (TEN_NM_SMMSEG_BASE_N1_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1), (UINT32) RShiftU64 (RegValue, 32));
      PciWrite32 (TEN_NM_SMMSEG_BASE_N0_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1), (UINT32)RegValue);
      break;
    case SmmRegChaSadSmmLimit:
      PciWrite32 (TEN_NM_SMMSEG_LIMIT_N1_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1), (UINT32) RShiftU64 (RegValue, 32));
      PciWrite32 (TEN_NM_SMMSEG_LIMIT_N0_CHABC_SAD_REG (mNonIioBusNum[CpuIndex] + 1), (UINT32)RegValue);
      break;
    default:
      ASSERT (FALSE);
    }
    ReleaseSpinLock (mCsrAccessLock);
  } else {
    switch (RegName) {
    //
    // Client uses MSR
    //
    case  SmmRegFeatureControl:
      AsmWriteMsr64 (EFI_MSR_HASWELL_SMM_FEATURE_CONTROL, RegValue);
      break;
    default:
      ASSERT (FALSE);
    }
  }
}

/**
  Disable SMRR register if SMRR is supported and SmmCpuFeaturesNeedConfigureMtrrs()
  returns TRUE.
**/
VOID
EFIAPI
SmmCpuFeaturesDisableSmrr (
  VOID
  )
{
  switch (mThisCpu->Family) {
  default:
    return;
  }
}

/**
  Enable SMRR register if SMRR is supported and SmmCpuFeaturesNeedConfigureMtrrs()
  returns TRUE.
**/
VOID
EFIAPI
SmmCpuFeaturesReenableSmrr (
  VOID
  )
{
  switch (mThisCpu->Family) {
  default:
    return;
  }
}

/**
  Determines if MTRR registers must be configured to set SMRAM cache-ability
  when executing in System Management Mode.

  @retval TRUE   MTRR registers must be configured to set SMRAM cache-ability.
  @retval FALSE  MTRR registers do not need to be configured to set SMRAM
                 cache-ability.
**/
BOOLEAN
EFIAPI
SmmCpuFeaturesNeedConfigureMtrrs (
  VOID
  )
{
  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    return FALSE;
  default:
    return TRUE;
  }
}

/**
  This function sets DR6 & DR7 according to SMM save state, before running SMM C code.
  They are useful when you want to enable hardware breakpoints in SMM without entry SMM mode.

  NOTE: It might not be appreciated in runtime since it might
        conflict with OS debugging facilities. Turn them off in RELEASE.

  @param    CpuIndex              CPU Index

**/
VOID
EFIAPI
SmmFeatureCpuSmmDebugEntry (
  IN UINTN  CpuIndex
  )
{
  SMRAM_SAVE_STATE_MAP *CpuSaveState;

  if (FeaturePcdGet (PcdCpuSmmDebug)) {
    if (mSaveStateInMsr) {
      AsmWriteDr6 ((UINTN)AsmReadMsr64 (MSR_SMRAM_DR6));
      AsmWriteDr7 ((UINTN)AsmReadMsr64 (MSR_SMRAM_DR7));
    } else {
      CpuSaveState = (SMRAM_SAVE_STATE_MAP *)((UINTN)(mSmBase[CpuIndex] + SMRAM_SAVE_STATE_MAP_OFFSET));
      if (mSmmFeatureSaveStateRegisterLma == EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT) {
        AsmWriteDr6 (CpuSaveState->x86._DR6);
        AsmWriteDr7 (CpuSaveState->x86._DR7);
      } else {
        AsmWriteDr6 ((UINTN)CpuSaveState->x64._DR6);
        AsmWriteDr7 ((UINTN)CpuSaveState->x64._DR7);
      }
    }
  }
}

/**
  This function restores DR6 & DR7 to SMM save state.

  NOTE: It might not be appreciated in runtime since it might
        conflict with OS debugging facilities. Turn them off in RELEASE.

  @param    CpuIndex              CPU Index

**/
VOID
EFIAPI
SmmFeatureCpuSmmDebugExit (
  IN UINTN  CpuIndex
  )
{
  SMRAM_SAVE_STATE_MAP *CpuSaveState;

  if (FeaturePcdGet (PcdCpuSmmDebug)) {
    if (mSaveStateInMsr) {
      // DR6/DR7 MSR is RO
    } else {
      CpuSaveState = (SMRAM_SAVE_STATE_MAP *)((UINTN)(mSmBase[CpuIndex] + SMRAM_SAVE_STATE_MAP_OFFSET));
      if (mSmmFeatureSaveStateRegisterLma == EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT) {
        CpuSaveState->x86._DR7 = (UINT32)AsmReadDr7 ();
        CpuSaveState->x86._DR6 = (UINT32)AsmReadDr6 ();
      } else {
        CpuSaveState->x64._DR7 = AsmReadDr7 ();
        CpuSaveState->x64._DR6 = AsmReadDr6 ();
      }
    }
  }
}

/**
  Processor specific hook point each time a CPU enters System Management Mode.

  @param[in] CpuIndex  The index of the CPU that has entered SMM.  The value
                       must be between 0 and the NumberOfCpus field in the
                       System Management System Table (SMST).
**/
VOID
EFIAPI
SmmCpuFeaturesRendezvousEntry (
  IN UINTN  CpuIndex
  )
{
  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    //
    // Configure SMRR/EMRR register at each SMM entry.
    //
    if (mFeatureContext.SmrrEnabled) {
      ConfigSmrr ();
    }
    if (mFeatureContext.Smrr2Enabled) {
      //
      // Note that SMRR2 registers can't be written in the first SMI because
      // MSR Save State feature is not enabled.
      //
      InitSmrr2 (mSmrr2Base, mSmrr2Size);
    }

    if (mSmmRuntimeCtlHooksEnable) {
      SmmRuntimeControlInSmmRendezvous (NULL);
    }

    return ;

  default:
    return ;
  }
}

/**
  Processor specific hook point at Smi RendezvousExit.

  @param[in] CpuIndex  The index of the CPU that is exiting SMM.  The value must
                       be between 0 and the NumberOfCpus field in the System
                       Management System Table (SMST).
**/
volatile static UINT32 mNumOfCpu = 0;

VOID
SmmRuntimeCtlExit (
  IN UINTN CpuIndex
  )
{
  UINT32          Value;
  UINT64          Timer;
  //
  // Serialize the MSR programming in BSP and all APs.
  //
  if (mSmmRuntimeCtlHooksEnable) {
    TurnOffMorphing (NULL);
  }

  if (CpuIndex != gSmst->CurrentlyExecutingCpu) {
    //
    // We are APs, exit after BSP resets mNumOfCpu.
    //
    InterlockedIncrement (&mNumOfCpu);
    while (mNumOfCpu != 0) {
      ;
    }

  } else {
    //
    // I am BSP
    //
    Timer = StartSyncTimer ();
    while (mNumOfCpu != gSmst->NumberOfCpus - 1) {
      if (IsSyncTimerTimeout (Timer)) {
        DEBUG ((DEBUG_ERROR, "BSP sync with APs timeout in SmmRuntimeCtlExit!\n"));
        //
        // LCME only requires the CPU thread which singles MCE to enter SMM,
        //  so force exit SMM when SMM sync is timeout.
        // The timeout should be replaced by SMM MP service in future for good SMM performance.
        //
        break;
      }
    }

    //
    // Resets mNumOfCpu to 0 to signal All APs to exit.
    //
    do {
      Value = mNumOfCpu;
    } while (InterlockedCompareExchange32 (
               &mNumOfCpu,
               Value,
               0
               ) != Value);

    if (GetPendSmiFlag ()) {
      SetPendSmiFlag (FALSE);
      SendSmiIpi (GetApicId ());
      SendSmiIpiAllExcludingSelf ();
    }
    TransitionEmcaHandlerState (SmmMceFinishedState, SmmMceNotStartedState);
  }
}

/**
  Processor specific hook point each time a CPU exits System Management Mode.

  @param[in] CpuIndex  The index of the CPU that is exiting SMM.  The value must
                       be between 0 and the NumberOfCpus field in the System
                       Management System Table (SMST).
**/
VOID
EFIAPI
SmmCpuFeaturesRendezvousExit (
  IN UINTN  CpuIndex
  )
{
  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    if (CpuIndex == gSmst->CurrentlyExecutingCpu) {
      ClearRasRegister ();
    }
      SmmRuntimeCtlExit (CpuIndex);
    return ;
  default:
    return ;
  }
}

/**
  Hook point in normal execution mode that allows the one CPU that was elected
  as monarch during System Management Mode initialization to perform additional
  initialization actions immediately after all of the CPUs have processed their
  first SMI and called SmmCpuFeaturesInitializeProcessor() relocating SMBASE
  into a buffer in SMRAM and called SmmCpuFeaturesHookReturnFromSmm().
**/
VOID
EFIAPI
SmmCpuFeaturesSmmRelocationComplete (
  VOID
  )
{
  //
  // Dump some SMM feature setting after SMM rellocation
  //
  DEBUG ((EFI_D_INFO, "CpuSmmFeatureControlSupported= %d\n", mSmmFeatureControlSupported));
  DEBUG ((EFI_D_INFO, "CpuSmmSaveStateInMsr         = %d\n", mSaveStateInMsr));
  DEBUG ((EFI_D_INFO, "CpuSmmProtectedModeEnable    = %d\n", mSmmProtectedModeEnable));
  DEBUG ((EFI_D_INFO, "CpuSmmUseDelayIndication     = %d\n", mSmmUseDelayIndication));
  DEBUG ((EFI_D_INFO, "CpuSmmUseBlockIndication     = %d\n", mSmmUseBlockIndication));
  DEBUG ((DEBUG_INFO, "CpuSmmSupovrStateLock        = %d\n", mSmmSupovrStateLock));

  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    if (!IsSmrrSupported ()) {
      return ;
    }
    mFeatureContext.SmrrEnabled = TRUE;
    mFeatureContext.EmrrSupported = IsEmrrSupported ();
    if (IsSmrr2Supported () && mSmrr2Size != 0 && mSaveStateInMsr) {
      //
      // SMRR2 feature requires SaveStateInMsr feature enabled
      //
      mFeatureContext.Smrr2Enabled = TRUE;
    } else {
      mFeatureContext.Smrr2Enabled = FALSE;
    }
    return ;
  default:
    return ;
  }
}

/**
  Setup SMM Protected Mode context for processor.

  @param  ProcessorNumber    The processor number.
  @param  SmBase             The SMBASE value of the processor.
  @param  StackAddress       Stack address of the processor.
  @param  GdtBase            Gdt table base address of the processor.
  @param  GdtSize            Gdt table size of the processor.
  @param  CodeSegment        Code segment value.
  @param  ProtModeIdt        Pointer to protected-mode IDT descriptor.
**/
VOID
SetupSmmProtectedModeContext(
  IN UINTN                    ProcessorNumber,
  IN UINT32                   SmBase,
  IN UINT32                   StackAddress,
  IN UINTN                    GdtBase,
  IN UINTN                    GdtSize,
  IN UINT16                   CodeSegment,
  IN IA32_DESCRIPTOR          *ProtModeIdt
  )
{
  SMMSEG                      *ThreadSmmSeg;
  IA32_SEGMENT_DESCRIPTOR     *GdtDescriptor;
  UINTN                       GdtEntryIndex;

  if (!mSmmProtectedModeEnable) {
    return;
  }

  //
  // Fill SMMSEG structure fields
  //
  ThreadSmmSeg = &mSmmProtModeContext[ProcessorNumber].SmmProtectedModeSMMSEG;
  ThreadSmmSeg->GDTRLimit = (UINT32)(GdtSize - 1);
  ThreadSmmSeg->GDTRBaseOffset = (UINT32)GdtBase - SmBase;
  ThreadSmmSeg->CSSelector = CodeSegment;
  ThreadSmmSeg->ESPOffset = StackAddress - SmBase;
  ThreadSmmSeg->IDTRLimit = ProtModeIdt->Limit;
  ThreadSmmSeg->IDTRBaseOffset = (UINT32)ProtModeIdt->Base - SmBase;
  //
  // Patch 32bit CS/SS segment base to SMBASE, SS = CS + 8 decided by hardware
  //
  GdtEntryIndex = ThreadSmmSeg->CSSelector / sizeof(IA32_SEGMENT_DESCRIPTOR);
  GdtDescriptor = (IA32_SEGMENT_DESCRIPTOR *)GdtBase;
  GdtDescriptor[GdtEntryIndex].Bits.BaseLow = (UINT16)0x0000FFFF & SmBase;
  GdtDescriptor[GdtEntryIndex].Bits.BaseMid = (UINT8)0x000000FF & (SmBase >> 16);
  GdtDescriptor[GdtEntryIndex].Bits.BaseHigh = (UINT8)0x000000FF & (SmBase >> 24);
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseLow = (UINT16)0x0000FFFF & SmBase;
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseMid = (UINT8)0x000000FF & (SmBase >> 16);
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseHigh = (UINT8)0x000000FF & (SmBase >> 24);
}

/**
  Enable SMM Protected Mode.

  @param  ProcessorNumber    The processor number.

**/
VOID
EnableSmmProtectedMode (
  IN UINTN   ProcessorNumber
  )
{
  SMMSEG     *ThreadSmmSeg;
  MSR_SMM_MCA_CAP_REGISTER      McaCap;
  MSR_SMM_PROT_MODE_BASE_REGISTER   SmmProtModeReg;

  //
  // Check if hardware support SMM PROT MODE feature
  //
  McaCap.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);
  if (McaCap.Bits.SmmProtMode == 0) {
    mSmmProtectedModeEnable = FALSE;
    FreePages (mSmmProtModeContext, EFI_SIZE_TO_PAGES(sizeof(SMM_PROT_MODE_CONTEXT) * PcdGet32(PcdCpuMaxLogicalProcessorNumber)));
    return;
  }

  //
  // Enable the SMM PROT MODE feature in the SMMSEG structure
  //
  ThreadSmmSeg = &mSmmProtModeContext[ProcessorNumber].SmmProtectedModeSMMSEG;
  ThreadSmmSeg->SmmSegFeatureEnables = (SMMSEG_FEATURE_ENABLE | SMMSEG_FEATURE_CR4_MCE_CTL_ENABLE);

  //
  // SMMSEG should be 256-byte aligned 32-bit address
  //
  ASSERT (((UINT64)(UINTN)ThreadSmmSeg & ~SMM_PROT_MODE_BASE_PADDR_MASK) == 0);
  SmmProtModeReg.Uint64 = (UINT32)(UINTN)ThreadSmmSeg;
  SmmProtModeReg.Bits.Enable = 1;
  AsmWriteMsr64 (MSR_SMM_PROT_MODE_BASE, SmmProtModeReg.Uint64);
}

/**
  Called during the very first SMI into System Management Mode to initialize
  CPU features, including SMBASE, for the currently executing CPU.  Since this
  is the first SMI, the SMRAM Save State Map is at the default address of
  SMM_DEFAULT_SMBASE + SMRAM_SAVE_STATE_MAP_OFFSET.  The currently executing
  CPU is specified by CpuIndex and CpuIndex can be used to access information
  about the currently executing CPU in the ProcessorInfo array and the
  HotPlugCpuData data structure.

  @param[in] CpuIndex        The index of the CPU to initialize.  The value
                             must be between 0 and the NumberOfCpus field in
                             the System Management System Table (SMST).
  @param[in] IsMonarch       TRUE if the CpuIndex is the index of the CPU that
                             was elected as monarch during System Management
                             Mode initialization.
                             FALSE if the CpuIndex is not the index of the CPU
                             that was elected as monarch during System
                             Management Mode initialization.
  @param[in] ProcessorInfo   Pointer to an array of EFI_PROCESSOR_INFORMATION
                             structures.  ProcessorInfo[CpuIndex] contains the
                             information for the currently executing CPU.
  @param[in] CpuHotPlugData  Pointer to the CPU_HOT_PLUG_DATA structure that
                             contains the ApidId and SmBase arrays.
**/
VOID
EFIAPI
SmmCpuFeaturesInitializeProcessor (
  IN UINTN                      CpuIndex,
  IN BOOLEAN                    IsMonarch,
  IN EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  IN CPU_HOT_PLUG_DATA          *CpuHotPlugData
  )
{
  SMRAM_SAVE_STATE_MAP              *CpuState;
  MSR_SMM_MCA_CAP_REGISTER          McaCap;
  UINT16                            LogProcIndexInPackage;
  UINT64                            CpuBusNum;
  UINT32                            ApicId;
  UINT32                            IntraPackageIdBits;
  SMM_CPU_SYNC_FEATURE              *SyncFeature;
  UINTN                             Cr0;
  UINT8                             *SmmSupovrStateLockAddress;

  mSmmProcessorInfo = ProcessorInfo;

  //
  // Adjust size of SMRR if IED is enabled.
  //
  if (mIEDEnabled) {
    if (!mIEDBaseInitialized) {
      if (mIEDRamBase != 0) {
        //
        // If IED base address is set, IEB ram range should be reported by SMM ACCESS2 protocol
        // and SMM range size has already covered IED range
        //
        CpuHotPlugData->IEDBase = mIEDRamBase;
      } else {
        //
        // If IED base address is not set, we assume it is located at end of SMM range
        //
        CpuHotPlugData->IEDBase = CpuHotPlugData->SmrrBase + CpuHotPlugData->SmrrSize;
        //
        // Enlarge SMM range size because SMRR should cover SMM + IED ranges
        //
        CpuHotPlugData->SmrrSize += mIEDRamSize;
      }
      DEBUG ((DEBUG_INFO, "IEDRAM Base: 0x%x\n", CpuHotPlugData->IEDBase));
      DEBUG ((DEBUG_INFO, "SMRR Base: 0x%x, SMRR Size: 0x%x\n", CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize));
      mIEDBaseInitialized = TRUE;
    }
  } else {
    CpuHotPlugData->IEDBase = (UINT32) -1;
  }

  mSmBase[CpuIndex] = (UINT32)CpuHotPlugData->SmBase[CpuIndex];

  SyncFeature = &(mSmmSyncFeature[CpuIndex]);
  SyncFeature->DelayIndicationSupported = FALSE;
  SyncFeature->BlockIndicationSupported = FALSE;
  SyncFeature->HaswellLogProcEnBit = (UINT64)(INT64)(-1);

  mSmmInitSaveStateInMsr = FALSE;
  McaCap.Uint64 = 0;

  //
  // Configure SMBASE.
  //
  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    if (IsSkylakeServer ()) {
      //
      // Get the Non-IIO bus number assigned by BIOS
      //
      CpuBusNum = AsmReadMsr64 (SKX_MSR_CPU_BUSNUMBER);
      ASSERT ((CpuBusNum & BIT63) != 0);
      mNonIioBusNum[CpuIndex] = (UINT8) (CpuBusNum & 0xff);
    } else if (IsIcelakeServer () || IsSnowridgeServer() || IsSprServer ()) {
      CpuBusNum = AsmReadMsr64 (MSR_CPU_BUSNUMBER);
      ASSERT ((CpuBusNum & BIT63) != 0);
      mNonIioBusNum[CpuIndex] = (UINT8) (CpuBusNum & 0xff);
    }

    //
    // Check SMM Code Access Check bit before access SMM Feature Control MSR
    //
    McaCap.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);
    if (McaCap.Bits.SmmCodeAccessChk != 0) {
      mSmmFeatureControlSupported = TRUE;
    }
    if (McaCap.Bits.SmmCpuSvrstr != 0 &&
        (SmmReadReg64 (CpuIndex, SmmRegFeatureControl) & SMM_CPU_SAVE_EN_BIT) != 0) {
      AsmWriteMsr64 (MSR_SMRAM_SMBASE, CpuHotPlugData->SmBase[CpuIndex]);
        mSmmInitSaveStateInMsr = TRUE;
    } else {
      //
      // Fall back to legacy SMBASE setup.
      //
      CpuState = (SMRAM_SAVE_STATE_MAP *)(UINTN)(SMM_DEFAULT_SMBASE + SMRAM_SAVE_STATE_MAP_OFFSET);
      CpuState->x86.SMBASE = (UINT32)CpuHotPlugData->SmBase[CpuIndex];
    }
    break;

  default:
    return;
  }

  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSnowridge:
  case CpuSpr:
    if(InitSmmRuntimeCtlHooks() == EFI_SUCCESS) {
      SetPendSmiFlag(FALSE);
    }
    //
    // IEDBase = -1 means disabled
    //
    if (CpuHotPlugData->IEDBase != (UINT32) -1) {
      RelocateIedBase (CpuIndex, CpuHotPlugData->IEDBase, IsMonarch);
    }
    if (IsSmrrSupported ()) {
      InitSmrr (CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize, IsMonarch);
    }
    if (IsEmrrSupported ()) {
      InitEmrr (CpuHotPlugData->IEDBase);
    }
    if (IsSmrr2Supported ()) {
      CalculateSmrr2 (CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize, IsMonarch);
    }

    ConfigMsrSaveState (CpuIndex);

    if (mThisCpu->Family != CpuSpr) {
      EnableSmmSaveControl ();
    }

    if ( IsIcelakeServer () || IsSnowridgeServer() || IsSprServer ()) {
      InitChaSmmReg (CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize, CpuIndex, IsMonarch);
    }

    if (mSmmUseDelayIndication || mSmmUseBlockIndication) {
      if (IsSkylakeServer () || IsIcelakeServer () || IsSnowridgeServer() || IsSprServer ()) {
        //
        // Bit [7:0] of this MSR, CURRENT_THREAD_ID, holds an unique number of a thread within
        // the package
        //
        LogProcIndexInPackage = (UINT16) AsmMsrBitFieldRead64 (MSR_THD_LOGICAL_ID, 0, 7);
      } else {
        ApicId = GetInitialApicId ();
        //
        // CPUID.(EAX=0Bh, ECX=01h) provides topology information for the Core level
        //
        AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &IntraPackageIdBits, NULL, NULL, NULL);
        IntraPackageIdBits &= 0x1f;
        LogProcIndexInPackage = ApicId & ((1 << IntraPackageIdBits) - 1);
      }
      SyncFeature->HaswellLogProcEnBit = LShiftU64 (1ull, LogProcIndexInPackage);
    }
    if (mSmmUseDelayIndication) {
      McaCap.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);
      SyncFeature->DelayIndicationSupported = (BOOLEAN)(McaCap.Bits.LongFlowIndication != 0);
    }
    if (mSmmUseBlockIndication) {
      SyncFeature->BlockIndicationSupported = TRUE;
    }
    if ((AsmReadMsr64 (ICX_MSR_PLATFORM_INFO) & BIT59) != 0) {
      //
      // For S3 resume at this point, the first byte at SmmSupovrStateLockAddress
      // should have been patched to 0x90 (NOP) in SmmCpuFeaturesCompleteSmmReadyToLock()
      // during normal boot, here to make sure the first byte at SmmSupovrStateLockAddress
      // is 0xC3 (RET) as initial value.
      //
      Cr0 = AsmReadCr0();
      AsmWriteCr0 (Cr0 & ~CR0_WP);
      SmmSupovrStateLockAddress = (UINT8 *)(UINTN)SmmSupovrStateLock;
      *SmmSupovrStateLockAddress = 0xC3; // RET
      AsmWriteCr0 (Cr0);
      mSmmSupovrStateLock = TRUE;
      if (!PcdGetBool(PcdCpuSmmRestrictedMemoryAccess)) {
        if (CpuIndex == 0) {
          DEBUG ((DEBUG_ERROR, "Unsupported Configuration - PcdCpuSmmRestrictedMemoryAccess is FALSE but SmmSupovrStateLock is TRUE\n"));
        }
        mSmmSupovrStateLock = FALSE;
      }
    }
    if (mSmmProtectedModeEnable) {
      EnableSmmProtectedMode (CpuIndex);
    }
    if (CpuIndex == 0) {
      DEBUG((DEBUG_INFO, "  Microcode ID (0x%08x)  = 0x%016lx\n", 0x0000008B, AsmReadMsr64 (0x0000008B)));
      DEBUG((DEBUG_INFO, "  Platform ID (0x%08x)   = 0x%016lx\n", 0x00000017, AsmReadMsr64 (0x00000017)));
      DEBUG((DEBUG_INFO, "  SMM_MCA_CAP (0x%08x)   = 0x%016lx\n", MSR_SMM_MCA_CAP, AsmReadMsr64 (MSR_SMM_MCA_CAP)));
      DEBUG((DEBUG_INFO, "  PLATFORM_INFO (0x%08x) = 0x%016lx\n", ICX_MSR_PLATFORM_INFO, AsmReadMsr64 (ICX_MSR_PLATFORM_INFO)));
    }

    return ;
  default:
    ASSERT (FALSE);
    return ;
  }
}

/**
  Return the size, in bytes, of a custom SMI Handler in bytes.  If 0 is
  returned, then a custom SMI handler is not provided by this library,
  and the default SMI handler must be used.

  @retval 0    Use the default SMI handler.
  @retval > 0  Use the SMI handler installed by SmmCpuFeaturesInstallSmiHandler()
               The caller is required to allocate enough SMRAM for each CPU to
               support the size of the custom SMI handler.
**/
UINTN
EFIAPI
SmmCpuFeaturesGetSmiHandlerSize (
  VOID
  )
{
  if (mSmmProtectedModeEnable) {
    return gcSmiPMHandlerSize;
  }
  return gcSmmFeatureSmiHandlerSize;
}

/**
  Install a custom SMI handler for the CPU specified by CpuIndex.  This function
  is only called if SmmCpuFeaturesGetSmiHandlerSize() returns a size is greater
  than zero and is called by the CPU that was elected as monarch during System
  Management Mode initialization.

  @param[in] CpuIndex   The index of the CPU to install the custom SMI handler.
                        The value must be between 0 and the NumberOfCpus field
                        in the System Management System Table (SMST).
  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] SmiStack   The stack to use when an SMI is processed by the
                        the CPU specified by CpuIndex.
  @param[in] StackSize  The size, in bytes, if the stack used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtBase    The base address of the GDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtSize    The size, in bytes, of the GDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtBase    The base address of the IDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtSize    The size, in bytes, of the IDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] Cr3        The base address of the page tables to use when an SMI
                        is processed by the CPU specified by CpuIndex.
**/
VOID
EFIAPI
SmmCpuFeaturesInstallSmiHandler (
  IN UINTN   CpuIndex,
  IN UINT32  SmBase,
  IN VOID    *SmiStack,
  IN UINTN   StackSize,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize,
  IN UINTN   IdtBase,
  IN UINTN   IdtSize,
  IN UINT32  Cr3
  )
{
  if (mSmmProtectedModeEnable) {
    //
    // Initialize protected mode IDT
    //
    InitProtectedModeIdt (Cr3);
  }

  //
  // Initialize values in template before copy
  //
  gSmmFeatureSmiStack   = (UINT32)((UINTN)SmiStack + StackSize - sizeof (UINTN));
  gSmmFeatureSmiCr3     = Cr3;
  gSmmFeatureSmiHandlerIdtr.Base = IdtBase;
  gSmmFeatureSmiHandlerIdtr.Limit = (UINT16)(IdtSize - 1);

  //
  // Set the value at the top of the CPU stack to the CPU Index
  //
  *(UINTN*)(UINTN)gSmmFeatureSmiStack = CpuIndex;
  gPMStackDesc[0] = gSmmFeatureSmiStack;

  //
  // Copy template to CPU specific SMI handler location
  //
  if (mSmmProtectedModeEnable) {
    gProtModeSmbase = SmBase;
    CopyMem (
      (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
      (VOID*)gcSmiPMHandlerTemplate,
      gcSmiPMHandlerSize
      );
    //
    // Prepare for the SMMSEG structure
    //
    SetupSmmProtectedModeContext (
      CpuIndex,
      (UINT32)SmBase,
      gSmmFeatureSmiStack,
      GdtBase,
      GdtSize,
      SMMSEG_PROTECT_MODE_CODE_SEGMENT,
      (IA32_DESCRIPTOR *)(UINTN)gProtModeIdtr
      );
  } else {
    gSmmFeatureSmbase = SmBase;
    CopyMem (
      (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
      (VOID*)gcSmmFeatureSmiHandlerTemplate,
      gcSmmFeatureSmiHandlerSize
      );
  }
}

/**
  Check to see if an SMM register is supported by a specified CPU.

  @param[in] CpuIndex  The index of the CPU to check for SMM register support.
                       The value must be between 0 and the NumberOfCpus field
                       in the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to check for support.

  @retval TRUE   The SMM register specified by RegName is supported by the CPU
                 specified by CpuIndex.
  @retval FALSE  The SMM register specified by RegName is not supported by the
                 CPU specified by CpuIndex.
**/
BOOLEAN
EFIAPI
SmmCpuFeaturesIsSmmRegisterSupported (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName
  )
{
  switch (mThisCpu->Family) {
  case CpuSkylake:
  case CpuIcelake:
  case CpuSpr:
    switch (RegName) {
    case SmmRegFeatureControl:
      return mSmmFeatureControlSupported;
    case SmmRegSmmEnable:
      return FALSE;
    case SmmRegSmmDelayed:
      return mSmmSyncFeature[CpuIndex].DelayIndicationSupported;
    case SmmRegSmmBlocked:
      return mSmmSyncFeature[CpuIndex].BlockIndicationSupported;
    default:
      break;
    }
  default:
    break;
  }
  return FALSE;
}

/**
  Returns the current value of the SMM register for the specified CPU.
  If the SMM register is not supported, then 0 is returned.

  @param[in] CpuIndex  The index of the CPU to read the SMM register.  The
                       value must be between 0 and the NumberOfCpus field in
                       the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to read.

  @return  The value of the SMM register specified by RegName from the CPU
           specified by CpuIndex.
**/
UINT64
EFIAPI
SmmCpuFeaturesGetSmmRegister (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName
  )
{
  if (SmmCpuFeaturesIsSmmRegisterSupported (CpuIndex, RegName)) {
    if (RegName == SmmRegFeatureControl) {
      return SmmReadReg64 (CpuIndex, RegName);
    }
    if ((SmmReadReg64 (CpuIndex, RegName) & mSmmSyncFeature[CpuIndex].HaswellLogProcEnBit) != 0) {
      return 1;
    }
  }
  return 0;
}

/**
  Sets the value of an SMM register on a specified CPU.
  If the SMM register is not supported, then no action is performed.

  @param[in] CpuIndex  The index of the CPU to write the SMM register.  The
                       value must be between 0 and the NumberOfCpus field in
                       the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to write.
                       registers are read-only.
  @param[in] Value     The value to write to the SMM register.
**/
VOID
EFIAPI
SmmCpuFeaturesSetSmmRegister (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName,
  IN UINT64        Value
  )
{
  UINT64  NewValue;

  if (SmmCpuFeaturesIsSmmRegisterSupported (CpuIndex, RegName)) {
    if (RegName == SmmRegFeatureControl) {
      SmmWriteReg64 (CpuIndex, RegName, Value);
    } else {
      NewValue = SmmReadReg64 (CpuIndex, RegName);
      if (Value != 0) {
        NewValue = NewValue | mSmmSyncFeature[CpuIndex].HaswellLogProcEnBit;
      } else {
        NewValue = NewValue & (~mSmmSyncFeature[CpuIndex].HaswellLogProcEnBit);
      }
      SmmWriteReg64 (CpuIndex, RegName, NewValue);
    }
  }
}

/**
  This function is hook point called after the gEfiSmmReadyToLockProtocolGuid
  notification is completely processed.
**/
VOID
EFIAPI
SmmCpuFeaturesCompleteSmmReadyToLock (
  VOID
  )
{
  UINT8  *SmmSupovrStateLockAddress;
  UINTN  Cr0;
  DEBUG ((EFI_D_INFO, "SmmCpuFeaturesCompleteSmmReadyToLock...\n"));

  SmmCodeCheckEnableAndLock();
  DEBUG ((EFI_D_INFO, "Locked SMM code check feature\n"));

  if (mSmmSupovrStateLock) {
    DEBUG ((DEBUG_INFO, "Patch SmmSupovrStateLock\n"));
    Cr0 = AsmReadCr0();
    AsmWriteCr0 (Cr0 & ~CR0_WP);

    SmmSupovrStateLockAddress = (UINT8 *)(UINTN)SmmSupovrStateLock;
    *SmmSupovrStateLockAddress = 0x90; // NOP

    AsmWriteCr0 (Cr0);
  }

  DEBUG ((EFI_D_INFO, "SmmCpuFeaturesCompleteSmmReadyToLock Done\n"));
}

/**
  This API provides a method for a CPU to allocate a specific region for storing page tables.

  This API can be called more than once to allocate the page table memory.

  Allocates the number of 4KB pages of type EfiRuntimeServicesData and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  This function can also return NULL if there is no preference on where the page tables are allocated in SMRAM.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer for page tables.
  @retval NULL      Fail to allocate a specific region for storing page tables,
                    Or there is no preference on where the page tables are allocated in SMRAM.

**/
VOID *
EFIAPI
SmmCpuFeaturesAllocatePageTableMemory (
  IN UINTN           Pages
  )
{
  if (Pages == 0) {
    return NULL;
  }

  return NULL;
}

//
// ServerCommonPkg BpHotFix begin
//
/**
  Is Lmce supported by the firmware

  @param[in] LmceFwrEn True if Lmce is supported by the firmware
  @retval NONE

**/
VOID
IsLmceFwrEnabled(
  BOOLEAN *LmceFwrEn
  )
{
  UINT64                            FtControl;

  if(LmceFwrEn != NULL) {
    FtControl = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
    if(BitFieldRead64(FtControl, N_MSR_LMCE_ON_BIT, N_MSR_LMCE_ON_BIT) == 0) {
      *LmceFwrEn = FALSE;
    } else {
      *LmceFwrEn = TRUE;
    }
  }
}

/**
  Is Lmce supported by the processor

  @param[in] Supported True if Lmce is supported by the processor
  @retval NONE

**/
VOID
IsLmceSupported (
  BOOLEAN * Supported
  )
{
  MSR_IA32_MCG_CAP_REGISTER    McgCap;

  if(Supported != NULL) {
    McgCap.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
    if (McgCap.Bits.MCG_LMCE_P == 0) {
      *Supported = FALSE;
    } else {
      *Supported = TRUE;
    }
  }
}

/**
  This function sets and locks SMM_FEATURE_CONTROL_UBOX_DFX_REG CSR.
**/
VOID
SmmCodeCheckEnableAndLock(
  VOID
  )
{
  SMM_FEATURE_CONTROL_UBOX_DFX_STRUCT UboxFeatCtrl;

  if(PcdGetBool (PcdCpuSmmCodeAccessCheckEnable)){
    UboxFeatCtrl.Data = UsraCsrRead(0, 0, SMM_FEATURE_CONTROL_UBOX_DFX_REG);
    UboxFeatCtrl.Bits.smm_code_chk_en = 1;
    DEBUG ((EFI_D_INFO, "UBOX smm_code_chk_en\n"));
    UboxFeatCtrl.Bits.lock = 1;
    UsraCsrWrite(0, 0, SMM_FEATURE_CONTROL_UBOX_DFX_REG, UboxFeatCtrl.Data);
  }
}

//
// ServerCommonPkg BpHotFix end
//