/** @file
  Initializes UFS host controller located on SCS Iosf2Ocp bridge.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

#include <Register/PchRegs.h>
#include <Register/PchRegsScsUfs.h>
#include <Private/Library/PchFiaLib.h>
#include "PeiScsInitInternal.h"

/**
  Waits until test condition is satisfied. This function guarantees that
  timeout will be at least as long as the one specified.

  @param[in] RegisterAddress  Address of the register to test
  @param[in] RegisterMask     Value of the mask to apply to the register
  @param[in] TestValue        Test value against which a register is tested
  @param[in] TimeoutUs        Value of the timeout in us granularity

  @retval TRUE if register assumed test value before timeout, FALSE otherwise
**/
STATIC
BOOLEAN
WaitUntil (
  IN UINTN   RegisterAddress,
  IN UINT32  RegisterMask,
  IN UINT32  TestValue,
  IN UINT32  TimeoutUs
  )
{
  while (TimeoutUs > 0) {
    if ((MmioRead32 (RegisterAddress) & RegisterMask) == TestValue) {
      return TRUE;
    }
    MicroSecondDelay (10);
    TimeoutUs -= 10;
  }
  return FALSE;
}

/**
  Enables UFS host controller.

  @param[in] MmioBase Address of the MMIO base

  @retval TRUE if HC has been enabled, FALSE otherwise
**/
STATIC
BOOLEAN
ScsUfsEnableHc (
  IN UINTN  MmioBase
  )
{
  MmioWrite32 (MmioBase + R_SCS_MEM_UFS_EN, B_SCS_MEM_UFS_EN);
  return WaitUntil (MmioBase + R_SCS_MEM_UFS_EN, B_SCS_MEM_UFS_EN, B_SCS_MEM_UFS_EN, 50);
}

/**
  Performs UIC layer programming.
  This function assumes that UFS HC has already been enabled.

  @param[in] MmioBase  Address of the MMIO base
**/
STATIC
VOID
ScsUfsUicLayerProgramming (
  IN UINTN  MmioBase
  )
{
  BOOLEAN ControllerReady;

  //
  // Clear IS.UCCS status
  //
  MmioAnd32 (MmioBase + R_SCS_MEM_UFS_IS, ~(UINT32)B_SCS_MEM_UFS_IS_UCCS);

  ControllerReady = WaitUntil (
                      MmioBase + R_SCS_MEM_UFS_HC_STATUS,
                      B_SCS_MEM_UFS_HCS_UCRDY,
                      B_SCS_MEM_UFS_HCS_UCRDY,
                      50);
  if (ControllerReady) {
    //
    // We only have to set one attribute in UIC layer - LCCEnable(0x155E0000) = 0
    //
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG1, 0x155E0000);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG2, 0x0);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG3, 0x0);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UIC_CMD, V_SCS_MEM_UFS_UIC_CMD_DME_SET);
  } else {
    DEBUG ((DEBUG_ERROR, "UFS not ready for UIC programming\n"));
    ASSERT (FALSE);
  }
}

/**
  Configure Ufs controller

  @param[in] PciBaseAddress  PCI config base address of the controller
  @param[in] MmioBase        MMIO base address
**/
STATIC
VOID
ScsUfsInitMmioRegisters (
  IN UINT64  PciBaseAddress,
  IN UINTN   MmioBase
  )
{
  ScsControllerEnableMmio (PciBaseAddress, MmioBase);
  //
  // Configure UFS REF_CLK
  //
  MmioWrite32 (MmioBase + R_SCS_MEM_UFS_REF_CLK, V_SCS_MEM_UFS_REF_CLK);

  //
  // Enable host controller and perform UIC layer programming.
  // Reference code has to leave UFS enabled after performing this programming since UIC settings are volatile.
  // UFS pre-boot driver is assumed not to reset host controller during controller init.
  //
  if (ScsUfsEnableHc (MmioBase)) {
    ScsUfsUicLayerProgramming (MmioBase);
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to enable UFS host controller!\n"));
  }
  ScsControllerDisableMmio (PciBaseAddress);
}

/**
  Get the number of lanes that a given UFS controller may use.

  @param[in] UfsIndex  Index of the UFS controller

  @return A number of available lanes.
**/
STATIC
UINT8
ScsUfsGetNumOfAvailableLanes (
  IN UINT8  UfsIndex
  )
{
  UINT8  LaneIndex;
  UINT8  AvailableLanes;

  //
  // According to UFS 2.0 specification UFS can support up to 2 lanes.
  //
  AvailableLanes = 0;
  for (LaneIndex = 0; LaneIndex < 2; LaneIndex++) {
    if (PchFiaIsUfsLaneConnected (UfsIndex, LaneIndex)) {
      AvailableLanes++;
    } else {
      break;
    }
  }

  return AvailableLanes;
}

/**
  Enables UFS controller.

  @param[in] SiPolicyPpi     Pointer to SI_POLICY_PPI
  @param[in] UfsIndex        UFS controller index
  @param[in] MmioBase        MMIO address to be used during init flow
  @param[in] IsBootMedium    A flag to indicate if UFS is used to boot
  @param[in] AvailableLanes  Number of lanes available to UFS controller
**/
STATIC
VOID
ScsUfsEnable (
  IN SI_POLICY_PPI  *SiPolicy,
  IN UINT8          UfsIndex,
  IN UINTN          MmioBase,
  IN BOOLEAN        IsBootMedium,
  IN UINT8          AvailableLanes
  )
{
  UINT64         UfsBaseAddress;
  IOSF2OCP_PORT  *UfsIosf2OcpPort;
  UINT8          DeviceNum = 0;
  UINT8          FunctionNum = 0;
  SCS_UFS_MMP    *Mmp;
  UINT8          InterruptPin;
  UINT8          Irq;

  DEBUG ((DEBUG_INFO, "Enabling UFS, UFS index = %d\n", UfsIndex));

  ScsGetUfsBdf (UfsIndex, &DeviceNum, &FunctionNum);
  UfsBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     DeviceNum,
                     FunctionNum,
                     0
                     );
  UfsIosf2OcpPort = Iosf2OcpGetUfsPort (UfsIndex);
  Mmp = ScsGetUfsMmp (UfsIndex);
  ItssGetDevIntConfig (
    SiPolicy,
    DeviceNum,
    FunctionNum,
    &InterruptPin,
    &Irq
    );

  Iosf2OcpDisableBar1 (UfsIosf2OcpPort);
  PsfDisableDeviceBar (PsfScsUfsPort (UfsIndex), (BIT3 | BIT2));
  MmpInit (Mmp);
  if (!IsBootMedium) {
    Iosf2OcpEnableUfs (UfsIosf2OcpPort, AvailableLanes);
    ScsUfsInitMmioRegisters (UfsBaseAddress, MmioBase);
  }
  Iosf2OcpConfigureInterrupts (UfsIosf2OcpPort, InterruptPin, Irq);
}

/**
  Disable UFS controller.

  @param[in] UfsIndex  Index of the UFS controller
**/
STATIC
VOID
ScsUfsDisable (
  IN UINT8  UfsIndex
  )
{
  UINT64         UfsBaseAddress;
  IOSF2OCP_PORT  *UfsIosf2OcpPort;
  UINT8          DeviceNum = 0;
  UINT8          FunctionNum = 0;
  SCS_UFS_MMP    *Mmp;

  DEBUG ((DEBUG_INFO, "Disabling UFS, UFS index = %d\n", UfsIndex));

  ScsGetUfsBdf (UfsIndex, &DeviceNum, &FunctionNum);
  UfsBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     DeviceNum,
                     FunctionNum,
                     0
                     );
  UfsIosf2OcpPort = Iosf2OcpGetUfsPort (UfsIndex);
  Mmp = ScsGetUfsMmp (UfsIndex);

  Iosf2OcpDisableBar1 (UfsIosf2OcpPort);
  PsfDisableDeviceBar (PsfScsUfsPort (UfsIndex), (BIT3 | BIT2));
  PciSegmentOr16 (
    UfsBaseAddress + R_SCS_CFG_PG_CONFIG,
    (B_SCS_CFG_PG_CONFIG_SE| B_SCS_CFG_PG_CONFIG_PGE | B_SCS_CFG_PG_CONFIG_I3E)
    );
  ScsControllerPutToD3 (UfsBaseAddress);
  MmpDisable (Mmp);
  Iosf2OcpDisableUfs (UfsIosf2OcpPort);
  PsfDisableDevice (PsfScsUfsPort (UfsIndex));
  PmcDisableScsUfs (UfsIndex);
}

/**
  Enables or disables UFS host controller.

  @param[in]  SiPolicy            Pointer to SI_POLICY_PPI
  @param[in]  UfsIndex            Index of the UFS controler to init
  @param[in]  TempMemBaseAddress  Temporary MMIO address to be used during MMIO init
  @param[out] UfsInfo             Pointer to SCS_UFS_INFO structure
**/
VOID
ScsUfsInit (
  IN  SI_POLICY_PPI   *SiPolicy,
  IN  UINT8           UfsIndex,
  IN  UINTN           TempMemBaseAddress,
  OUT SCS_UFS_INFO    *UfsInfo
  )
{
  SCS_UFS_CONFIG             *UfsConfig;
  SCS_UFS_CONTROLLER_CONFIG  UfsControllerConfig;
  EFI_STATUS                 Status;
  BOOLEAN                    IsBootMedium;
  UINT8                      AvailableLanes;

  DEBUG ((DEBUG_INFO, "UFS init start, UFS index = %d\n", UfsIndex));

  if (UfsIndex >= PchGetMaxUfsNum ()) {
    ASSERT (FALSE);
    return;
  }

  Status = GetConfigBlock ((VOID*) SiPolicy, &gUfsConfigGuid, (VOID*) &UfsConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate UFS config block\n"));
    ASSERT (FALSE);
    return;
  }
  UfsControllerConfig = UfsConfig->UfsControllerConfig[UfsIndex];

  //
  // If UFS is not supported by PMC return here.
  // PSF access to the device has already been disabled in pre-mem.
  //
  if (!PmcIsScsUfsSupported (UfsIndex)) {
    UfsInfo->Supported = FALSE;
    return;
  }

  IsBootMedium = ScsIsUfsBootMedium (UfsIndex);
  AvailableLanes = ScsUfsGetNumOfAvailableLanes (UfsIndex);

  if (AvailableLanes == 0) {
    UfsInfo->Supported = FALSE;
  } else {
    UfsInfo->Supported = TRUE;
  }

  if ((UfsControllerConfig.Enable || IsBootMedium) && (AvailableLanes != 0)) {
    ScsUfsEnable (
      SiPolicy,
      UfsIndex,
      TempMemBaseAddress,
      IsBootMedium,
      AvailableLanes
      );
  } else {
    ScsUfsDisable (UfsIndex);
  }
  DEBUG ((DEBUG_INFO, "UFS init finished\n"));
}

