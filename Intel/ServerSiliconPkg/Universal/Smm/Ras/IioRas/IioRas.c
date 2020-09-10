/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2019 Intel Corporation. <BR>

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

//
// Includes
//
#include <Library/MemoryCoreLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/IioRasProtocol.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/IioUds.h>
#include <Library/MmPciBaseLib.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <SysRegs.h>
#include <Ras/RasStateMachine.h>
#include "UncoreCommonIncludes.h"
#include <Register/PchRegsPmc.h>

#define IIO_RAS_SIGNATURE  SIGNATURE_32 ('R', 'I', 'I', 'O')

EFI_IIO_RAS_PROTOCOL      *mIioRasProtocol;
BIOS_ACPI_PARAM           *mAcpiParameter;
EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess;
static EFI_IIO_UDS_PROTOCOL            *mIioUds;

extern EFI_GUID gEfiIioRasProtocolGuid;

/**

  GenerateRasSci() generates an SCI interrupt for Ras OS Consumption

  @param  None.

  @retval EFI_STATUS EFI_SUCCESS - Success.
  @retval other - failure.

**/
EFI_STATUS
GenerateRasSci (
    VOID
)
{
  UINT32 GpeCtrl = 0;
  UINT16 PmBase = 0;

  // PM base address
  PmBase = MmioRead16 (PmcGetPwrmBase () + PCH_PMC_ACPI_BASE);
  PmBase = (UINT16) (PmBase & 0xFF80);

  // Generate SCI event
  gSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT32, (PmBase + R_ACPI_IO_GPE_CNTL), 1, &GpeCtrl);
  GpeCtrl |= B_ACPI_IO_GPE_CNTL_SWGPE_CTRL;
  gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, (PmBase + R_ACPI_IO_GPE_CNTL), 1, &GpeCtrl);
  return EFI_SUCCESS;
}

/**

  IioOnline() is responsible for initializing the Incoming Socket's IIO.

  @param  Socket - Denotes the "Incoming Socket" number. i.e. The socket number that is "being online'd"

  @retval EFI_STATUS EFI_SUCCESS - Success.
  @retval other - failure.

**/
EFI_STATUS
EFIAPI
IioOnline (
  IN UINT8 Socket
  )
{
  return EFI_SUCCESS;
}

/**

  IioOffline() is responsible for "taking offline" the Outgoing Socket's IIO.
  It utilizes the DXE IioInit Library.

  @param  Socket - Denotes the "Outgoing Socket" number. i.e. The socket number that is "being offline'd"

  @retval EFI_STATUS STATUS_DONE - Success.
  @retval other - failure.

**/
EFI_STATUS
EFIAPI
IioOffline (
  IN UINT8 Socket
  )
{
  DEBUG ((EFI_D_INFO | EFI_D_ERROR, "    IIO Offline Flow In Progress.\n"));
  //
  // Send Pcode command to disable IIO
  //
  //IioPortPowerDown(mIioGlobalData);
  //mAcpiParameter->IioPresentBitMask &= ~(1 << Socket);
  //mAcpiParameter->IioChangeMask &= (~1 << (Socket-1));
  //mAcpiParameter->IoApicEnable = 0;
  // Request OS check the bus/device
  //mAcpiParameter->SciRequestParam[0]   = SCI_CHECK;
  //mAcpiParameter->SciRequestParam[1]   = 0;
  //mAcpiParameter->SciRequestParam[2]   = 0;
  //mAcpiParameter->SciRequestParam[3]   = 0;

  // Generate SCI
  GenerateRasSci();

  DEBUG ((EFI_D_INFO | EFI_D_ERROR, "    IIO Offline Flow  Done.\n"));
  return STATUS_DONE;
}

//
//  Driver entry point
//
EFI_STATUS
EFIAPI
IioRasDriverEntry (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
    )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_HANDLE                    Handle = NULL;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *AcpiNvsProtocol = NULL;
  // If IIO RAS driver is already initialized, then no further action needed.
  Status = gSmst->SmmLocateProtocol (&gEfiIioRasProtocolGuid, NULL, &mIioRasProtocol);
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  //
  // Allocate the protocol instance and install the protocol
  //
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_IIO_RAS_PROTOCOL), &mIioRasProtocol);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mIioRasProtocol, sizeof (EFI_IIO_RAS_PROTOCOL));

  //
  // Init Drive Protocols for other SMM driver to call
  //
  mIioRasProtocol->IioOnline = IioOnline;
  mIioRasProtocol->IioOffline = IioOffline;

  //
  // Install IIO RAS protocol
  //
  Status = gSmst->SmmInstallProtocolInterface (&Handle, &gEfiIioRasProtocolGuid, EFI_NATIVE_INTERFACE, mIioRasProtocol);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate needed protocols
  //
  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &AcpiNvsProtocol);
  ASSERT_EFI_ERROR (Status);
  mAcpiParameter = AcpiNvsProtocol->Area;
  ASSERT (mAcpiParameter);

  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&mIioUds);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

