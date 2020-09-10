
/** @file
  CHABC_SAD1.h

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


#ifndef _CHABC_SAD1_h
#define _CHABC_SAD1_h
#include <Base.h>

/* VID_BCAST_1_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x11fe9000)                                                      */
/*      SPRB0 (0x11fe9000)                                                      */
/*      SPRHBM (0x11fe9000)                                                     */
/*      SPRC0 (0x11fe9000)                                                      */
/*      SPRMCC (0x11fe9000)                                                     */
/*      SPRUCC (0x11fe9000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_BCAST_1_CHABC_SAD1_REG 0x03110000

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

} VID_BCAST_1_CHABC_SAD1_STRUCT;

/* DID_BCAST_1_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x11fe9002)                                                      */
/*      SPRB0 (0x11fe9002)                                                      */
/*      SPRHBM (0x11fe9002)                                                     */
/*      SPRC0 (0x11fe9002)                                                      */
/*      SPRMCC (0x11fe9002)                                                     */
/*      SPRUCC (0x11fe9002)                                                     */
/* Register default value on SPRA0: 0x00003457                                  */
/* Register default value on SPRB0: 0x00003457                                  */
/* Register default value on SPRHBM: 0x00003457                                 */
/* Register default value on SPRC0: 0x00003457                                  */
/* Register default value on SPRMCC: 0x00003457                                 */
/* Register default value on SPRUCC: 0x00003457                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_BCAST_1_CHABC_SAD1_REG 0x03110002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003457*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_BCAST_1_CHABC_SAD1_STRUCT;

/* PCICMD_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x11fe9004)                                                      */
/*      SPRB0 (0x11fe9004)                                                      */
/*      SPRHBM (0x11fe9004)                                                     */
/*      SPRC0 (0x11fe9004)                                                      */
/*      SPRMCC (0x11fe9004)                                                     */
/*      SPRUCC (0x11fe9004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_BCAST_1_CHABC_SAD1_REG 0x03110004

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

} PCICMD_BCAST_1_CHABC_SAD1_STRUCT;

/* PCISTS_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x11fe9006)                                                      */
/*      SPRB0 (0x11fe9006)                                                      */
/*      SPRHBM (0x11fe9006)                                                     */
/*      SPRC0 (0x11fe9006)                                                      */
/*      SPRMCC (0x11fe9006)                                                     */
/*      SPRUCC (0x11fe9006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_BCAST_1_CHABC_SAD1_REG 0x03110006

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

} PCISTS_BCAST_1_CHABC_SAD1_STRUCT;

/* RID_CCR_BCAST_1_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9008)                                                      */
/*      SPRB0 (0x21fe9008)                                                      */
/*      SPRHBM (0x21fe9008)                                                     */
/*      SPRC0 (0x21fe9008)                                                      */
/*      SPRMCC (0x21fe9008)                                                     */
/*      SPRUCC (0x21fe9008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_BCAST_1_CHABC_SAD1_REG 0x03120008

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

} RID_CCR_BCAST_1_CHABC_SAD1_STRUCT;

/* CLSR_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x1fe900c)                                                       */
/*      SPRB0 (0x1fe900c)                                                       */
/*      SPRHBM (0x1fe900c)                                                      */
/*      SPRC0 (0x1fe900c)                                                       */
/*      SPRMCC (0x1fe900c)                                                      */
/*      SPRUCC (0x1fe900c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_BCAST_1_CHABC_SAD1_REG 0x0310000C

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

} CLSR_BCAST_1_CHABC_SAD1_STRUCT;

/* PLAT_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x1fe900d)                                                       */
/*      SPRB0 (0x1fe900d)                                                       */
/*      SPRHBM (0x1fe900d)                                                      */
/*      SPRC0 (0x1fe900d)                                                       */
/*      SPRMCC (0x1fe900d)                                                      */
/*      SPRUCC (0x1fe900d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_BCAST_1_CHABC_SAD1_REG 0x0310000D

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

} PLAT_BCAST_1_CHABC_SAD1_STRUCT;

/* HDR_BCAST_1_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x1fe900e)                                                       */
/*      SPRB0 (0x1fe900e)                                                       */
/*      SPRHBM (0x1fe900e)                                                      */
/*      SPRC0 (0x1fe900e)                                                       */
/*      SPRMCC (0x1fe900e)                                                      */
/*      SPRUCC (0x1fe900e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_BCAST_1_CHABC_SAD1_REG 0x0310000E

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

} HDR_BCAST_1_CHABC_SAD1_STRUCT;

/* BIST_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x1fe900f)                                                       */
/*      SPRB0 (0x1fe900f)                                                       */
/*      SPRHBM (0x1fe900f)                                                      */
/*      SPRC0 (0x1fe900f)                                                       */
/*      SPRMCC (0x1fe900f)                                                      */
/*      SPRUCC (0x1fe900f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_BCAST_1_CHABC_SAD1_REG 0x0310000F

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

} BIST_BCAST_1_CHABC_SAD1_STRUCT;

/* SVID_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x11fe902c)                                                      */
/*      SPRB0 (0x11fe902c)                                                      */
/*      SPRHBM (0x11fe902c)                                                     */
/*      SPRC0 (0x11fe902c)                                                      */
/*      SPRMCC (0x11fe902c)                                                     */
/*      SPRUCC (0x11fe902c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_BCAST_1_CHABC_SAD1_REG 0x0311002C

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

} SVID_BCAST_1_CHABC_SAD1_STRUCT;

/* SDID_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x11fe902e)                                                      */
/*      SPRB0 (0x11fe902e)                                                      */
/*      SPRHBM (0x11fe902e)                                                     */
/*      SPRC0 (0x11fe902e)                                                      */
/*      SPRMCC (0x11fe902e)                                                     */
/*      SPRUCC (0x11fe902e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_BCAST_1_CHABC_SAD1_REG 0x0311002E

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

} SDID_BCAST_1_CHABC_SAD1_STRUCT;

/* CAPPTR_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x1fe9034)                                                       */
/*      SPRB0 (0x1fe9034)                                                       */
/*      SPRHBM (0x1fe9034)                                                      */
/*      SPRC0 (0x1fe9034)                                                       */
/*      SPRMCC (0x1fe9034)                                                      */
/*      SPRUCC (0x1fe9034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_BCAST_1_CHABC_SAD1_REG 0x03100034

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

} CAPPTR_BCAST_1_CHABC_SAD1_STRUCT;

/* INTL_BCAST_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x1fe903c)                                                       */
/*      SPRB0 (0x1fe903c)                                                       */
/*      SPRHBM (0x1fe903c)                                                      */
/*      SPRC0 (0x1fe903c)                                                       */
/*      SPRMCC (0x1fe903c)                                                      */
/*      SPRUCC (0x1fe903c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_BCAST_1_CHABC_SAD1_REG 0x0310003C

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

} INTL_BCAST_1_CHABC_SAD1_STRUCT;

/* INTPIN_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x1fe903d)                                                       */
/*      SPRB0 (0x1fe903d)                                                       */
/*      SPRHBM (0x1fe903d)                                                      */
/*      SPRC0 (0x1fe903d)                                                       */
/*      SPRMCC (0x1fe903d)                                                      */
/*      SPRUCC (0x1fe903d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_BCAST_1_CHABC_SAD1_REG 0x0310003D

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

} INTPIN_BCAST_1_CHABC_SAD1_STRUCT;

/* MINGNT_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x1fe903e)                                                       */
/*      SPRB0 (0x1fe903e)                                                       */
/*      SPRHBM (0x1fe903e)                                                      */
/*      SPRC0 (0x1fe903e)                                                       */
/*      SPRMCC (0x1fe903e)                                                      */
/*      SPRUCC (0x1fe903e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_BCAST_1_CHABC_SAD1_REG 0x0310003E

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

} MINGNT_BCAST_1_CHABC_SAD1_STRUCT;

/* MAXLAT_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x1fe903f)                                                       */
/*      SPRB0 (0x1fe903f)                                                       */
/*      SPRHBM (0x1fe903f)                                                      */
/*      SPRC0 (0x1fe903f)                                                       */
/*      SPRMCC (0x1fe903f)                                                      */
/*      SPRUCC (0x1fe903f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_BCAST_1_CHABC_SAD1_REG 0x0310003F

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

} MAXLAT_BCAST_1_CHABC_SAD1_STRUCT;

/* PXPCAP_BCAST_1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9040)                                                      */
/*      SPRB0 (0x21fe9040)                                                      */
/*      SPRHBM (0x21fe9040)                                                     */
/*      SPRC0 (0x21fe9040)                                                      */
/*      SPRMCC (0x21fe9040)                                                     */
/*      SPRUCC (0x21fe9040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_BCAST_1_CHABC_SAD1_REG 0x03120040

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

} PXPCAP_BCAST_1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD0_BCAST_1_CHABC_SAD1_REG supported on:                              */
/*      SPRA0 (0x21fe9044)                                                      */
/*      SPRB0 (0x21fe9044)                                                      */
/*      SPRHBM (0x21fe9044)                                                     */
/*      SPRC0 (0x21fe9044)                                                      */
/*      SPRMCC (0x21fe9044)                                                     */
/*      SPRUCC (0x21fe9044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_BCAST_1_CHABC_SAD1_REG 0x03120044

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

} PCIE_RSVD0_BCAST_1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD1_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9048)                                                      */
/*      SPRB0 (0x21fe9048)                                                      */
/*      SPRHBM (0x21fe9048)                                                     */
/*      SPRC0 (0x21fe9048)                                                      */
/*      SPRMCC (0x21fe9048)                                                     */
/*      SPRUCC (0x21fe9048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_BCAST_1_N0_CHABC_SAD1_REG 0x03120048

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

} PCIE_RSVD1_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD1_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe904c)                                                      */
/*      SPRB0 (0x21fe904c)                                                      */
/*      SPRHBM (0x21fe904c)                                                     */
/*      SPRC0 (0x21fe904c)                                                      */
/*      SPRMCC (0x21fe904c)                                                     */
/*      SPRUCC (0x21fe904c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_BCAST_1_N1_CHABC_SAD1_REG 0x0312004C

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

} PCIE_RSVD1_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD2_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9050)                                                      */
/*      SPRB0 (0x21fe9050)                                                      */
/*      SPRHBM (0x21fe9050)                                                     */
/*      SPRC0 (0x21fe9050)                                                      */
/*      SPRMCC (0x21fe9050)                                                     */
/*      SPRUCC (0x21fe9050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_BCAST_1_N0_CHABC_SAD1_REG 0x03120050

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

} PCIE_RSVD2_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD2_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9054)                                                      */
/*      SPRB0 (0x21fe9054)                                                      */
/*      SPRHBM (0x21fe9054)                                                     */
/*      SPRC0 (0x21fe9054)                                                      */
/*      SPRMCC (0x21fe9054)                                                     */
/*      SPRUCC (0x21fe9054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_BCAST_1_N1_CHABC_SAD1_REG 0x03120054

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

} PCIE_RSVD2_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD3_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9058)                                                      */
/*      SPRB0 (0x21fe9058)                                                      */
/*      SPRHBM (0x21fe9058)                                                     */
/*      SPRC0 (0x21fe9058)                                                      */
/*      SPRMCC (0x21fe9058)                                                     */
/*      SPRUCC (0x21fe9058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_BCAST_1_N0_CHABC_SAD1_REG 0x03120058

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

} PCIE_RSVD3_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD3_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe905c)                                                      */
/*      SPRB0 (0x21fe905c)                                                      */
/*      SPRHBM (0x21fe905c)                                                     */
/*      SPRC0 (0x21fe905c)                                                      */
/*      SPRMCC (0x21fe905c)                                                     */
/*      SPRUCC (0x21fe905c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_BCAST_1_N1_CHABC_SAD1_REG 0x0312005C

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

} PCIE_RSVD3_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD4_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9060)                                                      */
/*      SPRB0 (0x21fe9060)                                                      */
/*      SPRHBM (0x21fe9060)                                                     */
/*      SPRC0 (0x21fe9060)                                                      */
/*      SPRMCC (0x21fe9060)                                                     */
/*      SPRUCC (0x21fe9060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_BCAST_1_N0_CHABC_SAD1_REG 0x03120060

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

} PCIE_RSVD4_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD4_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9064)                                                      */
/*      SPRB0 (0x21fe9064)                                                      */
/*      SPRHBM (0x21fe9064)                                                     */
/*      SPRC0 (0x21fe9064)                                                      */
/*      SPRMCC (0x21fe9064)                                                     */
/*      SPRUCC (0x21fe9064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_BCAST_1_N1_CHABC_SAD1_REG 0x03120064

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

} PCIE_RSVD4_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD5_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9068)                                                      */
/*      SPRB0 (0x21fe9068)                                                      */
/*      SPRHBM (0x21fe9068)                                                     */
/*      SPRC0 (0x21fe9068)                                                      */
/*      SPRMCC (0x21fe9068)                                                     */
/*      SPRUCC (0x21fe9068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_BCAST_1_N0_CHABC_SAD1_REG 0x03120068

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

} PCIE_RSVD5_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD5_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe906c)                                                      */
/*      SPRB0 (0x21fe906c)                                                      */
/*      SPRHBM (0x21fe906c)                                                     */
/*      SPRC0 (0x21fe906c)                                                      */
/*      SPRMCC (0x21fe906c)                                                     */
/*      SPRUCC (0x21fe906c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_BCAST_1_N1_CHABC_SAD1_REG 0x0312006C

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

} PCIE_RSVD5_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD6_BCAST_1_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9070)                                                      */
/*      SPRB0 (0x21fe9070)                                                      */
/*      SPRHBM (0x21fe9070)                                                     */
/*      SPRC0 (0x21fe9070)                                                      */
/*      SPRMCC (0x21fe9070)                                                     */
/*      SPRUCC (0x21fe9070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_BCAST_1_N0_CHABC_SAD1_REG 0x03120070

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

} PCIE_RSVD6_BCAST_1_N0_CHABC_SAD1_STRUCT;

/* PCIE_RSVD6_BCAST_1_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9074)                                                      */
/*      SPRB0 (0x21fe9074)                                                      */
/*      SPRHBM (0x21fe9074)                                                     */
/*      SPRC0 (0x21fe9074)                                                      */
/*      SPRMCC (0x21fe9074)                                                     */
/*      SPRUCC (0x21fe9074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_BCAST_1_N1_CHABC_SAD1_REG 0x03120074

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

} PCIE_RSVD6_BCAST_1_N1_CHABC_SAD1_STRUCT;

/* PCIE_RSVD7_BCAST_1_CHABC_SAD1_REG supported on:                              */
/*      SPRA0 (0x21fe9078)                                                      */
/*      SPRB0 (0x21fe9078)                                                      */
/*      SPRHBM (0x21fe9078)                                                     */
/*      SPRC0 (0x21fe9078)                                                      */
/*      SPRMCC (0x21fe9078)                                                     */
/*      SPRUCC (0x21fe9078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_BCAST_1_CHABC_SAD1_REG 0x03120078

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

} PCIE_RSVD7_BCAST_1_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_0_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe9090)                                                      */
/*      SPRB0 (0x21fe9090)                                                      */
/*      SPRHBM (0x21fe9090)                                                     */
/*      SPRC0 (0x21fe9090)                                                      */
/*      SPRMCC (0x21fe9090)                                                     */
/*      SPRUCC (0x21fe9090)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_0_CHABC_SAD1_REG 0x03120090

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

} IOAPIC_TARGET_LIST_CFG_0_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_1_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe9094)                                                      */
/*      SPRB0 (0x21fe9094)                                                      */
/*      SPRHBM (0x21fe9094)                                                     */
/*      SPRC0 (0x21fe9094)                                                      */
/*      SPRMCC (0x21fe9094)                                                     */
/*      SPRUCC (0x21fe9094)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_1_CHABC_SAD1_REG 0x03120094

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

} IOAPIC_TARGET_LIST_CFG_1_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_2_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe9098)                                                      */
/*      SPRB0 (0x21fe9098)                                                      */
/*      SPRHBM (0x21fe9098)                                                     */
/*      SPRC0 (0x21fe9098)                                                      */
/*      SPRMCC (0x21fe9098)                                                     */
/*      SPRUCC (0x21fe9098)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_2_CHABC_SAD1_REG 0x03120098

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

} IOAPIC_TARGET_LIST_CFG_2_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_3_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe909c)                                                      */
/*      SPRB0 (0x21fe909c)                                                      */
/*      SPRHBM (0x21fe909c)                                                     */
/*      SPRC0 (0x21fe909c)                                                      */
/*      SPRMCC (0x21fe909c)                                                     */
/*      SPRUCC (0x21fe909c)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_3_CHABC_SAD1_REG 0x0312009C

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

} IOAPIC_TARGET_LIST_CFG_3_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_4_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe90a0)                                                      */
/*      SPRB0 (0x21fe90a0)                                                      */
/*      SPRHBM (0x21fe90a0)                                                     */
/*      SPRC0 (0x21fe90a0)                                                      */
/*      SPRMCC (0x21fe90a0)                                                     */
/*      SPRUCC (0x21fe90a0)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_4_CHABC_SAD1_REG 0x031200A0

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

} IOAPIC_TARGET_LIST_CFG_4_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_5_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe90a4)                                                      */
/*      SPRB0 (0x21fe90a4)                                                      */
/*      SPRHBM (0x21fe90a4)                                                     */
/*      SPRC0 (0x21fe90a4)                                                      */
/*      SPRMCC (0x21fe90a4)                                                     */
/*      SPRUCC (0x21fe90a4)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_5_CHABC_SAD1_REG 0x031200A4

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

} IOAPIC_TARGET_LIST_CFG_5_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_6_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe90a8)                                                      */
/*      SPRB0 (0x21fe90a8)                                                      */
/*      SPRHBM (0x21fe90a8)                                                     */
/*      SPRC0 (0x21fe90a8)                                                      */
/*      SPRMCC (0x21fe90a8)                                                     */
/*      SPRUCC (0x21fe90a8)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_6_CHABC_SAD1_REG 0x031200A8

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

} IOAPIC_TARGET_LIST_CFG_6_CHABC_SAD1_STRUCT;

/* IOAPIC_TARGET_LIST_CFG_7_CHABC_SAD1_REG supported on:                        */
/*      SPRA0 (0x21fe90ac)                                                      */
/*      SPRB0 (0x21fe90ac)                                                      */
/*      SPRHBM (0x21fe90ac)                                                     */
/*      SPRC0 (0x21fe90ac)                                                      */
/*      SPRMCC (0x21fe90ac)                                                     */
/*      SPRUCC (0x21fe90ac)                                                     */
/* Register default value on SPRA0: 0x10101010                                  */
/* Register default value on SPRB0: 0x10101010                                  */
/* Register default value on SPRHBM: 0x10101010                                 */
/* Register default value on SPRC0: 0x10101010                                  */
/* Register default value on SPRMCC: 0x10101010                                 */
/* Register default value on SPRUCC: 0x10101010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define IOAPIC_TARGET_LIST_CFG_7_CHABC_SAD1_REG 0x031200AC

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

} IOAPIC_TARGET_LIST_CFG_7_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG supported on:                  */
/*      SPRA0 (0x21fe90b0)                                                      */
/*      SPRB0 (0x21fe90b0)                                                      */
/*      SPRHBM (0x21fe90b0)                                                     */
/*      SPRC0 (0x21fe90b0)                                                      */
/*      SPRMCC (0x21fe90b0)                                                     */
/*      SPRUCC (0x21fe90b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Maps out the bus base/limits for the locally mapped MMCFG addresses.  Any locally mapped MMCFG
which has a bus_id greater than the last limit bus, will be sent to the ubox.
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit0  base root bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit1  limit bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit2  limit bus for IIO Stack 1
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit3  limit bus for IIO Stack 2
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit0  limit bus for IIO Stack 3
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit1  limit bus for IIO Stack 4
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit2  limit bus for IIO Stack 5
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit3  limit bus for IIO Stack 6
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit0  limit bus for IIO Stack 7
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit1  limit bus for IIO Stack 8
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit2  limit bus for IIO Stack 9
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit3  limit bus for IIO Stack 10
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_3.Limit0  limit bus for IIO Stack 11
*/


#define MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG 0x031200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG supported on:                  */
/*      SPRA0 (0x21fe90b4)                                                      */
/*      SPRB0 (0x21fe90b4)                                                      */
/*      SPRHBM (0x21fe90b4)                                                     */
/*      SPRC0 (0x21fe90b4)                                                      */
/*      SPRMCC (0x21fe90b4)                                                     */
/*      SPRUCC (0x21fe90b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Maps out the bus base/limits for the locally mapped MMCFG addresses.  Any locally mapped MMCFG
which has a bus_id greater than the last limit bus, will be sent to the ubox.
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit0  base root bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit1  limit bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit2  limit bus for IIO Stack 1
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit3  limit bus for IIO Stack 2
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit0  limit bus for IIO Stack 3
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit1  limit bus for IIO Stack 4
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit2  limit bus for IIO Stack 5
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit3  limit bus for IIO Stack 6
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit0  limit bus for IIO Stack 7
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit1  limit bus for IIO Stack 8
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit2  limit bus for IIO Stack 9
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit3  limit bus for IIO Stack 10
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_3.Limit0  limit bus for IIO Stack 11
*/


#define MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG 0x031200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_ADDRESS_CFG_2_CHABC_SAD1_REG supported on:                  */
/*      SPRA0 (0x21fe90b8)                                                      */
/*      SPRB0 (0x21fe90b8)                                                      */
/*      SPRHBM (0x21fe90b8)                                                     */
/*      SPRC0 (0x21fe90b8)                                                      */
/*      SPRMCC (0x21fe90b8)                                                     */
/*      SPRUCC (0x21fe90b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Maps out the bus base/limits for the locally mapped MMCFG addresses.  Any locally mapped MMCFG
which has a bus_id greater than the last limit bus, will be sent to the ubox.
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit0  base root bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit1  limit bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit2  limit bus for IIO Stack 1
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit3  limit bus for IIO Stack 2
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit0  limit bus for IIO Stack 3
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit1  limit bus for IIO Stack 4
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit2  limit bus for IIO Stack 5
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit3  limit bus for IIO Stack 6
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit0  limit bus for IIO Stack 7
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit1  limit bus for IIO Stack 8
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit2  limit bus for IIO Stack 9
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit3  limit bus for IIO Stack 10
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_3.Limit0  limit bus for IIO Stack 11
*/


#define MMCFG_LOCAL_RULE_ADDRESS_CFG_2_CHABC_SAD1_REG 0x031200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_ADDRESS_CFG_2_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_ADDRESS_CFG_3_CHABC_SAD1_REG supported on:                  */
/*      SPRA0 (0x21fe90bc)                                                      */
/*      SPRB0 (0x21fe90bc)                                                      */
/*      SPRHBM (0x21fe90bc)                                                     */
/*      SPRC0 (0x21fe90bc)                                                      */
/*      SPRMCC (0x21fe90bc)                                                     */
/*      SPRUCC (0x21fe90bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Maps out the bus base/limits for the locally mapped MMCFG addresses.  Any locally mapped MMCFG
which has a bus_id greater than the last limit bus, will be sent to the ubox.
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit0  base root bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit1  limit bus for IIO Stack 0
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit2  limit bus for IIO Stack 1
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_0.Limit3  limit bus for IIO Stack 2
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit0  limit bus for IIO Stack 3
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit1  limit bus for IIO Stack 4
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit2  limit bus for IIO Stack 5
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_1.Limit3  limit bus for IIO Stack 6
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit0  limit bus for IIO Stack 7
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit1  limit bus for IIO Stack 8
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit2  limit bus for IIO Stack 9
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_2.Limit3  limit bus for IIO Stack 10
		  MMCFG_LOCAL_RULE_ADDRESS_CFG_3.Limit0  limit bus for IIO Stack 11
*/


#define MMCFG_LOCAL_RULE_ADDRESS_CFG_3_CHABC_SAD1_REG 0x031200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */
    UINT32 limit3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Maps out the bus base/limits for the locally
                               mapped MMCFG addresses
                            */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_ADDRESS_CFG_3_CHABC_SAD1_STRUCT;

/* MMCFG_RULE_N0_CHABC_SAD1_REG supported on:                                   */
/*      SPRA0 (0x21fe90c0)                                                      */
/*      SPRB0 (0x21fe90c0)                                                      */
/*      SPRHBM (0x21fe90c0)                                                     */
/*      SPRC0 (0x21fe90c0)                                                      */
/*      SPRMCC (0x21fe90c0)                                                     */
/*      SPRUCC (0x21fe90c0)                                                     */
/* Register default value on SPRA0: 0x00003FC8                                  */
/* Register default value on SPRB0: 0x00003FC8                                  */
/* Register default value on SPRHBM: 0x00003FC8                                 */
/* Register default value on SPRC0: 0x00003FC8                                  */
/* Register default value on SPRMCC: 0x00003FC8                                 */
/* Register default value on SPRUCC: 0x00003FC8                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMCFG Rule Config
*/


#define MMCFG_RULE_N0_CHABC_SAD1_REG 0x031200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMCFG interleave decoder. */
    UINT32 length : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000004*/

                            /*
                               This field document the max bus number supported
                               by the interleave decoder. The following
                               configurations are supported: 3b110 :
                               MaxBusNumber = 63 (ie. 64MB MMCFG range) 3b101 :
                               MaxBusNumber = 127 (ie 128MB MMCFG range) 3b100
                               : MaxBusNumber = 256 (ie 256MB MMCFG range)
                               3b010 : MaxBusNumber = 512 (ie 512MB MMCFG
                               range) 3b001 : MaxBusNumber = 1048 (ie 1G MMCFG
                               range) 3b000 : MaxBusNumber = 2048 (ie 2G MMCFG
                               range) Minimum granularity of MMCFG range will
                               be 64MB.
                            */
    UINT32 rsvd : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid_targets : 8;

                            /* Bits[13:6], Access Type=RW, default=0x000000FF*/

                            /*
                               Marks the valid targets. Request to an invalid
                               target will result in an ABORT
                            */
    UINT32 rsvd_14 : 12;

                            /* Bits[25:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_address : 6;

                            /* Bits[31:26], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the MMCFG rule
                               base address. The granularity of MMCFG rule is
                               64MB. Note that this interleave decoder can be
                               used for higher segments of the MMCFG and is not
                               restricted to Segment 0 of MMCFG. Check
                               MMCFG_TargetList for Interleaved target list
                               used by this decoder.
                            */

  } Bits;
  UINT32 Data;

} MMCFG_RULE_N0_CHABC_SAD1_STRUCT;

/* MMCFG_RULE_N1_CHABC_SAD1_REG supported on:                                   */
/*      SPRA0 (0x21fe90c4)                                                      */
/*      SPRB0 (0x21fe90c4)                                                      */
/*      SPRHBM (0x21fe90c4)                                                     */
/*      SPRC0 (0x21fe90c4)                                                      */
/*      SPRMCC (0x21fe90c4)                                                     */
/*      SPRUCC (0x21fe90c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMCFG Rule Config
*/


#define MMCFG_RULE_N1_CHABC_SAD1_REG 0x031200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_address : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the MMCFG rule
                               base address. The granularity of MMCFG rule is
                               64MB. Note that this interleave decoder can be
                               used for higher segments of the MMCFG and is not
                               restricted to Segment 0 of MMCFG. Check
                               MMCFG_TargetList for Interleaved target list
                               used by this decoder.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMCFG_RULE_N1_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe90c8)                                                      */
/*      SPRB0 (0x21fe90c8)                                                      */
/*      SPRHBM (0x21fe90c8)                                                     */
/*      SPRC0 (0x21fe90c8)                                                      */
/*      SPRMCC (0x21fe90c8)                                                     */
/*      SPRUCC (0x21fe90c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This register contains -                                                Target for the local bus decode of MMCFG, Target physical stack ID for logical IIO stacks ; It also contains the  Mode bit for managing the root bus for IIO Stacks
		  b1    send all requests which match the root bus for this stack to the ubox
		  b0    send only requests which match the root bus with device ID less than 16 to the ubox
*/


#define MMCFG_LOCAL_RULE_CHABC_SAD1_REG 0x031200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 0.
                            */
    UINT32 target1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 1.
                            */
    UINT32 target2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 2.
                            */
    UINT32 target3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 3.
                            */
    UINT32 target4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 4.
                            */
    UINT32 target5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 5.
                            */
    UINT32 mode0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               0 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode1 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               1 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode2 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               2 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode3 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               3 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode4 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               4 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode5 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               5 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_CHABC_SAD1_STRUCT;

/* MMCFG_LOCAL_RULE_CFG2_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe90cc)                                                      */
/*      SPRB0 (0x21fe90cc)                                                      */
/*      SPRHBM (0x21fe90cc)                                                     */
/*      SPRC0 (0x21fe90cc)                                                      */
/*      SPRMCC (0x21fe90cc)                                                     */
/*      SPRUCC (0x21fe90cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This register contains -                                                Target for the local bus decode of MMCFG, Target physical stack ID for logical IIO stacks ; It also contains the  Mode bit for managing the root bus for IIO Stacks
		  b1    send all requests which match the root bus for this stack to the ubox
		  b0    send only requests which match the root bus with device ID less than 16 to the ubox
*/


#define MMCFG_LOCAL_RULE_CFG2_CHABC_SAD1_REG 0x031200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 0.
                            */
    UINT32 target1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 1.
                            */
    UINT32 target2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 2.
                            */
    UINT32 target3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 3.
                            */
    UINT32 target4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 4.
                            */
    UINT32 target5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Target for the local bus decode of MMCFG. Target
                               physical stack ID for logical IIO Stack 5.
                            */
    UINT32 mode0 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               0 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode1 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               1 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode2 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               2 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode3 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               3 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode4 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               4 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 mode5 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Mode bit for managing the root bus for IIO Stack
                               5 b1 send all requests which match the root bus
                               for this stack to the ubox b0 send only requests
                               which match the root bus with device ID less
                               than 16 to the ubox
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMCFG_LOCAL_RULE_CFG2_CHABC_SAD1_STRUCT;

/* MMCFG_TARGET_LIST_CHABC_SAD1_REG supported on:                               */
/*      SPRA0 (0x21fe90ec)                                                      */
/*      SPRB0 (0x21fe90ec)                                                      */
/*      SPRHBM (0x21fe90ec)                                                     */
/*      SPRC0 (0x21fe90ec)                                                      */
/*      SPRMCC (0x21fe90ec)                                                     */
/*      SPRUCC (0x21fe90ec)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMCFG target.
		  b1xxx  Target is a local IIO Stack.  Actual stack ID target will be determined by MMCFG_Local_Rule_CFG
		  b0xxx  Target is remote socket with NodeID xxx
*/


#define MMCFG_TARGET_LIST_CHABC_SAD1_REG 0x031200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000008*/

                            /*
                               MMCFG target. b1xxx Target is a local IIO Stack.
                               Actual stack ID target will be determined by
                               MMCFG_Local_Rule_CFG b0xxx Target is remote
                               socket with NodeID xxx
                            */

  } Bits;
  UINT32 Data;

} MMCFG_TARGET_LIST_CHABC_SAD1_STRUCT;

/* LT_CONTROL_CHABC_SAD1_REG supported on:                                      */
/*      SPRA0 (0x21fe90f0)                                                      */
/*      SPRB0 (0x21fe90f0)                                                      */
/*      SPRHBM (0x21fe90f0)                                                     */
/*      SPRC0 (0x21fe90f0)                                                      */
/*      SPRMCC (0x21fe90f0)                                                     */
/*      SPRUCC (0x21fe90f0)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Target NodeID of the Intel TXT Target
		  b1xxxx  Target is local IIO Stack (target xxxx is always the legacy stack)
		  b00xxx  Target is remote socket with NodeID xxx . It also has the enable control
*/


#define LT_CONTROL_CHABC_SAD1_REG 0x031200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lt_target : 5;

                            /* Bits[4:0], Access Type=RW/O/L, default=0x00000010*/

                            /* target IIO stack for LT. */
    UINT32 lt_target_enable : 1;

                            /* Bits[5:5], Access Type=RW/O/L, default=0x00000000*/

                            /*
                               when 1 is written, bit[3:0] of this register
                               becomes read-only. sad_control_cfg also becomes
                               read-only when a 1 is written to this bit.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LT_CONTROL_CHABC_SAD1_STRUCT;

/* SAD_CONTROL_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x21fe90f4)                                                      */
/*      SPRB0 (0x21fe90f4)                                                      */
/*      SPRHBM (0x21fe90f4)                                                     */
/*      SPRC0 (0x21fe90f4)                                                      */
/*      SPRMCC (0x21fe90f4)                                                     */
/*      SPRUCC (0x21fe90f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* NodeID of the local Socket. This register can not be overwritten (Read only) if LT_Target_Enable ==1 (Under LT_Control). Same as LT_Target, the value cant change to prevent any attack to Intel TXT.
*/


#define SAD_CONTROL_CHABC_SAD1_REG 0x031200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 local_nodeid : 3;

                            /* Bits[2:0], Access Type=RW/O/L, default=0x00000000*/

                            /*
                               NodeID of the local Socket. This register can
                               not be overwritten (Read only) if
                               LT_Target_Enable ==1 (Under LT_Control). Same as
                               LT_Target, the value cant change to prevent any
                               attack to Intel TXT.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SAD_CONTROL_CHABC_SAD1_STRUCT;

/* SAD_TARGET_CHABC_SAD1_REG supported on:                                      */
/*      SPRA0 (0x21fe90f8)                                                      */
/*      SPRB0 (0x21fe90f8)                                                      */
/*      SPRHBM (0x21fe90f8)                                                     */
/*      SPRC0 (0x21fe90f8)                                                      */
/*      SPRMCC (0x21fe90f8)                                                     */
/*      SPRUCC (0x21fe90f8)                                                     */
/* Register default value on SPRA0: 0x00000410                                  */
/* Register default value on SPRB0: 0x00000410                                  */
/* Register default value on SPRHBM: 0x00000410                                 */
/* Register default value on SPRC0: 0x00000410                                  */
/* Register default value on SPRMCC: 0x00000410                                 */
/* Register default value on SPRUCC: 0x00000410                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Target IDs for SAD
*/


#define SAD_TARGET_CHABC_SAD1_REG 0x031200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flash_target : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000010*/

                            /*
                               Target NodeID of the Flash Target b1xxxx Target
                               is the local socket. Target (xxxx) is always the
                               legacy stack. b00xxx Target is remote socket
                               with NodeID xxx. Target will be the legacy stack
                               on that socket.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vga_target : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000010*/

                            /*
                               Target NodeID of the VGA Target b1xxxx Target is
                               the local socket. Target xxxx is always the
                               legacy stack. b00xxx Target is remote socket
                               with NodeID xxx. Target will be the legacy stack
                               on that socket.
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sourceid : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               SourceID of the Socket. Programmable by BIOS. By
                               default, the value should be part of the APICID
                               that represent the socket.
                            */
    UINT32 sourceiden : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Enable SourceID feature. */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 legacy_pch_target : 5;

                            /* Bits[21:17], Access Type=RW, default=0x00000000*/

                            /*
                               Target NodeID of the Legacy PCH Target b1xxxx
                               Target is the local socket. Target is the local
                               stack with stack id xxxx. b00xxx Target is
                               remote socket with NodeID xxx.
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SAD_TARGET_CHABC_SAD1_STRUCT;

/* PXPENHCAP_BCAST_1_CHABC_SAD1_REG supported on:                               */
/*      SPRA0 (0x21fe9100)                                                      */
/*      SPRB0 (0x21fe9100)                                                      */
/*      SPRHBM (0x21fe9100)                                                     */
/*      SPRC0 (0x21fe9100)                                                      */
/*      SPRMCC (0x21fe9100)                                                     */
/*      SPRUCC (0x21fe9100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_BCAST_1_CHABC_SAD1_REG 0x03120100

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

} PXPENHCAP_BCAST_1_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_0_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9108)                                                      */
/*      SPRB0 (0x21fe9108)                                                      */
/*      SPRHBM (0x21fe9108)                                                     */
/*      SPRC0 (0x21fe9108)                                                      */
/*      SPRMCC (0x21fe9108)                                                     */
/*      SPRUCC (0x21fe9108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_0_CHABC_SAD1_REG 0x03120108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_0_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_1_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe910c)                                                      */
/*      SPRB0 (0x21fe910c)                                                      */
/*      SPRHBM (0x21fe910c)                                                     */
/*      SPRC0 (0x21fe910c)                                                      */
/*      SPRMCC (0x21fe910c)                                                     */
/*      SPRUCC (0x21fe910c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_1_CHABC_SAD1_REG 0x0312010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_1_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_2_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9110)                                                      */
/*      SPRB0 (0x21fe9110)                                                      */
/*      SPRHBM (0x21fe9110)                                                     */
/*      SPRC0 (0x21fe9110)                                                      */
/*      SPRMCC (0x21fe9110)                                                     */
/*      SPRUCC (0x21fe9110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_2_CHABC_SAD1_REG 0x03120110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_2_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_3_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9114)                                                      */
/*      SPRB0 (0x21fe9114)                                                      */
/*      SPRHBM (0x21fe9114)                                                     */
/*      SPRC0 (0x21fe9114)                                                      */
/*      SPRMCC (0x21fe9114)                                                     */
/*      SPRUCC (0x21fe9114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_3_CHABC_SAD1_REG 0x03120114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_3_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_4_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9118)                                                      */
/*      SPRB0 (0x21fe9118)                                                      */
/*      SPRHBM (0x21fe9118)                                                     */
/*      SPRC0 (0x21fe9118)                                                      */
/*      SPRMCC (0x21fe9118)                                                     */
/*      SPRUCC (0x21fe9118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_4_CHABC_SAD1_REG 0x03120118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_4_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_5_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe911c)                                                      */
/*      SPRB0 (0x21fe911c)                                                      */
/*      SPRHBM (0x21fe911c)                                                     */
/*      SPRC0 (0x21fe911c)                                                      */
/*      SPRMCC (0x21fe911c)                                                     */
/*      SPRUCC (0x21fe911c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_5_CHABC_SAD1_REG 0x0312011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_5_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_6_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9120)                                                      */
/*      SPRB0 (0x21fe9120)                                                      */
/*      SPRHBM (0x21fe9120)                                                     */
/*      SPRC0 (0x21fe9120)                                                      */
/*      SPRMCC (0x21fe9120)                                                     */
/*      SPRUCC (0x21fe9120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_6_CHABC_SAD1_REG 0x03120120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_6_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_7_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9124)                                                      */
/*      SPRB0 (0x21fe9124)                                                      */
/*      SPRHBM (0x21fe9124)                                                     */
/*      SPRC0 (0x21fe9124)                                                      */
/*      SPRMCC (0x21fe9124)                                                     */
/*      SPRUCC (0x21fe9124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_7_CHABC_SAD1_REG 0x03120124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_7_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_8_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9128)                                                      */
/*      SPRB0 (0x21fe9128)                                                      */
/*      SPRHBM (0x21fe9128)                                                     */
/*      SPRC0 (0x21fe9128)                                                      */
/*      SPRMCC (0x21fe9128)                                                     */
/*      SPRUCC (0x21fe9128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_8_CHABC_SAD1_REG 0x03120128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_8_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_9_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe912c)                                                      */
/*      SPRB0 (0x21fe912c)                                                      */
/*      SPRHBM (0x21fe912c)                                                     */
/*      SPRC0 (0x21fe912c)                                                      */
/*      SPRMCC (0x21fe912c)                                                     */
/*      SPRUCC (0x21fe912c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_9_CHABC_SAD1_REG 0x0312012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_9_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_10_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9130)                                                      */
/*      SPRB0 (0x21fe9130)                                                      */
/*      SPRHBM (0x21fe9130)                                                     */
/*      SPRC0 (0x21fe9130)                                                      */
/*      SPRMCC (0x21fe9130)                                                     */
/*      SPRUCC (0x21fe9130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_10_CHABC_SAD1_REG 0x03120130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_10_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_11_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9134)                                                      */
/*      SPRB0 (0x21fe9134)                                                      */
/*      SPRHBM (0x21fe9134)                                                     */
/*      SPRC0 (0x21fe9134)                                                      */
/*      SPRMCC (0x21fe9134)                                                     */
/*      SPRUCC (0x21fe9134)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_11_CHABC_SAD1_REG 0x03120134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_11_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_12_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9138)                                                      */
/*      SPRB0 (0x21fe9138)                                                      */
/*      SPRHBM (0x21fe9138)                                                     */
/*      SPRC0 (0x21fe9138)                                                      */
/*      SPRMCC (0x21fe9138)                                                     */
/*      SPRUCC (0x21fe9138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_12_CHABC_SAD1_REG 0x03120138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_12_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_13_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe913c)                                                      */
/*      SPRB0 (0x21fe913c)                                                      */
/*      SPRHBM (0x21fe913c)                                                     */
/*      SPRC0 (0x21fe913c)                                                      */
/*      SPRMCC (0x21fe913c)                                                     */
/*      SPRUCC (0x21fe913c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_13_CHABC_SAD1_REG 0x0312013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_13_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_14_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9140)                                                      */
/*      SPRB0 (0x21fe9140)                                                      */
/*      SPRHBM (0x21fe9140)                                                     */
/*      SPRC0 (0x21fe9140)                                                      */
/*      SPRMCC (0x21fe9140)                                                     */
/*      SPRUCC (0x21fe9140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_14_CHABC_SAD1_REG 0x03120140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_14_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_15_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9144)                                                      */
/*      SPRB0 (0x21fe9144)                                                      */
/*      SPRHBM (0x21fe9144)                                                     */
/*      SPRC0 (0x21fe9144)                                                      */
/*      SPRMCC (0x21fe9144)                                                     */
/*      SPRUCC (0x21fe9144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_15_CHABC_SAD1_REG 0x03120144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_15_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_16_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9148)                                                      */
/*      SPRB0 (0x21fe9148)                                                      */
/*      SPRHBM (0x21fe9148)                                                     */
/*      SPRC0 (0x21fe9148)                                                      */
/*      SPRMCC (0x21fe9148)                                                     */
/*      SPRUCC (0x21fe9148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_16_CHABC_SAD1_REG 0x03120148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_16_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_17_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe914c)                                                      */
/*      SPRB0 (0x21fe914c)                                                      */
/*      SPRHBM (0x21fe914c)                                                     */
/*      SPRC0 (0x21fe914c)                                                      */
/*      SPRMCC (0x21fe914c)                                                     */
/*      SPRUCC (0x21fe914c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_17_CHABC_SAD1_REG 0x0312014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_17_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_18_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9150)                                                      */
/*      SPRB0 (0x21fe9150)                                                      */
/*      SPRHBM (0x21fe9150)                                                     */
/*      SPRC0 (0x21fe9150)                                                      */
/*      SPRMCC (0x21fe9150)                                                     */
/*      SPRUCC (0x21fe9150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_18_CHABC_SAD1_REG 0x03120150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_18_CHABC_SAD1_STRUCT;

/* MMIO_RULE_CFG_19_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9154)                                                      */
/*      SPRB0 (0x21fe9154)                                                      */
/*      SPRHBM (0x21fe9154)                                                     */
/*      SPRC0 (0x21fe9154)                                                      */
/*      SPRMCC (0x21fe9154)                                                     */
/*      SPRUCC (0x21fe9154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO RULE Config
*/


#define MMIO_RULE_CFG_19_CHABC_SAD1_REG 0x03120154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 10;

                            /* Bits[11:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */
    UINT32 limitaddress : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[31:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIO target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_RULE_CFG_19_CHABC_SAD1_STRUCT;

/* MMIO_UBOX_RULE_N0_CHABC_SAD1_REG supported on:                               */
/*      SPRA0 (0x21fe9180)                                                      */
/*      SPRB0 (0x21fe9180)                                                      */
/*      SPRHBM (0x21fe9180)                                                     */
/*      SPRC0 (0x21fe9180)                                                      */
/*      SPRMCC (0x21fe9180)                                                     */
/*      SPRUCC (0x21fe9180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO UBOX RULE Config - Uses an MMIOL decoder, but can hit above 4G - base and limit sizes extended accordingly.
*/


#define MMIO_UBOX_RULE_N0_CHABC_SAD1_REG 0x03120180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable for this MMIO UBOX rule. */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baseaddress : 30;

                            /* Bits[31:2], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:22] of the MMIO rule
                               base address. Both base and limit must match to
                               declare a match to this MMIO rule. The
                               granularity of MMIO rule is 64MB.
                            */

  } Bits;
  UINT32 Data;

} MMIO_UBOX_RULE_N0_CHABC_SAD1_STRUCT;

/* MMIO_UBOX_RULE_N1_CHABC_SAD1_REG supported on:                               */
/*      SPRA0 (0x21fe9184)                                                      */
/*      SPRB0 (0x21fe9184)                                                      */
/*      SPRHBM (0x21fe9184)                                                     */
/*      SPRC0 (0x21fe9184)                                                      */
/*      SPRMCC (0x21fe9184)                                                     */
/*      SPRUCC (0x21fe9184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* MMIO UBOX RULE Config - Uses an MMIOL decoder, but can hit above 4G - base and limit sizes extended accordingly.
*/


#define MMIO_UBOX_RULE_N1_CHABC_SAD1_REG 0x03120184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limitaddress : 30;

                            /* Bits[30:1], Access Type=RW, default=0x00000000*/

                            /*
                               This correspond to Addr[51:22] of the MMIO rule
                               top limit address. Both base and limit must
                               match to declare a match to this MMIO rule.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_UBOX_RULE_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_0_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91d8)                                                      */
/*      SPRB0 (0x21fe91d8)                                                      */
/*      SPRHBM (0x21fe91d8)                                                     */
/*      SPRC0 (0x21fe91d8)                                                      */
/*      SPRMCC (0x21fe91d8)                                                     */
/*      SPRUCC (0x21fe91d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_0_N0_CHABC_SAD1_REG 0x031201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_0_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_0_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91dc)                                                      */
/*      SPRB0 (0x21fe91dc)                                                      */
/*      SPRHBM (0x21fe91dc)                                                     */
/*      SPRC0 (0x21fe91dc)                                                      */
/*      SPRMCC (0x21fe91dc)                                                     */
/*      SPRUCC (0x21fe91dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_0_N1_CHABC_SAD1_REG 0x031201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_0_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_1_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91e0)                                                      */
/*      SPRB0 (0x21fe91e0)                                                      */
/*      SPRHBM (0x21fe91e0)                                                     */
/*      SPRC0 (0x21fe91e0)                                                      */
/*      SPRMCC (0x21fe91e0)                                                     */
/*      SPRUCC (0x21fe91e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_1_N0_CHABC_SAD1_REG 0x031201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_1_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_1_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91e4)                                                      */
/*      SPRB0 (0x21fe91e4)                                                      */
/*      SPRHBM (0x21fe91e4)                                                     */
/*      SPRC0 (0x21fe91e4)                                                      */
/*      SPRMCC (0x21fe91e4)                                                     */
/*      SPRUCC (0x21fe91e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_1_N1_CHABC_SAD1_REG 0x031201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_1_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_2_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91e8)                                                      */
/*      SPRB0 (0x21fe91e8)                                                      */
/*      SPRHBM (0x21fe91e8)                                                     */
/*      SPRC0 (0x21fe91e8)                                                      */
/*      SPRMCC (0x21fe91e8)                                                     */
/*      SPRUCC (0x21fe91e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_2_N0_CHABC_SAD1_REG 0x031201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_2_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_2_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91ec)                                                      */
/*      SPRB0 (0x21fe91ec)                                                      */
/*      SPRHBM (0x21fe91ec)                                                     */
/*      SPRC0 (0x21fe91ec)                                                      */
/*      SPRMCC (0x21fe91ec)                                                     */
/*      SPRUCC (0x21fe91ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_2_N1_CHABC_SAD1_REG 0x031201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_2_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_3_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91f0)                                                      */
/*      SPRB0 (0x21fe91f0)                                                      */
/*      SPRHBM (0x21fe91f0)                                                     */
/*      SPRC0 (0x21fe91f0)                                                      */
/*      SPRMCC (0x21fe91f0)                                                     */
/*      SPRUCC (0x21fe91f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_3_N0_CHABC_SAD1_REG 0x031201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_3_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_3_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91f4)                                                      */
/*      SPRB0 (0x21fe91f4)                                                      */
/*      SPRHBM (0x21fe91f4)                                                     */
/*      SPRC0 (0x21fe91f4)                                                      */
/*      SPRMCC (0x21fe91f4)                                                     */
/*      SPRUCC (0x21fe91f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_3_N1_CHABC_SAD1_REG 0x031201F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_3_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_4_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91f8)                                                      */
/*      SPRB0 (0x21fe91f8)                                                      */
/*      SPRHBM (0x21fe91f8)                                                     */
/*      SPRC0 (0x21fe91f8)                                                      */
/*      SPRMCC (0x21fe91f8)                                                     */
/*      SPRUCC (0x21fe91f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_4_N0_CHABC_SAD1_REG 0x031201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_4_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_4_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe91fc)                                                      */
/*      SPRB0 (0x21fe91fc)                                                      */
/*      SPRHBM (0x21fe91fc)                                                     */
/*      SPRC0 (0x21fe91fc)                                                      */
/*      SPRMCC (0x21fe91fc)                                                     */
/*      SPRUCC (0x21fe91fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_4_N1_CHABC_SAD1_REG 0x031201FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_4_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_5_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9200)                                                      */
/*      SPRB0 (0x21fe9200)                                                      */
/*      SPRHBM (0x21fe9200)                                                     */
/*      SPRC0 (0x21fe9200)                                                      */
/*      SPRMCC (0x21fe9200)                                                     */
/*      SPRUCC (0x21fe9200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_5_N0_CHABC_SAD1_REG 0x03120200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_5_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_5_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9204)                                                      */
/*      SPRB0 (0x21fe9204)                                                      */
/*      SPRHBM (0x21fe9204)                                                     */
/*      SPRC0 (0x21fe9204)                                                      */
/*      SPRMCC (0x21fe9204)                                                     */
/*      SPRUCC (0x21fe9204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_5_N1_CHABC_SAD1_REG 0x03120204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_5_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_6_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9208)                                                      */
/*      SPRB0 (0x21fe9208)                                                      */
/*      SPRHBM (0x21fe9208)                                                     */
/*      SPRC0 (0x21fe9208)                                                      */
/*      SPRMCC (0x21fe9208)                                                     */
/*      SPRUCC (0x21fe9208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_6_N0_CHABC_SAD1_REG 0x03120208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_6_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_6_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe920c)                                                      */
/*      SPRB0 (0x21fe920c)                                                      */
/*      SPRHBM (0x21fe920c)                                                     */
/*      SPRC0 (0x21fe920c)                                                      */
/*      SPRMCC (0x21fe920c)                                                     */
/*      SPRUCC (0x21fe920c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_6_N1_CHABC_SAD1_REG 0x0312020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_6_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_7_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9210)                                                      */
/*      SPRB0 (0x21fe9210)                                                      */
/*      SPRHBM (0x21fe9210)                                                     */
/*      SPRC0 (0x21fe9210)                                                      */
/*      SPRMCC (0x21fe9210)                                                     */
/*      SPRUCC (0x21fe9210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_7_N0_CHABC_SAD1_REG 0x03120210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_7_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_7_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9214)                                                      */
/*      SPRB0 (0x21fe9214)                                                      */
/*      SPRHBM (0x21fe9214)                                                     */
/*      SPRC0 (0x21fe9214)                                                      */
/*      SPRMCC (0x21fe9214)                                                     */
/*      SPRUCC (0x21fe9214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_7_N1_CHABC_SAD1_REG 0x03120214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_7_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_8_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9218)                                                      */
/*      SPRB0 (0x21fe9218)                                                      */
/*      SPRHBM (0x21fe9218)                                                     */
/*      SPRC0 (0x21fe9218)                                                      */
/*      SPRMCC (0x21fe9218)                                                     */
/*      SPRUCC (0x21fe9218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_8_N0_CHABC_SAD1_REG 0x03120218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_8_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_8_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe921c)                                                      */
/*      SPRB0 (0x21fe921c)                                                      */
/*      SPRHBM (0x21fe921c)                                                     */
/*      SPRC0 (0x21fe921c)                                                      */
/*      SPRMCC (0x21fe921c)                                                     */
/*      SPRUCC (0x21fe921c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_8_N1_CHABC_SAD1_REG 0x0312021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_8_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_9_N0_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9220)                                                      */
/*      SPRB0 (0x21fe9220)                                                      */
/*      SPRHBM (0x21fe9220)                                                     */
/*      SPRC0 (0x21fe9220)                                                      */
/*      SPRMCC (0x21fe9220)                                                     */
/*      SPRUCC (0x21fe9220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_9_N0_CHABC_SAD1_REG 0x03120220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_9_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_9_N1_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9224)                                                      */
/*      SPRB0 (0x21fe9224)                                                      */
/*      SPRHBM (0x21fe9224)                                                     */
/*      SPRC0 (0x21fe9224)                                                      */
/*      SPRMCC (0x21fe9224)                                                     */
/*      SPRUCC (0x21fe9224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_9_N1_CHABC_SAD1_REG 0x03120224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_9_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_10_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9228)                                                      */
/*      SPRB0 (0x21fe9228)                                                      */
/*      SPRHBM (0x21fe9228)                                                     */
/*      SPRC0 (0x21fe9228)                                                      */
/*      SPRMCC (0x21fe9228)                                                     */
/*      SPRUCC (0x21fe9228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_10_N0_CHABC_SAD1_REG 0x03120228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_10_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_10_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe922c)                                                      */
/*      SPRB0 (0x21fe922c)                                                      */
/*      SPRHBM (0x21fe922c)                                                     */
/*      SPRC0 (0x21fe922c)                                                      */
/*      SPRMCC (0x21fe922c)                                                     */
/*      SPRUCC (0x21fe922c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_10_N1_CHABC_SAD1_REG 0x0312022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_10_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_11_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9230)                                                      */
/*      SPRB0 (0x21fe9230)                                                      */
/*      SPRHBM (0x21fe9230)                                                     */
/*      SPRC0 (0x21fe9230)                                                      */
/*      SPRMCC (0x21fe9230)                                                     */
/*      SPRUCC (0x21fe9230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_11_N0_CHABC_SAD1_REG 0x03120230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_11_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_11_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9234)                                                      */
/*      SPRB0 (0x21fe9234)                                                      */
/*      SPRHBM (0x21fe9234)                                                     */
/*      SPRC0 (0x21fe9234)                                                      */
/*      SPRMCC (0x21fe9234)                                                     */
/*      SPRUCC (0x21fe9234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_11_N1_CHABC_SAD1_REG 0x03120234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_11_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_12_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9238)                                                      */
/*      SPRB0 (0x21fe9238)                                                      */
/*      SPRHBM (0x21fe9238)                                                     */
/*      SPRC0 (0x21fe9238)                                                      */
/*      SPRMCC (0x21fe9238)                                                     */
/*      SPRUCC (0x21fe9238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_12_N0_CHABC_SAD1_REG 0x03120238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_12_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_12_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe923c)                                                      */
/*      SPRB0 (0x21fe923c)                                                      */
/*      SPRHBM (0x21fe923c)                                                     */
/*      SPRC0 (0x21fe923c)                                                      */
/*      SPRMCC (0x21fe923c)                                                     */
/*      SPRUCC (0x21fe923c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_12_N1_CHABC_SAD1_REG 0x0312023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_12_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_13_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9240)                                                      */
/*      SPRB0 (0x21fe9240)                                                      */
/*      SPRHBM (0x21fe9240)                                                     */
/*      SPRC0 (0x21fe9240)                                                      */
/*      SPRMCC (0x21fe9240)                                                     */
/*      SPRUCC (0x21fe9240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_13_N0_CHABC_SAD1_REG 0x03120240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_13_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_13_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9244)                                                      */
/*      SPRB0 (0x21fe9244)                                                      */
/*      SPRHBM (0x21fe9244)                                                     */
/*      SPRC0 (0x21fe9244)                                                      */
/*      SPRMCC (0x21fe9244)                                                     */
/*      SPRUCC (0x21fe9244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_13_N1_CHABC_SAD1_REG 0x03120244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_13_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_14_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9248)                                                      */
/*      SPRB0 (0x21fe9248)                                                      */
/*      SPRHBM (0x21fe9248)                                                     */
/*      SPRC0 (0x21fe9248)                                                      */
/*      SPRMCC (0x21fe9248)                                                     */
/*      SPRUCC (0x21fe9248)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_14_N0_CHABC_SAD1_REG 0x03120248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_14_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_14_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe924c)                                                      */
/*      SPRB0 (0x21fe924c)                                                      */
/*      SPRHBM (0x21fe924c)                                                     */
/*      SPRC0 (0x21fe924c)                                                      */
/*      SPRMCC (0x21fe924c)                                                     */
/*      SPRUCC (0x21fe924c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_14_N1_CHABC_SAD1_REG 0x0312024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_14_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_15_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9250)                                                      */
/*      SPRB0 (0x21fe9250)                                                      */
/*      SPRHBM (0x21fe9250)                                                     */
/*      SPRC0 (0x21fe9250)                                                      */
/*      SPRMCC (0x21fe9250)                                                     */
/*      SPRUCC (0x21fe9250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_15_N0_CHABC_SAD1_REG 0x03120250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_15_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_15_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9254)                                                      */
/*      SPRB0 (0x21fe9254)                                                      */
/*      SPRHBM (0x21fe9254)                                                     */
/*      SPRC0 (0x21fe9254)                                                      */
/*      SPRMCC (0x21fe9254)                                                     */
/*      SPRUCC (0x21fe9254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_15_N1_CHABC_SAD1_REG 0x03120254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_15_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_16_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9258)                                                      */
/*      SPRB0 (0x21fe9258)                                                      */
/*      SPRHBM (0x21fe9258)                                                     */
/*      SPRC0 (0x21fe9258)                                                      */
/*      SPRMCC (0x21fe9258)                                                     */
/*      SPRUCC (0x21fe9258)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_16_N0_CHABC_SAD1_REG 0x03120258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_16_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_16_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe925c)                                                      */
/*      SPRB0 (0x21fe925c)                                                      */
/*      SPRHBM (0x21fe925c)                                                     */
/*      SPRC0 (0x21fe925c)                                                      */
/*      SPRMCC (0x21fe925c)                                                     */
/*      SPRUCC (0x21fe925c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_16_N1_CHABC_SAD1_REG 0x0312025C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_16_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_17_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9260)                                                      */
/*      SPRB0 (0x21fe9260)                                                      */
/*      SPRHBM (0x21fe9260)                                                     */
/*      SPRC0 (0x21fe9260)                                                      */
/*      SPRMCC (0x21fe9260)                                                     */
/*      SPRUCC (0x21fe9260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_17_N0_CHABC_SAD1_REG 0x03120260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_17_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_17_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9264)                                                      */
/*      SPRB0 (0x21fe9264)                                                      */
/*      SPRHBM (0x21fe9264)                                                     */
/*      SPRC0 (0x21fe9264)                                                      */
/*      SPRMCC (0x21fe9264)                                                     */
/*      SPRUCC (0x21fe9264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_17_N1_CHABC_SAD1_REG 0x03120264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_17_N1_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_18_N0_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe9268)                                                      */
/*      SPRB0 (0x21fe9268)                                                      */
/*      SPRHBM (0x21fe9268)                                                     */
/*      SPRC0 (0x21fe9268)                                                      */
/*      SPRMCC (0x21fe9268)                                                     */
/*      SPRUCC (0x21fe9268)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_18_N0_CHABC_SAD1_REG 0x03120268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable this mmioh decoder. */
    UINT32 baseaddress : 22;

                            /* Bits[22:1], Access Type=RW, default=0x00000000*/

                            /*
                               Base address for this decoder - compared against
                               PA[51:30]
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_18_N0_CHABC_SAD1_STRUCT;

/* MMIOH_RULE_CFG_18_N1_CHABC_SAD1_REG supported on:                            */
/*      SPRA0 (0x21fe926c)                                                      */
/*      SPRB0 (0x21fe926c)                                                      */
/*      SPRHBM (0x21fe926c)                                                     */
/*      SPRC0 (0x21fe926c)                                                      */
/*      SPRMCC (0x21fe926c)                                                     */
/*      SPRUCC (0x21fe926c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Base-Limit Style decoder for MMIOH region. Contains rule enable, base addr, limit addr, and target node id. 
Local MS2IOSF count: 6
Remote Socket count: 7
Total Decoders: 13
*/


#define MMIOH_RULE_CFG_18_N1_CHABC_SAD1_REG 0x0312026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limitaddress : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit address for this decoder - comapred
                               against PA[51:30]
                            */
    UINT32 target : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               NodeID of the MMIOH target b1xxxx - target is a
                               local IIO stack with local stack id xxxx b00xxx
                               - target is remote socket with nodeid xxx
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIOH_RULE_CFG_18_N1_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_ADDR_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe9298)                                                      */
/*      SPRB0 (0x21fe9298)                                                      */
/*      SPRHBM (0x21fe9298)                                                     */
/*      SPRC0 (0x21fe9298)                                                      */
/*      SPRMCC (0x21fe9298)                                                     */
/*      SPRUCC (0x21fe9298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Address Cfg register - contains base, limit.
*/


#define BLOCK_DECODER_ADDR_N0_CHABC_SAD1_REG 0x03120298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 block_base : 26;

                            /* Bits[25:0], Access Type=RW, default=0x00000000*/

                            /*
                               Base address[51:26] of block/mailbox decoder
                               region.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BLOCK_DECODER_ADDR_N0_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_ADDR_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe929c)                                                      */
/*      SPRB0 (0x21fe929c)                                                      */
/*      SPRHBM (0x21fe929c)                                                     */
/*      SPRC0 (0x21fe929c)                                                      */
/*      SPRMCC (0x21fe929c)                                                     */
/*      SPRUCC (0x21fe929c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Address Cfg register - contains base, limit.
*/


#define BLOCK_DECODER_ADDR_N1_CHABC_SAD1_REG 0x0312029C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 block_limit : 26;

                            /* Bits[25:0], Access Type=RW, default=0x00000000*/

                            /*
                               Limit Address [51:26] of block/mailbox decoder
                               region.
                            */
    UINT32 block_rule_en : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* Enable for the Block Decoder Rule - turns it on. */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BLOCK_DECODER_ADDR_N1_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_0_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92a0)                                                      */
/*      SPRB0 (0x11fe92a0)                                                      */
/*      SPRHBM (0x11fe92a0)                                                     */
/*      SPRC0 (0x11fe92a0)                                                      */
/*      SPRMCC (0x11fe92a0)                                                     */
/*      SPRUCC (0x11fe92a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_0_CHABC_SAD1_REG 0x031102A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_0_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_1_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92a2)                                                      */
/*      SPRB0 (0x11fe92a2)                                                      */
/*      SPRHBM (0x11fe92a2)                                                     */
/*      SPRC0 (0x11fe92a2)                                                      */
/*      SPRMCC (0x11fe92a2)                                                     */
/*      SPRUCC (0x11fe92a2)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_1_CHABC_SAD1_REG 0x031102A2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_1_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_2_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92a4)                                                      */
/*      SPRB0 (0x11fe92a4)                                                      */
/*      SPRHBM (0x11fe92a4)                                                     */
/*      SPRC0 (0x11fe92a4)                                                      */
/*      SPRMCC (0x11fe92a4)                                                     */
/*      SPRUCC (0x11fe92a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_2_CHABC_SAD1_REG 0x031102A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_2_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_3_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92a6)                                                      */
/*      SPRB0 (0x11fe92a6)                                                      */
/*      SPRHBM (0x11fe92a6)                                                     */
/*      SPRC0 (0x11fe92a6)                                                      */
/*      SPRMCC (0x11fe92a6)                                                     */
/*      SPRUCC (0x11fe92a6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_3_CHABC_SAD1_REG 0x031102A6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_3_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_4_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92a8)                                                      */
/*      SPRB0 (0x11fe92a8)                                                      */
/*      SPRHBM (0x11fe92a8)                                                     */
/*      SPRC0 (0x11fe92a8)                                                      */
/*      SPRMCC (0x11fe92a8)                                                     */
/*      SPRUCC (0x11fe92a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_4_CHABC_SAD1_REG 0x031102A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_4_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_5_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92aa)                                                      */
/*      SPRB0 (0x11fe92aa)                                                      */
/*      SPRHBM (0x11fe92aa)                                                     */
/*      SPRC0 (0x11fe92aa)                                                      */
/*      SPRMCC (0x11fe92aa)                                                     */
/*      SPRUCC (0x11fe92aa)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_5_CHABC_SAD1_REG 0x031102AA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_5_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_6_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92ac)                                                      */
/*      SPRB0 (0x11fe92ac)                                                      */
/*      SPRHBM (0x11fe92ac)                                                     */
/*      SPRC0 (0x11fe92ac)                                                      */
/*      SPRMCC (0x11fe92ac)                                                     */
/*      SPRUCC (0x11fe92ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_6_CHABC_SAD1_REG 0x031102AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_6_CHABC_SAD1_STRUCT;

/* BLOCK_DECODER_EN_CFG_7_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x11fe92ae)                                                      */
/*      SPRB0 (0x11fe92ae)                                                      */
/*      SPRHBM (0x11fe92ae)                                                     */
/*      SPRC0 (0x11fe92ae)                                                      */
/*      SPRMCC (0x11fe92ae)                                                     */
/*      SPRUCC (0x11fe92ae)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Block Mode Decoder Enable Cfg register - contains enable vector for all channels per socket.
PA[18:16] is the socket index.
PA[13:12] is MC target
PA[15:14] is Channel target
*/


#define BLOCK_DECODER_EN_CFG_7_CHABC_SAD1_REG 0x031102AE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 block_mc_ch_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               16 bit vector indicating which of the 16
                               possible channels (4 CH x 4 MC) are enabled for
                               block mode. PA[15:12] is the index into the
                               vector - PA[13:12] selects the MC and PA[15:14]
                               is the channel. This results in the following
                               encoding: Bit[0] MC0,CH0 Bit[1] MC1,CH0 Bit[2]
                               MC2,CH0 Bit[3] MC3,CH0 Bit[4] MC0,CH1 Bit[5]
                               MC1,CH1 Bit[6] MC2,CH1 etc. This vector is
                               replicated once per socket. If SAD decodes to a
                               channel that is not enabled, SAD will abort in
                               the requesting socket.
                            */

  } Bits;
  UINT16 Data;

} BLOCK_DECODER_EN_CFG_7_CHABC_SAD1_STRUCT;

/* HA_THRESHOLDS1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9308)                                                      */
/*      SPRB0 (0x21fe9308)                                                      */
/*      SPRHBM (0x21fe9308)                                                     */
/*      SPRC0 (0x21fe9308)                                                      */
/*      SPRMCC (0x21fe9308)                                                     */
/*      SPRUCC (0x21fe9308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA register containing HA thresholds
*/


#define HA_THRESHOLDS1_CHABC_SAD1_REG 0x03120308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 resp_func_bl_rsp_sel : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block BL responses
                               in HAPIPE when Clock modulation is enabled.
                            */
    UINT32 resp_func_bl_rsp_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block BL responses
                               in HAPIPE when Clock modulation is enabled.
                            */
    UINT32 resp_func_conf_rsp_sel : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block Conflicts
                               responses in HAPIPE.
                            */
    UINT32 resp_func_conf_rsp_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block Conflicts
                               responses in HAPIPE.
                            */
    UINT32 ha_bl_rsp_threshold : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000000*/

                            /*
                               This describes the threshold for Bl Rsp
                               occupancy in the BL Fifo in the HAPIPE. If the
                               number of valid BL responses in the HABLFIFO >
                               HA_BL_Rsp_Threshold, then we will determine CHA
                               to be active and take appropriate steps to
                               determine how to modulate clocks based on the
                               CHA activity.
                            */
    UINT32 resp_func_enclkmod_rsp_en : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block Conflicts
                               responses in HAPIPE.
                            */
    UINT32 resp_func_enclkmod_rsp_sel : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Response Function select for block Conflicts
                               responses in HAPIPE.
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_THRESHOLDS1_CHABC_SAD1_STRUCT;

/* HA_THRESHOLDS_CHABC_SAD1_REG supported on:                                   */
/*      SPRA0 (0x21fe930c)                                                      */
/*      SPRB0 (0x21fe930c)                                                      */
/*      SPRHBM (0x21fe930c)                                                     */
/*      SPRC0 (0x21fe930c)                                                      */
/*      SPRMCC (0x21fe930c)                                                     */
/*      SPRUCC (0x21fe930c)                                                     */
/* Register default value on SPRA0: 0xC800037F                                  */
/* Register default value on SPRB0: 0xC800037F                                  */
/* Register default value on SPRHBM: 0xC800037F                                 */
/* Register default value on SPRC0: 0xC800037F                                  */
/* Register default value on SPRMCC: 0xC800037F                                 */
/* Register default value on SPRUCC: 0xC800037F                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA register containing HA thresholds
*/


#define HA_THRESHOLDS_CHABC_SAD1_REG 0x0312030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak_fifo_threshold : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000007*/

                            /*
                               Defines the number of credits to release back to
                               CMS for AK bouncing. Since this can be a value
                               of 0-7, 1 will be added to this value to make
                               the possible range of credits 1-8. The AK fifo
                               in HA is 8 entries deep.
                            */
    UINT32 mem_rd_credits : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000003*/

                            /* Mem Rd Egress Staging buffer (ESB) Credits */
    UINT32 mem_wr_credits : 2;

                            /* Bits[6:5], Access Type=RW/P, default=0x00000003*/

                            /* Mem Wr Egress Staging buffer (ESB) Credits */
    UINT32 snoop_credits : 2;

                            /* Bits[8:7], Access Type=RW/P, default=0x00000002*/

                            /* Snoop Egress Staging buffer (ESB) Credits */
    UINT32 fwd_credits : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000001*/

                            /* Forward Egress Staging buffer (ESB) Credits */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hacreditchange : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* Toggle this bit to load credit values */
    UINT32 resp_func_bypass_sel : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for Bypass Disable */
    UINT32 resp_func_bypass_en : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for Bypass Disable */
    UINT32 resp_func_retry_sel : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for Retry Disable */
    UINT32 resp_func_retry_en : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for Retry Select */
    UINT32 resp_func_ak_sel : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for Ak Disable */
    UINT32 resp_func_ak_en : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for Ak Select */
    UINT32 resp_func_tor_sel : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for Tor Disable */
    UINT32 resp_func_tor_en : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for Tor Select */
    UINT32 resp_func_osb_sel : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for OSB Disable */
    UINT32 resp_func_osb_en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for OSB Select */
    UINT32 disegressisocres : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Disable Egress Isoc credit reservation */
    UINT32 ak_credits : 3;

                            /* Bits[27:25], Access Type=RW/P, default=0x00000004*/

                            /* AK Egress Staging buffer (ESB) Credits */
    UINT32 resp_func_tor_ddrt_sel : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Select for Tor DDRT Disable */
    UINT32 resp_func_tor_ddrt_en : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Response Function Enable for Tor DDRT Select */
    UINT32 bl_wb_credits : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000003*/

                            /* IAL BL WB Egress Staging buffer (ESB) Credits */

  } Bits;
  UINT32 Data;

} HA_THRESHOLDS_CHABC_SAD1_STRUCT;

/* HA_COH_CHABC_SAD1_REG supported on:                                          */
/*      SPRA0 (0x21fe9310)                                                      */
/*      SPRB0 (0x21fe9310)                                                      */
/*      SPRHBM (0x21fe9310)                                                     */
/*      SPRC0 (0x21fe9310)                                                      */
/*      SPRMCC (0x21fe9310)                                                     */
/*      SPRUCC (0x21fe9310)                                                     */
/* Register default value on SPRA0: 0x06040301                                  */
/* Register default value on SPRB0: 0x06040301                                  */
/* Register default value on SPRHBM: 0x06040301                                 */
/* Register default value on SPRC0: 0x06040301                                  */
/* Register default value on SPRMCC: 0x06040301                                 */
/* Register default value on SPRUCC: 0x06040301                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA Coherency Config Register
*/


#define HA_COH_CHABC_SAD1_REG 0x03120310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 active_node_mask : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000001*/

                            /*
                               Active Nodes In System. One bit per Node ID of
                               caching agents in the system. It is used to
                               control which node snoops are spawned to and
                               responses are expected from. Default value is
                               NodeID_0 and NodeID_1 active.
                            */
    UINT32 dis_directory : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               1b1 - Disable Directory. Directory should be
                               disabled for single socket configurations.
                               Directory should be enabled for all multi-socket
                               configurations.
                            */
    UINT32 dis_spec_rd : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               1b1- Disable Early Read (Wait for snoop
                               Response) on HitME$ hit. This bit should be set
                               to 1 when HitME$ is enabled AND 2LM mode is
                               disabled.
                            */
    UINT32 rsvd : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frc_osb_loc_invitoe : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Force enablement of InvItoE OSB */
    UINT32 en_osb_loc_invitoe : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Threshold Based Enablement of InvItoE OSB */
    UINT32 osb_ad_vna_thr : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI AD VNA Threshold for local InvIToE
                               OSB. If Intel UPI VNA credits available is >=
                               OSB_AD_VNA_Thr in each of the Intel UPI ports,
                               then allow OSB
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 osb_snp_thr : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000001*/

                            /*
                               SNP Staging buffer Threshold for local InvIToE
                               OSB. If snoop staging buffers available is >=
                               OSB_SNP_Thr, then allow OSB
                            */
    UINT32 dis_ha_idle_bypass : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Disable bypassing HApipe under idle conditions
                               for latency savings
                            */
    UINT32 iodc_enable : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enabled IODC. IODC can be enabled only in
                               DIRECTORY enabled configurations.
                            */
    UINT32 disable_loc_meminvitox : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Defeature bit to disable 2LM local WCiLF
                               optimization</p>
                            */
    UINT32 disable_rmt_meminvitox : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Defeature bit to disable 2LM remote
                               WCiLF/PCIe write optimization</p>
                            */
    UINT32 kti0enabled : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Intel UPI0 is enabled. This is used by the OSB
                               logic to determine if Intel UPI0 is enabled.
                            */
    UINT32 kti1enabled : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Intel UPI1 is enabled. This is used by the OSB
                               logic to determine if Intel UPI1 is enabled.
                            */
    UINT32 kti2enabled : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI2 is enabled. This is used by the OSB
                               logic to determine if Intel UPI2 is enabled.
                            */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rdinvownacceptsm : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Allows Data_M in the local CA without WB to
                               memory for local RdInvOwn receiving Rsp*IWb.
                               Expect to be set to 1 in multi-socket systems.
                            */
    UINT32 rddatamigacceptsm : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Allows Data_M in the local CA without WB to
                               memory for local RdDataMig receiving Rsp*IWb.
                               Expect to be set to 1 in multi-socket systems.
                            */

  } Bits;
  UINT32 Data;

} HA_COH_CHABC_SAD1_STRUCT;

/* HA_COH_CFG_1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe9318)                                                      */
/*      SPRB0 (0x21fe9318)                                                      */
/*      SPRHBM (0x21fe9318)                                                     */
/*      SPRC0 (0x21fe9318)                                                      */
/*      SPRMCC (0x21fe9318)                                                     */
/*      SPRUCC (0x21fe9318)                                                     */
/* Register default value on SPRA0: 0x0000010E                                  */
/* Register default value on SPRB0: 0x0000010E                                  */
/* Register default value on SPRHBM: 0x0000010E                                 */
/* Register default value on SPRC0: 0x0000010E                                  */
/* Register default value on SPRMCC: 0x0000010E                                 */
/* Register default value on SPRUCC: 0x0000010E                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA Coherency Config Register
*/


#define HA_COH_CFG_1_CHABC_SAD1_REG 0x03120318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iodc_num_enabled_entries : 4;

                            /* Bits[3:0], Access Type=RW, default=0x0000000E*/

                            /* Number of enabled IODC entries (max=default=14). */
    UINT32 rsvd : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ha_bypass_mode : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000001*/

                            /*
                               HApipe idle Bypass mode. 01: Less conservative
                               HA idle bypass mode. Credits for target and the
                               MEM_RD credits are available.
                            */
    UINT32 ha_mini_bypass_dis : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               HA Mini Bypass mode. When a Read request gets
                               read out of the TORReqQ and there is not
                               anything ahead of it in the HApipe, the read
                               will be issued right away saving 2 cycles.
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 staleatosopten : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               A to S directory optimization. When RdData*
                               finds DIR=A and all snoop responses received are
                               RspI, then directory is moved to S and data is
                               returned in S-state. This optimization will not
                               be effective in xNC configuration where BuriedM
                               is possible.
                            */
    UINT32 buriedca : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Buried Caching Agent. This should be set in xNC
                               configurations where buriedM is possible. i.e. a
                               requestor can issue a request while holding the
                               line in modified state. Processor caching agent
                               will never have buriedM.
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 gateosbiodcalloc : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When OSB indicates that there arent enough snoop
                               credits dont allocate IODC entry.
                            */
    UINT32 iodc_alloc_osb_ad_vna_thr : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               UPI AD VNA threshold for IODC allocation OSB. If
                               UPI VNA credits available is >=
                               IODC_Alloc_OSB_AD_VNA_Thr in each of UPI port,
                               then allow IODC_Alloc_OSB.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 iodc_alloc_osb_snp_thr : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               SNP Staging buffer threshold for IODC allocation
                               OSB. If snoop staging buffers available is >=
                               IODC_Alloc_OSB_SNP_Thr in each of UPI port, then
                               allow IODC_Alloc_OSB.
                            */
    UINT32 iodc_alloc_osb_bl_vna_thr : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               UPI BL VNA threshold for IODC allocation OSB. If
                               UPI VNA credits available is >=
                               IODC_Alloc_OSB_BL_VNA_Thr in each of UPI port,
                               then allow IODC_Alloc_OSB.
                            */
    UINT32 osb_bl_vna_thr : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               UPI BL VNA threshold for local InvItoE OSB. If
                               UPI VNA credits available is >= OSB_BL_VNA_Thr
                               in each of UPI port, then allow OSB.
                            */
    UINT32 xnc_nonsnpwr_diri_enable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Set directory to DIR=I for NonSnpWr. This should
                               be enabled in xNC configs where we are
                               gauranteed that there is no conflicts between
                               NonSnpWr address ranges and coherent address
                               ranges. This will enable D2K optimization for
                               following NonSnpRds.
                            */
    UINT32 xnc_optmemrdxtos_disable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Defeature bit for xNC configs. When set, the
                               optimized flow for MemRdXtoS in xNC buriedM
                               configs implemented in the MC will be disabled.
                               The alternate flow which converts MemRdXtoS to
                               MemRdData will be used instead for xNC buriedM
                               configs.</p> <p><b>Non-xNC configs
                               (default):</b>{buriedca,
                               xnc_optmemrdxtos_disable, M2M_xNCBuriedM} = {0,
                               0, 0}</p> <p><b>xNC config POR:</b>{buriedca,
                               xnc_optmemrdxtos_disable, M2M_xNCBuriedM} = =
                               {1, 0, 1}</p> <p><b>xNC config defeature
                               mode:</b>{buriedca, xnc_optmemrdxtos_disable,
                               M2M_xNCBuriedM} = = {1, 1, 0}</p>
                            */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_1_CHABC_SAD1_STRUCT;

/* HA_COH_CFG_2_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe9320)                                                      */
/*      SPRB0 (0x21fe9320)                                                      */
/*      SPRHBM (0x21fe9320)                                                     */
/*      SPRC0 (0x21fe9320)                                                      */
/*      SPRMCC (0x21fe9320)                                                     */
/*      SPRUCC (0x21fe9320)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000028                                 */
/* Register default value on SPRC0: 0x00000028                                  */
/* Register default value on SPRMCC: 0x00000028                                 */
/* Register default value on SPRUCC: 0x00000028                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA Coherency&nbsp; Config Register 2
*/


#define HA_COH_CFG_2_CHABC_SAD1_REG 0x03120320

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in HA. To enable this feature in the TOR
                               the HA_COH_CFG_TOR.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.AllUpdateOptEn bit must also be
                               set.
                            */
    UINT32 usenontempopcodefromiosftoial : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 1, use Non-Temporal opcode (MemInvNT
                               instead of MemInv) from CHA to Intel Accelerator
                               Link (IAL) devices, when IOSF/PCIe is accessing
                               IAL accelerator-owned address with an ItoM. NT
                               behavior is optimal for streaming writes from
                               PCIe, as IAL accelerator, while in device bias,
                               will suppress normal tendency to re-acquire
                               ownership of the line before PCIe completes the
                               write to memory following the ItoM. When set to
                               0, NT opcode is not used, and an IAL accelerator
                               in device bias may retake a line before an
                               ItoM/write flow completes.
                            */
    UINT32 rsvd : 12;

                            /* Bits[15:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sparehacsrunnnh : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* <p>HA spare CSR</p> */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_2_CHABC_SAD1_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in HA. To enable this feature in the TOR
                               the HA_COH_CFG_TOR.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.AllUpdateOptEn bit must also be
                               set.
                            */
    UINT32 usenontempopcodefromiosftoial : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 1, use Non-Temporal opcode (MemInvNT
                               instead of MemInv) from CHA to Intel Accelerator
                               Link (IAL) devices, when IOSF/PCIe is accessing
                               IAL accelerator-owned address with an ItoM. NT
                               behavior is optimal for streaming writes from
                               PCIe, as IAL accelerator, while in device bias,
                               will suppress normal tendency to re-acquire
                               ownership of the line before PCIe completes the
                               write to memory following the ItoM. When set to
                               0, NT opcode is not used, and an IAL accelerator
                               in device bias may retake a line before an
                               ItoM/write flow completes.
                            */
    UINT32 disnmclean : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When this chicken bit is Set, CHA uses the value
                               of 0 for NMClean indication.
                            */
    UINT32 rsvd : 11;

                            /* Bits[15:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sparehacsrunnnh : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* <p>HA spare CSR</p> */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_2_CHABC_SAD1_SPRB0_STRUCT;
#endif /* (SPRB0_HOST) */

#if defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in HA. To enable this feature in the TOR
                               the HA_COH_CFG_TOR.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.AllUpdateOptEn bit must also be
                               set.
                            */
    UINT32 usenontempopcodefromiosftoial : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 1, use Non-Temporal opcode (MemInvNT
                               instead of MemInv) from CHA to Intel Accelerator
                               Link (IAL) devices, when IOSF/PCIe is accessing
                               IAL accelerator-owned address with an ItoM. NT
                               behavior is optimal for streaming writes from
                               PCIe, as IAL accelerator, while in device bias,
                               will suppress normal tendency to re-acquire
                               ownership of the line before PCIe completes the
                               write to memory following the ItoM. When set to
                               0, NT opcode is not used, and an IAL accelerator
                               in device bias may retake a line before an
                               ItoM/write flow completes.
                            */
    UINT32 disnmclean : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When this chicken bit is Set, CHA uses the value
                               of 0 for NMClean indication.
                            */
    UINT32 dis_spec_rd_rsf_req : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               RSF structure tracks remote socket caching of
                               HOM lines. If SRF shows exclusive ownership, we
                               disable the read to memory till we receive snoop
                               response for remote socket snoop in HA. If this
                               bit field is set to 1, we donot send mem read
                               with snoop.
                            */
    UINT32 rsvd : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sparehacsrunnnh : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* <p>HA spare CSR</p> */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_2_CHABC_SAD1_SPRHBM_STRUCT;
#endif /* (SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in HA. To enable this feature in the TOR
                               the HA_COH_CFG_TOR.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.AllUpdateOptEn bit must also be
                               set.
                            */
    UINT32 usenontempopcodefromiosftoial : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 1, use Non-Temporal opcode (MemInvNT
                               instead of MemInv) from CHA to Intel Accelerator
                               Link (IAL) devices, when IOSF/PCIe is accessing
                               IAL accelerator-owned address with an ItoM. NT
                               behavior is optimal for streaming writes from
                               PCIe, as IAL accelerator, while in device bias,
                               will suppress normal tendency to re-acquire
                               ownership of the line before PCIe completes the
                               write to memory following the ItoM. When set to
                               0, NT opcode is not used, and an IAL accelerator
                               in device bias may retake a line before an
                               ItoM/write flow completes.
                            */
    UINT32 disnmclean : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When this chicken bit is Set, CHA uses the value
                               of 0 for NMClean indication.
                            */
    UINT32 dis_spec_rd_rsf_req : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               RSF structure tracks remote socket caching of
                               HOM lines. If RSF shows exclusive ownership, we
                               disable the read to memory till we receive snoop
                               response for remote socket snoop in HA. If this
                               bit field is set to 1, we donot send mem read
                               with snoop.
                            */
    UINT32 forceniwr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Useful survivability hook. When set, forces
                               MemWrNI from HA.
                            */
    UINT32 disiodcincforreads : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               If we have a IODC Hit, we look up to check IODC
                               inclusive bit. For Read requests that hit IODC,
                               we should not really care about IODC Inclusive
                               bit. IODC Inclusive bit should only matter when
                               we send Write requests. If it read out
                               IODCNI(non inclusive) for a read request, this
                               Inclusive bit gets stored in HA and a MemWr that
                               might follow might send a MemWrNI instead of
                               MemWr. This is not optimal.
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sparehacsrunnnh : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* <p>HA spare CSR</p> */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_2_CHABC_SAD1_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in HA. To enable this feature in the TOR
                               the HA_COH_CFG_TOR.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               HA. To enable this feature in the TOR the
                               HA_COH_CFG_TOR.AllUpdateOptEn bit must also be
                               set.
                            */
    UINT32 usenontempopcodefromiosftoial : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 1, use Non-Temporal opcode (MemInvNT
                               instead of MemInv) from CHA to Intel Accelerator
                               Link (IAL) devices, when IOSF/PCIe is accessing
                               IAL accelerator-owned address with an ItoM. NT
                               behavior is optimal for streaming writes from
                               PCIe, as IAL accelerator, while in device bias,
                               will suppress normal tendency to re-acquire
                               ownership of the line before PCIe completes the
                               write to memory following the ItoM. When set to
                               0, NT opcode is not used, and an IAL accelerator
                               in device bias may retake a line before an
                               ItoM/write flow completes.
                            */
    UINT32 rsvd : 12;

                            /* Bits[15:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sparehacsrunnnh : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* <p>HA spare CSR</p> */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_2_CHABC_SAD1_STRUCT;

/* HA_OSB_CHABC_SAD1_REG supported on:                                          */
/*      SPRA0 (0x21fe9328)                                                      */
/*      SPRB0 (0x21fe9328)                                                      */
/*      SPRHBM (0x21fe9328)                                                     */
/*      SPRC0 (0x21fe9328)                                                      */
/*      SPRMCC (0x21fe9328)                                                     */
/*      SPRUCC (0x21fe9328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA OSB config
*/


#define HA_OSB_CHABC_SAD1_REG 0x03120328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 frc_osb_loc_rd : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Force enablement of local read OSB */
    UINT32 en_osb_loc_rd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Threshold based enablement of local read OSB */
    UINT32 en_osb_loc_rdcur : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Threshold based&nbsp;enablement of local RdCur
                               OSB
                            */
    UINT32 osb_ad_vna_locrd_thr : 5;

                            /* Bits[7:3], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI AD VNA Threshold for local read OSB.
                               If Intel UPI VNA credits available is >=
                               OSB_AD_VNA_Thr in each of Intel UPI port, then
                               allow OSB.
                            */
    UINT32 osb_snp_locrd_thr : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               SNP Staging buffer Threshold for local read OSB.
                               If snoop staging buffers available is >=
                               OSB_SNP_LocRd_Thr, then allow OSB.
                            */
    UINT32 osb_bl_vna_locrd_thr : 3;

                            /* Bits[13:11], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI BL VNA threshold for local read OSB.
                               If Intel UPI VNA credits available is >=
                               OSB_BL_VNA_LocRd_Thr in each of Intel UPI port,
                               then allow OSB.
                            */
    UINT32 frc_osb_rmt_rd : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Force enablement of remote read OSB */
    UINT32 en_osb_rmt_rd : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Threshold Based Enablement of remote read OSB */
    UINT32 osb_ad_vna_rmtrd_thr : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI AD VNA Threshold for remote read OSB.
                               If Intel UPI VNA credits available is >=
                               OSB_AD_VNA_RmtRd_Thr in each of Intel UPI port,
                               then allow OSB.
                            */
    UINT32 osb_snp_rmtrd_thr : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               SNP Staging buffer Threshold for remote read
                               OSB. If snoop staging buffers available is >=
                               OSB_SNP_RmtRd_Thr, then allow OSB.
                            */
    UINT32 osb_bl_vna_rmtrd_thr : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Intel UPI BL VNA threshold for remote read OSB.
                               If Intel UPI VNA credits available is >=
                               OSB_BL_VNA_RmtRd_Thr in each of Intel UPI port,
                               then allow OSB.
                            */
    UINT32 en_osb_loc_invxtoi : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Threshold Based Enablement of Local InvXtoI OSB.
                               Note: We can only have OSB for InvXtoI if we
                               enable OSB for InvItoE (local). That is,
                               en_osb_loc_invitoe needs to be set.
                            */
    UINT32 en_osb_rmt_invxtoi : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Threshold Based Enablement of Remote InvXtoI
                               OSB. We also need ha_osb_cfg_en_osb_rmt_rd to be
                               set for this.
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_OSB_CHABC_SAD1_STRUCT;

/* HA_OSB_POWERHEUR_CHABC_SAD1_REG supported on:                                */
/*      SPRA0 (0x21fe9330)                                                      */
/*      SPRB0 (0x21fe9330)                                                      */
/*      SPRHBM (0x21fe9330)                                                     */
/*      SPRC0 (0x21fe9330)                                                      */
/*      SPRMCC (0x21fe9330)                                                     */
/*      SPRUCC (0x21fe9330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA OSB PowerHeur config
*/


#define HA_OSB_POWERHEUR_CHABC_SAD1_REG 0x03120330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 osbpoweren : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* OSB Power Heuristics Enable */
    UINT32 osbtimermax : 5;

                            /* Bits[5:1], Access Type=RW, default=0x00000000*/

                            /* OSB Timer Maximum Value (within 32-bits) */
    UINT32 osbtimerfraction : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               OSB Time period fraction size within the OSB
                               timer window after which we should start looking
                               at the stats (local read count and local read w/
                               DIR!=I count) for determining NUMA_OSB_off
                            */
    UINT32 osbprogshift : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000000*/

                            /*
                               OSB Programmable Shift Value (Local read count
                               shift to determine NUMA mode. If
                               ((loca_read_count >> OSBProgShift)
                               >local_read_w/_DIR!=I_count)&nbsp; then
                               NUMA_OSB_off)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_OSB_POWERHEUR_CHABC_SAD1_STRUCT;

/* HA_XNC_NID_MASK_CHABC_SAD1_REG supported on:                                 */
/*      SPRA0 (0x21fe9338)                                                      */
/*      SPRB0 (0x21fe9338)                                                      */
/*      SPRHBM (0x21fe9338)                                                     */
/*      SPRC0 (0x21fe9338)                                                      */
/*      SPRMCC (0x21fe9338)                                                     */
/*      SPRUCC (0x21fe9338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA XNC NID mask config
*/


#define HA_XNC_NID_MASK_CHABC_SAD1_REG 0x03120338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_osb_enable_nid_mask : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               One bit per Node ID of caching agents in the
                               system. It is used to control which nodes will
                               be allowed to OSB for local read and remote
                               read/InvItoE. When the bit is set that NID will
                               be allowed to OSB for read/rmtInv.
                            */
    UINT32 atos_opt_enable_nid_mask : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               One bit per Node ID of caching agents in the
                               system. It is used to control which nodes will
                               be allowed to do AtoS Optimization. When the bit
                               is set that NID will be allowed to do AtoS
                               optimization. This NID mask will be effective
                               only in xNC configs with buriedM possibility
                               which is indicated by the HA_COH_CFG_1.buriedCA
                               bit.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_XNC_NID_MASK_CHABC_SAD1_STRUCT;

/* PAM_W_CP_N0_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x21fe93d0)                                                      */
/*      SPRB0 (0x21fe93d0)                                                      */
/*      SPRHBM (0x21fe93d0)                                                     */
/*      SPRC0 (0x21fe93d0)                                                      */
/*      SPRMCC (0x21fe93d0)                                                     */
/*      SPRUCC (0x21fe93d0)                                                     */
/* Register default value on SPRA0: 0x01000209                                  */
/* Register default value on SPRB0: 0x01000209                                  */
/* Register default value on SPRHBM: 0x01000209                                 */
/* Register default value on SPRC0: 0x01000209                                  */
/* Register default value on SPRMCC: 0x01000209                                 */
/* Register default value on SPRUCC: 0x01000209                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Control Policy
*/


#define PAM_W_CP_N0_CHABC_SAD1_REG 0x031203D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} PAM_W_CP_N0_CHABC_SAD1_STRUCT;

/* PAM_W_CP_N1_CHABC_SAD1_REG supported on:                                     */
/*      SPRA0 (0x21fe93d4)                                                      */
/*      SPRB0 (0x21fe93d4)                                                      */
/*      SPRHBM (0x21fe93d4)                                                     */
/*      SPRC0 (0x21fe93d4)                                                      */
/*      SPRMCC (0x21fe93d4)                                                     */
/*      SPRUCC (0x21fe93d4)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Control Policy
*/


#define PAM_W_CP_N1_CHABC_SAD1_REG 0x031203D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to PAM_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} PAM_W_CP_N1_CHABC_SAD1_STRUCT;

/* PAM_W_RAC_N0_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe93d8)                                                      */
/*      SPRB0 (0x21fe93d8)                                                      */
/*      SPRHBM (0x21fe93d8)                                                     */
/*      SPRC0 (0x21fe93d8)                                                      */
/*      SPRMCC (0x21fe93d8)                                                     */
/*      SPRUCC (0x21fe93d8)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Read Access Control
*/


#define PAM_W_RAC_N0_CHABC_SAD1_REG 0x031203D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} PAM_W_RAC_N0_CHABC_SAD1_STRUCT;

/* PAM_W_RAC_N1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe93dc)                                                      */
/*      SPRB0 (0x21fe93dc)                                                      */
/*      SPRHBM (0x21fe93dc)                                                     */
/*      SPRC0 (0x21fe93dc)                                                      */
/*      SPRMCC (0x21fe93dc)                                                     */
/*      SPRUCC (0x21fe93dc)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Read Access Control
*/


#define PAM_W_RAC_N1_CHABC_SAD1_REG 0x031203DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} PAM_W_RAC_N1_CHABC_SAD1_STRUCT;

/* PAM_W_WAC_N0_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe93e0)                                                      */
/*      SPRB0 (0x21fe93e0)                                                      */
/*      SPRHBM (0x21fe93e0)                                                     */
/*      SPRC0 (0x21fe93e0)                                                      */
/*      SPRMCC (0x21fe93e0)                                                     */
/*      SPRUCC (0x21fe93e0)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Write Access Control
*/


#define PAM_W_WAC_N0_CHABC_SAD1_REG 0x031203E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} PAM_W_WAC_N0_CHABC_SAD1_STRUCT;

/* PAM_W_WAC_N1_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe93e4)                                                      */
/*      SPRB0 (0x21fe93e4)                                                      */
/*      SPRHBM (0x21fe93e4)                                                     */
/*      SPRC0 (0x21fe93e4)                                                      */
/*      SPRMCC (0x21fe93e4)                                                     */
/*      SPRUCC (0x21fe93e4)                                                     */
/* Register default value on SPRA0: 0x00000C00                                  */
/* Register default value on SPRB0: 0x00000C00                                  */
/* Register default value on SPRHBM: 0x00000C00                                 */
/* Register default value on SPRC0: 0x00000C00                                  */
/* Register default value on SPRMCC: 0x00000C00                                 */
/* Register default value on SPRUCC: 0x00000C00                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: PAM_W                                            */
/* SPRB0 Security PolicyGroup: PAM_W                                            */
/* SPRHBM Security PolicyGroup: PAM_W                                           */
/* SPRC0 Security PolicyGroup: PAM_W                                            */
/* SPRMCC Security PolicyGroup: PAM_W                                           */
/* SPRUCC Security PolicyGroup: PAM_W                                           */
/* PAM_W Write Access Control
*/


#define PAM_W_WAC_N1_CHABC_SAD1_REG 0x031203E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in PAM_W security policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} PAM_W_WAC_N1_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_CP_0X3FF_N0_CHABC_SAD1_REG supported on:                     */
/*      SPRA0 (0x21fe93e8)                                                      */
/*      SPRB0 (0x21fe93e8)                                                      */
/*      SPRHBM (0x21fe93e8)                                                     */
/*      SPRC0 (0x21fe93e8)                                                      */
/*      SPRMCC (0x21fe93e8)                                                     */
/*      SPRUCC (0x21fe93e8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Control Policy
*/


#define LB_W_MCHECK_CHA_CP_0X3FF_N0_CHABC_SAD1_REG 0x031203E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_CP_0X3FF_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_CP_0X3FF_N1_CHABC_SAD1_REG supported on:                     */
/*      SPRA0 (0x21fe93ec)                                                      */
/*      SPRB0 (0x21fe93ec)                                                      */
/*      SPRHBM (0x21fe93ec)                                                     */
/*      SPRC0 (0x21fe93ec)                                                      */
/*      SPRMCC (0x21fe93ec)                                                     */
/*      SPRUCC (0x21fe93ec)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Control Policy
*/


#define LB_W_MCHECK_CHA_CP_0X3FF_N1_CHABC_SAD1_REG 0x031203EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_CP_0X3FF_N1_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_RAC_0X3FF_N0_CHABC_SAD1_REG supported on:                    */
/*      SPRA0 (0x21fe93f0)                                                      */
/*      SPRB0 (0x21fe93f0)                                                      */
/*      SPRHBM (0x21fe93f0)                                                     */
/*      SPRC0 (0x21fe93f0)                                                      */
/*      SPRMCC (0x21fe93f0)                                                     */
/*      SPRUCC (0x21fe93f0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Read Access Control
*/


#define LB_W_MCHECK_CHA_RAC_0X3FF_N0_CHABC_SAD1_REG 0x031203F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_RAC_0X3FF_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_RAC_0X3FF_N1_CHABC_SAD1_REG supported on:                    */
/*      SPRA0 (0x21fe93f4)                                                      */
/*      SPRB0 (0x21fe93f4)                                                      */
/*      SPRHBM (0x21fe93f4)                                                     */
/*      SPRC0 (0x21fe93f4)                                                      */
/*      SPRMCC (0x21fe93f4)                                                     */
/*      SPRUCC (0x21fe93f4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Read Access Control
*/


#define LB_W_MCHECK_CHA_RAC_0X3FF_N1_CHABC_SAD1_REG 0x031203F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_RAC_0X3FF_N1_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_WAC_0X3FF_N0_CHABC_SAD1_REG supported on:                    */
/*      SPRA0 (0x21fe93f8)                                                      */
/*      SPRB0 (0x21fe93f8)                                                      */
/*      SPRHBM (0x21fe93f8)                                                     */
/*      SPRC0 (0x21fe93f8)                                                      */
/*      SPRMCC (0x21fe93f8)                                                     */
/*      SPRUCC (0x21fe93f8)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Write Access Control
*/


#define LB_W_MCHECK_CHA_WAC_0X3FF_N0_CHABC_SAD1_REG 0x031203F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_WAC_0X3FF_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_WAC_0X3FF_N1_CHABC_SAD1_REG supported on:                    */
/*      SPRA0 (0x21fe93fc)                                                      */
/*      SPRB0 (0x21fe93fc)                                                      */
/*      SPRHBM (0x21fe93fc)                                                     */
/*      SPRC0 (0x21fe93fc)                                                      */
/*      SPRMCC (0x21fe93fc)                                                     */
/*      SPRUCC (0x21fe93fc)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff  */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_BCAST_1_0x000_0x3ff */
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Write Access Control
*/


#define LB_W_MCHECK_CHA_WAC_0X3FF_N1_CHABC_SAD1_REG 0x031203FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_WAC_0X3FF_N1_CHABC_SAD1_STRUCT;

/* QPI_XNC_MASK_CHABC_SAD1_REG supported on:                                    */
/*      SPRA0 (0x21fe9400)                                                      */
/*      SPRB0 (0x21fe9400)                                                      */
/*      SPRHBM (0x21fe9400)                                                     */
/*      SPRC0 (0x21fe9400)                                                      */
/*      SPRMCC (0x21fe9400)                                                     */
/*      SPRUCC (0x21fe9400)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains info about the XNCs in the system.
*/


#define QPI_XNC_MASK_CHABC_SAD1_REG 0x03120400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xncnodemask : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Denotes which nodes in a system are XNCs.
                               xNCNodeMask[node_id]=1 if the node is an XNC,
                               and 0 otherwise.
                            */
    UINT32 evctclnen : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Send EvctCln message when victimizing an E-state
                               line from the cache, unless the line is being
                               monitored
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QPI_XNC_MASK_CHABC_SAD1_STRUCT;

/* REMOTE_HIBWMEM_MASK_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe9404)                                                      */
/*      SPRB0 (0x21fe9404)                                                      */
/*      SPRHBM (0x21fe9404)                                                     */
/*      SPRC0 (0x21fe9404)                                                      */
/*      SPRMCC (0x21fe9404)                                                     */
/*      SPRUCC (0x21fe9404)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains info about the Remote High Bandwidth memory targets in the system.
*/


#define REMOTE_HIBWMEM_MASK_CHABC_SAD1_REG 0x03120404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nodemask : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Denotes which remote nodes in the system have
                               High Bandwidth memory which requires EvictClean
                               messages to properly maintain directory state.
                               NodeMask[node_id]=1 if the node is of this type,
                               and 0 otherwise. When the corresponding bit is
                               set to one, DRAM rules mapping to that remote
                               node that have their DRAM attribute field set to
                               High Bandwidth Coherent DRAM will have
                               EvictCleans sent to them when addresses in those
                               ranges are evicted from the local socket's
                               LLC+SF.&nbsp;
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REMOTE_HIBWMEM_MASK_CHABC_SAD1_STRUCT;

/* SHADOW_UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG supported on:                */
/*      SPRA0 (0x21fe9408)                                                      */
/*      SPRB0 (0x21fe9408)                                                      */
/*      SPRHBM (0x21fe9408)                                                     */
/*      SPRC0 (0x21fe9408)                                                      */
/*      SPRMCC (0x21fe9408)                                                     */
/*      SPRUCC (0x21fe9408)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define SHADOW_UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG 0x03120408

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

} SHADOW_UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_STRUCT;

/* SHADOW_UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG supported on:                */
/*      SPRA0 (0x21fe940c)                                                      */
/*      SPRB0 (0x21fe940c)                                                      */
/*      SPRHBM (0x21fe940c)                                                     */
/*      SPRC0 (0x21fe940c)                                                      */
/*      SPRMCC (0x21fe940c)                                                     */
/*      SPRUCC (0x21fe940c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define SHADOW_UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG 0x0312040C

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

} SHADOW_UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_STRUCT;

/* UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG supported on:                       */
/*      SPRA0 (0x21fe9410)                                                      */
/*      SPRB0 (0x21fe9410)                                                      */
/*      SPRHBM (0x21fe9410)                                                     */
/*      SPRC0 (0x21fe9410)                                                      */
/*      SPRMCC (0x21fe9410)                                                     */
/*      SPRUCC (0x21fe9410)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG 0x03120410

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

} UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_STRUCT;

/* UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG supported on:                       */
/*      SPRA0 (0x21fe9414)                                                      */
/*      SPRB0 (0x21fe9414)                                                      */
/*      SPRHBM (0x21fe9414)                                                     */
/*      SPRC0 (0x21fe9414)                                                      */
/*      SPRMCC (0x21fe9414)                                                     */
/*      SPRUCC (0x21fe9414)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
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


#define UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG 0x03120414

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

} UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_STRUCT;

/* HA_COH_CFG_TOR_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9418)                                                      */
/*      SPRB0 (0x21fe9418)                                                      */
/*      SPRHBM (0x21fe9418)                                                     */
/*      SPRC0 (0x21fe9418)                                                      */
/*      SPRMCC (0x21fe9418)                                                     */
/*      SPRUCC (0x21fe9418)                                                     */
/* Register default value on SPRA0: 0x19380030                                  */
/* Register default value on SPRB0: 0x19380030                                  */
/* Register default value on SPRHBM: 0x19380030                                 */
/* Register default value on SPRC0: 0x19380030                                  */
/* Register default value on SPRMCC: 0x19380030                                 */
/* Register default value on SPRUCC: 0x19380030                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* HA Coherency Config Register
*/


#define HA_COH_CFG_TOR_CHABC_SAD1_REG 0x03120418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hitme_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               1b1 - Enable HitME cache overloading. HitME$
                               should be enabled only when directory is
                               enabled.
                            */
    UINT32 hitme_rfo_dirs : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               1b1 - Enable HitME DIR=S RFO optimization. This
                               optimization allows snoops to be broadcast in
                               parallel with memory lookup for RFO HitS in the
                               local CA.
                            */
    UINT32 hitme_shar : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* HitME cache allows caching of SHARed lines */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hitme_ways : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000003*/

                            /*
                               Identifies which ways of the SF can be used for
                               HitME cache entries
                            */
    UINT32 rsvd_12 : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hacreditcnt : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               Number of credits in the TOR to the HApipe for
                               the TORReqQ
                            */
    UINT32 haegrcreditthreshold : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000003*/

                            /*
                               Threshold for the AD/BL egress credits for the
                               HApipe. When the number of credits available is
                               higher than this number, the HA is allowed to
                               acquire a credit. This can never be set to less
                               than 3.
                            */
    UINT32 hitme_alloconlocalme : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Convert SF entry to HitME$ entry when a remote
                               request hits the local CA in M/E state and
                               results in Data_M/E transfer.
                            */
    UINT32 haddrtcreditcnt : 3;

                            /* Bits[27:25], Access Type=RW, default=0x00000004*/

                            /*
                               Number of credits in the TOR to the HApipe for
                               the DDRT Wr TORReqQ
                            */
    UINT32 disablehabypass2lminv : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Disable the HA idle bypass for a request that
                               may send InvItoX
                            */
    UINT32 appdirectdirupdateopten : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for
                               AppDirect lines. Optimization will write Dir=A
                               for remote WbIData(Ptl) and prevent updates from
                               A->I for local requests. This bit controls the
                               feature in TOR. To enable this feature in the HA
                               the HA_COH_CFG_2.AppDirectUpdateOptEn bit must
                               also be set.
                            */
    UINT32 twolmdirupdateopten : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for 2LM
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               TOR. To enable this feature in the HA the
                               HA_COH_CFG_2.TwoLMUpdateOptEn bit must also be
                               set.
                            */
    UINT32 alldirupdateopten : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Enable directory update optimization for all
                               lines. Optimization will write Dir=A for remote
                               WbIData(Ptl) and prevent updates from A->I for
                               local requests. This bit controls the feature in
                               TOR. To enable this feature in the HA the
                               HA_COH_CFG_2.AllUpdateOptEn bit must also be
                               set.
                            */

  } Bits;
  UINT32 Data;

} HA_COH_CFG_TOR_CHABC_SAD1_STRUCT;

/* TOR_THRESHOLDS_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9420)                                                      */
/*      SPRB0 (0x21fe9420)                                                      */
/*      SPRHBM (0x21fe9420)                                                     */
/*      SPRC0 (0x21fe9420)                                                      */
/*      SPRMCC (0x21fe9420)                                                     */
/*      SPRUCC (0x21fe9420)                                                     */
/* Register default value on SPRA0: 0x001F3D0A                                  */
/* Register default value on SPRB0: 0x001F3D0A                                  */
/* Register default value on SPRHBM: 0x001F3D0A                                 */
/* Register default value on SPRC0: 0x001F3D0A                                  */
/* Register default value on SPRMCC: 0x001F3D0A                                 */
/* Register default value on SPRUCC: 0x001F3D0A                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* contains a set of thresholds that can control how much of the TOR various types of requests are allowed to occupy.
*/


#define TOR_THRESHOLDS_CHABC_SAD1_REG 0x03120420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rrq_count_threshold : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000000A*/

                            /*
                               controls the maximum number of RRQ requests that
                               can allocate into the TOR.
                            */
    UINT32 loc2rem_thresh_norm : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000008*/

                            /*
                               number of local to remote requests allowed in
                               pipeline when remote requests are also present
                               in pipeline
                            */
    UINT32 loc2rem_thresh_empty : 5;

                            /* Bits[14:10], Access Type=RW, default=0x0000000F*/

                            /*
                               number of local to remote requests allowed in
                               pipeline when pipeline is empty of remote
                               requests
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 prq_count_threshold : 5;

                            /* Bits[20:16], Access Type=RW, default=0x0000001F*/

                            /*
                               controls the maximum number of PRQ requests that
                               can be allocated into the TOR.
                            */
    UINT32 rsvd_21 : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TOR_THRESHOLDS_CHABC_SAD1_STRUCT;

/* LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_REG supported on:                     */
/*      SPRA0 (0x21fe9424)                                                      */
/*      SPRB0 (0x21fe9424)                                                      */
/*      SPRHBM (0x21fe9424)                                                     */
/*      SPRC0 (0x21fe9424)                                                      */
/*      SPRMCC (0x21fe9424)                                                     */
/*      SPRUCC (0x21fe9424)                                                     */
/* Register default value on SPRA0: 0x000C520C                                  */
/* Register default value on SPRB0: 0x000C520C                                  */
/* Register default value on SPRHBM: 0x000C520C                                 */
/* Register default value on SPRC0: 0x000C520C                                  */
/* Register default value on SPRMCC: 0x000C520C                                 */
/* Register default value on SPRUCC: 0x000C520C                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW memory QoS LOC TOR threshold
*/


#define LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_REG 0x03120424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 loc_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable TOR threshold for low BW memory.</p>
                               <p>This enable bit controls whether we use the 4
                               TOR thresholds in this register</p> <p></p>
                            */
    UINT32 loc_wr_maxbw_th : 5;

                            /* Bits[5:1], Access Type=RW, default=0x00000006*/

                            /*
                               <p>TOR threshold for low BW memory for getting
                               to local write maxBW.</p> <p>This is the number
                               of TOR entries per CHA needed to saturate the
                               low BW memory&nbsp;with local writes.</p>
                               <p></p>
                            */
    UINT32 loc_wr_hi_th : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000008*/

                            /*
                               <p>TOR high threshold for low BW memory for
                               getting to local write maxBW.</p> <p>This is the
                               number of TOR entries per CHA that is higher
                               than that needed to saturate the low BW
                               memory&nbsp;with local writes. This is used to
                               take in a few more local low BW memory writes
                               than that needed to saturate BW.</p> <p></p>
                            */
    UINT32 loc_wr_vic_hi_th : 5;

                            /* Bits[15:11], Access Type=RW, default=0x0000000A*/

                            /*
                               <p>TOR threshold for low BW memory at which last
                               level cache low BW memory Victims have to be
                               dealt with.</p> <p></p>
                            */
    UINT32 loc_rdwr_hi_th : 5;

                            /* Bits[20:16], Access Type=RW, default=0x0000000C*/

                            /*
                               <p>TOR threshold for low BW memory for getting
                               to local&nbsp;read/write maxBW.</p> <p>This is
                               the number of TOR entries per CHA needed to
                               saturate the low BW memory&nbsp;with local
                               read/writes.</p>
                            */
    UINT32 rsvd : 10;

                            /* Bits[30:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_tor_req_fifo_enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable a separate TOR REQ FIFO for DDRT write
                               transactions.</p> <p></p> <p></p>
                            */

  } Bits;
  UINT32 Data;

} LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_STRUCT;

/* LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_REG supported on:                */
/*      SPRA0 (0x21fe9428)                                                      */
/*      SPRB0 (0x21fe9428)                                                      */
/*      SPRHBM (0x21fe9428)                                                     */
/*      SPRC0 (0x21fe9428)                                                      */
/*      SPRMCC (0x21fe9428)                                                     */
/*      SPRUCC (0x21fe9428)                                                     */
/* Register default value on SPRA0: 0x000E6290                                  */
/* Register default value on SPRB0: 0x000E6290                                  */
/* Register default value on SPRHBM: 0x000E6290                                 */
/* Register default value on SPRC0: 0x000E6290                                  */
/* Register default value on SPRMCC: 0x000E6290                                 */
/* Register default value on SPRUCC: 0x000E6290                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW memory QoS rem TOR threshold
*/


#define LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_REG 0x03120428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 loctorem_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable local to remote TOR threshold for low
                               BW remote memory.</p> <p>This enable bit
                               controls whether we use the 4 TOR thresholds in
                               this register</p>
                            */
    UINT32 loctorem_maxbw_th : 5;

                            /* Bits[5:1], Access Type=RW, default=0x00000008*/

                            /*
                               <p>TOR threshold for low BW memory for local to
                               remote&nbsp;write maxBW.&nbsp;This is the
                               threshold in the source socket TOR for accesses
                               going to remote socket.</p> <p>This is the
                               number of TOR entries per CHA needed to saturate
                               the low BW remote memory&nbsp;with local to
                               remote writes.</p>
                            */
    UINT32 loctorem_hi_th : 5;

                            /* Bits[10:6], Access Type=RW, default=0x0000000A*/

                            /*
                               <p>TOR high threshold for low BW memory for
                               local to remote&nbsp;write maxBW. This is the
                               threshold in the source socket TOR for accesses
                               going to remote socket.</p> <p>This is the
                               number of TOR entries per CHA that is higher
                               than that needed to saturate the low BW remote
                               memory&nbsp;with local to remote&nbsp;writes.
                               This is used to take in a few more
                               remote&nbsp;low BW memory writes than that
                               needed to saturate BW.</p>
                            */
    UINT32 loctorem_vic_hi_th : 5;

                            /* Bits[15:11], Access Type=RW, default=0x0000000C*/

                            /*
                               <p>TOR threshold for low BW memory for local to
                               remote&nbsp;write maxBW.</p> <p>This is the
                               number of TOR entries per CHA needed to saturate
                               the low BW remote memory&nbsp;with local to
                               remote writes.</p>
                            */
    UINT32 loctorem_rdwr_hi_th : 5;

                            /* Bits[20:16], Access Type=RW, default=0x0000000E*/

                            /*
                               <p>TOR threshold for low BW memory for local to
                               remote&nbsp;read/write maxBW. This is the
                               threshold in the source socket TOR for accesses
                               going to remote socket.</p> <p>This is the
                               number of TOR entries per CHA needed to saturate
                               the low BW remote memory&nbsp;with local to
                               remote read/writes.</p>
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_STRUCT;

/* SSLLC_ALLOC_POLICY_CHABC_SAD1_REG supported on:                              */
/*      SPRA0 (0x21fe9430)                                                      */
/*      SPRB0 (0x21fe9430)                                                      */
/*      SPRHBM (0x21fe9430)                                                     */
/*      SPRC0 (0x21fe9430)                                                      */
/*      SPRMCC (0x21fe9430)                                                     */
/*      SPRUCC (0x21fe9430)                                                     */
/* Register default value on SPRA0: 0x1314FFFF                                  */
/* Register default value on SPRB0: 0x1314FFFF                                  */
/* Register default value on SPRHBM: 0x1314FFFF                                 */
/* Register default value on SPRC0: 0x1314FFFF                                  */
/* Register default value on SPRMCC: 0x1314FFFF                                 */
/* Register default value on SPRUCC: 0x1314FFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ss LLC Allocation policy 
*/


#define SSLLC_ALLOC_POLICY_CHABC_SAD1_REG 0x03120430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flr : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000FF*/

                            /*
                               Threshold below which we start allocating
                               victims to ssLLC
                            */
    UINT32 ceiling : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /*
                               Threshold above which we stop allocating victims
                               to ssLLC
                            */
    UINT32 sharinc : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000004*/

                            /*
                               Increment amount to counter for every&nbsp;LLC
                               read hit S (multi-CV)
                            */
    UINT32 vicdec : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000001*/

                            /* Decrement amount for every core victim */
    UINT32 en_shar_m_alloc : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /* Enable allocation of shared M-lines in ssLLC */
    UINT32 en_shar_drd_alloc_always : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Enable allocation of all&nbsp;clean-shared
                               Data&nbsp;lines (RspSFwd)
                            */
    UINT32 en_shar_drd_alloc_2plus : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable allocation of all&nbsp;clean-shared
                               Data&nbsp;lines (RspSFwd) if&nbsp;2 or more
                               already and we are appending</p>
                            */
    UINT32 en_shar_drd_alloc_3plus : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable allocation of all&nbsp;clean-shared
                               Data&nbsp;lines (RspSFwd) if 3&nbsp;or more
                               already and we are appending</p>
                            */
    UINT32 en_shar_crd_alloc_always : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Enable allocation of all&nbsp;clean-shared Code
                               lines (RspSFwd)
                            */
    UINT32 en_shar_crd_alloc_2plus : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable allocation of all&nbsp;clean-shared
                               Code&nbsp;lines (RspSFwd) if&nbsp;2 or more
                               already and we are appending</p>
                            */
    UINT32 en_shar_crd_alloc_3plus : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Enable allocation of all&nbsp;clean-shared
                               Code&nbsp;lines (RspSFwd) if 3&nbsp;or more
                               already and we are appending</p>
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SSLLC_ALLOC_POLICY_CHABC_SAD1_STRUCT;

/* UPI_ROUTING_CONFIG_CHABC_SAD1_REG supported on:                              */
/*      SPRA0 (0x11fe9438)                                                      */
/*      SPRB0 (0x11fe9438)                                                      */
/*      SPRHBM (0x11fe9438)                                                     */
/*      SPRC0 (0x11fe9438)                                                      */
/*      SPRMCC (0x11fe9438)                                                     */
/*      SPRUCC (0x11fe9438)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Selects the index bits used for indexing into the UpiRoutingTable.LogicalUpiTarget. The register supports defining up to 16 different indexing/interleave modes to UPI. Currently only 3 modes are used, the remaining are reserved for future.
*/


#define UPI_ROUTING_CONFIG_CHABC_SAD1_REG 0x03110438

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 upi_interleave_mode : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 : Index = {NID[2:0],PA[12]^PA[6]} (interleaves
                               1 to 2 UPI links between 8 nodes) 1 : Index =
                               {NID[1:0], MOD3[1:0]} (interleaves 1 to 3 UPI
                               links between 4 nodes) 2 : Index = {NID[1:0],
                               PA[13:12]^PA[7:6]} (interleaves 1 to 4 UPI links
                               between 4 nodes) 3-15 : RSVD
                            */
    UINT16 rsvd : 12;

                            /* Bits[15:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} UPI_ROUTING_CONFIG_CHABC_SAD1_STRUCT;

/* IIO_ALTERNATE_WAYS_CHABC_SAD1_REG supported on:                              */
/*      SPRA0 (0x21fe9440)                                                      */
/*      SPRB0 (0x21fe9440)                                                      */
/*      SPRHBM (0x21fe9440)                                                     */
/*      SPRC0 (0x21fe9440)                                                      */
/*      SPRMCC (0x21fe9440)                                                     */
/*      SPRUCC (0x21fe9440)                                                     */
/* Register default value on SPRA0: 0x000C0000                                  */
/* Register default value on SPRB0: 0x000C0000                                  */
/* Register default value on SPRHBM: 0x000C0000                                 */
/* Register default value on SPRC0: 0x000C0000                                  */
/* Register default value on SPRMCC: 0x000C0000                                 */
/* Register default value on SPRUCC: 0x000C0000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* LLC Way Mask for IIO ItoM and RdCurr (SKX b302231)
*/


#define IIO_ALTERNATE_WAYS_CHABC_SAD1_REG 0x03120440

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 way_mask : 20;

                            /* Bits[19:0], Access Type=RW/V, default=0x000C0000*/

                            /*
                               available LLC way vectors for class of service
                               of IIO ItoM/RdCurr (default is two high ways,
                               programmed by decoding fuse)
                            */
    UINT32 enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* alternate ways for IIO ItoM/RdCurr enable */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IIO_ALTERNATE_WAYS_CHABC_SAD1_STRUCT;

/* REMOTE_WAYS_N0_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9450)                                                      */
/*      SPRB0 (0x21fe9450)                                                      */
/*      SPRHBM (0x21fe9450)                                                     */
/*      SPRC0 (0x21fe9450)                                                      */
/*      SPRMCC (0x21fe9450)                                                     */
/*      SPRUCC (0x21fe9450)                                                     */
/* Register default value on SPRA0: 0x000C0000                                  */
/* Register default value on SPRB0: 0x000C0000                                  */
/* Register default value on SPRHBM: 0x000C0000                                 */
/* Register default value on SPRC0: 0x000C0000                                  */
/* Register default value on SPRMCC: 0x000C0000                                 */
/* Register default value on SPRUCC: 0x000C0000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Available LLC/SF ways vectors for Remote Requests/Probes
*/


#define REMOTE_WAYS_N0_CHABC_SAD1_REG 0x03120450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 remoteways : 20;

                            /* Bits[19:0], Access Type=RW/V, default=0x000C0000*/

                            /* Available LLC/SF ways vectors for KWbPushMtoI */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REMOTE_WAYS_N0_CHABC_SAD1_STRUCT;

/* REMOTE_WAYS_N1_CHABC_SAD1_REG supported on:                                  */
/*      SPRA0 (0x21fe9454)                                                      */
/*      SPRB0 (0x21fe9454)                                                      */
/*      SPRHBM (0x21fe9454)                                                     */
/*      SPRC0 (0x21fe9454)                                                      */
/*      SPRMCC (0x21fe9454)                                                     */
/*      SPRUCC (0x21fe9454)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Available LLC/SF ways vectors for Remote Requests/Probes
*/


#define REMOTE_WAYS_N1_CHABC_SAD1_REG 0x03120454

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

} REMOTE_WAYS_N1_CHABC_SAD1_STRUCT;

#if defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* RSF_EVCTCLN_NODE_MASK_CHABC_SAD1_REG supported on:                           */
/*      SPRHBM (0x21fe9458)                                                     */
/*      SPRC0 (0x21fe9458)                                                      */
/*      SPRMCC (0x21fe9458)                                                     */
/*      SPRUCC (0x21fe9458)                                                     */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRHBM BDF: 31_29_1                                    */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains info about the XNCs in the system.
*/


#define RSF_EVCTCLN_NODE_MASK_CHABC_SAD1_REG 0x03120458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 maskfordramreq : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x000000FF*/

                            /*
                               This is an 8 bit field. We index into this reg
                               field using requests NodeID. The value of each
                               bit specifies if that socket is expecting a
                               evctcln or not. 1 - Send EvctCLn 0 - Donot send
                               EvctCln
                            */
    UINT32 maskforddrtreq : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x000000FF*/

                            /*
                               This is an 8 bit field. We index into this reg
                               field using request NodeID. The value of each
                               bit specifies if that socket is expecting a
                               evctcln or not. 1 - Send EvctCLn 0 - Donot send
                               EvctCln
                            */
    UINT32 maskforhbmreq : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x000000FF*/

                            /*
                               This is an 8 bit field. We index into this reg
                               field using requests NodeID. The value of each
                               bit specifies if that socket is expecting a
                               evctcln or not. 1 - Send EvctCLn 0 - Donot send
                               EvctCln
                            */
    UINT32 maskforialreq : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x000000FF*/

                            /*
                               This is an 8 bit field. We index into this reg
                               field using requests NodeID. The value of each
                               bit specifies if that socket is expecting a
                               evctcln or not. 1 - Send EvctCLn 0 - Donot send
                               EvctCln
                            */

  } Bits;
  UINT32 Data;

} RSF_EVCTCLN_NODE_MASK_CHABC_SAD1_STRUCT;
#endif /* (SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* LLC_HIT_TOR_ENTRIES_CHABC_SAD1_REG supported on:                             */
/*      SPRA0 (0x21fe945c)                                                      */
/*      SPRB0 (0x21fe945c)                                                      */
/*      SPRHBM (0x21fe945c)                                                     */
/*      SPRC0 (0x21fe945c)                                                      */
/*      SPRMCC (0x21fe945c)                                                     */
/*      SPRUCC (0x21fe945c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000003                                  */
/* Register default value on SPRHBM: 0x00000003                                 */
/* Register default value on SPRC0: 0x00000003                                  */
/* Register default value on SPRMCC: 0x00000003                                 */
/* Register default value on SPRUCC: 0x00000003                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* contains the number of TOR entries that should be programmed to be reserved for LLC hits. Based on this, a mask would be created to reserve the specific entries in the TOR keeping in mind the already reserved entries in TOR 
*/


#define LLC_HIT_TOR_ENTRIES_CHABC_SAD1_REG 0x0312045C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_llc_hit_entries : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number of LLC hit entries to be reserved in the
                               TOR
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LLC_HIT_TOR_ENTRIES_CHABC_SAD1_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* QLRU_CONFIG_CHABC_SAD1_REG supported on:                                     */
/*      SPRC0 (0x21fe9460)                                                      */
/*      SPRMCC (0x21fe9460)                                                     */
/*      SPRUCC (0x21fe9460)                                                     */
/* Register default value on SPRC0: 0x5B59FAAF                                  */
/* Register default value on SPRMCC: 0x5B59FAAF                                 */
/* Register default value on SPRUCC: 0x5B59FAAF                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRC0 BDF: 31_29_1                                     */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* uCR: LRU Update values for various opcodes that update the LRU bits.
*/


#define QLRU_CONFIG_CHABC_SAD1_REG 0x03120460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /* LRU Age Update value for CRd/Monitor on LLC Hit */
    UINT32 cr_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000003*/

                            /*
                               LRU Age Update value for Prefetch Code (to MLC)
                               on LLC Hit
                            */
    UINT32 cr_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /*
                               LRU Age Update value for Prefetch Code (to LLC)
                               on LLC Hit
                            */
    UINT32 cr_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000002*/

                            /*
                               LRU Age Update value for DRd/RFO/DRdPTE/ItoM on
                               LLC Hit
                            */
    UINT32 cr_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000002*/

                            /*
                               LRU Age Update value for Prefetch data (to MLC)
                               on LLC Hit
                            */
    UINT32 cr_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000002*/

                            /*
                               LRU Age Update value for PrefRFO / PrefData (to
                               LLC) on LLC Hit
                            */
    UINT32 cr_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000003*/

                            /*
                               LRU Age Update value for CRd / Monitor on LLC
                               Miss (Normal Fill)
                            */
    UINT32 cr_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /*
                               LRU Age Update value for Prefetch Code (to MLC)
                               on LLC Miss (Normal Fill)
                            */
    UINT32 cr_8 : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for Prefetch Code (to LLC)
                               on LLC Miss (Normal Fill)
                            */
    UINT32 cr_9 : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000002*/

                            /*
                               LRU Age Update value for DRd / RFO / DRdPTE /
                               ItoM on LLC Miss (Normal Fill)
                            */
    UINT32 cr_b : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for Prefetch data (to MLC)
                               on LLC Miss (Normal Fill)
                            */
    UINT32 cr_d : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for PrefRFO / PrefData (to
                               LLC) on LLC Miss (Normal Fill)
                            */
    UINT32 cr_f : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000003*/

                            /*
                               LRU Age Update value for demand read and MLC
                               prefetche on LLC Hit, when the current age is
                               2/3.
                            */
    UINT32 cr_10 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000002*/

                            /* LRU Age update value for IIO requests on LLC Hit */
    UINT32 cr_11 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000001*/

                            /* LRU Age update for IIO requests on LLC Miss */
    UINT32 cr_12 : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000001*/

                            /* LRU Age update for Writebacks */

  } Bits;
  UINT32 Data;

} QLRU_CONFIG_CHABC_SAD1_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* QLRU_CONFIG2_CHABC_SAD1_REG supported on:                                    */
/*      SPRC0 (0x21fe9464)                                                      */
/*      SPRMCC (0x21fe9464)                                                     */
/*      SPRUCC (0x21fe9464)                                                     */
/* Register default value on SPRC0: 0x03145003                                  */
/* Register default value on SPRMCC: 0x03145003                                 */
/* Register default value on SPRUCC: 0x03145003                                 */
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRC0 BDF: 31_29_1                                     */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* uCR: LRU Update values for various opcodes that update the LRU bits.
*/


#define QLRU_CONFIG2_CHABC_SAD1_REG 0x03120464

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_13 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /* LRU Age update for WbPushHint */
    UINT32 rsvd : 10;

                            /* Bits[11:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cr_6_llc : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for CRd / Monitor on LLC
                               allocate
                            */
    UINT32 cr_7_llc : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for Prefetch Code (to MLC)
                               on LLC allocate
                            */
    UINT32 rsvd_16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cr_9_llc : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for DRd / RFO / DRdPTE /
                               ItoM on LLC Miss (Vulnerable Fill)
                            */
    UINT32 cr_b_llc : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000001*/

                            /*
                               LRU Age Update value for Prefetch data (to MLC)
                               on LLC Miss (Vulnerable Fill)
                            */
    UINT32 cr_14 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /* LRU Age update for CLCleanse */
    UINT32 cr_15 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000003*/

                            /* LRU Age update for LLC allocation from SF Victim */
    UINT32 cr_16 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /* LRU Age update for Remote Requests */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QLRU_CONFIG2_CHABC_SAD1_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* LB_W_MCHECK_CHA_CP_N0_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe94e8)                                                      */
/*      SPRB0 (0x21fe94e8)                                                      */
/*      SPRHBM (0x21fe94e8)                                                     */
/*      SPRC0 (0x21fe94e8)                                                      */
/*      SPRMCC (0x21fe94e8)                                                     */
/*      SPRUCC (0x21fe94e8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Control Policy
*/


#define LB_W_MCHECK_CHA_CP_N0_CHABC_SAD1_REG 0x031204E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_CP_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_CP_N1_CHABC_SAD1_REG supported on:                           */
/*      SPRA0 (0x21fe94ec)                                                      */
/*      SPRB0 (0x21fe94ec)                                                      */
/*      SPRHBM (0x21fe94ec)                                                     */
/*      SPRC0 (0x21fe94ec)                                                      */
/*      SPRMCC (0x21fe94ec)                                                     */
/*      SPRUCC (0x21fe94ec)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Control Policy
*/


#define LB_W_MCHECK_CHA_CP_N1_CHABC_SAD1_REG 0x031204EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to LB_W_MCHECK_CHA WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_CP_N1_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_RAC_N0_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x21fe94f0)                                                      */
/*      SPRB0 (0x21fe94f0)                                                      */
/*      SPRHBM (0x21fe94f0)                                                     */
/*      SPRC0 (0x21fe94f0)                                                      */
/*      SPRMCC (0x21fe94f0)                                                     */
/*      SPRUCC (0x21fe94f0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Read Access Control
*/


#define LB_W_MCHECK_CHA_RAC_N0_CHABC_SAD1_REG 0x031204F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_RAC_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_RAC_N1_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x21fe94f4)                                                      */
/*      SPRB0 (0x21fe94f4)                                                      */
/*      SPRHBM (0x21fe94f4)                                                     */
/*      SPRC0 (0x21fe94f4)                                                      */
/*      SPRMCC (0x21fe94f4)                                                     */
/*      SPRUCC (0x21fe94f4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Read Access Control
*/


#define LB_W_MCHECK_CHA_RAC_N1_CHABC_SAD1_REG 0x031204F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed read
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_RAC_N1_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_WAC_N0_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x21fe94f8)                                                      */
/*      SPRB0 (0x21fe94f8)                                                      */
/*      SPRHBM (0x21fe94f8)                                                     */
/*      SPRC0 (0x21fe94f8)                                                      */
/*      SPRMCC (0x21fe94f8)                                                     */
/*      SPRUCC (0x21fe94f8)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Write Access Control
*/


#define LB_W_MCHECK_CHA_WAC_N0_CHABC_SAD1_REG 0x031204F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_WAC_N0_CHABC_SAD1_STRUCT;

/* LB_W_MCHECK_CHA_WAC_N1_CHABC_SAD1_REG supported on:                          */
/*      SPRA0 (0x21fe94fc)                                                      */
/*      SPRB0 (0x21fe94fc)                                                      */
/*      SPRHBM (0x21fe94fc)                                                     */
/*      SPRC0 (0x21fe94fc)                                                      */
/*      SPRMCC (0x21fe94fc)                                                     */
/*      SPRUCC (0x21fe94fc)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff */
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/PIPE/CFG_CBO_BCAST_1_0x400_0xfff*/
/* Struct generated from SPRA0 BDF: 31_29_1                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* LB_W_MCHECK_CHA Write Access Control
*/


#define LB_W_MCHECK_CHA_WAC_N1_CHABC_SAD1_REG 0x031204FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_42 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 sai_mask_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to registers in LB_W_MCHECK_CHA security
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_W_MCHECK_CHA_WAC_N1_CHABC_SAD1_STRUCT;
#endif /* _CHABC_SAD1_h */
