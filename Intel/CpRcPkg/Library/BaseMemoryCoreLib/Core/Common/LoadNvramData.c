/** @file

  LoadNvramData.c - Load Nvram data

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <SysHost.h>

#include "Include/BaseMemoryCoreInternal.h"
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/CrcLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/CompressedVariableLib.h>

#include <Ppi/ReadOnlyVariable2.h>

#include <Memory/RasNvramStruct.h>
#if FixedPcdGetBool(PcdFspBinaryEnable)  == 1
#include <Library/FspCommonLib.h>
#endif

extern EFI_GUID gEfiMemoryConfigDataGuid;

/**
  Update Host->nvram.mem for S3 saved memory RAS features.

  @param[in]  Host        - Pointer to SysHost structure.

  @retval None

**/

VOID
LoadImcS3RasData (
  IN PSYSHOST                         Host
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiVariable;
  UINT8   Socket;
  UINT8   Node;
  UINT8   Imc;
  UINT8   Ch;
  UINT8   SktCh;
  UINT8   Dimm;
  UINT8   Rank;
  UINTN   DataSize;
  UINT8   i;
  UINT16  Crc16;
  BOOLEAN IsCrcGood;

  EFI_STATUS                      Status = EFI_SUCCESS;
  IMC_RAS_S3_SAVE                 *ImcRasS3SaveData;
  CHAR16         EfiImcRasS3SaveDataVariable[] = L"ImcRasS3SaveData";

  DEBUG ((EFI_D_INFO, "LoadImcS3RasData: entered\n"));

  //
  // Locate Variable Services PPI
  //

  Status = PeiServicesLocatePpi (
                &gEfiPeiReadOnlyVariable2PpiGuid,
                0,
                NULL,
                &PeiVariable
                );
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (IMC_RAS_S3_SAVE);
  ImcRasS3SaveData = RcAllocatePool (DataSize);
  if (ImcRasS3SaveData == NULL) {
    DEBUG ((EFI_D_ERROR, "Cannot allocate pool for ImcRasS3SaveData!\n"));
    return;
  }

  Status = PeiVariable->GetVariable (
             PeiVariable,
             EfiImcRasS3SaveDataVariable,
             &gMemRasS3DataVariableGuid,
             NULL,
             &DataSize,
             ImcRasS3SaveData
             );
  if (EFI_ERROR (Status)){
    RcFreePool (ImcRasS3SaveData);
    DEBUG ((EFI_D_ERROR, "LoadImcS3RasData: could not find gMemRasS3DataVariableGuid\n"));
    return;
  }

  //
  // If the RAS S3 save data was good, calculate the CRC on the NVRAM
  // data before updating.
  //
  Crc16 = Host->nvram.mem.Crc16;
  Host->nvram.mem.Crc16 = 0;
  Status = CalculateCrc16 (
    (VOID *) &Host->nvram.mem,
    sizeof (Host->nvram.mem),
    &Host->nvram.mem.Crc16
    );
  ASSERT_EFI_ERROR (Status);
  ASSERT (Crc16 == Host->nvram.mem.Crc16);
  if (EFI_ERROR (Status) || Crc16 != Host->nvram.mem.Crc16) {
    IsCrcGood = FALSE;
  } else {
    IsCrcGood = TRUE;
  }

  for (Node = 0; Node < MC_MAX_NODE; Node++) {
    Socket = NODE_TO_SKT(Node);
    Imc = Node % MAX_IMC;
    if (Host->nvram.mem.socket[Socket].imc[Imc].enabled == 0){
      continue;
    }

    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      SktCh = NODECH_TO_SKTCH(Node, Ch);
      if (Host->nvram.mem.socket[Socket].channelList[SktCh].enabled == 0){
        continue;
      }

      //
      // save rank spare failover info
      //
      Host->nvram.mem.socket[Socket].channelList[SktCh].spareInUse = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].spareInUse;
      //
      // save mirror failover info
      //
      Host->nvram.mem.socket[Socket].channelList[SktCh].chFailed = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].chFailed;
      Host->nvram.mem.socket[Socket].channelList[SktCh].ngnChFailed = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].ngnChFailed;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (Host->nvram.mem.socket[Socket].channelList[SktCh].dimmList[Dimm].dimmPresent == 0){
          continue;
        }

        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (Host->nvram.mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].enabled == 0){
            continue;
          }
          if (Host->nvram.mem.socket[Socket].channelList[SktCh].dimmList[Dimm].mapOut[Rank] == 1){
            continue;
          }
          //
          // save DRAM failure (SDDC) logical rank info
          //
          Host->nvram.mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].devTagInfo =
          ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].dimmList[Dimm].rankList[Rank].devTagInfo;
        } // rank
      } // Dimm

      //
      // Per rank error counts and thresholds and status
      //
      Host->nvram.mem.socket[Socket].channelList[SktCh].rankErrCountStatus = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrCountStatus;
      for (i = 0; i < (MAX_RANK_CH * SUB_CH)/2; i++) {
        Host->nvram.mem.socket[Socket].channelList[SktCh].rankErrCountInfo[i] = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrCountInfo[i];
        Host->nvram.mem.socket[Socket].channelList[SktCh].rankErrThresholdInfo[i] = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].rankErrThresholdInfo[i];
      }
      for (i = 0; i < (MAX_RANK_CH * SUB_CH); i++) {
        //
        // Save Plus 1 data
        //
        Host->nvram.mem.socket[Socket].channelList[SktCh].Plus1Failover[i] = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].Plus1Failover[i];
        //
        //Save ECC modes
        //
        Host->nvram.mem.socket[Socket].channelList[SktCh].EccModeMcMain[i] = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].EccModeMcMain[i];
      } // for (i)
      Host->nvram.mem.socket[Socket].channelList[SktCh].LinkRetryErrLimits = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].LinkRetryErrLimits;
      Host->nvram.mem.socket[Socket].channelList[SktCh].LinkLinkFail = ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].LinkLinkFail;
    }  // for (Ch)

    //
    // SMI settings.
    //
    Host->nvram.mem.socket[Socket].smiCtrlUboxMisc = ImcRasS3SaveData->mem.socket[Socket].smiCtrlUboxMisc;
    Host->nvram.mem.socket[Socket].imc[Imc].EmcaLtCtlMcMainExt = ImcRasS3SaveData->mem.socket[Socket].imc[Imc].EmcaLtCtlMcMainExt;
    Host->nvram.mem.socket[Socket].imc[Imc].ExRasConfigHaCfg = ImcRasS3SaveData->mem.socket[Socket].imc[Imc].ExRasConfigHaCfg;
    Host->nvram.mem.socket[Socket].imc[Imc].SmiSpareCtlMcMainExt =  ImcRasS3SaveData->mem.socket[Socket].imc[Imc].SmiSpareCtlMcMainExt;
    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      SktCh = NODECH_TO_SKTCH(Node, Ch);
      if (Host->nvram.mem.socket[Socket].channelList[SktCh].enabled == 0){
        continue;
      }
      Host->nvram.mem.socket[Socket].channelList[SktCh].SmiSpareCtlMcMainExt =  ImcRasS3SaveData->mem.socket[Socket].channelList[SktCh].SmiSpareCtlMcMainExt;
    }

  } //Node

  //
  // If the CRC was good before, recalculate it over the new data. If it was
  // not, then set the CRC back to its original value to indicate that the new
  // data is not verified.
  //
  if (IsCrcGood) {
    Host->nvram.mem.Crc16 = 0;
    Status = CalculateCrc16 (
      (VOID *) &Host->nvram.mem,
      sizeof (Host->nvram.mem),
      &Host->nvram.mem.Crc16
      );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      Host->nvram.mem.Crc16 = 0xFFFF;
    }
  } else {
    Host->nvram.mem.Crc16 = Crc16;  // restore original CRC
  }

  RcFreePool (ImcRasS3SaveData);

  DEBUG ((EFI_D_INFO, "LoadImcS3RasData: exiting\n"));

} // LoadImcS3RasData

/**
  Load saved training data.

  @param[in]  Host        - Pointer to SysHost structure.

  @retval None

**/

VOID
LoadMemNvram (
  IN PSYSHOST  Host
  )
{
  EFI_STATUS        Status;
  SYS_SETUP         *Setup = NULL;
  MEMORY_POLICY_PPI *MemSetupPpi = NULL;
  BOOLEAN           FspBinary = FALSE;

  Status = PeiServicesLocatePpi (
             &gMemoryPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &MemSetupPpi
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    DEBUG ((EFI_D_ERROR, "LoadMemNvram: PeiServicesLocatePpi not found\n"));
    return;
  }

  Setup = MemSetupPpi->SysSetup;

   if ((Setup->mem.options & ATTEMPT_FAST_BOOT)      ||
       (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD) ||
       (GetSysBootMode () == S3Resume)               ||
       (IsAdrHandlingNeeded (Setup))) {

    DEBUG ((EFI_D_INFO, "LoadMemNvram: DOING loading memNvram\n"));

    //
    // FSP Binary builds do not compress the variable. Note that FSP Binary
    // builds are not the same thing as regular FSP builds. FSP Binary builds
    // only consist of PEI code, and it is up to the user to handle save of Nvram
    // since the S3NvramSave DXE driver is not executed. Standard FSP builds do
    // execute the S3NvramSave path.
    //

#if (FixedPcdGetBool (PcdFspBinaryEnable) == 1)

    if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
      FspBinary = TRUE;
    }

#endif // #if (FixedPcdGetBool (PcdFspBinaryEnable) == 1)

    if (FspBinary == TRUE) {

      Status = LoadUncompressedVariable (L"MemoryConfig", gEfiMemoryConfigDataGuid, &(Host->nvram), sizeof (Host->nvram));
      if (EFI_ERROR (Status)) {

        //
        // FSP Binary builds return EFI_INVALID_PARAMETER when the variable is
        // is not found. This and EFI_NOT_FOUND are not a fatal errors, the
        // variable simply may not be there. But other errors are fatal.
        //

        if ((Status != EFI_NOT_FOUND) &&
            (Status != EFI_INVALID_PARAMETER)) {
          DEBUG ((EFI_D_ERROR, "LoadMemNvram: Unexpected LoadUncompressedVariable failure, Status = %r\n", Status));
          ASSERT (FALSE);
          return;
        }

      }

    } else {

      Status = LoadCompressedVariable (L"MemoryConfig", gEfiMemoryConfigDataGuid, &(Host->nvram), sizeof (Host->nvram));
      if (EFI_ERROR (Status)) {

        //
        // EFI_NOT_FOUND is not a fatal error, the variable simply may not be there. But
        // Other errors are fatal.
        //

        if (Status != EFI_NOT_FOUND) {
          DEBUG ((EFI_D_ERROR, "LoadMemNvram: Unexpected LoadCompressedVariable failure, Status = %r\n", Status));
          ASSERT (FALSE);
          return;
        }
      }

      if (GetSysBootMode() == S3Resume) {
        DEBUG ((EFI_D_INFO, "LoadMemNvram: Boot mode is S3Resume\n"));
        LoadImcS3RasData (Host);
      } else {
        DEBUG ((EFI_D_INFO, "LoadMemNvram: Not loading S3 path\n"));
      }

    }

   } else {
     DEBUG ((EFI_D_INFO, "LoadMemNvram: skipping loading memNvram\n"));
   }

  DEBUG ((EFI_D_INFO, "LoadMemNvram: exiting\n"));

} // LoadMemNvram

/**
  Load saved System data.

  Load the saved System Info from Nvram, if present. Not being
  present is not an error, and does not assert. If other error
  cases are encountered, this function will assert, but must
  return

  @param - None

  @retval None

**/

EFI_STATUS
LoadSysInfoNvram (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariablePpi;
  SYS_INFO_VAR_NVRAM              *SysInfoTablePtr = NULL;
  SYS_INFO_VAR_NVRAM              *SysInfoVariablePtr = NULL;
  UINTN                           SysInfoVariableSize;
  UINT32                          VarAttrib;

  //
  // Get the variable access PPI
  //

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiVariablePpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "LoadSysInfoNvram: gEfiPeiReadOnlyVariable2PpiGuid not found\n"));
    ASSERT (FALSE);
    return Status;
  }

  SysInfoVariableSize = sizeof (SYS_INFO_VAR_NVRAM);

  //
  // Allocate memory to hold the saved variable
  //

  SysInfoVariablePtr = (SYS_INFO_VAR_NVRAM *)RcAllocatePool (SysInfoVariableSize);
  if (SysInfoVariablePtr == NULL) {
    DEBUG ((EFI_D_ERROR,"LoadSysInfoNvram: Allocating memory failed\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  VarAttrib = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;

  Status = PeiVariablePpi->GetVariable (
            PeiVariablePpi,
            SYS_INFO_NVRAM_VAR_NAME,
            &gEfiSysInfoVarNvramDataGuid,
            &VarAttrib,
            &SysInfoVariableSize,
            SysInfoVariablePtr
            );
  if (EFI_ERROR (Status)) {

    //
    // Note that not finding the variable is not necessarily a real error,
    // if may be that the variable simply is not present. We would expect
    // that Status == EFI_NOT_FOUND, so log Status unless something else occurred.
    //

    DEBUG ((EFI_D_INFO, "LoadSysInfoNvram: Did not find saved SysInfo in Nvram, Status = %r\n", Status));
    RcFreePool (SysInfoVariablePtr);
    return Status;
  }

  DEBUG ((EFI_D_INFO, "LoadSysInfoNvram: Found saved SysInfo in Nvram\n"));

  //
  // Check that the size of the saved variable matches the expected size
  //

  if (SysInfoVariableSize != sizeof (SYS_INFO_VAR_NVRAM)) {
    DEBUG ((EFI_D_ERROR, "LoadSysInfoNvram: Saved SysInfo in Nvram does not match size of struct\n"));
    ASSERT (FALSE);
    RcFreePool (SysInfoVariablePtr);
    return EFI_DEVICE_ERROR;

  }

  SysInfoTablePtr = GetSysInfoVarNvramPtr ();
  if (SysInfoTablePtr == NULL) {
    DEBUG ((EFI_D_ERROR, "LoadSysInfoNvram: failed to locate SysInfo pointer from table\n"));
    ASSERT (FALSE);
    RcFreePool (SysInfoVariablePtr);
    return EFI_DEVICE_ERROR;
  }

  CopyMem (SysInfoTablePtr, SysInfoVariablePtr, sizeof (SYS_INFO_VAR_NVRAM));

  RcFreePool (SysInfoVariablePtr);

  return EFI_SUCCESS;

} // LoadSysInfoNvram

/**
  Load saved training data.

  @param  None

  @retval !EFI_SUCCESS on failure

**/

EFI_STATUS
EFIAPI
LoadNvramData (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_BOOT_MODE BootMode;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  //
  // Determine boot mode
  //

  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  ZeroMem ((UINT8 *) &Host->nvram.mem, sizeof (struct memNvram));

  if (BootMode != BOOT_WITH_DEFAULT_SETTINGS) {

    DEBUG ((EFI_D_INFO, "LoadNvramData: BootMode != BOOT_WITH_DEFAULT_SETTINGS, Loading saved Nvram data\n"));

    LoadMemNvram (Host);

    Status = LoadSysInfoNvram ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "LoadNvramData: LoadSysInfoNvram failed, Status = %r\n", Status));
      return Status;
    }

  } else {

    DEBUG ((EFI_D_INFO, "LoadNvramData: BootMode == BOOT_WITH_DEFAULT_SETTINGS, skippign loading saved Nvram data\n"));

  }

  return EFI_SUCCESS;

} // LoadNvramData
