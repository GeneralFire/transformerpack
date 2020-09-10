/** @file

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

#ifdef DDRT_SUPPORT
#include <MemDecode.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/MemoryServicesLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/MemRcLib.h>
#include <Library/PmemMiscLib.h>
#include <Library/KtiApi.h>

VOID
DisplayBuffer (
  PSYSHOST Host,
  UINT8 *buffer,
  UINT32 length
  )
{
  UINT32 i;

  for (i = 0 ; i < length ; i++)  {

    RcDebugPrint (SDBG_DEFAULT, "%02X ", *buffer++);
    if ((((i + 1) % 16) == 0) && (i != 0))  {
      RcDebugPrint (SDBG_DEFAULT, "\n" );
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n" );

  return;
}

/**

This routine computes the checksum of a table of given size.

@param TblStart     -  Start of the table for which checksum needs to be computed
@param BytesCount   -  Total size of the table for which checksum needs to be computed

@retval Checksum computed
@retval VOID

**/
UINT8
ChsumTbl (
  UINT8 *TblStart,
  UINT32 BytesCount
  )
{
  UINTN                          i;
  UINT8                          res = *TblStart;

  for (i = 1; i < BytesCount; i++) res = res + TblStart[i];

  return (res);
} // ChsumTbl


/**

  This routine translates the error codes related to the Partion requests to proper
  error responses defined in the spec.
  @param status - Status of the Partion Request

  @retval Value that needs to be programmed in the response field for this request
**/
UINT16
EncodePartitionResponse (UINT8 status)
{
  switch (status) {

  case PARTITIONING_SUCCESS:
    return NGN_PART_SIZE_STAT_SUCCESS;

  case INTERLEAVE_PARTICIPANT_UNAVAILABLE:
    return NGN_PART_SIZE_STAT_INTERLEAVE_PARTICIPANT_MISSING;

  case INTERLEAVE_PARAMETER_MISMATCH:
    return NGN_PART_SIZE_STAT_INTERLEAVE_SET_NOT_FOUND;

  case FNV_FW_ERROR:
    return NGN_PART_SIZE_STAT_AEP_FW_ERROR;

  case PARTITION_TOO_BIG:
    return NGN_PART_SIZE_STAT_PARTITION_SIZE_TOO_BIG;

  case  INSUFFICIENT_SILICON_RESOURCES:
    return NGN_PART_SIZE_STAT_INSUFFICIENT_SILICON_RESOURCES;

  case SECURITY_STATE_LOCKED:
  default:
    return NGN_PART_SIZE_STAT_UNDEFINED;
  }
}


/**

  This routine translates the error codes related to the Interleave  requests to proper
  error responses defined in the spec.
  @param status - Status of the Interleave Request

  @retval Value that needs to be programmed in the response field for this request
**/
UINT8
EncodeInterleaveResponse (UINT8 status)
{
  switch (status) {

  case OPERATION_SUCCESS:
    return NGN_INT_CH_STAT_SUCCESS;

  case INTERLEAVE_PARTICIPANT_UNAVAILABLE:
    return NGN_INT_CH_STAT_MATCHING_DIMM_MISSING;

  case INTERLEAVE_PARAMETER_MISMATCH:
    return NGN_INT_CH_STAT_INTERLEAVE_PARAMETERS_MISMATCH;

  case INSUFFICIENT_SILICON_RESOURCES:
    return NGN_INT_CH_STAT_INSUFFICIENT_SILICON_RESOURCES;

  case PARTITIONING_REQUEST_FAILED:
    return NGN_INT_CH_STAT_PARTITIONING_FAILED;

  case MISSING_CFG_REQUEST:
    return NGN_INT_CH_STAT_CFG_REQ_MISSING;

  case CHANNEL_INTERLEAVE_MISMATCH:
    return NGN_INT_CH_STAT_CHANNEL_INTERLEAVE_MISMATCH;

  case NM_POPULATION_MISMATCH:
    return NGN_INT_CH_STAT_UNSUPPORTED;

  case INTERLEAVE_NOT_ALIGNED:
    return NGN_INT_CH_STAT_PARTITION_OFFSET_NOT_ALIGNED;

  default:
    return NGN_INT_CH_STAT_NOT_PROCESSED;
  }
}

/** This routine encodes the interleave granularity.

  Encode interleave granularity based on the memory type and Interleave granularities in cofig. data.

  @param[in] Host             - Pointer to sysHost
  @param[in] MemType          - Memory type
  @param[in] InterleaveSize   - Interleave size

  @retval   -   Encoded interleave granularities based on the memory type.

**/
UINT8
EncodeInterleaveGranularity (
  IN PSYSHOST Host,
  IN UINT16   MemType,
  IN UINT32   InterleaveSize
  )
{
  UINT8 chIntSize, tgtIntSize;

  tgtIntSize = (UINT8) (InterleaveSize >> 8);
  chIntSize = (UINT8) InterleaveSize;

  if ((tgtIntSize == INTERLEAVE_FORMAT_4KB) && (chIntSize == INTERLEAVE_FORMAT_256B)) {
    return (MEM_INT_GRAN_CH_256B_TGT_4KB);
  } else if ((tgtIntSize == INTERLEAVE_FORMAT_4KB) && (chIntSize == INTERLEAVE_FORMAT_4KB)) {
    return (MEM_INT_GRAN_CH_4KB_TGT_4KB);
  } else if ((tgtIntSize == INTERLEAVE_FORMAT_256B) && (chIntSize ==INTERLEAVE_FORMAT_256B)) {
    return (MEM_INT_GRAN_CH_256B_TGT_4KB);
  }

  return MEM_INT_GRAN_ERROR;
}

/**

  This routine clears the interleave information structure in CCUR table
  @param Host            - Pointer to sysHost
  @param Sckt            - socket number
  @param Ch              - channel number
  @param Dimm            - Dimm number
  @param InterleaveIndex - Index to the Interleave requests

  @retval
**/
VOID
ClearCfgCurInterleaveStruct (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    InterleaveIndex
  )
{
  UINT8                                     Index;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *InterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveIdPtr;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

  InterleavePtr = &DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex];
  InterleaveIdPtr = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveIndex];

  //clear the data in the struct
  InterleavePtr->InterleaveSetIndex = 0;
  InterleavePtr->NumOfDimmsInInterleaveSet = 0;
  InterleavePtr->InterleaveMemoryType = 0;
  InterleavePtr->InterleaveSize = 0;
  InterleavePtr->InterleaveChangeResponseStatus = 0;

  //Other variables that are not part of the spec
  InterleavePtr->RecPresent = 0;
  InterleavePtr->Valid = 0;
  InterleavePtr->Processed = 0;
  InterleavePtr->SadLimit = 0;

  //clear the interleave dimm id information.
  for (Index = 0; Index < MaxChDdr; Index++) {
    InterleaveIdPtr->Socket = 0;
    InterleaveIdPtr->Channel = 0;
    InterleaveIdPtr->Dimm = 0;
    InterleaveIdPtr->PartitionOffset = 0;
    InterleaveIdPtr->PartitionSize = 0;
    InterleaveIdPtr++;
  } // Index
  return;
}

/**

  This routine clears the CCUR table and updates response parameters for new dimms.
  @param Host     - Pointer to sysHost
  @param Sckt     - socket Number
  @param Ch       - channel Number
  @param Dimm     - Dimm Number


  @retval SUCCESS
**/
UINT32
UpdateCfgCurForNewDimm (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT8    Ch,
  UINT8    Dimm
  )
{
  UINT8                         InterleaveIndex;
  struct dimmNvram              (*dimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT  *DdrtDimmInfo;
  UINT8                         DimmUnmappedReason;

  dimmNvList = &Host->nvram.mem.socket[Sckt].channelList[Ch].dimmList;
  DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

  DdrtDimmInfo->NgnCfgCurPresent = 1;
  DdrtDimmInfo->NgnCfgCurValid = 1;

  //Clear other variables.
  // in 1LM mode, VolatileMemSizeIntoSpa = 0; otherwise VolatileMemSizeIntoSpa = volCap
  if (!IS_DDR_2LM_OR_MIXED (Host)) {
    DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = 0;
  } else {
    DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = ((*dimmNvList)[Dimm].volCap - (UINT16)Host->var.mem.socket[Sckt].channelList[Ch].volRemSize);
  }
  DdrtDimmInfo->NgnCfgCur.body.PersistentMemSizeIntoSpa = 0;

  if (IsNvDimmUnmapped (Sckt, Ch, Dimm, &DimmUnmappedReason) && (DimmUnmappedReason == NVDIMM_UNMAPPED_POPULATION_NOT_POR)) {
      RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurForNewDimm: PMem is unmapped (Memory Population POR Enforcement)\n");
      DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_POPULATION_ISSUE;
  } else if (IsNvDimmUnmapped (Sckt, Ch, Dimm, &DimmUnmappedReason) &&  (DimmUnmappedReason == NVDIMM_UNMAPPED_SKU_LIMIT_VIOLATION)) {
    RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurForNewDimm: PMem is unmapped (SKU Limit Violation)\n");
    DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SKU_LIMIT_VIOLATION;
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurForNewDimm: Setting default CCUR for new DIMM\n");
    DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_PM_NOT_MAPPED;
  }

  //Update the Interleave records that they are not processed.
  for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
    if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].RecPresent) {
      DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus = NGN_INT_CH_STAT_NOT_PROCESSED;
    }
  }

  return SUCCESS;
}

/**
  This routine is to update CCUR tables after memory mapping to reflect the updated dimm configuration wherever applicable.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @return Void.
**/
VOID
UpdateCfgCurAfterMemmap (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                                     Sckt;
  UINT8                                     Ch;
  UINT8                                     Dimm;
  UINT8                                     DimmUnmappedReason;
  UINT8                                     InterleaveIndex;
  UINT8                                     j;
  MEM_NVRAM_STRUCT                          *NvramMem;
  CHANNEL_NVRAM_STRUCT                      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT                         (*DimmNvList)[MAX_DIMM];
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *SourceInterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *SourceInterleaveIdPtr;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveIdPtr;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  UINT8                                     MaxChDdr;

  NvramMem = &Host->nvram.mem;

  //If we are running in degraded memory map level, we will not honor any request from NVMDIMM mgmt driver.
  //We have handle this condition already in UpdateRequestforDegradedMemoryMapping
  if (MemMapData->MemMapDegradeLevel) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ChannelNvList = &NvramMem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

        if (IsNvDimmUnmapped (Sckt, Ch, Dimm, &DimmUnmappedReason) && (DimmUnmappedReason == NVDIMM_UNMAPPED_POPULATION_NOT_POR)) {
          if (!DdrtDimmInfo->NewDimm) {
            RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: DIMM Unmapped (Memory Population POR Enforcement)\n");
            //
            // Update only configuration status (preserve rest of the data in case when after boot, Population Enforcement POR will be disabled in BIOS Setup, so we could go back to this configuration on next boot)
            //
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_POPULATION_ISSUE;
          } else {
            //
            // This is new DIMM - and need to be unmapped - create CCUR record with proper status
            //
            UpdateCfgCurForNewDimm (Host, Sckt, Ch, Dimm);
          }
          continue;
        }

        if ((DdrtDimmInfo->NgnCfgOutPresent != 0) &&
            (DdrtDimmInfo->NgnCfgOut.body.SequenceNumber == DdrtDimmInfo->NgnCfgReq.body.SequenceNumber)) {
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: Valid COUT found, ignoring CIN\n");

          // Update the present flag. This will be used by the DXE code (valid flag is already set/cleared according to the CCUR validation results)
          DdrtDimmInfo->NgnCfgCurPresent = 1;

          // In case we are restoring the previously broken current configuration we should also update status
          if (DdrtDimmInfo->NgnCfgCurValid) {
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SUCCESS;
          }

          //
          // Check if we have an error status in the DIMM interleave and update status
          //
          for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
            if (!DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].RecPresent || !DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].Valid) {
              continue;
            }
            if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus != NGN_INT_CH_STAT_SUCCESS) {
              if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_INSUFFICIENT_SILICON_RESOURCES ||
                  DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_UNSUPPORTED) {
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_UNDEFINED;
              } else if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_CHANNEL_INTERLEAVE_MISMATCH) {
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_ALL_DIMM_NOT_FOUND;
              }
            }
          } // InterleaveIndex

          // In case we are switching from 1LM to 2LM mode
          if (!IS_DDR_2LM_OR_MIXED (Host)) {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = 0;
          } else {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = ((*DimmNvList)[Dimm].volCap - (UINT16)Host->var.mem.socket[Sckt].channelList[Ch].volRemSize);
          }
        } else if (DdrtDimmInfo->NgnCfgReqPresent) {
          // Check if a CfgIn request was not valid
          if (!DdrtDimmInfo->NgnCfgReqValid) {
            if (DdrtDimmInfo->NgnCfgCurValid) {
              if (DdrtDimmInfo->NgnCfgReqRev != GetPmemPcdRecordsHeaderRevision ()) {
                RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: CIN revision error, CCUR valid\n");
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CFGIN_REVISION_ERROR;
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: CIN invalid, CCUR valid\n");
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CFGIN_ERROR_CCUR_USED;
              }
            } else {
              RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: CIN and CCUR both invalid\n");
              DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CFGIN_ERROR_DIMM_NOT_MAPPED;
              DdrtDimmInfo->NgnCfgCur.body.PersistentMemSizeIntoSpa = 0;
              if (!IS_DDR_2LM_OR_MIXED (Host)) {
                DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = 0;
              } else {
                DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = (*DimmNvList)[Dimm].volCap;
              }
            }
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: Valid CIN found\n");

          //Update Partition information of CCUR if needed
          if (DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent && DdrtDimmInfo->NgnCfgReq.partitionSize.Valid) {
            //check if the partition request was successfully implemented by looking at the response status.
            //If not successful, we can skip updating the CCUR complete;y, because, we will not honor interleave request if partition request fails.
            if (DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus != NGN_PART_SIZE_STAT_SUCCESS) {
              continue;
            }
            //Update CCUR body with success response code
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SUCCESS;
            // Clear BIOS partition error (if any)
            DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_NO_ERROR;
            DdrtDimmInfo->NgnCfgErrorPresent = 0;
          }

          //Since this is a new request, the amount of non-volatile memory mapped into the SPA by default is zero
          DdrtDimmInfo->NgnCfgCur.body.PersistentMemSizeIntoSpa = 0;
          //Update CCUR body with size of data mapped into the SPA. For volatile memory, assume that all is mapped into the SPA.
          //For non-volatile memory, need to examine the interleave request to understand the mapping.  (convert from 64MB gran
          // to byte gran).
          // Since this is a new request; in 1LM mode, VolatileMemSizeIntoSpa = 0; otherwise VolatileMemSizeIntoSpa = volCap
          if (!IS_DDR_2LM_OR_MIXED (Host)) {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = 0;
          } else {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = ((*DimmNvList)[Dimm].volCap - (UINT16)Host->var.mem.socket[Sckt].channelList[Ch].volRemSize);
          }

          //Update the present and valid variables. This will be used by the DXE code
          DdrtDimmInfo->NgnCfgCurPresent = 1;
          DdrtDimmInfo->NgnCfgCurValid = 1;

          //Update Interleave information of CCUR if needed
          for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
            //Skip if there is no interleave request or if the request failed our validation previously
            if (!DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].RecPresent || !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].Valid) {
              ClearCfgCurInterleaveStruct (Host, Sckt, Ch, Dimm, InterleaveIndex);
              continue;
            }
            //Skip if this interleave is not processed.
            if (DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus != NGN_INT_CH_STAT_SUCCESS) {
              //Clear interleave information in the CCUR records because we haven honored an interleave request
              ClearCfgCurInterleaveStruct (Host, Sckt, Ch, Dimm, InterleaveIndex);
              continue;
            }

            //Update CCUR body with success response code
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SUCCESS;

            //
            // Check if DIMM interleave has an error in implementation and update status
            //
            if (DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus != NGN_INT_CH_STAT_SUCCESS) {
              if (DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_INSUFFICIENT_SILICON_RESOURCES ||
                  DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_UNSUPPORTED) {
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_UNDEFINED;
              } else if (DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_CHANNEL_INTERLEAVE_MISMATCH) {
                DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_ALL_DIMM_NOT_FOUND;
              }
            }

            SourceInterleavePtr = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex];
            SourceInterleaveIdPtr = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveIndex];
            DestInterleavePtr = &DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex];
            DestInterleaveIdPtr = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveIndex];

            //The non-volaitle memory contributed by this valid interleave request must be added to get the overall non-volatile memory size mapped into the SPA
            DdrtDimmInfo->NgnCfgCur.body.PersistentMemSizeIntoSpa += SourceInterleaveIdPtr->PartitionSize;

            //Fill in the required data in the Host structure
            DestInterleavePtr->InterleaveSetIndex = SourceInterleavePtr->InterleaveSetIndex;
            DestInterleavePtr->NumOfDimmsInInterleaveSet = SourceInterleavePtr->NumOfDimmsInInterleaveSet;
            DestInterleavePtr->InterleaveMemoryType = SourceInterleavePtr->InterleaveMemoryType;
            DestInterleavePtr->InterleaveSize = SourceInterleavePtr->InterleaveSize;
            DestInterleavePtr->InterleaveChangeResponseStatus = SourceInterleavePtr->InterleaveChangeResponseStatus;

            //Other variables that are not part of the spec
            DestInterleavePtr->RecPresent = SourceInterleavePtr->RecPresent;
            DestInterleavePtr->Valid = SourceInterleavePtr->Valid;
            DestInterleavePtr->Processed = SourceInterleavePtr->Processed;
            DestInterleavePtr->SadLimit = SourceInterleavePtr->SadLimit;

            //Update th interleave Dimm id information.
            for (j = 0; j < DestInterleavePtr->NumOfDimmsInInterleaveSet; j++) {
              DestInterleaveIdPtr->Socket = SourceInterleaveIdPtr->Socket;
              DestInterleaveIdPtr->Channel = SourceInterleaveIdPtr->Channel;
              DestInterleaveIdPtr->Dimm = SourceInterleaveIdPtr->Dimm;
              DestInterleaveIdPtr->PartitionOffset = SourceInterleaveIdPtr->PartitionOffset; //This data is in 64MB gran
              DestInterleaveIdPtr->PartitionSize = SourceInterleaveIdPtr->PartitionSize;   //This data is in 64MB gran
              DestInterleaveIdPtr++;
              SourceInterleaveIdPtr++;
            }//j
          } // InterleaveIndex
        } else if ((DdrtDimmInfo->NgnCfgCurPresent == 1) &&
                   (DdrtDimmInfo->NgnCfgCurValid == 1)) {
          //
          // Restore successful configuration status if previously unmapped DIMM was mapped again
          //
          if (((DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus == NGN_CFGCUR_RESPONSE_POPULATION_ISSUE) ||
               (DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus == NGN_CFGCUR_RESPONSE_PM_NOT_MAPPED) ||
              (DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus == NGN_CFGCUR_RESPONSE_SKU_LIMIT_VIOLATION)) &&
              !IsNvDimmUnmapped (Sckt, Ch, Dimm, NULL)) {
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SUCCESS;
          }

          //
          // Check if the DIMM interleave request was failed to be implemented, if so update an error.
          //
          for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
            //Skip if there is no interleave request or if the request failed our validation previously
            if (!DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].RecPresent || !DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].Valid) {
              continue;
            }
            if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_INSUFFICIENT_SILICON_RESOURCES ||
                DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_UNSUPPORTED) {
              DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_UNDEFINED;
            } else if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveIndex].InterleaveChangeResponseStatus == NGN_INT_CH_STAT_CHANNEL_INTERLEAVE_MISMATCH) {
              DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_ALL_DIMM_NOT_FOUND;
            }
          } // InterleaveIndex

          //
          // Valid CCUR already present.  Leave as-is
          //
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgCurAfterMemmap: Valid CCUR found\n");
          if (!IS_DDR_2LM_OR_MIXED (Host)) {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = 0;
          } else {
            DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = ((*DimmNvList)[Dimm].volCap - (UINT16)Host->var.mem.socket[Sckt].channelList[Ch].volRemSize);
          }
        } else if (DdrtDimmInfo->NewDimm) {
          // This is a new DIMM or the DIMM is unmapped due to population POR enforcement
          // Clear the CUR record and update the configuration status
          UpdateCfgCurForNewDimm (Host, Sckt, Ch, Dimm);
        }
      }//Dimm
    }//Ch
  }//Sckt
}

/**

  This routine copies interleave request details from CfgIn to CfgOut for a dimm
  @param Host              - Pointer to sysHost
  @param Socket            - Socket number
  @param Ch                - Channel number
  @param Dimm              - Dimm number
  @param InterleaveCounter - Count of interleave PCAT tables read.

  @retval SUCCESS
**/
UINT32
CopyInterleaveInformation (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    InterleaveCounter
  )
{
  UINT8                                     Count;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *SourceInterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *SourceInterleaveIdPtr;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveIdPtr;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  SourceInterleavePtr = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveCounter];
  SourceInterleaveIdPtr = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveCounter];
  DestInterleavePtr = &DdrtDimmInfo->NgnCfgOut.interleave[InterleaveCounter];
  DestInterleaveIdPtr = DdrtDimmInfo->NgnCfgOut.interleaveId[InterleaveCounter];

  //Fill in the required data in the Host structure
  DestInterleavePtr->RecPresent = SourceInterleavePtr->RecPresent;
  DestInterleavePtr->Valid = SourceInterleavePtr->Valid;
  DestInterleavePtr->Processed = SourceInterleavePtr->Processed;
  DestInterleavePtr->SadLimit = SourceInterleavePtr->SadLimit;
  DestInterleavePtr->InterleaveSetIndex = SourceInterleavePtr->InterleaveSetIndex;
  DestInterleavePtr->NumOfDimmsInInterleaveSet = SourceInterleavePtr->NumOfDimmsInInterleaveSet;
  DestInterleavePtr->InterleaveMemoryType = SourceInterleavePtr->InterleaveMemoryType;
  DestInterleavePtr->InterleaveSize = SourceInterleavePtr->InterleaveSize;

  //Scan for PCAT table and fill in the Host structure.
  for (Count = 0; Count < SourceInterleavePtr->NumOfDimmsInInterleaveSet; Count++) {

    DestInterleaveIdPtr->Socket = SourceInterleaveIdPtr->Socket;
    DestInterleaveIdPtr->Channel = SourceInterleaveIdPtr->Channel;
    DestInterleaveIdPtr->Dimm = SourceInterleaveIdPtr->Dimm;
    DestInterleaveIdPtr->PartitionOffset = SourceInterleaveIdPtr->PartitionOffset;
    DestInterleaveIdPtr->PartitionSize = SourceInterleaveIdPtr->PartitionSize;
    DestInterleaveIdPtr++;
    SourceInterleaveIdPtr++;
  }
  return SUCCESS;
}

/**
  This routine is to update CfgOut tables after memory mapping to update the validation status.
  It copies the contents of CFGIN requests (Partition and Interleave) into CfgOut and updates the
  response status in the cfgOut structure.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @return Void.
**/
VOID
UpdateCfgOutAfterMemmap (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                        Sckt;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT8                        InterleaveIndex;
  MEM_NVRAM_STRUCT             *NvramMem;
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  UINT8                        MaxChDdr;

  NvramMem = &Host->nvram.mem;

  //If we are running in degraded memory map level, we will not honor any request from NVMDIMM mgmt driver.
  //We have handle this condition already in UpdateRequestforDegradedMemoryMapping
  if (MemMapData->MemMapDegradeLevel) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ChannelNvList = &NvramMem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM Dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

        //Check if there are CfgIn request is present. If not skip this Dimm.
        if (!DdrtDimmInfo->NgnCfgReqPresent) {
          continue;
        }

        if ((DdrtDimmInfo->NgnCfgOutPresent != 0) && (DdrtDimmInfo->NgnCfgOut.body.SequenceNumber == (DdrtDimmInfo->NgnCfgReq.body.SequenceNumber))) {
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateCfgOutAfterMemmap: Valid COUT found, ignoring CIN\n");
          continue;
        }

        DdrtDimmInfo->NgnCfgOutPresent = 1;

        //check if we have a partition req.
        if (DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent) {
          //Copy CFGIN partition information into CFGOUT struct.
          DdrtDimmInfo->NgnCfgOut.partitionSize.RecPresent = DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent;
          DdrtDimmInfo->NgnCfgOut.partitionSize.Valid = DdrtDimmInfo->NgnCfgReq.partitionSize.Valid;
          DdrtDimmInfo->NgnCfgOut.partitionSize.PartitionSize = DdrtDimmInfo->NgnCfgReq.partitionSize.PartitionSize;

          //Note: Response status for the Partition has been updated while performing the paritioning.

          //check if the partition request is found invalid or if the request was failed to be implemented, if so update an error.
          if (( !DdrtDimmInfo->NgnCfgReq.partitionSize.Valid) || DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus != NGN_PART_SIZE_STAT_SUCCESS) {
            DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
          }
        }

        //check if we have an interleave request
        for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
          //Check if there is an interleave request present
          if (DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].RecPresent) {
            //Update CfgOut with Interleaverequest details.
            CopyInterleaveInformation (Host, Sckt, Ch, Dimm, InterleaveIndex);

            //Note: Response status for the Interleave request has been updated.

            //check if the partition request is found invalid or if the request was failed to be implemented, if so update an error.
            if ( !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].Valid || (DdrtDimmInfo->NgnCfgOut.interleave[InterleaveIndex].InterleaveChangeResponseStatus != NGN_INT_CH_STAT_SUCCESS)) {
              DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
            }
          }
        }//InterleaveIndex
      }//Dimm
    }//Ch
  }//Sckt
  return;
}

/**

  This routine is to parses through the buffer containing Current Configuration Body and updates the Host structures accordingly
  @param Host              - Pointer to sysHost
  @param Socket            - Socket number
  @param Ch                - Channel number
  @param Dimm              - Dimm number
  @param PlatformCurCfgBdy - Pointer to the Platform Current configuration table body buffer

  @retval SUCCESS
**/
UINT32
CopyCurrentCfgBody (
  PSYSHOST                           Host,
  UINT8                              Socket,
  UINT8                              Ch,
  UINT8                              Dimm,
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY *PlatformCurCfgBdy
  )
{
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = PlatformCurCfgBdy->ConfigurationStatus;
  DdrtDimmInfo->NgnCfgCur.body.VolatileMemSizeIntoSpa = (UINT16) (RShiftU64 (PlatformCurCfgBdy->VolatileMemSizeIntoSpa, CONVERT_B_TO_64MB));    //Total amount of 2LM size
  DdrtDimmInfo->NgnCfgCur.body.PersistentMemSizeIntoSpa = (UINT16) (RShiftU64 (PlatformCurCfgBdy->PersistentMemSizeIntoSpa, CONVERT_B_TO_64MB));  //Totsl amount of NV size.

  return SUCCESS;
}

/**
  This routine is to update failure condition to all the request we have received from the NVMDIMM management software.
  This routine is called during memory mapping due to silicon resource deficit.
  MEM_MAP_LVL_NORMAL - NORMAL memory mapping
  MEM_MAP_LVL_IGNORE_CFGIN   - Will not honor requests from NVMDIMM management software.
  MEM_MAP_LVL_IGNORE_NEW_DIMMS   - MEM_MAP_LVL_IGNORE_CFGIN + Will not mapping volatile region of new dimms added to the system

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS
**/
UINT32
UpdateRequestforDegradedMemoryMapping (
  IN PSYSHOST Host
  )
{
  UINT8                        Sckt;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT8                        DimmUnmappedReason;
  UINT8                        InterleaveReqId;
  MEM_NVRAM_STRUCT             *NvramMem;
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;

  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ChannelNvList = &NvramMem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM Dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

        //Skip if this Dimm is new to the system
        if (DdrtDimmInfo->NewDimm == 1) {
          //
          // Create CCUR record with proper data and status code
          //
          UpdateCfgCurForNewDimm (Host, Sckt, Ch, Dimm);
          continue;
        }

        if (IsNvDimmUnmapped (Sckt, Ch, Dimm, &DimmUnmappedReason) && (DimmUnmappedReason == NVDIMM_UNMAPPED_SKU_LIMIT_VIOLATION)) {
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateRequestforDegradedMemoryMapping: DIMM Unmapped (SKU Limit Violation)\n");
          //
          // Update only configuration status (preserve rest of the data in case of SKU upgrade / DIMMs moved to other system, so configuration could be restored)
          //
          DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_SKU_LIMIT_VIOLATION;
          DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_SKU_LIMIT_VIOLATION;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "UpdateRequestforDegradedMemoryMapping: CCUR used\n");
          //
          // Update the configuration error variable in CCUr with proper error code
          //
          DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CFGIN_ERROR_CCUR_USED;
          DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
        }

        //Check if there are partition request  present and valid for this Dimm.
        if (DdrtDimmInfo->NgnCfgReqPresent  && DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent && DdrtDimmInfo->NgnCfgReq.partitionSize.Valid)  {
          //If present update the CFGOUT for this partition request with proper response code
          //Update CfgOut for failure of partion request
          DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (INSUFFICIENT_SILICON_RESOURCES);
        }

        //Check if there are Intreleave request  present and valid for this Dimm.
        for (InterleaveReqId = 0; InterleaveReqId < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveReqId++) {
          //Break if the interve info is not valid.
          if (!DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].RecPresent || !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].Valid) {
            break;
          }

          //Set the Valid bit to 0 for all the interleaves and update the response with proper response status
          DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].Valid = 0;
          DdrtDimmInfo->NgnCfgOut.interleave[InterleaveReqId].InterleaveChangeResponseStatus = EncodeInterleaveResponse (INSUFFICIENT_SILICON_RESOURCES);
        }
      }//Dimm
    }//Ch
  }//socket
  return SUCCESS;
}


/**

  This routine is to parses through the buffer containing Partition Size PCAT table and updates the Host structures accordingly
  @param Host       - Pointer to sysHost
  @param Socket     - Socket number
  @param Ch         - Channel number
  @param Dimm       - Dimm number
  @param PCATPtr    - Pointer to the PCAT table buffer
  @param RecordType - 0-CFG_IN record  1- CFG_OUT record.

  @retval SUCCESS
**/
UINT32
UpdatePartitionRequest (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    *PCATPtr,
  UINT8    RecordType
  )
{
  NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT *platformPCATPartition;
  DDRT_DIMM_DEVICE_INFO_STRUCT        *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  platformPCATPartition = (NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT *) ((UINT8*) PCATPtr + sizeof (NGN_PCAT_HEADER));
  if (RecordType == CFG_IN) {
    DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent = 1;
    //Update the Partition Size data(Non Vaolatile size in 64MB gran) in the Host structure for this partition request
    DdrtDimmInfo->NgnCfgReq.partitionSize.PartitionSize = (UINT16) (RShiftU64 (platformPCATPartition->PartitionSize, CONVERT_B_TO_64MB));
  } else if (RecordType == CFG_OUT) {
    DdrtDimmInfo->NgnCfgOut.partitionSize.RecPresent = 1;
    //Update the Partition Size data(Non Vaolatile size in 64MB gran) in the Host structure for this partition request
    DdrtDimmInfo->NgnCfgOut.partitionSize.PartitionSize = (UINT16) (RShiftU64 (platformPCATPartition->PartitionSize, CONVERT_B_TO_64MB));
  }

  return SUCCESS;
}

/**

  This routine scans through the existing dimm information in the Host
  NVRAm struct to find a match for the manufacturer id and serial id mentioned
  in the dimm interleave id table and fill the dimm lcoation in the Host structs.

  @param Host     - Pointer to sysHost
  @param destInterleaveIdPtr   - Pointer to the destination structure.
  @param DimmManufacturerId       - Manufacturer id mentiond in the dimm interleave id table.
  @param DimmSerialNumber     -  Serial number mentioned in the dimm interleave id table.

  @retval SUCCESS
**/
UINT32
FillDimmLocation (PSYSHOST Host, NGN_DIMM_INTERLEAVE_ID_HOST *destInterleaveIdPtr, NGN_DIMM_SPD_UNIQUE_DIMM_ID *DimmUniqueId)
{
  UINT8 sckt;
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct memNvram *nvramMem;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  nvramMem = &Host->nvram.mem;

  for (sckt = 0; sckt<MAX_SOCKET; sckt++) {
    if (CheckSocketEnabled (sckt) == 0) {
      continue;
    }
    channelNvList = &nvramMem->socket[sckt].channelList;
    for (ch = 0; ch<MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = &nvramMem->socket[sckt].channelList[ch].dimmList;
      // For each DIMM
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if not an NVMDIMM
        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          continue;
        }

        //If the Manufacturer id and serial number match, store the dimm location info. in the host struct.
        if ((!CompareMem ((UINT8 *)&DimmUniqueId->MfgIdCode, (UINT8 *)&(*dimmNvList)[dimm].VendorID, NGN_MAX_MANUFACTURER_STRLEN))  &&
            (!CompareMem ((UINT8 *)&DimmUniqueId->MfgLocation, (UINT8 *)&(*dimmNvList)[dimm].SPDMMfLoc, NGN_MAX_MANUFACTURER_LOC_STRLEN)) &&
            (!CompareMem ((UINT8 *)&DimmUniqueId->SerialNumber[0], (UINT8 *)&(*dimmNvList)[dimm].SPDModSN, NGN_MAX_SERIALNUMBER_STRLEN)) &&
            (!CompareMem ((UINT8 *)&DimmUniqueId->MfgDate, (UINT8 *)&(*dimmNvList)[dimm].SPDModDate, NGN_MAX_MANUFACTURER_DATE_STRLEN))) {
          RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM UID matched\n");
          RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                  "UID->IdCodeLocDateSN %02x%02x-%02x-%02x%02x-%02x%02x%02x%02x\n", DimmUniqueId->MfgIdCode & 0xFF,
                                  DimmUniqueId->MfgIdCode >> 8, DimmUniqueId->MfgLocation, DimmUniqueId->MfgDate.Year,
                                  DimmUniqueId->MfgDate.Week, DimmUniqueId->SerialNumber[NGN_BYTE0], DimmUniqueId->SerialNumber[NGN_BYTE1],
                                  DimmUniqueId->SerialNumber[NGN_BYTE2], DimmUniqueId->SerialNumber[NGN_BYTE3]);

          destInterleaveIdPtr->Socket = sckt;
          destInterleaveIdPtr->Channel = ch;
          destInterleaveIdPtr->Dimm = dimm;
          return SUCCESS;
        }
      }
    }
  }

  //If we are here, we havent found a match for the given manufacturer id and serial number in the system.
  //so load some invalid values to let the validation code know that we didnt find a match.
  destInterleaveIdPtr->Socket = 0xFF;
  destInterleaveIdPtr->Channel = 0xFF;
  destInterleaveIdPtr->Dimm = 0xFF;
  return FAILURE;
}

/**

  This routine is to parses through the buffer containing Interleave information PCAT table and updates the Host structures accordingly
  @param Host              - Pointer to sysHost
  @param Socket            - Socket number
  @param Ch                - Channel number
  @param Dimm              - Dimm number
  @param PCATPtr           - Pointer to the PCAT table buffer
  @param RecordType        - 0- CFG_CUR record  1-CFG_IN record  2- CFG_OUT record.
  @param InterleaveCounter - Count of interleave PCAT tables read.

  @retval SUCCESS
**/
UINT32
UpdateInterleaveInformation (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    *PCATPtr,
  UINT8    RecordType,
  UINT8    InterleaveCounter)
{
  UINT8                                     Count;
  UINT8                                     Sock;
  UINT8                                     DieCount;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT      *PlatformPCATInterleave;
  NGN_DIMM_INTERLEAVE_ID                    *PlatformPCATInterleaveId;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveIdPtr;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  DIMM_NVRAM_STRUCT                         (*DimmNvList)[MAX_DIMM];

  PlatformPCATInterleave = (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT *) ((UINT8*) PCATPtr + sizeof (NGN_PCAT_HEADER));
  PlatformPCATInterleaveId = (NGN_DIMM_INTERLEAVE_ID *) ((UINT8*) PlatformPCATInterleave + sizeof (NGN_DIMM_INTERLEAVE_INFORMATION_PCAT));

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  if (RecordType == CFG_CUR) {
    DdrtDimmInfo->NgnCfgCurUidsValid = DCPMM_CFG_CUR_UIDS_VALID;
  }

  //Initialize the destination pointer to the corrct structure based on the record type.
  if (RecordType == CFG_CUR) {
    DestInterleavePtr = &DdrtDimmInfo->NgnCfgCur.interleave[InterleaveCounter];
    DestInterleaveIdPtr = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveCounter];
  } else if (RecordType == CFG_IN) {
    DestInterleavePtr = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveCounter];
    DestInterleaveIdPtr = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveCounter];
  } else {
    DestInterleavePtr = &DdrtDimmInfo->NgnCfgOut.interleave[InterleaveCounter];
    DestInterleaveIdPtr = DdrtDimmInfo->NgnCfgOut.interleaveId[InterleaveCounter];
  }

  //Set the Interleave record preset bit
  DestInterleavePtr->RecPresent = 1;

  //Fill in the required data in the Host structure
  DestInterleavePtr->InterleaveSetIndex = PlatformPCATInterleave->InterleaveSetIndex;
  DestInterleavePtr->NumOfDimmsInInterleaveSet = PlatformPCATInterleave->NumOfDimmsInInterleaveSet;
  DestInterleavePtr->InterleaveMemoryType = PlatformPCATInterleave->InterleaveMemoryType;
  DestInterleavePtr->InterleaveSize = PlatformPCATInterleave->InterleaveSize;
  DestInterleavePtr->InterleaveChangeResponseStatus = PlatformPCATInterleave->InterleaveChangeResponseStatus;

  //Scan for PCAT table and fill in the Host structure.
  for (Count = 0; Count < PlatformPCATInterleave->NumOfDimmsInInterleaveSet; Count++) {
    if (RecordType == CFG_CUR) {
      for (Sock = 0; Sock < MAX_SOCKET; Sock++) {
        if (CheckSocketEnabled (Sock) == 0) {
          continue;
        }
        DieCount = GetAcpiDieCount (Sock);
        if (PlatformPCATInterleaveId->DimmLocation.SocketId == (Sock / DieCount)) {
          break;
        }
      }
      if (Sock == MAX_SOCKET) {
        //
        // No Matching socket found
        //
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "No socket match found\n");
        return FAILURE;
      }

      DestInterleaveIdPtr->Socket = Sock;
      DestInterleaveIdPtr->Channel = (PlatformPCATInterleaveId->DimmLocation.ImcId * GetNumChannelPerMc()) + PlatformPCATInterleaveId->DimmLocation.ChannelId;
      DestInterleaveIdPtr->Dimm = PlatformPCATInterleaveId->DimmLocation.SlotId;

      //
      // Check if DimmUniqueId is correct
      //
      DimmNvList = &Host->nvram.mem.socket[DestInterleaveIdPtr->Socket].channelList[DestInterleaveIdPtr->Channel].dimmList;
      if ((CompareMem ((UINT8 *)&PlatformPCATInterleaveId->DimmUniqueId.MfgIdCode, (UINT8 *)&(*DimmNvList)[DestInterleaveIdPtr->Dimm].VendorID, NGN_MAX_MANUFACTURER_STRLEN))  ||
          (CompareMem ((UINT8 *)&PlatformPCATInterleaveId->DimmUniqueId.MfgLocation, (UINT8 *)&(*DimmNvList)[DestInterleaveIdPtr->Dimm].SPDMMfLoc, NGN_MAX_MANUFACTURER_LOC_STRLEN)) ||
          (CompareMem ((UINT8 *)&PlatformPCATInterleaveId->DimmUniqueId.SerialNumber[0], (UINT8 *)&(*DimmNvList)[DestInterleaveIdPtr->Dimm].SPDModSN, NGN_MAX_SERIALNUMBER_STRLEN)) ||
          (CompareMem ((UINT8 *)&PlatformPCATInterleaveId->DimmUniqueId.MfgDate, (UINT8 *)&(*DimmNvList)[DestInterleaveIdPtr->Dimm].SPDModDate, NGN_MAX_MANUFACTURER_DATE_STRLEN))) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM UID does not match\n");
        DdrtDimmInfo->NgnCfgCurUidsValid = DCPMM_CFG_CUR_UIDS_INVALID;
        if ((*DimmNvList)[DestInterleaveIdPtr->Dimm].dimmPresent == DCPMM_DIMM_NOT_PRESENT) {
          //
          // No Dimm Present on the Slot and UID's Mismatch
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "No dimm Present DIMM UID does not match\n");
          DdrtDimmInfo->NgnCfgCurUidsValid |= DCPMM_DIMM_NOT_PRESENT;
        } else {
          //
          // Dimm Present on the Slot and UID's Mismatch
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Dimm Present - DIMM UID does not match\n");
          DdrtDimmInfo->NgnCfgCurUidsValid |= DCPMM_DIMM_PRESENT;
        }
      }

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "InterleaveEntry[%x] - %x:%x:%x\n",
        Count, DestInterleaveIdPtr->Socket, DestInterleaveIdPtr->Channel, DestInterleaveIdPtr->Dimm);
    } else if (RecordType == CFG_IN) {
      FillDimmLocation (Host, DestInterleaveIdPtr, &PlatformPCATInterleaveId->DimmUniqueId);
    }
    DestInterleaveIdPtr->PartitionOffset = (UINT16) (RShiftU64 (PlatformPCATInterleaveId->PartitionOffset, CONVERT_B_TO_64MB));
    DestInterleaveIdPtr->PartitionSize = (UINT16) (RShiftU64 (PlatformPCATInterleaveId->PartitionSize, CONVERT_B_TO_64MB));
    DestInterleaveIdPtr++;
    PlatformPCATInterleaveId++;
  }

  return SUCCESS;
}

/**

  This routine to parses through the buffer looking for PCAT tables and updates the host structures accordingly
  @param Host         - Pointer to sysHost
  @param Socket       - socket number
  @param Ch           - Channel number
  @param Dimm         - Dimm number
  @param Pcat         - Pointer to the PCAT table buffer
  @param PcatLength   -  Length of the PCAT table buffer
  @param RecordType   -  0- CFG_CUR record  1-CFG_IN record  2- CFG_OUT record.

  @retval SUCCESS Operation completed successfully
          FAILURE Error occurred during PCATs processing
**/
UINT32
HandlePCATs (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Channel,
  UINT8     Dimm,
  UINT8     *Pcat,
  UINT32    PcatLength,
  UINT8     RecordType
  )
{
  UINT32                       PcatLengthConsumed;
  UINT32                       Offset = 0;
  UINT8                        InterleaveCounter = 0;
  NGN_PCAT_HEADER              *PlatformPcatHdr;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);

  // Check PCATs length
  if ((Pcat == NULL) || (PcatLength == 0)) {
    // For current configuration PCATs might not be available
    if (CFG_CUR == RecordType) {
      if (Pcat != NULL) {
        return SUCCESS;
      }
      DdrtDimmInfo->NgnCfgCurValid = 0;
      DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CCUR_CHECKSUM_NOT_VALID;
    } else if (CFG_IN == RecordType) {
      // For input configuration PCATs are expected
      DdrtDimmInfo->NgnCfgReqValid = 0;
      DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
    } else {
      DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
    }
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PCATs not found!\n");
    return FAILURE;
  }

  // Set pointer to the first PCAT header
  PlatformPcatHdr = (NGN_PCAT_HEADER *) Pcat;

  // Set PCATs length to be consumed
  PcatLengthConsumed = PcatLength;

  // Consume all available PCATs
  while (PcatLengthConsumed > 0) {

    //Read the PCAT table header
    PlatformPcatHdr = (NGN_PCAT_HEADER *) (((UINT8 *) PlatformPcatHdr) + Offset);

    // Validate PCATs length (this will assure that we don't reach negative values when updating already consumed PCATs length)
    if ((sizeof (NGN_PCAT_HEADER) < PcatLengthConsumed) && (PlatformPcatHdr->Length <= PcatLengthConsumed)) {
      switch (PlatformPcatHdr->Type) {
      case NGN_PCAT_TYPE_DIMM_INTERLEAVE_INFORMATION:
        //If its is an Interleave Request/Info update the Interleave infor/Request struct in the host structure.
        if (InterleaveCounter < MAX_UNIQUE_NGN_DIMM_INTERLEAVE) {
          if (UpdateInterleaveInformation (Host, Socket, Channel, Dimm, (UINT8 *) PlatformPcatHdr, RecordType, InterleaveCounter) == FAILURE) {
            //
            // There was a problem finding a socket match
            //
            return FAILURE;
          }
        } else {
          //Log a warning and notify NVMDIMM driver if there are more than 2 interleave requests
          if (CFG_CUR == RecordType) {
            DdrtDimmInfo->NgnCfgCurValid = 0;
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CCUR_CHECKSUM_NOT_VALID;
          } else if (CFG_IN == RecordType) {
            DdrtDimmInfo->NgnCfgReqValid = 0;
            DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
          } else {
            DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
          }
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Interleave requests exceed two!\n");
          OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_MAX_INTERLEAVE_SETS_EXCEEDED, Socket, Channel, Dimm, 0xFF);
          return FAILURE;
        }
        InterleaveCounter++;
        break;

      case NGN_PCAT_TYPE_DIMM_PARTITION_SIZE_CHANGE:
        //If its is an Partition request update  Partition request in the host structure.
        UpdatePartitionRequest (Host, Socket, Channel, Dimm, (UINT8 *) PlatformPcatHdr, RecordType);
        break;

      default:
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Unknown PCAT type found, skip processing...\n");
        break;
      }

      // Update already consumed PCATs length
      PcatLengthConsumed -= PlatformPcatHdr->Length;

      // Set offset to the next PCAT (it will be used only when not all PCATs were consumed)
      Offset = PlatformPcatHdr->Length;
    } else {
      // Invalid PCAT Table length (header data corrupted)
      if (CFG_CUR == RecordType) {
        DdrtDimmInfo->NgnCfgCurValid = 0;
        DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_CCUR_CHECKSUM_NOT_VALID;
      } else if (CFG_IN == RecordType) {
        DdrtDimmInfo->NgnCfgReqValid = 0;
        DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
      } else {
        DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;
      }
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid PCATs length!\n");
      return FAILURE;
    }
  }

  return SUCCESS;
}

/**

  This routine to issue getplatformconfigdata command and read the current configuration data from the the Dimm
  @param[in] Host     - Pointer to sysHost
  @param[in] socket   - socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number


  @retval TRUE - if a new Dimm is detected
  @retval FALSE - if it is the same Dimm form the previous boot
**/
UINT32
ReadCurrentCfgTable (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT32    CurCfgOffset,
  IN UINT32    CurCfgSize
  )
{
  UINT32                              Offset = 0;
  UINT32                              Counter = 0;
  UINT32                              Status = SUCCESS;
  UINT32                              CCurLength;
  UINT32                              PcatLength;
  UINT16                              Length = BYTES_128;
  UINT8                               i;
  UINT8                               OffsetAdjustment;
  UINT8                               MbStatus = 0;
  UINT8                               *Pcat;
  UINT8                               *CurCfgBuffer;
  UINT8                               RetrieveOption = RETRIEVE_PARTITION_DATA;
  UINT8                               *TempCurCfgBuffer;
  FNV_CIS_BUFFER                      FnvBuffer;
  NGN_DIMM_PLATFORM_CFG_HEADER        *PlatformCurCfgHeader;
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY  *PlatformCurCfgBdy;
  DDRT_DIMM_DEVICE_INFO_STRUCT        *DdrtDimmInfo;
  UINT32                              BufferSize;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);

  //
  // Find the offset to the CurCfg table for the platform config body
  // GetPlatformConfigData only supports reads @ 128 byte boundary
  // So if the offset is not in 128bytes boundary, we will have to adjust it and start at 128 byte boundary
  //
  OffsetAdjustment = CurCfgOffset % BYTES_128;

  //
  // Moving the offste pointer to 128byte boundary
  //
  Offset = CurCfgOffset - OffsetAdjustment;

  //
  // Calculate the Buffer size required to read configuration Current data from PMEM module
  // Increase the buffer size to be read OffsetAdjustment and CurCfgSize size
  //
  BufferSize = OffsetAdjustment + CurCfgSize;

  //
  // Check if the Buffer size is 128 byte boundary, if not increase Buffersize  to next 128 byte boundary
  // Set the Counter for number of 128 byte reads required
  //
  if ((BufferSize % BYTES_128)) {
    BufferSize += (BYTES_128 - (BufferSize % BYTES_128));
  }
  Counter = BufferSize / BYTES_128;

  // Allocare buffer to store config curr PCD data
  TempCurCfgBuffer = RcAllocatePool  (sizeof (UINT8) * BufferSize);
  if (TempCurCfgBuffer == NULL) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Failed to allocate struct TempCurCfgBuffer!\n");
    return FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ReadCurrentCfgTable->GetPlatformConfigData, partition = %d, OffsetAdjustment = %d, CurCfgOffset = %d, CurCfgSize = %d, Counter = %d, CUR_CFG_BufferSize= %d \n",
    BIOS_PARTITION, OffsetAdjustment, CurCfgOffset, CurCfgSize, Counter, BufferSize);

  //
  // Check buffer size should not for max limit of 64kb and
  // Check Buffsize should be 128byte boundary and number of 128 byte reads should be equal to buffersize
  //
  if ((BufferSize <= INTEL_PARTITION_SIZE) && ((BufferSize % BYTES_128) == 0) && ((Counter * BYTES_128) == BufferSize)) {
    //
    // Read the complete data to a temporary buffer to parse PCAT tables
    //
    for (i = 0; i < Counter; i++) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "ReadCurrentCfgTable->GetPlatformConfigData, partition = %d, option = %d, offset = %d\n", BIOS_PARTITION, RetrieveOption, Offset);

      Status = GetPlatformConfigData (Host, Socket, Channel, Dimm, BIOS_PARTITION, RetrieveOption, Offset, &FnvBuffer, &MbStatus);
      if (Status == FAILURE) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
        EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Channel, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_COMM_FAILED;
        RcFreePool (TempCurCfgBuffer);
        return FAILURE;
      } else if ((Status == WARN_NVMCTRL_MEDIA_NOTREADY) || (Status == WARN_NVMCTRL_MEDIA_INERROR) || (Status == WARN_NVMCTRL_MEDIA_RESERVED)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Problem with Media Access\n");
        EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, (UINT8)Status, Socket, Channel, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_MEDIA_ERROR;
        RcFreePool (TempCurCfgBuffer);
        return FAILURE;
      }
      CopyMem (((UINT8 *) (TempCurCfgBuffer)) + (i * Length) , (UINT8 *) FnvBuffer.platformData, Length);
      Offset = Offset + Length;
    } // Counter loop
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BIOS partition data size exceeded! \n");
    EwlOutputType1 (WARN_CFGCUR_STATUS, WARN_CFGCUR_DATASIZE_EXCEEDED, Socket, Channel, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_DATA_SIZE_ERROR;
    RcFreePool (TempCurCfgBuffer);
    return FAILURE;
  }

  //
  // Adjust back the temporary Input buffer based on the offsetAdjustment value
  // (This is to negate the adjustment we made to read from 128byte boundary earlier)
  //
  CurCfgBuffer = ((UINT8 *) (TempCurCfgBuffer)) + OffsetAdjustment;

  //
  // Locate and parse the CCUR table and update the Host struct with proper values read
  //
  PlatformCurCfgHeader = (NGN_DIMM_PLATFORM_CFG_HEADER *) CurCfgBuffer;
  CCurLength = PlatformCurCfgHeader->Length;

  //
  // Check for 'CCUR' signature. Return if the signature doesn't match.
  //
  if (PlatformCurCfgHeader->Signature != NGN_CURRENT_CONFIGURATION_SIGNATURE) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_SIGNATURE_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CCUR signature not found in the emEM DIMM Partition %d\n", BIOS_PARTITION);
    DisplayBuffer (Host, CurCfgBuffer, CurCfgSize);
    EwlOutputType1 (WARN_CFGCUR_STATUS, WARN_CFGCUR_SIGNATURE_MISMATCH, Socket, Channel, Dimm, NO_RANK);
    RcFreePool (TempCurCfgBuffer);
    return FAILURE;
  }

  //
  // Check if the checksum passes
  //
  if (ChsumTbl ((UINT8 *) CurCfgBuffer, CCurLength)) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_CHECKSUM_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Current Configuration Record checksum failure.\n");
    DisplayBuffer (Host, CurCfgBuffer, CurCfgSize);
    EwlOutputType1 (WARN_CFGCUR_STATUS, WARN_CFGCUR_CHECKSUM_MISMATCH, Socket, Channel, Dimm, NO_RANK);
    RcFreePool (TempCurCfgBuffer);
    return FAILURE;
  }

  //
  // Store revision info
  //
  DdrtDimmInfo->NgnCfgCurRev = PlatformCurCfgHeader->Revision;

  //
  // Check if the revision is compatible
  //
  if (GetPmemPcdRecordsHeaderRevision () != PlatformCurCfgHeader->Revision) {
    //
    //
    // Check if revision is compatible with current one
    //
    if (!IsPmemPcdRecordsHeaderRevisionCompatible (PlatformCurCfgHeader->Revision)) {
      DdrtDimmInfo->NgnCfgErrorPresent = 1;
      DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_CCUR_REVISION_ERROR;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision mismatch (CCUR)\n");
      DisplayBuffer (Host, CurCfgBuffer, CurCfgSize);
      EwlOutputType1 (WARN_CFGCUR_STATUS, WARN_CFGCUR_REVISION_MISMATCH, Socket, Channel, Dimm, NO_RANK);
      RcFreePool (TempCurCfgBuffer);
      return FAILURE;
    } else {
      //
      // Revision is compatible, CR DXE will update header revision and handle the CCUR "conversion"
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision deprecated (CCUR)\n");
    }
  }

  //
  // Copy the CCUR body into the Host struct
  //
  PlatformCurCfgBdy = (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY *) ((UINT8 *) CurCfgBuffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));
  CopyCurrentCfgBody (Host, Socket, Channel, Dimm, PlatformCurCfgBdy);

  //
  // Initialize PCAT table pointer and handle PCAT tables.
  //
  Pcat = (UINT8 *) CurCfgBuffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY);
  PcatLength = CCurLength - sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) - sizeof (NGN_DIMM_PLATFORM_CFG_CURRENT_BODY);
  Status = HandlePCATs (Host, Socket, Channel, Dimm, Pcat, PcatLength, CFG_CUR);
  if (Status != SUCCESS) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_DATA_ERROR;
    DisplayBuffer (Host, CurCfgBuffer, CurCfgSize);
    RcFreePool (TempCurCfgBuffer);
    return FAILURE;
  }

  //
  // Set the CCUR as ready for processing
  //
  DdrtDimmInfo->NgnCfgCurPresent = 1;

  RcFreePool (TempCurCfgBuffer);
  return SUCCESS;
}

/**

  This routine verifies whether BIOS partiton Error is recoverable

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - socket number
  @param[in] Ch           - Channel number
  @param[in] Dimm         - Dimm number

  @retval TRUE BIOS partition error is recoverable, FALSE otherwise

**/
BOOLEAN
IsBiosPartitionErrorRecoverable (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  if ((Host == NULL) || (Socket >= MAX_SOCKET) || (Ch >= MAX_CH) || (Dimm >= MAX_DIMM)) {
    return FALSE;
  }

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  if ((DdrtDimmInfo->NgnCfgErrorPresent == 1) &&
      ((DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] == NGN_DIMM_COMM_FAILED) ||
       (DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] == NGN_DIMM_MEDIA_ERROR))) {
    return FALSE;
  }

  return TRUE;
}

/**

  This routine to issue getplatformconfigdata command and reads the configuration header data from BIOS partition
  to get the offset of the current configuration data
  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number


  @retval TRUE - if a new dimm is detected
  @retval FALSE - if it is the same dimm form the previous boot
**/
UINT32
ReadBIOSPartition (
  IN SYSHOST *Host,
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   Dimm
  )
{
  UINT8                                   RetrieveOption = RETRIEVE_PARTITION_DATA;
  UINT8                                   MbStatus = 0;
  FNV_CIS_BUFFER                          FnvBuffer;
  NGN_DIMM_PLATFORM_CFG_HEADER            *PlatformCfgHdr;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *PlatformCfgBdy;
  UINT32                                  Offset = 0;
  UINT32                                  Status = SUCCESS;
  DDRT_DIMM_DEVICE_INFO_STRUCT            *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ReadBIOSPartition->GetPlatformConfigData, partition = %d, option = %d, offset = %d\n", BIOS_PARTITION, RetrieveOption, Offset);

  Status = GetPlatformConfigData (Host, Socket, Ch, Dimm, BIOS_PARTITION, RetrieveOption, Offset, &FnvBuffer, &MbStatus);
  if (Status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
    EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
    DisableDIMM (Host, Socket, Ch, Dimm);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_COMM_FAILED;
    return FAILURE;
  } else if ((Status == WARN_NVMCTRL_MEDIA_NOTREADY) || (Status == WARN_NVMCTRL_MEDIA_INERROR) || (Status == WARN_NVMCTRL_MEDIA_RESERVED)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Problem with Media Access\n");
    EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, (UINT8)Status, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_MEDIA_ERROR;
    return FAILURE;
  }

  //
  // Initialize pointers to the header and body
  //
  PlatformCfgHdr = (NGN_DIMM_PLATFORM_CFG_HEADER *) &FnvBuffer.platformData;

  //
  // Check for 'DMHD' signature. Return if the signature doesn't match
  //
  if (PlatformCfgHdr->Signature != NGN_CONFIGURATION_HEADER_SIGNATURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DMHD signature not found in the PMem DIMM Partition %d\n", BIOS_PARTITION);
    DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_SIGNATURE_ERROR;
    return SUCCESS;
  }

  //
  // Check if the revision is compatible
  //
  if (GetPmemConfigHeaderRevision () != PlatformCfgHdr->Revision) {
    //
    // Check if revision is valid
    //
    if (!IsPmemConfigHeaderRevisionCompatible (PlatformCfgHdr->Revision)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(PMem DIMM Partition %d) PMem Mgmt SW - BIOS interface revision mismatch (DMHD)\n", BIOS_PARTITION);
      DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
      EwlOutputType1 (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_REVISION_MISMATCH, Socket, Ch, Dimm, NO_RANK);
      DdrtDimmInfo->NgnCfgErrorPresent = 1;
      DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_HDR_REVISION_ERROR;
      return FAILURE;
    } else {
      //
      // Revision is compatible, CR DXE will update header revision to the current spec revision
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(PMem DIMM Partition %d) PMem Mgmt SW - BIOS interface revision deprecated (DMHD)\n",BIOS_PARTITION);
    }
  }

  //
  // Check if the checksum passes
  //
  if (ChsumTbl((UINT8 *)&FnvBuffer.platformData, PlatformCfgHdr->Length)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BIOS Partition DMHD checksum failure.\n");
    DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
    EwlOutputType1 (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_CHECKSUM_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_CHECKSUM_ERROR;
    return FAILURE;
  }

  PlatformCfgBdy = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (((UINT8 *)&FnvBuffer.platformData) + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER));
  Status = ReadCurrentCfgTable (Host, Socket, Ch, Dimm, PlatformCfgBdy->CurrentConfStartOffset, PlatformCfgBdy->CurrentConfDataSize);

  return Status;
}

/**

  This routine to issue getplatformconfigdata command and read the  configuration Input requests from the the Dimm
  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Channel                - Channel number
  @param[in] Dimm                   - Dimm number
  @param[in] ConfRequestDataOffset  - Offset of CfgIn table in Platform Config Data Region 2.
  @param[in] ConfRequestDataSize    - Offset of CfgIn table

  @retval SUCCESS
  @retval FAILURE
**/
UINT32
ReadCfgInTable (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT32    ConfRequestDataOffset,
  IN UINT32    ConfRequestDataSize
  )
{
  UINT32                            Offset;
  UINT32                            Counter = 0;
  UINT32                            Status = SUCCESS;
  UINT32                            CInLength;
  UINT32                            PcatLength;
  UINT16                            Length = BYTES_128;
  UINT8                             i;
  UINT8                             OffsetAdjustment;
  UINT8                             MbStatus = 0;
  UINT8                             *Pcat;
  UINT8                             *CfgInBuffer;
  UINT8                             RetrieveOption = RETRIEVE_PARTITION_DATA;
  UINT8                             *TempCfgInBuffer;
  FNV_CIS_BUFFER                    FnvBuffer;
  NGN_DIMM_PLATFORM_CFG_HEADER      *PlatformCfgInputHeader;
  NGN_DIMM_PLATFORM_CFG_INPUT_BODY  *PlatformCfgInputBdy;
  DDRT_DIMM_DEVICE_INFO_STRUCT      *DdrtDimmInfo;
  UINT32                            BufferSize;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);

  //
  // Find the offset to the CfgIn table for the platform config body
  // GetPlatformConfigData only supports reads @ 128 byte boundary.
  // So if the offset is not in 128bytes boundary, we will have to adjust it and start at 128 byte boundary.
  //
  OffsetAdjustment = ConfRequestDataOffset % 128;

  //
  // Moving the offset pointer to 128byte boundary
  //
  Offset = ConfRequestDataOffset - OffsetAdjustment;

  //
  // Calculate the Buffer size required to read configuration input data from PMEM module
  // Increase the buffer size to be read OffsetAdjustment and CurReqSize size
  //
  BufferSize = OffsetAdjustment + ConfRequestDataSize;

  //
  // Check if the Buffer size is 128 byte boundary, if not increase Buffersize  to next 128 byte boundary
  // Set the Counter for number of 128 byte reads required
  //
  if ((BufferSize % BYTES_128)) {
    BufferSize += (BYTES_128 - (BufferSize % BYTES_128));
  }
  Counter = BufferSize / BYTES_128;

  // Allocare buffer to store config request PCD data
  TempCfgInBuffer = RcAllocatePool  (sizeof (UINT8) * BufferSize);
  if (TempCfgInBuffer == NULL) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Failed to allocate struct TempCfgInBuffer!\n");
    return FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ReadCfgInTable->GetPlatformConfigData, partition = %d, OffsetAdjustment = %d, ConfRequestDataOffset = %d, ConfRequestDataSize = %d, Counter = %d, CFG_IN_BufferSize= %d \n",
    OS_PARTITION, OffsetAdjustment, ConfRequestDataOffset, ConfRequestDataSize, Counter, BufferSize);

  //
  // Check buffer size should not for max limit of 64kb and
  // Check Buffsize should be 128byte boundary and number of 128 byte reads should be equal to buffersize
  //
  if ((BufferSize <= INTEL_PARTITION_SIZE) && ((BufferSize % BYTES_128) == 0) && ((Counter * BYTES_128) == BufferSize)) {
    //
    // Read the complete CfgIn data to a temporary buffer to parse PCAT tables.
    //
    for (i = 0; i < Counter; i++) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "ReadCfgInTable->GetPlatformConfigData, partition = %d, option = %d, offset = %d\n", OS_PARTITION, RetrieveOption, Offset);

      Status = GetPlatformConfigData (Host, Socket, Channel, Dimm, OS_PARTITION, RetrieveOption, Offset, &FnvBuffer, &MbStatus);
      if (Status == FAILURE) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket,Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
        EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Channel, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_COMM_FAILED;
        RcFreePool (TempCfgInBuffer);
        return FAILURE;
      } else if ((Status == WARN_NVMCTRL_MEDIA_NOTREADY) || (Status == WARN_NVMCTRL_MEDIA_INERROR) || (Status == WARN_NVMCTRL_MEDIA_RESERVED)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket,Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Problem with Media Access\n");
        EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, (UINT8)Status, Socket, Channel, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_MEDIA_ERROR;
        RcFreePool (TempCfgInBuffer);
        return FAILURE;
      }
      CopyMem (((UINT8 *)(TempCfgInBuffer)) + (i * Length) , (UINT8 *)FnvBuffer.platformData, Length);
      Offset = Offset + Length;
    } // Counter loop
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OS partition(CIN) data size exceeded! \n");
    EwlOutputType1 (WARN_CFGIN_STATUS, WARN_CFGIN_DATASIZE_EXCEEDED, Socket, Channel, Dimm, NO_RANK);
    RcFreePool (TempCfgInBuffer);
    return FAILURE;
  }

  //
  // Adjust back the temporary Input buffer based on the offsetAdjustment value. (This is to negate the adjustment we made to read from 128byte boundary earlier.)
  //
  CfgInBuffer = ((UINT8 *) (TempCfgInBuffer)) + OffsetAdjustment;

  //
  // Initialize platform config input header
  //
  PlatformCfgInputHeader = (NGN_DIMM_PLATFORM_CFG_HEADER *) CfgInBuffer;
  CInLength = PlatformCfgInputHeader->Length;

  //
  // Check for 'CIN_' signature. Return if the signature doesn't match.
  //
  if (PlatformCfgInputHeader->Signature != NGN_CONFIGURATION_INPUT_SIGNATURE) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_SIGNATURE_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CIN_ signature not found in the PMem DIMM PCD 2.\n");
    DisplayBuffer (Host, CfgInBuffer, ConfRequestDataSize);
    EwlOutputType1 (WARN_CFGIN_STATUS, WARN_CFGIN_SIGNATURE_MISMATCH, Socket, Channel, Dimm, NO_RANK);
    RcFreePool (TempCfgInBuffer);
    return FAILURE;
  }

  //
  // Check if the checksum passes
  //
  if (ChsumTbl ((UINT8 *) CfgInBuffer, CInLength)) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_CHECKSUM_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Configuration Input Record checksum failure.\n");
    DisplayBuffer (Host, CfgInBuffer, ConfRequestDataSize);
    EwlOutputType1 (WARN_CFGIN_STATUS, WARN_CFGIN_CHECKSUM_MISMATCH, Socket, Channel, Dimm, NO_RANK);
    RcFreePool (TempCfgInBuffer);
    return FAILURE;
  }

  //
  // Store revision info
  //
  DdrtDimmInfo->NgnCfgReqRev = PlatformCfgInputHeader->Revision;

  //
  // Check if the revision is compatible
  //
  if (GetPmemPcdRecordsHeaderRevision () != PlatformCfgInputHeader->Revision) {
    //
    //
    // Check if revision is compatible with current one
    //
    if (!IsPmemPcdRecordsHeaderRevisionCompatible (PlatformCfgInputHeader->Revision)) {
      DdrtDimmInfo->NgnCfgErrorPresent = 1;
      DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_CIN_REVISION_ERROR;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision mismatch (CIN)\n");
      DisplayBuffer (Host, CfgInBuffer, ConfRequestDataSize);
      EwlOutputType1 (WARN_CFGIN_STATUS, WARN_CFGIN_REVISION_MISMATCH, Socket, Channel, Dimm, NO_RANK);
      RcFreePool (TempCfgInBuffer);
      return FAILURE;
    } else {
      //
      // Revision is compatible (if this is new request it will be rejected and CR DXE driver will write COUT with proper error code,
      // if this is already processed request CR DXE code will update header revision to inform software that new request needs new
      // revision to be processed)
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision deprecated (CIN)\n");
    }
  }

  //
  // Initialize platform config input body
  //
  PlatformCfgInputBdy = (NGN_DIMM_PLATFORM_CFG_INPUT_BODY *) ((UINT8 *) CfgInBuffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER));

  //
  // Fill in the sequence number for this CfgIn record in the Host struct
  //
  DdrtDimmInfo->NgnCfgReq.body.SequenceNumber = PlatformCfgInputBdy->SequenceNumber;

  //
  // Fill in the PCAT table records for this CfgIn record in the Host struct
  //
  Pcat = (UINT8 *) CfgInBuffer + sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) + sizeof (NGN_DIMM_PLATFORM_CFG_INPUT_BODY);
  PcatLength = CInLength - sizeof (NGN_DIMM_PLATFORM_CFG_HEADER) - sizeof (NGN_DIMM_PLATFORM_CFG_INPUT_BODY);
  Status = HandlePCATs (Host, Socket, Channel, Dimm, Pcat, PcatLength, CFG_IN);
  if (Status != SUCCESS) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_DATA_ERROR;
    DisplayBuffer (Host, CfgInBuffer, ConfRequestDataSize);
    RcFreePool (TempCfgInBuffer);
    return FAILURE;
  }

  //
  // Set the CfgReq as ready for processing
  //
  DdrtDimmInfo->NgnCfgReqPresent = 1;

  RcFreePool (TempCfgInBuffer);
  return SUCCESS;
}

/**

  This routine to issue getplatformconfigdata command and read the  configuration Input requests from the the dimm
  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Channel                - Channel number
  @param[in] Dimm                   - Dimm number
  @param[in] ConfResponseDataOffset - Offset of CfgOut table in Platform Config Data Region 2.
  @param[in] ConfResponseDataSize   - Offset of CfgOut table

  @retval SUCCESS
**/
UINT32
ReadCfgOutTable (
  SYSHOST *Host,
  UINT8   Socket,
  UINT8   Ch,
  UINT8   Dimm,
  UINT32  ConfResponseDataOffset,
  UINT32  ConfResponseDataSize
  )
{
 UINT16                            Length;
  UINT8                             i;
  UINT8                             *CfgOutBuffer;
  UINT8                             OffsetAdjustment;
  UINT8                             MbStatus = 0;
  UINT8                             *TempCfgOutBuffer;
  UINT8                             RetrieveOption = RETRIEVE_PARTITION_DATA;
  UINT32                            Offset;
  UINT32                            Counter = 0;
  UINT32                            Status = SUCCESS;
  FNV_CIS_BUFFER                    FnvBuffer;
  NGN_DIMM_PLATFORM_CFG_HEADER      *PlatformCfgOutputHeader;
  NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY *PlatformCfgOutputBdy;
  DDRT_DIMM_DEVICE_INFO_STRUCT      *DdrtDimmInfo;
  UINT32                            BufferSize;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  Length = BYTES_128;
  //
  // Find the offset to the CfgOut table for the platform config body
  // GetPlatformConfigData only supports reads @ 128 byte boundary.
  // So if the offset is not in 128bytes boundary, we will have to adjust it and start at 128 byte boundary.
  //
  OffsetAdjustment = ConfResponseDataOffset % 128;

  //
  // Moving the offste pointer to 128byte boundary
  //
  Offset = ConfResponseDataOffset - OffsetAdjustment;

  //
  // Calculate the Buffer size required to read configuration Out data from PMEM module
  // Increase the buffer size to be read OffsetAdjustment and CurOutSize size
  //
  BufferSize = OffsetAdjustment + ConfResponseDataSize;

  //
  // Check if the Buffer size is 128 byte boundary, if not increase Buffersize  to next 128 byte boundary
  // Set the Counter for number of 128 byte reads required
  //
  if ((BufferSize % BYTES_128)) {
    BufferSize += (BYTES_128 - (BufferSize % BYTES_128));
  }
  Counter = BufferSize / BYTES_128;

  // Allocare buffer to store config request PCD data
  TempCfgOutBuffer = RcAllocatePool  (sizeof (UINT8) * BufferSize);
  if (TempCfgOutBuffer == NULL) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Failed to allocate struct TempCfgOutBuffer!\n");
    return FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ReadCfgOutTable->GetPlatformConfigData, partition = %d, OffsetAdjustment = %d, ConfResponseDataOffset = %d, ConfResponseDataSize = %d, Counter = %d, CFG_OUT_BufferSize= %d \n",
    OS_PARTITION, OffsetAdjustment, ConfResponseDataOffset, ConfResponseDataSize, Counter, BufferSize);

  //
  // Check buffer size should not for max limit of 64kb and
  // Check Buffsize should be 128byte boundary and number of 128 byte reads should be equal to buffersize
  //
  if ((BufferSize <= INTEL_PARTITION_SIZE) && ((BufferSize % BYTES_128) == 0) && ((Counter * BYTES_128) == BufferSize)) {
    // Read the complete CfgOut data to a temporary buffer to parse PCAT tables.
    for (i = 0; i < Counter; i++) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
         "ReadCfgOutTable->GetPlatformConfigData, partition = %d, option = %d, offset = %d\n", OS_PARTITION, RetrieveOption, Offset);

      Status = GetPlatformConfigData (Host, Socket, Ch, Dimm, OS_PARTITION, RetrieveOption, Offset, &FnvBuffer, &MbStatus);
      if (Status == FAILURE) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
        EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_COMM_FAILED;
        RcFreePool (TempCfgOutBuffer);
        return FAILURE;
      } else if ((Status == WARN_NVMCTRL_MEDIA_NOTREADY) || (Status == WARN_NVMCTRL_MEDIA_INERROR) || (Status == WARN_NVMCTRL_MEDIA_RESERVED)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Problem with Media Access\n");
        EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, (UINT8)Status, Socket, Ch, Dimm, NO_RANK);
        DdrtDimmInfo->NgnCfgErrorPresent = 1;
        DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_MEDIA_ERROR;
        RcFreePool (TempCfgOutBuffer);
        return FAILURE;
      }
      CopyMem (((UINT8 *)(TempCfgOutBuffer)) + (i * Length) , (UINT8 *)FnvBuffer.platformData, Length);
      Offset = Offset + Length;
    } // Counter loop
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OS partition(COUT) data size exceeded! \n");
    EwlOutputType1 (WARN_CFGOUT_STATUS, WARN_CFGOUT_DATASIZE_EXCEEDED, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_DATA_SIZE_ERROR;
    RcFreePool (TempCfgOutBuffer);
    return FAILURE;
  }

  //
  // Adjust back the temporary Input buffer based on the offsetAdjustment value.
  // (This is to negate the adjustment we made to read from 128byte boundary earlier.)
  //
  CfgOutBuffer = ((UINT8 *)(TempCfgOutBuffer)) + OffsetAdjustment;

  //
  // Initialize platform Confug input header
  //
  PlatformCfgOutputHeader = (NGN_DIMM_PLATFORM_CFG_HEADER *)CfgOutBuffer;

  //
  // Check for 'COUT' signature. Return if the signature doesnt match.
  //
  if (PlatformCfgOutputHeader->Signature != NGN_CONFIGURATION_OUTPUT_SIGNATURE) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_SIGNATURE_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "COUT signature not found in the NGN DIMM PCD 2.\n");
    DisplayBuffer (Host, CfgOutBuffer, ConfResponseDataSize);
    EwlOutputType1 (WARN_CFGOUT_STATUS, WARN_CFGOUT_SIGNATURE_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    RcFreePool (TempCfgOutBuffer);
    return FAILURE;
  }

  //
  // Check if the checksum passes
  //
  if (ChsumTbl((UINT8 *)CfgOutBuffer, PlatformCfgOutputHeader->Length)) {
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_CHECKSUM_ERROR;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Configuration Output Record checksum failure.\n");
    DisplayBuffer (Host, CfgOutBuffer, ConfResponseDataSize);
    EwlOutputType1 (WARN_CFGOUT_STATUS, WARN_CFGOUT_CHECKSUM_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    RcFreePool (TempCfgOutBuffer);
    return FAILURE;
  }

  //
  // Store revision info
  //
  DdrtDimmInfo->NgnCfgOutRev = PlatformCfgOutputHeader->Revision;

  //
  // Check if the revision is compatible
  //
  if (GetPmemPcdRecordsHeaderRevision () != PlatformCfgOutputHeader->Revision) {
    //
    //
    // Check if revision is compatible with current revision
    //
    if (!IsPmemPcdRecordsHeaderRevisionCompatible (PlatformCfgOutputHeader->Revision)) {
      DdrtDimmInfo->NgnCfgErrorPresent = 1;
      DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_COUT_REVISION_ERROR;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision mismatch (COUT)\n");
      DisplayBuffer (Host, CfgOutBuffer, ConfResponseDataSize);
      EwlOutputType1 (WARN_CFGOUT_STATUS, WARN_CFGOUT_REVISION_MISMATCH, Socket, Ch, Dimm, NO_RANK);
      RcFreePool (TempCfgOutBuffer);
      return FAILURE;
    } else {
      //
      // Revision is compatible, but not supported (CR DXE code will update header revision to inform software that new request needs new revision to be processed)
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Mgmt SW - BIOS interface revision deprecated (COUT)\n");
    }
  }

  //
  // Initialize platform Config Output body
  //
  PlatformCfgOutputBdy = (NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY *) ((UINT8 *)CfgOutBuffer + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER));

  //
  // Fill in the sequence number for this CfgOut record in the host struct
  //
  DdrtDimmInfo->NgnCfgOut.body.SequenceNumber = PlatformCfgOutputBdy->SequenceNumber;
  DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = PlatformCfgOutputBdy->ValidationStatus;


  //
  // We don't need to read the PCAT tables like interleave info and partition size for CFG_OUT records.
  // Set the CfgOut as ready for processing
  //
  DdrtDimmInfo->NgnCfgOutPresent = 1;

  RcFreePool (TempCfgOutBuffer);
  return SUCCESS;
}

/**

  This routine verifies whether new configuration request is present

  @param[in] Socket       - socket number
  @param[in] Ch           - Channel number
  @param[in] Dimm         - Dimm number

  @retval TRUE New configuration request present, FALSE otherwise

**/
BOOLEAN
OsPartitionIsNewRequestPresent (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  if ((Socket >= MAX_SOCKET) || (Ch >= MAX_CH) || (Dimm >= MAX_DIMM)) {
    return FALSE;
  }

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  if ((((DdrtDimmInfo->NgnCfgReqPresent == 1) && (DdrtDimmInfo->NgnCfgOutPresent == 0)) ||
      ((DdrtDimmInfo->NgnCfgReqPresent == 1) && (DdrtDimmInfo->NgnCfgOutPresent == 1) &&
       (DdrtDimmInfo->NgnCfgReq.body.SequenceNumber != DdrtDimmInfo->NgnCfgOut.body.SequenceNumber))) &&
      (DdrtDimmInfo->NgnCfgReqRev == GetPmemPcdRecordsHeaderRevision ()) ){
    return TRUE;
  }

  return FALSE;
}

/**

  This routine to issue getplatformconfigdata command and read the  configuration Input requests from the the dimm
  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number

  @retval SUCCESS
**/
UINT32
ReadOSPartition (
  IN SYSHOST *Host,
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   Dimm
  )
{
  UINT8                                   RetrieveOption = RETRIEVE_PARTITION_DATA;
  UINT8                                   MbStatus = 0;
  FNV_CIS_BUFFER                          FnvBuffer;
  NGN_DIMM_PLATFORM_CFG_HEADER            *PlatformCfgHdr;
  NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY  *PlatformCfgBdy;
  UINT32                                  Offset;
  UINT32                                  Status = SUCCESS;
  DDRT_DIMM_DEVICE_INFO_STRUCT            *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  Offset = 0;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ReadOSPartition->GetPlatformConfigData, partition = %d, option = %d, offset = %d\n", OS_PARTITION, RetrieveOption, Offset);

  Status = GetPlatformConfigData (Host, Socket, Ch, Dimm, OS_PARTITION, RetrieveOption, Offset, &FnvBuffer, &MbStatus);
  if (Status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
    EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
    DisableDIMM (Host, Socket, Ch, Dimm);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_COMM_FAILED;
    return FAILURE;
  } else if ((Status == WARN_NVMCTRL_MEDIA_NOTREADY) || (Status == WARN_NVMCTRL_MEDIA_INERROR) || (Status == WARN_NVMCTRL_MEDIA_RESERVED)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Problem with Media Access\n");
    EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, (UINT8)Status, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_MEDIA_ERROR;
    return FAILURE;
  }

  //
  // Initialize pointers to the header and body
  //
  PlatformCfgHdr = (NGN_DIMM_PLATFORM_CFG_HEADER *) &FnvBuffer.platformData;

  //
  // Check for 'DMHD' signature. Return if the signature doesnt match
  //
  if (PlatformCfgHdr->Signature != NGN_CONFIGURATION_HEADER_SIGNATURE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DMHD signature not found in the PMem DIMM Partition %d\n", OS_PARTITION);
    DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_SIGNATURE_ERROR;
    return SUCCESS;
  }

  //
  // Check if the revision is compatible
  //
  if (GetPmemConfigHeaderRevision () != PlatformCfgHdr->Revision) {
    //
    // Check if revision is valid
    //
    if (!IsPmemConfigHeaderRevisionCompatible (PlatformCfgHdr->Revision)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(PMem DIMM Partition %d) PMem Mgmt SW - BIOS interface revision mismatch (DMHD)\n", OS_PARTITION);
      DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
      EwlOutputType1 (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_REVISION_MISMATCH, Socket, Ch, Dimm, NO_RANK);
      DdrtDimmInfo->NgnCfgErrorPresent = 1;
      DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_HDR_REVISION_ERROR;
      return FAILURE;
    } else {
      //
      // Revision is compatible, CR DXE will update header revision to the current spec revision
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(PMem DIMM Partition %d) PMem Mgmt SW - BIOS interface revision deprecated (DMHD)\n", OS_PARTITION);
    }
  }

  //
  // Check buffer length
  //
  if (PlatformCfgHdr->Length > (sizeof (UINT32) * NUM_PAYLOAD_REG)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OS Partition length invalid.\n");
    DisplayBuffer (Host, (UINT8 *) PlatformCfgHdr, 128);
    EwlOutputType1 (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_LENGTH_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_DATA_SIZE_ERROR;
    return FAILURE;
  }

  //
  // Check if the checksum passes
  //
  if (ChsumTbl((UINT8 *)&FnvBuffer.platformData, PlatformCfgHdr->Length)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OS Partition DMHD checksum failure.\n");
    DisplayBuffer (Host, (UINT8 *)PlatformCfgHdr, 128);
    EwlOutputType1 (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_CHECKSUM_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    DdrtDimmInfo->NgnCfgErrorPresent = 1;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_CHECKSUM_ERROR;
    return FAILURE;
  }

  PlatformCfgBdy = (NGN_DIMM_PLATFORM_CFG_DESCRIPTION_BODY *) (((UINT8 *)&FnvBuffer.platformData) + sizeof(NGN_DIMM_PLATFORM_CFG_HEADER));
  if (PlatformCfgBdy->ConfRequestDataSize > 0) {
    Status = ReadCfgInTable (Host, Socket, Ch, Dimm, PlatformCfgBdy->ConfRequestDataOffset, PlatformCfgBdy->ConfRequestDataSize);
    if (Status == FAILURE) {
      return FAILURE;
    }
  }

  if (PlatformCfgBdy->ConfResponseDataSize > 0) {
    Status = ReadCfgOutTable (Host, Socket, Ch, Dimm, PlatformCfgBdy->ConfResponseDataOffset, PlatformCfgBdy->ConfResponseDataSize);
  }

  return Status;
}

/**

  This routine verifies whether new DIMM is present in the system
  and takes appropriate actions

  @param[in] Socket       - socket number
  @param[in] Ch           - Channel number
  @param[in] Dimm         - Dimm number

  @retval TRUE new DIMM is present in the system, FALSE otherwise

**/
VOID
CheckAndProcessNewDimm (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  if ((Socket >= MAX_SOCKET) || (Ch >= MAX_CH) || (Dimm >= MAX_DIMM)) {
    return;
  }

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  //
  // Initially assume that DIMM is not new to the system
  //
  DdrtDimmInfo->NewDimm = FALSE;

  //
  // No errors found no need to proceed further
  // (DIMM is not new in the system and PCD is healthy)
  //
  if (!DdrtDimmInfo->NgnCfgErrorPresent) {
    return;
  }

  if ((DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] == NGN_DIMM_SIGNATURE_ERROR) &&
      (DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] == NGN_DIMM_SIGNATURE_ERROR)) {
    //
    // BIOS and OS partition  DMHD signatures was not found we
    // can assume that new NVDIMM is populated in the system
    // (clear errors to process new DIMM correctly)
    //
    DdrtDimmInfo->NgnCfgErrorPresent = 0;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] = NGN_DIMM_NO_ERROR;
    DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] = NGN_DIMM_NO_ERROR;

    //
    // Set new DIMM flag
    //
    DdrtDimmInfo->NewDimm = TRUE;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "New PMem DIMM detected\n");
    return;
  }

  if (DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_BIOS_PARTITION] == NGN_DIMM_SIGNATURE_ERROR) {
    //
    // BIOS partition corruption detected, check if new request is present
    // (try to recover only when user requested new provisioning / re-provisioning)
    //
    if (!OsPartitionIsNewRequestPresent (Socket, Ch, Dimm)) {
      DdrtDimmInfo->NgnCfgReqPresent = 0;
      DdrtDimmInfo->NgnCfgOutPresent = 0;
    }
    EwlOutputType1 (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_SIGNATURE_MISMATCH, Socket, Ch, Dimm, NO_RANK);
    return;
  }

  if (DdrtDimmInfo->NgnCfgError.PcdPartitionError[NGN_DIMM_PCD_OS_PARTITION] == NGN_DIMM_SIGNATURE_ERROR) {
    //
    // OS partition corruption detected
    //
    EwlOutputType1 (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_SIGNATURE_MISMATCH, Socket, Ch, Dimm, NO_RANK);
  }
}

/**

  This routine checks against several criteria to see if we have mismatched DIMMs in the system.
  We currently do not support DIMMs of different SKU or different EKV stepping.

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS - if all the NVMDIMMs in the system match.
  @retval FAILURE - if all the NVMDIMMs in the system do not match

**/
UINT32
CheckForIncompatibleDimms (
  IN SYSHOST *Host
  )
{
  UINT8                  Socket;
  UINT8                  Channel;
  UINT8                  Dimm;
  UINT8                  Sku = 0x00;
  UINT8                  Step = 0xFF;
  MEM_NVRAM_STRUCT      *NvramMem;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT    (*DimmNvList)[MAX_DIMM];
  SYS_SETUP              *Setup;
  UINT8                  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  NvramMem = &Host->nvram.mem;

  // For each socket
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    ChannelNvList = &NvramMem->socket[Socket].channelList;

    // For each channel
    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if ((*ChannelNvList)[Channel].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Socket].channelList[Channel].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }


        if (!IsNvDimmEnabled (Host, Socket, Channel, Dimm)) {
          continue;
        }

        //
        // Scan through each DIMM to check Far Memory Controller Type
        //
        if ((*DimmNvList)[Dimm].fmcType == FMC_EKV_TYPE || (*DimmNvList)[Dimm].fmcType == FMC_FNV_TYPE) {
          Host->var.mem.EkvPresent = TRUE;
        } else if ((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) {
          Host->var.mem.BwvPresent = TRUE;
        }

        // Scan through each DIMM to see if the SKU information matches
        if (Sku == 0x00) {
          // Store SKU information
          Sku = (*DimmNvList)[Dimm].dimmSku & DIMM_SKU_MASK;
          if ((*DimmNvList)[Dimm].dimmSku == 0x00) {
            continue; // context block failure DIMM, continue to boot
          }
        } else if ((*DimmNvList)[Dimm].dimmSku == 0x00) {
          continue; // context block failure DIMM, continue to boot
        } else if ( Sku != ((*DimmNvList)[Dimm].dimmSku & DIMM_SKU_MASK) ) {
          RcDebugPrintWithDevice (0xFFFF, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM SKU mismatch\n");
          if (Setup->mem.dfxMemSetup.dfxOptions & CR_MIXED_SKU) {
            FatalError (ERR_NGN, NGN_SKU_MISMATCH);
          } else {
            EwlOutputType1 (WARN_DIMM_COMPAT, WARN_DIMM_SKU_MISMATCH, Socket, Channel, Dimm, NO_RANK);
          }
          return FAILURE;
        }

        // Scan through each DIMM to see if the EKV stepping matches
        if (Step == 0xFF) {
          // Store EKV stepping read from SPD (byte 198)
          Step = (*DimmNvList)[Dimm].fmcRev;
        } else if (Step != (*DimmNvList)[Dimm].fmcRev) {
          RcDebugPrintWithDevice (0xFFFF, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM controller revision mismatch\n");
          if (Setup->mem.dfxMemSetup.dfxOptions & CR_MIXED_SKU) {
            FatalError (ERR_NGN, NGN_SKU_MISMATCH);
          } else {
            EwlOutputType1 (WARN_DIMM_COMPAT, WARN_DIMM_SKU_MISMATCH, Socket, Channel, Dimm, NO_RANK);
          }
          return FAILURE;
        }


      } // Dimm loop
    } // Channel loop
  } // Socket loop

  return SUCCESS;
}

/**
  This function gets the FMC type string according to the DCPMM dimm device ID.

  @param[in] DeviceId    Device Identification.

  @retval  FMC type string.
**/
CONST
CHAR8 *
GetFmcTypeString (
  IN UINT16       DeviceId
  )
{
  switch (DeviceId) {
  case FNV:
    return "PMEM100";
  case EKV:
    return "PMEM100";
  case BWV:
    return "PMEM200";
  case CWV:
    return "CWV";
  default:
    return "Unknown";
  }
}

/**

  This routine is to issue identifydim and getplatformconfigdata command and read the current configuration data from all the NGN dimms in the system.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS - if NGN DIMM PCD is read and the Host struct is populated.
  @retval FAILURE - if Dimm SKUs mismatch
**/
UINT32
GetDimmDeviceData (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT32                       Status = SUCCESS;
  UINT8                        Sckt;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT8                        i;
  MEM_NVRAM_STRUCT             *NvramMem;
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DIMM_INFO_STRUCT             DimmInfoList;
  FNV_CIS_BUFFER               FnvBuffer;
  DIMM_PARTITION_DATA          (*DimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT8                        MbStatus = 0;
  UINT64_STRUCT                TempData;
  BOOT_STATUS_REGISTER         Bsr;
  BOOT_STATUS_HIGH_REGISTER    BsrHi;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  MEMORY_MAP_HOST              *MemMapHost;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem (&DimmInfoList, sizeof (DimmInfoList));
  NvramMem = &Host->nvram.mem;

  //
  // WPQ Flush is supported only by CLX Silicon and AEP DIMMs with FW Build versions >= 0x4774.
  // Major revision 0x1/Minor revision 0x0
  //
  MemMapHost = GetMemMapHostPointer ();
  MemMapHost->IsWpqFlushSupported = TRUE;

  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }
    ChannelNvList = &NvramMem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }


        //Skip if this is not a NVMDIMM Dimm or it is disabled
        if (!IsNvDimmEnabled (Host, Sckt, Ch, Dimm)) {
          continue;
        }
        //
        // 1. IDENTIFY_DIMM
        // Issue identify Dimm to the NVMDIMM to get the raw capacity, Manufacturer id, serial number, and model number.
        //
        Status = IdentifyDimm (Host, Sckt, Ch, Dimm, AIT_DRAM_FLAG_CHECK_DISABLE, &DimmInfoList, &MbStatus);
        if (Status == SUCCESS) {
          //Update Dimm identification information in the NVRAM struct
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Firmware Revision: " );
          (*DimmNvList)[Dimm].firmwareRevision.majorVersion = DimmInfoList.fwr.majorVersion;
          (*DimmNvList)[Dimm].firmwareRevision.minorVersion = DimmInfoList.fwr.minorVersion;
          (*DimmNvList)[Dimm].firmwareRevision.hotfixVersion = DimmInfoList.fwr.hotfixVersion;
          (*DimmNvList)[Dimm].firmwareRevision.buildVersion = DimmInfoList.fwr.buildVersion;

          if (DimmInfoList.fwr.majorVersion == AEP_MAJOR_VERSION_WPQ_ENABLED
              && DimmInfoList.fwr.minorVersion ==  AEP_MINOR_VERSION_WPQ_ENABLED
              && DimmInfoList.fwr.buildVersion < AEP_BUILD_NUMBER_WPQ_ENABLED) {
            MemMapHost->IsWpqFlushSupported = FALSE;
          }

          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "%02X %02X %02X %04X\n",
              (*DimmNvList)[Dimm].firmwareRevision.majorVersion, (*DimmNvList)[Dimm].firmwareRevision.minorVersion,
              (*DimmNvList)[Dimm].firmwareRevision.hotfixVersion, (*DimmNvList)[Dimm].firmwareRevision.buildVersion);
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Manufacturer: " );
          CopyMem(&(*DimmNvList)[Dimm].manufacturer[0], &DimmInfoList.mf[0], NGN_MAX_MANUFACTURER_STRLEN);
          for (i = 0 ; i < NGN_MAX_MANUFACTURER_STRLEN ; i++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "%02X", (*DimmNvList)[Dimm].manufacturer[i]);
          }
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SerialNumber: " );
          CopyMem(&(*DimmNvList)[Dimm].serialNumber[0], &DimmInfoList.sn[0], NGN_MAX_SERIALNUMBER_STRLEN);
          for (i = 0 ; i < NGN_MAX_SERIALNUMBER_STRLEN ; i++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "%02X", (*DimmNvList)[Dimm].serialNumber[i]);
          }
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PartNumber: " );
          CopyMem(&(*DimmNvList)[Dimm].PartNumber[0], &DimmInfoList.Pn[0], NGN_MAX_PARTNUMBER_STRLEN);
          for (i = 0 ; i < NGN_MAX_PARTNUMBER_STRLEN ; i++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "%c", (*DimmNvList)[Dimm].PartNumber[i]);
          }
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Unique ID: ");
          CopyMem(&(*DimmNvList)[Dimm].Uniqueid[0], &DimmInfoList.Uid[0], NGN_MAX_UID_STRLEN);
          for (i = 0; i < NGN_MAX_UID_STRLEN; i++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "%02X ", (*DimmNvList)[Dimm].Uniqueid[i]);
          }
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
          (*DimmNvList)[Dimm].InterfaceFormatCode = DimmInfoList.ifc;
          (*DimmNvList)[Dimm].dimmSku = DimmInfoList.dimmSku;
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "dimmSku: 0x%02X \n", (*DimmNvList)[Dimm].dimmSku);
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "NVM Controller: %a (FIS: VID 0x%04X, DID 0x%04X, RID 0x%04X)\n",
            GetFmcTypeString (DimmInfoList.deviceID),
            DimmInfoList.vendorID,
            DimmInfoList.deviceID,
            DimmInfoList.revisionID
            );
          //
          // Do not copy VID/DID/RID from FIS IdentifyDimm response to DIMM structure.
          // In the strucutre we got JEDEC values from SPD. These are appropriate to use.
          //
          Host->var.mem.socket[Sckt].channelList[Ch].FisVersion[Dimm] = DimmInfoList.api;
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "NVM FIS version: %X.%02X\n", (Host->var.mem.socket[Sckt].channelList[Ch].FisVersion[Dimm] >> 8), (UINT8)Host->var.mem.socket[Sckt].channelList[Ch].FisVersion[Dimm]);
          // Assume the DIMM is not new ONLY on initial pass
          if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION) {
            DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);
            DdrtDimmInfo->NewDimm = FALSE;
          }

          //Update raw capacity of the Dimm, convert from 4kb granularity to 64MB granularity
          (*DimmNvList)[Dimm].rawCap = (UINT16) (DimmInfoList.rc >>CONVERT_4KB_TO_64MB_GRAN);
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "DIMM info RawCap:%d(64MB gran)  %d(4kb gran)\n",(*DimmNvList)[Dimm].rawCap, DimmInfoList.rc);
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt,Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
          //log data format:
          //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
          //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
          EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Sckt, Ch, Dimm, NO_RANK);
          DisableDIMM (Host, Sckt, Ch, Dimm);
        }
      }//Dimm
    }//  Ch
  } //Sckt

  //Check for dimms with incompatible SKUs and if the dimms are not compatible donot map the volatile/persistent region.
  Status = CheckForIncompatibleDimms (Host);
  if (Status != SUCCESS) {
    return Status;
  }

  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }
    ChannelNvList = &NvramMem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM Dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

        //If we are running in degraded memory mapping mode (MEM_MAP_LVL_IGNORE_CFGIN_IGNORE_NEW_DIMMS) and this is a new Dimm,
        //skip updating the size of the new dimms in the Host struct. This will make sure they are not mapped.
        if ((MemMapData->MemMapDegradeLevel >= MEM_MAP_LVL_IGNORE_CFGIN_IGNORE_NEW_DIMMS) && (DdrtDimmInfo->NewDimm)) {
          continue;
        }

        // Skip if Boot Status Register indicates media is disabled
        ReadBootStatusRegisters (Host, Sckt, Ch, Dimm, &Bsr, &BsrHi);
        if (!FNV_MEDIA_IS_USABLE (Bsr)) {
          //
          // Reset capacities stored in NVRAM since this DIMM can no longer be used until the media is re-enabled
          //
          (*DimmNvList)[Dimm].volCap = 0;
          (*DimmNvList)[Dimm].nonVolCap = 0;
          (*DimmNvList)[Dimm].perCap = 0;
          (*DimmNvList)[Dimm].WbCachePerCap = 0;

          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Media Disabled\n");
          EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Sckt, Ch, Dimm, NO_RANK);
          continue;
        }

        //
        // 2.GET_DIMM_PARTITION_INFO
        // call GetDimmPartitionInfo to get the volCap, nonVolCap and their DPAs.
        //
        Status = GetDimmPartitionInfo (Host, Sckt, Ch, Dimm, &FnvBuffer, &MbStatus);
        if (Status == SUCCESS) {

          DimmPartitionList = GetFnvCisBufferDimmPartition (&FnvBuffer);
          (*DimmNvList)[Dimm].volCap = (UINT16) ((*DimmPartitionList)[Sckt][Ch][Dimm].volatileCap  >> CONVERT_4KB_TO_64MB_GRAN);
          (*DimmNvList)[Dimm].nonVolCap = (UINT16) ((*DimmPartitionList)[Sckt][Ch][Dimm].persistentCap  >> CONVERT_4KB_TO_64MB_GRAN);
          //Start addresses are in 64byte granularity
          TempData = RShiftUINT64 ((*DimmPartitionList)[Sckt][Ch][Dimm].volatileStart, CONVERT_B_TO_64MB);
          (*DimmNvList)[Dimm].volRegionDPA = TempData.lo;
          TempData = RShiftUINT64 ((*DimmPartitionList)[Sckt][Ch][Dimm].persistentStart, CONVERT_B_TO_64MB);
          (*DimmNvList)[Dimm].perRegionDPA = TempData.lo;
          RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "VolDPA: %d   PerDPA: %d VolCap: %d NonVolCap: %d\n", (*DimmNvList)[Dimm].volRegionDPA,
            (*DimmNvList)[Dimm].perRegionDPA, (*DimmNvList)[Dimm].volCap, (*DimmNvList)[Dimm].nonVolCap);

        } else if (MbStatus == MB_STAT_MEDIA_DISABLED) {
          //
          // Reset capacities stored in NVRAM since this DIMM can no longer be used until the media is re-enabled
          // and skip the DIMM.
          //
          (*DimmNvList)[Dimm].volCap = 0;
          (*DimmNvList)[Dimm].nonVolCap = 0;
          (*DimmNvList)[Dimm].perCap = 0;
          (*DimmNvList)[Dimm].WbCachePerCap = 0;
          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem DIMM is Media Disabled\n");
          EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Sckt, Ch, Dimm, NO_RANK);
          continue;

        } else {

          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt,Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
          //log data format:
          //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
          //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
          EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Sckt, Ch, Dimm, NO_RANK);
          DisableDIMM (Host, Sckt, Ch, Dimm);
        }
      }//Dimm
    }//Ch
  } //Sckt

  return SUCCESS;
}

/**

Routine Description: SetDieSparingPolicy calls SetDieSparing per dimm.

  @param[in] Host   - Pointer to the system Host (root) structure
  @param[in] Socket - Socket affected

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
SetAepDieSparing (
  IN PSYSHOST       Host,
  IN UINT8          Socket
  )
{
  UINT8                     Ch;
  UINT8                     Dimm;
  DIMM_NVRAM_STRUCT         (*DimmNvList)[MAX_DIMM];
  CHANNEL_NVRAM_STRUCT      (*ChannelNvList)[MAX_CH];
  UINT8                     Enable;
  UINT8                     Supported = 0;
  UINT32                    Status = SUCCESS;
  UINT8                     MbStatus = 0;
  SYS_SETUP                 *Setup;
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    // Get the DimmNvList to see if any of them are NVMDIMMs
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      // Skip if DIMM/AEP DIMM not present
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      // Skip if Boot Status Register indicates media is disabled
      ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
      if (!FNV_MEDIA_IS_USABLE (Bsr)) {
        ShowBsr (Host, Socket, Ch, Dimm);
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM Media Disabled! Skip Die Sparing Policy setting\n");
        EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Socket, Ch, Dimm, NO_RANK);
        continue;
      }
      // Found an AepDimm in the channel, set die sparing on this DIMM if it's Die Sparing Capable
      if ((*DimmNvList)[Dimm].dimmSku & DIMM_SKU_PACKAGE_SPARING) {
        Status = GetDieSparingPolicy (Host, Socket, Ch, Dimm, &Enable, &Supported, &MbStatus);
        if (Status == FAILURE) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket,Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMEM DIMM failed\n");
          //log data format:
          //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
          //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
          EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
          DisableDIMM (Host, Socket, Ch, Dimm);
        }

        if (Supported == 0) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DieSparing not supported\n");
          continue;
        }

        Enable = Setup->mem.DieSparing;
        Status = SetDieSparingPolicy (Host, Socket, Ch, Dimm, Enable, &MbStatus);
        if (Status == FAILURE) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket,Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMEM DIMM failed\n");
          //log data format:
          //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
          //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
          EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
          DisableDIMM (Host, Socket, Ch, Dimm);
        }
      }
    }
  }
  return SUCCESS;
}

/**
  This routine checks if memory mode or app direct mode is enabled on the dimm.

  The routine assumes all dimms have the same Sku values.
  It performs following action if memomry mode or app direct mode sku bits are disabled:
  - Downgrade memory mapping to 1LM if memory mode is disabled in SKU (Bit 0)
  - Downgrade to NON_PER_MEM_MODE if app direct mode is disabled in SKU (Bit 2)

  @param[in] Host - Pointer to sysHost

  @retval
**/
VOID
CheckSkuForSupportedMode (
  IN  SYSHOST  *Host
  )
{
  UINT8                     Socket;
  UINT8                     Ch;
  UINT8                     Dimm;
  MEM_NVRAM_STRUCT          *NvramMem;
  CHANNEL_NVRAM_STRUCT      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT         (*DimmNvList)[MAX_DIMM];
  UINT8                     ReturnFlag;
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;
  ReturnFlag = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    ChannelNvList = &NvramMem->socket[Socket].channelList;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Socket].channelList[Ch].dimmList;
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Skip if this is not a NVMDIMM dimm or it is disabled
        //
        if (!IsNvDimmEnabled (Host, Socket, Ch, Dimm)) {
          continue;
        }

        //
        //  Memory mode not enabled
        //
        if (((*DimmNvList)[Dimm].dimmSku & DIMM_SKU_MEMORY_MODE) == 0) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Memory mode not enabled. Booting in 1Lm only. \n");
          Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
          ReturnFlag = 1;
        }

        //
        // AppDirect mode not enabled
        //
        if (((*DimmNvList)[Dimm].dimmSku & DIMM_SKU_APP_DIRECT) == 0) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "App Direct Mode not enabled. \n");
          Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
          ReturnFlag = 1;
        }

        if (ReturnFlag) {
          return;
        }
      }
    }
  }
}


/**
  This routine initializes the memory size fields in the structures
  for all the NGN DIMMs in the system

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval 0   - SUCCESS
**/
UINT32
InitializeNGNDIMM (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                        Socket;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT32                       Status;
  BOOT_STATUS_REGISTER         Bsr;
  BOOT_STATUS_HIGH_REGISTER    BsrHi;
  MEM_NVRAM_STRUCT             *NvramMem;
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;

  // Fill-in the dimm information returned from IdentifyDimm and GetDimmPartitionInfo commands.
  Status = GetDimmDeviceData (Host, MemMapData);

  CheckSkuForSupportedMode (Host);

  // If there is Failure because of SKU mismatch donot read PCDs or honor any interleaves.
  if (Status != FAILURE) {

    // If we have already calculated the resource requirements, return back.
    if (MemMapData->MemMapState != MEM_MAP_STATE_RESOURCE_CALCULATION) {
      return SUCCESS;
    }

    // For each socket
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (CheckSocketEnabled (Socket) == 0) {
        continue;
      }
      ChannelNvList = &NvramMem->socket[Socket].channelList;

      // For each channel
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }
        DimmNvList = &NvramMem->socket[Socket].channelList[Ch].dimmList;

        // For each DIMM
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }

          // Skip if this is not a NVMDIMM dimm or it is disabled
          if (!IsNvDimmEnabled (Host, Socket, Ch, Dimm)) {
            continue;
          }

          // Skip if Boot Status Register indicates media is disabled
          ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
          if (!FNV_MEDIA_IS_USABLE (Bsr)) {

            //
            // Reset capacities stored in NVRAM since this DIMM can no longer be used until the media is re-enabled
            //
            (*DimmNvList)[Dimm].volCap = 0;
            (*DimmNvList)[Dimm].nonVolCap = 0;
            (*DimmNvList)[Dimm].perCap = 0;
            (*DimmNvList)[Dimm].WbCachePerCap = 0;

            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM Media Disabled! PCD is not read!\n");
            EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Socket, Ch, Dimm, NO_RANK);
            continue;
          }

          DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

          // Read the PCD partition 1, to get the Current configuration.
          Status = ReadBIOSPartition (Host, Socket, Ch, Dimm);
          if (Status == FAILURE) {
            if (IsBiosPartitionErrorRecoverable (Host, Socket, Ch, Dimm)) {
              //
              // Read the OS partition, to check if we can recover BIOS partition corruption by new request
              //
              Status = ReadOSPartition (Host, Socket, Ch, Dimm);
              if (Status == FAILURE) {
                continue;
              }
              //
              // Check if new request is present (try to recover only when user requested new provisioning / re-provisioning)
              //
              if (!OsPartitionIsNewRequestPresent (Socket, Ch, Dimm)) {
                DdrtDimmInfo->NgnCfgReqPresent = 0;
                DdrtDimmInfo->NgnCfgOutPresent = 0;
                continue;
              }
            } else {
              //
              // BIOS partition error not recoverable
              //
              continue;
            }
          } else {
            //
            // Read the OS partition, to get the CIN and COUT
            //
            Status = ReadOSPartition (Host, Socket, Ch, Dimm);
            if (Status == FAILURE) {
              continue;
            }
          }

          //
          // Check if we detected the new DIMM or this is the BIOS/OS partition error
          //
          CheckAndProcessNewDimm (Socket, Ch, Dimm);
        }
      }
    }

    if (FeaturePcdGet (PcdCteBuild) == FALSE) {
      // Set NVMDIMM die sparing
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (CheckSocketEnabled (Socket) != 0) {
          SetAepDieSparing (Host, Socket);
        }
      }
    }
  } //Status = Failure(dimmSKU mismatch)

  // Init variables for blk window size and/or control region window size
  InitializeDdrtDimmChip (Host);

  return SUCCESS;
}

/**

  This routine propery handles DCPMMs unmapped due to population violation.

  @param[in] Host        - Pointer to sysHost

**/
VOID
HandleUnmappedDcpmms (
  IN SYSHOST *Host
  )
{
  UINT8                        Socket;
  UINT8                        Ch;
  UINT8                        Dimm;
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
      if (Host->nvram.mem.socket[Socket].channelList[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }
        if (!IsNvDimmEnabled (Host, Socket, Ch, Dimm)) {
          continue;
        }
        if (IsNvDimmUnmapped (Socket, Ch, Dimm, NULL)) {
          if (!IsNvDimmRemapped (Socket, Ch, Dimm)) {
            //
            // Unmapped DCPMMs have no usable capacity
            //
            (*DimmNvList)[Dimm].volCap = 0;
            (*DimmNvList)[Dimm].nonVolCap = 0;
            (*DimmNvList)[Dimm].perCap = 0;
            (*DimmNvList)[Dimm].WbCachePerCap = 0;
            //
            // Get DDRT DIMM Device info structure
            //
            DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);
            if (DdrtDimmInfo != NULL) {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                (DdrtDimmInfo->NewDimm == 1) ? "New PMem unmapped\n" : "PMem unmapped\n");
            }
          } else {
            Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmapped = 0;
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Remapping non-interleaved PMem\n");
          }
        }
      }
    }
  }
}

/**

  This routine validates  the Partition request records read from PCD1
  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - socket number
  @param[in] Ch       - Channel number
  @param[in] Dimm     -  dimm number
  @param[out] MbStatus -  MB status response code

  @retval PARTITION_TOO_BIG - if the partition size is greater than dimm size.
  @retval SECURITY_STATE_LOCKED - if the security state of the CR DIMM is locked
  @retval FNV_FW_ERROR - if the security state of the CR DIMM MB command fails
  @retval VALIDATION_SUCCESS - If validation succeeds
**/
UINT8
ValidatePartitionRequest (
  IN SYSHOST  *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  OUT UINT8   *MbStatus
  )
{
  UINT32                       Security = 0;
  UINT32                       OptStatus;
  UINT32                       Status = FAILURE;
  MEM_NVRAM_STRUCT             *NvramMem;
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  NvramMem = &Host->nvram.mem;
  DimmNvList = &NvramMem->socket[Socket].channelList[Ch].dimmList;
  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  //
  // Gather security info
  //
  Status = GetSecurityState (Host, Socket, Ch, Dimm, &Security, &OptStatus, MbStatus);

  if (Status == SUCCESS) {
    if (Security & SECURITY_LOCKED) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Security State - Locked\n");
      return SECURITY_STATE_LOCKED;
    }
  } else if (Status == FAILURE) {
    //
    // Disable DIMM
    //
    DisableDIMM (Host, Socket, Ch, Dimm);
    return FNV_FW_ERROR;
  }

  if (DdrtDimmInfo->NgnCfgReq.partitionSize.PartitionSize > (*DimmNvList)[Dimm].rawCap) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Partition Request Validation FAILED. - Partition requested too big.\n");
    return PARTITION_TOO_BIG;
  }

  return VALIDATION_SUCCESS;
}

/**

  This routine validates a specific Interleave request read from PCD1

  @param[in] Host                       - Pointer to sysHost
  @param[in] SourceInterleaveId         - Pointer to the InterleaveId array of the source
  @param[in] DestInterleaveId           - Pointer to the InterleaveId array of the destination
  @param[in] NumOfDimmsInInterleaveSet  - Number of DIMMs in the interleave

  @retval status

**/
UINT32
CompareDimmInterleaveId (
  IN SYSHOST                     *Host,
  IN NGN_DIMM_INTERLEAVE_ID_HOST *SourceInterleaveId,
  IN NGN_DIMM_INTERLEAVE_ID_HOST *DestInterleaveId,
  IN UINT8                        NumOfDimmsInInterleaveSet
  )
{
  UINT8 i;

  // Loop through the source DIMM interleave set
  for (i = 0; i < NumOfDimmsInInterleaveSet; i++) {
    RcDebugPrintWithDevice (
      SDBG_MAX,
      SourceInterleaveId->Socket,
      SourceInterleaveId->Channel,
      SourceInterleaveId->Dimm, NO_SUBCH,
      NO_RANK,
      NO_STROBE,
      NO_BIT,
      "Comparing DIMM Interleave ID (PartitionOffset 0x%x, PartitionSize 0x%x)\n",
      SourceInterleaveId->PartitionOffset,
      SourceInterleaveId->PartitionSize);
    if (CompareMem ((UINT8 *) SourceInterleaveId, (UINT8 *) DestInterleaveId, sizeof (NGN_DIMM_INTERLEAVE_ID_HOST))) {
      RcDebugPrintWithDevice (
        SDBG_MINMAX,
        DestInterleaveId->Socket,
        DestInterleaveId->Channel,
        DestInterleaveId->Dimm, NO_SUBCH,
        NO_RANK,
        NO_STROBE,
        NO_BIT,
        "DIMM Interleave ID Mismatch (PartitionOffset 0x%x, PartitionSize 0x%x)\n",
        DestInterleaveId->PartitionOffset,
        DestInterleaveId->PartitionSize);
      return FAILURE;
    }

    SourceInterleaveId++;
    DestInterleaveId++;
  } //i

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM Interleave IDs Match\n");
  return SUCCESS;
}

/**

  This routine validates the current channel mapping for an interleave request

  @param[in] InterleaveChannelBitmap   - Bitmap of active channels in the interleave
  @param[in] NumOfDimmsInInterleaveSet - Number of DIMMs in the interleave

  @retval status

**/
UINT32
ValidateInterleaveChannelMapping (
  IN UINT32  InterleaveChannelBitmap,
  IN UINT8   NumOfDimmsInInterleaveSet
  )
{
  UINT8         Index;
  UINT16        IntFmtSize;
  PM_INTERLEAVE *IntFormatList;
  UINT32        Status;

  Status = FAILURE;

  IntFmtSize = GetAppDirectNumIntFormats ();

  IntFormatList = RcAllocatePool (IntFmtSize * sizeof (PM_INTERLEAVE));

  if (IntFormatList == NULL) {
    RcDebugPrint (SDBG_MINMAX,"Failed to allocate struct IntFormatList!\n");
    return FAILURE;
  }

  GetAppDirectIntFormatList (IntFormatList);

  for (Index = 0; Index < IntFmtSize; Index++) {
    if (InterleaveChannelBitmap == IntFormatList[Index].InterleaveBitmap) {
      Status = SUCCESS;
      break;
    }
  }

  if (Status != SUCCESS) {
    RcDebugPrint (SDBG_MIN, "Invalid InterleaveChannelBitmap = 0x%x\n", InterleaveChannelBitmap);
  }
  RcFreePool (IntFormatList);
  return Status;
}

/**

  This routine validates the interleave set channel order

  @param NumOfDimmsInInterleaveSet - Number of DIMMs in the interleave
  #param DimmNum - DIMM number in the interleave set
  @param ImcNum - IMC number of the current DIMM in the interleave
  @param ImcChannelNum - IMC channel number of the current DIMM in the interleave
  @param PreviousImcNum - IMC number of the previous DIMM in the interleave
  @param PreviousImcChannelNum - IMC channel number of the previous DIMM in the interleave

  @retval status

**/
UINT32
ValidateInterleaveChannelOrder (
  IN UINT8  NumOfDimmsInInterleaveSet,
  IN UINT8  DimmNum,
  IN UINT8  ImcNum,
  IN UINT8  ImcChannelNum,
  IN UINT8  PreviousImcNum,
  IN UINT8  PreviousImcChannelNum
  )
{
  UINT32 Status = SUCCESS;

  if (GetNumChannelPerMc () == 2) {
    //
    // Verify for platforms having 2 Ch per Mc
    //
    Status = SUCCESS;
  } else {
    //
    // Verify for platforms having 3 Ch per Mc
    //
    switch (NumOfDimmsInInterleaveSet) {
    case 2:
      if (DimmNum) {
        if (ImcNum == PreviousImcNum) {
          // Lowest numbered channel must be the first DIMM in the two-way interleave set
          if (ImcChannelNum <= PreviousImcChannelNum) {
            Status = FAILURE;
          }
        } else {
          // Channel on the first MC must be the first DIMM in the two-way interleave set.
          if (ImcNum <= PreviousImcNum) {
            Status = FAILURE;
          }
        }
      }
      break;
    case 3:
      // CH0 must be the first DIMM in the interleave set, followed by CH1, and CH2.
      if (DimmNum != ImcChannelNum) {
        Status = FAILURE;
      }
      break;
    case 4:
      // Interleave sets between MCs must have DIMMs ordered from lowest channel on MC0, lowest channel on MC1,
      // highest channel on MC0, and highest channel on MC1.
      switch (DimmNum) {
        case 1:
        case 3:
          if ((ImcChannelNum != PreviousImcChannelNum) || (ImcNum <= PreviousImcNum)) {
            Status = FAILURE;
          }
          break;
        case 2:
          if ((ImcChannelNum <= PreviousImcChannelNum) || (ImcNum >= PreviousImcNum)) {
            Status = FAILURE;
          }
          break;
        default:
          break;
      }
      break;
    case 6:
      // MC0/CH0 must be the first DIMM in each interleave set, followed by MC1/CH1, MC0/CH2, MC1/CH0, MC0/CH1, MC1/CH2
      if (DimmNum == 0) {
        if (ImcChannelNum || ImcNum) {
          Status = FAILURE;
        }
      } else if (((DimmNum % 3) != ImcChannelNum) || (ImcNum == PreviousImcNum)) {
        Status = FAILURE;
      }
      break;
    default:
      // Not a valid interleave request
      Status = FAILURE;
      break;
    }
  }

  return Status;
}

/**

  This routine validates a specific interleave request read from PCD1

  @param [in] Host                           - Pointer to sysHost
  @param [in] MemMapData                     - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param [in] Socket                         - Socket number
  @param [in] Ch                             - Channel number
  @param [in] Dimm                           - DIMM number
  @param [in] InterleaveReqId                - Interleave request id
  @param [in] RecordType                     - Type of the record validated CFG_CUR/CFG_IN
  @param [out] InterleaveChannelBitmap       - bitmap of the current interleave set
  @retval status

**/
UINT8
ValidateInterleaveRequest (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           Ch,
  IN UINT8           Dimm,
  IN UINT8           InterleaveReqId,
  IN UINT8           RecordType,
  OUT UINT32         *InterleaveChannelBitmap
  )
{
  UINT8                                     i;
  UINT8                                     j;
  UINT8                                     NumOfDimmsInInterleaveSet;
  UINT8                                     SocketNum;
  UINT8                                     ImcNum;
  UINT8                                     ChannelNum;
  UINT8                                     ImcChannelNum;
  UINT8                                     DimmNum;
  UINT8                                     PreviousDimmSktNum = 0;
  UINT8                                     PreviousImcNum = 0;
  UINT8                                     PreviousImcChannelNum = 0;
  UINT8                                     Granularity;
  UINT8                                     MaxChPerMc;
  UINT8                                     DimmUnmappedReason;
  UINT16                                    MemType;
  UINT32                                    Status;
  MEM_VAR_STRUCT                            *Mem;
  MEM_NVRAM_STRUCT                          *NvramMem;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *SourceInterleave = NULL;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleave = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *SourceInterleaveId = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveId = NULL;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *ParticipatingDdrtDimmInfo;
  UINT8                                     MaxChDdr;

  NvramMem = &Host->nvram.mem;
  Mem = &Host->var.mem;
  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);
  MaxChPerMc = GetNumChannelPerMc ();

  *InterleaveChannelBitmap = 0;

  // Do not honor the PMEM records if dfxPerMemMode = NON_PER_MEM_MODE
  if (Mem->dfxMemVars.dfxPerMemMode == NON_PER_MEM_MODE) {
    return PMEM_REGIONS_NOT_PROCESSED;
  }

  //
  // Check Unique ID mismatch for CCUR records
  //
  if ((RecordType == CFG_CUR) && ((DdrtDimmInfo->NgnCfgCurUidsValid & BIT0) == DCPMM_CFG_CUR_UIDS_INVALID) && ((DdrtDimmInfo->NgnCfgCurUidsValid & BIT1) == DCPMM_DIMM_PRESENT)) {
    return INTERLEAVE_PARAMETER_MISMATCH;
  }

  // Find the memory type
  if (IS_DDR_2LM_OR_MIXED (Host) &&
      (MemMapData->PmemCaching == LegacyPmCaching)) {
    MemType = MemType2lmDdrWbCacheAppDirect;
  } else {
    MemType = MemType1lmAppDirect;
  }

  if (RecordType == CFG_IN) {
    Granularity = EncodeInterleaveGranularity (Host, MemType, DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].InterleaveSize);
    NumOfDimmsInInterleaveSet = DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].NumOfDimmsInInterleaveSet;
    SourceInterleave = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId];
    SourceInterleaveId = &DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][0];
  } else {
    Granularity = EncodeInterleaveGranularity (Host, MemType, DdrtDimmInfo->NgnCfgCur.interleave[InterleaveReqId].InterleaveSize);
    NumOfDimmsInInterleaveSet = DdrtDimmInfo->NgnCfgCur.interleave[InterleaveReqId].NumOfDimmsInInterleaveSet;
    SourceInterleave = &DdrtDimmInfo->NgnCfgCur.interleave[InterleaveReqId];
    SourceInterleaveId = &DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveReqId][0];
  }

  //
  // Confirm interleave granularity
  //
  if (Granularity == MEM_INT_GRAN_ERROR) {
    return INTERLEAVE_FORMAT_NOT_VALID;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "NumOfDimmsInInterleaveSet = %d\n", NumOfDimmsInInterleaveSet);
  MaxChDdr = GetMaxChDdr ();
  for (i = 0; i < NumOfDimmsInInterleaveSet; i++) {

    // Init temp variables based on DIMM ID struct
    if (RecordType == CFG_IN) {
      SocketNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Socket;
      ChannelNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Channel;
      DimmNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Dimm;
    } else {
      SocketNum = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveReqId][i].Socket;
      ChannelNum = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveReqId][i].Channel;
      DimmNum = DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveReqId][i].Dimm;
    }

    // Check for valid socket, ch, dimm number
    if (SocketNum >= MAX_SOCKET || ChannelNum >= MaxChDdr || DimmNum >= MAX_DIMM) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Participating DIMM has invalid socket %d, channel %d or DIMM %d number\n", SocketNum, ChannelNum, DimmNum);
      return INTERLEAVE_PARTICIPANT_UNAVAILABLE;
    }

    ParticipatingDdrtDimmInfo = GetDdrtDimmInfo (SocketNum, ChannelNum, DimmNum);

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RecordType = %d; Entry[%d] = S:%d;C:%d;D:%d\n", RecordType, i, SocketNum, ChannelNum, DimmNum);

    // Check if the dimms in the interleave list are present in the system.

    // Skip if there is no NVMDIMM in the specified location.
    if (NvramMem->socket[SocketNum].channelList[ChannelNum].dimmList[DimmNum].DcpmmPresent == 0) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Participating DIMM N%d.C%d.D%d is not present\n", SocketNum, ChannelNum, DimmNum);
      return INTERLEAVE_PARTICIPANT_UNAVAILABLE;
    }

    // Check if the participating DIMM's request has already been processed
    if ((RecordType == CFG_IN) &&
        (ParticipatingDdrtDimmInfo->NgnCfgOutPresent != 0) &&
        (ParticipatingDdrtDimmInfo->NgnCfgOut.body.SequenceNumber == ParticipatingDdrtDimmInfo->NgnCfgReq.body.SequenceNumber)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Participating DIMM N%d.C%d.D%d is already processed\n", SocketNum, ChannelNum, DimmNum);
      return INTERLEAVE_PARTICIPANT_UNAVAILABLE;
    }

    // Check if the participating DIMM has the proper table present
    if (((RecordType == CFG_CUR) && (ParticipatingDdrtDimmInfo->NgnCfgCurPresent == 0)) ||
        ((RecordType == CFG_IN) && (ParticipatingDdrtDimmInfo->NgnCfgReqPresent == 0))) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Participating DIMM N%d.C%d.D%d is missing required CFG record\n", SocketNum, ChannelNum, DimmNum);
      return MISSING_CFG_REQUEST;
    }

    // Check if the partition size contributed by this DIMM is aligned to the Interleave Alignment Size
    // of the interleave capability information
    // Skip if not aligned
    if ((SourceInterleaveId->PartitionSize % ((BIT0 << INTERLEAVE_ALIGNMENT_SIZE) >> CONVERT_B_TO_64MB)) != 0) {
      return INTERLEAVE_NOT_ALIGNED;
    }

    //
    // Count Adx1 DCPMM interleaving
    //
    CountAdx1DcpmmInterleave (MemMapData, Socket, Ch, Dimm, InterleaveReqId, RecordType);

    // Validate one way interleave
    if (NumOfDimmsInInterleaveSet == 1) {
      if ((SocketNum == Socket) && (ChannelNum == Ch) && (DimmNum == Dimm)) {
        MemMapData->MemMapSocket[Socket].AppDirectByOneChannelBitmap |= (BIT0 << Ch);

        // Mark ADx1 DCPMMs that have been unmapped due to a population violation for remapping
        if (IsNvDimmUnmapped (Socket, Ch, Dimm, &DimmUnmappedReason)) {
          if (DimmUnmappedReason == NVDIMM_UNMAPPED_POPULATION_NOT_POR) {
            Mem->socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmRemapped = 1;
          }
        }
        return VALIDATION_SUCCESS;
      } else {
        return INTERLEAVE_PARAMETER_MISMATCH;
      }
    }

    // Except for ADx1, unmapped DCPMMs cannot be used in any interleave
    if (IsNvDimmUnmapped (Socket, Ch, Dimm, NULL)) {
      Mem->socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmRemapped = 0;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave request failed due to unmapped PMem\n");
      return INTERLEAVE_PARTICIPANT_UNAVAILABLE;
    }

    // Interleave requests across sockets not supported
    if ((i != 0) && (SocketNum != PreviousDimmSktNum)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave requests across sockets not supported\n");
      return INTERLEAVE_PARAMETER_MISMATCH;
    }

    // Check all other interleave types

    *InterleaveChannelBitmap |= (1 << ChannelNum);
    ImcNum = ChannelNum / MaxChPerMc;
    ImcChannelNum = ChannelNum % MaxChPerMc;

    // Confirm interleave set channel order is correct
    Status = ValidateInterleaveChannelOrder (NumOfDimmsInInterleaveSet, i, ImcNum, ImcChannelNum, PreviousImcNum, PreviousImcChannelNum);
    if (Status != SUCCESS) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave Set Invalid\n");
      return INTERLEAVE_PARAMETER_MISMATCH;
    }

    Status = FAILURE;

    // Scan through the interleave requests of the participating dimm to find a matching request
    for (j = 0; j < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; j++) {
      if (RecordType == CFG_IN) {
        DestInterleave = &ParticipatingDdrtDimmInfo->NgnCfgReq.interleave[j];
        DestInterleaveId = &ParticipatingDdrtDimmInfo->NgnCfgReq.interleaveId[j][0];
      } else {
        DestInterleave = &ParticipatingDdrtDimmInfo->NgnCfgCur.interleave[j];
        DestInterleaveId = &ParticipatingDdrtDimmInfo->NgnCfgCur.interleaveId[j][0];
      }

      // Skip if there is no interleave record present
      if (!DestInterleave->RecPresent) {
        continue;
      }

      // Skip if basic Interleave information doesn't match
      if ((DestInterleave->NumOfDimmsInInterleaveSet != SourceInterleave->NumOfDimmsInInterleaveSet) ||
          (DestInterleave->InterleaveMemoryType != SourceInterleave->InterleaveMemoryType) ||
          (DestInterleave->InterleaveSize != SourceInterleave->InterleaveSize)) {
        continue;
      }

      // Compare the list of DIMMs participating in the interleave and see if they match
      Status = CompareDimmInterleaveId (Host, SourceInterleaveId, DestInterleaveId, NumOfDimmsInInterleaveSet);
      if (Status == SUCCESS) {
        break;
      }
    }

    // If we cannot find a matching interleave request, return error
    if (Status != SUCCESS) {
      return INTERLEAVE_PARAMETER_MISMATCH;
    }

    PreviousDimmSktNum = SocketNum;
    PreviousImcNum = ImcNum;
    PreviousImcChannelNum = ImcChannelNum;
  }

  // Check for a valid channel mapping
  Status = ValidateInterleaveChannelMapping (*InterleaveChannelBitmap, NumOfDimmsInInterleaveSet);
  if (Status != SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave Set Invalid\n");
    return INTERLEAVE_PARAMETER_MISMATCH;
  }

  if (NumOfDimmsInInterleaveSet == 1) {
    // Clear InterleaveChannelBitmap for non-interleaved cfgs
    *InterleaveChannelBitmap = 0;
  }

  return VALIDATION_SUCCESS;
}

/**

  This routine validates  the specific record read from PCD1 and stores the validation result in Host struct
  @param [in] Host                     - Pointer to sysHost
  @param [in] MemMapData               - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param [in] Socket                   - Socket number
  @param [in] Ch                       - Channel number
  @param [in] Dimm                     - Dimm number
  @param [in] RecordType               - Type of the record validated CFG_CUR/CFG_IN
  @param [out] InterleaveChannelBitmap - bitmap of the current interleave set

  @retval SUCCESS - Validation of the record successful
  @retval FAILURE - Validation of the record failed

**/
UINT32
ValidateCfgRecords (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  UINT8              Socket,
  UINT8              Ch,
  UINT8              Dimm,
  UINT8              RecordType,
  OUT UINT32         *InterleaveChannelBitmap
  )
{
  UINT8                        Status;
  UINT8                        i;
  UINT8                        MbStatus = 0;
  BOOLEAN                      RecPresent = 0;
  BOOLEAN                      *RecValid = NULL;
  BOOLEAN                      ValidationFailureFlag = 0;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

  //Start updating the CfgOut records with proper Status if validation fails.
  if (RecordType == CFG_IN)  {
    DdrtDimmInfo->NgnCfgOut.body.SequenceNumber = DdrtDimmInfo->NgnCfgReq.body.SequenceNumber;
    //Init validation Status to 0 (Success), the failure conditions will be updated as necessary.
    DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_SUCCESS;
  }
  //Partition table is valid only for CFG_IN
  if (RecordType == CFG_IN) {
    //Validate the Partition request.
    if (DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent == 1) {

      //Update the CfgOut record
      DdrtDimmInfo->NgnCfgOut.partitionSize.RecPresent = 1;

      Status = ValidatePartitionRequest (Host, Socket, Ch, Dimm, &MbStatus);
      if (Status != VALIDATION_SUCCESS) {
        ValidationFailureFlag = 1;
        DdrtDimmInfo->NgnCfgReq.partitionSize.Valid = 0;
        DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = ((MbStatus << 8) | EncodePartitionResponse (Status));
      } else{
        DdrtDimmInfo->NgnCfgReq.partitionSize.Valid = 1;
      }
    }//partitionSize.RecPresent
  }//CFG_IN

  //Validate the Interleave request one by one.
  for (i = 0; i<MAX_UNIQUE_NGN_DIMM_INTERLEAVE; i++) {
    if (RecordType == CFG_IN) {
      RecPresent = DdrtDimmInfo->NgnCfgReq.interleave[i].RecPresent;
      RecValid = & (DdrtDimmInfo->NgnCfgReq.interleave[i].Valid);
      if (RecPresent) {
        //Update the CfgOut record
        DdrtDimmInfo->NgnCfgOut.interleave[i].RecPresent = 1;
      }
    } else if (RecordType == CFG_CUR) {
      RecPresent = DdrtDimmInfo->NgnCfgCur.interleave[i].RecPresent;
      RecValid = & (DdrtDimmInfo->NgnCfgCur.interleave[i].Valid);
    }
    if (RecPresent == 1) {
      Status = ValidateInterleaveRequest (Host, MemMapData, Socket, Ch, Dimm, i, RecordType, InterleaveChannelBitmap);
      if (Status != VALIDATION_SUCCESS) {

        *RecValid = 0;
        ValidationFailureFlag = 1;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave Info/Request Validation FAILED. ERROR Code: %d \n", Status);

        if (RecordType == CFG_IN) {
          // Update the CgfOut record
          DdrtDimmInfo->NgnCfgOut.interleave[i].InterleaveChangeResponseStatus = EncodeInterleaveResponse (Status);

          // If there is a Partition Size Change request, then update it so it is not applied to the system
          if ((DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent == 1) && (DdrtDimmInfo->NgnCfgReq.partitionSize.Valid == 1))  {
            DdrtDimmInfo->NgnCfgReq.partitionSize.Valid = 0;

            if (Status == INTERLEAVE_FORMAT_NOT_VALID)  {
              DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (NOT_PROCESSED);
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Partition Request Validation FAILED. ERROR Code: %d \n", NOT_PROCESSED);
            } else if (Status == INTERLEAVE_PARTICIPANT_UNAVAILABLE) {
              DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (INTERLEAVE_PARTICIPANT_UNAVAILABLE);
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Partition Request Validation FAILED. ERROR Code: %d \n", INTERLEAVE_PARTICIPANT_UNAVAILABLE);
            } else if (Status == INTERLEAVE_PARAMETER_MISMATCH) {
              DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (INTERLEAVE_PARAMETER_MISMATCH);
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Partition Request Validation FAILED. ERROR Code: %d \n", INTERLEAVE_PARAMETER_MISMATCH);
            } else if (Status == INSUFFICIENT_SILICON_RESOURCES) {
              DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (NOT_PROCESSED);
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Partition Request Validation FAILED due to insufficient silicon resources. ERROR Code: %d \n", NOT_PROCESSED);
            }
          }
        } else if (RecordType == CFG_CUR) {

          if ((Status == INTERLEAVE_PARTICIPANT_UNAVAILABLE) || (Status == MISSING_CFG_REQUEST)) {
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_ALL_DIMM_NOT_FOUND;
          } else if (Status == INTERLEAVE_PARAMETER_MISMATCH) {
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_PM_NOT_MAPPED;
          } else if (Status == INSUFFICIENT_SILICON_RESOURCES) {
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_UNDEFINED;
          }
        }

      } else {
        *RecValid = 1;

        // Check if we should remap only the persistent portion of an unmapped MM+ADx1 DCPMM
        if (IsNvDimmRemapped (Socket, Ch, Dimm)) {
          if (RecordType == CFG_CUR) {
            // Indicate we remapped the persistent portion of this ADx1 DCPMM (volCap will be cleared later)
            DdrtDimmInfo->NgnCfgCur.body.ConfigurationStatus = NGN_CFGCUR_RESPONSE_VOL_MEM_NOT_MAPPED;
          } else if ((RecordType == CFG_IN) &&
                     (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].volCap != 0)) {
            // Do not allow a partial CIN request
            Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmRemapped = 0;
            *RecValid = 0;
            ValidationFailureFlag = 1;
            DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_POPULATION_ISSUE;
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Interleave Request Validation FAILED due to a population issue\n");
          }
        }
      }
    }//recordPresent
  }//for

  if (ValidationFailureFlag != 1) {
    return SUCCESS;
  } else{
    return FAILURE;
  }
}



/**

  This routine goes through bothe CfgCur and CfgIn and validates the current config and config requests.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
ValidateNGNDimmData (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                        Sckt;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT8                        InterleaveSetNum;
  UINT32                       Status;
  UINT32                       InterleaveChannelBitmap;
  UINT32                       PrevReqInterleaveChannelBitmap;
  UINT32                       PrevCurInterleaveChannelBitmap;
  MEM_NVRAM_STRUCT             *NvramMem;
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  BOOLEAN                      PartitionCompliance;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;

  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ChannelNvList = &NvramMem->socket[Sckt].channelList;
    InterleaveChannelBitmap = 0;
    PrevReqInterleaveChannelBitmap = 0;
    PrevCurInterleaveChannelBitmap = 0;
    PartitionCompliance = TRUE;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Sckt].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM Dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        if (!IsNvDimmEnabled (Host, Sckt, Ch, Dimm)) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

        //Initialize NGN Dimm by populating the necessary fields in Host struct
        //Skip if this Dimm is new to the system
        if (DdrtDimmInfo->NewDimm == 1) {
          continue;
        }

        //If we donot have a CfgIn record, check if there is a CfgCur record that is read.
        if (DdrtDimmInfo->NgnCfgCurPresent == 1) {
          if (DdrtDimmInfo->NgnCfgCurRev != GetPmemPcdRecordsHeaderRevision ()){
            RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgCur revision deprecated - validate and convert to new revision\n");
          }
          //If found, validate the CfgCur record.
          Status = ValidateCfgRecords (Host, MemMapData, Sckt, Ch, Dimm, CFG_CUR, &InterleaveChannelBitmap);
          if (Status != SUCCESS) {
            DdrtDimmInfo->NgnCfgCurValid = 0;
            RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgCur Validation FAILED. \n");
          } else {
            DdrtDimmInfo->NgnCfgCurValid = 1;
            RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgCur Validation SUCCESSFUL. \n");
            if (InterleaveChannelBitmap) {
              if (PrevCurInterleaveChannelBitmap == 0) {
                PrevCurInterleaveChannelBitmap = InterleaveChannelBitmap;
              } else if (PrevCurInterleaveChannelBitmap != InterleaveChannelBitmap) {
                // Multiple interleave sets detected on the socket.  Generate warning
                PartitionCompliance = FALSE;
              }
            }
          }
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Could not find a CfgCur record to validate. \n");
        }


        //skip if there are no CfgIn records for this Dimm
        if ((DdrtDimmInfo->NgnCfgReqPresent == 1) && (DdrtDimmInfo->NgnCfgReqRev == GetPmemPcdRecordsHeaderRevision ())) {
          //Check if the request has been processed. If so, skip this Dimm.
          if ((DdrtDimmInfo->NgnCfgOutPresent != 0)  && (DdrtDimmInfo->NgnCfgOut.body.SequenceNumber == (DdrtDimmInfo->NgnCfgReq.body.SequenceNumber))) {
            continue;
          }

          //Validate the CfgReq records
          Status = ValidateCfgRecords (Host, MemMapData, Sckt, Ch, Dimm, CFG_IN, &InterleaveChannelBitmap);
          if (Status != SUCCESS) {
            DdrtDimmInfo->NgnCfgReqValid = 0;
            RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgReq Validation FAILED. \n");
          } else {
            DdrtDimmInfo->NgnCfgReqValid = 1;
            RcDebugPrintWithDevice (SDBG_MAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgReq Validation SUCCESSFUL. \n");
            if (InterleaveChannelBitmap) {
              if (PrevReqInterleaveChannelBitmap == 0) {
                PrevReqInterleaveChannelBitmap = InterleaveChannelBitmap;
              } else if (PrevReqInterleaveChannelBitmap != InterleaveChannelBitmap) {
                // Multiple interleave sets detected on the socket.  Generate warning
                PartitionCompliance = FALSE;
              }
            }
          }
        } else if (DdrtDimmInfo->NgnCfgReqPresent == 1) {
          if ((DdrtDimmInfo->NgnCfgOutPresent != 0)  && (DdrtDimmInfo->NgnCfgOut.body.SequenceNumber == (DdrtDimmInfo->NgnCfgReq.body.SequenceNumber))) {
            //
            // CIN and COUT with compatible revision present, BIOS will apply CCUR with new revision, don't process CIN and COUT records
            // (OS Partition header will be updated with current revision and CCUR table with new revision will be written to inform software
            // that further request needs new revision)
            //
            DdrtDimmInfo->NgnCfgReqPresent = 0;
            DdrtDimmInfo->NgnCfgOutPresent = 0;
            continue;
          }
          //
          // CIN request present with compatible, but not supported revision. BIOS will not process request. CCUR will be applied
          // (OS Partition header will be updated with current revision and CCUR table with new revision and COUT table with error code
          // will be written to inform software that request needs new revision to be processed)
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CfgReq revision deprecated - won't be applied. \n");
          //
          // Fill error codes (it will be used after memory mapping is done to create proper COUT table)
          //
          DdrtDimmInfo->NgnCfgReqValid = 0;
          DdrtDimmInfo->NgnCfgReq.partitionSize.Valid =0;
          DdrtDimmInfo->NgnCfgReq.partitionSize.ResponseStatus = NGN_PART_SIZE_STAT_UNDEFINED;
          for (InterleaveSetNum = 0; InterleaveSetNum < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveSetNum++) {
            if (DdrtDimmInfo->NgnCfgReq.interleave[InterleaveSetNum].RecPresent) {
              DdrtDimmInfo->NgnCfgReq.interleave[InterleaveSetNum].Valid = 0;
              DdrtDimmInfo->NgnCfgOut.interleave[InterleaveSetNum].InterleaveChangeResponseStatus = NGN_INT_CH_STAT_NOT_PROCESSED;
            }
          }
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Could not find a CfgReq record to validate. \n");
        }
      }//Dimm
    }//Ch

    if (PartitionCompliance == FALSE) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DDRT partition mismatch on Socket\n");
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDRT_PARTITION_MISMATCH, Sckt, NO_CH, NO_DIMM, NO_RANK);
    }
  }//socket

  return SUCCESS;
}

/**

  This routines clears the Interleaverequests related to a specific Dimm

  @param Host     - Pointer to sysHost
  @param Sckt     - socket number
  @param Ch       - Channel number
  @param Dimm     - Dimm number

  @retval SUCCESS

**/
UINT32
HandlePartitionFailure (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   NvmDimmStatusCode
  )
{

  UINT8                                     NumOfDimmsInInterleaveSet = 0;
  UINT8                                     i;
  UINT8                                     j;
  UINT8                                     SocketNum;
  UINT8                                     ChannelNum;
  UINT8                                     DimmNum;
  UINT8                                     InterleaveReqId;
  UINT32                                    Status = SUCCESS;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *SourceInterleave = NULL;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleave = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *SourceInterleaveId = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveId = NULL;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *ParticipatingDdrtDimmInfo;

  SocketNum = 0;
  ChannelNum = 0;
  DimmNum = 0;

  DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);

  //Update CfgOut for failure of partion request
  DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = ((NvmDimmStatusCode & 0xFF) << 8) | (EncodePartitionResponse (FNV_FW_ERROR));
  DdrtDimmInfo->NgnCfgOut.body.ValidationStatus = NGN_CFGOUT_VLD_STAT_ERROR;

  for (InterleaveReqId = 0; InterleaveReqId<MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveReqId++) {

    //Break if the interve info is not valid.
    if (!DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].RecPresent || !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].Valid) {
      break;
    }

    //Set the Valid bit to 0 for all the interleaves.
    DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].Valid = 0;
    DdrtDimmInfo->NgnCfgOut.interleave[InterleaveReqId].InterleaveChangeResponseStatus = EncodeInterleaveResponse (PARTITIONING_REQUEST_FAILED);

    NumOfDimmsInInterleaveSet = DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId].NumOfDimmsInInterleaveSet;
    SourceInterleave = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveReqId];
    for (i = 0; i<NumOfDimmsInInterleaveSet; i++) {
      SocketNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Socket;
      ChannelNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Channel;
      DimmNum = DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][i].Dimm;
      SourceInterleaveId = &DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveReqId][0];
      ParticipatingDdrtDimmInfo = GetDdrtDimmInfo (SocketNum, ChannelNum, DimmNum);

      for (j = 0; j<MAX_UNIQUE_NGN_DIMM_INTERLEAVE; j++) {
        DestInterleave = &(ParticipatingDdrtDimmInfo->NgnCfgReq.interleave[j]);
        DestInterleaveId = &(ParticipatingDdrtDimmInfo->NgnCfgReq.interleaveId[j][0]);
        //skip if basic Interleave information doesnt match
        if ((DestInterleave->NumOfDimmsInInterleaveSet != SourceInterleave->NumOfDimmsInInterleaveSet) && (DestInterleave->InterleaveMemoryType != SourceInterleave->InterleaveMemoryType)) {
          continue;
        }

        //Compare the list of dimms participating in the interleave and see if they match
        Status = CompareDimmInterleaveId (Host, SourceInterleaveId, DestInterleaveId, NumOfDimmsInInterleaveSet);
        if (Status == SUCCESS) {
          //We have found a matching interleave request in another Dimm, clear the valid bit for this interleave set.
          ParticipatingDdrtDimmInfo->NgnCfgReq.interleave[j].Valid = 0;
          ParticipatingDdrtDimmInfo->NgnCfgOut.interleave[InterleaveReqId].InterleaveChangeResponseStatus = EncodeInterleaveResponse (NOT_PROCESSED);
        }
      }//j
    }//i
  }//InterleaveReqId
  return SUCCESS;
}


/**

This routine handles partitioning requests when the CR Mgmt software is managing the DIMMs

@param[in] Host        - Pointer to sysHost
@param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

@retval SUCCESS

**/
UINT32
HandlePartitionRequests (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8                        Socket;
  UINT8                        Channel;
  UINT8                        Dimm;
  UINT8                        MaxChPerMc;
  UINT32                       Status;
  UINT64_STRUCT                TempData;
  UINT32                       VolSize = 0;
  UINT32                       NonVolSize = 0;
  UINT8                        MbStatus = 0;
  MEM_NVRAM_STRUCT             *NvramMem;
  IMC_NVRAM_STRUCT             (*ImcNvList)[MAX_IMC];
  CHANNEL_NVRAM_STRUCT         (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT            (*DimmNvList)[MAX_DIMM];
  DDR_RANK_STRUCT              (*RankList)[MAX_RANK_DIMM];
  FNV_CIS_BUFFER               FnvBuffer;
  DIMM_PARTITION_DATA          (*DimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  SYS_SETUP                    *Setup;
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  MaxChPerMc = GetNumChannelPerMc ();

  NvramMem = &Host->nvram.mem;

  // For each socket
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    ImcNvList = &NvramMem->socket[Socket].imc;
    ChannelNvList = &NvramMem->socket[Socket].channelList;

    // For each channel
    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if ((*ChannelNvList)[Channel].enabled == 0) {
        continue;
      }
      DimmNvList = &NvramMem->socket[Socket].channelList[Channel].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM DIMM
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        RankList = &NvramMem->socket[Socket].channelList[Channel].dimmList[Dimm].rankList;

        //Skip if DIMM is disabled
        if (!IsNvDimmEnabled (Host, Socket, Channel, Dimm)) {
          continue;
        }

        //
        // Skip if DIMM unmapped (population POR mismatch/SKU Limit violation)
        //
        if (IsNvDimmUnmapped (Socket, Channel, Dimm, NULL)) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);

        //
        // If this DIMM is brand-new, then partition it to 100% memory mode in multiples of 1 GB
        //
        if (DdrtDimmInfo->NewDimm == 1) {
          //
          // Compute the sizes for the non-volatile and volatile partitions in 64MB granularity
          //
          (*DimmNvList)[Dimm].volCap = (*DimmNvList)[Dimm].rawCap & ~(MEM_1GB_AT_64MB_GRAN - 1);
          (*DimmNvList)[Dimm].nonVolCap = 0;

          // Reset PMEM hole size for this IMC
          (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize = DEFAULT_MEMORY_HOLE_GRANULARITY;

          if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION) {

            (*DimmNvList)[Dimm].volRegionDPA = DDRT_VOL_DPA_START;
            (*DimmNvList)[Dimm].perRegionDPA = (*DimmNvList)[Dimm].volRegionDPA + (*DimmNvList)[Dimm].volCap;

          } else if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION_DONE) {

            //
            // Partition the Dimm
            //
            VolSize = (*DimmNvList)[Dimm].volCap << CONVERT_64MB_TO_4KB_GRAN;
            NonVolSize = (*DimmNvList)[Dimm].nonVolCap << CONVERT_64MB_TO_4KB_GRAN;

            Status = SetDimmPartitionInfo (Host, Socket, Channel, Dimm, VolSize, NonVolSize, &FnvBuffer, &MbStatus);

            if (Status == SUCCESS) {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SetDimmPartitionInfo, Status(SUCCESS); MbStatus=%d \n", MbStatus);
              //
              // Start addresses are in byte granularity
              //
              DimmPartitionList = GetFnvCisBufferDimmPartition (&FnvBuffer);
              TempData = RShiftUINT64 ((*DimmPartitionList)[Socket][Channel][Dimm].volatileStart, CONVERT_B_TO_64MB);
              (*DimmNvList)[Dimm].volRegionDPA = TempData.lo;
              TempData = RShiftUINT64 ((*DimmPartitionList)[Socket][Channel][Dimm].persistentStart, CONVERT_B_TO_64MB);
              (*DimmNvList)[Dimm].perRegionDPA = TempData.lo;

            } else {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SetDimmPartitionInfo, Status(FAILURE); MbStatus=%d \n", MbStatus);
              //
              // Partitioning failed, so there is no volatile or non-voaltile capacity to map for this Dimm
              //
              (*DimmNvList)[Dimm].nonVolCap = 0;
              (*DimmNvList)[Dimm].volCap = 0;
              (*DimmNvList)[Dimm].volRegionDPA = 0;
              (*DimmNvList)[Dimm].perRegionDPA = 0;
            }
          }
          continue;
        }

        if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          // Make sure AppDirectHoleSize has been initialized
          if ((*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize == 0) {
            (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize = DEFAULT_MEMORY_HOLE_GRANULARITY;
          }
        }

        //
        // check if there are partition request is present and valid for this dimm.
        //
        if (DdrtDimmInfo->NgnCfgReqPresent  && DdrtDimmInfo->NgnCfgReq.partitionSize.RecPresent && DdrtDimmInfo->NgnCfgReq.partitionSize.Valid)  {

          //
          // Compute the sizes for the non-volatile and volatile partitions in 64MB granularity
          //
          (*DimmNvList)[Dimm].nonVolCap = (UINT16) DdrtDimmInfo->NgnCfgReq.partitionSize.PartitionSize;
          if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
            if ((Setup->mem.AppDirectMemoryHole) && ((*DimmNvList)[Dimm].nonVolCap < (*DimmNvList)[Dimm].rawCap)) {
              // Add padding in case a PMEM memory hole is needed.
              while (((*DimmNvList)[Dimm].nonVolCap + (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize) > (*DimmNvList)[Dimm].rawCap) {
                (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize /= 2;
              }

              if (((*DimmNvList)[Dimm].nonVolCap) && ((*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize)) {
                (*DimmNvList)[Dimm].nonVolCap += (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize;
              }
            } else if ((Setup->mem.AppDirectMemoryHole) && ((((*DimmNvList)[Dimm].nonVolCap & GB_ALIGN_NVM) + (*ImcNvList)[Channel/MAX_MC_CH].AppDirectHoleSize) > (*DimmNvList)[Dimm].rawCap)) {
              (*ImcNvList)[Channel/MAX_MC_CH].AppDirectHoleSize /=2;
            }
            (*DimmNvList)[Dimm].volCap = ((UINT16) ((*DimmNvList)[Dimm].rawCap - (*DimmNvList)[Dimm].nonVolCap)) & ~(MEM_1GB_AT_64MB_GRAN - 1);
          } else {
            (*DimmNvList)[Dimm].volCap = ((UINT16) ((*DimmNvList)[Dimm].rawCap - DdrtDimmInfo->NgnCfgReq.partitionSize.PartitionSize)) & ~(MEM_1GB_AT_64MB_GRAN - 1);
          }
          if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION) {

            (*DimmNvList)[Dimm].volRegionDPA = DDRT_VOL_DPA_START;
            (*DimmNvList)[Dimm].perRegionDPA = (*DimmNvList)[Dimm].volRegionDPA + (*DimmNvList)[Dimm].volCap;

          } else if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION_DONE) {

            //
            // Compute the sizes for the non-volatile and volatile partitions in 4KB granularity
            //
            NonVolSize = (*DimmNvList)[Dimm].nonVolCap << CONVERT_64MB_TO_4KB_GRAN;
            VolSize = (*DimmNvList)[Dimm].volCap << CONVERT_64MB_TO_4KB_GRAN;

            Status = SetDimmPartitionInfo (Host, Socket, Channel, Dimm, VolSize, NonVolSize, &FnvBuffer, &MbStatus);

            if (Status == SUCCESS) {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SetDimmPartitionInfo, Status(SUCCESS); MbStatus=%d \n", MbStatus);

              //
              // Update CfgOut for success of partition request
              //
              DdrtDimmInfo->NgnCfgOut.partitionSize.ResponseStatus = EncodePartitionResponse (PARTITIONING_SUCCESS);

              //
              // Start addresses are in byte granularity
              //
              DimmPartitionList = GetFnvCisBufferDimmPartition (&FnvBuffer);
              TempData = RShiftUINT64 ((*DimmPartitionList)[Socket][Channel][Dimm].volatileStart, CONVERT_B_TO_64MB);
              (*DimmNvList)[Dimm].volRegionDPA = TempData.lo;
              TempData = RShiftUINT64 ((*DimmPartitionList)[Socket][Channel][Dimm].persistentStart, CONVERT_B_TO_64MB);
              (*DimmNvList)[Dimm].perRegionDPA = TempData.lo;

            } else {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SetDimmPartitionInfo, Status(FAILURE); MbStatus=%d \n", MbStatus);

              //
              // If the partition request failed, clear all the interleave requests from this dimm.
              //
              HandlePartitionFailure (Host, Socket, Channel, Dimm, MbStatus);

              //
              // Partitioning failed, so there is no volatile or non-voaltile capacity to map for this dimm
              //
              (*DimmNvList)[Dimm].nonVolCap = 0;
              (*DimmNvList)[Dimm].volCap = 0;
              (*DimmNvList)[Dimm].volRegionDPA = 0;
              (*DimmNvList)[Dimm].perRegionDPA = 0;
            }
          }
        } else if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && DdrtDimmInfo->NgnCfgOutPresent != 0) {
          //  Need to check AppDirectHoleSize
          if ((Setup->mem.AppDirectMemoryHole) && ((*DimmNvList)[Dimm].nonVolCap)) {
            while ((*DimmNvList)[Dimm].nonVolCap < (DdrtDimmInfo->NgnCfgOut.partitionSize.PartitionSize + (*ImcNvList)[Channel/MAX_MC_CH].AppDirectHoleSize)) {
              (*ImcNvList)[Channel/MaxChPerMc].AppDirectHoleSize /= 2;
            }
          }
          if ((Setup->mem.AppDirectMemoryHole) && ((((*DimmNvList)[Dimm].nonVolCap & GB_ALIGN_NVM) + (*ImcNvList)[Channel/MAX_MC_CH].AppDirectHoleSize + ((*DimmNvList)[Dimm].volCap)) > (*DimmNvList)[Dimm].rawCap)) {
            (*ImcNvList)[Channel/MAX_MC_CH].AppDirectHoleSize /=2;
          }
        }

        //
        // Clear volatile memory capacity for remapped ADx1 DCPMMs to avoid non-POR 2LM configurations
        //
        if (IsNvDimmRemapped (Socket, Channel, Dimm)) {
          (*DimmNvList)[Dimm].volCap = 0;
        }
      }
    }
  }

  //Re-initialize variables for blk window size to handle any dimms that may have been updated with a paritioning request
  InitializeDdrtDimmChip (Host);

  return SUCCESS;
}

/**

  This gets the socket interleave information from the interleave request.

  @param[in]      InterleavePtr    - Pointer to interleave table
  @param[in]      InterleaveIdPtr  - Pointer to the first DIMM info struct for this interleave set
  @param[in, out] ScktInterleave     - Pointer to the socket interleave structure
  @param[in, out] SktWays           - Pointer to the variable to store total IMC ways

  @retval SUCCESS

**/
UINT32
GetScktInterleaveInfo (
  IN     NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *InterleavePtr,
  IN     NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveIdPtr,
  IN OUT UINT32                                    ScktInterleave[MC_MAX_NODE],
  IN OUT UINT8                                     *SktWays
  )
{
  UINT8 Index;
  UINT8 Socket;
  UINT8 Mc;
  UINT8 NumOfDimmsInInterleaveSet;
  UINT8 MaxImc;
  UINT8 MaxChPerMc;

  NumOfDimmsInInterleaveSet = InterleavePtr->NumOfDimmsInInterleaveSet;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  //
  // Loop through the source InterleaveDimm set
  //
  for (Index = 0; Index < NumOfDimmsInInterleaveSet; Index++) {
    //
    // Find the socket number and MC number for this DIMM
    //
    Socket = InterleaveIdPtr->Socket;
    Mc = InterleaveIdPtr->Channel / MaxChPerMc;

    if (ScktInterleave[ (Socket * MaxImc) + Mc] == 0) {
      //
      // Update the socket interleave structure for every MC that is part of the interleave.
      //
      ScktInterleave[ (Socket * MaxImc) + Mc] = 1;
      (*SktWays)++;
    }
    InterleaveIdPtr++;
  }

  return SUCCESS;
}

/**

  This gets the channel interleave information across all participating MC's from the interleave request.

  @param[in]      InterleavePtr      - Pointer to the first interleave set
  @param[in]      InterleaveIdPtr    - Pointer to the first DIMM info struct for this interleave set
  @param[in, out] ChInterleaveBitMap - Pointer to the channel interleave bitmap
  @param[in, out] ChWays             - Pointer to the variable to store total channel ways

  @retval SUCCESS

**/
UINT32
GetChInterleaveInfo (
  IN     NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *InterleavePtr,
  IN     NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveIdPtr,
  IN OUT UINT8                                     *ChInterleaveBitMap,
  IN OUT UINT8                                     *ChWays
  )
{
  UINT8                       Index;
  UINT8                       McIndex;
  UINT8                       InterleaveId;
  UINT8                       Counter;
  UINT8                       SocketId;
  UINT8                       McId;
  UINT8                       ChannelBitMap[MC_MAX_NODE];
  UINT8                       NumOfDimmsInInterleaveSet;
  UINT8                       McParticipatedFlag;
  UINT8                       MaxImc;
  UINT8                       MaxChPerMc;
  NGN_DIMM_INTERLEAVE_ID_HOST *TempInterleaveIdPtr;

  *ChInterleaveBitMap = 0;
  Counter = 0;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  ZeroMem (&ChannelBitMap, sizeof (ChannelBitMap));

  NumOfDimmsInInterleaveSet = InterleavePtr->NumOfDimmsInInterleaveSet;
  //
  // Loop for each socket and each MC
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      McParticipatedFlag = 0;
      TempInterleaveIdPtr = InterleaveIdPtr;
      //
      // Loop through the source Interleave DIMM set
      //
      for (InterleaveId = 0; InterleaveId < NumOfDimmsInInterleaveSet; InterleaveId++) {
        //
        // Find the socket number and MC number for this DIMM
        //
        SocketId = TempInterleaveIdPtr->Socket;
        McId = TempInterleaveIdPtr->Channel / MaxChPerMc;

        if (Index == SocketId && McIndex == McId) {
          McParticipatedFlag = 1;
          //
          // Update the channel interleave bit map for this MC
          //
          ChannelBitMap[Counter] |= (BIT0 << (TempInterleaveIdPtr->Channel - (McId * MaxChPerMc)));
        }
        TempInterleaveIdPtr++;
      } // InterleaveId
      if (McParticipatedFlag) {
        Counter++;
      }
    } // McIndex
  } // Index

  if (Counter > 1) {
    //
    // Verify if channel interleave bitmap matches between all MCs in this interleave set, if not return error.
    //
    for (Index = 1; Index < Counter; Index++) {
      if (ChannelBitMap[Index] != ChannelBitMap[Index-1]) {
        return FAILURE;
      }
    }
  } else if (Counter == 0) {
    //
    // s5331732: If a match can't be found, return fail to prevent default zero data from being treated as real
    //
    return FAILURE;
  }
  *ChWays = CalculateWaysfromBitmap (ChannelBitMap[0]);

  *ChInterleaveBitMap = ChannelBitMap[0];

  return SUCCESS;
}

/**

  This routine compare the NM population of all the MC involved in the SAD rule and returns error if they dont match.

  @param[in]      scktInterleave  - sckt interleave struct that holds list MC  involved in the request.
  @param[in, out] NMBitmap        - Pointer to variable to store the bitmap


  @retval SUCCESS - if NM population matches
  @retval Failure - if NM population doesnt match


**/
UINT32
CheckNmPopulation (
  IN     UINT32   scktInterleave[MC_MAX_NODE],
  IN OUT UINT8    *NMBitmap
  )
{
  UINT8 socketNum;
  UINT8 mc;
  UINT8 Bitmap;
  UINT8 MaxImc;

  Bitmap = 0xff;
  MaxImc = GetMaxImc ();

  //Parse through each socket to create a SAD
  for (socketNum = 0; socketNum<MAX_SOCKET; socketNum++) {
    if (CheckSocketEnabled (socketNum) == 0) {
      continue;
    }

    for (mc = 0; mc < MaxImc; mc++) {

      //Skip the sockets that is not participating in the interleave
      if (scktInterleave[ (socketNum * MaxImc) + mc] == 0) {
        continue;
      }

      if ((Bitmap != 0xFF) && (Bitmap != FindNmBitMap (socketNum, mc))) {
        return FAILURE;
      } else{
        Bitmap = FindNmBitMap (socketNum, mc);
      }

    }//mc
  }//socketnum

  *NMBitmap = Bitmap;
  return SUCCESS;
}


/**

  This routine creates a DRAM rule for a specific interleave request and updates the SADTable struct in corresponding sockets.

  @param[in, out] Host            - Pointer to sysHost
  @param[in, out] MemMapData      - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]      Sckt            - socket number
  @param[in]      Ch              - Channel number
  @param[in]      Dimm            - DIMM number
  @param[in]      InterleavePtr   - Pointer to interleave table
  @param[in]      InterleaveIdPtr - pointer to the first DIMM info struct for this interleave set
  @param[in, out] NodeLimit       - Pointer to return the limit of SAD created.

  @retval SADLimit

**/
UINT32
ApplyInterleave (
  IN OUT PSYSHOST                                  Host,
  IN OUT MEMORY_MAP_DATA                           *MemMapData,
  IN     UINT8                                     Sckt,
  IN     UINT8                                     Ch,
  IN     UINT8                                     Dimm,
  IN     NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *InterleavePtr,
  IN     NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveIdPtr,
  IN OUT UINT32                                    *NodeLimit
  )
{
  UINT16      MemType;
  UINT8       SktWays;
  UINT8       ChInterleaveBitMap;
  UINT8       NMBitmap;
  UINT32      ScktInterleave[MC_MAX_NODE];
  UINT16      InterSizePerCh;
  UINT32      Status;
  UINT8       ChWays;
  SYS_SETUP   *Setup;

  ChInterleaveBitMap = 0;
  NMBitmap = 0;
  ZeroMem (&ScktInterleave, sizeof (ScktInterleave));
  InterSizePerCh = 0;
  SktWays = 0;
  ChWays = 0;
  Setup = GetSysSetupPointer ();

  MemType = MemType1lmAppDirect;
  if (IS_DDR_2LM_OR_MIXED (Host) &&
      (MemMapData->PmemCaching == LegacyPmCaching)) {
    MemType = MemType2lmDdrWbCacheAppDirect;
  }

  //
  // Get the MCs that are involved in this interleave
  //
  Status = GetScktInterleaveInfo (InterleavePtr, InterleaveIdPtr, ScktInterleave, &SktWays);

  //
  // Get the channel interleave for this interleave. (channel interleave should be the same in each MC involved. This is
  // a restriction in SKX. NVMDIMM mgmt driver is responsible for placing requests in which the channel interleave info
  // matches between the MCs)
  //
  Status = GetChInterleaveInfo (InterleavePtr, InterleaveIdPtr, &ChInterleaveBitMap, &ChWays);
  if (Status == FAILURE) {
    *NodeLimit = 0;
    RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Channel interleave doesn't match between IMCs specified in the interleave request.\n"
      );
    return CHANNEL_INTERLEAVE_MISMATCH;
  }

  //
  // In a 1LM, check number of x1 interleave is only 1 before allowing AD x1 mapping.
  //
  if ((MemType == MemType1lmAppDirect) && ((SktWays * ChWays) == ONE_WAY)) {
    if (Check1lmX1IntlvForAdX1 (MemMapData, Sckt)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "System does not meet the volatile memory POR symmetry requirement to allow mapping ADx1.\n"
        );
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_PMEM_X1_POPULATION_INVALID, Sckt, Ch, Dimm, NO_RANK);
      return INSUFFICIENT_SILICON_RESOURCES;
    }
  }

  //If the MemType is pmem$, make sure the NM population matches between IMCs.
  if (MemType == MemType2lmDdrWbCacheAppDirect) {
    Status = CheckNmPopulation (ScktInterleave, &NMBitmap);
    if (Status == FAILURE) {
      *NodeLimit = 0;
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "NM population doesn't match between IMCs specified in the interleave request.\n"
        );
      return NM_POPULATION_MISMATCH;
    }
  }

  //Determine the interleave size per channel for this interleave set
  InterSizePerCh = InterleaveIdPtr->PartitionSize;

  Status = AddSadRule (
             Host,
             MemMapData,
             ScktInterleave,
             SktWays,
             ChInterleaveBitMap,
             NMBitmap,
             InterSizePerCh,
             MemType,
             FALSE,
             InterleavePtr->InterleaveSize,
             NodeLimit
             );

  if (Status != SUCCESS) {
    return Status;
  }
  return OPERATION_SUCCESS;
}


/**

  Description: This routine sets the "processedFlag" & SadLimit variable in the interleave request
  struct in Dimm structs if the requests are processed and SADs were created and updates the
  DIMM interleave status in PCD records.

  @param Host                - Pointer to sysHost
  @param Socket              - Socket Id
  @param Ch                  - channel id
  @param Dimm                - Dimm num
  @param *SourceInterleave   - Pointer to the interleave struct
  @param *SourceInterleaveId - Pointer to the Dimm id struct.
  @param NodeLimit           - Limit of SAD Rule related to the request
  @param RecordType          - specifies if this is CFGIN or CFG_CUR
  @param responseStatus      - Response Status

  @retval SUCCESS - if sucessfull
  @retval Failure - if not sucessfull


**/
UINT32
UpdateProcessedFlagAndIntlvStatus (
  PSYSHOST                                  Host,
  UINT8                                     Socket,
  UINT8                                     Ch,
  UINT8                                     Dimm,
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *SourceInterleave,
  NGN_DIMM_INTERLEAVE_ID_HOST               *SourceInterleaveId,
  UINT32                                    NodeLimit,
  UINT8                                     RecordType,
  UINT32                                    responseStatus
  )
{
  UINT8                                     NumOfDimmsInInterleaveSet = 0;
  UINT8                                     i;
  UINT8                                     j;
  UINT8                                     SocketNum;
  UINT8                                     ChannelNum;
  UINT8                                     DimmNum;
  UINT32                                    Status = SUCCESS;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *DestInterleave = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *DestInterleaveId = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *TempInterleaveId = SourceInterleaveId;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;

  SocketNum = 0;
  ChannelNum = 0;
  DimmNum = 0;

  NumOfDimmsInInterleaveSet = SourceInterleave->NumOfDimmsInInterleaveSet;
  for (i = 0; i<NumOfDimmsInInterleaveSet; i++) {
    //Init temp variables based on Dimm id struct
    SocketNum = TempInterleaveId->Socket;
    ChannelNum = TempInterleaveId->Channel;
    DimmNum = TempInterleaveId->Dimm;
    DdrtDimmInfo = GetDdrtDimmInfo (SocketNum, ChannelNum, DimmNum);

    //Scan through the interleave requests of the participating Dimm to find a matching request.
    for (j = 0; j<MAX_UNIQUE_NGN_DIMM_INTERLEAVE; j++) {
      if (RecordType == CFG_IN) {
        DestInterleave = &(DdrtDimmInfo->NgnCfgReq.interleave[j]);
        DestInterleaveId = &(DdrtDimmInfo->NgnCfgReq.interleaveId[j][0]);
      } else if (RecordType == CFG_CUR) {
        DestInterleave = &(DdrtDimmInfo->NgnCfgCur.interleave[j]);
        DestInterleaveId = &(DdrtDimmInfo->NgnCfgCur.interleaveId[j][0]);
      } else {
        return FAILURE;
      }

      //skip if basic Interleave information doesnt match
      if ((DestInterleave->NumOfDimmsInInterleaveSet != SourceInterleave->NumOfDimmsInInterleaveSet) && (DestInterleave->InterleaveMemoryType != SourceInterleave->InterleaveMemoryType)) {
        continue;
      }

      //Compare the list of dimms participating in the interleave and see if they match
      Status = CompareDimmInterleaveId (Host, SourceInterleaveId, DestInterleaveId, NumOfDimmsInInterleaveSet);
      if (Status == SUCCESS) {
        break;
      }
    }

    if (Status == SUCCESS)  {
      //If we are here, we have found a matching interleave request.
      DestInterleave->Processed = 1;
      DestInterleave->SadLimit = NodeLimit;

      // Make sure we don't go out of bounds for array "interleave"
      if (j >= MAX_UNIQUE_NGN_DIMM_INTERLEAVE) {
        return FAILURE;
      }
      //
      // Update the DIMM interleave change status into COUT or CCUR DIMM Interleave information.
      //
      if (RecordType == CFG_IN) {
        DdrtDimmInfo->NgnCfgOut.interleave[j].InterleaveChangeResponseStatus = EncodeInterleaveResponse ((UINT8) responseStatus);
      } else if (RecordType == CFG_CUR) {
        DdrtDimmInfo->NgnCfgCur.interleave[j].InterleaveChangeResponseStatus = EncodeInterleaveResponse ((UINT8) responseStatus);
      }
    } else  {
      return FAILURE;
    }
    TempInterleaveId++;
  }

  return SUCCESS;
}

/**
  This routine goes through each NVMDIMM on the socket and if there are no CFGIn requests, it will apply the
  current config based on CCUR data retreived from the DIMM.

  @param[in] Host      - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] SocketNum - Socket number

  @retval Status

**/
UINT32
ApplyCcurRecords (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  )
{
  UINT8                                     ChannelNum;
  UINT8                                     DimmNum;
  UINT8                                     InterleaveNum;
  UINT32                                    NodeLimit;
  UINT32                                    Status;
  MEM_NVRAM_STRUCT                          *NvramMem;
  MEM_VAR_STRUCT                            *Mem;
  CHANNEL_NVRAM_STRUCT                      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT                         (*DimmNvList)[MAX_DIMM];
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *Interleave = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveId = NULL;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;
  Mem = &Host->var.mem;
  ChannelNvList = &NvramMem->socket[SocketNum].channelList;

  // For each channel
  for (ChannelNum = 0; ChannelNum < MaxChDdr; ChannelNum++) {
    if ((*ChannelNvList)[ChannelNum].enabled == 0) {
      continue;
    }

    DimmNvList = &NvramMem->socket[SocketNum].channelList[ChannelNum].dimmList;

    // For each DIMM
    for (DimmNum = 0; DimmNum < MAX_DIMM; DimmNum++) {
      if ((*DimmNvList)[DimmNum].dimmPresent == 0) {
        continue;
      }

      // Skip if this is not a NVMDIMM DIMM
      if ((*DimmNvList)[DimmNum].DcpmmPresent == 0) {
        continue;
      }

      // Skip if this DIMM unmapped
      if (IsNvDimmUnmapped (SocketNum, ChannelNum, DimmNum, NULL)) {
        continue;
      }

      DdrtDimmInfo = GetDdrtDimmInfo (SocketNum, ChannelNum, DimmNum);

      // Skip if this DIMM is new to the system
      if (DdrtDimmInfo->NewDimm == 1) {
        continue;
      }

      // If we are not in degraded memory mapping mode, we will honor new requests
      if (!MemMapData->MemMapDegradeLevel) {
        // Skip if a CfgIn request is present and valid for this DIMM
        if (DdrtDimmInfo->NgnCfgReqPresent && DdrtDimmInfo->NgnCfgReqValid) {
          continue;
        }
      }

      // Skip if there is no CCUR present or valid for this DIMM
      if (!DdrtDimmInfo->NgnCfgCurPresent || !DdrtDimmInfo->NgnCfgCurValid) {
        continue;
      }

      for (InterleaveNum = 0; InterleaveNum < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveNum++) {
        // Skip if there is no interleave record or if the record failed our validation previously
        if (!DdrtDimmInfo->NgnCfgCur.interleave[InterleaveNum].RecPresent ||
            !DdrtDimmInfo->NgnCfgCur.interleave[InterleaveNum].Valid) {
          continue;
        }

        // Skip if this interleave is already processed.
        if (DdrtDimmInfo->NgnCfgCur.interleave[InterleaveNum].Processed == 1) {
          continue;
        }

        Interleave = &DdrtDimmInfo->NgnCfgCur.interleave[InterleaveNum];
        InterleaveId = &DdrtDimmInfo->NgnCfgCur.interleaveId[InterleaveNum][0];
        NodeLimit = 0;

        // Call the helper function to create the SAD rule for this interleave
        Status = ApplyInterleave (Host, MemMapData, SocketNum, ChannelNum, DimmNum, Interleave, InterleaveId, &NodeLimit);

        // Update the interleave structure of all the participating DIMMs to denote the request has been implemented and update the response status.
        UpdateProcessedFlagAndIntlvStatus (Host, SocketNum, ChannelNum, DimmNum, Interleave, InterleaveId, NodeLimit, CFG_CUR, Status);
      } // InterleaveNum
    } // DimmNum
  } // ChannelNum

  return SUCCESS;
}


/**
  This routine goes through each NVMDIMM in the socket and if there are CFGIn requests, it will apply the
  interleave requests retrieved from the DIMM

  @param[in] Host      - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] SocketNum - Socket number

  @retval Status

**/
UINT32
ApplyInterleaveRequests (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  )
{
  UINT8                                     ChannelNum;
  UINT8                                     DimmNum;
  UINT8                                     InterleaveNum;
  UINT32                                    NodeLimit;
  UINT32                                    Status;
  MEM_NVRAM_STRUCT                          *NvramMem;
  CHANNEL_NVRAM_STRUCT                      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT                         (*DimmNvList)[MAX_DIMM];
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *Interleave = NULL;
  NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveId = NULL;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;
  ChannelNvList = &NvramMem->socket[SocketNum].channelList;

  // For each channel
  for (ChannelNum = 0; ChannelNum < MaxChDdr; ChannelNum++) {
    if ((*ChannelNvList)[ChannelNum].enabled == 0) {
      continue;
    }

    DimmNvList = &NvramMem->socket[SocketNum].channelList[ChannelNum].dimmList;

    // For each DIMM
    for (DimmNum = 0; DimmNum < MAX_DIMM; DimmNum++) {
      if ((*DimmNvList)[DimmNum].dimmPresent == 0) {
        continue;
      }

      // Skip if this is not a NVMDIMM DIMM
      if ((*DimmNvList)[DimmNum].DcpmmPresent == 0) {
        continue;
      }

      // Skip if this DIMM unmapped
      if (IsNvDimmUnmapped (SocketNum, ChannelNum, DimmNum, NULL)) {
        continue;
      }

      DdrtDimmInfo = GetDdrtDimmInfo (SocketNum, ChannelNum, DimmNum);

      // Skip if this DIMM is new to the system
      if (DdrtDimmInfo->NewDimm == 1) {
        continue;
      }

      //Check if there are CfgIn request is present and valid for this DIMM. If not present, skip this DIMM.
      if (!DdrtDimmInfo->NgnCfgReqPresent || !DdrtDimmInfo->NgnCfgReqValid) {
        continue;
      }

      for (InterleaveNum = 0; InterleaveNum < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveNum++) {
        //Skip if there is no interleave record or if the record failed our validation previously
        if (!DdrtDimmInfo->NgnCfgReq.interleave[InterleaveNum].RecPresent ||
            !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveNum].Valid) {
          continue;
        }

        // Skip if this interleave is already processed
        if (DdrtDimmInfo->NgnCfgReq.interleave[InterleaveNum].Processed == 1) {
          continue;
        }

        Interleave = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveNum];
        InterleaveId = &DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveNum][0];
        NodeLimit = 0;

        // Call the helper function to create the SAD rule for this interleave
        Status = ApplyInterleave (Host, MemMapData, SocketNum, ChannelNum, DimmNum, Interleave, InterleaveId, &NodeLimit);

        //Update the interleave structure of all the participating dimms to denote the request has been implemented and update the response status.
        UpdateProcessedFlagAndIntlvStatus (Host, SocketNum, ChannelNum, DimmNum, Interleave, InterleaveId, NodeLimit, CFG_IN, Status);
      } // InterleaveNum
    } // DimmNum
  } // ChannelNum

  return SUCCESS;
}
#endif // DDRT_SUPPORT
