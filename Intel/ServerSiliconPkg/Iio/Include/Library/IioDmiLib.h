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

#ifndef _IIO_DMI_INIT_H_
#define _IIO_DMI_INIT_H_
#include <Base.h>

/**
  Main DMI initialization

  @param[in]  IioGlobalData   Pointer to the IIO global data
  @param[in]  IioIndex        Socket/IIO complex index
  @param[in]  Phase           Defines the phase of PCI initialization that the PCI driver is executing
 */
VOID
IioDmiInit (
  IN IIO_GLOBALS                        *IioGlobalData,
  IN UINT8                              IioIndex,
  IN IIO_INIT_PHASE                     Phase
  );

/**
  DMI link specific initialization before training.

  @param[in]  IioGlobalData   Pointer to the IIO global data
  @param[in]  IioIndex        Socket/IIO complex index
  @param[in] PortIndex      Index port that will be used
 */
VOID
IioDmiLinkInit (
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          PortIndex
  );

/**
  DMi/RLink post initialization settings.

  @param[in]  IioIndex  Socket/IIO complex index
**/
VOID
IioDmiPostInit (
  IN UINT8  IioIndex
  );

/**
 This function will issue a link re-train process in DMI if required

 This function is not called for RLINK!

 @param[in] IioGlobalData  Pointer to the Internal Data Storage (UDS) for this driver
 @param[in] IioIndex       Socket Index
 @param[in] PortIndex      Index port that will be used
**/
VOID
IioDmiLinkReTrain (
  IN  IIO_GLOBALS                  *IioGlobalData,
  IN  UINT8                        IioIndex,
  IN  UINT8                        PortIndex
  );

/**
  Disables DMI port on non-legacy sockets

  @param[in] IioIndex           Socket Index
**/
VOID
IioDmiDisableDmiOnNonLegacySkt (
  IN  UINT8             IioIndex
  );

/**
  This routine will program DMI TcVc mappings for IIO and PCH side.
  Also it will update LNKCTRL PCH side to support DMI Max Link Speed.

  @param[in] IioGlobalData    - Pointer to IIO_GLOBALS structure
**/
VOID
IioDmiTcVcSetup (
  IN  IIO_GLOBALS         *IioGlobalData
  );

/**
  DMI on ICX-SP needs spec. credits settings (LBG side only)
  This should be moved to PCH code.
**/
VOID
ProgramCustomPchCredits (
  VOID
  );

#endif /* _IIO_DMI_INIT_H_*/
