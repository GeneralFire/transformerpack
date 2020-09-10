
/** @file
  MS2IDI_MAIN.h

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
/* LB_MCHECK_W                                                                  */
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

/* SPRB0 Security Policy Groups:                                                */
/* LB_MCHECK_W                                                                  */
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

/* SPRHBM Security Policy Groups:                                               */
/* LB_MCHECK_W                                                                  */
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

/* SPRC0 Security Policy Groups:                                                */
/* LB_MCHECK_W                                                                  */
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

/* SPRMCC Security Policy Groups:                                               */
/* LB_MCHECK_W                                                                  */
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

/* SPRUCC Security Policy Groups:                                               */
/* LB_MCHECK_W                                                                  */
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


#ifndef _MS2IDI_MAIN_h
#define _MS2IDI_MAIN_h
#include <Base.h>

/* SNC_BASE_1_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x20000000)                                                      */
/*      SPRB0 (0x20000000)                                                      */
/*      SPRHBM (0x20000000)                                                     */
/*      SPRC0 (0x20000000)                                                      */
/*      SPRMCC (0x20000000)                                                     */
/*      SPRUCC (0x20000000)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_BASE_1 Configuration
*/


#define SNC_BASE_1_MS2IDI_MAIN_REG 0x14020000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Base address [45:30] */
    UINT32 twolm_mask : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /* TwoLM_Mask[45:40] */
    UINT32 hi_twolm_mask : 6;

                            /* Bits[27:22], Access Type=RW/P, default=0x00000000*/

                            /* TwoLM_Mask [51:46] */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_1_MS2IDI_MAIN_STRUCT;

/* SNC_BASE_2_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x20000004)                                                      */
/*      SPRB0 (0x20000004)                                                      */
/*      SPRHBM (0x20000004)                                                     */
/*      SPRC0 (0x20000004)                                                      */
/*      SPRMCC (0x20000004)                                                     */
/*      SPRUCC (0x20000004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_BASE_2 Configuration
*/


#define SNC_BASE_2_MS2IDI_MAIN_REG 0x14020004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Base address [45:30] */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 twolm_mask : 12;

                            /* Bits[28:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               TwoLM_Mask[39:28], For CNX TwoLM_Mask[28] is
                               always 0
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_2_MS2IDI_MAIN_STRUCT;

/* SNC_BASE_3_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x20000008)                                                      */
/*      SPRB0 (0x20000008)                                                      */
/*      SPRHBM (0x20000008)                                                     */
/*      SPRC0 (0x20000008)                                                      */
/*      SPRMCC (0x20000008)                                                     */
/*      SPRUCC (0x20000008)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_BASE Configuration
*/


#define SNC_BASE_3_MS2IDI_MAIN_REG 0x14020008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Base address [45:30] */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_3_MS2IDI_MAIN_STRUCT;

/* SNC_BASE_4_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x20000010)                                                      */
/*      SPRB0 (0x20000010)                                                      */
/*      SPRHBM (0x20000010)                                                     */
/*      SPRC0 (0x20000010)                                                      */
/*      SPRMCC (0x20000010)                                                     */
/*      SPRUCC (0x20000010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_BASE Configuration
*/


#define SNC_BASE_4_MS2IDI_MAIN_REG 0x14020010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Base address [45:30] */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_4_MS2IDI_MAIN_STRUCT;

/* SNC_BASE_5_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x20000014)                                                      */
/*      SPRB0 (0x20000014)                                                      */
/*      SPRHBM (0x20000014)                                                     */
/*      SPRC0 (0x20000014)                                                      */
/*      SPRMCC (0x20000014)                                                     */
/*      SPRUCC (0x20000014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_BASE Configuration
*/


#define SNC_BASE_5_MS2IDI_MAIN_REG 0x14020014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Base address [45:30] */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_5_MS2IDI_MAIN_STRUCT;

/* SNC_UPPER_BASE_MS2IDI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20000018)                                                      */
/*      SPRB0 (0x20000018)                                                      */
/*      SPRHBM (0x20000018)                                                     */
/*      SPRC0 (0x20000018)                                                      */
/*      SPRMCC (0x20000018)                                                     */
/*      SPRUCC (0x20000018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC_UPPER_BASE Configuration
*/


#define SNC_UPPER_BASE_MS2IDI_MAIN_REG 0x14020018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upper_base_1 : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC Upper_base_1 address */
    UINT32 upper_base_2 : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000000*/

                            /* SNC Upper_base_2 address[51:46] */
    UINT32 upper_base_3 : 6;

                            /* Bits[17:12], Access Type=RW/P, default=0x00000000*/

                            /* SNC Upper_base_3 address[51:46] */
    UINT32 upper_base_4 : 6;

                            /* Bits[23:18], Access Type=RW/P, default=0x00000000*/

                            /* SNC Upper_base_4 address[51:46] */
    UINT32 upper_base_5 : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000000*/

                            /* SNC Upper_base_5 address[51:46] */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_UPPER_BASE_MS2IDI_MAIN_STRUCT;

/* SNC_CONFIG_MS2IDI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x2000001c)                                                      */
/*      SPRB0 (0x2000001c)                                                      */
/*      SPRHBM (0x2000001c)                                                     */
/*      SPRC0 (0x2000001c)                                                      */
/*      SPRMCC (0x2000001c)                                                     */
/*      SPRUCC (0x2000001c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SNC Configuration
*/


#define SNC_CONFIG_MS2IDI_MAIN_REG 0x1402001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 full_snc_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Enable for SNC */
    UINT32 snc_ind_enable : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* SNC range check enable */
    UINT32 num_clusters : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /* Number of SNC Clusters */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_CONFIG_MS2IDI_MAIN_STRUCT;

/* UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x20000020)                                                      */
/*      SPRB0 (0x20000020)                                                      */
/*      SPRHBM (0x20000020)                                                     */
/*      SPRC0 (0x20000020)                                                      */
/*      SPRMCC (0x20000020)                                                     */
/*      SPRUCC (0x20000020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UNCORE SNC Configuration. This register needs to be programmed before SNC_CONFIG- otherwise causes a hang in BIOS.

The Base_CHA_Cluster needs to be programmed before BIOS can try to reach all other CHAs- this is needed for LUT lookup

*/


#define UNCORE_SNC_CONFIG_MS2IDI_MAIN_REG 0x14020020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_cha_per_cluster : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /* Num CHA Per Cluster/Base CHA for Cluster 0 */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_1 : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 1 */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_2 : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 2 */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_3 : 7;

                            /* Bits[30:24], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 3 */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCORE_SNC_CONFIG_MS2IDI_MAIN_STRUCT;

/* UMA_CLUSTER_MS2IDI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x10000024)                                                      */
/*      SPRB0 (0x10000024)                                                      */
/*      SPRHBM (0x10000024)                                                     */
/*      SPRC0 (0x10000024)                                                      */
/*      SPRMCC (0x10000024)                                                     */
/*      SPRUCC (0x10000024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UMA Clustering Configuration
*/


#define UMA_CLUSTER_MS2IDI_MAIN_REG 0x14010024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 uma_cluster_enable : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /* Enable for UMA clustering */
    UINT16 defeature_xor : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit for XOR defeature mode. By default,
                               UMA mode clustering uses
                               Addr[26:25]^Addr[18:17]^Addr[12:11]^Addr[9:8].
                               But when this bit is set to 1, Addr[12:11] are
                               used.
                            */
    UINT16 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} UMA_CLUSTER_MS2IDI_MAIN_STRUCT;

/* XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG supported on:                     */
/*      SPRA0 (0x20000038)                                                      */
/*      SPRB0 (0x20000038)                                                      */
/*      SPRHBM (0x20000038)                                                     */
/*      SPRC0 (0x20000038)                                                      */
/*      SPRMCC (0x20000038)                                                     */
/*      SPRUCC (0x20000038)                                                     */
/* Register default value on SPRA0: 0x000400BD                                  */
/* Register default value on SPRB0: 0x000400BD                                  */
/* Register default value on SPRHBM: 0x000400BD                                 */
/* Register default value on SPRC0: 0x000400BD                                  */
/* Register default value on SPRMCC: 0x000400BD                                 */
/* Register default value on SPRUCC: 0x000400BD                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Configuration for XPT Prefetch
*/


#define XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_REG 0x14020038

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 priorityprefetchfordemandread : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               If this bit is set to 1, XPT prefetch messages
                               generated for demand reads will be tagged as
                               priority transactions. This demand priority
                               settings must be mutually exclusive with ISOC,
                               which may be enabled elsewhere (e.g. in CHA).
                               (For remote memory requests, demand priority may
                               only be used in system with four processor nodes
                               or fewer.)
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_LOCAL_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT;

/* XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_REG supported on:                     */
/*      SPRA0 (0x2000003c)                                                      */
/*      SPRB0 (0x2000003c)                                                      */
/*      SPRHBM (0x2000003c)                                                     */
/*      SPRC0 (0x2000003c)                                                      */
/*      SPRMCC (0x2000003c)                                                     */
/*      SPRUCC (0x2000003c)                                                     */
/* Register default value on SPRA0: 0x008186A0                                  */
/* Register default value on SPRB0: 0x008186A0                                  */
/* Register default value on SPRHBM: 0x008186A0                                 */
/* Register default value on SPRC0: 0x008186A0                                  */
/* Register default value on SPRMCC: 0x008186A0                                 */
/* Register default value on SPRUCC: 0x008186A0                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Configuration for XPT Prefetch
*/


#define XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_REG 0x1402003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptfeedbackwincntlimit : 17;

                            /* Bits[16:0], Access Type=RW, default=0x000186A0*/

                            /* Sets the number of cycles per feedback window */
    UINT32 xptminisadmodeselect : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               Selects Mode to index the Minisad 000 = address
                               [8+N:6+N], 001 = {address [51:6+N]}%3, 010 =
                               {address [8],address [51:6+N]%3}, 011 = {address
                               [51:6+N]}%5, 100 = {address [8],address
                               [51:6+N]%5}
                            */
    UINT32 xptforceprefetch : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Generate prefetch regardless of predictor
                               result, but still follows opcode setting.
                            */
    UINT32 xptresponsefuncsel : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Select for XPT Response Function */
    UINT32 xptresponsefuncdis : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Dis for XPT Response Function */
    UINT32 xptmcminisadmode : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Mode to select if MC MiniSAD is enabled or not.
                               1 = MC miniSAD is enabled, 0 = MC MiniSAD is
                               disabled
                            */
    UINT32 xpt_defeature_xor : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* Defeature XOR for XPT's channel ID decode */
    UINT32 xpt_mispredict_tracker_defeature : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Defeature for local XPT mispredict tracker.
                               Default to 0.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_LOCAL_PREFETCH_CONFIG2_MS2IDI_MAIN_STRUCT;

/* XPT_2_ENTRY_MINISAD_TABLE_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000040)                                                      */
/*      SPRB0 (0x20000040)                                                      */
/*      SPRHBM (0x20000040)                                                     */
/*      SPRC0 (0x20000040)                                                      */
/*      SPRMCC (0x20000040)                                                     */
/*      SPRUCC (0x20000040)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_2_ENTRY_MINISAD_TABLE_MS2IDI_MAIN_REG 0x14020040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_2_ENTRY_MINISAD_TABLE_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_0_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000044)                                                      */
/*      SPRB0 (0x20000044)                                                      */
/*      SPRHBM (0x20000044)                                                     */
/*      SPRC0 (0x20000044)                                                      */
/*      SPRMCC (0x20000044)                                                     */
/*      SPRUCC (0x20000044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_0_MS2IDI_MAIN_REG 0x14020044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_0_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_1_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000048)                                                      */
/*      SPRB0 (0x20000048)                                                      */
/*      SPRHBM (0x20000048)                                                     */
/*      SPRC0 (0x20000048)                                                      */
/*      SPRMCC (0x20000048)                                                     */
/*      SPRUCC (0x20000048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_1_MS2IDI_MAIN_REG 0x14020048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_1_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_2_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x2000004c)                                                      */
/*      SPRB0 (0x2000004c)                                                      */
/*      SPRHBM (0x2000004c)                                                     */
/*      SPRC0 (0x2000004c)                                                      */
/*      SPRMCC (0x2000004c)                                                     */
/*      SPRUCC (0x2000004c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_2_MS2IDI_MAIN_REG 0x1402004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_2_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_3_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000050)                                                      */
/*      SPRB0 (0x20000050)                                                      */
/*      SPRHBM (0x20000050)                                                     */
/*      SPRC0 (0x20000050)                                                      */
/*      SPRMCC (0x20000050)                                                     */
/*      SPRUCC (0x20000050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_3_MS2IDI_MAIN_REG 0x14020050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_3_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_4_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000054)                                                      */
/*      SPRB0 (0x20000054)                                                      */
/*      SPRHBM (0x20000054)                                                     */
/*      SPRC0 (0x20000054)                                                      */
/*      SPRMCC (0x20000054)                                                     */
/*      SPRUCC (0x20000054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_4_MS2IDI_MAIN_REG 0x14020054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_4_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_MINISAD_TABLE_5_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000058)                                                      */
/*      SPRB0 (0x20000058)                                                      */
/*      SPRHBM (0x20000058)                                                     */
/*      SPRC0 (0x20000058)                                                      */
/*      SPRMCC (0x20000058)                                                     */
/*      SPRUCC (0x20000058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT Mini-SAD Table.  Each entry has 5 bits and the format: {Generate_Prefetch,MC_ID[3:0]}
*/


#define XPT_32_ENTRY_MINISAD_TABLE_5_MS2IDI_MAIN_REG 0x14020058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entry0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry4 : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 entry5 : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000000*/

                            /*
                               Minisad Entry- MSB is Enable prefetch to target.
                               LSB 4 bits are target's Port ID
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_MINISAD_TABLE_5_MS2IDI_MAIN_STRUCT;

/* XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG supported on:                    */
/*      SPRA0 (0x2000005c)                                                      */
/*      SPRB0 (0x2000005c)                                                      */
/*      SPRHBM (0x2000005c)                                                     */
/*      SPRC0 (0x2000005c)                                                      */
/*      SPRMCC (0x2000005c)                                                     */
/*      SPRUCC (0x2000005c)                                                     */
/* Register default value on SPRA0: 0x000400BD                                  */
/* Register default value on SPRB0: 0x000400BD                                  */
/* Register default value on SPRHBM: 0x000400BD                                 */
/* Register default value on SPRC0: 0x000400BD                                  */
/* Register default value on SPRMCC: 0x000400BD                                 */
/* Register default value on SPRUCC: 0x000400BD                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Configuration for XPT Prefetch
*/


#define XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_REG 0x1402005C

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 priorityprefetchfordemandread : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               If this bit is set to 1, XPT prefetch messages
                               generated for demand reads will be tagged as
                               priority transactions. This demand priority
                               settings must be mutually exclusive with ISOC,
                               which may be enabled elsewhere (e.g. in CHA).
                               (For remote memory requests, demand priority may
                               only be used in system with four processor nodes
                               or fewer.)
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptprefetchdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Disable XPT Prefetch */
    UINT32 xptprefetchsubgrp1dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disable XPT prefetch for CRd, DRd,
                               RFO. Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp2en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for DRdPTE,
                               PrefCode, PrefData, PrefRFO. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp3en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for PRd and UCRdF.
                               Not applicable when XPTPrefetchDisable is 1
                            */
    UINT32 xptprefetchsubgrp4en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               When set, enable XPT prefetch for
                               WiL,WCiL,WCiLF. Not applicable when
                               XPTPrefetchDisable is 1
                            */
    UINT32 xpthitpredictthresh : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Minimum number of hits in last 32 accesses for
                               next access to be predicted as hit
                            */
    UINT32 xptmispredthresh : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000100*/

                            /*
                               Number of spurious prefetches in last window for
                               predictor to be disabled in next window
                            */
    UINT32 xptupdtlocalhitonremoteaddr : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               If enabled, updates the XPT local history table
                               for LLC hits on remote addresses (SKX mode).
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_REMOTE_PREFETCH_CONFIG1_MS2IDI_MAIN_STRUCT;

/* XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_REG supported on:                    */
/*      SPRA0 (0x20000060)                                                      */
/*      SPRB0 (0x20000060)                                                      */
/*      SPRHBM (0x20000060)                                                     */
/*      SPRC0 (0x20000060)                                                      */
/*      SPRMCC (0x20000060)                                                     */
/*      SPRUCC (0x20000060)                                                     */
/* Register default value on SPRA0: 0x008186A0                                  */
/* Register default value on SPRB0: 0x008186A0                                  */
/* Register default value on SPRHBM: 0x008186A0                                 */
/* Register default value on SPRC0: 0x008186A0                                  */
/* Register default value on SPRMCC: 0x008186A0                                 */
/* Register default value on SPRUCC: 0x008186A0                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Configuration for XPT Prefetch
*/


#define XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_REG 0x14020060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xptfeedbackwincntlimit : 17;

                            /* Bits[16:0], Access Type=RW, default=0x000186A0*/

                            /* Sets the number of cycles per feedback window */
    UINT32 xptminisadmodeselect : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               Selects Mode to index the Minisad 000 = address
                               [8+N:6+N], 001 = {address [51:6+N]}%3, 010 =
                               {address [8],address [51:6+N]%3}, 011 = {address
                               [51:6+N]}%5, 100 = {address [8],address
                               [51:6+N]%5}
                            */
    UINT32 xptforceprefetch : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Generate prefetch regardless of predictor
                               result, but still follows opcode setting.
                            */
    UINT32 xptresponsefuncsel : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Select for XPT Response Function */
    UINT32 xptresponsefuncdis : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Dis for XPT Response Function */
    UINT32 upi_link_interleave : 2;

                            /* Bits[24:23], Access Type=RW, default=0x00000001*/

                            /*
                               UPI LInk interleave. 00 / 01 = use PA[12] XOR
                               PA[6] , 10 = use mod3, 11 = PA[13:12] XOR
                               PA[7:6]
                            */
    UINT32 rsvd : 1;

                            /* Bits[25:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xpt_mispredict_tracker_defeature : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Defeature bit for Remote XPT mispredicrt
                               tracker. Defaults to 0.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_REMOTE_PREFETCH_CONFIG2_MS2IDI_MAIN_STRUCT;


/* PRQ_MS2IDI_MAIN_REG supported on:                                            */
/*      SPRA0 (0x2000008c)                                                      */
/*      SPRB0 (0x2000008c)                                                      */
/*      SPRHBM (0x2000008c)                                                     */
/*      SPRC0 (0x2000008c)                                                      */
/*      SPRMCC (0x2000008c)                                                     */
/*      SPRUCC (0x2000008c)                                                     */
/* Register default value on SPRA0: 0x00000200                                  */
/* Register default value on SPRB0: 0x00000200                                  */
/* Register default value on SPRHBM: 0x00000200                                 */
/* Register default value on SPRC0: 0x00000200                                  */
/* Register default value on SPRMCC: 0x00000200                                 */
/* Register default value on SPRUCC: 0x00000200                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ Configuration
*/


#define PRQ_MS2IDI_MAIN_REG 0x1402008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 num_prq_credits : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Number of PRQ Credits: Safe Default Value of 2,
                               real value determined by
                               CHANumPrqEntries/IIOstacks
                            */
    UINT32 prq_en : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* PRQ Enable */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PRQ_MS2IDI_MAIN_STRUCT;







/* LB_MCHECK_W_CP_N0_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x200000a8)                                                      */
/*      SPRB0 (0x200000a8)                                                      */
/*      SPRHBM (0x200000a8)                                                     */
/*      SPRC0 (0x200000a8)                                                      */
/*      SPRMCC (0x200000a8)                                                     */
/*      SPRUCC (0x200000a8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N0_MS2IDI_MAIN_REG 0x140200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_ctrl_pol_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_CP_N0_MS2IDI_MAIN_STRUCT;

/* LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x200000ac)                                                      */
/*      SPRB0 (0x200000ac)                                                      */
/*      SPRHBM (0x200000ac)                                                     */
/*      SPRC0 (0x200000ac)                                                      */
/*      SPRMCC (0x200000ac)                                                     */
/*      SPRUCC (0x200000ac)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N1_MS2IDI_MAIN_REG 0x140200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_ctrl_pol_32 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_33 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_34 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_35 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_36 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_37 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_38 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_39 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_40 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_41 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_43 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_44 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_45 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_46 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_47 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_48 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_49 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_50 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_51 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_52 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_53 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_54 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_55 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_56 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_57 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_58 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_59 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_60 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_61 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_62 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_ctrl_pol_63 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to LB_MCHECK_W WAC, RAC and
                               CP registers, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_CP_N1_MS2IDI_MAIN_STRUCT;

/* LB_MCHECK_W_RAC_N0_MS2IDI_MAIN_REG supported on:                             */
/*      SPRA0 (0x200000b0)                                                      */
/*      SPRB0 (0x200000b0)                                                      */
/*      SPRHBM (0x200000b0)                                                     */
/*      SPRC0 (0x200000b0)                                                      */
/*      SPRMCC (0x200000b0)                                                     */
/*      SPRUCC (0x200000b0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N0_MS2IDI_MAIN_REG 0x140200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_RAC_N0_MS2IDI_MAIN_STRUCT;

/* LB_MCHECK_W_RAC_N1_MS2IDI_MAIN_REG supported on:                             */
/*      SPRA0 (0x200000b4)                                                      */
/*      SPRB0 (0x200000b4)                                                      */
/*      SPRHBM (0x200000b4)                                                     */
/*      SPRC0 (0x200000b4)                                                      */
/*      SPRMCC (0x200000b4)                                                     */
/*      SPRUCC (0x200000b4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N1_MS2IDI_MAIN_REG 0x140200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_RAC_N1_MS2IDI_MAIN_STRUCT;

/* LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_REG supported on:                             */
/*      SPRA0 (0x200000b8)                                                      */
/*      SPRB0 (0x200000b8)                                                      */
/*      SPRHBM (0x200000b8)                                                     */
/*      SPRC0 (0x200000b8)                                                      */
/*      SPRMCC (0x200000b8)                                                     */
/*      SPRUCC (0x200000b8)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_REG 0x140200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_WAC_N0_MS2IDI_MAIN_STRUCT;

/* LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_REG supported on:                             */
/*      SPRA0 (0x200000bc)                                                      */
/*      SPRB0 (0x200000bc)                                                      */
/*      SPRHBM (0x200000bc)                                                     */
/*      SPRC0 (0x200000bc)                                                      */
/*      SPRMCC (0x200000bc)                                                     */
/*      SPRUCC (0x200000bc)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_REG 0x140200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lb_mcheck_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */
    UINT32 lb_mcheck_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in LB_MCHECK_W
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} LB_MCHECK_W_WAC_N1_MS2IDI_MAIN_STRUCT;

/* DDRT_RANGE_BASE_MS2IDI_MAIN_REG supported on:                                */
/*      SPRA0 (0x200000f0)                                                      */
/*      SPRB0 (0x200000f0)                                                      */
/*      SPRHBM (0x200000f0)                                                     */
/*      SPRC0 (0x200000f0)                                                      */
/*      SPRMCC (0x200000f0)                                                     */
/*      SPRUCC (0x200000f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base of DDRT Address range. Address [51:26] will be compared against this
*/


#define DDRT_RANGE_BASE_MS2IDI_MAIN_REG 0x140200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_base_address : 26;

                            /* Bits[25:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DDRT address range's base address. Address
                               [51:26] will be checked against this.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_RANGE_BASE_MS2IDI_MAIN_STRUCT;

/* DDRT_RANGE_LIMIT_MS2IDI_MAIN_REG supported on:                               */
/*      SPRA0 (0x200000f4)                                                      */
/*      SPRB0 (0x200000f4)                                                      */
/*      SPRHBM (0x200000f4)                                                     */
/*      SPRC0 (0x200000f4)                                                      */
/*      SPRMCC (0x200000f4)                                                     */
/*      SPRUCC (0x200000f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Limit of DDRT Address range. Address [51:26] will be compared against this
*/


#define DDRT_RANGE_LIMIT_MS2IDI_MAIN_REG 0x140200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_limit_address : 26;

                            /* Bits[25:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DDRT address range's limit address. Address
                               [51:26] will be checked against this.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_RANGE_LIMIT_MS2IDI_MAIN_STRUCT;

/* DDRT_QOS_CFG1_MS2IDI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200000f8)                                                      */
/*      SPRB0 (0x200000f8)                                                      */
/*      SPRHBM (0x200000f8)                                                     */
/*      SPRC0 (0x200000f8)                                                      */
/*      SPRMCC (0x200000f8)                                                     */
/*      SPRUCC (0x200000f8)                                                     */
/* Register default value on SPRA0: 0x00400020                                  */
/* Register default value on SPRB0: 0x00400020                                  */
/* Register default value on SPRHBM: 0x00400020                                 */
/* Register default value on SPRC0: 0x00400020                                  */
/* Register default value on SPRMCC: 0x00400020                                 */
/* Register default value on SPRUCC: 0x00400020                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DDRT QoS feature configuration1 - contains thresholds to compare number of transactions against before asserting throttle
*/


#define DDRT_QOS_CFG1_MS2IDI_MAIN_REG 0x140200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrtwrthreshold : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000020*/

                            /*
                               Threshold to compare number of DDRT Wr
                               transactions count against
                            */
    UINT32 ddrtthreshold : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000040*/

                            /*
                               Threshold to compare number of all DDRT
                               transactions count against
                            */

  } Bits;
  UINT32 Data;

} DDRT_QOS_CFG1_MS2IDI_MAIN_STRUCT;

/* DDRT_QOS_CFG2_MS2IDI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x200000fc)                                                      */
/*      SPRB0 (0x200000fc)                                                      */
/*      SPRHBM (0x200000fc)                                                     */
/*      SPRC0 (0x200000fc)                                                      */
/*      SPRMCC (0x200000fc)                                                     */
/*      SPRUCC (0x200000fc)                                                     */
/* Register default value on SPRA0: 0x03000100                                  */
/* Register default value on SPRB0: 0x03000100                                  */
/* Register default value on SPRHBM: 0x03000100                                 */
/* Register default value on SPRC0: 0x03000100                                  */
/* Register default value on SPRMCC: 0x03000100                                 */
/* Register default value on SPRUCC: 0x03000100                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DDRT QoS feature configuration2 - contains leakybucket counter window, setting to count either Wr/All trnasactions and feature disable
*/


#define DDRT_QOS_CFG2_MS2IDI_MAIN_REG 0x140200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktwindowcntr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000100*/

                            /*
                               Value to load into the Leaky Bucket Time Window
                               counter- determines the rate at which the
                               Saturating leaky bucket counter leaks
                            */
    UINT32 rsvd : 8;

                            /* Bits[23:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrtwroralltranscount : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               Setting to count either: 1: DDRT Wr transactions
                               0: All DDRT Transactions
                            */
    UINT32 ddrtthdisable : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               Disable for DDRT QoS feature 1: Disabled 0:
                               Enabled
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_QOS_CFG2_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_PREFETCH_BASE_0_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000100)                                                      */
/*      SPRB0 (0x20000100)                                                      */
/*      SPRHBM (0x20000100)                                                     */
/*      SPRC0 (0x20000100)                                                      */
/*      SPRMCC (0x20000100)                                                     */
/*      SPRUCC (0x20000100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT_32_ENTRY_PREFETCH_BASE Configuration Range of IPM memory per SNC Cluster for KNH 1LM/2LM. Configuration Range of DDR memory per SNC Cluster for ICX.
*/


#define XPT_32_ENTRY_PREFETCH_BASE_0_MS2IDI_MAIN_REG 0x14020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_32_entry_prefetch_base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /* xpt_32_entry_prefetch_base [51:30] */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_PREFETCH_BASE_0_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_PREFETCH_BASE_1_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000104)                                                      */
/*      SPRB0 (0x20000104)                                                      */
/*      SPRHBM (0x20000104)                                                     */
/*      SPRC0 (0x20000104)                                                      */
/*      SPRMCC (0x20000104)                                                     */
/*      SPRUCC (0x20000104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT_32_ENTRY_PREFETCH_BASE Configuration Range of IPM memory per SNC Cluster for KNH 1LM/2LM. Configuration Range of DDR memory per SNC Cluster for ICX.
*/


#define XPT_32_ENTRY_PREFETCH_BASE_1_MS2IDI_MAIN_REG 0x14020104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_32_entry_prefetch_base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /* xpt_32_entry_prefetch_base [51:30] */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_PREFETCH_BASE_1_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_PREFETCH_BASE_2_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x20000108)                                                      */
/*      SPRB0 (0x20000108)                                                      */
/*      SPRHBM (0x20000108)                                                     */
/*      SPRC0 (0x20000108)                                                      */
/*      SPRMCC (0x20000108)                                                     */
/*      SPRUCC (0x20000108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT_32_ENTRY_PREFETCH_BASE Configuration Range of IPM memory per SNC Cluster for KNH 1LM/2LM. Configuration Range of DDR memory per SNC Cluster for ICX.
*/


#define XPT_32_ENTRY_PREFETCH_BASE_2_MS2IDI_MAIN_REG 0x14020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_32_entry_prefetch_base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /* xpt_32_entry_prefetch_base [51:30] */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_PREFETCH_BASE_2_MS2IDI_MAIN_STRUCT;

/* XPT_32_ENTRY_PREFETCH_BASE_3_MS2IDI_MAIN_REG supported on:                   */
/*      SPRA0 (0x2000010c)                                                      */
/*      SPRB0 (0x2000010c)                                                      */
/*      SPRHBM (0x2000010c)                                                     */
/*      SPRC0 (0x2000010c)                                                      */
/*      SPRMCC (0x2000010c)                                                     */
/*      SPRUCC (0x2000010c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT_32_ENTRY_PREFETCH_BASE Configuration Range of IPM memory per SNC Cluster for KNH 1LM/2LM. Configuration Range of DDR memory per SNC Cluster for ICX.
*/


#define XPT_32_ENTRY_PREFETCH_BASE_3_MS2IDI_MAIN_REG 0x1402010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_32_entry_prefetch_base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /* xpt_32_entry_prefetch_base [51:30] */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_32_ENTRY_PREFETCH_BASE_3_MS2IDI_MAIN_STRUCT;

/* MBE_MS2IDI_MAIN_REG supported on:                                            */
/*      SPRA0 (0x20000110)                                                      */
/*      SPRB0 (0x20000110)                                                      */
/*      SPRHBM (0x20000110)                                                     */
/*      SPRC0 (0x20000110)                                                      */
/*      SPRMCC (0x20000110)                                                     */
/*      SPRUCC (0x20000110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Memory Bandwidth Enforcement Enhancement Config register
*/


#define MBE_MS2IDI_MAIN_REG 0x14020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llcvictimcntdisable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable counting LLC Victims 1: LLC Victim
                               counting is disabled 0: LLC Victim counting is
                               enabled
                            */
    UINT32 mbe_disable : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable counting All MBE events 1: MBE Event
                               counting is disabled 0: MBE Event counting is
                               enabled
                            */
    UINT32 mbe_directtocore_disable : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable D2C: 1: D2C is disabled, so use PRE to
                               count LLC Misses 0: D2C is enabled, use U2C
                               data.sric to count LLC Misses
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MBE_MS2IDI_MAIN_STRUCT;

/* LOCAL_DDRT_RANGE_BASE_MS2IDI_MAIN_REG supported on:                          */
/*      SPRA0 (0x20000114)                                                      */
/*      SPRB0 (0x20000114)                                                      */
/*      SPRHBM (0x20000114)                                                     */
/*      SPRC0 (0x20000114)                                                      */
/*      SPRMCC (0x20000114)                                                     */
/*      SPRUCC (0x20000114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Local Socket's DDRT range base. Address [51:26] will be compared against this. Used for MBE feature to determine Local socket transactions
*/


#define LOCAL_DDRT_RANGE_BASE_MS2IDI_MAIN_REG 0x14020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_base_address : 26;

                            /* Bits[25:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DDRT address range's base address. Address
                               [51:26] will be checked against this.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCAL_DDRT_RANGE_BASE_MS2IDI_MAIN_STRUCT;

/* XPT_MEM_INTERLEAVE_MS2IDI_MAIN_REG supported on:                             */
/*      SPRA0 (0x2000011c)                                                      */
/*      SPRB0 (0x2000011c)                                                      */
/*      SPRHBM (0x2000011c)                                                     */
/*      SPRC0 (0x2000011c)                                                      */
/*      SPRMCC (0x2000011c)                                                     */
/*      SPRUCC (0x2000011c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Memory Channel Interleave Granularity for XPT. Supported granularities: 64B - N=0, 256B - N=2, 4kB - N=6 in ICX   
*/


#define XPT_MEM_INTERLEAVE_MS2IDI_MAIN_REG 0x1402011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_memory_interleave_granularity : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Memory Channel Interleave Granularity for XPT.
                               Supported granularities: 64B - N=0, 256B - N=2,
                               4kB - N=6 in ICX
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_MEM_INTERLEAVE_MS2IDI_MAIN_STRUCT;

/* DDRT_QOS_CFG3_MS2IDI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20000120)                                                      */
/*      SPRB0 (0x20000120)                                                      */
/*      SPRHBM (0x20000120)                                                     */
/*      SPRC0 (0x20000120)                                                      */
/*      SPRMCC (0x20000120)                                                     */
/*      SPRUCC (0x20000120)                                                     */
/* Register default value on SPRA0: 0x00000050                                  */
/* Register default value on SPRB0: 0x00000050                                  */
/* Register default value on SPRHBM: 0x00000050                                 */
/* Register default value on SPRC0: 0x00000050                                  */
/* Register default value on SPRMCC: 0x00000050                                 */
/* Register default value on SPRUCC: 0x00000050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* DDRT QoS feature configuration3 - contains programmable leaky bucket saturating value
*/


#define DDRT_QOS_CFG3_MS2IDI_MAIN_REG 0x14020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /*
                               Value at which DDRT QOS Leaky Bucket counter
                               Saturates
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_QOS_CFG3_MS2IDI_MAIN_STRUCT;

/* LOCAL_DDRT_RANGE_LIMIT_MS2IDI_MAIN_REG supported on:                         */
/*      SPRA0 (0x20000128)                                                      */
/*      SPRB0 (0x20000128)                                                      */
/*      SPRHBM (0x20000128)                                                     */
/*      SPRC0 (0x20000128)                                                      */
/*      SPRMCC (0x20000128)                                                     */
/*      SPRUCC (0x20000128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Local Socket's DDRT range limit. Address [51:26] will be compared against this. Used for MBE feature to determine Local socket transactions
*/


#define LOCAL_DDRT_RANGE_LIMIT_MS2IDI_MAIN_REG 0x14020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_limit_address : 26;

                            /* Bits[25:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DDRT address range's limit address. Address
                               [51:26] will be checked against this.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCAL_DDRT_RANGE_LIMIT_MS2IDI_MAIN_STRUCT;

/* C2U_DATA_COUNT_MS2IDI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20000130)                                                      */
/*      SPRB0 (0x20000130)                                                      */
/*      SPRHBM (0x20000130)                                                     */
/*      SPRC0 (0x20000130)                                                      */
/*      SPRMCC (0x20000130)                                                     */
/*      SPRUCC (0x20000130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Captures the bits[39:8] of free
running counter that counts
valid C2U data headers  Read
only
*/


#define C2U_DATA_COUNT_MS2IDI_MAIN_REG 0x14020130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data_count : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               captures free running counter[39:8] that counts
                               Data_Headers
                            */

  } Bits;
  UINT32 Data;

} C2U_DATA_COUNT_MS2IDI_MAIN_STRUCT;

/* U2C_DATA_COUNT_MS2IDI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20000140)                                                      */
/*      SPRB0 (0x20000140)                                                      */
/*      SPRHBM (0x20000140)                                                     */
/*      SPRC0 (0x20000140)                                                      */
/*      SPRMCC (0x20000140)                                                     */
/*      SPRUCC (0x20000140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Captures the bits[39:8] of free
running counter that counts
valid C2U data headers  Read
only
*/


#define U2C_DATA_COUNT_MS2IDI_MAIN_REG 0x14020140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data_count : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               captures free running counter[39:8] that counts
                               Data_Headers
                            */

  } Bits;
  UINT32 Data;

} U2C_DATA_COUNT_MS2IDI_MAIN_STRUCT;

/* XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_REG supported on:                         */
/*      SPRA0 (0x20000144)                                                      */
/*      SPRB0 (0x20000144)                                                      */
/*      SPRHBM (0x20000144)                                                     */
/*      SPRC0 (0x20000144)                                                      */
/*      SPRMCC (0x20000144)                                                     */
/*      SPRUCC (0x20000144)                                                     */
/* Register default value on SPRA0: 0x00040141                                  */
/* Register default value on SPRB0: 0x00040141                                  */
/* Register default value on SPRHBM: 0x00040141                                 */
/* Register default value on SPRC0: 0x00040141                                  */
/* Register default value on SPRMCC: 0x00040141                                 */
/* Register default value on SPRUCC: 0x00040141                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT 10nm Enhancement- FIFO and Credit tracker config
*/


#define XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_REG 0x14020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xpt_fifo_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* XPT FIFO Enable. */
    UINT32 rsvd : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xpt_fifo_window_ctr : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000004*/

                            /*
                               XPT FIFO Shift enable - time window. When this
                               window expires, FIFO will shift and drop oldest
                               XPT Value can be between 7 and 0. The logic is
                               designed in a way that when this value is
                               programed 1, it actually behaves like a 0. i.e.
                               drops packet every cycle A programed value of 0
                               leads to a packet being dropped every 8 cycles.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xpt_num_enabled_credits : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000001*/

                            /*
                               XPT FIFO Number of enabled Credits. Programmable
                               upto 4. Minimum value= 1 if XPT is enabled
                            */
    UINT32 rsvd_11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xpt_num_fifo_entries_enabled : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000004*/

                            /*
                               Num of enabled entries in XPT FIFO. Should be
                               programmed to minimum 1 if XPT FIFO is enabled
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_FIFO_CREDIT_CONFIG_MS2IDI_MAIN_STRUCT;

/* TWOLM_QOS_CONFIG1_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x20000154)                                                      */
/*      SPRB0 (0x20000154)                                                      */
/*      SPRHBM (0x20000154)                                                     */
/*      SPRC0 (0x20000154)                                                      */
/*      SPRMCC (0x20000154)                                                     */
/*      SPRUCC (0x20000154)                                                     */
/* Register default value on SPRA0: 0x01000050                                  */
/* Register default value on SPRB0: 0x01000050                                  */
/* Register default value on SPRHBM: 0x01000050                                 */
/* Register default value on SPRC0: 0x01000050                                  */
/* Register default value on SPRMCC: 0x01000050                                 */
/* Register default value on SPRUCC: 0x01000050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains LeakyBkt Counter Sat Value and Time Window Counter Value
*/


#define TWOLM_QOS_CONFIG1_MS2IDI_MAIN_REG 0x14020154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /* Saturation Value for 2LM Leakybkt Counter */
    UINT32 leakybktwindowcntr : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000100*/

                            /* Time window cntr Value for 2LM LeakyBkt Cntr */

  } Bits;
  UINT32 Data;

} TWOLM_QOS_CONFIG1_MS2IDI_MAIN_STRUCT;

/* TWOLM_QOS_CONFIG2_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x20000164)                                                      */
/*      SPRB0 (0x20000164)                                                      */
/*      SPRHBM (0x20000164)                                                     */
/*      SPRC0 (0x20000164)                                                      */
/*      SPRMCC (0x20000164)                                                     */
/*      SPRUCC (0x20000164)                                                     */
/* Register default value on SPRA0: 0x00001040                                  */
/* Register default value on SPRB0: 0x00001040                                  */
/* Register default value on SPRHBM: 0x00001040                                 */
/* Register default value on SPRC0: 0x00001040                                  */
/* Register default value on SPRMCC: 0x00001040                                 */
/* Register default value on SPRUCC: 0x00001040                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains 2LM Threshold, Low_Threshold and ThDisable bit
*/


#define TWOLM_QOS_CONFIG2_MS2IDI_MAIN_REG 0x14020164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 threshold : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000040*/

                            /*
                               Threshold value above which 2LMDDRTThExceeded to
                               CMS is asserted
                            */
    UINT32 low_threshold : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000010*/

                            /*
                               Low Threshold below which all distress detection
                               bits are cleared
                            */
    UINT32 thdisable : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bit to defeature 2LMDDRThExceeded assertion to
                               CMS
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TWOLM_QOS_CONFIG2_MS2IDI_MAIN_STRUCT;

/* DEDUP_CONFIG1_MS2IDI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x20000168)                                                      */
/*      SPRB0 (0x20000168)                                                      */
/*      SPRHBM (0x20000168)                                                     */
/*      SPRC0 (0x20000168)                                                      */
/*      SPRMCC (0x20000168)                                                     */
/*      SPRUCC (0x20000168)                                                     */
/* Register default value on SPRA0: 0x01000050                                  */
/* Register default value on SPRB0: 0x01000050                                  */
/* Register default value on SPRHBM: 0x01000050                                 */
/* Register default value on SPRC0: 0x01000050                                  */
/* Register default value on SPRMCC: 0x01000050                                 */
/* Register default value on SPRUCC: 0x01000050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains LeakyBkt Counter Sat Value and Time Window Counter Value
*/


#define DEDUP_CONFIG1_MS2IDI_MAIN_REG 0x14020168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /* Saturation Value for Dedup Leakybkt Counter */
    UINT32 leakybktwindowcntr : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000100*/

                            /* Time window cntr Value for Dedup LeakyBkt Cntr */

  } Bits;
  UINT32 Data;

} DEDUP_CONFIG1_MS2IDI_MAIN_STRUCT;

/* DEDUP_CONFIG2_MS2IDI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x2000016c)                                                      */
/*      SPRB0 (0x2000016c)                                                      */
/*      SPRHBM (0x2000016c)                                                     */
/*      SPRC0 (0x2000016c)                                                      */
/*      SPRMCC (0x2000016c)                                                     */
/*      SPRUCC (0x2000016c)                                                     */
/* Register default value on SPRA0: 0x001F007F                                  */
/* Register default value on SPRB0: 0x001F007F                                  */
/* Register default value on SPRHBM: 0x001F007F                                 */
/* Register default value on SPRC0: 0x001F007F                                  */
/* Register default value on SPRMCC: 0x001F007F                                 */
/* Register default value on SPRUCC: 0x001F007F                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains Dedup upper Threshold, Low_Threshold 
*/


#define DEDUP_CONFIG2_MS2IDI_MAIN_REG 0x1402016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 up_threshold : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x0000007F*/

                            /*
                               Upper Threshold against which the DeDup
                               saturating counter is compared and the
                               DedupThExceeded signal is asserted based on
                               that, i.e. if the counter is greater than this
                               threshold, the exceeded signal is asserted to
                               CMS. Once the signal is asserted to CMS, CMS
                               will Throttle injection onto the MESH. Up
                               Threshold value must be greater than low
                               treshold.
                            */
    UINT32 low_threshold : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x0000001F*/

                            /*
                               Programmable Low Threshold against which the
                               DeDup saturating counter is compared and the
                               DedupThExceeded signal is de-asserted based on
                               that, i.e. if the counter is less than this
                               threshold, the exceeded signal is de-asserted.
                               If the counter value is between the upper and
                               lower thresholds, the exceeded signal remains
                               unchanged The exceed signal falling below low
                               threshold will cause CMS to stop throttling
                               traffic onto the MESH. Low Treshold value must
                               be less than Up threshold value.
                            */

  } Bits;
  UINT32 Data;

} DEDUP_CONFIG2_MS2IDI_MAIN_STRUCT;

/* CR_MBE_CONFIG1_MS2IDI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20000170)                                                      */
/*      SPRB0 (0x20000170)                                                      */
/*      SPRHBM (0x20000170)                                                     */
/*      SPRC0 (0x20000170)                                                      */
/*      SPRMCC (0x20000170)                                                     */
/*      SPRUCC (0x20000170)                                                     */
/* Register default value on SPRA0: 0x01000050                                  */
/* Register default value on SPRB0: 0x01000050                                  */
/* Register default value on SPRHBM: 0x01000050                                 */
/* Register default value on SPRC0: 0x01000050                                  */
/* Register default value on SPRMCC: 0x01000050                                 */
/* Register default value on SPRUCC: 0x01000050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains LeakyBkt Counter Sat Value and Time Window Counter Value
*/


#define CR_MBE_CONFIG1_MS2IDI_MAIN_REG 0x14020170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /* Saturation Value for CR MBE Leakybkt Counter */
    UINT32 leakybktwindowcntr : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000100*/

                            /* Time window cntr Value for CR MBE LeakyBkt Cntr */

  } Bits;
  UINT32 Data;

} CR_MBE_CONFIG1_MS2IDI_MAIN_STRUCT;

/* CR_MBE_CONFIG2_MS2IDI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20000174)                                                      */
/*      SPRB0 (0x20000174)                                                      */
/*      SPRHBM (0x20000174)                                                     */
/*      SPRC0 (0x20000174)                                                      */
/*      SPRMCC (0x20000174)                                                     */
/*      SPRUCC (0x20000174)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains thresholds
*/


#define CR_MBE_CONFIG2_MS2IDI_MAIN_REG 0x14020174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 threshold : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000040*/

                            /*
                               Threshold value above which 2LMDDRTThExceeded to
                               CMS is asserted
                            */
    UINT32 thdisable : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bit to defeature MBE CR. If this bit is set, MBE
                               CR is disabled, else enabled
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CR_MBE_CONFIG2_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000178)                                                      */
/*      SPRB0 (0x20000178)                                                      */
/*      SPRHBM (0x20000178)                                                     */
/*      SPRC0 (0x20000178)                                                      */
/*      SPRMCC (0x20000178)                                                     */
/*      SPRUCC (0x20000178)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_REG 0x14020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 generate_rmt_prefetch : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* generate rmt prefetch */
    UINT32 upi_id : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               UPI link ID. Needs to be looked up in the LUT
                               for mesh parameters
                            */
    UINT32 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_limit : 22;

                            /* Bits[29:8], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket limit for Remote XPT prefetchable
                               range check
                            */
    UINT32 dnode_id : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_0_N0_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_0_N1_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x2000017c)                                                      */
/*      SPRB0 (0x2000017c)                                                      */
/*      SPRHBM (0x2000017c)                                                     */
/*      SPRC0 (0x2000017c)                                                      */
/*      SPRMCC (0x2000017c)                                                     */
/*      SPRUCC (0x2000017c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_0_N1_MS2IDI_MAIN_REG 0x1402017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dnode_id : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_base : 22;

                            /* Bits[25:4], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket base for Remote XPT prefetchable
                               range check
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_0_N1_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_1_N0_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000180)                                                      */
/*      SPRB0 (0x20000180)                                                      */
/*      SPRHBM (0x20000180)                                                     */
/*      SPRC0 (0x20000180)                                                      */
/*      SPRMCC (0x20000180)                                                     */
/*      SPRUCC (0x20000180)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_1_N0_MS2IDI_MAIN_REG 0x14020180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 generate_rmt_prefetch : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* generate rmt prefetch */
    UINT32 upi_id : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               UPI link ID. Needs to be looked up in the LUT
                               for mesh parameters
                            */
    UINT32 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_limit : 22;

                            /* Bits[29:8], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket limit for Remote XPT prefetchable
                               range check
                            */
    UINT32 dnode_id : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_1_N0_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_1_N1_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000184)                                                      */
/*      SPRB0 (0x20000184)                                                      */
/*      SPRHBM (0x20000184)                                                     */
/*      SPRC0 (0x20000184)                                                      */
/*      SPRMCC (0x20000184)                                                     */
/*      SPRUCC (0x20000184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_1_N1_MS2IDI_MAIN_REG 0x14020184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dnode_id : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_base : 22;

                            /* Bits[25:4], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket base for Remote XPT prefetchable
                               range check
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_1_N1_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_2_N0_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000188)                                                      */
/*      SPRB0 (0x20000188)                                                      */
/*      SPRHBM (0x20000188)                                                     */
/*      SPRC0 (0x20000188)                                                      */
/*      SPRMCC (0x20000188)                                                     */
/*      SPRUCC (0x20000188)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_2_N0_MS2IDI_MAIN_REG 0x14020188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 generate_rmt_prefetch : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* generate rmt prefetch */
    UINT32 upi_id : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               UPI link ID. Needs to be looked up in the LUT
                               for mesh parameters
                            */
    UINT32 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_limit : 22;

                            /* Bits[29:8], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket limit for Remote XPT prefetchable
                               range check
                            */
    UINT32 dnode_id : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_2_N0_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_2_N1_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x2000018c)                                                      */
/*      SPRB0 (0x2000018c)                                                      */
/*      SPRHBM (0x2000018c)                                                     */
/*      SPRC0 (0x2000018c)                                                      */
/*      SPRMCC (0x2000018c)                                                     */
/*      SPRUCC (0x2000018c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_2_N1_MS2IDI_MAIN_REG 0x1402018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dnode_id : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_base : 22;

                            /* Bits[25:4], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket base for Remote XPT prefetchable
                               range check
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_2_N1_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_3_N0_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000190)                                                      */
/*      SPRB0 (0x20000190)                                                      */
/*      SPRHBM (0x20000190)                                                     */
/*      SPRC0 (0x20000190)                                                      */
/*      SPRMCC (0x20000190)                                                     */
/*      SPRUCC (0x20000190)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_3_N0_MS2IDI_MAIN_REG 0x14020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 generate_rmt_prefetch : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* generate rmt prefetch */
    UINT32 upi_id : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               UPI link ID. Needs to be looked up in the LUT
                               for mesh parameters
                            */
    UINT32 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_limit : 22;

                            /* Bits[29:8], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket limit for Remote XPT prefetchable
                               range check
                            */
    UINT32 dnode_id : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_3_N0_MS2IDI_MAIN_STRUCT;

/* XPT_UPI_DECODE_TABLE_3_N1_MS2IDI_MAIN_REG supported on:                      */
/*      SPRA0 (0x20000194)                                                      */
/*      SPRB0 (0x20000194)                                                      */
/*      SPRHBM (0x20000194)                                                     */
/*      SPRC0 (0x20000194)                                                      */
/*      SPRMCC (0x20000194)                                                     */
/*      SPRUCC (0x20000194)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* XPT UPI DECODE TABLE.  Each entry has Base[51:30], Limit[51:30] and 3bits with format: {UPI_ID[1:0], Generate_Prefetch}
*/


#define XPT_UPI_DECODE_TABLE_3_N1_MS2IDI_MAIN_REG 0x14020194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dnode_id : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Destination Node ID of remote socket */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remote_socket_base : 22;

                            /* Bits[25:4], Access Type=RW, default=0x00000000*/

                            /*
                               Remote socket base for Remote XPT prefetchable
                               range check
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XPT_UPI_DECODE_TABLE_3_N1_MS2IDI_MAIN_STRUCT;

/* MS2IDI_SPARE_MS2IDI_MAIN_REG supported on:                                   */
/*      SPRA0 (0x200001d0)                                                      */
/*      SPRB0 (0x200001d0)                                                      */
/*      SPRHBM (0x200001d0)                                                     */
/*      SPRC0 (0x200001d0)                                                      */
/*      SPRMCC (0x200001d0)                                                     */
/*      SPRUCC (0x200001d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains the Register which is a spare CSR in case required for any defeature. Added in ICX B0 time frame.
*/


#define MS2IDI_SPARE_MS2IDI_MAIN_REG 0x140201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Spare field 0 if required for any defeature
                               needs
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MS2IDI_SPARE_MS2IDI_MAIN_STRUCT;

/* LEAKY_BUCKET_CR_PTMBE_MS2IDI_MAIN_REG supported on:                          */
/*      SPRA0 (0x200001d4)                                                      */
/*      SPRB0 (0x200001d4)                                                      */
/*      SPRHBM (0x200001d4)                                                     */
/*      SPRC0 (0x200001d4)                                                      */
/*      SPRMCC (0x200001d4)                                                     */
/*      SPRUCC (0x200001d4)                                                     */
/* Register default value on SPRA0: 0x00500050                                  */
/* Register default value on SPRB0: 0x00500050                                  */
/* Register default value on SPRHBM: 0x00500050                                 */
/* Register default value on SPRC0: 0x00500050                                  */
/* Register default value on SPRMCC: 0x00500050                                 */
/* Register default value on SPRUCC: 0x00500050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Leaky bucket threshold for CR PTMBE threads 0 and 1
*/


#define LEAKY_BUCKET_CR_PTMBE_MS2IDI_MAIN_REG 0x140201D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue_0 : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for CR MBE Leakybkt Counter for
                               thread 0
                            */
    UINT32 leakybktsatvalue_1 : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for CR MBE Leakybkt Counter for
                               thread 1
                            */

  } Bits;
  UINT32 Data;

} LEAKY_BUCKET_CR_PTMBE_MS2IDI_MAIN_STRUCT;

/* LEAKY_BUCKET_DDR_PTMBE_MS2IDI_MAIN_REG supported on:                         */
/*      SPRA0 (0x200001d8)                                                      */
/*      SPRB0 (0x200001d8)                                                      */
/*      SPRHBM (0x200001d8)                                                     */
/*      SPRC0 (0x200001d8)                                                      */
/*      SPRMCC (0x200001d8)                                                     */
/*      SPRUCC (0x200001d8)                                                     */
/* Register default value on SPRA0: 0x00500050                                  */
/* Register default value on SPRB0: 0x00500050                                  */
/* Register default value on SPRHBM: 0x00500050                                 */
/* Register default value on SPRC0: 0x00500050                                  */
/* Register default value on SPRMCC: 0x00500050                                 */
/* Register default value on SPRUCC: 0x00500050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Leaky bucket threshold for DDR PTMBE threads 0 and 1
*/


#define LEAKY_BUCKET_DDR_PTMBE_MS2IDI_MAIN_REG 0x140201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue_0 : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for DDR MBE Leakybkt Counter
                               for thread0
                            */
    UINT32 leakybktsatvalue_1 : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for DDR MBE Leakybkt Counter
                               for thread 1
                            */

  } Bits;
  UINT32 Data;

} LEAKY_BUCKET_DDR_PTMBE_MS2IDI_MAIN_STRUCT;

/* LEAKY_BUCKET_CR_DIST_MS2IDI_MAIN_REG supported on:                           */
/*      SPRA0 (0x200001dc)                                                      */
/*      SPRB0 (0x200001dc)                                                      */
/*      SPRHBM (0x200001dc)                                                     */
/*      SPRC0 (0x200001dc)                                                      */
/*      SPRMCC (0x200001dc)                                                     */
/*      SPRUCC (0x200001dc)                                                     */
/* Register default value on SPRA0: 0x00500050                                  */
/* Register default value on SPRB0: 0x00500050                                  */
/* Register default value on SPRHBM: 0x00500050                                 */
/* Register default value on SPRC0: 0x00500050                                  */
/* Register default value on SPRMCC: 0x00500050                                 */
/* Register default value on SPRUCC: 0x00500050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Leaky bucket threshold and time window for CR distress
*/


#define LEAKY_BUCKET_CR_DIST_MS2IDI_MAIN_REG 0x140201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for CR distress Leakybkt
                               Counter
                            */
    UINT32 leakybkttimewindowctr : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000050*/

                            /*
                               Saturation Value for CR distress Leakybkt time
                               window counter
                            */

  } Bits;
  UINT32 Data;

} LEAKY_BUCKET_CR_DIST_MS2IDI_MAIN_STRUCT;

/* PTMBE_DEFEATURE_MS2IDI_MAIN_REG supported on:                                */
/*      SPRA0 (0x200001e0)                                                      */
/*      SPRB0 (0x200001e0)                                                      */
/*      SPRHBM (0x200001e0)                                                     */
/*      SPRC0 (0x200001e0)                                                      */
/*      SPRMCC (0x200001e0)                                                     */
/*      SPRUCC (0x200001e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PTMBE defeature bit
*/


#define PTMBE_DEFEATURE_MS2IDI_MAIN_REG 0x140201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ptmbe_defeature : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Per thread MBE defeature bit . If this bit is
                               set, the MBE implementation will fall back from
                               the per thread mode to the legacy per core MBE
                               in wave 1
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PTMBE_DEFEATURE_MS2IDI_MAIN_STRUCT;

/* TWOLM_QOS_CONFIG5_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x20000200)                                                      */
/*      SPRB0 (0x20000200)                                                      */
/*      SPRHBM (0x20000200)                                                     */
/*      SPRC0 (0x20000200)                                                      */
/*      SPRMCC (0x20000200)                                                     */
/*      SPRUCC (0x20000200)                                                     */
/* Register default value on SPRA0: 0x01000050                                  */
/* Register default value on SPRB0: 0x01000050                                  */
/* Register default value on SPRHBM: 0x01000050                                 */
/* Register default value on SPRC0: 0x01000050                                  */
/* Register default value on SPRMCC: 0x01000050                                 */
/* Register default value on SPRUCC: 0x01000050                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains Remote Transaction counting LeakyBkt Counter Sat Value and Time Window Counter Value
*/


#define TWOLM_QOS_CONFIG5_MS2IDI_MAIN_REG 0x14020200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 leakybktsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000050*/

                            /* Saturation Value for Remote 2LM Leakybkt Counter */
    UINT32 leakybktwindowcntr : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000100*/

                            /*
                               Remote Time window counter Value for 2LM
                               LeakyBkt Counter
                            */

  } Bits;
  UINT32 Data;

} TWOLM_QOS_CONFIG5_MS2IDI_MAIN_STRUCT;

/* TWOLM_QOS_CONFIG6_MS2IDI_MAIN_REG supported on:                              */
/*      SPRA0 (0x20000220)                                                      */
/*      SPRB0 (0x20000220)                                                      */
/*      SPRHBM (0x20000220)                                                     */
/*      SPRC0 (0x20000220)                                                      */
/*      SPRMCC (0x20000220)                                                     */
/*      SPRUCC (0x20000220)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains 2LM Threshold for remote transaction counting leaky bucket
*/


#define TWOLM_QOS_CONFIG6_MS2IDI_MAIN_REG 0x14020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 threshold : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000040*/

                            /*
                               Threshold value above which 2LMDDRTThExceeded
                               for Remote transactions is asserted
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TWOLM_QOS_CONFIG6_MS2IDI_MAIN_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* QOS_2LM_CONFIG3_MS2IDI_MAIN_REG supported on:                                */
/*      SPRB0 (0x20000240)                                                      */
/*      SPRHBM (0x20000240)                                                     */
/*      SPRC0 (0x20000240)                                                      */
/*      SPRMCC (0x20000240)                                                     */
/*      SPRUCC (0x20000240)                                                     */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains the defeature bit for message based vs wire based signalling
*/


#define QOS_2LM_CONFIG3_MS2IDI_MAIN_REG 0x14020240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gbl_distress_sig_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               When this bit is set, ms2idi looks at the global
                               distress signal and not the message based
                               distress.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QOS_2LM_CONFIG3_MS2IDI_MAIN_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* QOS_2LM_CONFIG4_1_MS2IDI_MAIN_REG supported on:                              */
/*      SPRB0 (0x20000260)                                                      */
/*      SPRHBM (0x20000260)                                                     */
/*      SPRC0 (0x20000260)                                                      */
/*      SPRMCC (0x20000260)                                                     */
/*      SPRUCC (0x20000260)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains the bit vector defining distress source for distress wire 1 of wires zero and one
*/


#define QOS_2LM_CONFIG4_1_MS2IDI_MAIN_REG 0x14020260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gbl_distress_sig_en : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit vector indicates what source ID has
                               asserted the distress wire associated with this
                               bit vector. The bits starting from MSBs indicate
                               MC IDs and the bits starting from LSBs indicate
                               UPI IDs. For eg, 1111000000001111 represents
                               MC0,MC1,MC2,MC3 and UPI3, UPI2,UPI1 and UPI0
                               (each of the 1s starting from MSB)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QOS_2LM_CONFIG4_1_MS2IDI_MAIN_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* QOS_2LM_CONFIG4_0_MS2IDI_MAIN_REG supported on:                              */
/*      SPRB0 (0x20000280)                                                      */
/*      SPRHBM (0x20000280)                                                     */
/*      SPRC0 (0x20000280)                                                      */
/*      SPRMCC (0x20000280)                                                     */
/*      SPRUCC (0x20000280)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRHBM Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRC0 Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI           */
/* SPRMCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* SPRUCC Register File:    sprsp_top/ms2idi0[0]/MS2IDI_TOP/MEM_MS2IDI          */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains the bit vector defining distress source for distress wire 0 of wires zero and one
*/


#define QOS_2LM_CONFIG4_0_MS2IDI_MAIN_REG 0x14020280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gbl_distress_sig_en : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit vector indicates what source ID has
                               asserted the distress wire associated with this
                               bit vector. The bits starting from MSBs indicate
                               MC IDs and the bits starting from LSBs indicate
                               UPI IDs. For eg, 1111000000001111 represents
                               MC0,MC1,MC2,MC3 and UPI3, UPI2,UPI1 and UPI0
                               (each of the 1s starting from MSB)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QOS_2LM_CONFIG4_0_MS2IDI_MAIN_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */
#endif /* _MS2IDI_MAIN_h */
