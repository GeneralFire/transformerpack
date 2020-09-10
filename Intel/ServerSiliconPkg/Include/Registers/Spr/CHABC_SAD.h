
/** @file
  CHABC_SAD.h

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
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* BOOT_W                                                                       */
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
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/* PAM_W                                                                        */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |                           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _CHABC_SAD_h
#define _CHABC_SAD_h
#include <Base.h>

/* VID_CHABC_SAD_REG supported on:                                              */
/*      SPRA0 (0x11fe8000)                                                      */
/*      SPRB0 (0x11fe8000)                                                      */
/*      SPRHBM (0x11fe8000)                                                     */
/*      SPRC0 (0x11fe8000)                                                      */
/*      SPRMCC (0x11fe8000)                                                     */
/*      SPRUCC (0x11fe8000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_CHABC_SAD_REG 0x03010000

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

} VID_CHABC_SAD_STRUCT;

/* DID_CHABC_SAD_REG supported on:                                              */
/*      SPRA0 (0x11fe8002)                                                      */
/*      SPRB0 (0x11fe8002)                                                      */
/*      SPRHBM (0x11fe8002)                                                     */
/*      SPRC0 (0x11fe8002)                                                      */
/*      SPRMCC (0x11fe8002)                                                     */
/*      SPRUCC (0x11fe8002)                                                     */
/* Register default value on SPRA0: 0x0000344F                                  */
/* Register default value on SPRB0: 0x0000344F                                  */
/* Register default value on SPRHBM: 0x0000344F                                 */
/* Register default value on SPRC0: 0x0000344F                                  */
/* Register default value on SPRMCC: 0x0000344F                                 */
/* Register default value on SPRUCC: 0x0000344F                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_CHABC_SAD_REG 0x03010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000344F*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_CHABC_SAD_STRUCT;

/* PCICMD_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x11fe8004)                                                      */
/*      SPRB0 (0x11fe8004)                                                      */
/*      SPRHBM (0x11fe8004)                                                     */
/*      SPRC0 (0x11fe8004)                                                      */
/*      SPRMCC (0x11fe8004)                                                     */
/*      SPRUCC (0x11fe8004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_CHABC_SAD_REG 0x03010004

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

} PCICMD_CHABC_SAD_STRUCT;

/* PCISTS_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x11fe8006)                                                      */
/*      SPRB0 (0x11fe8006)                                                      */
/*      SPRHBM (0x11fe8006)                                                     */
/*      SPRC0 (0x11fe8006)                                                      */
/*      SPRMCC (0x11fe8006)                                                     */
/*      SPRUCC (0x11fe8006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_CHABC_SAD_REG 0x03010006

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

} PCISTS_CHABC_SAD_STRUCT;

/* RID_CCR_CHABC_SAD_REG supported on:                                          */
/*      SPRA0 (0x21fe8008)                                                      */
/*      SPRB0 (0x21fe8008)                                                      */
/*      SPRHBM (0x21fe8008)                                                     */
/*      SPRC0 (0x21fe8008)                                                      */
/*      SPRMCC (0x21fe8008)                                                     */
/*      SPRUCC (0x21fe8008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_CHABC_SAD_REG 0x03020008

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

} RID_CCR_CHABC_SAD_STRUCT;

/* CLSR_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x1fe800c)                                                       */
/*      SPRB0 (0x1fe800c)                                                       */
/*      SPRHBM (0x1fe800c)                                                      */
/*      SPRC0 (0x1fe800c)                                                       */
/*      SPRMCC (0x1fe800c)                                                      */
/*      SPRUCC (0x1fe800c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_CHABC_SAD_REG 0x0300000C

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

} CLSR_CHABC_SAD_STRUCT;

/* PLAT_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x1fe800d)                                                       */
/*      SPRB0 (0x1fe800d)                                                       */
/*      SPRHBM (0x1fe800d)                                                      */
/*      SPRC0 (0x1fe800d)                                                       */
/*      SPRMCC (0x1fe800d)                                                      */
/*      SPRUCC (0x1fe800d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_CHABC_SAD_REG 0x0300000D

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

} PLAT_CHABC_SAD_STRUCT;

/* HDR_CHABC_SAD_REG supported on:                                              */
/*      SPRA0 (0x1fe800e)                                                       */
/*      SPRB0 (0x1fe800e)                                                       */
/*      SPRHBM (0x1fe800e)                                                      */
/*      SPRC0 (0x1fe800e)                                                       */
/*      SPRMCC (0x1fe800e)                                                      */
/*      SPRUCC (0x1fe800e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_CHABC_SAD_REG 0x0300000E

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

} HDR_CHABC_SAD_STRUCT;

/* BIST_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x1fe800f)                                                       */
/*      SPRB0 (0x1fe800f)                                                       */
/*      SPRHBM (0x1fe800f)                                                      */
/*      SPRC0 (0x1fe800f)                                                       */
/*      SPRMCC (0x1fe800f)                                                      */
/*      SPRUCC (0x1fe800f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_CHABC_SAD_REG 0x0300000F

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

} BIST_CHABC_SAD_STRUCT;

/* SVID_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x11fe802c)                                                      */
/*      SPRB0 (0x11fe802c)                                                      */
/*      SPRHBM (0x11fe802c)                                                     */
/*      SPRC0 (0x11fe802c)                                                      */
/*      SPRMCC (0x11fe802c)                                                     */
/*      SPRUCC (0x11fe802c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_CHABC_SAD_REG 0x0301002C

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

} SVID_CHABC_SAD_STRUCT;

/* SDID_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x11fe802e)                                                      */
/*      SPRB0 (0x11fe802e)                                                      */
/*      SPRHBM (0x11fe802e)                                                     */
/*      SPRC0 (0x11fe802e)                                                      */
/*      SPRMCC (0x11fe802e)                                                     */
/*      SPRUCC (0x11fe802e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_CHABC_SAD_REG 0x0301002E

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

} SDID_CHABC_SAD_STRUCT;

/* CAPPTR_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x1fe8034)                                                       */
/*      SPRB0 (0x1fe8034)                                                       */
/*      SPRHBM (0x1fe8034)                                                      */
/*      SPRC0 (0x1fe8034)                                                       */
/*      SPRMCC (0x1fe8034)                                                      */
/*      SPRUCC (0x1fe8034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_CHABC_SAD_REG 0x03000034

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

} CAPPTR_CHABC_SAD_STRUCT;

/* INTL_CHABC_SAD_REG supported on:                                             */
/*      SPRA0 (0x1fe803c)                                                       */
/*      SPRB0 (0x1fe803c)                                                       */
/*      SPRHBM (0x1fe803c)                                                      */
/*      SPRC0 (0x1fe803c)                                                       */
/*      SPRMCC (0x1fe803c)                                                      */
/*      SPRUCC (0x1fe803c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_CHABC_SAD_REG 0x0300003C

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

} INTL_CHABC_SAD_STRUCT;

/* INTPIN_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x1fe803d)                                                       */
/*      SPRB0 (0x1fe803d)                                                       */
/*      SPRHBM (0x1fe803d)                                                      */
/*      SPRC0 (0x1fe803d)                                                       */
/*      SPRMCC (0x1fe803d)                                                      */
/*      SPRUCC (0x1fe803d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_CHABC_SAD_REG 0x0300003D

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

} INTPIN_CHABC_SAD_STRUCT;

/* MINGNT_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x1fe803e)                                                       */
/*      SPRB0 (0x1fe803e)                                                       */
/*      SPRHBM (0x1fe803e)                                                      */
/*      SPRC0 (0x1fe803e)                                                       */
/*      SPRMCC (0x1fe803e)                                                      */
/*      SPRUCC (0x1fe803e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_CHABC_SAD_REG 0x0300003E

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

} MINGNT_CHABC_SAD_STRUCT;

/* MAXLAT_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x1fe803f)                                                       */
/*      SPRB0 (0x1fe803f)                                                       */
/*      SPRHBM (0x1fe803f)                                                      */
/*      SPRC0 (0x1fe803f)                                                       */
/*      SPRMCC (0x1fe803f)                                                      */
/*      SPRUCC (0x1fe803f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_CHABC_SAD_REG 0x0300003F

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

} MAXLAT_CHABC_SAD_STRUCT;

/* PXPCAP_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x21fe8040)                                                      */
/*      SPRB0 (0x21fe8040)                                                      */
/*      SPRHBM (0x21fe8040)                                                     */
/*      SPRC0 (0x21fe8040)                                                      */
/*      SPRMCC (0x21fe8040)                                                     */
/*      SPRUCC (0x21fe8040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_CHABC_SAD_REG 0x03020040

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

} PXPCAP_CHABC_SAD_STRUCT;

/* PCIE_RSVD0_CHABC_SAD_REG supported on:                                       */
/*      SPRA0 (0x21fe8044)                                                      */
/*      SPRB0 (0x21fe8044)                                                      */
/*      SPRHBM (0x21fe8044)                                                     */
/*      SPRC0 (0x21fe8044)                                                      */
/*      SPRMCC (0x21fe8044)                                                     */
/*      SPRUCC (0x21fe8044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_CHABC_SAD_REG 0x03020044

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

} PCIE_RSVD0_CHABC_SAD_STRUCT;

/* PCIE_RSVD1_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8048)                                                      */
/*      SPRB0 (0x21fe8048)                                                      */
/*      SPRHBM (0x21fe8048)                                                     */
/*      SPRC0 (0x21fe8048)                                                      */
/*      SPRMCC (0x21fe8048)                                                     */
/*      SPRUCC (0x21fe8048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_N0_CHABC_SAD_REG 0x03020048

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

} PCIE_RSVD1_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD1_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe804c)                                                      */
/*      SPRB0 (0x21fe804c)                                                      */
/*      SPRHBM (0x21fe804c)                                                     */
/*      SPRC0 (0x21fe804c)                                                      */
/*      SPRMCC (0x21fe804c)                                                     */
/*      SPRUCC (0x21fe804c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_N1_CHABC_SAD_REG 0x0302004C

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

} PCIE_RSVD1_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD2_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8050)                                                      */
/*      SPRB0 (0x21fe8050)                                                      */
/*      SPRHBM (0x21fe8050)                                                     */
/*      SPRC0 (0x21fe8050)                                                      */
/*      SPRMCC (0x21fe8050)                                                     */
/*      SPRUCC (0x21fe8050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_N0_CHABC_SAD_REG 0x03020050

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

} PCIE_RSVD2_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD2_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8054)                                                      */
/*      SPRB0 (0x21fe8054)                                                      */
/*      SPRHBM (0x21fe8054)                                                     */
/*      SPRC0 (0x21fe8054)                                                      */
/*      SPRMCC (0x21fe8054)                                                     */
/*      SPRUCC (0x21fe8054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_N1_CHABC_SAD_REG 0x03020054

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

} PCIE_RSVD2_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD3_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8058)                                                      */
/*      SPRB0 (0x21fe8058)                                                      */
/*      SPRHBM (0x21fe8058)                                                     */
/*      SPRC0 (0x21fe8058)                                                      */
/*      SPRMCC (0x21fe8058)                                                     */
/*      SPRUCC (0x21fe8058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_N0_CHABC_SAD_REG 0x03020058

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

} PCIE_RSVD3_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD3_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe805c)                                                      */
/*      SPRB0 (0x21fe805c)                                                      */
/*      SPRHBM (0x21fe805c)                                                     */
/*      SPRC0 (0x21fe805c)                                                      */
/*      SPRMCC (0x21fe805c)                                                     */
/*      SPRUCC (0x21fe805c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_N1_CHABC_SAD_REG 0x0302005C

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

} PCIE_RSVD3_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD4_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8060)                                                      */
/*      SPRB0 (0x21fe8060)                                                      */
/*      SPRHBM (0x21fe8060)                                                     */
/*      SPRC0 (0x21fe8060)                                                      */
/*      SPRMCC (0x21fe8060)                                                     */
/*      SPRUCC (0x21fe8060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_N0_CHABC_SAD_REG 0x03020060

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

} PCIE_RSVD4_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD4_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8064)                                                      */
/*      SPRB0 (0x21fe8064)                                                      */
/*      SPRHBM (0x21fe8064)                                                     */
/*      SPRC0 (0x21fe8064)                                                      */
/*      SPRMCC (0x21fe8064)                                                     */
/*      SPRUCC (0x21fe8064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_N1_CHABC_SAD_REG 0x03020064

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

} PCIE_RSVD4_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD5_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8068)                                                      */
/*      SPRB0 (0x21fe8068)                                                      */
/*      SPRHBM (0x21fe8068)                                                     */
/*      SPRC0 (0x21fe8068)                                                      */
/*      SPRMCC (0x21fe8068)                                                     */
/*      SPRUCC (0x21fe8068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_N0_CHABC_SAD_REG 0x03020068

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

} PCIE_RSVD5_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD5_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe806c)                                                      */
/*      SPRB0 (0x21fe806c)                                                      */
/*      SPRHBM (0x21fe806c)                                                     */
/*      SPRC0 (0x21fe806c)                                                      */
/*      SPRMCC (0x21fe806c)                                                     */
/*      SPRUCC (0x21fe806c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_N1_CHABC_SAD_REG 0x0302006C

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

} PCIE_RSVD5_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD6_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8070)                                                      */
/*      SPRB0 (0x21fe8070)                                                      */
/*      SPRHBM (0x21fe8070)                                                     */
/*      SPRC0 (0x21fe8070)                                                      */
/*      SPRMCC (0x21fe8070)                                                     */
/*      SPRUCC (0x21fe8070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_N0_CHABC_SAD_REG 0x03020070

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

} PCIE_RSVD6_N0_CHABC_SAD_STRUCT;

/* PCIE_RSVD6_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8074)                                                      */
/*      SPRB0 (0x21fe8074)                                                      */
/*      SPRHBM (0x21fe8074)                                                     */
/*      SPRC0 (0x21fe8074)                                                      */
/*      SPRMCC (0x21fe8074)                                                     */
/*      SPRUCC (0x21fe8074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_N1_CHABC_SAD_REG 0x03020074

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

} PCIE_RSVD6_N1_CHABC_SAD_STRUCT;

/* PCIE_RSVD7_CHABC_SAD_REG supported on:                                       */
/*      SPRA0 (0x21fe8078)                                                      */
/*      SPRB0 (0x21fe8078)                                                      */
/*      SPRHBM (0x21fe8078)                                                     */
/*      SPRC0 (0x21fe8078)                                                      */
/*      SPRMCC (0x21fe8078)                                                     */
/*      SPRUCC (0x21fe8078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_CHABC_SAD_REG 0x03020078

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

} PCIE_RSVD7_CHABC_SAD_STRUCT;

/* PAM0123_CHABC_SAD_REG supported on:                                          */
/*      SPRA0 (0x21fe8080)                                                      */
/*      SPRB0 (0x21fe8080)                                                      */
/*      SPRHBM (0x21fe8080)                                                     */
/*      SPRC0 (0x21fe8080)                                                      */
/*      SPRMCC (0x21fe8080)                                                     */
/*      SPRUCC (0x21fe8080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM0123 Configuration register.  All fields of this register is locked with both LTCtrlSts.ConfigLock and PAM0123_CFG.Lock are set.  This lock is bypassable with SMM writes.
      
*/


#define PAM0123_CHABC_SAD_REG 0x03020080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam0_hienable : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               0F0000-0FFFFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0F0000 to
                               0FFFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM. Note that PAM0 covers BIOS regions
                               (F0000-FFFFF range). An INIT may send the
                               processor to jump to FFFF0 address. Therefore,
                               it is a good idea to protect PAM0 range.
                               Generally, BIOS does not need to change F region
                               after it has locked configuration. Other PAM
                               regions are used for shadowing option ROMs. That
                               process happens after config locking and
                               requires changes to these PAM regs.
                            */
    UINT32 rsvd_6 : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam1_loenable : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               0C0000-0C3FFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0C0000 to
                               0C3FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam1_hienable : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               0C4000-0C7FFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0C4000 to
                               0C7FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam2_loenable : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               0C8000-0CBFFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0C8000 to
                               0CBFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam2_hienable : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               0CC000-0CFFFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0CC000 to
                               0CFFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam3_loenable : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               0D0000-0D3FFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0D0000 to
                               0D3FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_26 : 2;

                            /* Bits[27:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam3_hienable : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               0D4000-0D7FFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0D4000 to
                               0D7FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PAM0123_CHABC_SAD_STRUCT;

/* PAM456_CHABC_SAD_REG supported on:                                           */
/*      SPRA0 (0x21fe8084)                                                      */
/*      SPRB0 (0x21fe8084)                                                      */
/*      SPRHBM (0x21fe8084)                                                     */
/*      SPRC0 (0x21fe8084)                                                      */
/*      SPRMCC (0x21fe8084)                                                     */
/*      SPRUCC (0x21fe8084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM456 Configuration register.  All fields of this register is locked with both LTCtrlSts.ConfigLock and PAM0123_CFG.Lock are set.  This lock is bypassable with SMM writes.
      
*/


#define PAM456_CHABC_SAD_REG 0x03020084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pam4_loenable : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               0D8000-0DBFFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0D8000 to
                               0DBFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam4_hienable : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               0DC000-0DFFFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0DC000 to
                               0DFFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_6 : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam5_loenable : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               0E0000-0E3FFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0E0000 to
                               0E3FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam5_hienable : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               0E4000-0E7FFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0E4000 to
                               0E7FFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam6_loenable : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               0E8000-0EBFFF Attribute (LOENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0E8000 to
                               0EBFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pam6_hienable : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               0EC000-0EFFFF Attribute (HIENABLE) This field
                               controls the steering of read and write cycles
                               that address the BIOS area from 0EC000 to
                               0EFFFF. 00: DRAM Disabled: All accesses are
                               directed to DMI. 01: Read Only: All reads are
                               sent to DRAM. All writes are forwarded to DMI.
                               10: Write Only: All writes are send to DRAM.
                               Reads are serviced by DMI. 11: Normal DRAM
                               Operation: All reads and writes are serviced by
                               DRAM.
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PAM456_CHABC_SAD_STRUCT;

/* SNC_LOCK_CHABC_SAD_REG supported on:                                         */
/*      SPRA0 (0x21fe8088)                                                      */
/*      SPRB0 (0x21fe8088)                                                      */
/*      SPRHBM (0x21fe8088)                                                     */
/*      SPRC0 (0x21fe8088)                                                      */
/*      SPRMCC (0x21fe8088)                                                     */
/*      SPRUCC (0x21fe8088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This register indicates which of the SNCs' have their DRAM Rule registers locked and prevented from being overriden.
*/


#define SNC_LOCK_CHABC_SAD_REG 0x03020088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snc_lock : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* Bit N indicates SNC N is locked. */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_LOCK_CHABC_SAD_STRUCT;

/* MESEG_BASE_N0_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8090)                                                      */
/*      SPRB0 (0x21fe8090)                                                      */
/*      SPRHBM (0x21fe8090)                                                     */
/*      SPRC0 (0x21fe8090)                                                      */
/*      SPRMCC (0x21fe8090)                                                     */
/*      SPRUCC (0x21fe8090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Corresponds to A[51:19] of the base address memory range that is allocated to the ME.
*/


#define MESEG_BASE_N0_CHABC_SAD_REG 0x03020090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 19;

                            /* Bits[18:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mebase : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the base address
                               memory range that is allocated to the ME.
                            */

  } Bits;
  UINT32 Data;

} MESEG_BASE_N0_CHABC_SAD_STRUCT;

/* MESEG_BASE_N1_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8094)                                                      */
/*      SPRB0 (0x21fe8094)                                                      */
/*      SPRHBM (0x21fe8094)                                                     */
/*      SPRC0 (0x21fe8094)                                                      */
/*      SPRMCC (0x21fe8094)                                                     */
/*      SPRUCC (0x21fe8094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Corresponds to A[51:19] of the base address memory range that is allocated to the ME.
*/


#define MESEG_BASE_N1_CHABC_SAD_REG 0x03020094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mebase : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the base address
                               memory range that is allocated to the ME.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MESEG_BASE_N1_CHABC_SAD_STRUCT;

/* MESEG_LIMIT_N0_CHABC_SAD_REG supported on:                                   */
/*      SPRA0 (0x21fe8098)                                                      */
/*      SPRB0 (0x21fe8098)                                                      */
/*      SPRHBM (0x21fe8098)                                                     */
/*      SPRC0 (0x21fe8098)                                                      */
/*      SPRMCC (0x21fe8098)                                                     */
/*      SPRUCC (0x21fe8098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Indicates whether the ME Stolen Memory range is enabled or not and Corresponds to A[51:19] of the limit address memory range that is allocated to the ME. Minimum granularity is 1MB for this region.
*/


#define MESEG_LIMIT_N0_CHABC_SAD_REG 0x03020098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates whether the ME Stolen Memory range is
                               enabled or not. When enabled, all IA access to
                               this range must be aborted.
                            */
    UINT32 rsvd_12 : 7;

                            /* Bits[18:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 melimit : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the limit address
                               memory range that is allocated to the ME.
                               Minimum granularity is 1MB for this region.
                            */

  } Bits;
  UINT32 Data;

} MESEG_LIMIT_N0_CHABC_SAD_STRUCT;

/* MESEG_LIMIT_N1_CHABC_SAD_REG supported on:                                   */
/*      SPRA0 (0x21fe809c)                                                      */
/*      SPRB0 (0x21fe809c)                                                      */
/*      SPRHBM (0x21fe809c)                                                     */
/*      SPRC0 (0x21fe809c)                                                      */
/*      SPRMCC (0x21fe809c)                                                     */
/*      SPRUCC (0x21fe809c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Indicates whether the ME Stolen Memory range is enabled or not and Corresponds to A[51:19] of the limit address memory range that is allocated to the ME. Minimum granularity is 1MB for this region.
*/


#define MESEG_LIMIT_N1_CHABC_SAD_REG 0x0302009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 melimit : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the limit address
                               memory range that is allocated to the ME.
                               Minimum granularity is 1MB for this region.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MESEG_LIMIT_N1_CHABC_SAD_STRUCT;

/* SMMSEG_BASE_N0_CHABC_SAD_REG supported on:                                   */
/*      SPRA0 (0x21fe80a0)                                                      */
/*      SPRB0 (0x21fe80a0)                                                      */
/*      SPRHBM (0x21fe80a0)                                                     */
/*      SPRC0 (0x21fe80a0)                                                      */
/*      SPRMCC (0x21fe80a0)                                                     */
/*      SPRUCC (0x21fe80a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Corresponds to A[51:19] of the base address memory range that is allocated to the SMM Memory Segment.
*/


#define SMMSEG_BASE_N0_CHABC_SAD_REG 0x030200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 19;

                            /* Bits[18:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 smmbase : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the base address
                               memory range that is allocated to the SMM Memory
                               Segment.
                            */

  } Bits;
  UINT32 Data;

} SMMSEG_BASE_N0_CHABC_SAD_STRUCT;

/* SMMSEG_BASE_N1_CHABC_SAD_REG supported on:                                   */
/*      SPRA0 (0x21fe80a4)                                                      */
/*      SPRB0 (0x21fe80a4)                                                      */
/*      SPRHBM (0x21fe80a4)                                                     */
/*      SPRC0 (0x21fe80a4)                                                      */
/*      SPRMCC (0x21fe80a4)                                                     */
/*      SPRUCC (0x21fe80a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Corresponds to A[51:19] of the base address memory range that is allocated to the SMM Memory Segment.
*/


#define SMMSEG_BASE_N1_CHABC_SAD_REG 0x030200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smmbase : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the base address
                               memory range that is allocated to the SMM Memory
                               Segment.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMMSEG_BASE_N1_CHABC_SAD_STRUCT;

/* SMMSEG_LIMIT_N0_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe80a8)                                                      */
/*      SPRB0 (0x21fe80a8)                                                      */
/*      SPRHBM (0x21fe80a8)                                                     */
/*      SPRC0 (0x21fe80a8)                                                      */
/*      SPRMCC (0x21fe80a8)                                                     */
/*      SPRUCC (0x21fe80a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Indicates whether the SMM Stolen Memory range is enabled or not. When enabled, all access from MS2IDI agents that are not IA-cores must be aborted. Also it corresponds to  A[51:19] of the limit address memory range allocated to the ME. Minimum granularity is 1MB for this region
*/


#define SMMSEG_LIMIT_N0_CHABC_SAD_REG 0x030200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates whether the SMM Segment Stolen Memory
                               range is enabled or not. When enabled, all
                               access from MS2IDI agents that are not IA-cores
                               must be aborted.
                            */
    UINT32 rsvd_12 : 7;

                            /* Bits[18:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 smmlimit : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the limit address
                               memory range that is allocated to the ME.
                               Minimum granularity is 1MB for this region.
                            */

  } Bits;
  UINT32 Data;

} SMMSEG_LIMIT_N0_CHABC_SAD_STRUCT;

/* SMMSEG_LIMIT_N1_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe80ac)                                                      */
/*      SPRB0 (0x21fe80ac)                                                      */
/*      SPRHBM (0x21fe80ac)                                                     */
/*      SPRC0 (0x21fe80ac)                                                      */
/*      SPRMCC (0x21fe80ac)                                                     */
/*      SPRUCC (0x21fe80ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Indicates whether the SMM Stolen Memory range is enabled or not. When enabled, all access from MS2IDI agents that are not IA-cores must be aborted. Also it corresponds to  A[51:19] of the limit address memory range allocated to the ME. Minimum granularity is 1MB for this region
*/


#define SMMSEG_LIMIT_N1_CHABC_SAD_REG 0x030200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smmlimit : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Corresponds to A[51:19] of the limit address
                               memory range that is allocated to the ME.
                               Minimum granularity is 1MB for this region.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMMSEG_LIMIT_N1_CHABC_SAD_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* FZM_CAPABILITY_CHABC_SAD_REG supported on:                                   */
/*      SPRB0 (0x21fe80b0)                                                      */
/*      SPRHBM (0x21fe80b0)                                                     */
/*      SPRC0 (0x21fe80b0)                                                      */
/*      SPRMCC (0x21fe80b0)                                                     */
/*      SPRUCC (0x21fe80b0)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRB0 BDF: 31_29_0                                     */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This register effectively implements a scratchpad for BIOS to record which memory region, as defined by the general purpose DRAM rules, is fast-zero capable.
*/


#define FZM_CAPABILITY_CHABC_SAD_REG 0x030200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_vector : 16;

                            /* Bits[15:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               BIOS may use this field, one bit per general-
                               purpose DRAM rule, to record which memory
                               regions are fast-zero capable. (Hardware does
                               not read or write this register field.)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FZM_CAPABILITY_CHABC_SAD_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* PXPENHCAP_CHABC_SAD_REG supported on:                                        */
/*      SPRA0 (0x21fe8100)                                                      */
/*      SPRB0 (0x21fe8100)                                                      */
/*      SPRHBM (0x21fe8100)                                                     */
/*      SPRC0 (0x21fe8100)                                                      */
/*      SPRMCC (0x21fe8100)                                                     */
/*      SPRUCC (0x21fe8100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_CHABC_SAD_REG 0x03020100

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

} PXPENHCAP_CHABC_SAD_STRUCT;

/* DRNG_PREFETCH_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8108)                                                      */
/*      SPRB0 (0x21fe8108)                                                      */
/*      SPRHBM (0x21fe8108)                                                     */
/*      SPRC0 (0x21fe8108)                                                      */
/*      SPRMCC (0x21fe8108)                                                     */
/*      SPRUCC (0x21fe8108)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* <p>DRNG prefetch threshold</p>
*/


#define DRNG_PREFETCH_CHABC_SAD_REG 0x03020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 drng_prefetch_threshold : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               The prefetch threshold allows the number of
                               prefetch requests to be reduced from the maximum
                               value. The prefetch threshold value of 16
                               prefetch maximum can be overwritten via this
                               CSR. A prefetch may be issued if the number of
                               inflight prefetches + valid prefetch buffers <
                               drng_prefetch_threshold. drng_prefetch_threshold
                               can be programmed to 0 to disable drng prefetch.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRNG_PREFETCH_CHABC_SAD_STRUCT;

/* GFX_REMOTE_SM_BASE_CFG_0_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe8110)                                                      */
/*      SPRB0 (0x21fe8110)                                                      */
/*      SPRHBM (0x21fe8110)                                                     */
/*      SPRC0 (0x21fe8110)                                                      */
/*      SPRMCC (0x21fe8110)                                                     */
/*      SPRUCC (0x21fe8110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base address for the specified graphics stolen memory range. For the REMOTE version of this register, this is the base address of the remote socket's graphics stolen memory. For the LOCAL version of this register, there will be one copy for each Gen instance on the die and version N of the local version of this register is the base address of the stolen memory for the Nth Gen instance.
*/


#define GFX_REMOTE_SM_BASE_CFG_0_CHABC_SAD_REG 0x03020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               enable bit for this graphics stolen memory
                               range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_address : 29;

                            /* Bits[31:3], Access Type=RW, default=0x00000000*/

                            /*
                               bits [51:23] of the base address for this
                               graphics stolen memory range.
                            */

  } Bits;
  UINT32 Data;

} GFX_REMOTE_SM_BASE_CFG_0_CHABC_SAD_STRUCT;

/* GFX_REMOTE_SM_BASE_CFG_1_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe8114)                                                      */
/*      SPRB0 (0x21fe8114)                                                      */
/*      SPRHBM (0x21fe8114)                                                     */
/*      SPRC0 (0x21fe8114)                                                      */
/*      SPRMCC (0x21fe8114)                                                     */
/*      SPRUCC (0x21fe8114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base address for the specified graphics stolen memory range. For the REMOTE version of this register, this is the base address of the remote socket's graphics stolen memory. For the LOCAL version of this register, there will be one copy for each Gen instance on the die and version N of the local version of this register is the base address of the stolen memory for the Nth Gen instance.
*/


#define GFX_REMOTE_SM_BASE_CFG_1_CHABC_SAD_REG 0x03020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               enable bit for this graphics stolen memory
                               range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_address : 29;

                            /* Bits[31:3], Access Type=RW, default=0x00000000*/

                            /*
                               bits [51:23] of the base address for this
                               graphics stolen memory range.
                            */

  } Bits;
  UINT32 Data;

} GFX_REMOTE_SM_BASE_CFG_1_CHABC_SAD_STRUCT;

/* GFX_REMOTE_SM_LIMIT_CFG_0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe8118)                                                      */
/*      SPRB0 (0x21fe8118)                                                      */
/*      SPRHBM (0x21fe8118)                                                     */
/*      SPRC0 (0x21fe8118)                                                      */
/*      SPRMCC (0x21fe8118)                                                     */
/*      SPRUCC (0x21fe8118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* The base address of the non-stolen memory that is immediately above the corresponding graphics stolen memory ranges.
For the REMOTE version of this register the limit of the remote socket's graphics stolen memory is equal to one less than the value in this register.
For the LOCAL version of this register the limit of the local socket's graphics stolen memory is equal to one less than the value in this register. &nbsp;Note, when there are multiple graphics agents in the local socket, the limit of agent N's stolen memory is one less than the base of agent N+1's stolen memory. &nbsp;But the limit of the last graphics agent's stolen memory range is equal to one less than the value in this register. &nbsp;
*/


#define GFX_REMOTE_SM_LIMIT_CFG_0_CHABC_SAD_REG 0x03020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 non_stolen_base_address : 29;

                            /* Bits[28:0], Access Type=RW, default=0x00000000*/

                            /*
                               bits [51:23] of the base address for the non-
                               stolen memory range that sits above the local
                               socket's graphics stolen memory.
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GFX_REMOTE_SM_LIMIT_CFG_0_CHABC_SAD_STRUCT;

/* GFX_REMOTE_SM_LIMIT_CFG_1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe811c)                                                      */
/*      SPRB0 (0x21fe811c)                                                      */
/*      SPRHBM (0x21fe811c)                                                     */
/*      SPRC0 (0x21fe811c)                                                      */
/*      SPRMCC (0x21fe811c)                                                     */
/*      SPRUCC (0x21fe811c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* The base address of the non-stolen memory that is immediately above the corresponding graphics stolen memory ranges.
For the REMOTE version of this register the limit of the remote socket's graphics stolen memory is equal to one less than the value in this register.
For the LOCAL version of this register the limit of the local socket's graphics stolen memory is equal to one less than the value in this register. &nbsp;Note, when there are multiple graphics agents in the local socket, the limit of agent N's stolen memory is one less than the base of agent N+1's stolen memory. &nbsp;But the limit of the last graphics agent's stolen memory range is equal to one less than the value in this register. &nbsp;
*/


#define GFX_REMOTE_SM_LIMIT_CFG_1_CHABC_SAD_REG 0x0302011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 non_stolen_base_address : 29;

                            /* Bits[28:0], Access Type=RW, default=0x00000000*/

                            /*
                               bits [51:23] of the base address for the non-
                               stolen memory range that sits above the local
                               socket's graphics stolen memory.
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GFX_REMOTE_SM_LIMIT_CFG_1_CHABC_SAD_STRUCT;

/* LLCHITINTENSE_CHABC_SAD_REG supported on:                                    */
/*      SPRA0 (0x21fe8128)                                                      */
/*      SPRB0 (0x21fe8128)                                                      */
/*      SPRHBM (0x21fe8128)                                                     */
/*      SPRC0 (0x21fe8128)                                                      */
/*      SPRMCC (0x21fe8128)                                                     */
/*      SPRUCC (0x21fe8128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register is used to program thresholds to  detect the LLCHitIntense conditions and assert the signal in CHA
*/


#define LLCHITINTENSE_CHABC_SAD_REG 0x03020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 detectsnoopandllchit : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, we need to detetc core/HA snoops
                               and LLCHit accesses. When 0, the effect of
                               LLCHitIntense Workload is ignored on clock
                               modulation altogether
                            */
    UINT32 llchit_threshold : 5;

                            /* Bits[5:1], Access Type=RW, default=0x00000000*/

                            /*
                               If LLCHitCnt in CHA > LLCHit_Theshold (value
                               programmed in this field) the LLCHitIntense
                               signal is set. LLCHitIntense signal indicates if
                               we have LLC Hit Intensive Workloads and that
                               affects clock modulation rates.
                            */
    UINT32 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 llcmiss_threshold : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /*
                               This threshold is used to clear the
                               LLCHitIntense indication in the CHA when the
                               number of misses reach LLCMiss_Threshold if the
                               LLCHitCnt <= LLCHit_Threshold LLCHitIntense
                               signal indicates if we have LLC Hit Intensive
                               Workloads and that affects clock modulation
                               rates.
                            */
    UINT32 en_snoop_for_hitintensewl : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, core/HA snoops are used to
                               determine if the Workload is HitIntense. When
                               set to 0, the effect of Snoops is not present on
                               determining LLCHitIntense Workload or not.
                            */
    UINT32 enable_llchitintenseonclkmod : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the effect of LLCHitIntense is also
                               applied to determine whether to clkmod or not.
                            */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LLCHITINTENSE_CHABC_SAD_STRUCT;

/* CLOCK_MODULATION_RATE_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8138)                                                      */
/*      SPRB0 (0x21fe8138)                                                      */
/*      SPRHBM (0x21fe8138)                                                     */
/*      SPRC0 (0x21fe8138)                                                      */
/*      SPRMCC (0x21fe8138)                                                     */
/*      SPRUCC (0x21fe8138)                                                     */
/* Register default value on SPRA0: 0x00025000                                  */
/* Register default value on SPRB0: 0x00025000                                  */
/* Register default value on SPRHBM: 0x00025000                                 */
/* Register default value on SPRC0: 0x00025000                                  */
/* Register default value on SPRMCC: 0x00025000                                 */
/* Register default value on SPRUCC: 0x00025000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* clock modulation rate
*/


#define CLOCK_MODULATION_RATE_CHABC_SAD_REG 0x03020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idle_clockmod_rate : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This describes idle clock modulation rate.The
                               value is programmed using the below table. F No
                               Modulation E 99% D 90% C 80% B 75% A 67% 9 50% 8
                               No Modulation 7 No Modulation 6 1% 5 10% 4 20% 3
                               25% 2 33% 1 50% 0 No Modulation Eg: 33%
                               modulation means there is 1 clock pulse for
                               every 3 free running clocks. 80% modulation
                               means there are 4 clock pulses for every 5 free
                               running clocks.
                            */
    UINT32 med_clockmod_rate : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This describes Medium clock modulation rate.The
                               value is programmed using the below table. F No
                               Modulation E 99% D 90% C 80% B 75% A 67% 9 50% 8
                               No Modulation 7 No Modulation 6 1% 5 10% 4 20% 3
                               25% 2 33% 1 50% 0 No Modulation Eg: 33%
                               modulation means there is 1 clock pulse for
                               every 3 free running clocks. 80% modulation
                               means there are 4 clock pulses for every 5 free
                               running clocks.
                            */
    UINT32 high_clockmod_rate : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This describes High clock modulation rate.The
                               value is programmed using the below table. F No
                               Modulation E 99% D 90% C 80% B 75% A 67% 9 50% 8
                               No Modulation 7 No Modulation 6 1% 5 10% 4 20% 3
                               25% 2 33% 1 50% 0 No Modulation Eg: 33%
                               modulation means there is 1 clock pulse for
                               every 3 free running clocks. 80% modulation
                               means there are 4 clock pulses for every 5 free
                               running clocks.
                            */
    UINT32 disable_clockmod : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               This is a disable for clock modulation. Will
                               need to set this bit to a 1 to disable the whole
                               clock modulation feature.
                            */
    UINT32 inactive_clockmod_rate : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000002*/

                            /*
                               This is the clock mod rate to be used during
                               Pre/Post-Si. This will bypass the ClockMod rate
                               produced by the hardware and force this fixed
                               value instead. This will be used when
                               Enable_DebugClockMod is set.
                            */
    UINT32 enable_inactiveclockmod : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               When set, the Debug_ClockMod_Rate will be used
                               instead of the rate generated by hardware.
                            */
    UINT32 enable_pipevalid9to16 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the PipeValid signals that feeds into
                               the CHA ClkMod enable will include pipestages 9
                               through 16. Currently only PipeValid1 through
                               PipeValid8 is being used.
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CLOCK_MODULATION_RATE_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe8190)                                                      */
/*      SPRB0 (0x21fe8190)                                                      */
/*      SPRHBM (0x21fe8190)                                                     */
/*      SPRC0 (0x21fe8190)                                                      */
/*      SPRMCC (0x21fe8190)                                                     */
/*      SPRUCC (0x21fe8190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_REG 0x03020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe8194)                                                      */
/*      SPRB0 (0x21fe8194)                                                      */
/*      SPRHBM (0x21fe8194)                                                     */
/*      SPRC0 (0x21fe8194)                                                      */
/*      SPRMCC (0x21fe8194)                                                     */
/*      SPRUCC (0x21fe8194)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_REG 0x03020194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe8198)                                                      */
/*      SPRB0 (0x21fe8198)                                                      */
/*      SPRHBM (0x21fe8198)                                                     */
/*      SPRC0 (0x21fe8198)                                                      */
/*      SPRMCC (0x21fe8198)                                                     */
/*      SPRUCC (0x21fe8198)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_REG 0x03020198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe819c)                                                      */
/*      SPRB0 (0x21fe819c)                                                      */
/*      SPRHBM (0x21fe819c)                                                     */
/*      SPRC0 (0x21fe819c)                                                      */
/*      SPRMCC (0x21fe819c)                                                     */
/*      SPRUCC (0x21fe819c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_REG 0x0302019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81a0)                                                      */
/*      SPRB0 (0x21fe81a0)                                                      */
/*      SPRHBM (0x21fe81a0)                                                     */
/*      SPRC0 (0x21fe81a0)                                                      */
/*      SPRMCC (0x21fe81a0)                                                     */
/*      SPRUCC (0x21fe81a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_REG 0x030201A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81a4)                                                      */
/*      SPRB0 (0x21fe81a4)                                                      */
/*      SPRHBM (0x21fe81a4)                                                     */
/*      SPRC0 (0x21fe81a4)                                                      */
/*      SPRMCC (0x21fe81a4)                                                     */
/*      SPRUCC (0x21fe81a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_REG 0x030201A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81a8)                                                      */
/*      SPRB0 (0x21fe81a8)                                                      */
/*      SPRHBM (0x21fe81a8)                                                     */
/*      SPRC0 (0x21fe81a8)                                                      */
/*      SPRMCC (0x21fe81a8)                                                     */
/*      SPRUCC (0x21fe81a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_REG 0x030201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81ac)                                                      */
/*      SPRB0 (0x21fe81ac)                                                      */
/*      SPRHBM (0x21fe81ac)                                                     */
/*      SPRC0 (0x21fe81ac)                                                      */
/*      SPRMCC (0x21fe81ac)                                                     */
/*      SPRUCC (0x21fe81ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_REG 0x030201AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81b0)                                                      */
/*      SPRB0 (0x21fe81b0)                                                      */
/*      SPRHBM (0x21fe81b0)                                                     */
/*      SPRC0 (0x21fe81b0)                                                      */
/*      SPRMCC (0x21fe81b0)                                                     */
/*      SPRUCC (0x21fe81b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_REG 0x030201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81b4)                                                      */
/*      SPRB0 (0x21fe81b4)                                                      */
/*      SPRHBM (0x21fe81b4)                                                     */
/*      SPRC0 (0x21fe81b4)                                                      */
/*      SPRMCC (0x21fe81b4)                                                     */
/*      SPRUCC (0x21fe81b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_REG 0x030201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81b8)                                                      */
/*      SPRB0 (0x21fe81b8)                                                      */
/*      SPRHBM (0x21fe81b8)                                                     */
/*      SPRC0 (0x21fe81b8)                                                      */
/*      SPRMCC (0x21fe81b8)                                                     */
/*      SPRUCC (0x21fe81b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_REG 0x030201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81bc)                                                      */
/*      SPRB0 (0x21fe81bc)                                                      */
/*      SPRHBM (0x21fe81bc)                                                     */
/*      SPRC0 (0x21fe81bc)                                                      */
/*      SPRMCC (0x21fe81bc)                                                     */
/*      SPRUCC (0x21fe81bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_REG 0x030201BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81c0)                                                      */
/*      SPRB0 (0x21fe81c0)                                                      */
/*      SPRHBM (0x21fe81c0)                                                     */
/*      SPRC0 (0x21fe81c0)                                                      */
/*      SPRMCC (0x21fe81c0)                                                     */
/*      SPRUCC (0x21fe81c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_REG 0x030201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81c4)                                                      */
/*      SPRB0 (0x21fe81c4)                                                      */
/*      SPRHBM (0x21fe81c4)                                                     */
/*      SPRC0 (0x21fe81c4)                                                      */
/*      SPRMCC (0x21fe81c4)                                                     */
/*      SPRUCC (0x21fe81c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_REG 0x030201C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81c8)                                                      */
/*      SPRB0 (0x21fe81c8)                                                      */
/*      SPRHBM (0x21fe81c8)                                                     */
/*      SPRC0 (0x21fe81c8)                                                      */
/*      SPRMCC (0x21fe81c8)                                                     */
/*      SPRUCC (0x21fe81c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_REG 0x030201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81cc)                                                      */
/*      SPRB0 (0x21fe81cc)                                                      */
/*      SPRHBM (0x21fe81cc)                                                     */
/*      SPRC0 (0x21fe81cc)                                                      */
/*      SPRMCC (0x21fe81cc)                                                     */
/*      SPRUCC (0x21fe81cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_REG 0x030201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81d0)                                                      */
/*      SPRB0 (0x21fe81d0)                                                      */
/*      SPRHBM (0x21fe81d0)                                                     */
/*      SPRC0 (0x21fe81d0)                                                      */
/*      SPRMCC (0x21fe81d0)                                                     */
/*      SPRUCC (0x21fe81d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_REG 0x030201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81d4)                                                      */
/*      SPRB0 (0x21fe81d4)                                                      */
/*      SPRHBM (0x21fe81d4)                                                     */
/*      SPRC0 (0x21fe81d4)                                                      */
/*      SPRMCC (0x21fe81d4)                                                     */
/*      SPRUCC (0x21fe81d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_REG 0x030201D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81d8)                                                      */
/*      SPRB0 (0x21fe81d8)                                                      */
/*      SPRHBM (0x21fe81d8)                                                     */
/*      SPRC0 (0x21fe81d8)                                                      */
/*      SPRMCC (0x21fe81d8)                                                     */
/*      SPRUCC (0x21fe81d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_REG 0x030201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_REG supported on:                        */
/*      SPRA0 (0x21fe81dc)                                                      */
/*      SPRB0 (0x21fe81dc)                                                      */
/*      SPRHBM (0x21fe81dc)                                                     */
/*      SPRC0 (0x21fe81dc)                                                      */
/*      SPRMCC (0x21fe81dc)                                                     */
/*      SPRUCC (0x21fe81dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_REG 0x030201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81e0)                                                      */
/*      SPRB0 (0x21fe81e0)                                                      */
/*      SPRHBM (0x21fe81e0)                                                     */
/*      SPRC0 (0x21fe81e0)                                                      */
/*      SPRMCC (0x21fe81e0)                                                     */
/*      SPRUCC (0x21fe81e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_REG 0x030201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81e4)                                                      */
/*      SPRB0 (0x21fe81e4)                                                      */
/*      SPRHBM (0x21fe81e4)                                                     */
/*      SPRC0 (0x21fe81e4)                                                      */
/*      SPRMCC (0x21fe81e4)                                                     */
/*      SPRUCC (0x21fe81e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_REG 0x030201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81e8)                                                      */
/*      SPRB0 (0x21fe81e8)                                                      */
/*      SPRHBM (0x21fe81e8)                                                     */
/*      SPRC0 (0x21fe81e8)                                                      */
/*      SPRMCC (0x21fe81e8)                                                     */
/*      SPRUCC (0x21fe81e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_REG 0x030201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81ec)                                                      */
/*      SPRB0 (0x21fe81ec)                                                      */
/*      SPRHBM (0x21fe81ec)                                                     */
/*      SPRC0 (0x21fe81ec)                                                      */
/*      SPRMCC (0x21fe81ec)                                                     */
/*      SPRUCC (0x21fe81ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_REG 0x030201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81f0)                                                      */
/*      SPRB0 (0x21fe81f0)                                                      */
/*      SPRHBM (0x21fe81f0)                                                     */
/*      SPRC0 (0x21fe81f0)                                                      */
/*      SPRMCC (0x21fe81f0)                                                     */
/*      SPRUCC (0x21fe81f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_REG 0x030201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81f4)                                                      */
/*      SPRB0 (0x21fe81f4)                                                      */
/*      SPRHBM (0x21fe81f4)                                                     */
/*      SPRC0 (0x21fe81f4)                                                      */
/*      SPRMCC (0x21fe81f4)                                                     */
/*      SPRUCC (0x21fe81f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_REG 0x030201F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81f8)                                                      */
/*      SPRB0 (0x21fe81f8)                                                      */
/*      SPRHBM (0x21fe81f8)                                                     */
/*      SPRC0 (0x21fe81f8)                                                      */
/*      SPRMCC (0x21fe81f8)                                                     */
/*      SPRUCC (0x21fe81f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_REG 0x030201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe81fc)                                                      */
/*      SPRB0 (0x21fe81fc)                                                      */
/*      SPRHBM (0x21fe81fc)                                                     */
/*      SPRC0 (0x21fe81fc)                                                      */
/*      SPRMCC (0x21fe81fc)                                                     */
/*      SPRUCC (0x21fe81fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_REG 0x030201FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_14_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8200)                                                      */
/*      SPRB0 (0x21fe8200)                                                      */
/*      SPRHBM (0x21fe8200)                                                     */
/*      SPRC0 (0x21fe8200)                                                      */
/*      SPRMCC (0x21fe8200)                                                     */
/*      SPRUCC (0x21fe8200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_14_N0_CHABC_SAD_REG 0x03020200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_14_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_14_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8204)                                                      */
/*      SPRB0 (0x21fe8204)                                                      */
/*      SPRHBM (0x21fe8204)                                                     */
/*      SPRC0 (0x21fe8204)                                                      */
/*      SPRMCC (0x21fe8204)                                                     */
/*      SPRUCC (0x21fe8204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_14_N1_CHABC_SAD_REG 0x03020204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_14_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_15_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8208)                                                      */
/*      SPRB0 (0x21fe8208)                                                      */
/*      SPRHBM (0x21fe8208)                                                     */
/*      SPRC0 (0x21fe8208)                                                      */
/*      SPRMCC (0x21fe8208)                                                     */
/*      SPRUCC (0x21fe8208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_15_N0_CHABC_SAD_REG 0x03020208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_15_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_15_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe820c)                                                      */
/*      SPRB0 (0x21fe820c)                                                      */
/*      SPRHBM (0x21fe820c)                                                     */
/*      SPRC0 (0x21fe820c)                                                      */
/*      SPRMCC (0x21fe820c)                                                     */
/*      SPRUCC (0x21fe820c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_15_N1_CHABC_SAD_REG 0x0302020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_15_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_16_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8210)                                                      */
/*      SPRB0 (0x21fe8210)                                                      */
/*      SPRHBM (0x21fe8210)                                                     */
/*      SPRC0 (0x21fe8210)                                                      */
/*      SPRMCC (0x21fe8210)                                                     */
/*      SPRUCC (0x21fe8210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_16_N0_CHABC_SAD_REG 0x03020210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_16_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_16_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8214)                                                      */
/*      SPRB0 (0x21fe8214)                                                      */
/*      SPRHBM (0x21fe8214)                                                     */
/*      SPRC0 (0x21fe8214)                                                      */
/*      SPRMCC (0x21fe8214)                                                     */
/*      SPRUCC (0x21fe8214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_16_N1_CHABC_SAD_REG 0x03020214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_16_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_17_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8218)                                                      */
/*      SPRB0 (0x21fe8218)                                                      */
/*      SPRHBM (0x21fe8218)                                                     */
/*      SPRC0 (0x21fe8218)                                                      */
/*      SPRMCC (0x21fe8218)                                                     */
/*      SPRUCC (0x21fe8218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_17_N0_CHABC_SAD_REG 0x03020218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_17_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_17_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe821c)                                                      */
/*      SPRB0 (0x21fe821c)                                                      */
/*      SPRHBM (0x21fe821c)                                                     */
/*      SPRC0 (0x21fe821c)                                                      */
/*      SPRMCC (0x21fe821c)                                                     */
/*      SPRUCC (0x21fe821c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_17_N1_CHABC_SAD_REG 0x0302021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_17_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_18_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8220)                                                      */
/*      SPRB0 (0x21fe8220)                                                      */
/*      SPRHBM (0x21fe8220)                                                     */
/*      SPRC0 (0x21fe8220)                                                      */
/*      SPRMCC (0x21fe8220)                                                     */
/*      SPRUCC (0x21fe8220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_18_N0_CHABC_SAD_REG 0x03020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_18_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_18_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8224)                                                      */
/*      SPRB0 (0x21fe8224)                                                      */
/*      SPRHBM (0x21fe8224)                                                     */
/*      SPRC0 (0x21fe8224)                                                      */
/*      SPRMCC (0x21fe8224)                                                     */
/*      SPRUCC (0x21fe8224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_18_N1_CHABC_SAD_REG 0x03020224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_18_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_19_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8228)                                                      */
/*      SPRB0 (0x21fe8228)                                                      */
/*      SPRHBM (0x21fe8228)                                                     */
/*      SPRC0 (0x21fe8228)                                                      */
/*      SPRMCC (0x21fe8228)                                                     */
/*      SPRUCC (0x21fe8228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_19_N0_CHABC_SAD_REG 0x03020228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_19_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_19_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe822c)                                                      */
/*      SPRB0 (0x21fe822c)                                                      */
/*      SPRHBM (0x21fe822c)                                                     */
/*      SPRC0 (0x21fe822c)                                                      */
/*      SPRMCC (0x21fe822c)                                                     */
/*      SPRUCC (0x21fe822c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_19_N1_CHABC_SAD_REG 0x0302022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_19_N1_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_20_N0_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8230)                                                      */
/*      SPRB0 (0x21fe8230)                                                      */
/*      SPRHBM (0x21fe8230)                                                     */
/*      SPRC0 (0x21fe8230)                                                      */
/*      SPRMCC (0x21fe8230)                                                     */
/*      SPRUCC (0x21fe8230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_20_N0_CHABC_SAD_REG 0x03020230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               to a remote processor socket via a Compute
                               Express Link (CXL). (CXL was previously referred
                               to as Intel Accelerator Link or IAL.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Sets the memory type for the remote DRAM rule
                               according to the following encoding.</p> <p>00 -
                               Coherent DRAM</p> <p>01 - Memory Mapped CFG</p>
                               <p>10 - Low Bandwidth Coherent DRAM</p> <p>11 -
                               High Bandwidth Coherent DRAM</p>
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This corresponds to Addr[51:26] of the DRAM
                               rule's&nbsp;base address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_20_N0_CHABC_SAD_STRUCT;

/* REMOTE_DRAM_RULE_CFG_20_N1_CHABC_SAD_REG supported on:                       */
/*      SPRA0 (0x21fe8234)                                                      */
/*      SPRB0 (0x21fe8234)                                                      */
/*      SPRHBM (0x21fe8234)                                                     */
/*      SPRC0 (0x21fe8234)                                                      */
/*      SPRMCC (0x21fe8234)                                                     */
/*      SPRUCC (0x21fe8234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers are used to map out the remote NUMA interleaved memory in a multi-socket system.
*/


#define REMOTE_DRAM_RULE_CFG_20_N1_CHABC_SAD_REG 0x03020234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Target NodeID for this remote DRAM rule.
                               Target node is only 3 bits because the target is
                               assumed to be a remote target. Placing the local
                               socket NodeID in as the target for this rule is
                               not supported.</p>
                            */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address.
                            */

  } Bits;
  UINT32 Data;

} REMOTE_DRAM_RULE_CFG_20_N1_CHABC_SAD_STRUCT;

/* DRAM_ROUTETHROUGH_RANGE_CHABC_SAD_REG supported on:                          */
/*      SPRA0 (0x21fe8298)                                                      */
/*      SPRB0 (0x21fe8298)                                                      */
/*      SPRHBM (0x21fe8298)                                                     */
/*      SPRC0 (0x21fe8298)                                                      */
/*      SPRMCC (0x21fe8298)                                                     */
/*      SPRUCC (0x21fe8298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies which DRAM rules are targetting an external memory channel by routing through an internal memory controller.
*/


#define DRAM_ROUTETHROUGH_RANGE_CHABC_SAD_REG 0x03020298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 route_through_vector : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector that specifices, for each
                               individual DRAM rule whether that DRAM rule is
                               routing through an internal memory controller to
                               read an external memory controller. This case is
                               used when the CHA has no VNA credits for
                               communicating with the external memory
                               controller directly on its own. Regardless of
                               the value in this field, the target of DRAM rule
                               is the target specified by DRAM_MC_CHANNEL and
                               DRAM_MC_TARGET. A 1 in bit N of this vector
                               means that the actual target of the message is
                               the MC specified in Route Table 2 but that the
                               message is being routed through the target MC on
                               its way to the MC specified by Route Table 2.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_ROUTETHROUGH_RANGE_CHABC_SAD_STRUCT;

/* QPI_TIMEOUT_CTRL_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe82d0)                                                      */
/*      SPRB0 (0x21fe82d0)                                                      */
/*      SPRHBM (0x21fe82d0)                                                     */
/*      SPRC0 (0x21fe82d0)                                                      */
/*      SPRMCC (0x21fe82d0)                                                     */
/*      SPRUCC (0x21fe82d0)                                                     */
/* Register default value on SPRA0: 0x1FFFFFFE                                  */
/* Register default value on SPRB0: 0x1FFFFFFE                                  */
/* Register default value on SPRHBM: 0x1FFFFFFE                                 */
/* Register default value on SPRC0: 0x1FFFFFFE                                  */
/* Register default value on SPRMCC: 0x1FFFFFFE                                 */
/* Register default value on SPRUCC: 0x1FFFFFFE                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Intel UPI/TOR Timeout Configuration
*/


#define QPI_TIMEOUT_CTRL_CHABC_SAD_REG 0x030202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enabletortimeout : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Intel UPI Timeout for certain classes of
                               Intel UPI transactions. A unique CHA timeout
                               range can be programmed based on the timeout
                               level of the transaction (refer to Intel UPI
                               specifications for further details of the
                               mapping of levels to the transaction type):
                               Actual CHA timeout value varies and could be any
                               value between lower bound and upper bound and is
                               defined as follows: Upper bound of the CHA
                               timeout:- [2(N+11)*Level_M_offset]/F Lower bound
                               of the CHA timeout:-
                               [2(N+11)*(Level_M_offset-1)]/F Where: - N is the
                               number of bits in the upper base counter
                               (defined by entry_ctr_inc_ctl). - Level_M_offset
                               (here M=0,1,3,4,6) is the value programed in
                               levelM_offset bit-field. - F is the CPU clock
                               frequency.
                            */
    UINT32 entry_ctr_inc_ctl : 3;

                            /* Bits[3:1], Access Type=RW/P, default=0x00000007*/

                            /*
                               Indication of which bit of upper base counter
                               increments entry counter. Value of the upper
                               base counter maps to actual timeout intervals as
                               follows (assuming a 1.0 GHz base frequency 000:
                               no upper-base counter is used (~ 2.1*offset us
                               timeout @1.0Ghz) 001: 2-bit upper-base counter
                               is used (~ 8.2*offset us timeout @1.0Ghz) 010:
                               5-bit upper-base counter is used (~ 66*offset us
                               timeout @1.0Ghz) 011: 8-bit upper-base counter
                               is used (~ 524*offset us timeout @1.0Ghz) 100:
                               11-bit upper-base counter is used (~ 4.2*offset
                               ms timeout @1.0Ghz) 101: 15-bit upper-base
                               counter is used (~ 67*offset ms timeout @1.0Ghz)
                               110: 18-bit upper-base counter is used (~
                               537*offset ms timeout @1.0Ghz) 111: 20-bit
                               upper-base counter is used (~ 2.2*offset second
                               timeout @1.0Ghz)
                            */
    UINT32 level0_offset : 5;

                            /* Bits[8:4], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 0 Timeout offset.
                               Offsets must be > 1.
                            */
    UINT32 level1_offset : 5;

                            /* Bits[13:9], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 1 Timeout offset.
                               Offsets must be > 1. Level1-Level6 offsets must
                               be >= Level0 offset.
                            */
    UINT32 level2_offset : 5;

                            /* Bits[18:14], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 2 Timeout offset.
                               Offsets must be > 1. Level1-Level6 offsets must
                               be >= Level0 offset.
                            */
    UINT32 level3_offset : 5;

                            /* Bits[23:19], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 3 Timeout offset.
                               Offsets must be > 1. Level1-Level6 offsets must
                               be >= Level0 offset.
                            */
    UINT32 level4_offset : 5;

                            /* Bits[28:24], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 4 Timeout offset.
                               Offsets must be > 1. Level1-Level6 offsets must
                               be >= Level0 offset.
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QPI_TIMEOUT_CTRL_CHABC_SAD_STRUCT;

/* QPI_TIMEOUT_CTRL2_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe82d4)                                                      */
/*      SPRB0 (0x21fe82d4)                                                      */
/*      SPRHBM (0x21fe82d4)                                                     */
/*      SPRC0 (0x21fe82d4)                                                      */
/*      SPRMCC (0x21fe82d4)                                                     */
/*      SPRUCC (0x21fe82d4)                                                     */
/* Register default value on SPRA0: 0x0000001F                                  */
/* Register default value on SPRB0: 0x0000001F                                  */
/* Register default value on SPRHBM: 0x0000001F                                 */
/* Register default value on SPRC0: 0x0000001F                                  */
/* Register default value on SPRMCC: 0x0000001F                                 */
/* Register default value on SPRUCC: 0x0000001F                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Intel UPI/TOR Timeout Configuration 2
*/


#define QPI_TIMEOUT_CTRL2_CHABC_SAD_REG 0x030202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 level6_offset : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Encoded Intel UPI Level 6 Timeout offset.
                               Offsets must be > 1. Level1-Level6 offsets must
                               be >= Level0 offset.
                            */
    UINT32 disableretrytracking : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable TOR timeout tracking on retry requests
                               (before they have been acked)
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QPI_TIMEOUT_CTRL2_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_0_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82e0)                                                      */
/*      SPRB0 (0x21fe82e0)                                                      */
/*      SPRHBM (0x21fe82e0)                                                     */
/*      SPRC0 (0x21fe82e0)                                                      */
/*      SPRMCC (0x21fe82e0)                                                     */
/*      SPRUCC (0x21fe82e0)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_0_CHABC_SAD_REG 0x030202E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_0_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_1_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82e4)                                                      */
/*      SPRB0 (0x21fe82e4)                                                      */
/*      SPRHBM (0x21fe82e4)                                                     */
/*      SPRC0 (0x21fe82e4)                                                      */
/*      SPRMCC (0x21fe82e4)                                                     */
/*      SPRUCC (0x21fe82e4)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_1_CHABC_SAD_REG 0x030202E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_1_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_2_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82e8)                                                      */
/*      SPRB0 (0x21fe82e8)                                                      */
/*      SPRHBM (0x21fe82e8)                                                     */
/*      SPRC0 (0x21fe82e8)                                                      */
/*      SPRMCC (0x21fe82e8)                                                     */
/*      SPRUCC (0x21fe82e8)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_2_CHABC_SAD_REG 0x030202E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_2_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_3_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82ec)                                                      */
/*      SPRB0 (0x21fe82ec)                                                      */
/*      SPRHBM (0x21fe82ec)                                                     */
/*      SPRC0 (0x21fe82ec)                                                      */
/*      SPRMCC (0x21fe82ec)                                                     */
/*      SPRUCC (0x21fe82ec)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_3_CHABC_SAD_REG 0x030202EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_3_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_4_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82f0)                                                      */
/*      SPRB0 (0x21fe82f0)                                                      */
/*      SPRHBM (0x21fe82f0)                                                     */
/*      SPRC0 (0x21fe82f0)                                                      */
/*      SPRMCC (0x21fe82f0)                                                     */
/*      SPRUCC (0x21fe82f0)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_4_CHABC_SAD_REG 0x030202F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_4_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_5_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82f4)                                                      */
/*      SPRB0 (0x21fe82f4)                                                      */
/*      SPRHBM (0x21fe82f4)                                                     */
/*      SPRC0 (0x21fe82f4)                                                      */
/*      SPRMCC (0x21fe82f4)                                                     */
/*      SPRUCC (0x21fe82f4)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_5_CHABC_SAD_REG 0x030202F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_5_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_6_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82f8)                                                      */
/*      SPRB0 (0x21fe82f8)                                                      */
/*      SPRHBM (0x21fe82f8)                                                     */
/*      SPRC0 (0x21fe82f8)                                                      */
/*      SPRMCC (0x21fe82f8)                                                     */
/*      SPRUCC (0x21fe82f8)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_6_CHABC_SAD_REG 0x030202F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_6_CHABC_SAD_STRUCT;

/* IOPORT_TARGET_LIST_CFG_7_CHABC_SAD_REG supported on:                         */
/*      SPRA0 (0x21fe82fc)                                                      */
/*      SPRB0 (0x21fe82fc)                                                      */
/*      SPRHBM (0x21fe82fc)                                                     */
/*      SPRC0 (0x21fe82fc)                                                      */
/*      SPRMCC (0x21fe82fc)                                                     */
/*      SPRUCC (0x21fe82fc)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IO port target list config.
                  b1xxx  Target is local IIO Stack xxxx
                  b0xxx  Target is remote socket with NodeID xxx";
*/


#define IOPORT_TARGET_LIST_CFG_7_CHABC_SAD_REG 0x030202FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               IOPORT target. b1xxxx Target is local IIO Stack
                               xxxx b00xxx Target is remote socket with NodeID
                               xxx
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package1 : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package2 : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 package3 : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000010*/

                            /* See package 0 description. */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOPORT_TARGET_LIST_CFG_7_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_0_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8300)                                                      */
/*      SPRB0 (0x21fe8300)                                                      */
/*      SPRHBM (0x21fe8300)                                                     */
/*      SPRC0 (0x21fe8300)                                                      */
/*      SPRMCC (0x21fe8300)                                                     */
/*      SPRUCC (0x21fe8300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_0_CHABC_SAD_REG 0x03020300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_0_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_0_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8304)                                                      */
/*      SPRB0 (0x21fe8304)                                                      */
/*      SPRHBM (0x21fe8304)                                                     */
/*      SPRC0 (0x21fe8304)                                                      */
/*      SPRMCC (0x21fe8304)                                                     */
/*      SPRUCC (0x21fe8304)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_0_CHABC_SAD_REG 0x03020304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_0_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_1_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8308)                                                      */
/*      SPRB0 (0x21fe8308)                                                      */
/*      SPRHBM (0x21fe8308)                                                     */
/*      SPRC0 (0x21fe8308)                                                      */
/*      SPRMCC (0x21fe8308)                                                     */
/*      SPRUCC (0x21fe8308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_1_CHABC_SAD_REG 0x03020308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_1_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_1_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe830c)                                                      */
/*      SPRB0 (0x21fe830c)                                                      */
/*      SPRHBM (0x21fe830c)                                                     */
/*      SPRC0 (0x21fe830c)                                                      */
/*      SPRMCC (0x21fe830c)                                                     */
/*      SPRUCC (0x21fe830c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_1_CHABC_SAD_REG 0x0302030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_1_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_2_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8310)                                                      */
/*      SPRB0 (0x21fe8310)                                                      */
/*      SPRHBM (0x21fe8310)                                                     */
/*      SPRC0 (0x21fe8310)                                                      */
/*      SPRMCC (0x21fe8310)                                                     */
/*      SPRUCC (0x21fe8310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_2_CHABC_SAD_REG 0x03020310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_2_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_2_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8314)                                                      */
/*      SPRB0 (0x21fe8314)                                                      */
/*      SPRHBM (0x21fe8314)                                                     */
/*      SPRC0 (0x21fe8314)                                                      */
/*      SPRMCC (0x21fe8314)                                                     */
/*      SPRUCC (0x21fe8314)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_2_CHABC_SAD_REG 0x03020314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_2_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_3_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8318)                                                      */
/*      SPRB0 (0x21fe8318)                                                      */
/*      SPRHBM (0x21fe8318)                                                     */
/*      SPRC0 (0x21fe8318)                                                      */
/*      SPRMCC (0x21fe8318)                                                     */
/*      SPRUCC (0x21fe8318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_3_CHABC_SAD_REG 0x03020318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_3_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_3_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe831c)                                                      */
/*      SPRB0 (0x21fe831c)                                                      */
/*      SPRHBM (0x21fe831c)                                                     */
/*      SPRC0 (0x21fe831c)                                                      */
/*      SPRMCC (0x21fe831c)                                                     */
/*      SPRUCC (0x21fe831c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_3_CHABC_SAD_REG 0x0302031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_3_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_4_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8320)                                                      */
/*      SPRB0 (0x21fe8320)                                                      */
/*      SPRHBM (0x21fe8320)                                                     */
/*      SPRC0 (0x21fe8320)                                                      */
/*      SPRMCC (0x21fe8320)                                                     */
/*      SPRUCC (0x21fe8320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_4_CHABC_SAD_REG 0x03020320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_4_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_4_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8324)                                                      */
/*      SPRB0 (0x21fe8324)                                                      */
/*      SPRHBM (0x21fe8324)                                                     */
/*      SPRC0 (0x21fe8324)                                                      */
/*      SPRMCC (0x21fe8324)                                                     */
/*      SPRUCC (0x21fe8324)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_4_CHABC_SAD_REG 0x03020324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_4_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_5_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8328)                                                      */
/*      SPRB0 (0x21fe8328)                                                      */
/*      SPRHBM (0x21fe8328)                                                     */
/*      SPRC0 (0x21fe8328)                                                      */
/*      SPRMCC (0x21fe8328)                                                     */
/*      SPRUCC (0x21fe8328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_5_CHABC_SAD_REG 0x03020328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_5_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_5_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe832c)                                                      */
/*      SPRB0 (0x21fe832c)                                                      */
/*      SPRHBM (0x21fe832c)                                                     */
/*      SPRC0 (0x21fe832c)                                                      */
/*      SPRMCC (0x21fe832c)                                                     */
/*      SPRUCC (0x21fe832c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_5_CHABC_SAD_REG 0x0302032C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_5_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_6_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8330)                                                      */
/*      SPRB0 (0x21fe8330)                                                      */
/*      SPRHBM (0x21fe8330)                                                     */
/*      SPRC0 (0x21fe8330)                                                      */
/*      SPRMCC (0x21fe8330)                                                     */
/*      SPRUCC (0x21fe8330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_6_CHABC_SAD_REG 0x03020330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_6_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_6_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8334)                                                      */
/*      SPRB0 (0x21fe8334)                                                      */
/*      SPRHBM (0x21fe8334)                                                     */
/*      SPRC0 (0x21fe8334)                                                      */
/*      SPRMCC (0x21fe8334)                                                     */
/*      SPRUCC (0x21fe8334)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_6_CHABC_SAD_REG 0x03020334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_6_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_7_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8338)                                                      */
/*      SPRB0 (0x21fe8338)                                                      */
/*      SPRHBM (0x21fe8338)                                                     */
/*      SPRC0 (0x21fe8338)                                                      */
/*      SPRMCC (0x21fe8338)                                                     */
/*      SPRUCC (0x21fe8338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_7_CHABC_SAD_REG 0x03020338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_7_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_7_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe833c)                                                      */
/*      SPRB0 (0x21fe833c)                                                      */
/*      SPRHBM (0x21fe833c)                                                     */
/*      SPRC0 (0x21fe833c)                                                      */
/*      SPRMCC (0x21fe833c)                                                     */
/*      SPRUCC (0x21fe833c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_7_CHABC_SAD_REG 0x0302033C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_7_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_8_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8340)                                                      */
/*      SPRB0 (0x21fe8340)                                                      */
/*      SPRHBM (0x21fe8340)                                                     */
/*      SPRC0 (0x21fe8340)                                                      */
/*      SPRMCC (0x21fe8340)                                                     */
/*      SPRUCC (0x21fe8340)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_8_CHABC_SAD_REG 0x03020340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_8_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_8_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe8344)                                                      */
/*      SPRB0 (0x21fe8344)                                                      */
/*      SPRHBM (0x21fe8344)                                                     */
/*      SPRC0 (0x21fe8344)                                                      */
/*      SPRMCC (0x21fe8344)                                                     */
/*      SPRUCC (0x21fe8344)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_8_CHABC_SAD_REG 0x03020344

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_8_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_9_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe8348)                                                      */
/*      SPRB0 (0x21fe8348)                                                      */
/*      SPRHBM (0x21fe8348)                                                     */
/*      SPRC0 (0x21fe8348)                                                      */
/*      SPRMCC (0x21fe8348)                                                     */
/*      SPRUCC (0x21fe8348)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_9_CHABC_SAD_REG 0x03020348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_9_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_9_CHABC_SAD_REG supported on:                            */
/*      SPRA0 (0x21fe834c)                                                      */
/*      SPRB0 (0x21fe834c)                                                      */
/*      SPRHBM (0x21fe834c)                                                     */
/*      SPRC0 (0x21fe834c)                                                      */
/*      SPRMCC (0x21fe834c)                                                     */
/*      SPRUCC (0x21fe834c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_9_CHABC_SAD_REG 0x0302034C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_9_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_10_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8350)                                                      */
/*      SPRB0 (0x21fe8350)                                                      */
/*      SPRHBM (0x21fe8350)                                                     */
/*      SPRC0 (0x21fe8350)                                                      */
/*      SPRMCC (0x21fe8350)                                                     */
/*      SPRUCC (0x21fe8350)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_10_CHABC_SAD_REG 0x03020350

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_10_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_10_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe8354)                                                      */
/*      SPRB0 (0x21fe8354)                                                      */
/*      SPRHBM (0x21fe8354)                                                     */
/*      SPRC0 (0x21fe8354)                                                      */
/*      SPRMCC (0x21fe8354)                                                     */
/*      SPRUCC (0x21fe8354)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_10_CHABC_SAD_REG 0x03020354

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_10_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_11_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8358)                                                      */
/*      SPRB0 (0x21fe8358)                                                      */
/*      SPRHBM (0x21fe8358)                                                     */
/*      SPRC0 (0x21fe8358)                                                      */
/*      SPRMCC (0x21fe8358)                                                     */
/*      SPRUCC (0x21fe8358)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_11_CHABC_SAD_REG 0x03020358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_11_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_11_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe835c)                                                      */
/*      SPRB0 (0x21fe835c)                                                      */
/*      SPRHBM (0x21fe835c)                                                     */
/*      SPRC0 (0x21fe835c)                                                      */
/*      SPRMCC (0x21fe835c)                                                     */
/*      SPRUCC (0x21fe835c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_11_CHABC_SAD_REG 0x0302035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_11_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_12_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8360)                                                      */
/*      SPRB0 (0x21fe8360)                                                      */
/*      SPRHBM (0x21fe8360)                                                     */
/*      SPRC0 (0x21fe8360)                                                      */
/*      SPRMCC (0x21fe8360)                                                     */
/*      SPRUCC (0x21fe8360)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_12_CHABC_SAD_REG 0x03020360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_12_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_12_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe8364)                                                      */
/*      SPRB0 (0x21fe8364)                                                      */
/*      SPRHBM (0x21fe8364)                                                     */
/*      SPRC0 (0x21fe8364)                                                      */
/*      SPRMCC (0x21fe8364)                                                     */
/*      SPRUCC (0x21fe8364)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_12_CHABC_SAD_REG 0x03020364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_12_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_13_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8368)                                                      */
/*      SPRB0 (0x21fe8368)                                                      */
/*      SPRHBM (0x21fe8368)                                                     */
/*      SPRC0 (0x21fe8368)                                                      */
/*      SPRMCC (0x21fe8368)                                                     */
/*      SPRUCC (0x21fe8368)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_13_CHABC_SAD_REG 0x03020368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_13_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_13_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe836c)                                                      */
/*      SPRB0 (0x21fe836c)                                                      */
/*      SPRHBM (0x21fe836c)                                                     */
/*      SPRC0 (0x21fe836c)                                                      */
/*      SPRMCC (0x21fe836c)                                                     */
/*      SPRUCC (0x21fe836c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_13_CHABC_SAD_REG 0x0302036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_13_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_14_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8370)                                                      */
/*      SPRB0 (0x21fe8370)                                                      */
/*      SPRHBM (0x21fe8370)                                                     */
/*      SPRC0 (0x21fe8370)                                                      */
/*      SPRMCC (0x21fe8370)                                                     */
/*      SPRUCC (0x21fe8370)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_14_CHABC_SAD_REG 0x03020370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_14_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_14_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe8374)                                                      */
/*      SPRB0 (0x21fe8374)                                                      */
/*      SPRHBM (0x21fe8374)                                                     */
/*      SPRC0 (0x21fe8374)                                                      */
/*      SPRMCC (0x21fe8374)                                                     */
/*      SPRUCC (0x21fe8374)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_14_CHABC_SAD_REG 0x03020374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_14_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG_15_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8378)                                                      */
/*      SPRB0 (0x21fe8378)                                                      */
/*      SPRHBM (0x21fe8378)                                                     */
/*      SPRC0 (0x21fe8378)                                                      */
/*      SPRMCC (0x21fe8378)                                                     */
/*      SPRUCC (0x21fe8378)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_15_CHABC_SAD_REG 0x03020378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_15_CHABC_SAD_STRUCT;

/* INTERLEAVE_LIST_CFG_15_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe837c)                                                      */
/*      SPRB0 (0x21fe837c)                                                      */
/*      SPRHBM (0x21fe837c)                                                     */
/*      SPRC0 (0x21fe837c)                                                      */
/*      SPRMCC (0x21fe837c)                                                     */
/*      SPRUCC (0x21fe837c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_15_CHABC_SAD_REG 0x0302037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_15_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_0_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8380)                                                      */
/*      SPRB0 (0x21fe8380)                                                      */
/*      SPRHBM (0x21fe8380)                                                     */
/*      SPRC0 (0x21fe8380)                                                      */
/*      SPRMCC (0x21fe8380)                                                     */
/*      SPRUCC (0x21fe8380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_0_CHABC_SAD_REG 0x03020380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_0_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_1_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8384)                                                      */
/*      SPRB0 (0x21fe8384)                                                      */
/*      SPRHBM (0x21fe8384)                                                     */
/*      SPRC0 (0x21fe8384)                                                      */
/*      SPRMCC (0x21fe8384)                                                     */
/*      SPRUCC (0x21fe8384)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_1_CHABC_SAD_REG 0x03020384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_1_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_2_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8388)                                                      */
/*      SPRB0 (0x21fe8388)                                                      */
/*      SPRHBM (0x21fe8388)                                                     */
/*      SPRC0 (0x21fe8388)                                                      */
/*      SPRMCC (0x21fe8388)                                                     */
/*      SPRUCC (0x21fe8388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_2_CHABC_SAD_REG 0x03020388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_2_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_3_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe838c)                                                      */
/*      SPRB0 (0x21fe838c)                                                      */
/*      SPRHBM (0x21fe838c)                                                     */
/*      SPRC0 (0x21fe838c)                                                      */
/*      SPRMCC (0x21fe838c)                                                     */
/*      SPRUCC (0x21fe838c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_3_CHABC_SAD_REG 0x0302038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_3_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_4_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8390)                                                      */
/*      SPRB0 (0x21fe8390)                                                      */
/*      SPRHBM (0x21fe8390)                                                     */
/*      SPRC0 (0x21fe8390)                                                      */
/*      SPRMCC (0x21fe8390)                                                     */
/*      SPRUCC (0x21fe8390)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_4_CHABC_SAD_REG 0x03020390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_4_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_5_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8394)                                                      */
/*      SPRB0 (0x21fe8394)                                                      */
/*      SPRHBM (0x21fe8394)                                                     */
/*      SPRC0 (0x21fe8394)                                                      */
/*      SPRMCC (0x21fe8394)                                                     */
/*      SPRUCC (0x21fe8394)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_5_CHABC_SAD_REG 0x03020394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_5_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_6_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8398)                                                      */
/*      SPRB0 (0x21fe8398)                                                      */
/*      SPRHBM (0x21fe8398)                                                     */
/*      SPRC0 (0x21fe8398)                                                      */
/*      SPRMCC (0x21fe8398)                                                     */
/*      SPRUCC (0x21fe8398)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_6_CHABC_SAD_REG 0x03020398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_6_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_7_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe839c)                                                      */
/*      SPRB0 (0x21fe839c)                                                      */
/*      SPRHBM (0x21fe839c)                                                     */
/*      SPRC0 (0x21fe839c)                                                      */
/*      SPRMCC (0x21fe839c)                                                     */
/*      SPRUCC (0x21fe839c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_7_CHABC_SAD_REG 0x0302039C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_7_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_8_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe83a0)                                                      */
/*      SPRB0 (0x21fe83a0)                                                      */
/*      SPRHBM (0x21fe83a0)                                                     */
/*      SPRC0 (0x21fe83a0)                                                      */
/*      SPRMCC (0x21fe83a0)                                                     */
/*      SPRUCC (0x21fe83a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_8_CHABC_SAD_REG 0x030203A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_8_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_9_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe83a4)                                                      */
/*      SPRB0 (0x21fe83a4)                                                      */
/*      SPRHBM (0x21fe83a4)                                                     */
/*      SPRC0 (0x21fe83a4)                                                      */
/*      SPRMCC (0x21fe83a4)                                                     */
/*      SPRUCC (0x21fe83a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_9_CHABC_SAD_REG 0x030203A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_9_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_10_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83a8)                                                      */
/*      SPRB0 (0x21fe83a8)                                                      */
/*      SPRHBM (0x21fe83a8)                                                     */
/*      SPRC0 (0x21fe83a8)                                                      */
/*      SPRMCC (0x21fe83a8)                                                     */
/*      SPRUCC (0x21fe83a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_10_CHABC_SAD_REG 0x030203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_10_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_11_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83ac)                                                      */
/*      SPRB0 (0x21fe83ac)                                                      */
/*      SPRHBM (0x21fe83ac)                                                     */
/*      SPRC0 (0x21fe83ac)                                                      */
/*      SPRMCC (0x21fe83ac)                                                     */
/*      SPRUCC (0x21fe83ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_11_CHABC_SAD_REG 0x030203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_11_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_12_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83b0)                                                      */
/*      SPRB0 (0x21fe83b0)                                                      */
/*      SPRHBM (0x21fe83b0)                                                     */
/*      SPRC0 (0x21fe83b0)                                                      */
/*      SPRMCC (0x21fe83b0)                                                     */
/*      SPRUCC (0x21fe83b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_12_CHABC_SAD_REG 0x030203B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_12_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_13_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83b4)                                                      */
/*      SPRB0 (0x21fe83b4)                                                      */
/*      SPRHBM (0x21fe83b4)                                                     */
/*      SPRC0 (0x21fe83b4)                                                      */
/*      SPRMCC (0x21fe83b4)                                                     */
/*      SPRUCC (0x21fe83b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_13_CHABC_SAD_REG 0x030203B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_13_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_14_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83b8)                                                      */
/*      SPRB0 (0x21fe83b8)                                                      */
/*      SPRHBM (0x21fe83b8)                                                     */
/*      SPRC0 (0x21fe83b8)                                                      */
/*      SPRMCC (0x21fe83b8)                                                     */
/*      SPRUCC (0x21fe83b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_14_CHABC_SAD_REG 0x030203B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_14_CHABC_SAD_STRUCT;

/* DRAM_RULE_CFG2_15_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83bc)                                                      */
/*      SPRB0 (0x21fe83bc)                                                      */
/*      SPRHBM (0x21fe83bc)                                                     */
/*      SPRC0 (0x21fe83bc)                                                      */
/*      SPRMCC (0x21fe83bc)                                                     */
/*      SPRUCC (0x21fe83bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_15_CHABC_SAD_REG 0x030203BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_15_CHABC_SAD_STRUCT;

/* DRAM_H0_RT0_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83c0)                                                      */
/*      SPRB0 (0x21fe83c0)                                                      */
/*      SPRHBM (0x21fe83c0)                                                     */
/*      SPRC0 (0x21fe83c0)                                                      */
/*      SPRMCC (0x21fe83c0)                                                     */
/*      SPRUCC (0x21fe83c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT0_MODE0_CHABC_SAD_REG 0x030203C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT0_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H0_RT0_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83c4)                                                      */
/*      SPRB0 (0x21fe83c4)                                                      */
/*      SPRHBM (0x21fe83c4)                                                     */
/*      SPRC0 (0x21fe83c4)                                                      */
/*      SPRMCC (0x21fe83c4)                                                     */
/*      SPRUCC (0x21fe83c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT0_MODE1_CHABC_SAD_REG 0x030203C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT0_MODE1_CHABC_SAD_STRUCT;

/* DRAM_H1_RT0_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83c8)                                                      */
/*      SPRB0 (0x21fe83c8)                                                      */
/*      SPRHBM (0x21fe83c8)                                                     */
/*      SPRC0 (0x21fe83c8)                                                      */
/*      SPRMCC (0x21fe83c8)                                                     */
/*      SPRUCC (0x21fe83c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT0_MODE0_CHABC_SAD_REG 0x030203C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT0_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H1_RT0_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83cc)                                                      */
/*      SPRB0 (0x21fe83cc)                                                      */
/*      SPRHBM (0x21fe83cc)                                                     */
/*      SPRC0 (0x21fe83cc)                                                      */
/*      SPRMCC (0x21fe83cc)                                                     */
/*      SPRUCC (0x21fe83cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT0_MODE1_CHABC_SAD_REG 0x030203CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT0_MODE1_CHABC_SAD_STRUCT;

/* DRAM_H0_RT1_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83d0)                                                      */
/*      SPRB0 (0x21fe83d0)                                                      */
/*      SPRHBM (0x21fe83d0)                                                     */
/*      SPRC0 (0x21fe83d0)                                                      */
/*      SPRMCC (0x21fe83d0)                                                     */
/*      SPRUCC (0x21fe83d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT1_MODE0_CHABC_SAD_REG 0x030203D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT1_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H0_RT1_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83d4)                                                      */
/*      SPRB0 (0x21fe83d4)                                                      */
/*      SPRHBM (0x21fe83d4)                                                     */
/*      SPRC0 (0x21fe83d4)                                                      */
/*      SPRMCC (0x21fe83d4)                                                     */
/*      SPRUCC (0x21fe83d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT1_MODE1_CHABC_SAD_REG 0x030203D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT1_MODE1_CHABC_SAD_STRUCT;

/* DRAM_H1_RT1_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83d8)                                                      */
/*      SPRB0 (0x21fe83d8)                                                      */
/*      SPRHBM (0x21fe83d8)                                                     */
/*      SPRC0 (0x21fe83d8)                                                      */
/*      SPRMCC (0x21fe83d8)                                                     */
/*      SPRUCC (0x21fe83d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT1_MODE0_CHABC_SAD_REG 0x030203D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT1_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H1_RT1_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83dc)                                                      */
/*      SPRB0 (0x21fe83dc)                                                      */
/*      SPRHBM (0x21fe83dc)                                                     */
/*      SPRC0 (0x21fe83dc)                                                      */
/*      SPRMCC (0x21fe83dc)                                                     */
/*      SPRUCC (0x21fe83dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT1_MODE1_CHABC_SAD_REG 0x030203DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT1_MODE1_CHABC_SAD_STRUCT;

/* DRAM_H0_RT2_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83e0)                                                      */
/*      SPRB0 (0x21fe83e0)                                                      */
/*      SPRHBM (0x21fe83e0)                                                     */
/*      SPRC0 (0x21fe83e0)                                                      */
/*      SPRMCC (0x21fe83e0)                                                     */
/*      SPRUCC (0x21fe83e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT2_MODE0_CHABC_SAD_REG 0x030203E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT2_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H0_RT2_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83e4)                                                      */
/*      SPRB0 (0x21fe83e4)                                                      */
/*      SPRHBM (0x21fe83e4)                                                     */
/*      SPRC0 (0x21fe83e4)                                                      */
/*      SPRMCC (0x21fe83e4)                                                     */
/*      SPRUCC (0x21fe83e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT2_MODE1_CHABC_SAD_REG 0x030203E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT2_MODE1_CHABC_SAD_STRUCT;

/* DRAM_H1_RT2_MODE0_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83e8)                                                      */
/*      SPRB0 (0x21fe83e8)                                                      */
/*      SPRHBM (0x21fe83e8)                                                     */
/*      SPRC0 (0x21fe83e8)                                                      */
/*      SPRMCC (0x21fe83e8)                                                     */
/*      SPRUCC (0x21fe83e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT2_MODE0_CHABC_SAD_REG 0x030203E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT2_MODE0_CHABC_SAD_STRUCT;

/* DRAM_H1_RT2_MODE1_CHABC_SAD_REG supported on:                                */
/*      SPRA0 (0x21fe83ec)                                                      */
/*      SPRB0 (0x21fe83ec)                                                      */
/*      SPRHBM (0x21fe83ec)                                                     */
/*      SPRC0 (0x21fe83ec)                                                      */
/*      SPRMCC (0x21fe83ec)                                                     */
/*      SPRUCC (0x21fe83ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT2_MODE1_CHABC_SAD_REG 0x030203EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT2_MODE1_CHABC_SAD_STRUCT;

/* DRAM_MC_CHANNEL_CHABC_SAD_REG supported on:                                  */
/*      SPRA0 (0x21fe83f0)                                                      */
/*      SPRB0 (0x21fe83f0)                                                      */
/*      SPRHBM (0x21fe83f0)                                                     */
/*      SPRC0 (0x21fe83f0)                                                      */
/*      SPRMCC (0x21fe83f0)                                                     */
/*      SPRUCC (0x21fe83f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies which route table each DRAM_RULE will use to determine its target ch_ID
00 - means the target memory channel comes from route table 0
01 - means the target memory channel comes from route table 1
10 - means the target memory channel comes from route table 2
11-Reserved
There are 16 fields for each of the 24 dram rules. Field mc_channel_N specifies the mechanism for determining the channel_ID for dram rule N.
*/


#define DRAM_MC_CHANNEL_CHABC_SAD_REG 0x030203F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_channel_0 : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 0 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_1 : 2;

                            /* Bits[3:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 1 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_2 : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 2 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_3 : 2;

                            /* Bits[7:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 3 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_4 : 2;

                            /* Bits[9:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 4 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_5 : 2;

                            /* Bits[11:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 5 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_6 : 2;

                            /* Bits[13:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 6 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_7 : 2;

                            /* Bits[15:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 7 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_8 : 2;

                            /* Bits[17:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 8 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_9 : 2;

                            /* Bits[19:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 9 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_10 : 2;

                            /* Bits[21:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 10 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_11 : 2;

                            /* Bits[23:22], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 11 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_12 : 2;

                            /* Bits[25:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 12 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_13 : 2;

                            /* Bits[27:26], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 13 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_14 : 2;

                            /* Bits[29:28], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 14 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_15 : 2;

                            /* Bits[31:30], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 15 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */

  } Bits;
  UINT32 Data;

} DRAM_MC_CHANNEL_CHABC_SAD_STRUCT;

/* DRAM_MC_TARGET_CHABC_SAD_REG supported on:                                   */
/*      SPRA0 (0x21fe83f4)                                                      */
/*      SPRB0 (0x21fe83f4)                                                      */
/*      SPRHBM (0x21fe83f4)                                                     */
/*      SPRC0 (0x21fe83f4)                                                      */
/*      SPRMCC (0x21fe83f4)                                                     */
/*      SPRUCC (0x21fe83f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Contains a set of 2 bit fields which individually specify for each DRAM rule which route table output will be used to determine that rule's MC target.
00 - means the target memory channel comes from route table 0
01 - means the target memory channel comes from route table 1
10 - means the target memory channel comes from route table 2
11-means the target memory channel comes from route table 2LM
There are 16 fields for each of the 24 dram rules. Field mc_channel_N specifies the mechanism for determining the channel_ID for dram rule N.
*/


#define DRAM_MC_TARGET_CHABC_SAD_REG 0x030203F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_target_0 : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_0 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_1 : 2;

                            /* Bits[3:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_1 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_2 : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_2 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_3 : 2;

                            /* Bits[7:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_3 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_4 : 2;

                            /* Bits[9:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_4 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_5 : 2;

                            /* Bits[11:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_5 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_6 : 2;

                            /* Bits[13:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_6 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_7 : 2;

                            /* Bits[15:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_7 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_8 : 2;

                            /* Bits[17:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_8 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_9 : 2;

                            /* Bits[19:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_9 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_10 : 2;

                            /* Bits[21:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_10 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_11 : 2;

                            /* Bits[23:22], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_11 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_12 : 2;

                            /* Bits[25:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_12 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_13 : 2;

                            /* Bits[27:26], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_13 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_14 : 2;

                            /* Bits[29:28], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_14 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_15 : 2;

                            /* Bits[31:30], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_15 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */

  } Bits;
  UINT32 Data;

} DRAM_MC_TARGET_CHABC_SAD_STRUCT;

/* DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_REG supported on:                           */
/*      SPRA0 (0x21fe83f8)                                                      */
/*      SPRB0 (0x21fe83f8)                                                      */
/*      SPRHBM (0x21fe83f8)                                                     */
/*      SPRC0 (0x21fe83f8)                                                      */
/*      SPRMCC (0x21fe83f8)                                                     */
/*      SPRUCC (0x21fe83f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_0_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Contains the base interleave granularity for each of the dram interleaves for all route tables- RT0,RT1,RT2
*/


#define DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_REG 0x030203F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt0_interleave_mode : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT0 route table indexing
                               logic 0 - use [8:6] when in a power of two mode.
                               Use [51:6] as input to the mod3 for 3-way
                               interleave modes. 1 - reserved 2 - use [10:8]
                               when in a power of two mode. Use [51:8] as input
                               to the mod3 for 3-way interleave modes. this is
                               the 256B interleave mode without XOR 3 -
                               reserved 4 - reserved 5 - reserved 6 - use
                               [14:12] when in a power of two mode. Use [51:12]
                               as input to the mod3 for 3-way interleave modes
                               7 - use the following bit-wise XOR as the index
                               [10:8] ^ [16:14] ^ [24:22] in power of two mode.
                               Use {51:11, [10:8] ^ [16:14] ^ [24:22]} as input
                               to the mod3 for 3-way interleave modes. This is
                               the perf recommended 256B-XOR interleave mode
                               for power of 2.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt1_interleave_shift : 3;

                            /* Bits[6:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT1 route table indexing
                               logic 0 use [8:6] when in a power of two mode.
                               Use [51:6] as input to the mod3 for 3-way
                               interleave modes. 1 reserved 2 use [10:8] when
                               in a power of two mode. Use [51:8] as input to
                               the mod3 for 3-way interleave modes. this is the
                               256B interleave mode without XOR 3 reserved 4
                               reserved 5 reserved 6 - use [14:12] when in a
                               power of two mode. Use [51:12] as input to the
                               mod3 for 3-way interleave modes 7 - use the
                               following bit-wise XOR as the index [10:8] ^
                               [16:14] ^ [24:22] in power of two mode. Use
                               {51:11, [10:8] ^ [16:14] ^ [24:22]} as input to
                               the mod3 for 3-way interleave modes. This is the
                               perf recommended 256B-XOR interleave mode for
                               power of 2.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt2_interleave_shift : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT2 route table indexing
                               logic. 0 reserved 1 reserved 2 use [9:8] when in
                               power of two mode. Use [51:8] as input to the
                               mod3 for 3-way interleave modes (used in
                               mirroring modes) 3 reserved 4 reserved 5
                               reserved 6 use [13:12] when in power of 2 mode.
                               Use [51:12] as input to the mod3 for 3-way
                               interleave modes. (Standard 4KB page granular,
                               low BW memory interleave mode) 7-reserved
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_STRUCT;

/* CBOMCACONFIG_LBW_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8800)                                                      */
/*      SPRB0 (0x21fe8800)                                                      */
/*      SPRHBM (0x21fe8800)                                                     */
/*      SPRC0 (0x21fe8800)                                                      */
/*      SPRMCC (0x21fe8800)                                                     */
/*      SPRUCC (0x21fe8800)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SnoopFanoutRouter Intel UPI-Port-Selection Control.  If a request comes in from NodeID=i, a fanout snoop is sent to the respective Intel UPI for which the bits in Send2KtiPort_i are set, if the ith bit in CHA_RTR_SNP_CTL1.Enable_Fanout is set to 1.
*/


#define CBOMCACONFIG_LBW_CHABC_SAD_REG 0x03020800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clr_sbit : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This allows us to trigger CMCI and not MCErr for
                               all LLC Evictions, when we are in poison
                               recovery enable mode. Set to 1 to have JKT like
                               behavior which will trigger MCErr. Setting it to
                               0 will trigger CMCI instead.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CBOMCACONFIG_LBW_CHABC_SAD_STRUCT;

/* CHA_RTR_SNP_CTL0_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe8808)                                                      */
/*      SPRB0 (0x21fe8808)                                                      */
/*      SPRHBM (0x21fe8808)                                                     */
/*      SPRC0 (0x21fe8808)                                                      */
/*      SPRMCC (0x21fe8808)                                                     */
/*      SPRUCC (0x21fe8808)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SnoopFanoutRouter Intel UPI-Port-Selection Control.  If a request comes in from NodeID=i, a fanout snoop is sent to the respective Intel UPI for which the bits in Send2KtiPort_i are set, if the ith bit in CHA_RTR_SNP_CTL1.Enable_Fanout is set to 1.
*/


#define CHA_RTR_SNP_CTL0_CHABC_SAD_REG 0x03020808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 send2ktiport_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 0, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_0 are set, if
                               the 0th bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 1, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_1 are set, if
                               the 1st bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 2, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_2 are set, if
                               the 2nd bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 3, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_3 are set, if
                               the 3rd bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 4, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_4 are set, if
                               the 4th bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 5, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_5 are set, if
                               the 5th bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 6, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_6 are set, if
                               the 6th bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */
    UINT32 send2ktiport_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               SnoopFanoutRouter Intel UPI-Port-Selection
                               Control. If a request comes in from NodeID= 7, a
                               fanout snoop is sent to the respective Intel UPI
                               for which the bits in Send2KtiPort_7 are set, if
                               the 7th bit in CHA_RTR_SNP_CTL1.Enable_Fanout is
                               set to 1.
                            */

  } Bits;
  UINT32 Data;

} CHA_RTR_SNP_CTL0_CHABC_SAD_STRUCT;

/* CHA_RTR_SNP_CTL1_CHABC_SAD_REG supported on:                                 */
/*      SPRA0 (0x21fe880c)                                                      */
/*      SPRB0 (0x21fe880c)                                                      */
/*      SPRHBM (0x21fe880c)                                                     */
/*      SPRC0 (0x21fe880c)                                                      */
/*      SPRMCC (0x21fe880c)                                                     */
/*      SPRUCC (0x21fe880c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/EGRESS/CFG_CBO_BCAST_0_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SnoopFanoutRouter Enable Control Register
*/


#define CHA_RTR_SNP_CTL1_CHABC_SAD_REG 0x0302080C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_fanout : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* SnoopFanoutRouter Enable Control Register */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CHA_RTR_SNP_CTL1_CHABC_SAD_STRUCT;







#endif /* _CHABC_SAD_h */
