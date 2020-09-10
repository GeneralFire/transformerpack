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
#include <MemHostChipCommon.h>
#include <Library/CxlIpLib.h>
#include <IndustryStandard/Cxl.h>
#include <Library/CxlMemLib.h>
#include <Include/CxlMemInternal.h>
#include <Library/RcDebugLib.h>
#include <Library/KtiApi.h>
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>


UINT16 DvsecSizeHighRegsOffset[MAX_CXL_HDM_RANGES] = {
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange1SizeHigh),
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange2SizeHigh)
};

UINT16 DvsecSizeLowRegsOffset[MAX_CXL_HDM_RANGES] = {
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange1SizeLow),
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange2SizeLow)
};

UINT16 DvsecBaseHighRegsOffset[MAX_CXL_HDM_RANGES] = {
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange1BaseHigh),
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange2BaseHigh)
};

UINT16 DvsecBaseLowRegsOffset[MAX_CXL_HDM_RANGES] = {
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange1BaseLow),
  OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceRange2BaseLow)
};


/**
  This function gets the data of DVSEC registers related to the specified CXL memory device.

  @param[in]  ScktId                Socket index.
  @param[in]  StackId               Stack index on socket.
  @param[out] DVSEC_REGISTERS       Information data of DVSEC registers.

  @retval EFI_SUCCESS               This function gets the register data successfully.
  @retval EFI_UNSUPPORTED           The CXL device does not contain CXL memory device.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.
**/
EFI_STATUS
GetCxlMemDeviceRegData (
  IN  UINT8                 ScktId,
  IN  UINT8                 StackId,
  OUT CXL_MEM_DVSEC_REGS    *CxlMemRegs
  )
{
  UINT8                     HdmId;
  EFI_STATUS                Status;

  if (CxlMemRegs == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (GetCxlStatus (ScktId, StackId) != AlreadyInCxlMode) {
    RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, CXL device not found\n", ScktId, StackId);
    return EFI_UNSUPPORTED;
  }

  //
  // Get the DVSEC register data of CXL memory device from CXL device CSRs.
  //
  Status = CxlDeviceDvsecRegisterAccess (
             ScktId,
             StackId,
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceCapability),
             UsraWidth16,
             CxlOperationRead,
             &CxlMemRegs->CapabilityReg
             );

  for (HdmId = 0; HdmId < MAX_CXL_HDM_RANGES; HdmId++) {
    Status |= CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecSizeHighRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationRead,
                &CxlMemRegs->SizeHighReg[HdmId]
                );
    Status |= CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecSizeLowRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationRead,
                &CxlMemRegs->SizeLowReg[HdmId]
                );
  }

  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to read the DVSEC registers to get the information of CXL memory device\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function gets the identifiers (S/N and VID pair) of the specified CXL device.

  @param[in]  ScktId                Socket index.
  @param[in]  StackId               Stack index on socket.
  @param[out] SerialNumber          Pointer to serial number data.
  @param[out] VendorId              Pointer to vendor ID.

  @retval EFI_SUCCESS               This function gets the identifiers successfully.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.
**/
EFI_STATUS
GetCxlDeviceIdentifiers (
  IN  UINT8                 ScktId,
  IN  UINT8                 StackId,
  OUT UINT64                *SerialNumber,
  OUT UINT16                *VendorId
  )
{
  if ((SerialNumber == NULL) || (VendorId == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the serial number and vendor ID from the CXL endpoint device.
  //
  *SerialNumber = GetCxlDevSerialNum (ScktId, StackId);
  *VendorId     = GetCxlDevVid (ScktId, StackId);

  return EFI_SUCCESS;
}

/**
  This function enables CXL.mem protocol operation on CXL device, CXL Downstream Port and CXL Upstream Port.

  @param[in] ScktId                Socket index.
  @param[in] StackId               Stack index on socket.

  @retval EFI_SUCCESS              This function programs the DVSEC registers successfully.
  @retval EFI_UNSUPPORTED          DVSEC register programming is not supported.
**/
EFI_STATUS
EnableCxlMemProtocol (
  IN UINT8                    ScktId,
  IN UINT8                    StackId
  )
{
  EFI_STATUS                          Status;
  CXL_DVSEC_FLEX_BUS_DEVICE_CONTROL   CxlDeviceControl;
  CXL_1_1_DVSEC_FLEX_BUS_PORT_CONTROL CxlPortControl;

  //
  // Enable CXL.mem protocol operation on CXL Device.
  //
  CxlDeviceControl.Uint16 = 0;
  CxlDeviceControl.Bits.MemEnable = 1;
  Status = CxlDeviceDvsecRegisterAndThenOr16 (
             ScktId,
             StackId,
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceControl),
             ~CxlDeviceControl.Uint16,
             CxlDeviceControl.Uint16
             );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to enable CXL.mem protocol operation on CXL Device\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  //
  // Enable CXL.mem protocol operation on CXL Downstream Port.
  //
  CxlPortControl.Uint16 = 0;
  CxlPortControl.Bits.MemEnable = 1;
  Status = CxlPortDvsecRegisterAndThenOr16 (
             ScktId,
             StackId,
             FALSE,    // Downstream port
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_PORT, PortControl),
             ~CxlPortControl.Uint16,
             CxlPortControl.Uint16
             );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to enable CXL.mem protocol operation on CXL Downstream Port\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  //
  // Enable CXL.mem protocol operation on CXL Upstream Port.
  //
  Status = CxlPortDvsecRegisterAndThenOr16 (
             ScktId,
             StackId,
             TRUE,    // Upstream port
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_PORT, PortControl),
             ~CxlPortControl.Uint16,
             CxlPortControl.Uint16
             );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to enable CXL.mem protocol operation on CXL Upstream Port\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function configures the base addresses of HDM ranges for CXL device.

  @param[in] ScktId                Socket index.
  @param[in] StackId               Stack index on socket.
  @param[in] CxlMemDevConfig       Configuration information of CXL memory device.

  @retval EFI_SUCCESS              This function programs the DVSEC registers successfully.
  @retval EFI_UNSUPPORTED          DVSEC register programming is not supported.
**/
EFI_STATUS
ConfigureCxlMemBaseAddr (
  IN UINT8                    ScktId,
  IN UINT8                    StackId,
  IN CXL_MEM_DEVICE_CONFIG    *CxlMemDevConfig
  )
{
  UINT8                                       HdmId;
  UINT32                                      MemBaseAddrLow;
  UINT32                                      MemBaseAddrHigh;
  EFI_STATUS                                  Status;
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_HIGH  BaseHighRegs[MAX_CXL_HDM_RANGES];
  CXL_DVSEC_FLEX_BUS_DEVICE_RANGE1_BASE_LOW   BaseLowRegs[MAX_CXL_HDM_RANGES];

  for (HdmId = 0; HdmId < MAX_CXL_HDM_RANGES; HdmId++) {
    if (CxlMemDevConfig->CxlMemConfig[HdmId].BaseAddress == 0) {
      continue;    // Base address of HDMs will not be mapped to zero.
    }

    MemBaseAddrLow  = (UINT32) RShiftU64 ((UINT32) CxlMemDevConfig->CxlMemConfig[HdmId].BaseAddress, CONVERT_B_TO_256MB_GRAN);
    MemBaseAddrHigh = (UINT32) RShiftU64 (CxlMemDevConfig->CxlMemConfig[HdmId].BaseAddress, 32);
    RcDebugPrint (SDBG_MAX, "  Range %d: BaseAddrHigh = 0x%x, BaseAddrLow = 0x%x(256MB)\n", HdmId, MemBaseAddrHigh, MemBaseAddrLow);

    //
    // Read DVSEC Flex Bus range base high and low registers.
    //
    Status = CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecBaseHighRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationRead,
                &BaseHighRegs[HdmId].Uint32
                );
    Status |= CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecBaseLowRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationRead,
                &BaseLowRegs[HdmId].Uint32
                );
    if (EFI_ERROR (Status)) {
      RcDebugPrint (SDBG_MAX,
        "CXL: Socket %d Stack %d, Failed to read DVSEC registers to configure the CXL memory base addresses\n",
        ScktId,
        StackId
        );
      return EFI_UNSUPPORTED;
    }

    //
    // Update memory base address.
    //
    // The register fields of MEMORY_BASE_LOW/HIGH of range 1 and 2 are the same
    BaseLowRegs[HdmId].Bits.MemoryBaseLow = MemBaseAddrLow;
    BaseHighRegs[HdmId].Uint32            = MemBaseAddrHigh;

    //
    // Write DVSEC Flex Bus range base high and low registers.
    //
    Status = CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecBaseHighRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationWrite,
                &BaseHighRegs[HdmId].Uint32
                );
    Status |= CxlDeviceDvsecRegisterAccess (
                ScktId,
                StackId,
                DvsecBaseLowRegsOffset[HdmId],
                UsraWidth32,
                CxlOperationWrite,
                &BaseLowRegs[HdmId].Uint32
                );
    if (EFI_ERROR (Status)) {
      RcDebugPrint (SDBG_MAX,
        "CXL: Socket %d Stack %d, Failed to write DVSEC registers to configure the CXL memory base addresses\n",
        ScktId,
        StackId
        );
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function programs the DVSEC registers for the specified CXL memory device.

  @param[in] ScktId                Socket index.
  @param[in] StackId               Stack index on socket.
  @param[in] CxlMemDevConfig       Configuration information of CXL memory device.

  @retval EFI_SUCCESS              This function programs the DVSEC registers successfully.
  @retval EFI_UNSUPPORTED          DVSEC register programming is not supported.
**/
EFI_STATUS
SetCxlMemDvsecRegs (
  IN UINT8                    ScktId,
  IN UINT8                    StackId,
  IN CXL_MEM_DEVICE_CONFIG    *CxlMemDevConfig
  )
{
  EFI_STATUS                      Status;
  CXL_DVSEC_FLEX_BUS_DEVICE_LOCK  LockRegs;

  //
  // Check if control register, memory base low register and memory base high register are writeable for CXL Device.
  //
  Status = CxlDeviceDvsecRegisterAccess (
             ScktId,
             StackId,
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceLock),
             UsraWidth16,
             CxlOperationRead,
             &LockRegs.Uint16
             );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to read the DVSEC Flex Bus Lock register\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  } else if (LockRegs.Bits.ConfigLock != 0) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to configure CXL memory device since the related DVSEC registers are locked\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  //
  // Configure base addresses of HDM ranges for CXL device.
  //
  Status = ConfigureCxlMemBaseAddr (ScktId, StackId, CxlMemDevConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Enable CXL.mem protocol operation.
  //
  Status = EnableCxlMemProtocol (ScktId, StackId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Lock control register, memory base low register and memory base high register for CXL device.
  //
  LockRegs.Bits.ConfigLock = 1;
  Status = CxlDeviceDvsecRegisterAccess (
             ScktId,
             StackId,
             OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceLock),
             UsraWidth16,
             CxlOperationWrite,
             &LockRegs.Uint16
             );
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX,
      "CXL: Socket %d Stack %d, Failed to lock control register and memory base address registers\n",
      ScktId,
      StackId
      );
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function programs the IP registers for the specified CXL memory device.

  @param[in] ScktId                Socket index.
  @param[in] StackId               Stack index on socket.
  @param[in] CxlMemDevConfig       Configuration information of CXL memory device.

  @retval EFI_SUCCESS              This function programs the IP registers successfully.
**/
EFI_STATUS
SetCxlMemIpRegs (
  IN UINT8                    ScktId,
  IN UINT8                    StackId,
  IN CXL_MEM_DEVICE_CONFIG    *CxlMemDevConfig
  )
{
  UINT8                                    HdmId;
  UINT64                                   DevMemBase;
  UINT64                                   DevMemLimit;
  MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_STRUCT MemCabCtrlStsReg;
  CLK_GATE_CTRL_IIO_CXL_CM_STRUCT          ClkGateCtrlReg;

  //
  // Program DEVICE_MEM_BASE and DEVICE_MEM_LIMIT registers with the base and limit of HDM ranges.
  //
  DevMemBase = (UINT64) -1;
  DevMemLimit = 0;
  for (HdmId = 0; HdmId < MAX_CXL_HDM_RANGES; HdmId++) {
    if (CxlMemDevConfig->CxlMemConfig[HdmId].BaseAddress == 0) {
      continue;    // Base address of HDMs will not be mapped to zero.
    }

    //
    // Locate the base and limit of the continuous HDM ranges for the specified CXL device.
    // DevMemBase <= Address <= DevMemLimit.
    //
    DevMemBase  = MIN (DevMemBase, CxlMemDevConfig->CxlMemConfig[HdmId].BaseAddress);
    DevMemLimit = MAX (DevMemLimit, CxlMemDevConfig->CxlMemConfig[HdmId].LimitAddress - 1);
  }

  if (DevMemBase >= DevMemLimit) {
    RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, Cannot locate CXL device memory range\n", ScktId, StackId);
    return EFI_UNSUPPORTED;
  }
  RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, DevMemBase = 0x%lx, DevMemLimit = 0x%lx\n", ScktId, StackId, DevMemBase, DevMemLimit);

  UsraCsrWrite (ScktId, StackId, DEVICE_MEM_BASE_N0_IIO_CXL_CM_REG, (UINT32) DevMemBase);
  UsraCsrWrite (ScktId, StackId, DEVICE_MEM_BASE_N1_IIO_CXL_CM_REG, (UINT32) RShiftU64 (DevMemBase, 32));
  UsraCsrWrite (ScktId, StackId, DEVICE_MEM_LIMIT_N0_IIO_CXL_CM_REG, (UINT32) DevMemLimit);
  UsraCsrWrite (ScktId, StackId, DEVICE_MEM_LIMIT_N1_IIO_CXL_CM_REG, (UINT32) RShiftU64 (DevMemLimit, 32));

  //
  // Program MEM_CAB_CONTROL_STATUS register to force the MEM CAB to de-active when CXL.mem is enabled.
  //
  MemCabCtrlStsReg.Data = UsraCsrRead (ScktId, StackId, MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_REG);
  MemCabCtrlStsReg.Bits.force_mem_cab_active = 0;
  MemCabCtrlStsReg.Bits.force_mem_cab_de_active = 1;
  UsraCsrWrite (ScktId, StackId, MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_REG, MemCabCtrlStsReg.Data);

  //
  // Program CLK_GATE_CTRL register to enable clock gating.
  //
  ClkGateCtrlReg.Data = UsraCsrRead (ScktId, StackId, CLK_GATE_CTRL_IIO_CXL_CM_REG);
  ClkGateCtrlReg.Bits.clock_gate_enable = 1;
  UsraCsrWrite (ScktId, StackId, CLK_GATE_CTRL_IIO_CXL_CM_REG, ClkGateCtrlReg.Data);

  return EFI_SUCCESS;
}

/**
  This function configures the specified CXL memory device.

  @param[in] ScktId                Socket index.
  @param[in] StackId               Stack index on socket.
  @param[in] CxlMemDevConfig       Configuration information of CXL memory device.

  @retval EFI_SUCCESS              This function configures the CXL memory device successfully.
  @retval EFI_UNSUPPORTED          The CXL device does not contain CXL memory device.
  @retval EFI_INVALID_PARAMETER    Some of input parameters are invalid.
**/
EFI_STATUS
EFIAPI
ConfigureCxlMemDeviceInfo (
  IN UINT8                    ScktId,
  IN UINT8                    StackId,
  IN CXL_MEM_DEVICE_CONFIG    *CxlMemDevConfig
  )
{
  EFI_STATUS                  Status;

  if ((ScktId >= MAX_SOCKET) || (StackId >= MAX_CXL_PER_SOCKET) || (CxlMemDevConfig == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, Configure CXL memory Device\n", ScktId, StackId);

  //
  // Program CXL.mem DVSEC registers.
  //
  Status = SetCxlMemDvsecRegs (ScktId, StackId, CxlMemDevConfig);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, Failed to program CXL.mem DVSEC registers\n", ScktId, StackId);
    return Status;
  }

  //
  // Program CXL.mem IP registers.
  //
  Status = SetCxlMemIpRegs (ScktId, StackId, CxlMemDevConfig);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX, "CXL: Socket %d Stack %d, Failed to program CXL.mem IP registers\n", ScktId, StackId);
    return Status;
  }

  return EFI_SUCCESS;
}
