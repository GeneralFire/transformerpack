/** @file

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
#ifndef __M2IAL_LIB_H__
#define __M2IAL_LIB_H__

#include <Uefi.h>

/**
  M2IAL:  Check if device exists

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval                - TRUE if device exist and FALSE otherwise

**/

BOOLEAN
EFIAPI
M2IalDeviceExists (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Unmask Global Errors

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalUnmaskGlError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Unmask Ingress Errors

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalUnmaskR2PinError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Set UPI agent credits

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param VnaCr      - VN0 Credits

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetUpiCrd (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 Vn0Cr
  );

/**
  M2IAL:  Program m2ial route table

  @param SocId       - Socket ID
  @param CtrId       - Controller ID
  @param KtiRtMode   - Kti Routing mode
  @param M2PcieKtiRt - Route Table

  @retval            - NONE

**/

VOID
EFIAPI
M2IalSetRouteTable (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT8  KtiRtMode,
  IN UINT32 M2PcieKtiRt
  );

/**
  M2IAL:  Read and Write back the egress error log.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - R2 Egress error log

**/

UINT32
EFIAPI
M2IalR2WriteBackEgressErrLog (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Read and Write back the ingress error log.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - R2 Ingress error log

**/

UINT32
EFIAPI
M2IalR2WriteBackIngressErrLog (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Mask parity error in the ingress error mask.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - NONE

**/

VOID
EFIAPI
M2IalR2MaskParIngressErr (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Clear DDRT Defeature.

  @param SocId        - Socket ID
  @param CtrId        - Controller ID
  @param DdrtQosMode  - DDRT QoS Mode

  @retval           - NONE

**/

VOID
EFIAPI
M2IalClearDdrtDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 DdrtQosMode
  );

/**
  M2IAL:  Get The R2 General Control Register.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - The R2 General Control Register.

**/

UINT32
EFIAPI
M2IalGetR2GenCtrl (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Set the NDR DRS routes

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param TempR2pGnCtrl - Temporary register value read previously.

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetRouteNdrDrs (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 TempR2pGnCtrl
  );

/**
  M2IAL:  Program m2pcie port into route table

  @param SocId           - Socket ID
  @param DSocId          - Socket ID destination
  @param LocalPort       - The local port to route
  @param PortFieldWidth  - The local port width

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetPortRouteTable (
  IN UINT8  SocId,
  IN UINT8  DSocId,
  IN UINT32 LocalPort,
  IN UINT32 PortFieldWidth
  );

/**
  M2IAL:  Set the PRQ Credit Error Mask for each CHA

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param Mask      - mask when true, un-mask when false

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetR2CrdErrMask (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN BOOLEAN Mask
  );

/**
  M2IAL:  Set the PRQ initial credits  for each CHA

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param TotCha        - Total number of CHA
  @param PrqPerIoStack - PRQs per IO Stack

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetR2InitialCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 TotCha,
  IN UINT8 PrqPerIoStack
  );

/**
  M2IAL:  Get the Intial value of shared credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Cred0         - Shared credit 0
  @param Cred1         - Shared credit 1
  @param Cred2         - Shared credit 2
  @param Cred3         - Shared credit 3


  @retval              - NONE

**/

VOID
EFIAPI
M2IalGetR2InitShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  OUT UINT32 *Cred0,
  OUT UINT32 *Cred1,
  OUT UINT32 *Cred2,
  OUT UINT32 *Cred3
  );

/**
  M2IAL:  Set the Intial value of shared credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Cred0         - Shared credit 0
  @param Cred1         - Shared credit 1
  @param Cred2         - Shared credit 2
  @param Cred3         - Shared credit 3

  @retval              - NONE

**/

VOID
EFIAPI
M2IalSetR2InitShrCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Cred0,
  IN UINT32 Cred1,
  IN UINT32 Cred2,
  IN UINT32 Cred3
  );

/**
  M2IAL:  Set the initial credits for iio2ubox

  @param SocId     - Socket ID
  @param CtrId     - Controller ID

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetIio2UboxInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId
  );

/**
  M2IAL:  Set the PRQ initial credits usiing and index

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param RegIdx    - Table index for the register
  @param Credit    - Credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetPrqInitCrdIdx (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 RegIdx,
  IN UINT32 Credit
  );

/**
  M2IAL:  Set the initial credits for NDR/NCS to KTI

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param Vn0Ad    - VN0 AD credit value
  @param BlNcs    - BL NCS credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetNdrNcs2KtiInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Vn0Ad,
  IN UINT32 BlNcs
  );

/**
  M2IAL:  Set the initial credits for NBC/NCS to Iio

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param Credit   - Credit value

  @retval          - NONE

**/

VOID
EFIAPI
M2IalSetNcbNcs2IioInitCrd (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT32 Credit
  );

/**
  M2IAL:  Set the mesh credit update value

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param MeshCrdUpdate   - Mesh credit update value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2IalSetMeshCrdUpdate (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 MeshCrdUpdate
  );

/**
  M2IAL:  A routine for programming the QoS Performance settings.

  @param SocId        - Socket ID
  @param IioStack     - Stack Index
  @param DdrtQosMode  - DDRT QoS Mode

  @retval None

**/

VOID
EFIAPI
M2IalSetCrQosPerformance (
  IN UINT8 SocId,
  IN UINT8 IioStack,
  IN UINT8 DdrtQosMode
  );

/**
  M2IAL:  Set the NCS/NCB/DRS defeature bit, to override credit.

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Defeature       - NCS/NCB/DRS defeature bit value

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2IALSetNcsNcbDrsDefeature (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Defeature
  );

/**
  M2IAL:  Get The R2 General Control Register.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - The value of the SB_REG R2GenCtrl

**/

UINT32
EFIAPI
M2IalGetR2GenCtrl (
  UINT8 SocId,
  UINT8 CtrId
  );

/**
  M2IAL:  Set The R2 General Control Register.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param R2GenCtrl  - The value R2GenCtrl

  @retval           - NONE

**/

VOID
EFIAPI
M2IalSetR2GenCtrl (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 R2GenCtrl
  );
#endif // __M2IAL_LIB_H__
