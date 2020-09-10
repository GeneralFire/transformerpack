/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/PipeSyncLib.h>
#include <SysHostChip.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/UncoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include "BiosSsaChipFunc.h"
#include "SysHostChipCommon.h"
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuAccessLib.h>
#include <Library/CpuInitLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/KtiApi.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/PcdLib.h>
#include <Library/ChaLib.h>
#include <Library/SysHostPointerLib.h>
#include <Register/Cpuid.h>
#include <Library/PeiIioInitLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PcuIpLib.h>
#include <Library/MeUmaLib.h>
#include <Library/CpuAndRevisionLib.h>
#include "Include/ProcMemInitLibInternal.h"

#include <Library/IoLib.h>
#include <Ppi/Dwr.h>
#include <Library/PeiServicesLib.h>

#include <Library/PcieMmcfgLib.h>
#include <ScratchpadList.h>
#include <Library/LastBootErrorPeiLib.h>
#include <Library/PmcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>

#include <Library/PeiServicesLib.h>
#include <ReferenceCodePolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <RcVersion.h>
#include <SiliconSetting.h>
#include <Library/UsraCsrLib.h>
#include <Library/SimulationServicesLib.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <UncoreCommonIncludes.h>
#include <Library/S3mLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/HbmCoreLib.h>
#include <MemCommon.h>

#include <Guid/MrcPlatformHooksGuid.h>

//
// Fixed base for SIM_BUILD
//
#ifdef GNRSRF_HOST
#define PCU_BAR_BASE  0x80000000
#define MEM_BAR_BASE  (PCU_BAR_BASE + UBOX_MMIO_BAR_SIZE)
#else // GNRSRF_HOST
#define MEM_BAR_BASE  (8 << 28)
#endif // GNRSRF_HOST

//
// Simics MSR definitions
//

#define MSR_SIMICS_MEMFLOWS    0xdeadbee0
#define MSR_SIMICS_HBMFLOWS    0xdeadbee2

//
// Memory Subsystem Map Tables
//

//
// ICX with 4 MCs, 2 CH per MC
//
MRC_MSM MsmTableIcx[] = {
  {Ddr4MemSs, 2, 18, 18, 72},
  {Ddr4MemSs, 2, 18, 18, 72},
  {Ddr4MemSs, 2, 18, 18, 72},
  {Ddr4MemSs, 2, 18, 18, 72}
};

MRC_MSM MsmTableSnr[] = {
  {Ddr4MemSs, 2, 18, 18, 72}
};

#ifdef ICXD_LCC_SKU
MRC_MSM MsmTableIcxDe[] = {
  {Ddr4MemSs, 3, 18, 18, 72},
};
#else // ICXD_LCC_SKU
MRC_MSM MsmTableIcxDe[] = {
  {Ddr4MemSs, 2, 18, 18, 72},
  {Ddr4MemSs, 2, 18, 18, 72},
};
#endif // ICXD_LCC_SKU


MRC_MSM MsmTableSprDdr5[] = {
  {Ddr5MemSs, 2, 20, 10, 80},
  {Ddr5MemSs, 2, 20, 10, 80},
  {Ddr5MemSs, 2, 20, 10, 80},
  {Ddr5MemSs, 2, 20, 10, 80}
};

MRC_MSM MsmTableGnrDdr5[] = {
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
  {Ddr5MemSs, 1, 20, 10, 80},
};

MRC_MSM MsmTableDntDdr5[] = {
  { Ddr5MemSs, 1, 10, 10, 40 }
};

MRC_MSM MsmTableReserved10nm[] = {
   { ReservedMemSs, 0, 0, 0, 0 }
};


//
// Function Declarations
//

STATIC
VOID
CheckSupportedCpu (
  IN PSYSHOST  Host
  );

//
// Function Implementations
//

/*++

Routine Description:

  Initialize the Cpu info based on any params sent to RcSim.
  This initializes the CpuTypeInfo library with these settings so
  that subsequent calls to GetCpuType returns the correct info.

  @param[out] Family          A pointer to the Cpu family.
  @param[out] Type            A pointer to the Cpu Type.
  @param[out] SubType         A pointer to the Cpu Subtype.
  @param[out] Stepping        A pointer to the Cpu stepping.

  @retval EFI_SUCCESS

--*/

EFI_STATUS
InitCpuFamilySteppingSim (
  OUT UINT32 *Family,
  OUT UINT8 *Type,
  OUT UINT8 *SubType,
  OUT UINT8 *Stepping
  )
{

  return EFI_SUCCESS;

} // InitCpuFamilySteppingSim

/*++

Routine Description:

  Initialize the Cpu info by calling into CpuTypeLib.

  @param[out] Host          A pointer to host.

  @retval EFI_SUCCESS

--*/

EFI_STATUS
EFIAPI
InitCpuFamilyTypeStepping (
  OUT PSYSHOST Host
  )
{
  UINT32 Family = 0;
  UINT8 Type = 0;
  UINT8 SubType = 0;
  UINT8 Stepping = 0;

  InitCpuFamilySteppingSim (&Family, &Type, &SubType, &Stepping);

  return EFI_SUCCESS;

} // InitCpuFamilyTypeStepping

/**
  Initialize Reference Code Policy

  @param VOID

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval OTHERS            Function retured with failure.

**/
EFI_STATUS
EFIAPI
InitializeReferenceCodePolicy (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_PPI_DESCRIPTOR      *ReferenceCodePolicyPpiDescPtr;
  VOID                        *ReferenceCodePolicyPtr;

  RcDebugPrint (SDBG_DEFAULT, "\n\n[Enter] Initialize Reference Code Policy!\n");

  //
  // Build Data HOB for Reference Code Policy
  //
  ReferenceCodePolicyPtr = BuildGuidHob (&gReferenceCodePolicyHobGuid, (UINTN)sizeof (REFERENCE_CODE_POLICY));
  if (ReferenceCodePolicyPtr == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "Failed to build Data HOB for Reference Code Policy due to out of resource !!!\n");
    ASSERT (ReferenceCodePolicyPtr != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Update HOB with default settings from fixed PCD
  //
  ((REFERENCE_CODE_POLICY *) ReferenceCodePolicyPtr)->NumaEn = PcdGet8 (PcdNumaEn);
  ((REFERENCE_CODE_POLICY *) ReferenceCodePolicyPtr)->UmaBasedClustering = PcdGet8 (PcdUmaBasedClustering);

  //
  // Print the default settings of Reference Code Policy
  //
  RcDebugPrint (SDBG_DEFAULT, ">>> Print the default settings of Reference Code Policy! Begin >>>\n");
  RcDebugPrint (SDBG_DEFAULT, "ReferenceCodePolicyPtr->NumaEn = %X\n", ((REFERENCE_CODE_POLICY *) ReferenceCodePolicyPtr)->NumaEn);
  RcDebugPrint (SDBG_DEFAULT, "ReferenceCodePolicyPtr->UmaBasedClustering = %X\n", ((REFERENCE_CODE_POLICY *) ReferenceCodePolicyPtr)->UmaBasedClustering);
  RcDebugPrint (SDBG_DEFAULT, "<<< Print the default settings of Reference Code Policy! End   <<<\n");

  //
  // Publish PPI ReferenceCodePolicyPpi that points to the HOB
  //
  ReferenceCodePolicyPpiDescPtr = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (ReferenceCodePolicyPpiDescPtr == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "Failed to allocateZeroPool for ReferenceCodePolicyPpi Descriptor !!!\n");
    ASSERT (ReferenceCodePolicyPpiDescPtr != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  ReferenceCodePolicyPpiDescPtr->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  ReferenceCodePolicyPpiDescPtr->Guid = &gReferenceCodePolicyPpiGuid;
  ReferenceCodePolicyPpiDescPtr->Ppi = ReferenceCodePolicyPtr;
  Status = PeiServicesInstallPpi (ReferenceCodePolicyPpiDescPtr);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_DEFAULT, "Failed to publish Reference Code Policy PPI! Status = %r\n", Status);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  RcDebugPrint (SDBG_DEFAULT, "[Exit] Initialize Reference Code Policy!\n\n");

  return EFI_SUCCESS;
}

/**
  Call owners of data structures to allocate memory for and initialize their structures

  @retval N/A

**/
VOID
InitMrcDataStructures (
  VOID
  )
{
  //
  // Initialize all DDRIO IP Lib data structures, including NVRAM data
  //
  InitDdrioDataStructure ();
  //
  // Validate DRAM data structure
  //
  ValidateDramDataStucture ();
  //
  // Validate RCD data structure
  //
  ValidateRcdDataStucture ();
  //
  // Validate LRdimm Data Buffer data structure
  //
  ValidateDbDataStucture();
}


/**

   Executed by SBSP only. Initialize sysHost structure with default data.

  @param Host - pointer to sysHost structure on stack

**/
VOID
InitializeDefaultData (
  IN PSYSHOST  Host
  )
{
  UINT32  CsrReg;

  MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_STRUCT    mmcfgLocalRule0;
  MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_STRUCT    mmcfgLocalRule1;
  UINT64_STRUCT  msrData;
  UINT32 ImcCount;
  UINT8  SbspScktId;
  UINT8  Bus[MAX_IIO_STACK];
  UINT8  BusOffset;
  UINT8  ScktId;
  UINT8  StackId;
  UINT8  Index;
  struct sysNvram  *nvramPtr;
  SYS_SETUP *Setup;
  UINT8       Skt10nmUboxBus0;
  KTI_HOST_OUT *KtiHostOutPtr;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;
  RC_VERSION *RcVersion;
  CAPID3_PCU_FUN3_STRUCT  CapId3;

  InitMrcDataStructures ();

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  Skt10nmUboxBus0 = 0;
  CpuCsrAccessVar   = GetSysCpuCsrAccessVar ();
  ZeroMem (Bus, sizeof (UINT8) * MAX_IIO_STACK);

  RcVersion = (RC_VERSION *) PcdGetPtr (PcdRcVersion);
  CopyMem (&Host->var.common.RcVersion, RcVersion, sizeof (RC_VERSION));
  CopyMem (&Host->nvram.common.RcVersion, RcVersion, sizeof (RC_VERSION));

  PopulateMemChipPolicy (Host);

  //
  // Initialize Reference Code Policy.
  //
  InitializeReferenceCodePolicy ();

  //
  // Get CPU bus numbers from MSR 0x300
  //
  SbspScktId = GetSbspSktId ();

  OutPort32 (0x0cf8, LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0);  // MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG
  mmcfgLocalRule0.Data = InPort32 (0x0cfc);
  OutPort32 (0x0cf8, LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1);  // MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG
  mmcfgLocalRule1.Data = InPort32 (0x0cfc);

  //
  // TODO: Need to set more stacks' base/limit per MMCFG_LOCAL_RULE_ADDRESS_CFG_X_CHABC_SAD1_REG for SPR.
  //
  Bus[0] = (UINT8)mmcfgLocalRule0.Bits.limit0;
  Bus[1] = (UINT8)mmcfgLocalRule0.Bits.limit1 + 1;      //stack1 base is stack0 limit + 1
  Bus[2] = (UINT8)mmcfgLocalRule0.Bits.limit2 + 1;      //stack2 base is stack1 limit + 1
  Bus[3] = (UINT8)mmcfgLocalRule0.Bits.limit3 + 1;      //stack3 base is stack2 limit + 1
  Bus[4] = (UINT8)mmcfgLocalRule1.Bits.limit0 + 1;      //stack4 base is stack3 limit + 1
  Bus[5] = (UINT8)mmcfgLocalRule1.Bits.limit1 + 1;      //stack5 base is stack4 limit + 1

  msrData = ReadMsr (MSR_CPU_BUSNUMBER);
  Skt10nmUboxBus0 = (UINT8)(msrData.lo & 0xff);


  if (SbspScktId >= MAX_SOCKET) {
    RcDebugPrint (SDBG_DEFAULT, "ERROR SbspScktId = %d and MAX_SOCKET = %d\n", SbspScktId, MAX_SOCKET);
    return;
  }
  SetSysSocketId (SbspScktId);
  SetSysSbspSocketIdNv (SbspScktId);
  RcDebugPrint (SDBG_DEFAULT, "SBSP socket = %d\n", SbspScktId);
  SetSocketPresentBitMap (1 << SbspScktId);
  Host->var.common.sbsp = 1;

  Setup = GetSysSetupPointer ();



  SetMeRequestedSize (0);
  SetIeRequestedSize (0);

  //
  // Get MMCFG base address
  //
  CsrReg = GetMmcfgAddress();
  //
  // CsrReg = MMCFG_Base
  //
  KtiHostOutPtr->mmCfgBase = CsrReg;

  // For O*L
  KtiHostOutPtr->HostAddress = (UINT32) (UINTN) Host;
  KtiHostOutPtr->SlavePipeAddress = (UINT32) (UINTN) &PeiPipeSlaveInit;
  KtiHostOutPtr->MemRasFlag = 0;
  KtiHostOutPtr->inComingSktId = 0xFF;

  //
  // Out of reset, SEC code assigned 32 buses to each socket
  //
  BusOffset = 0;
  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
    //
    // The buses of SBSP socket must be updated first, and the other buses should be shift accordingly.
    //
    if (ScktId == SbspScktId) {
      BusOffset = 0;
    } else if (ScktId < SbspScktId) {
      BusOffset = (ScktId + 1) * 0x20;
    } else {
      BusOffset = ScktId * 0x20;
    }

    for (StackId = 0; StackId < MAX_IIO_STACK; StackId++) {
      KtiHostOutPtr->StackBus[ScktId][StackId] = Bus[StackId] + BusOffset;
    }

    KtiHostOutPtr->mmCfgBaseL[ScktId] = CsrReg;
    KtiHostOutPtr->mmCfgBaseH[ScktId] = 0;
    KtiHostOutPtr->SocketLastBus[ScktId] = 0x1f + BusOffset;
    KtiHostOutPtr->SocketFirstBus[ScktId] = Bus[IIO_CSTACK] + BusOffset;
    KtiHostOutPtr->segmentSocket[ScktId] = 0;
    KtiHostOutPtr->Socket10nmUboxBus0[ScktId] = Skt10nmUboxBus0 + BusOffset;

  }

  Host->var.mem.IsDdrMemInitDone = FALSE;

  UpdateCpuCsrAccessVar_PEI(CpuCsrAccessVar);

  //
  // from this point on, SBSP at legacy socket can utilize UsraCsrRead/UsraCsrWrite functions for CSR access
  //

  //
  // load CAPID and CPUID information to Host->var.common.procCom for SBSP.
  //
  ZeroMem (&Host->var.common.procCom[SbspScktId], sizeof (struct processorCommon));
  LoadCapId (SbspScktId);
  LoadCpuId (SbspScktId);


  //
  // init emulation flag in sysHost structure
  //
  if (HybridSystemLevelEmulationEnabled ()) {
    CsrReg = SIMICS_FLAG;
  } else {
    CsrReg = UsraCsrRead (SbspScktId, EMULATION_FLAG_CSR_BOXINSTANCE, CSR_EMULATION_FLAG_UBOX_CFG_REG);
  }
  //
  // read SoftSim flag from CSR
  //
  if (CsrReg == 0xFFFFFFFF) {
    SetEmulation(0);
  } else {
    SetEmulation((UINT8) (CsrReg & 0xff));
  }

#ifdef DEBUG_CODE_BLOCK
  if ((CsrReg & QUIET_MODE) && (CsrReg != 0xFFFFFFFF)) {
    SetDebugLevel(SDBG_MIN);
  }
#endif // DEBUG_CODE_BLOCK

  //
  // Check for optional OEM NVRAM image
  //
  nvramPtr = (struct sysNvram *) (UINTN) Setup->common.nvramPtr;
  if ((nvramPtr != NULL) && (&Host->nvram != nvramPtr)) {
    CopyMem ((UINT8 *)&Host->nvram, (UINT8 *)nvramPtr, sizeof (Host->nvram));
  }

  ImcCount = MAX_IMC;

  //
  // Load default data for DdrMaxImc, DdrMaxCh and DdrNumChPerMc, will update them in GetMemSubsystemMap
  //
  CpuCsrAccessVar->DdrMaxImc             = (UINT8) ImcCount;
  CpuCsrAccessVar->DdrNumChPerMc         = MAX_MC_CH;
  CpuCsrAccessVar->DdrMaxCh              = (UINT8) (MAX_MC_CH * ImcCount);

  CpuCsrAccessVar->HbmMaxIoInst         = MAX_HBM_IO;
  CpuCsrAccessVar->HbmMaxCh             = MAX_HBM_CH;
  CpuCsrAccessVar->HbmNumChPerMc        = MAX_HBM_MC_CH;
  CpuCsrAccessVar->HbmNumChPerIo        = MAX_HBM_IO_CH;

  // Assert that build structure size is adequate for runtime loops
  if (MAX_IMC < CpuCsrAccessVar->DdrMaxImc) {
    RcDebugPrint (SDBG_DEFAULT, "\nMRC build error!  MAX_IMC %d is less than DdrMaxImc %d\n", MAX_IMC, CpuCsrAccessVar->DdrMaxImc);
    RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_IMC_NUMBER_EXCEEDED);
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) || IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      KtiHostOutPtr->KtiPortCnt = SI_MAX_KTI_PORTS_SPR;
    } else {
      KtiHostOutPtr->KtiPortCnt = SI_MAX_KTI_PORTS_GNRSRF;
    }
    CapId3.Data = Host->var.common.procCom[SbspScktId].capid3;
    if (CapId3.Bits.capid3_27 == 1 ||
      CapId3.Bits.capid3_28 == 1 ||
      CapId3.Bits.capid3_29 == 1 ||
      CapId3.Bits.capid3_30 == 1) {
      KtiHostOutPtr->HbmSku = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    KtiHostOutPtr->KtiPortCnt = 0;
  } else {
    KtiHostOutPtr->KtiPortCnt = SI_MAX_KTI_PORTS_ICX;
  }

  //
  // init the scaling to 100
  //

  if (KtiHostOutPtr->KtiPortCnt > MAX_FW_KTI_PORTS) {
    RcDebugPrint (SDBG_DEFAULT, "\n\n**** Reported KTI port count (%d) exceeds max num of KTI ports supported by BIOS (%d) ****", KtiHostOutPtr->KtiPortCnt, MAX_FW_KTI_PORTS);
    RC_FATAL_ERROR (FALSE, 0, 0);
  }

  for (Index = 0; Index < CpuCsrAccessVar->DdrMaxImc; Index++) {
    Host->var.mem.socket[SbspScktId].imcEnabled[Index] = 1;
  }

  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
    for (Index = 0; Index < CpuCsrAccessVar->DdrMaxImc; Index++) {
      Host->var.mem.socket[ScktId].ImcPresent[Index] = TRUE; // assume all present
    }
    //
    // Continue if this socket is not present
    //
    if (SocketPresent (ScktId) == FALSE) {
      continue;
    }
    Host->var.mem.socket[ScktId].ddrFreqCheckFlag = 0;
    Host->var.mem.socket[ScktId].ioInitdone = 0;
    CpuCsrAccessVar->MemSsType[ScktId] = 0;
  }
  //
  // Now that iMC info available, update the structure
  //
  UpdateCpuCsrAccessVar_PEI(CpuCsrAccessVar);
}

/**
  Convert simulation value for RxDFE into RX_DFE_DISABLE, RX_DFE_ENABLE, RX_DFE_DEFAULT.
  If caller specifies an invalid value, do nothing.

  @param[in]      Host        - SysHost pointer
  @param[in]      SimPreamble - Preamble setting requested by simulator
  @param[in,out]  PreambleOut - Pointer to caller's output

  @retval   N/A
**/
VOID
SimRxDfeSettingDecode (
  IN        PSYSHOST  Host,
  IN        UINT32    SimRxDfe,
  IN OUT    UINT8     *RxDfeOut
  )
{
}

/**
  Convert simulation value for preamble into PREAMBLE_1TCLK, PREAMBLE_2TCLK or READ/WRITE_PREAMBLE_DEFAULT
  If caller specifies anything other than the above, do nothing.

  @param[in]      Host        - SysHost pointer
  @param[in]      SimPreamble - Preamble setting requested by simulator
  @param[in]      PreambleType- Read or Write preamble
  @param[in,out]  PreambleOut - Pointer to caller's output

  @retval   N/A
**/
VOID
SimPreambleSettingDecode (
  IN        PSYSHOST  Host,
  IN        UINT32    SimPreamble,
  IN        UINT8     PreambleType,
  IN OUT    UINT8     *PreambleOut
  )
{
}

/**
  Initialize Warnings for Sim targets.

  Disable certain warning types promotion to error for Sim
  targets. While we should try to disable warning promototion
  as much as is possible, there will be cases where this is not
  possible.

  @param  None

  @retval None

**/

VOID
InitializeWarningsSim (
  VOID
  )
{


  return;

} // InitializeWarningsSim

  /**
  Initialize data for simulation targets

  @param[in]    Host    - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
InitializePlatformDataSim (
  IN        PSYSHOST  Host
  )
{
  UINT32      RetStatus = SUCCESS;

  InitializeWarningsSim ();


  return RetStatus;
}

/**

  Get emulation memory flows

  @param[in]    Host    - Pointer to sysHost structure

  @retval None

**/

VOID
GetEmulationMemFlows (
  IN PSYSHOST  Host
  )
{

  UINT32 SimicsMrcOption = 0;
  UINT64_STRUCT SimicsMemFlows;
  UINT8 SbspSktId;

  SbspSktId = GetSysSbspSocketIdNv ();

  //
  // SimicsMrcOption is the value of $mrc (TRUE or FALSE)
  // set in the Simics script
  //

  SimicsMrcOption = UsraCsrRead (SbspSktId, EMULATION_FLAG_CSR_BOXINSTANCE, CSR_EMULATION_FLAG_UBOX_CFG_REG);

  RcDebugPrint (SDBG_DEFAULT, "GetEmulationMemFlows: Simics $mrc value = %x\n", SimicsMrcOption);

  switch (GetEmulation ()) {

  case SIMICS_FLAG:

      if (SimicsMrcOption & DDR_TRAINING_EN) {

        //Initialze MemFlows to disable, and only enable the flows desired by simics
        DisableAllMemFlows ();

        //
        // MSR_SIMICS_MEMFLOWS contains the value of $mrc_mem_flows
        // from the Simics script
        //

        SimicsMemFlows = ReadMsr (MSR_SIMICS_MEMFLOWS);

        RcDebugPrint (SDBG_DEFAULT, "GetEmulationMemFlows: Simics $mrc_mem_flows.lo = %x\n", SimicsMemFlows.lo);
        RcDebugPrint (SDBG_DEFAULT, "GetEmulationMemFlows: Simics $mrc_mem_flows.hi = %x\n", SimicsMemFlows.hi);

        //
        // put the raw data coming from Simics into memFlows
        //

        Host->memFlows    = SimicsMemFlows.lo;
        Host->memFlowsExt = SimicsMemFlows.hi;

        //
        // memFlowsExt2 and memFlowsExt3 will need to be added
        // here once Simics has added support.
        //

        //
        // Now cleanup the mem flows coming out of Simics.
        //

        DisableMemFlow (CmdTxEqTraining);
        DisableMemFlow (PhaseCrosstalkTraining);
        DisableMemFlow (TxDdjcTraining);

        EnableMemFlow (NormalModeSwitch);
#ifndef SPR_HOST
        EnableMemFlow (SenseAmpOffsetTraining);
#endif // !SPR_HOST
        EnableMemFlow (CheckPorCompatibility);
        EnableMemFlow (PostTrainingInitialization);
        EnableMemFlow (EarlyInitThermalThrottling);
        EnableMemFlow (LateMemoryControllerConfig);
        EnableMemFlow (EarlyMemoryControllerConfig);
        EnableMemFlow (InitializeMemoryMap);
        EnableMemFlow (InitThermalThrottling);
        EnableMemFlow (InitializeCmiCreditProgramming);
        EnableMemFlow (MmrcInitExecution);

      } else {

        if (HybridSystemLevelEmulationEnabled ()) {

          EnableMemFlow (NormalModeSwitch);
          EnableMemFlow (CheckPorCompatibility);
          EnableMemFlow (InitializeMemoryMap);

        } else {

          DisableAllMemFlows ();
        }
      }

      break;

    default:
      break;
  }


} // GetEmulationMemFlows

/**

Arguments:

  Host - pointer to sysHost structure on stack

**/
VOID
InitializePlatformData (
  IN PSYSHOST  Host
  )
{
  RC_VERSION  *RcVersion;
  SYS_SETUP   *Setup;

  EFI_STATUS Status;
  UINT8 sbspSktId;
  BIOS_NON_STICKY_SCRATCHPAD12_STRUCT ScratchPad12;
  BIOS_NON_STICKY_SCRATCHPAD1_STRUCT  ScratchPad01;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  MRC_STATUS HookStatus;

  //
  // Install MrcHooksSevicesPpi
  //
  InstallMrcHooksServicesPpi (Host);

  //
  // Install MrcHooksChipSevicesPpi
  //
  InstallMrcHooksChipServicesPpi (Host);

  //
  // Init memory data
  //

  InitializeMemoryData (Host);

  //
  // Load data from Nvram. We load Mem, SysInfo, and Ras data
  // from Nvram in this function.
  //

  LoadNvramData ();

  //
  // Get ME requested size
  //

  MeUmaConfigureRequestedSegmentSize ();

  //
  // Call platform hook for platform-specific init. Note that
  // failing to locate the platform hooks interface may be
  // expected, such as in the case of FSP.
  //

  HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;
  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->PlatformEwlInit (Host);
  }

  //
  // If Hooks PlatformEwlInit returns not implemented, or
  // if the hooks are not present, perform the generic EWL init.
  //

  if (HookStatus == MRC_FUNCTION_NOT_IMPLEMENTED) {
    EwlInit ();
  }

  //
  // Add our default warning promotion list
  //

  MemInitDefaultWarnings (Host);

  //
  // Add any platform specific warning promotion to errors
  //

  if (MrcPlatformHooksInterface != NULL) {
    MrcPlatformHooksInterface->PlatformMemInitWarning (Host);
  }



  Setup = GetSysSetupPointer ();

  CheckRankMapout (Host);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // For CTE build, DDR and HBM are mutually exclusive
    //
    EnableAllMemFlows ();
  } else {
    //
    // For non-CTE builds, enable all memFlows by default
    //
    EnableAllMemFlows ();

    Host->memFlows    &= Setup->mem.memFlows;
    Host->memFlowsExt &= Setup->mem.memFlowsExt;
    Host->memFlowsExt2 &= Setup->mem.memFlowsExt2;
    Host->memFlowsExt3 &= Setup->mem.memFlowsExt3;

    //
    // Disable training steps for HVM
    //
    DisableTrainingStepsTarget ();
  }

  Host->var.mem.mccpubusno = 2;
  Host->var.mem.rtDefaultValue = MRC_ROUND_TRIP_DEFAULT_VALUE * GetRoundTripStepUnit ();

  InitializePlatformDataSim (Host);

  GetEmulationMemFlows (Host);


  RcDebugPrint (SDBG_DEFAULT, "memFlows = 0x%x, memFlowsExt = 0x%x, memFlowsExt2 = 0x%x, memFlowsExt3 = 0x%x\n", Host->memFlows, Host->memFlowsExt, Host->memFlowsExt2, Host->memFlowsExt3);
  RcDebugPrint (SDBG_DEFAULT, "Emulation Value is: %x!\n", GetEmulation());
  switch (GetEmulation()) {
    case 0:
      RcDebugPrint (SDBG_DEFAULT, "Running on hardware\n");
      break;

    case SIMICS_FLAG:
      RcDebugPrint (SDBG_DEFAULT, "Running on Simics\n");
      RcDebugPrint (SDBG_DEFAULT, "Revision: %x\n", GetEmulation());
      break;

    case RTL_SIM_FLAG:
      RcDebugPrint (SDBG_DEFAULT, "Running on RTL Simulator\n");
      break;
    default:
      //Automated add of default case. Please review.
      break;
  }

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "ICX-D processor detected\n");
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "SNR processor detected\n");
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "ICX processor detected\n");
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "SPR processor detected\n");
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "GNR processor detected\n");
  } else if (IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "SRF processor detected\n");
  }

  RcVersion = (RC_VERSION *) PcdGetPtr (PcdRcVersion);

  if (!(HybridSystemLevelEmulationEnabled ())) {
    sbspSktId = GetSysSbspSocketIdNv ();
    //
    //  Check CPU stepping
    //
    CheckSupportedCpu(Host);

    //
    // Save the RC version to scratchpad
    //
    // Read first to preserve other data bits in "REVISION" CSR
    //
    ScratchPad01.Data = UsraCsrRead (sbspSktId, 0, SR_RC_REVISION_CSR);
    ScratchPad12.Bits.RcVersionMajor       = RcVersion->Major;
    ScratchPad12.Bits.RcVersionMinor       = RcVersion->Minor;
    ScratchPad01.Bits.RcVersionRevision    = RcVersion->Revision;
    ScratchPad12.Bits.RcVersionBuildNumber = RcVersion->BuildNumber;
    UsraCsrWrite (sbspSktId, 0, SR_RC_REVISION_CSR, ScratchPad01.Data);
    UsraCsrWrite (sbspSktId, 0, SR_RC_VERSION_CSR,  ScratchPad12.Data);
  }

  RcDebugPrint (
    SDBG_DEFAULT,
    "\nRC Version: %d.%d.%d.%04x\n",
    RcVersion->Major,
    RcVersion->Minor,
    RcVersion->Revision,
    RcVersion->BuildNumber
    );
}


/**
     Check if unsupported CPU/steppings are installed by these rules:
     - BIOS supports current CPU stepping N and one stepping older (N-1)
     - BIOS warns if CPU stepping is greater (newer) than N
     - BIOS halts if CPU steppings is lower (older) than N-1

  @param Host - pointer to sysHost structure on stack

**/
STATIC
VOID
CheckSupportedCpu (
  IN PSYSHOST  Host
  )
{
  UINT32   CpuFamily = 0;
  UINT8    CpuStepping = 0;
  BOOLEAN  Unsupported = TRUE;
  UINT32   RegEax=0;
  ICX_MSR_PLATFORM_INFO_REGISTER PlatformInfoDataMsrIcx;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT32)((RegEax >> 4) & 0xFFFF);
  CpuStepping = (UINT8)(RegEax & 0xF);

  //
  // Check SamplePart bit in MSR_PLATFORM_INFO
  // Client BIOS should *not* boot with a sample part if PCD set
  //
  if (FeaturePcdGet (PcdEnableSamplePartCheck) == TRUE) {
    if (IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision(CPU_ICXD,REV_ALL)  || IsCpuAndRevision(CPU_ICXSP,REV_ALL) || IsCpuAndRevision(CPU_TNR,REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      PlatformInfoDataMsrIcx.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
      if (PlatformInfoDataMsrIcx.Bits.SamplePart == 1) {
        RcDebugPrint (SDBG_DEFAULT, "Error: Non-production CPU detected. System halted.\n");
        HaltCpuLocal();
      }
    }
  }

  if (IsCpuAndRevision (CPU_ICXSP, REV_R0) || IsCpuAndRevision (CPU_ICXD, REV_A0) || IsCpuAndRevision (CPU_SNR, REV_A0) ||
    IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    //
    // Normal case - BIOS supports current stepping N and older stepping N-1
    //
    Unsupported = FALSE;
    RcDebugPrint (SDBG_DEFAULT, "CPU Stepping %2x detected\n", CpuStepping);
  } else if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_R0) || IsCpuAndRevisionOrLater (CPU_ICXD, REV_A0) || IsCpuAndRevisionOrLater (CPU_SNR, REV_A0) ||
    IsCpuAndRevisionOrLater (CPU_TNR, REV_A0) || IsCpuAndRevisionOrLater (CPU_SPRSP, REV_A0)) {
    //
    // Abnormal case -  Warn for newer stepping N+1
    //
    Unsupported = FALSE;
    RcDebugPrint (SDBG_DEFAULT, "Warning: Newer CPU Stepping %2x detected\n", CpuStepping);
  }

  if (Unsupported) {
    //
    // Error case - Unsupported CPU family and/or  stepping. Halt system
    //
    RcDebugPrint (SDBG_DEFAULT, "Error: Unsupported CPU Family 0x%x and/or Stepping %2x detected. System halted.\n", CpuFamily, CpuStepping);
    HaltCpuLocal();
  }
}

/**
  Initialize Memory Subsystem map

  @param[in]      Host          Pointer to sysHost structure
  @param[in]      Socket        Processor socket
  @param[in out]  NumMemSs      Number of entries in MemSsList
  @param[in out]  MemSsList     Array of MemSs info

  @retval MRC_STATUS_SUCCESS    Array of MemSs info and NumMemSs is updated
  @retval MRC_STATUS_FAILURE    Processor type not found
**/
MRC_STATUS
GetMemSubsystemMap (
  IN      PSYSHOST      Host,
  IN      UINT32        Socket,
  IN OUT  UINT32        *NumMemSs,
  IN OUT  MRC_MSM       *MemSsList
  )
{
  UINT32              MemSs;
  MRC_MSM             *MsmListPtr;
  UINT32              MsmListSize;
  UINT32              NumDataCh;
  UINT32              NumDdrMsm;
  UINT32              NumHbmMsm;
  UINT32              MaxDdrMsm;
  UINT32              MaxHbmMsm;
  CPU_CSR_ACCESS_VAR  *CpuCsrAccessVar;

  NumDataCh = 0;
  NumDdrMsm = 0;
  NumHbmMsm = 0;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    MaxDdrMsm = 1;      // TODO: Use dfx setup options
    MaxHbmMsm = 1;
  } else {
    MaxDdrMsm = MAX_IMC;
    MaxHbmMsm = MAX_INT32;
  }

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  //
  // Fill in buffer with Memory Subsystem Info
  //
  if (IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision(CPU_ICXD,REV_ALL)  || IsCpuAndRevision(CPU_ICXSP,REV_ALL) || IsCpuAndRevision(CPU_TNR,REV_ALL)) {

    if (IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision(CPU_TNR, REV_ALL)) {

      //
      // SNR and TNR get the Snr DDR4 table
      //

      CpuCsrAccessVar->MemSsType[Socket] = Ddr4MemSs;
      MsmListPtr = MsmTableSnr;
      MsmListSize = sizeof (MsmTableSnr) / sizeof (MRC_MSM);

    } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {

      //
      // ICXD has a separate DDR4 table
      //

      CpuCsrAccessVar->MemSsType[Socket] = Ddr4MemSs;
      MsmListPtr = MsmTableIcxDe;
      MsmListSize = sizeof (MsmTableIcxDe) / sizeof (MRC_MSM);

    } else {

      //
      // ICX has a separate DDR4 table
      //

      CpuCsrAccessVar->MemSsType[Socket] = Ddr4MemSs;
      MsmListPtr = MsmTableIcx;
      MsmListSize = sizeof (MsmTableIcx) / sizeof (MRC_MSM);
    }

  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {

    //
    // SPR and DNT are always DDR5
    //

    CpuCsrAccessVar->MemSsType[Socket] = Ddr5MemSs;

    //
    // DNT has a separate table
    //

    if (IsDntPresent (Host, Socket)) {

      MsmListPtr  = MsmTableDntDdr5;
      MsmListSize = sizeof (MsmTableDntDdr5) / sizeof (MRC_MSM);

    } else {

    //
    // And SPR has a separate table
    //

      MsmListPtr  = MsmTableSprDdr5;
      MsmListSize = sizeof (MsmTableSprDdr5) / sizeof (MRC_MSM);

    } // IsDntPresent

  //
  // GNR and SRF are always DDR5
  //

  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {

    if (IsDdr5MemSsPresent (Host, Socket)) {

      CpuCsrAccessVar->MemSsType[Socket] = Ddr5MemSs;
      MsmListPtr = MsmTableGnrDdr5;
      MsmListSize = sizeof (MsmTableGnrDdr5) / sizeof (MRC_MSM);
    }

  } else {
    return MRC_STATUS_FAILURE;
  }

  //
  // Copy MemSs parameters
  //
  RC_FATAL_ERROR (*NumMemSs >= MsmListSize, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
  for (MemSs = 0; MemSs < MsmListSize; MemSs++) {
    MemSsList[MemSs] = MsmTableReserved10nm[0];
    //
    // Verify DDR legacy structure sizes are large enough
    //
    if (IsMemSsDdrType (&MsmListPtr[MemSs])) {

      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDataCh <= MAX_MC_CH, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
#ifdef DDR5_SUPPORT
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDataIoFubsPerCh <= MAX_STROBE_DDR5, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDataIoFubsPerSubCh <= (MAX_STROBE_DDR5 / 2), ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDqLanesPerCh <= MAX_BITS_DDR5, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
#else
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDataIoFubsPerCh <= MAX_STROBE, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDataIoFubsPerSubCh <= MAX_STROBE, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
      RC_FATAL_ERROR (MsmListPtr[MemSs].NumDqLanesPerCh <= MAX_BITS, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
#endif //DDR5_SUPPORT
      if (NumDdrMsm < MaxDdrMsm) {
        MemSsList[MemSs] = MsmListPtr[MemSs];
        NumDataCh = MsmListPtr[MemSs].NumDataCh;
        NumDdrMsm++;
      }
    }
    //
    // Verify HBM legacy structure sizes are large enough...
    //
    if ((MsmListPtr[MemSs].MemSsType == Hbm2MemSs)) {
      if (NumHbmMsm < MaxHbmMsm) {
        MemSsList[MemSs] = MsmListPtr[MemSs];
        NumHbmMsm++;
      }
    }
  }

  //
  // First time through, update CpuCsrAccessVar variables for DDR topology
  //
  if (Socket == 0) {
    CpuCsrAccessVar->DdrNumChPerMc   = (UINT8) NumDataCh;
    CpuCsrAccessVar->DdrMaxImc       = (UINT8) NumDdrMsm;
    CpuCsrAccessVar->DdrMaxCh        = (UINT8) (NumDdrMsm * NumDataCh);
  }

  //
  // Update the IMC number for SPR.
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) && SocketPresent (Socket)) {
    UpdateImcNumber (Host, (UINT8) Socket);
  }

  *NumMemSs = MsmListSize;
  RC_FATAL_ERROR (CpuCsrAccessVar->DdrMaxImc <= MAX_IMC, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);

  return MRC_STATUS_SUCCESS;
} // GetMemSubsystemMap

/**
  Initialize CPU ID related fields in sysHost.
  If the processor detected is supported by this driver, initialize
  the function pointer table and return TRUE.

  @param  Host    - Pointer to sysHost structure

  @retval   TRUE  - the detected processor is supported
            FALSE - the detected processor is not supported
**/
BOOLEAN
ProcessorSupported (
  IN PSYSHOST  Host
  )
{
  InitCpuFamilyTypeStepping (Host);

  if (!(IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision(CPU_ICXD,REV_ALL)  || IsCpuAndRevision(CPU_ICXSP,REV_ALL) || IsCpuAndRevision(CPU_TNR,REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL) || IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL))) {
    return FALSE;
  }

  return TRUE;
}

VOID
EarlyUncoreInit (
  PSYSHOST    Host
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;


  UINT8                   sbspSktId;
  BIOS_SCRATCHPAD1_STRUCT Sp1;

  if (PcdGetBool (PcdMrcBeginEndDebugHooks) == TRUE) {
    OutPort8 (0x98, 0xde);
  }


  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  //
  // Invoke KTI initialization (KTIRC)
  //
  RcDebugPrint (SDBG_DEFAULT, "KTI Init starting...");
  KtiInit ();

  if (!(KtiHostOutPtr->IioEarlyLinkInitDone)) {
    RcDebugPrint (SDBG_DEFAULT, "\nIIO EarlyLink Init Start.\n");
    IioEarlyLinkInit();
    RcDebugPrint (SDBG_DEFAULT, "IIO EarlyLink Init completed! Reset Requested: %x\n", GetSysResetRequired());
  }

  //
  // If reset pending, clr this bit so SetBIOSInitDone() will not set BIOS_REST_CPL_1 early
  //
  sbspSktId = GetSysSbspSocketIdNv ();
  if (GetSysResetRequired() != 0) {
    Sp1.Data = UsraCsrRead (sbspSktId, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);

    Sp1.Bits.ResetAllowed = 0;
    UsraCsrWrite (sbspSktId, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, Sp1.Data);

  }

}  // EarlyUncoreInit

VOID
EarlyCpuInit (
  PSYSHOST    Host
  )
{
  EFI_STATUS      Status;
  DWR_PPI         *dwr;

  //
  // CPU feature early config (Flex Ratio, Desired Cores, SMT etc. that needs a reset to take effect)
  //
  RcDebugPrint (SDBG_DEFAULT, "CPU Feature Early Config starting...\n");
  CpuInit();
  RcDebugPrint (SDBG_DEFAULT, "CPU Feature Early Config completed! Reset Requested: %x\n", GetSysResetRequired());

  if (PmcIsDwrBootMode() || ((GetEmulation() == 0) && (GetSysResetRequired() == 0))) {
    //
    // Collect previous boot errors if any
    //
#ifdef IA32
    CollectPrevBootFatalErrors (Host);
#endif // IA32
  }

  Status = PeiServicesLocatePpi(&gDirtyWarmResetGuid, 0, NULL, &dwr);
  if (!EFI_ERROR(Status)) {
    RcDebugPrint (SDBG_DEFAULT, "DWR: DWR flow detected\n");
    dwr->ExecuteDirtyWarmResetFlow();
  }
}  // EarlyCpuInit

VOID
LateUncoreInit (
  PSYSHOST    Host
  )
{
  UncoreEnableMeshMode ();

  if (!(PcdGetBool (PcdEmuBiosSkipS3MAccess))) {
    //
    // Only runs S3M softstrap provision flow if the system is
    // running Warm-Reset Elimination flow
    //
    S3mCpuStrapProvision ();

    S3mCfrPatchProvision (P_U_CODE_PATCH);
    S3mCfrPatchProvision (S3M_FW_PATCH);
  }

  if (GetSysResetRequired () == 0) {
    IioEarlyPostLinkInit ();
  }

}  // LateUncoreInit

VOID
FixupPamAttributes(
  PSYSHOST    Host
  )
{
  UINT8                                   Socket;

  //
  // A platform PEIM should enable R/W access to E/F segment in the S3 boot path
  // otherwise, this AP wakeup buffer can't be accessed during CPU S3 operation.
  //
  if (GetSysBootMode () == S3Resume) {
    for (Socket = 0; Socket < MAX_SOCKET; ++Socket) {
      if(SocketPresent (Socket)) {
        ChaFixupPamAttributes (Socket, 0);
      }
    }
  }
}

