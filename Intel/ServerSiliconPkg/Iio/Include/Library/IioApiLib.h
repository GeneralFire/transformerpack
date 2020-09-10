/** @file
  Prototypes for IIO Public API functions

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

#ifndef _IIO_API_LIB_H_
#define _IIO_API_LIB_H_

/**
  Get Vtd base address
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

  @retval    Vtd base address
**/
UINT32
EFIAPI
GetVtdBar (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**
  Get socket ending bus number
  @param[OUT] BusNumber - BusNumber
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index

  @retval    EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetSocketStackLimitBus (
  OUT UINT8     *BusNumber,
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**
  Get max port number every socket
  @param[in] SocId    - Socket Index
  @retval    Max port number per socket
**/
UINT8
EFIAPI
GetMaxPortPerSocket (
  IN UINT8     SocId
  );

/**
  Get the stack which the PCIe port belongs to
  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    Stack id
**/
UINT8
EFIAPI
GetStackPerPort (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  );

/**
  Get the bus number which the PCIe port belongs to
  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    bus number
**/
UINT8
EFIAPI
GetSocketPortBusNum (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  );

/**
  Get current PCIe port map
  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index

  @retval    Current PCIe port map
**/
UINT8
EFIAPI
GetCurrentPXPMap (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  );

/**
  Get PCIe port ownership
  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    PCIe port ownership
**/
UINT8
EFIAPI
GetPCIePortOwnerShip (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  );

/**
  Check if VMD is enabled for given IIO port.

  @param[in] IioIndex - IIO instance (usually socket)
  @param[in] IioPort  - IIO port index in IIO instance

  @return TRUE is returned if VMD is enabled for IIO port, otherwise FALSE.
**/
BOOLEAN
EFIAPI
IioVmdPortIsEnabled (
  IN  UINT8    IioIndex,
  IN  UINT8    IioPort
  );

/**
  Return VMD device location for IIO stack.

  @param[in] IioIndex   - IIO instance (usually socket).
  @param[in] IioStack   - IIO stack index in IIO instance.
  @param[in] PciDevPtr  - Pointer to a buffer for PCI device number of VMD device.
  @param[in] PciFuncPtr - Pointer to a buffer for PCI function number of VMD device.

  @retval EFI_INVALID_PARAMETER - Invalid IIO instance or stack.
  @retval EFI_NOT_STARTED       - VMD is not enabled in the given IIO stack.
  @retval EFI_SUCCESS           - VMD is enabled, device and function number updated.
**/
EFI_STATUS
EFIAPI
IioVmdGetPciLocation (
  IN  UINT8    IioIndex,
  IN  UINT8    IioStack,
  OUT UINT8   *PciDevPtr,
  OUT UINT8   *PciFuncPtr
  );

/**
  Returns TRUE if given PCH rootport can be assigned to VMD

  @param[in] IioIndex   - IIO instance (usually socket).
  @param[in] PortId     - Index of the PCH rootport.

  @return TRUE if given PCH rootport can be assigned to VMD
 */
BOOLEAN
EFIAPI
IioVmdIsPchPortAllowed (
  IN UINT8  IioIndex,
  IN UINT8  PortId
  );

/**
  Functions sets link params for given PCIE port.

  @param[in]   IioIndex          Socket Index
  @param[in]   PortIndex         Port Index
  @param[out]  LinkWidth         Buffer to return current link width
  @param[out]  MaxLinkWidth      Buffer to return max link width
  @param[out]  LinkSpeed         Buffer to return current link speed
 */
VOID
IioGetPciePortLinkStatus (
  IN  UINT8     IioIndex,
  IN  UINT8     PortIndex,
  OUT UINT8     *LinkWidth,
  OUT UINT8     *MaxLinkWidth,
  OUT UINT8     *LinkSpeed
  );

/**
  Return stack bus per socket
  @param[in] SocId      - Socket Index
  @param[in] StackId    - Stack Index

  @retval    Stack bus per socket

**/
UINT8
EFIAPI
GetSocketStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId
  );

/**
  Return HidePEXPMenu status
  @param[in] PortIndex  - setup control

  @retval    HidePEXPMenu

**/
UINT8
EFIAPI
GetHidePEXPMenu (
  IN UINT32     PortIndex
  );

/**
  This function handles WAs needed in Intel IPs for given Segment, Bus, Device, Function
  in early PCIe enumeration.

  @param[in] Segment                 - Segment
  @param[in] Bus                     - Bus
  @param[in] Device                  - Device of the virtual root port
  @param[in] Function                - Function of the virtual root port
  @param[in] DidVid                  - Device-Vendor ID in the given Bus, Device, Function.
**/
VOID
IioPciHookBeforeEnumeration (
  IN  UINT8          Segment,
  IN  UINT8          Bus,
  IN  UINT8          Device,
  IN  UINT8          Function,
  IN  UINT32         DidVid
  );
#endif // _IIO_API_LIB_H_
