/** @file
  UBA PCH Softstrap Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#ifndef _UBA_SOFT_STRAP_UPDATE_LIB_H
#define _UBA_SOFT_STRAP_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Protocol/Spi.h>

#define PLATFORM_SOFT_STRAP_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'S', 'T', 'P')
#define PLATFORM_SOFT_STRAP_UPDATE_VERSION    02

// {F06383FE-54BD-4ae1-9C99-1DC83B6A7277}
#define   PLATFORM_PCH_SOFTSTRAP_CONFIG_DATA_GUID \
{ 0xf06383fe, 0x54bd, 0x4ae1, { 0x9c, 0x99, 0x1d, 0xc8, 0x3b, 0x6a, 0x72, 0x77 } }

STATIC  EFI_GUID gPlatformPchSoftStrapConfigDataGuid = PLATFORM_PCH_SOFTSTRAP_CONFIG_DATA_GUID;

typedef struct _PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY {
  UINT8  SoftStrapNumber;
  UINT8  BitfieldOffset;
  UINT8  BitfieldLength;
  UINT32 Value;
} PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY;

typedef
VOID
(*PLATFORM_SPECIFIC_PCH_SOFTSTRAP_UPDATE) (
  IN UINT8            *FlashDescriptorCopy
  );

typedef struct {
  UINT32                                  Signature;
  UINT32                                  Version;
  PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY      *PchSoftStrapTablePtr;
  PLATFORM_SPECIFIC_PCH_SOFTSTRAP_UPDATE  PchSoftStrapPlatformSpecificUpdate;
} PLATFORM_PCH_SOFTSTRAP_UPDATE;


EFI_STATUS
GetPchSoftSoftStrapTable (
  IN  VOID                      **PchSoftStrapTable
  );

VOID
PlatformSpecificPchSoftStrapUpdate (
  IN OUT  UINT8                 *FlashDescriptorCopy
  );

#endif //_UBA_SOFT_STRAP_UPDATE_LIB_H
