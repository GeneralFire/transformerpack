/** @file
  Definitions for HECI Trace library

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#ifndef _HECI_TRACE_LIB_H_
#define _HECI_TRACE_LIB_H_

#include <Library/HeciTransportCoreLib.h>

/**
  Print HECI message to the terminal.

  Normally just trace the header, if needed dump whole message to terminal.

  @param[in]     HeciTransport HECI transport pointer the message has to be sent via
  @param[in]     Prefix        Text prefix, whether it is receive or send
  @param[in]     Header        Packet header
  @param[in]     Body          Pointer to the packet body
  @param[in]     BodyLen       The length of packet body
**/
VOID
HeciPktTrace (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN     CHAR8                    *Prefix,
  IN     HECI_PACKET_HEADER       PacketHeader,
  IN     UINT8                    *PacketBody,
  IN     UINT32                   PacketBodyLength
  );

#endif // _HECI_TRACE_LIB_H_
