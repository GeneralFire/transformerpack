/** @file
  PcatProtocol.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 Intel Corporation. <BR>

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

#ifndef _PCAT_PROTOCOL_H_
#define _PCAT_PROTOCOL_H_

extern EFI_GUID gAcpiPcatProtocolGuid;

//
// Type definitions
//

typedef
EFI_STATUS
(EFIAPI *EFI_PCAT_INITIALIZE_PCAT) (
  OUT UINT64* PcatTablePtr,
  IN  UINT64 PcatTableSize
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PCAT_CREATE_SAET) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PCAT_ADD_VLR_TO_SAET) (
  IN  UINT8   *Vlr,
  IN  UINTN   VlrSize
  );

typedef
BOOLEAN
(EFIAPI *EFI_PCAT_IS_INITIALIZED) (
  VOID
  );
///
/// Protocol definition
///
typedef struct {
  EFI_PCAT_INITIALIZE_PCAT  InitializeAcpiPcat;
  EFI_PCAT_CREATE_SAET      CreateAcpiPcatSaet;
  EFI_PCAT_ADD_VLR_TO_SAET  AddVlr2PcatSaet;
  EFI_PCAT_IS_INITIALIZED   IsInitialized;
} EFI_ACPI_PCAT_PROTOCOL;




#endif // _PCAT_PROTOCOL_H_

