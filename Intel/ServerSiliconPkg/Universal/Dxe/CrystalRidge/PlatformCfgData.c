/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include "CrystalRidge.h"
#include "XlateFunctions/XlateFunctions.h"
#include "PlatformCfgData.h"
#include <Include/MemMapHost.h>
#include <Library/MemMapDataLib.h>
#include <Library/KtiApi.h>
#include <Chip/Include/SysHostChip.h>
#include <Chip/Include/FnvAccessCommon.h>
#include "Fis.h"

/*****************************************************************************
 * Local definitions
 *****************************************************************************/
#if 0
#define PCDDEBUG(Expr) CRDEBUG(Expr)
#else
#define PCDDEBUG(Expr)
#endif

#define PCD_DATA_CCUR ((UINT8)(BIT0 << CFG_CUR))
#define PCD_DATA_CIN  ((UINT8)(BIT0 << CFG_IN))
#define PCD_DATA_COUT ((UINT8)(BIT0 << CFG_OUT))
#define PCD_DATA_OEM  ((UINT8)(BIT0 << CFG_OEM))
#define PCD_DATA_ALL  (PCD_DATA_CCUR | PCD_DATA_CIN | PCD_DATA_COUT | PCD_DATA_OEM)

/*****************************************************************************
 * Variables
 *****************************************************************************/
UINT8                   mNvdimmCfgCurRecordId[MAX_SOCKET][MAX_CH][MAX_DIMM];
UINT8                   mNvdimmCfgCoutRecordId[MAX_SOCKET][MAX_CH][MAX_DIMM];
UINT8                   mNvdimmCfgErrorRecordId[MAX_SOCKET][MAX_CH][MAX_DIMM];
UINT8                   mNvdimmCfgDataPresent[MAX_SOCKET][MAX_CH][MAX_DIMM];
extern MEMORY_MAP_HOST  *mMemMapHost;
extern CR_INFO          mCrInfo;


/*++

  Update the platform configuration data for each NGN DIMM

  @param None

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

++*/
EFI_STATUS
UpdatePcd (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT8                         Skt;
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT8                         MaxChDdr;
  UINT8                         *CfgCurRecBuffer = NULL;
  UINT32                        CfgCurRecLength = 0;
  UINT8                         *CfgOutRecBuffer = NULL;
  UINT32                        CfgOutRecLength = 0;
  BOOLEAN                       CurrentMailbox;
  NVDIMM                        *NvmDimm;
  BOOLEAN                       OutputPayloadValid;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "UpdatePcd()\n"));

  //
  // Force BIOS Mailbox, needed for updating BIOS partition
  //
  CurrentMailbox = mUseOSMailbox;
  mUseOSMailbox = FALSE;

  //
  // Load the CfgCur and CfgOut records and create indices
  //
  Status = LoadCfgRecordsForPCDUpdates ();
  if (Status != EFI_SUCCESS) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Configuration records not found - no update needed\n"));
    goto GetOut;
  }

  //
  // Update platform configuration data for each DIMM
  //
  MaxChDdr = GetMaxChDdr ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        // Skip if media is disabled
        NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
        if (NvmDimm != NULL) {
          if (GetMediaStatus (NvmDimm) != MEDIA_READY) {
            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Skipping PCD update due to media disabled state\n", Skt, Ch, Dimm));
            continue;
          }
        } else {
          continue;
        }

        //
        // Check if BIOS Partition can be updated
        //
        if (IsPcdPartitonHealthy (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION)) {
          //
          // If DIMM has an update to the CfgCur record then create the record
          //
          if (mNvdimmCfgCurRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) {
            //
            // Create the CfgCur record in the local buffer from the HOB data
            //
            CreateCfgCurRecordFromHostData (mNvdimmCfgCurRecordId[Skt][Ch][Dimm], &CfgCurRecBuffer, &CfgCurRecLength);
            //
            // Call OEM-hook to update/modify Current Config record in buffer
            //
            OemBeforeUpdatePCDBIOSPartition (Skt, Ch, Dimm, &CfgCurRecBuffer, &CfgCurRecLength);
            //
            // Display record
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: CCUR Record:\n", Skt, Ch, Dimm));
            DisplayBuffer (DEBUG_INFO, CfgCurRecBuffer, CfgCurRecLength);
          }  else {
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: No CCUR Record created\n", Skt, Ch, Dimm));
          }
          //
          // Update PCD BIOS partition
          //
          if (CfgCurRecBuffer != NULL) {
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Updating BIOS Partition\n", Skt, Ch, Dimm));
            Status = UpdatePcdBiosPartition (Skt, Ch, Dimm, CfgCurRecBuffer, CfgCurRecLength);
            if (EFI_ERROR (Status)) {
              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error updating the BIOS Partition. Status = %r\n", Skt, Ch, Dimm, Status));
              //
              // Free local buffer with CfgCur record
              //
              gBS->FreePool (CfgCurRecBuffer);
              CfgCurRecBuffer = NULL;
              CfgCurRecLength = 0;
              continue;
            }
          }
        } else {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("PCD") "S%d.C%d.D%d: BIOS partition error detected - skipping update\n", Skt, Ch, Dimm));
        }

        //
        // Check if OS Partition can be updated
        //
        if (IsPcdPartitonHealthy (Skt, Ch, Dimm, NGN_DIMM_PCD_OS_PARTITION)) {
          //
          // If DIMM has an update to the CfgCur\CfOut record then create the record
          // or if BIOS Partition error was detected, we need to updated the OS Partition
          // CfgCur status accordingly
          //
          if ((mNvdimmCfgCurRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) ||
              (mNvdimmCfgCoutRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) ||
              !IsPcdPartitonHealthy (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION)) {
            //
            // Read the platform configuration data to aid in updating the PCD OS partition
            //
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Read the OS Partition to aid in updating CCUR record\n", Skt, Ch, Dimm));
            Status = GetPCD (Skt, Ch, Dimm, NGN_DIMM_PCD_OS_PARTITION, PCD_DATA_ALL, &OutputPayloadValid);
            if (EFI_ERROR (Status) || !OutputPayloadValid) {
              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error getting the OS Partition. Status = %r\n", Skt, Ch, Dimm, Status));
            } else {
              //
              // if BIOS Partition error was detected, we need to updated the OS Partition CfgCur status accordingly
              //
              if (!IsPcdPartitonHealthy (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION)) {
                CreateCfgCurRecordForBiosPartitonFailure (mNvdimmCfgErrorRecordId[Skt][Ch][Dimm], &CfgCurRecBuffer, &CfgCurRecLength);
                //
                // Display record
                //
                CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: CCUR Record:\n", Skt, Ch, Dimm));
                DisplayBuffer (DEBUG_INFO, CfgOutRecBuffer, CfgOutRecLength);
              } else {
                //
                // BIOS partition is healthy - modify CCUR records if needed (SKU Limit Violation/DIMM unmapped)
                //
                if (mNvdimmCfgCurRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) {
                  UpdateCfgCurRecordForOsPartitionUpdate (CfgCurRecBuffer);
                }
              }
              //
              // If DIMM has an update to the CfgOut record then create the record
              //
              if (mNvdimmCfgCoutRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) {
                //
                // Create the CfgOut record in the local buffer from the HOB data
                //
                CreateCfgOutRecordFromHostData (mNvdimmCfgCoutRecordId[Skt][Ch][Dimm], &CfgOutRecBuffer, &CfgOutRecLength);
                //
                // Call OEM-hook to update/modify Config Output record in buffer
                //
                OemBeforeUpdatePCDOSPartition (Skt, Ch, Dimm, &CfgOutRecBuffer, &CfgOutRecLength);
                //
                // Display record
                //
                CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: COUT Record:\n", Skt, Ch, Dimm));
                DisplayBuffer (DEBUG_INFO, CfgOutRecBuffer, CfgOutRecLength);
              } else {
                CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: No COUT Record created\n", Skt, Ch, Dimm));
              }
            }
            //
            // Update PCD OS partition
            //
            if ((CfgCurRecBuffer != NULL) || (CfgOutRecBuffer != NULL)) {
              CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Updating OS Partition\n", Skt, Ch, Dimm));
              Status = UpdatePcdOsPartition (Skt, Ch, Dimm, OutputPayloadValid, CfgCurRecBuffer, CfgCurRecLength, CfgOutRecBuffer, CfgOutRecLength);
              if (EFI_ERROR (Status)) {
                CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error updating the OS Partition. Status = %X\n", Skt, Ch, Dimm, Status));
              }
            }
          }
        } else {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("PCD") "S%d.C%d.D%d: OS partition error detected - skipping update\n", Skt, Ch, Dimm));
        }

        // Free local buffer with CfgCur record
        if (CfgCurRecBuffer != NULL) {
          gBS->FreePool (CfgCurRecBuffer);
          CfgCurRecBuffer = NULL;
          CfgCurRecLength = 0;
        }

        // Free local buffer with CfgOut record
        if (CfgOutRecBuffer != NULL) {
          gBS->FreePool (CfgOutRecBuffer);
          CfgOutRecBuffer = NULL;
          CfgOutRecLength = 0;
        }
      }
    }
  }
 GetOut:
  mUseOSMailbox = CurrentMailbox;
  return EFI_SUCCESS;
}

/*++

  Checks whether PCD partition is healthy or not

  @param[in] Skt  - Socket number
  @param[in] Ch   - Channel number
  @param[in] Dimm - DIMM number
  @param[in] Type - PCD partition ID

  @retval TRUE    - PCD partition is healthy,
  @retval FALSE   - PCD partition is not healthy

++*/
BOOLEAN
IsPcdPartitonHealthy (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 PartitionId
  )
{
  UINT8                         MaxChDdr;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  MaxChDdr = GetMaxChDdr ();

  if (Skt >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM || PartitionId >= NGN_DIMM_PCD_PARTITION_MAX) {
    ASSERT (FALSE);
    return FALSE;
  }

  DdrtDimmInfo = GetDdrtDimmInfo (Skt, Ch, Dimm);

  if ((mNvdimmCfgErrorRecordId[Skt][Ch][Dimm] == NGN_NOT_POPULATED) ||
      ((mNvdimmCfgErrorRecordId[Skt][Ch][Dimm] != NGN_NOT_POPULATED) && (DdrtDimmInfo->NgnCfgError.PcdPartitionError[PartitionId] == NGN_DIMM_NO_ERROR))) {
    return TRUE;
  }

  return FALSE;
}

/*++

  Retrieves PCD configuration of given type

  @param  Skt         - Socket number
  @param  Ch          - Channel number
  @param  Dimm        - DIMM number
  @param  Type        - PCD configuration type (CCUR/COUT/CIN/OEM)
  @param  Data        - PCD Data

  @retval EFI_INVALID_PARAMETER - PCD Data is null / configuration type not supported
  @retval EFI_NOT_FOUND         - PCD configuration data not found
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
GetPcdConfigurationData (
  UINT8 Skt,
  UINT8 Ch,
  UINT8 Dimm,
  UINT8 Type,
  UINT8 **Data
  )
{
  EFI_STATUS  Status;
  UINT32      ConfStartOffset = 0;
  UINT32      ConfDataSize = 0;

  if (NULL == Data) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Type) {
    case CFG_CUR:
      if (mNvdimmCfgDataPresent[Skt][Ch][Dimm] & PCD_DATA_CCUR) {
        ConfStartOffset = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->CurrentConfStartOffset;
        ConfDataSize = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->CurrentConfDataSize;
      }
      break;

    case CFG_IN:
      if (mNvdimmCfgDataPresent[Skt][Ch][Dimm] & PCD_DATA_CIN) {
        ConfStartOffset = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfRequestDataOffset;
        ConfDataSize = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfRequestDataSize;
      }
      break;

    case CFG_OUT:
      if (mNvdimmCfgDataPresent[Skt][Ch][Dimm] & PCD_DATA_COUT) {
        ConfStartOffset = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfResponseDataOffset;
        ConfDataSize = ((NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *)(mLargePayloadBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfResponseDataSize;
      }
      break;

    case CFG_OEM:
      if (mNvdimmCfgDataPresent[Skt][Ch][Dimm] & PCD_DATA_OEM) {
        ConfStartOffset = NGN_PCD_OEM_START;
        ConfDataSize = NGN_PCD_OEM_SIZE;
      }
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  if (ConfDataSize == 0) {
    return EFI_NOT_FOUND;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, ConfDataSize, Data);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR("PCD") "S%d.C%d.D%d: Memory allocation for Pcd Configuration Data retrieving failed (%r)\n", Skt, Ch, Dimm, Status));
    return Status;
  }
  CRDEBUG (( DEBUG_INFO, "Allocate Pool Buffer @ 0x%X of Length %d\n", *Data, ConfDataSize)) ;

  CopyMem (*Data, (mLargePayloadBuffer + ConfStartOffset), ConfDataSize);

  return EFI_SUCCESS;
}

/*++

  Updates the platform configuration data stored in the BIOS partition for each NGN DIMM

  @param[in] Skt              - Socket number
  @param[in] Ch               - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] CfgCurBuffer     - CfgCur record buffer
  @param[in] CfgCurRecLength  - CfgCur record length

  @retval EFI_SUCCESS         - Operation completed successfully,
                                error code otherwise

++*/
EFI_STATUS
UpdatePcdBiosPartition (
  IN UINT8  Skt,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT8  *CfgCurRecBuffer,
  IN UINT32 CfgCurRecLength
  )
{
  EFI_STATUS                              Status;
  UINT8                                   *Buffer = NULL;
  UINT8                                   *OemDataBuffer = NULL;
  UINT32                                  LengthPlatformCfgHeaderBody;
  UINT32                                  LargePayloadOutputOffset;
  NGN_DIMM_PLATFORM_CFG_HEADER            *HeaderDimmCfg;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *HeaderDimmBody;
  UINT64                                  TempOemTableId;
  BOOLEAN                                 OutputPayloadValid = FALSE;
  BOOLEAN                                 OemDataPresent = TRUE;
  NVDIMM                                  *DimmPtr;

  CRDEBUG ((DEBUG_INFO, "UpdatePcdBiosPartiton()\n" ));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (!DimmPtr) {
    return EFI_NOT_FOUND;
  }
  // Create a buffer for the record
  LengthPlatformCfgHeaderBody = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY);

  //
  // Bottom 64KB used for general PCD
  // Top 64KB reserved for OEM usage
  // Don't update if there is overlap
  //
  if ((CfgCurRecLength + LengthPlatformCfgHeaderBody) > NGN_PCD_GEN_SIZE) {
    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: PCD BIOS Partition Data Size too large.  Skipping update\n", Skt, Ch, Dimm));
    return EFI_OUT_OF_RESOURCES;
  }
  Status = gBS->AllocatePool(EfiBootServicesData, LengthPlatformCfgHeaderBody, &Buffer);
  if(EFI_ERROR (Status)) {
    goto GetOut;
  }

  // Zero buffer
  ZeroMem (Buffer, LengthPlatformCfgHeaderBody);

  TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

  // Set pointers to header and body
  HeaderDimmCfg = (NGN_DIMM_PLATFORM_CFG_HEADER *) Buffer;
  HeaderDimmBody = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (Buffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));


  // Create DIMM configuration header each time PCD is written as it may be corrupted
  HeaderDimmCfg->Signature = NGN_DIMM_CONFIGURATION_HEADER_SIG;
  HeaderDimmCfg->Length = LengthPlatformCfgHeaderBody;
  HeaderDimmCfg->Revision = GetPmemConfigHeaderRevision ();
  HeaderDimmCfg->Checksum = 0;
  CopyMem (HeaderDimmCfg->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (HeaderDimmCfg->OemId));
  CopyMem (&(HeaderDimmCfg->OemTableId),  &TempOemTableId, sizeof (HeaderDimmCfg->OemTableId));
  HeaderDimmCfg->OemRevision = EFI_ACPI_OEM_REVISION;
  HeaderDimmCfg->CreatorId = EFI_ACPI_CREATOR_ID;
  HeaderDimmCfg->CreatorRevision = EFI_ACPI_CREATOR_REVISION;

  // Update offset into the large payload region input buffer
  LargePayloadOutputOffset = LengthPlatformCfgHeaderBody;

  if (mUseWpqFlush) {
    IssueWpqFlush (DimmPtr, FALSE);
  }

  // Update CfgCur length parameter in DIMM configuration header since this is a new record
  HeaderDimmBody->CurrentConfDataSize = CfgCurRecLength;
  // Update CfgCur parameters in DIMM configuration header
  HeaderDimmBody->CurrentConfStartOffset = LargePayloadOutputOffset;


  // No CfgIn in DIMM configuration header in BIOS partiton
  HeaderDimmBody->ConfRequestDataOffset = 0;
  HeaderDimmBody->ConfRequestDataSize = 0;

  //No CfgOut in DIMM configuration header in BIOS partition
  HeaderDimmBody->ConfResponseDataOffset = 0;
  HeaderDimmBody->ConfResponseDataSize = 0;

  // Compute checksum of header
  ComputeChecksum (Buffer, LengthPlatformCfgHeaderBody, &(HeaderDimmCfg->Checksum));

  // Copy header record to large payload input region
  Status = CopyFromBufferToLargePayload (Buffer, 0, Skt, Ch, Dimm, 0, LengthPlatformCfgHeaderBody, LARGE_PAYLOAD_INPUT);
  if(EFI_ERROR (Status)) {
    goto GetOut;
  }

  // Copy current configuration buffer to large payload input region
  Status = CopyFromBufferToLargePayload(CfgCurRecBuffer, 0, Skt, Ch, Dimm, LargePayloadOutputOffset, CfgCurRecLength, LARGE_PAYLOAD_INPUT);
  if(EFI_ERROR (Status)) {
    goto GetOut;
  }

  // Check if OEM Data is present
  if (!(mNvdimmCfgDataPresent[Skt][Ch][Dimm] & PCD_DATA_OEM)) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Read the BIOS Partition to get OEM record\n", Skt, Ch, Dimm));
    Status = GetPCD (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION, PCD_DATA_OEM, &OutputPayloadValid);
    if (EFI_ERROR (Status) || !OutputPayloadValid) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error getting the BIOS Partition. Status = %r\n",Skt, Ch, Dimm, Status));
      OemDataPresent = FALSE;
    }
  }

  if(OemDataPresent) {
    // Retrieve and copy existing OEM data
    Status = GetPcdConfigurationData(Skt, Ch, Dimm, CFG_OEM, &OemDataBuffer);
    if(EFI_ERROR (Status)) {
      goto GetOut;
    }

    // Copy OEM data to large payload input region
    Status = CopyFromBufferToLargePayload(OemDataBuffer, 0, Skt, Ch, Dimm, NGN_PCD_OEM_START, NGN_PCD_OEM_SIZE, LARGE_PAYLOAD_INPUT);
    if(EFI_ERROR (Status)) {
      goto GetOut;
    }
  } else {
    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR("PCD") "S%d.C%d.D%d: Error updating OEM BIOS data. Status = %r\n", Skt, Ch, Dimm, Status));
  }

  if (mUseWpqFlush == FALSE) {
    Status = CopyFromLargePayloadToBuffer(Skt, Ch, Dimm, 0, mLargePayloadBuffer, 0, NGN_PCD_TOTAL_SIZE,LARGE_PAYLOAD_INPUT);
    if(EFI_ERROR (Status)) {
      goto GetOut;
    }
  }

  // Write the platform configuration data
  Status = SetPCD (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION);

GetOut:
  // Free local buffers if needed
  if (Buffer != NULL) {
    gBS->FreePool(Buffer);
    Buffer = NULL;
  }

  if (OemDataBuffer != NULL) {
    gBS->FreePool(OemDataBuffer);
    OemDataBuffer = NULL;
  }
  return Status;
}

/*++

  Update the platform configuration data stored in the OS partiton for each NGN DIMM

  @param[in] Skt                - Socket number
  @param[in] Ch                 - Channel number
  @param[in] Dimm               - DIMM number
  @param[in] OutputPayloadValid - Specifies whether Output Payload has valid data
  @param[in] CfgCurRecBuffer    - CfgCur record buffer
  @param[in] CfgCurRecLength    - CfgCur record length
  @param[in] CfgOutRecBuffer    - CfgOut record buffer
  @param[in] CfgOutRecLength    - CfgOut record length

  @retval EFI_SUCCESS           - Operation completed successfully,
                                  error code otherwise

++*/
EFI_STATUS
UpdatePcdOsPartition (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT8   Dimm,
  IN BOOLEAN OutputPayloadValid,
  IN UINT8   *CfgCurRecBuffer,
  IN UINT32  CfgCurRecLength,
  IN UINT8   *CfgOutRecBuffer,
  IN UINT32  CfgOutRecLength
  )
{
  EFI_STATUS                              Status;
  UINT8                                   *Buffer;
  UINT32                                  LengthPlatformCfgHeaderBody;
  UINT32                                  LargePayloadOutputOffset;
  NGN_DIMM_PLATFORM_CFG_HEADER            *HeaderDimmCfg;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *HeaderDimmBody;
  UINT64                                  TempOemTableId;
  NVDIMM                                  *DimmPtr;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "UpdatePcdOsPartiton()\n"));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (!DimmPtr) {
    return EFI_NOT_FOUND;
  }

  // Create a buffer for the record
  LengthPlatformCfgHeaderBody = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY);
  Status = gBS->AllocatePool (EfiBootServicesData, LengthPlatformCfgHeaderBody, &Buffer);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "Allocate Pool Buffer @ 0x%X of Length %d\n", Buffer, LengthPlatformCfgHeaderBody));
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  // Zero buffer
  ZeroMem (Buffer, LengthPlatformCfgHeaderBody);

  if (mUseWpqFlush) {
    IssueWpqFlush (DimmPtr, FALSE);
  }

  // Set pointers to header and body
  HeaderDimmCfg = (NGN_DIMM_PLATFORM_CFG_HEADER *) Buffer;
  HeaderDimmBody = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (Buffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));

  if (OutputPayloadValid) {
    // Copy DIMM configuration header from large payload region to local buffer
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy DIMM configuration header from large payload region to local buffer\n", Skt, Ch, Dimm));
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, 0, Buffer, 0, LengthPlatformCfgHeaderBody, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

    //
    // Create DIMM configuration header
    //
    HeaderDimmCfg->Signature = NGN_DIMM_CONFIGURATION_HEADER_SIG;
    HeaderDimmCfg->Length = LengthPlatformCfgHeaderBody;
    CopyMem (HeaderDimmCfg->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (HeaderDimmCfg->OemId));
    CopyMem (&(HeaderDimmCfg->OemTableId), &TempOemTableId, sizeof (HeaderDimmCfg->OemTableId));
    HeaderDimmCfg->OemRevision = EFI_ACPI_OEM_REVISION;
    HeaderDimmCfg->CreatorId = EFI_ACPI_CREATOR_ID;
    HeaderDimmCfg->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  }

  //
  // If there was not PCD#1 error always update revision
  // to the current one used by BIOS
  //
  HeaderDimmCfg->Revision = GetPmemConfigHeaderRevision ();
  HeaderDimmCfg->Checksum = 0;

  // Update offset into the large payload region input buffer
  LargePayloadOutputOffset = LengthPlatformCfgHeaderBody;

  // Update the CfgCur record if there is a new record
  if ((CfgCurRecBuffer != NULL) && (CfgCurRecLength != 0)) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy CfgCur record to large payload input region\n", Skt, Ch, Dimm));
    // Copy CfgCur record to large payload input region
    Status = CopyFromBufferToLargePayload (
      CfgCurRecBuffer,
      0,
      Skt,
      Ch,
      Dimm,
      LargePayloadOutputOffset,
      CfgCurRecLength,
      LARGE_PAYLOAD_INPUT
      );
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    // Update CfgCur length parameter in DIMM configuration header since this is a new record
    HeaderDimmBody->CurrentConfDataSize = CfgCurRecLength;

    // Update CfgCur parameters in DIMM configuration header
    HeaderDimmBody->CurrentConfStartOffset = LargePayloadOutputOffset;
    LargePayloadOutputOffset += HeaderDimmBody->CurrentConfDataSize;
  }

  // Otherwise, then keep the existing CfgCur record if it exists
  else if (HeaderDimmBody->CurrentConfDataSize) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy existing CfgCur record to large payload input region\n", Skt, Ch, Dimm));

    // Copy existing CfgCur record to large payload input region
    Status = CopyLPOutputToLPInput (
      Skt,
      Ch,
      Dimm,
      HeaderDimmBody->CurrentConfStartOffset,
      Skt,
      Ch,
      Dimm,
      LargePayloadOutputOffset,
      HeaderDimmBody->CurrentConfDataSize
      );
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    // Update CfgCur parameters in DIMM configuration header
    HeaderDimmBody->CurrentConfStartOffset = LargePayloadOutputOffset;
    LargePayloadOutputOffset += HeaderDimmBody->CurrentConfDataSize;
  }

  // Keep the existing CfgIn record if it exists
  if (HeaderDimmBody->ConfRequestDataSize) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy existing CfgIn record to large payload input region\n", Skt, Ch, Dimm));

    // Copy existing CfgIn record to large payload input region
    Status = CopyLPOutputToLPInput (
      Skt,
      Ch,
      Dimm,
      HeaderDimmBody->ConfRequestDataOffset,
      Skt,
      Ch,
      Dimm,
      LargePayloadOutputOffset,
      HeaderDimmBody->ConfRequestDataSize
      );
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    // Update CfgIn parameters in DIMM configuration header
    HeaderDimmBody->ConfRequestDataOffset = LargePayloadOutputOffset;
    LargePayloadOutputOffset += HeaderDimmBody->ConfRequestDataSize;
  }

  // Update the Cfgout record if there is a new record
  if ((CfgOutRecBuffer != NULL) && (CfgOutRecLength != 0)) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy CfgOut record to large payload input region\n", Skt, Ch, Dimm));

    // Copy CfgOut record to large payload input region
    Status = CopyFromBufferToLargePayload (
      CfgOutRecBuffer,
      0,
      Skt,
      Ch,
      Dimm,
      LargePayloadOutputOffset,
      CfgOutRecLength,
      LARGE_PAYLOAD_INPUT);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    // Update CfgOut length parameter in DIMM configuration header since this is a new record
    HeaderDimmBody->ConfResponseDataSize = CfgOutRecLength;

    // Update CfgOut parameters in DIMM configuration header
    HeaderDimmBody->ConfResponseDataOffset = LargePayloadOutputOffset;
    LargePayloadOutputOffset += HeaderDimmBody->ConfResponseDataSize;
  }

  // Otherwise, then keep the existing CfgOut record if it exists
  else if (HeaderDimmBody->ConfResponseDataSize) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy existing CfgOut record to large payload input region\n", Skt, Ch, Dimm));

    // Copy existing CfgOut record to large payload input region
    Status = CopyLPOutputToLPInput (
      Skt,
      Ch,
      Dimm,
      HeaderDimmBody->ConfResponseDataOffset,
      Skt,
      Ch,
      Dimm,
      LargePayloadOutputOffset,
      HeaderDimmBody->ConfResponseDataSize
      );
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    // Update CfgCur parameters in DIMM configuration header
    HeaderDimmBody->ConfResponseDataOffset = LargePayloadOutputOffset;
    LargePayloadOutputOffset += HeaderDimmBody->ConfResponseDataSize;
  }

  // Bottom 64KB used for general PCD
  // Top 64KB reserved for OEM usage
  // Don't update if there is overlap
  if (LargePayloadOutputOffset > NGN_PCD_GEN_SIZE) {
    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: PCD OS Partition Data Size too large.  Skipping update\n", Skt, Ch, Dimm));
    return EFI_OUT_OF_RESOURCES;
  }

  // Compute checksum of header
  ComputeChecksum (Buffer, LengthPlatformCfgHeaderBody, &(HeaderDimmCfg->Checksum));

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy header record to large payload input region\n", Skt, Ch, Dimm));

  // Copy header record to large payload input region
  Status = CopyFromBufferToLargePayload (Buffer, 0, Skt, Ch, Dimm, 0, LengthPlatformCfgHeaderBody, LARGE_PAYLOAD_INPUT);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  if (mUseWpqFlush == FALSE) {
    CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, 0, mLargePayloadBuffer, 0, LargePayloadOutputOffset, LARGE_PAYLOAD_INPUT);
  }

  if (OutputPayloadValid) {
    // Copy existing OEM data to large payload input region
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Copy existing OEM data to large payload input region\n", Skt, Ch, Dimm));
    Status = CopyLPOutputToLPInput (
      Skt,
      Ch,
      Dimm,
      NGN_PCD_OEM_START,
      Skt,
      Ch,
      Dimm,
      NGN_PCD_OEM_START,
      NGN_PCD_OEM_SIZE
      );
    if (Status != EFI_SUCCESS) {
      CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error updating OEM OS data. Status = %X\n", Skt, Ch, Dimm, Status));
    } else {
      if (mUseWpqFlush == FALSE) {
        CopyFromLargePayloadToBuffer (
          Skt,
          Ch,
          Dimm,
          NGN_PCD_OEM_START,
          mLargePayloadBuffer,
          0,
          NGN_PCD_OEM_SIZE,
          LARGE_PAYLOAD_INPUT);
      }
    }
  }

  // Write the platform configuration data
  Status = SetPCD (Skt, Ch, Dimm, NGN_DIMM_PCD_OS_PARTITION);

  return Status;
}

/*++

  Writes the data in the large payload region to the specified partiton on the NGN DIMM

  @param[in] Skt         - Socket number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] PartitionId - Specified partition

  @retval EFI_SUCCESS    - Operation completed successfully,
                           error code otherwise

++*/
EFI_STATUS
SetPCD (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 PartitionId
  )
{
  EFI_STATUS                  Status;
  NVDIMM                      *NvmDimm = NULL;
  FIS_SET_PLATFORM_CONFIG_REQ Data;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Set PCD in partition %d\n", Skt, Ch, Dimm, PartitionId));

  // Get the pointer to the NGN DIMM structure
  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);

  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Send the command
  //
  if (GetMediaStatus (NvmDimm) == MEDIA_READY) {

    ZeroMem (&Data, sizeof (Data));
    Data.Bits.PartitionId = PartitionId;
    Data.Bits.InSmallPayload = FALSE;

    Status = FisRequest (Skt, Ch, Dimm, &Data.DWord[0], sizeof (Data), NULL, SET_PLATFORM_CONFIG, FIS_DEFTIMEOUT, NULL);

  } else {

    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}


/*++

  Retrieves PCD descriptor

  @param  Skt         - Socket number
  @param  Ch          - Channel number
  @param  Dimm        - DIMM number
  @param  Data        - PCD Data

  @retval EFI_INVALID_PARAMETER - PCD Data is null
  @retval EFI_DEVICE_ERROR      - PCD Descriptor retrieving failed
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
GetPcdDescriptor (
  UINT8 Skt,
  UINT8 Ch,
  UINT8 Dimm,
  UINT8 *Data
  )
{
  EFI_STATUS Status;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Get PCD Descriptor Data\n", Skt, Ch, Dimm));

  if (NULL == Data) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, 0, Data, 0, (sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY)), LARGE_PAYLOAD_OUTPUT);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/*++

  Validates PCD descriptor values

  @param  Data                   -PCD Data

  @retval EFI_INVALID_PARAMETER - PCD Data is null
  @retval EFI_DEVICE_ERROR      - PCD Descriptor validation failed
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
ValidatePcdDescriptor (
  UINT8 *Data
  )
{
  NGN_DIMM_PLATFORM_CFG_HEADER            *CfgDescriptionHeader;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *CfgDescriptionBody;
  BOOLEAN                                 Sorted;
  UINT8                                   CfgIdx;
  UINT8                                   Checksum;
  UINT64                                  CfgSize = 0;
  struct {
    UINT64  StartOffset;
    UINT64  DataSize;
  } CfgInfo[NGN_PCD_MAX_CFG], CfgInfoTmp;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "Validate PCD Descriptor Data\n"));

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CfgDescriptionHeader = (NGN_DIMM_PLATFORM_CFG_HEADER *) Data;
  CfgDescriptionBody = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (Data + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));

  // Check signature
  if (CfgDescriptionHeader->Signature != NGN_DIMM_CONFIGURATION_HEADER_SIG) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Cfg Description Header Signature invalid\n"));
    return EFI_INCOMPATIBLE_VERSION;
  }

  // Check revision
  if (GetPmemConfigHeaderRevision () != CfgDescriptionHeader->Revision) {
    //
    // Check if revision is valid
    //
    if (!IsPmemConfigHeaderRevisionCompatible (CfgDescriptionHeader->Revision)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Cfg Description Header Revision invalid\n"));
      return EFI_INCOMPATIBLE_VERSION;
    } else {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Cfg Description Header Revision compatible, but not supported\n"));
    }
  }

  // Check buffer length
  if (CfgDescriptionHeader->Length > CR_LARGE_PAYLOAD_SIZE) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Cfg Description Header Length invalid\n"));
    return EFI_INCOMPATIBLE_VERSION;
  }

  // Confirm checksum
  ComputeChecksum (Data, CfgDescriptionHeader->Length, &Checksum);
  if (Checksum != 0) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Checksum failure\n"));
    return EFI_INCOMPATIBLE_VERSION;
  }

  // Check if data sizes and offsets are valid
  CfgInfo[0].DataSize = CfgDescriptionBody->CurrentConfDataSize;
  CfgInfo[0].StartOffset = CfgDescriptionBody->CurrentConfStartOffset;

  CfgInfo[1].DataSize = CfgDescriptionBody->ConfRequestDataSize;
  CfgInfo[1].StartOffset = CfgDescriptionBody->ConfRequestDataOffset;

  CfgInfo[2].DataSize = CfgDescriptionBody->ConfResponseDataSize;
  CfgInfo[2].StartOffset = CfgDescriptionBody->ConfResponseDataOffset;

  // Sort start offsets
  do {
    Sorted = TRUE;
    for (CfgIdx = 0; CfgIdx < (NGN_PCD_MAX_CFG -1); CfgIdx++) {
      if (CfgInfo[CfgIdx].StartOffset > CfgInfo[CfgIdx + 1].StartOffset) {
        CfgInfoTmp = CfgInfo[CfgIdx];
        CfgInfo[CfgIdx] = CfgInfo[CfgIdx + 1];
        CfgInfo[CfgIdx + 1] = CfgInfoTmp;
        Sorted = FALSE;
      }
    }
  } while (!Sorted);

  for (CfgIdx = 0; CfgIdx < NGN_PCD_MAX_CFG; CfgIdx++) {
    if (CfgInfo[CfgIdx].StartOffset == 0 && CfgInfo[CfgIdx].DataSize == 0) {
      continue;
    }

    if (CfgInfo[CfgIdx].StartOffset < (sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY))) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "CfgInfo[%x] StartOffset invalid\n", CfgIdx));
      return EFI_INVALID_PARAMETER;
    }
  }

  // Check if there is no configurations data overlaps
  for (CfgIdx = 0; CfgIdx < (NGN_PCD_MAX_CFG -1); CfgIdx++) {
    if (CfgInfo[CfgIdx].StartOffset == 0 && CfgInfo[CfgIdx].DataSize == 0) {
      continue;
    }

    if ((CfgInfo[CfgIdx].StartOffset + CfgInfo[CfgIdx].DataSize) > CfgInfo[CfgIdx + 1].StartOffset) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "CfgInfo[%x] DataSize invalid\n", CfgIdx));
      return EFI_INVALID_PARAMETER;
    }
  }

  // Check configuration size
  for (CfgIdx = 0; CfgIdx < NGN_PCD_MAX_CFG; CfgIdx++) {
    if (CfgInfo[CfgIdx].StartOffset == 0 && CfgInfo[CfgIdx].DataSize == 0) {
      continue;
    }

    CfgSize += (CfgInfo[CfgIdx].StartOffset + CfgInfo[CfgIdx].DataSize);
    if (CfgSize > (sizeof (UINT8) * CR_LARGE_PAYLOAD_SIZE)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "Total CfgSize invalid\n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  return EFI_SUCCESS;
}

/*++

  Retrieves PCD configuration current / input / output data

  @param  Skt         - Socket number
  @param  Ch          - Channel number
  @param  Dimm        - DIMM number
  @param  Types       - PCD configuration types map (CCUR/COUT/CIN/OEM)
  @param  Data        - PCD Data

  @retval EFI_INVALID_PARAMETER - PCD Data is null
  @retval EFI_DEVICE_ERROR      - PCD configuration data retrieving failed
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
GetPcdConfigurationsData (
  UINT8 Skt,
  UINT8 Ch,
  UINT8 Dimm,
  UINT8 Types,
  UINT8 *Data
  )
{
  EFI_STATUS                              Status;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *CfgDescriptionBody;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Get PCD Configurations Data\n", Skt, Ch, Dimm));

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CfgDescriptionBody = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (mLargePayloadBuffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));

  if ((Types & PCD_DATA_CCUR) && (CfgDescriptionBody->CurrentConfDataSize)) {
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, CfgDescriptionBody->CurrentConfStartOffset, mLargePayloadBuffer, CfgDescriptionBody->CurrentConfStartOffset, CfgDescriptionBody->CurrentConfDataSize, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  if ((Types & PCD_DATA_CIN) && (CfgDescriptionBody->ConfRequestDataSize)) {
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, CfgDescriptionBody->ConfRequestDataOffset, mLargePayloadBuffer, CfgDescriptionBody->ConfRequestDataOffset, CfgDescriptionBody->ConfRequestDataSize, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  if ((Types & PCD_DATA_COUT) && (CfgDescriptionBody->ConfResponseDataSize)) {
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, CfgDescriptionBody->ConfResponseDataOffset, mLargePayloadBuffer, CfgDescriptionBody->ConfResponseDataOffset, CfgDescriptionBody->ConfResponseDataSize, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  if (Types & PCD_DATA_OEM) {
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, NGN_PCD_OEM_START, mLargePayloadBuffer, NGN_PCD_OEM_START, NGN_PCD_OEM_SIZE, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

  mNvdimmCfgDataPresent[Skt][Ch][Dimm] = Types;

  return EFI_SUCCESS;
}

/*++

  Reads the data from the specified partition on the NGN DIMM into the large payload region

  @param[in]  Skt                 - Socket number
  @param[in]  Ch                  - Channel number
  @param[in]  Dimm                - DIMM number
  @param[in]  PartitionId         - Specified partition
  @param[in]  Types               - PCD configuration types map (CCUR/COUT/CIN/OEM)
  @param[out] Valid               - Specifies whether Output Payload contains valid data from the PCD

  @retval EFI_SUCCESS             - Operation completed successfully,
                                    error code otherwise

++*/
EFI_STATUS
GetPCD (
  IN  UINT8   Skt,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  IN  UINT8   PartitionId,
  IN  UINT8   Types,
  OUT BOOLEAN *Valid
  )
{
  EFI_STATUS                  Status;
  NVDIMM                      *NvmDimm;
  FIS_GET_PLATFORM_CONFIG_REQ FisReq;

  if (NULL == Valid) {
    return EFI_INVALID_PARAMETER;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Get PCD from partition %d\n", Skt, Ch, Dimm, PartitionId));

  //
  // Clear data presence and validity indicators for currently processed NVDIMM
  //
  mNvdimmCfgDataPresent[Skt][Ch][Dimm] = 0;
  *Valid = FALSE;

  //
  // Get the pointer to the NVDIMM structure
  //
  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // If NVDIMM is media disabled PCD is not available.
  //
  if (GetMediaStatus (NvmDimm) != MEDIA_READY) {
    return EFI_DEVICE_ERROR;
  }

  ZeroMem (&FisReq, sizeof (FisReq));
  FisReq.Bits.PartitionId = PartitionId;
  FisReq.Bits.InSmallPayload = FALSE;
  FisReq.Bits.JustSize = FALSE;

  Status = FisRequest (Skt, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), NULL, GET_PLATFORM_CONFIG, FIS_DEFTIMEOUT, NULL);
  if (!EFI_ERROR (Status)) {
    //
    // Check if PCD data needs to be validated
    // (no need to validate when retrieving only
    // OEM part of PCD partition - no info to validate
    // consistency of this data)
    //
    if (Types != PCD_DATA_OEM) {
      Status = GetPcdDescriptor (Skt, Ch, Dimm, mLargePayloadBuffer);
      if (!EFI_ERROR (Status)) {
        Status = ValidatePcdDescriptor (mLargePayloadBuffer);
        if (!EFI_ERROR (Status)) {
          Status = GetPcdConfigurationsData (Skt, Ch, Dimm, Types, mLargePayloadBuffer);
          if (!EFI_ERROR (Status)) {
           *Valid = TRUE;
          }
        } else {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Invalid PCD configuration records for partition %d\n", Skt, Ch, Dimm, PartitionId));
          DisplayBuffer (DEBUG_ERROR, mLargePayloadBuffer, (sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY)));
        }
      }
    } else {
      Status = GetPcdConfigurationsData (Skt, Ch, Dimm, Types, mLargePayloadBuffer);
      if (!EFI_ERROR (Status)) {
       *Valid = TRUE;
      }
    }
  }
  return Status;
}

/**
  This routine converts BIOS Partition Failure Reason to CCUR configuration status code

  @param[in] BiosPartitonFailureReason   - BIOS Partition Failure Reason

  @retval CCUR configuration status code

**/
UINT16
GetCfgCurStatusForBiosPartitonFailure (
  IN UINT8 BiosPartitonFailureReason
  )
{
  switch(BiosPartitonFailureReason) {
    case NGN_DIMM_HDR_REVISION_ERROR:
    case NGN_DIMM_CCUR_REVISION_ERROR:
    case NGN_DIMM_SIGNATURE_ERROR:
    case NGN_DIMM_DATA_OFFSET_ERROR:
    case NGN_DIMM_DATA_SIZE_ERROR:
    case NGN_DIMM_DATA_ERROR:
    case NGN_DIMM_CHECKSUM_ERROR:
      //
      // Use checksum error code to indicate data errors
      //
      return NGN_CFGCUR_RESPONSE_CCUR_CHECKSUM_NOT_VALID;

    case NGN_DIMM_COMM_FAILED:
    case NGN_DIMM_MEDIA_ERROR:
      //
      // Use undefined error for media error/DIMM communication failed
      // (we might hit this error code in case of interim error
      // when PCD was retrieved via SMBUS)
      //
      return NGN_CFGCUR_UNDEFINED;


    default:
      //
      // PCD partition failure error unimplemented
      //
      ASSERT (FALSE);
      return NGN_CFGCUR_UNDEFINED;
  }
}

/*++

  Creates the CCUR record for OS partition update when BIOS partition failure was detected.

  @param[in]  Index           - Index to CfgCurRecord record containing the CfgCur data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgCur record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

++*/
VOID
CreateCfgCurRecordForBiosPartitonFailure (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  )
{
  EFI_STATUS                          Status;
  UINT16                              LengthCfgCur;
  UINT8                               *Buffer;
  UINT8                               Skt;
  UINT8                               Ch;
  UINT8                               Dimm;
  NGN_DIMM_PLATFORM_CFG_HEADER        *HeaderCfgCur;
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY  *BodyCfgCur;
  DDRT_DIMM_DEVICE_INFO_STRUCT        *DdrtDimmInfo;
  NGN_DIMM_CFG_ERROR_HOST             *RecordCfgErr;
  UINT64                              TempOemTableId;

  if (CfgRecord == NULL || CfgRecordLength == NULL) {
    ASSERT (FALSE);
    return;
  }

  if (Index >= NGN_DIMM_MAX_NUM_OF_CFG_RECORDS) {
    ASSERT (FALSE);
    *CfgRecord = NULL;
    *CfgRecordLength = 0;
    return;
  }

  // Create local variables to reference DIMM
  Skt = mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Socket;
  Ch = mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Channel;
  Dimm = mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Dimm;

  // Create a pointer to the data for this record that was stored in MemMapHost
  DdrtDimmInfo = GetDdrtDimmInfo (Skt, Ch, Dimm);
  RecordCfgErr = &(DdrtDimmInfo->NgnCfgError);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: CreateCfgCurRecordForBiosPartitonFailure ()\n", Skt, Ch, Dimm));

  Status = GetPcdConfigurationData (Skt, Ch, Dimm, CFG_CUR, &Buffer);
  if (!EFI_ERROR (Status)) {
    //
    // Update status in existing CCUR data
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Update configuration status in existing CCUR\n", Skt, Ch, Dimm));

    ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(Buffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfigurationStatus = GetCfgCurStatusForBiosPartitonFailure (RecordCfgErr->PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION]);

    //
    // Calculate new checksum
    //
    ((NGN_DIMM_PLATFORM_CFG_HEADER*)Buffer)->Checksum = 0;
    ComputeChecksum (Buffer, ((NGN_DIMM_PLATFORM_CFG_HEADER*)Buffer)->Length, &(((NGN_DIMM_PLATFORM_CFG_HEADER*)Buffer)->Checksum));

    //
    // Update return values
    //
    *CfgRecord = Buffer;
    *CfgRecordLength = ((NGN_DIMM_PLATFORM_CFG_HEADER*)Buffer)->Length;
  } else {
    //
    // If CCUR can't be retrieved we need to create the new record with the proper error code,
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Create new CfgCur\n", Skt, Ch, Dimm));

    //
    // Compute the size of the CfgCur record.  Size includes the header and body plus size for any associated interleaves
    //
    LengthCfgCur = sizeof( NGN_DIMM_PLATFORM_CFG_HEADER ) + sizeof( NGN_DIMM_PLATFORM_CFG_CURRENT_BODY ) ;

    //
    // Create a buffer for the record
    //
    Status = gBS->AllocatePool (EfiBootServicesData, LengthCfgCur * sizeof(UINT8), &Buffer);
    CRDEBUG ((DEBUG_INFO, "Allocate Pool Buffer @ 0x%X of Length %d\n", Buffer, LengthCfgCur));
    if (!EFI_ERROR (Status)) {
      //
      // Zero buffer
      //
      ZeroMem (Buffer, LengthCfgCur) ;

      TempOemTableId  = PcdGet64(PcdAcpiDefaultOemTableId);

      // Create header in buffer
      HeaderCfgCur = (NGN_DIMM_PLATFORM_CFG_HEADER * )Buffer;
      HeaderCfgCur->Signature = NGN_DIMM_CURRENT_CONFIG_SIG;
      HeaderCfgCur->Length = LengthCfgCur;
      HeaderCfgCur->Revision = GetPmemPcdRecordsHeaderRevision ();
      CopyMem (HeaderCfgCur->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof(HeaderCfgCur->OemId));
      CopyMem (&(HeaderCfgCur->OemTableId), &TempOemTableId, sizeof(HeaderCfgCur->OemTableId));
      HeaderCfgCur->OemRevision = EFI_ACPI_OEM_REVISION;
      HeaderCfgCur->CreatorId = EFI_ACPI_CREATOR_ID;
      HeaderCfgCur->CreatorRevision = EFI_ACPI_CREATOR_REVISION;

      //
      // Create body in buffer
      //
      BodyCfgCur = (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY * )(Buffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER));
      BodyCfgCur->ConfigurationStatus = GetCfgCurStatusForBiosPartitonFailure (RecordCfgErr->PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION]);

      //
      // Update checksum
      //
      ComputeChecksum(Buffer, LengthCfgCur, &(HeaderCfgCur->Checksum));

      // Update return values
      *CfgRecord = Buffer;
      *CfgRecordLength = LengthCfgCur;
    } else {
      CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR("PCD") "S%d.C%d.D%d: Memory allocation for CCUR record creation failed (%r)\n", Skt, Ch, Dimm));
    }
  }
}

/**

  Updates the CCUR record for OS Partition update.

  @param[in] CfgRecord       - Buffer with CfgCur record data

  @retval None

++*/
VOID
UpdateCfgCurRecordForOsPartitionUpdate (
  IN UINT8   *CfgRecord
  )
{
  if (NULL == CfgRecord) {
    ASSERT (FALSE);
    return;
  }

  switch (((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(CfgRecord + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfigurationStatus) {
    case NGN_CFGCUR_RESPONSE_POPULATION_ISSUE:
    case NGN_CFGCUR_RESPONSE_SKU_LIMIT_VIOLATION:
      //
      // For those two cases we need to modify OS Partition CCUR to clear VolatileMemSizeIntoSpa and PersistentMemSizeIntoSpa
      // (we need to keep those value on BIOS Partition CCUR in case we need to restore configuration e.g. Memory POR Enforcement disabled etc.)
      //
      ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(CfgRecord + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->PersistentMemSizeIntoSpa = 0;
      ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(CfgRecord + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->VolatileMemSizeIntoSpa = 0;
      //
      // Calculate new checksum
      //
      ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum = 0;
      ComputeChecksum (CfgRecord, ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Length, &(((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum));
      return;

    case NGN_CFGCUR_RESPONSE_PM_NOT_MAPPED:
      //
      // Clear size of persistent memory mapped into the SPA
      //
      ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(CfgRecord + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->PersistentMemSizeIntoSpa = 0;
      //
      // Calculate new checksum
      //
      ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum = 0;
      ComputeChecksum (CfgRecord, ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Length, &(((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum));
      return;

    case NGN_CFGCUR_RESPONSE_VOL_MEM_NOT_MAPPED:
      //
      // Clear size of volatile memory mapped into the SPA
      //
      ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(CfgRecord + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->VolatileMemSizeIntoSpa = 0;
      //
      // Calculate new checksum
      //
      ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum = 0;
      ComputeChecksum (CfgRecord, ((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Length, &(((NGN_DIMM_PLATFORM_CFG_HEADER*)CfgRecord)->Checksum));
      return;

    default:
      return;
  }
}

/**

  Creates the CCUR record from the MemMapHost data.

  @param[in]  Index           - Index to CfgCurRecord record containing the CfgCur data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgCur record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

++*/
VOID
CreateCfgCurRecordFromHostData (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  )
{
  UINT8                                IntIndex = 0;
  UINT8                                IntDimm;
  UINT8                                indexInterleaveDIMM;
  UINT8                                *buffer;
  UINT32                               lengthCfgCur;
  UINT32                               BufferOffset = 0;
  NGN_DIMM_CFG_CUR_HOST                *RecordCfgCur;
  NGN_DIMM_PLATFORM_CFG_HEADER         *HeaderCfgCur;
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY   *BodyCfgCur;
  NGN_PCAT_HEADER                      *PcatHeader;
  UINT32                               PcatLength;
  UINT32                               PcatOffset = 0;
  UINT32                               PcatDimmInfoOffset = 0;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT *InterleaveInfoPCAT;
  NGN_DIMM_INTERLEAVE_ID               *InterleaveInfoId;
  EFI_STATUS                           Status;
  UINT64                               TempOemTableId;
  UINT8                                Skt;
  UINT8                                Ch;
  UINT8                                Dimm;
  DDRT_DIMM_DEVICE_INFO_STRUCT         *DdrtDimmInfo;
  BOOLEAN                              OutputPayloadValid = FALSE;
  BOOLEAN                              CreateRecord = FALSE;
  BOOLEAN                              CreateDimmInfo = FALSE;
  UINT8                                SocketDieCount;
  UINT8                                PcdRecHdrRev;

  if (CfgRecord == NULL || CfgRecordLength == NULL) {
    ASSERT (FALSE);
    return;
  }

  if (Index >= NGN_DIMM_MAX_NUM_OF_CFG_RECORDS) {
    ASSERT (FALSE);
    *CfgRecord = NULL;
    *CfgRecordLength = 0;
    return;
  }

  // Create local variables to reference DIMM
  Skt = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Socket;
  Ch = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Channel;
  Dimm = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Dimm;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: CreateCfgCurRecordFromHostData()\n", Skt, Ch, Dimm));

  // Create a pointer to the data for this record that was stored in MemMapHost
  DdrtDimmInfo = GetDdrtDimmInfo (Skt, Ch, Dimm);
  RecordCfgCur = &(DdrtDimmInfo->NgnCfgCur);

  //
  // Check if the CCUR was applied successfully and if this is not the fresh DIMM
  //
  if ((!DdrtDimmInfo->NewDimm) &&
      (RecordCfgCur->body.ConfigurationStatus != NGN_CFGCUR_RESPONSE_SUCCESS) &&
      (RecordCfgCur->body.ConfigurationStatus != NGN_CFGCUR_RESPONSE_CFGIN_ERROR_CCUR_USED) &&
      (RecordCfgCur->body.ConfigurationStatus != NGN_CFGCUR_RESPONSE_CFGIN_REVISION_ERROR) &&
      (RecordCfgCur->body.ConfigurationStatus != NGN_CFGCUR_UNDEFINED)) {
    //
    // If CCUR wasn't applied we should not modify the existing BIOS CCUR data
    // this will allow to restore the configuration when error was caused by
    // the interleave set breaking (removed or swapped DIMMs)
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: CfgCur was not applied (error code 0x%X)\n",
        Skt, Ch, Dimm, RecordCfgCur->body.ConfigurationStatus));

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "S%d.C%d.D%d: Read the BIOS Partition to get existing CfgCur record\n",
        Skt, Ch, Dimm));

    Status = GetPCD (Skt, Ch, Dimm, NGN_DIMM_PCD_BIOS_PARTITION, PCD_DATA_ALL, &OutputPayloadValid);
    if (EFI_ERROR (Status) || !OutputPayloadValid) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PCD") "S%d.C%d.D%d: Error getting the BIOS Partition. Status = %r\n",
        Skt, Ch, Dimm, Status));
      //
      // If CCUR can't be retrieved we need to create the new record with the proper error code,
      // but configuration won't be restored if the configuration when error is caused by
      // the interleave set breaking (removed or swapped DIMMs)
      //
      CreateRecord = TRUE;
    } else {
      Status = GetPcdConfigurationData (Skt, Ch, Dimm, CFG_CUR, &buffer);
      if (!EFI_ERROR (Status)) {
        //
        // Get current revision
        //
        PcdRecHdrRev = GetPmemPcdRecordsHeaderRevision ();
        //
        // Update revision if needed (case when DIMMs with old revision were removed or swapped)
        //
        if (PcdRecHdrRev != ((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Revision) {
          //
          // Check if anything need to be added to current configuration to be compliant with current revision
          //
          if ((((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Revision == NGN_CR_SW_FW_INTERFACE_REVISION_1_00_CFG_HDR) &&
              ((PcdRecHdrRev == NGN_CR_SW_FW_INTERFACE_REVISION_2_00_CFG_HDR) ||
               (PcdRecHdrRev == NGN_CR_SW_FW_INTERFACE_REVISION_2_01_CFG_HDR))) {
            //
            // Fill DIMM location info
            //
            CreateDimmInfo = TRUE;
          }
          if ((((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Revision == NGN_CR_SW_FW_INTERFACE_REVISION_2_00_CFG_HDR) &&
              (PcdRecHdrRev == NGN_CR_SW_FW_INTERFACE_REVISION_2_01_CFG_HDR)) {
            //
            // Fill DIMM location info
            //
            CreateDimmInfo = TRUE;
          }
          //
          // Update Revision with current one
          //
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Update revision in existing CfgCur\n", Skt, Ch, Dimm));
          ((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Revision = PcdRecHdrRev;
        }

        //
        // Update status in existing CCUR data
        //
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Update configuration status in existing CfgCur\n", Skt, Ch, Dimm));
        ((NGN_DIMM_PLATFORM_CFG_CURRENT_BODY*)(buffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER)))->ConfigurationStatus = RecordCfgCur->body.ConfigurationStatus;

        //
        // Update interleave change response status and create DIMM info (if needed)
        //
        PcatLength = (((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Length - sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) - sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY));
        while (PcatLength > 0) {
          //
          // Read the PCAT table header
          //
          PcatHeader = (NGN_PCAT_HEADER*)(buffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY) + PcatOffset);
          if (PcatHeader->Type == NGN_PCAT_TYPE_DIMM_INTERLEAVE_INFORMATION) {
            if (IntIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE) {
              InterleaveInfoPCAT = (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT*)((UINT8*)PcatHeader + sizeof (NGN_PCAT_HEADER));
              //
              // Update interleave change response status - set as not processed
              //
              CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Update interleave change response status in existing CfgCur\n", Skt, Ch, Dimm));
              InterleaveInfoPCAT->InterleaveChangeResponseStatus = NGN_INT_CH_STAT_NOT_PROCESSED;
              //
              // Fill DIMM info for all DIMMs from interleave set (if needed)
              //
              if (CreateDimmInfo) {
                for (IntDimm = 0; IntDimm < InterleaveInfoPCAT->NumOfDimmsInInterleaveSet; IntDimm++) {
                  //
                  // Get DIMM index
                  //
                  indexInterleaveDIMM = mNvdimmCfgCurRecordId[RecordCfgCur->interleaveId[IntIndex][IntDimm].Socket][RecordCfgCur->interleaveId[IntIndex][IntDimm].Channel][RecordCfgCur->interleaveId[IntIndex][IntDimm].Dimm];
                  if (indexInterleaveDIMM >= NGN_DIMM_MAX_NUM_OF_CFG_RECORDS) {
                    continue;
                  }
                  //
                  // Get Socket Die count
                  //
                  SocketDieCount = GetAcpiDieCount (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket);
                  if (SocketDieCount == 0) {
                    continue;
                  }
                  //
                  // Fill DIMM info
                  //
                  InterleaveInfoId = (NGN_DIMM_INTERLEAVE_ID*)((UINT8*)PcatHeader + sizeof (NGN_PCAT_HEADER) + sizeof (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT) + PcatDimmInfoOffset);
                  InterleaveInfoId->DimmLocation.SocketId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket / SocketDieCount;
                  InterleaveInfoId->DimmLocation.DieId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket % SocketDieCount;
                  InterleaveInfoId->DimmLocation.ImcId = ConvertSktChToImc (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Channel);
                  InterleaveInfoId->DimmLocation.ChannelId = ConvertSktChToImcCh (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Channel);
                  InterleaveInfoId->DimmLocation.SlotId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Dimm;
                  PcatDimmInfoOffset += sizeof (NGN_DIMM_INTERLEAVE_ID);
                }
              }
              IntIndex++;
            }
          }
          //
          // Update already consumed PCATs length
          //
          PcatLength -= PcatHeader->Length;
          //
          // Set offset to the next PCAT (it will be used only when not all PCATs were consumed)
          //
          PcatOffset = PcatHeader->Length;
        }

        //
        // Calculate new checksum
        //
        ((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Checksum = 0;
        ComputeChecksum (buffer, ((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Length, &(((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Checksum));
        //
        // Update return values
        //
        *CfgRecord = buffer;
        *CfgRecordLength = ((NGN_DIMM_PLATFORM_CFG_HEADER*)buffer)->Length;
      } else {
        //
        // If CCUR can't be updated we need to create the new record with the proper error code,
        // but configuration won't be restored if the configuration when error is caused by
        // the interleave set breaking (removed or swapped DIMMs)
        //
        CreateRecord = TRUE;
      }
    }
  } else {
    CreateRecord = TRUE;
  }

  if (CreateRecord) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: Create new CfgCur\n", Skt, Ch, Dimm));
    //
    // Compute the size of the CfgCur record.  Size includes the header and body plus size for any associated interleaves
    //
    lengthCfgCur = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY);
    for (IntIndex = 0; IntIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; IntIndex++)  {
      if ((RecordCfgCur->interleave[IntIndex].RecPresent) && (RecordCfgCur->interleave[IntIndex].Valid)) {
        lengthCfgCur += sizeof (NGN_PCAT_HEADER) + sizeof (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT) + (RecordCfgCur->interleave[IntIndex].NumOfDimmsInInterleaveSet * sizeof (NGN_DIMM_INTERLEAVE_ID));
      }
    }

    //
    // Create a buffer for the record
    //
    Status = gBS->AllocatePool (EfiBootServicesData, lengthCfgCur * sizeof (UINT8), &buffer);
    CRDEBUG ((DEBUG_INFO, "Allocate Pool Buffer @ 0x%X of Length %d\n", buffer, lengthCfgCur));
    ASSERT_EFI_ERROR (Status);

    //
    // Zero buffer
    //
    ZeroMem (buffer, lengthCfgCur);

    TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

    //
    // Create header in buffer
    //
    HeaderCfgCur = (NGN_DIMM_PLATFORM_CFG_HEADER * ) buffer;
    HeaderCfgCur->Signature = NGN_DIMM_CURRENT_CONFIG_SIG;
    HeaderCfgCur->Length = lengthCfgCur;
    HeaderCfgCur->Revision = GetPmemPcdRecordsHeaderRevision ();
    CopyMem (HeaderCfgCur->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (HeaderCfgCur->OemId));
    CopyMem (&(HeaderCfgCur->OemTableId), &TempOemTableId, sizeof (HeaderCfgCur->OemTableId));
    HeaderCfgCur->OemRevision = EFI_ACPI_OEM_REVISION;
    HeaderCfgCur->CreatorId = EFI_ACPI_CREATOR_ID;
    HeaderCfgCur->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
    BufferOffset = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER);

    //
    // Create body in buffer
    //
    BodyCfgCur = (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY * ) (buffer + BufferOffset);
    BodyCfgCur->ConfigurationStatus = RecordCfgCur->body.ConfigurationStatus;
    BodyCfgCur->VolatileMemSizeIntoSpa = (UINT64) RecordCfgCur->body.VolatileMemSizeIntoSpa << 26;
    BodyCfgCur->PersistentMemSizeIntoSpa = (UINT64) RecordCfgCur->body.PersistentMemSizeIntoSpa << 26;
    BufferOffset += sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY);

    //
    // Create interleave information tables
    //
    for (IntIndex = 0; IntIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; IntIndex++)  {

      if ((RecordCfgCur->interleave[IntIndex].RecPresent) && (RecordCfgCur->interleave[IntIndex].Valid)) {

        PcatHeader = (NGN_PCAT_HEADER * ) (buffer + BufferOffset);
        PcatHeader->Type = NGN_PCAT_TYPE_DIMM_INTERLEAVE_INFORMATION;
        PcatHeader->Length = sizeof (NGN_PCAT_HEADER) + sizeof (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT) + (RecordCfgCur->interleave[IntIndex].NumOfDimmsInInterleaveSet * sizeof (NGN_DIMM_INTERLEAVE_ID));
        BufferOffset += sizeof (NGN_PCAT_HEADER);

        InterleaveInfoPCAT = (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT * ) (buffer + BufferOffset);
        InterleaveInfoPCAT->InterleaveSetIndex = RecordCfgCur->interleave[IntIndex].InterleaveSetIndex;
        InterleaveInfoPCAT->NumOfDimmsInInterleaveSet = RecordCfgCur->interleave[IntIndex].NumOfDimmsInInterleaveSet;
        InterleaveInfoPCAT->InterleaveMemoryType = RecordCfgCur->interleave[IntIndex].InterleaveMemoryType;
        InterleaveInfoPCAT->InterleaveSize = RecordCfgCur->interleave[IntIndex].InterleaveSize;
        InterleaveInfoPCAT->InterleaveChangeResponseStatus = RecordCfgCur->interleave[IntIndex].InterleaveChangeResponseStatus;
        BufferOffset += sizeof (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT);

        for (IntDimm = 0; IntDimm < RecordCfgCur->interleave[IntIndex].NumOfDimmsInInterleaveSet; IntDimm++)  {

          InterleaveInfoId = (NGN_DIMM_INTERLEAVE_ID * ) (buffer + BufferOffset);

          indexInterleaveDIMM = mNvdimmCfgCurRecordId[RecordCfgCur->interleaveId[IntIndex][IntDimm].Socket][RecordCfgCur->interleaveId[IntIndex][IntDimm].Channel][RecordCfgCur->interleaveId[IntIndex][IntDimm].Dimm];
          if (indexInterleaveDIMM < NGN_DIMM_MAX_NUM_OF_CFG_RECORDS) {
            InterleaveInfoId->DimmUniqueId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].DimmUniqueId;
            SocketDieCount = GetAcpiDieCount (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket);
            InterleaveInfoId->DimmLocation.SocketId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket / SocketDieCount;
            InterleaveInfoId->DimmLocation.DieId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Socket % SocketDieCount;
            InterleaveInfoId->DimmLocation.ImcId = ConvertSktChToImc (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Channel);
            InterleaveInfoId->DimmLocation.ChannelId = ConvertSktChToImcCh (mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Channel);
            InterleaveInfoId->DimmLocation.SlotId = mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][indexInterleaveDIMM].Dimm;
            InterleaveInfoId->PartitionOffset = (UINT64) RecordCfgCur->interleaveId[IntIndex][IntDimm].PartitionOffset << 26;
            InterleaveInfoId->PartitionSize = (UINT64) RecordCfgCur->interleaveId[IntIndex][IntDimm].PartitionSize << 26;
            BufferOffset += sizeof (NGN_DIMM_INTERLEAVE_ID);
          }
        }
      }
    }

    // Update checksum
    ComputeChecksum (buffer, lengthCfgCur, &(HeaderCfgCur->Checksum));

    // Update return values
    *CfgRecord = buffer;
    *CfgRecordLength = lengthCfgCur;
  }
}

/**

  Creates the COUT record from the MemMapHost data.

  @param[in] Index            - Index to CfgOutRecord record containing the CfgOut data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgOut record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

**/
VOID
CreateCfgOutRecordFromHostData (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  )
{
  UINT8                                  IntIndex;
  UINT8                                  *Buffer;
  UINT8                                  Skt;
  UINT8                                  Ch;
  UINT8                                  Dimm;
  UINT16                                 LengthCfgOut;
  UINT16                                 BufferOffset;
  UINT16                                 TempBufferOffset;
  UINT16                                 LengthPlatformCfgHeaderBody;
  UINT32                                 ConfRequestDataOffset;
  UINT32                                 ConfRequestDataSize;
  UINT32                                 ConfRequestOffsetVariableBody;
  NGN_DIMM_CFG_OUT_HOST                  *RecordCfgOut;
  NGN_DIMM_PLATFORM_CFG_HEADER           *headerCfgOut;
  NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY      *bodyCfgOut;
  NGN_PCAT_HEADER                        *HeaderPCAT;
  NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT    *partitionSizePCAT;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT   *InterleaveInfoPCAT;
  NGN_DIMM_PLATFORM_CFG_HEADER           *headerDIMMCfg;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *headerDIMMBody;
  EFI_STATUS                             Status;
  UINT64                                 TempOemTableId;
  DDRT_DIMM_DEVICE_INFO_STRUCT           *DdrtDimmInfo;

  if (CfgRecord == NULL || CfgRecordLength == NULL) {
    ASSERT (FALSE);
    return;
  }

  if (Index >= NGN_DIMM_MAX_NUM_OF_CFG_RECORDS) {
    ASSERT (FALSE);
    *CfgRecord = NULL;
    *CfgRecordLength = 0;
    return;
  }

  // Create local variables to reference DIMM
  Skt = mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Socket;
  Ch = mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Channel;
  Dimm = mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Dimm;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("PCD") "S%d.C%d.D%d: CreateCfgOutRecordFromHostData()\n", Skt, Ch, Dimm));

  // Create a buffer for the record
  LengthPlatformCfgHeaderBody = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY);
  Status = gBS->AllocatePool (EfiBootServicesData, LengthPlatformCfgHeaderBody, &Buffer);
  CRDEBUG ((DEBUG_INFO, "Allocate Pool Buffer @ 0x%X of Length %d\n", Buffer, LengthPlatformCfgHeaderBody));
  ASSERT_EFI_ERROR (Status);

  // Zero Buffer
  ZeroMem (Buffer, LengthPlatformCfgHeaderBody);

  // Copy DIMM configuration header from large payload region to local Buffer
  Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, 0, Buffer, 0, LengthPlatformCfgHeaderBody, LARGE_PAYLOAD_OUTPUT);
  ASSERT (Status == EFI_SUCCESS);

  // Set pointers to header and body
  headerDIMMCfg = (NGN_DIMM_PLATFORM_CFG_HEADER *) Buffer;
  headerDIMMBody = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (Buffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));

  // Determine offset and size of configuration input record
  ConfRequestDataOffset = headerDIMMBody->ConfRequestDataOffset;
  ConfRequestDataSize = headerDIMMBody->ConfRequestDataSize;

  // Free memory used for header
  gBS->FreePool (headerDIMMCfg);

  // Create a pointer to the data for this record that was stored in MemMapHost
  DdrtDimmInfo = GetDdrtDimmInfo (Skt, Ch, Dimm);
  RecordCfgOut = &(DdrtDimmInfo->NgnCfgOut);

  // Compute the size of the CfgOut record.  Size includes the header and body plus size for any associated partition request and interleaves that need to be copied form the configuration input table
  LengthCfgOut = (UINT16) ConfRequestDataSize + sizeof (NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY) - sizeof (NGN_DIMM_PLATFORM_CFG_INPUT_BODY);

  // Create a Buffer for the record
  Status = gBS->AllocatePool (EfiBootServicesData, LengthCfgOut * sizeof (UINT8), &Buffer);
  CRDEBUG ((DEBUG_INFO, "Allocate Pool Buffer @ 0x%X of Length %d\n", Buffer, LengthCfgOut));
  ASSERT_EFI_ERROR (Status);

  // Zero Buffer
  ZeroMem (Buffer, LengthCfgOut);

  TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

  // Create header in Buffer
  headerCfgOut = (NGN_DIMM_PLATFORM_CFG_HEADER *) Buffer;
  headerCfgOut->Signature = NGN_DIMM_OUTPUT_CONFIG_SIG;
  headerCfgOut->Length = LengthCfgOut;
  headerCfgOut->Revision = GetPmemPcdRecordsHeaderRevision ();
  CopyMem (headerCfgOut->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (headerCfgOut->OemId));
  CopyMem (&(headerCfgOut->OemTableId), &TempOemTableId, sizeof (headerCfgOut->OemTableId));
  headerCfgOut->OemRevision = EFI_ACPI_OEM_REVISION;
  headerCfgOut->CreatorId = EFI_ACPI_CREATOR_ID;
  headerCfgOut->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  BufferOffset = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER);

  // Create body in Buffer
  bodyCfgOut = (NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY *) (Buffer + BufferOffset);
  bodyCfgOut->SequenceNumber = RecordCfgOut->body.SequenceNumber;
  bodyCfgOut->ValidationStatus = RecordCfgOut->body.ValidationStatus;
  BufferOffset += sizeof (NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY);


  // Copy DIMM partition size change and DIMM interleave information tables from configuration input record to configuration output record
  ConfRequestOffsetVariableBody = sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_INPUT_BODY);
  if (ConfRequestDataSize > ConfRequestOffsetVariableBody) {
    Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, ConfRequestDataOffset + ConfRequestOffsetVariableBody, Buffer + BufferOffset, 0, ConfRequestDataSize - ConfRequestOffsetVariableBody, LARGE_PAYLOAD_OUTPUT);
    ASSERT (Status == EFI_SUCCESS);


    // Update partition table
    if (RecordCfgOut->partitionSize.RecPresent) {

      // Set temporary pointer to start of PCAT structures
      TempBufferOffset = BufferOffset;

      // Find DIMM partition size change table
      HeaderPCAT = (NGN_PCAT_HEADER *) (Buffer + TempBufferOffset);
      while (HeaderPCAT->Type != NGN_PCAT_TYPE_DIMM_PARTITION_SIZE_CHANGE) {
        TempBufferOffset += HeaderPCAT->Length;
        HeaderPCAT = (NGN_PCAT_HEADER *) (Buffer + TempBufferOffset);
      }

      // Update the partition size response status
      partitionSizePCAT = (NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT *) (Buffer + TempBufferOffset + sizeof (NGN_PCAT_HEADER));
      partitionSizePCAT->ResponseStatus = RecordCfgOut->partitionSize.ResponseStatus;
    }


    // Update interleave information tables
    for (IntIndex = 0, TempBufferOffset = BufferOffset; IntIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; IntIndex++)  {

      if (RecordCfgOut->interleave[IntIndex].RecPresent) {

        // Find DIMM intereleave information table
        HeaderPCAT = (NGN_PCAT_HEADER *) (Buffer + TempBufferOffset);

        while (HeaderPCAT->Type != NGN_PCAT_TYPE_DIMM_INTERLEAVE_INFORMATION) {
          TempBufferOffset += HeaderPCAT->Length;
          HeaderPCAT = (NGN_PCAT_HEADER *) (Buffer + TempBufferOffset);
        }

        // Update the interleave change response status
        InterleaveInfoPCAT = (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT *) (Buffer + TempBufferOffset + sizeof (NGN_PCAT_HEADER));
        InterleaveInfoPCAT->InterleaveChangeResponseStatus = RecordCfgOut->interleave[IntIndex].InterleaveChangeResponseStatus;

        // Advance to next PCAT header
        TempBufferOffset += HeaderPCAT->Length;
      }
    }

    // Update checksum
    ComputeChecksum (Buffer, LengthCfgOut, &(headerCfgOut->Checksum));

    // Update return values
    *CfgRecord = Buffer;
    *CfgRecordLength = LengthCfgOut;
  } else {
    *CfgRecord = NULL;
    *CfgRecordLength = 0;
  }
}

/**

  Loads Configuration Records for updating the platform configuration data for each NGN DIMM and populate the record indices.

  @param None

  @retval EFI_SUCCESS - Operation completed successfully, error code otherwise

**/
EFI_STATUS
LoadCfgRecordsForPCDUpdates (
  VOID
  )
{
  UINT8 Index;

  CRDEBUG ((DEBUG_INFO, "LoadCfgRecordsForPCDUpdates()\n" ));

  SetMem (&mNvdimmCfgCurRecordId, sizeof (mNvdimmCfgCurRecordId), NGN_NOT_POPULATED);
  SetMem (&mNvdimmCfgCoutRecordId, sizeof (mNvdimmCfgCoutRecordId), NGN_NOT_POPULATED);

  if (mMemMapHost != NULL) {
    //
    // Update the array indices for the configuration current (CCUR) records
    //
    for (Index = 0; Index < mMemMapHost->NumCfgRecords[NGN_DIMM_CFG_CUR_REC]; Index++) {
      CRDEBUG ((
        DEBUG_INFO,
        "Found CCUR Entry Sckt = %d, Ch = %d, Dimm = %d\n",
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Socket,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Channel,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Dimm
        ));

      //
      // Store record index
      //
      mNvdimmCfgCurRecordId[mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Socket][mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Channel][mMemMapHost->CfgRecords[NGN_DIMM_CFG_CUR_REC][Index].Dimm] = Index;
    }

    //
    // Update the array indices for the configuration output (COUT) records
    //
    for (Index = 0; Index < mMemMapHost->NumCfgRecords[NGN_DIMM_CFG_OUT_REC]; Index++) {
      CRDEBUG ((
        DEBUG_INFO,
        "Found COUT Entry Sckt = %d, Ch = %d, Dimm = %d\n",
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Socket,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Channel,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Dimm
        ));

      //
      // Store record index
      //
      mNvdimmCfgCoutRecordId[mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Socket][mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Channel][mMemMapHost->CfgRecords[NGN_DIMM_CFG_OUT_REC][Index].Dimm] = Index;
    }

    //
    // Update the array indices for the configuration error records
    //
    for (Index = 0; Index < mMemMapHost->NumCfgRecords[NGN_DIMM_CFG_ERROR_REC]; Index++) {
      CRDEBUG ((
        DEBUG_INFO,
        "Found CFG ERROR Entry Sckt = %d, Ch = %d, Dimm = %d\n",
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Socket,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Channel,
        mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Dimm
        ));

      //
      // Store record index
      //
      mNvdimmCfgErrorRecordId[mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Socket][mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Channel][mMemMapHost->CfgRecords[NGN_DIMM_CFG_ERROR_REC][Index].Dimm] = Index;
    }
  } else {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**

  Copies a source buffer to a destination buffer

  This function copies Length bytes from Source to Destination. It uses 8-byte writes
  if buffer addresses and size are aligned.

  @param  Destination   The pointer to the destination buffer of the memory copy
  @param  Source        The pointer to the source buffer of the memory copy
  @param  Length        The number of bytes to copy from Source to Destination

**/
VOID
CopyMemForLargePayload (
  IN VOID   *Destination,
  IN VOID   *Source,
  IN UINTN  Length
  )
{
  UINTN Index = 0;

  if (((((UINT64) Destination) % 8) == 0) && ((((UINT64) Source) % 8) == 0) && ((((UINT64) Length) % 8) == 0)) {
    for (Index = 0; Index < (Length / 8); Index++) {
      *(((volatile UINT64 *) Destination) + Index) = *(((volatile UINT64 *) Source) + Index);
    }
  } else {
    PCDDEBUG ((DEBUG_INFO, "CopyMemForLargePayload: Address or length is not 8-byte aligned. Falling back to standard CopyMem.\n"));
    CopyMem ((VOID *) Destination, (VOID *) Source, Length);
  }
}


/*++
  Copy data from the SMM large payload output region to a local buffer

  @param NvmDimm        - pointer to the Dimm structure
  @param srcOffset      - Source offset in the large payload region
  @param dstBuffer      - Destination pointer to local buffer
  @param dstOffset      - Destination offset in the local buffer
  @param length         - Number of bytes to copy
  @param largePayloadRegion - If is = LARGE_PAYLOAD_INPUT, copies from Input Payload Region, if is = LARGE_PAYLOAD_OUTPUT copies from Output Payload Region

  @retval EFI_INVALID_PARAMETER - Destination Buffer is null
  @retval EFI_NOT_FOUND         - DIMM not found
  @retval EFI_SUCCESS           - Success
++*/
EFI_STATUS CopyFromNvdimmLargePayload (
  NVDIMM   *DimmPtr,
  UINT32    srcOffset,
  UINT8    *dstBuffer,
  UINT32    dstOffset,
  UINT32    length,
  BOOLEAN   largePayloadRegion
  )
{
  UINT32  chIntSize;
  UINT32  numBytesFirstPartialLine;
  UINT32  numBytesToCopyFirstPartialLine;
  UINT32  numFullLinesToCopy;
  UINT32  numBytesToCopyLastPartialLine;
  UINT32  dstBufferOffset;
  UINT32  i;
  UINT64  srcDpaBase;
  UINT64  spa;
  EFI_STATUS  status;

  // Check destination buffer poiner
  if (dstBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify length
  if (length == 0)  {
    return EFI_SUCCESS;
  }

  // Determine interleave parameters for the large payload region
  chIntSize = mCrInfo.NvdimmInfo.ChGran;

  // Determine number of bytes on first line beginning at offset
  numBytesFirstPartialLine = chIntSize - (srcOffset % chIntSize);

  // Compute transfer sizes
  if (length <= numBytesFirstPartialLine) {
    numBytesToCopyFirstPartialLine = length;
  } else  {
    numBytesToCopyFirstPartialLine = numBytesFirstPartialLine;
  }
  numFullLinesToCopy = (length - numBytesToCopyFirstPartialLine) / chIntSize;
  numBytesToCopyLastPartialLine = (length - numBytesToCopyFirstPartialLine - (numFullLinesToCopy * chIntSize)) % chIntSize;

  // Set the DPA of the specified payload region
  if (largePayloadRegion == LARGE_PAYLOAD_INPUT) {
    if (mUseOSMailbox) {
      srcDpaBase = CR_OS_MB_LARGE_INPAYLOAD;
    } else {
      srcDpaBase = CR_BIOS_MB_LARGE_INPAYLOAD;
    }
  } else {
    if (mUseOSMailbox) {
      srcDpaBase = CR_OS_MB_LARGE_OUTPAYLOAD;
    } else {
      srcDpaBase = CR_BIOS_MB_LARGE_OUTPAYLOAD;
    }
  }

  // Initialize the buffer offset
  dstBufferOffset = dstOffset;

  // Copy first partial line
  if (numBytesToCopyFirstPartialLine) {

    // Compute SPA of lineNumber
    status = XlateDpaOffsetToSpa (DimmPtr, DimmPtr->CntrlBase, srcDpaBase + srcOffset, &spa);
    ASSERT (status == EFI_SUCCESS);

    PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
    PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", dstBuffer + dstBufferOffset, spa, numBytesToCopyFirstPartialLine));

    // Perform copy operation
    CopyMemForLargePayload ((VOID*)(dstBuffer + dstBufferOffset), (VOID * ) spa, numBytesToCopyFirstPartialLine);

    // Update srcOffset and dstBufferOffset
    srcOffset += numBytesToCopyFirstPartialLine;
    dstBufferOffset += numBytesToCopyFirstPartialLine;
  }

  // Copy full lines
  if (numFullLinesToCopy) {

    for (i = 0; i < numFullLinesToCopy; i++) {

      // Compute SPA of lineNumber
      status = XlateDpaOffsetToSpa (DimmPtr, DimmPtr->CntrlBase, srcDpaBase + srcOffset, &spa);
      ASSERT (status == EFI_SUCCESS);

      PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
      PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", dstBuffer + dstBufferOffset, spa, chIntSize));

      // Perform copy operation
      CopyMemForLargePayload ((VOID *) (dstBuffer + dstBufferOffset), (VOID *) spa, chIntSize);

      // Update srcOffset and dstBufferOffset
      srcOffset += chIntSize;
      dstBufferOffset += chIntSize;
    }
  }

  // Copy last partial line
  if (numBytesToCopyLastPartialLine) {

    // Compute SPA of lineNumber
    status = XlateDpaOffsetToSpa (DimmPtr, DimmPtr->CntrlBase, srcDpaBase + srcOffset, &spa);
    ASSERT (status == EFI_SUCCESS);

    PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
    PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", dstBuffer + dstBufferOffset, spa, numBytesToCopyLastPartialLine));

    // Perform copy operation
    CopyMemForLargePayload ((VOID *) (dstBuffer + dstBufferOffset), (VOID *) spa, numBytesToCopyLastPartialLine);
  }
  return EFI_SUCCESS;
}

/*++

  Copy data from the SMM large payload output region to a local buffer

  @param[in]  SrcSkt         - Source socket
  @param[in]  SrcCh          - Source channel
  @param[in]  SrcDimm        - Source dimm
  @param[in]  SrcOffset      - Source offset in the large payload region
  @param[out] DstBuffer      - Pointer to destination buffer.
  @param[in]  DstOffset      - Destination offset in the local buffer
  @param[in]  Length         - Number of bytes to copy
  @param[in]  LargePayloadRegion - If is = LARGE_PAYLOAD_INPUT, copies from Input Payload Region, if is = LARGE_PAYLOAD_OUTPUT copies from Output Payload Region

  @retval EFI_INVALID_PARAMETER - Destination Buffer is null
  @retval EFI_NOT_FOUND         - DIMM not found
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
CopyFromLargePayloadToBuffer (
  IN  UINT8   SrcSkt,
  IN  UINT8   SrcCh,
  IN  UINT8   SrcDimm,
  IN  UINT32  SrcOffset,
  OUT UINT8  *DstBuffer,
  IN  UINT32  DstOffset,
  IN  UINT32  Length,
  IN  BOOLEAN LargePayloadRegion
  )
{
  EFI_STATUS  Status;
  NVDIMM      *DimmPtr;

  PCDDEBUG ((DEBUG_INFO, "CopyFromLargePayloadToBuffer()\n"));

  if (DstBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Length == 0) {
    return EFI_SUCCESS;
  }

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, SrcSkt, SrcCh, SrcDimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (LargePayloadRegion) {
    case LARGE_PAYLOAD_INPUT:
      Status = CopyFromNvdimmLargePayload (DimmPtr, SrcOffset, DstBuffer, DstOffset, Length, LARGE_PAYLOAD_INPUT);
      break;

    case LARGE_PAYLOAD_OUTPUT:
      Status = CopyFromNvdimmLargePayload (DimmPtr, SrcOffset, DstBuffer, DstOffset, Length, LARGE_PAYLOAD_OUTPUT);
      if (!EFI_ERROR (Status)) {
        //
        // CR workaround:
        // perform 2nd read only for FNV & EKV (1st gen)
        //
        if (IsNvmCtrlFirstGen (DimmPtr)) {
          Status = CopyFromNvdimmLargePayload (DimmPtr, SrcOffset, DstBuffer, DstOffset, Length, LARGE_PAYLOAD_OUTPUT);
        }
      }
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }
  return Status;
}

/*++

  Copy data from a local buffer to SMM large payload region

  @param[in] srcBuffer            - Source pointer to local buffer
  @param[in] srcOffset            - Source offset in the local buffer
  @param[in] dstSkt               - Destination socket
  @param[in] dstCh                - Destination channel
  @param[in] dstDimm              - Destination dimm
  @param[in] dstOffset            - Destination offset in the large payload region
  @param[in] length               - Number of bytes to copy
  @param[in] largePayloadRegion   - If it is = LARGE_PAYLOAD_INPUT, copies to the Large Input Payload, else if = LARGE_PAYLOAD_OUTPUT, copies buffer
                                    to the Large Output Payload

  @retval EFI_NOT_FOUND           - DIMM not found
  @retval EFI_SUCCESS             - Success

++*/
EFI_STATUS
CopyFromBufferToLargePayload (
  IN UINT8   *srcBuffer,
  IN UINT32  srcOffset,
  IN UINT8   dstSkt,
  IN UINT8   dstCh,
  IN UINT8   dstDimm,
  IN UINT32  dstOffset,
  IN UINT32  length,
  IN BOOLEAN largePayloadRegion
  )
{
  UINT32  chIntSize;
  UINT32  numBytesFirstPartialLine;
  UINT32  numBytesToCopyFirstPartialLine;
  UINT32  numFullLinesToCopy;
  UINT32  numBytesToCopyLastPartialLine;
  UINT32  srcBufferOffset;
  UINT32  i;
  NVDIMM  *pDimm = NULL;
  UINT64  dstDpaBase;
  UINT64  spa;
  EFI_STATUS  status;

  PCDDEBUG ((DEBUG_INFO, "CopyBufferToLPInput()\n"));

  // Get the NVDIMM struct for the DIMM
  PCDDEBUG ((DEBUG_INFO, "Calling GetDimm(%d, %d, %d)\n", dstSkt, dstCh, dstDimm));
  pDimm = GetDimm (&mCrInfo.NvdimmInfo, dstSkt, dstCh, dstDimm, NVDIMM_FLAG_ENABLED);
  if (pDimm == NULL) {
    PCDDEBUG ((EFI_D_ERROR, "Unable to find DIMM\n"));
    return EFI_NOT_FOUND;
  }

  // Verify length
  if (length == 0)  {
    PCDDEBUG ((DEBUG_ERROR, "Length is zero\n"));
    return (EFI_SUCCESS);
  }

  // Determine interleave parameters for the large payload region
  chIntSize = mCrInfo.NvdimmInfo.ChGran;

  // Determine number of bytes on first line beginning at offset
  numBytesFirstPartialLine = chIntSize - (dstOffset % chIntSize);

  // Compute transfer sizes
  if (length <= numBytesFirstPartialLine)  {
    numBytesToCopyFirstPartialLine = length;
  } else  {
    numBytesToCopyFirstPartialLine = numBytesFirstPartialLine;
  }
  numFullLinesToCopy = (length - numBytesToCopyFirstPartialLine) / chIntSize;
  numBytesToCopyLastPartialLine = (length - numBytesToCopyFirstPartialLine - (numFullLinesToCopy * chIntSize)) % chIntSize;

  // Set the DPA of the specified payload region
  if (largePayloadRegion == LARGE_PAYLOAD_INPUT) {
    if (mUseOSMailbox) {
      dstDpaBase = CR_OS_MB_LARGE_INPAYLOAD;
    } else {
      dstDpaBase = CR_BIOS_MB_LARGE_INPAYLOAD;
    }
  } else {
    // We believe we cannot copy anything to the Large Payload Output Region,
    // return error status
    return (EFI_UNSUPPORTED);
  }

  // Initialize the buffer offset
  srcBufferOffset = srcOffset;

  // Copy first partial line
  if (numBytesToCopyFirstPartialLine)  {

    // Compute SPA
    status = XlateDpaOffsetToSpa (pDimm, pDimm->CntrlBase, (dstDpaBase + dstOffset), &spa);
    ASSERT (status == EFI_SUCCESS);

    PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
    PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", spa, srcBuffer + srcBufferOffset, numBytesToCopyFirstPartialLine));

    // Perform copy operation
    CopyMemForLargePayload ((VOID *) spa, (VOID *) (srcBuffer + srcBufferOffset), numBytesToCopyFirstPartialLine);

    // Update dstOffset and srcBufferOffset
    dstOffset += numBytesToCopyFirstPartialLine;
    srcBufferOffset += numBytesToCopyFirstPartialLine;
  }

  // Copy full lines
  if (numFullLinesToCopy)  {

    for (i = 0; i < numFullLinesToCopy; i++)  {

      // Compute SPA of lineNumber
      status = XlateDpaOffsetToSpa (pDimm, pDimm->CntrlBase, (dstDpaBase + dstOffset), &spa);
      ASSERT (status == EFI_SUCCESS);

      PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
      PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", spa, srcBuffer + srcBufferOffset, chIntSize));

      // Perform copy operation
      CopyMemForLargePayload ((VOID *) spa, (VOID *) (srcBuffer + srcBufferOffset), chIntSize);

      // Update dstOffset and srcBufferOffset
      dstOffset += chIntSize;
      srcBufferOffset += chIntSize;
    }
  }

  // Copy last partial line
  if (numBytesToCopyLastPartialLine)  {

    // Compute SPA of lineNumber
    status = XlateDpaOffsetToSpa (pDimm, pDimm->CntrlBase, (dstDpaBase + dstOffset), &spa);
    ASSERT (status == EFI_SUCCESS);

    PCDDEBUG ((DEBUG_INFO, "Translated Address is SPA = 0x%lX\n", spa));
    PCDDEBUG ((DEBUG_INFO, "Calling CopyMem (0x%lX, 0x%lX, 0x%x)\n", spa, srcBuffer + srcBufferOffset, numBytesToCopyLastPartialLine));

    // Perform copy operation
    CopyMemForLargePayload ((VOID *) spa, (VOID *) (srcBuffer + srcBufferOffset), numBytesToCopyLastPartialLine);

  }
  return EFI_SUCCESS;
}

/*++

  Copy data from SMM large payload region to SMM large payload region

  @param srcSkt       - Source socket
  @param srcCh        - Source channel
  @param srcDimm      - Source dimm
  @param srcOffset    - Source offset in the large payload region
  @param dstSkt       - Destination socket
  @param dstCh        - Destination channel
  @param dstDimm      - Destination dimm
  @param dstOffset    - Destination offset in the large payload region
  @param length       - Number of bytes to copy

  @retval EFI_NOT_FOUND         - DIMM not found
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
CopyLPOutputToLPInput (
  UINT8   SrcSkt,
  UINT8   SrcCh,
  UINT8   SrcDimm,
  UINT32  SrcOffset,
  UINT8   DstSkt,
  UINT8   DstCh,
  UINT8   DstDimm,
  UINT32  DstOffset,
  UINT32  Length
  )
{
  UINT8       Buffer[NGN_INT_BUFFER_SIZE];
  UINT32      CurSrcOffset;
  UINT32      CurDstOffset;
  UINT32      RemBytesToCopy;
  UINT32      NumBytesToCopy;
  EFI_STATUS  Status;

  PCDDEBUG ((DEBUG_INFO, CR_INFO_STR ("PCD") "CopyLPOutputToLPInput()\n"));

  RemBytesToCopy = Length;
  CurSrcOffset = SrcOffset;
  CurDstOffset = DstOffset;

  while (RemBytesToCopy) {
    if (RemBytesToCopy > NGN_INT_BUFFER_SIZE) {
      NumBytesToCopy = NGN_INT_BUFFER_SIZE;
    } else {
      NumBytesToCopy = RemBytesToCopy;
    }

    // Copy source to internal buffer
    Status = CopyFromLargePayloadToBuffer (SrcSkt, SrcCh, SrcDimm, CurSrcOffset, Buffer, 0, NumBytesToCopy, LARGE_PAYLOAD_OUTPUT);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Copy internal buffer to destination
    Status = CopyFromBufferToLargePayload (Buffer, 0, DstSkt, DstCh, DstDimm, CurDstOffset, NumBytesToCopy, LARGE_PAYLOAD_INPUT);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    RemBytesToCopy -= NumBytesToCopy;
    CurSrcOffset += NumBytesToCopy;
    CurDstOffset += NumBytesToCopy;
  }

  return EFI_SUCCESS;
}

/*++

  Compute byte checksum of a buffer

  @param[in]  srcBuffer         - Pointer to local buffer
  @param[in]  length            - Number of bytes to checksum
  @param[out] chksum            - Computed checksum

  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
ComputeChecksum (
  IN  UINT8   *srcBuffer,
  IN  UINT32  length,
  OUT UINT8   *chksum
  )
{
  UINT8 computedChksum = 0;

  PCDDEBUG ((DEBUG_INFO, "ComputeChecksum()\n"));

  if ((srcBuffer == NULL) || (length == 0)) {
    return (EFI_INVALID_PARAMETER);
  }

  while (length--) {
    computedChksum += (UINT8) *srcBuffer++;
  }

  computedChksum = ~(computedChksum) + 1;

  *chksum = computedChksum;

  return (EFI_SUCCESS);
}

/*++

  Displays a buffer of a specified length

  @param[in] DisplayType  - Display Type, DEBUG_INFO or DEBUG_ERROR
  @param[in] Buffer       - Pointer to local buffer
  @param[in] Length       - Number of bytes to display

  @retval EFI_SUCCESS - Success

++*/
EFI_STATUS
DisplayBuffer (
  IN UINT32 DisplayType,
  IN UINT8  *Buffer,
  IN UINT32 Length
  )
{
  UINT32 Index;

  if (Buffer != NULL && Length > 0) {
    Index = 0;
    do {
      CRDEBUG ((DisplayType, "%02X ", *Buffer++));
      if (++Index % 16 == 0) {
        CRDEBUG ((DisplayType, "\n"));
      }
    } while (Index < Length);

    if (Index % 16 != 0) {
      CRDEBUG ((DisplayType, "\n"));
    }
  }
  return EFI_SUCCESS;
}

/**
  Fills a target buffer with zeros

  This function fills Length bytes of Buffer with zeros. It uses 8-byte writes
  if buffer address and size are aligned.

  @param  Buffer  The pointer to the target buffer to fill with zeros
  @param  Size    The number of bytes in Buffer to fill with zeros

  @return None.
**/
VOID
ZeroMemForLargePayload (
  IN VOID   *Buffer,
  IN UINTN  Size
  )
{
  UINTN Index = 0;

  if (((((UINT64) Buffer) % 8) == 0) && ((((UINT64) Size) % 8) == 0)) {
    for (Index = 0; Index < (Size / 8); Index++) {
      *(((volatile UINT64 *) Buffer) + Index) = 0x0000000000000000;
    }
  } else {

    PCDDEBUG ((DEBUG_INFO, "ZeroMemForLargePayload: Address or length is not 8-byte aligned. Falling back to standard ZeroMem.\n"));
    ZeroMem ((VOID *) Buffer, Size);
  }
}

