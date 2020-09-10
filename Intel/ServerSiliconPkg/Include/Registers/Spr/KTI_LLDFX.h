
/** @file
  KTI_LLDFX.h

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
/* DFX                                                                          */
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
/* DFX                                                                          */
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
/* DFX                                                                          */
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
/* DFX                                                                          */
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
/* DFX                                                                          */
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
/* DFX                                                                          */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _KTI_LLDFX_h
#define _KTI_LLDFX_h
#include <Base.h>

/* VID_KTI_LLDFX_REG supported on:                                              */
/*      SPRA0 (0x11e08000)                                                      */
/*      SPRB0 (0x11e08000)                                                      */
/*      SPRHBM (0x11e08000)                                                     */
/*      SPRC0 (0x11e08000)                                                      */
/*      SPRMCC (0x11e08000)                                                     */
/*      SPRUCC (0x11e08000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_KTI_LLDFX_REG 0x09310000

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

} VID_KTI_LLDFX_STRUCT;

/* DID_KTI_LLDFX_REG supported on:                                              */
/*      SPRA0 (0x11e08002)                                                      */
/*      SPRB0 (0x11e08002)                                                      */
/*      SPRHBM (0x11e08002)                                                     */
/*      SPRC0 (0x11e08002)                                                      */
/*      SPRMCC (0x11e08002)                                                     */
/*      SPRUCC (0x11e08002)                                                     */
/* Register default value on SPRA0: 0x00003240                                  */
/* Register default value on SPRB0: 0x00003240                                  */
/* Register default value on SPRHBM: 0x00003240                                 */
/* Register default value on SPRC0: 0x00003240                                  */
/* Register default value on SPRMCC: 0x00003240                                 */
/* Register default value on SPRUCC: 0x00003240                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_KTI_LLDFX_REG 0x09310002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003240*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_KTI_LLDFX_STRUCT;

/* PCICMD_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x11e08004)                                                      */
/*      SPRB0 (0x11e08004)                                                      */
/*      SPRHBM (0x11e08004)                                                     */
/*      SPRC0 (0x11e08004)                                                      */
/*      SPRMCC (0x11e08004)                                                     */
/*      SPRUCC (0x11e08004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_KTI_LLDFX_REG 0x09310004

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

} PCICMD_KTI_LLDFX_STRUCT;

/* PCISTS_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x11e08006)                                                      */
/*      SPRB0 (0x11e08006)                                                      */
/*      SPRHBM (0x11e08006)                                                     */
/*      SPRC0 (0x11e08006)                                                      */
/*      SPRMCC (0x11e08006)                                                     */
/*      SPRUCC (0x11e08006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_KTI_LLDFX_REG 0x09310006

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

} PCISTS_KTI_LLDFX_STRUCT;

/* RID_CCR_KTI_LLDFX_REG supported on:                                          */
/*      SPRA0 (0x21e08008)                                                      */
/*      SPRB0 (0x21e08008)                                                      */
/*      SPRHBM (0x21e08008)                                                     */
/*      SPRC0 (0x21e08008)                                                      */
/*      SPRMCC (0x21e08008)                                                     */
/*      SPRUCC (0x21e08008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_KTI_LLDFX_REG 0x09320008

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

} RID_CCR_KTI_LLDFX_STRUCT;

/* CLSR_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x1e0800c)                                                       */
/*      SPRB0 (0x1e0800c)                                                       */
/*      SPRHBM (0x1e0800c)                                                      */
/*      SPRC0 (0x1e0800c)                                                       */
/*      SPRMCC (0x1e0800c)                                                      */
/*      SPRUCC (0x1e0800c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_KTI_LLDFX_REG 0x0930000C

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

} CLSR_KTI_LLDFX_STRUCT;

/* PLAT_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x1e0800d)                                                       */
/*      SPRB0 (0x1e0800d)                                                       */
/*      SPRHBM (0x1e0800d)                                                      */
/*      SPRC0 (0x1e0800d)                                                       */
/*      SPRMCC (0x1e0800d)                                                      */
/*      SPRUCC (0x1e0800d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_KTI_LLDFX_REG 0x0930000D

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

} PLAT_KTI_LLDFX_STRUCT;

/* HDR_KTI_LLDFX_REG supported on:                                              */
/*      SPRA0 (0x1e0800e)                                                       */
/*      SPRB0 (0x1e0800e)                                                       */
/*      SPRHBM (0x1e0800e)                                                      */
/*      SPRC0 (0x1e0800e)                                                       */
/*      SPRMCC (0x1e0800e)                                                      */
/*      SPRUCC (0x1e0800e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_KTI_LLDFX_REG 0x0930000E

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

} HDR_KTI_LLDFX_STRUCT;

/* BIST_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x1e0800f)                                                       */
/*      SPRB0 (0x1e0800f)                                                       */
/*      SPRHBM (0x1e0800f)                                                      */
/*      SPRC0 (0x1e0800f)                                                       */
/*      SPRMCC (0x1e0800f)                                                      */
/*      SPRUCC (0x1e0800f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_KTI_LLDFX_REG 0x0930000F

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

} BIST_KTI_LLDFX_STRUCT;

/* SVID_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x11e0802c)                                                      */
/*      SPRB0 (0x11e0802c)                                                      */
/*      SPRHBM (0x11e0802c)                                                     */
/*      SPRC0 (0x11e0802c)                                                      */
/*      SPRMCC (0x11e0802c)                                                     */
/*      SPRUCC (0x11e0802c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_KTI_LLDFX_REG 0x0931002C

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

} SVID_KTI_LLDFX_STRUCT;

/* SDID_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x11e0802e)                                                      */
/*      SPRB0 (0x11e0802e)                                                      */
/*      SPRHBM (0x11e0802e)                                                     */
/*      SPRC0 (0x11e0802e)                                                      */
/*      SPRMCC (0x11e0802e)                                                     */
/*      SPRUCC (0x11e0802e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_KTI_LLDFX_REG 0x0931002E

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

} SDID_KTI_LLDFX_STRUCT;

/* CAPPTR_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x1e08034)                                                       */
/*      SPRB0 (0x1e08034)                                                       */
/*      SPRHBM (0x1e08034)                                                      */
/*      SPRC0 (0x1e08034)                                                       */
/*      SPRMCC (0x1e08034)                                                      */
/*      SPRUCC (0x1e08034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_KTI_LLDFX_REG 0x09300034

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

} CAPPTR_KTI_LLDFX_STRUCT;

/* INTL_KTI_LLDFX_REG supported on:                                             */
/*      SPRA0 (0x1e0803c)                                                       */
/*      SPRB0 (0x1e0803c)                                                       */
/*      SPRHBM (0x1e0803c)                                                      */
/*      SPRC0 (0x1e0803c)                                                       */
/*      SPRMCC (0x1e0803c)                                                      */
/*      SPRUCC (0x1e0803c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_KTI_LLDFX_REG 0x0930003C

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

} INTL_KTI_LLDFX_STRUCT;

/* INTPIN_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x1e0803d)                                                       */
/*      SPRB0 (0x1e0803d)                                                       */
/*      SPRHBM (0x1e0803d)                                                      */
/*      SPRC0 (0x1e0803d)                                                       */
/*      SPRMCC (0x1e0803d)                                                      */
/*      SPRUCC (0x1e0803d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_KTI_LLDFX_REG 0x0930003D

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

} INTPIN_KTI_LLDFX_STRUCT;

/* MINGNT_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x1e0803e)                                                       */
/*      SPRB0 (0x1e0803e)                                                       */
/*      SPRHBM (0x1e0803e)                                                      */
/*      SPRC0 (0x1e0803e)                                                       */
/*      SPRMCC (0x1e0803e)                                                      */
/*      SPRUCC (0x1e0803e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_KTI_LLDFX_REG 0x0930003E

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

} MINGNT_KTI_LLDFX_STRUCT;

/* MAXLAT_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x1e0803f)                                                       */
/*      SPRB0 (0x1e0803f)                                                       */
/*      SPRHBM (0x1e0803f)                                                      */
/*      SPRC0 (0x1e0803f)                                                       */
/*      SPRMCC (0x1e0803f)                                                      */
/*      SPRUCC (0x1e0803f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_KTI_LLDFX_REG 0x0930003F

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

} MAXLAT_KTI_LLDFX_STRUCT;

/* PXPCAP_KTI_LLDFX_REG supported on:                                           */
/*      SPRA0 (0x21e08040)                                                      */
/*      SPRB0 (0x21e08040)                                                      */
/*      SPRHBM (0x21e08040)                                                     */
/*      SPRC0 (0x21e08040)                                                      */
/*      SPRMCC (0x21e08040)                                                     */
/*      SPRUCC (0x21e08040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_KTI_LLDFX_REG 0x09320040

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

} PXPCAP_KTI_LLDFX_STRUCT;

/* PCIE_RSVD0_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08044)                                                      */
/*      SPRB0 (0x21e08044)                                                      */
/*      SPRHBM (0x21e08044)                                                     */
/*      SPRC0 (0x21e08044)                                                      */
/*      SPRMCC (0x21e08044)                                                     */
/*      SPRUCC (0x21e08044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_KTI_LLDFX_REG 0x09320044

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

} PCIE_RSVD0_KTI_LLDFX_STRUCT;

/* PCIE_RSVD1_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08048)                                                      */
/*      SPRB0 (0x21e08048)                                                      */
/*      SPRHBM (0x21e08048)                                                     */
/*      SPRC0 (0x21e08048)                                                      */
/*      SPRMCC (0x21e08048)                                                     */
/*      SPRUCC (0x21e08048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_KTI_LLDFX_REG 0x09320048

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

} PCIE_RSVD1_KTI_LLDFX_STRUCT;

/* PCIE_RSVD2_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e0804c)                                                      */
/*      SPRB0 (0x21e0804c)                                                      */
/*      SPRHBM (0x21e0804c)                                                     */
/*      SPRC0 (0x21e0804c)                                                      */
/*      SPRMCC (0x21e0804c)                                                     */
/*      SPRUCC (0x21e0804c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_KTI_LLDFX_REG 0x0932004C

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

} PCIE_RSVD2_KTI_LLDFX_STRUCT;

/* PCIE_RSVD3_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08050)                                                      */
/*      SPRB0 (0x21e08050)                                                      */
/*      SPRHBM (0x21e08050)                                                     */
/*      SPRC0 (0x21e08050)                                                      */
/*      SPRMCC (0x21e08050)                                                     */
/*      SPRUCC (0x21e08050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_KTI_LLDFX_REG 0x09320050

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

} PCIE_RSVD3_KTI_LLDFX_STRUCT;

/* PCIE_RSVD4_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08054)                                                      */
/*      SPRB0 (0x21e08054)                                                      */
/*      SPRHBM (0x21e08054)                                                     */
/*      SPRC0 (0x21e08054)                                                      */
/*      SPRMCC (0x21e08054)                                                     */
/*      SPRUCC (0x21e08054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_KTI_LLDFX_REG 0x09320054

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

} PCIE_RSVD4_KTI_LLDFX_STRUCT;

/* PCIE_RSVD5_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08058)                                                      */
/*      SPRB0 (0x21e08058)                                                      */
/*      SPRHBM (0x21e08058)                                                     */
/*      SPRC0 (0x21e08058)                                                      */
/*      SPRMCC (0x21e08058)                                                     */
/*      SPRUCC (0x21e08058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_KTI_LLDFX_REG 0x09320058

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

} PCIE_RSVD5_KTI_LLDFX_STRUCT;

/* PCIE_RSVD6_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e0805c)                                                      */
/*      SPRB0 (0x21e0805c)                                                      */
/*      SPRHBM (0x21e0805c)                                                     */
/*      SPRC0 (0x21e0805c)                                                      */
/*      SPRMCC (0x21e0805c)                                                     */
/*      SPRUCC (0x21e0805c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_KTI_LLDFX_REG 0x0932005C

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

} PCIE_RSVD6_KTI_LLDFX_STRUCT;

/* PCIE_RSVD7_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08060)                                                      */
/*      SPRB0 (0x21e08060)                                                      */
/*      SPRHBM (0x21e08060)                                                     */
/*      SPRC0 (0x21e08060)                                                      */
/*      SPRMCC (0x21e08060)                                                     */
/*      SPRUCC (0x21e08060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_KTI_LLDFX_REG 0x09320060

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

} PCIE_RSVD7_KTI_LLDFX_STRUCT;

/* PCIE_RSVD8_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08064)                                                      */
/*      SPRB0 (0x21e08064)                                                      */
/*      SPRHBM (0x21e08064)                                                     */
/*      SPRC0 (0x21e08064)                                                      */
/*      SPRMCC (0x21e08064)                                                     */
/*      SPRUCC (0x21e08064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD8_KTI_LLDFX_REG 0x09320064

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

} PCIE_RSVD8_KTI_LLDFX_STRUCT;

/* PCIE_RSVD9_KTI_LLDFX_REG supported on:                                       */
/*      SPRA0 (0x21e08068)                                                      */
/*      SPRB0 (0x21e08068)                                                      */
/*      SPRHBM (0x21e08068)                                                     */
/*      SPRC0 (0x21e08068)                                                      */
/*      SPRMCC (0x21e08068)                                                     */
/*      SPRUCC (0x21e08068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD9_KTI_LLDFX_REG 0x09320068

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

} PCIE_RSVD9_KTI_LLDFX_STRUCT;

/* PCIE_RSVD10_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e0806c)                                                      */
/*      SPRB0 (0x21e0806c)                                                      */
/*      SPRHBM (0x21e0806c)                                                     */
/*      SPRC0 (0x21e0806c)                                                      */
/*      SPRMCC (0x21e0806c)                                                     */
/*      SPRUCC (0x21e0806c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD10_KTI_LLDFX_REG 0x0932006C

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

} PCIE_RSVD10_KTI_LLDFX_STRUCT;

/* PCIE_RSVD11_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e08070)                                                      */
/*      SPRB0 (0x21e08070)                                                      */
/*      SPRHBM (0x21e08070)                                                     */
/*      SPRC0 (0x21e08070)                                                      */
/*      SPRMCC (0x21e08070)                                                     */
/*      SPRUCC (0x21e08070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD11_KTI_LLDFX_REG 0x09320070

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

} PCIE_RSVD11_KTI_LLDFX_STRUCT;

/* PCIE_RSVD12_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e08074)                                                      */
/*      SPRB0 (0x21e08074)                                                      */
/*      SPRHBM (0x21e08074)                                                     */
/*      SPRC0 (0x21e08074)                                                      */
/*      SPRMCC (0x21e08074)                                                     */
/*      SPRUCC (0x21e08074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD12_KTI_LLDFX_REG 0x09320074

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

} PCIE_RSVD12_KTI_LLDFX_STRUCT;

/* PCIE_RSVD13_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e08078)                                                      */
/*      SPRB0 (0x21e08078)                                                      */
/*      SPRHBM (0x21e08078)                                                     */
/*      SPRC0 (0x21e08078)                                                      */
/*      SPRMCC (0x21e08078)                                                     */
/*      SPRUCC (0x21e08078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD13_KTI_LLDFX_REG 0x09320078

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

} PCIE_RSVD13_KTI_LLDFX_STRUCT;

/* PCIE_RSVD14_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e0807c)                                                      */
/*      SPRB0 (0x21e0807c)                                                      */
/*      SPRHBM (0x21e0807c)                                                     */
/*      SPRC0 (0x21e0807c)                                                      */
/*      SPRMCC (0x21e0807c)                                                     */
/*      SPRUCC (0x21e0807c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD14_KTI_LLDFX_REG 0x0932007C

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

} PCIE_RSVD14_KTI_LLDFX_STRUCT;

/* KTIMISCSTAT_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e080d4)                                                      */
/*      SPRB0 (0x21e080d4)                                                      */
/*      SPRHBM (0x21e080d4)                                                     */
/*      SPRC0 (0x21e080d4)                                                      */
/*      SPRMCC (0x21e080d4)                                                     */
/*      SPRUCC (0x21e080d4)                                                     */
/* Register default value on SPRA0: 0x00000003                                  */
/* Register default value on SPRB0: 0x00000003                                  */
/* Register default value on SPRHBM: 0x00000003                                 */
/* Register default value on SPRC0: 0x00000003                                  */
/* Register default value on SPRMCC: 0x00000003                                 */
/* Register default value on SPRUCC: 0x00000003                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktimiscstat
*/


#define KTIMISCSTAT_KTI_LLDFX_REG 0x093200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti_rate : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000003*/

                            /*
                               This reflects the supported current Intel UPI
                               rate setting into the PLL. 100 - 9.6 GT/s 101 -
                               10.4GT/s other - Reserved Note: The default
                               value of 3'b011 does not reflect the actual
                               Intel UPI rate. Reads of this register field
                               will always report one of the legal defined
                               values above.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIMISCSTAT_KTI_LLDFX_STRUCT;

/* KTIMISCCLKCTRL_KTI_LLDFX_REG supported on:                                   */
/*      SPRA0 (0x21e080ec)                                                      */
/*      SPRB0 (0x21e080ec)                                                      */
/*      SPRHBM (0x21e080ec)                                                     */
/*      SPRC0 (0x21e080ec)                                                      */
/*      SPRMCC (0x21e080ec)                                                     */
/*      SPRUCC (0x21e080ec)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* Register default value on SPRUCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktimiscclkctrl
*/


#define KTIMISCCLKCTRL_KTI_LLDFX_REG 0x093200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_agent_chain : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Obsolete, SKX doesn't have an agent csr critter
                               chain. Recommend remove this field in future
                               project.
                            */
    UINT32 disable_iov_chain : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Obsolete, HSX doesn't have 8-bit IOV csr critter
                               chain. Recommend remove this field in future
                               project.
                            */
    UINT32 rsvd : 3;

                            /* Bits[4:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti_misc_csr_clkgat_en : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* clock gate enable for UPI misc CSR and CSR chain */
    UINT32 rsvd_6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIMISCCLKCTRL_KTI_LLDFX_STRUCT;

/* PXPENHCAP_KTI_LLDFX_REG supported on:                                        */
/*      SPRA0 (0x21e08100)                                                      */
/*      SPRB0 (0x21e08100)                                                      */
/*      SPRHBM (0x21e08100)                                                     */
/*      SPRC0 (0x21e08100)                                                      */
/*      SPRMCC (0x21e08100)                                                     */
/*      SPRUCC (0x21e08100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_KTI_LLDFX_REG 0x09320100

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

} PXPENHCAP_KTI_LLDFX_STRUCT;

/* KTIMISCPRIVC0_KTI_LLDFX_REG supported on:                                    */
/*      SPRA0 (0x21e081d0)                                                      */
/*      SPRB0 (0x21e081d0)                                                      */
/*      SPRHBM (0x21e081d0)                                                     */
/*      SPRC0 (0x21e081d0)                                                      */
/*      SPRMCC (0x21e081d0)                                                     */
/*      SPRUCC (0x21e081d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktimiscprivc0
*/


#define KTIMISCPRIVC0_KTI_LLDFX_REG 0x093201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msgch_gateclk_dis : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               If set disables clock gating on message channel
                               endpoint and router
                            */
    UINT32 tap_csr_override_en : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               To enable some PLL CSR override function from
                               minit tap.
                            */
    UINT32 override_fuse_pn_term_disable : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Overrider fuse seting and enable Bit13 prgraming
                               to disable PN termination.
                            */
    UINT32 pn_term_disable : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable PN termination feature and go back to
                               JKT. (Need to set bit[12] to override the fuse)
                            */
    UINT32 spare4 : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare5 : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare6 : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare7 : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare8 : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare9 : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare10 : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare11 : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare12 : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare13 : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare14 : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare15 : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare16 : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare17 : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare18 : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare19 : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare20 : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare21 : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare22 : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare23 : 1;

                            /* Bits[23:23], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare24 : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare25 : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare26 : 1;

                            /* Bits[26:26], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare27 : 1;

                            /* Bits[27:27], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare28 : 1;

                            /* Bits[28:28], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare29 : 1;

                            /* Bits[29:29], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare30 : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 force_cfgwrltlck_nonposted : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Force all CfgWrLTLck requests targeting UPI
                               agent to be treated as non-posted. The purpose
                               is to revert to JKT/IVT/HSX QPI behavior (SKX
                               HSD 298055).
                            */

  } Bits;
  UINT32 Data;

} KTIMISCPRIVC0_KTI_LLDFX_STRUCT;

/* DFX_NTL_CTL_KTI_LLDFX_REG supported on:                                      */
/*      SPRA0 (0x21e08260)                                                      */
/*      SPRB0 (0x21e08260)                                                      */
/*      SPRHBM (0x21e08260)                                                     */
/*      SPRC0 (0x21e08260)                                                      */
/*      SPRMCC (0x21e08260)                                                     */
/*      SPRUCC (0x21e08260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* dfx_ntl_ctl
*/


#define DFX_NTL_CTL_KTI_LLDFX_REG 0x09320260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ntl_capt_mode : 2;

                            /* Bits[9:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               THIS REGISTER NOT TO BE USED ANYMORE(from ICX-R
                               timeframe). NTL Logic got moved to UniPhy along
                               with Registers and hence this will not be used.
                            */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ntl_comp_all : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               THIS REGISTER NOT TO BE USED ANYMORE(from ICX-R
                               timeframe). NTL Logic got moved to UniPhy along
                               with Registers and hence this will not be used.
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DFX_NTL_CTL_KTI_LLDFX_STRUCT;

/* KTIMISCMODLCK_KTI_LLDFX_REG supported on:                                    */
/*      SPRA0 (0x21e08300)                                                      */
/*      SPRB0 (0x21e08300)                                                      */
/*      SPRHBM (0x21e08300)                                                     */
/*      SPRC0 (0x21e08300)                                                      */
/*      SPRMCC (0x21e08300)                                                     */
/*      SPRUCC (0x21e08300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktimiscmodlck
*/


#define KTIMISCMODLCK_KTI_LLDFX_REG 0x09320300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iodfxlck_and_glblcsrlcken : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* IO DFX lock and global csr lock enable status */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIMISCMODLCK_KTI_LLDFX_STRUCT;

/* KTIMISCSCRATCHCFG32_KTI_LLDFX_REG supported on:                              */
/*      SPRA0 (0x21e08304)                                                      */
/*      SPRB0 (0x21e08304)                                                      */
/*      SPRHBM (0x21e08304)                                                     */
/*      SPRC0 (0x21e08304)                                                      */
/*      SPRMCC (0x21e08304)                                                     */
/*      SPRUCC (0x21e08304)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktimiscscratchcfg32
*/


#define KTIMISCSCRATCHCFG32_KTI_LLDFX_REG 0x09320304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratch_dn : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Scratch CFG Lower 16-bits (sticky) register
                               field.
                            */
    UINT32 scratch_up : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch CFG Upper 16-bits (non-sticky) register
                               field.
                            */

  } Bits;
  UINT32 Data;

} KTIMISCSCRATCHCFG32_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_CP_LO_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e08308)                                                      */
/*      SPRB0 (0x21e08308)                                                      */
/*      SPRHBM (0x21e08308)                                                     */
/*      SPRC0 (0x21e08308)                                                      */
/*      SPRMCC (0x21e08308)                                                     */
/*      SPRUCC (0x21e08308)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_CP LOWER 32bits
*/


#define UPIMISC_OS_W_CP_LO_KTI_LLDFX_REG 0x09320308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_CP_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_CP_HI_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e0830c)                                                      */
/*      SPRB0 (0x21e0830c)                                                      */
/*      SPRHBM (0x21e0830c)                                                     */
/*      SPRC0 (0x21e0830c)                                                      */
/*      SPRMCC (0x21e0830c)                                                     */
/*      SPRUCC (0x21e0830c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_CP UPPER 32bits
*/


#define UPIMISC_OS_W_CP_HI_KTI_LLDFX_REG 0x0932030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_CP_HI_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_RAC_LO_KTI_LLDFX_REG supported on:                              */
/*      SPRA0 (0x21e08310)                                                      */
/*      SPRB0 (0x21e08310)                                                      */
/*      SPRHBM (0x21e08310)                                                     */
/*      SPRC0 (0x21e08310)                                                      */
/*      SPRMCC (0x21e08310)                                                     */
/*      SPRUCC (0x21e08310)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_RAC LOWER 32bits
*/


#define UPIMISC_OS_W_RAC_LO_KTI_LLDFX_REG 0x09320310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_RAC_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_RAC_HI_KTI_LLDFX_REG supported on:                              */
/*      SPRA0 (0x21e08314)                                                      */
/*      SPRB0 (0x21e08314)                                                      */
/*      SPRHBM (0x21e08314)                                                     */
/*      SPRC0 (0x21e08314)                                                      */
/*      SPRMCC (0x21e08314)                                                     */
/*      SPRUCC (0x21e08314)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_RAC UPPER 32bits
*/


#define UPIMISC_OS_W_RAC_HI_KTI_LLDFX_REG 0x09320314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_RAC_HI_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_WAC_LO_KTI_LLDFX_REG supported on:                              */
/*      SPRA0 (0x21e08318)                                                      */
/*      SPRB0 (0x21e08318)                                                      */
/*      SPRHBM (0x21e08318)                                                     */
/*      SPRC0 (0x21e08318)                                                      */
/*      SPRMCC (0x21e08318)                                                     */
/*      SPRUCC (0x21e08318)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_WAC LOWER 32bits
*/


#define UPIMISC_OS_W_WAC_LO_KTI_LLDFX_REG 0x09320318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_WAC_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_OS_W_WAC_HI_KTI_LLDFX_REG supported on:                              */
/*      SPRA0 (0x21e0831c)                                                      */
/*      SPRB0 (0x21e0831c)                                                      */
/*      SPRHBM (0x21e0831c)                                                     */
/*      SPRC0 (0x21e0831c)                                                      */
/*      SPRMCC (0x21e0831c)                                                     */
/*      SPRUCC (0x21e0831c)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIMISC_OS_W_WAC UPPER 32bits
*/


#define UPIMISC_OS_W_WAC_HI_KTI_LLDFX_REG 0x0932031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_OS_W_WAC_HI_KTI_LLDFX_STRUCT;



















/* UPIMISC_DFX_CP_LO_KTI_LLDFX_REG supported on:                                */
/*      SPRA0 (0x21e08380)                                                      */
/*      SPRB0 (0x21e08380)                                                      */
/*      SPRHBM (0x21e08380)                                                     */
/*      SPRC0 (0x21e08380)                                                      */
/*      SPRMCC (0x21e08380)                                                     */
/*      SPRUCC (0x21e08380)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_CP LOWER 32bits
*/


#define UPIMISC_DFX_CP_LO_KTI_LLDFX_REG 0x09320380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_CP_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_DFX_CP_HI_KTI_LLDFX_REG supported on:                                */
/*      SPRA0 (0x21e08384)                                                      */
/*      SPRB0 (0x21e08384)                                                      */
/*      SPRHBM (0x21e08384)                                                     */
/*      SPRC0 (0x21e08384)                                                      */
/*      SPRMCC (0x21e08384)                                                     */
/*      SPRUCC (0x21e08384)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_CP UPPER 32bits
*/


#define UPIMISC_DFX_CP_HI_KTI_LLDFX_REG 0x09320384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_CP_HI_KTI_LLDFX_STRUCT;

/* UPIMISC_DFX_RAC_LO_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e08388)                                                      */
/*      SPRB0 (0x21e08388)                                                      */
/*      SPRHBM (0x21e08388)                                                     */
/*      SPRC0 (0x21e08388)                                                      */
/*      SPRMCC (0x21e08388)                                                     */
/*      SPRUCC (0x21e08388)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_RAC LOWER 32bits
*/


#define UPIMISC_DFX_RAC_LO_KTI_LLDFX_REG 0x09320388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_RAC_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_DFX_RAC_HI_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e0838c)                                                      */
/*      SPRB0 (0x21e0838c)                                                      */
/*      SPRHBM (0x21e0838c)                                                     */
/*      SPRC0 (0x21e0838c)                                                      */
/*      SPRMCC (0x21e0838c)                                                     */
/*      SPRUCC (0x21e0838c)                                                     */
/* Register default value on SPRA0: 0x20000500                                  */
/* Register default value on SPRB0: 0x20000500                                  */
/* Register default value on SPRHBM: 0x20000500                                 */
/* Register default value on SPRC0: 0x20000500                                  */
/* Register default value on SPRMCC: 0x20000500                                 */
/* Register default value on SPRUCC: 0x20000500                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_RAC UPPER 32bits
*/


#define UPIMISC_DFX_RAC_HI_KTI_LLDFX_REG 0x0932038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_RAC_HI_KTI_LLDFX_STRUCT;

/* UPIMISC_DFX_WAC_LO_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e08390)                                                      */
/*      SPRB0 (0x21e08390)                                                      */
/*      SPRHBM (0x21e08390)                                                     */
/*      SPRC0 (0x21e08390)                                                      */
/*      SPRMCC (0x21e08390)                                                     */
/*      SPRUCC (0x21e08390)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_WAC LOWER 32bits
*/


#define UPIMISC_DFX_WAC_LO_KTI_LLDFX_REG 0x09320390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_WAC_LO_KTI_LLDFX_STRUCT;

/* UPIMISC_DFX_WAC_HI_KTI_LLDFX_REG supported on:                               */
/*      SPRA0 (0x21e08394)                                                      */
/*      SPRB0 (0x21e08394)                                                      */
/*      SPRHBM (0x21e08394)                                                     */
/*      SPRC0 (0x21e08394)                                                      */
/*      SPRMCC (0x21e08394)                                                     */
/*      SPRUCC (0x21e08394)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3          */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktimisc/CFG_KTI0_3         */
/* Struct generated from SPRA0 BDF: 30_1_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIMISC_DFX_WAC UPPER 32bits
*/


#define UPIMISC_DFX_WAC_HI_KTI_LLDFX_REG 0x09320394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIMISC_DFX_WAC_HI_KTI_LLDFX_STRUCT;
#endif /* _KTI_LLDFX_h */
