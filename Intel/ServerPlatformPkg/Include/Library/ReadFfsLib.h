/** @file
  Read FFS Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2018 Intel Corporation. <BR>

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

#ifndef __READ_FFS__
#define __READ_FFS__
#include <Base.h>
#include <Pi/PiFirmwareVolume.h>

#define MAX_COMPRESS_ITEM 196
#define MAX_FFS_BUFFER_SIZE  8*1024
#define COMPRESS_DUPLICATE 1
#define COMPRESS_SINGLE    2

#pragma pack(1)
typedef struct {
  UINT16                   Value;
  UINT16                   Length;
  UINT8                    Type;
  UINT16                   Offset;
} COMPRESS_ITEM;

typedef struct {
  UINT32                    Signature;
  UINT32                    Count;
  //COMPRESS_ITEM             item[Count];
} COMPRESS_HOBO_DATA;
#pragma pack()


BOOLEAN NormalHobToCompressHob(IN OUT VOID* hobAddr,IN OUT UINTN* size);
BOOLEAN CompressHobToNormalHob(IN OUT VOID* hobAddr,OUT UINTN* size);

//read a FFS from FV.
UINT8*
PreMemReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN BOOLEAN skipheader
  );

EFI_STATUS
ReadFFSFile (
  IN EFI_FIRMWARE_VOLUME_HEADER* FwVolHeader,
  IN EFI_GUID FFSGuid,
  IN UINT32   FFSDataSize,
  IN OUT VOID *FFSData,
  OUT UINT32  *FFSSize,
  IN BOOLEAN skipheader
  );
EFI_STATUS ValidateCommonFvHeader (  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader  );
#endif
