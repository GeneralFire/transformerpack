/** @file
  Data format for Universal Data Structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef   __FPGA_SOCKET_CONFIGURATION_DATA_H__
#define   __FPGA_SOCKET_CONFIGURATION_DATA_H__

//#include <Base.h>
#include <Fpga.h>

extern EFI_GUID gFpgaSocketVariableGuid;
#define FPGA_SOCKET_CONFIGURATION_NAME L"FpgaSocketConfig"

#pragma pack(1)

typedef struct {

  // User Bitmap to enable the FPGA socket.
  UINT8    FpgaSetupEnabled;

  // for each socket enabled, use this Bit stream GUID Index
  // Note: variable is Index+ 1 for unused default to be 0
  UINT8    FpgaSocketGuid[FPGA_MAX_SOCKET];
  // FPGA Temperature Threshold 1/2: Max value clamped at 100 C;
  // i.e. if the SW tries to write value greater than 100 C, HW will automatically default to 100 C.
  UINT8    FpgaThermalTH1[FPGA_MAX_SOCKET];
  UINT8    FpgaThermalTH2[FPGA_MAX_SOCKET];

  // FPGA reserved data
  UINT8    FpgaReserved[14];
} FPGA_SOCKET_CONFIGURATION;
#pragma pack()

#endif

