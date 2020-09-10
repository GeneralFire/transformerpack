/** @file
  Contains definitions relevant to the WHEA Error Log Listener.

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

#ifndef _WHEA_ERROR_LOG_LISTENER_H_
#define _WHEA_ERROR_LOG_LISTENER_H_

#include <Protocol/GlobalNvsArea.h>

//
//PCIE section size is around 250 bytes. And this section is much larger than memory section type.
//so 4096 byte pre-allocation range is enough for 8 sections.
//Windows support multiple section, not support multiple records.
//BIOS support one section per record, support multiple records.
//
#define  MAX_RECORD      8
#define  MAX_SECTION     8


/**
  The WHEA Error Logger.

  This function is responsible for logging errors via the Elog interface.

  @param[in]  CodeType      Indicates the type of status code being reported.
  @param[in[  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param[in]  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param[in]  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to serial I/O successfully.

**/
EFI_STATUS
EFIAPI
LogErrorToWhea (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  );

#endif //  _WHEA_ERROR_LOG_LISTENER_H_
