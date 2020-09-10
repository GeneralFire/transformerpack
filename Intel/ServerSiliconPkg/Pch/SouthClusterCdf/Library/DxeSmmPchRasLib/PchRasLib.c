/** @file
  Implementation of PCH interface for RAS.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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
#include <Register/PchRegsIeh.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/ResetSystemLib.h>
#include <Library/DebugLib.h>
#include <PchResetPlatformSpecific.h>
#include <Register/PchRegsPcr.h>
#include <Library/PchPcrLib.h>
#include <Register/PchRegsDmiRlink.h>

//
// PCH device structure
//
typedef struct {
  UINT8                   Socket;
  UINT8                   Bus;
  UINT8                   Device;
  UINT8                   Function;
  BOOLEAN                 Bridge;
} RAS_PCH_PCI_DEVICES;

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
  UINT64      IehRegBase;
  UINT16      IehDeviceId;

  IehRegBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, PCI_DEVICE_NUMBER_PCH_IEH, PCI_FUNCTION_NUMBER_PCH_IEH, 0);
  IehDeviceId = PciSegmentRead16 (IehRegBase + PCI_DEVICE_ID_OFFSET);

  if (IehDeviceId == 0xFFFF) {
    DEBUG((DEBUG_ERROR, "IEH CSR parity error, resetting the platform\n"));
    ResetPlatformSpecific (sizeof (PCH_RESET_DATA), NULL);
  }

  if (IehDeviceId == V_CDF_PCH_IEH_CFG_DEVICE_ID) {
    if (Device != NULL) {
      *Device = PCI_DEVICE_NUMBER_PCH_IEH;
    }
    if (Function != NULL) {
      *Function = PCI_FUNCTION_NUMBER_PCH_IEH;
    }
    if (Did != NULL) {
      *Did = V_CDF_PCH_IEH_CFG_DEVICE_ID;
    }
    if (Type != NULL) {
      *Type = (PciSegmentRead8 (IehRegBase + R_PCH_IEH_IEHTYPE));
    }
    return TRUE;
  }

  return FALSE;
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
  OUT UINT8    *Socket,
  OUT UINT8    *Bus,
  OUT UINT8    *Device,
  OUT UINT8    *Function
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
  UINT32    Ues;
  UINT32    Ces;
  UINT32    Res;
  UINT16    DevSts;
  UINT16    PciSts;

  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors\n"));

  Ues = PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_UES);
  PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_UES, Ues);
  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors,R_PCH_RLINK_PCR_UES = 0x%x\n", Ues));

  Ces = PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_CES);
  PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_CES, Ces);
  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors,R_PCH_RLINK_PCR_CES = 0x%x\n", Ces));

  Res = PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_RES);
  PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_RES, Res);
  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors,R_PCH_RLINK_PCR_RES = 0x%x\n", Res));

  DevSts = PchPcrRead16 (PID_DMI, R_PCH_RLINK_PCR_DEVSTS);
  PchPcrWrite16 (PID_DMI, R_PCH_RLINK_PCR_DEVSTS, DevSts);
  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors, R_PCH_RLINK_PCR_DEVSTS = 0x%x\n", DevSts));

  PciSts = PchPcrRead16 (PID_DMI,  R_PCH_RLINK_PCR_PCISTS);
  PchPcrWrite16 (PID_DMI,  R_PCH_RLINK_PCR_PCISTS, PciSts);
  DEBUG((DEBUG_ERROR,"PchRasClearRlinkErrors, R_PCH_RLINK_PCR_PCISTS = 0x%x\n", PciSts));

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
  UINT32    Res;
  Res = PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_RES);
  DEBUG((DEBUG_ERROR,"PchElogDmiHandler R_PCH_RLINK_PCR_RES = 0x%x\n", Res));

  *Error = Res;
  if (Res & (B_PCH_RLINK_RES_CR | B_PCH_RLINK_RES_ENR)) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  UINT32    Res;

  PchRasGetDmiErrors (&Res);

  if (Res & B_PCH_RLINK_RES_CR) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  UINT32    Res;

  PchRasGetDmiErrors (&Res);

  if (Res & B_PCH_RLINK_RES_ENR) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_CEM, 0xffffffff);
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
  UINT16        DevCtl = 0;
  UINT16        CmdSts;

  if (PcieErrEn == 1) {
    if (PcieUncorrErrEn == 1) {
      PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_UEM, ErrUncMsk);
      DevCtl |= (B_PCH_RLINK_PCR_DEVCTL_NFERE | B_PCH_RLINK_PCR_DEVCTL_FERE);
    }

    if (PcieCorrErrEn == 1) {
      PchPcrWrite32 (PID_DMI, R_PCH_RLINK_PCR_CEM, ErrCorMsk);
      DevCtl |= B_PCH_RLINK_PCR_DEVCTL_CERE;
    }
  }

  PchPcrWrite16 (PID_DMI, R_PCH_RLINK_PCR_DEVCTL, (PchPcrRead16 (PID_DMI, R_PCH_RLINK_PCR_DEVCTL) | DevCtl));

  if (PcieUncorrErrEn == 1) {
    CmdSts = PchPcrRead16 (PID_DMI, R_PCH_RLINK_PCR_PCICMD);
    CmdSts |= (B_PCH_RLINK_PCR_PCICMD_SEE | B_PCH_RLINK_PCR_PCICMD_PERE);
    CmdSts = PchPcrWrite16 (PID_DMI, R_PCH_RLINK_PCR_PCICMD, CmdSts);
  }
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

