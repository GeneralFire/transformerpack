/** @file
  Interface of IEH RAS library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef __IEH_TYPES_H__
#define __IEH_TYPES_H__

typedef enum {
  NullDevice = 0,
  GlobalIeh,
  SatelliteIehNorth,
  SatelliteIehSouth,
  IehEndPoint,
  RCiEP,
  IehLocalDevice,
  IehDevTypeEnd
} IEH_DEVICE_TYPE;

typedef enum {
  IioRootPort = 1,
  PcieRootPort,
  PcieEndPoint,
  InternalDevice,
  DMIDevice,
  RlinkDevice,
  CbDMADevice,
  VTDDevice,
  RASDevice,
  M2PCIeDevice,
  SPDDevice,
  McDDRDevice,
  VppDevice,
  //to add new device here
  IehEndPointTypeEnd
} IEH_END_POINT_TYPE;

typedef enum {
  PsfDino = 1,
  PsfPciePi5,
  PsfHcX,
  PcieGen4DmiPi5,
  PcieGen5IalPi5,
  SpdI3cBus,
  CxpSmBus,
  //to add new device here
  IehLclDevTypeEnd
} IEH_LOCAL_DEV_TYPE;

typedef union {
  UINT64  Data;
  struct {
    UINT64 ChOnMc : 2;                /* channel number */
    UINT64 McId :2;                  /* Memory Controller ID */
    UINT64 Reserved: 60;
  } DsiMcddr;

  struct {
    VOID *pSouthIeh;
  } DsiDmi;

  struct {
    UINT64 Stack : 8;                /* Stack number, this is only used in lookup table*/
    UINT64 Reserved: 56;
  } DsiSatIeh;

  struct {
    UINT64 ErrorSourceId : 8;        /* ErrorSourceId , this is only used in lookup table*/
    UINT64 LclDevType : 8;
    UINT64 Reserved: 48;
  } DsiIehLclDev;
} DEV_SPECIFIC_INFO;

typedef struct{
  BOOLEAN              Valid;
  IEH_DEVICE_TYPE      Type;
  UINT8                Socket;
  UINT8                Stack;
  UINT8                Bus;
  UINT8                Dev;
  UINT8                Func;
  UINT32               SbPortId;
  UINT8                DevCount;      /*This DevCount means how many devices share the same bitindex*/
  DEV_SPECIFIC_INFO    DevSpecInfo;
} IEH_DEVICE_COMMON;

typedef struct {
  IEH_DEVICE_COMMON  Common;
  UINT8              IehVer;
  BOOLEAN            RcecSupport;
  UINT8              LocalErrorSource;
  UINT8              MaxBitIdx;       /*This indicate the maximum index of ConnectedDeviceList array*/
  IEH_DEVICE_COMMON  **ConnectedDeviceList;
} GLOBAL_IEH;

typedef struct {
  IEH_DEVICE_COMMON   Common;
  UINT8               IehVer;
  UINT8               BitIndex;
  BOOLEAN             RcecSupport;
  UINT32              ABMRCiEP;
  UINT8               RcecAbnLbn;
  UINT8               RcecAbnNbn;
  UINT8               LocalErrorSource;
  UINT8               MaxBitIdx;       /*This indicate the maximum index of ConnectedDeviceList array*/
  IEH_DEVICE_COMMON   **ConnectedDeviceList;
} SATELLITE_IEH ;

typedef union {
  UINT64  Data;
  struct {
    UINT64 Reserved  : 32;
    UINT64 CxlValid  : 8;
    UINT64 Reserved2 : 24;
  } CxlDevInfo;
} LCL_DEV_SPEC_INFO;

typedef struct {
  IEH_DEVICE_COMMON   Common;
  IEH_LOCAL_DEV_TYPE  IehLclDevType;
  UINT8               ErrorSourceId;
  LCL_DEV_SPEC_INFO   LclSpecInfo;
} IEH_LOCAL_DEVICE ;

typedef struct {
  IEH_DEVICE_COMMON   Common;
  IEH_END_POINT_TYPE  IehEndPointType;
  UINT8               BitIndex;
} IEH_END_POINT ;

typedef struct {
  IEH_DEVICE_COMMON Common;
} RCiEP_DEVICE;

typedef union {
  IEH_DEVICE_COMMON Common;
  GLOBAL_IEH        GlobalIEH;
  SATELLITE_IEH     SatelliteIEH;
  IEH_END_POINT     IehEndPoint;
  IEH_LOCAL_DEVICE  IehLocalDevice;
  RCiEP_DEVICE      RciepDevice;
} NODE_DEVICE;

#endif
