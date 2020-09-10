/** @file
  Register definitions for machine check registers defined in
  IA32 Software Developer's Manual.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _IPMINETFNFIRMWAREDEFINITIONS_H_
#define _IPMINETFNFIRMWAREDEFINITIONS_H_

//
// Net function definition for Firmware command
//
#define EFI_SM_NETFN_FIRMWARE 0x08

//
// All Firmware commands and their structure definitions to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get BMC Execution Context
----------------------------------------------------------------------------------------*/
#define EFI_FIRMWARE_GET_BMC_EXECUTION_CONTEXT  0x23

//
//  Constants and Structure definitions for "Get Device ID" command to follow here
//
typedef struct {
  UINT8   CurrentExecutionContext;
  UINT8   PartitionPointer;
} EFI_IPMI_MSG_GET_BMC_EXEC_RSP;

//
// Current Execution Context responses
//
#define EFI_FIRMWARE_BMC_IN_FULL_RUNTIME        0x10
#define EFI_FIRMWARE_BMC_IN_FORCED_UPDATE_MODE  0x11


#endif

