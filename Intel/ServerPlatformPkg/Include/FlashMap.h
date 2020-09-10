/** @file
  Platform specific flash layout, All PCD's are defined in .fdf file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _FLASH_MAP_H_
#define _FLASH_MAP_H_


#define FIXED_FLASH_BLOCK_SIZE     FixedPcdGet32(PcdFlashBlockSize)



// APTIOV_SERVER_OVERRIDE_RC_START : Added to resolve the redefinition error
#ifndef FLASH_BASE
#define FLASH_BASE                                                          PcdGet32(PcdBiosBase)
#endif
#ifndef FLASH_SIZE
#define FLASH_SIZE                                                          PcdGet32(PcdBiosSize)
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Added to resolve the redefinition error.
#define FLASH_SIZE_FIXED                                                    FixedPcdGet32(PcdBiosSize)

// PEI FV definitions

#define FLASH_REGION_FV_IBB2_OFFSET                                         FixedPcdGet32(PcdFlashFvIbb2Offset)

#define FLASH_REGION_FV_MRC_NORMAL_BASE                                     FixedPcdGet32(PcdFlashFvMrcNormalBase)
#define FLASH_REGION_FV_MRC_NORMAL_SIZE                                     FixedPcdGet32(PcdFlashFvMrcNormalSize)
#define FLASH_REGION_FV_MRC_BASE                                            FixedPcdGet32(PcdFlashFvMrcBase)
#define FLASH_REGION_FV_MRC_SIZE                                            FixedPcdGet32(PcdFlashFvMrcSize)
#define FLASH_REGION_FV_PRE_MEM_BASE                                        FixedPcdGet32(PcdFlashFvPreMemBase)
#define FLASH_REGION_FV_PRE_MEM_SIZE                                        FixedPcdGet32(PcdFlashFvPreMemSize)

#define FLASH_REGION_MICROCODE_FV_BASE                                      FixedPcdGet32(PcdFlashNvStorageMicrocodeBase)
#define FLASH_REGION_MICROCODE_FV_SIZE                                      FixedPcdGet32(PcdFlashNvStorageMicrocodeSize)

// no #defines for BG NV storage

// No #defines for Policy

// No #defines for PEI ACM base

// No #defines for PEI ACM Oem base

#define FLASH_REGION_FV_SECPEI_BASE                                         FixedPcdGet32(PcdFlashFvSecPeiBase)
#define FLASH_REGION_FV_SECPEI_SIZE                                         FixedPcdGet32(PcdFlashFvSecPeiSize)


// Main FV definitions


#define FLASH_REGION_FVMAIN_BASE                                            FixedPcdGet32(PcdFlashFvMainBase)
#define FLASH_REGION_FVMAIN_SIZE                                            FixedPcdGet32(PcdFlashFvMainSize)

#define FLASH_REGION_FVMAIN_BASE_CPX                                        FixedPcdGet32(PcdFlashFvMainBaseCpx)
#define FLASH_REGION_FVMAIN_SIZE_CPX                                        FixedPcdGet32(PcdFlashFvMainSizeCpx)

#define FLASH_REGION_OEM_FV_BASE                                            FixedPcdGet32(PcdFlashFvOemLogoBase)
#define FLASH_REGION_OEM_FV_SIZE                                            FixedPcdGet32(PcdFlashFvOemLogoSize)


#define FLASH_REGION_FVWHEA_BASE                                            FixedPcdGet32(PcdFlashFvWheaBase)
#define FLASH_REGION_FVWHEA_SIZE                                            FixedPcdGet32(PcdFlashFvWheaSize)


#define FLASH_REGION_RUNTIME_UPDATABLE_BASE                                 FixedPcdGet32(PcdFlashNvStorageVariableBase)
#define FLASH_REGION_RUNTIME_UPDATABLE_SIZE                                 FixedPcdGet32(PcdFlashNvStorageVariableSize)

#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_VARIABLE_STORE_BASE     FixedPcdGet32(PcdFlashNvStorageVariableBase)
#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_VARIABLE_STORE_SIZE     FixedPcdGet32(PcdFlashNvStorageVariableSize)

#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_EVENT_LOG_BASE          FixedPcdGet32(PcdFlashFvNvStorageEventLogBase)
#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_EVENT_LOG_SIZE          FixedPcdGet32(PcdFlashFvNvStorageEventLogSize)

#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_FTW_WORKING_BASE        FixedPcdGet32(PcdFlashNvStorageFtwWorkingBase)
#define FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_FTW_WORKING_SIZE        FixedPcdGet32(PcdFlashNvStorageFtwWorkingSize)

#define FLASH_REGION_NV_FTW_SPARE_BASE                                      FixedPcdGet32(PcdFlashNvStorageFtwSpareBase)
#define FLASH_REGION_NV_FTW_SPARE_SIZE                                      FixedPcdGet32(PcdFlashNvStorageFtwSpareSize)

#define FLASH_REGION_BIOS_ACM_FV_BASE                                       FixedPcdGet32(PcdFlashFvAcmRegionBase)
#define FLASH_REGION_BIOS_ACM_FV_SIZE                                       FixedPcdGet32(PcdFlashFvAcmRegionSize)

#define FLASH_REGION_BIOS_ACM_BASE                                          FixedPcdGet32(PcdFlashFvAcmBase)
#define FLASH_REGION_BIOS_ACM_SIZE                                          FixedPcdGet32(PcdFlashFvAcmSize)

#define FLASH_REGION_FV_POST_MEMORY_BASE                                     FixedPcdGet32(PcdFlashFvPostMemoryBase)
#define FLASH_REGION_FV_POST_MEMORY_SIZE                                     FixedPcdGet32(PcdFlashFvPostMemorySize)

#define FLASH_REGION_FV_FSPS_BASE                                           FixedPcdGet32(PcdFlashFvFspSBase)
#define FLASH_REGION_FV_FSPS_SIZE                                           FixedPcdGet32(PcdFlashFvFspSSize)

#define FLASH_REGION_FV_FSPSCPX_BASE                                        FixedPcdGet32(PcdFlashFvFspSCpxBase)
#define FLASH_REGION_FV_FSPSCPX_SIZE                                        FixedPcdGet32(PcdFlashFvFspSCpxSize)






#endif // #ifndef _FLASH_MAP_H_

