/** @file
  RegAccess.h
  This file include register access.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2016 Intel Corporation. <BR>

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

#ifndef _REGACCESS_H_
#define _REGACCESS_H_

#include "MspData.h"

typedef enum {
  NoError,
  InvalidInstance,
  InvalidBoxType,
  InvalidMode,
  BoxTypeNotInSimulation,
  InvalidRemapType,
  UnknownError
} ACCESS_ERROR_TYPE;

typedef enum {
  ModeRead,
  ModeWrite,
  ModeOther
} ACCESS_MODE;

//***********************Structures used for RegisterAccess******************//
typedef struct {
  UINT32  Offset;           ///> Register Offset relative to the base address.
  UINT32  Mask;             ///> Starting bit within the register.
  UINT8   ShiftBit;         ///> Number of bits to shift to get to the register field in question
} REGISTER_ACCESS;

//******************************Functions Prototype**************************//
//
// Memory mapped PCI IO
//
#define PCI_CFG_32B_READ(PciExpressBase, Bus, Device, Function, Register) \
  Mmio32Read (PciExpressBase + \
    (UINT32)(Bus << 20) + \
    (UINT32)(Device << 15) + \
    (UINT32)(Function << 12) + \
    (UINT32)(Register))

#define PCI_CFG_32B_WRITE(PciExpressBase, Bus, Device, Function, Register, Value) \
  Mmio32Write (PciExpressBase + \
    (UINT32)(Bus << 20) + \
    (UINT32)(Device << 15) + \
    (UINT32)(Function << 12) + \
    (UINT32)(Register), \
    Value)

//
// Memory Mapped IO
//
UINT32
Mmio32Read (
  IN        UINTN      RegisterAddress
)
;

VOID
Mmio32Write (
  IN        UINTN      RegisterAddress,
  IN        UINT32      Value
)
;

UINT16
Mmio16Read (
  IN        UINTN      RegisterAddress
)
;

VOID
Mmio16Write (
  IN        UINTN      RegisterAddress,
  IN        UINT16      Value
)
;

UINT8
Mmio8Read (
  IN        UINTN      RegisterAddress
)
;

VOID
Mmio8Write (
  IN        UINTN      RegisterAddress,
  IN        UINT8       Value
)
;


#define Mmio32Or(Register, OrData)    Mmio32Write (Register, Mmio32Read (Register) | OrData)
#define Mmio16Or(Register, OrData)    Mmio16Write (Register, Mmio16Read (Register) | OrData)
#define Mmio8Or(Register, OrData)     Mmio8Write (Register, Mmio8Read (Register) | OrData)

#define Mmio32And(Register, AndData)  Mmio32Write (Register, Mmio32Read (Register) & (AndData))
#define Mmio16And(Register, AndData)  Mmio16Write (Register, Mmio16Read (Register) & (AndData))
#define Mmio8And(Register, AndData)   Mmio8Write (Register, Mmio8Read (Register) & (AndData))

#define Mmio32AndThenOr(Register, AndData, OrData)  Mmio32Write (Register, (((Mmio32Read (Register)& (AndData))) | OrData))
#define Mmio16AndThenOr(Register, AndData, OrData)  Mmio16Write (Register, (((Mmio16Read (Register)& (AndData))) | OrData))
#define Mmio8AndThenOr(Register, AndData, OrData)   Mmio8Write (Register, (((Mmio8Read (Register)& (AndData))) | OrData))

/**
  Returns the information for the specific channel/instance

  @param[in]      Boxtype     Unit to select
  @param[in]      Channel     Channel under test
  @param[in]      Instance    Instance within the channel


  @retval         Value Read
**/
UINT32
GetRegisterAccessInfo (
  IN        MSP_DATA    *MspData,
  IN        UINT8       BoxType,
  IN        UINT8       Channel,
  IN        UINT8       Instance,
  IN  OUT   UINT32      *Command,
  IN  OUT   UINT8       *PortId,
  IN  OUT   UINT32      *Offset,
  IN  OUT   UINT8       *Bus,
  IN  OUT   UINT8       *Device,
  IN  OUT   UINT8       *Func,
  IN        UINT32      Mode
);

/**
  Reads registers from a specified Unit

  @param[in]      Boxtype     Unit to select
  @param[in]      Channel     Channel under test
  @param[in]      Instance    Instance within the channel
  @param[in]      Register    Register information

  @retval         Value Read
**/
UINT32
MemRegRead (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register
);
/**
  Writes registers from a specified Unit

  @param[in]      Boxtype     Unit to select
  @param[in]      Channel     Channel under test
  @param[in]      Instance    Instance within the channel
  @param[in]      Register    Register information
  @param[in]      Data        Data to be written
  @param[in]      Be          Byte enable

  @retval         None
**/
VOID
MemRegWrite (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register,
  IN        UINT32          Data,
  IN        UINT8           Be
);
/**
  Reads the register field of the specified offset

  @param[in]      Boxtype     Unit to select
  @param[in]      Channel     Channel under test
  @param[in]      Instance    Instance within the channel
  @param[in]      Register    Register information

  @retval         None
**/
UINT32
MemFieldRead (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register
);
/**
  Writes the register field of the specified offset

  @param[in]      Boxtype     Unit to select
  @param[in]      Channel     Channel under test
  @param[in]      Instance    Instance within the channel
  @param[in]      Register    Register information
  @param[in]      Data        Data to be written
  @param[in]      Be          Byte enable

  @retval         None
**/

VOID
MemFieldWrite (
  IN        MSP_DATA        *MspData,
  IN        UINT8           BoxType,
  IN        UINT8           Channel,
  IN        UINT8           Instance,
  IN        REGISTER_ACCESS Register,
  IN        UINT32          Value,
  IN        UINT8           Be
)
;

#endif // _REGACCESS_H

