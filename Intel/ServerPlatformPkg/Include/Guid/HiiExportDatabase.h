/** @file
  Defines GUID for HII Export Database variable

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 Intel Corporation. <BR>

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

#ifndef _HII_EXPORT_DATABASE_H_
#define _HII_EXPORT_DATABASE_H_

//
// {1B838190-4625-4ead-ABC9-CD5E6AF18FE0}
//
#define EFI_HII_EXPORT_DATABASE_GUID { 0x1b838190, 0x4625, 0x4ead, { 0xab, 0xc9, 0xcd, 0x5e, 0x6a, 0xf1, 0x8f, 0xe0 } }

#define EFI_EXPORT_DATABASE_VARIABLE_NAME  L"HiiDB"

extern EFI_GUID gEfiHiiExportDatabaseGuid;

#endif
