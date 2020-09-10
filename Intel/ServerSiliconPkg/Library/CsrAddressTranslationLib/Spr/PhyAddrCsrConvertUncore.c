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
extern CONST CHAR8 INVALID_FUN_INSTANCE_MESSAGE[];
extern CONST UINT8 mDeviceFunctionTable[MAX_BOX_TYPES][MAX_FUNCBLK];

//
// UPIMDF Instance ID to PCI BDF's Device & Function mapping
//
UINT8    UpimdfDevMapping[MAX_UPIMDF_INSTANCE_SPRUCC] = {9, 10, 11, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30};
#define  UPIMDF_KTI_LLPMON_FUN      0         // Link/Phy0.DecodingBridge.UPIMDF
#define  UPIMDF_KTI_CIOPHYDFX_FUN   1         // DFX0.DecodingBridge.UPIMDF


/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA-MISC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CHA_MISC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;

  if ((InstId >= MAX_SPR_CHA) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }
  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId] + 1;
  Dev = (mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3)+(InstId/8) ;
  Fun = InstId % 8;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

//  1. CHA-PMA

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA-PMA device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CHA_PMA (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  return CNVT_CHA_MISC(SocketId, InstId, CsrOffset, CpuCsrAccessVarPtr,InstType);
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type CHA-ABC device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_CHA_ABC (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;

  if ((InstId != 0) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId] + 1;
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type PCU device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_PCU(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;

  if ((InstId >= MAX_ALL_PCU) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId] + 1;
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

//  6. M2MEM
//
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type M2MEM device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_M2MEM(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT8   MaxM2mem;
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;

  MaxM2mem = MAX_SPR_M2MEM;
  if (CpuCsrAccessVarPtr->HbmSku) {
    MaxM2mem = MAX_SPR_HBM_M2MEM;
  }
  if ((InstId >= MaxM2mem) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId];
  if (InstId < MAX_SPR_M2MEM) {
    Dev = ((mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3) + InstId);
    Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7;
  } else {
    Dev = (mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3) + (InstId / 4) - 1;
    Fun = (mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7) + (InstId % 4) + 1;
  }

  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type KTI device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_KTI(
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

  if ((InstType != CsrBoxInst) ||
      ((InstId >= (CpuCsrAccessVarPtr->KtiPortCnt) && (InstId < UPIMDF_INST_OFFSET)) ||
       (InstId >= (UPIMDF_INST_OFFSET + MAX_UPIMDF_INSTANCE_SPRUCC)))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId];
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7;
  if (InstId < (CpuCsrAccessVarPtr->KtiPortCnt)) {
    Dev = (mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3) + InstId;
  } else {
    //
    // UPIMDF UPI link layer
    //   Link/Phy0.DecodingBridge.UPIMDF Func 0 --- standard Link/Phy0 Func 1
    //   DFX0.DecodingBridge.UPIMDF Func 1      --- standard DFX0 Func 3
    //
    Dev = UpimdfDevMapping[InstId - UPIMDF_INST_OFFSET];
    if (Fun == 1) {
      Fun = UPIMDF_KTI_LLPMON_FUN;         // Link/Phy0.DecodingBridge.UPIMDF
    } else if (Fun == 3) {
      Fun = UPIMDF_KTI_CIOPHYDFX_FUN;      // DFX0.DecodingBridge.UPIMDF
    } else {
      DEBUG (( DEBUG_ERROR, INVALID_FUN_INSTANCE_MESSAGE, CsrOffset.Bits.funcblk));
      ASSERT(FALSE);
      return CONVERT_FAILED;
    }
  }
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type M3KTI device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_M3KTI(
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

  if ((InstType != CsrBoxInst) ||
      ((InstId >=  MAX_M3KTI) && (InstId < UPIMDF_INST_OFFSET)) ||
       (InstId >= (UPIMDF_INST_OFFSET + MAX_UPIMDF_INSTANCE_SPRUCC))) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId];
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] & 0x7;
  if (InstId < MAX_M3KTI) {
    Dev = (mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3) + InstId;
  } else {
    //
    // UPIMDF UPI Protocol layer
    //    UPIMDF M2UPI Func: 2  ---- standard M2UPI Func 0
    //    UPIMDF PMON0 Func: 3  ---- standard PMON0 Func 1
    //    UPIMDF PMON1 Func: 4  ---- standard PMON1 Func 2
    //
    Dev = UpimdfDevMapping[InstId - UPIMDF_INST_OFFSET];
    Fun += 2;
  }
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}


/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type UBOX device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_UBOX(
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

  if ((InstId >= MAX_ALL_UBOX) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

EFI_STATUS
GetBusNumberIehSatelliteCnvt (
  IN UINT8               SocketId,
  IN UINT8               BoxType,
  IN UINT8               BoxInst,
  IN CPU_CSR_ACCESS_VAR *CpuCsrAccessVarPtr,
  OUT UINT8 *BusNumber
  )
{
  UINT8 Stack = 0;

  Stack = IIO_STACK0 + BoxInst;

  if ((Stack >= 0) && (Stack < MAX_IIO_STACK)) {
    *BusNumber = CpuCsrAccessVarPtr->StackBus[SocketId][Stack];
  } else {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, BoxType, BoxInst));
    ASSERT(FALSE);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;

}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IEH_SATELLITE device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IEH_SATELLITE(
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
  EFI_STATUS Status;

  if ((InstId >= MAX_ALL_IEH) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Status = GetBusNumberIehSatelliteCnvt (SocketId, (UINT8)CsrOffset.Bits.boxtype, InstId, CpuCsrAccessVarPtr, (UINT8*)&Bus);
  if (EFI_ERROR (Status)) {
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type IEH_GLOBAL device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_IEH_GLOBAL(
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

  if ((InstId >= MAX_ALL_IEH) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[SocketId];
  Dev = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk] >> 3;
  Fun = mDeviceFunctionTable[CsrOffset.Bits.boxtype][CsrOffset.Bits.funcblk]& 0x7;
  return (UINTN)(CpuCsrAccessVarPtr->mmCfgBaseL[SocketId] | PCIE_REG_ADDR(Bus, Dev, Fun, CsrOffset.Bits.offset));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MS2IDI_MAIN device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_MS2IDI_MAIN(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;

  GlobalAddress = 0;
  if (InstType != CsrBoxInst) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (InstId == MS2IDI_MULTICAST) {
    GlobalAddress = 0xFF000;                      // Multicast case, address is 1020K (0xFF000)
  } else {
    //
    // Note: there are MS2IDI.SCF_IOCOH0 ~ MS2IDI.SCF_IOCOH11 (start from 0x60000) on SPR. If
    // need to access it, then the GlobalAddress should be updated for them
    //
    GlobalAddress = 0x1000 * InstId;
  }
  return (UINTN)(CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Data & 0xfff));
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type SPK device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_SPK(
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
  ASSERT(FALSE);
  return CONVERT_FAILED;
}

/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type MDFIS device.

  @param[in] SocketId             The Socket ID
  @param[in] InstId               Box Instance, 0 based
  @param[in] CsrOffset            Format of CSR register offset
  @param[in] CpuCsrAccessVarPtr   A pointer to the CSR access variable
  @param[in] InstType             The Box Instance Type: CsrBoxInst or CsrChId

  @retval                         The physical address converted from USRA address


**/
UINTN
EFIAPI
CNVT_MDFIS (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;

  GlobalAddress = 0;
  if (InstType != CsrBoxInst) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  if (InstId == MDFIS_MDFS_MULTICAST) {
    GlobalAddress = 0xFB000;    // MDFIS.MDFS_Multicast: 0xFB000
  } else if (InstId == SBO_MDFS_MULTICAST) {
    GlobalAddress = 0xFA000;    // SBo.MDFS_Multicast:   0xFA000
  } else if (InstId < MDFIS_RESERVED_INST_OFFSET) {
    GlobalAddress = 0xAC000 + 0x1000 * InstId; // Instance 0 - 39 start from 0xAC000, each has 4K size
  } else if ((InstId < MDFIS_RESERVED1_INST_OFFSET) && (InstId >= UPIMDF_INST_OFFSET)) {
    //
    // Instance 0x40~0x57 for UPIMDF MDFIS IP (SPR UCC), start from 0xDD000, each has 4K size
    //
    GlobalAddress = 0xdd000 + 0x1000 * (InstId - UPIMDF_INST_OFFSET);
  }

  return (UINTN)(CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Data & 0xfff));
}

//  45. BOX_S3M
/**

  Convert to physical address a for given SocketId, InstId, CsrOffset & CpuCsrAccessVarPtr
  for Box type BOX_S3M device.

  @param SocketId            - The Socket ID
  @param InstId              - Box Instance, 0 based
  @param CsrOffset           - Format of CSR register offset
  @param CpuCsrAccessVarPtr  - A pointer to the CSR access variable
  @param InstType            - The Box Instance Type: CsrBoxInst,CsrMcId,CsrChId
  @retval                      The physical address converted from USRA address

**/
UINTN
EFIAPI
CNVT_S3M (
  IN UINT8                   SocketId,
  IN UINT8                   InstId,
  IN CSR_OFFSET              CsrOffset,
  IN CPU_CSR_ACCESS_VAR*     CpuCsrAccessVarPtr,
  IN UINT8                   InstType
  )
{
  UINT32   GlobalAddress;

  GlobalAddress = 0xDC000;
  if ((InstId != 0) || (InstType != CsrBoxInst)) {
    DEBUG (( DEBUG_ERROR, INVALID_BOX_INSTANCE_MESSAGE, CsrOffset.Bits.boxtype, InstId));
    ASSERT(FALSE);
    return CONVERT_FAILED;
  }

  return (UINTN)(CpuCsrAccessVarPtr->MmioBar[SocketId][TYPE_SCF_BAR] | GlobalAddress | (CsrOffset.Data & 0xfff));
}