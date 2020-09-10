/** @file
  Configure .

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include <Uefi.h>
#include "Include/UboxRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <KtiSetupDefinitions.h>
#include <Guid/UboxIpInterface.h>

/**
  UBOX: Programm CPU Node ID.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param LocalSocId           - Node Id of the local Socket, 0xFFFFFFFF: no need to prgramm
  @param LockNodeId           - NodeId of the lock master, 0xFFFFFFFF: no need to prgramm
  @param LegacyNodeId         - NodeID of the legacy socket, 0xFFFFFFFF: no need to prgramm

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuNodeId (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 LocalSocId,
  IN UINT32                 LockNodeId,
  IN UINT32                 LegacyNodeId
  )
{
  CPUNODEID_UBOX_CFG_STRUCT     UboxCpuNodeId;

  UboxCpuNodeId.Data = UsraCsrRead (SocId, BoxInst, CPUNODEID_UBOX_CFG_REG);

  if (LocalSocId != 0xFFFFFFFF) {
    UboxCpuNodeId.Bits.lclnodeid = LocalSocId;
  }
  if (LockNodeId != 0xFFFFFFFF) {
    UboxCpuNodeId.Bits.locknodeid = LockNodeId;
  }
  if (LegacyNodeId != 0xFFFFFFFF) {
    UboxCpuNodeId.Bits.lgcnodeid = LegacyNodeId;
  }

  UsraCsrWrite (SocId, BoxInst, CPUNODEID_UBOX_CFG_REG, UboxCpuNodeId.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Programm CPU Node Local ID.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param LocalSocId           - Node Id of the local Socket
  @param RegMask              - Specify programm bits

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuLocalId (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  LocalSocId,
  IN UINT32                 RegMask
  )
{
  UINT32   Data32;

  Data32 = UsraCsrRead (SocId, BoxInst, CPUNODEID_UBOX_CFG_REG);
  Data32 = (Data32 & RegMask)  | LocalSocId;
  UsraCsrWrite (SocId, BoxInst, CPUNODEID_UBOX_CFG_REG, Data32);

  return EFI_SUCCESS;
}

/**
  UBOX: Programm group ID and NodeId mapping.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param GidNidMapping        - Programm group ID and NodeId mapping

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureGidNidMapping (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 GidNidMapping
  )
{
  UsraCsrWrite (SocId, BoxInst, GIDNIDMAP_UBOX_CFG_REG, GidNidMapping);

  return EFI_SUCCESS;
}

/**
  UBOX: Programm CPU Node ID to indicate which CPUs are enabled.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param CpuList              - Bit mask to indicate which node_id is enabled

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuEnabled (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 CpuList
  )
{
  CPUENABLE_UBOX_CFG_STRUCT   CpuEnableUboxCfg;

  CpuEnableUboxCfg.Data = 0;
  CpuEnableUboxCfg.Bits.valid = 1;
  CpuEnableUboxCfg.Bits.nodeiden = CpuList; // Set the list of CPUs in the system and enable the register

  UsraCsrWrite (SocId, BoxInst, CPUENABLE_UBOX_CFG_REG, CpuEnableUboxCfg.Data);

  return EFI_SUCCESS;
}


/**
  UBOX: Programm CPU broadcast list.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IntList              - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for interrupts and PMReq in an
                                hot-add capable system
  @param StopStartList        - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for StopReq/StartReq in a hot-add
                                capable system
  @param MiscList             - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for WbInvdAck/InvdAck/EOI in a
                                hot-add capable system
  @param IntPrioList          - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for IntPrioUpd in a hot-add
                                capable system
  @parm  BcastListRtn         - BCASTList

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuBroadcastList (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT32                 IntList,
  IN  UINT32                 StopStartList,
  IN  UINT32                 MiscList,
  IN  UINT32                 IntPrioList,
  OUT UINT32*                BcastListRtn
  )
{
  BCASTLIST_UBOX_CFG_STRUCT    BcastList;
  UINT32                       Data32;

  Data32 = BcastList.Data = UsraCsrRead (SocId, BoxInst, BCASTLIST_UBOX_CFG_REG);

  if (IntList != 0xFFFFFFFF) {
    BcastList.Bits.intlist = IntList;
  }
  if (StopStartList != 0xFFFFFFFF) {
    BcastList.Bits.stopstartlist = StopStartList;
  }
  if (MiscList != 0xFFFFFFFF) {
    BcastList.Bits.misclist = MiscList;
  }
  if (IntPrioList != 0xFFFFFFFF) {
    BcastList.Bits.intpriolist = IntPrioList;
  }

  UsraCsrWrite (SocId, BoxInst, BCASTLIST_UBOX_CFG_REG, BcastList.Data);

  *BcastListRtn = BcastList.Data;

  return EFI_SUCCESS;
}

/**
  UBOX: Configure IPI target list.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Type                 - Programm type: 0: clear specified target
                                1: set specified target
                                2: set all target list
  @param PendingSocId         - Pending Soc ID
  @param BcastListRtn         - return value of register BCASTLIST_UBOX_CFG_REG

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuIpiTargetList (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT8                  Type,
  IN  UINT8                  PendingSocId,
  OUT UINT32*                BcastListRtn
  )
{
  BCASTLIST_UBOX_CFG_STRUCT    BcastList;
  UINT32                       Data32;

  Data32 = BcastList.Data = UsraCsrRead (SocId, BoxInst, BCASTLIST_UBOX_CFG_REG);

  if (Type == 0) {
    BcastList.Data &= (~(1 << PendingSocId));
  } else if (Type == 1) {
    BcastList.Data |= (1 << PendingSocId);
  }

  UsraCsrWrite (SocId, BoxInst, BCASTLIST_UBOX_CFG_REG, BcastList.Data);

  *BcastListRtn = Data32;

  return EFI_SUCCESS;
}

/**
  UBOX: Programm Lock Control Register.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SplitLockEnable      - 1: Enable; 0: Disable

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureLockControlRegister (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SplitLockEnable
  )
{
  LOCKCONTROL_UBOX_CFG_STRUCT                LockControl;

  //
  // Lock Control, Enable Lock
  //
  LockControl.Data = UsraCsrRead (SocId, BoxInst, LOCKCONTROL_UBOX_CFG_REG);
  LockControl.Bits.lockdisable = 0;
  if (SplitLockEnable ==  KTI_ENABLE) {
    //
    // Set splitlockopt and skippculk
    //
    LockControl.Bits.splitlockopt = 1;
    LockControl.Bits.skippculk = 1;
  }
  UsraCsrWrite (SocId, BoxInst, LOCKCONTROL_UBOX_CFG_REG, LockControl.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Ubox per-error enable mask. If masked, a given Ubox error will not be signaled via SMI and will not be logged. .

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfullye

**/
EFI_STATUS
EFIAPI
UBoxConfigureUboxErrorCtrl (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst
  )
{
  UBOXERRCTL_UBOX_CFG_STRUCT                 UboxErrCtl;

  //
  // s4031310 Ubot timeout To set timeout duration 5 sec at 1.2GHz.
  //              NM  = (T0 * f )/NB     = (5.0 * 1.2E+9)/0xFFFFFF = 357.627889969 ~ 0x166[9:0]
  //              The count match register field specifies [9:4] of NM [9:0]
  //              0x166>>4 = 0x16[9:4]  ' 0x17[9:4] masking sure above 5 sec.
  //
  UboxErrCtl.Data = UsraCsrRead (SocId, BoxInst, UBOXERRCTL_UBOX_CFG_REG);
  UboxErrCtl.Bits.smitimeoutcountmatch = 0x17;
  UboxErrCtl.Bits.masterlocktimeoutcountmatch = 0x17;
  UsraCsrWrite (SocId, BoxInst, UBOXERRCTL_UBOX_CFG_REG, UboxErrCtl.Data);

  return EFI_SUCCESS;
}

/**

  UboxGetSetSaiRegisters.

  @param SocId         - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param SaiRegIndex   - SaiRegIndex
  @param Get           - TRUE: read register into *Value, FALSE: write register using data in *Value
  @param *Value        - *Value: pointer of UINT32, can be input & output

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
UBoxGetSetSaiRegisters (
  IN UINT8                   SocId,
  IN UINT8                   BoxInst,
  IN UBOX_SAI_REG_ENUM       SaiRegIndex,
  IN BOOLEAN                 Get,
  IN OUT UINT32              *Value
  )
{
  EFI_STATUS  Status;
  UINT32 RegAddress;

  Status      = EFI_SUCCESS;

  switch (SaiRegIndex) {
    case UboxLbWMcheckRacN0RegIndex:
      RegAddress = LB_MCHECK_W_RAC_N0_UBOX_CFG_REG;
      break;

    case UboxLbWMcheckWacN0RegIndex:
      RegAddress = LB_MCHECK_W_WAC_N0_UBOX_CFG_REG;
      break;

    case UboxLbWMcheckWacN1RegIndex:
      RegAddress = LB_MCHECK_W_WAC_N1_UBOX_CFG_REG;
      break;

    case UboxLbWMcheckCpN0RegIndex:
      RegAddress = LB_MCHECK_W_CP_N0_UBOX_CFG_REG;

      break;
    case UboxLbWMcheckCpN1RegIndex:
      RegAddress = LB_MCHECK_W_CP_N1_UBOX_CFG_REG;
      break;

    default:
      RegAddress = 0;
      break;
  }

  if (!RegAddress){
    //
    // Error returning
    //
    return EFI_INVALID_PARAMETER;
  }

  if (Get){
    *Value = UsraCsrRead (SocId, BoxInst, RegAddress);
  } else {
    UsraCsrWrite (SocId, BoxInst, RegAddress, *Value);
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Platform QoS Memory BW Enforcement Delay & Time Window setting

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfullye

**/
EFI_STATUS
EFIAPI
UBoxSetMemoryBWEnforcement (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst
  )
{
  UINT8   Index;
  UINT32  MbeDelaySettingDefault[3] = {0x0D0B0A09, 0x1D16120f, 0x0000552C};
  UINT32  MbeTimeWindowDefault[3]   = {0x0E0B0A09, 0x1e171310, 0x0000592d};
  UINT32  MbeDelaySettingSnr[3]     = {0x0A060401, 0x24140E0C, 0x0000B458};
  UINT32  MbeTimeWindowSnr[3]       = {0x18202001, 0x80806020, 0x0000FFFF};
  UINT32  UbxMbeDelayCsr[3]  = {NCEVENTS_CFG_MBE_DELAY_0_N0_UBOX_CFG_REG,
                                NCEVENTS_CFG_MBE_DELAY_0_N1_UBOX_CFG_REG,
                                NCEVENTS_CFG_MBE_DELAY_1_N0_UBOX_CFG_REG};
  UINT32  UbxMbeTimeWindowCsr[3] = {NCEVENTS_CFG_MBE_TIMEWINDOW_0_N0_UBOX_CFG_REG,
                                    NCEVENTS_CFG_MBE_TIMEWINDOW_0_N1_UBOX_CFG_REG,
                                    NCEVENTS_CFG_MBE_TIMEWINDOW_1_N0_UBOX_CFG_REG};
  //
  // This is basic tuning value for MBE
  //
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    for (Index = 0; Index < 3; Index++) {
      UsraCsrWrite (SocId, BoxInst, UbxMbeDelayCsr[Index], MbeDelaySettingSnr[Index]);
      UsraCsrWrite (SocId, BoxInst, UbxMbeTimeWindowCsr[Index], MbeTimeWindowSnr[Index]);
    }
  } else {
    for (Index = 0; Index < 3; Index++) {
      UsraCsrWrite (SocId, BoxInst, UbxMbeDelayCsr[Index], MbeDelaySettingDefault[Index]);
      UsraCsrWrite (SocId, BoxInst, UbxMbeTimeWindowCsr[Index], MbeTimeWindowDefault[Index]);
    }
  }

  return EFI_SUCCESS;
}