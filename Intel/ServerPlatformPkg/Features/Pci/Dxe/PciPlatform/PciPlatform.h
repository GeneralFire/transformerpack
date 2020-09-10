/** @file
  This is PCI platform initialization code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2018 Intel Corporation. <BR>

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

#ifndef _PCI_PLATFORM_MODULE_H_
#define _PCI_PLATFORM_MODULE_H_

#include <PiDxe.h>
#ifndef CDF_SC_FLAG
#include <Register/PchRegsEva.h>
#endif
#include <PchAccess.h>
#include <Cpu/CpuCoreRegs.h>
#include <Platform.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/S3PciLib.h>
#include <Library/PciLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/SetupLib.h>
#include <Protocol/Runtime.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/PciCallback.h>
#include <Protocol/PciPlatform.h>
#include <Protocol/PciIovPlatform.h>
#include <Protocol/PciIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <SystemBoard.h>
#include <Guid/SetupVariable.h>
#include <Guid/SocketVariable.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci.h>


#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif
//
// Global variables for Option ROMs
//

#define INVALID                     0xBD

#define PCI_CALLBACK_DATA_SIGNATURE SIGNATURE_32 ('P', 'c', 'i', 'c')

typedef struct {
  UINT32                    Signature;
  LIST_ENTRY                Link;
  EFI_PCI_CALLBACK_FUNC     Function;
  EFI_PCI_ENUMERATION_PHASE Phase;
} PCI_CALLBACK_DATA;

typedef struct {
  EFI_HANDLE                      PciPlatformHandle;
  EFI_HANDLE                      RootBridgeHandle;
  EFI_PCI_PLATFORM_PROTOCOL       PciPlatform;
  EFI_PCI_CALLBACK_PROTOCOL       PciCallback;
#ifdef EFI_PCI_IOV_SUPPORT
  EFI_PCI_IOV_PLATFORM_PROTOCOL   PciIovPlatform;
#endif
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo[MAX_SOCKET][MAX_IIO_STACK];
  EFI_CPU_IO2_PROTOCOL            *CpuIo;
  EFI_LIST_ENTRY                  PciCallbackList;
  EFI_PCI_CALLBACK_CONTEXT        Context;
  EFI_PCI_ENUMERATION_PHASE       PciEnumerationPhase;
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT8                           BusAssignedTime;
} PCI_PLATFORM_PRIVATE_DATA;

#define PCI_CALLBACK_DATA_FROM_LINK(_node) \
  CR ( \
    _node, \
    PCI_CALLBACK_DATA, \
    Link, \
    PCI_CALLBACK_DATA_SIGNATURE \
    )

extern PCI_PLATFORM_PRIVATE_DATA  mPciPrivateData;
extern EFI_GUID  gPchSataEfiLoadProtocolGuid;

/**

  Perform initialization by the phase indicated.

  @param This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  @param HostBridge    -  The associated PCI Host bridge handle.
  @param Phase         -  The phase of the PCI controller enumeration.
  @param ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

  @retval EFI_SUCCESS  -  Must return with success.

**/
EFI_STATUS
EFIAPI
PhaseNotify (
  IN EFI_PCI_PLATFORM_PROTOCOL                        *This,
  IN EFI_HANDLE                                       HostBridge,
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE    Phase,
  IN EFI_PCI_CHIPSET_EXECUTION_PHASE                  ChipsetPhase
  )
;

/**

  The PlatformPrepController() function can be used to notify the platform driver so that
  it can perform platform-specific actions. No specific actions are required.
  Several notification points are defined at this time. More synchronization points may be
  added as required in the future. The PCI bus driver calls the platform driver twice for
  every PCI controller-once before the PCI Host Bridge Resource Allocation Protocol driver
  is notified, and once after the PCI Host Bridge Resource Allocation Protocol driver has
  been notified.
  This member function may not perform any error checking on the input parameters. It also
  does not return any error codes. If this member function detects any error condition, it
  needs to handle those errors on its own because there is no way to surface any errors to
  the caller.

  @param This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  @param HostBridge    -  The associated PCI Host bridge handle.
  @param RootBridge    -  The associated PCI root bridge handle.
  @param PciAddress    -  The address of the PCI device on the PCI bus.
  @param Phase         -  The phase of the PCI controller enumeration.
  @param ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

  @retval EFI_SUCCESS  -  The function completed successfully.

**/
EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
;

/**

  Set the PciPolicy as EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_NO_ALIAS.

  @param This       -  The pointer to the Protocol itself.
  PciPolicy  -  the returned Policy.

  @retval EFI_UNSUPPORTED        -  Function not supported.
  @retval EFI_INVALID_PARAMETER  -  Invalid PciPolicy value.

**/
EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL            *This,
  OUT EFI_PCI_PLATFORM_POLICY              *PciPolicy
  )
;

/**

  Return a PCI ROM image for the onboard device represented by PciHandle.

  @param This       -  Protocol instance pointer.
  PciHandle  -  PCI device to return the ROM image for.
  RomImage   -  PCI Rom Image for onboard device.
  RomSize    -  Size of RomImage in bytes.

  @retval EFI_SUCCESS    -  RomImage is valid.
  @retval EFI_NOT_FOUND  -  No RomImage.

**/
EFI_STATUS
EFIAPI
GetPciRom (
  IN   CONST EFI_PCI_PLATFORM_PROTOCOL      *This,
  IN   EFI_HANDLE                           PciHandle,
  OUT  VOID                                 **RomImage,
  OUT  UINTN                                *RomSize
  )
;

/**

  Register a callback during PCI bus enumeration

  @param This       -  Protocol instance pointer.
  @param Function   -  Callback function pointer.
  @param Phase      -  PCI enumeration phase.

  @retval EFI_SUCCESS           - Function has registed successfully
  @retval EFI_UNSUPPORTED       - The function has been regisered
  @retval EFI_InVALID_PARAMETER - The parameter is incorrect

**/
EFI_STATUS
EFIAPI
RegisterPciCallback (
  IN  EFI_PCI_CALLBACK_PROTOCOL               *This,
  IN  EFI_PCI_CALLBACK_FUNC                   Function,
  IN  EFI_PCI_ENUMERATION_PHASE               Phase
  )
;

#endif
