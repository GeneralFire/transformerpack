/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2014 Intel Corporation. <BR>

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

#ifndef __UBA_SETUP_CONFIG_H__
#define __UBA_SETUP_CONFIG_H__

#define UBA_SETUP_CONFIG_SIGNATURE   SIGNATURE_32('M', 'O','S', 'C')
#define UBA_SETUP_CONFIG_VERSION     01

#define UBA_SETUP_CONFIG_VAR_NAME    L"UbaSetupCfg"


// {6426F33E-0950-470f-9EA7-AE157661973A}
#define UBA_SETUP_CONFIG_GUID \
  { \
    0x6426f33e, 0x950, 0x470f, 0x9e, 0xa7, 0xae, 0x15, 0x76, 0x61, 0x97, 0x3a \
  }


#pragma pack(1)

typedef
struct _UBA_SETUP
{

  UINT32      Signature;
  UINT32      Version;
  UINT32      Size;
  UINT8       Type;
  UINT8       SystemUuid[16];

  //
  // Left for platform use.
  //

  UINT8       RESV[35];     // Default total size is 64 bytes, padding left bytes.

} UBA_SETUP;

#pragma pack()



#ifndef VFRCOMPILE

STATIC EFI_GUID gUbaSetupCfgGuid = UBA_SETUP_CONFIG_GUID;
#endif


#endif // #ifndef __UBA_SETUP_CONFIG_H__
