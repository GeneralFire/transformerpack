/** @file
  GUID and structure used for debug status code policy.
 
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>
  
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

#ifndef _STATUS_CODE_DATA_TYPE_EX_DEBUG_GUID_H_
#define _STATUS_CODE_DATA_TYPE_EX_DEBUG_GUID_H_

#include <Library/SemaphoreLib.h>

#define STATUS_CODE_DATA_TYPE_EX_DEBUG_GUID \
  { 0x7859daa2, 0x926e, 0x4b01,{0x85, 0x86, 0xc6, 0x2d, 0x45, 0x64, 0x21, 0xd2} }

#define MAX_EX_DEBUG_SIZE     0x200 // Inherited from PEI status code max
#define MAX_EX_DEBUG_STR_LEN  (MAX_EX_DEBUG_SIZE - sizeof(EX_DEBUG_INFO))

typedef
CHAR8 *
(EFIAPI *PROCESS_BUFFER) (
  IN     VOID   *ProcessDataPtr,
  IN     CHAR8  *Buffer,
  IN OUT UINTN  *BufferSize
  );

typedef
VOID *
(EFIAPI *PRINT_SYNC_ACQUIRE) (
  VOID
  );

typedef
VOID *
(EFIAPI *PRINT_SYNC_RELEASE) (
  VOID
  );

typedef struct {
  PROCESS_BUFFER                ProcessBuffer; // Buffer processing function
  VOID                          *ProcessDataPtr; // Data needed for processing
  PRINT_SYNC_ACQUIRE            PrintSyncAcquire; // Acquire sync function
  PRINT_SYNC_RELEASE            PrintSyncRelease; // Release sync function
  UINT32                        DebugStringLen;
  CHAR8                         *DebugString; // Provided debug string
} EX_DEBUG_INFO;

extern EFI_GUID gStatusCodeDataTypeExDebugGuid;

#endif // _STATUS_CODE_DATA_TYPE_EX_DEBUG_GUID_H_
