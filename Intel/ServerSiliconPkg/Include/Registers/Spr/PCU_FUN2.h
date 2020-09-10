
/** @file
  PCU_FUN2.h

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


#ifndef _PCU_FUN2_h
#define _PCU_FUN2_h
#include <Base.h>

/* VID_1_30_2_PCU_FUN2_REG supported on:                                        */
/*      SPRA0 (0x11ff2000)                                                      */
/*      SPRB0 (0x11ff2000)                                                      */
/*      SPRHBM (0x11ff2000)                                                     */
/*      SPRC0 (0x11ff2000)                                                      */
/*      SPRMCC (0x11ff2000)                                                     */
/*      SPRUCC (0x11ff2000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Vendor ID Register
*/


#define VID_1_30_2_PCU_FUN2_REG 0x04210000

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

} VID_1_30_2_PCU_FUN2_STRUCT;

/* DID_1_30_2_PCU_FUN2_REG supported on:                                        */
/*      SPRA0 (0x11ff2002)                                                      */
/*      SPRB0 (0x11ff2002)                                                      */
/*      SPRHBM (0x11ff2002)                                                     */
/*      SPRC0 (0x11ff2002)                                                      */
/*      SPRMCC (0x11ff2002)                                                     */
/*      SPRUCC (0x11ff2002)                                                     */
/* Register default value on SPRA0: 0x0000325A                                  */
/* Register default value on SPRB0: 0x0000325A                                  */
/* Register default value on SPRHBM: 0x0000325A                                 */
/* Register default value on SPRC0: 0x0000325A                                  */
/* Register default value on SPRMCC: 0x0000325A                                 */
/* Register default value on SPRUCC: 0x0000325A                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Device Identification Number
*/


#define DID_1_30_2_PCU_FUN2_REG 0x04210002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000325A*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_30_2_PCU_FUN2_STRUCT;

/* PCICMD_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x11ff2004)                                                      */
/*      SPRB0 (0x11ff2004)                                                      */
/*      SPRHBM (0x11ff2004)                                                     */
/*      SPRC0 (0x11ff2004)                                                      */
/*      SPRMCC (0x11ff2004)                                                     */
/*      SPRUCC (0x11ff2004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Command Register
*/


#define PCICMD_1_30_2_PCU_FUN2_REG 0x04210004

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

} PCICMD_1_30_2_PCU_FUN2_STRUCT;

/* PCISTS_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x11ff2006)                                                      */
/*      SPRB0 (0x11ff2006)                                                      */
/*      SPRHBM (0x11ff2006)                                                     */
/*      SPRC0 (0x11ff2006)                                                      */
/*      SPRMCC (0x11ff2006)                                                     */
/*      SPRUCC (0x11ff2006)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Status Register
*/


#define PCISTS_1_30_2_PCU_FUN2_REG 0x04210006

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

} PCISTS_1_30_2_PCU_FUN2_STRUCT;

/* RID_CCR_1_30_2_PCU_FUN2_REG supported on:                                    */
/*      SPRA0 (0x21ff2008)                                                      */
/*      SPRB0 (0x21ff2008)                                                      */
/*      SPRHBM (0x21ff2008)                                                     */
/*      SPRC0 (0x21ff2008)                                                      */
/*      SPRMCC (0x21ff2008)                                                     */
/*      SPRUCC (0x21ff2008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_30_2_PCU_FUN2_REG 0x04220008

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

} RID_CCR_1_30_2_PCU_FUN2_STRUCT;

/* CLSR_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x1ff200c)                                                       */
/*      SPRB0 (0x1ff200c)                                                       */
/*      SPRHBM (0x1ff200c)                                                      */
/*      SPRC0 (0x1ff200c)                                                       */
/*      SPRMCC (0x1ff200c)                                                      */
/*      SPRUCC (0x1ff200c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Cache Line Size Register
*/


#define CLSR_1_30_2_PCU_FUN2_REG 0x0420000C

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

} CLSR_1_30_2_PCU_FUN2_STRUCT;

/* PLAT_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x1ff200d)                                                       */
/*      SPRB0 (0x1ff200d)                                                       */
/*      SPRHBM (0x1ff200d)                                                      */
/*      SPRC0 (0x1ff200d)                                                       */
/*      SPRMCC (0x1ff200d)                                                      */
/*      SPRUCC (0x1ff200d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Latency Timer
*/


#define PLAT_1_30_2_PCU_FUN2_REG 0x0420000D

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

} PLAT_1_30_2_PCU_FUN2_STRUCT;

/* HDR_1_30_2_PCU_FUN2_REG supported on:                                        */
/*      SPRA0 (0x1ff200e)                                                       */
/*      SPRB0 (0x1ff200e)                                                       */
/*      SPRHBM (0x1ff200e)                                                      */
/*      SPRC0 (0x1ff200e)                                                       */
/*      SPRMCC (0x1ff200e)                                                      */
/*      SPRUCC (0x1ff200e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Header Type
*/


#define HDR_1_30_2_PCU_FUN2_REG 0x0420000E

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

} HDR_1_30_2_PCU_FUN2_STRUCT;

/* BIST_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x1ff200f)                                                       */
/*      SPRB0 (0x1ff200f)                                                       */
/*      SPRHBM (0x1ff200f)                                                      */
/*      SPRC0 (0x1ff200f)                                                       */
/*      SPRMCC (0x1ff200f)                                                      */
/*      SPRUCC (0x1ff200f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI BIST Register
*/


#define BIST_1_30_2_PCU_FUN2_REG 0x0420000F

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

} BIST_1_30_2_PCU_FUN2_STRUCT;

/* SVID_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x11ff202c)                                                      */
/*      SPRB0 (0x11ff202c)                                                      */
/*      SPRHBM (0x11ff202c)                                                     */
/*      SPRC0 (0x11ff202c)                                                      */
/*      SPRMCC (0x11ff202c)                                                     */
/*      SPRUCC (0x11ff202c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem Vendor ID Register
*/


#define SVID_1_30_2_PCU_FUN2_REG 0x0421002C

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

} SVID_1_30_2_PCU_FUN2_STRUCT;

/* SDID_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x11ff202e)                                                      */
/*      SPRB0 (0x11ff202e)                                                      */
/*      SPRHBM (0x11ff202e)                                                     */
/*      SPRC0 (0x11ff202e)                                                      */
/*      SPRMCC (0x11ff202e)                                                     */
/*      SPRUCC (0x11ff202e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem ID Register
*/


#define SDID_1_30_2_PCU_FUN2_REG 0x0421002E

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

} SDID_1_30_2_PCU_FUN2_STRUCT;

/* CAPPTR_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x1ff2034)                                                       */
/*      SPRB0 (0x1ff2034)                                                       */
/*      SPRHBM (0x1ff2034)                                                      */
/*      SPRC0 (0x1ff2034)                                                       */
/*      SPRMCC (0x1ff2034)                                                      */
/*      SPRUCC (0x1ff2034)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_30_2_PCU_FUN2_REG 0x04200034

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

} CAPPTR_1_30_2_PCU_FUN2_STRUCT;

/* INTL_1_30_2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x1ff203c)                                                       */
/*      SPRB0 (0x1ff203c)                                                       */
/*      SPRHBM (0x1ff203c)                                                      */
/*      SPRC0 (0x1ff203c)                                                       */
/*      SPRMCC (0x1ff203c)                                                      */
/*      SPRUCC (0x1ff203c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Line Register
*/


#define INTL_1_30_2_PCU_FUN2_REG 0x0420003C

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

} INTL_1_30_2_PCU_FUN2_STRUCT;

/* INTPIN_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x1ff203d)                                                       */
/*      SPRB0 (0x1ff203d)                                                       */
/*      SPRHBM (0x1ff203d)                                                      */
/*      SPRC0 (0x1ff203d)                                                       */
/*      SPRMCC (0x1ff203d)                                                      */
/*      SPRUCC (0x1ff203d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_30_2_PCU_FUN2_REG 0x0420003D

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

} INTPIN_1_30_2_PCU_FUN2_STRUCT;

/* MINGNT_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x1ff203e)                                                       */
/*      SPRB0 (0x1ff203e)                                                       */
/*      SPRHBM (0x1ff203e)                                                      */
/*      SPRC0 (0x1ff203e)                                                       */
/*      SPRMCC (0x1ff203e)                                                      */
/*      SPRUCC (0x1ff203e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Min Grant Register
*/


#define MINGNT_1_30_2_PCU_FUN2_REG 0x0420003E

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

} MINGNT_1_30_2_PCU_FUN2_STRUCT;

/* MAXLAT_1_30_2_PCU_FUN2_REG supported on:                                     */
/*      SPRA0 (0x1ff203f)                                                       */
/*      SPRB0 (0x1ff203f)                                                       */
/*      SPRHBM (0x1ff203f)                                                      */
/*      SPRC0 (0x1ff203f)                                                       */
/*      SPRMCC (0x1ff203f)                                                      */
/*      SPRUCC (0x1ff203f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Max Latency Register
*/


#define MAXLAT_1_30_2_PCU_FUN2_REG 0x0420003F

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

} MAXLAT_1_30_2_PCU_FUN2_STRUCT;

/* DRAM_ENERGY_STATUS_PCU_FUN2_REG supported on:                                */
/*      SPRA0 (0x21ff2080)                                                      */
/*      SPRB0 (0x21ff2080)                                                      */
/*      SPRHBM (0x21ff2080)                                                     */
/*      SPRC0 (0x21ff2080)                                                      */
/*      SPRMCC (0x21ff2080)                                                     */
/*      SPRUCC (0x21ff2080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in all the Channels.  The counter will wrap around and continue counting when it reaches its limit.  
  ENERGY_UNIT for DRAM domain is 15.3uJ.
  The data is updated by PCODE and is Read Only for all SW.  
*/


#define DRAM_ENERGY_STATUS_PCU_FUN2_REG 0x04220080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* Energy Value */

  } Bits;
  UINT32 Data;

} DRAM_ENERGY_STATUS_PCU_FUN2_STRUCT;

/* UNCORE_FIVR_ERR_LOG_PCU_FUN2_REG supported on:                               */
/*      SPRA0 (0x21ff2084)                                                      */
/*      SPRB0 (0x21ff2084)                                                      */
/*      SPRHBM (0x21ff2084)                                                     */
/*      SPRC0 (0x21ff2084)                                                      */
/*      SPRMCC (0x21ff2084)                                                     */
/*      SPRUCC (0x21ff2084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Reports Uncore FIVR Faults - If any bit is set in 31:0 an uncore FIVR fault has occured, and firmware should FRB the socket. An IERR will also occur on this condition.  
*/


#define UNCORE_FIVR_ERR_LOG_PCU_FUN2_REG 0x04220084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fault_vector : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* Fault vector */

  } Bits;
  UINT32 Data;

} UNCORE_FIVR_ERR_LOG_PCU_FUN2_STRUCT;

/* PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_REG supported on:                          */
/*      SPRA0 (0x21ff2088)                                                      */
/*      SPRB0 (0x21ff2088)                                                      */
/*      SPRHBM (0x21ff2088)                                                     */
/*      SPRC0 (0x21ff2088)                                                      */
/*      SPRMCC (0x21ff2088)                                                     */
/*      SPRUCC (0x21ff2088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used by Pcode to report Package Power limit violations in the Platform PBM.

Provides information on the performance impact of the RAPL power limit. 
Provides the total time for which PACKAGE was throttled because of the RAPL power limit. Throttling here is defined as going below O.S requested P-state. 
Total time = Reg value * 1s *1 /2(time unit)
Usage model: Enables the O.S/Driver to learn about PACKAGE throttling as a result of RAPL limit.
Can be used by other S/W components that control the PACKAGE power.
 This register is mapped as an MSR with the name Package_Perf_Status
*/


#define PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_REG 0x04220088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwr_limit_throttle_ctr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reports the number of times the Power limiting
                               algorithm had to clip the power limit due to
                               hitting the lowest power state available.
                               Accumulated PACKAGE throttled time
                            */

  } Bits;
  UINT32 Data;

} PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_STRUCT;

/* DTS_CONFIG1_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x21ff2094)                                                      */
/*      SPRB0 (0x21ff2094)                                                      */
/*      SPRHBM (0x21ff2094)                                                     */
/*      SPRC0 (0x21ff2094)                                                      */
/*      SPRMCC (0x21ff2094)                                                     */
/*      SPRUCC (0x21ff2094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Location of correctionfactor F and scale factor C for 
DTS2.0
      bscherka: IVT HSD i1112231 DTS2.0 added this csr
               isteiner: Moved DTS registers for reconvergence
*/


#define DTS_CONFIG1_PCU_FUN2_REG 0x04220094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 f : 13;

                            /* Bits[12:0], Access Type=RW/L, default=0x00000000*/

                            /* Correction Factor F */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 c : 13;

                            /* Bits[28:16], Access Type=RW/L, default=0x00000000*/

                            /* Scale factor C */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTS_CONFIG1_PCU_FUN2_STRUCT;

/* DTS_CONFIG2_PCU_FUN2_REG supported on:                                       */
/*      SPRA0 (0x21ff2098)                                                      */
/*      SPRB0 (0x21ff2098)                                                      */
/*      SPRHBM (0x21ff2098)                                                     */
/*      SPRC0 (0x21ff2098)                                                      */
/*      SPRMCC (0x21ff2098)                                                     */
/*      SPRUCC (0x21ff2098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Location of time constant coefficients for DTS2.0 
inplementation.
      bscherka: IVT HSD i1112231 added this csr
               isteiner: Moved DTS registers for reconvergence
*/


#define DTS_CONFIG2_PCU_FUN2_REG 0x04220098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 af : 13;

                            /* Bits[12:0], Access Type=RW/L, default=0x00000000*/

                            /* Time constant coefficient AF */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 as : 13;

                            /* Bits[28:16], Access Type=RW/L, default=0x00000000*/

                            /* Time constant coefficient AS */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTS_CONFIG2_PCU_FUN2_STRUCT;


/* GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG supported on:                   */
/*      SPRA0 (0x21ff20a0)                                                      */
/*      SPRB0 (0x21ff20a0)                                                      */
/*      SPRHBM (0x21ff20a0)                                                     */
/*      SPRC0 (0x21ff20a0)                                                      */
/*      SPRMCC (0x21ff20a0)                                                     */
/*      SPRUCC (0x21ff20a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register is in the PCU CR space.  It contains information pertinent to the master slave IPC protocol and global enable/disable for PK CST and SST.  Expectation is that BIOS will write this register during the Reset/Init flow.
*/


#define GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG 0x042200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 am_i_master : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               If set, socket is master. Master socket will be
                               the leady socket. BIOS will set this bit in the
                               legacy socket.
                            */
    UINT32 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 my_nid : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /* NID of this socket. */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 master_nid : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Master socket NID. Can also be determined from
                               the Socket0 entry in the NID MAP register.
                            */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_STRUCT;

/* GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG supported on:                      */
/*      SPRA0 (0x21ff20a4)                                                      */
/*      SPRB0 (0x21ff20a4)                                                      */
/*      SPRHBM (0x21ff20a4)                                                     */
/*      SPRC0 (0x21ff20a4)                                                      */
/*      SPRMCC (0x21ff20a4)                                                     */
/*      SPRUCC (0x21ff20a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register is in the PCU CR space.  It contains NID information for sockets 0-3 in the platform.  Expectation is that BIOS will write this register during the Reset/Init flow.
*/


#define GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG 0x042200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 skt0_nid : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Socket0 NID */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt1_nid : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /* Socket1 NID */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt2_nid : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Socket2 NID */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skt3_nid : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* Socket3 NID */
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

} GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_STRUCT;

/* DRAM_POWER_INFO_N0_PCU_FUN2_REG supported on:                                */
/*      SPRA0 (0x21ff20a8)                                                      */
/*      SPRB0 (0x21ff20a8)                                                      */
/*      SPRHBM (0x21ff20a8)                                                     */
/*      SPRC0 (0x21ff20a8)                                                      */
/*      SPRMCC (0x21ff20a8)                                                     */
/*      SPRUCC (0x21ff20a8)                                                     */
/* Register default value on SPRA0: 0x00780118                                  */
/* Register default value on SPRB0: 0x00780118                                  */
/* Register default value on SPRHBM: 0x00780118                                 */
/* Register default value on SPRC0: 0x00780118                                  */
/* Register default value on SPRMCC: 0x00780118                                 */
/* Register default value on SPRUCC: 0x00780118                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Power allowed for DRAM
*/


#define DRAM_POWER_INFO_N0_PCU_FUN2_REG 0x042200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dram_tdp : 15;

                            /* Bits[14:0], Access Type=RW/L, default=0x00000118*/

                            /*
                               The Spec power allowed for DRAM. The TDP setting
                               is typical (not guaranteed). The units for this
                               value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dram_min_pwr : 15;

                            /* Bits[30:16], Access Type=RW/L, default=0x00000078*/

                            /*
                               The minimal power setting allowed for DRAM.
                               Lower values will be clamped to this value. The
                               minimum setting is typical (not guaranteed). The
                               units for this value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_POWER_INFO_N0_PCU_FUN2_STRUCT;

/* DRAM_POWER_INFO_N1_PCU_FUN2_REG supported on:                                */
/*      SPRA0 (0x21ff20ac)                                                      */
/*      SPRB0 (0x21ff20ac)                                                      */
/*      SPRHBM (0x21ff20ac)                                                     */
/*      SPRC0 (0x21ff20ac)                                                      */
/*      SPRMCC (0x21ff20ac)                                                     */
/*      SPRUCC (0x21ff20ac)                                                     */
/* Register default value on SPRA0: 0x00280258                                  */
/* Register default value on SPRB0: 0x00280258                                  */
/* Register default value on SPRHBM: 0x00280258                                 */
/* Register default value on SPRC0: 0x00280258                                  */
/* Register default value on SPRMCC: 0x00280258                                 */
/* Register default value on SPRUCC: 0x00280258                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Power allowed for DRAM
*/


#define DRAM_POWER_INFO_N1_PCU_FUN2_REG 0x042200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dram_max_pwr : 15;

                            /* Bits[14:0], Access Type=RW/L, default=0x00000258*/

                            /*
                               The maximal power setting allowed for DRAM.
                               Higher values will be clamped to this value. The
                               maximum setting is typical (not guaranteed). The
                               units for this value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dram_max_win : 7;

                            /* Bits[22:16], Access Type=RW/L, default=0x00000028*/

                            /*
                               The maximal time window allowed for the DRAM.
                               Higher values will be clamped to this value. x =
                               PKG_MAX_WIN[54:53] y = PKG_MAX_WIN[52:48] The
                               timing interval window is Floating Point number
                               given by 1.x * power(2,y). The unit of
                               measurement is defined in
                               DRAM_POWER_INFO_UNIT_MSR[TIME_UNIT].
                            */
    UINT32 rsvd_23 : 8;

                            /* Bits[30:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /* Lock bit to lock the Register */

  } Bits;
  UINT32 Data;

} DRAM_POWER_INFO_N1_PCU_FUN2_STRUCT;

/* PROCHOT_RESPONSE_RATIO_PCU_FUN2_REG supported on:                            */
/*      SPRA0 (0x21ff20b0)                                                      */
/*      SPRB0 (0x21ff20b0)                                                      */
/*      SPRHBM (0x21ff20b0)                                                     */
/*      SPRC0 (0x21ff20b0)                                                      */
/*      SPRMCC (0x21ff20b0)                                                     */
/*      SPRUCC (0x21ff20b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Controls the CPU response to an inbound platform assertion of xxPROCHOT# or PMAX detector by capping to the programmed ratio.
*/


#define PROCHOT_RESPONSE_RATIO_PCU_FUN2_REG 0x042200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 prochot_ratio : 8;

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Controls the CPU response to an inbound platform
                               assertion of xxPROCHOT# by capping to the
                               programmed ratio. Default value is Pn. The min
                               allowed ratio is defined by
                               PLATFORM_INFO[MIN_OPERATING_RATIO] Locked by
                               POWER_CTL1.PROCHOT_LOCK formerly
                               FIRM_CONFIG.PROCHOT_LOCK
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PROCHOT_RESPONSE_RATIO_PCU_FUN2_STRUCT;

/* MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN2_REG supported on:                     */
/*      SPRA0 (0x21ff20c8)                                                      */
/*      SPRB0 (0x21ff20c8)                                                      */
/*      SPRHBM (0x21ff20c8)                                                     */
/*      SPRC0 (0x21ff20c8)                                                      */
/*      SPRMCC (0x21ff20c8)                                                     */
/*      SPRUCC (0x21ff20c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used to report the thermal status of the memory.   The channel max temperature field is used to report the maximal temperature of all ranks.
*/


#define MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN2_REG 0x042200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel0_max_temperature : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel1_max_temperature : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel2_max_temperature : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN2_STRUCT;

/* MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN2_REG supported on:                     */
/*      SPRA0 (0x21ff20cc)                                                      */
/*      SPRB0 (0x21ff20cc)                                                      */
/*      SPRHBM (0x21ff20cc)                                                     */
/*      SPRC0 (0x21ff20cc)                                                      */
/*      SPRMCC (0x21ff20cc)                                                     */
/*      SPRUCC (0x21ff20cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used to report the thermal status of the memory.   The channel max temperature field is used to report the maximal temperature of all ranks.
*/


#define MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN2_REG 0x042200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel0_max_temperature : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel1_max_temperature : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel2_max_temperature : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN2_STRUCT;

/* MEM_TRML_TEMPERATURE_REPORT_2_PCU_FUN2_REG supported on:                     */
/*      SPRA0 (0x21ff20d0)                                                      */
/*      SPRB0 (0x21ff20d0)                                                      */
/*      SPRHBM (0x21ff20d0)                                                     */
/*      SPRC0 (0x21ff20d0)                                                      */
/*      SPRMCC (0x21ff20d0)                                                     */
/*      SPRUCC (0x21ff20d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used to report the thermal status of the memory.   The channel max temperature field is used to report the maximal temperature of all ranks.
*/


#define MEM_TRML_TEMPERATURE_REPORT_2_PCU_FUN2_REG 0x042200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel0_max_temperature : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel1_max_temperature : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel2_max_temperature : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_TRML_TEMPERATURE_REPORT_2_PCU_FUN2_STRUCT;

/* MEM_TRML_TEMPERATURE_REPORT_3_PCU_FUN2_REG supported on:                     */
/*      SPRA0 (0x21ff20d4)                                                      */
/*      SPRB0 (0x21ff20d4)                                                      */
/*      SPRHBM (0x21ff20d4)                                                     */
/*      SPRC0 (0x21ff20d4)                                                      */
/*      SPRMCC (0x21ff20d4)                                                     */
/*      SPRUCC (0x21ff20d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used to report the thermal status of the memory.   The channel max temperature field is used to report the maximal temperature of all ranks.
*/


#define MEM_TRML_TEMPERATURE_REPORT_3_PCU_FUN2_REG 0x042200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel0_max_temperature : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel1_max_temperature : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 channel2_max_temperature : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Temperature in Degrees (C). */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_TRML_TEMPERATURE_REPORT_3_PCU_FUN2_STRUCT;

/* DRAM_RAPL_PERF_STATUS_PCU_FUN2_REG supported on:                             */
/*      SPRA0 (0x21ff20d8)                                                      */
/*      SPRB0 (0x21ff20d8)                                                      */
/*      SPRHBM (0x21ff20d8)                                                     */
/*      SPRC0 (0x21ff20d8)                                                      */
/*      SPRMCC (0x21ff20d8)                                                     */
/*      SPRUCC (0x21ff20d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is used by Pcode to report DRAM Plane Power limit violations in the Platform PBM.

Provides information on the performance impact of the RAPL power limit. 
Provides the total time for which DRAM was throttled because of the RAPL power limit. Total time = Reg Value *1 s * 1/ (2time_unit)
Usage model: Enables the OS/Driver to learn about DRAM throttling as a result of RAPL limit.
Can be used by other S/W components that control the DRAM power.
This register is mapped as an MSR with the name DRAM_RAPL_PERF_STATUS.
*/


#define DRAM_RAPL_PERF_STATUS_PCU_FUN2_REG 0x042200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pwr_limit_throttle_ctr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reports the number of times the Power limiting
                               algorithm had to clip the power limit due to
                               hitting the lowest power state available.
                               Accumulated DRAM throttled time
                            */

  } Bits;
  UINT32 Data;

} DRAM_RAPL_PERF_STATUS_PCU_FUN2_STRUCT;

/* DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG supported on:                            */
/*      SPRA0 (0x21ff20dc)                                                      */
/*      SPRB0 (0x21ff20dc)                                                      */
/*      SPRHBM (0x21ff20dc)                                                     */
/*      SPRC0 (0x21ff20dc)                                                      */
/*      SPRMCC (0x21ff20dc)                                                     */
/*      SPRUCC (0x21ff20dc)                                                     */
/* Register default value on SPRA0: 0x00030000                                  */
/* Register default value on SPRB0: 0x00030000                                  */
/* Register default value on SPRHBM: 0x00030000                                 */
/* Register default value on SPRC0: 0x00030000                                  */
/* Register default value on SPRMCC: 0x00030000                                 */
/* Register default value on SPRUCC: 0x00030000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register effectively governs all major power saving 
engines and hueristics on the die.
*/


#define DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG 0x042200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 imc_apm_override_enable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_11 : 4;

                            /* Bits[14:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_15 : 5;

                            /* Bits[19:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 uncore_perf_plimit_override_enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_21 : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DYNAMIC_PERF_POWER_CTL_PCU_FUN2_STRUCT;


/* PERF_P_LIMIT_CONTROL_PCU_FUN2_REG supported on:                              */
/*      SPRA0 (0x21ff20e4)                                                      */
/*      SPRB0 (0x21ff20e4)                                                      */
/*      SPRHBM (0x21ff20e4)                                                     */
/*      SPRC0 (0x21ff20e4)                                                      */
/*      SPRMCC (0x21ff20e4)                                                     */
/*      SPRUCC (0x21ff20e4)                                                     */
/* Register default value on SPRA0: 0x7D000000                                  */
/* Register default value on SPRB0: 0x7D000000                                  */
/* Register default value on SPRHBM: 0x7D000000                                 */
/* Register default value on SPRC0: 0x7D000000                                  */
/* Register default value on SPRMCC: 0x7D000000                                 */
/* Register default value on SPRUCC: 0x7D000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register is BIOS configurable.  Dual mapping will prevent additional fast path events or polling needs from PCODE.  HW does not use the CSR input, it is primarily used by PCODE.  Note that PERF_P_LIMIT_CLIP must be nominally configured to guaranteed frequency + 1, if turbo related actions are needed in slave sockets.
*/


#define PERF_P_LIMIT_CONTROL_PCU_FUN2_REG 0x042200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 perf_plimit_enable : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /* Enable Performance P-limit feature */
    UINT32 perf_plimit_threshold : 5;

                            /* Bits[5:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               Uncore frequency threshold above which this
                               socket will trigger the feature and start trying
                               to raise frequency of other sockets.
                            */
    UINT32 reserved6 : 1;

                            /* Bits[6:6], Access Type=RW/V, default=0x00000000*/

                            /* Reserved. */
    UINT32 perf_plimit_clip : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000000*/

                            /*
                               Maximum value the floor is allowed to be set to
                               for perf P-limit.
                            */
    UINT32 reserved12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Reserved. */
    UINT32 reserved13 : 2;

                            /* Bits[14:13], Access Type=RW/V, default=0x00000000*/

                            /* Reserved. */
    UINT32 perf_plimit_differential : 3;

                            /* Bits[17:15], Access Type=RW/V, default=0x00000000*/

                            /*
                               Parameter used to tune how far below local
                               socket frequency remote socket frequency is
                               allowed to be. Also impacts rate at which
                               frequency drops when feature disengages.
                            */
    UINT32 reserved18 : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /* Reserved. */
    UINT32 reserved19 : 13;

                            /* Bits[31:19], Access Type=RO/V, default=0x00000FA0*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PERF_P_LIMIT_CONTROL_PCU_FUN2_STRUCT;

/* IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_REG supported on:                      */
/*      SPRA0 (0x21ff20e8)                                                      */
/*      SPRB0 (0x21ff20e8)                                                      */
/*      SPRHBM (0x21ff20e8)                                                     */
/*      SPRC0 (0x21ff20e8)                                                      */
/*      SPRMCC (0x21ff20e8)                                                     */
/*      SPRUCC (0x21ff20e8)                                                     */
/* Register default value on SPRA0: 0x00018AB4                                  */
/* Register default value on SPRB0: 0x00018AB4                                  */
/* Register default value on SPRHBM: 0x00018AB4                                 */
/* Register default value on SPRC0: 0x00018AB4                                  */
/* Register default value on SPRMCC: 0x00018AB4                                 */
/* Register default value on SPRUCC: 0x00018AB4                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Various controls
*/


#define IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_REG 0x042200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_floor_1 : 3;

                            /* Bits[2:0], Access Type=RW/V/P, default=0x00000004*/

                            /* IO Floor 1 */
    UINT32 io_thresh_1 : 3;

                            /* Bits[5:3], Access Type=RW/V/P, default=0x00000006*/

                            /* IO Threshold 1 */
    UINT32 io_floor_2 : 3;

                            /* Bits[8:6], Access Type=RW/V/P, default=0x00000002*/

                            /* IO Floor 2 */
    UINT32 io_thresh_2 : 3;

                            /* Bits[11:9], Access Type=RW/V/P, default=0x00000005*/

                            /* IO threshold 2 */
    UINT32 io_floor_3 : 3;

                            /* Bits[14:12], Access Type=RW/V/P, default=0x00000000*/

                            /* IO floor 3 */
    UINT32 io_thresh_3 : 3;

                            /* Bits[17:15], Access Type=RW/V/P, default=0x00000003*/

                            /* IO Threshold 3 */
    UINT32 kti_floor_1 : 3;

                            /* Bits[20:18], Access Type=RW/V/P, default=0x00000000*/

                            /* Intel UPI Floor 1 */
    UINT32 kti_thresh_1 : 3;

                            /* Bits[23:21], Access Type=RW/V/P, default=0x00000000*/

                            /* Intel UPI Threshold 1 */
    UINT32 kti_floor_2 : 3;

                            /* Bits[26:24], Access Type=RW/V/P, default=0x00000000*/

                            /* Intel UPI Floor 2 */
    UINT32 kti_thresh_2 : 3;

                            /* Bits[29:27], Access Type=RW/V/P, default=0x00000000*/

                            /* Intel UPI Threshold 2 */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovrd_enable : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*
                               IO_BW_PLIMIT Override Bit. 0 - Disable 1 -
                               Enable
                            */

  } Bits;
  UINT32 Data;

} IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_STRUCT;

/* MCA_ERR_SRC_LOG_PCU_FUN2_REG supported on:                                   */
/*      SPRA0 (0x21ff20ec)                                                      */
/*      SPRB0 (0x21ff20ec)                                                      */
/*      SPRHBM (0x21ff20ec)                                                     */
/*      SPRC0 (0x21ff20ec)                                                      */
/*      SPRMCC (0x21ff20ec)                                                     */
/*      SPRUCC (0x21ff20ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MCSourceLog is used by the PCU to log the error sources. This register is initialized to zeroes during reset. The PCU will set the relevant bits when the condition they represent appears. The PCU never clears the registers-the UBox or off-die entities should clear them when they are consumed, unless their processing involves taking down the platform.
*/


#define MCA_ERR_SRC_LOG_PCU_FUN2_REG 0x042200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_core_ierr : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. SKX b302820. */
    UINT32 rsvd : 9;

                            /* Bits[13:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rmsmi_internal : 1;

                            /* Bits[14:14], Access Type=RW/V/P, default=0x00000000*/

                            /* This socket asserted recoverable MSMI */
    UINT32 rmca_internal : 1;

                            /* Bits[15:15], Access Type=RW/V/P, default=0x00000000*/

                            /* This socket asserted recoverable MCA */
    UINT32 rmsmi : 1;

                            /* Bits[16:16], Access Type=RW/V/P, default=0x00000000*/

                            /* This socket observed recoverable MSMI */
    UINT32 rmca : 1;

                            /* Bits[17:17], Access Type=RW/V/P, default=0x00000000*/

                            /* This socket observed recoverable MSMI */
    UINT32 msmi_mcerr_internal : 1;

                            /* Bits[18:18], Access Type=RW/V/P, default=0x00000000*/

                            /* Internal error: This socket asserted MSMI_MCERR. */
    UINT32 msmi_ierr_internal : 1;

                            /* Bits[19:19], Access Type=RW/V/P, default=0x00000000*/

                            /* Internal error: This socket asserted MSMI_IERR. */
    UINT32 msmi_internal : 1;

                            /* Bits[20:20], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Internal error: This socket asserted a MSMI#.
                               This is OR (bit 19,18).
                            */
    UINT32 msmi_mcerr : 1;

                            /* Bits[21:21], Access Type=RW/V/P, default=0x00000000*/

                            /* External error: The package observed MSMI_MCERR. */
    UINT32 msmi_ierr : 1;

                            /* Bits[22:22], Access Type=RW/V/P, default=0x00000000*/

                            /* External error: The package observed MSMI_IERR. */
    UINT32 msmi : 1;

                            /* Bits[23:23], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               External error: The package observed MSMI# (for
                               any reason). It is or(bit 22, bit21); functions
                               as a Valid bit for the other two package
                               conditions. It has no effect when a local core
                               is associated with the error.
                            */
    UINT32 msmi_mcp_smbus : 1;

                            /* Bits[24:24], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Internal error: MCP SMBus has detected error on
                               the bus.
                            */
    UINT32 cam_parity_err : 1;

                            /* Bits[25:25], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Internal error: CAM Logical to Physical lookup
                               has detected parity error.
                            */
    UINT32 mcerr_internal : 1;

                            /* Bits[26:26], Access Type=RW/V/P, default=0x00000000*/

                            /* Internal error: This socket asserted MCERR. */
    UINT32 ierr_internal : 1;

                            /* Bits[27:27], Access Type=RW/V/P, default=0x00000000*/

                            /* Internal error: This socket asserted IERR. */
    UINT32 caterr_internal : 1;

                            /* Bits[28:28], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Internal error: This socket asserted a CATERR#.
                               This is OR (bit 27,26).
                            */
    UINT32 mcerr : 1;

                            /* Bits[29:29], Access Type=RW/V/P, default=0x00000000*/

                            /* External error: The package observed MCERR. */
    UINT32 ierr : 1;

                            /* Bits[30:30], Access Type=RW/V/P, default=0x00000000*/

                            /* External error: The package observed IERR. */
    UINT32 caterr : 1;

                            /* Bits[31:31], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               External error: The package observed CATERR#
                               (for any reason). It is or(bit 30, bit29);
                               functions as a Valid bit for the other two
                               package conditions. It has no effect when a
                               local core is associated with the error.
                            */

  } Bits;
  UINT32 Data;

} MCA_ERR_SRC_LOG_PCU_FUN2_STRUCT;

/* DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_REG supported on:                         */
/*      SPRA0 (0x21ff20f0)                                                      */
/*      SPRB0 (0x21ff20f0)                                                      */
/*      SPRHBM (0x21ff20f0)                                                     */
/*      SPRC0 (0x21ff20f0)                                                      */
/*      SPRMCC (0x21ff20f0)                                                     */
/*      SPRUCC (0x21ff20f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register is used by BIOS/OS/Integrated Graphics Driver/CPM Driver to limit the power budget of DRAM Plane.
  
  The overall package turbo power limitation is controlled by DRAM_PLANE_POWER_LIMIT.
*/


#define DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_REG 0x042200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dram_pp_pwr_lim : 15;

                            /* Bits[14:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This is the power limitation on the IA cores
                               power plane. The unit of measurement is defined
                               in DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 pwr_lim_ctrl_en : 1;

                            /* Bits[15:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit must be set in order to limit the power
                               of the DRAM power plane. 0b DRAM power plane
                               power limitation is disabled 1b DRAM power plane
                               power limitation is enabled
                            */
    UINT32 reserved : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ctrl_time_win : 7;

                            /* Bits[23:17], Access Type=RW/L, default=0x00000000*/

                            /*
                               x = CTRL_TIME_WIN[23:22] y =
                               CTRL_TIME_WIN[21:17] The timing interval window
                               is Floating Point number given by 1.x *
                               power(2,y). The unit of measurement is defined
                               in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT]. The
                               maximal time window is bounded by
                               PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN]. The minimum
                               time window is 1 unit of measurement (as defined
                               above).
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_STRUCT;

/* DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG supported on:                         */
/*      SPRA0 (0x21ff20f4)                                                      */
/*      SPRB0 (0x21ff20f4)                                                      */
/*      SPRHBM (0x21ff20f4)                                                     */
/*      SPRC0 (0x21ff20f4)                                                      */
/*      SPRMCC (0x21ff20f4)                                                     */
/*      SPRUCC (0x21ff20f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register is used by BIOS/OS/Integrated Graphics Driver/CPM Driver to limit the power budget of DRAM Plane.
  
  The overall package turbo power limitation is controlled by DRAM_PLANE_POWER_LIMIT.
*/


#define DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG 0x042200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 31;

                            /* Bits[30:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pp_pwr_lim_lock : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only.
                            */

  } Bits;
  UINT32 Data;

} DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_STRUCT;

/* THERMTRIP_CONFIG_PCU_FUN2_REG supported on:                                  */
/*      SPRA0 (0x21ff20f8)                                                      */
/*      SPRB0 (0x21ff20f8)                                                      */
/*      SPRHBM (0x21ff20f8)                                                     */
/*      SPRC0 (0x21ff20f8)                                                      */
/*      SPRMCC (0x21ff20f8)                                                     */
/*      SPRUCC (0x21ff20f8)                                                     */
/* Register default value on SPRA0: 0x000000C0                                  */
/* Register default value on SPRB0: 0x000000C0                                  */
/* Register default value on SPRHBM: 0x000000C0                                 */
/* Register default value on SPRC0: 0x000000C0                                  */
/* Register default value on SPRMCC: 0x000000C0                                 */
/* Register default value on SPRUCC: 0x000000C0                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register is used to configure whether the Thermtrip signal only carries the processor Trip information, or does it carry the Mem trip information as well. The register will be used by HW to enable ORing of the memtrip info into the thermtrip OR tree.
*/


#define THERMTRIP_CONFIG_PCU_FUN2_REG 0x042200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 memtrip0_to_xxthermtrip_n_en : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, Punit memtrip network 0 is enabled to
                               cause xxTHERMTRIP_N pin assertion. The specific
                               meaning and usage model of Memtrip network 0 is
                               defined by the SoC architecture and integration.
                            */
    UINT32 memtrip1_to_xxthermtrip_n_en : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, Punit memtrip network 1 is enabled to
                               cause xxTHERMTRIP_N pin assertion. The specific
                               meaning and usage model of Memtrip network 1 is
                               defined by the SoC architecture and integration.
                            */
    UINT32 memtrip0_to_xxmemtrip_n_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set, Punit memtrip network 0 is enabled to
                               cause xxMEMTRIP_N pin assertion. The specific
                               meaning and usage model of Memtrip network 0 is
                               defined by the SoC architecture and integration.
                            */
    UINT32 memtrip1_to_xxmemtrip_n_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set, Punit memtrip network 1 is enabled to
                               cause xxMEMTRIP_N pin assertion. The specific
                               meaning and usage model of Memtrip network 1 is
                               defined by the SoC architecture and integration.
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} THERMTRIP_CONFIG_PCU_FUN2_STRUCT;

/* PMONPCODEFILTER_PCU_FUN2_REG supported on:                                   */
/*      SPRA0 (0x21ff20fc)                                                      */
/*      SPRB0 (0x21ff20fc)                                                      */
/*      SPRHBM (0x21ff20fc)                                                     */
/*      SPRC0 (0x21ff20fc)                                                      */
/*      SPRMCC (0x21ff20fc)                                                     */
/*      SPRUCC (0x21ff20fc)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_2*/
/* Struct generated from SPRA0 BDF: 31_30_2                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register has three mappings depending on the type of Perfmon Events that are being counted. 
This register is read by Pcode and communicates the Masking information from the BIOS/SW to Pcode. 
*/


#define PMONPCODEFILTER_PCU_FUN2_REG 0x042200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 filter : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0xFFFFFFFF*/

                            /*
                               Pcode makes the decision on how to interpret the
                               32-bit field Interpretation2: 15:0 ThreadID
                               Interpretation1: 7:0 CoreID Interpretation0:
                               31:24 Voltage/Frequency Range 3 23:16
                               Voltage/Frequency Range 2 15:8 Voltage/Frequency
                               Range 1 7:0 Voltage/Frequency Range 0
                            */

  } Bits;
  UINT32 Data;

} PMONPCODEFILTER_PCU_FUN2_STRUCT;
#endif /* _PCU_FUN2_h */
