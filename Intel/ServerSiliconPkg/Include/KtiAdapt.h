/** @file
 KTI Adaptation Header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#ifndef _KTI_ADAPTATION_H_
#define _KTI_ADAPTATION_H_

typedef enum {
  ADAPTATION_NOT_REQUIRED,
  ADAPTATION_REQUIRED,
  ADAPTATION_REQUIRED_SPEED_NOT_SUPPORTED,
  ADAPTATION_FEATURE_NOT_SUPPORTED
}ADAPTATION_OPERATION;

#define KTI_TX_ADAPT_ENABLE               1
#define KTI_TX_ADAPT_DISABLE              0

#define KTI_BACK_CHANNEL_IN_USE           1

/**
  Programs txeq adaptation parameter for the link

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

  @retval Status - return KTI_SUCCESS on successful programming
                          KTI_IGNORE - Not supported

**/
KTI_STATUS
KtiProgramTxeqAdaptParams (
  IN  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN  UINT8                      Socket,
  IN  UINT8                      LinkIndex
  );

/**
  Check if there is next adaptation speed need to be adapted, or it is already finished.
  If it is finished, clears the TxAdaptEn bits for links.
  Otherwise, get the next adaptation speed and do the related steps (it is in without warm reset flow).

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Status - return KTI_SUCCESS on successful programming
                          KTI_IGNORE - Not supported

**/
KTI_STATUS
CheckAndExecuteKtiAdaptation (
  IN  KTI_SOCKET_DATA            *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  This routine checks if adaptation needs to be executed or not. It also updates the Adaptation status scratch register
  with the outcome. Executing the Adaptation engine is necessary only if all the populated sockets support
  adaptation and at least one of the following is TRUE:

      - CPU parts on the platform are changed/swapped
      - A valid adaptation table is not found
      - A valid adaptation table is found but the table entry for the requested adaptation speed is not found

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval BOOLEAN - TRUE  - if adaptation needed; FALSE - if not adaptation required

**/
BOOLEAN
CheckIfAdaptationNeeded (
  IN  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );
#endif
