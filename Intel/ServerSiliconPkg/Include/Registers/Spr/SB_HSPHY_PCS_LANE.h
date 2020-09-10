
/** @file
  SB_HSPHY_PCS_LANE.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>
  
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
  
  This file contains Silicon register definitions.
  
  This is a generated file; please do not modify it directly.
  
**/

/* The following security policy groups are used by registers in this file:     */

/* SPRA0 Security Policy Groups:                                                */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* HSPHY_GROUP_1                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | */
/*    MEM_CPL_SAI | VTD_SAI | HOSTCP_PMA_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI  */
/*    | PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HW_PCH_SAI | GT_PMA_SAI |        */
/*    DISPLAY_SAI | DISPLAY_KVM_SAI | CORE_EVENT_PROXY_SAI |                    */
/*    DEVICE_UNTRUSTED_SAI | RC_MORPHED_SAI | RCIOMMU_BYPASS_SAI |              */
/*    CSE_INTEL_SAI | OOB_MSM_SAI | GT2_SAI | DEVICE_UNTRUSTED_IAL_SAI |        */
/*    DEVICE_ABORT_SAI | DISPLAY2_SAI | DISPLAY3_SAI | OOB_MSM_UNTRUSTED_SAI |  */
/*    UNCORE_PMA_SAI | FXR_INTERNAL_SAI                                         */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _SB_HSPHY_PCS_LANE_h
#define _SB_HSPHY_PCS_LANE_h
#include <Base.h>
































































































































#endif /* _SB_HSPHY_PCS_LANE_h */
