/** @file
  Implementation of iio root port silicon interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <RcRegs.h>
#include <Library/RasDebugLib.h>
#include <Library/IoLib.h>
#include <Library/SystemInfoLib.h>

/**
  Get SideBand Private Chipset Register's phasical address

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset

  return Phasical MMIO address of this register
**/
UINTN
EFIAPI
CpuSbiGetPcrRegAddr (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset
  )
{
  CPU_CSR_ACCESS_VAR            *CpuCsrAccessVarPtr;
  UINT8                         Size = 4;
  SBREG_PTE0_UBOX_MISC_STRUCT   SbRegPte0;
  UINTN                         PhysicalAddress;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  if (CpuCsrAccessVarPtr->LastSBPortId[Socket] != PortId) {
    PhysicalAddress = GetCpuCsrAddress (Socket, 0, SBREG_PTE0_UBOX_MISC_REG, &Size);
    SbRegPte0.Data = MmioRead32 (PhysicalAddress);
    SbRegPte0.Bits.lower_portid = (UINT8)(PortId & 0xFF);
    SbRegPte0.Bits.upper_port_id = (PortId >> 8) & (BIT1 | BIT0);
    SbRegPte0.Bits.fid = (UINT8)(PortId >> 24);
    SbRegPte0.Bits.valid = 1;
    MmioWrite32 (PhysicalAddress, SbRegPte0.Data);
    CpuCsrAccessVarPtr->LastSBPortId[Socket] = PortId;
  }
  PhysicalAddress = (UINTN)(CpuCsrAccessVarPtr->MmioBar[Socket][TYPE_SBREG_BAR] | RegOffset);
  return PhysicalAddress;
}

/**
  Read a 8 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
**/
UINT8
EFIAPI
CpuSbiReadPcrReg8 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  return MmioRead8 (PhysicalAddress);
}

/**
  Read a 16 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
**/
UINT16
EFIAPI
CpuSbiReadPcrReg16 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  return MmioRead16 (PhysicalAddress);
}

/**
  Read a 32 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
**/
UINT32
EFIAPI
CpuSbiReadPcrReg32 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  return MmioRead32 (PhysicalAddress);
}

/**
  Write a 8 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
  @param[in]  Data          8 bit data to write
**/
VOID
EFIAPI
CpuSbiWritePcrReg8 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset,
  IN      UINT8   Data
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  MmioWrite8 (PhysicalAddress, Data);
}

/**
  Write a 16 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
  @param[in]  Data          16 bit data to write
**/
VOID
EFIAPI
CpuSbiWritePcrReg16 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset,
  IN      UINT16  Data
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  MmioWrite16 (PhysicalAddress, Data);
}

/**
  Write a 32 bit width Private Chipset Register

  @param[in]  Socket        Socket index.
  @param[in]  PortId        SideBand Port Id.
  @param[in]  RegOffset     Register's offset
  @param[in]  Data          32 bit data to write
**/
VOID
EFIAPI
CpuSbiWritePcrReg32 (
  IN      UINT8   Socket,
  IN      UINT32  PortId,
  IN      UINT16  RegOffset,
  IN      UINT32  Data
  )
{
  UINTN                         PhysicalAddress;
  PhysicalAddress = CpuSbiGetPcrRegAddr (Socket, PortId, RegOffset);
  MmioWrite32 (PhysicalAddress, Data);
}