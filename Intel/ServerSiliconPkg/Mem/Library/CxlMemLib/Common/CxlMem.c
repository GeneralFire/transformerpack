/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <MemHostChipCommon.h>
#include <Library/CxlIpLib.h>
#include <IndustryStandard/Cxl.h>
#include <Library/CxlMemLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Include/CxlMemInternal.h>
#include <Library/RcDebugLib.h>

/**
  This function gets the information of specified CXL memory device.
  Note: If CXL memory is not present in the CXL device, the API will still return EFI_SUCCESS,
  but the HdmCount in the CxlMemDevInfo is 0.

  @param[in]  ScktId                Socket index.
  @param[in]  StackId               Stack index on socket.
  @param[out] CxlMemDevInfo         Information data of CXL memory device.

  @retval EFI_SUCCESS               This function gets the information successfully.
  @retval EFI_UNSUPPORTED           This is not a valid CXL devices.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
GetCxlMemDeviceInfo (
  IN  UINT8                 ScktId,
  IN  UINT8                 StackId,
  OUT CXL_MEM_DEVICE_INFO   *CxlMemDevInfo
  )
{
  UINT8                                       HdmId;
  UINT64                                      MemSize;
  EFI_STATUS                                  Status;
  CXL_MEM_DVSEC_REGS                          CxlMemRegs;
  BOOLEAN                                     CxlCacheCapable;
  CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY        *CxlDeviceCapability;
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_LOW   *CxlDeviceSizeLow;
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_HIGH  *CxlDeviceSizeHigh;

  if ((ScktId >= MAX_SOCKET) || (StackId >= MAX_CXL_PER_SOCKET) || (CxlMemDevInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CxlMemRegs, sizeof (CxlMemRegs));
  ZeroMem (CxlMemDevInfo, sizeof (*CxlMemDevInfo));

  //
  // Get the DVSEC register data of CXL memory device.
  //
  Status = GetCxlMemDeviceRegData (ScktId, StackId, &CxlMemRegs);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the S/N and VID pair of CXL device.
  //
  Status = GetCxlDeviceIdentifiers (ScktId, StackId, &CxlMemDevInfo->SerialNumber, &CxlMemDevInfo->VendorId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  RcDebugPrint (SDBG_MAX,
    "CXL: Socket %d Stack %d, Serial number: %lx, Vendor ID: %x\n",
    ScktId,
    StackId,
    CxlMemDevInfo->SerialNumber,
    CxlMemDevInfo->VendorId
    );
  //
  // Check if there is CXL memory device found.
  //
  CxlDeviceCapability = (CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY*) &CxlMemRegs.CapabilityReg;
  if (CxlDeviceCapability->Bits.MemCapable == 0 || CxlDeviceCapability->Bits.HdmCount == 0) {
    RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, CXL memory device not found\n", ScktId, StackId);
    return EFI_SUCCESS;
  }
  CxlMemDevInfo->HdmCount       = (UINT8) CxlDeviceCapability->Bits.HdmCount;
  CxlMemDevInfo->MemHwInitMode  = (BOOLEAN) CxlDeviceCapability->Bits.MemHwInitMode;
  CxlCacheCapable               = (BOOLEAN) CxlDeviceCapability->Bits.CacheCapable;
  if ((CxlMemDevInfo->HdmCount > 0) && !CxlCacheCapable) {
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      RcDebugPrint (SDBG_MINMAX, "CXL: Socket %d Stack %d is memory expander and is not supported on SPR CPU\n", ScktId, StackId);
      return EFI_UNSUPPORTED;
    }
    CxlMemDevInfo->MemExpander = TRUE;
  }
  //
  // Parse the information data of CXL memory device.
  //
  for (HdmId = 0; HdmId < MAX_CXL_HDM_RANGES; HdmId++) {
    CxlDeviceSizeLow  = (CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_LOW*) &CxlMemRegs.SizeLowReg[HdmId];
    CxlDeviceSizeHigh = (CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_SIZE_HIGH*) &CxlMemRegs.SizeHighReg[HdmId];
    if (CxlDeviceSizeLow->Bits.MemoryInfoValid == 0) {
      continue;      // Continue if Range 1/2 Size High and Size Low registers are not valid.
    }
    MemSize = LShiftU64 (CxlDeviceSizeHigh->Bits.MemorySizeHigh, CONVERT_4GB_TO_64MB_GRAN) +
              LShiftU64 (CxlDeviceSizeLow->Bits.MemorySizeLow, CONVERT_256MB_TO_64MB_GRAN);

    CxlMemDevInfo->CxlMemInfo[HdmId].MemSize           = (UINT32) MemSize;
    CxlMemDevInfo->CxlMemInfo[HdmId].MediaType         = (UINT8) CxlDeviceSizeLow->Bits.MediaType;
    CxlMemDevInfo->CxlMemInfo[HdmId].DesiredInterleave = (UINT8) CxlDeviceSizeLow->Bits.DesiredInterleave;

    RcDebugPrint (SDBG_MAX,
      "  Range %d: MemSize = %d(64MB), MediaType = 0x%02x, DesiredInterleave = 0x%03x\n",
      HdmId,
      CxlMemDevInfo->CxlMemInfo[HdmId].MemSize,
      CxlMemDevInfo->CxlMemInfo[HdmId].MediaType,
      CxlMemDevInfo->CxlMemInfo[HdmId].DesiredInterleave
      );
  }
  return EFI_SUCCESS;
}

