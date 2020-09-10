/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include <SysHost.h>
#include <Library/SysHostPointerLib.h>

/**

  Get Channel Mapping

  @param Channel    - Channel number
  @param SubChannel - Sub Channel number
  @param AltChannel - Alternate Channel number

  @retval None

**/
VOID
GetChannelMapping (
  IN      UINT8    Channel,
  IN      UINT8    SubChannel,
  IN OUT  UINT8*   AltChannel
)
{
  *AltChannel = Channel;
} // GetChannelMapping

/**

  Get DQ DQS Mapping

  @param Channel    - Channel number
  @param SubChannel - Sub Channel number
  @param Strobe     - Strobe number
  @param AltStrobe  - Alternate Strobe number

  @retval None

**/
VOID
GetDqDqsMapping (
  IN      UINT8    Channel,
  IN      UINT8    SubChannel,
  IN      UINT8    Strobe,
  IN OUT  UINT8*   AltStrobe
)
{
  *AltStrobe = Strobe;
} // GetDqDqsMapping

/*

  Override Channel Mode operation

  IF current channel mode is ModeDdr5FullChannel it will not override
  IF current channel mode is ModeDdr5SubChannel it will override to ModeDdr5FullChannelPost40b

*/
VOID
OverrideChannelModeTo80bMode (
 VOID
)
{
}

/*

  Get the channel increment rate, based on the project and mode of operation (40b or 80b)

  @param[in]  Host  Pointer to sysHost structure

  @return     The increment rate to be used in the channel loops

*/
UINT8
GetChInc (
  IN  PSYSHOST  Host
  )
{
  return 1;
}