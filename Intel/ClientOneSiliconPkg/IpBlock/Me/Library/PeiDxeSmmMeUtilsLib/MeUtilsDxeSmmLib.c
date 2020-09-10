/** @file
  HECI utilities for use in DXE and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/

#include <PiPei.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/HeciProtocol.h>
#include <Ppi/HeciControlPpi.h>
#include <Register/MeRegs.h>
#include <MeFwHob.h>

STATIC BOOLEAN mInSmm = FALSE;
STATIC EFI_SMM_SYSTEM_TABLE2 *mSmmSystemTable2 = NULL;

/**
  Get HECI PCI Segment

  @retval HECI PCI Segment
**/
UINTN
GetHeciPciSegment (
  VOID
  )
{
  HECI_ACCESS              *HeciAccessProtocol;
  EFI_STATUS               Status;
  UINTN                    PciSegment;
  EFI_SMM_SYSTEM_TABLE2    *Smst2;
  BOOLEAN                  InSmm;

  InSmm = IsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciAccessSmmProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
    } else {
      Status = EFI_PROTOCOL_ERROR;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciAccessProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
  }
  if (EFI_ERROR (Status)) {
    if (!InSmm) {
      Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
      if (EFI_ERROR (Status)) {
        // There is no Legacy HECI, so print error
        DEBUG ((DEBUG_ERROR, "[ME] ERROR: (DXE) Cannot locate HECI protocol for segment(%r)\n", Status));
      }
    }
    return ME_SEGMENT;
  }

  Status = HeciAccessProtocol->PciIo.GetLocation (
                                       &(HeciAccessProtocol->PciIo),
                                       &PciSegment,
                                       NULL,
                                       NULL,
                                       NULL
                                       );
  ASSERT_EFI_ERROR (Status);

  return PciSegment;
}

/**
  Get HECI PCI Bus

  @retval HECI PCI Bus
**/
UINTN
GetHeciPciBus (
  VOID
  )
{
  HECI_ACCESS              *HeciAccessProtocol;
  EFI_STATUS               Status;
  UINTN                    PciBus;
  EFI_SMM_SYSTEM_TABLE2    *Smst2;
  BOOLEAN                  InSmm;

  InSmm = IsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciAccessSmmProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
    } else {
      Status = EFI_PROTOCOL_ERROR;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciAccessProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
  }
  if (EFI_ERROR (Status)) {
    if (!InSmm) {
      Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
      if (EFI_ERROR (Status)) {
        // There is no Legacy HECI, so print error
        DEBUG ((DEBUG_ERROR, "[ME] ERROR: (DXE) Cannot locate HECI protocol for bus(%r)\n", Status));
      }
    }
    return ME_BUS;
  }

  Status = HeciAccessProtocol->PciIo.GetLocation (
                                       &(HeciAccessProtocol->PciIo),
                                       NULL,
                                       &PciBus,
                                       NULL,
                                       NULL
                                       );
  ASSERT_EFI_ERROR (Status);

  return PciBus;
}

/**
  Get HECI PCI Device Number

  @retval HECI PCI Device Number
**/
UINTN
GetHeciPciDevice (
  VOID
  )
{
  HECI_ACCESS            *HeciAccessProtocol;
  EFI_STATUS             Status;
  UINTN                  PciDevice;
  EFI_SMM_SYSTEM_TABLE2  *Smst2;
  BOOLEAN                InSmm;

  InSmm = IsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciAccessSmmProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
    } else {
      Status = EFI_PROTOCOL_ERROR;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciAccessProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
  }
  if (EFI_ERROR (Status)) {
    if (!InSmm) {
      Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &HeciAccessProtocol);
      if (EFI_ERROR (Status)) {
        // There is no Legacy HECI, so print error
        DEBUG ((DEBUG_ERROR, "[ME] ERROR: (DXE) Cannot locate HECI protocol for device(%r)\n", Status));
      }
    }
    return ME_DEVICE_NUMBER;
  }

  Status = HeciAccessProtocol->PciIo.GetLocation (
                                       &(HeciAccessProtocol->PciIo),
                                       NULL,
                                       NULL,
                                       &PciDevice,
                                       NULL
                                       );
  ASSERT_EFI_ERROR (Status);

  return PciDevice;
}

/**
  Reads ME Firmware Status 1 information from HOB

  @param [in] Mefs1          Current Mefs1

  @retval Mefs1              If MeFwHob exists, return Firmware status 1 stored in HOB;
                             otherwise, return the original value directly.
**/
UINT32
GetMeFwsts1FromHob (
  IN UINT32 Mefs1
  )
{
  ME_FW_HOB           *MeFwHob;

  MeFwHob = GetFirstGuidHob (&gMeFwHobGuid);
  if (MeFwHob != NULL) {
    if (MeFwHob->Group[0].FunNumber == HECI1_DEVICE) {
      return MeFwHob->Group[0].Reg[0];
    } else {
      ASSERT (MeFwHob->Group[0].FunNumber == HECI1_DEVICE);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "HECI: %a() Can't read correctly MeFwHob info\n", __FUNCTION__));
  }

  return Mefs1;
}

/**
  Gets Heci Protocol, returns pointer to HECI Protocol

  @param[out] HeciVersion       HECI interface API Version

  @retval Valid pointer         Pointer to HECI Protocol
  @retval NULL                  Error, Protocol not found
**/
VOID*
GetHeci (
  OUT HECI_API_VERSION *HeciVersion
  )
{
  EFI_STATUS             Status;
  VOID                   *Heci;
  HECI_API_VERSION       HeciApiVersion;
  EFI_SMM_SYSTEM_TABLE2  *Smst2;
  BOOLEAN                InSmm;

  HeciApiVersion = HeciApiUnknown;
  Heci = NULL;
  InSmm = IsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &Heci);
    } else {
      Status = EFI_PROTOCOL_ERROR;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &Heci);
  }
  if (!EFI_ERROR (Status)) {
    HeciApiVersion = HeciApiHeciControl;
  } else {
    if (InSmm) {
      if (Smst2 != NULL) {
        Status = Smst2->SmmLocateProtocol (&gSmmHeciProtocolGuid, NULL, &Heci);
      } else {
        Status = EFI_PROTOCOL_ERROR;
      }
    } else {
      Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, &Heci);
    }
    if (!EFI_ERROR (Status)) {
      HeciApiVersion = HeciApiHeciProtocol;
    }
  }
  if (HeciVersion != NULL) {
    *HeciVersion = HeciApiVersion;
  }

  return Heci;
}

/**
  Wrapper for Heci HeciSendAndReceive HECI_CONTROL/HECI_PROTOCOL function

  @param[in]     This          Heci protocol instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     Retries       The number of retries requested (0 => no retry)
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes
  @param[in,out] SendStatus    Send operation status
  @param[in,out] ReceiveStatus Receive operation status

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
HeciSendWithAck (
  IN     VOID                *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout,
  IN     UINT8               Retries,
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *ReqMsg,
  IN     UINT32              ReqLen,
     OUT UINT32              *RspMsg,
  IN OUT UINT32              *RspLen,
  IN OUT EFI_STATUS          *SendStatus,
  IN OUT EFI_STATUS          *ReceiveStatus
  )
{
  EFI_STATUS       Status;
  HECI_CONTROL     *HeciControl;
  HECI_PROTOCOL    *HeciProtocol;
  HECI_API_VERSION HeciApiVersion;

  if (This == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }
  if (GetHeci (&HeciApiVersion) != This) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  if (HeciApiVersion == HeciApiHeciControl) {
    HeciControl = (HECI_CONTROL*) This;
    Status = HeciControl->HeciSendAndReceive (
                            HeciControl,
                            HeciPchId,
                            Timeout,
                            Retries,
                            HostAddress,
                            EngineAddress,
                            ReqMsg,
                            ReqLen,
                            RspMsg,
                            RspLen,
                            SendStatus,
                            ReceiveStatus
                            );
  } else if (HeciApiVersion == HeciApiHeciProtocol) {
    HeciProtocol = (HECI_PROTOCOL*) This;
    Status = HeciProtocol->SendwAck (
                             HECI1_DEVICE,
                             ReqMsg,
                             ReqLen,
                             RspLen,
                             HostAddress,
                             EngineAddress
                             );
  } else {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**
  Get Information is we are running in SMM or not

  @param[in, out]         Pointer to EFI_SMM_SYSTEM_TABLE2 pointer

  @retval  TRUE           We are in SMM
  @retval  FALSE          We are not in SMM
**/
BOOLEAN
IsInSmm (
  IN OUT EFI_SMM_SYSTEM_TABLE2  **SmmSystemTable2
  )
{
  if (SmmSystemTable2 != NULL) {
    *SmmSystemTable2 = mSmmSystemTable2;
  }

  return mInSmm;
}

/**
  The constructor function for ME Utils Library.

  The functions saves information about if the bios is running in SMM and pointer to
  SmmSystemTable2 in modules variables. These variables are used in IsInSmm function.

  @retval EFI_SUCCESS   The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MeUtilsLibConstructor (
  VOID
)
{
  EFI_STATUS                     Status;
  EFI_SMM_BASE2_PROTOCOL         *SmmBase2;
  EFI_SMM_SYSTEM_TABLE2          *Smst2;

  // Check if we are in 64bit mode,
  // and we can check for SMM
  if (sizeof (UINTN) == sizeof (UINT32)) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &SmmBase2
                  );
  if (EFI_ERROR (Status)) {
    mInSmm = FALSE;
    return EFI_SUCCESS;
  }

  Smst2 = NULL;
  mInSmm = FALSE;
  SmmBase2->InSmm (
              SmmBase2,
              &mInSmm
              );
  if (mInSmm) {
    Status = SmmBase2->GetSmstLocation (
                         SmmBase2,
                         &Smst2
                         );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      mSmmSystemTable2 = Smst2;
    }
  }

  return EFI_SUCCESS;
}

/**
  Function forces a reinit of the heci interface by following the reset heci interface via host algorithm
  in HPS 0.90 doc 4-17-06

  @retval EFI_NOT_FOUND           Can't find HECI PPI
  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_SUCCESS             Interface reset
**/
EFI_STATUS
ResetHeci1Interface (
  VOID
  )
{
  HECI_CONTROL           *Heci;
  HECI_PROTOCOL          *HeciProtocol;
  EFI_STATUS             Status;
  EFI_SMM_SYSTEM_TABLE2  *Smst2;
  BOOLEAN                InSmm;

  InSmm = IsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, (VOID**) &Heci);
      if (!EFI_ERROR (Status)) {
        Status = Heci->HeciReset (
                            Heci,
                            HECI_DEFAULT_PCH_ID,
                            NULL
                            );
      } else {
        Status = Smst2->SmmLocateProtocol (&gSmmHeciProtocolGuid, NULL, (VOID**) &HeciProtocol);
        if (!EFI_ERROR (Status)) {
          Status = HeciProtocol->ResetHeci (HECI1_DEVICE);
        }
      }
    } else {
      Status = EFI_PROTOCOL_ERROR;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &Heci);
    if (!EFI_ERROR (Status)) {
      Status = Heci->HeciReset (
                          Heci,
                          HECI_DEFAULT_PCH_ID,
                          NULL
                          );
    } else {
      Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &HeciProtocol);
      if (!EFI_ERROR (Status)) {
        Status = HeciProtocol->ResetHeci (HECI1_DEVICE);
      }
    }
  }

  return Status;
}