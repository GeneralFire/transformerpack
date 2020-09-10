/** @file

  MeUmaLib.c

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

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MeUmaLib.h>

#if ME_SUPPORT_FLAG
#include <Ppi/MeUmaPpi.h>
#endif // ME_SUPPORT_FLAG
#if IE_SUPPORT
#include <Ppi/IeHeci.h>
#endif // IE_SUPPORT

/**
  ME-BIOS hook before checking and triggering the proper type of reset.
**/
VOID
MeUmaHandleResetRequests (
  VOID
  )
{
#if ME_SUPPORT_FLAG
  //
  // Perform MEBIOS tasks before a reset is triggered after control returns to the caller
  //
  EFI_STATUS         Status = EFI_UNSUPPORTED;
  ME_UMA_PPI         *MeUma = NULL;

  //
  // Locate MeUma PPI.
  //
  Status = PeiServicesLocatePpi (&gMeUmaPpiGuid, 0, NULL, &MeUma);
  if (!EFI_ERROR (Status)) {
    //
    // Send HostResetWarning notification to ME
    //
    DEBUG((EFI_D_INFO, "[ME] Send HostResetWarning notification to ME\n"));
    Status = MeUma->MeUmaSendHostResetWarning (NULL);
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "[ME] HostResetWarning notification complete\n"));
    } else {
      DEBUG ((EFI_D_ERROR, "[ME] ERROR: HostResetWarning notification FAILED (%r)\n", Status));
    }
  }
#endif // ME_SUPPORT_FLAG
} // MeUmaHandleResetRequests

/**
  Get the size of ME Segment from the ME FW
  and
  Get the size of IE Segment from the IE FW
  Gathered sizes are set in to environment
**/
VOID
MeUmaConfigureRequestedSegmentSize (
  VOID
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
#if ME_SUPPORT_FLAG
  ME_UMA_PPI        *MeUma;
  ME_UMA_INFO       *MeUmaInfo;
  UINT32            Alignment;
#endif // ME_SUPPORT_FLAG
#if IE_SUPPORT
  UINT32            IeUmaSize; // IE Stolen Size in MB units
  UINT32            IeAlignment;
  UINT32            MeAlignment;
  PEI_IE_HECI_PPI   *IeHeciPpi;
#endif // IE_SUPPORT

  DEBUG ((EFI_D_INFO, "%a: Entered\n", __FUNCTION__));

  if (GetSysBootMode () != S3Resume) {
    SetMeRequestedSize (0);
    SetMeRequestedAlignment (1);
    SetIeRequestedSize (0);
    SetIeRequestedAlignment (1);

#if ME_SUPPORT_FLAG
    Status = PeiServicesLocatePpi (&gMeUmaPpiGuid, 0, NULL, &MeUma);
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a: ME Section\n", __FUNCTION__));
      Alignment = 1; // Default Alignment 1MB
      if (MeUma->MeUmaIsEnabled ()) {
        SetMeRequestedSize (MeUma->MeUmaGetSize (NULL, &Alignment));
        SetMeRequestedAlignment (Alignment);
      }
    }
    if (GetMeRequestedSize () != GetSysMeRequestedSizeNv ()) {
      SetSysMeRequestedSizeNv (GetMeRequestedSize ());
    }
    if (GetMeRequestedAlignment () != GetSysMeRequestedAlignmentNv ()) {
      SetSysMeRequestedAlignmentNv (GetMeRequestedAlignment ());
    }
    DEBUG ((EFI_D_INFO, "ME UMA size = %d MBytes\n", GetMeRequestedSize ()));
#endif // ME_SUPPORT_FLAG

#if IE_SUPPORT
    IeUmaSize = 0;
    IeAlignment = 1;

    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) { // For SKX: IE UMA = 0
      DEBUG ((EFI_D_ERROR, "[IE] WARNING: For SKX CPU there is no IE UMA support\n"));
    } else {
      DEBUG ((EFI_D_INFO, "%a: IE section\n", __FUNCTION__));
      //
      // Locate IeHeci PPI.
      //
      Status = PeiServicesLocatePpi (&gPeiIeHeciPpiGuid, 0, NULL, &IeHeciPpi);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_WARN, "[IE] WARNING: HECI PPI not found (%r), set IeUmaSize to 0\n"));
      } else {
        Status = IeHeciPpi->GetUmaSize (
                              IeHeciPpi,
                              &IeUmaSize,
                              &IeAlignment
                              );
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "UMA: IE UMA requested returned error %r\n", Status));
          IeUmaSize = 0;
          IeAlignment = 1;
        } else if (IeUmaSize + GetMeRequestedSize () > ME_UMA_SIZE_UPPER_LIMIT) {
          DEBUG ((
            EFI_D_ERROR,
            "UMA: IE UMA requested size too large. (%d + %d)\n",
            IeUmaSize,
            GetMeRequestedSize ()
            ));
          // Do not allow for too large UMA area allocation, but for size 0,
          // indicate error to IE FW in DID message (InitStat = ME_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS)
          IeUmaSize = 0;
          IeAlignment = 1;
        }
      }
    }
    // IeUmaSize is for IMR separation only
    SetIeRequestedSize (IeUmaSize);
    SetIeRequestedAlignment (IeAlignment);
    DEBUG ((EFI_D_INFO, "[IE] GetUmaSize = %d\n", IeUmaSize));

    if (MeUma != NULL) {
      // Verify ME and IE UMA alignment relation
      // and chose bigger for both
      MeAlignment = GetMeRequestedAlignment ();
      if (MeAlignment > IeAlignment) {
        if (MeAlignment > 1) {
          SetIeRequestedAlignment (MeAlignment);
        }
      } else {
        if (IeAlignment > 1) {
          SetMeRequestedAlignment (IeAlignment);
        }
      }
    }
#endif // IE_SUPPORT
    if (GetIeRequestedSize () != GetSysIeRequestedSizeNv ()) {
      SetSysIeRequestedSizeNv (GetIeRequestedSize ());
    }
    if (GetIeRequestedAlignment () != GetSysIeRequestedAlignmentNv ()) {
      SetSysIeRequestedAlignmentNv (GetIeRequestedAlignment ());
    }
  } else {
    SetIeRequestedSize (GetSysIeRequestedSizeNv ());
    SetIeRequestedAlignment (GetSysIeRequestedAlignmentNv ());
    SetMeRequestedSize (GetSysMeRequestedSizeNv ());
    SetMeRequestedAlignment (GetSysMeRequestedAlignmentNv ());
#if ME_SUPPORT_FLAG
    Status = PeiServicesLocatePpi (&gMeUmaPpiGuid, 0, NULL, &MeUma);
    if (!EFI_ERROR (Status)) {
      MeUmaInfo = MeUma->MeUmaGetInfo ();
      if (MeUmaInfo != NULL) {
        MeUmaInfo->MeUmaSize = GetMeRequestedSize ();
      }
    }
#endif // ME_SUPPORT_FLAG
  }

  DEBUG ((EFI_D_ERROR, "%a: Exiting (Status = %r)\n", __FUNCTION__, Status));
} // MeUmaConfigureRequestedSegmentSize

/**
  Functions sends DramInitDone messages to ME and IE FWs
  according to passed parameters

  @param [in] MrcStatus             - Return status from MRC code execution
  @param [in] MemSize               - System memory size
  @param [in] PreviousBootError     - Previous boot error
  @param [in] SubBootMode           - SubBoot mode defined in MRC Header
  @param [in] MeMemoryInformation   - Pointer to ME memory information
  @param [in] IeMemoryInformation   - Pointer to ME memory information
**/
VOID
MeUmaSendDramInitDone (
  IN EFI_STATUS             MrcStatus,
  IN UINT32                 MemSize,
  IN UINT8                  PreviousBootError,
  IN SUB_BOOT_MODE          SubBootMode,
  IN UMA_MEMORY_INFORMATION *MeMemoryInformation,
  IN UMA_MEMORY_INFORMATION *IeMemoryInformation
  )
{
#if ME_SUPPORT_FLAG
  ME_UMA_PPI        *MeUma = NULL;
  EFI_STATUS        MeStatus;
#endif // ME_SUPPORT_FLAG
#if IE_SUPPORT
  PEI_IE_HECI_PPI   *IeHeciPpi = NULL;
  UINT8             IeInitStat = 0;
  EFI_STATUS        IeStatus;
#endif // IE_SUPPORT

  //
  // Call MeConfigDidReg when ME UMA is configured correctly
  // or MRC error occurred
  //
  if (!GetSysResetRequired()) {
#if ME_SUPPORT_FLAG
    //
    // Locate MeUma PPI.
    //
    MeStatus = PeiServicesLocatePpi (&gMeUmaPpiGuid, 0, NULL, &MeUma);
    if (EFI_ERROR (MeStatus)) {
      DEBUG ((EFI_D_WARN, "[ME] WARNING: ME UMA PPI not found (%r), will not send MemoryInitDone\n", MeStatus));
    } else {
      // Prepare DramInitDone kick
      MeUma->MeUmaSaveDramInitDoneParameters (
               MrcStatus,
               MemSize,
               PreviousBootError,
               SubBootMode,
               MeMemoryInformation->NcMemLowBaseAddr,
               MeMemoryInformation->NcMemHighBaseAddr,
               MeMemoryInformation->NcMemLowLimit,
               MeMemoryInformation->NcMemHighLimit
               );
    }
#endif // ME_SUPPORT_FLAG

#if IE_SUPPORT
    //
    // Locate IeHeci PPI.
    //
    IeStatus = PeiServicesLocatePpi (&gPeiIeHeciPpiGuid, 0, NULL, &IeHeciPpi);
    if (EFI_ERROR (IeStatus)) {
      DEBUG ((EFI_D_WARN, "[IE] WARNING: HECI PPI not found (%r), will not send MemoryInitDone\n", IeStatus));
    } else {
      UINT8  Action = 0;

      IeInitStat = IE_MEMORY_INITIALIZATION_SUCCESSS;
      if (EFI_ERROR (MrcStatus)) {
        if (MemSize == 0) {
          DEBUG ((EFI_D_ERROR, "[IE] UMA: No Memory in Channels.\n"));
          IeInitStat = IE_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS;
        } else {
          DEBUG ((EFI_D_ERROR, "[IE] UMA: Memory Initialization Error.\n"));
          IeInitStat = IE_MEMORY_INITIALIZATION_ERROR;
        }
      } else {
        if ((IeHeciPpi->IeUmaSize > ME_UMA_SIZE_UPPER_LIMIT)) {
          DEBUG ((EFI_D_ERROR, "[IE] UMA: Invalid ME UMA Size requested\n"));
          IeInitStat = IE_MEMORY_INITIALIZATION_ERROR;
        }
      }
      IeStatus = IeHeciPpi->MemoryInitDoneNotify (
                              IeHeciPpi,
                              IeMemoryInformation->NcMemLowBaseAddr,
                              IeMemoryInformation->NcMemHighBaseAddr,
                              IeInitStat,
                              &Action
                              );
      if (!EFI_ERROR (IeStatus)) {
        DEBUG ((EFI_D_INFO, "[IE] IeMemoryInitDone Complete.\n"));
        IeStatus = IeHeciPpi->MemoryInitDoneAction (IeHeciPpi, Action);
        if (EFI_ERROR (IeStatus)) {
          DEBUG ((EFI_D_ERROR, "[IE] ERROR: MemoryInitDone action failed\n"));
        }
      } else {
        DEBUG ((EFI_D_ERROR, "[IE] ERROR: IeMemoryInitDone FAILED (%r).\n", IeStatus));
      }
    }
#endif // IE_SUPPORT
  }
} // MeUmaSendDramInitDone
