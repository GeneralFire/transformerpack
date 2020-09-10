/** @file
  ME HECI Access Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

@copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by the
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
  This file contains an 'Intel Peripheral Driver' and is uniquely
  identified as "Intel Reference Module" and is licensed for Intel
  CPUs and chipsets under the terms of your license agreement with
  Intel or your vendor. This file may be modified by the user, subject
  to additional terms of the license agreement.

**/
#ifndef __HECI_ACCESS_PROTOCOL_H__
#define __HECI_ACCESS_PROTOCOL_H__

#include <Uefi.h>
#include <Protocol/PciIo.h>

/**
  ME HECI Protocol GUID
  This Protocol provides an interface to communicate with Intel ME in DXE/SMM phase.
**/
extern EFI_GUID gHeciAccessProtocolGuid;
extern EFI_GUID gHeciAccessSmmProtocolGuid;

typedef struct _HECI_ACCESS_ HECI_ACCESS;

/**
  Get HECI Access Parameter

  @param[in]     This            Pointer to HECI Access device context structure
  @param[in,out] HeciAccessParam Pointer to HECI Access parameter

  @retval Operation EFI_STATUS
**/
typedef EFI_STATUS (EFIAPI *HECI_ACCESS_GET_PARAMETER) (
  IN      HECI_ACCESS   *This,
  IN OUT  UINTN         *HeciAccessParam
  );

/**
  Get Next HECI Access

  @param[in]     This          Pointer to HECI Access device context structure
  @param[in]     HeciAccess    Pointer to HECI Access device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_ACCES Next HeciAccess object
**/
typedef HECI_ACCESS* (EFIAPI *HECI_ACCESS_GET_NEXT_HECI) (
  IN      HECI_ACCESS   *This,
  IN      HECI_ACCESS   *HeciAccess
  );

/**
  PPI/PROTOCOL for ME-UEFI FW HECI Access.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  hardware access.
**/
typedef struct _HECI_ACCESS_ {
  EFI_PCI_IO_PROTOCOL               PciIo;               /// < PciIo R/W HECI PCIe device
  HECI_ACCESS_GET_NEXT_HECI         GetNextHeciAccess;   /// < HECI access protocol walker
  // HECI configuration access
  HECI_ACCESS_GET_PARAMETER         GetHeciPch;          /// < What PCH HECI belongs to
  HECI_ACCESS_GET_PARAMETER         GetHeciInterruptMode;/// < Default HECI Interrupt mode to be used
} HECI_ACCESS;

#endif // __HECI_ACCESS_PROTOCOL_H__
