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

#ifndef _MS2IOSF_CREDITS_COMMON_H_
#define _MS2IOSF_CREDITS_COMMON_H_

#include <IioRegs.h>

//
// Channel definitions
//
#define CH0_CRDT                0
#define CH1_CRDT                1
#define CH2_CRDT                2
#define CH3_CRDT                3
#define CH4_CRDT                4
#define CH5_CRDT                5
#define CH6_CRDT                6
#define CH7_CRDT                7
#define CH8_CRDT                8
#define CH9_CRDT                9
#define CH10_CRDT               10
#define CH11_CRDT               11

#define TCPRHPEND_OFFSET_MASK    0xFF0
#define TCPRHPEND_OFFSET         0xDA0

//
// MS2IOSF Signature
//
#define MS2ISOF_CREDITS_SIGNATURE  0x5F42445F

typedef struct {
  UINT32           RegisterAddress;       // 32 bit MMCFG encoded address
  UINT8            RegisterSize;
  UINT32           FieldMask;               // bit mask
  UINT32           Value;
} CREDITS_ENTRY;

#define REV_STR_CHARS                    8
#define HELP_STR_CHARS                   64

typedef struct {
  UINT32  Signature;                         // "_BD_"
  CHAR8   Revision[REV_STR_CHARS];           // 1
  UINT16  NumberEntries;                     //
  UINT8   DebugMode;                         // 0 or 1
  CHAR8   HelperString[HELP_STR_CHARS];      // fill in credits, table rev and build date
} CREDITS_HEADER;

#define CREDIT_TYPE_ITC    0
#define CREDIT_TYPE_OTC    1

typedef struct {
  UINT8             Bifurcation;
  CREDITS_ENTRY     *ItcPcieTable;
  CREDITS_ENTRY     *OtcPcieTable;
} PCIE_CREDIT_TABLE;

#endif // _MS2IOSF_CREDITS_COMMON_H_
