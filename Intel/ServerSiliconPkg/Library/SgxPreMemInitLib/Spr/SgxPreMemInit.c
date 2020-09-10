/** @file
  SgxPreMemInit LIBRARY

  Purpose of this library is to provide a hook in memory-code for enabling
  of Software Guard Extensions (SGX) programming before memory is initialized.
  There are more PEIMS and DRIVERS for later phases of SGX initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 - 2020 Intel Corporation. All rights reserved.<BR>

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

#include "SgxPreMemInit.h"

#include <Uefi.h>
#include <Cpu/CpuCoreRegs.h>
#include <Guid/SgxInitDataSpr.h>

#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/MemRcLib.h>
#include <Library/PipeSyncLib.h>
#include <Library/MemDecodeLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SgxPreMemInit.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MktmeEarlyFlow.h>
#include <Library/FusaSafPolicyLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/MemSmeeIpLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Memory/MemoryCheckpointCodes.h>

VOID
SgxPreMemInit (
  IN PSYSHOST Host
  );

static EFI_GUID gEfiPlatformTxtPolicyDataGuid = { 0xa353290b, 0x867d, 0x4cd3, { 0xa8, 0x1b, 0x4b, 0x7e, 0x5e, 0x10, 0x0e, 0x16 } };


/**

  Reports whether SGX capability is present in the system or not

  @param Host              Pointer to sysHost
  @param pSocketCount      Pointer to return socket count

  @retval TRUE             SGX capability is present in the system
  @retval FALSE            SGX capability is NOT present in the system

**/
BOOLEAN
IsSgxCapSupported (
  IN PSYSHOST            Host,
  OUT UINT8              *pSocketCount
  )
{
  BOOLEAN              Status = TRUE;
  UINT8                SocketId;
  MSR_MTRRCAP_REGISTER MtrrCap;
  UINT32               Temp;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX Cpuid7ebx;

  if (pSocketCount == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: pSocketCount is uninitialized!\n");
    Status = FALSE;
    return Status;
  }
  *pSocketCount = 0;

  //
  // SGX capability in CPUID.(EAX=7,ECX=0):EBX[2]=1
  //
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &Temp, &Cpuid7ebx.Uint32, &Temp, &Temp);
  if (!Cpuid7ebx.Bits.SGX) {
    RcDebugPrint (SDBG_MAX, "BSP is not SGX capable\n");
    Status = FALSE;
  } else {
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (!SocketPresent (SocketId)) {
        RcDebugPrint (SDBG_MAX, "  Socket %d not present\n", SocketId);
        continue;
      }
      (*pSocketCount)++;
      //
      // PRMRR capability in MSR IA32_MTRRCAP (FEh)[12] == 1
      //
      MtrrCap.Uint32 = ReadMsrPipeHL (SocketId, MSR_IA32_MTRRCAP).lo;
      if (!MtrrCap.Bits.Prmrr) {
        RcDebugPrint (SDBG_MAX, "Socket[%d] is NOT SGX capable\n", SocketId);
        Status = FALSE;
      }
    }
  }
  return Status;
}

/**

  Reports whether SGX capability is present in the system or not

  @param Host               Pointer to sysHost
  @param pSocketCount       Pointer to return socket count

  @retval TRUE              SGX capability is present in the system
  @retval FALSE             SGX capability is NOT present in the system

**/
BOOLEAN
IsSgxCapSupportedMt (
  IN PSYSHOST             Host,
  OUT UINT8               *pSocketCount
  )
{
  BOOLEAN  Status = TRUE;
  UINT8    Socket = GetSysSocketId ();
  UINT32   Temp;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX Cpuid7ebx;
  MSR_MTRRCAP_REGISTER MtrrCap;
  UINT8 tempCap;

  *pSocketCount = 0;
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &Temp, &Cpuid7ebx.Uint32, &Temp, &Temp);
  if (!Cpuid7ebx.Bits.SGX) {
    RcDebugPrint (SDBG_MAX, "BSP is not SGX capable\n");
    Status = FALSE;
  } else {
    MtrrCap.Uint32 = AsmReadMsr32 (MSR_IA32_MTRRCAP);
    if (!MtrrCap.Bits.Prmrr) {
      Status = FALSE;
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    // count all present sockets
    (*pSocketCount)++;
    if (Socket == GetSysSocketId ()) {
      continue;
    }
    GetPipePackage (Socket, (UINT8*)&tempCap, sizeof (tempCap));
    if (tempCap == 0) {
      Status = FALSE;
    }
  }
  return Status;
}

/**

  Called on APs (apart from main thread) checks SGX capability and sends result to main thread

  @param[in] Host      Pointer to sysHost

  @retval SUCCESS

**/
UINT32
SendSgxCapability (
  IN PSYSHOST Host
  )
{
  MSR_MTRRCAP_REGISTER MtrrCap;
  UINT8 SocketId = GetSysSocketId ();
  UINT8 tempCap;

  MtrrCap.Uint32 = AsmReadMsr32 (MSR_IA32_MTRRCAP);
  tempCap = (MtrrCap.Bits.Prmrr ? 1 : 0);

  SendPipePackage (Host, SocketId, &tempCap, sizeof (tempCap), RCSIM_NO_DELAY);
  return SUCCESS;
}


/**

  Program PRMRR Secured

  @param Host               Pointer to sysHost
  @param SgxPreMemInitHob   Pointer to SgxDataHob
  @param SecurityPolicy     Pointer to Security Policy

  @retval EFI_SUCCESS       PRMRR Secured has been enabled
  @retval Other             PRMRR Secured has not been enabled

**/
EFI_STATUS
SgxSetPrmrrSecured (
  IN PSYSHOST            Host,
  IN SGX_PREMEM_INIT_HOB *SgxPreMemInitHob,
  IN SECURITY_POLICY     *SecurityPolicy
  )
{
  // !DDR5 variable
  UINT8                            SocketIndex;
  UINT8                            Index;
  MC_TME_PRMRR_CFG_MC_2LM_STRUCT   McTmePrmrrCfg;
  // !DDR5 variable
  EFI_STATUS                       Status;
  EFI_PEI_SERVICES                 **PeiServices;
  MRC_HOOKS_CHIP_SERVICES_PPI      *MrcHooksChipServices;

  UINT8       MaxImc;

  MaxImc = GetMaxImc ();

  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;

  // Get the MRC Hooks Chip Services PPI
  Status = (*PeiServices)->LocatePpi (
    PeiServices,
    &gMrcHooksChipServicesPpiGuid,
    0,
    NULL,
    &MrcHooksChipServices
    );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX, "Find MRC Hooks Chip Services Ppi failed, return it's error!\n");
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  for (SocketIndex = 0; SocketIndex < 8; SocketIndex++) {
    if (!SocketPresent (SocketIndex)) {
      RcDebugPrint (SDBG_DEFAULT, "  Socket %d not present\n", SocketIndex);
      continue;
    }
    for (Index = 0; Index <  MaxImc * GetNumChannelPerMc (); Index++) {
      McTmePrmrrCfg.Data = MrcHooksChipServices->UsraCsrRead (SocketIndex, Index, MC_TME_PRMRR_CFG_MC_2LM_REG);
      McTmePrmrrCfg.Bits.prmrr_secured = 1;
      MrcHooksChipServices->UsraCsrWrite (SocketIndex, Index, MC_TME_PRMRR_CFG_MC_2LM_REG, McTmePrmrrCfg.Data);

      EnablePrmrrSecurity (SocketIndex, Index);
    }
  }

  return Status;
}


/**
  Get Cpu Supported PRMRR bitmap.

  @return the valid PRMRR size bitmap per socket.

**/
UINT64
GetCpuSupportedPrmrrBitmap (
  VOID
  )
{
  UINT8                SocketId;
  UINT64_STRUCT        ProtectedMemMsr;
  UINT64               ProtectedMemValidBitMask;

  ProtectedMemValidBitMask = MAX_UINT64;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    ProtectedMemMsr = ReadMsrPipeHL (SocketId, SPR_MSR_PRMRR_VALID_CONFIG);
    ProtectedMemValidBitMask &= LShiftU64 (ProtectedMemMsr.hi, 32 + CONVERT_MB_TO_B) +
                                LShiftU64 (ProtectedMemMsr.lo, CONVERT_MB_TO_B);
  }

  RcDebugPrint (SDBG_MAX, "CPU supported PRMRR size bitmap is 0x%lx\n", ProtectedMemValidBitMask);
  return ProtectedMemValidBitMask;
}

/**
  Verify if all CPUs support TME and if TME is enabled in BIOS settings

  @parma IN Host            Pointer to sysHost

  @retval EFI_SUCCESS       TME support has been verified
  @retval EFI_UNSUPPORTED   TME is not supported

**/
EFI_STATUS
EFIAPI
VerifyTmeSupport (
  IN PSYSHOST             Host
  )
{
  UINT32 Msr;
  MKTME_INIT_DATA_HOB *pMktmeInitDataHob = NULL;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] VerifyTmeSupport start\n");

  //Check if all the sockets support TME
  pMktmeInitDataHob = GetMkTmeInitHob ();
    if (pMktmeInitDataHob ==  NULL) {
    DEBUG ((EFI_D_ERROR," pMktmeInitDataHob returned NULL. Exitting...\n"));
    return EFI_D_ERROR;
  }

  if (!pMktmeInitDataHob->TmeCapability) {
    RcDebugPrint (SDBG_MAX, "Tme is not supported by the processor.\n");
    goto Error_Tme_Unsupported;
  }

  Msr = AsmReadMsr32 (MSR_TME_ACTIVATE);
  if ((Msr & B_TME_ACTIVATE_ENABLE) == 0) {
    RcDebugPrint (SDBG_MAX, "  TME is not enabled\n");
    goto Error_Tme_Unsupported;
  }
  RcDebugPrint (SDBG_DEFAULT, "[SGX] VerifyTmeSupport success end\n");
  return EFI_SUCCESS;

Error_Tme_Unsupported:
  return EFI_UNSUPPORTED;
}

/*
  Returns a pointer to SgxPreMemInitHob

  @retval SGX_PREMEM_INIT_HOB *pSgxPreMemInitHob     A pointer to SgxPreMemInitHob

*/
SGX_PREMEM_INIT_HOB *
EFIAPI
GetSgxPreMemInitHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *pGuidHob          = NULL;
  SGX_PREMEM_INIT_HOB *pSgxPreMemInitHob = NULL;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  pGuidHob = GetFirstGuidHob (&gSgxPreMemInitHobGuid);
  if (pGuidHob == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: SgxPreMemInitHob not found!\n");
  } else {
    pSgxPreMemInitHob = (SGX_PREMEM_INIT_HOB *)GET_GUID_HOB_DATA (pGuidHob);
    if (pSgxPreMemInitHob == NULL) {
      RcDebugPrint (SDBG_ERROR, "  Error: Unable to obtain SgxPreMemInitHob data!\n");
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return pSgxPreMemInitHob;
}


/*
  Returns a pointer to Security MemMap Hob

  @retval SECURITY_MEMMAP_HOB *pSecurityMemMapHob    A pointer to SecurityMemMapHob

*/
SECURITY_MEMMAP_HOB *
EFIAPI
GetSecurityMemMapHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *pGuidHob           = NULL;
  SECURITY_MEMMAP_HOB *pSecurityMemMapHob = NULL;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  pGuidHob = GetFirstGuidHob (&gSecurityMemMapHobGuid);
  if (pGuidHob == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: SecurityMemMapHob not found!\n");
  } else {
    pSecurityMemMapHob = (SECURITY_MEMMAP_HOB *)GET_GUID_HOB_DATA (pGuidHob);
    if (pSecurityMemMapHob == NULL) {
      RcDebugPrint (SDBG_ERROR, "  Error: Unable to obtain SecurityMemMapHob data!\n");
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return pSecurityMemMapHob;
}

/**
  Gets CPU supported PRM bitmap

  @return PRM size bitmap per socket as read from MSR PROTECTED_MEMORY_PER_SKT

**/
UINT64
GetCpuPrmSizeBitmap (
  VOID
  )
{
  UINT8                SocketId;
  UINT64_STRUCT        ProtectedMemMsr;
  UINT64               ProtectedMemValidBitMask;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  ProtectedMemValidBitMask = MAX_UINT64;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    ProtectedMemMsr = ReadMsrPipeHL (SocketId, SPR_MSR_PRMRR_VALID_CONFIG);
    ProtectedMemValidBitMask &= LShiftU64 (ProtectedMemMsr.hi, 32 + CONVERT_MB_TO_B) +
                                LShiftU64 (ProtectedMemMsr.lo, CONVERT_MB_TO_B);
  }

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return ProtectedMemValidBitMask;
}

/**
  Gets MEM supported PRM bitmap

  @return a valid PRM size bitmap per socket as read from memory map

**/
UINT64
GetMemPrmSizeBitmap (
  VOID
  )
{
  UINT64              MemPrmSizeBitmap    = 0;
  SECURITY_MEMMAP_HOB *pSecurityMemMapHob = NULL;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  pSecurityMemMapHob = GetSecurityMemMapHob ();
  if (pSecurityMemMapHob == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: Unable to get SecurityMemMapHob!\n");
    goto Exit_GetMemPrmSizeBitmap;
  }

  if (pSecurityMemMapHob->SgxPrmrrData.PrmrrCountPerPackage != 0) {
    MemPrmSizeBitmap = MultU64x64 (
      pSecurityMemMapHob->SgxPrmrrData.ValidPrmrrBitMap,
      pSecurityMemMapHob->SgxPrmrrData.PrmrrCountPerPackage
      );
  } else {
    RcDebugPrint (SDBG_ERROR, "  Error: PrmrrCountPerPackage equal to 0!\n");
  }

Exit_GetMemPrmSizeBitmap:
  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return MemPrmSizeBitmap;
}

/**
  Calculates PRM bitmap based on CPU and MEM support

  @param[in,out] UINT64 *pPrmSizeBitmap    Calculated PRM bitmap
  @param         UINT64 CpuPrmSizeBitmap   PrmSizeBitmap as read from MSR PROTECTED_MEMORY_PER_SKT
  @param         UINT64 MemPrmSizeBitmap   PrmSizeBitmap as read from SgxPrmrrData (memory map)

  @retval        EFI_SUCCESS           PRM bitmap calculated successfully
  @retval        EFI_INVALID_PARAMETER Invalid input parameters provided

**/
EFI_STATUS
EFIAPI
CalculatePrmSizeBitmap (
  IN OUT UINT64 *pPrmSizeBitmap,
         UINT64 CpuPrmSizeBitmap,
         UINT64 MemPrmSizeBitmap
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  if (NULL == pPrmSizeBitmap) {
    RcDebugPrint (SDBG_ERROR, "  Error: pPrmSizeBitmap is uninitialized!\n");
    Status = EFI_INVALID_PARAMETER;
    goto Exit_CalculatePrmSizeBitmap;
  }

  *pPrmSizeBitmap = CpuPrmSizeBitmap & MemPrmSizeBitmap;
Exit_CalculatePrmSizeBitmap:
  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return Status;
}

/**
  Verifies memory preconditions for SGX

  @param[in,out] *pAreMemoryPreconditionsMet
  @param[in,out] *pPrmSizeBitmap
  @param[in,out] *pPrmSize
  @param[in]     PrmrrCountPerPackage

  @retval EFI_SUCCESS  Memory preconditions for SGX were met
  @retval !EFI_SUCCESS Memory preconditions for SGX were NOT met

**/
EFI_STATUS
EFIAPI
VerifyMemoryPreconditions (
  IN OUT UINT8      *pAreMemoryPreconditionsMet,
  IN OUT BOOLEAN    *pIsPrmSizeInvalidated,
  IN OUT UINT64     *pPrmSize,
  IN OUT UINT64     *pPrmSizeBitmap,
  IN     UINT64     PrmrrCount,
  IN     UINT64     PrmrrCountPerPackage,
  IN     EFI_STATUS MemoryMapStatus
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  if (pAreMemoryPreconditionsMet == NULL ||
      pIsPrmSizeInvalidated      == NULL ||
      pPrmSize                   == NULL ||
      pPrmSizeBitmap             == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: Invalid input parameters!\n");
    Status = EFI_INVALID_PARAMETER;
    goto Exit_VerifyMemoryPreconditions;
  }

  *pAreMemoryPreconditionsMet = SECURITY_POLICY_UNSUPPORTED;

  //
  // PrmrrCount checks
  //
  // Validate PrmrrCount
  if (PrmrrCount == 0) {
    RcDebugPrint (SDBG_DEFAULT, "  Warning: PrmrrCount is equal to 0!\n");
  }
  // Validate PrmrrCountPerPackage
  if (PrmrrCountPerPackage == 0) {
    RcDebugPrint (SDBG_ERROR, "  Error: PrmrrCountPerPackage is equal to 0!\n");
    Status = EFI_UNSUPPORTED;
    goto Exit_VerifyMemoryPreconditions;
  }

  //
  // PrmSizeBitmap checks
  //
  // Validate PrmSizeBitmap
  if (*pPrmSizeBitmap == 0) {
    RcDebugPrint (SDBG_ERROR, "  Error: *pPrmSizeBitmap is equal to 0!\n");
    Status = EFI_UNSUPPORTED;
    *pPrmSize = 0;
    goto Exit_VerifyMemoryPreconditions;
  }

  //
  // PrmSize checks
  //
  // Check if PrmSize invalidated by memory map
  *pIsPrmSizeInvalidated = FALSE;
  if (MemoryMapStatus == EFI_OUT_OF_RESOURCES) {
    *pIsPrmSizeInvalidated = TRUE;
  }

  // Validate requested PrmSize against given *pPrmSizeBitmap
  if ((*pPrmSize & *pPrmSize - 1) != 0 ||
      (*pPrmSize & *pPrmSizeBitmap) == 0) {
    RcDebugPrint (SDBG_ERROR, "  Error: PrmSize has an invalid value: 0x%lx!\n", *pPrmSize);
    *pPrmSize = 0;

    if (*pPrmSizeBitmap) {
      // Select the lowest bit (minimal size) from the supported bitmap
      *pPrmSize = LShiftU64 (1, LowBitSet64 (*pPrmSizeBitmap));
    }
    RcDebugPrint (SDBG_DEFAULT, "  PrmSize corrected to a minimal supported size = 0x%lx\n", *pPrmSize);
    Status = EFI_INVALID_PARAMETER;
    goto Exit_VerifyMemoryPreconditions;
  }
  *pAreMemoryPreconditionsMet = SECURITY_POLICY_SUPPORTED;

Exit_VerifyMemoryPreconditions:
  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
  return Status;
}

/**

  Initial TME dispatcher called from MemCallTable, prepares HOB and checks support

  @param[in] Host      Pointer to sysHost

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
EFIAPI
SgxPreMemInitEntryPoint (
  IN PSYSHOST Host
  )
{
  SGX_PREMEM_INIT_HOB  *pSgxPreMemInitHob = NULL;
  SECURITY_POLICY      *pSecurityPolicy = NULL;
  BOOLEAN              SgxCapability;
  UINT8                SocketCount;

  switch (Host->var.mem.PostCodeMinor) {
    case CHECKPOINT_MINOR_SGX_CHECK_CAP_ST:
      pSgxPreMemInitHob = BuildGuidHob (&gSgxPreMemInitHobGuid, sizeof(*pSgxPreMemInitHob));
      if (pSgxPreMemInitHob == NULL) {
        return FAILURE;
      }
      ZeroMem (pSgxPreMemInitHob, sizeof (*pSgxPreMemInitHob));
      pSecurityPolicy = GetSecurityPolicy ();
      ASSERT (pSecurityPolicy != NULL);
      if (pSecurityPolicy == NULL) {
        RcDebugPrint (SDBG_MAX, "  GetSecurityPolicy returned NULL. Exiting...\n");
        return FAILURE;
      }
      SgxCapability = IsSgxCapSupported (Host, &SocketCount);
      pSecurityPolicy->IsSgxCapable = (SgxCapability ? 1 : 0);
      pSgxPreMemInitHob->SocketCount = SocketCount;
      break;
    case CHECKPOINT_MINOR_SGX_CHECK_CAP_MT:
      if (GetSysSocketId () == GetSysSbspSocketIdNv ()) {
        pSgxPreMemInitHob = BuildGuidHob (&gSgxPreMemInitHobGuid, sizeof(*pSgxPreMemInitHob));
        if (pSgxPreMemInitHob == NULL) {
          return FAILURE;
        }
        ZeroMem (pSgxPreMemInitHob, sizeof (*pSgxPreMemInitHob));
        SgxCapability = IsSgxCapSupportedMt (Host, &SocketCount);
        pSecurityPolicy->IsSgxCapable = (SgxCapability ? 1 : 0);
        pSgxPreMemInitHob->SocketCount = SocketCount;
      } else {
        SendSgxCapability (Host);
      }
      break;
    case CHECKPOINT_MINOR_SGX_PREMEM:
      SgxPreMemInit (Host);
      break;
  }
  return SUCCESS;
}

/**

  Main worker function, calculates most SGX initialization parameters

  @param[in] Host      Pointer to sysHost

  @retval none

**/
VOID
SgxPreMemInit (
  IN PSYSHOST Host
  )
{
  EFI_STATUS           Status          = EFI_SUCCESS;
  EFI_STATUS           MemoryMapStatus = EFI_SUCCESS;
  SGX_ERROR_CODE_ENUM  SgxErrorCode    = RS_SUCCESS;
  SGX_PRMRR_DATA       SgxMemMapData;

  SECURITY_POLICY      *pSecurityPolicy   = NULL;
  FUSA_SAF_POLICY      *pFusaSafPolicy    = NULL;
  SGX_PREMEM_INIT_HOB  *pSgxPreMemInitHob = NULL;

  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a BEGIN\n", __FUNCTION__);

  pSecurityPolicy = GetSecurityPolicy ();
  ASSERT (pSecurityPolicy != NULL);
  if (pSecurityPolicy == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: GetSecurityPolicy returned NULL, exiting...\n");
    return;
  }

  pFusaSafPolicy = GetFusaSafPolicy ();
  if (pFusaSafPolicy == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: GetFusaSafPolicy returned NULL, exiting...\n");
    return;
  }

  pSgxPreMemInitHob = GetSgxPreMemInitHob ();
  if (pSgxPreMemInitHob == NULL) {
    RcDebugPrint (SDBG_ERROR, "  Error: GetSgxPreMemInitHob returned NULL, exiting...\n");
    return;
  }

  if (pSecurityPolicy->DfxMockSgxIsNotCapable == SECURITY_POLICY_DFX_ENABLE) {
    pSecurityPolicy->IsSgxCapable = FALSE;
  }

  if (!pSecurityPolicy->IsSgxCapable) {
    RcDebugPrint (SDBG_ERROR, "  Error: System is NOT SGX capable!\n");
    SgxErrorCode = RS_PREMEM_SYS_NOT_CAPABLE;
    goto Exit_SgxPreMemInit;
  }

  // Obtain memory topology from memory map
  MemoryMapStatus = GetSgxPrmrrData (&SgxMemMapData);
  if (EFI_ERROR (Status) && Status != EFI_OUT_OF_RESOURCES) {
    RcDebugPrint (SDBG_ERROR, "  Error: Current memory topology does not support SGX PRMRR!\n");
    SgxErrorCode = RS_PREMEM_MEM_TOPOLOGY_ERR;
    goto Exit_SgxPreMemInit;
  }

  //
  // Copy initialize Pre-memory initialization HOB.
  //
  pSgxPreMemInitHob->PrmrrCount = SgxMemMapData.PrmrrCount;
  pSgxPreMemInitHob->PrmrrCountPerPackage = SgxMemMapData.PrmrrCountPerPackage;
  CopyMem (&pSgxPreMemInitHob->PrmrrBase, SgxMemMapData.PrmrrBase, sizeof (UINT64) * MAX_PRMRR_COUNT);
  CopyMem (&pSgxPreMemInitHob->McBitMap, SgxMemMapData.McBitMap, sizeof (UINT8) * MAX_SOCKET * MAX_PRMRR_COUNT);
  RcDebugPrint (SDBG_MAX, "SGX PrmrrCount: %d\n", SgxMemMapData.PrmrrCount);
  RcDebugPrint (SDBG_MAX, "SGX PrmrrCountPerPackage: %d\n", SgxMemMapData.PrmrrCountPerPackage);

  //
  // Verify SGX memory preconditions
  //
  Status = CalculatePrmSizeBitmap (
    &pSecurityPolicy->SprspOrLaterPrmSizeBitmap,
    GetCpuPrmSizeBitmap (),
    GetMemPrmSizeBitmap ()
    );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_ERROR, "  Error: Unable to calculate PrmSizeBitmap!\n");
    SgxErrorCode = RS_PREMEM_MEM_TOPOLOGY_ERR;
    goto Exit_SgxPreMemInit;
  }

  Status = VerifyMemoryPreconditions (
    &pSecurityPolicy->SprspOrLaterAreMemoryPreconditionsMet,
    &pSecurityPolicy->SprspOrLaterIsPrmSizeInvalidated,
    &pSecurityPolicy->SprspOrLaterPrmSize,
    &pSecurityPolicy->SprspOrLaterPrmSizeBitmap,
    pSgxPreMemInitHob->PrmrrCount,
    pSgxPreMemInitHob->PrmrrCountPerPackage,
    MemoryMapStatus
    );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_ERROR, "  Error: VerifyMemoryPreconditions failed!\n \
                                        SGX cannot be enabled with current PRM setting!\n", Status);
    SgxErrorCode = RS_PREMEM_MEM_TOPOLOGY_ERR;
    goto Exit_SgxPreMemInit;
  }
  pSecurityPolicy->IsHwCongifSupportedBySgx = pSecurityPolicy->SprspOrLaterAreMemoryPreconditionsMet;

  Status = VerifyTmeSupport (Host);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_ERROR, "  Error: VerifyTmeSupport is NOT valid for SGX \n");
    SgxErrorCode = RS_PREMEM_TME_DISABLED;
    goto Exit_SgxPreMemInit;
  }

  // Verify SGX enabled knob
  if (pSecurityPolicy->EnableSgx == SECURITY_POLICY_DISABLE) {
    RcDebugPrint (SDBG_DEFAULT, "  Warning: SGX is disabled on this system.\n");
    SgxErrorCode = RS_PREMEM_SGX_DISABLED;
    Status = EFI_NOT_READY;
    goto Exit_SgxPreMemInit;
  } else {
    Status = SgxSetPrmrrSecured (Host, pSgxPreMemInitHob, pSecurityPolicy);
    if (EFI_ERROR (Status)) {
      RcDebugPrint (SDBG_ERROR, "  Error: Unable to set PRMRR Secured.\n");
      SgxErrorCode = RS_PREMEM_PRMRR_NOT_SECURED;
      goto Exit_SgxPreMemInit;
    }
  }

Exit_SgxPreMemInit:
  if (pSgxPreMemInitHob != NULL) {
    pSgxPreMemInitHob->ErrorCode = SgxErrorCode;
  }
  RcDebugPrint (SDBG_DEFAULT, "  SgxErrorCode = 0x%02x\n", SgxErrorCode);
  RcDebugPrint (SDBG_DEFAULT, "[SGX] %a END\n", __FUNCTION__);
}
