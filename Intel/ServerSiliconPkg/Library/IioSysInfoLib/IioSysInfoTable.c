/** @file

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

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <IioSysInfoTable.h>
#include <Library/KtiApi.h>

IIO_SYS_INFO  IioSysInfoTableSprSp = {
  41,  // MaxPortNumPerSocket;
  5,   // MaxIouNumPerSocket;
  8,   // MaxPortNumPerIou;
  5,   // MaxNtbNumPerSocket;
  12,  // MaxStackNumPerSocket;
  {1, 9, 17, 25, 33, 0xFF, 0xFF, 0xFF},             // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 2,  3,  4,  5, 0xFF, 0xFF, 0xFF},             // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 8,  8,  8,  8,  8,
   0, 0,  0,  0,  0,  0}                            // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

IIO_SYS_INFO  IioSysInfoTableIcxSp = {
  21,  // MaxPortNumPerSocket;
  5,   // MaxIouNumPerSocket;
  4,   // MaxPortNumPerIou;
  5,   // MaxNtbNumPerSocket;
  6,   // MaxStackNumPerSocket;
  {1, 5, 9,  13, 17, 0xFF, 0xFF, 0xFF},        // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 2,  3,  4,  5,   0xFF, 0xFF, 0xFF},        // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 4,  4,  4,  4,   4,
   0, 0,  0,  0,  0,   0}                        // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];
};

IIO_SYS_INFO  IioSysInfoTableIcxDLcc = {
  5,  // MaxPortNumPerSocket;
  1,  // MaxIouNumPerSocket;
  4,  // MaxPortNumPerIou;
  1,  // MaxNtbNumPerSocket;
  6,  // MaxStackNumPerSocket;
  { 1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },        // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  { 1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },        // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  { 1, 4, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0 }                        // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

IIO_SYS_INFO  IioSysInfoTableIcxDHcc = {
  9,  // MaxPortNumPerSocket;
  2,  // MaxIouNumPerSocket;
  4,  // MaxPortNumPerIou;
  2,  // MaxNtbNumPerSocket;
  6,  // MaxStackNumPerSocket;
  {1, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},        // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},        // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 4, 0,    0,    0,    4,
   0, 0 ,0,    0,    0,    0}                        // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

IIO_SYS_INFO  IioSysInfoTableSnr = {
  5,  // MaxPortNumPerSocket;
  1,  // MaxIouNumPerSocket;
  4,  // MaxPortNumPerIou;
  0,  // MaxNtbNumPerSocket;
  5,  // MaxStackNumPerSocket;
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {   1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {   1,    4,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0}                // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

IIO_SYS_INFO  IioSysInfoTableTnr = {
  1,  // MaxPortNumPerSocket;
  0,  // MaxIouNumPerSocket;
  0,  // MaxPortNumPerIou;
  0,  // MaxNtbNumPerSocket;
  1,  // MaxStackNumPerSocket;
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {   1,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0}                // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

IIO_SYS_INFO  IioSysInfoTableSkx = {
  21,  // MaxPortNumPerSocket;
  5,   // MaxIouNumPerSocket;
  4,   // MaxPortNumPerIou;
  3,   // MaxNtbNumPerSocket;
  6,   // MaxStackNumPerSocket;
  {1, 5, 9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},        // ClusterPortByNtbIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 2,  3,   4,    5, 0xFF, 0xFF, 0xFF},        // StackIndexByIouIndex[MAX_IOU_NUM_PER_SOCKET];
  {1, 4,  4,   4,    4,    4,
   0, 0,  0,   0,    0,    0}                     // MaxPortNumByStackIndex[MAX_STACK_NUM_PER_SOCKET];;
};

/**

  Get IiosysInfo Table

  @param     IioSysInfoTable - IiosysInfo Table pointer of pointer.
  @retval    EFI_STATUS
**/


EFI_STATUS
GetIioSysInfoTable (
  IN OUT  IIO_SYS_INFO    **IioSysInfoTable
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    *IioSysInfoTable = &IioSysInfoTableSprSp;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    *IioSysInfoTable = &IioSysInfoTableIcxSp;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (GetChopType (0) == TypeLcc) {
      *IioSysInfoTable = &IioSysInfoTableIcxDLcc;
    } else {
      *IioSysInfoTable = &IioSysInfoTableIcxDHcc;
    }
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    *IioSysInfoTable = &IioSysInfoTableSnr;
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    *IioSysInfoTable = &IioSysInfoTableTnr;
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    *IioSysInfoTable = &IioSysInfoTableSkx;
  } else {
    *IioSysInfoTable = NULL;
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

