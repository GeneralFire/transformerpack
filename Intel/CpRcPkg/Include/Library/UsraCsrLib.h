/** @file
  Implementation of UsarCsLib class library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef __USRA_CSR_LIB_H__
#define __USRA_CSR_LIB_H__

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
  );

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
  IN  UINT32             Data32
  );
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
  );
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
  );

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
  IN  UINT8               SocId,
  IN  UINT8               BoxInst,
  IN  UINT32              CsrOffset
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  This API performs silicon CSR register read operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

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
  );

/**
  This API performs silicon CSR register write operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

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
  );

/**
  This API performs silicon CSR register write operations and also write to
  script to support S3.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] Bus                Bus
  @param[in] Device             Device
  @param[in] Function           Function
  @param[in] Offset             Register offset
  @param[in] Buffer             The data needs to be written to CSR.
  @param[in] AccessWidth        The Access width for 8, 16, 32, 64 -bit access. See USRA_ACCESS_WIDTH enum.

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
  );

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
  );

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
  );


#endif // #ifndef __USRA_CSR_LIB_H__
