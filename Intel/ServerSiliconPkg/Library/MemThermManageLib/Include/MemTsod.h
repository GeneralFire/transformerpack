/** @file
  Interface header for Memory TSOD feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_TSOD_H_
#define _MEM_TSOD_H

/**

  This function Initializes registers for Memory TSOD feature

  @param[in]  Socket               Socket Number
  @param[in]  Channel         Channel Number
  @param[in]  Dimm            Dimm Number

  @retval N/A

**/
VOID
InitializeTsod (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

/**

  Check if dimm is TSOD complaint, if not disable Closed Loop Thermal Throttling

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
CheckForTsodDimm (
  IN UINT8 Socket
  );

#endif // ifndef _MEM_TSOD_H_
