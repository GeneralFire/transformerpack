/** @file
  CXL memory library register header.

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

#ifndef _CXL_MEM_INTERNAL_H_
#define _CXL_MEM_INTERNAL_H_



typedef struct {
  UINT16          CapabilityReg;                      // DVSEC Intel Flex Bus capability register.
  UINT32          SizeHighReg[MAX_CXL_HDM_RANGES];    // DVSEC Intel Flex Bus range 1~2 size high registers.
  UINT32          SizeLowReg[MAX_CXL_HDM_RANGES];     // DVSEC Intel Flex Bus range 1~2 size low registers.
} CXL_MEM_DVSEC_REGS;


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
  );

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
  );

#endif  // _CXL_MEM_INTERNAL_H_
