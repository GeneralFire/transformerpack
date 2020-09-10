/** @file
  Implementation of UsarCsLib class library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/UsraCsrLib.h>

/**
  This API performs silicon CSR register read operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrOffset          Register offset

  @retval UINT32                The data from CSR register returned.
**/
UINT32
EFIAPI
UsraCsrRead (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrOffset
  )
{
  USRA_ADDRESS           RegisterId;
  UINT32                 Data32;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocketId, BoxInst, CsrOffset);

  RegisterRead (&RegisterId, &Data32);
  return Data32;
}

/**
  This API performs silicon CSR register write operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrOffset          Register offset
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
UsraCsrWrite (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrOffset,
  IN  OUT UINT32         Data32
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocketId, BoxInst, CsrOffset);

  RegisterWrite (&RegisterId, &Data32);
}

/**
  This API performs silicon CSR register write operations and also write to
  script to support S3.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrOffset          Register offset
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
S3UsraCsrWrite (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrOffset,
  IN  UINT32             Data32
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocketId, BoxInst, CsrOffset);

  RegisterId.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&RegisterId, &Data32);
}

/**
  This API performs silicon CSR register modify operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrOffset          Register offset
  @param[in] AndBuffer          A pointer of buffer for the value used for AND operation
                                A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer           A pointer of buffer for the value used for OR operation
                                A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS        The function completed successfully.
  @retval Others                Return Error
**/
RETURN_STATUS
EFIAPI
UsraCsrModify (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrOffset,
  IN  VOID               *AndBuffer,
  IN  VOID               *OrBuffer
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocketId, BoxInst, CsrOffset);

  return RegisterModify(&RegisterId, AndBuffer, OrBuffer);
}

/**
  This API gets the flat address .

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrOffset          Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
UsraGetCsrRegisterAddress (
  IN  UINT8    SocketId,
  IN  UINT8    BoxInst,
  IN  UINT32   CsrOffset
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_OFFSET_ADDRESS (RegisterId, SocketId, BoxInst, CsrOffset);

  return GetRegisterAddress (&RegisterId);
}

/**
  This API performs silicon CSR register read operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] MemBarId           BAR ID
  @param[in] Offset             Register offset
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

  @retval UINT64                The data from CSR register returned.
**/
UINT64
EFIAPI
UsraCsrMemRead (
  IN  UINT8              SocketId,
  IN  UINT8              MemBarId,
  IN  UINT32             Offset,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;
  UINT64                 Data64;
  UINT32                 DataUpper = 0;
  UINT32                 DataLower = 0;
  UINT8                  AdjustedWidth = AccessWidth;

  if (AccessWidth == UsraWidth64) {
    USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, UsraWidth32);
    ((USRA_ADDRESS *)(&RegisterId))->CsrMem.High64Split = 1;
    RegisterRead (&RegisterId, &DataUpper);
    AdjustedWidth = UsraWidth32;
  }


  USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, AdjustedWidth);
  RegisterRead (&RegisterId, &DataLower);

  Data64 = LShiftU64 ((UINT64) DataUpper, 32) | DataLower;

  return Data64;
}

/**
  This API performs silicon CSR register write operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] MemBarId           BAR ID
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

**/
VOID
EFIAPI
UsraCsrMemWrite (
  IN  UINT8              SocketId,
  IN  UINT8              MemBarId,
  IN  UINT32             Offset,
  IN  VOID               *Buffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;
  UINT32                 DataUpper = 0;
  UINT32                 DataLower = 0;
  VOID                   *SwapBuffer = Buffer;
  UINT8                  AdjustedWidth = AccessWidth;

  if (AccessWidth == UsraWidth64) {
    USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, UsraWidth32);
    ((USRA_ADDRESS *)(&RegisterId))->CsrMem.High64Split = 1;
    DataUpper = RShiftU64 (*(UINT64 *) Buffer, 32) & 0xFFFFFFFF;
    DataLower = (*(UINT64 *) Buffer) & 0xFFFFFFFF;
    RegisterWrite (&RegisterId, &DataUpper);
    SwapBuffer = &DataLower;
    AdjustedWidth = UsraWidth32;
  }

  USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, AdjustedWidth);
  RegisterWrite (&RegisterId, SwapBuffer);
}

/**
  This API performs silicon CSR register write operations and also write to
  script to support S3.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] MemBarId           BAR ID
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

**/
VOID
EFIAPI
S3UsraCsrMemWrite (
  IN  UINT8              SocketId,
  IN  UINT8              MemBarId,
  IN  UINT32             Offset,
  IN  VOID               *Buffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, AccessWidth);

  RegisterId.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&RegisterId, Buffer);
}

/**
  This API performs silicon CSR register modify operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] MemBarId           BAR ID
  @param[in] Offset             Register offset
  @param[in] AndBuffer          A pointer of buffer for the value used for AND operation
                                A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer           A pointer of buffer for the value used for OR operation
                                A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

  @retval RETURN_SUCCESS        The function completed successfully.
  @retval Others                Return Error
**/
RETURN_STATUS
EFIAPI
UsraCsrMemModify (
  IN  UINT8              SocketId,
  IN  UINT8              MemBarId,
  IN  UINT32             Offset,
  IN  VOID               *AndBuffer,
  IN  VOID               *OrBuffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, AccessWidth);

  return RegisterModify (&RegisterId, AndBuffer, OrBuffer);
}

/**
  This API gets the flat address .

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] MemBarId           BAR ID
  @param[in] Offset             Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
UsraGetCsrMemRegisterAddress (
  IN  UINT8               SocketId,
  IN  UINT8               MemBarId,
  IN  UINT32              Offset
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_MEM_ADDRESS (RegisterId, SocketId, MemBarId, Offset, UsraWidthMaximum);

  return GetRegisterAddress (&RegisterId);
}

/**
  This API performs silicon CSR register read operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] AccessWidth        The Access width for 8, 16, 32 -bit access. See USRA_ACCESS_WIDTH enum.

  @retval UINT64                The data from CSR register returned.
**/
UINT64
EFIAPI
UsraCsrCfgRead (
  IN  UINT8              SocketId,
  IN  UINT8              Bus,
  IN  UINT8              Device,
  IN  UINT8              Function,
  IN  UINT32             Offset,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;
  UINT64                 Data64;
  UINT32                 DataUpper = 0;
  UINT32                 DataLower = 0;
  UINT8                  AdjustedWidth = AccessWidth;

  if (AccessWidth == UsraWidth64) {
    USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, UsraWidth32);
    ((USRA_ADDRESS *)(&RegisterId))->CsrCfg.High64Split = 1;
    RegisterRead (&RegisterId, &DataUpper);
    AdjustedWidth = UsraWidth32;
  }

  USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, AdjustedWidth);
  RegisterRead (&RegisterId, &DataLower);

  Data64 = LShiftU64 ((UINT64) DataUpper, 32) | DataLower;

  return Data64;
}

/**
  This API performs silicon CSR register write operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32 -bit access. See USRA_ACCESS_WIDTH enum.

**/
VOID
EFIAPI
UsraCsrCfgWrite (
  IN  UINT8              SocketId,
  IN  UINT8              Bus,
  IN  UINT8              Device,
  IN  UINT8              Function,
  IN  UINT32             Offset,
  IN  VOID               *Buffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;
  UINT32                 DataUpper = 0;
  UINT32                 DataLower = 0;
  VOID                   *SwapBuffer = Buffer;
  UINT8                  AdjustedWidth = AccessWidth;

  if (AccessWidth == UsraWidth64) {
    USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, UsraWidth32);
    ((USRA_ADDRESS *)(&RegisterId))->CsrCfg.High64Split = 1;
    DataUpper = RShiftU64 (*(UINT64 *) Buffer, 32) & 0xFFFFFFFF;
    DataLower = (*(UINT64 *) Buffer) & 0xFFFFFFFF;
    RegisterWrite (&RegisterId, &DataUpper);
    SwapBuffer = &DataLower;
    AdjustedWidth = UsraWidth32;
  }

  USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, AdjustedWidth);
  RegisterWrite (&RegisterId, SwapBuffer);
}

/**
  This API performs silicon CSR register write operations and also write to
  script to support S3.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32 -bit access. See USRA_ACCESS_WIDTH enum.

**/
VOID
EFIAPI
S3UsraCsrCfgWrite (
  IN  UINT8              SocketId,
  IN  UINT8              Bus,
  IN  UINT8              Device,
  IN  UINT8              Function,
  IN  UINT32             Offset,
  IN  VOID               *Buffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, AccessWidth);

  RegisterId.Attribute.S3Enable = USRA_ENABLE;
  RegisterWrite (&RegisterId, Buffer);
}

/**
  This API performs silicon CSR register modify operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] AndBuffer          A pointer of buffer for the value used for AND operation
                                A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer           A pointer of buffer for the value used for OR operation
                                A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

  @retval RETURN_SUCCESS        The function completed successfully.
  @retval Others                Return Error
**/
RETURN_STATUS
EFIAPI
UsraCsrCfgModify (
  IN  UINT8              SocketId,
  IN  UINT8              Bus,
  IN  UINT8              Device,
  IN  UINT8              Function,
  IN  UINT32             Offset,
  IN  VOID               *AndBuffer,
  IN  VOID               *OrBuffer,
  IN  UINT8              AccessWidth
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, AccessWidth);

  return RegisterModify (&RegisterId, AndBuffer, OrBuffer);
}

/**
  This API gets the flat address .

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
UsraGetCsrCfgRegisterAddress (
  IN  UINT8              SocketId,
  IN  UINT8              Bus,
  IN  UINT8              Device,
  IN  UINT8              Function,
  IN  UINT32             Offset
  )
{
  USRA_ADDRESS           RegisterId;

  USRA_CSR_CFG_ADDRESS (RegisterId, SocketId, Bus, Device, Function, Offset, UsraWidthMaximum);

  return GetRegisterAddress (&RegisterId);
}
