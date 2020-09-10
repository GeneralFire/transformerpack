/** @file
  This protocol abstracts the PIRQ programming from the generic EFI Compatibility Support Modules (CSMs).

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2010 Intel Corporation. <BR>

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

  @par Revision Reference
  This protocol is defined in Framework for the EFI Compatibility Support Module specification.
  Version 0.97.
**/

#ifndef _EFI_LEGACY_INTERRUPT_H_
#define _EFI_LEGACY_INTERRUPT_H_


#define EFI_LEGACY_INTERRUPT_PROTOCOL_GUID \
  { \
    0x31ce593d, 0x108a, 0x485d, {0xad, 0xb2, 0x78, 0xf2, 0x1f, 0x29, 0x66, 0xbe } \
  }

typedef struct _EFI_LEGACY_INTERRUPT_PROTOCOL EFI_LEGACY_INTERRUPT_PROTOCOL;

/**
  Get the number of PIRQs this hardware supports.

  @param  This                  The protocol instance pointer.
  @param  NumberPirsq           The number of PIRQs that are supported.

  @retval EFI_SUCCESS           The number of PIRQs was returned successfully.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_INTERRUPT_GET_NUMBER_PIRQS)(
  IN EFI_LEGACY_INTERRUPT_PROTOCOL            *This,
  OUT UINT8                                   *NumberPirqs
  );

/**
  Gets the PCI location associated with this protocol.

  @param  This                  The Protocol instance pointer.
  @param  Bus                   The PCI Bus.
  @param  Device                The PCI Device.
  @param  Function              The PCI Function.

  @retval EFI_SUCCESS           The Bus, Device, and Function were returned successfully.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_INTERRUPT_GET_LOCATION)(
  IN EFI_LEGACY_INTERRUPT_PROTOCOL            *This,
  OUT UINT8                                   *Bus,
  OUT UINT8                                   *Device,
  OUT UINT8                                   *Function
  );

/**
  Read the PIRQ register and return the data

  @param  This                  The protocol instance pointer.
  @param  PirqNumber            The PIRQ register to read.
  @param  PirqData              The data read.

  @retval EFI_SUCCESS           The data was read.
  @retval EFI_INVALID_PARAMETER Invalid PIRQ number.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_INTERRUPT_READ_PIRQ)(
  IN EFI_LEGACY_INTERRUPT_PROTOCOL           *This,
  IN  UINT8                                  PirqNumber,
  OUT UINT8                                  *PirqData
  );

/**
  Write the specified PIRQ register with the given data.

  @param  This                  The protocol instance pointer.
  @param  PirqNumber            A PIRQ register to read.
  @param  PirqData              The data to write.

  @retval EFI_SUCCESS           The PIRQ was programmed.
  @retval EFI_INVALID_PARAMETER Invalid PIRQ number.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_INTERRUPT_WRITE_PIRQ)(
  IN EFI_LEGACY_INTERRUPT_PROTOCOL           *This,
  IN  UINT8                                  PirqNumber,
  IN UINT8                                   PirqData
  );

struct _EFI_LEGACY_INTERRUPT_PROTOCOL {
  ///
  ///   Gets the number of PIRQs supported.
  ///
  EFI_LEGACY_INTERRUPT_GET_NUMBER_PIRQS GetNumberPirqs;

  ///
  /// Gets the PCI bus, device, and function that is associated with this protocol.
  ///
  EFI_LEGACY_INTERRUPT_GET_LOCATION     GetLocation;

  ///
  /// Reads the indicated PIRQ register.
  ///
  EFI_LEGACY_INTERRUPT_READ_PIRQ        ReadPirq;

  ///
  /// Writes to the indicated PIRQ register.
  ///
  EFI_LEGACY_INTERRUPT_WRITE_PIRQ       WritePirq;
};

extern EFI_GUID gEfiLegacyInterruptProtocolGuid;

#endif
