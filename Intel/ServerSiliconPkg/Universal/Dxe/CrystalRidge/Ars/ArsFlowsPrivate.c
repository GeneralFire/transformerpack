/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Protocol/Dcpmm.h>

#include "CrystalRidge.h"
#include "Dsm.h"
#include "Fis.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsFlowsPrivate.h"
#include "XlateFunctions/XlateFunctions.h"
#include "Nfit.h"

//
// Internal module functions
//

/**

  Routine description: starts or stops the ARS for given dimms.
  Dimms' state in DimmsList is updated. Wrapper around SetArsForDimms.

  @param[in,out]  DimmsList   - list of the dimms.
  @param[in]      DimmListLen - number of dimms in the list.
  @param[in]      DoStart     - TRUE to start, FALSE to stop.

  @retval EFI_SUCCESS ARS successfully started.
  @retval other       ARS start failed. See status codes for NvmCtlrSetAddressRangeScrub.

**/
EFI_STATUS
SetArsForDimms (
  IN OUT ARS_DIMM   *DimmList,
  IN UINT8          DimmListLen,
  IN BOOLEAN        DoStart
  )
{
  UINT8             i;
  EFI_STATUS        Status = EFI_SUCCESS;
  ARS_DIMM          *ArsDimm;

  if (DimmList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (i = 0; i < DimmListLen; i++) {

    ArsDimm = &DimmList[i];

    if ((DoStart && ArsDimm->State == IN_PROGRESS) ||
        (!DoStart && ArsDimm->State != IN_PROGRESS)) {
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("ARS") "S%d.C%d.D%d: SetArsForDimms: unexpected state: ",
                ArsDimm->Socket, ArsDimm->Channel, ArsDimm->Dimm));
      CRDEBUG ((DEBUG_INFO, "DoStart: %d, State: %d\n",
                DoStart,
                ArsDimm->State));
      continue;
    }
    Status = NvmCtlrSetAddressRangeScrub (
      ArsDimm->Socket,
      ArsDimm->Channel,
      ArsDimm->Dimm,
      ArsDimm->StartDpa,
      ArsDimm->EndDpa,
      DoStart
      );

    if (DoStart) {
      //State update on Stop belongs to UpdateStatusOfDimm
      ArsDimm->State = EFI_ERROR (Status) ? ERROR : IN_PROGRESS;
    }
    if (EFI_ERROR (Status)) {

      CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: SetArsForDimms: NvmCtlrSetAddressRangeScrub (..., 0x%llx, ",
        ArsDimm->Socket,
        ArsDimm->Channel,
        ArsDimm->Dimm,
        ArsDimm->StartDpa));
      CRDEBUG ((DEBUG_ERROR, "0x%llx, \"%a\", USE_BIOS_MAILBOX) failed: %r\n",
        ArsDimm->EndDpa,
        DoStart ? "start" : "stop",
        Status));
      break;
    }
  }

  return Status;
}

/**

  Routine description: starts the ARS for given dimms. Dimms' state in DimmsList
  is updated. Wrapper around SetArsForDimms.

  @param[in,out]  DimmsList - list of the dimms.
  @param[in]      DimmsCount - number of dimms in the list.

  @retval EFI_SUCCESS ARS successfully started.
  @retval other       ARS start failed. See status codes for NvmCtlrSetAddressRangeScrub.

**/
EFI_STATUS
StartArsForDimms (
  IN OUT ARS_DIMM   *DimmsList,
  IN UINT8          DimmsCount
  )
{
  return SetArsForDimms (DimmsList, DimmsCount, START_ARS);
}


/**

  Routine description: aborts the ARS for given dimms. Dimms' state in DimmsList
  is updated. Wrapper around SetArsForDimms.

  @param[in,out]  DimmsList - list of the dimms.
  @param[in]      DimmsCount - number of dimms in the list.

  @retval EFI_SUCCESS ARS successfully aborted.
  @retval other       ARS start failed. See status codes for NvmCtlrSetAddressRangeScrub.

**/
EFI_STATUS
AbortArsForDimms (
  IN OUT ARS_DIMM   *DimmsList,
  IN UINT8          DimmsCount
  )
{
  return SetArsForDimms (DimmsList, DimmsCount, STOP_ARS);
}


//
// Functions exposed in ArsFlowsPrivate include file
//
/**
  @brief Get long operation status data for ARS operation from NVDIMM.

  @param[in]  Socket    - socket id (0 based)
  @param[in]  Channel   - channel id in socket (0 based)
  @param[in]  Dimm      - dimm id in channel (0 based)
  @param[out] ArsStatus - status for long operation

  @return Status returned by firmware, EFI_INVALID_PARAMETER when dimm not found
          or ArsStatus is NULL.
**/
EFI_STATUS
GetLongOperationStatusDataForArs (
  UINT8          Socket,
  UINT8          Channel,
  UINT8          Dimm,
  ARS_OP_STATUS *ArsStatus
  )
{
  EFI_STATUS                        Status;
  UINT8                             i;
  UINT16                            PercentComplete;
  FIS_GET_LONG_OPERATION_STATUS_RSP FisRsp;

  if (ArsStatus == NULL) {

    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = FisRequest (Socket, Channel, Dimm, NULL, 0, &FisRsp.DWord[0], GET_LONG_OPERATION, FIS_DEFTIMEOUT, NULL);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: Reading long operation status failed! (%r)\n",
            Socket, Channel, Dimm, Status));
    return Status;
  }
  //
  // Ensure it is ARS Command that the Long Op Status that it has returned.
  //
  if (FisRsp.Bits.Command != SET_ADDRESS_RANGE_SCRUB) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: Long Operation Status reports cmd %04X, not SET_ADDRESS_RANGE_SCRUB (%04X)!\n",
            Socket, Channel, Dimm, FisRsp.Bits.Command, SET_ADDRESS_RANGE_SCRUB));
    return EFI_NOT_FOUND;
  }

  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: GetLongOperationStatusDataForArs: ",
    Socket,
    Channel,
    Dimm
    ));
  CRDEBUG ((DEBUG_INFO, "(0x%04x): RAW: Percent: %x, ETC: %x, StatusCode: 0x%02x, ArsState: %d, SAWO: %d\n",
    ConvertSocketChannelDimmToNfitHandle (Socket, Channel, Dimm),
    FisRsp.Bits.PercentComplete,
    FisRsp.Bits.Etc,
    FisRsp.Bits.StatusCode,
    FisRsp.Bits.CmdData.Ars.ArsState,
    DsmStopArsWithOverflow ()
    ));

  if (DsmStopArsWithOverflow () && FisRsp.Bits.CmdData.Ars.ArsState == FIS_LONGOP_ARSSTATE_USER_ABORT_OR_POWER_CYCLE) {
    FisRsp.Bits.CmdData.Ars.ArsState = FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED;
  }

  ZeroMem (ArsStatus, sizeof (*ArsStatus));

  // HiWord of DataPtr contains ARS % Complete in BCD format = XXX
  PercentComplete = FisRsp.Bits.PercentComplete;
  ArsStatus->PercentComplete = ((PercentComplete >> 8) & 0xF) * 100 +
    ((PercentComplete >> 4) & 0xF) * 10 +
    (PercentComplete & 0xF);

  ArsStatus->StatusCode       = FisRsp.Bits.StatusCode;
  ArsStatus->ErrorsFound      = MIN (FisRsp.Bits.CmdData.Ars.ErrorsFound, NELEMENTS(ArsStatus->DpaErrorAddress));
  ArsStatus->InProgress       = (ArsStatus->StatusCode == FIS_STS_DEVICE_BUSY);
  ArsStatus->PrematurelyEnded = (FisRsp.Bits.CmdData.Ars.ArsState == FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED ||
                                 FisRsp.Bits.CmdData.Ars.ArsState == FIS_LONGOP_ARSSTATE_USER_ABORT_OR_POWER_CYCLE);
  ArsStatus->Overflow         = FisRsp.Bits.CmdData.Ars.ArsState == FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED;

  for (i = 0; i < ArsStatus->ErrorsFound; i++) {

    ArsStatus->DpaErrorAddress[i] = FisRsp.Bits.CmdData.Ars.DpaErrorAddress[i];
  }
  return Status;
}


/**
  @brief Find scrubbable interleave-sets for given SPA range.

  Supports PMEM regions only. Narrows down *StartSpaPtr and *EndSpaPtr to align with
  the first and the last interleave set found. Never extends the range.

  @param[in,out] StartSpaPtr            - Starting SPA address of the range for search. On exit can be
                                          changed to reflect the start SPA of the first interleave-set
                                          if it pointed before the interleave-set base address.
  @param[in,out] EndSpaPtr              - Ending SPA address of the range for search. On exit can be
                                          changed to reflect the limit of the last interleave-set found
                                          if it pointed behind the interleave-set limit.
  @param[out]    InterleaveSetsTable    - Optional buffer for the found interleave sets.
  @param[in]     InterleaveSetsTableLen - Number of interleave sets to find, also
                                          length of buffer if provided.

  @returns Number of interleave sets found is returned. Zero is returned also for error.
**/
UINT8
GetScrubbableInterleaveSets (
  IN OUT UINT64            *StartSpaPtr,
  IN OUT UINT64            *EndSpaPtr,
  OPTIONAL OUT SPA_INT_SET *InterleaveSetsTable,
  IN     UINT8              InterleaveSetsTableLen
  )
{
  EFI_STATUS    Status;
  UINT64        SpaAddress;
  UINT64        SadBase;
  UINT64        SadLimit;
  UINT64        FoundStartSpa;
  UINT64        FoundEndSpa;
  MEM_TYPE      MemType;
  UINT8         Skt;
  UINT8         Count;
  UINT8         DimmCount;

  if (StartSpaPtr == NULL || EndSpaPtr == NULL || (InterleaveSetsTable != NULL && InterleaveSetsTableLen == 0)) {

    ASSERT (FALSE);
    return 0;
  }
  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetScrubbableInterleaveSets: Getting interleave sets from: 0x%012llX - 0x%012llX\n",
          *StartSpaPtr, *EndSpaPtr));

  FoundStartSpa = *EndSpaPtr;
  FoundEndSpa = *StartSpaPtr;

  for (SpaAddress = *StartSpaPtr, Count = 0;
       SpaAddress < *EndSpaPtr && Count < InterleaveSetsTableLen; SpaAddress = SadLimit) {

    Status = CrGetSadInfo (SpaAddress, &Skt, &SadBase, &SadLimit, &MemType);
    if (EFI_ERROR (Status)) {
      //
      // No more memory, break the loop.
      //
      break;
    }
    CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetScrubbableInterleaveSets: Skt %d, SadBase 0x%012llX, SadLimit 0x%012llX, Type %d",
            Skt, SadBase, SadLimit, MemType));
    if (!IsPmemRgn (&mCrInfo.NvdimmInfo, SadBase)) {

      CRDEBUG ((DEBUG_INFO, " - non PMEM\n"));
      continue;
    }
    CRDEBUG ((DEBUG_INFO, " - PMEM\n"));

    if (SadLimit > *EndSpaPtr) {
      //
      // SAD limit is behind EndSpa, but we are interested up to EndSpa only.
      //
      SadLimit = *EndSpaPtr;
    }
    if (FoundStartSpa > SpaAddress) {

      FoundStartSpa = SpaAddress;
    }
    if (FoundEndSpa < SadLimit) {

      FoundEndSpa = SadLimit;
    }

    DimmCount = GetNumDimmsForPmemRgn(&mCrInfo.NvdimmInfo, SadBase);

    //
    // If the buffer for interleave sets was passed as a parameter we add the interleave set to it.
    // If not, we only increment the counter.
    //
    if (InterleaveSetsTable != NULL) {

      InterleaveSetsTable[Count].StartSpa = SpaAddress;
      InterleaveSetsTable[Count].EndSpa = SadLimit;
      InterleaveSetsTable[Count].SpaBase = SadBase;
      InterleaveSetsTable[Count].Enabled = TRUE;
      InterleaveSetsTable[Count].Running = FALSE;
      InterleaveSetsTable[Count].EndedPrematurely = FALSE;
      InterleaveSetsTable[Count].DimmCount = DimmCount;
      CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "-- added int set: 0x%012llX - 0x%012llX, %d DIMM(s)\n",
              InterleaveSetsTable[Count].StartSpa, InterleaveSetsTable[Count].EndSpa, InterleaveSetsTable[Count].DimmCount));
    }
    Count++;
  } // for (SpaAddress...)
  if (Count > 0) {
    *StartSpaPtr = FoundStartSpa;
    *EndSpaPtr = FoundEndSpa;
  }
  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetScrubbableInterleaveSets: %d PMEM interleave sets found\n", Count));

  return Count;
} // GetScrubbableInterleaveSets()


/**

  Routine description: returns next enabled and not running interleave set to scrub.

  @param[in]  InterleaveSetsList    - List of interleave sets
  @param[in]  InterleaveSetsListLen - Length of InterleaveSetsList passed in
  @param[out] IntSetIndex           - Index of interleave set found

  @returns  Next interleve set found or NULL, if not found.

**/
SPA_INT_SET *
GetNextInterleaveSet (
  IN SPA_INT_SET      *InterleaveSetsList,
  IN UINT8            InterleaveSetsListLen,
  OUT UINT8           *IntSetIndex
  )
{
  UINT8 i;

  for (i = 0; i < InterleaveSetsListLen; i++) {
    if (InterleaveSetsList[i].Enabled && !InterleaveSetsList[i].Running) {
      *IntSetIndex = i;
      return &InterleaveSetsList[i];
    }
  }

  return NULL;
}

/**

  Routine description: finds all NvDimms in given interleave set.

  @param[in]  IntSet      - interleave set for which to find all dimms
  @param[out] DimmList    - List of dimms found
  @param[in]  DimmListLen - Length of DimmList array passed in

  @returns Number of dimms found, 0 also for error in parameters.

**/
UINT8
GetDimmsInInterleaveSet (
  IN CONST SPA_INT_SET *IntSet,
  OUT ARS_DIMM         *DimmList,
  IN UINT8             DimmListLen
  )
{
  EFI_STATUS  Status;
  UINT64      BaseAddr;
  UINT8       Skt;
  UINT8       Ch;
  UINT8       Dimm;
  UINT8       TotalDimms;
  UINT32      ChGranularity;
  UINT64      CurrentSpa;
  UINT64      StartSpaGranularityStepOffset;
  UINT64      DpaLength;
  UINT64      Dpa;
  UINT64      DpaBase;
  UINT8       IntWays;
  UINT8       i;

  if (DimmList == NULL || DimmListLen == 0 ||
      (IntSet->StartSpa >= IntSet->EndSpa)) {
    return 0;
  }

  Status = GetDimmInfoFromSpa (IntSet->StartSpa, &BaseAddr, &Skt, &Ch, &Dimm, NULL);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: no dimm found for SPA = 0x%012llX\n", IntSet->StartSpa));
    return 0;
  }

  DpaBase = GetPmemDpaBase (BaseAddr, Skt, Ch, Dimm);
  if (DpaBase == 0) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: can't get DPA base address for for SADBaseAddr = 0x%012llX\n",
      BaseAddr
      ));
    return 0;
  }

  IntWays = GetInterleaveWaysForPmemRgn (BaseAddr);
  if (IntWays == 0) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: can't get interleave ways SADBaseAddr = 0x%012llX\n",
      BaseAddr
      ));
    return 0;
  }

  DpaLength = (IntSet->EndSpa - IntSet->StartSpa) / IntWays;
  ChGranularity = GetChGranularityFromSadBaseAddr (BaseAddr);
  if (ChGranularity == 0) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: bad channel granularity for SADBaseAddr = 0x%012llX\n",
      BaseAddr
      ));
    return 0;
  }
  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetDimmsInInterleaveSet: Found %d interleave ways, DPA len = 0x%llX, channel granularity %d\n",
          IntWays, DpaLength, ChGranularity));

  CurrentSpa = IntSet->StartSpa;
  AlignAddressForArs (&CurrentSpa, FALSE);
  StartSpaGranularityStepOffset = CurrentSpa % ChGranularity;

  TotalDimms = GetNumDimmsForPmemRgn (&mCrInfo.NvdimmInfo, BaseAddr);
  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetDimmsInInterleaveSet: Found %d DIMMs for SPA = 0x%012llX\n", TotalDimms, IntSet->StartSpa));

  for (i = 0; (i < TotalDimms) && (i < DimmListLen); i++) {

    Status = GetDimmInfoFromSpa (CurrentSpa, &BaseAddr, &Skt, &Ch, &Dimm, NULL);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: Error (%r) during search for dimm! CurrentSpa = 0x%012llX\n", Status, CurrentSpa));
      return 0;
    }
    Status = SpaToNvmDpa (CurrentSpa, &Skt, &Ch, &Dimm, &Dpa);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "GetDimmsInInterleaveSet: Error during SPA/DPA translation (%r)!\n", Status));
      return 0;
    }
    DimmList[i].SpaBase     = BaseAddr;
    DimmList[i].DpaBase     = DpaBase;
    DimmList[i].CurrentSpa  = CurrentSpa;
    DimmList[i].StartDpa    = Dpa;
    DimmList[i].EndDpa      = DimmList[i].StartDpa + DpaLength;
    DimmList[i].Socket      = Skt;
    DimmList[i].Channel     = Ch;
    DimmList[i].Dimm        = Dimm;
    DimmList[i].State       = NOT_STARTED;
    DimmList[i].ErrorsFound = 0;

    CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "GetDimmsInInterleaveSet: SPA = 0x%012llX (Base = 0x%012llX)",
            DimmList[i].CurrentSpa, DimmList[i].SpaBase));
    CRDEBUG ((DEBUG_INFO, " - found DPA: S%d.C%d.D%d 0x%llX - 0x%llX\n",
            DimmList[i].Socket, DimmList[i].Channel, DimmList[i].Dimm, DimmList[i].StartDpa, DimmList[i].EndDpa));

    AlignAddressForArs (&DimmList[i].StartDpa, FALSE);
    AlignAddressForArs (&DimmList[i].EndDpa,   TRUE);

    //
    // For next dimms in int set, we should align to the granularity steps
    //
    CurrentSpa -= StartSpaGranularityStepOffset;
    CurrentSpa += ChGranularity;

    //
    // this one is really needed only for the first dimm in interleave set,
    // so this needs to be zeroed out for later dimms
    //
    StartSpaGranularityStepOffset = 0;
  }

  return i;
}


/**

  Routine description: starts ARS for interleave set and updates its state
  Finds and updates the state for each of the NVDIMM found in interleave set.

  @param[in,out] IntSet      - Interleave set for which to start ARS
  @param[out]    DimmList    - List of the dimms found in interleave set
  @param[in]     DimmListLen - Length of DimmList array passed in
  @param[out]    DimmsCount  - Number of dimms found in interleave set

  @retval EFI_INVALID_PARAMETER One of parameters were invalid.
  @retval EFI_SUCCESS           ARS started successfully
  @retval other                 Other HW error

**/
EFI_STATUS
StartArsForInterleaveSet (
  IN OUT SPA_INT_SET  *IntSet,
  OUT ARS_DIMM        *DimmList,
  IN UINT8            DimmListLen,
  OUT UINT8           *DimmsCount
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  AbortStatus;

  if (IntSet == NULL || DimmList == NULL || DimmListLen == 0 || DimmsCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *DimmsCount = GetDimmsInInterleaveSet (
    IntSet,
    DimmList,
    DimmListLen
    );

  if (*DimmsCount == 0) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "StartArsForInterleaveSet - couldn't find the DPA regions in given SPA regions!\n"));
    return EFI_NOT_STARTED;
  }

  Status = StartArsForDimms (DimmList, *DimmsCount);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "StartArsForInterleaveSet - can't start ARS, aborting...\n"));
    AbortStatus = AbortArsForDimms (DimmList, *DimmsCount);
    if (AbortStatus == EFI_SUCCESS) {
      Status = EFI_ABORTED;
    } else {
      Status = EFI_DEVICE_ERROR;
    }
    IntSet->Running          = FALSE;
    IntSet->Enabled          = FALSE;
    IntSet->EndedPrematurely = TRUE;
  } else {
    IntSet->Running = TRUE;
    IntSet->EndedPrematurely = FALSE;
  }

  return Status;
}

/**

  Routine description: update the state for the dimm. Uses Get Long Operation
  Status.

  @param[in,out] ArsDimm - dimm to update

  @retval EFI_INVALID_PARAMETER Dimm not found or NULL given.
  @retval EFI_SUCCESS           Status updated.
  @retval other                 Other HW failure.

**/
EFI_STATUS
UpdateStatusOfDimm (
  IN OUT ARS_DIMM    *ArsDimm
  )
{
  EFI_STATUS      Status;
  NVDIMM          *NvmDimm;
  ARS_OP_STATUS   ArsStatus;
  UINT64          StartDpa;
  UINT64          CurrentDpa;
  UINT64          EndDpa;
  UINT64          TopErrorDpa = 0;
  UINT8           ArsInProgress;
  UINT8           i;

  if (ArsDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, ArsDimm->Socket, ArsDimm->Channel, ArsDimm->Dimm, NVDIMM_FLAG_ENABLED);
  if (NvmDimm == NULL) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: UpdateStatusOfDimm - can't find Dimm\n",
      ArsDimm->Socket,
      ArsDimm->Channel,
      ArsDimm->Dimm
      ));
    return EFI_INVALID_PARAMETER;
  }

  Status = NvmCtlrGetAddressRangeScrub (
    ArsDimm->Socket,
    ArsDimm->Channel,
    ArsDimm->Dimm,
    &StartDpa,
    &EndDpa,
    &CurrentDpa,
    &ArsInProgress
    );

  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: UpdateStatusOfDimm - can't get status of ARS from Dimm!\n",
      ArsDimm->Socket,
      ArsDimm->Channel,
      ArsDimm->Dimm
      ));

    ArsDimm->State = ERROR;
    return Status;
  }

  Status = XlateDpaOffsetToSpa (
    NvmDimm,
    ArsDimm->SpaBase,
    CurrentDpa - ArsDimm->DpaBase,
    &ArsDimm->CurrentSpa
    );

  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "UpdateStatusOfDimm - can't translate DPA to SPA!\n"));
    return Status;
  }

  Status = GetLongOperationStatusDataForArs (
    ArsDimm->Socket,
    ArsDimm->Channel,
    ArsDimm->Dimm,
    &ArsStatus
    );

  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: UpdateStatusOfDimm - can't get status of ARS from Dimm!\n",
      ArsDimm->Socket,
      ArsDimm->Channel,
      ArsDimm->Dimm
      ));

    ArsDimm->State = ERROR;
    return Status;
  }

  ArsDimm->ErrorsFound = ArsStatus.ErrorsFound;

  for (i = 0; (i < ArsDimm->ErrorsFound) && (i < ARRAY_SIZE (ArsDimm->DpaErrorAddress)); i++) {
    if (ArsStatus.DpaErrorAddress[i] > TopErrorDpa) {
      TopErrorDpa = ArsStatus.DpaErrorAddress[i];
    }
    ArsDimm->DpaErrorAddress[i] = ArsStatus.DpaErrorAddress[i];
  }
  if (ArsStatus.ErrorsFound) {
    TopErrorDpa = TopErrorDpa + UNCORRECTABLE_UNIT_SIZE;
    CurrentDpa = CurrentDpa > TopErrorDpa ? CurrentDpa : TopErrorDpa;
    XlateDpaOffsetToSpa (
      NvmDimm,
      ArsDimm->SpaBase,
      CurrentDpa - ArsDimm->DpaBase,
      &ArsDimm->CurrentSpa
      );
  }

  if (ArsInProgress != 0) {
    ArsDimm->State = IN_PROGRESS;
  } else if (ArsStatus.PrematurelyEnded) {
    if (ArsStatus.Overflow) {
      ArsDimm->State = ENDED_PREMATURELY_OVERFLOW;
    } else {
      ArsDimm->State = ENDED_PREMATURELY_OTHER;
    }
  } else {
    ArsDimm->State = FINISHED;
  }

  return EFI_SUCCESS;
}

/**

  Routine description: finds a Nvdimm in Dimm list, given the location.

  @param[in]  DimmList    - List of the dimms
  @param[in]  DimmListLen - Length of DimmList array passed in
  @param[in]  Socket      - Socket ID (0 based)
  @param[in]  Channel     - Channel ID (0 based, socket based)
  @param[in]  Dimm        - Slot ID (0 based, channel based)

  @returns ARS_DIMM structure for the dimm found or NULL, if not found.

**/
ARS_DIMM *
GetDimmFromArsDimmList (
  IN ARS_DIMM   *DimmList,
  IN UINT8      DimmListLen,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm
  )
{
  ARS_DIMM    *ArsDimm;
  UINT8       i;

  if (DimmList == NULL) {
    return NULL;
  }

  for (i = 0; i < DimmListLen; i++) {
    ArsDimm = &DimmList[i];

    if ((Socket == ArsDimm->Socket) &&
        (Channel == ArsDimm->Channel) &&
        (Dimm == ArsDimm->Dimm)) {

      return ArsDimm;
    }
  }

  return NULL;
}

/**

  Routine description: checks whether the ARS operation is finished on all given
  dimms.

  @param[in]  DimmList    - List of the dimms
  @param[in]  DimmListLen - Length of DimmList array passed in

  @returns TRUE if ARS is finished, FALSE otherwise.

**/
BOOLEAN
AreAllDimmsFinished (
  IN CONST ARS_DIMM    *DimmList,
  IN UINT8             DimmListLen
  )
{
  UINT8 i;

  if (DimmList == NULL || DimmListLen == 0) {
    return FALSE;
  }

  for (i = 0; i < DimmListLen; i++) {
    if (DimmList[i].State == IN_PROGRESS) {
      return FALSE;
    }
  }

  return TRUE;
}

/**

  Routine description: gets error records from the Nvdimm.

  @param[in]  ArsDimm           - dimm to get the errors from.
  @param[out] ErrorsFound       - number of errors added to list.
  @oaram[out] ErrorRecords      - list of error records found.
  @param[in]  ErrorRecordsLen   - length of ErrorRecords array passed in.

  @retval EFI_INVALID_PARAMETER Dimm not found or NULL given.
  @retval EFI_SUCCESS           Status updated.
  @retval other                 Other HW failure.

**/
EFI_STATUS
GetErrorRecordsFromDimm (
  IN CONST ARS_DIMM     *ArsDimm,
  OUT UINT16            *ErrorsFound,
  OUT ARS_ERROR_RECORD  *ErrorRecords,
  IN UINT16             ErrorRecordsLen
  )
{
  EFI_STATUS          Status;
  NVDIMM              *NvmDimm;
  UINT8               i;
  UINT64              ErrorSpa;

  if ((ArsDimm == NULL) ||
      (ErrorsFound == NULL) ||
      (ErrorRecords == NULL) ||
      (ErrorRecordsLen == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, ArsDimm->Socket, ArsDimm->Channel, ArsDimm->Dimm, NVDIMM_FLAG_ENABLED);
  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *ErrorsFound = 0;

  //
  // Perform translations for all errors found and build the
  // error record log
  //
  for (i = 0; (i < ArsDimm->ErrorsFound) && (i < ErrorRecordsLen); i++) {
    Status = XlateDpaOffsetToSpa (
      NvmDimm,
      ArsDimm->SpaBase,
      ArsDimm->DpaErrorAddress[i] - ArsDimm->DpaBase,
      &ErrorSpa);

    if (EFI_ERROR (Status)) {

      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    ErrorRecords[i].NfitHandle  = NFIT_HANDLE_BUILD (ArsDimm->Socket, ConvertSktChToImc(ArsDimm->Channel),
                                                     ConvertSktChToImcCh(ArsDimm->Channel), ArsDimm->Dimm);
    ErrorRecords[i].SpaOfErrLoc = ErrorSpa;
    ErrorRecords[i].Length      = ARS_ALIGNMENT_SIZE;

    *ErrorsFound += 1;
  }

  return EFI_SUCCESS;
}

/**

  Routine description: returns the state whether ARS for given dimm is in progress.

  @param[in]  ArsDimm           - dimm to check.

  @returns TRUE if in progress, FALSE otherwise.

**/
BOOLEAN
IsDimmInProgress (
  IN CONST ARS_DIMM   *ArsDimm
  )
{
  if (ArsDimm == NULL) {
    return FALSE;
  }

  return ArsDimm->State == IN_PROGRESS;
}

/**

  Routine description: returns the state whether ARS for given dimm ended prematurely.

  @param[in]  ArsDimm           - dimm to check.

  @returns TRUE if ended prematurely, FALSE otherwise.

**/
BOOLEAN
IsDimmEndedPrematurely (
  IN CONST ARS_DIMM   *ArsDimm
  )
{
  if (ArsDimm == NULL) {
    return FALSE;
  }

  return ArsDimm->State == ENDED_PREMATURELY_OTHER || IsDimmOverflow (ArsDimm);
}

/**

  Routine description: returns the state whether ARS for given dimm ended prematurely.

  @param[in]  ArsDimm           - dimm to check.

  @returns TRUE if ended prematurely, FALSE otherwise.

**/
BOOLEAN
IsDimmOverflow (
  IN CONST ARS_DIMM   *ArsDimm
  )
{
  if (ArsDimm == NULL) {
    return FALSE;
  }

  return ArsDimm->State == ENDED_PREMATURELY_OVERFLOW;
}


/**

  Routine description: returns SPA address of the current location of ARS operation
  for given dimm.

  @param[in]  ArsDimm           - dimm to check.

  @returns SPA address or 0, when problem occured.

**/
UINT64
GetCurrentSpaFromDimm (
  IN CONST ARS_DIMM   *ArsDimm
  )
{
  if (ArsDimm == NULL) {
    return 0;
  }

  return ArsDimm->CurrentSpa;
}
