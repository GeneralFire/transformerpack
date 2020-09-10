/** @file
  UBA Clocks Config Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef _UBA_CLOCKS_CONFIG_LIB_H
#define _UBA_CLOCKS_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <PiPei.h>
#include <Uefi/UefiSpec.h>

#define PLATFORM_CLOCKS_CONFIG_SIGNATURE  SIGNATURE_32 ('P', 'C', 'L', 'K')
#define PLATFORM_CLOCKS_CONFIG_VERSION    01

// {34F1B964-49C7-4CB6-B9BD-7043B37C90BE}
#define   PLATFORM_CLOCKS_CONFIG_DATA_GUID \
{ 0x34f1b964, 0x49c7, 0x4cb6, { 0xb9, 0xbd, 0x70, 0x43, 0xb3, 0x7c, 0x90, 0xbe } }

typedef
EFI_STATUS
(*CLOCKS_CONFIG_CALLBACK) (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
);

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  CLOCKS_CONFIG_CALLBACK     CallUpdate;

} PLATFORM_CLOCKS_CONFIG_TABLE;

EFI_STATUS
ConfigurePlatformClock (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *SmbusPpi
);

STATIC  EFI_GUID gPlatformClocksConfigDataGuid = PLATFORM_CLOCKS_CONFIG_DATA_GUID;

#endif //_UBA_CLOCKS_CONFIG_LIB_H

