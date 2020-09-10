/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#include <Protocol/SmmBase2.h>
#include <Library/SmmPeriodicTimerLib.h>
#include <Library/TimerLib.h>
#include <Library/RasDebugLib.h>
#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#include <Library/PlatMemTopologyLib.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/ErrorReportLib.h>
#include "SmbusRecovery.h"


SMBUS_RECOVERY_DATA_STRUCT                    mSmBusRecoveryData[MAX_SOCKET][MAX_SMBUS_SPD_SEGMENTS];
UINT8                                         mSktBeingWorkedOn = 0xFF;
UINT8                                         mSemaphorePollCount = 0;
UINT8                                         mSmbusRecoveryCallbackCount = 0;
UINT8                                         mSemapohre[MAX_SOCKET];


/**
  This Function will check if the SMBUS is recovered by the H/W or not.

  @param [in]   SktId       SktId to be restored
  @param [in]   SegIndex    SegIndex to be restored

  @retval EFI_SUCCESS: SMBUS recovery completed successfully.

**/
EFI_STATUS
EFIAPI
CheckSmbusRecoveryComplete (
  IN    UINT8     SktId,
  IN    UINT8     SegIndex
  )
{
  EFI_STATUS                       Status = EFI_ALREADY_STARTED;
  UINT32                           SmbCkovrd;

  //
  // H/W will set smb_ckovrd to '1' to indicate that the recovery is complete on the SMBUS.
  //
  SmbCkovrd = GetProcSmbCkovrd (SktId, SegIndex);
  if (SmbCkovrd ==0){
    return Status;
  } else if (SmbCkovrd == 1) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Error Recovery Complete on SktId:0x%x, SegIndex:0x%x, smb_ckovrd is 0x%x\n", SktId, SegIndex, SmbCkovrd));
    return  EFI_SUCCESS;
  }
  return EFI_SUCCESS;
}

/**

  This function will restore the state that was saved while initiating the SMBUS error recovery.
  This will also end the recovery process.

  @param [in]  SktId         SktId to be Restored
  @param [in]  SegIndex      SegIndex to be restored

  @retval  None

**/
VOID
EFIAPI
RestoreStateAndEndSmbusRecovery (
  IN   UINT8   SktId,
  IN   UINT8   SegIndex
  )
{
  UINT32                                  SmbSbe;
  UINT8                                   Count = 0;
  UINT32                                  PcodeMailboxData = 0;
  UINT32                                  PcodeMailboxStatus = 0;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "Restoring the state after SMBUS Error Recovery on SktId:0x%x, SegIndex:0x%x\n", SktId, SegIndex));

  //
  // Deactivate the Soft Reset
  //
  SetProcSmbSoftrst (SktId, SegIndex, 0);
  //
  // To clear SMB_SBE, initiate a SMBUS transaction for TSOD temperature read to a populated DIMM
  // (H/W might provide a bit to do this, when that happens then code to do the SMBUS transaction can be removed).
  //
  DoProcSmbTsodRead (SktId, SegIndex, mSmBusRecoveryData[SktId][SegIndex].Bits.TsodSA);
  //
  // Wait for the Smbus transaction to complete
  //
  Count = 0;
  SmbSbe = GetProcSmbSbe (SktId, SegIndex);

  while (SmbSbe) {
    Count++;
    MicroSecondDelay (1);
    SmbSbe = GetProcSmbSbe (SktId, SegIndex);
    if (Count >= 150) {
      break;
    }
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "smb_sbe is 0x%x on SktId :0x%x, SegIndex:0x%x\n", SmbSbe, SktId, SegIndex));
  //
  // Restore the state of the SMBUS that was saved before doing the recovery
  //
  SetProcSmbTsodPollEnSbeEn (
    SktId,
    SegIndex,
    mSmBusRecoveryData[SktId][SegIndex].Bits.TsodPollEn,
    mSmBusRecoveryData[SktId][SegIndex].Bits.SmbSbeEn
    );

  //
  // Enable CLTT
  //
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "Calling Pcode command to enable CLTT\n"));
  PcodeMailboxData = (1 << SegIndex);
  PcodeMailboxStatus = Bios2PcodeMailBoxWrite(SktId, MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL, PcodeMailboxData);
}

/**

  This is the callback routine registered for the Periodic SMI handler.
  This routine will check if the SMBus recovery process is complete.
  If so, this will end the recovery process and unregister the periodic handler.

  @param Handle
  @param *PeriodicTimer
  @param *CommBuffer
  @param *CommBufferSize

  @retval EFI_SUCCESS: If the SMBus is recovered, and the periodic handler is unregistered

**/

EFI_STATUS
EFIAPI
SmbusRecoveryCallback (
  IN  EFI_HANDLE    Handle,
  IN  CONST VOID    *PeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  )
{
  UINT8                                     SktId;
  UINT8                                     McId;
  UINT8                                     SegIndex;
  EFI_STATUS                                Status;
  BOOLEAN                                   Flag = TRUE;

  for (SktId = 0; SktId < MAX_SOCKET; SktId++ ) {
    if ((IsSocketPresent (SktId)) == FALSE) {
      continue;
    }

    for (McId = 0; McId < MAX_IMC; McId++) {
      if ((IsNodePresent (SKTMC_TO_NODE (SktId, McId))) == FALSE) {
        continue;
      }

      SegIndex = GetProcSmbInstanceFromMcId (McId);

      if (mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute & UNDER_RECOVERY) {
        mSmbusRecoveryCallbackCount++;
        Status = CheckSmbusRecoveryComplete (SktId, SegIndex);

        if (!EFI_ERROR (Status) || (mSmbusRecoveryCallbackCount >= SMBUS_RECOVERY_CALLBACK_COUNT)) {
          mSmbusRecoveryCallbackCount = 0;
          ReportSpdSmbusErrorLog (SktId, SegIndex, (Status == EFI_SUCCESS));
          RestoreStateAndEndSmbusRecovery (SktId, SegIndex);
          mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute &= ~(UNDER_RECOVERY);
        } else {
          Flag = FALSE;
          mSktBeingWorkedOn = 0xFF;
        }
      }
    }

    if ((mSemapohre[SktId]== 1) && (Flag == TRUE)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Release Semaphore\n"));
      ReleaseSemaphoreSmbus (SktId);
      mSemapohre[SktId] = 0;
    }
  }
  //
  // If all Smbus recovery operations were completed, end the periodic SMI timer.
  //
  if (Flag) {
    mSmbusRecoveryCallbackCount = 0;
    EndSmmPeriodicTimer ();
  }
  return EFI_SUCCESS;
}


/**

  This is the routine attached to the periodic SMI handler to check if the BIOS gets the semaphore.
  If the timeout happens, BIOS will override the semaphore.

  If BIOS gets the semaphore, unregister the periodic handler.

  @param  Handle
  @param  *PeriodicTimer
  @param  *CommBuffer
  @param  *CommBufferSize

  @retval EFI_SUCCESS: Completed executing the function.

**/

EFI_STATUS
EFIAPI
SemaphoreAcquireCallBack (
  IN  EFI_HANDLE    Handle,
  IN  CONST VOID    *PeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  UINT8                                 SktId;
  SktId = mSktBeingWorkedOn;

  mSemaphorePollCount++;

  //
  // Count is reached, override the semaphore
  //
  if (mSemaphorePollCount == 5) {
    if (!IsAquireSemaphored (SktId)) {
      OverrideSemaphore (SktId);
      mSemaphorePollCount = 0;
    }
  }

  //
  // If semaphore acquired, reset the poll count and unregister the handler
  //
  if (IsAquireSemaphored (SktId)) {
    EndSmmPeriodicTimer ();
    mSemaphorePollCount = 0;
    mSemapohre[SktId] = 1;
  }
  return Status;
}



/**

  This Function triggers the Smbus Recovery on the Controllers on which error happened

  @param  [in]  SktId            SktId to Trigger Smbus Recovery
  @param  [in]  SegIndex         SegIndex to Trigger Smbus Recovery

  @retval None

**/
VOID
EFIAPI
TriggerSmbusRecovery (
  IN    UINT8     SktId,
  IN    UINT8     SegIndex
  )
{
  RAS_DEBUG ((LEVEL_REG, "Smbus Error Recovery Triggered on SktId:0x%x, SegIndex:0x%x\n", SktId, SegIndex));
  TriggerSmbRecovery (SktId, SegIndex);
}

/**

  This function will check if the error happened on SMBUS, if so, save some of the details
  and prepare for the recovery.

  @param [in]  SktId            SktId to Check and Prepare Smbus Recovery.
  @param [in]  SegIndex         SegIndex to Check and Prepare Smbus Recovery.

  @retval FALSE: If no error happened.
  @retval TRUE:  If there was an error, and it is now prepared with bitmap of the memory controller on which error happened.

**/
BOOLEAN
EFIAPI
CheckAndPrepareSmbusRecovery (
  IN  UINT8    SktId,
  IN  UINT8    SegIndex
 )
{
  UINT32                                        TsodSa =0;
  UINT32                                        TsodPollEn =0;
  UINT32                                        SmbSbeEn =0;
  BOOLEAN                                       Flag = FALSE;
  UINT32                                        PcodeMailboxData = 0;
  UINT32                                        PcodeMailboxStatus = 0;

  if (GetProcSmbSmiEn (SktId, SegIndex) == 0) {
    return Flag;
  }

  if ((GetProcSmbSbe (SktId, SegIndex) == 1) || (GetProcSmbCkovrd (SktId, SegIndex) == 0) ) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Smbus Error is present on SktId:0x%x, SegmentId:0x%x\n", SktId, SegIndex));

    if (mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute & UNDER_RECOVERY) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Error Recovery is already triggerred on Smbus Error is present on SktId:0x%x, SegmentId:0x%x\n", SktId, SegIndex));
      return Flag;
    }

    //
    // When there is an SMBUS error,
    // save the tsod_sa (Last Slave address that did the Smbus Transaction.
    // we need this to issue an Smbus read transaction to clear the smb_sbe.
    // And also save the TsodPollEn and SmbSbeEn settings

    //
    // Save Smbus features and disable TSOD polling
    //
    GetProcSmbTsodSaTsodPollEnSbeEn (SktId, SegIndex, &TsodSa, &TsodPollEn, &SmbSbeEn);
    mSmBusRecoveryData[SktId][SegIndex].Bits.TsodSA = (UINT8) TsodSa;
    mSmBusRecoveryData[SktId][SegIndex].Bits.TsodPollEn = (UINT8) TsodPollEn;
    mSmBusRecoveryData[SktId][SegIndex].Bits.SmbSbeEn = (UINT8) SmbSbeEn;

    DisableTsodAndSmbSbeEn (SktId, SegIndex);

    //
    // Disable CLTT
    //
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Calling Pcode command to disable CLTT\n"));
    PcodeMailboxData = 1 << SegIndex;
    PcodeMailboxStatus = Bios2PcodeMailBoxWrite(SktId, MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL, PcodeMailboxData);

    Flag = TRUE;
  }
  return Flag;
}

/**

  This function will do the SMBUS recovery during the boot time.

  @param None

  @retval EFI_SUCCESS: If there is no error or if it was recovered successfully.

**/
EFI_STATUS
EFIAPI
BootTimeSmbusRecovery (
  VOID
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT8         Count;
  UINT8         SktId;
  UINT8         McId;
  UINT8         SegIndex;

  for (SktId = 0; SktId < MAX_SOCKET; SktId++) {
    if (IsSocketPresent (SktId) == FALSE) {
      continue;
    }

    // if UboxSmbus error was not detected, continue to scan next socket.
    if (IsUboxSmbusError (SktId) == FALSE) {
      continue;
    }

    for (McId = 0; McId < MAX_IMC; McId++) {
      if ((IsNodePresent (SKTMC_TO_NODE (SktId, McId))) == FALSE) {
        continue;
      }

      SegIndex = GetProcSmbInstanceFromMcId (McId);

      if (CheckAndPrepareSmbusRecovery (SktId, SegIndex)) {
        //
        // Trigger SMBUS recovery process  and check if the recovery is complete
        // Smbus recovery time is 35ms.
        // If recovery is success, restore the state saved before and end the recovery process.
        //

        TriggerSmbusRecovery (SktId, SegIndex);

        Count = 0;
        do {
          MicroSecondDelay (SMBUS_RECOVERY_TIME_35_MS * 1000);

          Status = CheckSmbusRecoveryComplete (SktId, SegIndex);
          Count++;
          if (Count >= 2) {
            break;
          }
        } while (Status != EFI_SUCCESS);

        ReportSpdSmbusErrorLog (SktId, SegIndex, (Status == EFI_SUCCESS));

        if (Status == EFI_SUCCESS) {
          RestoreStateAndEndSmbusRecovery (SktId, SegIndex);
        }
      }
    }
  }
  return Status;
}

/**

  This function will configure SMBUS on all controllers to generate SMI when error happens on SMBUS.

  @param  None

  @retval  None

**/
VOID
EFIAPI
ConfigureSmbusForErrorRecovery (
  VOID
  )
{
  UINT8                 SktId;
  UINT8                 McId;
  UINT8                 SegIndex;

  for (SktId = 0; SktId < MAX_SOCKET; SktId++ ) {
    if ((IsSocketPresent (SktId)) == FALSE) {
      continue;
    }
    for (McId = 0; McId < MAX_IMC; McId++) {
      if (IsNodePresent (SKTMC_TO_NODE (SktId, McId)) == FALSE) {
        continue;
      }

      SegIndex = GetProcSmbInstanceFromMcId (McId);

      if ((GetProcSmbSbe (SktId, SegIndex) == 0) && (GetProcSmbCkovrd (SktId, SegIndex) == 1)) {
        SetProcSmbSmiEnSbeEn (SktId, SegIndex, 1, 1);
        RAS_DEBUG ((LEVEL_REG, "Configuring Smbus For Error Recovery on SktId:0x%x, SegIndex:0x%x\n", SktId, SegIndex));
      }
    } // SegIndex
  } // SktId
}

/**

  This function is the entry point of the Smbus recovery SMM handler

  @param  None

  @retval  None

**/
VOID
EFIAPI
SmbusRecoverySmiHandler (
  VOID
  )
{
  UINT8                                     SktId;
  UINT8                                     McId;
  UINT8                                     SegIndex;
  BOOLEAN                                   Flag = FALSE;

  for (SktId = 0; SktId < MAX_SOCKET; SktId++ ) {
    if ((IsSocketPresent (SktId)) == FALSE) {
      continue;
    }

    // if UboxSmbus error was not detected in, continue to scan next socket.
    if (IsUboxSmbusError (SktId) == FALSE) {
      continue;
    }

    for (McId = 0; McId < MAX_IMC; McId++) {
      if ((IsNodePresent (SKTMC_TO_NODE (SktId, McId))) == FALSE) {
        continue;
      }

      SegIndex = GetProcSmbInstanceFromMcId (McId);

      //
      // Check for Smbus Error and if any prepare for its recovery
      //
      if (CheckAndPrepareSmbusRecovery (SktId, SegIndex)) {

        mSktBeingWorkedOn = SktId;

        if (!mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute) {
          //
          // AcquireSemaphore.  Here we will try to get the semaphore to access the
          // SMBUS. If we don't get it right away, we will trigger the Periodic SMI Handler for polling
          // the register until the timeout (5 sec), and then override the semaphore.
          //

          if (mSemapohre[SktId] == 0 ) {
            AquireSemaphore (SktId);
            if (IsAquireSemaphored (SktId)) {
              //
              // Trigger SMBUS recovery process and come back to see if the bus is recovered.
              // The recovery time is 35ms.  So register a periodic handler to check
              // to see if the recovery is complete.
              //
              mSemapohre[SktId] = 1;
              TriggerSmbusRecovery (SktId, SegIndex);

              mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute |= UNDER_RECOVERY;
              Flag = TRUE;
            } else {
              //
              // Trigger Periodic SMI Handler
              //
              SetSmmPeriodicTimer (SMBUS_RECOVERY_SEMAPHORE_PERIOD, SMBUS_RECOVERY_SEMAPHORE_INTERVAL, SemaphoreAcquireCallBack);
              StartSmmPeriodicTimer ();
              return;
            }
          } else {
            TriggerSmbusRecovery (SktId, SegIndex);
            mSmBusRecoveryData[SktId][SegIndex].Bits.Attribute |= UNDER_RECOVERY;
            Flag = TRUE;
          }
        }
      }
    }
  }

  if (Flag) {
    SetSmmPeriodicTimer (SMBUS_RECOVERY_TIME_PERIOD, SMBUS_RECOVERY_TIME_INTERVAL, SmbusRecoveryCallback);
    StartSmmPeriodicTimer ();
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Registering the periodic handler to check the recovery is complete or not\n"));
  }
}


/**

  Enable Smbus Recovery Handler.

  @param  None

  @retval None

**/
VOID
EFIAPI
EnableSmbusRecovery (
  VOID
  )
{
  BootTimeSmbusRecovery ();
  ConfigureSmbusForErrorRecovery ();
}

