/** @file
  Espi policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2019 Intel Corporation.

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
#ifndef _ESPI_CONFIG_H_
#define _ESPI_CONFIG_H_

#define ESPI_CONFIG_REVISION 2
extern EFI_GUID gEspiConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to ESPI.

  <b>Revision 1</b>:
  - Initial revision
  <b>Revision 2</b>:
  - Added LockLinkConfiguration field to config block
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    LPC (eSPI) Memory Range Decode Enable. When TRUE, then the range
    specified in PCLGMR[31:16] is enabled for decoding to LPC (eSPI).
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT32    LgmrEnable            :  1;
  /**
    eSPI Master and Slave BME settings.
    When TRUE, then the BME bit enabled in eSPI Master and Slave.
    0: FALSE, <b>1: TRUE </b>
  **/
  UINT32    BmeMasterSlaveEnabled :  1;
  /**
    Master HOST_C10 (Virtual Wire) to Slave Enable (VWHC10OE)
    <b>0b: Disable HOST_C10 reporting (HOST_C10 indication from PMC is ignored)</b>
    1b: Enable HOST_C10 reporting to Slave via eSPI Virtual Wire (upon receiving a HOST_C10 indication from PMC)
  **/
  UINT32    HostC10ReportEnable   :  1;
  /**
    eSPI Link Configuration Lock (SBLCL)
    If set to TRUE then communication through SET_CONFIG/GET_CONFIG
    to eSPI slaves addresses from range 0x0 - 0x7FF
    <b>1: TRUE</b>, 0: FALSE
  **/
  UINT32    LockLinkConfiguration :  1;
  /**
   Hardware Autonomous Enable (HAE)
   If set to TRUE, then the IP may request a PG whenever it is idle
  **/
  UINT32    EspiPmHAE             :  1;
  UINT32    RsvdBits              : 27;     ///< Reserved bits
} PCH_ESPI_CONFIG;

#pragma pack (pop)

#endif // _ESPI_CONFIG_H_

