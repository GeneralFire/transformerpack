/** @file
  KTI *KTI_CIOPHYDFX_REG related registers access.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Base.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/MemoryCoreLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/IoAccessLib.h>

/**
  Check to see if IAR write-enable bit is clear
  The write_en bit must be clear before reading the KTI IAR register

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval register value read
**/
BOOLEAN
EFIAPI
IsIarWriteEnableClear (
  UINT8                 SocId,
  UINT8                 BoxInst
  )
{
  PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT AfeIar;

  AfeIar.Data = UsraCsrRead (SocId, BoxInst, PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG);
  if (AfeIar.Bits.write_en == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Get IAR Data

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  Broadcast Data Lane UnitId should NOT be used for reads.
  @param RegOffset - Register offset; values come from the auto generated header file
  @param data      - Data to write

  @retval register value read

**/
UINT8
EFIAPI
GetIarData (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset
  )
{
  PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT AfeIar;

  AfeIar.Data = 0;
  AfeIar.Bits.register_id = RegOffset & 0x3F;
  AfeIar.Bits.unit_id = UnitId & 0x3F;
  AfeIar.Bits.write_en = 0;
  AfeIar.Bits.hw_override_en = 1;
  UsraCsrWrite (SocId, BoxInst, PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG, AfeIar.Data);
  AfeIar.Data = UsraCsrRead (SocId, BoxInst, PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG);

  return (UINT8) AfeIar.Bits.read_data;
}

/**
  Write to IAR register.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  @param RegOffset - Register offset; values come from the auto generated header file
  @param Data      - Data to write
**/
VOID
EFIAPI
WriteIarData (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset,
  UINT8                 Data
  )
{
  PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT AfeIar;
  AfeIar.Data = 0;
  AfeIar.Bits.register_id = RegOffset & 0x3F;
  AfeIar.Bits.unit_id = UnitId & 0x3F;
  AfeIar.Bits.wr_data = (UINT8) Data;
  AfeIar.Bits.write_en = 1;
  AfeIar.Bits.hw_override_en = 1;
  UsraCsrWrite (SocId, BoxInst, PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG, AfeIar.Data);
}
