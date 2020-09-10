/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_PCIE_INIT_H_
#define _IIO_PCIE_INIT_H_

#include <Base.h>
#include <IioPlatformData.h>

VOID
ShowRxStats (
  IN  IIO_GLOBALS     *IioGlobalData
  );

VOID
PciePhyPreLinkOverride (
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT8 Socket
  );


/**
   Per port Early initialization before link training is done.

  @param[in] IioGlobalData  Pointer to IIO global data/methods
  @param[in] IioIndex       Current socket.
  @param[in] PortIndex      Port number being initialized.
**/
VOID
PciePortEarlyInit (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8            IioIndex,
  IN  UINT8            PortIndex
  );

VOID
PciePortInit (
  IN  IIO_INIT_PHASE   Phase,
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8            IioIndex,
  IN  UINT8            Port
  );

VOID
PciePortPostInit (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8            IioIndex
  );

VOID
DfxPcieInit (
  IN  IIO_GLOBALS              *IioGlobalData,
  IN  UINT8                    IioIndex
  );

VOID
IioPcieWorkaroundsPreLinkPerSocket (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  );

VOID
IioPcieWorkaroundsPostLinkPerSocket (
  IN  IIO_GLOBALS          *IioGlobalData,
  IN  UINT8                IioIndex
  );

VOID
PciePhyTestMode (
  IN  IIO_GLOBALS              *IioGlobalData
  );

VOID
IioUpdatePcieConfigurationData (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8       IioIndex
  );

VOID
PcieProgramBifurcationPerSocket (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex
  );

VOID
PcieLinkTrainingInitPerSocket (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex
  );

VOID
ProgramIioClockConfiguration (
  IN  IIO_GLOBALS                           *IioGlobalData,
  IN  UINT8                                 IioIndex
  );

VOID
IioDisableLinkPorts (
  IN  IIO_GLOBALS            *IioGlobalData,
  IN  UINT8                  IioIndex
  );

/**
  This function will hide rootports based on PEXPHIDE Settings

  @param[in]     IioGlobalData  Pointer to IIO_GLOBALS
  @param[in]     IioIndex       Socket/IIO complex index
  @param[in out] IioDevFuncHide Pointer to table with devices to hide
**/
VOID
IioRootportsHide (
  IN      IIO_GLOBALS                   *IioGlobalData,
  IN      UINT8                         IioIndex,
  IN OUT  IIO_DEVFUNHIDE_TABLE          *IioDevFuncHide
  );

VOID
PcieErrInjConInit (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

EFI_STATUS
GetPcieInfoDefaultsTable (
  OUT  INTEL_DMI_PCIE_INFO    **PcieInfoDefaultsTable
  );

VOID
WaitTrainingFinishedEx (
  IN  IIO_GLOBALS                     *IioGlobalData
  );

#endif //_IIO_PCIE_INIT_H_
