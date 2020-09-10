/** @file
  GUID used for Cpu Nvram Data entries in the HOB list.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_NVRAM_DATA_GUID_H_
#define _CPU_NVRAM_DATA_GUID_H_

#include <CpuDataStruct.h>

#define EFI_CPU_NVRAM_DATA_GUID \
  { \
  0x184220a2, 0xe14c, 0x4497, { 0x85, 0xbb, 0x14, 0x90, 0xa9, 0xa1, 0xf0, 0xd3 }\
  }

#define EFI_CPU_NVRAM_DATA_VARIABLE_NAME L"CpuNvramData"

//
//  CPU_NVRAM
//  Data that need to be saved in NVRAM for S3 resume
//
typedef struct {                          // data that need to be saved in NVRAM for S3 resume
  UINT32 flexRatioCsr;                    //  Common for all sockets
  UINT64 DesiredCoresCsr[MAX_SOCKET];    //  One per socket 64bits
  UINT32 DesiredCoresCfg2Csr[MAX_SOCKET]; //  One per socket
} CPU_NVRAM;

extern EFI_GUID gEfiCpuNvramDataGuid;

#endif
