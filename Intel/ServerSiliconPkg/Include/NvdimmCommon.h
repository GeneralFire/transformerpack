/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

//
// NFIT NVDIMM handle has the following layout:
// Bit [3:0]   - DIMM
// Bit [7:4]   - Channel in Memory Controller
// Bit [11:8]  - Memory Controller
// Bit [15:12] - Socket
// Bit [27:16] - Node ID
// Bit [31:28] - reserved
// The macros below operate on NFIT handle.
//
#define NFIT_HANDLE_BUILD(Skt, Mc, Ch, Dimm) ((UINT32)(Skt) << 12 | (UINT32)(Mc) << 8 | (Ch) << 4 | (Dimm))
#define NFIT_HANDLE_TO_DIMM(Handle)          ((UINT8)(Handle) & 0x0F)
#define NFIT_HANDLE_TO_CH(Handle)            ((UINT8)((Handle) >> 4) & 0x0F)
#define NFIT_HANDLE_TO_MC(Handle)            ((UINT8)((Handle) >> 8) & 0x0F)
#define NFIT_HANDLE_TO_SKT(Handle)           ((UINT8)((Handle) >>12) & 0x0F)

#define NFIT_HANDLE_NONE ((UINT32)-1)
