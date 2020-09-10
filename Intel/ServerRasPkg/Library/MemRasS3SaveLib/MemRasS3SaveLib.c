/** @file
  IMC RAS S3 Save support

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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

#include <Library/SmmServicesTableLib.h>
#include <Library/MemRasS3SaveLib.h>
#include <Library/S3BootScriptLib.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Library/RasDebugLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RasSiliconLib.h>
#include <RcRegs.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/SiliconWorkaroundLib.h>

#define ECC_MODE_REG_LIST             {ECC_MODE_RANK0_MC_MAIN_REG, ECC_MODE_RANK1_MC_MAIN_REG, ECC_MODE_RANK2_MC_MAIN_REG, \
                                        ECC_MODE_RANK3_MC_MAIN_REG, ECC_MODE_RANK4_MC_MAIN_REG, ECC_MODE_RANK5_MC_MAIN_REG, \
                                          ECC_MODE_RANK6_MC_MAIN_REG, ECC_MODE_RANK7_MC_MAIN_REG}

#define CORRERRCNT_MCDDC_DP_LIST      {CORRERRCNT_0_MCDDC_DP_REG, CORRERRCNT_1_MCDDC_DP_REG, CORRERRCNT_2_MCDDC_DP_REG, \
                                        CORRERRCNT_3_MCDDC_DP_REG}

#define CORRERRTHRSHLD_MCDDC_DP_LIST  {CORRERRTHRSHLD_0_MCDDC_DP_REG, CORRERRTHRSHLD_1_MCDDC_DP_REG, CORRERRTHRSHLD_2_MCDDC_DP_REG, \
                                        CORRERRTHRSHLD_3_MCDDC_DP_REG}

#define PLUS1_RANK_MCDDC_DP_LIST      {PLUS1_RANK0_MCDDC_DP_REG, PLUS1_RANK1_MCDDC_DP_REG, PLUS1_RANK2_MCDDC_DP_REG, \
                                        PLUS1_RANK3_MCDDC_DP_REG, PLUS1_RANK4_MCDDC_DP_REG, PLUS1_RANK5_MCDDC_DP_REG, \
                                          PLUS1_RANK6_MCDDC_DP_REG, PLUS1_RANK7_MCDDC_DP_REG  }

///
/// Private structure used to pass patrol scrub context to the S3 boot script
/// dispatch callback.
///
typedef struct {
  UINT64  Signature;      ///< Must be SCRUB_INTERVAL_CONTEXT_SIGNATURE.
  UINT8   Socket;         ///< Socket index.
  UINT8   Mc;             ///< MC index in the socket.
  UINT32  ScrubInterval;  ///< Scrub interval to set.
} SCRUB_INTERVAL_CONTEXT;

#define SCRUB_INTERVAL_CONTEXT_SIGNATURE  SIGNATURE_64 ('P', 'A', 'T', 'R', 'S', 'C', 'R', 'B')

/**
  Restore the patrol scrub interval.

  This function is a callback for a EFI_BOOT_SCRIPT_DISPATCH_2 opcode in the
  boot script.

  @param[in]  ImageHandle         The handle for the EFI image.
  @param[in]  Context             The implementation-specific context data.

  @retval EFI_SUCCESS             The patrol scrub interval was restored.
  @retval EFI_INVALID_PARAMETER   The context pointer or data was invalid.
  @retval EFI_DEVICE_ERROR        The PCU mailbox command failed.
**/
EFI_STATUS
EFIAPI
RestoreScrubIntervalCallback (
  IN EFI_HANDLE ImageHandle,
  IN VOID       *Context
  )
{
  SCRUB_INTERVAL_CONTEXT  *ScrubIntervalContext;
  EFI_STATUS              Status;

  ScrubIntervalContext = (SCRUB_INTERVAL_CONTEXT *) Context;

  RAS_ASSERT (ScrubIntervalContext != NULL);
  if (ScrubIntervalContext == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RAS_ASSERT (ScrubIntervalContext->Signature == SCRUB_INTERVAL_CONTEXT_SIGNATURE);
  if (ScrubIntervalContext->Signature != SCRUB_INTERVAL_CONTEXT_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "SetScrubIntervalMailbox - Socket: %d; Mc: %d; ScrubInterval: 0x%x\n",
    ScrubIntervalContext->Socket,
    ScrubIntervalContext->Mc,
    ScrubIntervalContext->ScrubInterval
    ));

  Status = SetScrubIntervalMailbox (
    ScrubIntervalContext->Socket,
    ScrubIntervalContext->Mc,
    ScrubIntervalContext->ScrubInterval
    );

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "SetScrubIntervalMailbox - Status: %r\n",
    Status
    ));

  FreePool (ScrubIntervalContext);

  return Status;
}

/**
  Saves Patrol Scrub settings that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc -  Imc Number
  @retval None.

**/
VOID
EFIAPI
MemS3SavePatrolScrub (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8            Socket,
  IN UINT8            Imc
  )
{
  UINT64                          RegAddr;
  UINT32                          Data;
  UINT8                           Size;
  UINT8                           ChannelOnMc;
  UINT8                           ChOnSkt;
  SCRUBCTL_MC_MAIN_STRUCT         ScrubCtlReg;
  SCRUB_INTERVAL_CONTEXT          *ScrubIntervalContext;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "MemS3SavePatrolScrub - Socket: %d, MC: %d\n",
    Socket,
    Imc
    ));

  //
  // Set interval to zero before disabling on every channel.
  //
  SetScrubIntervalMailbox (Socket, Imc, 0);

  //
  // Save scrubctl, scrubaddrlo and scrubaddrhi registers
  //
  for (ChannelOnMc = 0; ChannelOnMc < MAX_MC_CH; ChannelOnMc++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Imc, ChannelOnMc);
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "MemS3SavePatrolScrub - MC channel: %d (socket channel: %d)\n",
      ChannelOnMc,
      ChOnSkt
      ));

    ScrubCtlReg.Data  = ReadCpuCsr (Socket, ChOnSkt, SCRUBCTL_MC_MAIN_REG);
    if (ScrubCtlReg.Bits.scrub_en == 1) { //Patrol scrub enabled
      Data = ScrubCtlReg.Data;
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBCTL_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite ScrubCtlReg - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG);
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite scrubAddrLo - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite scrubAddrHi - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG);
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite scrubAddr2Lo - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG);
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite scrubAddr2Hi - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBMASK_MC_MAIN_REG);
      RegAddr = GetCpuCsrAddress (Socket, ChOnSkt, SCRUBMASK_MC_MAIN_REG, &Size);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite ScrubMask - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
      S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

      //
      // Disable patrol scrub for this channel.
      //
      ScrubCtlReg.Bits.scrub_en = 0;
      WriteCpuCsr (Socket, ChOnSkt, SCRUBCTL_MC_MAIN_REG, ScrubCtlReg.Data);
    }
  }

  //
  // On S3 resume, set the interval after re-enabling on every channel.
  //
  if (MemTopology->Socket[Socket].PatrolScrubInterval[Imc] != 0) {
    ScrubIntervalContext = AllocatePool (sizeof (SCRUB_INTERVAL_CONTEXT));
    RAS_ASSERT (ScrubIntervalContext != NULL);
    if (ScrubIntervalContext != NULL) {
      ScrubIntervalContext->Signature = SCRUB_INTERVAL_CONTEXT_SIGNATURE;
      ScrubIntervalContext->Socket = Socket;
      ScrubIntervalContext->Mc = Imc;
      ScrubIntervalContext->ScrubInterval = MemTopology->Socket[Socket].PatrolScrubInterval[Imc];
      RAS_DEBUG ((
        LEVEL_BASIC_FLOW,
        "S3BootScriptSaveDispatch2 Context @ 0x%p - Socket: %d; Mc: %d; ScrubInterval: 0x%x\n",
        ScrubIntervalContext,
        ScrubIntervalContext->Socket,
        ScrubIntervalContext->Mc,
        ScrubIntervalContext->ScrubInterval
        ));
      S3BootScriptSaveDispatch2 (
        (VOID *) (UINTN) RestoreScrubIntervalCallback,
        (VOID *) ScrubIntervalContext
        );
    }
  }
}

/**

  Saves Leaky Bucket settings that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc -  Imc Number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLeakyBucket (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8  Socket,
  IN UINT8  Imc
  )
{

  UINT64                          RegAddr;
  UINT32                          Data;
  UINT8                           Size;
  //
  // Leaky bucket node values
  //
  Data = ReadCpuCsr (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CFG_MC_MAIN_REG);
  RegAddr = GetCpuCsrAddress (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CFG_MC_MAIN_REG, &Size);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite leakyBucketCfg - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

  Data = ReadCpuCsr (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CNTR_LO_MC_MAIN_REG);
  RegAddr = GetCpuCsrAddress (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CNTR_LO_MC_MAIN_REG, &Size);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite leakyBucketCounterLo - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

  Data = ReadCpuCsr (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CNTR_HI_MC_MAIN_REG);
  RegAddr = GetCpuCsrAddress (Socket, Imc * MAX_MC_CH, LEAKY_BUCKET_CNTR_HI_MC_MAIN_REG, &Size);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite leakyBucketCounterHi - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);

}

/**

  Saves Retry read register values per channel to be restored at end of S3 operation

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket   - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveRetryRd (
  IN MEM_TOPOLOGY         *MemTopology,
  IN OUT IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8                Socket,
  IN UINT8                Imc,
  IN UINT8                SktCh
  )
{
  UINT64                          RegAddr;
  UINT32                          Data;
  UINT8                           Size;

  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT    RetryRdErrLog;
  RETRY_RD_ERR_SET2_LOG_MCDDC_DP_STRUCT    RetryRdErrLogSet2;

  RetryRdErrLog.Data = ReadCpuCsr(Socket, SktCh, RETRY_RD_ERR_LOG_MCDDC_DP_REG);
  if( RetryRdErrLog.Bits.v == 1) {
    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_LOG_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_LOG_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryRead - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );

    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_LOG_ADDRESS1_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_LOG_ADDRESS1_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryReadAddr1 - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );

    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_LOG_ADDRESS2_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_LOG_ADDRESS2_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryReadAddr2 - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );
  }

  RetryRdErrLogSet2.Data = ReadCpuCsr(Socket, SktCh, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG);
  if( RetryRdErrLogSet2.Bits.v == 1) {
    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryReadset2 - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );

    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_ADDRESS1_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_ADDRESS1_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryReadset2Addr1 - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );

    Data = ReadCpuCsr (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_ADDRESS2_MCDDC_DP_REG);
    RegAddr = GetCpuCsrAddress (Socket, SktCh, RETRY_RD_ERR_SET2_LOG_ADDRESS2_MCDDC_DP_REG, &Size);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite RetryReadset2Addr2 - 0x%08x, 0x%08x\n", (UINTN)RegAddr, Data));
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );
  }
}


/**

  Saves Per Rank Error Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SavePerRankErrorSetting (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8  Socket,
  IN UINT8  Imc,
  IN UINT8  SktCh
  )
{

  ECC_MODE_RANK0_MC_MAIN_STRUCT   EccModeMcMain;
  UINT32                          EccModeReg[] = ECC_MODE_REG_LIST;
  UINT32                          CorrErrCntMcddcReg[] = CORRERRCNT_MCDDC_DP_LIST;
  UINT32                          CorrErrThrshldMcddcReg[] = CORRERRTHRSHLD_MCDDC_DP_LIST;
  UINT32                          Plus1RankMcddcReg[] = PLUS1_RANK_MCDDC_DP_LIST;
  UINT8                           i;

  //
  // Per rank error counts and thresholds and status
  //
  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrCountStatus = ReadCpuCsr (Socket, SktCh, CORRERRORSTATUS_MCDDC_DP_REG);
  for (i = 0; i < (MAX_RANK_CH * SUB_CH)/2; i++) {
    ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrCountInfo[i] = ReadCpuCsr (Socket, SktCh, CorrErrCntMcddcReg[i]);
    ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrThresholdInfo[i] = ReadCpuCsr (Socket, SktCh, CorrErrThrshldMcddcReg[i]);
  }
  for (i = 0; i < (MAX_RANK_CH * SUB_CH); i++) {
    //
    // Save Plus 1 data
    //
    ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].Plus1Failover[i] = ReadCpuCsr (Socket, SktCh, Plus1RankMcddcReg[i]);
    //
    //Save ECC modes
    //
    EccModeMcMain.Data = (UINT8) ReadCpuCsr (Socket, SktCh, EccModeReg[i]);
    ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].EccModeMcMain[i] = (UINT8)EccModeMcMain.Bits.mode;

  } // for (i)
}

/**

  Saves Link Fail Setting that needs to be restored later during S3 resume boot

  @param *MemTopology  -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLinkFailSetting (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8            Socket,
  IN UINT8            Imc,
  IN UINT8            SktCh
  )
{

  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].LinkRetryErrLimits = ReadCpuCsr (Socket, SktCh, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG);
  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].LinkLinkFail = ReadCpuCsr (Socket, SktCh, LINK_LINK_FAIL_MCDDC_DP_REG);

}

/**

  Saves Leaky Bucket 2nd Setting that needs to be restored later during S3 resume boot

  @param *MemTopology   -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveLeakyBucket2nd (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8            Socket,
  IN UINT8            Imc,
  IN UINT8            SktCh
  )
{


  UINT64                          RegAddr;
  UINT32                          Data;
  UINT8                           Size;

  Size = 0;
  //
  // Leaky bucket
  //
  Data = ReadCpuCsr (Socket, SktCh, LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG);
  RegAddr = GetCpuCsrAddress (Socket, SktCh, LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG, &Size);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "S3BootScriptSaveMemWrite leakyBucket2ndCounter - 0x%08x, 0x%08x, 0x%x\n", (UINTN)RegAddr, Data, Size));
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data);
}

/**

  Saves SMI Setting that needs to be restored later during S3 resume boot

  @param *MemTopology  -  Memory Topology
  @param *ImcRasS3SaveData  -  The data buffer to save S3 context
  @param Socket - Socket Number
  @param Imc  -  Imc Number
  @param SktCh - Socket Channel number

  @retval None.

**/
VOID
EFIAPI
MemS3SaveSmiSetting (
  IN MEM_TOPOLOGY     *MemTopology,
  IN IMC_RAS_S3_SAVE  *ImcRasS3SaveData,
  IN UINT8            Socket,
  IN UINT8            Imc,
  IN UINT8            SktCh
  )
{
  //
  // SMI settings.
  //
  if (IsSiliconWorkaroundEnabled ("S1409629810")) {
    ImcRasS3SaveData->mem.socket[Socket].smiCtrlUboxMisc = ReadCRProxyRegister (
                                                             Socket,
                                                             0,
                                                             MAILBOX_BIOS_CMD_CR_PROXY_UBOX_SMICTRL_CFG_INDEX
                                                             );
  } else {
    ImcRasS3SaveData->mem.socket[Socket].smiCtrlUboxMisc = ReadCpuCsr (Socket, 0, UBOX_SMICTRL_UBOX_MISC_REG);
  }
  ImcRasS3SaveData->mem.socket[Socket].imc[Imc].EmcaLtCtlMcMainExt   = (UINT8) ReadCpuCsr (Socket, Imc * MAX_MC_CH, EMCALTCTL_MC_MAIN_REG);
  ImcRasS3SaveData->mem.socket[Socket].imc[Imc].ExRasConfigHaCfg     = ReadCpuCsr (Socket, Imc, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].SmiSpareCtlMcMainExt = GetSmiSpareCtl(Socket, Imc, SktCh);
}
