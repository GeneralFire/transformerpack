/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef __RUN_CPGC_POINT_TEST_RESULT_METADATA_H__
#define __RUN_CPGC_POINT_TEST_RESULT_METADATA_H__

#include "ssabios.h"

#define RUN_CPGC_POINT_TEST_RESULT_METADATA_GUID \
{0x5158A303,0x0A50,0x4991,{0xB7,0x9D,0x75,0x73,0x67,0xA6,0xF3,0x37} }

#pragma pack (push, 1)

typedef struct RUN_CPGC_POINT_TEST_RESULT_METADATA{
  UINT32 ErrCntOverflowVal;
  UINT8 MajorVer;
  UINT8 MinorVer;
  UINT8 RevVer;
  UINT32 BuildVer;
  UINT8 StatusCode;
  UINT16 LineNumber;
  CHAR8 FileName[13];
}RUN_CPGC_POINT_TEST_RESULT_METADATA;


#pragma pack (pop)

#endif // __RUN_CPGC_POINT_TEST_RESULT_METADATA_H__
