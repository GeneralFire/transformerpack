/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2019 Intel Corporation. <BR>

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

#include "TxtDxeLib.h"
#include "TxtDxeProtocol.h"

//----------------------------------------------------------------------------
extern	LT_DXE_LIB_CONTEXT          mLtDxeCtx;

//----------------------------------------------------------------------------
///////////////////////	TXT_DXE_PROTOCOL /////////////////////////////////////
//----------------------------------------------------------------------------
//
TXT_DXE_PROTOCOL		mTxtDxeProtocol;	// TXT DXE Protocol
//
//----------------------------------------------------------------------------
/**
  BiosAcResetEstablishmentBit(): Reset Establishment Bit.

  @param[in] None

  @retval EFI_SUCCESS            Successful
  @retval Other                  Error

**/
//----------------------------------------------------------------------------
EFI_STATUS
EFIAPI
BiosAcResetEstablishmentBit (
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  return (ResetTpmEstBit (&mLtDxeCtx));
}

//----------------------------------------------------------------------------
/**
  BiosAcResetAuxIndex(): Reset Aux Index.

  @param[in] None

  @retval EFI_SUCCESS            Successful
  @retval Other                  Error

**/
//----------------------------------------------------------------------------
EFI_STATUS
EFIAPI
BiosAcResetAuxIndex (
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  return (ClearTpmAuxIndex (&mLtDxeCtx));
}

//----------------------------------------------------------------------------
/**
  InstallTxtDxeProtocol(): Install TXT DXE PROTOCOL.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS            Successful
  @retval Other                  Error

**/
//----------------------------------------------------------------------------
EFI_STATUS
InstallTxtDxeProtocol (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Local data
  EFI_STATUS        Status;
  TXT_DXE_PROTOCOL	*Tdp;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  // Check if TXT_DXE_PROTOCOL is already installed
  Tdp = NULL;
  Status = SystemTable->BootServices->LocateProtocol (			// Locate protocol
		&gTxtDxeProtocolGuid,
		NULL,
		(VOID**) &Tdp);
  if (Status == EFI_SUCCESS) { return EFI_SUCCESS; }			// If already installed -> Success
  if (Tdp != NULL) { return EFI_DEVICE_ERROR; }				// If not installed but protocol non-NULL -> Error

  // Init and Install TXT DXE Protocol
  mTxtDxeProtocol.BiosAcResetEstablishmentBit =	BiosAcResetEstablishmentBit; // Ptr to USB_CORE_INFO_STRUC
  mTxtDxeProtocol.BiosAcResetAuxIndex = BiosAcResetAuxIndex;		// Size of available USB Core Information
  Status = SystemTable->BootServices->InstallProtocolInterface (
		&ImageHandle,
		&gTxtDxeProtocolGuid,
		EFI_NATIVE_INTERFACE,
		&mTxtDxeProtocol);
  if (Status != EFI_SUCCESS) { return Status; }				// If not successful -> Error

  // Ensure protocol is installed
  Tdp = NULL;
  Status = SystemTable->BootServices->LocateProtocol (			// Locate protocol
		&gTxtDxeProtocolGuid,
		NULL,
		(VOID**) &Tdp);
  if (Status == EFI_SUCCESS) {						// If success
    if (Tdp != &mTxtDxeProtocol) { return EFI_DEVICE_ERROR; }		//   If not match -> Error
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

//----------------------------------------------------------------------------

