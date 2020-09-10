/** @file
  Definitions for Unique Platform ID messages support

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _UPID_MSGS_H_
#define _UPID_MSGS_H_

#pragma pack(1)

///
/// UPID (Unique Platform Id) Message Header
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 Feature : 8;
    UINT32 Command : 8;
    UINT32 Length  : 16;
  } Fields;
} UPID_MESSAGE_HEADER;

#define UPID_MESSAGE_BYTE_COUNT_MAP(a) (sizeof(a)-sizeof(UPID_MESSAGE_HEADER))

///
/// Typedef for UPID Feature
///
typedef enum {
  UpidFeaturePlatId                  = 0x00
} UPID_FEATURE;

///
/// Typedef for common feature commands
///
typedef enum {
  UpidCommonGetFeatureSupport        = 0x00,
  UpidCommonGetFeatureState          = 0x01,
  UpidCommonSetFeatureState          = 0x02,
  UpidCommonGetOsControlFeatureState = 0x03,
  UpidCommonSetOsControlFeatureState = 0x04
} UPID_COMMON_FEATURE;

///
/// Typedef for Platform Id commands
///
typedef enum {
  PlatIdGetPlatformId                = 0x05,
  PlatIdGetRefurbishCounter          = 0x06,
  PlatIdUpdateOemPlatformId          = 0x07
} PLAT_ID_FEATURE;

///
/// Typedef for UPID Status
///
typedef enum {
  UpidStatusSuccess,
  UpidStatusFeatureNotSupported,
  UpidStatusInvalidInputParameter,
  UpidStatusInternalError,
  UpidStatusNotAllowedAfterEop,
  UpidStatusMaxCountersExceeded,
} UPID_STATUS;

///
/// Common Features Definitions
///

///
/// Get Feature Support
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_FEATURE_SUPPORT_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureSupport;
} GET_FEATURE_SUPPORT_RESPONSE;

typedef union {
  GET_FEATURE_SUPPORT_REQUEST  Request;
  GET_FEATURE_SUPPORT_RESPONSE Response;
} GET_FEATURE_SUPPORT_BUFFER;

///
/// Get/Set Feature State
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureState;
} GET_FEATURE_STATE_RESPONSE;

typedef union {
  GET_FEATURE_STATE_REQUEST  Request;
  GET_FEATURE_STATE_RESPONSE Response;
} GET_FEATURE_STATE_BUFFER;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT8               PlatIdFeatureState;
} SET_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} SET_FEATURE_STATE_RESPONSE;

typedef union {
  SET_FEATURE_STATE_REQUEST  Request;
  SET_FEATURE_STATE_RESPONSE Response;
} SET_FEATURE_STATE_BUFFER;

///
/// Get/Set Os Control Feature State
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_OS_CONTROL_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureOsControlState;
} GET_OS_CONTROL_FEATURE_STATE_RESPONSE;

typedef union {
  GET_OS_CONTROL_FEATURE_STATE_REQUEST  Request;
  GET_OS_CONTROL_FEATURE_STATE_RESPONSE Response;
} GET_OS_CONTROL_FEATURE_STATE_BUFFER;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT8               PlatIdFeatureOsControlState;
} SET_OS_CONTROL_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} SET_OS_CONTROL_FEATURE_STATE_RESPONSE;

typedef union {
  SET_OS_CONTROL_FEATURE_STATE_REQUEST  Request;
  SET_OS_CONTROL_FEATURE_STATE_RESPONSE Response;
} SET_OS_CONTROL_FEATURE_STATE_BUFFER;

///
/// Platform Id Feature Defintions
///

///
/// Get Platform Id
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_PLATFORM_ID_REQUEST;

#define MAX_PLAT_ID_SIZE 32

typedef enum {
  OemPlatIdTypeNotSet,
  OemPlatIdTypeBinary,
  OemPlatIdTypePrintableString
} OEM_PLAT_ID_TYPE;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT32              OemPlatIdType;
  UINT8               OemPlatId[MAX_PLAT_ID_SIZE];
  UINT8               CsmePlatId[MAX_PLAT_ID_SIZE];
} GET_PLATFORM_ID_RESPONSE;

typedef union {
  GET_PLATFORM_ID_REQUEST  Request;
  GET_PLATFORM_ID_RESPONSE Response;
} GET_PLATFORM_ID_BUFFER;

//
/// Get Refurbish Counter
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_REFURBISH_COUNTER_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               RefurbishCounter;
} GET_REFURBISH_COUNTER_RESPONSE;

typedef union {
  GET_REFURBISH_COUNTER_REQUEST  Request;
  GET_REFURBISH_COUNTER_RESPONSE Response;
} GET_REFURBISH_COUNTER_BUFFER;

///
/// Update OEM Platform Id
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              OemPlatIdType;
  UINT8               OemPlatId[MAX_PLAT_ID_SIZE];
  UINT8               ManufacturingType;
} UPDATE_OEM_PLATFORM_ID_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} UPDATE_OEM_PLATFORM_ID_RESPONSE;

typedef union {
  UPDATE_OEM_PLATFORM_ID_REQUEST  Request;
  UPDATE_OEM_PLATFORM_ID_RESPONSE Response;
} UPDATE_OEM_PLATFORM_ID_BUFFER;

#pragma pack()

#endif // _UPID_MSGS_H_

