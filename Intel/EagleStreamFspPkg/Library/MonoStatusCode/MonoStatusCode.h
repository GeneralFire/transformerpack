/** @file MonoStatusCode.h
  Contains Platform specific implementations required to use status codes

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
**/

#ifndef _MONO_STATUS_CODE_H_
#define _MONO_STATUS_CODE_H_

#include "PiPei.h"
#include "Ppi/StatusCode.h"
#include "Ppi/MemoryDiscovered.h"

#include "Library/HobLib.h"
#include "Library/DebugLib.h"
#include "Library/IoLib.h"
#include "Library/SerialPortLib.h"
#include "Protocol/StatusCode.h"


#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>


//
// Status code reporting function
//
EFI_STATUS
SerialReportStatusCode (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN CONST EFI_GUID                 * CallerId,
  IN CONST EFI_STATUS_CODE_DATA     * Data OPTIONAL
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description
  CodeType    - GC_TODO: add argument description
  Value       - GC_TODO: add argument description
  Instance    - GC_TODO: add argument description
  CallerId    - GC_TODO: add argument description
  Data        - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

//
// This is the platform function that calls all of the listeners desired by the
// platform.
//
EFI_STATUS
EFIAPI
PlatformReportStatusCode (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN CONST EFI_GUID                 * CallerId,
  IN CONST EFI_STATUS_CODE_DATA     * Data OPTIONAL
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description
  CodeType    - GC_TODO: add argument description
  Value       - GC_TODO: add argument description
  Instance    - GC_TODO: add argument description
  CallerId    - GC_TODO: add argument description
  Data        - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

//
// Initialize the status code listeners and publish the status code PPI.
//

VOID
EFIAPI
InitializeMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN const EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  FfsHeader   - GC_TODO: add argument description
  PeiServices - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

extern EFI_PEI_PROGRESS_CODE_PPI    mStatusCodePpi;
extern EFI_PEI_PPI_DESCRIPTOR       mPpiListStatusCode;

#endif
