/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

  Unless otherwise agreed by Intel in writing, you may not remove or alter this notice
  or any other notice embedded in Materials by Intel or Intel's suppliers or licensors in any way.
**/

#include "SipCore3v0.h"

// Might need a static global to speed things up
EFI_PEI_MP_SERVICES_PPI   *mMpServices = NULL;

// FIXME: Implement so that module globals not needed
#define SECURITY_POLICY_PRMRR_SIZE_1GB       0x0040000000
CONST EFI_PEI_SERVICES   **mPeiServices = NULL;
UINT64                     mPrmrrSize = SECURITY_POLICY_PRMRR_SIZE_1GB;
UINT64                    *mPrmrrBase;
UINT64                     mPrmrrMask;
UINT8                      mPrmrrCount;
EFI_PROCESSOR_INFORMATION *mProcInfo;
UINTN                      mNumberProcessors = 1;
UINT8                      mSocketCount = 1;
UINTN                      mSystemBsp;
UINT32                     mBspSocketId;
#ifdef DEBUG
  volatile UINT8           mCore = 0;
#endif // DEBUG


#define B_MSR_PRMRR_CONFIG_BIT                  BIT3
#define B_MSR_PRMRR_PHYS_MASK_ASSIST            BIT9
#define B_MSR_PRMRR_PHYS_MASK_LOCK              BIT10
#define B_MSR_PRMRR_PHYS_MASK_VALID             BIT11
#define B_TME_ACTIVATE_KEYIDBITS_MASK           (BIT0 | BIT1 | BIT2 | BIT3)
#define B_BIOS_PKG_CONFIG_ENABLE                BIT1
#define B_BIOS_PKG_CONFIG_LOCK                  BIT0
#define B_SPR_MSR_SGX_RAS_OPTIN                 BIT0
#define B_HOSTIA_POSTBOOT_SAI                   BIT0
#define B_HOSTIA_UCODE_SAI                      BIT1
#define B_HOSTIA_SMM_SAI                        BIT2
#define B_HOSTIA_SUNPASS_SAI                    BIT3
#define B_HOSTIA_BOOT_SAI                       BIT4
#define B_PM_PCS_SAI                            BIT9
#define B_DFX_INTEL_MANUFACTURING_SAI           BIT24
#define B_DFX_INTEL_PRODUCTION_SAI              BIT42
#define B_OOB_MSM_SAI                           BIT61
#define B_MSR_MCG_CONTAIN_POISON_ENABLE         BIT0
#define B_MSR_MCG_CAP_SER_P                     BIT24

/**
  Detect MP Services data like system BSP, Socket BSPs.

  @param  None

  @retval EFI_SUCCESS       Load the Mp services structures. successfully.
          mMpServices       Pointer to MP Services PPI
          mSystemBsp        Thread number of the system BSP
          mProcInfo         Pointer to first element of array of EFI_PROCESSOR_INFORMATION
**/
EFI_STATUS
SgxMpServicesData (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINTN           EnabledProcessorNum;
  UINTN           Index;

  SGX_DEBUG ((SGX_D_VERBOSE, "SgxMpServicesData()\n"));
  Status = (*PeiServices)->LocatePpi (
    PeiServices,
    &gEfiPeiMpServicesPpiGuid,
    0,
    NULL,
    &mMpServices
    );
  if (EFI_ERROR (Status) || (mMpServices == NULL)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: Mp Services not found.\n"));
    return Status;
  }

  Status = mMpServices->GetNumberOfProcessors (
    PeiServices,
    mMpServices,
    &mNumberProcessors,
    &EnabledProcessorNum
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error: detecting number of processor threads.\n"));
    return Status;
  }

  //Memory will be freed in ClearGlobalVariables at the end of flow for this file
  mProcInfo = (EFI_PROCESSOR_INFORMATION *)
              AllocatePages (EFI_SIZE_TO_PAGES (sizeof (EFI_PROCESSOR_INFORMATION) * mNumberProcessors));
  if (mProcInfo == NULL) {
    SGX_DEBUG ((SGX_D_ERROR, "  Error: allocating mProcInfo.\n"));
    return EFI_NOT_FOUND;
  }
  ZeroMem (mProcInfo, sizeof (EFI_PROCESSOR_INFORMATION) * mNumberProcessors);

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < mNumberProcessors; Index++) {
    Status = mMpServices->GetProcessorInfo (
      PeiServices,
      mMpServices,
      Index,
      &mProcInfo[Index]
      );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Assign Package BSPs
    //
    if (Index == 0) {
      mProcInfo[Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT; // PackageBsp
    } else {
      if (mProcInfo[Index].Location.Package != mProcInfo[Index - 1].Location.Package) {
        // Next package/socket detected, mark processor as PBSB if package
        mProcInfo[Index].StatusFlag |= PROCESSOR_AS_PBSP_BIT;
        mSocketCount++;
      }
    }

#ifdef DEBUG
    if (mProcInfo[Index].StatusFlag & PROCESSOR_AS_PBSP_BIT) {
      SGX_DEBUG ((SGX_D_VERBOSE, "  Thread %d is BSP of Socket %d\n", Index, mProcInfo[Index].Location.Package));
    }
#endif // DEBUG

  } // for (Index)

  Status = mMpServices->WhoAmI (
    PeiServices,
    mMpServices,
    &mSystemBsp
    );
  if (EFI_ERROR (Status)) {
    SGX_DEBUG ((SGX_D_VERBOSE, "  Error detecting SystemBSP\n"));
    return Status;
  }
  SGX_DEBUG ((SGX_D_VERBOSE, "  System BSP Thread = %3X\n", mSystemBsp));

  mBspSocketId = mProcInfo[mSystemBsp].Location.Package;
  SGX_DEBUG ((SGX_D_VERBOSE, "  System BSP Package = %3X\n", mBspSocketId));

  return Status;
}

UINT64
SgxAsmMsrConditionalAndThenOr64 (
  IN UINT32 Index,
  IN UINT64 Mask,
  IN UINT64 AndData,
  IN UINT64 OrData
  )
{
  UINT64 Data64;

  Data64 = AsmReadMsr64 (Index);
  if ((Data64 & Mask) == 0) {
    return AsmMsrAndThenOr64 (Index, AndData, OrData);
  }
  return Data64;
}
/**
  Create PRMRR(s) based on the CalculatePrmrr results
  This function is executed at core scope

  @param  Thread  information about thread being executed

  @retval None
**/
VOID
ConfigureCorePrmrrs (
  UINTN                         Thread
  )
{
  UINT8                         PrmrrIndex;
  UINT32                        PrmrrRegister[MAX_PRMRR_COUNT] =
                                  { SPR_MSR_PRMRR_BASE_0, SPR_MSR_PRMRR_BASE_1_1, SPR_MSR_PRMRR_BASE_2_2, SPR_MSR_PRMRR_BASE_3_3,
                                    SPR_MSR_PRMRR_BASE_4_4, SPR_MSR_PRMRR_BASE_5_5, SPR_MSR_PRMRR_BASE_6_6, SPR_MSR_PRMRR_BASE_7_7 };

  //SgxQueueDebugM (SGX_D_VERBOSE, PEI_217_CONFIGURE_PRMRRS_BEGIN, NULL);

  for (PrmrrIndex = 0; PrmrrIndex < mPrmrrCount; PrmrrIndex++) {
    //
    // program the PRMRR_PHYSBASE_x
    //
    AsmMsrOr64 (
      PrmrrRegister[PrmrrIndex],
      (mPrmrrBase[PrmrrIndex] | EFI_CACHE_WRITEBACK | B_MSR_PRMRR_CONFIG_BIT)
      );

    SgxQueueDebugM (SGX_D_VERBOSE, PEI_219_PRMRR_BASE, "%d Thread[%d] = 0x%016lX",
                  PrmrrIndex, Thread, AsmReadMsr64 (SPR_MSR_PRMRR_BASE_0 + PrmrrIndex) );
  }

  //
  // program the PRMRR_PHYSMASK
  //
  SgxAsmMsrConditionalAndThenOr64 (
    SPR_MSR_PRMRR_MASK,
    B_MSR_PRMRR_PHYS_MASK_LOCK,
    ~(UINT64)(B_MSR_PRMRR_PHYS_MASK_VALID),
    mPrmrrMask | B_MSR_PRMRR_PHYS_MASK_LOCK
    );
  SgxQueueDebugM (SGX_D_VERBOSE, PEI_21A_PRMRR_MASK, "Thread[%d] = 0x%016lX",
                  Thread, AsmReadMsr64 (SPR_MSR_PRMRR_MASK) );

  SgxQueueDebugM (SGX_D_VERBOSE, PEI_218_CONFIGURE_PRMRRS_END, NULL);
}


VOID
CoreScopeProgramCorePrmrr (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Thread;

  Status = mMpServices->WhoAmI (
    mPeiServices,
    mMpServices,
    &Thread
    );
  if (EFI_ERROR (Status)) {
#ifdef DEBUG
    SgxQueueDebugM (SGX_D_VERBOSE, PEI_210_ERROR_DETECTING_THREAD, NULL);
    Thread = 0xBAD;
#endif // DEBUG
    return;
  }

  //
  // Exit if the thread is not a primary thread
  //
  if (mProcInfo[Thread].Location.Thread != 0) {
    return;
  }
#ifdef DEBUG
  if (Thread == 0) {
    mCore = 0;
  } else {
    mCore = mCore + 1;
  }
  SgxQueueDebug (SGX_D_VERBOSE, "<Core[%3X]>", mCore);
#endif // DEBUG

  ConfigureCorePrmrrs (Thread);

#ifdef DEBUG
  SgxQueueDebug (SGX_D_VERBOSE, "</Core[%3X]>", mCore);
#endif // DEBUG
}
