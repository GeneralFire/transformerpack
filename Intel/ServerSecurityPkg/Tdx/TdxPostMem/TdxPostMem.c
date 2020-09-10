/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2019 Intel Corporation. <BR>

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

#include <TdxPostMem.h>
#include <Library/SecurityPolicyLib.h>
#include <Register/ArchitecturalMsr.h>

EFI_PEI_SERVICES                   **mPeiServices = NULL;
EFI_PEI_MP_SERVICES_PPI            *mMpServicesPpi = NULL;
UINT32                             mSocketCount = 1;
BOOLEAN                            mIsTdxCapableSocket[MAX_SOCKET] = { 0 };
EFI_PROCESSOR_INFORMATION          *mProcInfo = NULL;
SPR_MSR_SEAMRR_BASE_REGISTER       mSeamrrBaseRegister;
SPR_MSR_SEAMRR_MASK_REGISTER       mSeamrrMaskRegister;
SECURITY_POLICY                    *mSecurityPolicy = NULL;

/**
Detects MP Services data.
Detects number of processors, number of sockets and BSPs for each socket.

@param  None

@retval EFI_SUCCESS       Load the Mp services structures successfully.
**/

EFI_STATUS
TdxPeiServicesData (
  VOID
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  UINTN                    ProcessorNum;
  UINTN                    EnabledProcessorNum;
  UINTN                    Index;

  Status = mMpServicesPpi->GetNumberOfProcessors (
                          mPeiServices,
                          mMpServicesPpi,
                          &ProcessorNum,
                          &EnabledProcessorNum
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error detecting number of processors.\n"));
    return Status;
  }

  Status = (*mPeiServices)->AllocatePool (
                          mPeiServices,
                          (sizeof (EFI_PROCESSOR_INFORMATION) * ProcessorNum),
                          (VOID **) &mProcInfo
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error allocating memory for mProcInfo table.\n"));
    return Status;
  }

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < ProcessorNum; Index++) {
    Status = mMpServicesPpi->GetProcessorInfo (
                            mPeiServices,
                            mMpServicesPpi,
                            Index,
                            &mProcInfo[Index]
                            );
    if (EFI_ERROR (Status)) {
      FreePool(mProcInfo);
      DEBUG ((EFI_D_ERROR, "Error getting processor info for processor: %d.\n", Index));
      return Status;
    }

    if (Index == 0) {
      mProcInfo[Index].StatusFlag |= PROCESSOR_AS_BSP_BIT; // PackageBsp
    } else if (mProcInfo[Index].Location.Package != mProcInfo[Index - 1].Location.Package) {
      mProcInfo[Index].StatusFlag |= PROCESSOR_AS_BSP_BIT;
      mSocketCount++;
    }
  }

  return Status;
}

/**
Executes CPUID on every BSP thread.

@param  None

@retval None
**/
VOID
IsTdxSupportedSocket (
  VOID
  )
{
  EFI_STATUS                                   Status;
  UINTN                                        Socket = 0;
  UINTN                                        Thread = 0;
  SPR_MSR_MTRRCAP_REGISTER                     MtrrCapReg;

  Status = mMpServicesPpi->WhoAmI (
                          mPeiServices,
                          mMpServicesPpi,
                          &Thread
                          );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Exit if the thread is NOT package BSP
  //
  if ((mProcInfo[Thread].StatusFlag & PROCESSOR_AS_BSP_BIT) == 0) {
    return;
  }

  //
  // Convert from thread to socket number
  //
  Socket = mProcInfo[Thread].Location.Package;

  //
  // TDX capability in UCODE_CR_MTRRCAP(0xfe).SEAMRR(bit15)
  //
  MtrrCapReg.Uint64 = AsmReadMsr64 (SNR_MSR_UCODE_CR_MTRRCAP);
  mIsTdxCapableSocket[Socket] = (MtrrCapReg.Bits.Smrr == 1);

  return;
}


/**
Reports whether TDX capability is present in the system or not
Starts multi-threaded function to execute CPUID

@param      None

@retval     TRUE      TDX capability is present in the system
            FALSE     TDX capability is NOT present in the system
**/
BOOLEAN
EFIAPI
IsTdxSupported (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT32                    SocketId;
  BOOLEAN                   TdxSupported = TRUE;

  IsTdxSupportedSocket ();
  Status = mMpServicesPpi->StartupAllAPs (
                          mPeiServices,
                          mMpServicesPpi,
                          (EFI_AP_PROCEDURE) IsTdxSupportedSocket,
                          FALSE,
                          0,
                          NULL
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Error checking TDX capability on all cores, continue init\n"));
  }

  //
  // Sytem supports TDX only if all packages support TDX
  //
  for (SocketId = 0; SocketId < mSocketCount; SocketId++) {
    DEBUG ((EFI_D_INFO, "IsTmeCapableSocket[%d] = %d\n", SocketId, mIsTdxCapableSocket[SocketId]));
    if (mIsTdxCapableSocket[SocketId] == FALSE) {
      TdxSupported = FALSE;
      break;
    }
  }

  return TdxSupported;
}

/**
Writes to SEAMRR BASE and MASK on every BSP thread.

@param  None

@retval None
**/
VOID
WriteToCoreSeamrr (
  VOID
  )
{
  EFI_STATUS                                   Status;
  UINTN                                        Thread = 0;

  Status = mMpServicesPpi->WhoAmI (
                          mPeiServices,
                          mMpServicesPpi,
                          &Thread
                          );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Exit if the thread is NOT package BSP
  //
  if ((mProcInfo[Thread].StatusFlag & PROCESSOR_AS_BSP_BIT) == 0) {
    return;
  }

  AsmWriteMsr64(SPR_MSR_SEAMRR_BASE, mSeamrrBaseRegister.Uint64);
  AsmWriteMsr64(SPR_MSR_SEAMRR_MASK, mSeamrrMaskRegister.Uint64);

  return;
}

/**

Determines maximum physical address width using CPUID command.

@retval UINT32 MaxPhyAddrBits - maximum physical address width that can be used on the system.

**/
UINT32
GetMaxPhyAddrBits() {
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX               CpuIdEax;
  UINT32                                       MaxPhyAddrBits = 0;

  AsmCpuidEx (CPUID_VIR_PHY_ADDRESS_SIZE, 0, &CpuIdEax.Uint32, NULL, NULL, NULL);
  MaxPhyAddrBits = CpuIdEax.Bits.PhysicalAddressBits;

  return MaxPhyAddrBits;
}

/**

Calculates proper SEAMRR BASE and MASK values, based on the PRMRR_0 confidurarion.
NOT YET IMPLEMENTED.

@param  None

@retval EFI_SUCCESS       All Core SEAMRR confugured properly.

**/
EFI_STATUS
ConfigureTdxSeamrr (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64     ValidSeamrrBitsMask = 0xFFFFFFFFFFFFFFFF;
  UINT32     MaxPhyAddrBits = GetMaxPhyAddrBits();
  UINT32     PhysicalAddr;

  if (mSecurityPolicy->EnableMktme) {
    PhysicalAddr = MaxPhyAddrBits - mSecurityPolicy->MkTmeKeyIdBits;
    ValidSeamrrBitsMask = (LShiftU64 (1, PhysicalAddr) - 1) & 0xfffffffffffff000;
  }

  mSeamrrBaseRegister.Uint64 = 0x102000008;
  mSeamrrMaskRegister.Uint64 = 0x000FFFFFFE000000 & ValidSeamrrBitsMask;

  // Set Lock bit
  mSeamrrMaskRegister.Bits.L = 1;

  WriteToCoreSeamrr ();
  Status = mMpServicesPpi->StartupAllAPs (
                          mPeiServices,
                          mMpServicesPpi,
                          (EFI_AP_PROCEDURE) WriteToCoreSeamrr,
                          FALSE,
                          0,
                          NULL
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Error writing to core SEAMRR\n"));
  }

  return Status;
}

/**

Configure M2M Seamrr base and mask CSRs

@param  Void
@retval Void
**/
VOID
ConfigureUncoreM2mTdxSeamrr (
  VOID
  )
{
  UINT8 SocketId;
  UINT8 ImcIndex;

  for (SocketId = 0; SocketId < mSocketCount; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    for (ImcIndex = 0; ImcIndex < GetMaxImc (); ImcIndex++) {
#ifdef GNRSRF_HOST
      UsraCsrWrite (SocketId, ImcIndex, B2CMI_SEAMRR_BASE_N0_M2MEM_MAIN_REG, (UINT32)mSeamrrBaseRegister.Uint64);
      UsraCsrWrite (SocketId, ImcIndex, B2CMI_SEAMRR_BASE_N1_M2MEM_MAIN_REG, (UINT32)RShiftU64 (mSeamrrBaseRegister.Uint64, 32));
      UsraCsrWrite (SocketId, ImcIndex, B2CMI_SEAMRR_MASK_N0_M2MEM_MAIN_REG, ((UINT32)mSeamrrMaskRegister.Uint64 | BIT11));
      UsraCsrWrite (SocketId, ImcIndex, B2CMI_SEAMRR_MASK_N1_M2MEM_MAIN_REG, (UINT32)RShiftU64 (mSeamrrMaskRegister.Uint64, 32));
#else // GNRSRF_HOST
      UsraCsrWrite (SocketId, ImcIndex, M2M_SEAMRR_BASE_N0_M2MEM_MAIN_REG, (UINT32)mSeamrrBaseRegister.Uint64);
      UsraCsrWrite (SocketId, ImcIndex, M2M_SEAMRR_BASE_N1_M2MEM_MAIN_REG, (UINT32)RShiftU64 (mSeamrrBaseRegister.Uint64, 32));
      UsraCsrWrite (SocketId, ImcIndex, M2M_SEAMRR_MASK_N0_M2MEM_MAIN_REG, ((UINT32)mSeamrrMaskRegister.Uint64 | BIT11));
      UsraCsrWrite (SocketId, ImcIndex, M2M_SEAMRR_MASK_N1_M2MEM_MAIN_REG, (UINT32)RShiftU64 (mSeamrrMaskRegister.Uint64, 32));
#endif // GNRSRF_HOST

      DEBUG ((EFI_D_INFO, "Setting M2M SeamrrBase (SocketId=%d,  ImcId=%d,  SeamrrBase=0x%016llx\n",
        SocketId,
        ImcIndex,
        mSeamrrBaseRegister.Uint64
        ));

      DEBUG ((EFI_D_INFO, "Setting M2M SeamrrMask (SocketId=%d,  ImcId=%d,  SeamrrMask=0x%016llx\n",
        SocketId,
        ImcIndex,
        mSeamrrMaskRegister.Uint64
        ));
    }
  }
}

/*

Returns TdxDataHob or creates empty one if not exist

@param  VOID

@retval pointer to hob structure on success
@retval NULL on failure

**/
TDX_DATA_HOB *
GetTdxDataHob (
  VOID
  )
{
  TDX_DATA_HOB *pTdxDataHob = NULL;
  EFI_HOB_GUID_TYPE *GuidHob = NULL;

  GuidHob = GetFirstGuidHob(&gTdxDataHobGuid);

  if (GuidHob == NULL) {
    pTdxDataHob = BuildGuidHob (&gTdxDataHobGuid, sizeof (*pTdxDataHob));

    if (pTdxDataHob == NULL) {
      DEBUG ((EFI_D_ERROR, " Error: Unable to allocate TDX_DATA_HOB\n"));
      return NULL;
    }

    ZeroMem (pTdxDataHob, sizeof (*pTdxDataHob));

  } else {
    pTdxDataHob = (TDX_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if (pTdxDataHob == NULL) {
      DEBUG ((EFI_D_ERROR, "[TDX] Error: Unable to find TDX_DATA_HOB\n"));
    }
  }
  return pTdxDataHob;
}

/**
PEIM entry point

@param FileHandle         Pointer to the PEIM FFS file header.
@param PeiServices        General purpose services available to every PEIM.

@retval EFI_SUCCESS       Operation completed successfully.
@retval Otherwise         TDX initialization failed.

**/
EFI_STATUS
EFIAPI
TdxPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           IsTdxCapable;
  TDX_DATA_HOB    *pTdxDataHob;

  DEBUG ((EFI_D_INFO, "TdxPostMem entry\n"));

  mPeiServices = (EFI_PEI_SERVICES **) PeiServices;

  Status = (*mPeiServices)->LocatePpi (
                              mPeiServices,
                              &gEfiPeiMpServicesPpiGuid,
                              0,
                              NULL,
                              &mMpServicesPpi
                              );

  if (EFI_ERROR (Status) || (mMpServicesPpi == NULL)) {
    DEBUG ((EFI_D_ERROR, "Error: Mp Services Ppi not found. Exiting TDX Flow.\n"));
    return Status;
  }

  // Collect all the MP Services data needed
  Status = TdxPeiServicesData ();
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error setting up Mp Services Protocol data. Exiting TDX Flow.\n"));
    return Status;
  }

  // Check if the system supports TDX
  IsTdxCapable = (UINT8) IsTdxSupported ();

  // Get tdx data hob
  pTdxDataHob = GetTdxDataHob ();
  if (pTdxDataHob == NULL) {
    DEBUG ((EFI_D_ERROR," GetTdxDataHob returned NULL. Exiting TDX Flow.\n"));
    return EFI_D_ERROR;
  }
  pTdxDataHob->TdxCapability = IsTdxCapable;

  mSecurityPolicy = GetSecurityPolicy ();
  if (mSecurityPolicy == NULL) {
    DEBUG ((EFI_D_ERROR," GetSecurityPolicy returned NULL. Exiting TDX Flow.\n"));
    return EFI_D_ERROR;
  }

  // Update SecurityPolicy hob.
  mSecurityPolicy->TdxCapability = IsTdxCapable;
  SetSecurityPolicy(mSecurityPolicy, sizeof(mSecurityPolicy));

  if (!mSecurityPolicy->EnableTdx) {
    DEBUG ((EFI_D_INFO, "TDX is not enabled in setup. Exiting TDX flow.\n"));
    return EFI_SUCCESS;
  }

  if (MsrTraceEnabled ()){
    DEBUG ((EFI_D_INFO, "TDX MSR Trace Start\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }

  // Configure SEAMRR
  Status = ConfigureTdxSeamrr ();
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error configuring SEAMRR. Exiting TDX Flow.\n"));
  }

  // Configure M2M SEAMRR WA for B0
  if (IsSiliconWorkaroundEnabled ("S14011198324")) {
    ConfigureUncoreM2mTdxSeamrr ();
  }

  if (MsrTraceEnabled ()){
    DEBUG ((EFI_D_INFO, "TDX MSR Trace End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }

  return Status;
}
