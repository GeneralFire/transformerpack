/** @file
  This file is I/O Expander library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PchIoExpander.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Register/PmcRegs.h>
#include <Register/SpiRegs.h>
#include <Register/P2sbRegs.h>
#include <PchReservedResources.h>

#define SOCKET_ID_0                     0

/**
  PchGetIoExpanderInfo return I/O Expander Info pointer.

  @param[in, out] IoExpanderInfo        The pointer to I/O Expander Info

  @retval EFI_SUCCESS                   The I/O Expander Info is returned.
  @retval EFI_NOT_FOUND                 Unable to find I/O Expander Info PPI
  @retval EFI_INCOMPATIBLE_VERSION      Found I/O Expander Info PPI is not compatible
  @retval EFI_INVALID_PARAMETER         Wrong params
**/
EFI_STATUS
PchGetIoExpanderInfo (
  IN OUT IO_EXPANDER_INFO       **IoExpanderInfo
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  IO_EXPANDER_INFO      *IoExpanderInfoHob;

  if (IoExpanderInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&gIoExpanderInfoHobGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  IoExpanderInfoHob = (IO_EXPANDER_INFO *) GET_GUID_HOB_DATA (GuidHob);

  if (IoExpanderInfoHob->Revision != IO_EXPANDER_INFO_REVISION) {
    return EFI_INCOMPATIBLE_VERSION;
  }

  *IoExpanderInfo = IoExpanderInfoHob;
  return EFI_SUCCESS;
}

/**
  PchGetNumberDetectedIoExpanders return Number of detected I/O Expanders

  Number of I/O Expanders depends on boot UEFI phase.
  Before IIO is configued the number is always 0

  @retval                       Number of detected I/O Expanders
**/
UINT8
PchGetNumberDetectedIoExpanders (
  VOID
  )
{
  UINT8                 IoExpanderIndex;
  UINT8                 IoExpanderCount;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }

  IoExpanderCount = 0;
  for (IoExpanderIndex = 0; IoExpanderIndex < IO_EXPANDER_MAX; IoExpanderIndex++) {
    if (IoExpanderInfo->Valid[IoExpanderIndex] == TRUE) {
      IoExpanderCount++;
    }
  }
  return IoExpanderCount;
}

/**
  PchGetIoExpanderIdBySocketId return I/O Expander Id by given Socket Id

  @param[in]  SocketId           Socket Id
  @param[out] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval EFI_SUCCESS                   The I/O Expander Info is returned.
  @retval EFI_NOT_FOUND                 Unable to find I/O Expander Info PPI
  @retval EFI_INCOMPATIBLE_VERSION      Found I/O Expander Info PPI is not compatible
  @retval EFI_INVALID_PARAMETER         Wrong params
**/
EFI_STATUS
PchGetIoExpanderIdBySocketId (
  IN  UINT8             SocketId,
  OUT UINT32            *IoExpanderId
  )
{
  UINT8                 IoExpanderIndex;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId == NULL ||
      SocketId == SOCKET_ID_0 ||
      SocketId >= MAX_SOCKET) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (IoExpanderIndex = 0; IoExpanderIndex < IO_EXPANDER_MAX; IoExpanderIndex++) {
    if (IoExpanderInfo->Valid[IoExpanderIndex] != TRUE) {
      continue;
    }
    if (IoExpanderInfo->SocketId[IoExpanderIndex] == SocketId) {
      *IoExpanderId = IoExpanderIndex;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  PchGetSegmentByIoExpanderId return I/O Expander segment number

  @param[in]  IoExpanderId           I/O Expander Id (0 ... n - I/O Expander)
  @param[out] Segment                I/O Expander segment number

  @retval EFI_SUCCESS                   The I/O Expander segment is returned.
  @retval EFI_NOT_FOUND                 Unable to find I/O Expander
  @retval EFI_INCOMPATIBLE_VERSION      Found I/O Expander Info PPI is not compatible
  @retval EFI_INVALID_PARAMETER         Wrong params
**/
EFI_STATUS
PchGetSegmentByIoExpanderId (
  IN  UINT32             IoExpanderId,
  OUT UINT8              *Segment
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId >= IO_EXPANDER_MAX || Segment == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return EFI_NOT_FOUND;
  }

  *Segment = IoExpanderInfo->Segment[IoExpanderId];
  return Status;
}

/**
  PchGetBusByIoExpanderId return I/O Expander bus number

  @param[in]  IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)
  @param[out] Bus                I/O Expander bus number

  @retval EFI_SUCCESS                   The I/O Expander bus is returned.
  @retval EFI_NOT_FOUND                 Unable to find I/O Expander
  @retval EFI_INCOMPATIBLE_VERSION      Found I/O Expander Info PPI is not compatible
  @retval EFI_INVALID_PARAMETER         Wrong params
**/
EFI_STATUS
PchGetBusByIoExpanderId (
  IN  UINT32            IoExpanderId,
  OUT UINT8             *Bus
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId >= IO_EXPANDER_MAX || Bus == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return EFI_NOT_FOUND;
  }

  *Bus = IoExpanderInfo->Bus[IoExpanderId];
  return Status;
}

/**
  PchGetP2sbBarByIoExpanderId return I/O Expander P2SB BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander P2SB BAR
**/
UINTN
PchGetP2sbBarByIoExpanderId (
  IN  UINT32            IoExpanderId
  )
{
  UINT64                P2sbBase;
  UINT32                PcrBase;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    ASSERT (FALSE);
    return 0;
  }
  P2sbBase = P2sbPciCfgBaseByIoExpanderId (IoExpanderId);
  if (PciSegmentRead32 (P2sbBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PchGetP2sbBarByIoExpanderId: Device is hidden\n"));
    return (UINTN) IoExpanderInfo->P2sbBar[IoExpanderId];
  }
  PcrBase = PciSegmentRead32 (P2sbBase + R_P2SB_CFG_SBREG_BAR) & B_P2SB_CFG_SBREG_RBA;
  if (PcrBase != (UINTN) IoExpanderInfo->P2sbBar[IoExpanderId]) {
    DEBUG ((DEBUG_ERROR, "PchGetP2sbBarByIoExpanderId: Updating BAR\n"));
    IoExpanderInfo->P2sbBar[IoExpanderId] = PcrBase;
  }
  return (UINTN) PcrBase;
}

/**
  PchGetPmcBarByIoExpanderId return I/O Expander PMC BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander PMC BAR
**/
UINTN
PchGetPmcBarByIoExpanderId (
  IN  UINT32            IoExpanderId
  )
{
  UINT64                PmcBase;
  UINT32                PmcBar;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    ASSERT (FALSE);
    return 0;
  }
  PmcBase = PmcPciCfgBaseByIoExpanderId (IoExpanderId);
  if (PciSegmentRead32 (PmcBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PchGetPmcBarByIoExpanderId: Device is hidden\n"));
    return (UINTN) IoExpanderInfo->PmcBar[IoExpanderId];
  }
  PmcBar = PciSegmentRead32 (PmcBase + R_PMC_CFG_BASE) & B_PMC_CFG_PWRM_BASE_MASK;
  if (PmcBar != (UINTN) IoExpanderInfo->PmcBar[IoExpanderId]) {
    DEBUG ((DEBUG_ERROR, "PchGetPmcBarByIoExpanderId: Updating BAR\n"));
    IoExpanderInfo->PmcBar[IoExpanderId] = PmcBar;
  }
  return (UINTN) PmcBar;
}

/**
  PchGetAcpiBarByIoExpanderId return I/O Expander ACPI BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander ACPI BAR
**/
UINTN
PchGetAcpiBarByIoExpanderId (
  IN  UINT32            IoExpanderId
  )
{
  UINT64                PmcBase;
  UINT32                AcpiBar;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    ASSERT (FALSE);
    return 0;
  }
  PmcBase = PmcPciCfgBaseByIoExpanderId (IoExpanderId);
  if (PciSegmentRead32 (PmcBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PchGetAcpiBarByIoExpanderId: Device is hidden\n"));
    return (UINTN) IoExpanderInfo->AcpiBar[IoExpanderId];
  }
  AcpiBar = PciSegmentRead16 (PmcBase + R_PMC_CFG_BASE) & B_PMC_CFG_PWRM_BASE_MASK;
  if (AcpiBar != (UINTN) IoExpanderInfo->AcpiBar[IoExpanderId]) {
    DEBUG ((DEBUG_ERROR, "PchGetAcpiBarByIoExpanderId: Updating BAR\n"));
    IoExpanderInfo->AcpiBar[IoExpanderId] = AcpiBar;
  }
  return (UINTN) AcpiBar;
}

/**
  PchGetTcoBarByIoExpanderId return I/O Expander TCO BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander TCO BAR
**/
UINTN
PchGetTcoBarByIoExpanderId (
  IN  UINT32            IoExpanderId
  )
{
  UINT64                PmcBase;
  UINT16                TcoBar;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    ASSERT (FALSE);
    return 0;
  }
  PmcBase = PmcPciCfgBaseByIoExpanderId (IoExpanderId);
  if (PciSegmentRead32 (PmcBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PchGetTcoBarByIoExpanderId: Device is hidden\n"));
    return (UINTN) IoExpanderInfo->TcoBar[IoExpanderId];
  }
  TcoBar = PciSegmentRead16 (PmcBase + R_PMC_CFG_BASE) & B_PMC_CFG_PWRM_BASE_MASK;
  if (TcoBar != (UINTN) IoExpanderInfo->TcoBar[IoExpanderId]) {
    DEBUG ((DEBUG_ERROR, "PchGetTcoBarByIoExpanderId: Updating BAR\n"));
    IoExpanderInfo->TcoBar[IoExpanderId] = TcoBar;
  }
  return (UINTN) TcoBar;
}

/**
  PchGetSpiBarByIoExpanderId return I/O Expander SPI PCIe BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander SPI PCIe BAR
**/
UINTN
PchGetSpiBarByIoExpanderId (
  IN  UINT32            IoExpanderId
  )
{
  UINT64                SpiBase;
  UINT32                SpiBar;
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (Status == EFI_NOT_FOUND) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  SpiBase = SpiPciCfgBaseByIoExpanderId (IoExpanderId);
  if (PciSegmentRead32 (SpiBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PchGetSpiBarByIoExpanderId: Device is hidden\n"));
    return (UINTN) IoExpanderInfo->SpiBar[IoExpanderId];
  }
  SpiBar = PciSegmentRead32 (SpiBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK);
  if (SpiBar != (UINTN) IoExpanderInfo->SpiBar[IoExpanderId]) {
    DEBUG ((DEBUG_ERROR, "PchGetSpiBarByIoExpanderId: Updating BAR\n"));
    IoExpanderInfo->SpiBar[IoExpanderId] = SpiBar;
  }
  return (UINTN) SpiBar;
}
