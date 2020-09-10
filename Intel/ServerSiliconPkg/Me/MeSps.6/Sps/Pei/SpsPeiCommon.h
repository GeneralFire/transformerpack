/** @file
  This driver manages the initial phase of SPS ME firmware support specified in
  SPS ME-BIOS Interface Specification.

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

#ifndef _SPS_PEI_COMMON_H_
#define _SPS_PEI_COMMON_H_

#define SPS_INIT_TIMEOUT  2000000 // [us] Timeout when waiting for InitComplete
#define SPS_RESET_TIMEOUT 5000000 // [us] Timeout when waiting for InitComplete
#define STALL_1US         1
#define STALL_1MS         1000
#define STALL_1S          1000000

/**
  Finalize SPS Non-Functional flow

  @param[in] SpsPrefix     Pointer to SpsPrefix for debug info
*/
VOID
MeIsNonFunctional (
  IN CONST CHAR8            *SpsPrefix
  );

/**
  Create HOB with SPS info for DXE

  @param[in] SpsPrefix     Pointer to SpsPrefix for debug info
  @param[in] WorkFlow      The state of ME firmware observed at PEI
  @param[in] FeatureSet    ME features enabled reported in Get ME-BIOS Interface response
  @param[in] FeatureSet2   ME features enabled reported in Get ME-BIOS Interface response
  @param[in] PwrOptBoot    Whether NM in ME requests power optimized boot
  @param[in] Cores2Disable Whether NM in ME requests disabling cores

  @retval EFI_SUCCESS      The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES HOB creation failed
**/
EFI_STATUS
SpsHobCreate (
  IN CONST CHAR8            *SpsPrefix,
  IN UINT8                  WorkFlow,
  IN UINT32                 FeatureSet,
  IN UINT32                 FeatureSet2,
  IN BOOLEAN                PwrOptBoot,
  IN UINT8                  Cores2Disable
  );

/**
  SPS PEI Pre-Mem entry point.

  @param[in] FileHandle  PEIM file handle
  @param[in] PeiServices General purpose services available to every PEIM

  @retval EFI_SUCCESS    The function completed successfully.
**/
EFI_STATUS
SpsPeiPreMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

/**
  SPS PEI Post-Mem entry point.

  @param[in] FileHandle  PEIM file handle
  @param[in] PeiServices General purpose services available to every PEIM

  @retval EFI_SUCCESS    The function completed successfully.
**/
EFI_STATUS
SpsPeiPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

#endif // _SPS_PEI_CONMON_H_
