/** @file
  The VGA Mini Port Protocol used to set the text display mode of a VGA controller.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2010 Intel Corporation. <BR>

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

#ifndef __VGA_MINI_PORT_H_
#define __VGA_MINI_PORT_H_

///
/// Global ID for the EFI_VGA_MINI_PORT_PROTOCOL.
///
#define EFI_VGA_MINI_PORT_PROTOCOL_GUID \
  { \
    0xc7735a2f, 0x88f5, 0x4882, {0xae, 0x63, 0xfa, 0xac, 0x8c, 0x8b, 0x86, 0xb3 } \
  }

///
/// Forward declaration for the EFI_VGA_MINI_PORT_PROTOCOL.
///
typedef struct _EFI_VGA_MINI_PORT_PROTOCOL  EFI_VGA_MINI_PORT_PROTOCOL;

/**
  Sets the text display mode of a VGA controller.

  Sets the text display mode of the VGA controller to the mode specified by
  ModeNumber.  A ModeNumber of 0 is a request for an 80x25 text mode.  A
  ModeNumber of 1 is a request for an 80x50 text mode.  If ModeNumber is greater
  than MaxModeNumber, then EFI_UNSUPPORTED is returned.  If the VGA controller
  is not functioning properly, then EFI_DEVICE_ERROR is returned.  If the VGA
  controller is sucessfully set to the mode number specified by ModeNumber, then
  EFI_SUCCESS is returned.

  @param[in] This         A pointer to the EFI_VGA_MINI_PORT_PROTOCOL instance.
  @param[in] ModeNumber   The requested mode number.  0 for 80x25.  1 for 80x5.

  @retval EFI_SUCCESS        The mode number was set.
  @retval EFI_UNSUPPORTED    The mode number specified by ModeNumber is not supported.
  @retval EFI_DEVICE_ERROR   The device is not functioning properly.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_VGA_MINI_PORT_SET_MODE)(
  IN EFI_VGA_MINI_PORT_PROTOCOL  *This,
  IN UINTN                        ModeNumber
  );

struct _EFI_VGA_MINI_PORT_PROTOCOL {
  EFI_VGA_MINI_PORT_SET_MODE  SetMode;
  ///
  /// MMIO base address of the VGA text mode framebuffer.  Typically set to 0xB8000.
  ///
  UINT64                      VgaMemoryOffset;
  ///
  /// I/O Port address for the VGA CRTC address register. Typically set to 0x3D4.
  ///
  UINT64                      CrtcAddressRegisterOffset;
  ///
  /// I/O Port address for the VGA CRTC data register.  Typically set to 0x3D5.
  ///
  UINT64                      CrtcDataRegisterOffset;
  ///
  /// PCI Controller MMIO BAR index of the VGA text mode frame buffer.  Typically
  /// set to EFI_PCI_IO_PASS_THROUGH_BAR
  ///
  UINT8                       VgaMemoryBar;
  ///
  /// PCI Controller I/O BAR index of the VGA CRTC address register.  Typically
  /// set to EFI_PCI_IO_PASS_THROUGH_BAR
  ///
  UINT8                       CrtcAddressRegisterBar;
  ///
  /// PCI Controller I/O BAR index of the VGA CRTC data register.  Typically set
  /// to EFI_PCI_IO_PASS_THROUGH_BAR
  ///
  UINT8                       CrtcDataRegisterBar;
  ///
  /// The maximum number of text modes that this VGA controller supports.
  ///
  UINT8                       MaxMode;
};

extern EFI_GUID gEfiVgaMiniPortProtocolGuid;

#endif
