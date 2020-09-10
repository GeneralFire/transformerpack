/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "MktmeStructures.h"
#include <Library/MktmeEarlyFlow.h>
#include <Library/PipeSyncLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemTypeLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/UsraCsrLib.h>
#include <RcRegs.h>
#include <Library/PcuMailBoxLib.h>

VOID
EFIAPI
CreateMktmeInitDataHob (
  IN  MKTME_INIT_DATA_HOB *MktmeInitDataHob
  );

/**

Reports whether the input is a power of 2 or not

@retval TRUE  - If the input is a power of 2
        FALSE - If the input is not a power of 2

**/
BOOLEAN
IsPowerOfTwo64 (
  UINT64        Operand
  )
{
  if (Operand == 0) {
    return TRUE;
  }
  return (BOOLEAN) ((Operand & (Operand - 1)) == 0);
}

/**

Determines maximum physical address width using CPUID command.

@retval UINT32 MaxPhyAddrBits - maximum physical address width that can be used on the system.

**/
UINT32
GetMaxPhyAddrBits (
  VOID
  )
{
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX               CpuIdEax;
  UINT32                                       MaxPhyAddrBits = 0;

  AsmCpuidEx (CPUID_VIR_PHY_ADDRESS_SIZE, 0, &CpuIdEax.Uint32, NULL, NULL, NULL);
  MaxPhyAddrBits = CpuIdEax.Bits.PhysicalAddressBits;

  return MaxPhyAddrBits;
}

/**

Reports whether TME capability is present in the system or not

@param[out] Host  Pointer to sysHost

@retval     TRUE  - TME capability is present in the system
            FALSE - TME capability is NOT present in the system

**/
UINT32
EFIAPI
IsTmeCapSupported (
  OUT PSYSHOST Host
  )
{
  UINT8                                        SocketId;
  CPUID_REGISTER_INFO                          CpuId7;
  MKTME_INIT_DATA_HOB                          MktmeInitDataHob;

  //
  //TME capability in CPUID.(EAX = 7,ECX = 0):ECX[13]
  //
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }
    ExecuteCpuidPipe (SocketId, CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &CpuId7.Eax, &CpuId7.Ebx, &CpuId7.Ecx, &CpuId7.Edx);
    CopyMem (&Host->var.common.procCom[SocketId].ExtCpuid7, &CpuId7, sizeof (CPUID_REGISTER_INFO));

    if ((CpuId7.Ecx & TMECPUIDBIT) == 0) {
      MktmeInitDataHob.TmeCapability = FALSE;
      DEBUG ((EFI_D_ERROR, "TME CPUID bit is NOT supported by Socket %d \n", SocketId));
      break;
    }
    MktmeInitDataHob.TmeCapability = TRUE;
    ZeroMem (&CpuId7, sizeof (CpuId7));
  }

  CreateMktmeInitDataHob (&MktmeInitDataHob);

  return SUCCESS;
}

/**

Reports whether MK-TME capability is present in the system or not

@param[in]  Host  Pointer to sysHost

@retval TRUE  - MK-TME capability is present in the system
        FALSE - MK-TME capability is NOT present in the system

**/
BOOLEAN
EFIAPI
IsMktmeCapSupported (
  IN PSYSHOST Host
  )
{
  UINT8               SocketId;
  TME_CAPABILITY_TYPE TmeCapMsrValue;
  CPUID_REGISTER_INFO CpuId1B;
  BOOLEAN             IsMktmeSupported = TRUE;

  DEBUG ((EFI_D_INFO, "IsMktmeCapSupported called \n"));

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    ExecuteCpuidPipe (SocketId, MKTMECPUID_1B, 0, &CpuId1B.Eax, &CpuId1B.Ebx, &CpuId1B.Ecx, &CpuId1B.Edx);
    CopyMem (&Host->var.common.procCom[SocketId].ExtCpuid1B, &CpuId1B, sizeof (CPUID_REGISTER_INFO));
    // CPUID.1B.0.EAX must has value 0x1 if socket is mktme capable
    if (CpuId1B.Eax != 0x1) {
      DEBUG ((EFI_D_ERROR, "MK-TME CPUID 1B is NOT supported by socket:%d \n", SocketId));
      IsMktmeSupported = FALSE;
      break;
    }
    ZeroMem (&CpuId1B, sizeof (CpuId1B));

    TmeCapMsrValue.Data = ReadMsrPipeHL (SocketId, SPR_MSR_TME_CAPABILITY);
    if ((TmeCapMsrValue.Reg.Bits.MkTmeMaxKeyidBits != 0) && (TmeCapMsrValue.Reg.Bits.MkTmeMaxKeys != 0)) {
      DEBUG ((EFI_D_INFO, "MKTME is supported by SocketId: %d  \n", SocketId));
    } else {
      DEBUG ((EFI_D_ERROR, "MKTME is NOT supported by SocketId: %d  \n", SocketId));
      IsMktmeSupported = FALSE;
      break;
    }

    // MKTME capability in CPUID.7:EDX[18]
    if (((Host->var.common.procCom[SocketId].ExtCpuid7.Edx & BIT18) == 0)) {
      DEBUG ((EFI_D_ERROR, "MK-TME CPUID 7:EDX[18] is NOT supported by socket:%d \n", SocketId));
      IsMktmeSupported = FALSE;
      break;
    }
  }

  return IsMktmeSupported;
}



/**

Calculates number of KeyIdBits availible for MK-TME usage.

@param[in]  VOID

@retval UINT8 KeyIdBits  - Number of KeyIdBits calculated

**/
UINT8
EFIAPI
CalculateKeyIdBits (
  VOID
  )
{
  UINT32                   MaxPhyAddrBits = 0;
  UINT8                    SocketId = 0;
  TME_CAPABILITY_TYPE      TmeCapMsrValue;
  UINT8                    MaxKeyIdBits = 0xFF;
  UINT8                    PhysicalAddressBits = 0;
  UINT8                    KeyIdBits = 0;


  MaxPhyAddrBits = GetMaxPhyAddrBits ();
  DEBUG ((EFI_D_INFO, "MaxPhyAddrBits: %d\n", MaxPhyAddrBits));

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }
    TmeCapMsrValue.Data = ReadMsrPipeHL (SocketId, SPR_MSR_TME_CAPABILITY);

    if (MaxKeyIdBits > (UINT8) TmeCapMsrValue.Reg.Bits.MkTmeMaxKeyidBits) {
      MaxKeyIdBits = (UINT8) TmeCapMsrValue.Reg.Bits.MkTmeMaxKeyidBits;
    }
  }

  PhysicalAddressBits = GetLatestConsumePA ();
  if (MaxPhyAddrBits >= PhysicalAddressBits) {
    KeyIdBits = (UINT8) (MaxPhyAddrBits - PhysicalAddressBits);
    if (KeyIdBits > MaxKeyIdBits) {
      KeyIdBits = MaxKeyIdBits;
    }
  } else {
    EwlOutputType24 (WARN_TME_MKTME_FAILURE, MINOR_ERR_PHYSICAL_ADDRESS_BITS_EXCEEDED_MAX, EwlSeverityWarning);
    DEBUG ((EFI_D_ERROR, "Physical address bits value exceeded max value. There is no space for KeyId.\n"));
    DEBUG ((EFI_D_ERROR, "Setting KeyIdBits value to zero. Continue TME flow.\n"));
    KeyIdBits = 0;
  }

  return KeyIdBits;
}

/**

Configures MKTME MSRs: TmeActivate(0x981) Exclude Base(0x983) and Mask(0x984)

@param
        TmeKey             - TME Key retrieved from flash
        GetVariableStatus  - EFI_STATUS for GetVariable (TMEKey)
        KeyIdBits          - Number of KeyId bits that will be configured in TmeActivate MSR
        MktmeIntegrity     - Determines whether integrity should be enabled or not

@retval None

**/
VOID
ConfigureTmeMSRs (
       IN      PSYSHOST           Host,
       IN      TMEKEY             TmeKey,
       IN      EFI_STATUS         GetVariableStatus,
       IN      UINT16             TotalKeyIdBits,
       IN      BOOLEAN            MktmeIntegrity
       )
{
  BootMode                       SysBootMode;
  UINT8                          SocketId;
  UINT64_STRUCT                  Key0;
  UINT64_STRUCT                  Key1;
  struct SADTable                SadEntry;
  UINT32                         SadIndex = 0;
  UINT32                         PrevLimit = 0;
  struct memVar                  *mem = NULL;
  TME_EXCLUDE_BASE_TYPE          TmeExcludeBase;
  SECURITY_POLICY                *SecurityPolicy = NULL;
  UINT64                         Base = 0;
  UINT64                         Length = 0;
  UINT64                         BitMask = 0;
  TME_EXCLUDE_MASK_TYPE          TmeExcludeMask;
  INT64                          Remainder = 0;
  UINT32                         MaxPhyAddrBits = 0;
  TME_ACTIVATE_TYPE              TmeActivate;
  EFI_STATUS                     Status = EFI_SUCCESS;

  TmeActivate.Reg.Uint64 = 0;
  TmeExcludeBase.Reg.Uint64 = 0;
  TmeExcludeMask.Reg.Uint64 = 0;

  SecurityPolicy = GetSecurityPolicy ();
  if (SecurityPolicy == NULL) {
    EwlOutputType24 (WARN_TME_MKTME_FAILURE, MINOR_ERR_SECURITY_POLICY_NOT_FOUND, EwlSeverityWarning);
    DEBUG ((EFI_D_ERROR, " GetSecurityPolicy returned NULL. Exiting TME Flow.\n"));
    return;
  }

  DEBUG ((EFI_D_INFO, "ConfigureTmeMSRs called KeyIdBits: %d\n", TotalKeyIdBits));

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!SocketPresent (SocketId)) {
      continue;
    }

    SysBootMode = GetSysBootMode ();

    if ((GetVariableStatus == EFI_SUCCESS) &&
        ((SysBootMode == S3Resume) || // S3 flow
        ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == AdrResume)) || //ADR flow
        (Host->var.mem.dfxMemVars.dfxPerMemMode == PER_MEM_MODE))) { // PMEM flow
      DEBUG ((EFI_D_INFO, "Configure TME activate MSR to use the existing key.\n"));
      TmeActivate.Reg.Bits.KeySelect = 1;
      TmeActivate.Reg.Bits.SaveKeyForStandby = 0;
      Key0.lo = (UINT32) TmeKey.Key[SocketId].Key0 & UINT64LOMASK;
      Key0.hi = (UINT32) InternalMathRShiftU64 ((TmeKey.Key[SocketId].Key0 & UINT64HIMASK), 32);
      Key1.lo = (UINT32) TmeKey.Key[SocketId].Key1 & UINT64LOMASK;
      Key1.hi = (UINT32) InternalMathRShiftU64 ((TmeKey.Key[SocketId].Key1 & UINT64HIMASK), 32);
      WriteMsrPipeHL (SocketId, SPR_MSR_BIOS_TME_KEY_0, Key0);
      WriteMsrPipeHL (SocketId, SPR_MSR_BIOS_TME_KEY_1, Key1);
    } else {
      DEBUG ((EFI_D_INFO, "Configure TME activate MSR to generate a new key.\n"));
      TmeActivate.Reg.Bits.KeySelect = 0;
      TmeActivate.Reg.Bits.SaveKeyForStandby = 1;
    }

    // TME Workarounds
    // To program TME_KEY MSR with any value in lieu of uCode
    if (IsSiliconWorkaroundEnabled ("S1706724385")) {
      TmeActivate.Reg.Bits.KeySelect = 1;
      TmeActivate.Reg.Bits.SaveKeyForStandby = 0;
      Key0.lo = 0x12345678;
      Key0.hi = 0x27914386;
      Key1.lo = 0x81936504;
      Key1.hi = 0x73015249;
      WriteMsrPipeHL (SocketId, SPR_MSR_BIOS_TME_KEY_0, Key0);
      WriteMsrPipeHL (SocketId, SPR_MSR_BIOS_TME_KEY_1, Key1);
    }

    // Apply workaround integrity for spr A0
    // If integrity is supported & spr_revision is not A0, set bit 49 of TME_ACTIVATE
    if ((IsSiliconWorkaroundEnabled ("S14010689545") == FALSE) && MktmeIntegrity) {
      TmeActivate.Reg.Uint64 = TmeActivate.Reg.Uint64 | MKTME_INTEGRITY_BIT;
    } else {
      TmeActivate.Reg.Uint64 = TmeActivate.Reg.Uint64 & (~MKTME_INTEGRITY_BIT);
    }

    // Set m2m.Sysfeatures0.prefDisable to 1 if tme is enabled
    if ((IsSiliconWorkaroundEnabled ("S14010197660"))) {
      SYSFEATURES0_M2MEM_MAIN_STRUCT M2mSysFeatures0;

      M2mSysFeatures0.Data = GetM2mSysFeatures0 (SocketId, 0);
      M2mSysFeatures0.Bits.prefdisable = 1;
      UsraCsrWrite (SocketId, 0, SYSFEATURES0_M2MEM_MAIN_REG, M2mSysFeatures0.Data);
    }

    // Diasble Uncore GV when TME is enabled
    if (IsSiliconWorkaroundEnabled ("S14010738859")) {
      if (!SocketPresent (SocketId)) {
        continue;
      }
      Status = PcuSetMiscWorkaroundEnable (SocketId, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_CFC_GV_DISABLE);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "Error while disabling Uncore GV \n"));
      }
    }

    TmeActivate.Reg.Bits.MkTmeKeyidBits = TotalKeyIdBits;
    if (SecurityPolicy->EnableTdx) {
      TmeActivate.Reg.Bits.TdxHkidThreshold = (SecurityPolicy->KeySplit > TotalKeyIdBits) ? TotalKeyIdBits : SecurityPolicy->KeySplit;
    } else {
      TmeActivate.Reg.Bits.TdxHkidThreshold = 0;
    }

    TmeActivate.Reg.Bits.TmeEnable = 1;

    if (TotalKeyIdBits != 0) {
      TmeActivate.Reg.Uint64 = TmeActivate.Reg.Uint64 | BIT48;
    }

    TmeActivate.Reg.Bits.Lock = 1;

    DEBUG ((EFI_D_INFO, "TmeActivate value: %lx\n", TmeActivate.Data));

    //Exclude CR AD region if availible
    if (Host->var.mem.dfxMemVars.dfxPerMemMode != NON_PER_MEM_MODE) {
      mem = &(Host->var.mem);

      for (SadIndex = 0; SadIndex < SAD_RULES_10NM; SadIndex++) {
        SadEntry = mem->socket[SocketId].SAD[SadIndex];
        if (SadEntry.Enable == 1 && (SadEntry.type == MemType1lmAppDirect)) {
          Base = (UINT64) PrevLimit;
          Base = MultU64x64(Base, 64);
          Length = (UINT64) SadEntry.Limit;
          Length = MultU64x64(Length, 64) - Base;
          break;
        }
        PrevLimit = mem->socket[SocketId].SAD[SadIndex].Limit;
      }
    } else{
      Base = SecurityPolicy->DfxTmeExclusionBase;
      Length = SecurityPolicy->DfxTmeExclusionLength;
    }

    if (Length > 0) {
      DivS64x64Remainder (Base, Length, &Remainder);
    } else {
      Remainder = 1;
    }

    if ((IsPowerOfTwo64 (Length)) && (Remainder == 0)) {
      //Set upper 63:MAXPHYS bits to 0
      //MAXPHYS includes keyID bits
      MaxPhyAddrBits = GetMaxPhyAddrBits();
      BitMask = (LShiftU64 (1, MaxPhyAddrBits) - 1) & ~(0xFFF);

      TmeExcludeBase.Reg.Uint64 = Base & BitMask;

      Length = ~(Length - 1);
      TmeExcludeMask.Reg.Uint64 = Length & BitMask;
      TmeExcludeMask.Reg.Bits.Enable = 1;
    } else {
      DEBUG ((EFI_D_INFO, "Base and Mask value does not follow the range register programming rules. Setting Base and Mask to 0\n"));
    }

    WriteMsrPipeHL (SocketId, SPR_MSR_TME_EXCLUDE_BASE, TmeExcludeBase.Data);
    DEBUG ((EFI_D_INFO, "WriteMsrPipeHL Successful for MSR_TME_EXCLUDE_BASE %x\n", SPR_MSR_TME_EXCLUDE_BASE));
    WriteMsrPipeHL (SocketId, SPR_MSR_TME_EXCLUDE_MASK, TmeExcludeMask.Data);
    DEBUG ((EFI_D_INFO, "WriteMsrPipeHL Successful for MSR_TME_EXCLUDE_MASK %x\n", SPR_MSR_TME_EXCLUDE_MASK));
    WriteMsrPipeHL (SocketId, SPR_MSR_TME_ACTIVATE, TmeActivate.Data);
    DEBUG ((EFI_D_INFO, "WriteMsrPipeHL Successful for MSR_TME_ACTIVATE %x\n", SPR_MSR_TME_ACTIVATE));
  }
  return;

}

VOID
EFIAPI
CreateMktmeInitDataHob (
  MKTME_INIT_DATA_HOB *MktmeInitDataHob
  )
{
  VOID *HobPtr = NULL;

  HobPtr = BuildGuidDataHob (
    &gMktmeInitDataGuid,
    MktmeInitDataHob,
    sizeof (*MktmeInitDataHob)
    );

  if (HobPtr == NULL) {
    DEBUG ((EFI_D_ERROR, "Error publishing MTKME_INIT_DATA HOB data\n"));
  }
}

MKTME_INIT_DATA_HOB *
GetMkTmeInitHob (VOID)
{
  EFI_HOB_GUID_TYPE      *GuidHob = NULL;
  MKTME_INIT_DATA_HOB    *MktmeInitDataHob = NULL;

  GuidHob = GetFirstGuidHob (&gMktmeInitDataGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "  Error: TME DATA HOB not found, exiting...\n"));
    ASSERT (FALSE);
    return NULL;
  }
  MktmeInitDataHob = (MKTME_INIT_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  if (MktmeInitDataHob == NULL) {
    DEBUG ((EFI_D_ERROR, "  Error: TME DATA HOB not found, exiting...\n"));
    return NULL;
  }
  return MktmeInitDataHob;
}

/**

Entry point for MKTME Early Init

@retval Status

**/
UINT32
EFIAPI
MktmeEarlyFlow (IN PSYSHOST Host)
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  UINTN                              size = 0;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *PeiVariable;
  EFI_PEI_SERVICES                   **PeiServices;
  TME_CAPABILITY_TYPE                TmeCapMsrValue;
  TMEKEY                             TmeKey;
  EFI_STATUS                         GetVariableStatus;
  UINT8                              SocketId = 0;
  SECURITY_POLICY                    *SecurityPolicy;
  BOOLEAN                            MktmeIntegrity = FALSE;
  MKTME_INIT_DATA_HOB                *pMktmeInitDataHob = NULL;

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_MKTME_EARLY_FLOW, CHECKPOINT_MINOR_MKTME_EARLY_FLOW, SocketId));
  DEBUG ((EFI_D_INFO, "TME Early Flow called !\n"));

  SecurityPolicy = GetSecurityPolicy ();

  if (SecurityPolicy == NULL) {
    EwlOutputType24 (WARN_TME_MKTME_FAILURE, MINOR_ERR_SECURITY_POLICY_NOT_FOUND, EwlSeverityWarning);
    DEBUG ((EFI_D_ERROR," SecurityPolicy returned NULL. Exiting TME Flow.\n"));
    return EFI_D_ERROR;
  }

  pMktmeInitDataHob = GetMkTmeInitHob ();
  if (pMktmeInitDataHob ==  NULL) {
    DEBUG ((EFI_D_ERROR," pMktmeInitDataHob returned NULL. Exiting TME Flow.\n"));
    return EFI_D_ERROR;
  }
  pMktmeInitDataHob->MktmeCapability = IsMktmeCapSupported (Host);

  //Check the number of KeyIdBits availible for each socket and store it in security policy
  if (pMktmeInitDataHob->MktmeCapability) {
    pMktmeInitDataHob->KeyIdBits = CalculateKeyIdBits ();

    SecurityPolicy->MkTmeKeyIdBits = pMktmeInitDataHob->KeyIdBits;
  }

  SecurityPolicy->TmeCapability = pMktmeInitDataHob->TmeCapability;
  SetSecurityPolicy (SecurityPolicy, sizeof (SecurityPolicy));

  if (!pMktmeInitDataHob->TmeCapability) {
    DEBUG ((EFI_D_INFO, "Tme is not supported by the processor.\n"));
    return EFI_SUCCESS;
  }

  //Check if TME is enabled in BIOS setup options
  if (SecurityPolicy->EnableTme == 0) {
    DEBUG ((EFI_D_INFO, " Tme is not enabled in BIOS settings. Exiting TME Flow.\n"));
    return EFI_SUCCESS;
  }

  if ((SecurityPolicy->EnableMktme == 1) && (pMktmeInitDataHob->KeyIdBits != 0)) {
    // Check integrity support initializing it to TRUE
    MktmeIntegrity = TRUE;
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (!SocketPresent (SocketId)) {
        continue;
      }
      TmeCapMsrValue.Data = ReadMsrPipeHL (SocketId, SPR_MSR_TME_CAPABILITY);

      // Integrity is supported when every processor supports it.
      if (!TmeCapMsrValue.Reg.Bits.AesXts128WithIntegrity || SecurityPolicy->MktmeIntegrity == SECURITY_POLICY_DISABLE) {
        MktmeIntegrity = FALSE;
        break;
      }
    }
  } else {
    pMktmeInitDataHob->KeyIdBits = 0;
  }

  size = sizeof (TmeKey);
  PeiServices = (EFI_PEI_SERVICES**) (UINTN) Host->var.common.oemVariable;

  if (PeiServices == NULL) {
    DEBUG ((EFI_D_ERROR, " PeiServices is NULL.Exiting. \n"));
    return (UINT32) Status;
  }

  //Read the EFI variable that stores TME Key
  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gEfiPeiReadOnlyVariable2PpiGuid,
                             0,
                             NULL,
                             (void **)&PeiVariable
                             );
  if (PeiVariable == NULL) {
    DEBUG ((EFI_D_ERROR," PeiVariable is NULL.Exiting. \n"));
    return (UINT32) Status;
  }

  GetVariableStatus = PeiVariable->GetVariable (PeiVariable, L"TmeKey", &gEfiMktmekeyGuid, NULL, &size, (void *)&(TmeKey));
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR," Getvariable error! Status = %r. Exiting. \n", Status));
    return (UINT32) Status;
  }

  ConfigureTmeMSRs (Host, TmeKey, GetVariableStatus, pMktmeInitDataHob->KeyIdBits, MktmeIntegrity);

  return SUCCESS;
}
