/** @file
  Interface of Iio RAS library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef __IIO_RAS_LIB_H__
#define __IIO_RAS_LIB_H__

#include <Library/PlatPolicyLib.h>
#include <Library/RasMailBoxLib.h>
#include <Guid/RasNonStandardErrorSection.h>
/**
  This function clears the IIO root port status registers for the
  specified PCI Express device.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval    None
**/
VOID
EFIAPI
ClearRootPortStatus (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT32  ErrSeverity
  );


/**
  This function masks PCI exp errors reporting.

  @retval    Status.
**/
 VOID
 EFIAPI
 IioPcieMaskErrors (
  IN   UINT8                    Socket,
  IN   UINT8                    Bus,
  IN   UINT8                    Device,
  IN   UINT8                    Function
  );

/**
  Function to enable PCH DMI Errors

  @param[in] PcieErrEn       - Pcie error enable option.
  @param[in] PcieCorrErrEn   - correctable error enable.
  @param[in] PcieUncorrErren - uncorrectable error enable.
  @param[in] ErrCorMsk       - Correctable error Mask.
  @param[in] ErrUncMsk       - Uncorrectable error Mask.
  @param[in] ErrUncSev       - Uncorrectable error severity.
**/
VOID
EFIAPI
PchRasEnableDmiError(
  IN UINT8              PcieErrEn,
  IN UINT8              PcieCorrErrEn,
  IN UINT8              PcieUncorrErrEn,
  IN UINT32             ErrCorMsk,
  IN UINT32             ErrUncMsk,
  IN UINT32             ErrUncSev
  );

/**
  Enable PCH RAS feature

  @param none
  @retval none
**/
VOID
EFIAPI
PchEnableErrors (
  VOID
  );

 /**

     Enable DMA

     @param None

     @retval None

 **/
VOID
EFIAPI
IioEnableDmaErrors (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

 /**

  Clear DMA Errors

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  ErrSeverity -- Error Severity

     @retval None

 **/
VOID
EFIAPI
IioClearDmaErrors (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   ErrSeverity
  );

/**
  Function to clear PCH errors

  @retval None.
**/
VOID
EFIAPI
PchClearErrors (
  VOID
  );

/**
  Handler for PCH DMI

  @param[in] Bus        Device BUS number to check

  @retval EFI_SUCCESS.

**/
VOID
EFIAPI
PchDmiErrorHandler (
  VOID
  );

/**
  This function enable Enable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
IioCpuEnableIoMca (
  IN      VOID
  );

/**
  This function enable Disable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
IioCpuDisableIoMca (
  IN      VOID
  );



////////////////////////////////////////////////////////////////////////
//LER function definitions
////////////////////////////////////////////////////////////////////////
/**
  This Function will enable all the Pcie errors, that need to be reported as LER.

  @param[in]  Skt       The socket number of the root port.
  @param[in]  Bus       The PCI bus number of the root port.
  @param[in]  Device    The PCI device number of the root port.
  @param[in]  Function  The PCI function number of the root port.

  @retval    Status.
**/
VOID
EFIAPI
IioLerEnable (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This Function will DisableLerMAerrorLogging.

  @param[in] Ioh        The IOH number.

  @retval    Status.
**/
RETURN_STATUS
EFIAPI
IioLerDisMcaErrLog (
  IN      UINT8   SocketId
  );


/**
  This function will enable Iio PCIE Root Port error reporting.

  @param[in] RootBridgeLoop        The socket number.

  @retval    Status.
**/
VOID
EFIAPI
IioRootPortErrorEnable (
  IN      UINT8   Skt,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function is to handle IIO Root Port device XP Error

  @param[in]  Socket      Socket number.
  @param[in]  Bus         PCI bus number.
  @param[in]  Device      PCI device number.
  @param[in]  Function    PCI function number.
  @param[in]  ErrSeverity IIO Internal Error Severity.
**/
VOID
EFIAPI
IioXpErrHandler (
  IN    UINT8                        Socket,
  IN    UINT8                        Bus,
  IN    UINT8                        Device,
  IN    UINT8                        Function,
  IN    UINT8                        ErrSeverity
  );

/**
  IIO PCIe leaky bucket handler.

  @param[in]  Socket    Socket number.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
  @param[in]  Severity  Error severity.
**/
VOID
EFIAPI
IioPcieLeakyBucketHandler (
  IN    UINT8   Socket,
  IN    UINT8   Bus,
  IN    UINT8   Device,
  IN    UINT8   Function,
  IN    UINT32  Severity
  );

/**
  This function is to handle IIO Internal Device (VTD, CBDMA, IRP, IRPRING, ITC, OTC, XP, MISC ) Error

  @param[in]  Socket      Socket number.
  @param[in]  IioStack    Stack number.
  @param[in]  Bus         PCI bus number.
  @param[in]  Device      PCI device number.
  @param[in]  Function    PCI function number.
  @param[in]  ErrSeverity IIO Internal Error Severity.
  @param[in]  ErrType     IIO Internal Error Type.
**/
VOID
EFIAPI
IioIntDevErrHandler (
  IN    UINT8                        Socket,
  IN    UINT8                        IioStack,
  IN    UINT8                        Bus,
  IN    UINT8                        Device,
  IN    UINT8                        Function,
  IN    UINT8                        ErrSeverity,
  IN    RAS_IIO_INTERNAL_ERROR_TYPE  ErrType
  );

/**
  enable RAS pci device, each iio stack have one RAS device.

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none
**/
VOID
EFIAPI
EnableRasDevice (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  );

 /**

   Enable VTD


  @param  [in]  Socket  -- socket index
  @param  [in]  Instance -- instance of satallite IEH per socket.

  @retval None
 **/
VOID
EFIAPI
IioEnableVtd (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  );

 /**
  handle PCH PCI & root port devices errors.

  @param[in] Bus            Device's bus number.


  @retval    None
--*/
VOID
EFIAPI
PchErrorHandler (
  IN     UINT8           SevPerStack
  );

/**
  This routine purpose is to pass policy data to mailbox

  @param  PolicyData - pointer to policy data
  @param  MailBox - pointer to mailbox
  @retval none
**/
VOID
EFIAPI
GetPcieInitPars (
  RAS_POLICY       *PolicyData,
  RAS_MAIL_BOX     *MailBox
  );

/**
  This routine purpose is to pass policy data to mailbox

  @param  PolicyData - pointer to policy data
  @param  MailBox - pointer to mailbox
  @retval none
**/
VOID
EFIAPI
GetIioInitPars(
  RAS_POLICY       *PolicyData,
  RAS_MAIL_BOX     *MailBox
  );

#endif
