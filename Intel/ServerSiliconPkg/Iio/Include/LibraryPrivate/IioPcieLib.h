/** @file
This file defines private PCIe interface for the use by PCIe
initialization flow of IIO-RC.

@copyright
INTEL CONFIDENTIAL
Copyright 2019 - 2020 Intel Corporation. <BR>

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


#ifndef _IIOPCIELIB_H_
#define _IIOPCIELIB_H_

#include <Chip/Include/EvAutoRecipe.h>
#include <Base.h>
#include <IioPlatformData.h>

extern EV_RECIPE_HEADER IioUniphyRecipeVer;
extern EV_RECIPE_HEADER IioUniphyRecipeVer_Cpx;
extern EV_RECIPE_ENTRY  IioUniphyRecipe[];     // This table can be compressed
extern EV_RECIPE_ENTRY  IioUniphyRecipe_Cpx[];
extern UINT32           IioUniphyRecipeSize;   // Size of IioUniphyRecipe[] in bytes, if smaller than IioUniphyRecipeVer.NumberEntries table is compressed

extern EV_RECIPE_HEADER IioMgphyRecipeVer;
extern EV_RECIPE_ENTRY  IioMgphyRecipe[];      // This table can be compressed
extern UINT32           IioMgphyRecipeSize;    // Size of IioMgphyRecipe[] in bytes, if smaller than IioMgphyRecipeVer.NumberEntries table is compressed

extern EV_RECIPE_HEADER IioDmiphyRecipeVer;
extern EV_RECIPE_ENTRY  IioDmiphyRecipe[];      // This table can be compressed
extern UINT32           IioDmiphyRecipeSize;    // Size of IioDmiphyRecipe[] in bytes, if smaller than IioDmiphyRecipeVer.NumberEntries table is compressed

extern EV_RECIPE_HEADER IioPciephyRecipeVer;
extern EV_RECIPE_ENTRY  IioPciephyRecipe[];      // This table can be compressed
extern UINT32           IioPciephyRecipeSize;    // Size of IioDmiphyRecipe[] in bytes, if smaller than IioPciephyRecipeVer.NumberEntries table is compressed

extern EV_RECIPE_HEADER *IioMgPhyRecipeHeaderListIcxd[];    // Pointers to MgPhy recipe tables headers
extern UINT8            *IioMgPhyRecipeListIcxd[];          // This is list of pointers of compressed MgPhy recipe tables
extern UINT32           *IioMgPhyRecipeSizeListIcxd[];      // This is list of pointers to sizes of MgPhy recipe tables
extern UINT8            IioNoOfMgphyRecipeEntries;

/**
  Program Root port escalation status

  @param[in] IioIndex             - Socket number
  @param[in] PortIndex            - Port number
  @param[in] EnableCorrectableErr - Enable correctable errors
  @param[in] EnableNonFatalErr    - Enable non-fatal errors
  @param[in] EnableFatalErr       - Enable fatal errors
**/
VOID
RpErrEscalationProg (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex,
  IN  BOOLEAN                   EnableCorrectableErr,
  IN  BOOLEAN                   EnableNonFatalErr,
  IN  BOOLEAN                   EnableFatalErr
  );


/**
  Lock MSINXTPTR reg

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
LockMsiCapPtr (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  );

/**
  Lock PXPNXTPTR reg

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
LockExpCapPtr (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  );

/**
  Program necessary errors settings

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
PcieErrProg (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  );

/**
  The purpose of this routine is to do any PCIE Port programming prior to actually training the PCIE links

  @param[in] IioGlobalData - Pointer to IioGlobalData
  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
PcieEarlyInit (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  );

/**
  Program SOSCTL.SRISEN to enable SRIS support.

  @param[in] IioGlobalData - Pointer to IioGlobalData
  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
IioSrisEnable (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  );

/**
  Program MSIMsgCtrl register

  @param[in] IioIndex      - Index to CPU/IIO
  @param[in] PortIndex     - Index to PCIe port
  @param[in] MsiEnabled    - MSI interrupts should be enabled
 */
VOID
ProgramMsiMsgCtrl (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  IN  BOOLEAN  MsiEnabled
  );

/**
  Program power state in PMCSR register

  @param[in] IioIndex      - Index to CPU/IIO
  @param[in] PortIndex     - Index to PCIe port
  @param[in] PowerState    - Register value to set
 */
VOID
SetPowerState (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex,
  IN  UINT8  PowerState
  );

/**
    Gets and print rx stats per lane

    @param[in] IioGlobalData        - Pointer to IioGlobalData
**/
VOID
ShowRxStatsPrivate (
  IN  IIO_GLOBALS     *IioGlobalData
  );

/**
    This function will initialize PCIe Error Injection

    @param[in] IioIndex      - Index to CPU/IIO
    @param[in] PortIndex     - Index to PCIe port
**/
VOID
PcieErrInjConInitPrivate (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

/**
   The purpose of this routine is to configure earlier the PCIE Port Bifurcation
   Control register in order to access Pcie Configuration Space for Gen3
   that include ICX-D and SNR CPUs.

    @param[in] IioGlobalData - Pointer to IioGlobalData
    @param[in] IioIndex      - Index to Iio
**/
VOID
PcieDynamicPortSubdivision (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex
  );

/**
   The purpose of this routine is to do the necessary work
   required to enable the PCIE Ports. In addition this routine could and should be be
   used to do any other initialization required before PCI
   Express training.

    @param[in] IioGlobalData - Pointer to IioGlobalData
    @param[in] IioIndex      - Index to Iio
**/
VOID
PcieLinkTrainingInit (
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               IioIndex
  );

/**
    The purpose of this function is to apply all the
    Gen3 override required in the link

    @param[in] IioGlobalData - Pointer to IIO_GLOBALS structure
    @param[in] IioIndex      - Socket index
    @param[in] PortIndex     - Port index
**/
VOID
Gen3PrelinkOverride (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  );

/**
    The purpose of this function is to apply all the
    Gen4 override required in the link

    @param[in] IioGlobalData - Pointer to IIO_GLOBALS structure
    @param[in] IioIndex      - Socket index
    @param[in] PortIndex     - Port index
**/
VOID
Gen4PrelinkOverride (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  );

/**
    PCIe interrupt swizzle register init.

    @param[in] IioGlobalData - Pointer to IIO globals.
    @param[in] IioIndex      - Socket number of the PCIe device being initialized.
    @param[in] PortIndex     - Port number of the PCIe device being initialized.
**/
VOID
IntxSwzCtrlInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  );

/**
 If HotPlug is enabled by policy, HotPlug should be enabled by setting enable bit
 in VPPCSR register and defining number of PCA9555 device and slot number of PCA9555

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
IioVppHotPlugInit (
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex
  );

/**
    This function configures MiscCtrlSts1 register needed in early PCIe initializations.

    @param[in] IioIndex      - Socket number
    @param[in] PortIndex     - Port number
**/
VOID
ConfigureMiscCtrlSts1 (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
  );

/**
  Set no compliance bit for the given port.

  @param[in] IioIndex              - Socket number
  @param[in] PortIndex             - Port number
  @param[in] EnterNoCompliance     - TRUE or FALSE if given port should block compliance entry
**/
VOID
IioSetNoCompliance (
  IN  UINT8      IioIndex,
  IN  UINT8      PortIndex,
  IN  BOOLEAN    EnterNoCompliance
  );

/**
  Disable leaky bucket Gen3 degrade.

  The Gen4 IP can operate at Gen1/2/3 speeds depending on the device attached
  to the root port. There are separate enable bits for degrading from each of
  Gen2/3/4 to slower speeds whenever a leaky bucket error event happens at the
  given speed. They should all be disabled by default. However, the CSR default
  value has Gen3 degrade enabled for some steppings. This function works around
  that issue.

  Note that the Gen3 degrade feature may be re-enabled by the RAS IIO error
  handling code, depending on its policy settings.

  @param[in]  IioIndex    Socket number of the PCIe device.
  @param[in]  PortIndex   Port number of the PCIe device.
**/
VOID
DisableLeakyBucketGen3Degrade (
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  );


/**
  Clear error status bit6 in XPUNCERRSTS register
  Context: HSD 3613071

  @param[in] IioGlobalData - IIO global data structure
  @param[in] IioIndex      - IIO index
**/
VOID
ClearXPUncErrStsBit6 (
  IN  IIO_GLOBALS          *IioGlobalData,
  IN  UINT8                IioIndex
  );


/**
  Perform early workarounds for each PCIe port before its link is enabled

  @param[in] IioGlobalData - Pointer to IioGlobalData structure.
  @param[in] IioIndex      - IIO index
**/
VOID
IioPcieWorkarounds (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  );

/**
  Clear received PCIe Completion with UR Status.

  @param[in] IioIndex       IIO index (socket number)
  @param[in] PortIndex      PCIe port index
**/
VOID
DmiClearReceivedCompletionWithUrStatus (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

/**
  Perform early workarounds for each PCIe port before its link is enabled.
  These are cpu-specific WA (non-common for all pci-genX ports).

  @param[in] IioGlobalData  Pointer to IioGlobalData structure.
  @param[in] IioIndex       CPU/IIO-complex index
  @param[in] PortIndex      Index of the port
**/
VOID
IioPcieCpuSpecificWorkarounds (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

/**
 @brief Enable access to IOMT tool.

 @param[in] IioIndex - IIO instance index (usually socket).

 @return Void.
**/
VOID
IioEnableIomtRegisterAccess (
  IN  UINT8                           IioIndex
);

/**
  Enable OS Access to DMI and DFX functions.

  @param[in] IioIndex      - Socket number of the port being configured.
**/
VOID
IioEnableOsAccessDmiDfx (
  IN  UINT8                           IioIndex
  );

/**
  This function disables treating MsgD with len>16 as malformed TLP

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioDisableMsgDLenCheck (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  );

/**
  This function enabled the Completion Timeout Mask bit for all Root Ports
  in order RAS to work properly.

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port
**/
VOID
IioEnableCtoMaskBit (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  );

/**
  This function locks Subsystem Vendor ID

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioLockSvid (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  );

/**
  This function locks Subsystem Device ID

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioLockSdid (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  );

/**
  This function sets Bus Number for RootBusSn+1 (CPM, CPK and HQM) in OOBMSM registers

  @param[in] IioIndex     - IIO index (socket number)
**/
VOID
IioSetRootBusNoOfExtendedStack (
  IN  UINT8                           IioIndex
  );

/**
  Program UPCFGCSR to enable support for PCIE DLW (Dynamic Link Width)

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioPcieEnableDlw (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  );

/**
  IOSF parity detection support for CPM/CPK Virtual Root Port (VrP) and End Points associated.

  @param[in] IioIndex     - IIO index (socket number)

**/
VOID
IioParityEnable (
  IN  UINT8                           IioIndex
  );

/**
  IOSF parity detection support for CPK Virtual Root Port (VrP) and End Points associated.

  @param[in] IioIndex     - IIO index (socket number)

**/
VOID
IioParityEnableCpk (
  IN  UINT8                           IioIndex
  );

/**
  IOSF parity detection support for CPM Virtual Root Port (VrP) and End Points associated.

  @param[in] IioIndex     - IIO index (socket number)

**/
VOID
IioParityEnableCpm (
  IN  UINT8                           IioIndex
  );


/**
This function disable the ECRC generation and ECRC checking of the Advanced Error Capabilities and Control Register
for the given End Point described by the Bus, Device, Function information.

@param[in] IioIndex                   - IIO index (socket number)
@param[in] EndPointBus                - Bus,
@param[in] EndPointDev                - Device
@param[in] EndPointFun                - Function
@param[in] RegisterOffsetFromHeader   - Offset in header of the AECC register for given End Point
**/
VOID
DisableEcrcGenerationAndChecking(
  IN UINT8        IioIndex,
  IN UINT8        EndPointBus,
  IN UINT8        EndPointDev,
  IN UINT8        EndPointFun,
  IN UINT32       RegisterOffsetFromHeader
  );

/**
  This function sets Max Payload Size field  in Device Control Register for IPs in RootBusSn+1,
   and their RootBusSn (Virtual Root Port) when applicable.

  @param[in] IioIndex     - IIO index (socket number)

**/
VOID
IioSetMpsForIpInExtendedStack (
  IN  UINT8                           IioIndex
  );

/**
  Program ECRC Check Capable and ECRC Generation Capable bits on PCIe Root Ports

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioPcieSetEcrcCap (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  );

/**
  Program Multicast Capability -  ECRC regeneration supported on PCIe Root Ports

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioPcieSetMcstCap (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  );

/**
  This function will set PLKCTL.CL bit to lock all RWS-L bits - capabilities,
    next capability pointer, SSID/SVID, slot register, etc) - on PCIe ports.

  @param[in] IioIndex       - Index to the current Socket
  @param[in] PortIndex      - Index to the current PCIe Port
**/
VOID
PciePersonalityLockEnable (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex
  );

/**
  Used to set what lanes within the PCIE ports should be masked during link training
  according to setup knob Override Max Link Width.

  @param[in] IioGlobalData            Pointer to IioGlobalData
  @param[in] IioIndex                 IIO index (socket number)
  @param[in] PortIndex                PCIe port index
  @param[in] DefaultMaxLinkWidth      Platform Default Max link width
  @param[in] OverrideMaxLinkWidth     Max link width set by BIOS knob to override default
  @param[in] LaneReversedLink         Port configured as lane reversed
**/
VOID
IioPcieMaxLinkWidth (
  IIO_GLOBALS                               *IioGlobalData,
  UINT8                                     IioIndex,
  UINT8                                     PortIndex,
  UINT8                                     DefaultMaxLinkWidth,
  UINT8                                     OverrideMaxLinkWidth
);

/**
  This function determine the Register Type and write registers using IioPciExpressRead and IioPciExpressWrite functions.

  @param[in] IioIndex     - IIO instance (socket index)
  @param[in] PortIndex    - IIO port number
  @param[in] RegAddress   - Register to access
  @param[in] RegSize      - Register size in bytes
  @param[in] FieldMask    - Mask for data to write in register
  @param[in] DataEvRecipe - Data to write in the register provided by Electrical Validation team
  @param[in] TableIndex   - Register index for tracing/debug
  @param[in] Config       - Configuration type

  @retval                   EFI_SUCCESS
  @retval                   EFI_UNSUPPORTED
  @retval                   EFI_INVALID_PARAMETER
**/
EFI_STATUS
IioWriteSb (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8             IioIndex,
  IN  UINT8             PortIndex,
  IN  UINT32            RegAddress,
  IN  UINT8             RegSize,
  IN  UINT32            FieldMask,
  IN  UINT32            DataEvRecipe,
  IN  UINT32            TableIndex,
  IN  UINT16            Config
  );

/**
  This function determine the Register Type and write registers using IioPciExpressRead and IioPciExpressWrite functions.

  @param[in] IioGlobalData- IioGlobalData pointer
  @param[in] IioIndex     - IIO instance (socket index)
  @param[in] PortIndex    - IIO port number
  @param[in] RegAddress   - Register to access
  @param[in] RegSize      - Register size in bytes
  @param[in] FieldMask    - Mask for data to write in register
  @param[in] DataEvRecipe - Data to write in the register provided by Electrical Validation team
  @param[in] TableIndex   - Register index for tracing/debug
  @param[in] Config       - Configuration type

  @retval                   EFI_SUCCESS
  @retval                   EFI_UNSUPPORTED
  @retval                   EFI_INVALID_PARAMETER
**/
EFI_STATUS
IioWriteMemBar (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8             IioIndex,
  IN  UINT8             PortIndex,
  IN  UINT32            RegAddress,
  IN  UINT8             RegSize,
  IN  UINT32            FieldMask,
  IN  UINT32            DataEvRecipe,
  IN  UINT32            TableIndex,
  IN  UINT16            Config
  );

/**
   Specific 14nm settings for VPP

   @param[in] IioIndex      - Socket number
   @param[in] HpEnabled     - TRUE if Hot Plug is enabled
 */
VOID
VppPlatformInit (
  IN  UINT8    IioIndex,
  IN  BOOLEAN  HpEnabled
  );

/**
  Read FormFactor value from MiscCtrlStat1 register - this is supported only by Gen3 controllers
  @param[in]   IioIndex            Socket number
  @param[in]   PortIndex           Port number
  @param[out]  FormFactorPresent   Buffer will contains TRUE if FF value was read
  @param[out]  FormFactorValue     Buffer for Ff value (0xFF is value not read)
 */
VOID
GetMiscCtlStat1FormFactor (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  OUT BOOLEAN  *FormFactorPresent,
  OUT UINT8    *FormFactorValue
  );

/**
  This function is to initialize PCIe PTM GTE Control Status Register

  @param[in] IioIndex       - Index to CPU/IIO
  @param[in] PortIndex      - Index to PCIe port
**/
VOID
PcieEnablePtmGte (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

/**
  This function is to hide unused root ports.

  @param[in] IioIndex         Socket/IIO complex index
  @param[in] Stack            Stack Index
  @param[in] HideData         Pointer to data to write into register
**/
VOID
IioHideUnusedRootPorts (
  IN  UINT8                   IioIndex,
  IN  UINT8                   Stack,
  IN  UINT32                  *HideData
  );

/**
This function provides PCI Workaround for Intel IPs just before EDK enumeration.

@param[in] Segment            - Segment
@param[in] Bus                - Bus of the virtual root port
@param[in] Device             - Device of the virtual root port
@param[in] Function           - Function of the virtual root port
@param[in] DidVid             - Device-Vendor ID in the given Bus, Device, Function.

**/
VOID
IioPciIpHookBeforeEnumeration (
  IN  UINT8          Segment,
  IN  UINT8          Bus,
  IN  UINT8          Device,
  IN  UINT8          Function,
  IN  UINT32         DidVid
  );
#endif //_IIOPCIELIB_H_
