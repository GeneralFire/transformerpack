
/** @file
  CHA_PMA.h

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
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* LB_MCHECK_CHA_W                                                              */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* LB_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OS_W                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | SAI_Reserved_5 | SAI_Reserved_6 |  */
/*    SAI_Reserved_7 | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI | VTD_SAI */
/*    | PM_DIE_TO_DIE_SAI | OOB_MSM_UNTRUSTED_SAI | HOSTCP_PMA_SAI |            */
/*    CSE_INTEL_SAI | CSE_OEM_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |           */
/*    PECI_MSM_SAI | PM_IOSS_SAI | CSE_DNX_SAI | FXR_INTERNAL_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | SAI_Reserved_26 |       */
/*    IRC_SAI | NPK_SAI | DISPLAY2_SAI | DISPLAY3_SAI | HW_PCH_SAI |            */
/*    SAI_Reserved_32 | SAI_Reserved_33 | SAI_Reserved_34 | GT_PMA_SAI |        */
/*    HSP_SAI | SAI_Reserved_37 | SAI_Reserved_38 | SAI_Reserved_39 |           */
/*    UNCORE_PMA_SAI | RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI |              */
/*    DFX_THIRDPARTY_SAI | DISPLAY_SAI | SAI_Reserved_45 | SAI_Reserved_46 |    */
/*    DISPLAY_KVM_SAI | GT2_SAI | SAI_Reserved_49 | DEVICE_UNTRUSTED_IAL_SAI |  */
/*    SAI_Reserved_51 | CORE_EVENT_PROXY_SAI | DEVICE_ABORT_SAI |               */
/*    RCIOMMU_BYPASS_SAI | SAI_Reserved_55 | SAI_Reserved_56 | IE_CSE_SAI |     */
/*    SAI_Reserved_58 | SAI_Reserved_59 | CPM_SAI | OOB_MSM_SAI | XGBE_SAI |    */
/*    DEVICE_UNTRUSTED_SAI                                                      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                       */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI                      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE                                                                     */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _CHA_PMA_h
#define _CHA_PMA_h
#include <Base.h>

/* VID_UNICAST_GROUP1_CHA_PMA_REG supported on:                                 */
/*      SPRA0 (0x11f00000)                                                      */
/*      SPRB0 (0x11f00000)                                                      */
/*      SPRHBM (0x11f00000)                                                     */
/*      SPRC0 (0x11f00000)                                                      */
/*      SPRMCC (0x11f00000)                                                     */
/*      SPRUCC (0x11f00000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_UNICAST_GROUP1_CHA_PMA_REG 0x01010000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vendor_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* The value is assigned by PCI-SIG to Intel. */

  } Bits;
  UINT16 Data;

} VID_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* DID_UNICAST_GROUP1_CHA_PMA_REG supported on:                                 */
/*      SPRA0 (0x11f00002)                                                      */
/*      SPRB0 (0x11f00002)                                                      */
/*      SPRHBM (0x11f00002)                                                     */
/*      SPRC0 (0x11f00002)                                                      */
/*      SPRMCC (0x11f00002)                                                     */
/*      SPRUCC (0x11f00002)                                                     */
/* Register default value on SPRA0: 0x0000324C                                  */
/* Register default value on SPRB0: 0x0000324C                                  */
/* Register default value on SPRHBM: 0x0000324C                                 */
/* Register default value on SPRC0: 0x0000324C                                  */
/* Register default value on SPRMCC: 0x0000324C                                 */
/* Register default value on SPRUCC: 0x0000324C                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_UNICAST_GROUP1_CHA_PMA_REG 0x01010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000324C*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PCICMD_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x11f00004)                                                      */
/*      SPRB0 (0x11f00004)                                                      */
/*      SPRHBM (0x11f00004)                                                     */
/*      SPRC0 (0x11f00004)                                                      */
/*      SPRMCC (0x11f00004)                                                     */
/*      SPRUCC (0x11f00004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_UNICAST_GROUP1_CHA_PMA_REG 0x01010004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 io_space_enable : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont decode
                               any IO BARs
                            */
    UINT16 memory_space_enable : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont decode
                               any memory BARs
                            */
    UINT16 bus_master_enable : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices dont generate
                               any transactions
                            */
    UINT16 special_cycle_enable : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Not applicable. Hardwired to 0. */
    UINT16 memory_write_and_invalidate_enable : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal devices. Hardwired to
                               0.
                            */
    UINT16 vga_palette_snoop_enable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal devices. Hardwired to
                               0.
                            */
    UINT16 parity_error_response : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit has no impact on error reporting from
                               these devices
                            */
    UINT16 idsel_stepping_wait_cycle_control : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal devices. Hardwired to
                               0.
                            */
    UINT16 serr_enable : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               This bit has no impact on error reporting from
                               these devices
                            */
    UINT16 fast_back_to_back_enable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCI Express and is hardwired
                               to 0
                            */
    UINT16 intx_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* N/A for these devices */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PCISTS_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x11f00006)                                                      */
/*      SPRB0 (0x11f00006)                                                      */
/*      SPRHBM (0x11f00006)                                                     */
/*      SPRC0 (0x11f00006)                                                      */
/*      SPRMCC (0x11f00006)                                                     */
/*      SPRUCC (0x11f00006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_UNICAST_GROUP1_CHA_PMA_REG 0x01010006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 intx_status : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Reflects the state of the INTA# signal at the
                               input of the enable/disable circuit. This bit is
                               set by HW to 1 when the INTA# is asserted. This
                               bit is reset by HW to 0 after the interrupt is
                               cleared (independent of the state of the
                               Interrupt Disable bit in the PCICMD register).
                               Hardwired to 0 on the processor
                            */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of a
                               capabilities list structure. When set to 1,
                               indicates the register at 34h provides an offset
                               into the function.
                            */
    UINT16 x66mhz_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 reserved : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 fast_back_to_back : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 master_data_parity_error : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 signaled_target_abort : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 received_target_abort : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 received_master_abort : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 signaled_system_error : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 detected_parity_error : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is set when the device receives a
                               packet on the primary side with an uncorrectable
                               data error (including a packet with poison bit
                               set) or an uncorrectable address/control parity
                               error. The setting of this bit is regardless of
                               the Parity Error Response bit (PERRE) in the
                               PCICMD register.
                            */

  } Bits;
  UINT16 Data;

} PCISTS_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* RID_CCR_UNICAST_GROUP1_CHA_PMA_REG supported on:                             */
/*      SPRA0 (0x21f00008)                                                      */
/*      SPRB0 (0x21f00008)                                                      */
/*      SPRHBM (0x21f00008)                                                     */
/*      SPRC0 (0x21f00008)                                                      */
/*      SPRMCC (0x21f00008)                                                     */
/*      SPRUCC (0x21f00008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_UNICAST_GROUP1_CHA_PMA_REG 0x01020008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reflects the Uncore Revision ID after reset.
                               Reflects the Compatibility Revision ID if BIOS
                               updates.
                            */
    UINT32 register_level_programming_interface : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Register Level Programming Interface */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000080*/

                            /* Sub class code */
    UINT32 base_class : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000008*/

                            /* Base class code */

  } Bits;
  UINT32 Data;

} RID_CCR_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* CLSR_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x1f0000c)                                                       */
/*      SPRB0 (0x1f0000c)                                                       */
/*      SPRHBM (0x1f0000c)                                                      */
/*      SPRC0 (0x1f0000c)                                                       */
/*      SPRMCC (0x1f0000c)                                                      */
/*      SPRUCC (0x1f0000c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_UNICAST_GROUP1_CHA_PMA_REG 0x0100000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Size of Cacheline */

  } Bits;
  UINT8 Data;

} CLSR_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PLAT_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x1f0000d)                                                       */
/*      SPRB0 (0x1f0000d)                                                       */
/*      SPRHBM (0x1f0000d)                                                      */
/*      SPRC0 (0x1f0000d)                                                       */
/*      SPRMCC (0x1f0000d)                                                      */
/*      SPRUCC (0x1f0000d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_UNICAST_GROUP1_CHA_PMA_REG 0x0100000D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 primary_latency_timer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 00h. */

  } Bits;
  UINT8 Data;

} PLAT_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* HDR_UNICAST_GROUP1_CHA_PMA_REG supported on:                                 */
/*      SPRA0 (0x1f0000e)                                                       */
/*      SPRB0 (0x1f0000e)                                                       */
/*      SPRHBM (0x1f0000e)                                                      */
/*      SPRC0 (0x1f0000e)                                                       */
/*      SPRMCC (0x1f0000e)                                                      */
/*      SPRUCC (0x1f0000e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_UNICAST_GROUP1_CHA_PMA_REG 0x0100000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 configuration_layout : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /* Type 0 header */
    UINT8 multi_function_device : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               This bit defaults to 1b since all these devices
                               are multi-function
                            */

  } Bits;
  UINT8 Data;

} HDR_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* BIST_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x1f0000f)                                                       */
/*      SPRB0 (0x1f0000f)                                                       */
/*      SPRHBM (0x1f0000f)                                                      */
/*      SPRC0 (0x1f0000f)                                                       */
/*      SPRMCC (0x1f0000f)                                                      */
/*      SPRUCC (0x1f0000f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_UNICAST_GROUP1_CHA_PMA_REG 0x0100000F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bist_tests : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Not supported. Hardwired to 00h */

  } Bits;
  UINT8 Data;

} BIST_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* SVID_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x11f0002c)                                                      */
/*      SPRB0 (0x11f0002c)                                                      */
/*      SPRHBM (0x11f0002c)                                                     */
/*      SPRC0 (0x11f0002c)                                                      */
/*      SPRMCC (0x11f0002c)                                                     */
/*      SPRUCC (0x11f0002c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_UNICAST_GROUP1_CHA_PMA_REG 0x0101002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_vendor_identification_number : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00008086*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SVID_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* SDID_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x11f0002e)                                                      */
/*      SPRB0 (0x11f0002e)                                                      */
/*      SPRHBM (0x11f0002e)                                                     */
/*      SPRC0 (0x11f0002e)                                                      */
/*      SPRMCC (0x11f0002e)                                                     */
/*      SPRUCC (0x11f0002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_UNICAST_GROUP1_CHA_PMA_REG 0x0101002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_identification_number : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00000000*/

                            /*
                               Assigned by the subsystem vendor to uniquely
                               identify the subsystem
                            */

  } Bits;
  UINT16 Data;

} SDID_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* CAPPTR_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x1f00034)                                                       */
/*      SPRB0 (0x1f00034)                                                       */
/*      SPRHBM (0x1f00034)                                                      */
/*      SPRC0 (0x1f00034)                                                       */
/*      SPRMCC (0x1f00034)                                                      */
/*      SPRUCC (0x1f00034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_UNICAST_GROUP1_CHA_PMA_REG 0x01000034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /*
                               Points to the first capability structure for the
                               device which is the PCIe capability.
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* INTL_UNICAST_GROUP1_CHA_PMA_REG supported on:                                */
/*      SPRA0 (0x1f0003c)                                                       */
/*      SPRB0 (0x1f0003c)                                                       */
/*      SPRHBM (0x1f0003c)                                                      */
/*      SPRC0 (0x1f0003c)                                                       */
/*      SPRMCC (0x1f0003c)                                                      */
/*      SPRUCC (0x1f0003c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_UNICAST_GROUP1_CHA_PMA_REG 0x0100003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 interrupt_line : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* N/A for these devices */

  } Bits;
  UINT8 Data;

} INTL_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* INTPIN_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x1f0003d)                                                       */
/*      SPRB0 (0x1f0003d)                                                       */
/*      SPRHBM (0x1f0003d)                                                      */
/*      SPRC0 (0x1f0003d)                                                       */
/*      SPRMCC (0x1f0003d)                                                      */
/*      SPRUCC (0x1f0003d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_UNICAST_GROUP1_CHA_PMA_REG 0x0100003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 interrupt_pin : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               N/A since these devices do not generate any
                               interrupt on their own
                            */

  } Bits;
  UINT8 Data;

} INTPIN_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* MINGNT_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x1f0003e)                                                       */
/*      SPRB0 (0x1f0003e)                                                       */
/*      SPRHBM (0x1f0003e)                                                      */
/*      SPRC0 (0x1f0003e)                                                       */
/*      SPRMCC (0x1f0003e)                                                      */
/*      SPRUCC (0x1f0003e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_UNICAST_GROUP1_CHA_PMA_REG 0x0100003E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mgv : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               The device does not burst as a PCI compliant
                               master.
                            */

  } Bits;
  UINT8 Data;

} MINGNT_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* MAXLAT_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x1f0003f)                                                       */
/*      SPRB0 (0x1f0003f)                                                       */
/*      SPRHBM (0x1f0003f)                                                      */
/*      SPRC0 (0x1f0003f)                                                       */
/*      SPRMCC (0x1f0003f)                                                      */
/*      SPRUCC (0x1f0003f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_UNICAST_GROUP1_CHA_PMA_REG 0x0100003F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mlv : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               The device has no specific requirements for how
                               often it needs to access the PCI bus.
                            */

  } Bits;
  UINT8 Data;

} MAXLAT_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PXPCAP_UNICAST_GROUP1_CHA_PMA_REG supported on:                              */
/*      SPRA0 (0x21f00040)                                                      */
/*      SPRB0 (0x21f00040)                                                      */
/*      SPRHBM (0x21f00040)                                                     */
/*      SPRC0 (0x21f00040)                                                      */
/*      SPRMCC (0x21f00040)                                                     */
/*      SPRUCC (0x21f00040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_UNICAST_GROUP1_CHA_PMA_REG 0x01020040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*
                               Identifies the PCI Express capability assigned
                               by PCI-SIG.
                            */
    UINT32 next_ptr : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Pointer to the next capability. Set to 0 to
                               indicate there are no more capability
                               structures.
                            */
    UINT32 capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000002*/

                            /*
                               PCI Express Capability is Compliant with Version
                               2.0 of the PCI Express Spec.
                            */
    UINT32 device_port_type : 4;

                            /* Bits[23:20], Access Type=RO, default=0x00000009*/

                            /* Device type is Root Complex Integrated Endpoint */
    UINT32 slot_implemented : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /* Not valid for PCIe integrated Endpoints */
    UINT32 interrupt_message_number : 5;

                            /* Bits[29:25], Access Type=RO, default=0x00000000*/

                            /*
                               Not valid for this device, since the device does
                               not generate interrupts
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PXPCAP_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PCIE_RSVD0_UNICAST_GROUP1_CHA_PMA_REG supported on:                          */
/*      SPRA0 (0x21f00044)                                                      */
/*      SPRB0 (0x21f00044)                                                      */
/*      SPRHBM (0x21f00044)                                                     */
/*      SPRC0 (0x21f00044)                                                      */
/*      SPRMCC (0x21f00044)                                                     */
/*      SPRUCC (0x21f00044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_UNICAST_GROUP1_CHA_PMA_REG 0x01020044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD0_UNICAST_GROUP1_CHA_PMA_STRUCT;

/* PCIE_RSVD1_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00048)                                                      */
/*      SPRB0 (0x21f00048)                                                      */
/*      SPRHBM (0x21f00048)                                                     */
/*      SPRC0 (0x21f00048)                                                      */
/*      SPRMCC (0x21f00048)                                                     */
/*      SPRUCC (0x21f00048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD1_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD1_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f0004c)                                                      */
/*      SPRB0 (0x21f0004c)                                                      */
/*      SPRHBM (0x21f0004c)                                                     */
/*      SPRC0 (0x21f0004c)                                                      */
/*      SPRMCC (0x21f0004c)                                                     */
/*      SPRUCC (0x21f0004c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_UNICAST_GROUP1_N1_CHA_PMA_REG 0x0102004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD1_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD2_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00050)                                                      */
/*      SPRB0 (0x21f00050)                                                      */
/*      SPRHBM (0x21f00050)                                                     */
/*      SPRC0 (0x21f00050)                                                      */
/*      SPRMCC (0x21f00050)                                                     */
/*      SPRUCC (0x21f00050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD2_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD2_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00054)                                                      */
/*      SPRB0 (0x21f00054)                                                      */
/*      SPRHBM (0x21f00054)                                                     */
/*      SPRC0 (0x21f00054)                                                      */
/*      SPRMCC (0x21f00054)                                                     */
/*      SPRUCC (0x21f00054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_UNICAST_GROUP1_N1_CHA_PMA_REG 0x01020054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD2_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD3_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00058)                                                      */
/*      SPRB0 (0x21f00058)                                                      */
/*      SPRHBM (0x21f00058)                                                     */
/*      SPRC0 (0x21f00058)                                                      */
/*      SPRMCC (0x21f00058)                                                     */
/*      SPRUCC (0x21f00058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD3_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD3_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f0005c)                                                      */
/*      SPRB0 (0x21f0005c)                                                      */
/*      SPRHBM (0x21f0005c)                                                     */
/*      SPRC0 (0x21f0005c)                                                      */
/*      SPRMCC (0x21f0005c)                                                     */
/*      SPRUCC (0x21f0005c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_UNICAST_GROUP1_N1_CHA_PMA_REG 0x0102005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD3_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD4_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00060)                                                      */
/*      SPRB0 (0x21f00060)                                                      */
/*      SPRHBM (0x21f00060)                                                     */
/*      SPRC0 (0x21f00060)                                                      */
/*      SPRMCC (0x21f00060)                                                     */
/*      SPRUCC (0x21f00060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD4_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD4_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00064)                                                      */
/*      SPRB0 (0x21f00064)                                                      */
/*      SPRHBM (0x21f00064)                                                     */
/*      SPRC0 (0x21f00064)                                                      */
/*      SPRMCC (0x21f00064)                                                     */
/*      SPRUCC (0x21f00064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_UNICAST_GROUP1_N1_CHA_PMA_REG 0x01020064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD4_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD5_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00068)                                                      */
/*      SPRB0 (0x21f00068)                                                      */
/*      SPRHBM (0x21f00068)                                                     */
/*      SPRC0 (0x21f00068)                                                      */
/*      SPRMCC (0x21f00068)                                                     */
/*      SPRUCC (0x21f00068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD5_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD5_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f0006c)                                                      */
/*      SPRB0 (0x21f0006c)                                                      */
/*      SPRHBM (0x21f0006c)                                                     */
/*      SPRC0 (0x21f0006c)                                                      */
/*      SPRMCC (0x21f0006c)                                                     */
/*      SPRUCC (0x21f0006c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_UNICAST_GROUP1_N1_CHA_PMA_REG 0x0102006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD5_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD6_UNICAST_GROUP1_N0_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00070)                                                      */
/*      SPRB0 (0x21f00070)                                                      */
/*      SPRHBM (0x21f00070)                                                     */
/*      SPRC0 (0x21f00070)                                                      */
/*      SPRMCC (0x21f00070)                                                     */
/*      SPRUCC (0x21f00070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_UNICAST_GROUP1_N0_CHA_PMA_REG 0x01020070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD6_UNICAST_GROUP1_N0_CHA_PMA_STRUCT;

/* PCIE_RSVD6_UNICAST_GROUP1_N1_CHA_PMA_REG supported on:                       */
/*      SPRA0 (0x21f00074)                                                      */
/*      SPRB0 (0x21f00074)                                                      */
/*      SPRHBM (0x21f00074)                                                     */
/*      SPRC0 (0x21f00074)                                                      */
/*      SPRMCC (0x21f00074)                                                     */
/*      SPRUCC (0x21f00074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_UNICAST_GROUP1_N1_CHA_PMA_REG 0x01020074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD6_UNICAST_GROUP1_N1_CHA_PMA_STRUCT;

/* PCIE_RSVD7_UNICAST_GROUP1_CHA_PMA_REG supported on:                          */
/*      SPRA0 (0x21f00078)                                                      */
/*      SPRB0 (0x21f00078)                                                      */
/*      SPRHBM (0x21f00078)                                                     */
/*      SPRC0 (0x21f00078)                                                      */
/*      SPRMCC (0x21f00078)                                                     */
/*      SPRUCC (0x21f00078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_UNICAST_GROUP1_CHA_PMA_REG 0x01020078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* This space is reserved */

  } Bits;
  UINT32 Data;

} PCIE_RSVD7_UNICAST_GROUP1_CHA_PMA_STRUCT;



/* CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_REG supported on:                          */
/*      SPRA0 (0x21f000a0)                                                      */
/*      SPRB0 (0x21f000a0)                                                      */
/*      SPRHBM (0x21f000a0)                                                     */
/*      SPRC0 (0x21f000a0)                                                      */
/*      SPRMCC (0x21f000a0)                                                     */
/*      SPRUCC (0x21f000a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Spare
*/


#define CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_REG 0x010200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBPIPES_UNICAST_CFG_SPARE_CHA_PMA_STRUCT;

/* AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG supported on:                            */
/*      SPRA0 (0x21f000d8)                                                      */
/*      SPRB0 (0x21f000d8)                                                      */
/*      SPRHBM (0x21f000d8)                                                     */
/*      SPRC0 (0x21f000d8)                                                      */
/*      SPRMCC (0x21f000d8)                                                     */
/*      SPRUCC (0x21f000d8)                                                     */
/* Register default value on SPRA0: 0x00001111                                  */
/* Register default value on SPRB0: 0x00001111                                  */
/* Register default value on SPRHBM: 0x00001111                                 */
/* Register default value on SPRC0: 0x00001111                                  */
/* Register default value on SPRMCC: 0x00001111                                 */
/* Register default value on SPRUCC: 0x00001111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration
*/


#define AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG 0x010200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_vna_port0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT0. Intel UPI0 */
    UINT32 ad_vna_port1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT1. Intel UPI1 */
    UINT32 ad_vna_port2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT2. Intel UPI2 */
    UINT32 ad_vna_port3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT3. Intel UPI3 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_STRUCT;

/* AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG supported on:                            */
/*      SPRA0 (0x21f000dc)                                                      */
/*      SPRB0 (0x21f000dc)                                                      */
/*      SPRHBM (0x21f000dc)                                                     */
/*      SPRC0 (0x21f000dc)                                                      */
/*      SPRMCC (0x21f000dc)                                                     */
/*      SPRUCC (0x21f000dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration
*/


#define AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG 0x010200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vnacreditchange : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Change VNA credits (Load from Shadow) */
    UINT32 disisocvnres : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Disable ISOC VN credit reservation */
    UINT32 rsvd_18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT;

/* BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG supported on:                            */
/*      SPRA0 (0x21f000e0)                                                      */
/*      SPRB0 (0x21f000e0)                                                      */
/*      SPRHBM (0x21f000e0)                                                     */
/*      SPRC0 (0x21f000e0)                                                      */
/*      SPRMCC (0x21f000e0)                                                     */
/*      SPRUCC (0x21f000e0)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration
*/


#define BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG 0x010200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT0. Intel UPI0 */
    UINT32 bl_vna_port1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT1. Intel UPI1 */
    UINT32 bl_vna_port2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT2. Intel UPI2 */
    UINT32 bl_vna_port3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT3. Intel UPI3 */
    UINT32 bl_vna_port4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT4. PCIE0 */
    UINT32 bl_vna_port5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT5. PCIE1 */
    UINT32 bl_vna_port6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT6. PCIE2 */
    UINT32 bl_vna_port7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT7. PCIE3 */

  } Bits;
  UINT32 Data;

} BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_STRUCT;

/* BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG supported on:                            */
/*      SPRA0 (0x21f000e4)                                                      */
/*      SPRB0 (0x21f000e4)                                                      */
/*      SPRHBM (0x21f000e4)                                                     */
/*      SPRC0 (0x21f000e4)                                                      */
/*      SPRMCC (0x21f000e4)                                                     */
/*      SPRUCC (0x21f000e4)                                                     */
/* Register default value on SPRA0: 0x00000111                                  */
/* Register default value on SPRB0: 0x00000111                                  */
/* Register default value on SPRHBM: 0x00000111                                 */
/* Register default value on SPRC0: 0x00000111                                  */
/* Register default value on SPRMCC: 0x00000111                                 */
/* Register default value on SPRUCC: 0x00000111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration
*/


#define BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG 0x010200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port8 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT8. PCIE4 */
    UINT32 bl_vna_port9 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT9. PCIE5 */
    UINT32 bl_vna_port10 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT10. UBOX */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vnacreditchange : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Change VNA credits (Load from Shadow) */
    UINT32 disisocvnres : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Disable ISOC VN credit reservation */
    UINT32 rsvd_18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT;

/* BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_REG supported on:                           */
/*      SPRA0 (0x21f000e8)                                                      */
/*      SPRB0 (0x21f000e8)                                                      */
/*      SPRHBM (0x21f000e8)                                                     */
/*      SPRC0 (0x21f000e8)                                                      */
/*      SPRMCC (0x21f000e8)                                                     */
/*      SPRUCC (0x21f000e8)                                                     */
/* Register default value on SPRA0: 0x00111111                                  */
/* Register default value on SPRB0: 0x00111111                                  */
/* Register default value on SPRHBM: 0x00111111                                 */
/* Register default value on SPRC0: 0x00111111                                  */
/* Register default value on SPRMCC: 0x00111111                                 */
/* Register default value on SPRUCC: 0x00111111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration for extra 6 MS2IOSF
*/


#define BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_REG 0x010200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port11 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT11. PCIE6 */
    UINT32 bl_vna_port12 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT12. PCIE7 */
    UINT32 bl_vna_port13 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT13. PCIE8 */
    UINT32 bl_vna_port14 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT14. PCIE9 */
    UINT32 bl_vna_port15 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT15. PCIE10 */
    UINT32 bl_vna_port16 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* BL VNA count for PORT16. PCIE11 */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_STRUCT;

/* BL_VNA_CREDIT_CONFIG2_N1_CHA_PMA_REG supported on:                           */
/*      SPRA0 (0x21f000ec)                                                      */
/*      SPRB0 (0x21f000ec)                                                      */
/*      SPRHBM (0x21f000ec)                                                     */
/*      SPRC0 (0x21f000ec)                                                      */
/*      SPRMCC (0x21f000ec)                                                     */
/*      SPRUCC (0x21f000ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Register related to VNA Credit Configuration for extra 6 MS2IOSF
*/


#define BL_VNA_CREDIT_CONFIG2_N1_CHA_PMA_REG 0x010200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 32;

                            /* Bits[31:0], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BL_VNA_CREDIT_CONFIG2_N1_CHA_PMA_STRUCT;

/* SHADOW_UPI_ROUTING_TABLE_N0_CHA_PMA_REG supported on:                        */
/*      SPRA0 (0x21f000f0)                                                      */
/*      SPRB0 (0x21f000f0)                                                      */
/*      SPRHBM (0x21f000f0)                                                     */
/*      SPRC0 (0x21f000f0)                                                      */
/*      SPRMCC (0x21f000f0)                                                     */
/*      SPRUCC (0x21f000f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Shadow copy of UPI_Routing_Table.

The 48 bit vector defined in this register captures 16 UPI ID values (supporting up to 8 UPI links). The index bit scheme selected by UpiRoutingConfig.UpiInterleaveMode is used to index to this UPI Routing Table.  
Note : If the number of UPI links supported between the nodes is less than the max number of UPI links supported by UpiRoutingConfig.UpiInterleaveMode, then the LogicalUpiTarget entries need to be replicated appropriately (ie. aliasing). 
*/


#define SHADOW_UPI_ROUTING_TABLE_N0_CHA_PMA_REG 0x010200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 logical_upi_target : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This 48 bit vector captures 16 UPI ID values,
                               each ID being 3 bits (supporting up to 8 UPI
                               links). The index bit scheme selected by
                               UpiRoutingConfig.UpiInterleaveMode is used to
                               index to this UPI Routing Table. Bits [2:0]
                               correspond to the UPI ID selected by Index==0,
                               bits[5:3] correspond to the UPI ID selected by
                               Index==1, etc.
                            */

  } Bits;
  UINT32 Data;

} SHADOW_UPI_ROUTING_TABLE_N0_CHA_PMA_STRUCT;

/* SHADOW_UPI_ROUTING_TABLE_N1_CHA_PMA_REG supported on:                        */
/*      SPRA0 (0x21f000f4)                                                      */
/*      SPRB0 (0x21f000f4)                                                      */
/*      SPRHBM (0x21f000f4)                                                     */
/*      SPRC0 (0x21f000f4)                                                      */
/*      SPRMCC (0x21f000f4)                                                     */
/*      SPRUCC (0x21f000f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Shadow copy of UPI_Routing_Table.

The 48 bit vector defined in this register captures 16 UPI ID values (supporting up to 8 UPI links). The index bit scheme selected by UpiRoutingConfig.UpiInterleaveMode is used to index to this UPI Routing Table.  
Note : If the number of UPI links supported between the nodes is less than the max number of UPI links supported by UpiRoutingConfig.UpiInterleaveMode, then the LogicalUpiTarget entries need to be replicated appropriately (ie. aliasing). 
*/


#define SHADOW_UPI_ROUTING_TABLE_N1_CHA_PMA_REG 0x010200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 logical_upi_target : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This 48 bit vector captures 16 UPI ID values,
                               each ID being 3 bits (supporting up to 8 UPI
                               links). The index bit scheme selected by
                               UpiRoutingConfig.UpiInterleaveMode is used to
                               index to this UPI Routing Table. Bits [2:0]
                               correspond to the UPI ID selected by Index==0,
                               bits[5:3] correspond to the UPI ID selected by
                               Index==1, etc.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SHADOW_UPI_ROUTING_TABLE_N1_CHA_PMA_STRUCT;

/* UPI_ROUTING_TABLE_N0_CHA_PMA_REG supported on:                               */
/*      SPRA0 (0x21f000f8)                                                      */
/*      SPRB0 (0x21f000f8)                                                      */
/*      SPRHBM (0x21f000f8)                                                     */
/*      SPRC0 (0x21f000f8)                                                      */
/*      SPRMCC (0x21f000f8)                                                     */
/*      SPRUCC (0x21f000f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Intel UPI routing table defines the mapping of the Intel UPI NodeIDs in the system to the Intel UPI ports on the die.
The shadow copy gets copied to the non-shadow copy at warm reset while the non-shadow copy is the one that represent the current contents of the actual routing table.
Also, both the shadow and non-shadow version of this register have both a bcst copy that is in the multicast BDF's address map and a non-bcst copy that is in the unicast BDF's address map. These copies map to the same physical registers and BIOS will program the ucst version only for those 2S-2link system topologies that require it.
*/


#define UPI_ROUTING_TABLE_N0_CHA_PMA_REG 0x010200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 logical_upi_target : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               This 48 bit vector captures 16 UPI ID values,
                               each ID being 3 bits (supporting up to 8 UPI
                               links). The index bit scheme selected by
                               UpiRoutingConfig.UpiInterleaveMode is used to
                               index to this UPI Routing Table. Bits [2:0]
                               correspond to the UPI ID selected by Index==0,
                               bits[5:3] correspond to the UPI ID selected by
                               Index==1, etc
                            */

  } Bits;
  UINT32 Data;

} UPI_ROUTING_TABLE_N0_CHA_PMA_STRUCT;

/* UPI_ROUTING_TABLE_N1_CHA_PMA_REG supported on:                               */
/*      SPRA0 (0x21f000fc)                                                      */
/*      SPRB0 (0x21f000fc)                                                      */
/*      SPRHBM (0x21f000fc)                                                     */
/*      SPRC0 (0x21f000fc)                                                      */
/*      SPRMCC (0x21f000fc)                                                     */
/*      SPRUCC (0x21f000fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Intel UPI routing table defines the mapping of the Intel UPI NodeIDs in the system to the Intel UPI ports on the die.
The shadow copy gets copied to the non-shadow copy at warm reset while the non-shadow copy is the one that represent the current contents of the actual routing table.
Also, both the shadow and non-shadow version of this register have both a bcst copy that is in the multicast BDF's address map and a non-bcst copy that is in the unicast BDF's address map. These copies map to the same physical registers and BIOS will program the ucst version only for those 2S-2link system topologies that require it.
*/


#define UPI_ROUTING_TABLE_N1_CHA_PMA_REG 0x010200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 logical_upi_target : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               This 48 bit vector captures 16 UPI ID values,
                               each ID being 3 bits (supporting up to 8 UPI
                               links). The index bit scheme selected by
                               UpiRoutingConfig.UpiInterleaveMode is used to
                               index to this UPI Routing Table. Bits [2:0]
                               correspond to the UPI ID selected by Index==0,
                               bits[5:3] correspond to the UPI ID selected by
                               Index==1, etc
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UPI_ROUTING_TABLE_N1_CHA_PMA_STRUCT;

/* PXPENHCAP_UNICAST_GROUP1_CHA_PMA_REG supported on:                           */
/*      SPRA0 (0x21f00100)                                                      */
/*      SPRB0 (0x21f00100)                                                      */
/*      SPRHBM (0x21f00100)                                                     */
/*      SPRC0 (0x21f00100)                                                      */
/*      SPRMCC (0x21f00100)                                                     */
/*      SPRUCC (0x21f00100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_UNICAST_GROUP1_CHA_PMA_REG 0x01020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* There is no capability at this location */
    UINT32 capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /* There is no capability at this location. */
    UINT32 next_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               Pointer to the next capability in the enhanced
                               configuration space. Set to 0 to indicate there
                               are no more capability structures.
                            */

  } Bits;
  UINT32 Data;

} PXPENHCAP_UNICAST_GROUP1_CHA_PMA_STRUCT;









/* CHA_SNC_CONFIG_CHA_PMA_REG supported on:                                     */
/*      SPRA0 (0x21f00220)                                                      */
/*      SPRB0 (0x21f00220)                                                      */
/*      SPRHBM (0x21f00220)                                                     */
/*      SPRC0 (0x21f00220)                                                      */
/*      SPRMCC (0x21f00220)                                                     */
/*      SPRUCC (0x21f00220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SNC Configuration, used for PRE/PCLS.  Relevant values should match those in SNC_CONFIG register.
*/


#define CHA_SNC_CONFIG_CHA_PMA_REG 0x01020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 full_snc_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* full snc enable */
    UINT32 snc_ind_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* SNC IND enable */
    UINT32 num_snc_clu : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /* Num of SNC Clusters */
    UINT32 rsvd : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sncbase1id : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* SNC cluster 1 base mesh ID */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sncbase2id : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* SNC cluster 2 base mesh ID */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sncbase3id : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* SNC cluster 3 base mesh ID */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CHA_SNC_CONFIG_CHA_PMA_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* MKTME_CONFIG_CHA_PMA_REG supported on:                                       */
/*      SPRC0 (0x21f00230)                                                      */
/*      SPRMCC (0x21f00230)                                                     */
/*      SPRUCC (0x21f00230)                                                     */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRC0 BDF: 31_0_0                                      */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MKTME Aliasing bits that need LB_MCHECK policy.
*/


#define MKTME_CONFIG_CHA_PMA_REG 0x01020230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 discleandirupdate : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When this chicken bit is Set, it disabled
                               sending MemWrPtlDirOnly for the MKTME aliasing
                               flows.
                            */
    UINT32 mapmktmedirstodira : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, The Directory State being send to MC
                               will be changed from S-state to A-state. This
                               should be set only when MKTME Aliasing is
                               enabled.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MKTME_CONFIG_CHA_PMA_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* CBO_ISOC_CONFIG_CHA_PMA_REG supported on:                                    */
/*      SPRA0 (0x21f00244)                                                      */
/*      SPRB0 (0x21f00244)                                                      */
/*      SPRHBM (0x21f00244)                                                     */
/*      SPRC0 (0x21f00244)                                                      */
/*      SPRMCC (0x21f00244)                                                     */
/*      SPRUCC (0x21f00244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Cbo ISOC Config 
*/


#define CBO_ISOC_CONFIG_CHA_PMA_REG 0x01020244

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 isoc_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable ISOC mode. This will be used for TOR
                               pipeline to reserve TOR entries for ISOC.
                            */
    UINT32 enablerlinktomcpriority : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable forcing of priority bit for AD/BL
                               messages to MC due to RLink requests
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_ISOC_CONFIG_CHA_PMA_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 isoc_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable ISOC mode. This will be used for TOR
                               pipeline to reserve TOR entries for ISOC. This
                               setting must be mutually exclusive with
                               EnableDemandPriority.
                            */
    UINT32 enablerlinktomcpriority : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable forcing of priority bit for AD/BL
                               messages to MC due to RLink requests
                            */
    UINT32 enabledemandprioritylocal : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set to 1, demand (i.e. non-
                               prefetch) transactions to memory will be tagged
                               as priority transactions. This setting must be
                               mutually exclusive with isoc_enable.
                            */
    UINT32 enabledemandpriorityremote : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set to 1, demand (i.e. non-
                               prefetch) transactions to remote memory via UPI
                               will be tagged as priority transactions. Setting
                               this bit to 1 also allows CHA to recognize
                               incoming UPI requests as demand priority
                               requests. Priority for remote memory requests
                               may only be used in systems with four processor
                               nodes or fewer. All sockets in a system should
                               typically be configured consistently (i.e.
                               remote demand priority off or on for all). This
                               setting must be mutually exclusive with
                               isoc_enable.
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_ISOC_CONFIG_CHA_PMA_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 isoc_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable ISOC mode. This will be used for TOR
                               pipeline to reserve TOR entries for ISOC.
                            */
    UINT32 enablerlinktomcpriority : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable forcing of priority bit for AD/BL
                               messages to MC due to RLink requests
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_ISOC_CONFIG_CHA_PMA_STRUCT;

/* CBO_COH_CONFIG_CHA_PMA_REG supported on:                                     */
/*      SPRA0 (0x21f00250)                                                      */
/*      SPRB0 (0x21f00250)                                                      */
/*      SPRHBM (0x21f00250)                                                     */
/*      SPRC0 (0x21f00250)                                                      */
/*      SPRMCC (0x21f00250)                                                     */
/*      SPRUCC (0x21f00250)                                                     */
/* Register default value on SPRA0: 0x0008A0A0                                  */
/* Register default value on SPRB0: 0x0008A0A0                                  */
/* Register default value on SPRHBM: 0x0008A0A0                                 */
/* Register default value on SPRC0: 0x0008A0A0                                  */
/* Register default value on SPRMCC: 0x0008A0A0                                 */
/* Register default value on SPRUCC: 0x0008A0A0                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Cbo Coh Config 
*/


#define CBO_COH_CONFIG_CHA_PMA_REG 0x01020250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ego : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable CHA Early GO mode for RFO */
    UINT32 directorydisabledisd2c : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When direcotry is disabled have D2C/D2K
                               indication from TOR to HA cleared. &nbsp;This is
                               expected to be 0 for wave1 products to retain
                               SKX behavior. &nbsp;This is expected to be 1 for
                               wave2 products becuase this is needed to support
                               RSF.
                            */
    UINT32 disablenm2waybias : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               The main disable/chicken bit for this NM 2 way
                               Bias feature is in M2Mem. But in CHA, we are
                               implementing this additional bit to force
                               InPrefWay=1 if we want the feature to be
                               disabled.
                            */
    UINT32 disablecdf_rem : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Disable CDF snoops (alias to non-CDF snoop) for
                               Remote requests
                            */
    UINT32 disablecdf_loc : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Disable CDF snoops (alias to non-CDF snoop) for
                               Local requests
                            */
    UINT32 enableearlyqpireq : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Enable sending RdInvOwn/InvItoE in parallel with
                               core snoops
                            */
    UINT32 cldemotedefeature : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               When set, CLDemote will simply send GO-I and
                               complete.
                            */
    UINT32 llcsharedrdcrd : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Enable migration from SF to LLC and to leave
                               shared lines in the LLC for Drd and Crd
                            */
    UINT32 drdgosone : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable GO_S on LLC_E state for DRD when the line
                               was valid in any core.&nbsp;
                            */
    UINT32 sendwbedata : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               For DRd, CRd, and PCIRdCur with LLC disabled
                               send a WbEData instead of WbSData when writing
                               back M data
                            */
    UINT32 mtoibias : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Use MtoI policy as opposed to MtoS policy */
    UINT32 defeature_drdgos_on_boot : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Default value should be 0. When 0: You should
                               force GoS for any DRd for which the incoming SAI
                               was the BOOT_BIOS_SAI, and behave "normally"
                               otherwise. Then for DRd that have the
                               BOOT_BIOS_SAI, you should set only the read bit.
                               For any other DRd, set the read bit and the
                               write bit. When 1: You should always behave
                               normally, and DRd should always set both the
                               Read and the Write bit.
                            */
    UINT32 biasfwd : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable RspFwdIWB mode, BiasFwdDoubleData &
                               BiasFwdLocalHome are used for further
                               qualifications. Table below shows the
                               qualifications and behaviors:</p> <p>BiasFwd
                               &nbsp; BiasFwdDoubleData &nbsp; &nbsp; Behaviour
                               for SnpData/Code with LLC=M &nbsp;&nbsp;<br>
                               &nbsp; &nbsp;0 &nbsp; x &nbsp; &nbsp; Original
                               behaviour. No C2C. Implicit writeback to HA
                               &nbsp; &nbsp;&nbsp;<br> &nbsp; &nbsp;1 &nbsp; 0
                               &nbsp; &nbsp; Fwd only when Home!=Requestor
                               &nbsp; &nbsp; &nbsp;<br> &nbsp; &nbsp;1 &nbsp; 1
                               &nbsp; &nbsp; Fwd for all cases"</p>
                            */
    UINT32 waitfordatacmp : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Wait for Data+Cmp before sending through Cpipe.
                               if 0h, will do it separately.
                            */
    UINT32 morphpcommittowcil : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Defeature bit to morph PCommit request to a WCiL */
    UINT32 biasfwddoubledata : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               RspFwdIWB when HOME==Requestor (BiasFwd must be
                               enabled)
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppressinvitoeonwrite : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* Do not send and InvItoE for WCiL, WiL, WCiLF */
    UINT32 llcmissendrd : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Enable LLC Miss message for DRd */
    UINT32 llcmissendrdpte : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Enable LLC Miss message for DRdPTE */
    UINT32 disisoctorres : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Disable TOR ISOC reservation */
    UINT32 disisocegrres : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Disable ISOC Egress Reservation */
    UINT32 disselfsnpindtocore : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               When it is set, the self-snoop indication to
                               core is disabled for the core DUE improvement
                               feature
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 swapktilinkintlv : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is required to support twisted board
                               topologies where link0 on one socket is
                               connected to link1 on the other socket in
                               2S-2Link configurations and with
                               EnableXorBasedKTILink set.
                            */
    UINT32 disdropsnpindtocore : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               When it is set, the drop snoop indication to
                               core is disabled
                            */
    UINT32 frcfwdinv : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is used to alias all conflict flows to
                               FwdInvItoE behaviour
                            */
    UINT32 rsvd_27 : 2;

                            /* Bits[28:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cdfsnpdataforclwb : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               When set, send CDFSnpData instead of SnpClean
                               for CLWB. This optimization may be needed if the
                               core does not support SnpClean (and downgrades
                               it to SnpData).
                            */
    UINT32 enableitomforwcilf : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               When it is set, InvItoM will be issued for
                               remote WCiLF
                            */
    UINT32 rrq_disable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               When it is set, implies the remote request queue
                               is never active and associated reserved TOR
                               entries could be reclaimed
                            */

  } Bits;
  UINT32 Data;

} CBO_COH_CONFIG_CHA_PMA_STRUCT;

/* CBO_COH_CONFIG1_CHA_PMA_REG supported on:                                    */
/*      SPRA0 (0x21f00254)                                                      */
/*      SPRB0 (0x21f00254)                                                      */
/*      SPRHBM (0x21f00254)                                                     */
/*      SPRC0 (0x21f00254)                                                      */
/*      SPRMCC (0x21f00254)                                                     */
/*      SPRUCC (0x21f00254)                                                     */
/* Register default value on SPRA0: 0x000A0820                                  */
/* Register default value on SPRB0: 0x000A0820                                  */
/* Register default value on SPRHBM: 0x008A0820                                 */
/* Register default value on SPRC0: 0x028A0820                                  */
/* Register default value on SPRMCC: 0x028A0820                                 */
/* Register default value on SPRUCC: 0x028A0820                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Cbo Coh Config 
*/


#define CBO_COH_CONFIG1_CHA_PMA_REG 0x01020254

#if defined(SPRA0_HOST) || defined(SPRB0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sendsnpcodeforrdcur : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Revert PCIRdCur/KRdCur/SnpCur to send SnpCode
                               instead of SnpCurr
                            */
    UINT32 snpdatabiasmtoioptdis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Don't allocate into LLC on RspIFwdFE with
                               SF=E/SnpData
                            */
    UINT32 sendrdinvforitomcachenear : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               ItoMCacheNear will send RdInv instead of
                               RdInvOwn to guarantee it never gets M data
                            */
    UINT32 enablepushmtoiiodctwolmopt : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable PushMtoI IODC 2LM Optimization */
    UINT32 morphitomwrtomempushwr : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Morph ItoMWr to MemPushWr to prevent allocation
                               to LLC.&nbsp;
                            */
    UINT32 enableearlyfastgopull : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Send FastGO_Pull at the same time as InvItoE for
                               WCiLF and ItoMWr_WT
                            */
    UINT32 ipq_disable : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* disable IPQ reserved TOR entry */
    UINT32 rsvd : 3;

                            /* Bits[9:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disablehabypassforialmem : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - disable HA Bypass for
                               requests targetting IAL Mem.
                            */
    UINT32 disabledirectoryforialmem : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               Survivability hook - disable directory for
                               requests targetting IAL Mem.
                            */
    UINT32 nollcenonwbmtoitoialfromms2iosf : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from MS2IOSF sources
                               that target memory homed at an IAL accelerator
                               device. Set this bit to avoid polluting the LLC;
                               when this bit is clear, LLC is allocated as for
                               non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromAny.)
                            */
    UINT32 nollcenonwbmtoitoialfromany : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from any source
                               (including MS2IOSF) that target memory homed at
                               an IAL accelerator device. Set this bit to avoid
                               polluting the LLC; when this bit is clear, LLC
                               is allocated as for non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromMs2Iosf.)
                            */
    UINT32 nollcenonwbmtoitoialfromialself : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly back to memory) for WbMtoI and WBEFtoI
                               transactions from IAL to its own memory. When
                               this bit is clear, LLC is allocated as for non
                               IAL memory.
                            */
    UINT32 enialevictclean : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               When a transaction is targeting remote IAL
                               accelerator device memory, this bit controls
                               whether an Evict Clean message may be sent to
                               the home socket to help manage (i.e. deallocate)
                               its remote snoop filter. Setting this bit to 1
                               enables such Evict Clean messages; with the bit
                               at 0, Evict Clean will not be sent.
                            */
    UINT32 recognizeddrtdistressfromlocal : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               A memory controller may signal distress due to
                               congestion with 2LM/DDRT accesses. Distress is
                               signaled via mesh message packets as well as
                               dedicated mesh wires. This bit controls whether
                               CHA recognizes and forwards the distress
                               indication received via mesh packets from local
                               memory controllers. To avoid undesired
                               interaction between the wire and mesh packet
                               distress protocols, CHA does not recognize and
                               forward local mesh-based distress by default.
                               Setting this bit to 1, allows local mesh-based
                               distress to be seen and forwarded to the core
                               interface.
                            */
    UINT32 sfclosen : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               If this bit is set to 1, the Core Valid (CV)
                               bits in the snoop filter (SF) array will store
                               class of service (CLOS) IDs in all CV modes. If
                               set to 0, CLOS is not stored, and the SF in CV
                               mode 0 may instead store a second sharer ID.
                            */
    UINT32 enableptmbe : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Per Thread Bandwidth Enhancement (PTMBE)
                               feature by setting this bit to 1. When enabled,
                               TOR records memory accesses to both DDR and DDRT
                               memories and sends appropriate indication to
                               MS2IDI over mesh AK/AD/BL wires to increment
                               corresponding PTMBE counters. These counters are
                               read by OS to throttle/unthrottle memory usage
                               on per core, per thread basis. This bit is
                               disabled by default.
                            */
    UINT32 disablesuppressinvitoeonialrsfmiss : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing a line
                               homed on an IAL accelerator device, the lookup
                               of the IAL remote snoop filter provides clear
                               indication of whether the line is residing in a
                               remote socket. If the RSF lookup is a miss, then
                               InvItoE does not need to be sent to remote
                               sockets (i.e. may be suppressed) because the RSF
                               miss clearly indicates that the other sockets
                               cannot have the line. This suppression of
                               InvItoE is the default behavior. If this bit is
                               set, the InvItoE will be sent anyway for this
                               IAL RSF miss scenario. If RSF is disabled in a
                               multi-socket system, this bit must be set.
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_COH_CONFIG1_CHA_PMA_SPRA0_SPRB0_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) */

#if defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sendsnpcodeforrdcur : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Revert PCIRdCur/KRdCur/SnpCur to send SnpCode
                               instead of SnpCurr
                            */
    UINT32 snpdatabiasmtoioptdis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Don't allocate into LLC on RspIFwdFE with
                               SF=E/SnpData
                            */
    UINT32 sendrdinvforitomcachenear : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               ItoMCacheNear will send RdInv instead of
                               RdInvOwn to guarantee it never gets M data
                            */
    UINT32 enablepushmtoiiodctwolmopt : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable PushMtoI IODC 2LM Optimization */
    UINT32 morphitomwrtomempushwr : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Morph ItoMWr to MemPushWr to prevent allocation
                               to LLC.&nbsp;
                            */
    UINT32 enableearlyfastgopull : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Send FastGO_Pull at the same time as InvItoE for
                               WCiLF and ItoMWr_WT
                            */
    UINT32 ipq_disable : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* disable IPQ reserved TOR entry */
    UINT32 rsvd : 3;

                            /* Bits[9:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disablehabypassforialmem : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - disable HA Bypass for
                               requests targetting IAL Mem.
                            */
    UINT32 rsfenabledforialmem : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               Survivability hook - if rsf enabled for IALMEM
                               requests, remote socket caching of IALMEM lines
                               will be tracked in RSF.
                            */
    UINT32 nollcenonwbmtoitoialfromms2iosf : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from MS2IOSF sources
                               that target memory homed at an IAL accelerator
                               device. Set this bit to avoid polluting the LLC;
                               when this bit is clear, LLC is allocated as for
                               non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromAny.)
                            */
    UINT32 nollcenonwbmtoitoialfromany : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from any source
                               (including MS2IOSF) that target memory homed at
                               an IAL accelerator device. Set this bit to avoid
                               polluting the LLC; when this bit is clear, LLC
                               is allocated as for non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromMs2Iosf.)
                            */
    UINT32 nollcenonwbmtoitoialfromialself : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly back to memory) for WbMtoI and WBEFtoI
                               transactions from IAL to its own memory. When
                               this bit is clear, LLC is allocated as for non
                               IAL memory.
                            */
    UINT32 enrsfevictclean : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               When a transaction is targeting remote addresses
                               that are being tracked by RSF, this bit controls
                               whether an Evict Clean message may be sent to
                               the home socket to help manage (i.e. deallocate)
                               its remote snoop filter. Setting this bit to 1
                               enables such Evict Clean messages; with the bit
                               at 0, Evict Clean will not be sent.
                            */
    UINT32 recognizeddrtdistressfromlocal : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               A memory controller may signal distress due to
                               congestion with 2LM/DDRT accesses. Distress is
                               signaled via mesh message packets as well as
                               dedicated mesh wires. This bit controls whether
                               CHA recognizes and forwards the distress
                               indication received via mesh packets from local
                               memory controllers. To avoid undesired
                               interaction between the wire and mesh packet
                               distress protocols, CHA does not recognize and
                               forward local mesh-based distress by default.
                               Setting this bit to 1, allows local mesh-based
                               distress to be seen and forwarded to the core
                               interface.
                            */
    UINT32 sfclosen : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               If this bit is set to 1, the Core Valid (CV)
                               bits in the snoop filter (SF) array will store
                               class of service (CLOS) IDs in all CV modes. If
                               set to 0, CLOS is not stored, and the SF in CV
                               mode 0 may instead store a second sharer ID.
                            */
    UINT32 enableptmbe : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Per Thread Bandwidth Enhancement (PTMBE)
                               feature by setting this bit to 1. When enabled,
                               TOR records memory accesses to both DDR and DDRT
                               memories and sends appropriate indication to
                               MS2IDI over mesh AK/AD/BL wires to increment
                               corresponding PTMBE counters. These counters are
                               read by OS to throttle/unthrottle memory usage
                               on per core, per thread basis. This bit is
                               disabled by default.
                            */
    UINT32 disablesuppressinvitoeonialrsfmiss : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing a line
                               homed on an IAL accelerator device, the lookup
                               of the IAL remote snoop filter provides clear
                               indication of whether the line is residing in a
                               remote socket. If the RSF lookup is a miss, then
                               InvItoE does not need to be sent to remote
                               sockets (i.e. may be suppressed) because the RSF
                               miss clearly indicates that the other sockets
                               cannot have the line. This suppression of
                               InvItoE is the default behavior. If this bit is
                               set, the InvItoE will be sent anyway for this
                               IAL RSF miss scenario. If RSF is disabled in a
                               multi-socket system, this bit must be set.
                            */
    UINT32 rsfenabledfordram : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - if rsf enabled for DRAM
                               requests, remote socket caching of DRAM lines
                               will be tracked in RSF and not
                               HITME/IODC/MEMORY.
                            */
    UINT32 rsfenabledforddrt : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - if rsf enabled for DDRT
                               requests, remote socket caching of DDRT lines
                               will be tracked in RSF.
                            */
    UINT32 rsfenabledforhbm : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - if rsf enabled for HBM
                               requests, remote socket caching of HBM lines
                               will be tracked in RSF.
                            */
    UINT32 disablesuppressinvitoeonhbmrsfmiss : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing an HBM
                               line, the lookup of the remote snoop filter
                               provides clear indication of whether the line is
                               residing in a remote socket. If the RSF lookup
                               is a miss, then InvItoE does not need to be sent
                               to remote sockets (i.e. may be suppressed)
                               because the RSF miss clearly indicates that the
                               other sockets cannot have the line. This
                               suppression of InvItoE is the default behavior.
                               If this bit is set, the InvItoE will be sent
                               anyway for this IAL RSF miss scenario. If RSF is
                               disabled in a multi-socket system, this bit must
                               be set.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_COH_CONFIG1_CHA_PMA_SPRHBM_STRUCT;
#endif /* (SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sendsnpcodeforrdcur : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Revert PCIRdCur/KRdCur/SnpCur to send SnpCode
                               instead of SnpCurr
                            */
    UINT32 snpdatabiasmtoioptdis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Don't allocate into LLC on RspIFwdFE with
                               SF=E/SnpData
                            */
    UINT32 sendrdinvforitomcachenear : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               ItoMCacheNear will send RdInv instead of
                               RdInvOwn to guarantee it never gets M data
                            */
    UINT32 enablepushmtoiiodctwolmopt : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable PushMtoI IODC 2LM Optimization */
    UINT32 morphitomwrtomempushwr : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Morph ItoMWr to MemPushWr to prevent allocation
                               to LLC.&nbsp;
                            */
    UINT32 enableearlyfastgopull : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Send FastGO_Pull at the same time as InvItoE for
                               WCiLF and ItoMWr_WT
                            */
    UINT32 ipq_disable : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* disable IPQ reserved TOR entry */
    UINT32 rsvd : 3;

                            /* Bits[9:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disablehabypassforialmem : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - disable HA Bypass for
                               requests targetting IAL Mem.
                            */
    UINT32 rsfenabledforialmem : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               Enable bit to specify that requests targetting
                               IAL Memtype is tracked by RSF. This bit has to
                               be 0 if Memtype is not tracked by RSF or Snoop
                               filter is not active.
                            */
    UINT32 nollcenonwbmtoitoialfromms2iosf : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from MS2IOSF sources
                               that target memory homed at an IAL accelerator
                               device. Set this bit to avoid polluting the LLC;
                               when this bit is clear, LLC is allocated as for
                               non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromAny.)
                            */
    UINT32 nollcenonwbmtoitoialfromany : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from any source
                               (including MS2IOSF) that target memory homed at
                               an IAL accelerator device. Set this bit to avoid
                               polluting the LLC; when this bit is clear, LLC
                               is allocated as for non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromMs2Iosf.)
                            */
    UINT32 nollcenonwbmtoitoialfromialself : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly back to memory) for WbMtoI and WBEFtoI
                               transactions from IAL to its own memory. When
                               this bit is clear, LLC is allocated as for non
                               IAL memory.
                            */
    UINT32 enrsfevictclean : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               When a transaction is targeting remote addresses
                               that are being tracked by RSF, this bit controls
                               whether an Evict Clean message may be sent to
                               the home socket to help manage (i.e. deallocate)
                               its remote snoop filter. Setting this bit to 1
                               enables such Evict Clean messages; with the bit
                               at 0, Evict Clean will not be sent.
                            */
    UINT32 recognizeddrtdistressfromlocal : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               A memory controller may signal distress due to
                               congestion with 2LM/DDRT accesses. Distress is
                               signaled via mesh message packets as well as
                               dedicated mesh wires. This bit controls whether
                               CHA recognizes and forwards the distress
                               indication received via mesh packets from local
                               memory controllers. To avoid undesired
                               interaction between the wire and mesh packet
                               distress protocols, CHA does not recognize and
                               forward local mesh-based distress by default.
                               Setting this bit to 1, allows local mesh-based
                               distress to be seen and forwarded to the core
                               interface.
                            */
    UINT32 sfclosen : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               If this bit is set to 1, the Core Valid (CV)
                               bits in the snoop filter (SF) array will store
                               class of service (CLOS) IDs in all CV modes. If
                               set to 0, CLOS is not stored, and the SF in CV
                               mode 0 may instead store a second sharer ID.
                            */
    UINT32 enableptmbe : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Per Thread Bandwidth Enhancement (PTMBE)
                               feature by setting this bit to 1. When enabled,
                               TOR records memory accesses to both DDR and DDRT
                               memories and sends appropriate indication to
                               MS2IDI over mesh AK/AD/BL wires to increment
                               corresponding PTMBE counters. These counters are
                               read by OS to throttle/unthrottle memory usage
                               on per core, per thread basis. This bit is
                               disabled by default.
                            */
    UINT32 disablesuppressinvitoeonialrsfmiss : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing a line
                               homed on an IAL accelerator device, the lookup
                               of the IAL remote snoop filter provides clear
                               indication of whether the line is residing in a
                               remote socket. If the RSF lookup is a miss, then
                               InvItoE does not need to be sent to remote
                               sockets (i.e. may be suppressed) because the RSF
                               miss clearly indicates that the other sockets
                               cannot have the line. This suppression of
                               InvItoE is the default behavior. If this bit is
                               set, the InvItoE will be sent anyway for this
                               IAL RSF miss scenario. If RSF is disabled in a
                               multi-socket system, this bit must be set.
                            */
    UINT32 rsfenabledfordram : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit to specify that requests targetting
                               DRAM Memtype is tracked by RSF. This bit has to
                               be 0 if Memtype is not tracked by RSF or Snoop
                               filter is not active.
                            */
    UINT32 rsfenabledforddrt : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit to specify that requests targetting
                               DDRT Memtype is tracked by RSF. This bit has to
                               be 0 if Memtype is not tracked by RSF or Snoop
                               filter is not active.
                            */
    UINT32 rsfenabledforhbm : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit to specify that requests targetting
                               HBM Memtype is tracked by RSF. This bit has to
                               be 0 if Memtype is not tracked by RSF or Snoop
                               filter is not active.
                            */
    UINT32 disablesuppressinvitoeonhbmrsfmiss : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing an HBM
                               line, the lookup of the remote snoop filter
                               provides clear indication of whether the line is
                               residing in a remote socket. If the RSF lookup
                               is a miss, then InvItoE does not need to be sent
                               to remote sockets (i.e. may be suppressed)
                               because the RSF miss clearly indicates that the
                               other sockets cannot have the line. This
                               suppression of InvItoE is the default behavior.
                               If this bit is set, the InvItoE will be sent
                               anyway for this IAL RSF miss scenario. If RSF is
                               disabled in a multi-socket system, this bit must
                               be set.
                            */
    UINT32 disablesnpfanoutforialmem : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               In a multi-socket system, when we hdont have a
                               fully connected system, we use snp fanout
                               feature. We send out snoop to UPI agent
                               connected to us and it will fanout snoops to all
                               the other sockets that needs to be snooped. If
                               this bit is 1 we disable Snp fanout feature for
                               IAL Mem Targets and CHA will send out snoops to
                               every socket instead of using fanout feature.
                            */
    UINT32 rsfallockwbsdata : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when we hdont have a
                               fully connected system, we use snp fanout
                               feature. We send out snoop to UPI agent
                               connected to us and it will fanout snoops to all
                               the other sockets that needs to be snooped. If
                               this bit is 1 we disable Snp fanout feature for
                               IAL Mem Targets and CHA will send out snoops to
                               every socket instead of using fanout feature.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_COH_CONFIG1_CHA_PMA_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sendsnpcodeforrdcur : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Revert PCIRdCur/KRdCur/SnpCur to send SnpCode
                               instead of SnpCurr
                            */
    UINT32 snpdatabiasmtoioptdis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Don't allocate into LLC on RspIFwdFE with
                               SF=E/SnpData
                            */
    UINT32 sendrdinvforitomcachenear : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               ItoMCacheNear will send RdInv instead of
                               RdInvOwn to guarantee it never gets M data
                            */
    UINT32 enablepushmtoiiodctwolmopt : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable PushMtoI IODC 2LM Optimization */
    UINT32 morphitomwrtomempushwr : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Morph ItoMWr to MemPushWr to prevent allocation
                               to LLC.&nbsp;
                            */
    UINT32 enableearlyfastgopull : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Send FastGO_Pull at the same time as InvItoE for
                               WCiLF and ItoMWr_WT
                            */
    UINT32 ipq_disable : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* disable IPQ reserved TOR entry */
    UINT32 rsvd : 3;

                            /* Bits[9:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disablehabypassforialmem : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Survivability hook - disable HA Bypass for
                               requests targetting IAL Mem.
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nollcenonwbmtoitoialfromms2iosf : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from MS2IOSF sources
                               that target memory homed at an IAL accelerator
                               device. Set this bit to avoid polluting the LLC;
                               when this bit is clear, LLC is allocated as for
                               non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromAny.)
                            */
    UINT32 nollcenonwbmtoitoialfromany : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly to memory) for WbMtoI, WbPushMtoI and
                               WbPMPushMtoI transactions from any source
                               (including MS2IOSF) that target memory homed at
                               an IAL accelerator device. Set this bit to avoid
                               polluting the LLC; when this bit is clear, LLC
                               is allocated as for non IAL memory. (See also
                               NoLLCEnOnWbMtoItoIalFromMs2Iosf.)
                            */
    UINT32 nollcenonwbmtoitoialfromialself : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, LLC is not allocated (i.e. write goes
                               directly back to memory) for WbMtoI and WBEFtoI
                               transactions from IAL to its own memory. When
                               this bit is clear, LLC is allocated as for non
                               IAL memory.
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 recognizeddrtdistressfromlocal : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               A memory controller may signal distress due to
                               congestion with 2LM/DDRT accesses. Distress is
                               signaled via mesh message packets as well as
                               dedicated mesh wires. This bit controls whether
                               CHA recognizes and forwards the distress
                               indication received via mesh packets from local
                               memory controllers. To avoid undesired
                               interaction between the wire and mesh packet
                               distress protocols, CHA does not recognize and
                               forward local mesh-based distress by default.
                               Setting this bit to 1, allows local mesh-based
                               distress to be seen and forwarded to the core
                               interface.
                            */
    UINT32 sfclosen : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               If this bit is set to 1, the Core Valid (CV)
                               bits in the snoop filter (SF) array will store
                               class of service (CLOS) IDs in all CV modes. If
                               set to 0, CLOS is not stored, and the SF in CV
                               mode 0 may instead store a second sharer ID.
                            */
    UINT32 enableptmbe : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Per Thread Bandwidth Enhancement (PTMBE)
                               feature by setting this bit to 1. When enabled,
                               TOR records memory accesses to both DDR and DDRT
                               memories and sends appropriate indication to
                               MS2IDI over mesh AK/AD/BL wires to increment
                               corresponding PTMBE counters. These counters are
                               read by OS to throttle/unthrottle memory usage
                               on per core, per thread basis. This bit is
                               disabled by default.
                            */
    UINT32 disablesuppressinvitoeonialrsfmiss : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               In a multi-socket system, when accessing a line
                               homed on an IAL accelerator device, the lookup
                               of the IAL remote snoop filter provides clear
                               indication of whether the line is residing in a
                               remote socket. If the RSF lookup is a miss, then
                               InvItoE does not need to be sent to remote
                               sockets (i.e. may be suppressed) because the RSF
                               miss clearly indicates that the other sockets
                               cannot have the line. This suppression of
                               InvItoE is the default behavior. If this bit is
                               set, the InvItoE will be sent anyway for this
                               IAL RSF miss scenario. If RSF is disabled in a
                               multi-socket system, this bit must be set.
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBO_COH_CONFIG1_CHA_PMA_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DBP_OBS_CR_CHA_PMA_REG supported on:                                         */
/*      SPRC0 (0x21f00258)                                                      */
/*      SPRMCC (0x21f00258)                                                     */
/*      SPRUCC (0x21f00258)                                                     */
/* Register default value on SPRC0: 0x0000001D                                  */
/* Register default value on SPRMCC: 0x0000001D                                 */
/* Register default value on SPRUCC: 0x0000001D                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRC0 BDF: 31_0_0                                      */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DBP configuration
*/


#define DBP_OBS_CR_CHA_PMA_REG 0x01020258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comparemask : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001D*/

                            /* Mask for DBP observer bit compare */
    UINT32 setnumberadjustment : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Mask DBP observer field. */
    UINT32 reserved : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /* reserved for future use. */

  } Bits;
  UINT32 Data;

} DBP_OBS_CR_CHA_PMA_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* CHA_QUIESCE_STATUS_CHA_PMA_REG supported on:                                 */
/*      SPRA0 (0x21f00260)                                                      */
/*      SPRB0 (0x21f00260)                                                      */
/*      SPRHBM (0x21f00260)                                                     */
/*      SPRC0 (0x21f00260)                                                      */
/*      SPRMCC (0x21f00260)                                                     */
/*      SPRUCC (0x21f00260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Quiesce status of various queues/structures in CHA. If a bit is set, that queue/structure is not quiesced
*/


#define CHA_QUIESCE_STATUS_CHA_PMA_REG 0x01020260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tor : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* TOR is not quiesced */
    UINT32 irq : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* IRQ is not quiesced */
    UINT32 prq : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* PRQ is not quiesced */
    UINT32 wbq : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* WBQ is not quiesced */
    UINT32 rrq : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* RRQ is not quiesced */
    UINT32 ipq : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* IPQ is not quiesced */
    UINT32 drng : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* DRNG prefetcher is not quiesced */
    UINT32 ha : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* HA is not quiesced */
    UINT32 esb : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* ESB is not quiesced */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CHA_QUIESCE_STATUS_CHA_PMA_STRUCT;

/* NUM_OF_HOPS_CHA_PMA_REG supported on:                                        */
/*      SPRA0 (0x21f00280)                                                      */
/*      SPRB0 (0x21f00280)                                                      */
/*      SPRHBM (0x21f00280)                                                     */
/*      SPRC0 (0x21f00280)                                                      */
/*      SPRMCC (0x21f00280)                                                     */
/*      SPRUCC (0x21f00280)                                                     */
/* Register default value on SPRA0: 0x00005555                                  */
/* Register default value on SPRB0: 0x00005555                                  */
/* Register default value on SPRHBM: 0x00005555                                 */
/* Register default value on SPRC0: 0x00005555                                  */
/* Register default value on SPRMCC: 0x00005555                                 */
/* Register default value on SPRUCC: 0x00005555                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/*  Core responses use the number of Hops
*/


#define NUM_OF_HOPS_CHA_PMA_REG 0x01020280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=0 */
    UINT32 ca1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=1 */
    UINT32 ca2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=2 */
    UINT32 ca3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=3 */
    UINT32 ca4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=4 */
    UINT32 ca5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=5 */
    UINT32 ca6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=6 */
    UINT32 ca7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000001*/

                            /* HOPS for SID[2:0]=7 */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NUM_OF_HOPS_CHA_PMA_STRUCT;

/* DBP_CONFIG_CHA_PMA_REG supported on:                                         */
/*      SPRA0 (0x21f00284)                                                      */
/*      SPRB0 (0x21f00284)                                                      */
/*      SPRHBM (0x21f00284)                                                     */
/*      SPRC0 (0x21f00284)                                                      */
/*      SPRMCC (0x21f00284)                                                     */
/*      SPRUCC (0x21f00284)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DBP configuration
*/


#define DBP_CONFIG_CHA_PMA_REG 0x01020284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Overall DBP enable */
    UINT32 cleanevictalwaysdead : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* CleanEvictAlwaysDead */
    UINT32 modifiedevictalwaysdead : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* ModifiedEvictAlwaysDead */
    UINT32 cleanevictalwayslive : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* CleanEvictAlwaysLive */
    UINT32 modifiedevictalwayslive : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* ModifiedEvictAlwaysLive */
    UINT32 disablecorecheck : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* DisableCoreCheck */
    UINT32 deadonvalidllc : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               1b1 - If Downgrade is set on follower do not
                               fill in LLC regardless of available LLC I-state
                               ways
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DBP_CONFIG_CHA_PMA_STRUCT;

/* TWO_LM_CONFIG_CHA_PMA_REG supported on:                                      */
/*      SPRA0 (0x21f00288)                                                      */
/*      SPRB0 (0x21f00288)                                                      */
/*      SPRHBM (0x21f00288)                                                     */
/*      SPRC0 (0x21f00288)                                                      */
/*      SPRMCC (0x21f00288)                                                     */
/*      SPRUCC (0x21f00288)                                                     */
/* Register default value on SPRA0: 0x0FFFFFF0                                  */
/* Register default value on SPRB0: 0x0FFFFFF0                                  */
/* Register default value on SPRHBM: 0x0FFFFFF0                                 */
/* Register default value on SPRC0: 0x0FFFFFF0                                  */
/* Register default value on SPRMCC: 0x0FFFFFF0                                 */
/* Register default value on SPRUCC: 0x0FFFFFF0                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Two LM Address mask and 2LM enable. &nbsp;Address mask determines which bits in the CHA are used for near memory set detection.
*/


#define TWO_LM_CONFIG_CHA_PMA_REG 0x01020288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mask : 18;

                            /* Bits[21:4], Access Type=RW, default=0x0003FFFF*/

                            /*
                               Two Level Memory Address Mask for address bits
                               [45:28]
                            */
    UINT32 mask_hi : 6;

                            /* Bits[27:22], Access Type=RW, default=0x0000003F*/

                            /*
                               Two Level Memory Address Mask for address bits
                               [51:46]
                            */
    UINT32 rsvd_28 : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Overall Two LM Enable */

  } Bits;
  UINT32 Data;

} TWO_LM_CONFIG_CHA_PMA_STRUCT;

/* REMOTE_SF_CHA_PMA_REG supported on:                                          */
/*      SPRA0 (0x21f002c0)                                                      */
/*      SPRB0 (0x21f002c0)                                                      */
/*      SPRHBM (0x21f002c0)                                                     */
/*      SPRC0 (0x21f002c0)                                                      */
/*      SPRMCC (0x21f002c0)                                                     */
/*      SPRUCC (0x21f002c0)                                                     */
/* Register default value on SPRA0: 0x0000F000                                  */
/* Register default value on SPRB0: 0x0000F000                                  */
/* Register default value on SPRHBM: 0x0000F000                                 */
/* Register default value on SPRC0: 0x0000F000                                  */
/* Register default value on SPRMCC: 0x0000F000                                 */
/* Register default value on SPRUCC: 0x0000F000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Describes configuration for the remote snoop filter (RSF) used for CXL Accelerator-owned memory regions.
*/


#define REMOTE_SF_CHA_PMA_REG 0x010202C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fpga_0_nodeid : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Node ID for FPGA0 (deprecated; does not apply to
                               CXL)
                            */
    UINT32 fpga_0_valid : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Valid for FPGA0 (deprecated; does not apply to
                               CXL)
                            */
    UINT32 fpga_1_nodeid : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Node ID for FPGA1 (deprecated; does not apply to
                               CXL)
                            */
    UINT32 fpga_1_valid : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Valid for FPGA1 (deprecated; does not apply to
                               CXL)
                            */
    UINT32 snoop_filter_active : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bit to enable Compute Express Link (CXL, a.k.a.
                               IAL) snoop filter. For single socket systems,
                               this bit should remain at its default of 0. For
                               multi-socket systems with CXL Accelerator
                               Devices, this bit must be set to 1 to enable the
                               remote snoop filter (RSF) for addresses in CXL-
                               owned memory ranges.
                            */
    UINT32 snoop_filter_size : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000000*/

                            /* FPGA Snoop Filter size encoding */
    UINT32 enable_rsf_always : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               When this bit is set we will use the RSF to
                               track requests to all lines. &nbsp;By default we
                               track just HBM lines.
                            */
    UINT32 rsvd_12 : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REMOTE_SF_CHA_PMA_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DBP_CONFIG_CFG2_N0_CHA_PMA_REG supported on:                                 */
/*      SPRC0 (0x21f002c8)                                                      */
/*      SPRMCC (0x21f002c8)                                                     */
/*      SPRUCC (0x21f002c8)                                                     */
/* Register default value on SPRC0: 0x04000064                                  */
/* Register default value on SPRMCC: 0x04000064                                 */
/* Register default value on SPRUCC: 0x04000064                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRC0 BDF: 31_0_0                                      */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DBP configuration
*/


#define DBP_CONFIG_CFG2_N0_CHA_PMA_REG 0x010202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numerofmissesthreshold : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000064*/

                            /*
                               This CSR field holds threshold value of number
                               of misses of observer LLC sets. If number of
                               misses crosses this threshold value, we can
                               start dropping clean evictions based on few
                               other condition's.
                            */
    UINT32 guardband : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Guard band to compare number of observer and non
                               observer set misses.
                            */
    UINT32 compareaganistmissrate : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               operating mode, if enabled, we make decision
                               solely based on comparing the miss rate of
                               observers sets against the
                               CR.number_of_misses_threshold set by the user
                            */
    UINT32 compareaganistothersets : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               operating mode, if enabled, we make decision
                               solely based on comparing the miss rate of
                               observer sets to non observer sets (if we set
                               miss rate to 0) OR we consider both conditions
                               if miss_rate is set to a non-zero value
                            */
    UINT32 dropcleanevictdisable : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               De-Feature bit if set to 1, new DBP algorithm
                               will not take effect.
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DBP_CONFIG_CFG2_N0_CHA_PMA_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* DBP_CONFIG_CFG2_N1_CHA_PMA_REG supported on:                                 */
/*      SPRC0 (0x21f002cc)                                                      */
/*      SPRMCC (0x21f002cc)                                                     */
/*      SPRUCC (0x21f002cc)                                                     */
/* Register default value on SPRC0: 0x00000096                                  */
/* Register default value on SPRMCC: 0x00000096                                 */
/* Register default value on SPRUCC: 0x00000096                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_UNICAST_GROUP1_0x000_0x2ff*/
/* Struct generated from SPRC0 BDF: 31_0_0                                      */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DBP configuration
*/


#define DBP_CONFIG_CFG2_N1_CHA_PMA_REG 0x010202CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 epochlength : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000096*/

                            /*
                               This CSR field is used as a threshold for number
                               of LLC access we have to wait before DBP logic
                               kicks in to figure out if we have to start
                               dropping clean evicts.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DBP_CONFIG_CFG2_N1_CHA_PMA_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */



#endif /* _CHA_PMA_h */
