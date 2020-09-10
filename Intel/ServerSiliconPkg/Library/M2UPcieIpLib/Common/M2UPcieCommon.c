/** @file

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
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Guid/M2UPcieIpInterface.h>

/**
  M2UPCIE:  Check if device exists

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param DevExists     - Device exist

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieDeviceExists (
  IN  UINT8    SocId,
  IN  UINT8    CtrId,
  OUT BOOLEAN* DevExists
  )
{
  UINT16 Data;

  Data = (UINT16)UsraCsrRead (SocId, CtrId, VID_M2UPCIE_MAIN_REG);
  *DevExists = ((Data == 0xFFFF)? FALSE : TRUE);

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Unmask Ingress Errors

  @param SocId         - Socket ID
  @param CtrId         - Controller ID

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieUnmaskR2PinError (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  UsraCsrWrite (SocId, CtrId, R2PINGERRMSK0_M2UPCIE_MAIN_REG, 0);

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Unmask Ingress Errors

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Parerring0msk - Parerring0msk bit field value
  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieMaskParerring0msk  (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 Parerring0msk
  )
{
  R2PINGERRMSK0_M2UPCIE_MAIN_STRUCT R2PingErrMsk0;
  R2PingErrMsk0.Data = UsraCsrRead (SocId, CtrId, R2PINGERRMSK0_M2UPCIE_MAIN_REG);
  R2PingErrMsk0.Bits.parerring0msk = Parerring0msk;
  UsraCsrWrite (SocId, CtrId, R2PINGERRMSK0_M2UPCIE_MAIN_REG, R2PingErrMsk0.Data);

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Set UPI agent credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param VnaCr         - VNA Credits

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
M2UPcieSetUpiCrd (
  IN UINT8  SocId,
  IN UINT8  CtrId,
  IN UINT32 VnaCr
  )
{
  UsraCsrWrite (SocId, CtrId, R2NDRTOKTIINITCRED_M2UPCIE_MAIN_REG, VnaCr);

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Read and Write back the egress error log.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param EgressErrLog  - Egress Error log

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2WriteBackEgressErrLog (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* EgressErrLog
  )
{
  UINT32 Data32;

  Data32 = UsraCsrRead (SocId, CtrId, R2EGRERRLOG_M2UPCIE_MAIN_REG);
  UsraCsrWrite (SocId, CtrId, R2EGRERRLOG_M2UPCIE_MAIN_REG, Data32);
  *EgressErrLog = Data32;

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Read and Write back the ingress error log.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param IngressErrLog - Ingress Error log

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2WriteBackIngressErrLog (
  IN  UINT8   SocId,
  IN  UINT8   CtrId,
  OUT UINT32* IngressErrLog
  )
{
  UINT32 Data32;
  Data32 = UsraCsrRead (SocId, CtrId, R2PINGERRLOG0_M2UPCIE_MAIN_REG);// RW1CS
  UsraCsrWrite (SocId, CtrId, R2PINGERRLOG0_M2UPCIE_MAIN_REG, Data32);
  *IngressErrLog = Data32;

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Mask parity error in the ingress error mask.

  @param SocId         - Socket ID
  @param CtrId         - Controller ID

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieR2MaskParIngressErr (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  R2PINGERRMSK0_M2UPCIE_MAIN_STRUCT r2pIngErrReg;
  //
  // Si W/A 310710: Mask parity error on rsingrblpm with CSR bit
  //
  r2pIngErrReg.Data = UsraCsrRead (SocId, CtrId, R2PINGERRMSK0_M2UPCIE_MAIN_REG);
  r2pIngErrReg.Bits.parerring0msk = 1;
  UsraCsrWrite (SocId, CtrId, R2PINGERRMSK0_M2UPCIE_MAIN_REG, r2pIngErrReg.Data);

  return EFI_SUCCESS;
}

/**
  M2UPCIE:  Set the PRQ Credit Error Mask for each CHA

  @param SocId           - Socket ID
  @param CtrId           - Controller ID
  @param Mask            - mask when true, un-mask when false

  @retval EFI_STATUS     - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
M2UPcieSetR2CrdErrMask (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN BOOLEAN Mask
  )
{
  if (Mask) {
    UsraCsrWrite (SocId, CtrId, R2EGRPRQERRMSK0_M2UPCIE_MAIN_REG, 0xFFFFFFFF);
  } else {
    UsraCsrWrite (SocId, CtrId, R2EGRPRQERRMSK0_M2UPCIE_MAIN_REG, 0);
  }

  return EFI_SUCCESS;
}
