/** @file
  Implementation of PCH interface for RAS.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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
#include <Library/PciSegmentLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/IehRegs.h>
#include <PchResetPlatformSpecific.h>

/**
  Function checks if IEH device is supported in PCH
  If supported returns Device, Function, DID and Type of IEH device

  @param [in]  Segment    - Segment number
  @param [in]  Bus        - Bus number
  @param [out] Device     - Device number
  @param [out] Function   - Function number
  @param [out] Did        - Device ID
  @param [out] Type       - IEH Type

  @retval               TRUE  - IEH device found
                        FALSE - IEH device NOT found
**/
BOOLEAN
PchRasGetIehDevice (
  IN           UINT8      Segment,
  IN           UINT8      Bus,
  OUT OPTIONAL UINT8      *Device,
  OUT OPTIONAL UINT8      *Function,
  OUT OPTIONAL UINT16     *Did,
  OUT OPTIONAL UINT8      *Type
  )
{
  UINT64      IehPciBaseAddress;
  UINT16      IehDeviceId;

  IehPciBaseAddress = IehPciCfgBase ();

  IehDeviceId = PciSegmentRead16 (IehPciBaseAddress + PCI_DEVICE_ID_OFFSET);
  if (IehDeviceId == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "IEH device is not accessible\n"));
    return FALSE;
  }

  if (Device != NULL) {
    *Device = IehDevNumber ();
  }
  if (Function != NULL) {
    *Function = IehFuncNumber ();
  }
  if (Did != NULL) {
    *Did = IehDeviceId;
  }
  if (Type != NULL) {
    *Type = (PciSegmentRead8 (IehPciBaseAddress + R_IEH_CFG_IEHTYPEH_IEHTYPE));
  }

  return TRUE;
}

/**
  Get PCH device info by index number.

  @param [in]  Index      - index number
  @param [out] Socket     - socket number
  @param [out] Bus        - bus number
  @param [out] Device     - Device number
  @param [out] Function   - function number

  @retval             TRUE  - found new device
                      FALSE - no more PCH device
**/
BOOLEAN
EFIAPI
PchRasGetNextDevice (
  IN  UINT32    Index,
  OUT UINT8     *Socket,
  OUT UINT8     *Bus,
  OUT UINT8     *Device,
  OUT UINT8     *Function
  )
{
  return FALSE;
}

/**
  Function to clear DMI errors

  @param[in] Bus        Device bus number to check

  **/
VOID
EFIAPI
PchRasClearDmiErrors (
  IN      UINT8   Bus
  )
{

}

/**
  get  DMI errors

  @param[out] error        DMI error

  @retval             TRUE  - DMI has error
                      FALSE - no PCH DMI error

  **/
BOOLEAN
EFIAPI
PchRasGetDmiErrors (
  OUT   UINT32    *Error
  )
{
  return FALSE;
}

/**
  Detect if correctable error message is received or an internal correctable error detected

  @retval               TRUE  - DMI has CR error
                        FALSE - No CR PCH DMI error

  **/
BOOLEAN
EFIAPI
PchRasIsCrDmiError (
  VOID
  )
{
  return FALSE;
}

/**
  Detect if either afatal or a non-fatal error message is received or an internal fatal error is detected

  @retval               TRUE  - DMI has ENR error
                        FALSE - No ENR PCH DMI error
  **/
BOOLEAN
EFIAPI
PchRasIsEnrDmiError (
  VOID
  )
{
  return FALSE;
}

/**
  disable DMI correctable error.

  **/
VOID
EFIAPI
PchRasDisableDmiCorrectError (
  VOID
  )
{

}

/**
Function to enable PCH DMI

  @param[in] PcieErrEn       - Pcie error enable option.
  @param[in] PcieCorrErrEn   - correctable error enable.
  @param[in] PcieUncorrErren - uncorrectable error enable.
  @param[in] ErrCorMsk       - Correctable error Mask.
  @param[in] ErrUncMsk       - Uncorrectable error Mask.
  @param[in] ErrUncSev       - Uncorrectable error severity.
**/
VOID
EFIAPI
PchRasEnableDmiErrorRepoting (
  IN UINT8              PcieErrEn,
  IN UINT8              PcieCorrErrEn,
  IN UINT8              PcieUncorrErrEn,
  IN UINT32             ErrCorMsk,
  IN UINT32             ErrUncMsk,
  IN UINT32             ErrUncSev
  )
{

}

/**
    Subroutine for clearing the PCI SERR# status (and LPC IOCHK# status) in the
    NMI Status and Control Register.  This keeps the system from executing in an
    infinite SMI loop when the system is configured to SMI on NMI.
**/
VOID
EFIAPI
PchRasClearSerrStatus (
  VOID
  )
{

}

VOID
EFIAPI
PchRasDumpPchRegisters(
  VOID
  )
{

}

