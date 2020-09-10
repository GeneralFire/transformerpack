/** @file

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
#include <Library/CpuAndRevisionLib.h>

UINT32 SncBaseRegisterOffset_Icx[NUM_OF_SNC_BASE] = {
  SNC_BASE_1_MS2IDI_MAIN_REG,
  SNC_BASE_2_MS2IDI_MAIN_REG,
  SNC_BASE_3_MS2IDI_MAIN_REG,
  SNC_BASE_4_MS2IDI_MAIN_REG,
  SNC_BASE_5_MS2IDI_MAIN_REG,
  SNC_UPPER_BASE_MS2IDI_MAIN_REG
};

UINT32    XptUpiDecodeTableN0[] = { XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_REG,      //UPI0
                                    XPT_UPI_DECODE_TABLE_1_N0_MS2IDI_MAIN_REG,      //UPI1
                                    XPT_UPI_DECODE_TABLE_2_N0_MS2IDI_MAIN_REG };    //UPI2

UINT32    XptUpiDecodeTableN1[] = { XPT_UPI_DECODE_TABLE_0_N1_MS2IDI_MAIN_REG,      //UPI0
                                    XPT_UPI_DECODE_TABLE_1_N1_MS2IDI_MAIN_REG,      //UPI1
                                    XPT_UPI_DECODE_TABLE_2_N1_MS2IDI_MAIN_REG };    //UPI2

UINT32   Xpt32EntryPrefetchBaseReg[NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE] = {  XPT_32_ENTRY_PREFETCH_BASE_0_MS2IDI_MAIN_REG,
                                                                                XPT_32_ENTRY_PREFETCH_BASE_1_MS2IDI_MAIN_REG,
                                                                                XPT_32_ENTRY_PREFETCH_BASE_2_MS2IDI_MAIN_REG,
                                                                                XPT_32_ENTRY_PREFETCH_BASE_3_MS2IDI_MAIN_REG };

UINT32   Xpt32EntryTableReg[NUM_OF_REGSTERS_MINISAD_32_ENTRY] = { XPT_32_ENTRY_MINISAD_TABLE_0_MS2IDI_MAIN_REG,
                                                                  XPT_32_ENTRY_MINISAD_TABLE_1_MS2IDI_MAIN_REG,
                                                                  XPT_32_ENTRY_MINISAD_TABLE_2_MS2IDI_MAIN_REG,
                                                                  XPT_32_ENTRY_MINISAD_TABLE_3_MS2IDI_MAIN_REG,
                                                                  XPT_32_ENTRY_MINISAD_TABLE_4_MS2IDI_MAIN_REG,
                                                                  XPT_32_ENTRY_MINISAD_TABLE_5_MS2IDI_MAIN_REG };

/**
  MS2IDI:  Enable xpt remote prefetch

  @param SocId           - Socket ID
  @param XptPrefDis      - Enable/Disable xptprefetchdisable

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetXptRmtPref (
  UINT8 SocId,
  UINT8 XptPrefDis
  )
{
  XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT XptRemotePrefetchConfig1;

  XptRemotePrefetchConfig1.Data = UsraCsrRead (SocId, 0, XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  XptRemotePrefetchConfig1.Bits.xptprefetchdisable = XptPrefDis;
  if (XptPrefDis == 0) {
    XptRemotePrefetchConfig1.Bits.xptprefetchsubgrp2en = 0;
    XptRemotePrefetchConfig1.Bits.xptprefetchsubgrp3en = 0;
    XptRemotePrefetchConfig1.Bits.xptprefetchsubgrp4en = 0;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG, XptRemotePrefetchConfig1.Data);
}

/**
  MS2IDI:  Get xpt prefetch disable value

  @param SocId           - Socket ID

  @retval                -xpt prefetch disable value

**/
UINT8
EFIAPI
Ms2IdiGetLocXptPrefDis (
  UINT8 SocId
  )
{
  UINT8 XptPrefDis;
  XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT XptPrefetchConfig1;

  XptPrefetchConfig1.Data = UsraCsrRead (SocId, 0, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  XptPrefDis = (UINT8)XptPrefetchConfig1.Bits.xptprefetchdisable;

  return XptPrefDis;
}

/**
  MS2IDI:  Get xpt prefetch disable value

  @param SocId           - Socket ID
  @param XptPrefDis      - Enable/Disable xptprefetchdisable

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetLocXptPrefDis (
  UINT8 SocId,
  UINT8 XptPrefDis
  )
{

  XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT XptPrefetchConfig1;

  XptPrefetchConfig1.Data = UsraCsrRead (SocId, 0, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  XptPrefetchConfig1.Bits.xptprefetchdisable = XptPrefDis;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG, XptPrefetchConfig1.Data);
}

/**
  MS2IDI:  Config xpt remote prefetch

  @param SocId                - Socket ID
  @param SysConfig            - System configuration
  @param XptMiniSadModeSelect - Selects Mode to index the Minisad

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiXptRmtPrefCfg (
  UINT8 SocId,
  UINT8 SysConfig,
  UINT8 XptMiniSadModeSelect
  )
{
  XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_STRUCT XptRemotePrefetchConfig2;
  UINT32                                         UpiXorEn;
  UINT32                                         Upi2or3Link;

  if (SysConfig == SYS_CONFIG_2S2L) {
    UpiXorEn    = 1;
    Upi2or3Link = 1;
  } else if (SysConfig == SYS_CONFIG_2S3L) {
    UpiXorEn    = 1;
    Upi2or3Link = 0;
  } else {
    UpiXorEn    = 0;
    Upi2or3Link = 0;
  }

  XptRemotePrefetchConfig2.Data = UsraCsrRead (SocId, 0, XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_REG);
  XptRemotePrefetchConfig2.Bits.upi_xor_en            = UpiXorEn;
  XptRemotePrefetchConfig2.Bits.upi_2or3_link         = Upi2or3Link;
  XptRemotePrefetchConfig2.Bits.xptminisadmodeselect  = XptMiniSadModeSelect;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_REG, XptRemotePrefetchConfig2.Data);
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

}

/**
  MS2IDI:  Set XptUpiDecodeTable

  @param SocId   - Socket ID
  @param UpiId   - Upi Id table
  @param TabIdx  - Table index

  @retval        - None

**/
VOID
EFIAPI
Ms2IdiSetDecUpiId (
  UINT8 SocId,
  UINT8 UpiId,
  UINT8 TabIdx
  )
{
  XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_STRUCT  XptUpiDecodeTable0;
  if (TabIdx >= (sizeof (XptUpiDecodeTableN0) / sizeof (UINT32))) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }
  XptUpiDecodeTable0.Data        = 0;
  XptUpiDecodeTable0.Bits.upi_id = UpiId;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XptUpiDecodeTableN0[TabIdx], XptUpiDecodeTable0.Data);
}

/**
  MS2IDI:  Clear XptUpiDecodeTable

  @param SocId      - Socket ID
  @param UpiLinkId  - Table index

  @retval        - None

**/
VOID
EFIAPI
Ms2IdiClearXptUpiDecTab (
  UINT8 SocId,
  UINT8 UpiLinkId
  )
{
  if (UpiLinkId >= (sizeof (XptUpiDecodeTableN0) / sizeof (UINT32))) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XptUpiDecodeTableN0[UpiLinkId], 0x0);
}

/**
  MS2IDI:  Program XptUpiDecodeTable

  @param SocId             - Socket ID
  @param DecodeTableIdx    - Table index
  @param RemSktId          - Remote Socket ID
  @param UpiId             - Upi Id table
  @param MemBase           - Memory base
  @param MemLimit          - Memory limit

  @retval        - None

**/
VOID
EFIAPI
Ms2IdiProgramXptUpiDecodeTable(
  IN UINT8                  SocId,
  IN UINT8                  DecodeTableIdx,
  IN UINT8                  RemSktId,
  IN UINT8                  UpiId,
  IN UINT32                 MemBase,
  IN UINT32                 MemLimit
  )
{
  XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_STRUCT   XptUpiDecodeTable0;
  XPT_UPI_DECODE_TABLE_0_N1_MS2IDI_MAIN_STRUCT   XptUpiDecodeTable1;

  if ((DecodeTableIdx >= (sizeof (XptUpiDecodeTableN0) / sizeof (UINT32))) ||
    (DecodeTableIdx >= (sizeof (XptUpiDecodeTableN1) / sizeof (UINT32)))) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }

  XptUpiDecodeTable0.Data = 0;
  XptUpiDecodeTable1.Data = 0;

  XptUpiDecodeTable0.Bits.generate_rmt_prefetch = 1;                //Eanbled
  XptUpiDecodeTable0.Bits.upi_id                = UpiId;            //upi link
  XptUpiDecodeTable0.Bits.remote_socket_limit   = MemLimit;
  XptUpiDecodeTable0.Bits.dnode_id              = (RemSktId & 0x3); //remote skt id [1:0]
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XptUpiDecodeTableN0[DecodeTableIdx], XptUpiDecodeTable0.Data);

  XptUpiDecodeTable1.Bits.dnode_id           = (RemSktId >> 2);  //remote skt id [2]
  XptUpiDecodeTable1.Bits.remote_socket_base = MemBase;          //Remote socket cluster 0 base
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XptUpiDecodeTableN1[DecodeTableIdx], XptUpiDecodeTable1.Data);
}

/**
  MS2IDI:  Set Snc Base Address

  @param SocId        - Socket ID
  @param SncBaseAddr  - Snc Base Address
  @param SncData      - SNC_BASE register data

  @retval             - None

**/
UINT32
EFIAPI
Ms2IdiUpdateDataSncBaseAddr (
  UINT8   SocId,
  UINT16  SncBaseAddr,
  UINT32  SncData
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase;

  SncBase.Data      = SncData;
  SncBase.Bits.addr = SncBaseAddr;

  return SncBase.Data;
}

/**
  MS2IDI:  Set Snc Base Address

  @param SocId        - Socket ID
  @param SncIdx       - SNC register index

  @retval             - None

**/
UINT32
EFIAPI
Ms2IdiGetSncBaseAddr (
  UINT8  SocId,
  UINT8  SncIdx
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase;

  if (SncIdx >= NUM_OF_SNC_BASE) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return MS2IDI_INVALID_VALUE;
  }

  SncBase.Data = UsraCsrRead (SocId, 0, SncBaseRegisterOffset_Icx[SncIdx]);

  return SncBase.Bits.addr;
}

/**
  MS2IDI:  Set Snc Base Address

  @param SocId        - Socket ID
  @param SncIdx       - SNC register index
  @param SncBaseAddr  - Snc Base Address

  @retval             - None

**/
VOID
EFIAPI
Ms2IdiSetSncBaseAddr (
  UINT8  SocId,
  UINT8  SncIdx,
  UINT16 SncBaseAddr
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase;

  if (SncIdx >= NUM_OF_SNC_BASE) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }

  SncBase.Data = UsraCsrRead (SocId, 0, SncBaseRegisterOffset_Icx[SncIdx]);
  SncBase.Bits.addr = SncBaseAddr;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SncBaseRegisterOffset_Icx[SncIdx], SncBase.Data);
}

/**
  MS2IDI:  Set SNC_CONFIG cluster configuration.

  @param SocId          - Socket ID
  @param FullSncEnable  - Number of channels per cluster
  @param SncIndEnable   - Base of channels for cluster1
  @param NumClusters    - Base of channels for cluster2

  @retval               - SNC_CONFIG

**/
UINT32
EFIAPI
Ms2IdiSetSncCfg (
  UINT8 SocId,
  UINT8 FullSncEnable,
  UINT8 SncIndEnable,
  UINT8 NumClusters
  )
{
  SNC_CONFIG_MS2IDI_MAIN_STRUCT  SncConfigMs2Idi;

  SncConfigMs2Idi.Data = 0;
  SncConfigMs2Idi.Bits.full_snc_enable = FullSncEnable;
  SncConfigMs2Idi.Bits.snc_ind_enable  = SncIndEnable;
  SncConfigMs2Idi.Bits.num_clusters    = NumClusters;

  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_CONFIG_MS2IDI_MAIN_REG, SncConfigMs2Idi.Data);

  return SncConfigMs2Idi.Data;
}

/**
  MS2IDI:  Update SNC_CONFIG cluster configuration.

  @param SocId          - Socket ID
  @param FullSncEnable  - Number of channels per cluster
  @param SncIndEnable   - Base of channels for cluster1
  @param NumClusters    - Base of channels for cluster2

  @retval               - NONE

**/
VOID
EFIAPI
Ms2IdiUpdateSncCfg (
  UINT8 SocId,
  UINT8 FullSncEnable,
  UINT8 SncIndEnable,
  UINT8 NumClusters
  )
{
  SNC_CONFIG_MS2IDI_MAIN_STRUCT  SncConfigMs2Idi;

  SncConfigMs2Idi.Data = UsraCsrRead (SocId, 0, SNC_CONFIG_MS2IDI_MAIN_REG);

  SncConfigMs2Idi.Bits.full_snc_enable = FullSncEnable;
  SncConfigMs2Idi.Bits.snc_ind_enable  = SncIndEnable;
  SncConfigMs2Idi.Bits.num_clusters    = NumClusters;

  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_CONFIG_MS2IDI_MAIN_REG, SncConfigMs2Idi.Data);

}

/**
  MS2IDI:  Get SNC_CONFIG

  @param SocId           - Socket ID

  @retval                - SNC_CONFIG

**/
UINT32
EFIAPI
Ms2IdiGetSncCfg (
  UINT8 SocId
  )
{
  UINT32  SncCfg;
  SncCfg = UsraCsrRead (SocId, 0, SNC_CONFIG_MS2IDI_MAIN_REG);
  return SncCfg;
}

/**
  MS2IDI:  Get SNC_UPPER_BASE

  @param SocId           - Socket ID

  @retval                - SNC_UPPER_BASE

**/
UINT32
EFIAPI
Ms2IdiGetSncUpperBase (
  UINT8 SocId
  )
{
  UINT32  SncUpperBase;
  SncUpperBase = UsraCsrRead (SocId, 0, SNC_UPPER_BASE_MS2IDI_MAIN_REG);
  return SncUpperBase;
}

/**
  MS2IDI:  Set SNC_UPPER_BASE

  @param SocId           - Socket ID
  @param SncUpperBase    - SNC UPPER BASE

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSncUpperBase (
  UINT8  SocId,
  UINT32 SncUpperBase
  )
{
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_UPPER_BASE_MS2IDI_MAIN_REG, SncUpperBase);
}

/**
  MS2IDI:  Set Snc Base

  @param SocId        - Socket ID
  @param SncIdx       - SNC register index

  @retval             - None

**/
UINT32
EFIAPI
Ms2IdiGetSncBase (
  UINT8  SocId,
  UINT8  SncIdx
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase;

  if (SncIdx >= NUM_OF_SNC_BASE) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return MS2IDI_INVALID_VALUE;
  }

  SncBase.Data = UsraCsrRead (SocId, 0, SncBaseRegisterOffset_Icx[SncIdx]);

  return SncBase.Data;
}

/**
  MS2IDI:  Clear UMA_CLUSTER_CFG high part and configure

  @param SocId                - Socket ID
  @param UmaClusterEnable    - Enable/Disable UMA cluster
  @param DefeatureXor        - Defeature Xor

  @retval                     - UMA_CLUSTER_CFG

**/
UINT32
EFIAPI
Ms2IdiSetUmaClusterCfg (
  UINT8 SocId,
  UINT8 UmaClusterEnable,
  UINT8 DefeatureXor
  )
{
  UMA_CLUSTER_CFG_MS2IDI_MAIN_STRUCT UmaClusterCfg;

  UmaClusterCfg.Data = (UINT16)UsraCsrRead (SocId, 0, UMA_CLUSTER_CFG_MS2IDI_MAIN_REG);
  UmaClusterCfg.Bits.uma_cluster_enable = UmaClusterEnable;
  UmaClusterCfg.Bits.defeature_xor = DefeatureXor;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, UMA_CLUSTER_CFG_MS2IDI_MAIN_REG, UmaClusterCfg.Data);

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
  UMA_CLUSTER_CFG_MS2IDI_MAIN_STRUCT UmaClusterCfg;

  if (UmaClusterEnable == NULL || DefeatureXor == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  UmaClusterCfg.Data = (UINT16) UsraCsrRead (SocId, 0, UMA_CLUSTER_CFG_MS2IDI_MAIN_REG);
  *UmaClusterEnable = (UINT8) UmaClusterCfg.Bits.uma_cluster_enable;
  *DefeatureXor = (UINT8) UmaClusterCfg.Bits.defeature_xor;

  return EFI_SUCCESS;
}

/**
  MS2IDI:  Set SNC_CONFIG cluster configuration.

  @param SocId          - Socket ID
  @param FullSncEnable  - Number of channels per cluster
  @param SncIndEnable   - Base of channels for cluster1

  @retval               - None

**/
VOID
EFIAPI
Ms2IdiSetEnSncCfg (
  UINT8 SocId,
  UINT8 FullSncEnable,
  UINT8 SncIndEnable
  )
{
  SNC_CONFIG_MS2IDI_MAIN_STRUCT   SncConfigMs2Idi;

  SncConfigMs2Idi.Data = UsraCsrRead (SocId, 0, SNC_CONFIG_MS2IDI_MAIN_REG);
  SncConfigMs2Idi.Bits.full_snc_enable = FullSncEnable;
  SncConfigMs2Idi.Bits.snc_ind_enable  = SncIndEnable;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_CONFIG_MS2IDI_MAIN_REG, SncConfigMs2Idi.Data);
}

/**
  MS2IDI:  Get SNC_CONFIG cluster configuration fields.

  @param SocId          - Socket ID
  @param FullSncEnable  - Number of channels per cluster
  @param SncIndEnable   - Base of channels for cluster1
  @param NumClusters    - Base of channels for cluster2

  @retval               - SNC configuration in MS2IDI

**/
UINT32
EFIAPI
Ms2IdiGetFieldsSncCfg (
  UINT8 SocId,
  UINT8 *FullSncEnable,
  UINT8 *SncIndEnable,
  UINT8 *NumClusters
  )
{
  SNC_CONFIG_MS2IDI_MAIN_STRUCT  SncConfig;

  if (FullSncEnable == NULL || SncIndEnable == NULL || NumClusters == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return 0;
  }

  SncConfig.Data = UsraCsrRead (SocId, 0, SNC_CONFIG_MS2IDI_MAIN_REG);

  *FullSncEnable = (UINT8)SncConfig.Bits.full_snc_enable;
  *SncIndEnable  = (UINT8)SncConfig.Bits.snc_ind_enable;
  *NumClusters   = (UINT8)SncConfig.Bits.num_clusters;

  return SncConfig.Data;
}

/**
  MS2IDI:  Get Xpt 32 Entry Prefetch Base Register

  @param SocId        - Socket ID
  @param EntryIdx     - Register index

  @retval             - Xpt 32 Entry Prefetch Base Register

**/
UINT32
EFIAPI
Ms2IdiGetXptEntryPrefBase (
  UINT8  SocId,
  UINT8  EntryIdx
  )
{
  UINT32 Xpt32EntryPrefetchBase;

  if (EntryIdx >= NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return MS2IDI_INVALID_VALUE;
  }

  Xpt32EntryPrefetchBase = UsraCsrRead (SocId, 0, Xpt32EntryPrefetchBaseReg[EntryIdx]);

  return Xpt32EntryPrefetchBase;
}

/**
  MS2IDI:  Get Xpt 32 Entry Prefetch Base Register

  @param SocId        - Socket ID
  @param EntryIdx     - Register index
  @param Val          - Register value

  @retval             - None

**/
VOID
EFIAPI
Ms2IdiSetXptEntryPrefBase (
  UINT8  SocId,
  UINT8  EntryIdx,
  UINT32 Val
  )
{
  if (EntryIdx >= NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, Xpt32EntryPrefetchBaseReg[EntryIdx], Val);
}

/**
  MS2IDI:  Get Xpt 32 Entry Table Register

  @param SocId        - Socket ID
  @param EntryIdx     - Register index

  @retval             - Xpt 32 Entry Table Register

**/
UINT32
EFIAPI
Ms2IdiGetXptEntryTable (
  UINT8  SocId,
  UINT8  EntryIdx
  )
{
  UINT32 Xpt32EntryTable;
  if (EntryIdx >= NUM_OF_REGSTERS_MINISAD_32_ENTRY) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return MS2IDI_INVALID_VALUE;
  }
  Xpt32EntryTable = UsraCsrRead (SocId, 0, Xpt32EntryTableReg[EntryIdx]);

  return Xpt32EntryTable;
}

/**
  MS2IDI:  Get Xpt 32 Entry Prefetch Base Register

  @param SocId        - Socket ID
  @param EntryIdx     - Register index
  @param Val          - Register value

  @retval             - None

**/
VOID
EFIAPI
Ms2IdiSetXptEntryTable (
  UINT8  SocId,
  UINT8  EntryIdx,
  UINT32 Val
  )
{
  if (EntryIdx >= NUM_OF_REGSTERS_MINISAD_32_ENTRY) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, Xpt32EntryTableReg[EntryIdx], Val);
}

/**
  MS2IDI:  Get XPT_UPI_DECODE_TABLE_N0 and XPT_UPI_DECODE_TABLE_N1

  @param SocId                - Socket ID
  @param EntryIdx             - Register index
  @param XptUpiDecodeTableN0r - XPT Decode Table N0
  @param XptUpiDecodeTableN1r - XPT Decode Table N1

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiGetXptUpiDecTable (
  UINT8  SocId,
  UINT8  EntryIdx,
  UINT32 *XptUpiDecodeTableN0r,
  UINT32 *XptUpiDecodeTableN1r
  )
{
  if ((EntryIdx >= (sizeof (XptUpiDecodeTableN0) / sizeof (UINT32))) ||
    (EntryIdx >= (sizeof (XptUpiDecodeTableN1) / sizeof (UINT32)))) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (FALSE);
    return;
  }
  *XptUpiDecodeTableN0r = UsraCsrRead (SocId, 0, XptUpiDecodeTableN0[EntryIdx]);
  *XptUpiDecodeTableN1r = UsraCsrRead (SocId, 0, XptUpiDecodeTableN1[EntryIdx]);
}

/**
  MS2IDI:  Set SNC BASE TO LM MASKs

  @param SocId        - Socket ID
  @param TwoLmMask1   - Two Lm Mask for SNC_BASE_1
  @param TwoLmMask1H  - Two Lm Mask Hi for SNC_BASE_1
  @param TwoLmMask2   - Two Lm Mask for SNC_BASE_1

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiSetTwoLmMaskSncCfg (
  UINT8   SocId,
  UINT32  TwoLmMask1,
  UINT32  TwoLmMask1H,
  UINT32  TwoLmMask2
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase1_Ms2Idi;
  SNC_BASE_2_MS2IDI_MAIN_STRUCT SncBase2_Ms2Idi;

  SncBase1_Ms2Idi.Data = UsraCsrRead (SocId, 0, SNC_BASE_1_MS2IDI_MAIN_REG);
  SncBase1_Ms2Idi.Bits.twolm_mask    = TwoLmMask1;
  SncBase1_Ms2Idi.Bits.hi_twolm_mask = TwoLmMask1H;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_BASE_1_MS2IDI_MAIN_REG, SncBase1_Ms2Idi.Data);

  SncBase2_Ms2Idi.Data = UsraCsrRead (SocId, 0, SNC_BASE_2_MS2IDI_MAIN_REG);
  SncBase2_Ms2Idi.Bits.twolm_mask = TwoLmMask2;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, SNC_BASE_2_MS2IDI_MAIN_REG, SncBase2_Ms2Idi.Data);
}

/**
  MS2IDI:  Get SNC BASE TO LM MASKs

  @param SocId        - Socket ID
  @param TwoLmMask1   - Two Lm Mask for SNC_BASE_1
  @param TwoLmMask1H  - Two Lm Mask Hi for SNC_BASE_1
  @param TwoLmMask2   - Two Lm Mask for SNC_BASE_1

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiGetTwoLmMaskSncCfg (
  UINT8   SocId,
  UINT32  *TwoLmMask1,
  UINT32  *TwoLmMask1H,
  UINT32  *TwoLmMask2
  )
{
  SNC_BASE_1_MS2IDI_MAIN_STRUCT SncBase1_Ms2Idi;
  SNC_BASE_2_MS2IDI_MAIN_STRUCT SncBase2_Ms2Idi;

  if (TwoLmMask1 == NULL || TwoLmMask1H == NULL || TwoLmMask2 == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer detected\n"));
    ASSERT (FALSE);
    return;
  }

  SncBase1_Ms2Idi.Data = UsraCsrRead (SocId, 0, SNC_BASE_1_MS2IDI_MAIN_REG);
  *TwoLmMask1  = SncBase1_Ms2Idi.Bits.twolm_mask;
  *TwoLmMask1H = SncBase1_Ms2Idi.Bits.hi_twolm_mask;

  SncBase2_Ms2Idi.Data = UsraCsrRead (SocId, 0, SNC_BASE_2_MS2IDI_MAIN_REG);
  *TwoLmMask2 = SncBase2_Ms2Idi.Bits.twolm_mask;
}

/**
  MS2IDI:  Configure XPT Local Prefetch

  @param SocId           - Socket ID
  @param EnLocXptPref    - xpt prefetch enable value

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiEnLocXptPref (
  UINT8 SocId,
  UINT8 EnLocXptPref
  )
{
  XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT XptPrefetchConfig1;

  XptPrefetchConfig1.Data = UsraCsrRead (SocId, 0, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG);
  if (EnLocXptPref) {
    XptPrefetchConfig1.Bits.xptprefetchdisable = 0;
    //
    // s220529727 - IDI xpt_local Uncore Perf CSRs ICX-SP
    //
    XptPrefetchConfig1.Bits.xptprefetchsubgrp1dis = 0;
    XptPrefetchConfig1.Bits.xptprefetchsubgrp2en = 0;
    XptPrefetchConfig1.Bits.xptprefetchsubgrp3en = 0;
    XptPrefetchConfig1.Bits.xptprefetchsubgrp4en = 0;
    XptPrefetchConfig1.Bits.xpthitpredictthresh = 5;
    XptPrefetchConfig1.Bits.xptmispredthresh = 256;
    XptPrefetchConfig1.Bits.xptupdtlocalhitonremoteaddr = 0;
  } else {
    XptPrefetchConfig1.Bits.xptprefetchdisable = 1;
  }
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG, XptPrefetchConfig1.Data);
}

/**
  MS2IDI:  Configure XPT FIFO Enabled Credit Num

  @param SocId                - Socket ID
  @param XptFifoEnabledCredit - Xpt Fifo Enabled Creidt Num

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiXptCreditConfig (
  UINT8 SocId,
  UINT8 XptFifoEnabledCredit
  )
{
  XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_STRUCT XptFifoCreditConfig;

  XptFifoCreditConfig.Data = UsraCsrRead (SocId, 0, XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_REG);
  XptFifoCreditConfig.Bits.xpt_num_enabled_credits = XptFifoEnabledCredit;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_REG, XptFifoCreditConfig.Data);
}

/**
  MS2IDI:  Configure MiniSAD Mode

  @param SocId                - Socket ID
  @param MiniSadModeSelect    - Mini Sad Mode Selection
  @param McMiniSadMode        - Mc Mini Sad Mode Selection
  @param XorDefeature         - XOR defeature

  @retval                     - None

**/
VOID
EFIAPI
Ms2IdiSetMiniSadMode (
  UINT8 SocId,
  UINT8 MiniSadModeSelect,
  UINT8 McMiniSadMode,
  UINT8 XorDefeature
  )
{
  XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_STRUCT   XptPrefetchConfig2;

  XptPrefetchConfig2.Data = UsraCsrRead (SocId, 0, XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_REG);
  XptPrefetchConfig2.Bits.xptminisadmodeselect   = MiniSadModeSelect;
  XptPrefetchConfig2.Bits.xptmcminisadmode       = McMiniSadMode;
  XptPrefetchConfig2.Bits.xpt_defeature_xor      = XorDefeature;
  XptPrefetchConfig2.Bits.xptfeedbackwincntlimit = 0x186a0;
  XptPrefetchConfig2.Bits.xptforceprefetch       = 0;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_REG, XptPrefetchConfig2.Data);
}

/**
  MS2IDI:  Set xpt memory interleave granularity

  @param SocId            - Socket ID
  @param XptMemInterleave - Set xpt memory interleave granularity

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetXptMemInterleave (
  UINT8  SocId,
  UINT32 XptMemInterleave
  )
{
  XPT_MEM_INTERLEAVE_CFG_MS2IDI_MAIN_STRUCT XptMemIntCfg;

  XptMemIntCfg.Data = UsraCsrRead (SocId, 0, XPT_MEM_INTERLEAVE_CFG_MS2IDI_MAIN_REG);
  XptMemIntCfg.Bits.xpt_memory_interleave_granularity = XptMemInterleave;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, XPT_MEM_INTERLEAVE_CFG_MS2IDI_MAIN_REG, XptMemIntCfg.Data);
}

/**
  MS2IDI:  Clear high part of UNCORE_SNC_CONFIG and
           set num_cha_per_cluster.

  @param SocId    - Socket ID
  @param TotCha   - Total number of channels

  @retval         - UNCORE_SNC_CONFIG

**/
UINT32
EFIAPI
Ms2IdiSetNumChaSncCfg (
  UINT8 SocId,
  UINT8 TotCha
  )
{
  UNCORE_SNC_CONFIG_MS2IDI_MAIN_STRUCT  Ms2IdiUncoreSncCfg;

  Ms2IdiUncoreSncCfg.Data = UsraCsrRead (SocId, 0, UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG);
  Ms2IdiUncoreSncCfg.Bits.num_cha_per_cluster = TotCha;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG, Ms2IdiUncoreSncCfg.Data);

  return Ms2IdiUncoreSncCfg.Data;
}

/**
  MS2IDI:  Set UNCORE_SNC_CONFIG cluster configuration.

  @param SocId                  - Socket ID
  @param NumChaCluster          - Number of channels per cluster
  @param BaseChaCluster1        - Base of channels for cluster1
  @param BaseChaCluster2        - Base of channels for cluster2
  @param BaseChaCluster3        - Base of channels for cluster3

  @retval         - UNCORE_SNC_CONFIG

**/
UINT32
EFIAPI
Ms2IdiSetClustersSncCfg (
  UINT8 SocId,
  UINT8 NumChaCluster,
  UINT8 BaseChaCluster1,
  UINT8 BaseChaCluster2,
  UINT8 BaseChaCluster3
  )
{
  UNCORE_SNC_CONFIG_MS2IDI_MAIN_STRUCT  UncoreSncConfig;

  UncoreSncConfig.Data = 0;
  UncoreSncConfig.Bits.num_cha_per_cluster = NumChaCluster;
  UncoreSncConfig.Bits.base_cha_cluster_1  = BaseChaCluster1;
  UncoreSncConfig.Bits.base_cha_cluster_2  = BaseChaCluster2;
  UncoreSncConfig.Bits.base_cha_cluster_3  = BaseChaCluster3;
  UsraCsrWrite (SocId, MS2IDI_MULTICAST, UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG, UncoreSncConfig.Data);

  return UncoreSncConfig.Data;
}

/**
  MS2IDI:  Get UNCORE_SNC_CONFIG.

  @param SocId                  - Socket ID
  @param NumChaCluster          - Number of channels per cluster
  @param BaseChaCluster1        - Base of channels for cluster1
  @param BaseChaCluster2        - Base of channels for cluster2
  @param BaseChaCluster3        - Base of channels for cluster3

  @retval                       - Uncore SNC configuration in MS2IDI

**/
UINT32
EFIAPI
Ms2IdiGetUncoreSncCfg (
  IN   UINT8 SocId,
  OUT  UINT8 *NumChaCluster,
  OUT  UINT8 *BaseChaCluster1,
  OUT  UINT8 *BaseChaCluster2,
  OUT  UINT8 *BaseChaCluster3
  )
{
  UNCORE_SNC_CONFIG_MS2IDI_MAIN_STRUCT  UncoreSncConfig;

  UncoreSncConfig.Data = UsraCsrRead (SocId, 0, UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG);

  *NumChaCluster = (UINT8) UncoreSncConfig.Bits.num_cha_per_cluster;
  *BaseChaCluster1 = (UINT8) UncoreSncConfig.Bits.base_cha_cluster_1;
  *BaseChaCluster2 = (UINT8) UncoreSncConfig.Bits.base_cha_cluster_2;
  *BaseChaCluster3 = (UINT8) UncoreSncConfig.Bits.base_cha_cluster_3;

  return UncoreSncConfig.Data;
}

/**
  MS2IDI:  Get MBE D2C disable bit according to the current value
           of D2C enable.

  @param SocId                  - Socket ID
  @param Direct2CoreEn          - Indicate if D2C is enabled

  @retval                       - None

**/
VOID
EFIAPI
Ms2IdiSetD2C (
  IN         UINT8 SocId,
  IN BOOLEAN Direct2CoreEn
  )
{
  MBE_CFG_MS2IDI_MAIN_STRUCT Mbe;

  Mbe.Data = UsraCsrRead (SocId, 0, MBE_CFG_MS2IDI_MAIN_REG);

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    Mbe.Bits.mbe_directtocore_disable = 1;
  } else if (Direct2CoreEn) {
    Mbe.Bits.mbe_directtocore_disable = 0;
  } else {
    Mbe.Bits.mbe_directtocore_disable = 1;
  }

  UsraCsrWrite (SocId, MS2IDI_MULTICAST, MBE_CFG_MS2IDI_MAIN_REG, Mbe.Data);

  return;
}

/**
  Check if SNC registers are still in hardware default setting.

  @param    SocId              - Socket ID

  @return   TRUE: Yes, SNC registers are in hardware default setting.
  @return   FALSE: No, SNC registers are not in hardware default setting.
**/
BOOLEAN
EFIAPI
IsSncRegistersInHardwareDefaultSetting (
  IN  UINT8      SocId
  )
{
  UINT8          RegFullSncEn;
  UINT8          RegSncIndEn;
  UINT8          RegNumClusters;
  UINT8          RegUmaEn;
  UINT8          RegDefeatureXor;

  RegFullSncEn = 0;
  RegSncIndEn = 0;
  RegNumClusters = 0;
  RegUmaEn = 0;
  RegDefeatureXor = 0;

  Ms2IdiGetFieldsSncCfg (SocId, &RegFullSncEn, &RegSncIndEn, &RegNumClusters);
  Ms2IdiGetFieldsUmaClusterCfg (SocId, &RegUmaEn, &RegDefeatureXor);

  if (RegFullSncEn == 0 && RegSncIndEn == 0 && RegNumClusters == 0 && RegUmaEn == 0) {
    return TRUE;
  }

  return FALSE;
}
