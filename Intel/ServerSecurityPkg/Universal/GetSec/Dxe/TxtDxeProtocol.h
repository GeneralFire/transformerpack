/** @file
  This file contains definitions so that the functions in TXT DXE
  Driver can be used by other modules.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2016 Intel Corporation. <BR>

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

#ifndef _TXT_DXE_PROTOCOL_H_
#define _TXT_DXE_PROTOCOL_H_

#include <Uefi.h>

//----------------------------------------------------------------------------------------------
#pragma pack (1)
//
EFI_STATUS
InstallTxtDxeProtocol (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );
//
//----------------------------------------------------------------------------------------------
// TXT_DXE_PROTOCOL_RESET_ESTABLISHMENT
// This function resets establishment bit.
// Arguments:	None
// Result:	EFI_SUCCESS		Success
//		Other			Error
//----------------------------------------------------------------------------------------------
//
typedef
EFI_STATUS
(EFIAPI *TXT_DXE_PROTOCOL_RESET_ESTABLISHMENT)(
  );
//
//----------------------------------------------------------------------------------------------
// TXT_DXE_PROTOCOL_RESET_AUX_INDEX
// This function resets aux index.
// Arguments:	None
// Result:	EFI_SUCCESS		Success
//		Other			Error
//----------------------------------------------------------------------------------------------
//
typedef
EFI_STATUS
(EFIAPI *TXT_DXE_PROTOCOL_RESET_AUX_INDEX)(
  );
//
//----------------------------------------------------------------------------------------------
// TXT_DXE_PROTOCOL: Used to provide an interface to TXT DXE Driver so that different functions
//		in TXT DXE Driver can be used by other modules.
//----------------------------------------------------------------------------------------------
//
typedef	struct	_TXT_DXE_PROTOCOL	TXT_DXE_PROTOCOL;
//
struct _TXT_DXE_PROTOCOL {
  TXT_DXE_PROTOCOL_RESET_ESTABLISHMENT	BiosAcResetEstablishmentBit;	// Reset Establishment Bit
  TXT_DXE_PROTOCOL_RESET_AUX_INDEX	BiosAcResetAuxIndex;		// Reset AUX Index
};
//
//----------------------------------------------------------------------------------------------
//
extern	EFI_GUID	gTxtDxeProtocolGuid;		// TXT DXE Protocol GUID
//
//----------------------------------------------------------------------------------------------
#pragma pack ()
//----------------------------------------------------------------------------------------------

#endif
