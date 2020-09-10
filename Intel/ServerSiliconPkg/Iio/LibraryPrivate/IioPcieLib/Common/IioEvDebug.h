/** @file
  Header definition

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

#ifndef _IIOEVDEBUG_H_
#define _IIOEVDEBUG_H_

#include <IioPlatformData.h>

#define STOP_LOGGER          0
#define START_LOGGER         1
#define STOPLOG_MASK         0x3FF

#define G3DFRMERR_MASK       0xFFFFFFFF

#define LOGGER_DEPTH         64

/* Monbus load bus codes */
#define EVMONBUS             0x0
#define EVTRACEBUS           0x1

struct iio_monbusParameter {
  UINT8   busType;        // MonBus or Trace Bus
  UINT8   loadSel;        // Lookup code for monbus or tracebus
  UINT8   lsb;            // MSB for mask
  UINT8   msb;            // LSB for mask
};

extern const struct iio_monbusParameter EvParameterList[];
extern const UINT32                     PortLinkSelect[];

/**
  This routine converts LTSSM state & substate to a textual form.

  @param[in]  State          - Value of the Main LTSSM State.
  @param[in]  SubState       - Value of the Sub LTSSM State.
  @param[out] StateStrPtr    - Pointer to string of the Main LTSSM State.
  @param[out] SubstateStrPtr - Pointer to string of the Sub LTSSM State.
**/
VOID
DecodeLtssm (
  IN  UINT32        State,
  IN  UINT32        SubState,
  OUT CHAR8       **StateStrPtr,
  OUT CHAR8       **SubstateStrPtr
  );

/**
  This routine will dump Arc state of the link.

  @param[in] L0ExitArc - L0 Exit Arc value.

  @return String describing L0ExitArc is returned.
**/
CONST CHAR8*
DecodeLtssmArc (
  IN  UINT32       L0ExitArc
  );

/**
  The purpose is to setup the LTSSM logger.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to the Iio Port
  @param[in] Speed                - Speed
  @param[in] Stop                 - Flag to Stop or Start logger
  @param[in] Mask                 - Mask
**/
VOID
LoggerStartStop (
  IN  IIO_GLOBALS                        *IioGlobalData,
  IN  UINT8                               IioIndex,
  IN  UINT8                               PortIndex,
  IN  UINT8                               Speed,
  IN  UINT8                               Stop,
  IN  UINT8                               Mask
  );

/**
  The purpose is to dump the LTSSM logger results.

  @param[in] IioGlobalData        - Pointer to IioGlobalData
  @param[in] IioIndex             - Index to Iio
  @param[in] PortIndex            - Index to Iio Port
**/
VOID
LogSnapShot (
  IN  IIO_GLOBALS                        *IioGlobalData,
  IN  UINT8                               IioIndex,
  IN  UINT8                               PortIndex
  );

#endif /* _IIOEVDEBUG_H_ */
