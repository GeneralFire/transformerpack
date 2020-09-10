/** @file
  Implementation of Viral Iio RAS lib.

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PcieStdLib.h>
#include <Library/RasDebugLib.h>
#include <Library/IoLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/FpgaConfigurationLib.h>
#include <Library/IioTopologyLib.h>
#include "IioPlatformData.h"
#include <Library/HobLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/KtiApi.h>

IIO_TOPOLOGY  *mIioTopology = NULL;

VOID
EFIAPI
DumpIioTopology (
  VOID
  )
{
  UINT8                 Port;
  UINT8                 Bus;
  UINT8                 Device;
  UINT8                 Function;
  UINT8                 SktId;
  UINT8                 IioStack;
  IIO_TOPOLOGY          *IioTopology;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "DumpIioTopology \n"));

  IioTopology = GetIioTopology ();

  if (IioTopology == NULL) {
    return;
  }

  for (SktId = 0; SktId < MAX_SOCKET; SktId++) {
    if ((IioTopology->SktInfo[SktId].Valid == 1)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "SktId = 0x%x, MaxPort 0x%x\n", SktId, IioTopology->SktInfo[SktId].MaxPciePortNumberPerSkt));
      for (Port = 0; Port < IioTopology->SktInfo[SktId].MaxPciePortNumberPerSkt; Port++) {
        if (IioTopology->SktInfo[SktId].Port[Port].Enabled) {
          Bus = IioTopology->SktInfo[SktId].Port[Port].Bus;
          Device = IioTopology->SktInfo[SktId].Port[Port].Device;
          Function = IioTopology->SktInfo[SktId].Port[Port].Function;
          RAS_DEBUG ((LEVEL_FUNC_FLOW, "SktId = 0x%x,Port = 0x%x, Bus = 0x%x,Device = 0x%x,Function = 0x%x \n",SktId,Port,Bus,Device,Function));
        }
      }
    }
  }

  for (SktId = 0; SktId < MAX_SOCKET; SktId++) {
    for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
      if (IioTopology->SktInfo[SktId].IioStackInfo[IioStack].StackValid) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "SktId = 0x%x,IioStack = 0x%x \n", SktId, IioStack));
      }
    }
  }
}

RETURN_STATUS
EFIAPI
InitializeIioTopology (
  IN IIO_TOPOLOGY  *IioTopology
  )
{
  UINT8                       Skt;
  UINT8                       PciePortIndex;
  UINT8                       Index;
  UINT8                       MaxPciePortNumberPerSocket;
  UINT8                       IioStackIndex;
  IIO_GLOBALS                 *IioGlobalData;
  EFI_HOB_GUID_TYPE           *GuidHob;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitializeIioTopology!\n"));

  //
  // Get IIO SI Policy HOB
  //
  GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
  if (GuidHob == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitializeIioTopology: Failed to get gIioSiPolicyHobGuid Hob!\n"));
    RAS_ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    IioTopology->SktInfo[Skt].Valid = 0;
    if (IioGlobalData->IioVar.IioVData.SocketPresent[Skt]) {
      IioTopology->SktInfo[Skt].Valid = 1;
      IioTopology->SktInfo[Skt].BaseBusNum = IioGlobalData->IioVar.IioVData.SocketBaseBusNumber[Skt];
      IioTopology->SktInfo[Skt].PciSegment = IioGlobalData->IioVar.IioVData.SegmentSocket[Skt];
      //
      // Initialize Stack information
      //
      for (IioStackIndex = 0; IioStackIndex < MAX_IIO_STACK; IioStackIndex++) {
        IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].StackValid = (IioGlobalData->IioVar.IioVData.StackPresentBitmap[Skt] & (1 << IioStackIndex)) ? 1 : 0;
        IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].StackBus = IioGlobalData->IioVar.IioVData.SocketStackBus[Skt][IioStackIndex];
        IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].StackBusLimit = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[Skt][IioStackIndex];
        IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].RcrbBaseAddr = KTI_HOST_OUT_PTR->CpuInfo[Skt].CpuRes.StackRes[IioStackIndex].RcrbBase;
        if (IioStackIndex < MAX_CXL_PER_SOCKET) {
          IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].CxlValid = (BOOLEAN) (KTI_HOST_OUT_PTR->CxlInfo[Skt][IioStackIndex].CxlStatus == AlreadyInCxlMode);
        } else {
          IioTopology->SktInfo[Skt].IioStackInfo[IioStackIndex].CxlValid = FALSE;
        }
      }
      //
      // Initialize Port information
      //
      MaxPciePortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[Skt];
      IioTopology->SktInfo[Skt].MaxPciePortNumberPerSkt = MaxPciePortNumberPerSocket;
      RAS_ASSERT (MaxPciePortNumberPerSocket <= NUMBER_PORTS_PER_SOCKET);
      for (PciePortIndex = 0, Index = 0; PciePortIndex < MaxPciePortNumberPerSocket; PciePortIndex++){
        if (IioGlobalData->IioVar.IioOutData.PciePortPresent[(Skt * MaxPciePortNumberPerSocket) + PciePortIndex]) {
          IioTopology->SktInfo[Skt].Port[Index].Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[Skt][PciePortIndex];
          IioTopology->SktInfo[Skt].Port[Index].Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PciePortIndex].Device;
          IioTopology->SktInfo[Skt].Port[Index].Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PciePortIndex].Function;
          IioTopology->SktInfo[Skt].Port[Index].Enabled = TRUE;
          Index++;
        }
      }
      //
      // Initialize Ubox Bus Number for 10nm CPU
      //
      IioTopology->SktInfo[Skt].Socket10nmUboxBus0 = CpuCsrAccessVarPtr->Socket10nmUboxBus0[Skt];
    }
  }

  DumpIioTopology ();

  return EFI_SUCCESS;
}

/**
  This function will enable Iio error reporting.

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface            Interface instance.
  @param[in] Handle      The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
InitIioTopologyCallBack (
  IN      CONST EFI_GUID   *Protocol,
  IN      VOID             *Interface,
  IN      EFI_HANDLE        Handle
  )
{
  UINT8                       Skt;
  UINT8                       Index;
  UINT8                       MaxPciePortNumberPerSocket;

  if (mIioTopology == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    if (mIioTopology->SktInfo[Skt].Valid) {
      //
      // Initialize Port information
      //
      MaxPciePortNumberPerSocket = mIioTopology->SktInfo[Skt].MaxPciePortNumberPerSkt;
      for (Index = 0; Index < MaxPciePortNumberPerSocket; Index++){
        if (mIioTopology->SktInfo[Skt].Port[Index].Enabled) {
          PcieGetSecondarySubordinateBus (
            Skt,
            mIioTopology->SktInfo[Skt].Port[Index].Bus,
            mIioTopology->SktInfo[Skt].Port[Index].Device,
            mIioTopology->SktInfo[Skt].Port[Index].Function,
            NULL,
            &mIioTopology->SktInfo[Skt].Port[Index].SecondaryBus,
            &mIioTopology->SktInfo[Skt].Port[Index].SubordinateBus
          );
        }
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  Get Iio topology data, in case first one invoke this routine, it will initialize topology data.

  @retval  point of memoty topology info

**/
IIO_TOPOLOGY *
EFIAPI
GetIioTopology (
  VOID
  )
{
  ASSERT (mIioTopology);
  return mIioTopology;
}

/**
  Get iio port pci address.

  @param[in]  Skt  -- Socket index
  @param[in]  PortIndex  -- iio port index per socket
  @param[out] Seg  -- iio port segment number
  @param[out] Bus  -- iio port bus address
  @param[out] Dev  -- iio port device address
  @param[out] func -- iio port function address

  @retval  TRUE  -- iio port exist; FALSE -- iio port is not exist.

**/
BOOLEAN
EFIAPI
GetRootPortInfo (
  IN  UINT8    Skt,
  IN  UINT8    PortIndex,
  OUT UINT8    *Seg,
  OUT UINT8    *Bus,
  OUT UINT8    *Dev,
  OUT UINT8    *Fun
  )
{
  if (mIioTopology == NULL) {
    return FALSE;
  }

  RAS_ASSERT (Skt < MAX_SOCKET);
  if (PortIndex >= mIioTopology->SktInfo[Skt].MaxPciePortNumberPerSkt) {
    return FALSE;
  }

  if (mIioTopology->SktInfo[Skt].Port[PortIndex].Enabled) {
    if (Seg != NULL) {
      *Seg = mIioTopology->SktInfo[Skt].PciSegment;
    }
    if (Bus != NULL) {
      *Bus = mIioTopology->SktInfo[Skt].Port[PortIndex].Bus;
    }
    if (Dev != NULL) {
      *Dev = mIioTopology->SktInfo[Skt].Port[PortIndex].Device;
    }
    if (Fun != NULL) {
      *Fun = mIioTopology->SktInfo[Skt].Port[PortIndex].Function;
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Get the root port of a specific Pcie Device

  @param[in]  Skt  -- Socket index
  @param[in]  InputBus  -- The Bus Number of specific device
  @param[in]  InputDev  -- The Dev Number of specific device
  @param[in]  InputFun  -- The FUN Number of specific device
  @param[out] Seg  -- iio root port segment number
  @param[out] Bus  -- iio root port bus address
  @param[out] Dev  -- iio root port device address
  @param[out] func -- iio root port function address

  @retval  TRUE  -- iio port exist; FALSE -- iio port is not exist.

**/
BOOLEAN
EFIAPI
GetRootPortInfoOfPcieDev (
  IN  UINT8    Skt,
  IN  UINT8    InputBus,
  IN  UINT8    InputDev,
  IN  UINT8    InputFun,
  OUT UINT8    *Seg,
  OUT UINT8    *Bus,
  OUT UINT8    *Dev,
  OUT UINT8    *Fun
  )
{
  UINT8                       Index;
  UINT8                       MaxPciePortNumberPerSocket;
  BOOLEAN                     Found = FALSE;

  if (mIioTopology == NULL) {
    return FALSE;
  }

  RAS_ASSERT (Skt < MAX_SOCKET);

  MaxPciePortNumberPerSocket = mIioTopology->SktInfo[Skt].MaxPciePortNumberPerSkt;

  for (Index = 0; Index < MaxPciePortNumberPerSocket; Index++){
    if (mIioTopology->SktInfo[Skt].Port[Index].Enabled) {
      if (InputBus >= mIioTopology->SktInfo[Skt].Port[Index].SecondaryBus
            && InputBus <= mIioTopology->SktInfo[Skt].Port[Index].SubordinateBus) {
        Found = TRUE;
        break;
      }

      if ((InputBus == mIioTopology->SktInfo[Skt].Port[Index].Bus
            && InputDev == mIioTopology->SktInfo[Skt].Port[Index].Device)
              && InputFun == mIioTopology->SktInfo[Skt].Port[Index].Function) {
        Found = TRUE;
        break;
      }
    }
  }

  if (Found) {
    if (Seg != NULL) {
      *Seg = mIioTopology->SktInfo[Skt].PciSegment;
    }
    if (Bus != NULL) {
      *Bus = mIioTopology->SktInfo[Skt].Port[Index].Bus;
    }
    if (Dev != NULL) {
      *Dev = mIioTopology->SktInfo[Skt].Port[Index].Device;
    }
    if (Fun != NULL) {
      *Fun = mIioTopology->SktInfo[Skt].Port[Index].Function;
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check the IIO Stack availability

  @param[in]  Skt  -- Socket index
  @param[in]  IioStack  -- iio Stack index per socket

  @retval  TRUE  -- iio stack exist; FALSE -- iio stack is not exist.

**/
BOOLEAN
EFIAPI
IsIioStackValid (
  IN  UINT8    Skt,
  IN  UINT8    IioStack
  )
{
  if (mIioTopology == NULL) {
    return FALSE;
  }
  RAS_ASSERT (Skt < MAX_SOCKET);
  RAS_ASSERT (IioStack < MAX_IIO_STACK);
  return mIioTopology->SktInfo[Skt].IioStackInfo[IioStack].StackValid;
}

/**
  This implementation is to get IIO stack start bus number.

  @param  Socket -- socket index
  @param  IioStack -- iio stack index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetStackStartBus (
  IN      UINT8   Socket,
  IN      UINT8   IioStack,
  IN OUT  UINT8   *Bus
  )
{
  if (mIioTopology == NULL || !IsIioStackValid (Socket, IioStack) || Bus == NULL) {
    return FALSE;
  }

  *Bus = mIioTopology->SktInfo[Socket].IioStackInfo[IioStack].StackBus;
  return TRUE;
}

/**
  This implementation is to get IIO stack limit bus number.

  @param  Socket -- socket index
  @param  IioStack -- iio stack index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetStackLimitBus (
  IN      UINT8   Socket,
  IN      UINT8   IioStack,
  IN OUT  UINT8   *Bus
  )
{
  if (mIioTopology == NULL || !IsIioStackValid (Socket, IioStack) || Bus == NULL) {
    return FALSE;
  }

  *Bus = mIioTopology->SktInfo[Socket].IioStackInfo[IioStack].StackBusLimit;
  return TRUE;
}

/**
  This implementation is to get 10nm CPU UBOX bus number.
  Note!!!- Socket10nmUboxBus0 is a field only used for 10nm CPU,  SKX/CPX should not use this
  field. whoever want to use this function , please make sure use it properly.

  @param  Socket -- socket index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetSocket10nmUboxBus (
  IN      UINT8   Socket,
  IN OUT  UINT8   *Bus
  )
{
  if (mIioTopology == NULL || Bus == NULL) {
    return FALSE;
  }

  *Bus = mIioTopology->SktInfo[Socket].Socket10nmUboxBus0;
  return TRUE;
}

/**
  Get specific stack CXL Information

  @param  Socket -- socket index
  @param  Stack -- stack index
  @param  *RcrbBa -- RCRB Base Address

  @retval  TRUE -- CXL Information is valid, FALSE -- CXL Infomation is invalid
**/
BOOLEAN
EFIAPI
GetStackCxlInfo (
  IN      UINT32   Socket,
  IN      UINT8    Stack,
  OUT     UINTN    *RcrbBa,
  OUT     BOOLEAN  *CxlValid
  )
{
  if ((mIioTopology == NULL) || (RcrbBa == NULL && CxlValid == NULL)) {
    return FALSE;
  }

  if (RcrbBa != NULL) {
    *RcrbBa = mIioTopology->SktInfo[Socket].IioStackInfo[Stack].RcrbBaseAddr;
  }

  if (CxlValid != NULL) {
    *CxlValid = mIioTopology->SktInfo[Socket].IioStackInfo[Stack].CxlValid;
  }

  return TRUE;
}

/**

  The constructor function initialize Iio topology library.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitIioTopologyLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN        NewAllocation;
  EFI_STATUS     Status = EFI_SUCCESS;
  VOID           *Registration;

  mIioTopology = GetRasGlobalData (L"PlatIioTopology", sizeof (IIO_TOPOLOGY), &NewAllocation);

  if (mIioTopology == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (NewAllocation) {
    Status = InitializeIioTopology (mIioTopology);

    if(!EFI_ERROR(Status)) {
      //
      // Fill in more infomation at ready to Lock phase
      //
      Status = gSmst->SmmRegisterProtocolNotify (
                        &gEfiSmmReadyToLockProtocolGuid,
                        InitIioTopologyCallBack,
                        &Registration
                        );
    }
  }

  return Status;
}

