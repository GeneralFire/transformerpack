/** @file
  This file contains functions for accessing FIT and its entries

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification
**/
#include "FitAccess.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/Tpm20.h>
#include <Cpu/Include/CpuRegs.h>

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data

**/
VOID *
FindFitEntryData (
  IN UINT8                        Type
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT32                          EntryNum;
  UINT64                          FitTableOffset;
  UINT32                          Index;

  FitTableOffset = *(UINT64 *)(UINTN)(BASE_4GB - 0x40);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)(UINTN)FitTableOffset;
  if (FitEntry[0].Address != *(UINT64 *)"_FIT_   ") {
    DEBUG ((EFI_D_ERROR, "\tFIT signature was not found or is incorrect\n"));
    return NULL;
  }
  if (FitEntry[0].Type != FIT_TYPE_00_HEADER) {
    DEBUG ((EFI_D_ERROR, "\tFIT header type is incorrect\n"));
    return NULL;
  }
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0x00FFFFFF;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == Type) {
      DEBUG ((EFI_D_INFO, "\tFitEntry[%d]\t\t\t= 0x%08x\n", Index, &FitEntry[Index]));
      DEBUG ((EFI_D_INFO, "\tFitEntry[%d].Type= 0x%02x\n", Index, FitEntry[Index].Type));
      return &FitEntry[Index];
    }
  }
  DEBUG ((EFI_D_ERROR, "\tFIT entry type 0x%02x was not found\n", Type));

  return NULL;
}

/**
  Find BPM address

  @return BPM address

**/
VOID *
FindBpm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_0C_BOOT_POLICY_MANIFEST);
}

/**
  This functions finds the OBB Hash stored inside the Boot Policy Manifest (BPM)

  @param  BootMode              Current boot mode.

  @retval OBB Hash              A struture pointer where the OBB Hash is located inside the BPM.
  @retval NULL                  BPM was not found or is invalid

**/
VOID *
FindObbHash (
  EFI_BOOT_MODE                   BootMode
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  BPM_HEADER                      *Bpm;
  BPM_ELEMENT_HEADER              *Element;
  BPM_ELEMENT_HEADER              *ElementEnd;
  IBB_ELEMENT                     *Ibb;
  UINT32                          *IbbEntryPoint;
  HASH_LIST                       *DigestList;
  UINTN                           BpmSize;

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FindBpm ();
  if (FitEntry == NULL) {
    DEBUG ((EFI_D_ERROR, "\tBPM entry was not found inside the FIT\n"));
    return NULL;
  }
  Bpm = (BPM_HEADER *)(UINTN)FitEntry->Address;
  if (Bpm == NULL || Bpm->StructureID != BPM_SIGNATURE) {
    DEBUG ((EFI_D_ERROR, "\tBPM entry is not valid\n"));
    return NULL;
  }
  BpmSize = FitEntry->Size[0] | (FitEntry->Size[1] << 8) | (FitEntry->Size[2] << 16);
  DEBUG ((EFI_D_INFO, "\tBpm[0x%08x]\n", (UINT32)Bpm));
  DEBUG ((EFI_D_INFO, "\tBpmSize\t\t\t\t\t= 0x%08x\n", (UINT32)BpmSize));
  Element = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + Bpm->HdrSize);
  ElementEnd = (BPM_ELEMENT_HEADER *)((UINT8 *)Bpm + BpmSize);
  while (Element < ElementEnd) {
    if (Element->StructureId == IBB_SIGNATURE) {
      Ibb = (IBB_ELEMENT *)Element;
      if ((BootMode == BOOT_ON_S3_RESUME && Ibb->SetType == 1) ||
          (BootMode != BOOT_ON_S3_RESUME && Ibb->SetType == 0)) {
        IbbEntryPoint = (UINT32 *)(((UINT8 *)&Ibb->PostIbbHash)
                                   + sizeof (Ibb->PostIbbHash)
                                   + Ibb->PostIbbHash.Size);
        DigestList = (HASH_LIST *)(IbbEntryPoint + 1);
        return (UINT8 *)DigestList + DigestList->Size;
      }
    }
    Element = (BPM_ELEMENT_HEADER *)((UINT8 *)Element + Element->ElementSize);
  }

  return NULL;
}

/**
  This functions determines if the Hashed FVs are intended for Verified Boot, Measured or Both.

  @param  None

  @retval EFI_SUCCESS           The applicable type of boot, whether Verified, Measured or Both.

**/
UINT64
GetBaseHashFvFlag (
  VOID
  )
{
  UINT64            MsrBootGuardSacmInfo;
  UINT64            BootGuardCapability;
  UINT64            MeasuredBoot;
  UINT64            Flag;

  MsrBootGuardSacmInfo = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
  DEBUG ((EFI_D_INFO, "\tMSR_BOOT_GUARD_SACM_INFO[0x%08x] = 0x%016lx\n", MSR_BOOT_GUARD_SACM_INFO, AsmReadMsr64(MSR_BOOT_GUARD_SACM_INFO)));
  BootGuardCapability = MsrBootGuardSacmInfo & B_BOOT_GUARD_SACM_INFO_CAPABILITY;  //BIT32
  MeasuredBoot = MsrBootGuardSacmInfo & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT;      //BIT5
  Flag = 0;
  // If BootGuard (BTG) is not supported by the CPU, OBB verification can still be enforced with the PCD, PcdObbVerificationEnforcementEnable
  if (BootGuardCapability || PcdGetBool (PcdObbVerificationEnforcementEnable) == TRUE) {
    if (!(BootGuardCapability & B_BOOT_GUARD_SACM_INFO_CAPABILITY)) {
      DEBUG ((EFI_D_INFO, "\tBTG is not supported. Enforcing OBB Verification\n"));
    }
// APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported      
    //Flag |= HASHED_FV_FLAG_VERIFIED_BOOT;
// APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported      
  }
  if (MeasuredBoot) {
// APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported        
    //Flag |= HASHED_FV_FLAG_MEASURED_BOOT;
// APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported      
  }
  // If BootGuard (BTG) is not supported and there is no Measured boot, skip all FVs in the OBB verification
  if (!Flag) {
    // APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported  
    // Flag |= HASHED_FV_FLAG_SKIP_ALL;
	// APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported 
  }

  return Flag;
}

// APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported
/**
  This functions installs a PPI containing all Hashed FVs along with their Hash information.

  @param  StoredHashFvPpi       A structure pointer to PPI where hashed FVs along with their Hash information are stored.
  @param  HashInfo              A structure pointer to the Hash of all the FVs Hashes
  @param  BootMode              Current boot mode.

  @retval EFI_SUCCESS           PPI was successfully installed.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.

**/  
#if 0
EFI_STATUS
InstallStoredHashFvPpi (
  IN EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI  *StoredHashFvPpi,
  IN HASH_STRUCTURE                                     *HashInfo,
  IN EFI_BOOT_MODE                                      BootMode
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *PpiDescriptor;

  Status                  = EFI_SUCCESS;
  PpiDescriptor           = NULL;

  if (HashInfo != NULL) {
    StoredHashFvPpi->HashInfo.HashFlag   = FV_HASH_FLAG_BOOT_MODE (BootMode);
    StoredHashFvPpi->HashInfo.HashAlgoId = HashInfo->HashAlg;
    StoredHashFvPpi->HashInfo.HashSize   = HashInfo->Size;
    CopyMem (StoredHashFvPpi->HashInfo.Hash, HashInfo + 1, HashInfo->Size);
    //DEBUG_CODE
    UINT8 Index;
    DEBUG ((EFI_D_INFO, "\tStoredHashFvPpi[0x%08x] = 0x%08x\n", StoredHashFvPpi,*(UINT32 *)StoredHashFvPpi));
    DEBUG ((EFI_D_INFO, "\tHashInfo.HashFlag\t= 0x%016lx\n", StoredHashFvPpi->HashInfo.HashFlag));
    DEBUG ((EFI_D_INFO, "\tHashInfo.HashAlgoId= 0x%04x\n", StoredHashFvPpi->HashInfo.HashAlgoId));
    DEBUG ((EFI_D_INFO, "\tHashInfo.HashSize\t= 0x%04x\n", StoredHashFvPpi->HashInfo.HashSize));
    DEBUG ((EFI_D_INFO, "\tHashInfo.Hash\t\t\t=\n\t"));
    for (Index = 0; Index < StoredHashFvPpi->HashInfo.HashSize; Index++) {
      DEBUG ((EFI_D_INFO, "%02x", (UINT8)StoredHashFvPpi->HashInfo.Hash[Index]));
      if (Index == (StoredHashFvPpi->HashInfo.HashSize - 1)) {
        DEBUG ((EFI_D_INFO, "\n"));
      }
    }
    //DEBUG_CODE
  } else {
    StoredHashFvPpi->HashInfo.HashFlag   = 0;
    StoredHashFvPpi->HashInfo.HashAlgoId = TPM_ALG_NULL;
    StoredHashFvPpi->HashInfo.HashSize   = 0;
  }
  PpiDescriptor = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiDescriptor == NULL) {
    DEBUG ((EFI_D_ERROR, "\tFailing to allocate resources for PpiDescriptor\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  PpiDescriptor->Guid  = &gEdkiiPeiFirmwareVolumeInfoStoredHashFvPpiGuid;
  PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDescriptor->Ppi   = (VOID *) StoredHashFvPpi;
  Status = PeiServicesInstallPpi (PpiDescriptor);
  if (EFI_ERROR (Status)) {
    FreePool (PpiDescriptor);
  }

  return Status;
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported  
