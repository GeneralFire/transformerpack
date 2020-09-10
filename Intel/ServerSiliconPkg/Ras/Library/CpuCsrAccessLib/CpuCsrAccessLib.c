/** @file
  Implementation of CPU CSR access library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
#include <Library/UsraAccessApi.h>

/**

  Reads CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param RegOffset - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
EFIAPI
ReadCpuCsr (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   CsrOffset
  )
{
  USRA_ADDRESS           RegisterId;
  UINT32                 Data32;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocId, BoxInst, CsrOffset);

  RegisterRead (&RegisterId, &Data32);
  return Data32;
}

/**

  Writes CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param Offset    - Register offset; values come from the auto generated header file
  @param Data      - Register data to be written

  @retval None

**/
VOID
EFIAPI
WriteCpuCsr (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   CsrOffset,
  IN UINT32   Data32
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocId, BoxInst, CsrOffset);

  RegisterWrite (&RegisterId, &Data32);
}


/**

  Get CPU CSR MMIO address.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param RegOffset - Register offset; values come from the auto generated header file
  @param Size      - Register Size.

  @retval address value

**/
UINT64
EFIAPI
GetCpuCsrAddress (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT32   Offset,
  OUT UINT8    *Size
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocId, BoxInst, Offset);

  return GetRegisterAddress (&RegisterId);
}

/**
  The constructor function of CPU CSR access library.
  Note: When CPU CSR access library is used in another library's constructor(for example Lib A),
        current build tool does not put SmmUsraLibConstructor in front of Lib A's constructor correctly.
        So add empty constructor to adjust SmmUsraLibConstructor order.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
CpuCsrAccessLibConstructor (
  VOID
  )
{
  return EFI_SUCCESS;
}
