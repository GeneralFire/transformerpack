/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2015 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>


#define Clear_CUSTOMIZED_SETTING_NOT_FOUND_STRING  "Not found the ITK customized setting."
#define Clear_CUSTOMIZED_SETTING_CLEAN_SUCCESS_STRING  "The ITK customized settings have been cleared successfully."
#define Clear_CUSTOMIZED_SETTING_ABORT_STRING  "Abort action for SMM Protocol not ready."

typedef enum {

  AbortMessage = 0,
  SuccessMessage = 1,
  NotFoundMessage = 2
} Message_FLAG;

typedef enum {
  EnumClearSpiRegionAll,
  EnumClearSpiRegionBios,
  EnumClearSpiRegionMe,
  EnumClearSpiRegionGbE,
  EnumClearSpiRegionDescriptor,
  EnumClearSpiRegionPlatformData,
  EnumClearSpiRegionMax
} SPI_REGION_TYPE_Clear;

VOID
InternalGetCleanMessage (
  IN  Message_FLAG          Flag,
  OUT CHAR8                 *Pointer,
  OUT UINT16                *Status
  );


VOID
 HookforEraseRegion (
  IN  VOID                  *mEfiSmmSpiProtocol,
  OUT CHAR8                 *DataPointer,
  OUT UINT16                *MesStatus
 );



