/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2018 -2020 Intel Corporation.

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
#include <Library/CsrAddressTranslationLib.h>

extern CONST UINT8 mDeviceFunctionTable[MAX_BOX_TYPES][MAX_FUNCBLK];

#define MMIO_REGION_MCCADB_CH0_SC1 0x8000
#define MMIO_REGION_MCCADB_CH1_SC1 0xB000

//
//  DDRIO station ID ranges
//
//  |-------------------------------------------|
//  | 0x00 - 0x27  |  DATA Unicast Instance 0   |
//  | 0x28 - 0x2B  |  CC Unicast Instance 0     |
//  | 0x2C - 0x2C  |  CLK Unicast Instance 0    |
//  | 0x2D - 0x3E  |  xxxx                      |
//  | 0x3F - 0x3F  |  DDRINTF                   |
//  | 0x40 - 0x67  |  DATA Unicast Instance 1   |
//  | 0x68 - 0x6B  |  CC Unicast Instance 1     |
//  | 0x6C - 0x6C  |  CLK Unicast Instance 1    |
//  | 0x6D - 0x7E  |  xxxx                      |
//  | 0x7F - 0x7F  |  DDRCOMP                   |
//  | 0x80 - 0xA7  |  xxxx                      |
//  | 0xA8 - 0xA9  |  CC Multicast Instance 0   |
//  | 0xAA - 0xAB  |  CC Multicast Instance 1   |
//  | 0xAC - 0xAD  |  CC Broadcast              |
//  | 0xAE - 0xAF  |  xxxx                      |
//  | 0xB0 - 0xB3  |  DATA Multicast Instance 0 |
//  | 0xB4 - 0xB7  |  DATA Multicast Instance 1 |
//  | 0xB8 - 0xBB  |  DATA Broadcast            |
//  | 0xBC - 0xBE  |  xxxx                      |
//  | 0xBF - 0xBF  |  CLK Broadcast             |
//  | 0xC0 - 0xFE  |  xxxx                      |
//  | 0xFF - 0xFF  |  SAI                       |
//  |-------------------------------------------|
//
//
// Data/Cmd/Ctl/Clk unicast registers
//
#define MCIO_UNICAST_INST0_STATIONID_BASE               0x00
#define MCIO_UNICAST_INST0_STATIONID_LIMIT              0x2C
#define MCIO_UNICAST_INST_STATIONID_MULTIPLIER          0x40
//
// Cmd/Ctl multicast registers
//
#define MCIO_CC_MULTICAST_INST0_STATIONID_BASE          0xA8
#define MCIO_CC_MULTICAST_INST0_STATIONID_LIMIT         0xA9
#define MCIO_CC_MULTICAST_INST_STATIONID_MULTIPLIER     0x02
//
// Data multicast registers
//
#define MCIO_DATA_MULTICAST_INST0_STATIONID_BASE        0xB0
#define MCIO_DATA_MULTICAST_INST0_STATIONID_LIMIT       0xB3
#define MCIO_DATA_MULTICAST_INST_STATIONID_MULTIPLIER   0x04
/**

  Determine if the posted access method state is enabled.
  If enabled, return bit mask to OR with the physical CSR address to trigger a posted write.
  If not enabled, return 0.

  @param[in] CpuCsrAccessVarPtr  - A pointer to the CSR access variable

  @retval   If posted method state ENABLED:  Bit mask to OR with the physical CSR address
            If posted method state DISABLED: 0

**/
UINTN
GetPostedMethodMask (
  IN CPU_CSR_ACCESS_VAR      *CpuCsrAccessVarPtr
  )
{
  UINTN PostedMethodMask = 0;

  if (GetPostedMethodState (CpuCsrAccessVarPtr) == POSTED_METHOD_ENABLED) {
    PostedMethodMask = MEMSS_POSTED_WRITE_MASK;
  }
  return PostedMethodMask;
}

// 7. MC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DDR_MC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8  BoxInst;
  UINT8  McId;
  UINT8  ChId;
  UINTN  PostedMethodMask;
  UINTN  PhysicalAddress;
  UINT32 MmioRegion;
  UINT32 MemoryBar;
  UINT8  StationId;

  HbmCompat(CNVT_HBM_MC, SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType, &PhysicalAddress);
  if (PhysicalAddress != MAX_UINTN) {
    return PhysicalAddress;
  }

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = ChId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MC assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MC assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  MmioRegion = MMIO_REGION_MC0 + (BoxInst % MAX_MC_CH) * 0x08;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;
  StationId = (CsrOffset.Data & 0xffff) >> 8;

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
}

// 7. MC_GLOBAL
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MC_GLOBAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DDR_MC_GLOBAL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8  BoxInst;
  UINT8  McId;
  UINT8  ChId;
  UINTN  PostedMethodMask;
  UINTN  PhysicalAddress;
  UINT32 MmioRegion;
  UINT32 MemoryBar;
  UINT8  StationId;

  HbmCompat(CNVT_HBM_MC_GLOBAL, SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType, &PhysicalAddress);
  if (PhysicalAddress != MAX_UINTN) {
    return PhysicalAddress;
  }

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = McId * MAX_MC_CH;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MC_GLOBAL assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MC_GLOBAL assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  MmioRegion = MMIO_REGION_MCGLOBAL;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;
  StationId = (CsrOffset.Data & 0xffff) >> 8;

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
}

// 8. MCIO
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCIO device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_MCIO(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32 MmioRegion;
  UINT8  McId;
  UINT32 MemoryBar;
  UINT8  StationId;
  UINT8  BoxInst;
  UINT8  ChId;
  UINTN  PostedMethodMask;

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = ChId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_MCIO assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_MCIO assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  MmioRegion = MMIO_REGION_DDRIO;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;

  StationId = (CsrOffset.Data & 0xffff) >> 8;
  //
  // If the register has multiple instances within the DDRIO, this routine was called
  // with the offset of instance 0, and the offset must now be adjusted using the BoxInst.
  //
  if (StationId <= MCIO_UNICAST_INST0_STATIONID_LIMIT) {
    StationId += (BoxInst % CpuCsrAccessVarPtr->DdrNumChPerMc) * MCIO_UNICAST_INST_STATIONID_MULTIPLIER;
  } else if ((StationId >= MCIO_CC_MULTICAST_INST0_STATIONID_BASE) && (StationId <= MCIO_CC_MULTICAST_INST0_STATIONID_LIMIT)) {
    StationId += (BoxInst % CpuCsrAccessVarPtr->DdrNumChPerMc) * MCIO_CC_MULTICAST_INST_STATIONID_MULTIPLIER;
  } else if ((StationId >= MCIO_DATA_MULTICAST_INST0_STATIONID_BASE) && (StationId <= MCIO_DATA_MULTICAST_INST0_STATIONID_LIMIT)) {
    StationId += (BoxInst % CpuCsrAccessVarPtr->DdrNumChPerMc) * MCIO_DATA_MULTICAST_INST_STATIONID_MULTIPLIER;
  }

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
}

// 11. MCDDC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCDDC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DDR_MCDDC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8  BoxInst;
  UINT8  McId;
  UINT8  ChId;
  UINTN  PostedMethodMask;
  UINTN  PhysicalAddress;
  UINT32 MmioRegion;
  UINT32 MemoryBar;
  UINT8  StationId;

  HbmCompat(CNVT_HBM_MC, SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType, &PhysicalAddress);
  if (PhysicalAddress != MAX_UINTN) {
    return PhysicalAddress;
  }

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = ChId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MCDDC assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_DDR_MCDDC assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  // MC is 32K aligned
  // #define MMIO_REGION_MC0         0x20
  // #define MMIO_REGION_MC1         0x28
  MmioRegion = MMIO_REGION_MC0 + (BoxInst % MAX_MC_CH) * 0x08;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;
  StationId = (CsrOffset.Data & 0xffff) >> 8;

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
}

// 21. HBM
UINTN
EFIAPI
CNVT_HBM_IO_LOW (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    HbmIoId;
  UINT8    ChId;

  HbmIoId  = 0;
  ChId     = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerIo != 0);
  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    ChId = InstId % CpuCsrAccessVarPtr->HbmNumChPerIo;
    HbmIoId = InstId / CpuCsrAccessVarPtr->HbmNumChPerIo;
    break;
  case  CsrMcId:
    ChId = 0;
    HbmIoId = (InstId / (CpuCsrAccessVarPtr->HbmNumChPerIo / CpuCsrAccessVarPtr->HbmNumChPerMc));
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_IO_LOW assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (HbmIoId >= CpuCsrAccessVarPtr->HbmMaxIoInst) {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_IO_LOW assert on access to hbm io >= MAX_HBM_IO\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMIO + SPR_GLOBAL_ADDR_HBMIO_ALL * HbmIoId + SPR_GLOBAL_ADDR_HBMIO_CH * ChId;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Bits.offset & 0xfff));
}

UINTN
EFIAPI
CNVT_HBM_IO_HIGH (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    HbmIoId;
  UINT8    ChId;

  HbmIoId  = 0;
  ChId     = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerIo != 0);
  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    ChId = InstId % CpuCsrAccessVarPtr->HbmNumChPerIo;
    HbmIoId = InstId / CpuCsrAccessVarPtr->HbmNumChPerIo;
    break;
  case  CsrMcId:
    ChId = 0;
    HbmIoId = (InstId / (CpuCsrAccessVarPtr->HbmNumChPerIo / CpuCsrAccessVarPtr->HbmNumChPerMc));
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_IO_HIGH assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (HbmIoId >= CpuCsrAccessVarPtr->HbmMaxIoInst) {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_IO_HIGH assert on access to hbm io >= MAX_HBM_IO\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMIO + SPR_GLOBAL_ADDR_HBMIO_MCAST + \
                  SPR_GLOBAL_ADDR_HBMIO_ALL * HbmIoId + SPR_GLOBAL_ADDR_HBMIO_CH * ChId;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Bits.offset & 0xfff));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_AWORD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  return CNVT_HBM_IO_LOW (SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType);
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_DWORD (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  if (CsrOffset.Bits.offset >= SPR_GLOBAL_ADDR_HBMIO_MCAST) {
    return CNVT_HBM_IO_HIGH (SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType);
  } else {
    return CNVT_HBM_IO_LOW (SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType);
  }
}

// Per even channel to program the HBMIO_FUSE_TUNING
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MIDSTACK (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    HbmIoId;
  UINT8    McId;

  HbmIoId  = 0;
  McId     = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerIo != 0);
  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    McId = ((InstId % CpuCsrAccessVarPtr->HbmNumChPerIo) / CpuCsrAccessVarPtr->HbmNumChPerMc);
    HbmIoId = InstId / CpuCsrAccessVarPtr->HbmNumChPerIo;
    break;
  case  CsrMcId:
    McId = (InstId % (CpuCsrAccessVarPtr->HbmNumChPerIo / CpuCsrAccessVarPtr->HbmNumChPerMc));
    HbmIoId = (InstId / (CpuCsrAccessVarPtr->HbmNumChPerIo / CpuCsrAccessVarPtr->HbmNumChPerMc));
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MIDSTACK assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (HbmIoId >= CpuCsrAccessVarPtr->HbmMaxIoInst) {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MIDSTACK assert on access to hbm io >= MAX_HBM_IO\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMIO + SPR_GLOBAL_ADDR_HBMIO_MCAST + \
                    SPR_GLOBAL_ADDR_HBMIO_ALL * HbmIoId + SPR_GLOBAL_ADDR_HBMIO_CH * CpuCsrAccessVarPtr->HbmNumChPerMc * McId;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Bits.offset & 0xfff));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MID_GBL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    HbmIoId;

  HbmIoId    = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerIo != 0);
  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    HbmIoId = InstId / CpuCsrAccessVarPtr->HbmNumChPerIo;
    break;
  case  CsrMcId:
    HbmIoId = (InstId / (CpuCsrAccessVarPtr->HbmNumChPerIo / CpuCsrAccessVarPtr->HbmNumChPerMc));
    break;
  case  CsrIoId:
    HbmIoId = InstId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MID_GBL assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (HbmIoId >= CpuCsrAccessVarPtr->HbmMaxIoInst) {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MID_GBL assert on access to hbm io >= MAX_HBM_IO\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMIO + SPR_GLOBAL_ADDR_HBMIO_ALL * HbmIoId;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | CsrOffset.Bits.offset);
}

// 46  HBM2E MC
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    McId;
  UINT8    ChId;

  McId    = 0;
  ChId    = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    ChId = InstId % CpuCsrAccessVarPtr->HbmNumChPerMc;
    McId = InstId / CpuCsrAccessVarPtr->HbmNumChPerMc;
    break;
  case  CsrMcId:
    ChId = 0;
    McId = InstId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MC assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (McId >= (CpuCsrAccessVarPtr->HbmMaxCh / CpuCsrAccessVarPtr->HbmNumChPerMc))  {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MC assert on access to mc >= MAX_HBM_IMC\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMMC + SPR_GLOBAL_ADDR_HBMMC_RANGE * McId + SPR_GLOBAL_ADDR_HBMCH_RANGE * ChId;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | (GlobalAddress + (CsrOffset.Data % SPR_GLOBAL_ADDR_HBMCH_RANGE)));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_CPGC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    McId;
  UINT8    ChId;

  McId    = 0;
  ChId    = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    ChId = InstId % CpuCsrAccessVarPtr->HbmNumChPerMc;
    McId = InstId / CpuCsrAccessVarPtr->HbmNumChPerMc;
    break;
  case  CsrMcId:
    ChId = 0;
    McId = InstId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_CPGC assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (McId >= (CpuCsrAccessVarPtr->HbmMaxCh / CpuCsrAccessVarPtr->HbmNumChPerMc))  {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_CPGC assert on access to mc >= MAX_HBM_IMC\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMMC + SPR_GLOBAL_ADDR_HBMMC_RANGE * McId + SPR_GLOBAL_ADDR_HBMCH_RANGE * ChId + SPR_GLOBAL_ADDR_HBM_CPGC_OFFSET;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Data & 0xfff));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type HBM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_HBM_MC_GLOBAL (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;
  UINT8    McId;

  McId    = 0;

  ASSERT(CpuCsrAccessVarPtr->HbmNumChPerMc != 0);

  switch(InstType) {
  case  CsrBoxInst:
  case  CsrChId:
    McId = InstId / CpuCsrAccessVarPtr->HbmNumChPerMc;
    break;
  case  CsrMcId:
    McId = InstId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MC_GLOBAL assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }

  if (McId >= (CpuCsrAccessVarPtr->HbmMaxCh / CpuCsrAccessVarPtr->HbmNumChPerMc)) {
    DEBUG((DEBUG_ERROR, "CNVT_HBM_MC_GLOBAL assert on access to mc >= MAX_HBM_IMC\n"));
    ASSERT(0);
  }

  GlobalAddress = SPR_GLOBAL_ADDR_HBMMC + SPR_GLOBAL_ADDR_HBMMC_RANGE * McId + SPR_GLOBAL_ADDR_HBMMC_MISC;

  return (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Data & 0xfff));
}

// 28. SMEE
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type SMEE device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_SMEE (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32 MmioRegion;
  UINT8  McId;
  UINT32 MemoryBar;
  UINT8  StationId;
  UINTN  PostedMethodMask;

  if (InstType == CsrBoxInst) {
    ASSERT((InstId % CpuCsrAccessVarPtr->DdrNumChPerMc) < 3);
    MmioRegion = MMIO_REGION_SMEE_CH0_SPR + (InstId % CpuCsrAccessVarPtr->DdrNumChPerMc);
    McId = InstId / CpuCsrAccessVarPtr->DdrNumChPerMc;

    PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

    MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;
    StationId = (CsrOffset.Data & 0x0fff) >> 8;

    MmioRegion |= MemoryBar;
    return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
  } else {
    DEBUG (( DEBUG_ERROR, "Invalid SMEE Instance Id: %x \n", InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }
}

// 38. DRA
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type DRA_MAIN device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_DRA_MAIN (
  IN UINT8               SocketId,
  IN UINT8               InstId,
  IN CSR_OFFSET          CsrOffset,
  IN CPU_CSR_ACCESS_VAR* CpuCsrAccessVarPtr,
  IN UINT8               InstType
  )
{
  UINT32 MmioRegion;
  UINT8  McId;
  UINT32 MemoryBar;
  UINT8  StationId;
  UINT8  BoxInst;
  UINTN  PostedMethodMask;

  BoxInst = 0;
  McId    = 0;

  switch (InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  default:
    DEBUG ((DEBUG_ERROR, "CNVT_DRA_MAIN assert on InstType not init !\n"));
    ASSERT (0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG ((DEBUG_ERROR, "CNVT_DRA_MAIN assert on access to mc >= MAX_IMC\n"));
    ASSERT (0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  MmioRegion = MMIO_REGION_DRA;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;

  StationId = (CsrOffset.Data & 0xfff) >> 8;
  if ((InstId % MAX_MC_CH) == 1) {  // Should add offset for channel 1
    StationId |= STATION_ID_OFFSET_DRA_CH1;
  }

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (StationId << 8) | (CsrOffset.Bits.offset & 0xff) | PostedMethodMask);
}

// 39. MCCADB
/**

  Convert to physical address for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCCADB device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MCCADB(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32 MmioRegion;
  UINT8  McId;
  UINT32 MemoryBar;
  UINT8  BoxInst;
  UINT8  ChId;
  UINTN  PostedMethodMask;

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = ChId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_MCCADB assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_MCCADB assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  // #define MMIO_REGION_MCCADB_CH0  0x07
  // #define MMIO_REGION_MCCADB_CH1  0x0A
  MmioRegion = MMIO_REGION_MCCADB_CH0 + (BoxInst % MAX_MC_CH) * 0x03;
  if ((((CsrOffset.Bits.offset) & 0x8000) == MMIO_REGION_MCCADB_CH0_SC1) || (((CsrOffset.Bits.offset) & 0xB000) == MMIO_REGION_MCCADB_CH1_SC1)) {
    MmioRegion = MmioRegion + 1;
  }
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (CsrOffset.Bits.offset & 0xfff) | PostedMethodMask);
}

// 40. MCCPGC
/**

  Convert to physical address for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MCCPGC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MCCPGC(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32 MmioRegion;
  UINT8  McId;
  UINT32 MemoryBar;
  UINT8  BoxInst;
  UINT8  ChId;
  UINTN  PostedMethodMask;
  UINTN  PhysicalAddress;

  HbmCompat(CNVT_HBM_CPGC, SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr, InstType, &PhysicalAddress);
  if (PhysicalAddress != MAX_UINTN) {
    return PhysicalAddress;
  }

  BoxInst = 0;
  McId    = 0;
  ChId    = 0;

  switch(InstType) {
  case  CsrBoxInst:
    BoxInst = InstId;
    McId = BoxInst / MAX_MC_CH;
    break;
  case  CsrMcId:
    McId = InstId;
    BoxInst = McId * MAX_MC_CH;
    break;
  case  CsrChId:
    ChId = InstId;
    McId = ChId / MAX_MC_CH;
    BoxInst = ChId;
    break;
  default:
    DEBUG((DEBUG_ERROR, "CNVT_MCCPGC assert on InstType not init !\n"));
    ASSERT(0);
    break;
  }
  if (McId >= CpuCsrAccessVarPtr->DdrMaxImc)  {
    DEBUG((DEBUG_ERROR, "CNVT_MCCPGC assert on access to mc >= MAX_IMC\n"));
    ASSERT(0);
  }

  PostedMethodMask = GetPostedMethodMask (CpuCsrAccessVarPtr);

  // #define MMIO_REGION_MCCPGC_CH0  0x09
  // #define MMIO_REGION_MCCPGC_CH1  0x0C
  MmioRegion = MMIO_REGION_MCCPGC_CH0 + (BoxInst % MAX_MC_CH) * 0x03;
  MemoryBar = (CpuCsrAccessVarPtr->MmioBar[SocketId][McId + TYPE_MEM_BAR0]) >> 12;

  MmioRegion |= MemoryBar;
  return (UINTN)((MmioRegion << 12) | (CsrOffset.Bits.offset & 0xfff) | PostedMethodMask);
}

//  42. BOX_SB_I3C
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type BOX_SB_I3C device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId

  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_I3C (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32                     GlobalAddress;
  UINTN                      PhysicalAddress;

  GlobalAddress = 0;

  if ((InstType != CsrBoxInst) || (InstId >= MAX_I3C_INSTANCE)) {
    DEBUG (( DEBUG_ERROR, "Invalid I3C Instance Id: %x \n", InstId));
    ASSERT (FALSE);
    return CONVERT_FAILED;
  }

  if (InstId == I3C_INSTANCE_0) {
    GlobalAddress = I3C_INSTANCE_0_SCF_BAR_GLOBAL_ADDR;
  } else {
    GlobalAddress = I3C_INSTANCE_1_SCF_BAR_GLOBAL_ADDR;
  }

  PhysicalAddress = (UINTN) (CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Bits.offset & 0xfff));

  return PhysicalAddress;
}
