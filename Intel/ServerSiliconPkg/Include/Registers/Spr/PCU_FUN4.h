
/** @file
  PCU_FUN4.h

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
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* BIOS_W                                                                       */
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
/* PUNIT_OS_W                                                                   */
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
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | DFX_THIRDPARTY_SAI | GT_SAI | GT2_SAI | */
/*    DEVICE_UNTRUSTED_IAL_SAI | DEVICE_ABORT_SAI                               */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* P_U_CODE_W                                                                   */
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
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _PCU_FUN4_h
#define _PCU_FUN4_h
#include <Base.h>

/* VID_1_30_4_PCU_FUN4_REG supported on:                                        */
/*      SPRA0 (0x11ff4000)                                                      */
/*      SPRB0 (0x11ff4000)                                                      */
/*      SPRHBM (0x11ff4000)                                                     */
/*      SPRC0 (0x11ff4000)                                                      */
/*      SPRMCC (0x11ff4000)                                                     */
/*      SPRUCC (0x11ff4000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Vendor ID Register
*/


#define VID_1_30_4_PCU_FUN4_REG 0x04410000

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

} VID_1_30_4_PCU_FUN4_STRUCT;

/* DID_1_30_4_PCU_FUN4_REG supported on:                                        */
/*      SPRA0 (0x11ff4002)                                                      */
/*      SPRB0 (0x11ff4002)                                                      */
/*      SPRHBM (0x11ff4002)                                                     */
/*      SPRC0 (0x11ff4002)                                                      */
/*      SPRMCC (0x11ff4002)                                                     */
/*      SPRUCC (0x11ff4002)                                                     */
/* Register default value on SPRA0: 0x0000325C                                  */
/* Register default value on SPRB0: 0x0000325C                                  */
/* Register default value on SPRHBM: 0x0000325C                                 */
/* Register default value on SPRC0: 0x0000325C                                  */
/* Register default value on SPRMCC: 0x0000325C                                 */
/* Register default value on SPRUCC: 0x0000325C                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Device Identification Number
*/


#define DID_1_30_4_PCU_FUN4_REG 0x04410002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000325C*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_30_4_PCU_FUN4_STRUCT;

/* PCICMD_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x11ff4004)                                                      */
/*      SPRB0 (0x11ff4004)                                                      */
/*      SPRHBM (0x11ff4004)                                                     */
/*      SPRC0 (0x11ff4004)                                                      */
/*      SPRMCC (0x11ff4004)                                                     */
/*      SPRUCC (0x11ff4004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Command Register
*/


#define PCICMD_1_30_4_PCU_FUN4_REG 0x04410004

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

} PCICMD_1_30_4_PCU_FUN4_STRUCT;

/* PCISTS_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x11ff4006)                                                      */
/*      SPRB0 (0x11ff4006)                                                      */
/*      SPRHBM (0x11ff4006)                                                     */
/*      SPRC0 (0x11ff4006)                                                      */
/*      SPRMCC (0x11ff4006)                                                     */
/*      SPRUCC (0x11ff4006)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Status Register
*/


#define PCISTS_1_30_4_PCU_FUN4_REG 0x04410006

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

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

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

} PCISTS_1_30_4_PCU_FUN4_STRUCT;

/* RID_CCR_1_30_4_PCU_FUN4_REG supported on:                                    */
/*      SPRA0 (0x21ff4008)                                                      */
/*      SPRB0 (0x21ff4008)                                                      */
/*      SPRHBM (0x21ff4008)                                                     */
/*      SPRC0 (0x21ff4008)                                                      */
/*      SPRMCC (0x21ff4008)                                                     */
/*      SPRUCC (0x21ff4008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_30_4_PCU_FUN4_REG 0x04420008

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

} RID_CCR_1_30_4_PCU_FUN4_STRUCT;

/* CLSR_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x1ff400c)                                                       */
/*      SPRB0 (0x1ff400c)                                                       */
/*      SPRHBM (0x1ff400c)                                                      */
/*      SPRC0 (0x1ff400c)                                                       */
/*      SPRMCC (0x1ff400c)                                                      */
/*      SPRUCC (0x1ff400c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Cache Line Size Register
*/


#define CLSR_1_30_4_PCU_FUN4_REG 0x0440000C

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

} CLSR_1_30_4_PCU_FUN4_STRUCT;

/* PLAT_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x1ff400d)                                                       */
/*      SPRB0 (0x1ff400d)                                                       */
/*      SPRHBM (0x1ff400d)                                                      */
/*      SPRC0 (0x1ff400d)                                                       */
/*      SPRMCC (0x1ff400d)                                                      */
/*      SPRUCC (0x1ff400d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Latency Timer
*/


#define PLAT_1_30_4_PCU_FUN4_REG 0x0440000D

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

} PLAT_1_30_4_PCU_FUN4_STRUCT;

/* HDR_1_30_4_PCU_FUN4_REG supported on:                                        */
/*      SPRA0 (0x1ff400e)                                                       */
/*      SPRB0 (0x1ff400e)                                                       */
/*      SPRHBM (0x1ff400e)                                                      */
/*      SPRC0 (0x1ff400e)                                                       */
/*      SPRMCC (0x1ff400e)                                                      */
/*      SPRUCC (0x1ff400e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Header Type
*/


#define HDR_1_30_4_PCU_FUN4_REG 0x0440000E

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

} HDR_1_30_4_PCU_FUN4_STRUCT;

/* BIST_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x1ff400f)                                                       */
/*      SPRB0 (0x1ff400f)                                                       */
/*      SPRHBM (0x1ff400f)                                                      */
/*      SPRC0 (0x1ff400f)                                                       */
/*      SPRMCC (0x1ff400f)                                                      */
/*      SPRUCC (0x1ff400f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI BIST Register
*/


#define BIST_1_30_4_PCU_FUN4_REG 0x0440000F

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

} BIST_1_30_4_PCU_FUN4_STRUCT;

/* SVID_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x11ff402c)                                                      */
/*      SPRB0 (0x11ff402c)                                                      */
/*      SPRHBM (0x11ff402c)                                                     */
/*      SPRC0 (0x11ff402c)                                                      */
/*      SPRMCC (0x11ff402c)                                                     */
/*      SPRUCC (0x11ff402c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem Vendor ID Register
*/


#define SVID_1_30_4_PCU_FUN4_REG 0x0441002C

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

} SVID_1_30_4_PCU_FUN4_STRUCT;

/* SDID_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x11ff402e)                                                      */
/*      SPRB0 (0x11ff402e)                                                      */
/*      SPRHBM (0x11ff402e)                                                     */
/*      SPRC0 (0x11ff402e)                                                      */
/*      SPRMCC (0x11ff402e)                                                     */
/*      SPRUCC (0x11ff402e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem ID Register
*/


#define SDID_1_30_4_PCU_FUN4_REG 0x0441002E

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

} SDID_1_30_4_PCU_FUN4_STRUCT;

/* CAPPTR_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x1ff4034)                                                       */
/*      SPRB0 (0x1ff4034)                                                       */
/*      SPRHBM (0x1ff4034)                                                      */
/*      SPRC0 (0x1ff4034)                                                       */
/*      SPRMCC (0x1ff4034)                                                      */
/*      SPRUCC (0x1ff4034)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_30_4_PCU_FUN4_REG 0x04400034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Points to the first capability structure for the
                               device which is the PCIe capability.
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_1_30_4_PCU_FUN4_STRUCT;

/* INTL_1_30_4_PCU_FUN4_REG supported on:                                       */
/*      SPRA0 (0x1ff403c)                                                       */
/*      SPRB0 (0x1ff403c)                                                       */
/*      SPRHBM (0x1ff403c)                                                      */
/*      SPRC0 (0x1ff403c)                                                       */
/*      SPRMCC (0x1ff403c)                                                      */
/*      SPRUCC (0x1ff403c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Line Register
*/


#define INTL_1_30_4_PCU_FUN4_REG 0x0440003C

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

} INTL_1_30_4_PCU_FUN4_STRUCT;

/* INTPIN_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x1ff403d)                                                       */
/*      SPRB0 (0x1ff403d)                                                       */
/*      SPRHBM (0x1ff403d)                                                      */
/*      SPRC0 (0x1ff403d)                                                       */
/*      SPRMCC (0x1ff403d)                                                      */
/*      SPRUCC (0x1ff403d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_30_4_PCU_FUN4_REG 0x0440003D

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

} INTPIN_1_30_4_PCU_FUN4_STRUCT;

/* MINGNT_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x1ff403e)                                                       */
/*      SPRB0 (0x1ff403e)                                                       */
/*      SPRHBM (0x1ff403e)                                                      */
/*      SPRC0 (0x1ff403e)                                                       */
/*      SPRMCC (0x1ff403e)                                                      */
/*      SPRUCC (0x1ff403e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Min Grant Register
*/


#define MINGNT_1_30_4_PCU_FUN4_REG 0x0440003E

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

} MINGNT_1_30_4_PCU_FUN4_STRUCT;

/* MAXLAT_1_30_4_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x1ff403f)                                                       */
/*      SPRB0 (0x1ff403f)                                                       */
/*      SPRHBM (0x1ff403f)                                                      */
/*      SPRC0 (0x1ff403f)                                                       */
/*      SPRMCC (0x1ff403f)                                                      */
/*      SPRUCC (0x1ff403f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Max Latency Register
*/


#define MAXLAT_1_30_4_PCU_FUN4_REG 0x0440003F

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

} MAXLAT_1_30_4_PCU_FUN4_STRUCT;

/* GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG supported on:                      */
/*      SPRA0 (0x21ff4080)                                                      */
/*      SPRB0 (0x21ff4080)                                                      */
/*      SPRHBM (0x21ff4080)                                                     */
/*      SPRC0 (0x21ff4080)                                                      */
/*      SPRMCC (0x21ff4080)                                                     */
/*      SPRUCC (0x21ff4080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register is in the PCU CR space.  It contains NID information for sockets 4-7 in the platform.  Expectation is that BIOS will write this register during the Reset/Init flow.
*/


#define GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG 0x04420080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 skt4_nid : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Socket4 NID */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt5_nid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /* Socket5 NID */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt6_nid : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Socket6 NID */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt7_nid : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* Socket7 NID */
    UINT32 rsvd_15 : 13;

                            /* Bits[27:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt_valid : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               Valid bits indicating whether NID has been
                               programmed by BIOS. If bit is 0 after the
                               CST/SST ready bit is set, then it implies that
                               the socket is not populated.
                            */

  } Bits;
  UINT32 Data;

} GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_STRUCT;

/* VIRAL_CONTROL_PCU_FUN4_REG supported on:                                     */
/*      SPRA0 (0x21ff4084)                                                      */
/*      SPRB0 (0x21ff4084)                                                      */
/*      SPRHBM (0x21ff4084)                                                     */
/*      SPRC0 (0x21ff4084)                                                      */
/*      SPRMCC (0x21ff4084)                                                     */
/*      SPRUCC (0x21ff4084)                                                     */
/* Register default value on SPRA0: 0x00000700                                  */
/* Register default value on SPRB0: 0x00000700                                  */
/* Register default value on SPRHBM: 0x00000700                                 */
/* Register default value on SPRC0: 0x00000700                                  */
/* Register default value on SPRMCC: 0x00000700                                 */
/* Register default value on SPRUCC: 0x00000700                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Controls how PCU responds to viral and eMCA signalling.
      Note: This register must be programmed for eMCA mode even if viral is not enabled.
*/


#define VIRAL_CONTROL_PCU_FUN4_REG 0x04420084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 viral_reset : 1;

                            /* Bits[0:0], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               Clears the CATERR#/MSMI# pin after a viral is
                               signalled.
                            */
    UINT32 viral_fast_pin_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disables the signaling of IERR/MSMI
                               based on the global fatal signal. CATERR#
                               assertion dependent on message (MCERR/IERR)
                               received over message channel. MSMI# assertion
                               dependent on message received over message
                               channel. Values: 0 -> IERR signaling by global
                               fatal is enabled. In viral condition,
                               CATERR#/MSMI# will assert always with IERR
                               semantics. 1 -> IERR signaling by global fatal
                               is disabled. CATERR#/MSMI# assertion dependent
                               on message (MCERR/IERR/MSMI) received over
                               message channel.
                            */
    UINT32 emca_mode : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When set to high, MSMI input is enabled, and
                               viral will get propagated to MSMI# pin,
                               otherwise it will get propagated to CATERR# pin.
                            */
    UINT32 viral_log_disable : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               This will disable immediate logging of errors
                               while in viral mode.
                            */
    UINT32 viral_log : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Log bit indicating PUnit signaled viral. Clear
                               by setting VIRAL_RESET in this register.
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_pcu_hw_error : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Enable Viral Error signaling of PCU HW errors.
                               When bit is set PCU HW errors will result in
                               assertion of viral.
                            */
    UINT32 en_pcu_uc_error : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Enable Viral Error signaling of PCU uC errors.
                               When bit is set PCU micro-controller errors will
                               result in assertion of viral.
                            */
    UINT32 en_pcu_fw_error : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Enable Viral Error signaling of PCU Pcode
                               errors. When bit is set PCU Pcode errors will
                               result in assertion of viral.
                            */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VIRAL_CONTROL_PCU_FUN4_STRUCT;

/* PCU_FIRST_RMCA_TSC_LO_PCU_FUN4_REG supported on:                             */
/*      SPRA0 (0x21ff4088)                                                      */
/*      SPRB0 (0x21ff4088)                                                      */
/*      SPRHBM (0x21ff4088)                                                     */
/*      SPRC0 (0x21ff4088)                                                      */
/*      SPRMCC (0x21ff4088)                                                     */
/*      SPRUCC (0x21ff4088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the lower 4B of the TSC snapshot taken on the first internal RMCA detected. 
*/


#define PCU_FIRST_RMCA_TSC_LO_PCU_FUN4_REG 0x04420088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Low 4B of TSC snapshot taken on first internal
                               RMCA
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_RMCA_TSC_LO_PCU_FUN4_STRUCT;

/* PCU_FIRST_RMCA_TSC_HI_PCU_FUN4_REG supported on:                             */
/*      SPRA0 (0x21ff408c)                                                      */
/*      SPRB0 (0x21ff408c)                                                      */
/*      SPRHBM (0x21ff408c)                                                     */
/*      SPRC0 (0x21ff408c)                                                      */
/*      SPRMCC (0x21ff408c)                                                     */
/*      SPRUCC (0x21ff408c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the high 4B of the TSC snapshot taken on the first internal RMCA detected. 
*/


#define PCU_FIRST_RMCA_TSC_HI_PCU_FUN4_REG 0x0442008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               High 4B of TSC snapshot taken on first internal
                               RMCA
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_RMCA_TSC_HI_PCU_FUN4_STRUCT;


/* MEM_ACCUMULATED_BW_CH_CFG_0_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40a0)                                                      */
/*      SPRB0 (0x21ff40a0)                                                      */
/*      SPRHBM (0x21ff40a0)                                                     */
/*      SPRC0 (0x21ff40a0)                                                      */
/*      SPRMCC (0x21ff40a0)                                                     */
/*      SPRUCC (0x21ff40a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_0_PCU_FUN4_REG 0x044200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_0_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_1_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40a4)                                                      */
/*      SPRB0 (0x21ff40a4)                                                      */
/*      SPRHBM (0x21ff40a4)                                                     */
/*      SPRC0 (0x21ff40a4)                                                      */
/*      SPRMCC (0x21ff40a4)                                                     */
/*      SPRUCC (0x21ff40a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_1_PCU_FUN4_REG 0x044200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_1_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_2_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40a8)                                                      */
/*      SPRB0 (0x21ff40a8)                                                      */
/*      SPRHBM (0x21ff40a8)                                                     */
/*      SPRC0 (0x21ff40a8)                                                      */
/*      SPRMCC (0x21ff40a8)                                                     */
/*      SPRUCC (0x21ff40a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_2_PCU_FUN4_REG 0x044200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_2_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_3_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40ac)                                                      */
/*      SPRB0 (0x21ff40ac)                                                      */
/*      SPRHBM (0x21ff40ac)                                                     */
/*      SPRC0 (0x21ff40ac)                                                      */
/*      SPRMCC (0x21ff40ac)                                                     */
/*      SPRUCC (0x21ff40ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_3_PCU_FUN4_REG 0x044200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_3_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_4_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40b0)                                                      */
/*      SPRB0 (0x21ff40b0)                                                      */
/*      SPRHBM (0x21ff40b0)                                                     */
/*      SPRC0 (0x21ff40b0)                                                      */
/*      SPRMCC (0x21ff40b0)                                                     */
/*      SPRUCC (0x21ff40b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_4_PCU_FUN4_REG 0x044200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_4_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_5_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40b4)                                                      */
/*      SPRB0 (0x21ff40b4)                                                      */
/*      SPRHBM (0x21ff40b4)                                                     */
/*      SPRC0 (0x21ff40b4)                                                      */
/*      SPRMCC (0x21ff40b4)                                                     */
/*      SPRUCC (0x21ff40b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_5_PCU_FUN4_REG 0x044200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_5_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_6_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40b8)                                                      */
/*      SPRB0 (0x21ff40b8)                                                      */
/*      SPRHBM (0x21ff40b8)                                                     */
/*      SPRC0 (0x21ff40b8)                                                      */
/*      SPRMCC (0x21ff40b8)                                                     */
/*      SPRUCC (0x21ff40b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_6_PCU_FUN4_REG 0x044200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_6_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_7_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40bc)                                                      */
/*      SPRB0 (0x21ff40bc)                                                      */
/*      SPRHBM (0x21ff40bc)                                                     */
/*      SPRC0 (0x21ff40bc)                                                      */
/*      SPRMCC (0x21ff40bc)                                                     */
/*      SPRUCC (0x21ff40bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_7_PCU_FUN4_REG 0x044200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_7_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_8_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40c0)                                                      */
/*      SPRB0 (0x21ff40c0)                                                      */
/*      SPRHBM (0x21ff40c0)                                                     */
/*      SPRC0 (0x21ff40c0)                                                      */
/*      SPRMCC (0x21ff40c0)                                                     */
/*      SPRUCC (0x21ff40c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_8_PCU_FUN4_REG 0x044200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_8_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_9_PCU_FUN4_REG supported on:                       */
/*      SPRA0 (0x21ff40c4)                                                      */
/*      SPRB0 (0x21ff40c4)                                                      */
/*      SPRHBM (0x21ff40c4)                                                     */
/*      SPRC0 (0x21ff40c4)                                                      */
/*      SPRMCC (0x21ff40c4)                                                     */
/*      SPRUCC (0x21ff40c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_9_PCU_FUN4_REG 0x044200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_9_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_10_PCU_FUN4_REG supported on:                      */
/*      SPRA0 (0x21ff40c8)                                                      */
/*      SPRB0 (0x21ff40c8)                                                      */
/*      SPRHBM (0x21ff40c8)                                                     */
/*      SPRC0 (0x21ff40c8)                                                      */
/*      SPRMCC (0x21ff40c8)                                                     */
/*      SPRUCC (0x21ff40c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_10_PCU_FUN4_REG 0x044200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_10_PCU_FUN4_STRUCT;

/* MEM_ACCUMULATED_BW_CH_CFG_11_PCU_FUN4_REG supported on:                      */
/*      SPRA0 (0x21ff40cc)                                                      */
/*      SPRB0 (0x21ff40cc)                                                      */
/*      SPRHBM (0x21ff40cc)                                                     */
/*      SPRC0 (0x21ff40cc)                                                      */
/*      SPRMCC (0x21ff40cc)                                                     */
/*      SPRUCC (0x21ff40cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register contains a measurement proportional to the weighted DRAM BW for the channel (including all ranks).  The weights are configured in the memory controller channel register PM_CMD_PWR.
*/


#define MEM_ACCUMULATED_BW_CH_CFG_11_PCU_FUN4_REG 0x044200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The weighted BW value is calculated by the
                               memory controller based on the following
                               formula:Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE]
                               +Num_Reads * PM_CMD_PWR[PWR_CAS_R] + Num_Writes
                               * PM_CMD_PWR[PWR_CAS_W]
                            */

  } Bits;
  UINT32 Data;

} MEM_ACCUMULATED_BW_CH_CFG_11_PCU_FUN4_STRUCT;

/* MCP_THERMAL_REPORT_1_PCU_FUN4_REG supported on:                              */
/*      SPRA0 (0x21ff40d0)                                                      */
/*      SPRB0 (0x21ff40d0)                                                      */
/*      SPRHBM (0x21ff40d0)                                                     */
/*      SPRC0 (0x21ff40d0)                                                      */
/*      SPRMCC (0x21ff40d0)                                                     */
/*      SPRUCC (0x21ff40d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* MCP Thermal reporting.
      This CSR is a mirror of MSR (1a3h) MCP_THERMAL_REPORT_1
*/


#define MCP_THERMAL_REPORT_1_PCU_FUN4_REG 0x044200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 margin_to_throttle : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Margin to Throttle represents the nomalized
                               temperature margin until throttling engages on
                               any component. It reports the minimum of the
                               temperature margins on different dies. Each
                               margin represents the current sensor temperature
                               relative to the temperature when the hotspot on
                               the die is at its Tj,max. It is reported as a
                               negative number in signed, filtered 10.6 fixed-
                               point format. Note: This is also reported via
                               PECS as the PCS index 2 function.
                            */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCP_THERMAL_REPORT_1_PCU_FUN4_STRUCT;

/* MCP_THERMAL_REPORT_2_PCU_FUN4_REG supported on:                              */
/*      SPRA0 (0x21ff40d8)                                                      */
/*      SPRB0 (0x21ff40d8)                                                      */
/*      SPRHBM (0x21ff40d8)                                                     */
/*      SPRC0 (0x21ff40d8)                                                      */
/*      SPRMCC (0x21ff40d8)                                                     */
/*      SPRUCC (0x21ff40d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* MCP Thermal reporting.
      This CSR is a mirror of MSR (1A4h) MCP_THERMAL_REPORT_2.
*/


#define MCP_THERMAL_REPORT_2_PCU_FUN4_REG 0x044200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package_absolute_max_temperature : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Package Absolute Max Temperature reports the
                               hottest absolute temperature of any component in
                               the multi-chip package. This field is reported
                               in signed, absolute, filtered 10.6 fixed-point
                               format. Note: This is also reported via PECS as
                               PCS index 9 command with parameter 0x80.
                            */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCP_THERMAL_REPORT_2_PCU_FUN4_STRUCT;

/* UNC_TSC_SNAPSHOT_N0_PCU_FUN4_REG supported on:                               */
/*      SPRA0 (0x21ff40e0)                                                      */
/*      SPRB0 (0x21ff40e0)                                                      */
/*      SPRHBM (0x21ff40e0)                                                     */
/*      SPRC0 (0x21ff40e0)                                                      */
/*      SPRMCC (0x21ff40e0)                                                     */
/*      SPRUCC (0x21ff40e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register will capture the value of Uncore TSC on TSC_SYNC assertion rising edge detect in PCU
*/


#define UNC_TSC_SNAPSHOT_N0_PCU_FUN4_REG 0x044200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uncore_tsc_snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Value of the captured Uncore TSC on internal
                               rising edge of TSC_SYNC
                            */

  } Bits;
  UINT32 Data;

} UNC_TSC_SNAPSHOT_N0_PCU_FUN4_STRUCT;

/* UNC_TSC_SNAPSHOT_N1_PCU_FUN4_REG supported on:                               */
/*      SPRA0 (0x21ff40e4)                                                      */
/*      SPRB0 (0x21ff40e4)                                                      */
/*      SPRHBM (0x21ff40e4)                                                     */
/*      SPRC0 (0x21ff40e4)                                                      */
/*      SPRMCC (0x21ff40e4)                                                     */
/*      SPRUCC (0x21ff40e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register will capture the value of Uncore TSC on TSC_SYNC assertion rising edge detect in PCU
*/


#define UNC_TSC_SNAPSHOT_N1_PCU_FUN4_REG 0x044200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uncore_tsc_snapshot : 31;

                            /* Bits[30:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Value of the captured Uncore TSC on internal
                               rising edge of TSC_SYNC
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNC_TSC_SNAPSHOT_N1_PCU_FUN4_STRUCT;

/* TSC_HP_OFFSET_N0_PCU_FUN4_REG supported on:                                  */
/*      SPRA0 (0x21ff40e8)                                                      */
/*      SPRB0 (0x21ff40e8)                                                      */
/*      SPRHBM (0x21ff40e8)                                                     */
/*      SPRC0 (0x21ff40e8)                                                      */
/*      SPRMCC (0x21ff40e8)                                                     */
/*      SPRUCC (0x21ff40e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS may write this register to update the TSC in the hot plugged socket. This is a one time event. 
*/


#define TSC_HP_OFFSET_N0_PCU_FUN4_REG 0x044200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tsc_offset : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS programmed value to do a one-time update to
                               the Uncore TSC to align it for Hot Plug
                            */

  } Bits;
  UINT32 Data;

} TSC_HP_OFFSET_N0_PCU_FUN4_STRUCT;

/* TSC_HP_OFFSET_N1_PCU_FUN4_REG supported on:                                  */
/*      SPRA0 (0x21ff40ec)                                                      */
/*      SPRB0 (0x21ff40ec)                                                      */
/*      SPRHBM (0x21ff40ec)                                                     */
/*      SPRC0 (0x21ff40ec)                                                      */
/*      SPRMCC (0x21ff40ec)                                                     */
/*      SPRUCC (0x21ff40ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS may write this register to update the TSC in the hot plugged socket. This is a one time event. 
*/


#define TSC_HP_OFFSET_N1_PCU_FUN4_REG 0x044200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tsc_offset : 31;

                            /* Bits[30:0], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS programmed value to do a one-time update to
                               the Uncore TSC to align it for Hot Plug
                            */
    UINT32 tsc_update : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               When set, will result cause the TSC_OFFSET value
                               to be added to the Uncore TSC (one-shot)
                            */

  } Bits;
  UINT32 Data;

} TSC_HP_OFFSET_N1_PCU_FUN4_STRUCT;

/* PCU_FIRST_IERR_TSC_LO_PCU_FUN4_REG supported on:                             */
/*      SPRA0 (0x21ff40f0)                                                      */
/*      SPRB0 (0x21ff40f0)                                                      */
/*      SPRHBM (0x21ff40f0)                                                     */
/*      SPRC0 (0x21ff40f0)                                                      */
/*      SPRMCC (0x21ff40f0)                                                     */
/*      SPRUCC (0x21ff40f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the lower 4B of the TSC snapshot taken on the first internal IERR detected. 
*/


#define PCU_FIRST_IERR_TSC_LO_PCU_FUN4_REG 0x044200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Low 4B of TSC snapshot taken on first internal
                               IERR
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_IERR_TSC_LO_PCU_FUN4_STRUCT;

/* PCU_FIRST_IERR_TSC_HI_PCU_FUN4_REG supported on:                             */
/*      SPRA0 (0x21ff40f4)                                                      */
/*      SPRB0 (0x21ff40f4)                                                      */
/*      SPRHBM (0x21ff40f4)                                                     */
/*      SPRC0 (0x21ff40f4)                                                      */
/*      SPRMCC (0x21ff40f4)                                                     */
/*      SPRUCC (0x21ff40f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the high 4B of the TSC snapshot taken on the first internal IERR detected. 
*/


#define PCU_FIRST_IERR_TSC_HI_PCU_FUN4_REG 0x044200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               High 4B of TSC snapshot taken on first internal
                               IERR
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_IERR_TSC_HI_PCU_FUN4_STRUCT;

/* PCU_FIRST_MCERR_TSC_LO_PCU_FUN4_REG supported on:                            */
/*      SPRA0 (0x21ff40f8)                                                      */
/*      SPRB0 (0x21ff40f8)                                                      */
/*      SPRHBM (0x21ff40f8)                                                     */
/*      SPRC0 (0x21ff40f8)                                                      */
/*      SPRMCC (0x21ff40f8)                                                     */
/*      SPRUCC (0x21ff40f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the lower 4B of the TSC snapshot taken on the first internal MCERR detected. 
*/


#define PCU_FIRST_MCERR_TSC_LO_PCU_FUN4_REG 0x044200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Low 4B of TSC snapshot taken on first internal
                               MCERR
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_MCERR_TSC_LO_PCU_FUN4_STRUCT;

/* PCU_FIRST_MCERR_TSC_HI_PCU_FUN4_REG supported on:                            */
/*      SPRA0 (0x21ff40fc)                                                      */
/*      SPRB0 (0x21ff40fc)                                                      */
/*      SPRHBM (0x21ff40fc)                                                     */
/*      SPRC0 (0x21ff40fc)                                                      */
/*      SPRMCC (0x21ff40fc)                                                     */
/*      SPRUCC (0x21ff40fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_4*/
/* Struct generated from SPRA0 BDF: 31_30_4                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This CSR stores the high 4B of the TSC snapshot taken on the first internal MCERR detected. 
*/


#define PCU_FIRST_MCERR_TSC_HI_PCU_FUN4_REG 0x044200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               High 4B of TSC snapshot taken on first internal
                               MCERR
                            */

  } Bits;
  UINT32 Data;

} PCU_FIRST_MCERR_TSC_HI_PCU_FUN4_STRUCT;
#endif /* _PCU_FUN4_h */
