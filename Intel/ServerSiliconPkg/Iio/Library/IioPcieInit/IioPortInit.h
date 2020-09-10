/** @file
  This file defines everything that's needed for IIO VData struct.

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

#ifndef _IIO_PORTINIT_H_
#define _IIO_PORTINIT_H_


//
// Functions below are here temporary
// - this header will be removed on next step of IioPcieInit lib refactoring
//

/**
  PCIe Link initialization.

  @param[in] IioGlobalData  Pointer to IIO globals.
  @param[in] IioIndex       Socket number of the port being initialized.
  @param[in] PortIndex      Port number being initialized.
**/
VOID
PcieLinkInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  );


/**
    Configure SLOT power control register.

    @param IioIndex      - Socket number of the port being configured.
    @param PortIndex     - Port number being configured.
**/
VOID
ConfigureSlotPowerControl (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex
  );

#endif /* _IIO_PORTINIT_H_*/
