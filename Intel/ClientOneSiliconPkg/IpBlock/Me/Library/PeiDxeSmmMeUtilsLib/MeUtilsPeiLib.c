/** @file
  HECI utilities for use in PEI.

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
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeUtilsLib.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/HeciControlPpi.h>
#include <Ppi/HeciPpi.h>
#include <Register/MeRegs.h>
#include <Register/HeciRegs.h>



/*****************************************************************************
 * Local definitions.
 *****************************************************************************/

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
  HECI_CONTROL  *HeciControl;
  HECI_PPI      *HeciPpi;
  EFI_STATUS    Status;

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             NULL,
             (VOID**) &HeciControl
             );
  if (!EFI_ERROR (Status)) {
    Status = HeciControl->HeciReset (
                            HeciControl,
                            HECI_DEFAULT_PCH_ID,
                            NULL
                            );
  } else {
    Status = PeiServicesLocatePpi (&gHeciPpiGuid, 0, NULL, &HeciPpi);
    if (!EFI_ERROR (Status)) {
      ASSERT (FALSE);
    }
  }

  return Status;
}


/**
  Get HECI PCI Segment

  @retval HECI PCI Segment
**/
UINTN
GetHeciPciSegment (
  VOID
  )
{
  HECI_ACCESS              *HeciAccessPpi;
  EFI_STATUS               Status;
  UINTN                    PciSegment;

  Status = PeiServicesLocatePpi (&gHeciAccessPpiGuid, 0, NULL, &HeciAccessPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: (PPI) Cannot locate HECI PPI for segment (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return ME_SEGMENT;
  }

  Status = HeciAccessPpi->PciIo.GetLocation (
                                  &(HeciAccessPpi->PciIo),
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
  HECI_ACCESS              *HeciAccessPpi;
  EFI_STATUS               Status;
  UINTN                    PciBus;

  Status = PeiServicesLocatePpi (&gHeciAccessPpiGuid, 0, NULL, &HeciAccessPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: (PPI) Cannot locate HECI PPI for bus(%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return ME_BUS;
  }

  Status = HeciAccessPpi->PciIo.GetLocation (
                                  &(HeciAccessPpi->PciIo),
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
  HECI_ACCESS            *HeciAccessPpi;
  EFI_STATUS             Status;
  UINTN                  PciDevice;

  Status = PeiServicesLocatePpi (&gHeciAccessPpiGuid, 0, NULL, &HeciAccessPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: (PPI) Cannot locate HECI PPI for device(%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return ME_DEVICE_NUMBER;
  }

  Status = HeciAccessPpi->PciIo.GetLocation (
                                  &(HeciAccessPpi->PciIo),
                                  NULL,
                                  NULL,
                                  &PciDevice,
                                  NULL
                                  );
  ASSERT_EFI_ERROR (Status);

  return PciDevice;
}

/**
  Gets Heci PPI, returns pointer to HECI PPI

  @param[out] HeciVersion       HECI interface implementation Id

  @retval Valid pointer         Pointer to HECI PPI
  @retval NULL                  Error, PPI not found
**/
VOID*
GetHeci (
  OUT HECI_API_VERSION *HeciVersion
  )
{
  EFI_STATUS       Status;
  VOID             *HeciPpi;
  HECI_API_VERSION HeciApiVersion;

  HeciApiVersion = HeciApiUnknown;
  HeciPpi = NULL;
  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &HeciPpi);
  if (!EFI_ERROR (Status)) {
    HeciApiVersion = HeciApiHeciControl;
  } else {
    Status = PeiServicesLocatePpi (&gHeciPpiGuid, 0, NULL, &HeciPpi);
    if (!EFI_ERROR (Status)) {
      HeciApiVersion = HeciApiHeciProtocol;
    }
  }
  if (HeciVersion != NULL) {
    *HeciVersion = HeciApiVersion;
  }

  return HeciPpi;
}

/**
  Wrapper for Heci HeciSendAndReceive HECI_CONTROL/HECI_PROTOCOL function

  @param[in]     This          Heci PPI instance
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
  @retval EFI_STATUS            Returned by HeciSendAndReceive
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
  HECI_PPI         *HeciPpi;
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
    HeciPpi = (HECI_PPI*) This;
    Status = HeciPpi->SendwAck (
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
    *SmmSystemTable2 = NULL;
  }

  return FALSE;
}

/**
  The constructor function for ME Utils Library.

  @retval EFI_SUCCESS   The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MeUtilsLibConstructor (
  VOID
)
{
  return EFI_SUCCESS;
}