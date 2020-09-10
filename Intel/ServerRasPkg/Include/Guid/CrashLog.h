/** @file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef __CRASHLOG_H__
#define __CRASHLOG_H__

#define CRASHLOG_SIZE_DEBUG_PURPOSE                   0x030  // It will help to verify the extracted binary from BERT by matching the binary content

#define  MAX_CPU_CRASHLOG_REGIONS                     3
#define  MAX_PCH_CRASHLOG_REGIONS                     6

#define CRASHLOG_DISCOVERY_MECHANISM_LEG 0            //Legacy
#define CRASHLOG_DISCOVERY_MECHANISM_DES 1            //Descriptor Table
#define CRASHLOG_DISCOVERY_MECHANISM_DVS 2            //DVSec

#define CRASHLOG_HEADER_TYPE_LEGACY  0
#define CRASHLOG_HEADER_TYPE_1       1
#define CRASHLOG_HEADER_TYPE_2       2
#define CRASHLOG_HEADER_TYPE_3       3
#define CRASHLOG_HEADER_TYPE_4       4
#define CRASHLOG_HEADER_TYPE_RSV    0xf
//
// Structure of CrashLog Record Header
//
typedef struct {
  union {
    struct {
      UINT32  Revision             : 8;
      UINT32  HeadType             : 4;
      UINT32  ProductId            : 12;
      UINT32  RecordType           : 6;
      UINT32  Reserved             : 1;
      UINT32  Consumed             : 1;
    } Fields;
    UINT32 Data;
  } RecordVersion;
  UINT32  RecordSizeInDword;
} CRASHLOG_RECORD_HEADER_COMMON;


//
// Structure of CrashLog Record Region Information
//
typedef union {
  struct {
    UINT32  Address;
    UINT32  SizeInDword;
  } Fields;
  UINT32 Data[2];
} CRASHLOG_RECORD_REGION_INFO;

//
// Structure of Cpu CrashLog Struct
//
typedef struct {
  CRASHLOG_RECORD_REGION_INFO  RegionInfo[MAX_SOCKET][MAX_CPU_CRASHLOG_REGIONS];
} CPU_CRASHLOG_RECORD_REGIONS_STRUCT;

//
// Structure of Pch CrashLog Struct
//
typedef struct {
  CRASHLOG_RECORD_REGION_INFO  RegionInfo[MAX_PCH_CRASHLOG_REGIONS];
} PCH_CRASHLOG_RECORD_REGIONS_STRUCT;

//
// Structure of CrashLog memory buffer
//
typedef struct {
  UINT64  Address;
  UINT32  Size;
} CRASHLOG_RECORD_REGION_BUFF;


//
// Structure of CPU CrashLog Record Region Protocol
//
typedef struct {
  CRASHLOG_RECORD_REGION_BUFF  RegionBuffer[MAX_SOCKET][MAX_CPU_CRASHLOG_REGIONS];
} CPU_CRASHLOG_RECORD_REGION_PROTOCOL;

//
// Structure of PCH CrashLog Record Region Protocol
//
typedef struct {
  CRASHLOG_RECORD_REGION_BUFF  RegionBuffer[MAX_PCH_CRASHLOG_REGIONS];
} PCH_CRASHLOG_RECORD_REGION_PROTOCOL;
#endif
