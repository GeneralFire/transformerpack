/** @file

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

#ifndef _IIOCOMMON10NMMISCWA_H_
#define _IIOCOMMON10NMMISCWA_H_

#define BAD_TRANSACTION_WORKAROUND_INDEX 3

/**
  Routine contains common cross processor workarounds

  @param IioGlobalData - IIO Global data structure
  @param IioIndex      - IIO index

  @retval None
 */
VOID
IioCommon10nmMiscWAs (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
);

/**
  This routine is to address the post RWO settings of the IIOMISCCTRL global register

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None
*/
VOID
IioCommon10nmPortEnumCtrlWA (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
);

/**
  Routine to check if need clear Egstfwd bits for PSF SB Ports

  @param IioGlobalData - IIO Global data structure
  @param IioIndex      - IIO index

  @retval TRUE - Need clear Egstfwd bits for PSF SB Ports
          FALSE - Skip to clear Egstfwd bits for PSF SB Ports
 */
BOOLEAN
NeedEgstfwdCleared (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
);

#endif  _IIOCOMMON10NMMISCWA_H_
