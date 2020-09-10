/** @file
  Implementation of the library for enterprise PCIe root ports initialization

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchInfoLib.h>
#include <PchLimits.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegs.h>
#include <Library/PchPcieRpLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcrRegs.h>
#include <PcieRegs.h>
#include <Library/HobLib.h>
#include <Library/PsfLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/TimerLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <PchPcieRpConfig.h>
#include <EnterprisePcieConfig.h>

#include "PeiEnterprisePcieRpInitInternal.h"

/**
  PCIe controller bifurcation configuration strings.
**/
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8* mPcieControllerConfigName[] = {
  "default",
  "4x1",
  "1x2-2x1",
  "2x2",
  "1x4",
  "4x2",
  "1x4-2x2",
  "2x2-1x4",
  "2x4",
  "1x8"
};

#define DEBUG_PRINT_BIFURCATION(ControllerIndex, Config) \
        DEBUG ((DEBUG_INFO, "PCIe controller %d bifurcation is %a\n", (ControllerIndex), mPcieControllerConfigName[(Config)]))

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
STATIC
EFI_STATUS
BifurcationConvertEnumToValue (
  IN  PCIE_BIFURCATION_CONFIG  BifurcationPolicy,
  OUT UINT32                   *Value
  )
{
  switch (BifurcationPolicy) {
    case PcieBifurcation4x1:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED;
      break;
    case PcieBifurcation1x2_2x1:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED;
      break;
    case PcieBifurcation2x2:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED;
      break;
    case PcieBifurcation1x4:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED;
      break;
    case PcieBifurcation4x2:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_2_2;
      break;
    case PcieBifurcation1x4_2x2:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_2_2;
      break;
    case PcieBifurcation2x2_1x4:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_4;
      break;
    case PcieBifurcation2x4:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_4;
      break;
    case PcieBifurcation1x8:
      *Value = V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_8;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }
  if (*Value == V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  This function converts bifurcation control register value
  to a configuration enum used in policy

  @param[in]  Value               Control register field value
  @param[out] BifurcationPolicy   Bifurcation configuration enum

  @retval EFI_SUCCESS             Conversion successful
  @retval EFI_INVALID_PARAMETER   Invalid control register value given
**/
STATIC
EFI_STATUS
BifurcationConvertValueToEnum (
  IN  UINT32                   Value,
  OUT PCIE_BIFURCATION_CONFIG  *BifurcationPolicy
  )
{
  switch (Value) {
    case V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_2_2:
      *BifurcationPolicy = PcieBifurcation4x2;
      break;
    case V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_2_2:
      *BifurcationPolicy = PcieBifurcation1x4_2x2;
      break;
    case V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_4:
      *BifurcationPolicy = PcieBifurcation2x2_1x4;
      break;
    case V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_4:
      *BifurcationPolicy = PcieBifurcation2x4;
      break;
    case V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_8:
      *BifurcationPolicy = PcieBifurcation1x8;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}

/**
  Returns the PCIe controller bifurcation configuration

  @param[in] RpHandle                 Pointer to PCIE RP handle

  @retval PCIe controller configuration
**/
PCIE_BIFURCATION_CONFIG
PchPcieGetBifurcationConfig (
  IN  PCH_RP_HANDLE           *RpHandle
  )
{
  EFI_STATUS               Status;
  UINT32                   Data32;
  UINT8                    Response;
  PCIE_BIFURCATION_CONFIG  Config;
  PCH_RP_DEVICE            *RpDevice;

  RpDevice = RpHandle->Device;

  if (RpHandle->Callback->PchRpSbiExecution) {
    Status = RpHandle->Callback->PchRpSbiExecution (
      RpDevice,
      GetRpSbiPid (RpDevice->RootPortIndex),
      R_PCH_PCIE_MSG_PPBIFCTL_PRIV,
      PrivateControlRead,
      FALSE,
      &Data32,
      &Response
      );
    if (EFI_ERROR(Status)) {
      if (EFI_NO_RESPONSE == Status) {
        DEBUG ((DEBUG_INFO, "%a: PchSbiExecution failed - SBI is powered down\n", __FUNCTION__));
      } else {
        ASSERT_EFI_ERROR (Status);
      }
      return PcieBifurcationDefault;
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: PchRpSbiExecution not implemented\n", __FUNCTION__));
    return PcieBifurcationDefault;
  }

  Data32 = (Data32 & B_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0) >> N_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0;
  Status = BifurcationConvertValueToEnum (Data32, &Config);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return PcieBifurcationDefault;
  }

  return Config;
}

/**
  Sets the PCIe controller bifurcation configuration.
  Locks the register after write.

  @param[in] RpHandle               Pointer to PCIE RP handle
  @param[in] Config                 PCIe controller bifurcation configuration

  @retval EFI_STATUS                Status
**/
EFI_STATUS
PchPcieSetBifurcationConfig (
  IN  PCH_RP_HANDLE            *RpHandle,
  IN  PCIE_BIFURCATION_CONFIG  Config
  )
{
  EFI_STATUS               Status;
  UINT32                   Data32;
  UINT8                    Response;
  PCH_RP_DEVICE            *RpDevice;

  RpDevice = RpHandle->Device;

  if (Config == PcieBifurcationDefault) {
    // Read the default value fro the register
    if (RpHandle->Callback->PchRpSbiExecution) {
      Status = RpHandle->Callback->PchRpSbiExecution (
        RpDevice,
        GetRpSbiPid (RpDevice->RootPortIndex),
        R_PCH_PCIE_MSG_PPBIFCTL_PRIV,
        PrivateControlRead,
        FALSE,
        &Data32,
        &Response
      );
      if (EFI_ERROR(Status)) {
        if (EFI_NO_RESPONSE == Status) {
          DEBUG ((DEBUG_INFO, "%a: PchSbiExecution failed - SBI is powered down\n", __FUNCTION__));
          return Status;
        } else {
          ASSERT_EFI_ERROR (Status);
        }
      }
    } else {
      DEBUG ((DEBUG_INFO, "%a: PchRpSbiExecution not implemented\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
    }
    Data32 &= B_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0;
  } else {
    Status = BifurcationConvertEnumToValue (Config, &Data32);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  // Lock the bifurcation register
  Data32 |= 1 << N_PCH_PCIE_MSG_PPBIFCTL_PRIV_CL;

  if (RpHandle->Callback->PchRpSbiExecution) {
    Status = RpHandle->Callback->PchRpSbiExecution (
      RpDevice,
      PchGetPcieControllerSbiPid (RpDevice->CtrlIndex),
      R_PCH_PCIE_MSG_PPBIFCTL_PRIV,
      PrivateControlWrite,
      FALSE,
      &Data32,
      &Response
      );
  } else {
    DEBUG ((DEBUG_INFO, "%a: PchRpSbiExecution not implemented\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG_PRINT_BIFURCATION (RpDevice->CtrlIndex, Config);

  return Status;
}


/**
  Program bifurcation configuration for each PCIe RP controller based on policy.
  This also locks the bifurcation control register.

  @param[in] RpHandle                  Pointer to PCIE RP handle
**/
VOID
PcieBifurcationInit (
  IN PCH_RP_HANDLE             *RpHandle
  )
{
  EFI_STATUS              Status;

  //
  // Program bifurcation configuration for each PCIe RP controller based on policy.
  // This also locks the bifurcation control register.
  //
  Status = PchPcieSetBifurcationConfig (RpHandle, RpHandle->PcieRpPreMemConfig->Bifurcation[RpHandle->Device->CtrlIndex]);

  if (EFI_NO_RESPONSE == Status) {
    DEBUG ((DEBUG_WARN, "Can't set the bifurcation config, device is powered down\n"));
    return;
  } else if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
}


/**
  Get PSF config table

  @param[in     ] RpDevice                 Pointer to PCIE RP device
  @param[in, out] PsfPcieCtrlConfig        Pointer to PSF config table
**/
VOID
PcieRpGetPsfConfig (
  IN     PCH_RP_HANDLE            *RpHandle,
  IN OUT PSF_PCIE_CTRL_CONFIG     *PsfPcieCtrlConfig
  )
{
   PCIE_BIFURCATION_CONFIG  PcieBifurcationConfig;

   PcieBifurcationConfig = PchPcieGetBifurcationConfig (RpHandle);
   switch (PcieBifurcationConfig) {
     case PcieBifurcation4x2:
       *PsfPcieCtrlConfig = PsfPcieCtrl4xn;
       break;
     case PcieBifurcation1x4_2x2:
       *PsfPcieCtrlConfig = PsfPcieCtrl1x2n_2xn;
       break;
     case PcieBifurcation2x2_1x4:
       *PsfPcieCtrlConfig = PsfPcieCtrl2xn_1x2n;
       break;
     case PcieBifurcation2x4:
       *PsfPcieCtrlConfig = PsfPcieCtrl2x2n;
       break;
     case PcieBifurcation1x8:
       *PsfPcieCtrlConfig = PsfPcieCtrl1x4n;
       break;
     default:
       DEBUG ((DEBUG_ERROR, "%a: PcieRpGetPsfConfigTable set to Undefined for controller $d\n", __FUNCTION__, RpHandle->Device->CtrlIndex));
       *PsfPcieCtrlConfig = PsfPcieCtrlUndefined;
       break;
  }
}

