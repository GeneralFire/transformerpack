/** @file

  This is the library class header file for RCRB Region Access lib.

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

#ifndef __RCRB_ACCESS_LIB_H__
#define __RCRB_ACCESS_LIB_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraAccessApi.h>

typedef enum {
  TYPE_DMI_RCRB   = 0,
  TYPE_CXL_RCRB
} RCRB_FLAG;

/**
  Get RCRBBAR base address from the CXL RCRBBAR register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  CxlFlag   - Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI

  @retval               - The 64-bit RCRBBAR base address or 0 if failed.
**/
UINT64
EFIAPI
GetRcrbBar (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  BOOLEAN      CxlFlag
  );

/**
  Set RCRBBar for CXL or PCIe Port

  @param SocId                       Socket ID
  @param CtrId                       Controller ID
  @param CxlFlag                     Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI
  @param BarAddr                     RCRB Bar Address

  @retval                            - EFI_UNSUPPORTED: the function not implemented
                                     - EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
SetRcrbBar (
  IN  UINT8                       SocId,
  IN  UINT8                       CtrId,
  IN  BOOLEAN                     CxlFlag,
  IN  UINT32                      BarAddr
  );

/**
  Clear RCRBBar for CXL or PCIe Port.

  @param[in]  SocId                      - Socket ID
  @param[in]  CtrId                      - Controller ID
  @param[in]  CxlFlag                    - Indicate what type of the control, TRUE: Pcie G5 - CXL; FALSE: Pcie G4 - DMI

  @retval     EFI_SUCCESS                - Function completed successfully
  @retval     EFI_INVALID_PARAMETER      - Fuction failed due to input parameter wrong
**/
EFI_STATUS
EFIAPI
ClearRcrbBar (
  IN  UINT8                       SocId,
  IN  UINT8                       CtrId,
  IN  BOOLEAN                     CxlFlag
  );

/**
  Routine to probe and program the 64-bit MEMBAR0 register for CXL port.

  @param[in]  RcrbBaseAddr           RCRB base address
  @param[in]  BarRegOff              The offset of the Bar register
  @param[in]  MmioStartAddr          The start address of available MMIO resource
  @param[out] MemBar0Base            The pointer to the base address of MMIO resource allocated for MEMBAR0
  @param[out] MemBar0Size            The pointer to the size of MMIO resource requested by MEMBAR0

  @retval     EFI_SUCCESS            - Function return successfully.
              EFI_INVALID_PARAMETER  - Input parameter is invalid.
**/
EFI_STATUS
EFIAPI
ProgramMemBar0Register (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT32     BarRegOff,
  IN  UINT32     MmioStartAddr,
  OUT UINT64     *MemBar0Base,
  OUT UINT64     *MemBar0Size
  );

/**
  Routine to clear the 64-bit MEMBAR0 register for CXL port.

  @param[in]  RcrbBaseAddr           RCRB base address
  @param[in]  BarRegOff              The offset of the Bar register

  @retval     VOID

**/
VOID
EFIAPI
ClearMmioForCxlPort (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT32     BarRegOff
  );

/**
  Get MEMBAR0 base address using the CXL RCRBBAR register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  BarRegOff - The offset of the Bar register

  @retval               - The 64-bit RCRBBAR base address or 0 if failed.
**/
UINT64
EFIAPI
GetMemBar0 (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  UINT32       BarRegOff
  );

#endif //__RCRB_ACCESS_LIB_H__
