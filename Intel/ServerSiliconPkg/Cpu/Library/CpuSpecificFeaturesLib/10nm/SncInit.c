/** @file
  Code for SncInit feature (updates Core copies of SNC registers).

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include "CpuSpecificFeatures.h"
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/Ms2IdiLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>

typedef struct {
  UINT32 CsrOffset;
  UINT32 MsrOffset;
  UINT32 ValueMask;
} SNC_BASE_REGISTER_OFFSET;

SNC_BASE_REGISTER_OFFSET mSncBaseRegisterOffset[] = {
  {SNC_BASE_1_IIO_VTD_REG,     MSR_SNC_RANGE0_BASE,    MAX_UINT16}, // Bits 0-15
  {SNC_BASE_2_IIO_VTD_REG,     MSR_SNC_RANGE1_BASE,    MAX_UINT16}, // Bits 0-15
  {SNC_BASE_3_IIO_VTD_REG,     MSR_SNC_RANGE2_BASE,    MAX_UINT16}, // Bits 0-15
  {SNC_BASE_4_IIO_VTD_REG,     MSR_SNC_RANGE3_BASE,    MAX_UINT16}, // Bits 0-15
  {SNC_BASE_5_IIO_VTD_REG,     MSR_SNC_RANGE4_BASE,    MAX_UINT16}, // Bits 0-15
  {SNC_UPPER_BASE_IIO_VTD_REG, ICX_MSR_SNC_UPPER_BASE, 0x3FFFFFFF}  // Bits 0-29
};

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
SncGetConfigData (
  IN UINTN               NumberOfProcessors
  )
{
  VOID      *ConfigData;

  ConfigData = AllocateZeroPool (sizeof (ICX_MSR_SNC_CONFIG_REGISTER) * NumberOfProcessors);
  ASSERT (ConfigData != NULL);

  return ConfigData;
}

/**
  Detects if SncInit feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     SncInit feature is supported.
  @retval FALSE    SncInit feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
SncSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  ICX_MSR_SNC_CONFIG_REGISTER   *SncConfigMsr;

  if ((FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel))) {
    SncConfigMsr = (ICX_MSR_SNC_CONFIG_REGISTER *) ConfigData;
    ASSERT (SncConfigMsr != NULL);
    if (SncConfigMsr == NULL) {
      return FALSE;
    }
    SncConfigMsr[ProcessorNumber].Uint64 = AsmReadMsr64 (ICX_MSR_SNC_CONFIG);
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes SncInit feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the SncInit feature
                               must be enabled.
                               If FALSE, then the SncInit feature
                               must be disabled.

  @retval RETURN_SUCCESS            SncInit feature is initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
SncInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  UINT8                                SocketId;
  ICX_MSR_SNC_CONFIG_REGISTER          *SncConfigMsr;
  UINTN                                Index;
  UINT32                               SncBaseCsr;
  UINT32                               SncConfigCsr;
  UINT8                                CpuCnt;
  UINT32                               MmcfgBaseS0U1;
  UINT32                               MmcfgBaseS1U1;
  UINT32                               UcodeTrapMmcfgBase;
  BOOLEAN                              UcodeTrapSupported;

  //
  // The MSRs to program are Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {

    if (IsSiliconWorkaroundEnabled ("S1507951535") ) {
      //
      //  This WA is for 1S/2S SPR Ax stepping only
      //
      UcodeTrapSupported = ((GetSbspMicrocodeRev () & 0xFF) >= 0x13) ? TRUE : FALSE;

      //
      // Get number of socket
      //
      CpuCnt = 0;
      for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
        if (!SocketPresent (SocketId)) {
          continue;
        }
        CpuCnt++;
      }

      if (UcodeTrapSupported && (CpuCnt <= 2)) {
        //
        // Get Socket 0 and Socket 1 Ubox1 mmcfg base
        //
        MmcfgBaseS0U1 = UsraGetCsrRegisterAddress(0, 0, VID_UNICAST_GROUP0_CHA_MISC_REG) & 0xFFF00000;
        UcodeTrapMmcfgBase = (MmcfgBaseS0U1 >> 20) & 0x00000FFF;
        if (CpuCnt == 2) {
          MmcfgBaseS1U1 = UsraGetCsrRegisterAddress (1, 0, VID_UNICAST_GROUP0_CHA_MISC_REG) & 0xFFF00000;
          UcodeTrapMmcfgBase |= ((MmcfgBaseS1U1 >> 4) & 0x0FFF0000);
        }
        DEBUG ((DEBUG_INFO, "Socket[%x]Core[%x]: UcodeTrapMmcfgBase = 0x%08x\n",
          (UINT8)CpuInfo->ProcessorInfo.Location.Package,
          (UINT8)CpuInfo->ProcessorInfo.Location.Core,
          UcodeTrapMmcfgBase));

        //
        // Write VMSR_62H
        //
        if (!(GetEmulation() & SIMICS_FLAG)) {
          CPU_REGISTER_TABLE_WRITE64 (
            ProcessorNumber,
            Msr,
            MSR_BDF_PLA_VMSR_62H,
            (UINT64)UcodeTrapMmcfgBase
          );
        }
      }
    }

    SncConfigMsr = (ICX_MSR_SNC_CONFIG_REGISTER *) ConfigData;
    ASSERT (SncConfigMsr != NULL);
    if (SncConfigMsr == NULL) {
      return RETURN_OUT_OF_RESOURCES;
    }

    SocketId = (UINT8) CpuInfo->ProcessorInfo.Location.Package;

    DEBUG ((DEBUG_INFO, ":SNC: Socket = %d, Processor = %d\n", SocketId, ProcessorNumber));
    DEBUG ((DEBUG_INFO, "  SncConfigMsr = 0x%x\n", SncConfigMsr[ProcessorNumber].Uint32));
    if (SncConfigMsr[ProcessorNumber].Bits.SncIndicationEnable != 0) {
      //
      // If SNC is not enabled in cold-reset flow, don't program SNC Base MSRs
      //
      for (Index = 0; Index < ARRAY_SIZE (mSncBaseRegisterOffset); Index++) {
        SncBaseCsr = UsraCsrRead (SocketId, 0, mSncBaseRegisterOffset[Index].CsrOffset);
        DEBUG ((DEBUG_INFO, "  SncBaseCsr [0x%x] = 0x%x\n", Index, SncBaseCsr));
        CpuRegisterTableWrite (
          ProcessorNumber,
          Msr,
          mSncBaseRegisterOffset[Index].MsrOffset,
          mSncBaseRegisterOffset[Index].ValueMask,
          SncBaseCsr & mSncBaseRegisterOffset[Index].ValueMask
          );
      }
    }

    //
    // Update SNC Config MSR
    //
    SncConfigCsr = Ms2IdiGetSncCfg (SocketId);
    DEBUG ((DEBUG_INFO, "  SncConfigCsr = 0x%x\n", SncConfigCsr));
    SncConfigMsr[ProcessorNumber].Uint32 = SncConfigCsr;
    //
    // SncEnable bit will always be set to 0 in the core copy of SNC config register
    //
    SncConfigMsr[ProcessorNumber].Bits.SncEnable = 0;

    //
    // num_clusters definition in Core/CBPMA: 2'b00 2 Cluster, 2'b01 3 Clusters, 2'b10 4 Clusters, 2'b11 RSVD
    // num_clusters definition in MS2IDI/IIO/UPI/CHA: 2'b00 1 Cluster, 2'b01 2 Clusters, 2'b10 RSVD, 2'b11 4 Clusters
    //
    if (SncConfigMsr[ProcessorNumber].Bits.NumberOfClusters != 0) {
      SncConfigMsr[ProcessorNumber].Bits.NumberOfClusters--;
    }

    CPU_REGISTER_TABLE_WRITE64 (
      ProcessorNumber,
      Msr,
      ICX_MSR_SNC_CONFIG,
      SncConfigMsr[ProcessorNumber].Uint64
      );
  }

  return RETURN_SUCCESS;
}
