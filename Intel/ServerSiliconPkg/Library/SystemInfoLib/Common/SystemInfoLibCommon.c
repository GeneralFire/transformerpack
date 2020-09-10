/** @file SystemInfoLib.c

  Implementation of SystemInfoLib.  These functions are common across phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/HbmCoreLib.h>

/**
  This API gets Cpu Csr Access var info.

  @param  None

  @retval CPU_CSR_ACCESS_VAR*  The Pointer for system info returned.
**/
CPU_CSR_ACCESS_VAR *
GetSysCpuCsrAccessVar (
  VOID
  )
{
  return &(GetSystemInfoVar()->CpuCsrAccessVarHost);
}

/**
  This API gets reset required info.

  @param  None

  @retval UINT8  The data for reset requirted returned.
**/
UINT8
EFIAPI
GetSysResetRequired (
  VOID
  )
{
  return GetSystemInfoVar()->ResetRequired;
}

/**
  This API sets the value for reset required.

  @param[in] UINT8  Value to set for reset required.

  @retval None
**/
VOID
EFIAPI
SetSysResetRequired (
  IN UINT8  ResetRequired
  )
{
  GetSystemInfoVar()->ResetRequired = ResetRequired;
  return;
}

/**
  This API gets the value for emulation type.

  @param None

  @retval UINT8  The data for emulation type returned.
**/
UINT8
EFIAPI
GetEmulation (
  VOID
  )
{
  return GetSystemInfoVar()->Emulation;
}

/**
  This API sets the value for emulation type.

  @param[in] UINT8  Value to set for emulation type.

  @retval None
**/
VOID
EFIAPI
SetEmulation (
  IN UINT8  Emulation
  )
{
  GetSystemInfoVar()->Emulation = Emulation;
  return;
}

/**
  This API gets the value for Me requested size.

  @param None

  @retval UINT32  The data for Me requested size returned.
**/
UINT32
EFIAPI
GetMeRequestedSize (
  VOID
  )
{
  return GetSystemInfoVar()->MeRequestedSize;
}

/**
  This API sets the value for Me requested size.

  @param[in] UINT32  Value to set for Me requested size.

  @retval None
**/
VOID
EFIAPI
SetMeRequestedSize (
  IN UINT32  MeReqeustedSize
  )
{
  GetSystemInfoVar()->MeRequestedSize = MeReqeustedSize;
  return;
}

/**
  This API gets the value for Me requested alignment.

  @param None

  @retval UINT32  The data for Me requested alignment returned.
**/
UINT32
EFIAPI
GetMeRequestedAlignment (
  VOID
  )
{
  return GetSystemInfoVar()->MeRequestedAlignment;
}

/**
  This API sets the value for Me requested alignment.

  @param[in] UINT32  Value to set for Me requested alignment.

  @retval None
**/
VOID
EFIAPI
SetMeRequestedAlignment (
  IN UINT32  MeRequestedAlignment
  )
{
  GetSystemInfoVar()->MeRequestedAlignment = MeRequestedAlignment;
  return;
}

/**
  This API gets the value for Ie requested size.

  @param None

  @retval UINT32  The data for Ie requested size returned.
**/
UINT32
EFIAPI
GetIeRequestedSize (
  VOID
  )
{
  return GetSystemInfoVar()->IeRequestedSize;
}

/**
  This API sets the value for Ie requested size.

  @param[in] UINT32  Value to set for Ie requested size.

  @retval None
**/
VOID
EFIAPI
SetIeRequestedSize (
  IN UINT32  IeReqeustedSize
  )
{
  GetSystemInfoVar()->IeRequestedSize = IeReqeustedSize;
  return;
}

/**
  This API gets the value for Ie requested alignment.

  @param None

  @retval UINT32  The data for Ie requested alignment returned.
**/
UINT32
EFIAPI
GetIeRequestedAlignment (
  VOID
  )
{
  return GetSystemInfoVar()->IeRequestedAlignment;
}

/**
  This API sets the value for Ie requested alignment.

  @param[in] UINT32  Value to set for Ie requested alignment.

  @retval None
**/
VOID
EFIAPI
SetIeRequestedAlignment (
  IN UINT32  IeRequestedAlignment
  )
{
  GetSystemInfoVar()->IeRequestedAlignment = IeRequestedAlignment;
  return;
}

/**
  This API gets the value for check point.

  @param None

  @retval UINT32  The data for check point returned.
**/
UINT32
EFIAPI
GetCheckPoint (
  VOID
  )
{
  return GetSystemInfoVar()->CheckPoint;
}

/**
  This API sets the value for check point.

  @param[in] UINT32  Value to set for check point.

  @retval None
**/
VOID
EFIAPI
SetCheckPoint (
  IN UINT32  CheckPoint
  )
{
  GetSystemInfoVar()->CheckPoint = CheckPoint;
  return;
}

/**
  This API gets the value for boot mode.

  @param None

  @retval BootMode  The data for boot mode returned.
**/
BootMode
EFIAPI
GetSysBootMode (
  VOID
  )
{
  return GetSystemInfoVar()->SysBootMode;
}

/**
  This API sets the value for boot mode.

  @param[in] BootMode  Value to set for boot mode.

  @retval None
**/
VOID
EFIAPI
SetSysBootMode (
  IN BootMode  SysBootMode
  )
{
  GetSystemInfoVar()->SysBootMode = SysBootMode;
  return;
}

/**
  This API gets the value for cpu frequency.

  @param None

  @retval UINT64  The data for cpu frequency returned.
**/
UINT64
EFIAPI
GetCpuFreq (
  VOID
  )
{
  return GetSystemInfoVar()->CpuFreq;
}

/**
  This API sets the value for cpu frequency.

  @param[in] UINT64  Value to set for cpu frequency.

  @retval None
**/
VOID
EFIAPI
SetCpuFreq (
  IN UINT64  CpuFreq
  )
{
  GetSystemInfoVar()->CpuFreq = CpuFreq;
  return;
}

/**
  This API gets the value for cpu Socket Id.

  @param None

  @retval UINT8  The data for cpu Socket Id.
**/
UINT8
EFIAPI
GetSysSocketId (
  VOID
  )
{
  return GetSystemInfoVar()->SocketId;
}

/**
  This API sets the value for SocketId.

  @param[in] UINT8  Value to set for SocketId.

  @retval None
**/
VOID
EFIAPI
SetSysSocketId (
  IN UINT8  SocketId
  )
{
  GetSystemInfoVar()->SocketId = SocketId;
  return;
}

/**
  This API gets the pointer to SYS_INFO_VAR_NVRAM.

  @param None

  @retval SYS_INFO_VAR_NVRAM  The pointer to SYS_INFO_VAR_NVRAM.
**/
SYS_INFO_VAR_NVRAM *
EFIAPI
GetSysInfoVarNvramPtr (
  VOID
  )
{
  return &(GetSystemInfoVar ()->SysInfoVarNvram);
}

/**
  This API gets the value for MeRequestedSizeNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for MeRequestedSizeNv in SysInfoVarNvram.
**/
UINT32
EFIAPI
GetSysMeRequestedSizeNv (
  VOID
  )
{
  return GetSystemInfoVar ()->SysInfoVarNvram.MeRequestedSizeNv;
}

/**
  This API sets the value for MeRequestedSizeNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for MeRequestedSizeNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysMeRequestedSizeNv (
  IN UINT32  MeRequestedSizeNv
  )
{
  GetSystemInfoVar ()->SysInfoVarNvram.MeRequestedSizeNv = MeRequestedSizeNv;
  return;
}

/**
  This API gets the value for MeRequestedAlignmentNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for MeRequestedAlignmentNv in SysInfoVarNvram.**/
UINT32
EFIAPI
GetSysMeRequestedAlignmentNv (
  VOID
  )
{
  return GetSystemInfoVar ()->SysInfoVarNvram.MeRequestedAlignmentNv;
}

/**
  This API sets the value for MeRequestedAlignmentNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for MeRequestedAlignmentNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysMeRequestedAlignmentNv (
  IN UINT32  MeSysRequestedAlignmentNv
  )
{
  GetSystemInfoVar ()->SysInfoVarNvram.MeRequestedAlignmentNv = MeSysRequestedAlignmentNv;
  return;
}

/**
  This API gets the value for IeRequestedSizeNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for IeRequestedSizeNv in SysInfoVarNvram.
**/
UINT32
EFIAPI
GetSysIeRequestedSizeNv (
  VOID
  )
{
  return GetSystemInfoVar ()->SysInfoVarNvram.IeRequestedSizeNv;
}

/**
  This API sets the value for IeRequestedSizeNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for IeRequestedSizeNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysIeRequestedSizeNv (
  IN UINT32  IeRequestedSizeNv
  )
{
  GetSystemInfoVar ()->SysInfoVarNvram.IeRequestedSizeNv = IeRequestedSizeNv;
  return;
}

/**
  This API gets the value for IeRequestedAlignmentNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for IeRequestedAlignmentNv in SysInfoVarNvram.**/
UINT32
EFIAPI
GetSysIeRequestedAlignmentNv (
  VOID
  )
{
  return GetSystemInfoVar ()->SysInfoVarNvram.IeRequestedAlignmentNv;
}

/**
  This API sets the value for IeRequestedAlignmentNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for IeRequestedAlignmentNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysIeRequestedAlignmentNv (
  IN UINT32  IeSysRequestedAlignmentNv
  )
{
  GetSystemInfoVar ()->SysInfoVarNvram.IeRequestedAlignmentNv = IeSysRequestedAlignmentNv;
  return;
}

/**
  This API gets the value for SbspSocketIdNv in SysInfoVarNvram.

  @param None

  @retval UINT8  The data for SbspSocketIdNv in SysInfoVarNvram.
**/
UINT8
EFIAPI
GetSysSbspSocketIdNv (
  VOID
  )
{
  return GetSystemInfoVar ()->SysInfoVarNvram.SbspSocketIdNv;
}

/**
  This API sets the value for SbspSocketIdNv in SysInfoVarNvram.

  @param[in] UINT8  Value to set for SbspSocketIdNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysSbspSocketIdNv (
  IN UINT8  SbspSocketIdNv
  )
{
  GetSystemInfoVar ()->SysInfoVarNvram.SbspSocketIdNv = SbspSocketIdNv;
  return;
}

/**

  Check emulation type
  @param[in] EmuType  - Emulation Type to be checked

  @retval    TRUE       Emulation Type is same as input
  @retval    FALSE      Emulation Type is not same as input

**/
BOOLEAN
EFIAPI
CheckEmulationType (
  IN UINT8     EmuType
  )
{
  return ((GetEmulation () & EmuType) != 0) ? TRUE : FALSE;
}

/**

  Set emulation type
  @param[in] EmuType  - Emulation Type to be set

**/
VOID
SetEmulationType (
  IN UINT8     EmuType
  )
{
  SetEmulation (EmuType);
}

/**

  This API Starts USRA trace.

  @param None

  @retval None

**/
VOID
EFIAPI
UsraTraceStart (
  VOID
  )
{
  GetSystemInfoVar ()->UsraTraceControl = TRUE;
}

/**

  This API ends USRA trace.

  @param None

  @retval None

**/
VOID
EFIAPI
UsraTraceEnd (
  VOID
  )
{
  GetSystemInfoVar ()->UsraTraceControl = FALSE;
}

/**

  This API gets the configuration of USRA trace.

  @retval  - USRA trace configuration.

**/
UINT16
EFIAPI
GetUsraTraceConfiguration (
  VOID
  )
{
  return GetSystemInfoVar ()->UsraTraceConfiguration;
}

/**

  This API sets the configuration of USRA trace.

  @param[in] UsraTraceConfiguration  - USRA configuration to be set.

  @retval None

**/
VOID
EFIAPI
SetUsraTraceConfiguration (
  IN  UINT16     UsraTraceConfiguration
  )
{
  GetSystemInfoVar ()->UsraTraceConfiguration = UsraTraceConfiguration;
}

/**

  This API checks the status of USRA trace.

  @param None

  @retval True                    USRA Trace is working.
  @retval False                   USRA Trace is not working.

**/
BOOLEAN
EFIAPI
CheckUsraTraceContolFlag (
  VOID
  )
{
  return GetSystemInfoVar ()->UsraTraceControl;
}

/**

  This API will return whether the CpuCsrAccessVar info has been dumped.

  @param None

  @retval True                    CpuCsrAccessVar info has been dumped.
  @retval False                   CpuCsrAccessVar info has not been dumped.

**/
BOOLEAN
EFIAPI
CheckIfCpuCsrAccessVarInfoDumped (
  VOID
  )
{
  return GetSystemInfoVar ()->CpuCsrAccessVarInfoDumped;
}

/**

  This API disables the CpuCsrAccessVar dump info.

  @param  None

  @retval None

**/
VOID
EFIAPI
DisableCpuCsrAccessVarInfoDump (
  VOID
  )
{
  GetSystemInfoVar ()->CpuCsrAccessVarInfoDumped = TRUE;
}

/**
  This API gets the UPI connection node type for NodeId.

  @param[in] NodeId - Node ID

  @retval UINT8  The UPI connection node type for NodeId.
**/
UPI_CONNECTION_TYPE
EFIAPI
GetUpiConnectionType (
  IN UINT8  NodeId
  )
{
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  return CpuCsrAccessVar->UpiConnectionType[NodeId];
}

/**
  This API sets the UPI connection node type for NodeId.

  @param[in] NodeId            - Node ID
  @param[in] UpiConnectionType - UPI connection node type

  @retval None
**/
VOID
EFIAPI
SetUpiConnectionType (
  IN UINT8                NodeId,
  IN UPI_CONNECTION_TYPE  UpiConnectionType
  )
{
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  CpuCsrAccessVar->UpiConnectionType[NodeId] = UpiConnectionType;
}

/**
  Return the current state of the posted write CSR access method

  @param[out] CpuCsrAccessVarPtr - Pointer to CSR access variable
                                   If NULL, this function will locate the variable

  @retval POSTED_METHOD_ENABLED if posted writes are enabled
  @retval POSTED_METHOD_DISABLED otheriwse
**/
UINT8
EFIAPI
GetPostedMethodState (
  IN  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr
  )
{
  if (CpuCsrAccessVarPtr == NULL) {
    CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  }

  if (CpuCsrAccessVarPtr->PostedCsrAccessAllowed && CpuCsrAccessVarPtr->PostedWritesEnabled) {
    return POSTED_METHOD_ENABLED;
  } else {
    return POSTED_METHOD_DISABLED;
  }
}

/**
  This API gets PCIE segment number based on the socket number.

  @param  [in]  Socket             - Socket Id

  @retval UINT8  The PCIE Segment number returned.
**/
UINT8
EFIAPI
GetPcieSegment (
  UINT8 Socket
  )
{
  return GetSysCpuCsrAccessVar()->segmentSocket[Socket];
}

/**
  Get number of channel per memory controller

  @return the number of channel per memory controller

**/
UINT8
EFIAPI
GetNumChannelPerMc (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->DdrNumChPerMc > 0) && (CpuCsrAccessVar->DdrNumChPerMc <= MAX_MC_CH));

  return CpuCsrAccessVar->DdrNumChPerMc;
}

/**
  Get maximum number of IMC

  @return the maximum number of iMC

**/
UINT8
EFIAPI
GetMaxImc (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  if (CpuCsrAccessVar->DdrMaxImc > MAX_IMC){
    ASSERT(FALSE);
    return MAX_IMC;
  }

  return CpuCsrAccessVar->DdrMaxImc;
}

/**
  Get maximum DDR channel number of socket

  @return the maximum DDR number of socket

**/
UINT8
EFIAPI
GetMaxChDdr (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->DdrMaxCh > 0) && (CpuCsrAccessVar->DdrMaxCh <= MAX_CH));

  return CpuCsrAccessVar->DdrMaxCh;
}

/**
  Get number of channel per memory controller

  @return the number of channel per memory controller

**/
UINT8
EFIAPI
GetNumChannelPerMcHbm (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->HbmNumChPerMc > 0) && (CpuCsrAccessVar->HbmNumChPerMc <= MAX_HBM_MC_CH));

  return CpuCsrAccessVar->HbmNumChPerMc;
}

/**
  Get maximum number of HBM channel per HBM IO

  @retval maximum number of HBM channel per HBM IO

**/
UINT8
EFIAPI
GetNumChannelPerIoHbm (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->HbmNumChPerIo > 0) && (CpuCsrAccessVar->HbmNumChPerIo <= MAX_HBM_IO_CH));

  return CpuCsrAccessVar->HbmNumChPerIo;
}

/**
  Get maximum number of IMC per HBM IO

  @return the maximum number of iMC

**/
UINT8
EFIAPI
GetMaxImcPerIoHbm (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->HbmNumChPerIo > 0) && (CpuCsrAccessVar->HbmNumChPerIo <= MAX_HBM_IO_CH));
  ASSERT ((CpuCsrAccessVar->HbmNumChPerMc > 0) && (CpuCsrAccessVar->HbmNumChPerMc <= MAX_HBM_MC_CH));

  return (UINT8) (CpuCsrAccessVar->HbmNumChPerIo / CpuCsrAccessVar->HbmNumChPerMc);
}

/**
  Get maximum channel number of socket

  @return the maximum number of socket

**/
UINT8
EFIAPI
GetMaxChHbm (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  ASSERT ((CpuCsrAccessVar->HbmMaxCh > 0) && (CpuCsrAccessVar->HbmMaxCh <= MAX_HBM_CH));

  return CpuCsrAccessVar->HbmMaxCh;
}

/**
  Get maximum hbm io instance number of socket

  @return the maximum hbm io instance number of socket

**/
UINT8
EFIAPI
GetMaxIoHbm (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  if (CpuCsrAccessVar->HbmSku) {
    ASSERT ((CpuCsrAccessVar->HbmMaxIoInst > 0) && (CpuCsrAccessVar->HbmMaxIoInst <= MAX_HBM_IO));

    return CpuCsrAccessVar->HbmMaxIoInst;
  } else {
    return 0;
  }
}

/**

  Indicate if HVM Mode is enabled


  @retval   TRUE      HVM Mode is enabled
  @retval   FALSE     HVM Mode is disabled

**/
BOOLEAN
EFIAPI
IsHvmModeEnable (
  VOID
  )
{
  return PcdGetBool (PcdHvmModeEnabled);
} // IsHvmModeEnable
