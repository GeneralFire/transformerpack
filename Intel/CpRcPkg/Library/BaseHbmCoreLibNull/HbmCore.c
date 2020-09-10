/** @file
  Interface source file for the HBM library class.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <MrcCommonTypes.h>
#include <Library/HbmCoreLib.h>

/**

  Gets architectural variables for HBM

  @param[out]               HbmMaxIoInst        Pointer to specify the max number of instances
  @param[out]               HbmMaxChannel       Pointer to specify the max number of channels
  @param[out]               HbmNumChPerIo       Pointer to specify the max number of channels per IO

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmPopulateLegacyVariables (
      OUT   UINT8           *HbmMaxIoInst,
      OUT   UINT8           *HbmMaxChannel,
      OUT   UINT8           *HbmNumChPerIo
  )
{
  if (HbmMaxIoInst != NULL) {
    *HbmMaxIoInst   = 1;
  }
  if (HbmMaxChannel != NULL) {
    *HbmMaxChannel  = 1;
  }
  if (HbmNumChPerIo != NULL) {
    *HbmNumChPerIo  = 1;
  }

  return EFI_SUCCESS;
}

/**

  Populate HBM global variables based on the MSM table

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmPopulateArchVariables (
  IN        UINT8           Socket
  )
{
  return EFI_SUCCESS;
}

/**

  Verify HBM arch variables


  @param[in]                MsmListPtr          Pointer to MSM List

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
VerifyHbmMsmList (
  IN      MRC_MSM           *MsmListPtr
  )
{
  return EFI_SUCCESS;
}

/**

  Determine if an specific step needs to be executed


  @param[in]                FlowBit             An step represented by one or more bits

  @retval                   TRUE                All bits are set
  @retval                   FALSE               At least 1 bit is not set for current flow

**/
BOOLEAN
EFIAPI
IsHbmFlowEnabled (
  IN        UINT32          FlowBit
  )
{
  return FALSE;
}

/**

  Set the flow bits


  @param[in]                Flows               Value that represents all steps for HBM

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
SetHbmFlows (
  IN        UINT32          Flows
  )
{
  return EFI_SUCCESS;
}

/**

  Returns the flow bits


  @return                   Flow bits

**/
UINT32
EFIAPI
GetHbmFlows (
  VOID
  )
{
  return 0;
}

/**
  This function enables all HBM flows.

  @param VOID

  @retval EFI_SUCCESS   All HBM flows are enabled successfully.
**/
EFI_STATUS
EFIAPI
EnableAllHbmFlows (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**

  Allocates the required resources for HBM Host


  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval                   EFI_OUT_OF_RESOURCES  Pei services not able to allocate resources for HBM Host

**/
EFI_STATUS
EFIAPI
HbmHostAllocation (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Gets the value for the setup option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option

  @return                   Value for specified setup option

**/
UINT32
EFIAPI
HbmGetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmPolicyOption
  )
{
  return 0;
}

/**
  Sets the value for the setup option specified

  @param[in]                HBM_POLICY_OPTIONS  Specific HBM Setup Option
  @param[in]                SetupValue          Value

**/
VOID
EFIAPI
HbmSetPolicyOption (
  IN        HBM_POLICY_OPTIONS  HbmPolicyOption,
  IN        UINT32              ConfigValue
  )
{

}

/**

  Determines if channels is enabled or not

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)

  @retval                   TRUE                Channel is enabled
  @retval                   FALSE               Channels is not enabled

**/
BOOLEAN
EFIAPI
IsHbmChannelEnabled (
  IN        UINT8               Socket,
  IN        UINT8               Channel
  )
{
  return FALSE;
}

/**

  Determines if HBM memory subsystem is enabled or disabled.

  @param[in] Socket         Current Socket (0-Based)
  @param[in] MemSs          Current HBM memory subsystem

  @retval TRUE              HBM memory subsystem is enabled
  @retval FALSE             HBM memory subsystem is disabled

**/
BOOLEAN
EFIAPI
IsHbmMemSsEnabled (
  IN UINT8    Socket,
  IN UINT8    MemSs
  )
{
  return FALSE;
}

/**

  Sets if MemSs is enabled or not

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmMemSsEnable (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT8               Enable
  )
{

}

/**
  This function gets the HBM frequency from HBM Host.

  @param[in] SocketId         Socket index.

  @retval  The value of HBM frequency.
**/
HBM_FREQUENCY
EFIAPI
HbmGetFrequency (
  IN UINT8  SocketId
  )
{
  return HBM2E_2p8;
}

/**
  This function gets the HBM frequency in MHz from HBM Host for the specified socket.

  @param[in] SocketId         Socket index.

  @retval    The value of HBM frequency in Mhz.
**/
UINT16
EFIAPI
HbmGetFrequencyInMhz (
  IN UINT8  SocketId
  )
{
  return 0;
}

/**
  This function sets the HBM frequency into HBM Host.

  @param[in] SocketId         Socket index.
  @param[in] HbmFrequency     HBM frequency to set.

  @retval N/A.
**/
VOID
EFIAPI
HbmSetFrequency (
  IN UINT8          SocketId,
  IN HBM_FREQUENCY  HbmFrequency
  )
{
  return;
}

/**

  Sets (Enables/Disables) an HBM channel

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              Value to set

**/
VOID
EFIAPI
SetHbmChEnable (
  IN        UINT8               Socket,
  IN        UINT8               Channel,
  IN        UINT8               Enable
  )
{

}

/**

  Gets the start/end channel of a particular MemSs

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[out]               StartCh             Start channel of current MemSs (Pointer)
  @param[out]               EndCh               End channel of current MemSs (Pointer)

**/
VOID
EFIAPI
GetHbmStartEndCh (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
      OUT   UINT8               *StartCh,
      OUT   UINT8               *EndCh
  )
{
  *StartCh  = 0;
  *EndCh    = 0;
}

/**
  Gets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System


  @return                   Cache value for Mem Size

**/
UINT16
EFIAPI
GetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs
  )
{
  return 0;
}

/**
  Sets the cache value for Mem Size

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                MemSs               Current Memory Sub System
  @param[in]                Value               Value to set

**/
VOID
EFIAPI
SetHbmCacheMemSize (
  IN        UINT8               Socket,
  IN        UINT8               MemSs,
  IN        UINT16              Value
  )
{

}

/**
  Get the pointer to HBM device ID data.

  @param[in] Socket              Current Socket (0-Based)
  @param[in] HbmIoIndex          The HBMIO instance index (0~ max HBMIO in socket -1)

  @retval  The pointer to HBM device ID data.
**/
DEVICE_ID_WDR *
EFIAPI
GetHbmDeviceId (
  IN UINT8     Socket,
  IN UINT8     HbmIoIndex
  )
{
  //
  // This function is not allowed to be called as Hbm Host resources are not allocated in this route.
  //
  ASSERT (FALSE);
  return NULL;
}

/**
  HBM Discovery

  This function reads the Device ID data from HBM FW and detects the HBM device attributes
  according to the Device ID data. The HBM device attributes will be cached into HBM Host.

  @param[in] SocketId             Socket index.

  @retval  EFI_SUCCESS            This function completed the HBM discovery successfully.
  @retval  !EFI_SUCCESS           This function failed to discover HBM device.
**/
EFI_STATUS
EFIAPI
HbmDiscovery (
  IN UINT8       SocketId
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Get HBM MTR per Density

  @param[in]      HbmDensity     - HBM Density
  @param[in]      AddressingMode - Addressing Mode Support
  @param[in, out] DimmMtr        - Pointer to DIMM MTR

  @retval EFI_SUCCESS     if got
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
EFIAPI
HbmGetDimmMtr (
  IN     HBM_DENSITY             HbmDensity,
  IN     UINT8                   AddressingMode,
  IN OUT DIMMMTR_DATA_STRUCT     *DimmMtr
  )
{
  return EFI_UNSUPPORTED;
}