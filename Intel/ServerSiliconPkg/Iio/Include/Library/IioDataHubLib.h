/** @file
  This file defines everything that's needed for IIO Data Hub.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_DATA_HUB_LIB_H_
#define _IIO_DATA_HUB_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <IioPlatformData.h>
#include <Library/CommonTraceHubLib.h>

#include <IioConfig.h>

//
// Define the Stack ID based on CPUId location
//
#define IIO_DMI_CB_SINGLE_PCIE   0
#define IIO_CB_RLINK             0
#define IIO_CB_RLINK_NAC         0
#define IIO_CB_DMI_GEN4          0
#define IIO_DUAL_PCIE            1
#define IIO_FLIPPED_DUAL_PCIE    1
#define IIO_DUAL_PCIE_MCP        2
#define IIO_SINGLE_PCIE_0        1
#define IIO_SINGLE_PCIE_1        2
#define IIO_SINGLE_PCIE_2        3
#define IIO_SINGLE_PCIE_3        4
#define IIO_SINGLE_PCIE_4        5
#define IIO_SINGLE_PCIE_5        6
#define IIO_SINGLE_PCIE_6        7

#define IIO_DINO_0               8
#define IIO_DINO_1               9
#define IIO_DINO_2               10
#define IIO_DINO_3               11

#define IIO_SINGLE_HFI_0         2
#define IIO_SINGLE_HFI_1         3
#define IIO_DUAL_GUINT_0         2
#define IIO_DUAL_GUINT_1         3
#define IIO_RLINK                4
#define IIO_CPK_NAC              3
#define IIO_CPM_NAC              4
#define IIO_HQM                  2
#define IIO_CPM_NAC_ICXD         2
#define IIO_CPK_NAC_ICXD         3
#define IIO_CPK_NAC_ICXD_LCC     2
//
// Error code definitions below are obsolete and will be removed soon.
// Do not use, use standard EFI error codes instead.
//
#define IIO_SUCCESS              0
#define IIO_ERROR                0xE00000FF
#define IIO_NOT_READY            0xE00000FE
#define IIO_UNSUPPORTED          0xE00000FD
#define IIO_INVALID_PARAMETER    0xE00000FC
#define IIO_OUT_OF_RESOURCES     0xE00000FB

typedef enum {
  Ch0 = 0,
  Ch1,
  Ch2,
  Ch3,
  Ch4,
  Ch5,
  Ch6,
  Ch7,
  LastChannel
} IIO_CHANNELS;

//
// Used to differentiate between port 0 modes of operation
//
#define VID_DID_SKX_DMI_PORT    0x20208086
#define VID_DID_SKX_PCIE_PORT   0x20308086

//
// This enum lists possible current role of DMI port in the system.
//
typedef enum {
  IioDmiPort,         ///< IIO port is capable as functioning as a DMI port,
                      ///< regardless of current role in the system
  IioDmiPortActive,   ///< IIO port is capable as functioning as a DMI port,
                      ///< and is currently functioning as a DMI port
  IioDmiPortInactive, ///< IIO port is capable as functioning as a DMI port,
                      ///< but is not currently functioning as a DMI port;
                      ///< typically this would occur on a non-legacy socket (i.e. not socket 0)
  IioDmiPortAsPci,    ///< IIO port is capable as functioning as a DMI port,
                      ///< but currently acting as PCIe
  IioDmiIdentityMax
} IIO_DMI_IDENTITY;

/**
  This function retrieves IioGlobalData structure pointer from HOB if necessary

  @param IioGlobalData - Pointer to IioGlobalData pointer

  @retval EFI_SUCCESS       If IioGlobalData pointer retrieved successfully.
  @retval EFI_NOT_FOUND     If IioSiPolicyHob is not found.

**/
EFI_STATUS
GetIioGlobalData (
  IN OUT IIO_GLOBALS **IioGlobalData
  );

/**
  This function provides IioGlobalData structure pointer retrieves it from HOB if necessary

  @param IioGlobalData - Pointer to IioGlobalData
  @param FetchFromHob  - allow to override saved pointer find the HOB again

  @retval EFI_SUCCESS       If IioGlobalData pointer retrieved successfully.
  @retval EFI_NOT_FOUND     If IioSiPolicyHob is not found.

**/
EFI_STATUS
GetIioGlobalDataEx (
  IN OUT IIO_GLOBALS        **IioGlobalData,
  IN     BOOLEAN            FetchFromHob
  );

VOID
IioStall (
  UINT32      UsDelay
  );


VOID
IioPreLinkDataInitPerSocket (
  IN OUT IIO_GLOBALS     *IioGlobalData,
  IN UINT8                Iio
  );

BOOLEAN
IsPciePortAvailable (
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS       PciAddress,
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                              IioIndex
  );

BOOLEAN
IsPciePortDLActive (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

BOOLEAN
IsPCIeGen4 (
  VOID
  );

/**
  Returns generation of the PCIE port.

  @param[in] IioGlobalData - Pointer to IIO global data
  @param[in] IioIndex      - Socket Index
  @param[in] PortIndex     - Port Index

  @retval The ports supported Pcie controller generation.
**/
IIO_PORT_GEN
IioPortControllerGen (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

BOOLEAN
IsPciePortTPHCapable (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  );

BOOLEAN
IsIioPortPopulated (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               PortIndex
  );

BOOLEAN
IioVData_ConfigurePciePort (
  IN  IIO_GLOBALS                     *IioGlobalData,
  IN  UINT8                           Iio,
  IN  UINT8                           PortIndex
  );

UINT8
CheckBifurcationMapPort (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  );

BOOLEAN
CheckPciePortEnable (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             Iio,
  IN  UINT8                             PortIndex
  );

UINT8
CheckVarPortEnable (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             Iio,
  IN  UINT8                             PortIndex
  );

VOID
SetPcieLinkWidthInit (
  IIO_GLOBALS  *IioGlobalData,
  UINT8        IioIndex,
  UINT8        PortIndex
  );

/**
  Returns ASPM value for given IIO port based on global ASPM override and per-port configuration
  If the ASPM shouldn't be changed previous value is returned.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval PCIE_LIB_ASPM_POLICY - ASPM value to set in for port
**/
UINT8
GetAspmSettingForPort (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8            IioIndex,
  IN  UINT8            PortIndex
  );

UINT8
GetAssignedPortAddressWidth (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS       PciAddress
  );

UINT8
GetAssignedPortIndexWidth (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             Iio,
  IN  UINT8                             PortIndex
  );

UINT64
GetPortBarSize (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  );

UINT64
GetPortBarAddr (
  IN  UINT8         IioIndex,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  UINT16        Offset
  );

/**
  This function checks whether given IIO port is DMI port of given identity.

  @param[in] IioIndex     - Index of IIO instance
  @param[in] PortIndex    - IIO port index
  @param[in] DmiIdentity  - current role of DMI port in the system

  @return TRUE if port is acting as specified in the DmiIdentity parameter, otherwise FALSE.
**/
BOOLEAN
IioDmiIsPortIdentity (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex,
  IN  IIO_DMI_IDENTITY   DmiIdentity
  );


/**
  This function checks if the given CPU is connected to R-Link

  @returns TRUE if the CPU is connected to R-Link, otherwise FALSE.
**/
BOOLEAN
HasRlink (
  VOID
  );


BOOLEAN
CheckNtbPortConfSetting (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  );

VOID
IioPortCalculateLaneDisableMask (
  IIO_GLOBALS                           *IioGlobalData,
  UINT8                                 IioIndex,
  UINT8                                 PortIndex,
  UINT16                                *LaneMask
);

VOID
SetMemorySpaceEnable (
  IN  UINT8         IioIndex,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  UINT16        Offset,
  IN  BOOLEAN       Enable
  );

/**
  Return TRUE if VMD 2.0 is supported
 */
BOOLEAN
IsVmd20Supported (
    VOID
  );

BOOLEAN
IsVMDEnabledForStack (
  IN  UINT8              IioIndex,
  IN  UINT8              StackIndex
  );

/**
    Check if given Port on the IIO complex will be configured as VMD

    @param IioIndex      - Iio Index
    @param PortIndex     - Port Index

    @return TRUE if given port will be configured as VMD, FALSE otherwise.
**/
BOOLEAN
IsVMDEnabledForPort (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  );


BOOLEAN
CheckFPGAPort (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex
  );

BOOLEAN
IsSuperClusterPort (
  UINT8        PortIndex
  );

BOOLEAN
CheckPortBifx8Low (
  UINT8 IioIndex,
  UINT8 PortIndex
  );

BOOLEAN
CheckPortBifx8High (
  UINT8 IioIndex,
  UINT8 PortIndex
  );

BOOLEAN
CheckPortBifx4Low (
  UINT8 IioIndex,
  UINT8 PortIndex
  );

BOOLEAN
CheckPortBifx4High (
  UINT8 IioIndex,
  UINT8 PortIndex
  );

BOOLEAN
IsPcieRootPortStack (
  IN IIO_GLOBALS        *IioGlobalData,
  IN UINT8              IioIndex,
  IN UINT8              StackIndex
  );

UINT8
GetCbDmaStack (
  VOID
  );


/**
  Determine if the specified stack is the DMI stack

  @param Stack - The IIO stack to check.

  @return TRUE/FALSE depending on whether the specified PCIe stack is DMI or not
**/
BOOLEAN
IsDmiStack (
  IN  UINT8             Stack
  );

/**
  Get the stack which the PCIe port belongs to

  @param[in] IioIndex       Socket Index
  @param[in] PortIndex      Port Index

  @retval  Stack number
**/
UINT8
IioGetStackForPort (
  IN UINT8     IioIndex,
  IN UINT8     PortIndex
  );

/**
  Returns true if given socket is present in current configuration
  For now this is only wrapper on IioGlobal var.

  @param[in]  IioIndex  Index of the socket
  @retval  TRUE   if given socket is present
  @retval  FALSE  otherwise
 */
BOOLEAN
IioIsSocketPresent (
  IN  UINT8  IioIndex
  );

/*
  Return if the stack indexed is present or not

  @param[in] IioIndex      - Socket Index
  @param[in] StackIndex    - Stack Index

  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present
**/
BOOLEAN
IioIsStackPresent (
  IN UINT8     IioIndex,
  IN UINT8     StackIndex
  );

BOOLEAN
IioIsPortPresent (
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  );

/**
  Get MS2IOSF channels per stack.

  @param    StackIndex  Stack Index

  @retval   Channels    Channels Connected
**/
UINT8*
GetMs2iosfChannels (
  IN  UINT8    StackIndex
  );

UINT8
GetMs2iosfChannelIndex (
  IN  UINT8    PortIndex
  );


IIO_CONFIG*
GetIioConfig ( VOID );

IIO_V_DATA*
GetIioVData ( VOID );

IIO_OUT_DATA*
GetIioOutData ( VOID );

UINT8
GetSocketPortBusNumber (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

/**
  Function to obtain bus number when the stack is the Uncore
  @param IioIndex                 - The socket number
  @param UncoreStackIndex         - The index of the uncore stack
  @param UncoreBusNumber          - Pointer to the value that represents the bus number (output)

  @retval EFI_INVALID_PARAMETER     When invalid case options
  @retval EFI_SUCCESS               When a valid assignment is done
**/
EFI_STATUS
GetSocketUncoreBusNumber (
  IN    UINT8      IioIndex,
  IN    UINT8      UncoreStackIndex,
  OUT   UINT8      *UncoreBusNumber
  );

UINT8
GetPciePortDevice (
  IN UINT8 PortIndex
  );

UINT8
GetPciePortFunction (
  IN UINT8 PortIndex
  );

UINT8
GetPciePortSuperClusterPort (
  IN UINT8 PortIndex
  );


INTEL_IIO_PORT_INFO*
GetPciePortInfo (
  IN UINT8 PortIndex
  );

UINT8
GetPciePortOwnership (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  );

UINT8
GetPciePortLinkSpeed (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex,
  IN  UINT8        SupportedSpeedsVector
  );

EFI_STATUS
GetPcieStackPerPort (
  IN     UINT8  IioIndex,
  IN     UINT8  PortIndex,
     OUT UINT8  *StackIndex

  );

UINT8
GetMaxPciePortNumberPerSocket (
  IN UINT8 IioIndex
  );

VOID
GetPciePortBusDeviceFunction (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN OUT PCI_ROOT_BRIDGE_PCI_ADDRESS *PciAddress
  );

/**
IioTraceHubGetPciBusNo: Returns the NPK PCI bus number

@param[in]  TraceHubDevice - Specify CPU or PCH trace hub device
@param[in]  IioIndex       - Index to CPU/IIO
@param[in]  Stack          - Stack number

@retval  UINT8             - Returns the NPK PCI bus no
**/
UINT8
IioTraceHubGetPciBusNo (
  IN TRACE_HUB_DEVICE     TraceHubDevice,
  IN UINT8                IioIndex,
  IN UINT8                Stack
  );

/**
  Returns TRUE if currently running on CPX cpu

  @retval TRUE  - this is CPX
  @retval FALSE - this is not CPX
 */
BOOLEAN
IsThisCpx ( VOID );

/**

  Returns boolean if the fourth PCIe Gen3 controller is present
  for a given CPU socket.

  @param IioIndex          - Socket ID of the CPU who's sku info requested, 0...3

  @retval TRUE  - 4th controller present
  @retval FALSE - otherwise

**/
BOOLEAN
IioIsFourthPCIeGen3ControllerPresent (
  IN UINT8         IioIndex
  );

/**
  Checks if PCI device at given address exists.

  @param[in]  IioIndex         - Socket ID
  @param[in]  Bus              - PCI bus number
  @param[in]  Device           - PCI device number
  @param[in]  Function         - PCI function number
  @param[out] PciHdrPtr        - Optional pointer to a buffer for PCI device header

  @retval TRUE if exists
  @retval FALSE otherwise
**/

BOOLEAN
IioIsDevicePresent (
  IN  UINT8                      IioIndex,
  IN  UINT8                      Bus,
  IN  UINT8                      Device,
  IN  UINT8                      Function,
  OUT PCI_TYPE_GENERIC           *PciHdrPtr
  );

/**
  Checks whether it is HCC SKU or not.

  @param[in] IioIndex - IIO instance index (usually socket)

  @retval TRUE    if it is HCC SKU
  @retval FALSE   otherwise
**/
BOOLEAN
IioIsHccSku (
  UINT8 IioIndex
  );

/**
  Checks whether it is LCC SKU or not.

  @param[in] IioIndex - IIO instance index (usually socket)

  @retval TRUE    if it is LCC SKU
  @retval FALSE   otherwise
**/
BOOLEAN
IioIsLccSku (
  UINT8 IioIndex
  );

/**
  Returns Pcie Negotiated Link Width from link status register.

  @param[in]  IioIndex               - Socket ID
  @param[in]  PortIndex              - Controller ID
  @param[out] NegotiatedLinkWidthPtr - Pointer to UINT16 Value that returns Negotiated Link Width

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieNegotiatedLinkWidth (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *NegotiatedLinkWidthPtr
  );

/**
  Returns Pcie Max Link Width from link capability register.

  @param[in]  IioIndex          - Socket ID
  @param[in]  PortIndex         - Controller ID
  @param[out] MaxLinkWidthPtr   - Pointer to UINT32 Value that returns Max Link Width

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieMaxLinkWidth (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT32                      *MaxLinkWidthPtr
  );

/**
  Returns Pcie Current Link Speed from link status register.

  @param[in]  IioIndex             - Socket ID
  @param[in]  PortIndex            - Controller ID
  @param[out] CurrentLinkSpeedPtr  - Pointer to UINT16 Value that returns Current Link Speed

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieCurrentLinkSpeed (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *CurrentLinkSpeedPtr
  );

/**
  Returns Pcie Target Link Speed from link control2 register.

  @param[in]  IioIndex           - Socket ID
  @param[in]  PortIndex          - Controller ID
  @param[out] TargetLinkSpeedPtr - Pointer to UINT16 Value that returns Target Link Speed

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieTargetLinkSpeed (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *TargetLinkSpeedPtr
  );

/**
   Gets the mode of the given stack.

   @param[in] IioGlobalData - Pointer to IIO global data
   @param[in] IioIndex      - IIO instance index (usually socket)
   @param[in] Stack         - IIO stack index

   @retval PI5_MODE_DMI           Stack is in DMI mode (only stack 0 for SPR)
   @retval PI5_MODE_IAL           Stack is in IAL mode
   @retval PI5_MODE_GEN4_ONLY     Stack is in Gen4 only mode
   @retval PI5_MODE_GEN5_ONLY     Stack is in Gen5 only mode
   @retval PI5_MODE_GEN5_AND_GEN4 Stack is in Gen5/Gen4 mixed mode
   @retval PI5_MODE_NTB_GEN4      Stack is in Gen4 only NTB mode
   @retval PI5_MODE_NTB           Stack is in Gen5/Gen4 mixed NTB mode
**/
PI5_MODES
GetStackMode (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        Stack
  );

/**
    This function hides PCIE Port menu in setup menu for ports that can not be configured.

    @param[in] IioIndex      - Socket number of the port being initialized.
    @param[in] PortIndex     - Port number being initialized.
    @param[in] IioGlobalData - Pointer to IIO globals.
**/
VOID
SetPEXPHideForNotConfiguredPorts (
  IN     UINT8                           IioIndex,
  IN     UINT8                           PortIndex,
  IN OUT IIO_GLOBALS                     *IioGlobalData
  );

/**
  Checks whether the given channel index is Vc1 or Vcm.

  @param[in] IioIndex     - Socket Index
  @param[in] StackIndex   - MS2IOSF stack index
  @param[in] ChannelIndex - MS2IOSF stack channel index
  @param[in] CheckVc1     - Check if this MS2IOSF channel is assigned to Vc1
  @param[in] CheckVcm     - Check if this MS2IOSF channel is assigned to Vcm

  @retval TRUE    if given channel is Vc1 or Vcm based on input check
  @retval FALSE   otherwise
**/
BOOLEAN
IsVc1OrVcm (
  IN UINT8    IioIndex,
  IN UINT8    StackIndex,
  IN UINT8    ChannelIndex,
  IN BOOLEAN  CheckVc1,
  IN BOOLEAN  CheckVcm
  );

/**
  Returns TRUE if any lane is assigned to given port.

  @param[in]  IioGlobalData  Pointer to IIO globals
  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number

  @return     TRUE if any lane is assigned to given port
 */
BOOLEAN
IioAreLanesAssignedToPort (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

/**
  Returns number of lanes assigned to given port based on this port bifurcation

  @param[in]  IioGlobalData  Pointer to IIO globals
  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number

  @return     number of lanes assigned to given port
 */
UINT8
IioGetLanesNumOfPort (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

/**
  Returns TRUE if HotPlug is enabled for given port

  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number
 */
BOOLEAN
IioIsHpEnabledOnPort (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  );

/**
  Get HCX sub-system type
  @param[in] IioIndex

  @retval    The HCX sub-system type.
**/
IIO_HCX_TYPE
EFIAPI
GetHcxType (
  IN  UINT8       IioIndex
  );
#endif // _IIO_DATA_HUB_LIB_H_
