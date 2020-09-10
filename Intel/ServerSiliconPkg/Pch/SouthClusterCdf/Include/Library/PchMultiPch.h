/** @file
  Prototype of the MultiPch library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _PCH_MULTI_PCH_LIB_H_
#define _PCH_MULTI_PCH_LIB_H_

#include <Library/PchMultiPchBase.h>

#define PCH_IP_INFO_REVISION              1

typedef struct _PCH_IP_INFO {
  /**
    Revision 1:   Original version
  **/
  UINT8                        Revision;

  BOOLEAN                      Valid[PCH_MAX];
  UINT8                        SocketId[PCH_MAX];
  UINT8                        Segment[PCH_MAX];
  UINT8                        Bus[PCH_MAX];
  UINT64                       P2sbBar[PCH_MAX];
  UINT64                       TempBar[PCH_MAX];
  UINT64                       PmcBar[PCH_MAX];
  UINT64                       SpiBar[PCH_MAX];
} PCH_IP_INFO;

#define PCH_ALIGN_TO_1MB(n)               (((n) + 0x100000 - 1) & ~(0x100000 - 1))

#define PCH_NL_PCR_MMIO_SIZE              0x01000000                                                             ///< 16MB
#define PCH_NL_TEMP_MMIO_SIZE             0x00200000                                                             ///< 2MB
#define PCH_NL_PWRM_MMIO_SIZE             0x00010000                                                             ///< 64KB
#define PCH_NL_SPI_MMIO_SIZE              0x00001000                                                             ///< 4KB

#define PCH_NL_PCR_BASE_OFFSET            0x00000000                                                             ///< SBREG MMIO base offset in Pch preserved MMIO
#define PCH_NL_PCR_BASE_ADDRESS(Base)     ((Base) + PCH_NL_PCR_BASE_OFFSET)                                      ///< SBREG MMIO base address
#define PCH_NL_TEMP_BASE_OFFSET           (PCH_NL_PCR_BASE_OFFSET + PCH_NL_PCR_MMIO_SIZE)                        ///< Temporary MMIO base offset in Pch preserved MMIO
#define PCH_NL_TEMP_BASE_ADDRESS(Base)    ((Base) + PCH_NL_TEMP_BASE_OFFSET)                                     ///< Temporary MBAR MMIO base address
#define PCH_NL_PWRM_BASE_OFFSET           (PCH_NL_PCR_BASE_OFFSET + PCH_NL_TEMP_MMIO_SIZE)                       ///< PMC MBAR MMIO base offset in Pch preserved MMIO
#define PCH_NL_PWRM_BASE_ADDRESS(Base)    ((Base) + PCH_NL_PWRM_BASE_OFFSET)                                     ///< PMC MBAR MMIO base address
#define PCH_NL_SPI_BASE_OFFSET            (PCH_NL_PWRM_BASE_OFFSET + PCH_NL_PWRM_MMIO_SIZE)                      ///< SPI MBAR MMIO base offset in Pch preserved MMIO
#define PCH_NL_SPI_BASE_ADDRESS(Base)     ((Base) + PCH_NL_SPI_BASE_OFFSET)                                      ///< SPI MBAR MMIO base address

//
// PCH BAR Size
//
#define NON_LEGACY_PCH_MMIO_SIZE          PCH_ALIGN_TO_1MB(PCH_NL_SPI_BASE_OFFSET + PCH_NL_SPI_MMIO_SIZE)


/**
  PchGetNumberDetectedPch return Number of detected PCHs

  Number of PCH depends also boot UEFI phase.
  Before configure PCIe topology across CPUs the number is always 1

  @retval                       Number of detected PCHs
**/
UINT8
PchGetNumberDetectedPch (
  VOID
  );

/**
  GetPchBusByPchId return PCH bus number

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH bus number
**/
UINT8
PchGetBusByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetP2sbBaseByPchId return PCH P2SB PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH P2SB PCIe config space base address
**/
UINTN
PchGetP2sbBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetP2sbBarByPchId return PCH P2SB BAR

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH P2SB BAR
**/
UINTN
PchGetP2sbBarByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetPmcBaseByPchId return PCH PMC PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH PMC PCIe config space base address
**/
UINTN
PchGetPmcBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetLpcBaseByPchId return PCH LPC PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH LPC PCIe config space base address
**/
UINTN
PchGetLpcBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetSmbusBaseByPchId return PCH SMBUS PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH SMBUS PCIe config space base address
**/
UINTN
PchGetSmbusBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetSpiBaseByPchId return PCH SPI PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH SPI PCIe config space base address
**/
UINTN
PchGetSpiBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetSpiBarByPchId return PCH SPI PCIe BAR

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH SPI PCIe BAR
**/
UINTN
PchGetSpiBarByPchId (
  IN  UINT8             PchId
  );

  /**
  PchGetLanBaseByPchId return PCH LAN PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH LAN PCIe config space base address
**/
UINTN
PchGetLanBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetTraceHubBaseByPchId return PCH Trace Hub PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH Trace Hub PCIe config space base address
**/
UINTN
PchGetTraceHubBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetHdaBaseByPchId return PCH HDA PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH HDA PCIe config space base address
**/
UINTN
PchGetHdaBaseByPchId (
  IN  UINT8             PchId
  );

/**
  PchGetSataBaseByPchId return PCH Sata PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH Sata PCIe config space base address
**/
UINTN
PchGetSataBaseByPchId (
  IN  UINT8             PchId
  );

/**
  GetPchXhciBaseByPchId return PCH Xhci PCIe config space base address

  @param[in] PchId              PCH Id (0 - Legacy PCH, 1 ... n - Non-Legacy PCH)

  @retval                       PCH Xhci PCIe config space base address
**/
UINTN
PchGetXhciBaseByPchId (
  IN  UINT8             PchId
  );

#endif // _PCH_MULTI_PCH_LIB_H_
