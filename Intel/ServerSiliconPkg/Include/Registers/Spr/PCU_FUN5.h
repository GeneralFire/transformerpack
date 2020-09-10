
/** @file
  PCU_FUN5.h

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


#ifndef _PCU_FUN5_h
#define _PCU_FUN5_h
#include <Base.h>

/* VID_1_30_5_PCU_FUN5_REG supported on:                                        */
/*      SPRA0 (0x11ff5000)                                                      */
/*      SPRB0 (0x11ff5000)                                                      */
/*      SPRHBM (0x11ff5000)                                                     */
/*      SPRC0 (0x11ff5000)                                                      */
/*      SPRMCC (0x11ff5000)                                                     */
/*      SPRUCC (0x11ff5000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Vendor ID Register
*/


#define VID_1_30_5_PCU_FUN5_REG 0x04510000

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

} VID_1_30_5_PCU_FUN5_STRUCT;

/* DID_1_30_5_PCU_FUN5_REG supported on:                                        */
/*      SPRA0 (0x11ff5002)                                                      */
/*      SPRB0 (0x11ff5002)                                                      */
/*      SPRHBM (0x11ff5002)                                                     */
/*      SPRC0 (0x11ff5002)                                                      */
/*      SPRMCC (0x11ff5002)                                                     */
/*      SPRUCC (0x11ff5002)                                                     */
/* Register default value on SPRA0: 0x0000325D                                  */
/* Register default value on SPRB0: 0x0000325D                                  */
/* Register default value on SPRHBM: 0x0000325D                                 */
/* Register default value on SPRC0: 0x0000325D                                  */
/* Register default value on SPRMCC: 0x0000325D                                 */
/* Register default value on SPRUCC: 0x0000325D                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Device Identification Number
*/


#define DID_1_30_5_PCU_FUN5_REG 0x04510002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000325D*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_30_5_PCU_FUN5_STRUCT;

/* PCICMD_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x11ff5004)                                                      */
/*      SPRB0 (0x11ff5004)                                                      */
/*      SPRHBM (0x11ff5004)                                                     */
/*      SPRC0 (0x11ff5004)                                                      */
/*      SPRMCC (0x11ff5004)                                                     */
/*      SPRUCC (0x11ff5004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Command Register
*/


#define PCICMD_1_30_5_PCU_FUN5_REG 0x04510004

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

} PCICMD_1_30_5_PCU_FUN5_STRUCT;

/* PCISTS_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x11ff5006)                                                      */
/*      SPRB0 (0x11ff5006)                                                      */
/*      SPRHBM (0x11ff5006)                                                     */
/*      SPRC0 (0x11ff5006)                                                      */
/*      SPRMCC (0x11ff5006)                                                     */
/*      SPRUCC (0x11ff5006)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Status Register
*/


#define PCISTS_1_30_5_PCU_FUN5_REG 0x04510006

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

} PCISTS_1_30_5_PCU_FUN5_STRUCT;

/* RID_CCR_1_30_5_PCU_FUN5_REG supported on:                                    */
/*      SPRA0 (0x21ff5008)                                                      */
/*      SPRB0 (0x21ff5008)                                                      */
/*      SPRHBM (0x21ff5008)                                                     */
/*      SPRC0 (0x21ff5008)                                                      */
/*      SPRMCC (0x21ff5008)                                                     */
/*      SPRUCC (0x21ff5008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_30_5_PCU_FUN5_REG 0x04520008

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

} RID_CCR_1_30_5_PCU_FUN5_STRUCT;

/* CLSR_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x1ff500c)                                                       */
/*      SPRB0 (0x1ff500c)                                                       */
/*      SPRHBM (0x1ff500c)                                                      */
/*      SPRC0 (0x1ff500c)                                                       */
/*      SPRMCC (0x1ff500c)                                                      */
/*      SPRUCC (0x1ff500c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Cache Line Size Register
*/


#define CLSR_1_30_5_PCU_FUN5_REG 0x0450000C

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

} CLSR_1_30_5_PCU_FUN5_STRUCT;

/* PLAT_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x1ff500d)                                                       */
/*      SPRB0 (0x1ff500d)                                                       */
/*      SPRHBM (0x1ff500d)                                                      */
/*      SPRC0 (0x1ff500d)                                                       */
/*      SPRMCC (0x1ff500d)                                                      */
/*      SPRUCC (0x1ff500d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Latency Timer
*/


#define PLAT_1_30_5_PCU_FUN5_REG 0x0450000D

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

} PLAT_1_30_5_PCU_FUN5_STRUCT;

/* HDR_1_30_5_PCU_FUN5_REG supported on:                                        */
/*      SPRA0 (0x1ff500e)                                                       */
/*      SPRB0 (0x1ff500e)                                                       */
/*      SPRHBM (0x1ff500e)                                                      */
/*      SPRC0 (0x1ff500e)                                                       */
/*      SPRMCC (0x1ff500e)                                                      */
/*      SPRUCC (0x1ff500e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Header Type
*/


#define HDR_1_30_5_PCU_FUN5_REG 0x0450000E

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

} HDR_1_30_5_PCU_FUN5_STRUCT;

/* BIST_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x1ff500f)                                                       */
/*      SPRB0 (0x1ff500f)                                                       */
/*      SPRHBM (0x1ff500f)                                                      */
/*      SPRC0 (0x1ff500f)                                                       */
/*      SPRMCC (0x1ff500f)                                                      */
/*      SPRUCC (0x1ff500f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI BIST Register
*/


#define BIST_1_30_5_PCU_FUN5_REG 0x0450000F

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

} BIST_1_30_5_PCU_FUN5_STRUCT;

/* SVID_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x11ff502c)                                                      */
/*      SPRB0 (0x11ff502c)                                                      */
/*      SPRHBM (0x11ff502c)                                                     */
/*      SPRC0 (0x11ff502c)                                                      */
/*      SPRMCC (0x11ff502c)                                                     */
/*      SPRUCC (0x11ff502c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem Vendor ID Register
*/


#define SVID_1_30_5_PCU_FUN5_REG 0x0451002C

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

} SVID_1_30_5_PCU_FUN5_STRUCT;

/* SDID_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x11ff502e)                                                      */
/*      SPRB0 (0x11ff502e)                                                      */
/*      SPRHBM (0x11ff502e)                                                     */
/*      SPRC0 (0x11ff502e)                                                      */
/*      SPRMCC (0x11ff502e)                                                     */
/*      SPRUCC (0x11ff502e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem ID Register
*/


#define SDID_1_30_5_PCU_FUN5_REG 0x0451002E

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

} SDID_1_30_5_PCU_FUN5_STRUCT;

/* CAPPTR_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x1ff5034)                                                       */
/*      SPRB0 (0x1ff5034)                                                       */
/*      SPRHBM (0x1ff5034)                                                      */
/*      SPRC0 (0x1ff5034)                                                       */
/*      SPRMCC (0x1ff5034)                                                      */
/*      SPRUCC (0x1ff5034)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_30_5_PCU_FUN5_REG 0x04500034

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

} CAPPTR_1_30_5_PCU_FUN5_STRUCT;

/* INTL_1_30_5_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x1ff503c)                                                       */
/*      SPRB0 (0x1ff503c)                                                       */
/*      SPRHBM (0x1ff503c)                                                      */
/*      SPRC0 (0x1ff503c)                                                       */
/*      SPRMCC (0x1ff503c)                                                      */
/*      SPRUCC (0x1ff503c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Line Register
*/


#define INTL_1_30_5_PCU_FUN5_REG 0x0450003C

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

} INTL_1_30_5_PCU_FUN5_STRUCT;

/* INTPIN_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x1ff503d)                                                       */
/*      SPRB0 (0x1ff503d)                                                       */
/*      SPRHBM (0x1ff503d)                                                      */
/*      SPRC0 (0x1ff503d)                                                       */
/*      SPRMCC (0x1ff503d)                                                      */
/*      SPRUCC (0x1ff503d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_30_5_PCU_FUN5_REG 0x0450003D

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

} INTPIN_1_30_5_PCU_FUN5_STRUCT;

/* MINGNT_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x1ff503e)                                                       */
/*      SPRB0 (0x1ff503e)                                                       */
/*      SPRHBM (0x1ff503e)                                                      */
/*      SPRC0 (0x1ff503e)                                                       */
/*      SPRMCC (0x1ff503e)                                                      */
/*      SPRUCC (0x1ff503e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Min Grant Register
*/


#define MINGNT_1_30_5_PCU_FUN5_REG 0x0450003E

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

} MINGNT_1_30_5_PCU_FUN5_STRUCT;

/* MAXLAT_1_30_5_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x1ff503f)                                                       */
/*      SPRB0 (0x1ff503f)                                                       */
/*      SPRHBM (0x1ff503f)                                                      */
/*      SPRC0 (0x1ff503f)                                                       */
/*      SPRMCC (0x1ff503f)                                                      */
/*      SPRUCC (0x1ff503f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Max Latency Register
*/


#define MAXLAT_1_30_5_PCU_FUN5_REG 0x0450003F

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

} MAXLAT_1_30_5_PCU_FUN5_STRUCT;

/* MH_SENSE_500NS_PCU_FUN5_REG supported on:                                    */
/*      SPRA0 (0x21ff5080)                                                      */
/*      SPRB0 (0x21ff5080)                                                      */
/*      SPRHBM (0x21ff5080)                                                     */
/*      SPRC0 (0x21ff5080)                                                      */
/*      SPRMCC (0x21ff5080)                                                     */
/*      SPRUCC (0x21ff5080)                                                     */
/* Register default value on SPRA0: 0x00C84032                                  */
/* Register default value on SPRB0: 0x00C84032                                  */
/* Register default value on SPRHBM: 0x00C84032                                 */
/* Register default value on SPRC0: 0x00C84032                                  */
/* Register default value on SPRMCC: 0x00C84032                                 */
/* Register default value on SPRUCC: 0x00C84032                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Input sense assertion and period configuration.
*/


#define MH_SENSE_500NS_PCU_FUN5_REG 0x04520080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnfg_500_nanosec : 10;

                            /* Bits[9:0], Access Type=RW/P, default=0x00000032*/

                            /*
                               500ns equivalent in FXCLK. BIOS calculate number
                               of FXCLK to be equivalent to 500 nanoseconds.
                               CNTR_500_NANOSEC will count up to this value,
                               then reset to zero. For pre-Si validation,
                               minimum 2 can be set to speed up the simulation.
                               The recommended CNFG_500_NANOSEC value based on
                               100MHz FXCLK frequency is 50.
                            */
    UINT32 rsvd : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mh_in_sense_assert : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000002*/

                            /*
                               MEMHOT Input Sense Assertion Time in number of
                               CNTR_500_NANOSEC. BIOS calculate number of
                               CNFG_500_NANOSEC for 1usec/2usec input_sense
                               duration. This field determines the duty cycle
                               between sensing and asserting in the following
                               manner: mh_io_cntr <= mh_in_sense_assert: sense
                               modemh_io_cntr > mh_in_sense_assert: output
                               mode. Here is MH_IN_SENSE_ASSERT ranges: 0 or 1
                               Reserved, 2 - 7 1usec - 3.5usec sense assertion
                               time in 500nsec increment.
                            */
    UINT32 mh_sense_period : 10;

                            /* Bits[25:16], Access Type=RW, default=0x000000C8*/

                            /*
                               MEMHOT Input Sense Period in number of
                               CNTR_500_NANOSEC. Determines how long a complete
                               memhot sense and output cycle is. Must be larger
                               than mh_in_sense_assert.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MH_SENSE_500NS_PCU_FUN5_STRUCT;

/* MH_EXT_STAT_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x21ff5084)                                                      */
/*      SPRB0 (0x21ff5084)                                                      */
/*      SPRHBM (0x21ff5084)                                                     */
/*      SPRC0 (0x21ff5084)                                                      */
/*      SPRMCC (0x21ff5084)                                                     */
/*      SPRUCC (0x21ff5084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Capture externally asserted MEM_HOT[1:0]# assertion detection.
*/


#define MH_EXT_STAT_PCU_FUN5_REG 0x04520084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mh_ext_stat_0 : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               MEM_HOT[0]# assertion status at this sense
                               period. Set if MEM_HOT[0]# is asserted
                               externally for this sense period, this running
                               status bit will automatically updated with the
                               next sensed value in the next MEM_HOT input
                               sense phase.
                            */
    UINT32 mh_ext_stat_1 : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               MEM_HOT[1]# assertion status at this sense
                               period. Set if MEM_HOT[1]# is asserted
                               externally for this sense period, this running
                               status bit will automatically updated with the
                               next sensed value in the next MEM_HOT input
                               sense phase.
                            */
    UINT32 mh_ext_stat_2 : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               MEM_HOT_IN[1]# assertion status for non-purley
                               platform. Set if MEM HOT pins are working in uni
                               directional mode.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MH_EXT_STAT_PCU_FUN5_STRUCT;

/* MH_MAINCTL_PCU_FUN5_REG supported on:                                        */
/*      SPRA0 (0x21ff5088)                                                      */
/*      SPRB0 (0x21ff5088)                                                      */
/*      SPRHBM (0x21ff5088)                                                     */
/*      SPRC0 (0x21ff5088)                                                      */
/*      SPRMCC (0x21ff5088)                                                     */
/*      SPRUCC (0x21ff5088)                                                     */
/* Register default value on SPRA0: 0x00008000                                  */
/* Register default value on SPRB0: 0x00008000                                  */
/* Register default value on SPRHBM: 0x00008000                                 */
/* Register default value on SPRC0: 0x00008000                                  */
/* Register default value on SPRMCC: 0x00008000                                 */
/* Register default value on SPRUCC: 0x00008000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MEMHOT control configuration
*/


#define MH_MAINCTL_PCU_FUN5_REG 0x04520088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mh_pin_unidirection_mode : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables MEM_HOT Pins to change to uni-
                               directional pins for non-purley platform. When
                               0, the MEM_HOT_0/1 pins are bi-directional pins
                               and same pin is used for sensing as well as
                               driving output. When 1, MEM_HOT_0 will act as
                               MEM_HOT_OUT and MEM_HOT_1 will act as MEM_HOT_IN
                               by default. This polarity can be swrapped by
                               using bit CHICKEN_BIT_SWAP_MHIO_PINS. Refer to
                               CHICKEN_BIT_SWAP_MHIO_PINS description for more
                               info.
                            */
    UINT32 rsvd : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 memhot0_to_xxmemhot0_out_en : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[0] to xxmemhot0 output pin
                               for Purley Platform Bi-directional mode. When
                               set, memhot0 coming from PMAs will drive
                               xxmemhot0 output pin.
                            */
    UINT32 memhot1_to_xxmemhot0_out_en : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[1] to xxmemhot0 output pin
                               for Purley Platform Bi-directional mode. When
                               set, memhot1 coming from PMAs will drive
                               xxmemhot0 output pin.
                            */
    UINT32 memhot0_to_xxmemhot1_out_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[0] to xxmemhot1 output pin
                               for Purley Platform Bi-directional mode. When
                               set, memhot0 coming from PMAs will drive
                               xxmemhot1 output pin.
                            */
    UINT32 memhot1_to_xxmemhot1_out_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[1] to xxmemhot1 output pin
                               for Purley Platform Bi-directional mode. When
                               set, memhot1 coming from PMAs will drive
                               xxmemhot1 output pin.
                            */
    UINT32 xxmemhot0_in_to_memhot0_en : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               xxmemhot0 pin input to punit_pma_temp_hot[0] for
                               Purley Platform Bi-directional mode. When set,
                               xxmemhot0_in indication coming from GPIO pin
                               will drive memhot0 output from Punit to PMA.
                            */
    UINT32 xxmemhot0_in_to_memhot1_en : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               xxmemhot0 pin input to punit_pma_temp_hot[1] for
                               Purley Platform Bi-directional mode. When set,
                               xxmemhot0_in indication coming from GPIO pin
                               will drive memhot1 output from Punit to PMA.
                            */
    UINT32 xxmemhot1_in_to_memhot0_en : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               xxmemhot1 pin input to punit_pma_temp_hot[0] for
                               Purley Platform Bi-directional mode. When set,
                               xxmemhot1_in indication coming from GPIO pin
                               will drive memhot0 output from Punit to PMA.
                            */
    UINT32 xxmemhot1_in_to_memhot1_en : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               xxmemhot1 pin input to punit_pma_temp_hot[1] for
                               Purley Platform Bi-directional mode. When set,
                               xxmemhot1_in indication coming from GPIO pin
                               will drive memhot1 output from Punit to PMA.
                            */
    UINT32 rsvd_12 : 3;

                            /* Bits[14:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mh_output_en : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               MEM_HOT output generation logic enable control.
                               When 0, the MEM_HOT output generation logic is
                               disabled, i.e. MEM_HOT[1:0]# outputs are in de-
                               asserted state, no assertion regardless of the
                               memory temperature. Sensing of externally
                               asserted MEM_HOT[1:0]# is not affected by this
                               bit. iMC will always reset the MH1_DIMM_VAL and
                               MH0_DIMM_VAL bits in the next DCLK so there is
                               no impact to the PCODE update to the
                               MH_TEMP_STAT registers. When 1, the MEM_HOT
                               output generation logic is enabled.
                            */
    UINT32 mh_sense_en : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Externally asserted MEM_HOTsense control enable
                               bit. When set, the MEM_HOT sense logic is
                               enabled.
                            */
    UINT32 mhot_smi_en : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Generate SMI during internal MEMHOT# event
                               assertion. When set, the MEM_HOT sense logic is
                               enabled.
                            */
    UINT32 mhot_ext_smi_en : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Generate SMI event when either MEMHOT[1:0]# is
                               externally asserted. When set, the MEM_HOT sense
                               logic is enabled.
                            */
    UINT32 rsvd_19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 unidir_memhot0_to_memhot_out_en : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[0] to xxmemhot output pin.
                               When set, memhot0 coming from PMAs will drive
                               xxmemhot output pin.
                            */
    UINT32 unidir_memhot1_to_memhot_out_en : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               pma_punit_temp_hot[1] to xxmemhot output pin.
                               When set, memhot1 coming from PMAs will drive
                               xxmemhot output pin.
                            */
    UINT32 unidir_memhot_in_en : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configuration mask to enable/disable routing of
                               memhot_in indication in Uni-direction mode. When
                               set, memhot_in indication coming from xxmemhot
                               will propagate to PMAs.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MH_MAINCTL_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_0_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff5090)                                                      */
/*      SPRB0 (0x21ff5090)                                                      */
/*      SPRHBM (0x21ff5090)                                                     */
/*      SPRC0 (0x21ff5090)                                                      */
/*      SPRMCC (0x21ff5090)                                                     */
/*      SPRUCC (0x21ff5090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_0_PCU_FUN5_REG 0x04520090

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

} DRAM_ENERGY_STATUS_CH_CFG_0_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_1_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff5094)                                                      */
/*      SPRB0 (0x21ff5094)                                                      */
/*      SPRHBM (0x21ff5094)                                                     */
/*      SPRC0 (0x21ff5094)                                                      */
/*      SPRMCC (0x21ff5094)                                                     */
/*      SPRUCC (0x21ff5094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_1_PCU_FUN5_REG 0x04520094

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

} DRAM_ENERGY_STATUS_CH_CFG_1_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_2_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff5098)                                                      */
/*      SPRB0 (0x21ff5098)                                                      */
/*      SPRHBM (0x21ff5098)                                                     */
/*      SPRC0 (0x21ff5098)                                                      */
/*      SPRMCC (0x21ff5098)                                                     */
/*      SPRUCC (0x21ff5098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_2_PCU_FUN5_REG 0x04520098

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

} DRAM_ENERGY_STATUS_CH_CFG_2_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_3_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff509c)                                                      */
/*      SPRB0 (0x21ff509c)                                                      */
/*      SPRHBM (0x21ff509c)                                                     */
/*      SPRC0 (0x21ff509c)                                                      */
/*      SPRMCC (0x21ff509c)                                                     */
/*      SPRUCC (0x21ff509c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_3_PCU_FUN5_REG 0x0452009C

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

} DRAM_ENERGY_STATUS_CH_CFG_3_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_4_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50a0)                                                      */
/*      SPRB0 (0x21ff50a0)                                                      */
/*      SPRHBM (0x21ff50a0)                                                     */
/*      SPRC0 (0x21ff50a0)                                                      */
/*      SPRMCC (0x21ff50a0)                                                     */
/*      SPRUCC (0x21ff50a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_4_PCU_FUN5_REG 0x045200A0

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

} DRAM_ENERGY_STATUS_CH_CFG_4_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_5_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50a4)                                                      */
/*      SPRB0 (0x21ff50a4)                                                      */
/*      SPRHBM (0x21ff50a4)                                                     */
/*      SPRC0 (0x21ff50a4)                                                      */
/*      SPRMCC (0x21ff50a4)                                                     */
/*      SPRUCC (0x21ff50a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_5_PCU_FUN5_REG 0x045200A4

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

} DRAM_ENERGY_STATUS_CH_CFG_5_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_6_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50a8)                                                      */
/*      SPRB0 (0x21ff50a8)                                                      */
/*      SPRHBM (0x21ff50a8)                                                     */
/*      SPRC0 (0x21ff50a8)                                                      */
/*      SPRMCC (0x21ff50a8)                                                     */
/*      SPRUCC (0x21ff50a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_6_PCU_FUN5_REG 0x045200A8

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

} DRAM_ENERGY_STATUS_CH_CFG_6_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_7_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50ac)                                                      */
/*      SPRB0 (0x21ff50ac)                                                      */
/*      SPRHBM (0x21ff50ac)                                                     */
/*      SPRC0 (0x21ff50ac)                                                      */
/*      SPRMCC (0x21ff50ac)                                                     */
/*      SPRUCC (0x21ff50ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_7_PCU_FUN5_REG 0x045200AC

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

} DRAM_ENERGY_STATUS_CH_CFG_7_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_8_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50b0)                                                      */
/*      SPRB0 (0x21ff50b0)                                                      */
/*      SPRHBM (0x21ff50b0)                                                     */
/*      SPRC0 (0x21ff50b0)                                                      */
/*      SPRMCC (0x21ff50b0)                                                     */
/*      SPRUCC (0x21ff50b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_8_PCU_FUN5_REG 0x045200B0

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

} DRAM_ENERGY_STATUS_CH_CFG_8_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_9_PCU_FUN5_REG supported on:                       */
/*      SPRA0 (0x21ff50b4)                                                      */
/*      SPRB0 (0x21ff50b4)                                                      */
/*      SPRHBM (0x21ff50b4)                                                     */
/*      SPRC0 (0x21ff50b4)                                                      */
/*      SPRMCC (0x21ff50b4)                                                     */
/*      SPRUCC (0x21ff50b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_9_PCU_FUN5_REG 0x045200B4

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

} DRAM_ENERGY_STATUS_CH_CFG_9_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_10_PCU_FUN5_REG supported on:                      */
/*      SPRA0 (0x21ff50b8)                                                      */
/*      SPRB0 (0x21ff50b8)                                                      */
/*      SPRHBM (0x21ff50b8)                                                     */
/*      SPRC0 (0x21ff50b8)                                                      */
/*      SPRMCC (0x21ff50b8)                                                     */
/*      SPRUCC (0x21ff50b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_10_PCU_FUN5_REG 0x045200B8

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

} DRAM_ENERGY_STATUS_CH_CFG_10_PCU_FUN5_STRUCT;

/* DRAM_ENERGY_STATUS_CH_CFG_11_PCU_FUN5_REG supported on:                      */
/*      SPRA0 (0x21ff50bc)                                                      */
/*      SPRB0 (0x21ff50bc)                                                      */
/*      SPRHBM (0x21ff50bc)                                                     */
/*      SPRC0 (0x21ff50bc)                                                      */
/*      SPRMCC (0x21ff50bc)                                                     */
/*      SPRUCC (0x21ff50bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around and continue counting when it reaches its limit.
     ENERGY_UNIT for DRAM domain is 15.3uJ.
     The data is updated by PCODE and is Read Only for all SW.
*/


#define DRAM_ENERGY_STATUS_CH_CFG_11_PCU_FUN5_REG 0x045200BC

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

} DRAM_ENERGY_STATUS_CH_CFG_11_PCU_FUN5_STRUCT;


/* SMT_CONTROL_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x21ff50d4)                                                      */
/*      SPRB0 (0x21ff50d4)                                                      */
/*      SPRHBM (0x21ff50d4)                                                     */
/*      SPRC0 (0x21ff50d4)                                                      */
/*      SPRMCC (0x21ff50d4)                                                     */
/*      SPRUCC (0x21ff50d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* SMT Control Register
*/


#define SMT_CONTROL_PCU_FUN5_REG 0x045200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 thread_mask : 4;

                            /* Bits[11:8], Access Type=RO/P, default=0x00000000*/

                            /*
                               Thread Mask indicates which threads are enabled
                               in the core. CNX: The LSB is the enable bit for
                               Thread 0, whereas the MSB is the enable bit for
                               Thread 1. This field is determined by FW based
                               on CSR_DESIRED_CORES[SMT_DISABLE]. KNH: both
                               cores have the same mask. The value in
                               THREAD_MASK is the OR-function of Intel's Fuse
                               setting and BIOS setting. This field is
                               determined by FW based on
                               CSR_DESIRED_CORES[SMT_DISABLE] and
                               PCU_CR_FUSED_CORES[FUSED_THREAD_MASK].
                            */
    UINT32 rsvd_12 : 12;

                            /* Bits[23:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fused_smt_capability : 1;

                            /* Bits[24:24], Access Type=RO/P, default=0x00000000*/

                            /*
                               Enabled threads in the package. 0b 1 thread 1b 2
                               threads KNH: 0 : Single Thread 1 : 4 thread /
                               SMT4 Single Thread (ST) mode. The THREAD_MASK
                               for a Core are: a) 4'b0001 b) 4'b0010 c) 4'b0100
                               d) 4'b1000 SMT4 is enabled. The THREAD_MASK for
                               a Core is 4'b1111. This value represents the
                               inverted value of the HT_DIS fuse
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMT_CONTROL_PCU_FUN5_STRUCT;

/* PCU_LT_CTRL_PCU_FUN5_REG supported on:                                       */
/*      SPRA0 (0x21ff50d8)                                                      */
/*      SPRB0 (0x21ff50d8)                                                      */
/*      SPRHBM (0x21ff50d8)                                                     */
/*      SPRC0 (0x21ff50d8)                                                      */
/*      SPRMCC (0x21ff50d8)                                                     */
/*      SPRUCC (0x21ff50d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PCU Intel TXT Control Attributes
*/


#define PCU_LT_CTRL_PCU_FUN5_REG 0x045200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_spare : 31;

                            /* Bits[30:0], Access Type=RW, default=0x00000000*/

                            /* Reserved for BIOS */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RW/O, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PCU_LT_CTRL_PCU_FUN5_STRUCT;

/* PCU_BIOS_SPARE2_PCU_FUN5_REG supported on:                                   */
/*      SPRA0 (0x21ff50dc)                                                      */
/*      SPRB0 (0x21ff50dc)                                                      */
/*      SPRHBM (0x21ff50dc)                                                     */
/*      SPRC0 (0x21ff50dc)                                                      */
/*      SPRMCC (0x21ff50dc)                                                     */
/*      SPRUCC (0x21ff50dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Miscellaneous configuration bits used to communicate between BIOS and PCU   
*/


#define PCU_BIOS_SPARE2_PCU_FUN5_REG 0x045200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_spare : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               <p>[0:0] - Set by BIOS to request legacy APIC ID
                               assignment.</p> <p>[11:1] - Reserved</p>
                               <p>[13:12] - 2 bit encoding of connected DIMM
                               type:</p> <p style="margin-left:
                               40px;">2&#39;b00 - UDIMM</p> <p style="margin-
                               left: 40px;">2&#39;b01 - RDIMM</p> <p
                               style="margin-left: 40px;">2&#39;b10 -
                               LRDIMM</p> <p style="margin-left:
                               40px;">2&#39;b11 - Reserved</p> <p>[15:14] - 2
                               bit encoding of the Vddrq voltage level:</p> <p
                               style="margin-left: 40px;">2&#39;b00 - 1.25
                               V</p> <p style="margin-left: 40px;">2&#39;b01 -
                               1.35 V</p> <p style="margin-left:
                               40px;">2&#39;b10 - 1.50 V</p> <p style="margin-
                               left: 40px;">2&#39;b11 - Reserved</p> <p>[16:16]
                               - Set by Pcode&nbsp;to indicate surprise reset
                               was detected.</p> <p>[17:17] - Reserved.</p>
                               <p>[18:18] - Used to select between Legacy ADR
                               behavior and the new ADR flow; if clear, use
                               legacy&nbsp;behavior(default).</p> <p>[19:19] -
                               Set by BIOS to enable PECI to control Energy
                               Performance BIAS out of band.</p> <p>[20:20] -
                               Set by BIOS to&nbsp;indicate WFR is present.</p>
                               <p>[21:21] - Set by Pcode to indicate ADR_DONE
                               for enhanced ADR flow.</p> <p>[30:22] -
                               Unused</p> <p>[31:31] -&nbsp;If set, enable GV
                               block for memory sparing.</p>
                            */

  } Bits;
  UINT32 Data;

} PCU_BIOS_SPARE2_PCU_FUN5_STRUCT;

/* PWR_LIMIT_MISC_INFO_PCU_FUN5_REG supported on:                               */
/*      SPRA0 (0x21ff50e0)                                                      */
/*      SPRB0 (0x21ff50e0)                                                      */
/*      SPRHBM (0x21ff50e0)                                                     */
/*      SPRC0 (0x21ff50e0)                                                      */
/*      SPRMCC (0x21ff50e0)                                                     */
/*      SPRUCC (0x21ff50e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Power Limit Miscellaneous Information register
*/


#define PWR_LIMIT_MISC_INFO_PCU_FUN5_REG 0x045200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pn_power_of_sku : 15;

                            /* Bits[14:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The socket power at Pn, the most efficient
                               frequency or the knee of V-F curve.
                            */
    UINT32 pbm_minimal_tau_allowed : 7;

                            /* Bits[21:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               The minimum time window for PBM algorithm. The
                               time window over which average power consumption
                               of the socket is regulated.
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PWR_LIMIT_MISC_INFO_PCU_FUN5_STRUCT;

/* TSYNC_CPL_LOG_PCU_FUN5_REG supported on:                                     */
/*      SPRA0 (0x21ff50f0)                                                      */
/*      SPRB0 (0x21ff50f0)                                                      */
/*      SPRHBM (0x21ff50f0)                                                     */
/*      SPRC0 (0x21ff50f0)                                                      */
/*      SPRMCC (0x21ff50f0)                                                     */
/*      SPRUCC (0x21ff50f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_5*/
/* Struct generated from SPRA0 BDF: 31_30_5                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register contains debug information regarding an erroneous completion. It will only be written if cpl_ur and cpl_uc (in ART_STS) are clear when the completion arrives.
*/


#define TSYNC_CPL_LOG_PCU_FUN5_REG 0x045200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 src : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* Logical Source Port ID */
    UINT32 opcode : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Opcode */
    UINT32 tag : 3;

                            /* Bits[26:24], Access Type=RO/V, default=0x00000000*/

                            /* Tag */
    UINT32 rsp : 2;

                            /* Bits[28:27], Access Type=RO/V, default=0x00000000*/

                            /* Response */
    UINT32 rsvd1 : 3;

                            /* Bits[31:29], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSYNC_CPL_LOG_PCU_FUN5_STRUCT;
#endif /* _PCU_FUN5_h */
