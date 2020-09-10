/** @file
  Macros to simplify and abstract the interface for IIO access.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_ACCESS_LIB_H_
#define _IIO_ACCESS_LIB_H_

#include <PiPei.h>
#include <IioPlatformData.h>

VOID
IioMmioWrite32 (
  IN  UINTN       Address,
  IN  UINT32      Data
  );

VOID
IioMmioWrite16 (
  IN  UINTN       Address,
  IN  UINT16      Data
  );

VOID
IioMmioWrite8 (
  IN  UINTN       Address,
  IN  UINT8       Data
  );

UINT32
IioMmioRead32 (
  IN  UINTN       Address
  );

UINT16
IioMmioRead16 (
  IN  UINTN       Address
  );

UINT8
IioMmioRead8 (
  IN  UINTN       Address
  );

VOID
IioMioOr32 (
  IN  UINTN       Address,
  IN  UINT32      DataOr
  );

VOID
IioMmioOr16 (
  IN  UINTN       Address,
  IN  UINT16      DataOr
  );

VOID
IioMmioOr8 (
  IN  UINTN  Address,
  IN  UINT8  DataOr
  );

VOID
IioMmioAnd32 (
  IN  UINTN       Address,
  IN  UINT32      DataAnd
  );

VOID
IioMmioAnd16 (
  IN  UINTN       Address,
  IN  UINT16      DataAnd
  );

VOID
IioMmioAnd8 (
  IN  UINTN       Address,
  IN  UINT8       DataAnd
  );

VOID
IioPciExpressWrite32 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT32            Data
  );

VOID
IioPciExpressWrite16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT16            Data
  );

VOID
IioPciExpressNonS3Write16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN UINT16            Data
  );

VOID
IioPciExpressWrite8 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset,
  IN  UINT8             Data
  );

UINT32
IioPciExpressRead32 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  );

UINT16
IioPciExpressRead16 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  );

UINT8
IioPciExpressRead8 (
  IN UINT8             IioIndex,
  IN UINT16            Bus,
  IN UINT16            Device,
  IN UINT8             Function,
  IN UINT32            Offset
  );

VOID
IioWriteCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  );

VOID
IioNonS3WriteCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  );

VOID
IioNonS3WriteCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT32            Data
  );

VOID
IioWriteCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT16            Data
  );

VOID
IioWriteCpuCsr8 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr,
  IN  UINT8             Data
  );

UINT32
IioReadCpuCsr32 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  );

UINT16
IioReadCpuCsr16 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  );

UINT8
IioReadCpuCsr8 (
  IN  UINT8             IioIndex,
  IN  UINT8             BoxInst,
  IN  UINT32            Csr
  );

VOID
IioBreakAtCheckPoint (
  IN  UINT8       Socket,
  IN  UINT8       MajorCode,
  IN  UINT8       MinorCode,
  IN  UINT16      Data
  );

/**
   This function obtains the PCI configuration space register offset of the specified
   Capabilities register set.

  @param[in] Socket      Device's socket number.
  @param[in] Bus         Device's bus number.
  @param[in] Device      Device's device number.
  @param[in] Function    Device's function number.
  @param[in] Cap         Desired Capability structure

  @retval  CapabilityOffset.
**/
UINT8
EFIAPI
IioPCIeGetCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT8   Cap
  );

/**
  This function obtains the extended PCI configuration space register offset for a
  specified Extended Capability for the specified PCI device.

  @param[in] Socket               Device's socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         PCIe extended capability ID not found
                                  non-zero  Extended PCI configuration space offset of the specified Ext Capability block
**/
UINT16
EFIAPI
IioPCIeGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId
  );

/**
  Returns address from MBAR_01 or 0 on error

  @param IioIndex   - Index to CPU/IIO
  @param Bus, Device, Function - PCIE port BDF

  @return - address or zero on error
 */
UINTN
GetAddressFromMBar01 (
  IN  UINT8                       IioIndex,
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  );


/**
  Read 32-bit register from Pcie port config space with support for NTB ports

  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param OutBuf        - Buffer to return register value.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 **/
EFI_STATUS
GetPcieCapReg32 (
  IN     UINT8         IioIndex,
  IN     UINT8         PortIndex,
  IN     UINT32        CapRegOffset,
  OUT    UINT32        *OutBuffer
  );

/**
  Write 32-bit register of Pcie port config space with support for NTB ports

  @param IIO_GLOBAL    - Pointer to IIO_GLOBALS
  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param Value         - Value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 **/
EFI_STATUS
SetPcieCapReg32 (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT32        Value
  );

/**
  Read 16-bit register from Pcie port config space with support for NTB ports

  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param OutBuf        - Buffer to return register value.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 **/
EFI_STATUS
GetPcieCapReg16 (
  IN     UINT8         IioIndex,
  IN     UINT8         PortIndex,
  IN     UINT32        CapRegOffset,
  IN OUT UINT16        *OutBuffer
  );

/**
  Write 16-bit register of Pcie port config space with support for NTB ports

  @param IIO_GLOBAL    - Pointer to IIO_GLOBALS
  @param IioIndex      - Index to CPU/IIO
  @param PortIndex     - Port index.
  @param CapRegOffset  - Register offset in Pcie cap.
  @param Value         - Value to written in the register.

  @retval EFI_SUCCESS on success
  @retval EFI_DEVICE_ERROR if can't find pcie dev. cap. or on Mbar value issue
 **/
EFI_STATUS
SetPcieCapReg16 (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT32        CapRegOffset,
  IN  UINT16        Value
  );

VOID
EnableMBarAccess (
  IN  UINT8                       IioIndex,
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  );

VOID
DisableMBarAccess (
  IN  UINT8                       IioIndex,
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress
  );

#endif // _IIO_ACCESS_LIB_H_
