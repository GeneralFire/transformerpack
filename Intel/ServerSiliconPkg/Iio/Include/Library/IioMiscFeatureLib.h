/** @file
  IIO Misc Feature initialization header file.

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

#ifndef _IIO_MISC_FEATURE_LIB_H_
#define _IIO_MISC_FEATURE_LIB_H_

/**

    Iio Misc Featues Init before link training.

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None

**/
VOID
IioMiscFeatureEarlyInit (
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8                                          IioIndex
  );

/**

    Enable Misc Featues by writing to IIOMISCCTRL

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None

**/
VOID
IioMiscFeatureInit (
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8                                          IioIndex
  );

/**

    Peer 2 peer memory config

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Current socket.
    @param PortIndex     - Port number being initialized.

    @retval None

**/
VOID
Peer2peerMemoryConfig (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  );

/**

  MS2IOSF channel MISCCTRLSTS and and PERFCTRLSTS register initial.

  @param Phase         - Initialization phase (pre, init, post).
  @param IioGlobalData - Pointer to IIO global data/methods
  @param IioIndex      - Current socket.
  @param StackIndex    - Stack number being initialized.

**/
VOID
Ms2iosfChannelMiscInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          StackIndex
  );

/**
  IIO HCx Init.
  @param IioGlobalData  - IioGlobalData Pointer
  @param IioIndex       - Index to Socket

  @retval None
**/
VOID
EFIAPI
IioHcxInit (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  );

/**
  Disable DSA PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioDsaPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

/**
  Disable IAX PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioIaxPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

/**
  Disable MSM PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioMsmPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

#endif /* _IIO_MISC_FEATURE_LIB_H_*/
