/** @file
  Far memory controller access mechanisms and mailbox commands.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <SysHost.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemFmcIpLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/SysHostPointerLib.h>
#include <FnvAccessCommon.h>
#include "Include/MemFmcIpLibInternal.h"


/**
  This function disables the extended ADR

  @param[in] Host   - Pointer to sysHost, the system host (root) structure
  @param[in] Socket - Socket Id

  @retval None
**/
VOID
DisableExtendedAdr (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT32 Status;
  UINT8  MbStatus;
  UINT8  Dimm;
  UINT8  Channel;

  if (NULL == Host) {
    return;
  }

  for (Channel = 0; Channel < MAX_CH; Channel++) {
    //
    // Skip if channel disabled
    //
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if DCPMM not present in the slot
      //
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      //
      // Initialize MB status before transaction
      //
      MbStatus = MB_STAT_UNDEFINED;
      //
      // Attempt to disable extended ADR
      //
      Status = SetExtendedAdrState (Host, Socket, Channel, Dimm, EXTENDED_ADR_DISABLED, &MbStatus);
      if ((SUCCESS == Status) && (MB_STAT_SUCCESS == MbStatus)) {
        RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Extended ADR Disabled successfully\n");
      }
    }
  }
}

/**
  This function disables the LSS Latch on warm boot flow

  @param[in] Host   - Pointer to sysHost, the system host (root) structure
  @param[in] Socket - Socket Id

  @retval None
**/
VOID
DisableLastSystemShutdownLatch (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT32                    Status;
  UINT8                     MbStatus;
  UINT8                     Dimm;
  UINT8                     Ch;
  CHANNEL_NVRAM_STRUCT      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT         (*DimmNvList)[MAX_DIMM];
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;

  if (NULL == Host) {
    return;
  }

  //
  // Disable the LSS latch only on the warm boot and S3 resume (FW disables the LSS latch on the cold boot flow)
  //
  if (!(((GetSysBootMode () == NormalBoot) && (Host->var.mem.WarmBootRequested == TRUE)) || (GetSysBootMode () == S3Resume))) {
    return;
  }

  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    //
    // Skip if channel disabled
    //
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots; Dimm++) {
      //
      // Skip if DCPMM not present in the slot
      //
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      //
      // Read NVDIMM BSR to determine if LSS Latch can be disabled
      //
      ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);

      //
      // Skip if BSR bit[29] set (NVMDIMM received surprise clock stop - don't disabled the latch)
      //
      if (Bsr.Bits.RebootRequired == 1) {
        RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem received surprise clock stop - don't disabled the LSS Latch\n");
        continue;
      }

      //
      // Skip if BSR bit[23] and bit[29] not set (WDB flush failed)
      //
      if ((Bsr.Bits.CreditReady == 0) && (Bsr.Bits.RebootRequired == 0)) {
        RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "WDB flush failed - don't disabled the LSS Latch\n");
        continue;
      }

      //
      // Initialize MB status before transaction
      //
      MbStatus = MB_STAT_UNDEFINED;

      Status = SetLatchSystemShutdownState (Host, Socket, Ch, Dimm, LSSS_DISABLED, &MbStatus);
      if ((SUCCESS == Status) && (MB_STAT_SUCCESS == MbStatus)) {
        RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "LSS Latch Disabled successfully\n");
      }
    }
  }
}

/**
  FMC CSR unlock sequence for a channel

  @param[in] Socket      - Processor socket ID
  @param[in] Ch          - Current channel

  @retval EFI_UNSUPPORTED if the feature is not supported
  @retval EFI_SUCCESS otherwise

**/
EFI_STATUS
EFIAPI
FmcCsrUnlockCh (
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  PSYSHOST            Host;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();

  if (Host->var.mem.socket[Socket].channelList[Ch].CsrUnlock == 0) {

    FnvPollingBootStatusRegisterCh (Host, Socket, Ch, FNV_CSR_UNLOCK);

    //
    // SlowWarm path also requires a wait for MediaReady (BSR[16])
    // Polling for FNV_MEDIA_DONE checks for MediaReady and also confirms
    // that MediaError (BSR[17]) is clear
    //
    if (Host->var.mem.DcpmmWaitMediaReady == TRUE) {
      FnvPollingBootStatusRegisterCh (Host, Socket, Ch, FNV_MEDIA_DONE);
    }

    Host->var.mem.socket[Socket].channelList[Ch].CsrUnlock = 1;

    ChannelNvList = GetChannelNvList (Host, Socket);

    if ((GetSysBootMode () == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast))) {
      for (Dimm = 0; Dimm < (*ChannelNvList)[Ch].maxDimm; Dimm++) {
        if (!IsDimmPresent (Socket, Ch, Dimm)) {
          continue;
        }
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
          WriteRCFnv (Host, Socket, Ch, Dimm, 0, RDIMM_RC0E);
        }
      }
    }
  }
  return EFI_SUCCESS;
}

