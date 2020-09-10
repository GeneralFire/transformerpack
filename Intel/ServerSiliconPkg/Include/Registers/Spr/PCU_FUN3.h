
/** @file
  PCU_FUN3.h

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


#ifndef _PCU_FUN3_h
#define _PCU_FUN3_h
#include <Base.h>

/* VID_1_30_3_PCU_FUN3_REG supported on:                                        */
/*      SPRA0 (0x11ff3000)                                                      */
/*      SPRB0 (0x11ff3000)                                                      */
/*      SPRHBM (0x11ff3000)                                                     */
/*      SPRC0 (0x11ff3000)                                                      */
/*      SPRMCC (0x11ff3000)                                                     */
/*      SPRUCC (0x11ff3000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Vendor ID Register
*/


#define VID_1_30_3_PCU_FUN3_REG 0x04310000

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

} VID_1_30_3_PCU_FUN3_STRUCT;

/* DID_1_30_3_PCU_FUN3_REG supported on:                                        */
/*      SPRA0 (0x11ff3002)                                                      */
/*      SPRB0 (0x11ff3002)                                                      */
/*      SPRHBM (0x11ff3002)                                                     */
/*      SPRC0 (0x11ff3002)                                                      */
/*      SPRMCC (0x11ff3002)                                                     */
/*      SPRUCC (0x11ff3002)                                                     */
/* Register default value on SPRA0: 0x0000325B                                  */
/* Register default value on SPRB0: 0x0000325B                                  */
/* Register default value on SPRHBM: 0x0000325B                                 */
/* Register default value on SPRC0: 0x0000325B                                  */
/* Register default value on SPRMCC: 0x0000325B                                 */
/* Register default value on SPRUCC: 0x0000325B                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Device Identification Number
*/


#define DID_1_30_3_PCU_FUN3_REG 0x04310002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000325B*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_30_3_PCU_FUN3_STRUCT;

/* PCICMD_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x11ff3004)                                                      */
/*      SPRB0 (0x11ff3004)                                                      */
/*      SPRHBM (0x11ff3004)                                                     */
/*      SPRC0 (0x11ff3004)                                                      */
/*      SPRMCC (0x11ff3004)                                                     */
/*      SPRUCC (0x11ff3004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Command Register
*/


#define PCICMD_1_30_3_PCU_FUN3_REG 0x04310004

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

} PCICMD_1_30_3_PCU_FUN3_STRUCT;

/* PCISTS_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x11ff3006)                                                      */
/*      SPRB0 (0x11ff3006)                                                      */
/*      SPRHBM (0x11ff3006)                                                     */
/*      SPRC0 (0x11ff3006)                                                      */
/*      SPRMCC (0x11ff3006)                                                     */
/*      SPRUCC (0x11ff3006)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Status Register
*/


#define PCISTS_1_30_3_PCU_FUN3_REG 0x04310006

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

} PCISTS_1_30_3_PCU_FUN3_STRUCT;

/* RID_CCR_1_30_3_PCU_FUN3_REG supported on:                                    */
/*      SPRA0 (0x21ff3008)                                                      */
/*      SPRB0 (0x21ff3008)                                                      */
/*      SPRHBM (0x21ff3008)                                                     */
/*      SPRC0 (0x21ff3008)                                                      */
/*      SPRMCC (0x21ff3008)                                                     */
/*      SPRUCC (0x21ff3008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_30_3_PCU_FUN3_REG 0x04320008

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

} RID_CCR_1_30_3_PCU_FUN3_STRUCT;

/* CLSR_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x1ff300c)                                                       */
/*      SPRB0 (0x1ff300c)                                                       */
/*      SPRHBM (0x1ff300c)                                                      */
/*      SPRC0 (0x1ff300c)                                                       */
/*      SPRMCC (0x1ff300c)                                                      */
/*      SPRUCC (0x1ff300c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Cache Line Size Register
*/


#define CLSR_1_30_3_PCU_FUN3_REG 0x0430000C

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

} CLSR_1_30_3_PCU_FUN3_STRUCT;

/* PLAT_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x1ff300d)                                                       */
/*      SPRB0 (0x1ff300d)                                                       */
/*      SPRHBM (0x1ff300d)                                                      */
/*      SPRC0 (0x1ff300d)                                                       */
/*      SPRMCC (0x1ff300d)                                                      */
/*      SPRUCC (0x1ff300d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Latency Timer
*/


#define PLAT_1_30_3_PCU_FUN3_REG 0x0430000D

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

} PLAT_1_30_3_PCU_FUN3_STRUCT;

/* HDR_1_30_3_PCU_FUN3_REG supported on:                                        */
/*      SPRA0 (0x1ff300e)                                                       */
/*      SPRB0 (0x1ff300e)                                                       */
/*      SPRHBM (0x1ff300e)                                                      */
/*      SPRC0 (0x1ff300e)                                                       */
/*      SPRMCC (0x1ff300e)                                                      */
/*      SPRUCC (0x1ff300e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Header Type
*/


#define HDR_1_30_3_PCU_FUN3_REG 0x0430000E

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

} HDR_1_30_3_PCU_FUN3_STRUCT;

/* BIST_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x1ff300f)                                                       */
/*      SPRB0 (0x1ff300f)                                                       */
/*      SPRHBM (0x1ff300f)                                                      */
/*      SPRC0 (0x1ff300f)                                                       */
/*      SPRMCC (0x1ff300f)                                                      */
/*      SPRUCC (0x1ff300f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI BIST Register
*/


#define BIST_1_30_3_PCU_FUN3_REG 0x0430000F

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

} BIST_1_30_3_PCU_FUN3_STRUCT;

/* SVID_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x11ff302c)                                                      */
/*      SPRB0 (0x11ff302c)                                                      */
/*      SPRHBM (0x11ff302c)                                                     */
/*      SPRC0 (0x11ff302c)                                                      */
/*      SPRMCC (0x11ff302c)                                                     */
/*      SPRUCC (0x11ff302c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem Vendor ID Register
*/


#define SVID_1_30_3_PCU_FUN3_REG 0x0431002C

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

} SVID_1_30_3_PCU_FUN3_STRUCT;

/* SDID_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x11ff302e)                                                      */
/*      SPRB0 (0x11ff302e)                                                      */
/*      SPRHBM (0x11ff302e)                                                     */
/*      SPRC0 (0x11ff302e)                                                      */
/*      SPRMCC (0x11ff302e)                                                     */
/*      SPRUCC (0x11ff302e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem ID Register
*/


#define SDID_1_30_3_PCU_FUN3_REG 0x0431002E

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

} SDID_1_30_3_PCU_FUN3_STRUCT;

/* CAPPTR_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x1ff3034)                                                       */
/*      SPRB0 (0x1ff3034)                                                       */
/*      SPRHBM (0x1ff3034)                                                      */
/*      SPRC0 (0x1ff3034)                                                       */
/*      SPRMCC (0x1ff3034)                                                      */
/*      SPRUCC (0x1ff3034)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_30_3_PCU_FUN3_REG 0x04300034

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

} CAPPTR_1_30_3_PCU_FUN3_STRUCT;

/* INTL_1_30_3_PCU_FUN3_REG supported on:                                       */
/*      SPRA0 (0x1ff303c)                                                       */
/*      SPRB0 (0x1ff303c)                                                       */
/*      SPRHBM (0x1ff303c)                                                      */
/*      SPRC0 (0x1ff303c)                                                       */
/*      SPRMCC (0x1ff303c)                                                      */
/*      SPRUCC (0x1ff303c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Line Register
*/


#define INTL_1_30_3_PCU_FUN3_REG 0x0430003C

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

} INTL_1_30_3_PCU_FUN3_STRUCT;

/* INTPIN_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x1ff303d)                                                       */
/*      SPRB0 (0x1ff303d)                                                       */
/*      SPRHBM (0x1ff303d)                                                      */
/*      SPRC0 (0x1ff303d)                                                       */
/*      SPRMCC (0x1ff303d)                                                      */
/*      SPRUCC (0x1ff303d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_30_3_PCU_FUN3_REG 0x0430003D

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

} INTPIN_1_30_3_PCU_FUN3_STRUCT;

/* MINGNT_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x1ff303e)                                                       */
/*      SPRB0 (0x1ff303e)                                                       */
/*      SPRHBM (0x1ff303e)                                                      */
/*      SPRC0 (0x1ff303e)                                                       */
/*      SPRMCC (0x1ff303e)                                                      */
/*      SPRUCC (0x1ff303e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Min Grant Register
*/


#define MINGNT_1_30_3_PCU_FUN3_REG 0x0430003E

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

} MINGNT_1_30_3_PCU_FUN3_STRUCT;

/* MAXLAT_1_30_3_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x1ff303f)                                                       */
/*      SPRB0 (0x1ff303f)                                                       */
/*      SPRHBM (0x1ff303f)                                                      */
/*      SPRC0 (0x1ff303f)                                                       */
/*      SPRMCC (0x1ff303f)                                                      */
/*      SPRUCC (0x1ff303f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Max Latency Register
*/


#define MAXLAT_1_30_3_PCU_FUN3_REG 0x0430003F

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

} MAXLAT_1_30_3_PCU_FUN3_STRUCT;


/* CAPID0_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff3084)                                                      */
/*      SPRB0 (0x21ff3084)                                                      */
/*      SPRHBM (0x21ff3084)                                                     */
/*      SPRC0 (0x21ff3084)                                                      */
/*      SPRMCC (0x21ff3084)                                                     */
/*      SPRUCC (0x21ff3084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID0_PCU_FUN3_REG 0x04320084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wayness : 2;

                            /* Bits[1:0], Access Type=RO/V, default=0x00000000*/

                            /* Max wayness. 00=1S, 01=2S, 10 = 4S, 11 = 8S */
    UINT32 scalable : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* If 1, implies S2S or S4S */
    UINT32 segment : 3;

                            /* Bits[5:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>111: Server;</p> <p>011: Server-FPGA;</p>
                               <p>001: Server-Fabric;</p> <p>100: HPC;</p>
                               <p>110: Server-Atom;</p> <p>010:
                               Workstation;</p> <p>000: HEDT;</p> <p>Others:
                               Reserved;</p>
                            */
    UINT32 capid0_6 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid0_7 : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 prg_tdp_lim_en : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* TURBO_POWER_LIMIT MSRs usage enabled */
    UINT32 llc_way_en : 3;

                            /* Bits[11:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicate LLC ways enabled 000: &nbsp;&nbsp;
                               &nbsp;11 ways; 001: &nbsp;&nbsp; &nbsp;12 ways;
                               010: &nbsp;&nbsp; &nbsp;2 ways; 011:
                               &nbsp;&nbsp; &nbsp;8 ways; 100: &nbsp;&nbsp;
                               &nbsp;20 ways
                            */
    UINT32 ht_dis : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Multi threading disabled */
    UINT32 capid0_13 : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 capid0_14 : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 vt_x3_en : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* VT-x3 enabled */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vmx_dis : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* VMX disabled */
    UINT32 smx_dis : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* SMX disabled */
    UINT32 lt_production : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>Intel Trusted Execution Technology support is
                               enabled in the platform.</p> <p>LT Production
                               fuse<br> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               Ilegal combination (should not be fused)<br>
                               &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; SMX(LT)
                               enable &nbsp; &nbsp;FIT boot enable &nbsp;
                               &nbsp; &nbsp; &nbsp; LT Production &nbsp; &nbsp;
                               &nbsp;Remark<br> &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp;0 &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; &nbsp; 0 &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; 1 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; LT
                               is disabled &nbsp;no need for production fuse
                               setting<br> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp;0 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; 1 &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               0 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; FIT boot is
                               only enabled if LT is enabled &nbsp; &nbsp;
                               &nbsp; &nbsp;<br> &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp;0 &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; &nbsp; 1 &nbsp; &nbsp; &nbsp;
                               &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                               &nbsp; 1 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; FIT
                               boot is only enabled if LT is enable</p>
                            */
    UINT32 lt_sx_en : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Intel TXT and FIT-boot Enable */
    UINT32 lt_smm_inhibit : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               LT for handling of SMI inhibit with opt-out SMM
                               enabled
                            */
    UINT32 tsc_deadline_dis : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /*
                               APIC timer last tick relative mode: Support for
                               TSC Deadline disabled
                            */
    UINT32 aes_dis : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /* Advanced Encryption Standard (AES) disabled */
    UINT32 capid0_24 : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 xsaveopt_dis : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /*
                               XSAVEOPT disabled. XSAVEOPT will be disable if
                               set.
                            */
    UINT32 gsse256_dis : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* All GSSE instructions will be disabled if set. */
    UINT32 capid0_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 art_dis : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /*
                               SparDisable support for Always Running APIC
                               Timer. ART (Always Running APIC Timer) function
                               in the APIC (enable legacy timer) disabled
                            */
    UINT32 capid0_29 : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* <p>RSVD</p> */
    UINT32 dcu_mode : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               DCU mode (as set by MSR (31h) DCU_MODE) enabled
                               0: normal 1: 16K 1/2 size ECC mode
                            */
    UINT32 capid0_31 : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} CAPID0_PCU_FUN3_STRUCT;

/* CAPID1_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff3088)                                                      */
/*      SPRB0 (0x21ff3088)                                                      */
/*      SPRHBM (0x21ff3088)                                                     */
/*      SPRC0 (0x21ff3088)                                                      */
/*      SPRMCC (0x21ff3088)                                                     */
/*      SPRUCC (0x21ff3088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID1_PCU_FUN3_REG 0x04320088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tph_en : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* TLP Processing Hint Enabled */
    UINT32 core_ras_en : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* Data Poisoning, MCA recovery enabled */
    UINT32 rsvd_2 : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 gv3_dis : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               GV3 disabled. Does not allow for the writing of
                               the IA32_PERF_CONTROL register in order to
                               change ratios
                            */
    UINT32 pwrbits_dis : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               0b Power features activated during reset 1b
                               Power features (especially clock gating) are not
                               activated
                            */
    UINT32 cpu_hot_add_en : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* Intel TXT - CPU HOT ADD enabled */
    UINT32 x2apic_en : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>Enable Extended APIC support.&nbsp;When set
                               enables the support of x2APIC (Extended APIC) in
                               the core and uncore.</p> <p>The fuse being set
                               will impact :<br> &nbsp; &nbsp;a. CPUID
                               indication x2APIC support<br> &nbsp; &nbsp;b.
                               uCode ability to enable x2APIC support<br>
                               &nbsp; &nbsp;c. unCore ability to generate and
                               send x2APIC messages.</p>
                            */
    UINT32 capid1_8 : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid1_9 : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1.[12&nbsp;downto 9] indicate supported
                               speed(s) for HBM memory. Any attempt to pick up
                               an unsupported value will be ignored.&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>[3:3] - If set, over-clocking is supported
                               and bits [2:0] are ignored.&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;</p> <p>[2:0] - Maximum
                               allowed memory frequency.&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>3b000 - up to HBM-800&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;</p>
                               <p>3b001 - up to HBM-1000&nbsp;&nbsp;&nbsp;</p>
                               <p>3b010 - up to HBM-1200&nbsp;&nbsp;&nbsp;</p>
                               <p>3b011 - up to HBM-1600&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp; &nbsp;&nbsp;&nbsp;</p>
                            */
    UINT32 capid1_10 : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1.[12&nbsp;downto 9] indicate supported
                               speed(s) for HBM memory. Any attempt to pick up
                               an unsupported value will be ignored.&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>[3:3] - If set, over-clocking is supported
                               and bits [2:0] are ignored.&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;</p> <p>[2:0] - Maximum
                               allowed memory frequency.&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>3b000 - up to HBM-800&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;</p>
                               <p>3b001 - up to HBM-1000&nbsp;&nbsp;&nbsp;</p>
                               <p>3b010 - up to HBM-1200&nbsp;&nbsp;&nbsp;</p>
                               <p>3b011 - up to HBM-1600&nbsp;&nbsp;</p>
                            */
    UINT32 capid1_11 : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1.[12&nbsp;downto 9] indicate supported
                               speed(s) for HBM memory. Any attempt to pick up
                               an unsupported value will be ignored.&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>[3:3] - If set, over-clocking is supported
                               and bits [2:0] are ignored.&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;</p> <p>[2:0] - Maximum
                               allowed memory frequency.&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>3b000 - up to HBM-800&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;</p>
                               <p>3b001 - up to HBM-1000&nbsp;&nbsp;&nbsp;</p>
                               <p>3b010 - up to HBM-1200&nbsp;&nbsp;&nbsp;</p>
                               <p>3b011 - up to HBM-1600&nbsp;</p>
                            */
    UINT32 capid1_12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1.[12&nbsp;downto 9] indicate supported
                               speed(s) for HBM memory. Any attempt to pick up
                               an unsupported value will be ignored.&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>[3:3] - If set, over-clocking is supported
                               and bits [2:0] are ignored.&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;</p> <p>[2:0] - Maximum
                               allowed memory frequency.&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp
                               ;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
                               <p>3b000 - up to HBM-800&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                               &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;</p>
                               <p>3b001 - up to HBM-1000&nbsp;&nbsp;&nbsp;</p>
                               <p>3b010 - up to HBM-1200&nbsp;&nbsp;&nbsp;</p>
                               <p>3b011 - up to HBM-1600&nbsp;</p>
                            */
    UINT32 disable_2lm : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* To Disable 2LM */
    UINT32 disable_ddrt : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable the use of DDRT devices, once the fuse
                               is blown to one, iMC will not issue any DDRT
                               commands or responding DDRT bus request.
                               Internally, any request targeting DDRT port is
                               treated as programming error. System may hang.
                            */
    UINT32 disable_cr_pmem : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               App Direct support with 3D-Xpoint memory is
                               disabled.
                            */
    UINT32 disable_cr_pmem_wt : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Read caching for App Direct mode support with
                               3D-Xpoint memory is disabled.
                            */
    UINT32 disable_mc_mirror_mode : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* To Disable Mirror Mode */
    UINT32 capid1_18 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Soft SKU Enable: When enabled (value == 1), MSRs
                               0xD0, 0xD1 are available to BIOS to perform
                               softsku fuse overrides
                            */
    UINT32 capid1_19 : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               3DS (3D stacked die) disable control. When set,
                               the C0, C1 and C2 signals will forced to zero.
                            */
    UINT32 capid1_20 : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               2 DPC disable control. When set, CS signals for
                               DIMM slot 1 (i.e. slot 0 is not disabled) are
                               disabled.
                            */
    UINT32 capid1_21 : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               16Gb or higher disable control. When set, the
                               address decode to the corresponding 16Gb or
                               higher mapping is disabled.
                            */
    UINT32 capid1_22 : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1_[25:22] indicate physical address size
                               supported in the core</p> <p>&#39;0001, 32
                               bits;</p> <p>&#39;0011, 36 bits;</p>
                               <p>&#39;0000, 39 bits;</p> <p>&#39;0101, 40
                               bits;</p> <p>&#39;0110, 42 bits;</p>
                               <p>&#39;0111, 44 bits;</p> <p>&#39;1000, 46
                               bits;</p> <p>&#39;1010, 50 bits;</p>
                               <p>&#39;1011, 52 bits;</p>
                            */
    UINT32 mem_pa_size : 3;

                            /* Bits[25:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>CAPID1_[25:22] indicate physical address size
                               supported in the core</p> <p>&#39;0001, 32
                               bits;</p> <p>&#39;0011, 36 bits;</p>
                               <p>&#39;0000, 39 bits;</p> <p>&#39;0101, 40
                               bits;</p> <p>&#39;0110, 42 bits;</p>
                               <p>&#39;0111, 44 bits;</p> <p>&#39;1000, 46
                               bits;</p> <p>&#39;1010, 50 bits;</p>
                               <p>&#39;1011, 52 bits;</p>
                            */
    UINT32 dmfc : 5;

                            /* Bits[30:26], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID1_30:26 imply the maximal supported
                               frequency of Memory DDR. The field is encoded
                               value of the maximum ratio.
                            */
    UINT32 capid1_31 : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} CAPID1_PCU_FUN3_STRUCT;

/* CAPID2_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff308c)                                                      */
/*      SPRB0 (0x21ff308c)                                                      */
/*      SPRHBM (0x21ff308c)                                                     */
/*      SPRC0 (0x21ff308c)                                                      */
/*      SPRMCC (0x21ff308c)                                                     */
/*      SPRUCC (0x21ff308c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID2_PCU_FUN3_REG 0x0432008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ras_levels : 5;

                            /* Bits[4:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID2_[4:0] indicate RAS levels: 5'b00000,
                               HEDT; 5'b00001, Std; 5'b00010, HPC; 5'b00011,
                               Adv; 5'b00101, 1SWS; 5'b00111, FPGA; 5'b01000,
                               Cirrus 5'b01001, Core;
                            */
    UINT32 capid2_5 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_6 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_7 : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_8 : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_9 : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_10 : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_11 : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_13 : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_14 : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_15 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_16 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_17 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_18 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_19 : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_20 : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_21 : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_22 : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid2_23 : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 0 will be disabled. */
    UINT32 capid2_24 : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 1 will be disabled. */
    UINT32 capid2_25 : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 2 will be disabled. */
    UINT32 capid2_26 : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 3 will be disabled. */
    UINT32 capid2_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 4 will be disabled. */
    UINT32 capid2_28 : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* When set Intel UPI link 5 will be disabled. */
    UINT32 capid2_29 : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* When set, UPI speed 12.8GT/s is disabled. */
    UINT32 capid2_30 : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* When set, UPI speed 14.4GT/s is disabled. */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CAPID2_PCU_FUN3_STRUCT;

/* CAPID3_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff3090)                                                      */
/*      SPRB0 (0x21ff3090)                                                      */
/*      SPRHBM (0x21ff3090)                                                     */
/*      SPRC0 (0x21ff3090)                                                      */
/*      SPRMCC (0x21ff3090)                                                     */
/*      SPRUCC (0x21ff3090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID3_PCU_FUN3_REG 0x04320090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capid3_0 : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_1 : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_2 : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_3 : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_4 : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_5 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_6 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID3_7 downto 6 indicates the Die Count in the
                               SoC.
                            */
    UINT32 capid3_7 : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID3_7 downto 6 indicates the Die Count in the
                               SoC.
                            */
    UINT32 capid3_8 : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 capid3_9 : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 disable_ecc : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* ECC disabled. When set, ECC is disabled. */
    UINT32 disable_dir : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* DIR disabled. When set, directory is disabled. */
    UINT32 capid3_12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 disable_rdimm : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disabled by disabling the RDIMM control word
                               access. In addition, the upper 5 bits of the 13b
                               T_STAB register will be treated as zeros, i.e.
                               the T_STAB can only have max of 255 DCLK delay
                               after clock-stopped power down mode which is in
                               sufficient for normal RDIMM clock stablization;
                               hence, users will not be able to support self-
                               refresh with clock off mode (S3, pkg C6) if the
                               RDIMM disable fuse is blown to one.Fuse download
                               may change the default value after reset de-
                               assertion.
                            */
    UINT32 disable_udimm : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               UDIMM disabled. When set, UDIMM support is
                               disabled.
                            */
    UINT32 capid3_15 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid3_16 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 disable_sparing : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               Sparing disabled. When set, rank sparing is
                               disabled.
                            */
    UINT32 disable_patrol_scrub : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Patrol scrub disabled. When set, rank patrol
                               scrub is disabled.
                            */
    UINT32 capid3_19 : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 disable_extended_addr_dimm : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* RSVD */
    UINT32 disable_asyncsr_adr : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               RAID-On-LOAD disabled. When set, memory ignores
                               ADR event.
                            */
    UINT32 disable_smbus_wrt : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /*
                               SMBUS write capability disabled. When set, SMBus
                               write is disabled.
                            */
    UINT32 capid3_23 : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /* When set, DDR Memory instance 0 is enabled. */
    UINT32 capid3_24 : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /* When set, DDR Memory instance 1 is enabled. */
    UINT32 capid3_25 : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /* When set, DDR Memory instance 2 is enabled. */
    UINT32 capid3_26 : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* When set, DDR Memory instance 3 is enabled. */
    UINT32 capid3_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* When set, HBM Memory instance 0 is enabled. */
    UINT32 capid3_28 : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* When set, HBM Memory instance 1 is enabled. */
    UINT32 capid3_29 : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* When set, HBM Memory instance 2 is enabled. */
    UINT32 capid3_30 : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* When set, HBM Memory instance 3 is enabled. */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CAPID3_PCU_FUN3_STRUCT;

/* CAPID4_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff3094)                                                      */
/*      SPRB0 (0x21ff3094)                                                      */
/*      SPRHBM (0x21ff3094)                                                     */
/*      SPRC0 (0x21ff3094)                                                      */
/*      SPRMCC (0x21ff3094)                                                     */
/*      SPRUCC (0x21ff3094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID4_PCU_FUN3_REG 0x04320094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 error_spoofing_dis : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Error spoofing disabled */
    UINT32 pfat_disable : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* PFAT disabled */
    UINT32 fit_boot_dis : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* FIT (Firmware Interface Table) boot disabled */
    UINT32 vmcs_shadowing_dis : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* VMCS shadowing disabled */
    UINT32 capid4_4 : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 capid4_5 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 physical_chop : 2;

                            /* Bits[7:6], Access Type=RO/V, default=0x00000000*/

                            /* 11:XCC; 10:HCC; 01:MCC; 00:LCC; */
    UINT32 capid4_8 : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 ocvolt_ovrd_dis : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* Voltage override overclocking disabled */
    UINT32 llc_qos_mon_en : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* Cache Monitoring Technology (CMT) is enabled */
    UINT32 llc_qos_en : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* Cache Allocation Technology (CAT) is enabled */
    UINT32 rtm_dis : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Hardware Lock Elison + (HLE+) disabled */
    UINT32 hle_dis : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* Hardware Lock Elison (HLE) disabled */
    UINT32 fma_dis : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               FMA (Floatingpoint Multiple Add) instructions
                               disabled
                            */
    UINT32 capid4_15 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* When set, FXR0 is enabled. */
    UINT32 capid4_16 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* When set, FXR1 is enabled. */
    UINT32 capid4_17 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* When set, TIP is enabled. */
    UINT32 capid4_18 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* When set, HQM is enabled. */
    UINT32 enhanced_mca_dis : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Enhanced MCA disabled */
    UINT32 smm_code_chk_dis : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Code access check disabled */
    UINT32 smm_cpu_svrstr_dis : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               SMM CPU save/restore disabled (part of enhanced
                               SMM)
                            */
    UINT32 targeted_smi_dis : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /* Targeted SMI disabled (part of enhanced SMM) */
    UINT32 sapm_dll_disable : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, System Agent Power Management (SAPM)
                               Dynamic Load Line (DLL) is disabled.
                            */
    UINT32 ufs_disable : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /* If set, Uncore Frequency Scaling is disabled */
    UINT32 pcps_disable : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /* Per-core P-state disabled */
    UINT32 eet_enable : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* Energy efficient turbo enabled */
    UINT32 capid4_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 oc_enabled : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* Overclocking enabled */
    UINT32 capid4_29 : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 dram_rapl_disable : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* DRAM RAPL disabled */
    UINT32 dram_power_meter_disable : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* DRAM Power Meter disabled */

  } Bits;
  UINT32 Data;

} CAPID4_PCU_FUN3_STRUCT;

/* CAPID5_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff3098)                                                      */
/*      SPRB0 (0x21ff3098)                                                      */
/*      SPRHBM (0x21ff3098)                                                     */
/*      SPRC0 (0x21ff3098)                                                      */
/*      SPRMCC (0x21ff3098)                                                     */
/*      SPRUCC (0x21ff3098)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID5_PCU_FUN3_REG 0x04320098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 second_imc : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000001*/

                            /* Indicates when second iMC is enabled */
    UINT32 iio_llcconfig_en : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* IIO to allocate in LLC enabled. */
    UINT32 timed_mwait_dis : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* TIMED_MWAIT_DIS */
    UINT32 cap_mirror_ddr4_en : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, DDR4 MC channel mirroring mode
                               capability is available. 0: Channel mirroring
                               mode capability is unavailable; 1: Channel
                               mirroring mode capability is available. If
                               channel mirroring capability is unavailable and
                               mirroring is configured then system results will
                               be unpredictable.
                            */
    UINT32 cap_mirror_ddrt_en : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, SXP MC channel mirroring mode
                               capability is available. 0: Channel mirroring
                               mode capability is unavailable; 1: Channel
                               mirroring mode capability is available. If
                               channel mirroring capability is unavailable and
                               mirroring is configured then system results will
                               be unpredictable. Fuse_TRUE.
                            */
    UINT32 cap_block_region_en : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, Block Region capability is available.
                               If Block Region capability is unavailable and
                               block region is configured then system results
                               will be unpredictable. Fuse_TRUE.
                            */
    UINT32 cap_pmem_en : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, Persistent Memory capability is
                               available. 0: Capability is unavailable; 1:
                               Capability is available. If capability is
                               unavailable and any feature related to the
                               capability is enabled then system results will
                               be unpredictable. Fuse_TRUE.
                            */
    UINT32 cap_nm_caching_en : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, two level memory caching (Near Memory
                               caching for Far Memory) capability is available.
                               0: Capability is unavailable; 1: Capability is
                               available. If capability is unavailable and any
                               feature related to the capability is enabled
                               then system results will be unpredictable.
                               Fuse_TRUE.
                            */
    UINT32 cap_nm_caching_pm_en : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* Near memory caching for persistent memory */
    UINT32 capid5_9 : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>When CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0010 indicate a Cicada 4S;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0001 indicate a Server D;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0100 indicate a DLC;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b1100 indicate an AP;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b0000 indicate a SNR-BTS;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1000 indicate a SNR-NSX;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1001 indicate a SNR-NIC;</p>
                            */
    UINT32 capid5_10 : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>When CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0010 indicate a Cicada 4S;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0001 indicate a Server D;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0100 indicate a DLC;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b1100 indicate an AP;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b0000 indicate a SNR-BTS;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1000 indicate a SNR-NSX;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1001 indicate a SNR-NIC;</p>
                            */
    UINT32 capid5_11 : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>When CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0010 indicate a Cicada 4S;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0001 indicate a Server D;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0100 indicate a DLC;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b1100 indicate an AP;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b0000 indicate a SNR-BTS;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1000 indicate a SNR-NSX;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1001 indicate a SNR-NIC;</p>
                            */
    UINT32 capid5_12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>When CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0010 indicate a Cicada 4S;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0001 indicate a Server D;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b0100 indicate a DLC;</p> <p>When
                               CAPID0_5:3 == 3&#39;b111, CAPID5_12:9 ==
                               4&#39;b1100 indicate an AP;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b0000 indicate a SNR-BTS;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1000 indicate a SNR-NSX;</p> <p>When
                               CAPID0_5:3 == 3&#39;b110, CAPID5_12:9 ==
                               4&#39;b1001 indicate a SNR-NIC;</p>
                            */
    UINT32 capid5_13 : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI0 is enabled. */
    UINT32 capid5_14 : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI1 is enabled. */
    UINT32 capid5_15 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI2 is enabled. */
    UINT32 capid5_16 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI3 is enabled. */
    UINT32 capid5_17 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI4 is enabled. */
    UINT32 capid5_18 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI5 is enabled. */
    UINT32 capid5_19 : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI6 is enabled. */
    UINT32 capid5_20 : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IDI7 is enabled. */
    UINT32 capid5_21 : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID5_[24:21] indicate allowed DDR MC channel
                               count.
                            */
    UINT32 capid5_22 : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID5_[24:21] indicate allowed DDR MC channel
                               count.
                            */
    UINT32 capid5_23 : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID5_[24:21] indicate allowed DDR MC channel
                               count.
                            */
    UINT32 capid5_24 : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               CAPID5_[24:21] indicate allowed DDR MC channel
                               count.
                            */
    UINT32 hwp_en : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /* HWPM Base feature */
    UINT32 acc_enable : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* Autonomous C-State Control */
    UINT32 capid5_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* When set, an external SKU.&nbsp; */
    UINT32 capid5_28 : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */
    UINT32 addr_based_mem_mirror : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* Address based memory mirroring enabled */
    UINT32 hitme_enable : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* Directory Cache enabled. */
    UINT32 capid5_31 : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* Reserved for future use. */

  } Bits;
  UINT32 Data;

} CAPID5_PCU_FUN3_STRUCT;

/* CAPID6_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff309c)                                                      */
/*      SPRB0 (0x21ff309c)                                                      */
/*      SPRHBM (0x21ff309c)                                                     */
/*      SPRC0 (0x21ff309c)                                                      */
/*      SPRMCC (0x21ff309c)                                                     */
/*      SPRUCC (0x21ff309c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID6_PCU_FUN3_REG 0x0432009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llc_slice_en_low : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Enabled Cbo slices low (Cbo with enabled LLC
                               slice).
                            */

  } Bits;
  UINT32 Data;

} CAPID6_PCU_FUN3_STRUCT;

/* CAPID7_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff30a0)                                                      */
/*      SPRB0 (0x21ff30a0)                                                      */
/*      SPRHBM (0x21ff30a0)                                                     */
/*      SPRC0 (0x21ff30a0)                                                      */
/*      SPRMCC (0x21ff30a0)                                                     */
/*      SPRUCC (0x21ff30a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID7_PCU_FUN3_REG 0x043200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llc_slice_en_high : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Enabled Cbo slices high (Cbo with enabled LLC
                               slice).
                            */

  } Bits;
  UINT32 Data;

} CAPID7_PCU_FUN3_STRUCT;

/* CAPID8_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff30ac)                                                      */
/*      SPRB0 (0x21ff30ac)                                                      */
/*      SPRHBM (0x21ff30ac)                                                     */
/*      SPRC0 (0x21ff30ac)                                                      */
/*      SPRMCC (0x21ff30ac)                                                     */
/*      SPRUCC (0x21ff30ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID8_PCU_FUN3_REG 0x043200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llc_ia_core_en_low : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* LLC_IA_CORE_EN_LOW */

  } Bits;
  UINT32 Data;

} CAPID8_PCU_FUN3_STRUCT;

/* CAPID9_PCU_FUN3_REG supported on:                                            */
/*      SPRA0 (0x21ff30b0)                                                      */
/*      SPRB0 (0x21ff30b0)                                                      */
/*      SPRHBM (0x21ff30b0)                                                     */
/*      SPRC0 (0x21ff30b0)                                                      */
/*      SPRMCC (0x21ff30b0)                                                     */
/*      SPRUCC (0x21ff30b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID9_PCU_FUN3_REG 0x043200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llc_ia_core_en_high : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* LLC_IA_CORE_EN_HIGH */

  } Bits;
  UINT32 Data;

} CAPID9_PCU_FUN3_STRUCT;

/* CAPID10_PCU_FUN3_REG supported on:                                           */
/*      SPRA0 (0x21ff30bc)                                                      */
/*      SPRB0 (0x21ff30bc)                                                      */
/*      SPRHBM (0x21ff30bc)                                                     */
/*      SPRC0 (0x21ff30bc)                                                      */
/*      SPRMCC (0x21ff30bc)                                                     */
/*      SPRUCC (0x21ff30bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* This register is a Capability Register used to expose feature support to BIOS for SKU differentiation.
*/


#define CAPID10_PCU_FUN3_REG 0x043200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capid10_0 : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 0 is enabled. */
    UINT32 capid10_1 : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 1 is enabled. */
    UINT32 capid10_2 : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 2 is enabled. */
    UINT32 capid10_3 : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* <p>When set, MS2IOSF in stack 3 is enabled.</p> */
    UINT32 capid10_4 : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 4 is enabled */
    UINT32 capid10_5 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 5 is enabled */
    UINT32 capid10_6 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 6 is enabled. */
    UINT32 capid10_7 : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* When set, M2IOSF in stack 7 is enabled. */
    UINT32 capid10_8 : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 8 is enabled. */
    UINT32 capid10_9 : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 9 is enabled. */
    UINT32 capid10_10 : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 10 is enabled. */
    UINT32 capid10_11 : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* When set, MS2IOSF in stack 11 is enabled. */
    UINT32 capid10_12 : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_13 : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_14 : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_15 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_16 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_17 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_18 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_19 : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_20 : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_21 : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_22 : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_23 : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_24 : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_25 : 1;

                            /* Bits[25:25], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_26 : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_27 : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_28 : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_29 : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_30 : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */
    UINT32 capid10_31 : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* <p>Reserved for future use.</p> */

  } Bits;
  UINT32 Data;

} CAPID10_PCU_FUN3_STRUCT;

/* CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG supported on:                              */
/*      SPRA0 (0x21ff30c0)                                                      */
/*      SPRB0 (0x21ff30c0)                                                      */
/*      SPRHBM (0x21ff30c0)                                                     */
/*      SPRC0 (0x21ff30c0)                                                      */
/*      SPRMCC (0x21ff30c0)                                                     */
/*      SPRUCC (0x21ff30c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Level 1 configurable TDP settings.
*/


#define CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG 0x043200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg_tdp : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*
                               Power for this TDP level. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT] Similar to
                               PACKAGE_POWER_SKU[PKG_TDP]
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tdp_ratio : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* TDP ratio for config tdp level 1. */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_LEVEL1_N0_PCU_FUN3_STRUCT;

/* CONFIG_TDP_LEVEL1_N1_PCU_FUN3_REG supported on:                              */
/*      SPRA0 (0x21ff30c4)                                                      */
/*      SPRB0 (0x21ff30c4)                                                      */
/*      SPRHBM (0x21ff30c4)                                                     */
/*      SPRC0 (0x21ff30c4)                                                      */
/*      SPRMCC (0x21ff30c4)                                                     */
/*      SPRUCC (0x21ff30c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Level 1 configurable TDP settings.
*/


#define CONFIG_TDP_LEVEL1_N1_PCU_FUN3_REG 0x043200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg_max_pwr : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*
                               Max pkg power setting allowed for this config
                               TDP level1. Higher values will be clamped down
                               to this value. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT]. Similar to
                               PACKAGE_POWER_SKU[PKG_MAX_PWR].
                            */
    UINT32 pkg_min_pwr : 16;

                            /* Bits[30:15], Access Type=RO, default=0x00000000*/

                            /*
                               Min pkg power setting allowed for this config
                               TDP level. Lower values will be clamped up to
                               this value. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT]. Similar to
                               PACKAGE_POWER_SKU[PKG_MIN_PWR].
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_LEVEL1_N1_PCU_FUN3_STRUCT;

/* CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG supported on:                              */
/*      SPRA0 (0x21ff30c8)                                                      */
/*      SPRB0 (0x21ff30c8)                                                      */
/*      SPRHBM (0x21ff30c8)                                                     */
/*      SPRC0 (0x21ff30c8)                                                      */
/*      SPRMCC (0x21ff30c8)                                                     */
/*      SPRUCC (0x21ff30c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Level 2 configurable TDP settings. On sku that does not support config tdp, these to registers will report out 0.
     
*/


#define CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG 0x043200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg_tdp : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*
                               Power for this TDP level. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT] Similar to
                               PACKAGE_POWER_SKU[PKG_TDP].
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tdp_ratio : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* TDP ratio for level 2. */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_LEVEL2_N0_PCU_FUN3_STRUCT;

/* CONFIG_TDP_LEVEL2_N1_PCU_FUN3_REG supported on:                              */
/*      SPRA0 (0x21ff30cc)                                                      */
/*      SPRB0 (0x21ff30cc)                                                      */
/*      SPRHBM (0x21ff30cc)                                                     */
/*      SPRC0 (0x21ff30cc)                                                      */
/*      SPRMCC (0x21ff30cc)                                                     */
/*      SPRUCC (0x21ff30cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Level 2 configurable TDP settings. On sku that does not support config tdp, these to registers will report out 0.
     
*/


#define CONFIG_TDP_LEVEL2_N1_PCU_FUN3_REG 0x043200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg_max_pwr : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*
                               Max pkg power setting allowed for config TDP
                               level 2. Higher values will be clamped down to
                               this value. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT]. Similar to
                               PACKAGE_POWER_SKU[PKG_MAX_PWR].
                            */
    UINT32 pkg_min_pwr : 16;

                            /* Bits[30:15], Access Type=RO, default=0x00000000*/

                            /*
                               Min pkg power setting allowed for this config
                               TDP level 2. Lower values will be clamped up to
                               this value. Units defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT]. Similar to
                               PACKAGE_POWER_SKU[PKG_MIN_PWR].
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_LEVEL2_N1_PCU_FUN3_STRUCT;

/* CONFIG_TDP_CONTROL_PCU_FUN3_REG supported on:                                */
/*      SPRA0 (0x21ff30d8)                                                      */
/*      SPRB0 (0x21ff30d8)                                                      */
/*      SPRHBM (0x21ff30d8)                                                     */
/*      SPRC0 (0x21ff30d8)                                                      */
/*      SPRMCC (0x21ff30d8)                                                     */
/*      SPRUCC (0x21ff30d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Rd/Wr register to allow platform SW to select TDP point and set lock via all three interfaces (MSR, MMIO and PECI/PCS)
*/


#define CONFIG_TDP_CONTROL_PCU_FUN3_REG 0x043200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tdp_level : 3;

                            /* Bits[2:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Config TDP level selected 0 = nominal TDP level
                               (default) 1 = Level from CONFIG_TDP_LEVEL_1 2 =
                               Level from CONFIG_TDP_LEVEL_2 3 = Level from
                               Config_tdp_ext_L1 4 = Level from
                               Config_tdp_ext_L2 5 = reserved 6 = reserved 7 =
                               reserved
                            */
    UINT32 rsvd : 28;

                            /* Bits[30:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 config_tdp_lock : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Config TDP level select lock 0 - unlocked. 1 -
                               locked till next reset.
                            */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_CONTROL_PCU_FUN3_STRUCT;

/* CONFIG_TDP_NOMINAL_PCU_FUN3_REG supported on:                                */
/*      SPRA0 (0x21ff30dc)                                                      */
/*      SPRB0 (0x21ff30dc)                                                      */
/*      SPRHBM (0x21ff30dc)                                                     */
/*      SPRC0 (0x21ff30dc)                                                      */
/*      SPRMCC (0x21ff30dc)                                                     */
/*      SPRUCC (0x21ff30dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Nominal TDP configuration
*/


#define CONFIG_TDP_NOMINAL_PCU_FUN3_REG 0x043200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tdp_ratio : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Pcode set based on SKU and factors in
                               SSKU/softbin and flex impact.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CONFIG_TDP_NOMINAL_PCU_FUN3_STRUCT;

/* FUSED_CORES_LOW_N0_PCU_FUN3_REG supported on:                                */
/*      SPRA0 (0x21ff30e0)                                                      */
/*      SPRB0 (0x21ff30e0)                                                      */
/*      SPRHBM (0x21ff30e0)                                                     */
/*      SPRC0 (0x21ff30e0)                                                      */
/*      SPRMCC (0x21ff30e0)                                                     */
/*      SPRUCC (0x21ff30e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Vector of SKU enabled IA cores in the package.
*/


#define FUSED_CORES_LOW_N0_PCU_FUN3_REG 0x043200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fused_core_mask_0 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Vector of SKU enabled IA cores in the package. */

  } Bits;
  UINT32 Data;

} FUSED_CORES_LOW_N0_PCU_FUN3_STRUCT;

/* FUSED_CORES_LOW_N1_PCU_FUN3_REG supported on:                                */
/*      SPRA0 (0x21ff30e4)                                                      */
/*      SPRB0 (0x21ff30e4)                                                      */
/*      SPRHBM (0x21ff30e4)                                                     */
/*      SPRC0 (0x21ff30e4)                                                      */
/*      SPRMCC (0x21ff30e4)                                                     */
/*      SPRUCC (0x21ff30e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Vector of SKU enabled IA cores in the package.
*/


#define FUSED_CORES_LOW_N1_PCU_FUN3_REG 0x043200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fused_core_mask_1 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Vector of SKU enabled IA cores in the package. */

  } Bits;
  UINT32 Data;

} FUSED_CORES_LOW_N1_PCU_FUN3_STRUCT;

/* FUSED_CORES_HIGH_N0_PCU_FUN3_REG supported on:                               */
/*      SPRA0 (0x21ff30e8)                                                      */
/*      SPRB0 (0x21ff30e8)                                                      */
/*      SPRHBM (0x21ff30e8)                                                     */
/*      SPRC0 (0x21ff30e8)                                                      */
/*      SPRMCC (0x21ff30e8)                                                     */
/*      SPRUCC (0x21ff30e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* register to reserve next location for each per-core mask/status registers in order to support 128 cores in future
*/


#define FUSED_CORES_HIGH_N0_PCU_FUN3_REG 0x043200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved 64 bit space. Flops need not be
                               implemented.
                            */

  } Bits;
  UINT32 Data;

} FUSED_CORES_HIGH_N0_PCU_FUN3_STRUCT;

/* FUSED_CORES_HIGH_N1_PCU_FUN3_REG supported on:                               */
/*      SPRA0 (0x21ff30ec)                                                      */
/*      SPRB0 (0x21ff30ec)                                                      */
/*      SPRHBM (0x21ff30ec)                                                     */
/*      SPRC0 (0x21ff30ec)                                                      */
/*      SPRMCC (0x21ff30ec)                                                     */
/*      SPRUCC (0x21ff30ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* register to reserve next location for each per-core mask/status registers in order to support 128 cores in future
*/


#define FUSED_CORES_HIGH_N1_PCU_FUN3_REG 0x043200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved 64 bit space. Flops need not be
                               implemented.
                            */

  } Bits;
  UINT32 Data;

} FUSED_CORES_HIGH_N1_PCU_FUN3_STRUCT;

/* FLEX_RATIO_N0_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x21ff30f0)                                                      */
/*      SPRB0 (0x21ff30f0)                                                      */
/*      SPRHBM (0x21ff30f0)                                                     */
/*      SPRC0 (0x21ff30f0)                                                      */
/*      SPRMCC (0x21ff30f0)                                                     */
/*      SPRUCC (0x21ff30f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* The Flexible Boot register is written by BIOS in order to modify the maximum non-turbo ratio on the next reset. The DP/MP systems use this FLEX_RATIO in this register to configure the maximum common boot ratio for all physical processors in the system. The value in the FLEX_RATIO take effect on the next reset based on the value of FLEX_EN.
*/


#define FLEX_RATIO_N0_PCU_FUN3_REG 0x043200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oc_extra_voltage : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Extra voltage to be used for Over Clocking. The
                               voltage is defined in units of 1/256 Volts.
                            */
    UINT32 flex_ratio : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Desired Flex ratio. When enabled, the value in
                               this field will set the maximum non-turbo ratio
                               available. If this value is greater than the
                               maximum ratio set by hardware, this field is
                               ignored.
                            */
    UINT32 enable : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Flex Ratio Enabled. When set to 1, indicates
                               that the value programmed in FLEX_RATIO field
                               will be used to override the maximum non-turbo
                               ratio on next reboot. When this bit is cleared,
                               all write to FLEX_RATIO field will be ignored.
                               However there will be no GP-fault# generated.
                            */
    UINT32 oc_bins : 3;

                            /* Bits[19:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field indicates the maximum number of bins
                               by which the part can be overclocked
                            */
    UINT32 oc_lock : 1;

                            /* Bits[20:20], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               Once set to 1, the overclocking bitfields become
                               locked from further writes are ignored. A reset
                               is required to clear the lock. Note: System BIOS
                               must set this bit to lock down overclocking
                               limits.
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FLEX_RATIO_N0_PCU_FUN3_STRUCT;

/* FLEX_RATIO_N1_PCU_FUN3_REG supported on:                                     */
/*      SPRA0 (0x21ff30f4)                                                      */
/*      SPRB0 (0x21ff30f4)                                                      */
/*      SPRHBM (0x21ff30f4)                                                     */
/*      SPRC0 (0x21ff30f4)                                                      */
/*      SPRMCC (0x21ff30f4)                                                     */
/*      SPRUCC (0x21ff30f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_3*/
/* Struct generated from SPRA0 BDF: 31_30_3                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* The Flexible Boot register is written by BIOS in order to modify the maximum non-turbo ratio on the next reset. The DP/MP systems use this FLEX_RATIO in this register to configure the maximum common boot ratio for all physical processors in the system. The value in the FLEX_RATIO take effect on the next reset based on the value of FLEX_EN.
*/


#define FLEX_RATIO_N1_PCU_FUN3_REG 0x043200F4

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

} FLEX_RATIO_N1_PCU_FUN3_STRUCT;
#endif /* _PCU_FUN3_h */
