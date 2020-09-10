/** @file
  This file defines everything that's needed for IIO VData struct.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef _IIOVMDINIT_H_
#define _IIOVMDINIT_H_

/**
    Volume Management Device (VMD) callback registration.
    Main VMD initialization needs to be done on End-Of-Pei
    to be sure that all rootports are initialized
**/
VOID
IioVmdInit ( VOID );

/**
  Rootports assigned to VMD needs to be visible for common initialization.
  Because of that it is necessary to disable VMD on early boot stage and enable VMD again
  (this is significant for the flow after warm reset).

  @param IioIndex      - index of the socket
**/
VOID
IioDisableVmdForInit (
  IN  UINT8               IioIndex
  );

/**
  Returns TRUE if given PCH rootport can be assigned to VMD

  @param[in] IioIndex       - IIO instance (usually socket).
  @param[in] PchRpIndex     - Index of the PCH rootport.

  @return TRUE if given PCH rootport can be assigned to VMD
 */
BOOLEAN
EFIAPI
IioIsPchPortAllowed (
  IN UINT8  IioIndex,
  IN UINT8  PchRpIndex
  );

#endif /* _IIOVMDINIT_H_*/
