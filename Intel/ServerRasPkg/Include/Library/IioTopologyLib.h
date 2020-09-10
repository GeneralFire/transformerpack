/** @file
  Internal header file for IIO RAS lib.

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

#ifndef _IIO_TOPOLOGY_H_
#define _IIO_TOPOLOGY_H_
#include <UncoreCommonIncludes.h>
#include <IioRegs.h>

//
// Type Definitions
//
typedef struct {
  UINT8 Enabled;
  UINT8 Bus;
  UINT8 Device;
  UINT8 Function;
  UINT8 SecondaryBus;
  UINT8 SubordinateBus;
} PCIE_PORT_INFO;

typedef struct {
  BOOLEAN  StackValid;
  UINT8    StackBus;
  UINT8    StackBusLimit;
  UINTN    RcrbBaseAddr;
  BOOLEAN  CxlValid;
} STACK_INFO;

typedef struct {
  BOOLEAN         Valid;
  UINT8           BaseBusNum;
  UINT8           PciSegment;
  UINT8           MaxPciePortNumberPerSkt;
  PCIE_PORT_INFO  Port[NUMBER_PORTS_PER_SOCKET];
  STACK_INFO      IioStackInfo[MAX_IIO_STACK];
  /*
  Note!!! - Socket10nmUboxBus0 is a field only used for 10nm CPU,  SKX/CPX should not use this
  field. whatever who want to use this field , please make sure use it properly
  */
  UINT8           Socket10nmUboxBus0;
} SKT_INFO;

typedef struct {
  UINT8     Stepping;
  SKT_INFO  SktInfo[MAX_SOCKET];
} IIO_TOPOLOGY;

/**
  Get Iio topology data, in case first one invoke this routine, it will initialize topology data.
  @retval  point of memoty topology info

**/
IIO_TOPOLOGY *
EFIAPI
GetIioTopology (
  VOID
  );


/**
  Get iio port pci address.

  @param[in]  Skt  -- Socket index
  @param[in]  PortIndex  -- iio port index per socket
  @param[out] Seg  -- iio port segment number
  @param[out] Bus  -- iio port bus address
  @param[out] Dev  -- iio port device address
  @param[out] func -- iio port function address

  @retval  TRUE  -- iio port exist; iio port is not exist.

**/
BOOLEAN
EFIAPI
GetRootPortInfo (
  IN  UINT8    Skt,
  IN  UINT8    PortIndex,
  OUT UINT8    *Seg,
  OUT UINT8    *Bus,
  OUT UINT8    *Dev,
  OUT UINT8    *Fun
  );

/**
  Get the root port of a specific Pcie Device

  @param[in]  Skt  -- Socket index
  @param[in]  InputBus  -- The Bus Number of specific device
  @param[in]  InputDev  -- The Dev Number of specific device
  @param[in]  InputFun  -- The FUN Number of specific device
  @param[out] Seg  -- iio root port segment number
  @param[out] Bus  -- iio root port bus address
  @param[out] Dev  -- iio root port device address
  @param[out] func -- iio root port function address

  @retval  TRUE  -- iio port exist; FALSE -- iio port is not exist.

**/
BOOLEAN
EFIAPI
GetRootPortInfoOfPcieDev (
  IN  UINT8    Skt,
  IN  UINT8    InputBus,
  IN  UINT8    InputDev,
  IN  UINT8    InputFun,
  OUT UINT8    *Seg,
  OUT UINT8    *Bus,
  OUT UINT8    *Dev,
  OUT UINT8    *Fun
  );

/**
  Check the IIO Stack availability

  @param[in]  Skt  -- Socket index
  @param[in]  IioStack  -- iio Stack index per socket

  @retval  TRUE  -- iio stack exist; FALSE -- iio stack is not exist.

**/
BOOLEAN
EFIAPI
IsIioStackValid (
  IN  UINT8    Skt,
  IN  UINT8    IioStack
  );

/**
  This implementation is to get IIO stack start bus number.

  @param  Socket -- socket index
  @param  IioStack -- iio stack index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetStackStartBus (
  IN      UINT8   Socket,
  IN      UINT8   IioStack,
  IN OUT  UINT8   *Bus
  );

/**
  This implementation is to get IIO stack limit bus number.

  @param  Socket -- socket index
  @param  IioStack -- iio stack index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetStackLimitBus (
  IN      UINT8   Socket,
  IN      UINT8   IioStack,
  IN OUT  UINT8   *Bus
  );

/**
  This implementation is to get 10nm CPU UBOX bus number.
  Note!!!- Socket10nmUboxBus0 is a field only used for 10nm CPU,  SKX/CPX should not use this
  field. whoever want to use this function , please make sure use it properly.

  @param  Socket -- socket index
  @param  *Bus -- Bus Number to return

  @retval  TRUE -- Bus Number is valid, FALSE -- Bus Number is invalid
**/
BOOLEAN
EFIAPI
GetSocket10nmUboxBus (
  IN      UINT8   Socket,
  IN OUT  UINT8   *Bus
  );

/**
  Get specific stack CXL Information

  @param  Socket -- socket index
  @param  Stack -- stack index
  @param  *RcrbBa -- RCRB Base Address

  @retval  TRUE -- CXL Information is valid, FALSE -- CXL Infomation is invalid
**/
BOOLEAN
EFIAPI
GetStackCxlInfo (
  IN      UINT32   Socket,
  IN      UINT8    Stack,
  OUT     UINTN    *RcrbBa,
  OUT     BOOLEAN  *CxlValid
  );
#endif
