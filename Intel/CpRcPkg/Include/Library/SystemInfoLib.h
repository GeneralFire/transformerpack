/** @file
  System Info Library Header File

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

#ifndef __SYSTEM_INFO_LIB_H__
#define __SYSTEM_INFO_LIB_H__

#include "SystemInfoVar.h"
#include <Uefi.h>

/**
  This function returns the pointer to SYSTEM_INFO_VAR.

  @param VOID

  @retval SYSTEM_INFO_VAR*   The pointer to SYSTEM_INFO_VAR.
**/
SYSTEM_INFO_VAR *
GetSystemInfoVar (
  VOID
  );

/**
  This API gets Cpu Csr Access var info.

  @param  None

  @retval CPU_CSR_ACCESS_VAR *   The Pointer for system info returned.
**/
CPU_CSR_ACCESS_VAR *
GetSysCpuCsrAccessVar (
  VOID
  );

//
// Reset types needed post execution
//
#define POST_RESET_NO_RESET       0x0
#define POST_RESET_CPU_ONLY       0x1    // bit0
#define POST_RESET_WARM           0x2    // bit1
#define POST_RESET_POWERGOOD      0x4    // bit2
#define POST_RESET_AMI            0x8    // bit3

/**
  This API gets reset required info.

  @param  None

  @retval UINT8  The data for reset requirted returned.
**/
UINT8
EFIAPI
GetSysResetRequired (
  VOID
  );

/**
  This API sets the value for reset required.

  @param[in] UINT8  Value to set for reset required.

  @retval None
**/
VOID
EFIAPI
SetSysResetRequired (
  IN UINT8  ResetRequired
  );

//
// Bit definitions used with GetEmulation and SetEmulation
//

#define SOFT_SDV_FLAG                     BIT0      // flag to indicate running on Soft SDV
#define VP_FLAG                           BIT1      // flag to indicate running on VP
#define SIMICS_FLAG                       BIT2      // flag to indicate running on Simics
#define RTL_SIM_FLAG                      BIT3      // flag to indicate running on the RTL simulator

/**
  This API gets the value for emulation type.

  @param None

  @retval UINT8  The data for emulation type returned.
**/
UINT8
EFIAPI
GetEmulation (
  VOID
  );

/**
  This API sets the value for emulation type.

  @param[in] UINT8  Value to set for emulation type.

  @retval None
**/
VOID
EFIAPI
SetEmulation (
  IN UINT8  Emulation
  );

/**
  This API gets the value for Me requested size.

  @param None

  @retval UINT32  The data for Me requested size returned.
**/
UINT32
EFIAPI
GetMeRequestedSize (
  VOID
  );

/**
  This API sets the value for Me requested size.

  @param[in] UINT32  Value to set for Me requested size.

  @retval NOne
**/
VOID
EFIAPI
SetMeRequestedSize (
  IN UINT32  MeRequestedSize
  );

/**
  This API gets the value for Me requested alignment.

  @param None

  @retval UINT32  The data for Me requested alignment returned.
**/
UINT32
EFIAPI
GetMeRequestedAlignment (
  VOID
  );

/**
  This API sets the value for Me requested alignment.

  @param[in] UINT32  Value to set for Me requested alignment.

  @retval None
**/
VOID
EFIAPI
SetMeRequestedAlignment (
  IN UINT32  MeRequestedAlignment
  );

/**
  This API gets the value for Ie requested size.

  @param None

  @retval UINT32  The data for Ie requested size returned.
**/
UINT32
EFIAPI
GetIeRequestedSize (
  VOID
  );

/**
  This API sets the value for Ie requested size.

  @param[in] UINT32  Value to set for Me requested size.

  @retval NOne
**/
VOID
EFIAPI
SetIeRequestedSize (
  IN UINT32  MeRequestedSize
  );

/**
  This API gets the value for Ie requested alignment.

  @param None

  @retval UINT32  The data for Ie requested alignment returned.
**/
UINT32
EFIAPI
GetIeRequestedAlignment (
  VOID
  );

/**
  This API sets the value for Ie requested alignment.

  @param[in] UINT32  Value to set for Ie requested alignment.

  @retval None
**/
VOID
EFIAPI
SetIeRequestedAlignment (
  IN UINT32  IeRequestedAlignment
  );

/**
  This API gets the value for check point.

  @param None

  @retval UINT32  The data for check point returned.
**/
UINT32
EFIAPI
GetCheckPoint (
  VOID
  );

/**
  This API sets the value for check point.

  @param[in] UINT32  Value to set for check point.

  @retval None
**/
VOID
EFIAPI
SetCheckPoint (
  IN UINT32  CheckPoint
  );

/**
  This API gets the pointer to SYS_INFO_VAR_NVRAM.

  @param None

  @retval SYS_INFO_VAR_NVRAM  The pointer to SYS_INFO_VAR_NVRAM.
**/
SYS_INFO_VAR_NVRAM *
EFIAPI
GetSysInfoVarNvramPtr (
  VOID
  );

/**
  This API gets the value for MeRequestedSizeNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for MeRequestedSizeNv in SysInfoVarNvram.
**/
UINT32
EFIAPI
GetSysMeRequestedSizeNv (
  VOID
  );

/**
  This API sets the value for MeRequestedSizeNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for MeRequestedSizeNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysMeRequestedSizeNv (
  IN UINT32  MeRequestedSizeNv
  );

/**
  This API gets the value for IeRequestedAlignmentNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for IeRequestedAlignmentNv in SysInfoVarNvram.**/
UINT32
EFIAPI
GetSysIeRequestedAlignmentNv (
  VOID
  );

/**
  This API sets the value for IeRequestedAlignmentNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for IeRequestedAlignmentNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysIeRequestedAlignmentNv (
  IN UINT32  IeSysRequestedAlignmentNv
  );

/**
  This API gets the value for IeRequestedSizeNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for IeRequestedSizeNv in SysInfoVarNvram.
**/
UINT32
EFIAPI
GetSysIeRequestedSizeNv (
  VOID
  );

/**
  This API sets the value for IeRequestedSizeNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for IeRequestedSizeNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysIeRequestedSizeNv (
  IN UINT32  MeRequestedSizeNv
  );

/**
  This API gets the value for MeRequestedAlignmentNv in SysInfoVarNvram.

  @param None

  @retval UINT32  The data for MeRequestedAlignmentNv in SysInfoVarNvram.**/
UINT32
EFIAPI
GetSysMeRequestedAlignmentNv (
  VOID
  );

/**
  This API sets the value for MeRequestedAlignmentNv in SysInfoVarNvram.

  @param[in] SYS_INFO_VAR_NVRAM  Value to set for MeRequestedAlignmentNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysMeRequestedAlignmentNv (
  IN UINT32  MeSysRequestedAlignmentNv
  );

/**
  This API gets the value for SbspSocketIdNv in SysInfoVarNvram.

  @param None

  @retval UINT8  The data for SbspSocketIdNv in SysInfoVarNvram.
**/
UINT8
EFIAPI
GetSysSbspSocketIdNv (
  VOID
  );

/**
  This API sets the value for SbspSocketIdNv in SysInfoVarNvram.

  @param[in] UINT8  Value to set for SbspSocketIdNv in SysInfoVarNvram.

  @retval None
**/
VOID
EFIAPI
SetSysSbspSocketIdNv (
  IN UINT8  SbspSocketIdNv
  );

/**
  This API gets the value for boot mode.

  @param None

  @retval BootMode  The data for boot mode returned.
**/
BootMode
EFIAPI
GetSysBootMode (
  VOID
  );

/**
  This API sets the value for boot mode.

  @param[in] BootMode  Value to set for boot mode.

  @retval None
**/
VOID
EFIAPI
SetSysBootMode (
  IN BootMode  SysBootMode
  );

/**
  This API gets the value for cpu frequency.

  @param None

  @retval UINT64  The data for cpu frequency returned.
**/
UINT64
EFIAPI
GetCpuFreq (
  VOID
  );

/**
  This API sets the value for cpu frequency.

  @param[in] UINT64  Value to set for cpu frequency.

  @retval None
**/
VOID
EFIAPI
SetCpuFreq (
  IN UINT64  CpuFreq
  );

/**
  This API gets the value for cpu Socket Id.

  @param None

  @retval UINT8  The data for cpu Socket Id.
**/
UINT8
EFIAPI
GetSysSocketId (
  VOID
  );

/**
  This API sets the value for SocketId.

  @param[in] UINT8  Value to set for SocketId.

  @retval None
**/
VOID
EFIAPI
SetSysSocketId (
  IN UINT8  SocketId
  );

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
  );

/**

  Set emulation type
  @param[in] EmuType  - Emulation Type to be set

**/
VOID
EFIAPI
SetEmulationType (
  IN UINT8     EmuType
  );

/**

  This API Starts USRA trace.

  @param None

  @retval None

**/
VOID
EFIAPI
UsraTraceStart (
  VOID
  );

/**

  This API ends USRA trace.

  @param None

  @retval None

**/
VOID
EFIAPI
UsraTraceEnd (
  VOID
  );

/**

  This API gets the configuration of USRA trace.

  @retval  - USRA trace configuration.

**/
UINT16
EFIAPI
GetUsraTraceConfiguration (
  VOID
  );


/**

  This API sets the configuration of USRA trace.

  @param[in] UsraTraceConfiguration  - USRA configuration to be set.

  @retval None

**/
VOID
EFIAPI
SetUsraTraceConfiguration (
  IN  UINT16     UsraTraceConfiguration
  );

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
  );


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
  );


/**

  This API disables the CpuCsrAccessVar dump info.

  @param  None

  @retval None

**/
VOID
EFIAPI
DisableCpuCsrAccessVarInfoDump (
  VOID
  );

/**
  This API gets the UPI connection node type for NodeId.

  @param[in] NodeId - Node ID

  @retval UINT8  The UPI connection node type for NodeId.
**/
UPI_CONNECTION_TYPE
EFIAPI
GetUpiConnectionType (
  IN UINT8  NodeId
  );

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
  );

/**
  This API gets UsraPpiPtr info.

  @param  None

  @retval UINTN  The data for UsraPpiPtr returned.
**/
UINTN
EFIAPI
GetUsraPpiPtr (
  VOID
  );

/**
  This API sets the value for UsraPpiPtr.

  @param[in] UINTN  Value to set for UsraPpiPtr.

  @retval None
**/
VOID
EFIAPI
SetUsraPpiPtr (
  IN UINTN  UsraPpiPtr
  );

/**
  Enable the posted CSR access method
  Subsequent CSR accesses will be sent using Posted semantics.

  A call to this function must always be paired with a call
  to PostedMethodDisable to avoid unintentionally sending all
  future CSR accesses using posted semantics.

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodEnable (
  VOID
  );

/**
  Disable the posted CSR access method
  Subsequent CSR accesses will be sent using non-posted semantics

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodDisable (
  VOID
  );

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
  );

/**
  This API gets PCIE segment number based on the socket number.

  @param  [in]  Socket             - Socket Id

  @retval UINT8  The PCIE Segment number returned.
**/
UINT8
EFIAPI
GetPcieSegment (
  IN  UINT8             Socket
  );

/**
  Get number of channel per memory controller

  @return the number of channel per memory controller

**/
UINT8
EFIAPI
GetNumChannelPerMc (
  VOID
  );

/**
  Get maximum number of IMC

  @return the maximum number of iMC

**/
UINT8
EFIAPI
GetMaxImc (
  VOID
  );

/**
  Get maximum DDR channel number of socket

  @return the maximum DDR number of socket

**/
UINT8
EFIAPI
GetMaxChDdr (
  VOID
  );

/**
  Get number of channel per memory controller

  @return the number of channel per memory controller

**/
UINT8
EFIAPI
GetNumChannelPerMcHbm (
  VOID
  );

/**
  Get maximum number of HBM channel per HBM IO

  @retval maximum number of HBM channel per HBM IO

**/
UINT8
EFIAPI
GetNumChannelPerIoHbm (
  VOID
  );

/**
  Get maximum number of IMC

  @return the maximum number of iMC

**/
UINT8
EFIAPI
GetMaxImcPerIoHbm (
  VOID
  );

/**
  Get maximum channel number of socket

  @return the maximum number of socket

**/
UINT8
EFIAPI
GetMaxChHbm (
  VOID
  );

/**
  Get maximum hbm io instance number of socket

  @return the maximum hbm io instance number of socket

**/
UINT8
EFIAPI
GetMaxIoHbm (
  VOID
  );

/**

  Indicate if HVM Mode is enabled


  @retval   TRUE      HVM Mode is enabled
  @retval   FALSE     HVM Mode is disabled

**/
BOOLEAN
EFIAPI
IsHvmModeEnable (
  VOID
  );

#endif // #ifndef __SYSTEM_INFO_LIB_H__
