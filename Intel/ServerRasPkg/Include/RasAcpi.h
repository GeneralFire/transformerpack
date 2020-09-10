/** @file
   Headfile for PCIe handshake between OS and firmware.

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

#ifndef _RAS_ACPI_H_
#define _RAS_ACPI_H_

#define MAX_ROOTPORT  400

#define EPDC_INFO_VALID 1
#define EDPC_INFO_NOTIFIED 2
#define EDPC_INFO_CLEARED 3

#define EDPC_NOTIFY_ERROR_INJECT  1
#define EDPC_NOTIFY_BUS_CHECK     2

//
//!!! Attention!!!, please keep in mind these structures should be consistent
// with OperationRegion RASP in RasSsdt.asl . in case you change any of these
// please also change OperationRegion RASP.
//
#pragma pack(push, 1)
typedef struct {
  UINT8         Segment;
  UINT8         Bus;
  UINT8         Device;
  UINT8         Function;
} EDPC_DEV_INFO;

typedef struct {
  //
  // There are three possible statuses of Valid infomation
  // EPDC_INFO_VALID, EDPC_INFO_NOTIFIED, EDPC_INFO_CLEARED
  //
  UINT8           Valid;
  EDPC_DEV_INFO   EdpcTrigger;
  EDPC_DEV_INFO   EdpcRootPort;
  UINT8           NotifyType;
} EDPC_INFO;

typedef struct {
  UINT32        BaseMmmioAddress;
  UINT8         BaseBusNumber;
  UINT8         SegNumber;
  UINT16        Reserve1;
} ROOT_PORT_INFO;

typedef struct {
  UINT16          OsNativeAerSupport;
  UINT16          PcieRootPortCount;
  ROOT_PORT_INFO  RootPortInfo[MAX_ROOTPORT];
  EDPC_INFO       EdpcInfo;
  UINT64          EmcaL1DirAddr;
  UINT8           EmcaEn;
  UINT8           WheaEnabled;
  UINT8           WheaSupportEn;
} RAS_ACPI_PARAM;

#pragma pack(pop)

#endif // #ifndef _RAS_ACPI_H_
