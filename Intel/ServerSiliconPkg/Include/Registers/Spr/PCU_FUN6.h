
/** @file
  PCU_FUN6.h

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


#ifndef _PCU_FUN6_h
#define _PCU_FUN6_h
#include <Base.h>

/* VID_1_30_6_PCU_FUN6_REG supported on:                                        */
/*      SPRA0 (0x11ff6000)                                                      */
/*      SPRB0 (0x11ff6000)                                                      */
/*      SPRHBM (0x11ff6000)                                                     */
/*      SPRC0 (0x11ff6000)                                                      */
/*      SPRMCC (0x11ff6000)                                                     */
/*      SPRUCC (0x11ff6000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Vendor ID Register
*/


#define VID_1_30_6_PCU_FUN6_REG 0x04610000

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

} VID_1_30_6_PCU_FUN6_STRUCT;

/* DID_1_30_6_PCU_FUN6_REG supported on:                                        */
/*      SPRA0 (0x11ff6002)                                                      */
/*      SPRB0 (0x11ff6002)                                                      */
/*      SPRHBM (0x11ff6002)                                                     */
/*      SPRC0 (0x11ff6002)                                                      */
/*      SPRMCC (0x11ff6002)                                                     */
/*      SPRUCC (0x11ff6002)                                                     */
/* Register default value on SPRA0: 0x0000325E                                  */
/* Register default value on SPRB0: 0x0000325E                                  */
/* Register default value on SPRHBM: 0x0000325E                                 */
/* Register default value on SPRC0: 0x0000325E                                  */
/* Register default value on SPRMCC: 0x0000325E                                 */
/* Register default value on SPRUCC: 0x0000325E                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Device Identification Number
*/


#define DID_1_30_6_PCU_FUN6_REG 0x04610002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000325E*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_30_6_PCU_FUN6_STRUCT;

/* PCICMD_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x11ff6004)                                                      */
/*      SPRB0 (0x11ff6004)                                                      */
/*      SPRHBM (0x11ff6004)                                                     */
/*      SPRC0 (0x11ff6004)                                                      */
/*      SPRMCC (0x11ff6004)                                                     */
/*      SPRUCC (0x11ff6004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Command Register
*/


#define PCICMD_1_30_6_PCU_FUN6_REG 0x04610004

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

} PCICMD_1_30_6_PCU_FUN6_STRUCT;

/* PCISTS_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x11ff6006)                                                      */
/*      SPRB0 (0x11ff6006)                                                      */
/*      SPRHBM (0x11ff6006)                                                     */
/*      SPRC0 (0x11ff6006)                                                      */
/*      SPRMCC (0x11ff6006)                                                     */
/*      SPRUCC (0x11ff6006)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Status Register
*/


#define PCISTS_1_30_6_PCU_FUN6_REG 0x04610006

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

} PCISTS_1_30_6_PCU_FUN6_STRUCT;

/* RID_CCR_1_30_6_PCU_FUN6_REG supported on:                                    */
/*      SPRA0 (0x21ff6008)                                                      */
/*      SPRB0 (0x21ff6008)                                                      */
/*      SPRHBM (0x21ff6008)                                                     */
/*      SPRC0 (0x21ff6008)                                                      */
/*      SPRMCC (0x21ff6008)                                                     */
/*      SPRUCC (0x21ff6008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_30_6_PCU_FUN6_REG 0x04620008

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

} RID_CCR_1_30_6_PCU_FUN6_STRUCT;

/* CLSR_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x1ff600c)                                                       */
/*      SPRB0 (0x1ff600c)                                                       */
/*      SPRHBM (0x1ff600c)                                                      */
/*      SPRC0 (0x1ff600c)                                                       */
/*      SPRMCC (0x1ff600c)                                                      */
/*      SPRUCC (0x1ff600c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Cache Line Size Register
*/


#define CLSR_1_30_6_PCU_FUN6_REG 0x0460000C

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

} CLSR_1_30_6_PCU_FUN6_STRUCT;

/* PLAT_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x1ff600d)                                                       */
/*      SPRB0 (0x1ff600d)                                                       */
/*      SPRHBM (0x1ff600d)                                                      */
/*      SPRC0 (0x1ff600d)                                                       */
/*      SPRMCC (0x1ff600d)                                                      */
/*      SPRUCC (0x1ff600d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Latency Timer
*/


#define PLAT_1_30_6_PCU_FUN6_REG 0x0460000D

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

} PLAT_1_30_6_PCU_FUN6_STRUCT;

/* HDR_1_30_6_PCU_FUN6_REG supported on:                                        */
/*      SPRA0 (0x1ff600e)                                                       */
/*      SPRB0 (0x1ff600e)                                                       */
/*      SPRHBM (0x1ff600e)                                                      */
/*      SPRC0 (0x1ff600e)                                                       */
/*      SPRMCC (0x1ff600e)                                                      */
/*      SPRUCC (0x1ff600e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Header Type
*/


#define HDR_1_30_6_PCU_FUN6_REG 0x0460000E

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

} HDR_1_30_6_PCU_FUN6_STRUCT;

/* BIST_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x1ff600f)                                                       */
/*      SPRB0 (0x1ff600f)                                                       */
/*      SPRHBM (0x1ff600f)                                                      */
/*      SPRC0 (0x1ff600f)                                                       */
/*      SPRMCC (0x1ff600f)                                                      */
/*      SPRUCC (0x1ff600f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI BIST Register
*/


#define BIST_1_30_6_PCU_FUN6_REG 0x0460000F

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

} BIST_1_30_6_PCU_FUN6_STRUCT;

/* SVID_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x11ff602c)                                                      */
/*      SPRB0 (0x11ff602c)                                                      */
/*      SPRHBM (0x11ff602c)                                                     */
/*      SPRC0 (0x11ff602c)                                                      */
/*      SPRMCC (0x11ff602c)                                                     */
/*      SPRUCC (0x11ff602c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem Vendor ID Register
*/


#define SVID_1_30_6_PCU_FUN6_REG 0x0461002C

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

} SVID_1_30_6_PCU_FUN6_STRUCT;

/* SDID_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x11ff602e)                                                      */
/*      SPRB0 (0x11ff602e)                                                      */
/*      SPRHBM (0x11ff602e)                                                     */
/*      SPRC0 (0x11ff602e)                                                      */
/*      SPRMCC (0x11ff602e)                                                     */
/*      SPRUCC (0x11ff602e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* Subsystem ID Register
*/


#define SDID_1_30_6_PCU_FUN6_REG 0x0461002E

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

} SDID_1_30_6_PCU_FUN6_STRUCT;

/* CAPPTR_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x1ff6034)                                                       */
/*      SPRB0 (0x1ff6034)                                                       */
/*      SPRHBM (0x1ff6034)                                                      */
/*      SPRC0 (0x1ff6034)                                                       */
/*      SPRMCC (0x1ff6034)                                                      */
/*      SPRUCC (0x1ff6034)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_30_6_PCU_FUN6_REG 0x04600034

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

} CAPPTR_1_30_6_PCU_FUN6_STRUCT;

/* INTL_1_30_6_PCU_FUN6_REG supported on:                                       */
/*      SPRA0 (0x1ff603c)                                                       */
/*      SPRB0 (0x1ff603c)                                                       */
/*      SPRHBM (0x1ff603c)                                                      */
/*      SPRC0 (0x1ff603c)                                                       */
/*      SPRMCC (0x1ff603c)                                                      */
/*      SPRUCC (0x1ff603c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Line Register
*/


#define INTL_1_30_6_PCU_FUN6_REG 0x0460003C

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

} INTL_1_30_6_PCU_FUN6_STRUCT;

/* INTPIN_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x1ff603d)                                                       */
/*      SPRB0 (0x1ff603d)                                                       */
/*      SPRHBM (0x1ff603d)                                                      */
/*      SPRC0 (0x1ff603d)                                                       */
/*      SPRMCC (0x1ff603d)                                                      */
/*      SPRUCC (0x1ff603d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_30_6_PCU_FUN6_REG 0x0460003D

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

} INTPIN_1_30_6_PCU_FUN6_STRUCT;

/* MINGNT_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x1ff603e)                                                       */
/*      SPRB0 (0x1ff603e)                                                       */
/*      SPRHBM (0x1ff603e)                                                      */
/*      SPRC0 (0x1ff603e)                                                       */
/*      SPRMCC (0x1ff603e)                                                      */
/*      SPRUCC (0x1ff603e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Min Grant Register
*/


#define MINGNT_1_30_6_PCU_FUN6_REG 0x0460003E

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

} MINGNT_1_30_6_PCU_FUN6_STRUCT;

/* MAXLAT_1_30_6_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x1ff603f)                                                       */
/*      SPRB0 (0x1ff603f)                                                       */
/*      SPRHBM (0x1ff603f)                                                      */
/*      SPRC0 (0x1ff603f)                                                       */
/*      SPRMCC (0x1ff603f)                                                      */
/*      SPRUCC (0x1ff603f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* PCI Max Latency Register
*/


#define MAXLAT_1_30_6_PCU_FUN6_REG 0x0460003F

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

} MAXLAT_1_30_6_PCU_FUN6_STRUCT;

/* RESOLVED_CORES_N0_PCU_FUN6_REG supported on:                                 */
/*      SPRA0 (0x21ff6080)                                                      */
/*      SPRB0 (0x21ff6080)                                                      */
/*      SPRHBM (0x21ff6080)                                                     */
/*      SPRC0 (0x21ff6080)                                                      */
/*      SPRMCC (0x21ff6080)                                                     */
/*      SPRUCC (0x21ff6080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* The resolved IA core mask contains the functional (enabled in SKU) and non-defeatured IA cores.
*/


#define RESOLVED_CORES_N0_PCU_FUN6_REG 0x04620080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core_mask_0 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The resolved IA core mask contains the
                               functional (enabled in SKU) and non-defeatured
                               IA cores. The mask is indexed by logical ID. It
                               is normally contiguous, unless BIOS defeature is
                               activated on a particular core. BSP and APIC IDs
                               will be set by the processor based on this
                               value. This field is determined by FW based on
                               CSR_DESIRED_CORES[CORE_OFF_MASK]. This field
                               contains lower 32 bits for core mask.
                            */

  } Bits;
  UINT32 Data;

} RESOLVED_CORES_N0_PCU_FUN6_STRUCT;

/* RESOLVED_CORES_N1_PCU_FUN6_REG supported on:                                 */
/*      SPRA0 (0x21ff6084)                                                      */
/*      SPRB0 (0x21ff6084)                                                      */
/*      SPRHBM (0x21ff6084)                                                     */
/*      SPRC0 (0x21ff6084)                                                      */
/*      SPRMCC (0x21ff6084)                                                     */
/*      SPRUCC (0x21ff6084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* The resolved IA core mask contains the functional (enabled in SKU) and non-defeatured IA cores.
*/


#define RESOLVED_CORES_N1_PCU_FUN6_REG 0x04620084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core_mask_1 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The resolved IA core mask contains the
                               functional (enabled in SKU) and non-defeatured
                               IA cores. The mask is indexed by logical ID. It
                               is normally contiguous, unless BIOS defeature is
                               activated on a particular core. BSP and APIC IDs
                               will be set by the processor based on this
                               value. This field is determined by FW based on
                               CSR_DESIRED_CORES[CORE_OFF_MASK]. This field
                               contains upper bits for core mask.
                            */

  } Bits;
  UINT32 Data;

} RESOLVED_CORES_N1_PCU_FUN6_STRUCT;

/* RESOLVED_CORES_HIGH_N0_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff6088)                                                      */
/*      SPRB0 (0x21ff6088)                                                      */
/*      SPRHBM (0x21ff6088)                                                     */
/*      SPRC0 (0x21ff6088)                                                      */
/*      SPRMCC (0x21ff6088)                                                     */
/*      SPRUCC (0x21ff6088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* register to reserve next location for each per-core mask/status registers in order to support 128 cores in future
*/


#define RESOLVED_CORES_HIGH_N0_PCU_FUN6_REG 0x04620088

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

} RESOLVED_CORES_HIGH_N0_PCU_FUN6_STRUCT;

/* RESOLVED_CORES_HIGH_N1_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff608c)                                                      */
/*      SPRB0 (0x21ff608c)                                                      */
/*      SPRHBM (0x21ff608c)                                                     */
/*      SPRC0 (0x21ff608c)                                                      */
/*      SPRMCC (0x21ff608c)                                                     */
/*      SPRUCC (0x21ff608c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* register to reserve next location for each per-core mask/status registers in order to support 128 cores in future
*/


#define RESOLVED_CORES_HIGH_N1_PCU_FUN6_REG 0x0462008C

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

} RESOLVED_CORES_HIGH_N1_PCU_FUN6_STRUCT;

/* CORE_FIVR_ERR_LOG_CFG_0_PCU_FUN6_REG supported on:                           */
/*      SPRA0 (0x21ff6090)                                                      */
/*      SPRB0 (0x21ff6090)                                                      */
/*      SPRHBM (0x21ff6090)                                                     */
/*      SPRC0 (0x21ff6090)                                                      */
/*      SPRMCC (0x21ff6090)                                                     */
/*      SPRUCC (0x21ff6090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Reports Core FIVR Faults - Bits correspond to cores which have a core IVR fault. Cores with a core IVR fault will not come out of reset. This field has the same field mapping as the CSR RESOLVED_CORES and results are reported in CSR RESOLVED_CORES. 
*/


#define CORE_FIVR_ERR_LOG_CFG_0_PCU_FUN6_REG 0x04620090

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

} CORE_FIVR_ERR_LOG_CFG_0_PCU_FUN6_STRUCT;

/* CORE_FIVR_ERR_LOG_CFG_1_PCU_FUN6_REG supported on:                           */
/*      SPRA0 (0x21ff6094)                                                      */
/*      SPRB0 (0x21ff6094)                                                      */
/*      SPRHBM (0x21ff6094)                                                     */
/*      SPRC0 (0x21ff6094)                                                      */
/*      SPRMCC (0x21ff6094)                                                     */
/*      SPRUCC (0x21ff6094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Reports Core FIVR Faults - Bits correspond to cores which have a core IVR fault. Cores with a core IVR fault will not come out of reset. This field has the same field mapping as the CSR RESOLVED_CORES and results are reported in CSR RESOLVED_CORES. 
*/


#define CORE_FIVR_ERR_LOG_CFG_1_PCU_FUN6_REG 0x04620094

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

} CORE_FIVR_ERR_LOG_CFG_1_PCU_FUN6_STRUCT;

/* CORE_FIVR_ERR_LOG_RSVD_CFG_0_PCU_FUN6_REG supported on:                      */
/*      SPRA0 (0x21ff6098)                                                      */
/*      SPRB0 (0x21ff6098)                                                      */
/*      SPRHBM (0x21ff6098)                                                     */
/*      SPRC0 (0x21ff6098)                                                      */
/*      SPRMCC (0x21ff6098)                                                     */
/*      SPRUCC (0x21ff6098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Reserved for Future. (for  65-128 cores). When core count increases, Change the index count in CORE_FIVR_ERR_LOG_CFG as required and decrease the count in CORE_FIVR_ERR_LOG_RSVD_CFG.
Reports Core FIVR Faults - Bits correspond to cores which have a core IVR fault. Cores with a core IVR fault will not come out of reset. This field has the same field mapping as the CSR RESOLVED_CORES and results are reported in CSR RESOLVED_CORES. 
*/


#define CORE_FIVR_ERR_LOG_RSVD_CFG_0_PCU_FUN6_REG 0x04620098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved 32 bit space. Flops need not be
                               implemented.
                            */

  } Bits;
  UINT32 Data;

} CORE_FIVR_ERR_LOG_RSVD_CFG_0_PCU_FUN6_STRUCT;

/* CORE_FIVR_ERR_LOG_RSVD_CFG_1_PCU_FUN6_REG supported on:                      */
/*      SPRA0 (0x21ff609c)                                                      */
/*      SPRB0 (0x21ff609c)                                                      */
/*      SPRHBM (0x21ff609c)                                                     */
/*      SPRC0 (0x21ff609c)                                                      */
/*      SPRMCC (0x21ff609c)                                                     */
/*      SPRUCC (0x21ff609c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6 */
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcfsms/ptpcfsms/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* Reserved for Future. (for  65-128 cores). When core count increases, Change the index count in CORE_FIVR_ERR_LOG_CFG as required and decrease the count in CORE_FIVR_ERR_LOG_RSVD_CFG.
Reports Core FIVR Faults - Bits correspond to cores which have a core IVR fault. Cores with a core IVR fault will not come out of reset. This field has the same field mapping as the CSR RESOLVED_CORES and results are reported in CSR RESOLVED_CORES. 
*/


#define CORE_FIVR_ERR_LOG_RSVD_CFG_1_PCU_FUN6_REG 0x0462009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved 32 bit space. Flops need not be
                               implemented.
                            */

  } Bits;
  UINT32 Data;

} CORE_FIVR_ERR_LOG_RSVD_CFG_1_PCU_FUN6_STRUCT;

/* PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff60a8)                                                      */
/*      SPRB0 (0x21ff60a8)                                                      */
/*      SPRHBM (0x21ff60a8)                                                     */
/*      SPRC0 (0x21ff60a8)                                                      */
/*      SPRMCC (0x21ff60a8)                                                     */
/*      SPRUCC (0x21ff60a8)                                                     */
/* Register default value on SPRA0: 0x00060000                                  */
/* Register default value on SPRB0: 0x00060000                                  */
/* Register default value on SPRHBM: 0x00060000                                 */
/* Register default value on SPRC0: 0x00060000                                  */
/* Register default value on SPRMCC: 0x00060000                                 */
/* Register default value on SPRUCC: 0x00060000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register allows platform  power consumption of the processor to be specified values.
*/


#define PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_REG 0x046200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppl1 : 17;

                            /* Bits[16:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field indicates the Platform power
                               limitation #1. Define the value of the platform
                               power limits in Watts Format is 14.3 and
                               resolution is 0.125W.
                            */
    UINT32 ppl1_en : 1;

                            /* Bits[17:17], Access Type=RW/L, default=0x00000001*/

                            /* Platform Power Limit 1 is enabled/disabled */
    UINT32 plt_clmp_lim_1 : 1;

                            /* Bits[18:18], Access Type=RW/L, default=0x00000001*/

                            /*
                               Platform Clamping limitation #1- when set will
                               allow CPU frequency going below P1 to meet power
                               limits. This bit cannot override the value of
                               the clamping fuse if set to 1.
                            */
    UINT32 ptw1 : 7;

                            /* Bits[25:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               Defines the value of the RAPL time window
                               according to the time_unit field of
                               "POWER_SKU_UNIT" MSR [TIME_UNIT]. TW1 =
                               TIME_UNIT (1+(x/4))2^y where x is the value
                               defined by upper 2 bits of the TW1 field y is
                               the value defined by lower 5 bits of the TW1
                               field
                            */
    UINT32 rsvd0 : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_STRUCT;

/* PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff60ac)                                                      */
/*      SPRB0 (0x21ff60ac)                                                      */
/*      SPRHBM (0x21ff60ac)                                                     */
/*      SPRC0 (0x21ff60ac)                                                      */
/*      SPRMCC (0x21ff60ac)                                                     */
/*      SPRUCC (0x21ff60ac)                                                     */
/* Register default value on SPRA0: 0x00060000                                  */
/* Register default value on SPRB0: 0x00060000                                  */
/* Register default value on SPRHBM: 0x00060000                                 */
/* Register default value on SPRC0: 0x00060000                                  */
/* Register default value on SPRMCC: 0x00060000                                 */
/* Register default value on SPRUCC: 0x00060000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register allows platform  power consumption of the processor to be specified values.
*/


#define PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG 0x046200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppl2 : 17;

                            /* Bits[16:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field indicates the Platform power
                               limitation #2. Define the value of the platform
                               power limits in Watts Format is 14.3 and
                               resolution is 0.125W.
                            */
    UINT32 ppl2_en : 1;

                            /* Bits[17:17], Access Type=RW/L, default=0x00000001*/

                            /* Platform Power Limit 2 is enabled/disabled */
    UINT32 plt_clmp_lim_2 : 1;

                            /* Bits[18:18], Access Type=RW/L, default=0x00000001*/

                            /*
                               Platform Clamping limitation #2 , when set will
                               allow CPU frequency going below P1 to meet power
                               limits. This bit cannot override the value of
                               the clamping fuse if set to 1.
                            */
    UINT32 ptw2 : 7;

                            /* Bits[25:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               Defines the value of the RAPL time window
                               according to the time_unit field of
                               "POWER_SKU_UNIT" MSR [TIME_UNIT]. TW2 =
                               TIME_UNIT (1+(x/4))2^y where x is the value
                               defined by upper 2 bits of the TW2 field y is
                               the value defined by lower 5 bits of the TW2
                               field TW2 =< TW1
                            */
    UINT32 rsvd1 : 5;

                            /* Bits[30:26], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 plt_pwr_lim_lock : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only. This bit
                               will typically set by BIOS during boot time or
                               resume from Sx.
                            */

  } Bits;
  UINT32 Data;

} PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_STRUCT;

/* PLATFORM_POWER_INFO_N0_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff60b0)                                                      */
/*      SPRB0 (0x21ff60b0)                                                      */
/*      SPRHBM (0x21ff60b0)                                                     */
/*      SPRC0 (0x21ff60b0)                                                      */
/*      SPRMCC (0x21ff60b0)                                                     */
/*      SPRUCC (0x21ff60b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register allows platform  power consumption of the processor to be specified values.
*/


#define PLATFORM_POWER_INFO_N0_PCU_FUN6_REG 0x046200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_ppl1_value : 17;

                            /* Bits[16:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This is the Maximum of the power limitation
                               range for PPL1 on the Power supply unit of the
                               platform. The unit of are 17 bits each in Watts
                               in a 14.3 format with resolution of 0.215W
                            */
    UINT32 min_ppl1_value : 15;

                            /* Bits[31:17], Access Type=RW/L, default=0x00000000*/

                            /*
                               This is the minimum of the power limitation
                               range for PPL1 on the Power supply unit of the
                               platform. The unit of are 17 bits each in Watts
                               in a 14.3 format with resolution of 0.215W
                            */

  } Bits;
  UINT32 Data;

} PLATFORM_POWER_INFO_N0_PCU_FUN6_STRUCT;

/* PLATFORM_POWER_INFO_N1_PCU_FUN6_REG supported on:                            */
/*      SPRA0 (0x21ff60b4)                                                      */
/*      SPRB0 (0x21ff60b4)                                                      */
/*      SPRHBM (0x21ff60b4)                                                     */
/*      SPRC0 (0x21ff60b4)                                                      */
/*      SPRMCC (0x21ff60b4)                                                     */
/*      SPRUCC (0x21ff60b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRB0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRHBM Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRC0 Security PolicyGroup: PUNIT_OS_W                                       */
/* SPRMCC Security PolicyGroup: PUNIT_OS_W                                      */
/* SPRUCC Security PolicyGroup: PUNIT_OS_W                                      */
/* This register allows platform  power consumption of the processor to be specified values.
*/


#define PLATFORM_POWER_INFO_N1_PCU_FUN6_REG 0x046200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_ppl2_value : 17;

                            /* Bits[16:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               TThis is the Maximum of the power limitation
                               range for PPL2 on the Power supply unit of the
                               platform. The unit of are 17 bits each in Watts
                               in a 14.3 format with resolution of 0.215W
                            */
    UINT32 max_time_window : 7;

                            /* Bits[23:17], Access Type=RW/L, default=0x00000000*/

                            /*
                               This is the time window limitation for both PTW1
                               and PTW2. The unit of measurement is defined in
                               "POWER_SKU_UNIT" MSR [TIME_UNIT].
                            */
    UINT32 reserved : 7;

                            /* Bits[30:24], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 plt_pwr_info_lock : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only. This bit
                               will typically set by BIOS during boot time or
                               resume from Sx.
                            */

  } Bits;
  UINT32 Data;

} PLATFORM_POWER_INFO_N1_PCU_FUN6_STRUCT;

/* TSYNC_DELAY_MIN_PCU_FUN6_REG supported on:                                   */
/*      SPRA0 (0x21ff60c4)                                                      */
/*      SPRB0 (0x21ff60c4)                                                      */
/*      SPRHBM (0x21ff60c4)                                                     */
/*      SPRC0 (0x21ff60c4)                                                      */
/*      SPRMCC (0x21ff60c4)                                                     */
/*      SPRUCC (0x21ff60c4)                                                     */
/* Register default value on SPRA0: 0x00000014                                  */
/* Register default value on SPRB0: 0x00000014                                  */
/* Register default value on SPRHBM: 0x00000014                                 */
/* Register default value on SPRC0: 0x00000014                                  */
/* Register default value on SPRMCC: 0x00000014                                 */
/* Register default value on SPRUCC: 0x00000014                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Time synchronization delay minimum. It is used when MaxAllowedDelay = 0x0000
*/


#define TSYNC_DELAY_MIN_PCU_FUN6_REG 0x046200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 delay_min : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000014*/

                            /*
                               Value added to ART for use in Type B time
                               synchroization
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Rsvd */

  } Bits;
  UINT32 Data;

} TSYNC_DELAY_MIN_PCU_FUN6_STRUCT;

/* TSYNC_DELAY_MED_PCU_FUN6_REG supported on:                                   */
/*      SPRA0 (0x21ff60c8)                                                      */
/*      SPRB0 (0x21ff60c8)                                                      */
/*      SPRHBM (0x21ff60c8)                                                     */
/*      SPRC0 (0x21ff60c8)                                                      */
/*      SPRMCC (0x21ff60c8)                                                     */
/*      SPRUCC (0x21ff60c8)                                                     */
/* Register default value on SPRA0: 0x00000028                                  */
/* Register default value on SPRB0: 0x00000028                                  */
/* Register default value on SPRHBM: 0x00000028                                 */
/* Register default value on SPRC0: 0x00000028                                  */
/* Register default value on SPRMCC: 0x00000028                                 */
/* Register default value on SPRUCC: 0x00000028                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Time synchronization delay medium. It is used when MaxAllowedDelay = 0x0001
*/


#define TSYNC_DELAY_MED_PCU_FUN6_REG 0x046200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 delay_med : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000028*/

                            /*
                               Value added to ART for use in Type B time
                               synchroization
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Rsvd */

  } Bits;
  UINT32 Data;

} TSYNC_DELAY_MED_PCU_FUN6_STRUCT;

/* TSYNC_DELAY_MAX_PCU_FUN6_REG supported on:                                   */
/*      SPRA0 (0x21ff60cc)                                                      */
/*      SPRB0 (0x21ff60cc)                                                      */
/*      SPRHBM (0x21ff60cc)                                                     */
/*      SPRC0 (0x21ff60cc)                                                      */
/*      SPRMCC (0x21ff60cc)                                                     */
/*      SPRUCC (0x21ff60cc)                                                     */
/* Register default value on SPRA0: 0x00000050                                  */
/* Register default value on SPRB0: 0x00000050                                  */
/* Register default value on SPRHBM: 0x00000050                                 */
/* Register default value on SPRC0: 0x00000050                                  */
/* Register default value on SPRMCC: 0x00000050                                 */
/* Register default value on SPRUCC: 0x00000050                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Time synchronization delay maximum. It is used when MaxAllowedDelay = 0x0010
*/


#define TSYNC_DELAY_MAX_PCU_FUN6_REG 0x046200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 delay_max : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000050*/

                            /*
                               Value added to ART for use in Type B time
                               synchroization
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Rsvd */

  } Bits;
  UINT32 Data;

} TSYNC_DELAY_MAX_PCU_FUN6_STRUCT;

/* TSYNC_ERR_INFO_PCU_FUN6_REG supported on:                                    */
/*      SPRA0 (0x21ff60e4)                                                      */
/*      SPRB0 (0x21ff60e4)                                                      */
/*      SPRHBM (0x21ff60e4)                                                     */
/*      SPRC0 (0x21ff60e4)                                                      */
/*      SPRMCC (0x21ff60e4)                                                     */
/*      SPRUCC (0x21ff60e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRB0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRHBM Security PolicyGroup: P_U_CODE_W                                      */
/* SPRC0 Security PolicyGroup: P_U_CODE_W                                       */
/* SPRMCC Security PolicyGroup: P_U_CODE_W                                      */
/* SPRUCC Security PolicyGroup: P_U_CODE_W                                      */
/* It constains debug information regarding an erroneous time synchronization request
*/


#define TSYNC_ERR_INFO_PCU_FUN6_REG 0x046200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flag_q_full : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               When a request arrived, the time synchronization
                               queue was already full
                            */
    UINT32 flag_msg_dis : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               When a request arrived, time synchroniation
                               messages were disabled
                            */
    UINT32 flag_unsup_ops : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* iA requst's operandes are not supported */
    UINT32 flag_max_retry : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               A Type B reqeust was retired TSYNC_MAX_TRY times
                               without success
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Rsvd0 */
    UINT32 req_sid : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               IOSB sideband source ID of erroneous reqeust.
                               This field is only set by HW if all FLAG_*
                               fields are clear.
                            */
    UINT32 req_typed : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               iAgentType of erroneous reqeust is Type D. This
                               field is only set by HW if all FLAG_* fields are
                               clear.
                            */
    UINT32 req_typeb_max : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               SyncType of erroneous reqeust is Type B. The
                               MaxAllowedDelay is maxium. This field is only
                               set by HW if all FLAG_* fields are clear.
                            */
    UINT32 req_typeb_med : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               SyncType of erroneous request is Type B. The
                               MaxAllowedDelay is medium. This field is only
                               set by HW if all FLAG_* fields are clear
                            */
    UINT32 req_typeb_min : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               SyncType of erroneous request is Type B. The
                               MaxAllowedDelay is low. This field is only set
                               by HW if all FLAG_* fields are clear
                            */
    UINT32 req_art_base : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               The TimeBaseCode of erroneous request is ART
                               Base. This field is only set by HW if all FLAG_*
                               fields are clear
                            */
    UINT32 rsvd1 : 3;

                            /* Bits[23:21], Access Type=RO, default=0x00000000*/

                            /* Rsvd1 */
    UINT32 req_sid_hier : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               HierarchicalIOSB sideband source ID of erroneous
                               request. This field is only set by HW if all
                               FLAG_* fields are clear.
                            */

  } Bits;
  UINT32 Data;

} TSYNC_ERR_INFO_PCU_FUN6_STRUCT;

/* TSYNC_MAX_TRY_PCU_FUN6_REG supported on:                                     */
/*      SPRA0 (0x21ff60e8)                                                      */
/*      SPRB0 (0x21ff60e8)                                                      */
/*      SPRHBM (0x21ff60e8)                                                     */
/*      SPRC0 (0x21ff60e8)                                                      */
/*      SPRMCC (0x21ff60e8)                                                     */
/*      SPRUCC (0x21ff60e8)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRB0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRHBM Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRC0 Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRMCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* SPRUCC Register File:    sprsp_top/punit_top/ptpcioregs/ptpcioregs/CFG_PCU_FUNC_6*/
/* Struct generated from SPRA0 BDF: 31_30_6                                     */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Maxium number of attempts for a Type B request
*/


#define TSYNC_MAX_TRY_PCU_FUN6_REG 0x046200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_try : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000004*/

                            /*
                               Maxium number of retry attempts for a Type B
                               request before failing
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Rsvd */

  } Bits;
  UINT32 Data;

} TSYNC_MAX_TRY_PCU_FUN6_STRUCT;
#endif /* _PCU_FUN6_h */
