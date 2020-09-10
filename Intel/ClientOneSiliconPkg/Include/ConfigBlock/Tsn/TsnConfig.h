/** @file
  TSN Config policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

@par Specification Reference:
**/

#ifndef _TSN_CONFIG_H_
#define _TSN_CONFIG_H_

#define TSN_CONFIG_REVISION 2
extern EFI_GUID gTsnConfigGuid;

#pragma pack (push,1)
/**
  The TSN_CONFIG block describes policies related to Time Sensitive Networking(TSN)

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added MultiVcEnable
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal TSN, 0: Disable; <b>1: Enable</b>.
  **/
  UINT8   Enable;
  UINT8   TsnLinkSpeed;           ///< Refer to "TSN_LINK_SPEED_OPTIONS". Default is <b> TSN_LINK_SPEED_2_5G_34MHZ</b>
  UINT8   MultiVcEnable;          ///< Enable Multi-Vc via Tc to DMA mapping. Deafult is </b> 0:disable</b>
  UINT8   Rsvd0;                  ///< Reserved bytes, align to multiple 4.
  UINT32  TsnMacAddressHigh;      ///< MAC Address High bits. Only two lower bytes are used.
  UINT32  TsnMacAddressLow;       ///< MAC Address Low bits
} TSN_CONFIG;

#pragma pack (pop)

typedef enum {
  TSN_LINK_SPEED_2_5G_24MHZ = 0,
  TSN_LINK_SPEED_1G_24MHZ,
  TSN_LINK_SPEED_2_5G_34MHZ,
  TSN_LINK_SPEED_1G_34MHZ
} TSN_LINK_SPEED_OPTIONS;

#endif
