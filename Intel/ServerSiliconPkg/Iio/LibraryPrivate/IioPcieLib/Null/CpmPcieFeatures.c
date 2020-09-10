/** @file
  PCIE features implementation for CPM 1.8

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <Base.h>
#include <IioPlatformData.h>

/**
This function provides PCI Workaround for Intel IPs just before EDK enumeration.

@param[in] Segment            - Segment
@param[in] Bus                - Bus of the virtual root port
@param[in] Dev                - Device of the virtual root port
@param[in] Function           - Function of the virtual root port
@param[in] DidVid             - Device-Vendor ID in the given Bus, Device, Function.

**/
VOID
IioPciIpHookBeforeEnumeration (
  IN  UINT8          Segment,
  IN  UINT8          Bus,
  IN  UINT8          Device,
  IN  UINT8          Function,
  IN  UINT32         DidVid
)
{
  //
  // Empty definition
  //
}

/**
  IOSF parity detection support for CPM/CPK Virtual Root Port (VrP) and End Point.
  
  @param[in] IioIndex     - IIO index (socket number)
**/
VOID
IioParityEnableCpm (
  IN UINT8        IioIndex
  )
{
  //
  // Empty definition
  //
}
