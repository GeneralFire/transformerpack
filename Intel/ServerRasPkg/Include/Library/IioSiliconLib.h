/** @file
  Interface of Iio silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef __IIO_SILICON_H__
#define __IIO_SILICON_H__

#define EDPC_FATAL_TRIGGER              0x1
#define EDPC_FATAL_NON_FATAL_TRIGGER    0x2

/**
   Enable DMI device RAS feature.

   @param  [in]  Socket  -- socket number
   @param  [in]  Bus  -- Bus number
   @param  [in]  Device -- Device number
   @param  [in]  Function -- function number

   @retval None

**/
VOID
EFIAPI
EnableDmiDevice (
  IN  UINT8    Socket,
  IN  UINT8    Bus,
  IN  UINT8    Device,
  IN  UINT8    Function
  );

VOID
EFIAPI
SetVtdSeverity (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

/**
  Program Vtd Error Mask register.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
SetVtdMsk (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

/**
  This function is to check the VTD Error, if there is error on VTD on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioVtdError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  Clear Vtd Error Error register.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
VOID
EFIAPI
ClearVtdStatus (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT32 ErrSeverity
  );

 /**
     Set DMA Errors Severity

     @param None

     @retval None

 **/
VOID
EFIAPI
IioSetDmaErrorsSev(
  IN   UINT8    Skt,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   Sev
  );

 /**

     Set DMA Errors Mask

     @param None

     @retval None

 **/
VOID
EFIAPI
IioSetDmaErrorsMsk (
  IN   UINT8    Skt,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   Msk
  );

/**
  This function is to check the CBDMA Error, if there is error on CBDMA on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioCbdmaError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

 /**

  Clear DMA Errors from silicon

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

     @retval None

 **/
VOID
EFIAPI
IioClearDmaErrorsSi (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

/**

Enable IRP


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none

**/
VOID
EFIAPI
IioEnableIrpError (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

/**

  Enable Iio ITC


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none

**/
VOID
EFIAPI
IioEnableItc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

/**

   Enable Iio TC


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none
 **/
 VOID
 EFIAPI
 IioEnableTc(
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

/**

   Enable Iio OTC


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none

 **/
 VOID
 EFIAPI
 IioEnableOtc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

/**

Enable Iio Enable IioSetIioMisc


  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  IioMiscErrorEn      - Policy Data
  @param  [in]  IioMiscErrorSevMap  - MIERRSV
  @param  [in]  IioMiscErrorBitMap  - MIERRCTL
  @retval none


**/
VOID
EFIAPI
IioSetIioMisc (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT8    IioMiscErrorEn,
  IN   UINT32   IioMiscErrorSevMap,
  IN   UINT32   IioMiscErrorBitMap
  );

/**
  This function is to check the IRP Error, if there is error on IRP on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioIrpError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  This function is to check the IRP RING Error, if there is error on IRP RING on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioIrpRingError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  This function is to check the ITC Error, if there is error on ITC on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioItcError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  This function is to check the OTC Error, if there is error on OTC on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioOtcError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  clear RAS pci device status, each iio stack have one RAS device.

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  ErrSeverity The error's severity

  @retval none
**/

VOID
EFIAPI
ClearRasDevice (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   ErrSeverity
  );

/**
  Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );


/**
  Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramUnCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );


/**
  Program Iio XP  Sev .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramErrSev (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
    Program Iio Correctable error count .

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioProgramCorrErrCnt (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
    Clear XP errors.

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXpClearErr (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
    set XP mask to disable.

    @param Socket    - Socket
    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function

**/
VOID
EFIAPI
SetXPMaskError (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function is to get the XP Global Error Status registers and to see if one of the enabled errors
  has occurred.

  @param[in] Socket         PCIe root port device's socket number.
  @param[in] Bus            PCIe root port device's bus number.
  @param[in] Device         PCIe root port device's device number.
  @param[in] Function       PCIe root port device's function number.
  @param[out] Xpcorsts      Xp correctable error status.
  @param[out] Xpuncsts      Xp uncorrectable error status.
  @param[out] xpglbsts      xp global error status.
  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected, or the device is not a root port.
--*/
BOOLEAN
IioGetXpErrorStatus (
  IN  UINT8    Socket,
  IN  UINT8    Bus,
  IN  UINT8    Device,
  IN  UINT8    Function,
  OUT UINT32   *Xpcorsts,
  OUT UINT32   *Xpuncsts,
  OUT UINT32   *Xpglbsts
  );

/**
  This function ennable UBOX IOMCA register

  @param[in] IioSev1Pcc        PCC equal 0 for severity 1 error.

  @retval    none.
**/
VOID
EFIAPI
UboxEnableIoMca (
  UINT8     IioSev1Pcc
  );

/**
  This function Disables IOMCA logging

  @retval    none.
**/
VOID
EFIAPI
UboxDisableIoMca (
  VOID
  );

/**
  This function enable enable poismsg spec behavior

   @param Socket    - Socket
   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function

   @retval    none.
**/
VOID
EFIAPI
IioEnablePoisMsgSpecBehaviorVtd (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function enable forwarding of Poison indication for VTD

   @param Socket    - Socket
   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function

   @retval    none.
**/
VOID
EFIAPI
IioEnablePoisonVtd (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function enable forwarding of Poison indication with the data.

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param CorrErrMsk  - Correctable error mask

  @retval    none.
**/
VOID
EFIAPI
IioEnablePoison (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function disable forwarding of Poison indication with the data.

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param CorrErrMsk  - Correctable error mask

  @retval    none.
**/
VOID
EFIAPI
IioDisablePoison (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function enable forwarding of Viral indication

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param ClearFlg  - Viral status clear flag
   @param IoMcaEn   - IoMca Enable

   @retval    none.
**/
VOID
EFIAPI
IioEnableViral (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT8   ClearFlg,
  IN      UINT8   IoMcaEn
  );

/**
  Disable IIO MSI error.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioDisMsiError (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  Program IIO errors (Correctable, Uncorrectable, Fatal).

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioProgramErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function will enable all the RP PIO errors.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
RppIoErrEnable (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  Program Leaky Bucket registers.

  The BDF provided is assumed to be an IIO PCIe root port. This function
  should not be called for any other device.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
IioProgramLeakyBucket (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  Get leacky bucket lane error mask.

  Return the error status mask of one lane per bit. If at least one lane has
  exceeded the threshold (status bit is set), get the status for
  re-equalization and degradation, and the error counts for each lane in the
  output parameters.

  Note that if the lane error mask is 0, the output parameters are not updated
  and should be ignored.

  @param[in]  Socket            Socket index.
  @param[in]  Bus               PCI bus number.
  @param[in]  Device            PCI device number.
  @param[in]  Function          PCI function number.
  @param[out] LaneErrorCount    Array of error counts for each lane.
  @param[out] Gen34ReEqualize   TRUE if Gen3/4 re-equalization was requested.
  @param[out] Gen2LinkDegrade   TRUE if Gen2 link degradation occurred.
  @param[out] Gen3LinkDegrade   TRUE if Gen3 link degradation occurred.
  @param[out] Gen4LinkDegrade   TRUE if Gen4 link degradation occurred.

  @return     The bitmask of lanes that were triggered.
**/
UINT16
EFIAPI
GetLeakyBucketLaneErrorMask (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  OUT     UINT8   LaneErrorCount[16],
  OUT     BOOLEAN *Gen34ReEqualize,
  OUT     BOOLEAN *Gen2LinkDegrade,
  OUT     BOOLEAN *Gen3LinkDegrade,
  OUT     BOOLEAN *Gen4LinkDegrade
  );

/**
  Clear leacky bucket status.

  Clear the leaky bucket error status for each lane.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
ClearLeakyBucketStatus (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  Dump IOH PCIe registers.

  Dump the IOH PCIe registers for a given BDF on a given socket.

  @param[in]  Socket    Socket index.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
**/
VOID
EFIAPI
DumpIohPcieRegisters (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  Programming M2PCIE Error Configuration

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
M2PcieErrorConfig (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

/**
  This function is to check the M2PCIE Error, if there is error on M2PCIE on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioM2pcieError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  );

/**
  Clear M2PCIE Error Status register.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
ClearM2pcieErrorStatus (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

/**
  This function checks if a Silicon specific PCI or PCIe device exist on a specificed address.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   Device is a Silicon specific device and it is exist.
                            False  Device is not a silicon specific device or it is not exist.
**/
BOOLEAN
EFIAPI
IsSiSpecPcieDeviceExist (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      VOID    *MmInfo
  );

/**
  This function is to get correctable error counter mask value

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    correctable error counter mask value
**/
UINT32
EFIAPI
IioGetCorrCntErrMsk (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      VOID    *MmInfo
  );
#endif
