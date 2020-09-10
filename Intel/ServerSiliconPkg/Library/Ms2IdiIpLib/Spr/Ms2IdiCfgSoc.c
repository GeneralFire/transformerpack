/** @file
  MS2IDI IP SPR specific implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/Ms2IdiLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/CpuAndRevisionLib.h>

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature and program the required registers

  @param[in] SocId            - Socket ID
  @param[in] DdrtQosMode      - DDRT QoS Mode

  @retval         - None

**/
VOID
EFIAPI
Ms2IdiSetEnDdrtQoSChip (
  IN UINT8  SocId,
  IN UINT8  DdrtQosMode
  )
{
  QOS_2LM_CONFIG3_MS2IDI_MAIN_STRUCT   Qos2LmConfig3;
  QOS_2LM_CONFIG4_0_MS2IDI_MAIN_STRUCT Qos2LmConfig4_0;
  QOS_2LM_CONFIG4_1_MS2IDI_MAIN_STRUCT Qos2LmConfig4_1;

  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    Qos2LmConfig3.Data   = UsraCsrRead (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG3_MS2IDI_MAIN_REG);
    Qos2LmConfig4_0.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG4_0_MS2IDI_MAIN_REG);
    Qos2LmConfig4_1.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG4_1_MS2IDI_MAIN_REG);

    if (DdrtQosMode == DDRT_QOS_MODE_0) {
      Qos2LmConfig3.Bits.gbl_distress_sig_en = 0x00;
      Qos2LmConfig4_0.Bits.gbl_distress_sig_en = 0x00;
      Qos2LmConfig4_1.Bits.gbl_distress_sig_en = 0x00;
    } else {
      Qos2LmConfig3.Bits.gbl_distress_sig_en = 0x01;
      Qos2LmConfig4_0.Bits.gbl_distress_sig_en = 0x3C0F;
      Qos2LmConfig4_1.Bits.gbl_distress_sig_en = 0x3C0F;
    }

    UsraCsrWrite (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG3_MS2IDI_MAIN_REG, Qos2LmConfig3.Data);
    UsraCsrWrite (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG4_0_MS2IDI_MAIN_REG, Qos2LmConfig4_0.Data);
    UsraCsrWrite (SocId, MS2IDI_MULTICAST, QOS_2LM_CONFIG4_1_MS2IDI_MAIN_REG, Qos2LmConfig4_1.Data);
  }
}

/**
  MS2IDI:  Clear UMA_CLUSTER_CFG high part and configure

  @param[in] SocId               - Socket ID
  @param[in] UmaClusterEnable    - Enable/Disable UMA cluster
  @param[in] DefeatureXor        - Defeature Xor

  @retval                     - UMA_CLUSTER_CFG

**/
UINT32
EFIAPI
Ms2IdiSetUmaClusterCfg (
  IN UINT8 SocId,
  IN UINT8 UmaClusterEnable,
  IN UINT8 DefeatureXor
  )
{
  UMA_CLUSTER_MS2IDI_MAIN_STRUCT UmaClusterCfg;

  UmaClusterCfg.Data = (UINT16)UsraCsrRead (SocId, MS2IDI_MULTICAST, UMA_CLUSTER_MS2IDI_MAIN_REG);
  //
  // MS2IDI Cluster ID Select Summary, UMA enabled will program UMA_MODE to 3
  // UMA_MODE[1:0] | ClusterID[1] | ClusterID[0]
  // 00            | SNC          | SNC
  // 01            | SNC          | UMA
  // 10            | UMA          | SNC
  // 11            | UMA          | UMA
  //
  if (UmaClusterEnable == 0) {
    UmaClusterCfg.Bits.uma_cluster_enable = 0;
  } else {
    UmaClusterCfg.Bits.uma_cluster_enable = 3;
  }
  UmaClusterCfg.Bits.defeature_xor = DefeatureXor;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, UMA_CLUSTER_MS2IDI_MAIN_REG, UmaClusterCfg.Data);

  return UmaClusterCfg.Data;
}

/**
  MS2IDI:  Get UMA_CLUSTER_CFG register bitfields

  This routine returns uma_cluster_enable and defeature_xor fields to
  the caller.

  @param[in]  SocId               Socket ID
  @param[out] UmaClusterEnable    UMA clustering enable bit value
  @param[out] DefeatureXor        XOR defeature mode enabling bit value

  @retval EFI_SUCCESS             UMA Clustering Enable bit and XOR defeature mode
                                  enabling were populated with values.
  @retval EFI_INVALID_PARAMETER   User provided NULL pointer(s).

**/
EFI_STATUS
EFIAPI
Ms2IdiGetFieldsUmaClusterCfg (
  IN  UINT8    SocId,
  OUT UINT8    *UmaClusterEnable,
  OUT UINT8    *DefeatureXor
  )
{
  UMA_CLUSTER_MS2IDI_MAIN_STRUCT UmaClusterCfg;

  if (UmaClusterEnable == NULL || DefeatureXor == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  UmaClusterCfg.Data = (UINT16) UsraCsrRead (SocId, MS2IDI_MULTICAST, UMA_CLUSTER_MS2IDI_MAIN_REG);
  *UmaClusterEnable = (UINT8) UmaClusterCfg.Bits.uma_cluster_enable;
  *DefeatureXor = (UINT8) UmaClusterCfg.Bits.defeature_xor;

  return EFI_SUCCESS;
}

/**
  MS2IDI:  Set xpt memory interleave granularity

  @param[in] SocId            - Socket ID
  @param[in] XptMemInterleave - Set xpt memory interleave granularity

**/
VOID
EFIAPI
Ms2IdiSetXptMemInterleave (
  IN UINT8  SocId,
  IN UINT32 XptMemInterleave
  )
{
  XPT_MEM_INTERLEAVE_MS2IDI_MAIN_STRUCT XptMemIntCfg;

  XptMemIntCfg.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, XPT_MEM_INTERLEAVE_MS2IDI_MAIN_REG);
  XptMemIntCfg.Bits.xpt_memory_interleave_granularity = XptMemInterleave;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_MEM_INTERLEAVE_MS2IDI_MAIN_REG, XptMemIntCfg.Data);
}

/**
  MS2IDI:  Get MBE D2C disable bit according to the current value
           of D2C enable.

  @param[in] SocId                  - Socket ID
  @param[in] Direct2CoreEn          - Indicate if D2C is enabled

**/
VOID
EFIAPI
Ms2IdiSetD2C (
  IN UINT8   SocId,
  IN BOOLEAN Direct2CoreEn
  )
{
  MBE_MS2IDI_MAIN_STRUCT Mbe;

  Mbe.Data = UsraCsrRead (SocId, MS2IDI_MULTICAST, MBE_MS2IDI_MAIN_REG);

  if (Direct2CoreEn) {
    Mbe.Bits.mbe_directtocore_disable = 0;
  } else {
    Mbe.Bits.mbe_directtocore_disable = 1;
  }

  UsraCsrWrite (SocId, MS2IDI_MULTICAST, MBE_MS2IDI_MAIN_REG, Mbe.Data);
}

/**
  MS2IDI:  Config priority local prefetch for demand read

  @param SocId                  - Socket ID
  @param EnablePriorityPrefecth - Enable priority prefetch for demand read

  @retval                       - None

**/
VOID
EFIAPI
Ms2IdiConfigPriorityPrefetchLocal (
  UINT8    SocId,
  BOOLEAN  EnablePriorityPrefecth
  )
{
  XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT  XptLocalPrefetchConfig1C0MccUcc;

  XptLocalPrefetchConfig1C0MccUcc.Data = UsraCsrRead (SocId, 0, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  XptLocalPrefetchConfig1C0MccUcc.Bits.priorityprefetchfordemandread = EnablePriorityPrefecth;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG, XptLocalPrefetchConfig1C0MccUcc.Data );
}

/**
  MS2IDI:  Config priority remote prefetch for demand read

  @param SocId                  - Socket ID
  @param EnablePriorityPrefecth - Enable priority prefetch for demand read

  @retval                       - None

**/
VOID
EFIAPI
Ms2IdiConfigPriorityPrefetchRemote (
  UINT8    SocId,
  BOOLEAN  EnablePriorityPrefecth
  )
{
  XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT XptRemotePrefetchConfig1C0MccUcc;

  XptRemotePrefetchConfig1C0MccUcc.Data = UsraCsrRead (SocId, 0, XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  XptRemotePrefetchConfig1C0MccUcc.Bits.priorityprefetchfordemandread = EnablePriorityPrefecth;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG, XptRemotePrefetchConfig1C0MccUcc.Data);
}
