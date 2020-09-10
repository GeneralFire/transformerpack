/** @file
  UBA ClockGen Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2015 Intel Corporation. <BR>

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

#ifndef _UBA_CLOCKGEN_UPDATE_LIB_H
#define _UBA_CLOCKGEN_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_CLOCKGEN_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'C', 'L', 'K')
#define PLATFORM_CLOCKGEN_UPDATE_VERSION    0x01

// {CF3845B1-7EB0-44ef-9D67-A80ECE6FED73}
#define   PLATFORM_CLOCKGEN_CONFIG_DATA_GUID \
{ 0xcf3845b1, 0x7eb0, 0x44ef, { 0x9d, 0x67, 0xa8, 0xe, 0xce, 0x6f, 0xed, 0x73 } }

#define PLATFORM_NUMBER_OF_CLOCKGEN_DATA    20
#define PLATFORM_CLOCKGEN_NO_ID             0xFF

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  UINTN                   IdOffset;     // Clockgen ID register offset
  UINT8                   Id;           // Clockgen ID
  UINTN                   DataLength;   // Number of clockgen data for write

  UINTN                   SpreadSpectrumByteOffset;
  UINT8                   SpreadSpectrumValue;

  UINT8                   Data[PLATFORM_NUMBER_OF_CLOCKGEN_DATA];

} PLATFORM_CLOCKGEN_UPDATE_TABLE;


EFI_STATUS
PlatformUpdateClockgen (
  IN  BOOLEAN     EnableSpreadSpectrum
);

STATIC  EFI_GUID gPlatformClockgenConfigDataGuid = PLATFORM_CLOCKGEN_CONFIG_DATA_GUID;

#endif //_UBA_CLOCKGEN_UPDATE_LIB_H
