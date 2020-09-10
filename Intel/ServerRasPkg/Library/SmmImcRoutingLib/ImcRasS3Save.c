/** @file
  Memory RAS S3 Save support

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2016 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3MemDataLib.h>
#include "SDDCPrivate.h"

static EFI_SMM_VARIABLE_PROTOCOL   *mSmmVariable;


/**

  Saves all RAS settings that needs to be restored later during S3 resume boot

  @param DispatchHandle   -  The handle of this callback, obtained when registering.
  @param DispatchContext  -  The predefined context which contained sleep type and phase.

  @retval EFI_SUCCESS            -  Operation successfully performed.
  @retval EFI_INVALID_PARAMETER  -  Invalid parameter passed in.

**/
EFI_STATUS
EFIAPI
ImcRasS3EntryCallback (
  IN EFI_HANDLE                           DispatchHandle,
  IN CONST  EFI_SMM_SX_REGISTER_CONTEXT  *DispatchContext,
  IN OUT    VOID                         *CommBuffer,     OPTIONAL
  IN OUT    UINTN                        *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  IMC_RAS_S3_SAVE                 *ImcRasS3SaveData;
  MEM_TOPOLOGY                    *MemTopology;
  MEMRAS_S3_PARAM                 *MemrasS3Param;
  UINT8                           Socket, Node, Imc, Ch, SktCh, Dimm;
  UINT8                           Rank, LogicalRank = 0;
  UINT8                           spareInUse;
  UINTN                           DataSize;

  //
  // Locate SmmVariableProtocol.
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&mSmmVariable);
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate Memory for IMC_RAS_S3_SAVE data structure
  //
  DataSize = sizeof (IMC_RAS_S3_SAVE);
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      DataSize,
                      &ImcRasS3SaveData
                      );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) return EFI_OUT_OF_RESOURCES;

  //
  // Get Memory Topology
  //
  MemTopology = GetMemTopology ();

  //
  // Get Memory RAS Runtime Data
  //
  MemrasS3Param = GetS3RasData ();


  //
  //For each Node, save context
  //
  for (Node = 0; Node < MC_MAX_NODE; Node++) {
    Socket = NODE_TO_SKT (Node);
    Imc = Node % MAX_IMC;
    SktCh = NODECHA_TO_SKTCHA (Node, 0);
    if (MemTopology->Socket[Socket].imcEnabled[Imc] == 0) continue;

    MemS3SavePatrolScrub (MemTopology, ImcRasS3SaveData, Socket, Imc);

    MemS3SaveLeakyBucket (MemTopology, ImcRasS3SaveData, Socket, Imc);

    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      SktCh = NODECHA_TO_SKTCHA (Node, Ch);
      if (MemTopology->Socket[Socket].ChannelInfo[SktCh].Enabled == 0) continue;

      //
      // Save Retry-Rd register information
      //
      MemS3SaveRetryRd (MemTopology, ImcRasS3SaveData, Socket, Imc, SktCh);

      // save rank spare failover info
      spareInUse = MemrasS3Param->SpareRank[Node][Ch].SpareInUse[0];
      ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].spareInUse = spareInUse;

      // save mirror failover info
      ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].chFailed = MemrasS3Param->ChFailed[Node][Ch];
      ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].ngnChFailed = MemrasS3Param->NgnChFailed[Node][Ch];

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].Present == 0) continue;

        for (Rank = 0; Rank < (MAX_RANK_DIMM * SUB_CH); Rank++) {
          if (MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].rankList[Rank].enabled == 0) continue;

          // save DRAM failure (SDDC) logical rank info
          if (GetSDDCState(Node, Ch, Rank) == SddcPlusOneAvailable ) {
            LogicalRank = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].logicalRank;
            ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].devTagInfo =
                MemrasS3Param->DevTag[Node][Ch][LogicalRank];
          }

          // Save physical rank id of old spared rank in nvram
          if (spareInUse == 1) {
            if (LogicalRank ==  MemrasS3Param->SpareRank[Node][Ch].OldLogicalRank[0]) {
              ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].oldSparePhysicalRank =
                  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].phyRank;

              // Loop to add the oldSpare rank and New spare rank incase of multi rank support.
              //ImcRasS3SaveData->nvram.mem.socket[Socket].channelList[SktCh].oldSparePhysicalRank[mMemrasS3Param.imc[NodeId].channelList[Ch].MultiSpareRankCnt] = SrcRank;
              //(mMemrasS3Param.MultiSpareRankCnt[Node][Ch])++;
              //ImcRasS3SaveData->nvram.common.imc[Node].channelList[Ch].newSparePhysRank =
              //MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].rankList[Rank].phyRank;

            } // if (LogicalRank)
          } // if (SpareInUse)

        } // rank
      } // Dimm

      MemS3SavePerRankErrorSetting (MemTopology, ImcRasS3SaveData, Socket, Imc, SktCh);

      MemS3SaveLinkFailSetting (MemTopology, ImcRasS3SaveData, Socket, Imc, SktCh);

      MemS3SaveLeakyBucket2nd (MemTopology, ImcRasS3SaveData, Socket, Imc, SktCh);

      //
      // SMI settings.
      //
      MemS3SaveSmiSetting (MemTopology, ImcRasS3SaveData, Socket, Imc, SktCh);

    }  // for (Ch)

  } //Node



    Status = mSmmVariable->SmmSetVariable (
    EFI_MEM_RAS_S3_SAVE_DATA_VARIABLE_NAME,
    &gMemRasS3DataVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    DataSize,
    ImcRasS3SaveData
    );


  gSmst->SmmFreePool (ImcRasS3SaveData);

  return EFI_SUCCESS;
}

VOID
EFIAPI
InitImcRasS3Save (VOID)
/**

    IMC ras s3 driver entry point

    @param None

    @retval None

**/
{

  EFI_STATUS                      Status;
  EFI_SMM_SX_REGISTER_CONTEXT     *EntryS3DispatchContext;
  EFI_SMM_SX_DISPATCH2_PROTOCOL   *SxDispatch;
  EFI_HANDLE                      SxDispatchHandle;
  UINTN                           Length;

  Length = sizeof ( EFI_SMM_SX_REGISTER_CONTEXT);
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      Length,
                      &EntryS3DispatchContext
                      );
  if (EFI_ERROR (Status))  return ;

  ZeroMem (EntryS3DispatchContext, sizeof (EFI_SMM_SX_REGISTER_CONTEXT));

  Status = gSmst->SmmLocateProtocol (
                &gEfiSmmSxDispatch2ProtocolGuid,
                NULL,
                &SxDispatch
                );
  if (EFI_ERROR (Status))  return;
  //
  // Register the callback for S3 entry
  //
  EntryS3DispatchContext->Type  = SxS3;
  EntryS3DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                                SxDispatch,
                                ImcRasS3EntryCallback,
                                EntryS3DispatchContext,
                                &SxDispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);

  return;
}


