/** @file
  File includes miscellaneous functions related to Xpt/Snc features

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

#include <Library/OemProcMemInitLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/MemTypeLib.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  Program SNC shadow register by mailbox command

  This routine sends the Mesh Mode BIOS to Pcode Mailbox command.

  @param  KtiInternalGlobal -   KTIRC Internal Global data

  @retval None

**/
VOID
ProgramSncShadowReg (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                      Socket, Index;
  UINT32                     MailboxData;
  SNC_CONFIG_IIO_VTD_STRUCT  IioSnc_Config;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      for (Index = 0; Index < MAX_CLUSTERS + 1; Index++) {
        //
        // Read SNC_BASE[1-5] registers and then write to SNC Shadow registers
        //
        IioGetSncIdx (Socket, 0, Index, &MailboxData);

        PcuCfgSnc (Socket, (SNC_INDEX_BASE_1 + Index), MailboxData);
      }

      //
      // Update SNC_UPPER_BASE shadow register
      //
      IioGetSncUpperBase (Socket, 0, &MailboxData);
      PcuCfgSnc (Socket, SNC_INDEX_UPPER_BASE, MailboxData);

      //
      // Update SNC_CONFIG shadow register, the num_snc_clu of CORE SNC_CONFIG is same as PMA
      // num_clusters definition in Core/PMA: 2'b00 2 Cluster, 2'b01 3 Clusters, 2'b10 4 Clusters, 2'b11 RSVD
      //
      IioSnc_Config.Data = IioGetSncCfg (KtiInternalGlobal->SbspSoc, 0);
      IioSnc_Config.Bits.full_snc_en = 0;
      if (IioSnc_Config.Bits.num_snc_clu != 0) {
        IioSnc_Config.Bits.num_snc_clu--;
      }

      //
      // This will always be set to 0 in the core copy of this register, because the core on
      // 10nm is only ever expected to produce the SNC indication (see below).
      // The uncore copy on the otherhand may have this set to 1.
      //
      PcuCfgSnc (Socket, SNC_INDEX_CONFIG, IioSnc_Config.Data);
    }
  }
}

/**
  Update XptPrefetch/KtiPrefetch enabled status in the KtiHostOut for 10nm

  This routine query the register of XptPrefetch/KtiPrefetch enabled status,
  then update them in KtiHostOut.

  @param[in] KtiInternalGlobal - Pointer to the KTI RC internal global structure

  @retval    None
**/
VOID
KtiUpdateXptKtiPrefetchStatus (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8 Socket;

  if (KTIVAR->OutXptPrefetch == KTI_ENABLE) {
    KTIVAR->OutXptPrefetch = KTI_DISABLE;
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      // check if socket is valid
      if (SocketPresent (Socket)) {
        if (Ms2IdiGetLocXptPrefDis (Socket) == KTI_DISABLE) {
          KTIVAR->OutXptPrefetch = KTI_ENABLE;
          break;
        }
      }
    }
  }

  if (KTIVAR->OutKtiPrefetch == KTI_ENABLE) {
    UpdateKtiPrefetchStatusAsRegisterValue (KtiInternalGlobal);
  }
}

/**
  API to dump Sub Numa Clustering (SNC) and XPT register.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
DumpSncXptKtiRegister (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8  Socket;
  UINT8  Ctr;
  UINT8  Cluster;
  UINT32 XptUpiDecodeTableN0r;
  UINT32 XptUpiDecodeTableN1r;
  UINT32 Data32;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n**** SNC XPT DUMP %a ****\n", "START"));

  //
  //Dump all SNC related registers
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    // check if socket is valid and snc enabled
    if (SocketPresent (Socket)) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n****  CPU %u: ****", Socket));
      //MS2IDI
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSNC_CONFIG_MS2IDI           : 0x%08X", Ms2IdiSncConfig (Socket)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUNCORE_SNC_CONFIG_MS2IDI    : 0x%08X", Ms2IdiUncoreSncConfig (Socket)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA_CLUSTER_MS2IDI          : 0x%08X", Ms2IdiUmaClusterCfgDefSpr(Socket)));
      //XPT
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_2_ENTRY_MINISAD_TABLE   : 0x%08X", Ms2IdiXPT2EntryMiniSadTabDef (Socket)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_LOCAL_PREFETCH_CONFIG%d : 0x%08X", 1, Ms2IdiXPTLocPrefCfg1Def (Socket)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_LOCAL_PREFETCH_CONFIG%d : 0x%08X", 2, Ms2IdiXPTLocPrefCfg2Def (Socket)));

      for (Ctr = 0; Ctr < NUM_OF_REGSTERS_MINISAD_32_ENTRY; Ctr++) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_32_ENTRY_MINISAD_TABLE_%x      : 0x%08X", Ctr, Ms2IdiGetXptEntryTable (Socket, Ctr)));
      }

      for (Ctr = 0; Ctr < NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE; Ctr++) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_32_ENTRY_PREFETCH_BASE_%x      : 0x%08X", Ctr, Ms2IdiGetXptEntryPrefBase (Socket, Ctr)));
      }

      //Remote XPT
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_REMOTE_PREFETCH_CONFIG%d : 0x%08X", 1, Ms2IdiXPTRmtPrefCfg1Def (Socket)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_REMOTE_PREFETCH_CONFIG%d : 0x%08X", 2, Ms2IdiXPTRmtPrefCfg2Def (Socket)));

      for (Ctr = 0; Ctr < (KtiInternalGlobal->KtiPortCnt); Ctr++) {
        Ms2IdiGetXptUpiDecTable (Socket, Ctr, &XptUpiDecodeTableN0r, &XptUpiDecodeTableN1r);
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_UPI_DECODE_TABLE_%x_N%d: 0x%08X", Ctr, 0, XptUpiDecodeTableN0r));
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nXPT_UPI_DECODE_TABLE_%x_N%d: 0x%08X", Ctr, 1, XptUpiDecodeTableN1r));
      }

      DumpKtiAgentM3KtiSncXptRegister (KtiInternalGlobal, Socket);
      //CHA
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCHA_SNC_CONFIG_CHA_PMA  : 0x%08X", ChaGetSncCfg (Socket, 0)));
      //IIO
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSNC_CONFIG_IIO_VTD      : 0x%08X", IioGetSncCfg (Socket, 0)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUNCORE_SNC_IIO_VTD      : 0x%08X", IioGetUncoreSnc (Socket, 0)));
      for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
        IioGetSncIdx (Socket, 0, Cluster, &Data32);
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSNC_BASE_%x_IIO_VTD     : 0x%08X", (Cluster + 1), Data32));
      }

      for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
        if (StackPresent (Socket, Ctr)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIIO %u:", Ctr));
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIIO_%d_SNC_CONFIG_IIO : 0x%08X", Ctr, IioGetSncCfg (Socket, Ctr)));
          for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
            IioGetSncIdx (Socket, Ctr, Cluster, &Data32);
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nIIO_%d_SNC_BASE_%x     : 0x%08X", Ctr, (Cluster + 1), Data32));
          }
        }
      }

      //M2MEM
      for (Ctr = 0; Ctr < KTIVAR->MaxSupportedM2m; Ctr++) {
        if ((KTIVAR->CpuInfo[Socket].M2mBitmap & (1 << Ctr)) == 0) {
          continue;
        }
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nM2MEM_%u_TOPOLOGY        : 0x%08X", Ctr, GetM2mTopology (Socket, Ctr)));
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nM2MEM_%u_PREFSADCONFIG%d : 0x%08X", Ctr, 0, KtiReadPciCfg(Socket, Ctr, (PREFETCHSADCONFIG_0_M2MEM_MAIN_REG))));
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nM2MEM_%u_PREFSADCONFIG%d : 0x%08X", Ctr, 1, KtiReadPciCfg(Socket, Ctr, (PREFETCHSADCONFIG_1_M2MEM_MAIN_REG))));
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nM2MEM_%u_SYSFEATURES0 : 0x%08X", Ctr, GetM2mSysFeatures0 (Socket, Ctr)));
      }
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n**** SNC XPT DUMP %a ****\n", "END"));
  return KTI_SUCCESS;
}