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
#ifndef __MS2IDI_LIB_H__
#define __MS2IDI_LIB_H__

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiHost.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/UsraCsrLib.h>

#define NUM_OF_SNC_BASE 6

#define Ms2IdiUncoreSncConfig(_Socket_)         UsraCsrRead (_Socket_, 0, UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG)
#define Ms2IdiSncConfig(_Socket_)               UsraCsrRead (_Socket_, 0, SNC_CONFIG_MS2IDI_MAIN_REG)
#ifdef GNRSRF_HOST
//
// Using SPR macro name until GNR instance of SncPrefetchLib is created
//
#define Ms2IdiUmaClusterCfgDefSpr(_Socket_)     ((UINT16)UsraCsrRead (_Socket_, 0, UMA_CLUSTER_CFG_MS2IDI_MAIN_REG))
#else //GNRSRF_HOST
#define Ms2IdiUmaClusterCfgDefSpr(_Socket_)     ((UINT16)UsraCsrRead (_Socket_, 0, UMA_CLUSTER_MS2IDI_MAIN_REG))
#endif // GNRSRF_HOST
#define Ms2IdiUmaClusterCfgDefIcx(_Socket_)     ((UINT16)UsraCsrRead (_Socket_, 0, UMA_CLUSTER_CFG_MS2IDI_MAIN_REG))
#define Ms2IdiXPT2EntryMiniSadTabDef(_Socket_)  UsraCsrRead (_Socket_, 0, XPT_2_ENTRY_MINISAD_TABLE_MS2IDI_MAIN_REG)
#define Ms2IdiXPTLocPrefCfg1Def(_Socket_)       UsraCsrRead (_Socket_, 0, XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG)
#define Ms2IdiXPTLocPrefCfg2Def(_Socket_)       UsraCsrRead (_Socket_, 0, XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_REG)
#define Ms2IdiXPTRmtPrefCfg1Def(_Socket_)       UsraCsrRead (_Socket_, 0, XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG)
#define Ms2IdiXPTRmtPrefCfg2Def(_Socket_)       UsraCsrRead (_Socket_, 0, XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_REG)

#define MS2IDI_INVALID_VALUE  0xFFFFFFFF
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
  );

/**
  MS2IDI:  Get xpt prefetch disable value

  @param SocId           - Socket ID

  @retval                -xpt prefetch disable value

**/
UINT8
EFIAPI
Ms2IdiGetLocXptPrefDis (
  UINT8 SocId
  );

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
  );

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
  );

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
  );

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
  );

/**
  MS2IDI:  Set XptUpiDecodeTable

  @param SocId   - Socket ID
  @param UpiId   - Upi Id table
  @param TabIdx  - Table index

  @retval                    - None

**/
VOID
EFIAPI
Ms2IdiSetDecUpiId (
  UINT8 SocId,
  UINT8 UpiId,
  UINT8 TabIdx
  );

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
  );

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
  IN UINT8  SocId,
  IN UINT8  DecodeTableIdx,
  IN UINT8  RemSktId,
  IN UINT8  UpiId,
  IN UINT32 MemBase,
  IN UINT32 MemLimit
  );

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
  );

/**
  MS2IDI:  Get LB_MCHECK_W_RAC_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_RAC_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueRac (
  UINT8 SocId
  );

/**
  MS2IDI:  Get LB_MCHECK_W_WAC_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_WAC_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueWac (
  UINT8 SocId
  );

/**
  MS2IDI:  Set LB_MCHECK_W_WAC_N0

  @param SocId           - Socket ID
  @param SaiLowValue     - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiLowValueWac (
  UINT8 SocId,
  UINT32 SaiLowValue
  );

/**
  MS2IDI:  Get LB_MCHECK_W_WAC_N1

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_WAC_N1

**/
UINT32
EFIAPI
Ms2IdiGetSaiHighValueWac (
  UINT8 SocId
  );

/**
  MS2IDI:  Set LB_MCHECK_W_WAC_N1

  @param SocId           - Socket ID
  @param SaiHighValue    - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiHighValueWac (
  UINT8 SocId,
  UINT32 SaiHighValue
  );

/**
  MS2IDI:  Get LB_MCHECK_W_CP_N0

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_CP_N0

**/
UINT32
EFIAPI
Ms2IdiGetSaiLowValueCp (
  UINT8 SocId
  );

/**
  MS2IDI:  Set LB_MCHECK_W_CP_N0

  @param SocId           - Socket ID
  @param SaiLowValue     - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiLowValueCp (
  UINT8 SocId,
  UINT32 SaiLowValue
  );

/**
  MS2IDI:  Get LB_MCHECK_W_CP_N1

  @param SocId           - Socket ID

  @retval                - LB_MCHECK_W_CP_N1

**/
UINT32
EFIAPI
Ms2IdiGetSaiHighValueCp (
  UINT8 SocId
  );

/**
  MS2IDI:  Set LB_MCHECK_W_CP_N1

  @param SocId           - Socket ID
  @param SaiHighValue    - Sai Low Value to set

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiSetSaiHighValueCp (
  UINT8 SocId,
  UINT32 SaiHighValue
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  MS2IDI:  Get SNC_CONFIG

  @param SocId           - Socket ID

  @retval                - SNC_CONFIG

**/
UINT32
EFIAPI
Ms2IdiGetSncCfg (
  UINT8 SocId
  );

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
  );

/**
  MS2IDI:  Get SNC_UPPER_BASE

  @param SocId           - Socket ID

  @retval                - SNC_UPPER_BASE

**/
UINT32
EFIAPI
Ms2IdiGetSncUpperBase (
  UINT8 SocId
  );

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
  );

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
  );

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature and program the required registers

  @param SocId            - Socket ID
  @param DdrtQosMode      - DDRT QoS Mode

  @retval         - None

**/
VOID
EFIAPI
Ms2IdiSetEnDdrtQoS (
  UINT8  SocId,
  UINT8  DdrtQosMode
  );

/**
  MS2IDI:  Enable/Disable the DDRT QoS Feature

  @param SocId    - Socket ID
  @param EnVal    - Enable/Disable the DDRT QoS Feature

  @retval         - None

**/
VOID
EFIAPI
Ms2IdiSetDdrtRange (
  UINT8 SocId,
  UINT32 DdrtBaseAddress,
  UINT32 DdrtLimitAddress
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  MS2IDI:  Configure XPT FIFO Enabled Credit Num

  @param SocId                - Socket ID
  @param XptFifoEnabledCredit - Xpt Fifo Enabled Creidt Num

  @retval                - None

**/
VOID
EFIAPI
Ms2IdiXptCreditConfig (
  UINT8 SocId,
  UINT8 XptFifoEnabledCredit
  );

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
  );

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
  );

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
  );

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
  );

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
  );
#endif //__MS2IDI_LIB_H__
