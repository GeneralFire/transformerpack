
/** @file
  UBOX_DFX.h

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


#ifndef _UBOX_DFX_h
#define _UBOX_DFX_h
#include <Base.h>

/* VID_0_0_1_UBOX_DFX_REG supported on:                                         */
/*      SPRA0 (0x11e01000)                                                      */
/*      SPRB0 (0x11e01000)                                                      */
/*      SPRHBM (0x11e01000)                                                     */
/*      SPRC0 (0x11e01000)                                                      */
/*      SPRMCC (0x11e01000)                                                     */
/*      SPRUCC (0x11e01000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_0_0_1_UBOX_DFX_REG 0x13110000

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

} VID_0_0_1_UBOX_DFX_STRUCT;

/* DID_0_0_1_UBOX_DFX_REG supported on:                                         */
/*      SPRA0 (0x11e01002)                                                      */
/*      SPRB0 (0x11e01002)                                                      */
/*      SPRHBM (0x11e01002)                                                     */
/*      SPRC0 (0x11e01002)                                                      */
/*      SPRMCC (0x11e01002)                                                     */
/*      SPRUCC (0x11e01002)                                                     */
/* Register default value on SPRA0: 0x00003251                                  */
/* Register default value on SPRB0: 0x00003251                                  */
/* Register default value on SPRHBM: 0x00003251                                 */
/* Register default value on SPRC0: 0x00003251                                  */
/* Register default value on SPRMCC: 0x00003251                                 */
/* Register default value on SPRUCC: 0x00003251                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_0_0_1_UBOX_DFX_REG 0x13110002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003251*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_0_0_1_UBOX_DFX_STRUCT;

/* PCICMD_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x11e01004)                                                      */
/*      SPRB0 (0x11e01004)                                                      */
/*      SPRHBM (0x11e01004)                                                     */
/*      SPRC0 (0x11e01004)                                                      */
/*      SPRMCC (0x11e01004)                                                     */
/*      SPRUCC (0x11e01004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_0_0_1_UBOX_DFX_REG 0x13110004

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

} PCICMD_0_0_1_UBOX_DFX_STRUCT;

/* PCISTS_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x11e01006)                                                      */
/*      SPRB0 (0x11e01006)                                                      */
/*      SPRHBM (0x11e01006)                                                     */
/*      SPRC0 (0x11e01006)                                                      */
/*      SPRMCC (0x11e01006)                                                     */
/*      SPRUCC (0x11e01006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_0_0_1_UBOX_DFX_REG 0x13110006

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

} PCISTS_0_0_1_UBOX_DFX_STRUCT;

/* RID_CCR_0_0_1_UBOX_DFX_REG supported on:                                     */
/*      SPRA0 (0x21e01008)                                                      */
/*      SPRB0 (0x21e01008)                                                      */
/*      SPRHBM (0x21e01008)                                                     */
/*      SPRC0 (0x21e01008)                                                      */
/*      SPRMCC (0x21e01008)                                                     */
/*      SPRUCC (0x21e01008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_0_0_1_UBOX_DFX_REG 0x13120008

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

} RID_CCR_0_0_1_UBOX_DFX_STRUCT;

/* CLSR_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x1e0100c)                                                       */
/*      SPRB0 (0x1e0100c)                                                       */
/*      SPRHBM (0x1e0100c)                                                      */
/*      SPRC0 (0x1e0100c)                                                       */
/*      SPRMCC (0x1e0100c)                                                      */
/*      SPRUCC (0x1e0100c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_0_0_1_UBOX_DFX_REG 0x1310000C

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

} CLSR_0_0_1_UBOX_DFX_STRUCT;

/* PLAT_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x1e0100d)                                                       */
/*      SPRB0 (0x1e0100d)                                                       */
/*      SPRHBM (0x1e0100d)                                                      */
/*      SPRC0 (0x1e0100d)                                                       */
/*      SPRMCC (0x1e0100d)                                                      */
/*      SPRUCC (0x1e0100d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_0_0_1_UBOX_DFX_REG 0x1310000D

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

} PLAT_0_0_1_UBOX_DFX_STRUCT;

/* HDR_0_0_1_UBOX_DFX_REG supported on:                                         */
/*      SPRA0 (0x1e0100e)                                                       */
/*      SPRB0 (0x1e0100e)                                                       */
/*      SPRHBM (0x1e0100e)                                                      */
/*      SPRC0 (0x1e0100e)                                                       */
/*      SPRMCC (0x1e0100e)                                                      */
/*      SPRUCC (0x1e0100e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_0_0_1_UBOX_DFX_REG 0x1310000E

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

} HDR_0_0_1_UBOX_DFX_STRUCT;

/* BIST_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x1e0100f)                                                       */
/*      SPRB0 (0x1e0100f)                                                       */
/*      SPRHBM (0x1e0100f)                                                      */
/*      SPRC0 (0x1e0100f)                                                       */
/*      SPRMCC (0x1e0100f)                                                      */
/*      SPRUCC (0x1e0100f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_0_0_1_UBOX_DFX_REG 0x1310000F

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

} BIST_0_0_1_UBOX_DFX_STRUCT;

/* SVID_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x11e0102c)                                                      */
/*      SPRB0 (0x11e0102c)                                                      */
/*      SPRHBM (0x11e0102c)                                                     */
/*      SPRC0 (0x11e0102c)                                                      */
/*      SPRMCC (0x11e0102c)                                                     */
/*      SPRUCC (0x11e0102c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_0_0_1_UBOX_DFX_REG 0x1311002C

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

} SVID_0_0_1_UBOX_DFX_STRUCT;

/* SDID_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x11e0102e)                                                      */
/*      SPRB0 (0x11e0102e)                                                      */
/*      SPRHBM (0x11e0102e)                                                     */
/*      SPRC0 (0x11e0102e)                                                      */
/*      SPRMCC (0x11e0102e)                                                     */
/*      SPRUCC (0x11e0102e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_0_0_1_UBOX_DFX_REG 0x1311002E

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

} SDID_0_0_1_UBOX_DFX_STRUCT;

/* CAPPTR_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x1e01034)                                                       */
/*      SPRB0 (0x1e01034)                                                       */
/*      SPRHBM (0x1e01034)                                                      */
/*      SPRC0 (0x1e01034)                                                       */
/*      SPRMCC (0x1e01034)                                                      */
/*      SPRUCC (0x1e01034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_0_0_1_UBOX_DFX_REG 0x13100034

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

} CAPPTR_0_0_1_UBOX_DFX_STRUCT;

/* INTL_0_0_1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x1e0103c)                                                       */
/*      SPRB0 (0x1e0103c)                                                       */
/*      SPRHBM (0x1e0103c)                                                      */
/*      SPRC0 (0x1e0103c)                                                       */
/*      SPRMCC (0x1e0103c)                                                      */
/*      SPRUCC (0x1e0103c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_0_0_1_UBOX_DFX_REG 0x1310003C

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

} INTL_0_0_1_UBOX_DFX_STRUCT;

/* INTPIN_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x1e0103d)                                                       */
/*      SPRB0 (0x1e0103d)                                                       */
/*      SPRHBM (0x1e0103d)                                                      */
/*      SPRC0 (0x1e0103d)                                                       */
/*      SPRMCC (0x1e0103d)                                                      */
/*      SPRUCC (0x1e0103d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_0_0_1_UBOX_DFX_REG 0x1310003D

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

} INTPIN_0_0_1_UBOX_DFX_STRUCT;

/* MINGNT_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x1e0103e)                                                       */
/*      SPRB0 (0x1e0103e)                                                       */
/*      SPRHBM (0x1e0103e)                                                      */
/*      SPRC0 (0x1e0103e)                                                       */
/*      SPRMCC (0x1e0103e)                                                      */
/*      SPRUCC (0x1e0103e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_0_0_1_UBOX_DFX_REG 0x1310003E

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

} MINGNT_0_0_1_UBOX_DFX_STRUCT;

/* MAXLAT_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x1e0103f)                                                       */
/*      SPRB0 (0x1e0103f)                                                       */
/*      SPRHBM (0x1e0103f)                                                      */
/*      SPRC0 (0x1e0103f)                                                       */
/*      SPRMCC (0x1e0103f)                                                      */
/*      SPRUCC (0x1e0103f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_0_0_1_UBOX_DFX_REG 0x1310003F

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

} MAXLAT_0_0_1_UBOX_DFX_STRUCT;

/* PXPCAP_0_0_1_UBOX_DFX_REG supported on:                                      */
/*      SPRA0 (0x21e01040)                                                      */
/*      SPRB0 (0x21e01040)                                                      */
/*      SPRHBM (0x21e01040)                                                     */
/*      SPRC0 (0x21e01040)                                                      */
/*      SPRMCC (0x21e01040)                                                     */
/*      SPRUCC (0x21e01040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_0_0_1_UBOX_DFX_REG 0x13120040

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

} PXPCAP_0_0_1_UBOX_DFX_STRUCT;

/* PCIE_RSVD0_0_0_1_UBOX_DFX_REG supported on:                                  */
/*      SPRA0 (0x21e01044)                                                      */
/*      SPRB0 (0x21e01044)                                                      */
/*      SPRHBM (0x21e01044)                                                     */
/*      SPRC0 (0x21e01044)                                                      */
/*      SPRMCC (0x21e01044)                                                     */
/*      SPRUCC (0x21e01044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_0_0_1_UBOX_DFX_REG 0x13120044

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

} PCIE_RSVD0_0_0_1_UBOX_DFX_STRUCT;

/* PCIE_RSVD1_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01048)                                                      */
/*      SPRB0 (0x21e01048)                                                      */
/*      SPRHBM (0x21e01048)                                                     */
/*      SPRC0 (0x21e01048)                                                      */
/*      SPRMCC (0x21e01048)                                                     */
/*      SPRUCC (0x21e01048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_1_N0_UBOX_DFX_REG 0x13120048

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

} PCIE_RSVD1_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD1_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e0104c)                                                      */
/*      SPRB0 (0x21e0104c)                                                      */
/*      SPRHBM (0x21e0104c)                                                     */
/*      SPRC0 (0x21e0104c)                                                      */
/*      SPRMCC (0x21e0104c)                                                     */
/*      SPRUCC (0x21e0104c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_1_N1_UBOX_DFX_REG 0x1312004C

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

} PCIE_RSVD1_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD2_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01050)                                                      */
/*      SPRB0 (0x21e01050)                                                      */
/*      SPRHBM (0x21e01050)                                                     */
/*      SPRC0 (0x21e01050)                                                      */
/*      SPRMCC (0x21e01050)                                                     */
/*      SPRUCC (0x21e01050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_1_N0_UBOX_DFX_REG 0x13120050

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

} PCIE_RSVD2_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD2_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01054)                                                      */
/*      SPRB0 (0x21e01054)                                                      */
/*      SPRHBM (0x21e01054)                                                     */
/*      SPRC0 (0x21e01054)                                                      */
/*      SPRMCC (0x21e01054)                                                     */
/*      SPRUCC (0x21e01054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_1_N1_UBOX_DFX_REG 0x13120054

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

} PCIE_RSVD2_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD3_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01058)                                                      */
/*      SPRB0 (0x21e01058)                                                      */
/*      SPRHBM (0x21e01058)                                                     */
/*      SPRC0 (0x21e01058)                                                      */
/*      SPRMCC (0x21e01058)                                                     */
/*      SPRUCC (0x21e01058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_1_N0_UBOX_DFX_REG 0x13120058

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

} PCIE_RSVD3_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD3_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e0105c)                                                      */
/*      SPRB0 (0x21e0105c)                                                      */
/*      SPRHBM (0x21e0105c)                                                     */
/*      SPRC0 (0x21e0105c)                                                      */
/*      SPRMCC (0x21e0105c)                                                     */
/*      SPRUCC (0x21e0105c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_1_N1_UBOX_DFX_REG 0x1312005C

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

} PCIE_RSVD3_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD4_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01060)                                                      */
/*      SPRB0 (0x21e01060)                                                      */
/*      SPRHBM (0x21e01060)                                                     */
/*      SPRC0 (0x21e01060)                                                      */
/*      SPRMCC (0x21e01060)                                                     */
/*      SPRUCC (0x21e01060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_1_N0_UBOX_DFX_REG 0x13120060

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

} PCIE_RSVD4_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD4_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01064)                                                      */
/*      SPRB0 (0x21e01064)                                                      */
/*      SPRHBM (0x21e01064)                                                     */
/*      SPRC0 (0x21e01064)                                                      */
/*      SPRMCC (0x21e01064)                                                     */
/*      SPRUCC (0x21e01064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_1_N1_UBOX_DFX_REG 0x13120064

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

} PCIE_RSVD4_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD5_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01068)                                                      */
/*      SPRB0 (0x21e01068)                                                      */
/*      SPRHBM (0x21e01068)                                                     */
/*      SPRC0 (0x21e01068)                                                      */
/*      SPRMCC (0x21e01068)                                                     */
/*      SPRUCC (0x21e01068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_1_N0_UBOX_DFX_REG 0x13120068

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

} PCIE_RSVD5_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD5_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e0106c)                                                      */
/*      SPRB0 (0x21e0106c)                                                      */
/*      SPRHBM (0x21e0106c)                                                     */
/*      SPRC0 (0x21e0106c)                                                      */
/*      SPRMCC (0x21e0106c)                                                     */
/*      SPRUCC (0x21e0106c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_1_N1_UBOX_DFX_REG 0x1312006C

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

} PCIE_RSVD5_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD6_0_0_1_N0_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01070)                                                      */
/*      SPRB0 (0x21e01070)                                                      */
/*      SPRHBM (0x21e01070)                                                     */
/*      SPRC0 (0x21e01070)                                                      */
/*      SPRMCC (0x21e01070)                                                     */
/*      SPRUCC (0x21e01070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_1_N0_UBOX_DFX_REG 0x13120070

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

} PCIE_RSVD6_0_0_1_N0_UBOX_DFX_STRUCT;

/* PCIE_RSVD6_0_0_1_N1_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e01074)                                                      */
/*      SPRB0 (0x21e01074)                                                      */
/*      SPRHBM (0x21e01074)                                                     */
/*      SPRC0 (0x21e01074)                                                      */
/*      SPRMCC (0x21e01074)                                                     */
/*      SPRUCC (0x21e01074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_1_N1_UBOX_DFX_REG 0x13120074

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

} PCIE_RSVD6_0_0_1_N1_UBOX_DFX_STRUCT;

/* PCIE_RSVD7_0_0_1_UBOX_DFX_REG supported on:                                  */
/*      SPRA0 (0x21e01078)                                                      */
/*      SPRB0 (0x21e01078)                                                      */
/*      SPRHBM (0x21e01078)                                                     */
/*      SPRC0 (0x21e01078)                                                      */
/*      SPRMCC (0x21e01078)                                                     */
/*      SPRUCC (0x21e01078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_0_0_1_UBOX_DFX_REG 0x13120078

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

} PCIE_RSVD7_0_0_1_UBOX_DFX_STRUCT;

/* INVALIDBUSACCESS_UBOX_DFX_REG supported on:                                  */
/*      SPRA0 (0x21e01080)                                                      */
/*      SPRB0 (0x21e01080)                                                      */
/*      SPRHBM (0x21e01080)                                                     */
/*      SPRC0 (0x21e01080)                                                      */
/*      SPRMCC (0x21e01080)                                                     */
/*      SPRUCC (0x21e01080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logging of the first time invalid bus access on CFG. 
*/


#define INVALIDBUSACCESS_UBOX_DFX_REG 0x13120080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bus : 8;

                            /* Bits[7:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Log the bus number when it is the first time of
                               the invalid bus access on CFG
                            */
    UINT32 invalidbusaccess : 1;

                            /* Bits[8:8], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Set the bit when it is the first time of the
                               invalid bus access on CFG
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} INVALIDBUSACCESS_UBOX_DFX_STRUCT;

/* SMM_FEATURE_CONTROL_UBOX_DFX_REG supported on:                               */
/*      SPRA0 (0x21e0108c)                                                      */
/*      SPRB0 (0x21e0108c)                                                      */
/*      SPRHBM (0x21e0108c)                                                     */
/*      SPRC0 (0x21e0108c)                                                      */
/*      SPRMCC (0x21e0108c)                                                     */
/*      SPRUCC (0x21e0108c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This CSR holds enable bits for on-chip storage of SMM and
        Code Access Check Violation features. This CSR can be written only from
        within SMM but can be read by SMM and non SMM code. This CSR is cleared
        only on cold reset.
*/


#define SMM_FEATURE_CONTROL_UBOX_DFX_REG 0x1312008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               If Lock = 1, CPU will drop writes to the CSR If
                               Lock = 0, CPU will allow update to b[2:0] If the
                               lock register field is set, then the entire
                               register (including the lock) becomes read-only
                            */
    UINT32 smm_cpu_save_en : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               If 0, SMI/RSM will save/restore state in SMRAM
                               If 1, SMI/RSM will save/restore state from SRAM
                            */
    UINT32 smm_code_chk_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               If 1, generate unrecoverable MCHK on code fetch
                               outside SMRR If 0, do not report any errors for
                               code fetch outside SMRR
                            */
    UINT32 reserved : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMM_FEATURE_CONTROL_UBOX_DFX_STRUCT;

/* BIST_RESULTS0_UBOX_DFX_REG supported on:                                     */
/*      SPRA0 (0x21e010b4)                                                      */
/*      SPRB0 (0x21e010b4)                                                      */
/*      SPRHBM (0x21e010b4)                                                     */
/*      SPRC0 (0x21e010b4)                                                      */
/*      SPRMCC (0x21e010b4)                                                     */
/*      SPRUCC (0x21e010b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Config read-only access to core BIST0 results 
*/


#define BIST_RESULTS0_UBOX_DFX_REG 0x131200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 results : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* 0 - BIST failure, 1 - BIST pass */

  } Bits;
  UINT32 Data;

} BIST_RESULTS0_UBOX_DFX_STRUCT;

/* BIST_RESULTS1_UBOX_DFX_REG supported on:                                     */
/*      SPRA0 (0x21e010b8)                                                      */
/*      SPRB0 (0x21e010b8)                                                      */
/*      SPRHBM (0x21e010b8)                                                     */
/*      SPRC0 (0x21e010b8)                                                      */
/*      SPRMCC (0x21e010b8)                                                     */
/*      SPRUCC (0x21e010b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Config read-only access to core BIST1 results 
*/


#define BIST_RESULTS1_UBOX_DFX_REG 0x131200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 results : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* 0 - BIST failure, 1 - BIST pass */

  } Bits;
  UINT32 Data;

} BIST_RESULTS1_UBOX_DFX_STRUCT;

/* MMIO_BASE_UBOX_DFX_REG supported on:                                         */
/*      SPRA0 (0x21e010d0)                                                      */
/*      SPRB0 (0x21e010d0)                                                      */
/*      SPRHBM (0x21e010d0)                                                     */
/*      SPRC0 (0x21e010d0)                                                      */
/*      SPRMCC (0x21e010d0)                                                     */
/*      SPRUCC (0x21e010d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* BIOS configured  base compare
*/


#define MMIO_BASE_UBOX_DFX_REG 0x131200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_23 : 29;

                            /* Bits[28:0], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS configured 8MB region per Ubox for MMIO CSR
                               access
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MMIO_BASE_UBOX_DFX_STRUCT;

/* SCF_BAR_UBOX_DFX_REG supported on:                                           */
/*      SPRA0 (0x21e010d4)                                                      */
/*      SPRB0 (0x21e010d4)                                                      */
/*      SPRHBM (0x21e010d4)                                                     */
/*      SPRC0 (0x21e010d4)                                                      */
/*      SPRMCC (0x21e010d4)                                                     */
/*      SPRUCC (0x21e010d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register for SCF region (contains MS2IDI)
*/


#define SCF_BAR_UBOX_DFX_REG 0x131200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} SCF_BAR_UBOX_DFX_STRUCT;

/* MEM0_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010d8)                                                      */
/*      SPRB0 (0x21e010d8)                                                      */
/*      SPRHBM (0x21e010d8)                                                     */
/*      SPRC0 (0x21e010d8)                                                      */
/*      SPRMCC (0x21e010d8)                                                     */
/*      SPRUCC (0x21e010d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM0_BAR_UBOX_DFX_REG 0x131200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM0_BAR_UBOX_DFX_STRUCT;

/* MEM1_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010dc)                                                      */
/*      SPRB0 (0x21e010dc)                                                      */
/*      SPRHBM (0x21e010dc)                                                     */
/*      SPRC0 (0x21e010dc)                                                      */
/*      SPRMCC (0x21e010dc)                                                     */
/*      SPRUCC (0x21e010dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM1_BAR_UBOX_DFX_REG 0x131200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM1_BAR_UBOX_DFX_STRUCT;

/* MEM2_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010e0)                                                      */
/*      SPRB0 (0x21e010e0)                                                      */
/*      SPRHBM (0x21e010e0)                                                     */
/*      SPRC0 (0x21e010e0)                                                      */
/*      SPRMCC (0x21e010e0)                                                     */
/*      SPRUCC (0x21e010e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM2_BAR_UBOX_DFX_REG 0x131200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM2_BAR_UBOX_DFX_STRUCT;

/* MEM3_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010e4)                                                      */
/*      SPRB0 (0x21e010e4)                                                      */
/*      SPRHBM (0x21e010e4)                                                     */
/*      SPRC0 (0x21e010e4)                                                      */
/*      SPRMCC (0x21e010e4)                                                     */
/*      SPRUCC (0x21e010e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM3_BAR_UBOX_DFX_REG 0x131200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM3_BAR_UBOX_DFX_STRUCT;

/* MEM4_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010e8)                                                      */
/*      SPRB0 (0x21e010e8)                                                      */
/*      SPRHBM (0x21e010e8)                                                     */
/*      SPRC0 (0x21e010e8)                                                      */
/*      SPRMCC (0x21e010e8)                                                     */
/*      SPRUCC (0x21e010e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM4_BAR_UBOX_DFX_REG 0x131200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM4_BAR_UBOX_DFX_STRUCT;

/* MEM5_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010ec)                                                      */
/*      SPRB0 (0x21e010ec)                                                      */
/*      SPRHBM (0x21e010ec)                                                     */
/*      SPRC0 (0x21e010ec)                                                      */
/*      SPRMCC (0x21e010ec)                                                     */
/*      SPRUCC (0x21e010ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM5_BAR_UBOX_DFX_REG 0x131200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM5_BAR_UBOX_DFX_STRUCT;

/* MEM6_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010f0)                                                      */
/*      SPRB0 (0x21e010f0)                                                      */
/*      SPRHBM (0x21e010f0)                                                     */
/*      SPRC0 (0x21e010f0)                                                      */
/*      SPRMCC (0x21e010f0)                                                     */
/*      SPRUCC (0x21e010f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM6_BAR_UBOX_DFX_REG 0x131200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM6_BAR_UBOX_DFX_STRUCT;

/* MEM7_BAR_UBOX_DFX_REG supported on:                                          */
/*      SPRA0 (0x21e010f4)                                                      */
/*      SPRB0 (0x21e010f4)                                                      */
/*      SPRHBM (0x21e010f4)                                                     */
/*      SPRC0 (0x21e010f4)                                                      */
/*      SPRMCC (0x21e010f4)                                                     */
/*      SPRUCC (0x21e010f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define MEM7_BAR_UBOX_DFX_REG 0x131200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} MEM7_BAR_UBOX_DFX_STRUCT;

/* SBREG_BAR_UBOX_DFX_REG supported on:                                         */
/*      SPRA0 (0x21e010f8)                                                      */
/*      SPRB0 (0x21e010f8)                                                      */
/*      SPRHBM (0x21e010f8)                                                     */
/*      SPRC0 (0x21e010f8)                                                      */
/*      SPRMCC (0x21e010f8)                                                     */
/*      SPRUCC (0x21e010f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define SBREG_BAR_UBOX_DFX_REG 0x131200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} SBREG_BAR_UBOX_DFX_STRUCT;

/* PCU_BAR_UBOX_DFX_REG supported on:                                           */
/*      SPRA0 (0x21e010fc)                                                      */
/*      SPRB0 (0x21e010fc)                                                      */
/*      SPRHBM (0x21e010fc)                                                     */
/*      SPRC0 (0x21e010fc)                                                      */
/*      SPRMCC (0x21e010fc)                                                     */
/*      SPRUCC (0x21e010fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Bar compare register
*/


#define PCU_BAR_UBOX_DFX_REG 0x131200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 limit : 11;

                            /* Bits[23:13], Access Type=RW, default=0x00000000*/

                            /* 4K aligned base address (22:12) of the region */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bar_no : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /* GPSB Bar Encoding */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* 1 is valid BAR and 0 is invalid BAR */

  } Bits;
  UINT32 Data;

} PCU_BAR_UBOX_DFX_STRUCT;

/* PXPENHCAP_0_0_1_UBOX_DFX_REG supported on:                                   */
/*      SPRA0 (0x21e01100)                                                      */
/*      SPRB0 (0x21e01100)                                                      */
/*      SPRHBM (0x21e01100)                                                     */
/*      SPRC0 (0x21e01100)                                                      */
/*      SPRMCC (0x21e01100)                                                     */
/*      SPRUCC (0x21e01100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1     */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_PCI_CFG_0_0_1    */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_0_0_1_UBOX_DFX_REG 0x13120100

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

} PXPENHCAP_0_0_1_UBOX_DFX_STRUCT;

/* SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_REG supported on:                          */
/*      SPRA0 (0x21e01150)                                                      */
/*      SPRB0 (0x21e01150)                                                      */
/*      SPRHBM (0x21e01150)                                                     */
/*      SPRC0 (0x21e01150)                                                      */
/*      SPRMCC (0x21e01150)                                                     */
/*      SPRUCC (0x21e01150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register indicates the count of the number of threads in SMM_DELAYED state. Ubox increments/decrements the counter everytime ucode performs an SMM_DELAYED_UPD_SET/CLEAR command respectively
*/


#define SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_REG 0x13120150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 delayed_count : 10;

                            /* Bits[9:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates the count of the number of
                               threads in SMM_DELAYED state. Ubox
                               increments/decrements the counter everytime
                               ucode performs an SMM_DELAYED_UPD_SET/CLEAR
                               command respectively
                            */
    UINT32 reserved : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_STRUCT;

/* SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_REG supported on:                          */
/*      SPRA0 (0x21e01154)                                                      */
/*      SPRB0 (0x21e01154)                                                      */
/*      SPRHBM (0x21e01154)                                                     */
/*      SPRC0 (0x21e01154)                                                      */
/*      SPRMCC (0x21e01154)                                                     */
/*      SPRUCC (0x21e01154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register indicates the count of the number of threads in SMM_BLOCKED state. Ubox increments/decrements the counter everytime ucode performs an SMM_BLOCKED_UPD_SET/CLEAR command respectively
*/


#define SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_REG 0x13120154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blocked_count : 10;

                            /* Bits[9:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit indicates the count of the number of
                               threads in SMM_BLOCKED state. Ubox
                               increments/decrements the counter everytime
                               ucode performs an SMM_BLOCKED_UPD_SET/CLEAR
                               command respectively
                            */
    UINT32 reserved : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_STRUCT;

/* SMM_ENABLE_MASK_UBOX_DFX_REG supported on:                                   */
/*      SPRA0 (0x21e01158)                                                      */
/*      SPRB0 (0x21e01158)                                                      */
/*      SPRHBM (0x21e01158)                                                     */
/*      SPRC0 (0x21e01158)                                                      */
/*      SPRMCC (0x21e01158)                                                     */
/*      SPRUCC (0x21e01158)                                                     */
/* Register default value on SPRA0: 0x000003FF                                  */
/* Register default value on SPRB0: 0x000003FF                                  */
/* Register default value on SPRHBM: 0x000003FF                                 */
/* Register default value on SPRC0: 0x000003FF                                  */
/* Register default value on SPRMCC: 0x000003FF                                 */
/* Register default value on SPRUCC: 0x000003FF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Each bit in this register corresponds to a logical thread-id bit. When set, ucode masks the corresponding thread-id bit before checking for a match against the MATCH register during targeted SMIs
*/


#define SMM_ENABLE_MASK_UBOX_DFX_REG 0x13120158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 thread_id_mask : 10;

                            /* Bits[9:0], Access Type=RW/L, default=0x000003FF*/

                            /*
                               Each bit in this field corresponds to a logical
                               thread-id bit. When set, ucode masks the
                               corresponding thread-id bit before checking for
                               a match against the MATCH register during
                               targeted SMIs
                            */
    UINT32 reserved : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMM_ENABLE_MASK_UBOX_DFX_STRUCT;

/* SMM_ENABLE_MATCH_UBOX_DFX_REG supported on:                                  */
/*      SPRA0 (0x21e0115c)                                                      */
/*      SPRB0 (0x21e0115c)                                                      */
/*      SPRHBM (0x21e0115c)                                                     */
/*      SPRC0 (0x21e0115c)                                                      */
/*      SPRMCC (0x21e0115c)                                                     */
/*      SPRUCC (0x21e0115c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Each bit in this register corresponds to a logical thread-id bit. When not masked, ucode checks for a match against threadId value, after masking thread-id bits based on the MASK register, during targeted SMIs
*/


#define SMM_ENABLE_MATCH_UBOX_DFX_REG 0x1312015C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 thread_id_match : 10;

                            /* Bits[9:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Each bit in this field corresponds to a logical
                               thread-id bit. When not masked, ucode checks for
                               a match against threadId value, after masking
                               thread-id bits based on the MASK register,
                               during targeted SMIs
                            */
    UINT32 reserved : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMM_ENABLE_MATCH_UBOX_DFX_STRUCT;

/* OS_W_CP_N0_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x21e01160)                                                      */
/*      SPRB0 (0x21e01160)                                                      */
/*      SPRHBM (0x21e01160)                                                     */
/*      SPRC0 (0x21e01160)                                                      */
/*      SPRMCC (0x21e01160)                                                     */
/*      SPRUCC (0x21e01160)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N0_UBOX_DFX_REG 0x13120160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_N0_UBOX_DFX_STRUCT;

/* OS_W_CP_N1_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x21e01164)                                                      */
/*      SPRB0 (0x21e01164)                                                      */
/*      SPRHBM (0x21e01164)                                                     */
/*      SPRC0 (0x21e01164)                                                      */
/*      SPRMCC (0x21e01164)                                                     */
/*      SPRUCC (0x21e01164)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N1_UBOX_DFX_REG 0x13120164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_N1_UBOX_DFX_STRUCT;

/* OS_W_CP_LO_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x21e01170)                                                      */
/*      SPRB0 (0x21e01170)                                                      */
/*      SPRHBM (0x21e01170)                                                     */
/*      SPRC0 (0x21e01170)                                                      */
/*      SPRMCC (0x21e01170)                                                     */
/*      SPRUCC (0x21e01170)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_LO_UBOX_DFX_REG 0x13120170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_LO_UBOX_DFX_STRUCT;

/* OS_W_RAC_LO_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e01180)                                                      */
/*      SPRB0 (0x21e01180)                                                      */
/*      SPRHBM (0x21e01180)                                                     */
/*      SPRC0 (0x21e01180)                                                      */
/*      SPRMCC (0x21e01180)                                                     */
/*      SPRUCC (0x21e01180)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_LO_UBOX_DFX_REG 0x13120180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_LO_UBOX_DFX_STRUCT;

/* OS_W_RAC_HI_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e01184)                                                      */
/*      SPRB0 (0x21e01184)                                                      */
/*      SPRHBM (0x21e01184)                                                     */
/*      SPRC0 (0x21e01184)                                                      */
/*      SPRMCC (0x21e01184)                                                     */
/*      SPRUCC (0x21e01184)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_HI_UBOX_DFX_REG 0x13120184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_HI_UBOX_DFX_STRUCT;

/* OS_W_WAC_LO_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e01188)                                                      */
/*      SPRB0 (0x21e01188)                                                      */
/*      SPRHBM (0x21e01188)                                                     */
/*      SPRC0 (0x21e01188)                                                      */
/*      SPRMCC (0x21e01188)                                                     */
/*      SPRUCC (0x21e01188)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_LO_UBOX_DFX_REG 0x13120188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_LO_UBOX_DFX_STRUCT;

/* OS_W_WAC_HI_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e0118c)                                                      */
/*      SPRB0 (0x21e0118c)                                                      */
/*      SPRHBM (0x21e0118c)                                                     */
/*      SPRC0 (0x21e0118c)                                                      */
/*      SPRMCC (0x21e0118c)                                                     */
/*      SPRUCC (0x21e0118c)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_HI_UBOX_DFX_REG 0x1312018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_HI_UBOX_DFX_STRUCT;







/* OS_W_CP_HI_UBOX_DFX_REG supported on:                                        */
/*      SPRA0 (0x21e011b0)                                                      */
/*      SPRB0 (0x21e011b0)                                                      */
/*      SPRHBM (0x21e011b0)                                                     */
/*      SPRC0 (0x21e011b0)                                                      */
/*      SPRMCC (0x21e011b0)                                                     */
/*      SPRUCC (0x21e011b0)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRB0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRHBM Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRC0 Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1           */
/* SPRMCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* SPRUCC Register File:    sprsp_top/ubox/ncuracu/ncuracu/CFG_UBOX_F1          */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_HI_UBOX_DFX_REG 0x131201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_HI_UBOX_DFX_STRUCT;

/* OS_W_RAC_N0_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e011d0)                                                      */
/*      SPRB0 (0x21e011d0)                                                      */
/*      SPRHBM (0x21e011d0)                                                     */
/*      SPRC0 (0x21e011d0)                                                      */
/*      SPRMCC (0x21e011d0)                                                     */
/*      SPRUCC (0x21e011d0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N0_UBOX_DFX_REG 0x131201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_N0_UBOX_DFX_STRUCT;

/* OS_W_RAC_N1_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e011d4)                                                      */
/*      SPRB0 (0x21e011d4)                                                      */
/*      SPRHBM (0x21e011d4)                                                     */
/*      SPRC0 (0x21e011d4)                                                      */
/*      SPRMCC (0x21e011d4)                                                     */
/*      SPRUCC (0x21e011d4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N1_UBOX_DFX_REG 0x131201D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_N1_UBOX_DFX_STRUCT;

/* OS_W_WAC_N0_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e011d8)                                                      */
/*      SPRB0 (0x21e011d8)                                                      */
/*      SPRHBM (0x21e011d8)                                                     */
/*      SPRC0 (0x21e011d8)                                                      */
/*      SPRMCC (0x21e011d8)                                                     */
/*      SPRUCC (0x21e011d8)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N0_UBOX_DFX_REG 0x131201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_N0_UBOX_DFX_STRUCT;

/* OS_W_WAC_N1_UBOX_DFX_REG supported on:                                       */
/*      SPRA0 (0x21e011dc)                                                      */
/*      SPRB0 (0x21e011dc)                                                      */
/*      SPRHBM (0x21e011dc)                                                     */
/*      SPRC0 (0x21e011dc)                                                      */
/*      SPRMCC (0x21e011dc)                                                     */
/*      SPRUCC (0x21e011dc)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N1_UBOX_DFX_REG 0x131201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_N1_UBOX_DFX_STRUCT;







/* LB_MCHECK_W_CP_N0_UBOX_DFX_REG supported on:                                 */
/*      SPRA0 (0x21e01210)                                                      */
/*      SPRB0 (0x21e01210)                                                      */
/*      SPRHBM (0x21e01210)                                                     */
/*      SPRC0 (0x21e01210)                                                      */
/*      SPRMCC (0x21e01210)                                                     */
/*      SPRUCC (0x21e01210)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N0_UBOX_DFX_REG 0x13120210

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

} LB_MCHECK_W_CP_N0_UBOX_DFX_STRUCT;

/* LB_MCHECK_W_CP_N1_UBOX_DFX_REG supported on:                                 */
/*      SPRA0 (0x21e01214)                                                      */
/*      SPRB0 (0x21e01214)                                                      */
/*      SPRHBM (0x21e01214)                                                     */
/*      SPRC0 (0x21e01214)                                                      */
/*      SPRMCC (0x21e01214)                                                     */
/*      SPRUCC (0x21e01214)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N1_UBOX_DFX_REG 0x13120214

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

} LB_MCHECK_W_CP_N1_UBOX_DFX_STRUCT;

/* LB_MCHECK_W_RAC_N0_UBOX_DFX_REG supported on:                                */
/*      SPRA0 (0x21e01218)                                                      */
/*      SPRB0 (0x21e01218)                                                      */
/*      SPRHBM (0x21e01218)                                                     */
/*      SPRC0 (0x21e01218)                                                      */
/*      SPRMCC (0x21e01218)                                                     */
/*      SPRUCC (0x21e01218)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N0_UBOX_DFX_REG 0x13120218

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

} LB_MCHECK_W_RAC_N0_UBOX_DFX_STRUCT;

/* LB_MCHECK_W_RAC_N1_UBOX_DFX_REG supported on:                                */
/*      SPRA0 (0x21e0121c)                                                      */
/*      SPRB0 (0x21e0121c)                                                      */
/*      SPRHBM (0x21e0121c)                                                     */
/*      SPRC0 (0x21e0121c)                                                      */
/*      SPRMCC (0x21e0121c)                                                     */
/*      SPRUCC (0x21e0121c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N1_UBOX_DFX_REG 0x1312021C

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

} LB_MCHECK_W_RAC_N1_UBOX_DFX_STRUCT;

/* LB_MCHECK_W_WAC_N0_UBOX_DFX_REG supported on:                                */
/*      SPRA0 (0x21e01220)                                                      */
/*      SPRB0 (0x21e01220)                                                      */
/*      SPRHBM (0x21e01220)                                                     */
/*      SPRC0 (0x21e01220)                                                      */
/*      SPRMCC (0x21e01220)                                                     */
/*      SPRUCC (0x21e01220)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N0_UBOX_DFX_REG 0x13120220

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

} LB_MCHECK_W_WAC_N0_UBOX_DFX_STRUCT;

/* LB_MCHECK_W_WAC_N1_UBOX_DFX_REG supported on:                                */
/*      SPRA0 (0x21e01224)                                                      */
/*      SPRB0 (0x21e01224)                                                      */
/*      SPRHBM (0x21e01224)                                                     */
/*      SPRC0 (0x21e01224)                                                      */
/*      SPRMCC (0x21e01224)                                                     */
/*      SPRUCC (0x21e01224)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRB0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRHBM Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRC0 Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1             */
/* SPRMCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* SPRUCC Register File:    sprsp_top/ubox/ncdecs/ncdecs/CFG_UBOX_F1            */
/* Struct generated from SPRA0 BDF: 30_0_1                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N1_UBOX_DFX_REG 0x13120224

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

} LB_MCHECK_W_WAC_N1_UBOX_DFX_STRUCT;

#endif /* _UBOX_DFX_h */
