/** @file
  CrystalRidge Measurement definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _CRYSTAL_RIDEG_MEASUREMENT_H_
#define _CRYSTAL_RIDEG_MEASUREMENT_H_

#define EV_DCPMM_EVENT_DATA_SIGNATURE "Intel DCPMM Sec"
#define EV_DCPMM_EVENT_DATA_VERSION 0

#define EV_DEVICE_TYPE_OEM 0x80000000

#define DCPMM_VENDOR_GUID { \
  0x429bdc67, 0x5159, 0x4916, { 0x93, 0x2a, 0x2e, 0xde, 0x73, 0x77, 0x1e, 0xdc } \
  }

///
/// EFI_TCG_DCPMM_EVENT_DATA_CONTEXT
///
typedef struct _EFI_TCG_DCPMM_EVENT_DATA_CONTEXT {
  UINT8  Socket;
  UINT8  Mc;
  UINT8  Ch;
  UINT8  Dimm;
} EFI_TCG_DCPMM_EVENT_DATA_CONTEXT;

//
// EFI_TCG_DCPMM_DEVICE_PATH
//
typedef struct _EFI_TCG_DCPMM_DEVICE_PATH {
  UINT8                            Type;
  UINT8                            SubType;
  UINT16                           Length;
  EFI_GUID                         Guid;
} EFI_TCG_DCPMM_DEVICE_PATH;

//
// EFI_TCG_DCPMM_EVENT_DATA_HEADER
//
typedef struct _EFI_TCG_DCPMM_EVENT_DATA_HEADER{
  UINT8                     Signature[16];
  UINT16                    Version;
  UINT16                    Length;
  UINT32                    SpdmHashAlgo;
  UINT32                    DeviceType;
  UINT32                    DevicePathSize;
  EFI_TCG_DCPMM_DEVICE_PATH DevicePath;
  struct dimmInformation    IdentifyData;
} EFI_TCG_DCPMM_EVENT_DATA_HEADER;

//
// EFI_TCG_DCPMM_EVENT_DATA
//
typedef struct _EFI_TCG_DCPMM_EVENT_DATA {
  EFI_TCG_DCPMM_EVENT_DATA_HEADER  EventData;
  UINT32                           Digest[NGN_FFW_BANK_NUM][NGN_FFW_CSR_NUM];
  EFI_TCG_DCPMM_EVENT_DATA_CONTEXT context;
} EFI_TCG_DCPMM_EVENT_DATA;

#endif
