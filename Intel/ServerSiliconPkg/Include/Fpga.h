/** @file

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

#ifndef _FPGA_DATA_H_
#define _FPGA_DATA_H_

#include <MaxSocket.h>

// Max FPGA Bit Streams, for all arrays, index loops,...
// Note: All of the arrays are of "UINT8", and 0xff is illegal marker,
//       So if this grows beyond that value, then the arrays will need refactoring.
#define FPGA_MAX_BITSTREAMS     0x20

// Max Sockets for FPGA's, for all arrays, index loops,...
// Note: all bit mask arrays are defined as "UINT8", so if this increases
//       Those will have to be refactored to hold the new data.
#define FPGA_MAX_SOCKET         MAX_SOCKET

// Max total Languages in FV Meta data, for all arrays, index loops,...
#define FPGA_FV_MAX_LANGUAGES   8

// Max total Platforms in FV Meta data, for all arrays, index loops,...
#define FPGA_FV_MAX_PLATFORMS   32

// Number of times to retry loading on fail
#define FPGA_MAX_LOADER_RETRIES 3

// Entries in BS GUID index array
#define FPGA_GBE_GUID_INDEX  0x00
#define FPGA_PCIE_GUID_INDEX 0x01
#define FPGA_NULL_GUID_INDEX 0xff

// Default Invalid Thermal Threshold
#define FPGA_INVALID_TH      0xff

// Resouce Allocation
#define FPGA_PREAllOCATE_BUS_NUM  0x02
#define FPGA_PREALLOCATE_MEM_SIZE 0x400000
// On FPGA, the VTD BAR is under FPGA device and require root bridge to decode. Root bridge decode is 1M Alignment.
#define FPGA_PREALLOCATE_VTD_SIZE 0x100000

//
// Major Checkpoint progress indicators written to debug port
//
#define STS_FPGA_LOADER_INIT              0x90
#define STS_FPGA_LOADER_PROGRAM_START     0x91
#define STS_FPGA_LOADER_PROGRAM_FAIL      0x92
#define STS_FPGA_DXE_ENTRY                0x93
#define STS_FPGA_DXE_CALLBACK             0x94
#define STS_FPGA_HIDE_PCIE_ROOT_PORT      0x95

#endif
