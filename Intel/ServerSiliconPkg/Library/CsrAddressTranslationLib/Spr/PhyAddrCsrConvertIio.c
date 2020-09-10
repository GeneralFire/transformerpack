/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2019-2020 Intel Corporation. <BR>

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

#include "CsrAddressTranslationLibInternal.h"

extern CONST CHAR8 INVALID_BOX_INSTANCE_MESSAGE[];
extern CONST CHAR8 BAR_NOT_INITIALIZED[];
extern CONST CHAR8 BAR_64_NOT_IN_PEI[];
extern CONST UINT8 mDeviceFunctionTable[MAX_BOX_TYPES][MAX_FUNCBLK];
STATIC UINT8 m2pcieDevTable[MAX_SKX_M2PCIE] = { 22, 21, 22, 23, 21};
STATIC UINT8 mMcxDevTable[6] = {10, 10, 11, 12, 12,13};


//  14. BOX_IIO_EXPPTMBAR
/**
  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO PCIE_EXPPTMBAR device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_EXPPTMBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                      PciBarL32Bit;
  UINT32                      PciBarH32Bit;
  UINT64                      PciBar64Bit;
  UINTN                       PhyAddress;
  UINT32                      Offset;

  BOOLEAN                     Support64bit;
  UINT32                      BarMaskValue;
  CSR_OFFSET                  BarCsrOffset;

  Support64bit = TRUE;
  PciBarL32Bit = 0x00;
  PciBarH32Bit = 0x00;
  PciBar64Bit  = 0x00;
  BarMaskValue = 0xFFFE0000;

  //
  // Read the Bar value
  //
  Offset = CsrOffset.Bits.offset;
  BarCsrOffset.Data = EXPPTMBAR_N0_IIO_PCIE_G5_REG;

  PhyAddress = CNVT_IIO_PCIE (SocketId, InstId, BarCsrOffset, CpuCsrAccessVarPtr,InstType);
  PciBarL32Bit = MmioRead32 (PhyAddress);

  //
  // Check if it supports 64 bit bar.
  //
  if (Support64bit) {
    PciBarH32Bit = MmioRead32 (PhyAddress + 4);

#ifdef IA32
    if ((PciBarH32Bit) != 0x00) {
      DEBUG((DEBUG_ERROR, BAR_64_NOT_IN_PEI));
      return CONVERT_FAILED;
    }
    if (((PciBarL32Bit & BarMaskValue) == BarMaskValue) || ((PciBarL32Bit & BarMaskValue) == 0x0)) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }
    PhyAddress = (UINTN)((PciBarL32Bit & BarMaskValue) + (Offset & 0x0000FFFF));
#else
    PciBar64Bit = (UINT64)(LShiftU64 ((UINT64)PciBarH32Bit, 32) + (PciBarL32Bit & BarMaskValue));
    if ((PciBar64Bit == 0) || (PciBar64Bit == (0xFFFFFFFF00000000 + (UINT64)BarMaskValue))) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }
    PhyAddress = (UINTN)(PciBar64Bit + (Offset & 0x0000FFFF));
#endif
  } else {
    if (((PciBarL32Bit & BarMaskValue) == BarMaskValue) || ((PciBarL32Bit & BarMaskValue) == 0x0)) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }

    PhyAddress = (UINTN)((PciBarL32Bit & BarMaskValue) | (Offset & 0x0000FFFF));
  }

  DEBUG ((EFI_D_VERBOSE, "[CSR PCI BAR Access] Address:0x%lx Data:0x%x\n", (UINT64)PhyAddress, MmioRead32 (PhyAddress)));

  return PhyAddress;
}

//  17. BOX_IIO_VTD
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_VTD device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_VTD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{

  UINT8      Dev;
  UINT8      Fun;
  UINT8      Bus;

  Bus = 0;
  Dev = 0;
  Fun = 0;

  if ((InstId >= MAX_LOGIC_IIO_STACK) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK0 + InstId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO DFX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_DFX (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{

  UINT8      Dev;
  UINT8      Fun;
  UINT8      Bus;

  Bus = 0;
  Dev = 0;
  Fun = 0;

  if ((InstId >= MAX_LOGIC_IIO_STACK) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK0 + InstId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, BoxInst, CsrOffset & CpuCsrAccessVarPtr
  for Box type MSM device.

  @param SocketId            - The Socket ID
  @param BoxInst             - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_MSM(
  IN UINT8               SocketId,
  IN UINT8               BoxInst,
  IN CSR_OFFSET          CsrOffset,
  IN CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr,
  IN UINT8               InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  if (BoxInst >= MAX_ALL_MSM) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, BoxInst));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

//  25. BOX_SB
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type BOX_SB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_SB (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                      SbRegPortId;
  UINTN                       PhysicalAddress;
  CSR_OFFSET                  SbCsrOffset;
  SBREG_PTE0_UBOX_MISC_STRUCT SbRegPte0;

  SbRegPte0.Data = 0;

  if (InstType != CsrBoxInst) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  SbRegPortId = GetSbRegPortId(InstId,CsrOffset.Data);
  if (SB_REG_PORT_ID_INVALID == SbRegPortId) {
    DEBUG (( DEBUG_ERROR, "Invalid SB Port Id: %x \n", SbRegPortId));
    return CONVERT_FAILED;
  }

  if (CpuCsrAccessVarPtr->LastSBPortId[SocketId] != SbRegPortId) {
    SbCsrOffset.Data = SBREG_PTE0_UBOX_MISC_REG;
    PhysicalAddress = CNVT_UBOX(SocketId, 0, SbCsrOffset, CpuCsrAccessVarPtr, InstType);
    SbRegPte0.Data = MmioRead32 (PhysicalAddress);
    SbRegPte0.Bits.lower_portid = (UINT8)(SbRegPortId & 0xFF);
    SbRegPte0.Bits.upper_port_id = (SbRegPortId >> 8) & (BIT1 | BIT0);
    SbRegPte0.Bits.fid = (UINT8)(SbRegPortId >> 24);
    SbRegPte0.Bits.valid = 1;
    MmioWrite32 (PhysicalAddress, SbRegPte0.Data);
    if (UbiosGenerationEnabled ()) {
      DEBUG((DEBUG_ERROR, "\nuBIOSTag AddrTypeSB Socket:0x%x;Port:0x%x;Inst:0x%x; Address: [0%08xh], Value:0%08xh\n", SocketId, SbRegPortId, InstId, PhysicalAddress, SbRegPte0.Data));
      DEBUG((DEBUG_ERROR, "  mov DWORD PTR ds:[0%08xh], 0%08xh\n", PhysicalAddress, SbRegPte0.Data));
    }
    CpuCsrAccessVarPtr->LastSBPortId[SocketId] = SbRegPortId;
  }

  PhysicalAddress = (UINTN)(CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SBREG_BAR] | (CsrOffset.Bits.offset & 0xffff));
  DEBUG((DEBUG_VERBOSE, "[SB Access]SB Mmio Address: 0x%x \n", PhysicalAddress));

  return PhysicalAddress;
}

//  30. BOX_NPK
/**
  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NPK device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_NPK (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  Bus = 0;
  Dev = 0;
  Fun = 0;

  if ((InstId >= MAX_DINO_INSTANCE_SPR) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  //
  // In KtiRcSim, put the NPK device to STACK0+10 to avoid it falls in the same range to the IIO stack1.
  //
  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8 + InstId];;
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NPK Memory device.  This function uses MTB bar on NPK to translate physical
  addresses to access registers in North Peak Mem space.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_NPK_MEM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
)
{
  UINT64                      PciBar64Bit;
  UINTN                       PhyAddress;
  UINT32                      Offset;
  BOOLEAN                     Support64bit;
  UINT32                      BarMaskValue;
  CSR_OFFSET                  BarCsrOffset;
  MTB_LBAR_NPK_MAIN_STRUCT    NpkMtbLBar;
  MTB_UBAR_NPK_MAIN_STRUCT    NpkMtbUBar;
  CMD_NPK_MAIN_STRUCT         NpkCmdReg;

  Support64bit = FALSE;
  PciBar64Bit = 0x00;
  BarMaskValue = 0xFFFFFFF0;

  //
  // Read the Bar values
  //
  Offset = CsrOffset.Bits.offset;
  BarCsrOffset.Data = MTB_LBAR_NPK_MAIN_REG;
  PhyAddress = CNVT_NPK (SocketId, InstId, BarCsrOffset, CpuCsrAccessVarPtr, InstType);
  NpkMtbLBar.Data = MmioRead32 (PhyAddress);

  BarCsrOffset.Data = MTB_UBAR_NPK_MAIN_REG;
  PhyAddress = CNVT_NPK (SocketId, InstId, BarCsrOffset, CpuCsrAccessVarPtr, InstType);
  NpkMtbUBar.Data = MmioRead32 (PhyAddress);

  BarCsrOffset.Data = CMD_NPK_MAIN_REG;
  PhyAddress = CNVT_NPK (SocketId, InstId, BarCsrOffset, CpuCsrAccessVarPtr, InstType);
  NpkCmdReg.Data = MmioRead32 (PhyAddress);

  //
  // Check memory type bits(2-1) in BAR to find whether it supports 64 bit bar.
  //
  if (NpkMtbLBar.Bits.adrng == 0x2) {
    Support64bit = TRUE;
  }

  if (Support64bit) {
#ifdef IA32
    if ((NpkMtbUBar.Data) != 0x00) {
      DEBUG((DEBUG_ERROR, BAR_64_NOT_IN_PEI));
      return CONVERT_FAILED;
    }
    if ((NpkMtbLBar.Data & BarMaskValue) == 0x0) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }
    PhyAddress = (NpkMtbLBar.Data & BarMaskValue) + Offset;
#else //#ifdef IA32
    PciBar64Bit = (UINT64)(LShiftU64((UINT64)NpkMtbUBar.Data, 32) | (NpkMtbLBar.Data & BarMaskValue));
    if (PciBar64Bit == 0) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }
    PhyAddress = (UINTN)(PciBar64Bit + Offset);
#endif //#ifdef IA32
  } else {
    if ((NpkMtbLBar.Data & BarMaskValue) == 0x0) {
      DEBUG((DEBUG_ERROR, BAR_NOT_INITIALIZED));
      return CONVERT_FAILED;
    }
    PhyAddress = (NpkMtbLBar.Data & BarMaskValue) + Offset;
  }
  DEBUG ((DEBUG_ERROR, "[CSR PCI BAR Access]Address:0x%x%x; PCI Cmd: 0x%04x\n", NpkMtbUBar.Data, (NpkMtbLBar.Data & BarMaskValue) + Offset, (UINT16)NpkCmdReg.Data));

  return PhyAddress;
}

//  35. IIO_M2IAL
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO M2IAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_M2IAL(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  if ((InstId >= MAX_LOGIC_IIO_STACK) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK0 + InstId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type DSA/IAX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DSA_IAX(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  if (((InstId >= MAX_DINO_INSTANCE_SPR) || (InstType != CsrBoxInst)) || \
  ((ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc) && (InstId > MAX_DINO_INSTANCE_SPR_1D))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  //
  // In KtiRcSim, put the NPK device to STACK0+10 to avoid it falls in the same range to the IIO stack1.
  //
  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8 + InstId];;
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CPM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CPM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  if (((InstId >= MAX_DINO_INSTANCE_SPR) || (InstType != CsrBoxInst)) || \
  ((ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc) && (InstId > MAX_DINO_INSTANCE_SPR_1D)) || \
  (CpuCsrAccessVarPtr->HcxType[SocketId] == IIO_HCX_NONE)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (CpuCsrAccessVarPtr->HcxType[SocketId] == IIO_HCB) {
    Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8] + InstId * HCX_BUS_RANGE + CPM_BUS_OFFSET;
  } else {
    Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8 + InstId] + CPM_BUS_OFFSET;
  }
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HQM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HQM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;

  if (((InstId >= MAX_DINO_INSTANCE_SPR) || (InstType != CsrBoxInst)) || \
  ((ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc) && (InstId > MAX_DINO_INSTANCE_SPR_1D)) || \
  (CpuCsrAccessVarPtr->HcxType[SocketId] == IIO_HCX_NONE)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (CpuCsrAccessVarPtr->HcxType[SocketId] == IIO_HCB) {
    Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8] + InstId * HCX_BUS_RANGE + HQM_BUS_OFFSET;
  } else {
    Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK8 + InstId] + HQM_BUS_OFFSET;
  }
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO_PCIEDMI device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_PCIEDMI(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                      BarMaskValue;
  UINT32                       DmiRcrbBase32;

  if ((InstId >= MAX_PI5_INSTANCE_SPR) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  BarMaskValue      = 0xFFFFE000;
  DmiRcrbBase32 = CpuCsrAccessVarPtr->StackMmiol[SocketId][0];
  return (UINTN)((DmiRcrbBase32 & BarMaskValue) | (CsrOffset.Bits.offset & ~BarMaskValue));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type NTB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_PCIENTB(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8      Bus;
  UINT8      Dev;
  UINT8      Fun;

  if ((InstId >= MAX_PI5_INSTANCE_SPR) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK1 + InstId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IIO PCIe device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IIO_PCIE(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8      Bus;
  UINT8      Dev;
  UINT8      Fun;

  if ((InstId > MAX_PI5_PORT) || (InstType != CsrBoxInst) || \
  (ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc && (InstId > MAX_PI5_PORT_1D))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (InstId == PORT_0_INDEX) {
    return CNVT_IIO_PCIEDMI (SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType);
  } else {
    Bus = CpuCsrAccessVarPtr->StackBus[SocketId][IIO_STACK1 + (InstId - 1) / NUMBER_PORTS_PER_STACK_SPR];
    Dev = ((mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3) + (InstId - 1) % NUMBER_PORTS_PER_STACK_SPR);
  }
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CXL.IO device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CXL_RCRBBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                      BarMaskValue;
  UINT32                      CxlRcrbBase32;

  if ((InstId >= MAX_PI5_INSTANCE_SPR - 1) || (InstType != CsrBoxInst) || \
      ((ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc) && (InstId > MAX_CXL_PER_SOCKET_1D))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT (FALSE);
    return CONVERT_FAILED;
  }

  BarMaskValue  = 0xFFFF0000;
  CxlRcrbBase32 = CpuCsrAccessVarPtr->StackMmiol[SocketId][InstId];
  return (UINTN) ((CxlRcrbBase32 & BarMaskValue) | (CsrOffset.Bits.offset & ~BarMaskValue));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for CXL DP's MEMBAR0 region (cxl.cm, cxl.am).

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CXL_DP_MBAR(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                      CxlDpMemBarLow, CxlDpMemBarHigh, Offset, BarMaskValue;
  CSR_OFFSET                  BarCsrOffset;
  UINTN                       PhyAddress;
  UINT64                      CxlMemBar64Bit;

  if ((InstId > MAX_CXL_PER_SOCKET) || (InstType != CsrBoxInst) || \
      ((ONE_DIE_IN_SOCKET == GetDieCount(SocketId) && GetChopType (0) == TypeXcc) && (InstId > MAX_CXL_PER_SOCKET_1D))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (GetCxlStatus (SocketId, InstId) != AlreadyInCxlMode) {
    DEBUG (( DEBUG_ERROR, "\n%a() - WARNING: Socket[%x]Stack[%x] is not CXL port!\n", __FUNCTION__, SocketId, InstId));
  }

  BarCsrOffset.Data = EXPPTMBAR_N0_IIO_PCIE_G5_REG;
  BarMaskValue = 0xFFFE0000;
  CxlDpMemBarLow = CxlDpMemBarHigh = 0;
  CxlMemBar64Bit = 0;
  Offset = CsrOffset.Bits.offset;

  CxlDpMemBarLow = MmioRead32 (CpuCsrAccessVarPtr->StackMmiol[SocketId][InstId] + BarCsrOffset.Bits.offset);
  CxlDpMemBarHigh = MmioRead32 (CpuCsrAccessVarPtr->StackMmiol[SocketId][InstId] + BarCsrOffset.Bits.offset + 4);

#ifdef IA32
    if ((CxlDpMemBarHigh) != 0x00) {
      DEBUG((DEBUG_ERROR, "[CXL DP MEMBAR0] CXL DP MEMBAR0 64 bit can not access in PEI phase!\n"));
      return CONVERT_FAILED;
    }
    if (((CxlDpMemBarLow & BarMaskValue) == BarMaskValue) || ((CxlDpMemBarLow & BarMaskValue) == 0x0)) {
      DEBUG((DEBUG_ERROR, "[CXL DP MEMBAR0] CXL DP MEMBAR0 is not initialized !\n"));
      return CONVERT_FAILED;
    }

    PhyAddress = (UINTN)((CxlDpMemBarLow & BarMaskValue) + (Offset & 0x0000FFFF));
#else
    CxlMemBar64Bit = (UINT64)(LShiftU64((UINT64)CxlDpMemBarHigh, 32) + (CxlDpMemBarLow & BarMaskValue));
    if ((CxlMemBar64Bit == 0) || (CxlMemBar64Bit == (0xFFFFFFFF00000000 + (UINT64)BarMaskValue))) {
      DEBUG((DEBUG_ERROR, "[CXL DP MEMBAR0] CXL DP MEMBAR0 is not initialized !\n"));
      return CONVERT_FAILED;
    }

    PhyAddress = (UINTN)(CxlMemBar64Bit + (UINT64)(Offset & 0x0000FFFF));
#endif

  return PhyAddress;
}
