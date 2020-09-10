/** @file
  Prototype of the I/O Expander library.

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

#ifndef _PCH_IO_EXPANDER_H_
#define _PCH_IO_EXPANDER_H_

#define IO_EXPANDER_MAX                3
#define IO_EXPANDER_INFO_REVISION      1

typedef struct {
  /**
    Revision 1:   Original version
  **/
  UINT8                        Revision;

  BOOLEAN                      Valid[IO_EXPANDER_MAX];
  UINT8                        SocketId[IO_EXPANDER_MAX];
  UINT8                        Segment[IO_EXPANDER_MAX];
  UINT8                        Bus[IO_EXPANDER_MAX];
  UINT64                       P2sbBar[IO_EXPANDER_MAX];
  UINT64                       TempBar[IO_EXPANDER_MAX];
  UINT64                       PmcBar[IO_EXPANDER_MAX];
  UINT64                       AcpiBar[IO_EXPANDER_MAX];
  UINT64                       TcoBar[IO_EXPANDER_MAX];
  UINT64                       SpiBar[IO_EXPANDER_MAX];
} IO_EXPANDER_INFO;

#define PCH_ALIGN_TO_1MB(n)                (((n) + 0x100000 - 1) & ~(0x100000 - 1))

#define PCH_IOE_PCR_MMIO_SIZE              0x01000000                                                             ///< 16MB
#define PCH_IOE_TEMP_MMIO_SIZE             0x00200000                                                             ///< 2MB
#define PCH_IOE_PMC_MMIO_SIZE              0x00010000                                                             ///< 64KB
#define PCH_IOE_SPI_MMIO_SIZE              0x00001000                                                             ///< 4KB

#define PCH_IOE_PCR_BASE_OFFSET            0x00000000                                                              ///< SBREG MMIO base offset in Pch preserved MMIO
#define PCH_IOE_PCR_BASE_ADDRESS(Base)     ((Base) + PCH_IOE_PCR_BASE_OFFSET)                                      ///< SBREG MMIO base address
#define PCH_IOE_TEMP_BASE_OFFSET           (PCH_IOE_PCR_BASE_OFFSET + PCH_IOE_PCR_MMIO_SIZE)                        ///< Temporary MMIO base offset in Pch preserved MMIO
#define PCH_IOE_TEMP_BASE_ADDRESS(Base)    ((Base) + PCH_IOE_TEMP_BASE_OFFSET)                                     ///< Temporary MBAR MMIO base address
#define PCH_IOE_PMC_BASE_OFFSET            (PCH_IOE_PCR_BASE_OFFSET + PCH_IOE_TEMP_MMIO_SIZE)                       ///< PMC MBAR MMIO base offset in Pch preserved MMIO
#define PCH_IOE_PMC_BASE_ADDRESS(Base)     ((Base) + PCH_IOE_PMC_BASE_OFFSET)                                     ///< PMC MBAR MMIO base address
#define PCH_IOE_SPI_BASE_OFFSET            (PCH_IOE_PMC_BASE_OFFSET + PCH_IOE_PMC_MMIO_SIZE)                      ///< SPI MBAR MMIO base offset in Pch preserved MMIO
#define PCH_IOE_SPI_BASE_ADDRESS(Base)     ((Base) + PCH_IOE_SPI_BASE_OFFSET)                                      ///< SPI MBAR MMIO base address

//
// I/O Expander MMIO Size
//
#define IOE_PCH_MMIO_SIZE                 PCH_ALIGN_TO_1MB(PCH_IOE_SPI_BASE_OFFSET + PCH_IOE_SPI_MMIO_SIZE)

extern EFI_GUID gIoExpanderPpiGuid;
extern EFI_GUID gIoExpanderInfoHobGuid;

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
  );

/**
  PchGetNumberDetectedIoExpanders return Number of detected I/O Expanders

  Number of I/O Expanders depends on boot UEFI phase.
  Before IIO is configued the number is always 0

  @retval                       Number of detected I/O Expanders
**/
UINT8
PchGetNumberDetectedIoExpanders (
  VOID
  );

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
  );

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
  IN  UINT32            IoExpanderId,
  OUT UINT8             *Segment
  );

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
  );

/**
  PchGetP2sbBarByIoExpanderId return I/O Expander P2SB BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander P2SB BAR
**/
UINTN
PchGetP2sbBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  );

/**
  PchGetPmcBarByIoExpanderId return I/O Expander PMC BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander PMC BAR
**/
UINTN
PchGetPmcBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  );

/**
  PchGetTcoBarByIoExpanderId return I/O Expander TCO BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander TCO BAR
**/
UINTN
PchGetTcoBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  );

/**
  PchGetTempBarByIoExpanderId return I/O Expander Temporary BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander Temporary BAR
**/
UINTN
PchGetTempBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  );

/**
  PchGetSpiBarByIoExpanderId return I/O Expander SPI PCIe BAR

  @param[in] IoExpanderId       I/O Expander Id (0 ... n - I/O Expander)

  @retval                       I/O Expander SPI PCIe BAR
**/
UINTN
PchGetSpiBarByIoExpanderId (
  IN  UINT32             IoExpanderId
  );

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
  );

/**
  PchInstallIoExpanderInfoHob installs IoExpanderInfo into HOB

  @retval EFI_SUCCESS                   The I/O Expander Info HOB is installed.
  @retval EFI_OUT_OF_RESOURCES          Out of memory
**/
EFI_STATUS
EFIAPI
PchInstallIoExpanderInfoHob (
  VOID
  );

#endif // _PCH_IO_EXPANDER_H_
