/** @file
  This file contaisn the functionality for the ME Error Injection.

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

#include <Library/BaseMemoryLib.h>
#include <Library/AddressDecodeLib.h>
#include <Library/ErrorInjectionLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/RasDebugLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/TimerLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#ifdef SKX_HOST
#include <Mem/Library/MemMcIpLib14nm/Include/MemMcRegs.h>
#else
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#endif
#include <Library/MemMcIpRasLib.h>
#include <SiliconSetting.h>
#include <Library/PcuMailBoxRasLib.h>
#include "WheaErrorInj.h"

UINT32                                        mInjAddrValue;
extern   BOOLEAN                                       Is10nmFam;
extern   UINT32                                        mMESegInjAddr;
extern   WHEA_EINJ_TRIGGER_ACTION_TABLE                *mEinjAction;
extern   WHEA_EINJ_PARAM_BUFFER                        *mEinjParam ;
extern   UINT8                                         mErrorExpected ;
extern   EFI_HANDLE                                    mPeriodicTimerHandle ;
extern   BOOLEAN                                       mMESegInjEn;
extern   BOOLEAN                                       mMESegIcvErrEn ;
extern   MEM_TOPOLOGY                                  *mMemTopologyData;


/**
    Write Data to error injection address to seed the error in memory.

    @param Skt - Socket Id

    @retval None

**/
VOID
EFIAPI
SeeErrorAp (
  UINT8 *Skt
  )
{
  UINT64  PhyAddr = 0;

  //
  // PhyAddr contains the error injection Address
  //
  PhyAddr = mEinjParam->EinjAddr;

  if (mMESegInjEn) {
    PhyAddr = mMESegInjAddr;
  }
  //
  // Write data to memory to seed error in memeory
  //
  if (PhyAddr != 0) {
    *(UINT32 *) (UINTN) PhyAddr = mInjAddrValue;
    AsmFlushCacheLine ((VOID *) (UINTN) PhyAddr);
    //AsmCommitLine ((VOID*)(UINTN)PhyAddr, mInjAddrValue);
    if (!(Is10nmFam)) {
      AsmWbinvd ();
    }
  }

}


/**

  This function enables the error injection on AEP DIMMS
  
  @retval None
  
**/
VOID
EFIAPI
EnableAepErrorInjection (
  VOID
  )
{
  EFI_STATUS  Status;

  RAS_DEBUG ((LEVEL_REG, "Enabling PMem Error injection\n"));
  Status = NvmCtlrPlatformEnableErrInjection ();
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_REG, "Enabling PMem Error injection failed\n"));
  }
}


/**

  This function injects an error for a specified memory type at a
  particular device physical address.

  @param[in]  Socket        - Socket Number
  @param[in]  Ch            - DDR Channel ID
  @param[in]  Dimm          - DIMM number
  @param[in]  Dpa           - DPA of the device
  @param[in]  MemoryType    - Type of memory mapped to DPA
  @param[in]  Enable        - Enable/disable poison for this address

  @retval None

**/
VOID
EFIAPI
InjectAepError (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  MemoryType,
  IN UINT8  Enable
  )
{
  EFI_STATUS Status;

  Status = NvmCtlrEnableErrInjection (Socket, Ch, Dimm);
  ASSERT_EFI_ERROR (Status);

  Status = NvmCtlrInjectPoisonError (Socket, Ch, Dimm, Dpa, MemoryType, Enable);
  if (EFI_ERROR (Status)) {
    mEinjParam->CmdStatus = WHEA_EINJ_CMD_INVALID_ACCESS;
    return;
  }

  // Per RAS team, leave this in even if no longer needed for trigger
  mEinjAction->Trigger0.InjectionAction                  = EFI_ACPI_6_2_EINJ_TRIGGER_ERROR;
  mEinjAction->Trigger0.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
  mEinjAction->Trigger0.Flags                            = 0;
  mEinjAction->Trigger0.Reserved0                        = 00;
  mEinjAction->Trigger0.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
  mEinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
  mEinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
  mEinjAction->Trigger0.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
  mEinjAction->Trigger0.RegisterRegion.Address           = Dpa;
  mEinjAction->Trigger0.Value                            = mInjAddrValue;
  mEinjAction->Trigger0.Mask                             = 0xFFFFFFFF;

  return;
}




/**

    Inject Memory Related errors. i.e. Patrol Scrub, Corrected or UC errors.

    @param ErrorToInject - BIT3 - Memory Correctable (ECC correctable)
                                 - BIT4  - Memory UC non-fatal (i.e. Patrol Scrub)
                                 - BIT5 - Memory UC Fatal - (ECC uncorrectable error)
                                 - BIT12 - Memory uncorrectable error (Core consumption IFU/DCU errors, software is expected to consume the line to trigger errors)

    @retval None

**/
VOID
EFIAPI
InjectMemoryError (
  UINT32   ErrorToInject
  )
{
  UINT8         node = 0;
  UINT8         SocketId;
  UINT8         MemoryControllerId;
  UINT64        PhyAddr;
  UINT8         SktCh;
  UINT32        PhyRank;
  TRANSLATED_ADDRESS TA;
  UINT8         MemoryType;
  UINT8         DdrtMemoryTransactionType;

  RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_STRUCT    EnjExtra;
  RSP_FUNC_ADDR_MATCH_LO_MC_MAIN_STRUCT         MatchLoReg;

  RSP_FUNC_ADDR_MASK_LO_MC_MAIN_STRUCT          MaskLoReg;
  UINT32                                        ScrubAddrLo1;
  UINT32                                        ScrubAddrLo2;
  EFI_STATUS                                    Status = EFI_SUCCESS;
  UINT32                                        RetryCount;
  UINT32                                        Read32;
  UINT32                                        ScrubIssued;
  UINT32                                        ScrubCtl;
  UINT32                                        Temp1 = 0;

  UINT32        ClusterXorModeEn = 0;
  UINT32        ChXorModeEn = 0;
  UINT64        PhyAddrC;
  UINT64        PhyAddrD;

  Read32   = 0;
  ZeroMem (&TA, sizeof(TRANSLATED_ADDRESS));
  MemoryType = 0;
  DdrtMemoryTransactionType = 0;

  //
  //Step 1.  Unlock error injection mechanism on socket basis
  // Scope is package level. No need to do on each thread.
  //
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, UnLockInjLogic, NULL);

  PhyAddr = mEinjParam->EinjAddr;

  if (IsCxlAddress(PhyAddr, &SocketId)) {
    RAS_DEBUG((LEVEL_REG, " Skipping Inject CXL Memory Error - PhyAddr = 0x%lx\n", (UINT64) PhyAddr));
    return;
  }
  RAS_DEBUG((LEVEL_REG, " InjectMemoryError - PhyAddr = 0x%lx\n", (UINT64) PhyAddr));

  if (mMESegInjEn) {
    PhyAddr = mMESegInjAddr;
    RAS_DEBUG ((LEVEL_REG, " Inject UMA Error - PhyAddr = 0x%lx\n", (UINT64) PhyAddr));
    if (PhyAddr == ((UINT64) - 1)) {
      mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
      MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
      return;
    }

    //
    // Need to do pre-setup before injecting error in ME segment
    // set MC9[Bit5] = 0 and MESEG_LIMIT.EN = 0
    //
    PreMESegErrInjSetup ();

    //
    // If the error is ICV type, we just need to corrupt the memory location, do the post ME injection setup
    // and call ME EINJ_CONSUME Command
    //
    if (mMESegIcvErrEn) {
      CorruptMESegInjOffset ();

      //
      // Need to do post-setup after memory corruption/error injection in ME seg
      // set MESEG_LIMIT.EN = 1 and MC9[Bit5] = 1
      //
      PostMESegErrInjSetup ();

      //
      // call ME EINJ_CONSUME command
      //
      InitiateUmaErrorConsume ();
      MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
      return;
    }
  }

  //
  // Translate physical address to DIMM Address
  //
  Status = SystemAddressToDimmAddress ((UINTN) PhyAddr, &TA);
  RAS_DEBUG((LEVEL_REG, "Address decode status:%r  \n", Status));

  if (EFI_ERROR (Status)) {
    mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
    RAS_DEBUG ((LEVEL_REG, " InjectMemoryError - Address translation error \n"));
    MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
    return;
  }

  SocketId = TA.SocketId;
  MemoryControllerId = TA.MemoryControllerId;
  PhyRank = TA.PhysicalRankId;
  node = SKTMC_TO_NODE(SocketId, MemoryControllerId);
  SktCh = NODECH_TO_SKTCH(node, TA.ChannelId);

  if (TA.MemType != MemType1lmDdr) {

    if (TA.MemType == MemType1lmAppDirect) {
      MemoryType = POISON_MEMORY_TYPE_APP_DIRECT;
    } else if (TA.MemType == MemType2lmDdrCacheMemoryMode) {
      MemoryType = POISON_MEMORY_TYPE_2LM;
    }

    if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL) {
      if ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType2 & WHEA_PATROL_SCRUB_ERROR)) {
        DdrtMemoryTransactionType = POISON_MEMORY_TYPE_PATROL_SCRUB;
      } else {
        DdrtMemoryTransactionType = MemoryType;
      }

      RAS_DEBUG ((LEVEL_REG, "Inject PMem Error of TransactionType %d in SktId %d, Channel %d, Dimmslot %d, DPA: 0x%lx\n",
                  DdrtMemoryTransactionType, SocketId, SktCh, TA.DimmSlot, TA.DPA));
      InjectAepError (SocketId, SktCh, TA.DimmSlot, TA.DPA, DdrtMemoryTransactionType, POISON_ENABLE_SET);
    } else {
      mEinjParam->CmdStatus = WHEA_EINJ_CMD_INVALID_ACCESS;
      RAS_DEBUG ((LEVEL_REG, "UnSupported Request.  The Address belongs to PMem.  Only Uncorrected Non Fatal Memory Error type to be selected.\n"));
    }
    MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
    return;
  }

  for (node = 0; node < MC_MAX_NODE; node++) {
    if (IsNodePresent (node) == FALSE) {
      continue;
    }
    //
    // Clear the Mc5 status shadow to track the error incase mcbank status gets cleared.
    //
    SetM2mMciStatusShadowN1 (NODE_TO_SKT (node), NODE_TO_MC (node), 0);
    SetM2mMciStatusShadowN0 (NODE_TO_SKT (node), NODE_TO_MC (node), 0);
  }
  node = SKTMC_TO_NODE (SocketId, MemoryControllerId);
  
  RAS_DEBUG((LEVEL_REG, " InjectMemoryError -Addr = %lx, Node = %x, Ch = %x , PhyRank = %x, \n", PhyAddr, node, TA.ChannelId, TA.PhysicalRankId));

  //
  // Invalidate the line in memory
  //
  mInjAddrValue = *(volatile UINT32 *) (UINTN) (PhyAddr);
  *(volatile  UINT32 *) (UINTN) (PhyAddr) = 0x12345678;
  AsmFlushCacheLine ((VOID*) (UINTN) PhyAddr);

  if (Is10nmFam) {
    AsmWbinvd ();
    Temp1 = *(volatile UINT32 *) (UINTN) (PhyAddr);
  }

  if (mErrorExpected == WHEA_REC_PATROLSCRUB_ERROR) {


    //
    // Disable Patrol scrub
    //
    ClearScrubInterval (SocketId, MemoryControllerId);
    SetScrubCtl (SocketId, MemoryControllerId, SktCh, 0);
    //
    // If the patrol scrub mode is SPA mode, need to move to legacy mode to setup the registers
    // and then move back to SPA mode. And while in the legacy mode, scrubbing has to be enabled once
    // for which Pkg C states should be disbled which is done through RAS_START command and reenabled
    // again using the RAS_EXIT mailbox commands
    //
    if (IsPatrolScrubSaMode (SocketId, MemoryControllerId, SktCh)) {
      RAS_DEBUG((LEVEL_REG, "\n\nWe are in SA mode\n"));
      //
      //Switch to Legacy Mode
      //
      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        RAS_DEBUG((LEVEL_REG, "\nSwitched to Legacy mode\n"));
        SwitchPatrolScrubMode (SocketId, MemoryControllerId, SktCh, TRUE);

        ScrubAddrLo1 = GetScrubAddressLo (SocketId, MemoryControllerId, SktCh);
        RAS_DEBUG((LEVEL_REG, "ScrubAddrLo Data = 0x%x\n", ScrubAddrLo1));

        //
        // Doing the legacy scrub
        //
        RAS_DEBUG((LEVEL_REG, "Enabling Scrub_en command\n"));
        SetScrubEnable (SocketId, MemoryControllerId, SktCh, 1);

        RestoreScrubInterval (SocketId, MemoryControllerId);

        //
        // Check for the scrub address to change as an indicator that scrubbing is working.  If there is no
        // change then delay and retry.
        //
        for (RetryCount = 0; RetryCount < MAX_SCRUB_RETRY_COUNT; RetryCount++) {
          GetScrubCtlForPatrol (SocketId, MemoryControllerId, SktCh, NULL, NULL, &ScrubIssued); // Skt,Mc,ChOnSkt,PtlCmpl,PtlStopped,ScrubIssued
          ScrubAddrLo2 = GetScrubAddressLo (SocketId, MemoryControllerId, SktCh);
          if ((ScrubAddrLo1 != ScrubAddrLo2) && (ScrubIssued == 1)) {
            RAS_DEBUG ((LEVEL_REG, "ScrubAddrLo.Data = 0x%x\n", ScrubAddrLo2));
            RAS_DEBUG ((LEVEL_REG, "ScrubCtl.scrubissued = 0x%x\n", ScrubIssued));
            break;
          }

          NanoSecondDelay (10);
          ScrubAddrLo1 = ScrubAddrLo2;
        }

        //
        // If scrubbing never started, then indicate failed in the command status, lock error
        // injection and return.
        //
        if (RetryCount >= MAX_SCRUB_RETRY_COUNT) {
          mEinjParam->CmdStatus  = WHEA_EINJ_CMD_FAILED;
          RAS_DEBUG((LEVEL_REG, "Scrubbing failed to start.\n"));
          MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
          return;
        }

        //
        // Disable Scrubbing
        //
        ClearScrubInterval (SocketId, MemoryControllerId);
        SetScrubCtl (SocketId, MemoryControllerId, SktCh, 0);
      }

      //
      //For 10nm CPUs scrub_en bit will be set in BIOS.So start the scrubbing 1MB below the intended address.
      //
      PhyAddrC = PhyAddr;
      if (!(IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL))) {
        PhyAddrC = GetPsStartAddress (SocketId, MemoryControllerId, SktCh, PhyAddr);
      }
      RAS_DEBUG((LEVEL_REG, "PhyAddrC (After round up or skip hole) = %lx\n", PhyAddrC));

      //
      // XOR mode support for patrol scrub error injection
      //
      GetMcmtr (SocketId, MemoryControllerId, SktCh, NULL, NULL, &ChXorModeEn, &ClusterXorModeEn);
      RAS_DEBUG((LEVEL_REG, "ChXorModeEn = %x, ClusterXorModeEn = %x, TargetWays = %x, ChannelWays = %x\n", ChXorModeEn, ClusterXorModeEn, TA.TargetWays, TA.ChannelWays));
      PhyAddrD = XorModeAddressCalc (PhyAddrC, ClusterXorModeEn, ChXorModeEn, TA.TargetWays, TA.ChannelWays);
      RAS_DEBUG((LEVEL_REG, "PhyAddrD (after XOR) = %lx\n", PhyAddrD));

      //
      //Program ScrubAddressLo and ScrubAddressHi registers
      //
      RAS_DEBUG((LEVEL_REG, "\nThe Address value to be set (with 6-bit right shift) is: 0x%lx\n", PhyAddrD >> 6));
      SetPatrolScrubAddress (SocketId, MemoryControllerId, SktCh, PhyAddrD);
      SetScrubCtrlTadRule (SocketId, MemoryControllerId, SktCh, FALSE);
      UpdateCurrentTad (SocketId, MemoryControllerId, SktCh, PhyAddrD, FALSE);
      //
      // Setting the start scrub register in scrubctl register
      //
      SetScrubCtlForPatrol (SocketId, MemoryControllerId, SktCh, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff); //Skt, Mc, ChOnSkt, startscrub, stop_on_err, ptl_cmpl, stop_on_cmpl , issueonce, stop_on_rank

      RAS_DEBUG ((LEVEL_REG, "\nThe Scrubctl value after setting the start Scrub is: 0x%lx\n", GetScrubCtl(SocketId, MemoryControllerId, SktCh)));
      RAS_DEBUG ((LEVEL_REG, "ScrubAddrLo Data is after start scrub0x%x\n", GetScrubAddressLo (SocketId, MemoryControllerId, SktCh)));
      //
      // SPA mode switching
      //
      RAS_DEBUG((LEVEL_REG, "\nSPA mode switch\n"));
      SwitchPatrolScrubMode (SocketId, MemoryControllerId, SktCh, TRUE);
      SwitchPatrolScrubMode (SocketId, MemoryControllerId, SktCh, FALSE);
    } else {
      //
      // The Patrol Scrub mode is legacy mode, just setup the address in the low/high registers
      //
      SetPatrolScrubLegacyAddress (SocketId, MemoryControllerId, SktCh, TA.PhysicalRankId,  (UINT32)TA.RankAddress);
    }
    RAS_DEBUG ((LEVEL_REG, "ScrubAddrLo Data: 0x%x\n", GetScrubAddressLo (SocketId, MemoryControllerId, SktCh)));
    ScrubCtl = GetScrubCtl (SocketId, MemoryControllerId, SktCh);
    ScrubCtl |= BIT31;
  }
  //
  // Preparing the Error Injection Table
  //
  if (!(mMESegInjEn)) {
    if (mErrorExpected == WHEA_REC_PATROLSCRUB_ERROR) {
      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        mEinjAction->Trigger0.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
        mEinjAction->Trigger0.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
        mEinjAction->Trigger0.Flags                            = 0;
        mEinjAction->Trigger0.Reserved0                        = 00;
        mEinjAction->Trigger0.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
        mEinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
        mEinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
        mEinjAction->Trigger0.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
        mEinjAction->Trigger0.RegisterRegion.Address           = GetScrubCtlAddress (SocketId,  MemoryControllerId, SktCh);
        mEinjAction->Trigger0.Value                            = ScrubCtl;
        mEinjAction->Trigger0.Mask                             = 0xffffffff;
        RAS_DEBUG((LEVEL_REG, "SocketId = 0x%x, MemoryControllerId= 0x%x\n", SocketId, MemoryControllerId));
        RAS_DEBUG((LEVEL_REG, "mEinjAction->Trigger0.RegisterRegion.Address: 0x%lx\n", mEinjAction->Trigger0.RegisterRegion.Address));
        RAS_DEBUG((LEVEL_REG, "mEinjAction->Trigger0.Value: 0x%x\n", mEinjAction->Trigger0.Value));
      } else {
        SetScrubEnable (SocketId, MemoryControllerId, SktCh, 1);
        RestoreScrubInterval (SocketId, MemoryControllerId);
      }
    }

    if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL) {
      mEinjAction->Trigger0.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger0.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger0.Flags                            = 0;
      mEinjAction->Trigger0.Reserved0                        = 00;
      mEinjAction->Trigger0.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
      mEinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
      mEinjAction->Trigger0.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
      mEinjAction->Trigger0.RegisterRegion.Address           = PhyAddr;
      mEinjAction->Trigger0.Value                            = mInjAddrValue;
      mEinjAction->Trigger0.Mask                             = 0xffffffff;

      mEinjAction->Trigger1.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger1.Instruction                      = EFI_ACPI_6_2_EINJ_READ_REGISTER_VALUE;
      mEinjAction->Trigger1.Flags                            = 0;
      mEinjAction->Trigger1.Reserved0                        = 00;
      mEinjAction->Trigger1.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
      mEinjAction->Trigger1.RegisterRegion.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger1.RegisterRegion.RegisterBitOffset = 0x00;
      mEinjAction->Trigger1.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
      mEinjAction->Trigger1.RegisterRegion.Address           = PhyAddr;
      mEinjAction->Trigger1.Value                            = Read32;
      mEinjAction->Trigger1.Mask                             = 0xffffffff;
    }

    if (mErrorExpected == WHEA_MEM_COR_ERROR) {
      mEinjAction->Trigger0.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger0.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger0.Flags                            = 0;
      mEinjAction->Trigger0.Reserved0                        = 00;
      mEinjAction->Trigger0.RegisterRegion.AddressSpaceId    = EFI_ACPI_6_2_SYSTEM_MEMORY;
      mEinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
      mEinjAction->Trigger0.RegisterRegion.AccessSize        = EFI_ACPI_6_2_DWORD;
      mEinjAction->Trigger0.RegisterRegion.Address           = PhyAddr;
      mEinjAction->Trigger0.Value                            = mInjAddrValue;
      mEinjAction->Trigger0.Mask                             = 0xffffffff;
    }
  }

  //
  // Setup Error Injection registers
  //
  WriteCpuCsr (SocketId, SktCh, RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_REG, 01);

  EnjExtra.Data = ReadCpuCsr (SocketId, SktCh, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG);
  EnjExtra.Bits.crc_err_inj_dev0_5_bits = 01;

  if ((ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL) ||
      (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL)) {
    if (!IsMirrorEanbled (SocketId, MemoryControllerId, SktCh)) {
      mInjAddrValue = INJECTION_ADDRESS_VALUE;
    }
    EnjExtra.Bits.crc_err_inj_dev1_5_bits = 05;
    WriteCpuCsr (SocketId, SktCh, RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG, 04);
  } else {
    WriteCpuCsr (SocketId, SktCh, RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG, 0);
  }
  WriteCpuCsr (SocketId, SktCh, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG, EnjExtra.Data);
  SetErrorInjMaskHi (SocketId, SktCh, 0);

  MaskLoReg.Data = ReadCpuCsr (SocketId, SktCh, RSP_FUNC_ADDR_MASK_LO_MC_MAIN_REG);
  MaskLoReg.Bits.addr_mask_lower  = 07;
  WriteCpuCsr (SocketId, SktCh, RSP_FUNC_ADDR_MASK_LO_MC_MAIN_REG, MaskLoReg.Data);

  //
  // For 10nm, set dis_tgr_inj = 1
  //
  SetDisTgrInj (SocketId, SktCh);

  //
  // Addr Match Lower: 32-bits (Match Addr[34:3])
  //
  MatchLoReg.Data = ReadCpuCsr (SocketId, SktCh, RSP_FUNC_ADDR_MATCH_LO_MC_MAIN_REG);
  MatchLoReg.Bits.addr_match_lower = (UINT32) (PhyAddr >> 03);
  WriteCpuCsr (SocketId, SktCh, RSP_FUNC_ADDR_MATCH_LO_MC_MAIN_REG, MatchLoReg.Data);

  //
  // RSP_FUNC_ADDR_MATCH_EN RSP_FUNC_ADDR_MATCH_EN 11:11 Enabling the Address Match Response Function when set.
  // The enable bit is self cleared after match and the lock is driven from the AND output of EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR) AND
  // MC_ERR_INJ_LCK.MC_ERR_INJ_LCK (MSR) registers.
  // Bits 10:0 Match Addr[45:35]
  //
  SetErrorInjMatchHi (SocketId, SktCh, PhyAddr);
  //
  // Write data to memory to seed error in memeory
  //
  SeeErrorAp (&SocketId);

  if (mMESegInjEn) {
    //
    // Need to do post-setup after error injection in ME seg
    // set MESEG_LIMIT.EN = 1 and MC9[Bit5] = 1
    //
    PostMESegErrInjSetup ();

    //
    // call ME EINJ_CONSUME command
    //
    InitiateUmaErrorConsume ();
  }

  //
  // Lock the error injection mechanism
  //
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
}

