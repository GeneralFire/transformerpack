/** @file
  MeSmbiosTablesUpdateProtocol definitions

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
#ifndef _ME_SMBIOS_UPDATE_PROTOCOL_H_
#define _ME_SMBIOS_UPDATE_PROTOCOL_H_

///
/// ME SMBIOS Update Protocol
///

/**
  This function is used to update ME SMBIOS table.

  @return Standard EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *ME_UPDATE_SMBIOS) (
  VOID
  );

/**
  ME SMBIOS Tables update protocol
  The interface functions are to update SMBIOS type 130 (in case of AMT) and 131.
  It is platform vendor's responsibility to implement the function instance.
**/
typedef struct {
  ME_UPDATE_SMBIOS                UpdateSmbios130;
  ME_UPDATE_SMBIOS                UpdateSmbios131;
} ME_SMBIOS_TABLES_UPDATE_PROTOCOL;

extern EFI_GUID gMeSmbiosTablesUpdateProtocolGuid;

#endif // _ME_SMBIOS_UPDATE_PROTOCOL_H_
