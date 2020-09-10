/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#ifndef _IIOSYSINFO_TABLE_H_
#define _IIOSYSINFO_TABLE_H_

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#define    MAX_IOU_NUM_PER_SOCKET    8
#define    MAX_STACK_NUM_PER_SOCKET  12

typedef struct {
  UINT8  MaxPortNumPerSocket;
  UINT8  MaxIouNumPerSocket;
  UINT8  MaxPortNumPerIou;
  UINT8  MaxNtbNumPerSocket;
  UINT8  MaxStackNumPerSocket;
  UINT8  ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  UINT8  StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  UINT8  MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];
} IIO_SYS_INFO;

EFI_STATUS
GetIioSysInfoTable (
  IN OUT  IIO_SYS_INFO    **IioSysInfoTable
  );
#endif // _IIOSYSINFO_TABLE_H_

