/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_DMI_CONF_H_
#define _IIO_DMI_CONF_H_

#include <IioPlatformData.h>

/**
  Lock DMI port capabilities

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
 */
VOID
IioDmiPersonalityLockEnable (
  IN  UINT8                         IioIndex,
  IN  UINT8                        PortIndex
  );

/**
  Program Vc channels and Tc/Vc mapping

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
 */
VOID
IioDmiProgramTcVcRegs (
  IN  IIO_GLOBALS *IioGlobalData
  );

/**
  Poll Vc channels and Tc/Vc mapping

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
 */
VOID
IioDmiPollTcVcRegs (
  IN  IIO_GLOBALS *IioGlobalData
  );

/**
  Disable gen. 2 override bit so that DMI will train to gen. 3
  Needed only for selected CPUs.

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
DisableGen2Override (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Disable DMI on given socket

  @param[in]  IioIndex  Socket/IIO complex index
 */
VOID
DisableDmiOnSocket (
  IN UINT8 IioIndex
  );

/**
  Returns TRUE if multiPch is running on this socket

  @param[in]  IioIndex  Socket/IIO complex index
  @return TRUE if multiPCH is running on this socket
 */
BOOLEAN
IsMultiPchRunningOnThisSocket (
  IN UINT8 IioIndex
  );

/**
  This function locks the North Bridge (Dmi, Rlink or other IP) LogPHY space,
  setting Secured Register Lock field in LCFG register.
 */
VOID
LockLogicalPhyNorthBridge (
  VOID
  );


/**
  Program DMI PCI command register:
    - set BME/MSE bits
    - disable interrupts

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiProgramCmdRegister (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Write back INT-PIN register to lock its value

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
LockIntPinRegister (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Program DMI MaxPayloadSize value

  @param[in]  IioIndex                Socket/IIO complex index
  @param[in]  PortIndex               Index of the port
  @param[in]  MaxPayloadSizeSupValue  Value to set in maxPayloadSizeSupported field
*/
VOID
IioDmiSetMpss (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT8 MaxPayloadSizeSupValue
  );

/**
  Cleanup ARI bit in DevCap2 register

  SA BSU 0.8.11
  devcap2.ari_en default is "1" for DMI port and it should be set to 0 by BIOS
  This is a permanent workaround

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiCleanUpDevCap2AriBit (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Program MiscCtrlSts register

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiMiscCtrlStsInit (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

/**
  Program PCIe Device Control register in DMI controller.

  @param[in] IioIndex                    Socket/IIO complex index
  @param[in] PortIndex                   Index of the port
  @param[in] UnsupportedRequestsEnabled  Enable/Disable Unsupported Requests bit
  @param[in] MaxPayloadSize              Max payload encoded as in the Device Control register
*/
VOID
IioDmiProgramDevCtrlReg (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN BOOLEAN  UnsupportedRequestsEnabled,
  IN UINT8    MaxPayloadSize
  );

/**
  Program DMI PmCsr register

  @param[in]  IioIndex    Socket/IIO complex index
  @param[in]  PortIndex   Index of the port
  @param[in]  PowerState  Power state register value
*/
VOID
IioDmiProgramPmCsr (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN UINT8    PowerState
  );

/**
  Program DMI DevCtrl2 register

  @param[in]  IioIndex                    Socket/IIO complex index
  @param[in]  PortIndex                   Index of the port
  @param[in]  ComplToDis                  CompletionTimeoutDisable register value
  @param[in]  ComplToVal                  CompletionTimeoutValue register value
*/
VOID
IioDmiProgramDevCtr2lReg (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN UINT8    ComplToDis,
  IN UINT8    ComplToVal
  );

/**
  Enable MSI on DMI Port

  @param[in]  IioIndex    Socket/IIO complex index
  @param[in]  PortIndex   Index of the port
  @param[in]  MsiEnabled  If MSI should be enabled for the port
*/
VOID
IioDmiEnableMsi (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex,
  IN BOOLEAN    MsiEnabled
  );

/**
  Clear vc1m_nosnoopopdis bit for DMI

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiClearVc1mNoSnoopopis (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
    Program DMI PerfCtrlSts register

    @param[in]  IioGlobalData  Pointer to global Data
    @param[in]  IioIndex       Socket number
    @param[in]  Port           Port number
**/
VOID
IioDmiProgramPerfCtrlSts (
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          PortIndex
  );

/**
  Program RootPortCmd register

  @param[in]  IioIndex       Socket number
  @param[in]  PortIndex      Index of the PCIE port
  @param[in]  CereEn         Enable correctable error reporting
  @param[in]  NfereEn        Enable non fatal error reporting
  @param[in]  FereEn         Enable fatal error reporting
**/
VOID
IioDmiProgramRootPortCmdReg (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  IN  BOOLEAN  CereEn,
  IN  BOOLEAN  NFereEn,
  IN  BOOLEAN  FereEn
  );

/**
  Set address and enable DmiRcBar

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
  @param[in]  DmiRcBarAddress  Address to set in DmiRcBar
*/
VOID
IioDmiEnableDmiRcBar (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex,
  IN UINT32     DmiRcBarAddress
  );

/**
  This function lock registers which needs to stay RO later

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiLockRegisters (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  );

/**
  This function clear Dmi error masks

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiClearErorMasks (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  );

/**
  Enable System Error Reporting (SERRE)

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiSerreInit (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  );

/**
  Program DMI port LinkCap register

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramLinkCap (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PortIndex
  );

/**
  Program DMI port LinkCap2 register

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramLinkCap2 (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PortIndex
  );

/**
  Program DMI port LinkCtls registers (LinkCtl, LnkCtl2, LinkCtl3)

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
  @param[in]  TargetLinkSpeed  Link speed to set in Link Control2 register
*/
VOID
IioDmiProgramLinkCtls (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PortIndex,
  IN UINT8        TargetLinkSpeed
  );

/**
  Program DMI port XptDefs registers (XptDef / XptDef2)

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramXptDefs (
  IN UINT8        IioIndex,
  IN UINT8        PortIndex
  );


/**
  Returns link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT16
IioDmiReadLinkStatus (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Set link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @param[in]  RegValue   Register value to set
*/
VOID
IioDmiWriteLinkStatus (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT16 RegValue
  );

/**
  Returns link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT16
IioDmiReadLinkControl (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Set link control register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @param[in]  RegValue   Register value to set
*/
VOID
IioDmiWriteLinkControl (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT16 RegValue
  );

/**
  Clean errors which can stay in registers after successful link training

  @param[in] IioIndex         Index to the current Socket
  @param[in] PortIndex        Index to the current PCIe Port
**/
VOID
IioDmiClearErrorsAfterLinkTraining (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

/*
  Returns link capabilities2 link speed value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT8
IioDmiGetLinkSpeed (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  This routine will update DMI link speed on PCH side.

  On input it takes a vector (bitmap) of supported IIO link speeds,
  matches to the PCH supported link speed vector, and the higest
  speed supported by both IIO and PCH is selected to be set in PCH
  and returned so that the caller can set it also in IIO stack.

  @param[in] IioInded  IIO instance (usually socket)
  @return On exit the highest common link speed is returned.
**/
UINT8
IioDmiSetPchLinkSpeed (
  IN  UINT8  IioIndex
  );

/**
  Returns link capabilities2 link speed value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT8
IioDmiGetLinkSpeedVector (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Return Max Payload Size set for DMI in Device Control register.
  Should be used after IioPostLateInitialize () completed.

  @retval  DMI Max Payload Size as defined in PCIe Device Control register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
EFIAPI
IioDmiIpGetMaxPayloadSize (
  VOID
  );

/**
  Return Max Payload Size Supported set for DMI in Device Capabilities register.

  @retval  DMI Max Payload Size Supported as defined in PCIe Device Capabilities register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
IioDmiIpGetMaxPayloadSizeSupported (
  VOID
  );

#endif /* _IIO_DMI_CONF_H_*/
