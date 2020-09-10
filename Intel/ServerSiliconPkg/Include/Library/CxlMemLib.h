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

#ifndef _CXL_MEM_LIB_H_
#define _CXL_MEM_LIB_H_

//
// CXL Memory Data Structure Definitions
//
typedef struct {
  UINT32  MemSize;              // CXL memory size with 64MB granularity.
  UINT8   MediaType;            // Memory media characteristics, 000 - Volatile memory, 001 - Non-volatile memory, 111 - (DVSEC Range2 only) Not Memory.
  UINT8   DesiredInterleave;    // CXL memory desired interleave, 00 - No Interleave, 01 - 256 Byte Granularity, 10 - 4K Interleave.
} CXL_MEM_INFO;

typedef struct {
  UINT64                SerialNumber;                   // A unique serial number to identify CXL device.
  UINT16                VendorId;                       // Vendor ID of CXL device.
  UINT8                 HdmCount;                       // HDM count in the CXL device. 0 if the CXL devices does not have memory attached.
  BOOLEAN               MemHwInitMode;                  // Indicates this CXL.mem capable device initializes memory
  BOOLEAN               MemExpander;                    // Indicates this CXL.mem is memory expander device type
  CXL_MEM_INFO          CxlMemInfo[MAX_CXL_HDM_RANGES]; // CXL memory information.
} CXL_MEM_DEVICE_INFO;

typedef struct {
  UINT64  BaseAddress;          // Base address of CXL memory in bytes.
  UINT64  LimitAddress;         // Limit address of CXL memory in bytes.
} CXL_MEM_CONFIG;

typedef struct {
  CXL_MEM_CONFIG   CxlMemConfig[MAX_CXL_HDM_RANGES];    // CXL memory configuration data.
} CXL_MEM_DEVICE_CONFIG;


//
// CXL Memory API Declarations
//
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
  );

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
  );

#endif  // _CXL_MEM_LIB_H_
