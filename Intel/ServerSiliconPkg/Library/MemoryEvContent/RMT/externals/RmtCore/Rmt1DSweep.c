/** @file
  Rmt1DSweep.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "Rmt1DSweep.h"
#include <Library/MemoryCoreLib.h>

/*

  Callback function to set up a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  RMT_MARGIN_1D_PARAMETER *Rmt1DSweep;
  RMT_CORE_TEST_DATA      *CoreTestData;
  CPGC_POINT_TEST_CONFIG  *CpgcPointTestCfg;
  MRC_RT                  MrcRankList;
  UINT8                   MemSsType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  PSYSHOST                Host;

  Rmt1DSweep = (RMT_MARGIN_1D_PARAMETER *)CommonParameters;
  CoreTestData = Rmt1DSweep->CoreTestData;
  CpgcPointTestCfg = &CoreTestData->CpgcPointTestCfg;
  Socket = Rmt1DSweep->CommonParameters.Socket;
  Host = Rmt1DSweep->CommonParameters.Host;

  CopyMem (CpgcPointTestCfg->LaneValBitmasks, CoreTestData->SelectMemPointTestCfg.LaneValBitmasks, sizeof (CpgcPointTestCfg->LaneValBitmasks));
  CopyMem (CpgcPointTestCfg->ChunkValBitmasks, CoreTestData->SelectMemPointTestCfg.ChunkValBitmasks, sizeof (CpgcPointTestCfg->ChunkValBitmasks));

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Rmt1DSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->SelectMemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }
  
      // TODO Turnaround test case are not supported!
      MrcRankList.Dimm = CoreTestData->DimmRankAs[Socket][Controller][Channel].Dimm;
      MrcRankList.Rank = CoreTestData->DimmRankAs[Socket][Controller][Channel].Rank;
      ChBitMask = 0x1 << (Controller * MAX_MC_CH + Channel);

      //
      // Set up the test sequence and address generator
      //
      SetupBssaCpgcTest (Host, Socket, MemSsType, ChBitMask, 1, &MrcRankList, (CPGC_CONFIGURE_HEADER*)CpgcPointTestCfg, TRUE, TRUE);
     
   } // Channel
  } // Controller

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to execute/Start a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  RMT_MARGIN_1D_PARAMETER *Rmt1DSweep;
  RMT_CORE_TEST_DATA      *CoreTestData;
  UINT8                   MemSsType;
  MRC_TT                  TestType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  PSYSHOST                Host;

  Rmt1DSweep = (RMT_MARGIN_1D_PARAMETER *)CommonParameters;
  CoreTestData = Rmt1DSweep->CoreTestData;
  Socket = CommonParameters->Socket;
  Host = CommonParameters->Host;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  TestType = RmtDqTest;
  ChBitMask = 0;

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Rmt1DSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->SelectMemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      ChBitMask |= 0x1 << (Controller * MAX_MC_CH + Channel);
    }// Channel
  } // Controller

  // Start test
  TestControl (Host, Socket, MemSsType, ChBitMask, NO_SUBCH, TestType, TRUE, START_TEST, NOT_STOP_TEST);

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to process measurement or test result

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to be executed before any sweep starts

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
#ifdef DDR5_SUPPORT
  RMT_MARGIN_1D_PARAMETER       *Rmt1DSweep;
  PSYSHOST                      Host;
  UINT8                         Socket;
  UINT8                         ChannelIndex;
  UINT8                         SubChannelIndex;
  UINT8                         Dimm;
  UINT8                         Buffer;
  DDR5_DATA_BUFFER_RWA0_STRUCT  DbRwA0;
  DDR5_DATA_BUFFER_RWA1_STRUCT  DbRwA1;

  if ((CommonParameters->Group >= DbDfeVrefDq0) && (CommonParameters->Group <= DbDfeVrefDq7)) {

    Rmt1DSweep = (RMT_MARGIN_1D_PARAMETER *)CommonParameters;
    Socket = CommonParameters->Socket;
    Host = CommonParameters->Host;

    for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

      if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
        continue;
      }

      Dimm = CommonParameters->Dimm[ChannelIndex];

      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (Buffer = 0; Buffer < BUFFERS_PER_SUBCH; Buffer++) {
          //
          // Save DEF feature, DFE_Vref and DEF training mode
          //
          ReadDbCwCache (Socket, ChannelIndex, SubChannelIndex, Dimm, Buffer, LRDIMM_DB_PAGE0, DDR5_DATA_BUFFER_RWA0_REG, &DbRwA0.Data);
          Rmt1DSweep->DbRegs[ChannelIndex][SubChannelIndex][Buffer].DbRwA0 = DbRwA0.Data;

          ReadDbCwCache (Socket, ChannelIndex, SubChannelIndex, Dimm, Buffer, LRDIMM_DB_PAGE0, DDR5_DATA_BUFFER_RWA1_REG, &DbRwA1.Data);
          Rmt1DSweep->DbRegs[ChannelIndex][SubChannelIndex][Buffer].DbRwA1 = DbRwA1.Data;

          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, ChannelIndex, Dimm, SubChannelIndex, NO_RANK, Buffer, NO_BIT,
            "Save DB RWA0 = 0x%x  RWA1 = 0x%x\n", DbRwA0.Data, DbRwA1.Data);

          //
          // Enable DEF feature, DFE_Vref and DEF training mode
          //
          DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RW93_REG, Buffer);

          if ((DbRwA0.Bits.dfe_feature_enable != 1) ||
            (DbRwA0.Bits.dfe_vref_enable != 1) ||
            (DbRwA0.Bits.rw_control_word_writes_broadcasted != 0)) {
            DbRwA0.Bits.dfe_feature_enable = 1;
            DbRwA0.Bits.dfe_vref_enable = 1;
            DbRwA0.Bits.rw_control_word_writes_broadcasted = 0;
            DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RWA0_REG, DbRwA0.Data);
          }

          if ((DbRwA1.Bits.global_dfe_training_mode_enable != 1) ||
            (DbRwA1.Bits.training_source != 0)) {
            DbRwA1.Bits.global_dfe_training_mode_enable = 1;
            DbRwA1.Bits.training_source = 0;
            DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RWA1_REG, DbRwA1.Data);
          }
        } // Buffer
      } // SubChannelIndex
    } // ChannelIndex
  }
#endif // DDR5_SUPPORT

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to be executed after any sweep ends

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
#ifdef DDR5_SUPPORT
  RMT_MARGIN_1D_PARAMETER       *Rmt1DSweep;
  PSYSHOST                      Host;
  UINT8                         Socket;
  UINT8                         ChannelIndex;
  UINT8                         SubChannelIndex;
  UINT8                         Dimm;
  UINT8                         Buffer;
  DDR5_DATA_BUFFER_RWA0_STRUCT  DbRwA0;
  DDR5_DATA_BUFFER_RWA1_STRUCT  DbRwA1;

  if ((CommonParameters->Group >= DbDfeVrefDq0) && (CommonParameters->Group <= DbDfeVrefDq7)) {
    Rmt1DSweep = (RMT_MARGIN_1D_PARAMETER *)CommonParameters;
    Socket = CommonParameters->Socket;
    Host = CommonParameters->Host;

    for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

      if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
        continue;
      }

      Dimm = CommonParameters->Dimm[ChannelIndex];

      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (Buffer = 0; Buffer < BUFFERS_PER_SUBCH; Buffer++) {

          DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RW93_REG, Buffer);

          //
          // Restore DEF feature and DFE_Vref
          //
          DbRwA0.Data = Rmt1DSweep->DbRegs[ChannelIndex][SubChannelIndex][Buffer].DbRwA0;
          DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RWA0_REG, DbRwA0.Data);

          //
          // Restore DEF training mode
          //
          DbRwA1.Data = Rmt1DSweep->DbRegs[ChannelIndex][SubChannelIndex][Buffer].DbRwA1;
          DbControlWordWrite (Host, Socket, ChannelIndex, 1 << SubChannelIndex, Dimm, 0, DDR5_DATA_BUFFER_RWA1_REG, DbRwA1.Data);

          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, ChannelIndex, Dimm, SubChannelIndex, NO_RANK, Buffer, NO_BIT,
            "Restore DB RWA0 = 0x%x  RWA1 = 0x%x\n", DbRwA0.Data, DbRwA1.Data);
        } // Buffer
      } // SubChannelIndex
    } // ChannelIndex
  }
#endif // DDR5_SUPPORT

  return MRC_STATUS_SUCCESS;
}
