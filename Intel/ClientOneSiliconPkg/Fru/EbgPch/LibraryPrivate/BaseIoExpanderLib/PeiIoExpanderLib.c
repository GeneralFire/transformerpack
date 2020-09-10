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

#include <Uefi/UefiBaseType.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchIoExpander.h>
#include <Library/HobLib.h>
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
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfoPpi;

  if (IoExpanderInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PeiServicesLocatePpi (&gIoExpanderInfoPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **) &IoExpanderInfoPpi
                                 );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IoExpanderInfoPpi->Revision != IO_EXPANDER_INFO_REVISION) {
    return EFI_INCOMPATIBLE_VERSION;
  }

  *IoExpanderInfo = IoExpanderInfoPpi;
  return Status;
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
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;
  UINT8                 IoExpanderIndex;
  UINT8                 IoExpanderCount;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
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
  EFI_STATUS            Status;
  UINT8                 IoExpanderIndex;
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
  IN  UINT32             IoExpanderId
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId >= IO_EXPANDER_MAX) {
    return 0;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  return (UINTN) IoExpanderInfo->P2sbBar[IoExpanderId];
}

/**
  PchGetPmcBarByIoExpanderId return I/O Expander PMC BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander PMC BAR
**/
UINTN
PchGetPmcBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId >= IO_EXPANDER_MAX) {
    return 0;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  return (UINTN) IoExpanderInfo->PmcBar[IoExpanderId];
}

/**
  PchGetTcoBarByIoExpanderId return I/O Expander TCO BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander TCO BAR
**/
UINTN
PchGetTcoBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  return (UINTN) IoExpanderInfo->TcoBar[IoExpanderId];
}

/**
  PchGetTempBarByIoExpanderId return I/O Expander Temporary BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander Temporary BAR
**/
UINTN
PchGetTempBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  )
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  if (IoExpanderId >= IO_EXPANDER_MAX) {
    return 0;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  return (UINTN) IoExpanderInfo->TempBar[IoExpanderId];
}

/**
  PchGetSpiBarByIoExpanderId return I/O Expander SPI PCIe BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander SPI PCIe BAR
**/
UINTN
PchGetSpiBarByIoExpanderId (
  IN  UINT32             IoExpanderId
)
{
  EFI_STATUS            Status;
  IO_EXPANDER_INFO      *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  if (IoExpanderInfo->Valid[IoExpanderId] != TRUE) {
    return 0;
  }
  return (UINTN) IoExpanderInfo->SpiBar[IoExpanderId];
}

/**
  PchFillIoExpanderInfo fills the I/O Expander Info.
  Function fills in the settings for I/O Expander PCHs.

  @param[in] IoExpanderInfo             The pointer to I/O Expander Info PPI instance
  @param[in] IoExpanderId               I/O Expander Id (0 ... n - I/O Expander)
  @param[in] SocketId                   The CPU Socket Id
  @param[in] Segment                    The I/O Expander's Segment
  @param[in] Bus                        The I/O Expander's Bus
  @param[in] MmioBase                   The I/O Expander's Mmio Base

  @retval EFI_SUCCESS                   The I/O Expander Info is set.
  @retval EFI_INVALID_PARAMETER         Wrong params
**/
EFI_STATUS
EFIAPI
PchFillIoExpanderInfo (
  IN  IO_EXPANDER_INFO                 *IoExpanderInfo,
  IN  UINT32                           IoExpanderId,
  IN  UINT8                            SocketId,
  IN  UINT8                            Segment,
  IN  UINT8                            Bus,
  IN  UINT64                           MmioBase
  )
{
  if (IoExpanderInfo == NULL || IoExpanderId >= IO_EXPANDER_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  IoExpanderInfo->Valid[IoExpanderId] = TRUE;
  IoExpanderInfo->SocketId[IoExpanderId] = SocketId;
  IoExpanderInfo->Segment[IoExpanderId] = Segment;
  IoExpanderInfo->Bus[IoExpanderId] = Bus;
  IoExpanderInfo->P2sbBar[IoExpanderId] = PCH_IOE_PCR_BASE_ADDRESS (MmioBase);
  IoExpanderInfo->PmcBar[IoExpanderId] = PCH_IOE_PMC_BASE_ADDRESS (MmioBase);
  IoExpanderInfo->SpiBar[IoExpanderId] = PCH_IOE_SPI_BASE_ADDRESS (MmioBase);
  IoExpanderInfo->TempBar[IoExpanderId] = PCH_IOE_TEMP_BASE_ADDRESS (MmioBase);

  return EFI_SUCCESS;
}

/**
  PchInstallIoExpanderInfoHob installs IoExpanderInfo into HOB

  @retval EFI_SUCCESS                   The I/O Expander Info HOB is installed.
  @retval EFI_OUT_OF_RESOURCES          Out of memory
**/
EFI_STATUS
EFIAPI
PchInstallIoExpanderInfoHob (
  VOID
  )
{
  EFI_STATUS                Status;
  VOID                      *HobPtr;
  IO_EXPANDER_INFO          *IoExpanderInfo;

  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HobPtr = BuildGuidDataHob (&gIoExpanderInfoHobGuid, IoExpanderInfo, sizeof (IO_EXPANDER_INFO));
  if (HobPtr == 0) {
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}
