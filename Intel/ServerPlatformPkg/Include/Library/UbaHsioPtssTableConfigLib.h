/** @file
  UBA Hsio Ptss Table Config Library Header File.

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

#ifndef _UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H
#define _UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/PchPolicy.h>
#include <PchSetupVariable.h>

#define PLATFORM_HSIO_PTSS_TABLE_SIGNATURE  SIGNATURE_32 ('P', 'H', 'P', 'T')
#define PLATFORM_HSIO_PTSS_TABLE_VERSION    01

// {47EA4CA7-F89A-42E6-89F0-20F4B72BA616}
#define   PLATFORM_HSIO_PTSS_TABLE_GUID \
{ 0x47ea4ca7, 0xf89a, 0x42e6, { 0x89, 0xf0, 0x20, 0xf4, 0xb7, 0x2b, 0xa6, 0x16 } }

typedef
VOID
(*HSIO_PTSS_TABLE_CONFIG_CALLBACK) (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy

);

typedef struct {
  UINT32                              Signature;
  UINT32                              Version;

  HSIO_PTSS_TABLE_CONFIG_CALLBACK     CallUpdate;

} PLATFORM_HSIO_PTSS_CONFIG_TABLE;

EFI_STATUS
InstallPlatformHsioPtssTable (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy
);

STATIC  EFI_GUID gPlatformHsioPtssTableGuid = PLATFORM_HSIO_PTSS_TABLE_GUID;

#endif //_UBA_HSIO_PTSS_TABLE_CONFIG_LIB_H

