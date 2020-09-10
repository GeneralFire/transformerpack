
/** @file
  UBOX_M2U.h

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


#ifndef _UBOX_M2U_h
#define _UBOX_M2U_h
#include <Base.h>

/* VID_0_0_5_UBOX_M2U_REG supported on:                                         */
/*      SPRA0 (0x11e05000)                                                      */
/*      SPRB0 (0x11e05000)                                                      */
/*      SPRHBM (0x11e05000)                                                     */
/*      SPRC0 (0x11e05000)                                                      */
/*      SPRMCC (0x11e05000)                                                     */
/*      SPRUCC (0x11e05000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_0_0_5_UBOX_M2U_REG 0x13510000

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

} VID_0_0_5_UBOX_M2U_STRUCT;

/* DID_0_0_5_UBOX_M2U_REG supported on:                                         */
/*      SPRA0 (0x11e05002)                                                      */
/*      SPRB0 (0x11e05002)                                                      */
/*      SPRHBM (0x11e05002)                                                     */
/*      SPRC0 (0x11e05002)                                                      */
/*      SPRMCC (0x11e05002)                                                     */
/*      SPRUCC (0x11e05002)                                                     */
/* Register default value on SPRA0: 0x00003255                                  */
/* Register default value on SPRB0: 0x00003255                                  */
/* Register default value on SPRHBM: 0x00003255                                 */
/* Register default value on SPRC0: 0x00003255                                  */
/* Register default value on SPRMCC: 0x00003255                                 */
/* Register default value on SPRUCC: 0x00003255                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_0_0_5_UBOX_M2U_REG 0x13510002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003255*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_0_0_5_UBOX_M2U_STRUCT;

/* PCICMD_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x11e05004)                                                      */
/*      SPRB0 (0x11e05004)                                                      */
/*      SPRHBM (0x11e05004)                                                     */
/*      SPRC0 (0x11e05004)                                                      */
/*      SPRMCC (0x11e05004)                                                     */
/*      SPRUCC (0x11e05004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_0_0_5_UBOX_M2U_REG 0x13510004

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

} PCICMD_0_0_5_UBOX_M2U_STRUCT;

/* PCISTS_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x11e05006)                                                      */
/*      SPRB0 (0x11e05006)                                                      */
/*      SPRHBM (0x11e05006)                                                     */
/*      SPRC0 (0x11e05006)                                                      */
/*      SPRMCC (0x11e05006)                                                     */
/*      SPRUCC (0x11e05006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_0_0_5_UBOX_M2U_REG 0x13510006

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

} PCISTS_0_0_5_UBOX_M2U_STRUCT;

/* RID_CCR_0_0_5_UBOX_M2U_REG supported on:                                     */
/*      SPRA0 (0x21e05008)                                                      */
/*      SPRB0 (0x21e05008)                                                      */
/*      SPRHBM (0x21e05008)                                                     */
/*      SPRC0 (0x21e05008)                                                      */
/*      SPRMCC (0x21e05008)                                                     */
/*      SPRUCC (0x21e05008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_0_0_5_UBOX_M2U_REG 0x13520008

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

} RID_CCR_0_0_5_UBOX_M2U_STRUCT;

/* CLSR_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x1e0500c)                                                       */
/*      SPRB0 (0x1e0500c)                                                       */
/*      SPRHBM (0x1e0500c)                                                      */
/*      SPRC0 (0x1e0500c)                                                       */
/*      SPRMCC (0x1e0500c)                                                      */
/*      SPRUCC (0x1e0500c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_0_0_5_UBOX_M2U_REG 0x1350000C

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

} CLSR_0_0_5_UBOX_M2U_STRUCT;

/* PLAT_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x1e0500d)                                                       */
/*      SPRB0 (0x1e0500d)                                                       */
/*      SPRHBM (0x1e0500d)                                                      */
/*      SPRC0 (0x1e0500d)                                                       */
/*      SPRMCC (0x1e0500d)                                                      */
/*      SPRUCC (0x1e0500d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_0_0_5_UBOX_M2U_REG 0x1350000D

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

} PLAT_0_0_5_UBOX_M2U_STRUCT;

/* HDR_0_0_5_UBOX_M2U_REG supported on:                                         */
/*      SPRA0 (0x1e0500e)                                                       */
/*      SPRB0 (0x1e0500e)                                                       */
/*      SPRHBM (0x1e0500e)                                                      */
/*      SPRC0 (0x1e0500e)                                                       */
/*      SPRMCC (0x1e0500e)                                                      */
/*      SPRUCC (0x1e0500e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_0_0_5_UBOX_M2U_REG 0x1350000E

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

} HDR_0_0_5_UBOX_M2U_STRUCT;

/* BIST_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x1e0500f)                                                       */
/*      SPRB0 (0x1e0500f)                                                       */
/*      SPRHBM (0x1e0500f)                                                      */
/*      SPRC0 (0x1e0500f)                                                       */
/*      SPRMCC (0x1e0500f)                                                      */
/*      SPRUCC (0x1e0500f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_0_0_5_UBOX_M2U_REG 0x1350000F

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

} BIST_0_0_5_UBOX_M2U_STRUCT;

/* SVID_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x11e0502c)                                                      */
/*      SPRB0 (0x11e0502c)                                                      */
/*      SPRHBM (0x11e0502c)                                                     */
/*      SPRC0 (0x11e0502c)                                                      */
/*      SPRMCC (0x11e0502c)                                                     */
/*      SPRUCC (0x11e0502c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_0_0_5_UBOX_M2U_REG 0x1351002C

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

} SVID_0_0_5_UBOX_M2U_STRUCT;

/* SDID_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x11e0502e)                                                      */
/*      SPRB0 (0x11e0502e)                                                      */
/*      SPRHBM (0x11e0502e)                                                     */
/*      SPRC0 (0x11e0502e)                                                      */
/*      SPRMCC (0x11e0502e)                                                     */
/*      SPRUCC (0x11e0502e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_0_0_5_UBOX_M2U_REG 0x1351002E

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

} SDID_0_0_5_UBOX_M2U_STRUCT;

/* CAPPTR_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x1e05034)                                                       */
/*      SPRB0 (0x1e05034)                                                       */
/*      SPRHBM (0x1e05034)                                                      */
/*      SPRC0 (0x1e05034)                                                       */
/*      SPRMCC (0x1e05034)                                                      */
/*      SPRUCC (0x1e05034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_0_0_5_UBOX_M2U_REG 0x13500034

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

} CAPPTR_0_0_5_UBOX_M2U_STRUCT;

/* INTL_0_0_5_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x1e0503c)                                                       */
/*      SPRB0 (0x1e0503c)                                                       */
/*      SPRHBM (0x1e0503c)                                                      */
/*      SPRC0 (0x1e0503c)                                                       */
/*      SPRMCC (0x1e0503c)                                                      */
/*      SPRUCC (0x1e0503c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_0_0_5_UBOX_M2U_REG 0x1350003C

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

} INTL_0_0_5_UBOX_M2U_STRUCT;

/* INTPIN_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x1e0503d)                                                       */
/*      SPRB0 (0x1e0503d)                                                       */
/*      SPRHBM (0x1e0503d)                                                      */
/*      SPRC0 (0x1e0503d)                                                       */
/*      SPRMCC (0x1e0503d)                                                      */
/*      SPRUCC (0x1e0503d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_0_0_5_UBOX_M2U_REG 0x1350003D

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

} INTPIN_0_0_5_UBOX_M2U_STRUCT;

/* MINGNT_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x1e0503e)                                                       */
/*      SPRB0 (0x1e0503e)                                                       */
/*      SPRHBM (0x1e0503e)                                                      */
/*      SPRC0 (0x1e0503e)                                                       */
/*      SPRMCC (0x1e0503e)                                                      */
/*      SPRUCC (0x1e0503e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_0_0_5_UBOX_M2U_REG 0x1350003E

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

} MINGNT_0_0_5_UBOX_M2U_STRUCT;

/* MAXLAT_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x1e0503f)                                                       */
/*      SPRB0 (0x1e0503f)                                                       */
/*      SPRHBM (0x1e0503f)                                                      */
/*      SPRC0 (0x1e0503f)                                                       */
/*      SPRMCC (0x1e0503f)                                                      */
/*      SPRUCC (0x1e0503f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_0_0_5_UBOX_M2U_REG 0x1350003F

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

} MAXLAT_0_0_5_UBOX_M2U_STRUCT;

/* PXPCAP_0_0_5_UBOX_M2U_REG supported on:                                      */
/*      SPRA0 (0x21e05040)                                                      */
/*      SPRB0 (0x21e05040)                                                      */
/*      SPRHBM (0x21e05040)                                                     */
/*      SPRC0 (0x21e05040)                                                      */
/*      SPRMCC (0x21e05040)                                                     */
/*      SPRUCC (0x21e05040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_0_0_5_UBOX_M2U_REG 0x13520040

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

} PXPCAP_0_0_5_UBOX_M2U_STRUCT;

/* PCIE_RSVD0_0_0_5_UBOX_M2U_REG supported on:                                  */
/*      SPRA0 (0x21e05044)                                                      */
/*      SPRB0 (0x21e05044)                                                      */
/*      SPRHBM (0x21e05044)                                                     */
/*      SPRC0 (0x21e05044)                                                      */
/*      SPRMCC (0x21e05044)                                                     */
/*      SPRUCC (0x21e05044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_0_0_5_UBOX_M2U_REG 0x13520044

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

} PCIE_RSVD0_0_0_5_UBOX_M2U_STRUCT;

/* PCIE_RSVD1_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05048)                                                      */
/*      SPRB0 (0x21e05048)                                                      */
/*      SPRHBM (0x21e05048)                                                     */
/*      SPRC0 (0x21e05048)                                                      */
/*      SPRMCC (0x21e05048)                                                     */
/*      SPRUCC (0x21e05048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_5_N0_UBOX_M2U_REG 0x13520048

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

} PCIE_RSVD1_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD1_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e0504c)                                                      */
/*      SPRB0 (0x21e0504c)                                                      */
/*      SPRHBM (0x21e0504c)                                                     */
/*      SPRC0 (0x21e0504c)                                                      */
/*      SPRMCC (0x21e0504c)                                                     */
/*      SPRUCC (0x21e0504c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_5_N1_UBOX_M2U_REG 0x1352004C

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

} PCIE_RSVD1_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD2_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05050)                                                      */
/*      SPRB0 (0x21e05050)                                                      */
/*      SPRHBM (0x21e05050)                                                     */
/*      SPRC0 (0x21e05050)                                                      */
/*      SPRMCC (0x21e05050)                                                     */
/*      SPRUCC (0x21e05050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_5_N0_UBOX_M2U_REG 0x13520050

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

} PCIE_RSVD2_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD2_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05054)                                                      */
/*      SPRB0 (0x21e05054)                                                      */
/*      SPRHBM (0x21e05054)                                                     */
/*      SPRC0 (0x21e05054)                                                      */
/*      SPRMCC (0x21e05054)                                                     */
/*      SPRUCC (0x21e05054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_5_N1_UBOX_M2U_REG 0x13520054

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

} PCIE_RSVD2_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD3_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05058)                                                      */
/*      SPRB0 (0x21e05058)                                                      */
/*      SPRHBM (0x21e05058)                                                     */
/*      SPRC0 (0x21e05058)                                                      */
/*      SPRMCC (0x21e05058)                                                     */
/*      SPRUCC (0x21e05058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_5_N0_UBOX_M2U_REG 0x13520058

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

} PCIE_RSVD3_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD3_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e0505c)                                                      */
/*      SPRB0 (0x21e0505c)                                                      */
/*      SPRHBM (0x21e0505c)                                                     */
/*      SPRC0 (0x21e0505c)                                                      */
/*      SPRMCC (0x21e0505c)                                                     */
/*      SPRUCC (0x21e0505c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_5_N1_UBOX_M2U_REG 0x1352005C

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

} PCIE_RSVD3_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD4_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05060)                                                      */
/*      SPRB0 (0x21e05060)                                                      */
/*      SPRHBM (0x21e05060)                                                     */
/*      SPRC0 (0x21e05060)                                                      */
/*      SPRMCC (0x21e05060)                                                     */
/*      SPRUCC (0x21e05060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_5_N0_UBOX_M2U_REG 0x13520060

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

} PCIE_RSVD4_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD4_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05064)                                                      */
/*      SPRB0 (0x21e05064)                                                      */
/*      SPRHBM (0x21e05064)                                                     */
/*      SPRC0 (0x21e05064)                                                      */
/*      SPRMCC (0x21e05064)                                                     */
/*      SPRUCC (0x21e05064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_5_N1_UBOX_M2U_REG 0x13520064

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

} PCIE_RSVD4_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD5_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05068)                                                      */
/*      SPRB0 (0x21e05068)                                                      */
/*      SPRHBM (0x21e05068)                                                     */
/*      SPRC0 (0x21e05068)                                                      */
/*      SPRMCC (0x21e05068)                                                     */
/*      SPRUCC (0x21e05068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_5_N0_UBOX_M2U_REG 0x13520068

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

} PCIE_RSVD5_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD5_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e0506c)                                                      */
/*      SPRB0 (0x21e0506c)                                                      */
/*      SPRHBM (0x21e0506c)                                                     */
/*      SPRC0 (0x21e0506c)                                                      */
/*      SPRMCC (0x21e0506c)                                                     */
/*      SPRUCC (0x21e0506c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_5_N1_UBOX_M2U_REG 0x1352006C

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

} PCIE_RSVD5_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD6_0_0_5_N0_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05070)                                                      */
/*      SPRB0 (0x21e05070)                                                      */
/*      SPRHBM (0x21e05070)                                                     */
/*      SPRC0 (0x21e05070)                                                      */
/*      SPRMCC (0x21e05070)                                                     */
/*      SPRUCC (0x21e05070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_5_N0_UBOX_M2U_REG 0x13520070

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

} PCIE_RSVD6_0_0_5_N0_UBOX_M2U_STRUCT;

/* PCIE_RSVD6_0_0_5_N1_UBOX_M2U_REG supported on:                               */
/*      SPRA0 (0x21e05074)                                                      */
/*      SPRB0 (0x21e05074)                                                      */
/*      SPRHBM (0x21e05074)                                                     */
/*      SPRC0 (0x21e05074)                                                      */
/*      SPRMCC (0x21e05074)                                                     */
/*      SPRUCC (0x21e05074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_5_N1_UBOX_M2U_REG 0x13520074

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

} PCIE_RSVD6_0_0_5_N1_UBOX_M2U_STRUCT;

/* PCIE_RSVD7_0_0_5_UBOX_M2U_REG supported on:                                  */
/*      SPRA0 (0x21e05078)                                                      */
/*      SPRB0 (0x21e05078)                                                      */
/*      SPRHBM (0x21e05078)                                                     */
/*      SPRC0 (0x21e05078)                                                      */
/*      SPRMCC (0x21e05078)                                                     */
/*      SPRUCC (0x21e05078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_0_0_5_UBOX_M2U_REG 0x13520078

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

} PCIE_RSVD7_0_0_5_UBOX_M2U_STRUCT;

/* PXPENHCAP_0_0_5_UBOX_M2U_REG supported on:                                   */
/*      SPRA0 (0x21e05100)                                                      */
/*      SPRB0 (0x21e05100)                                                      */
/*      SPRHBM (0x21e05100)                                                     */
/*      SPRC0 (0x21e05100)                                                      */
/*      SPRMCC (0x21e05100)                                                     */
/*      SPRUCC (0x21e05100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_0_0_5_UBOX_M2U_REG 0x13520100

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

} PXPENHCAP_0_0_5_UBOX_M2U_STRUCT;

/* QPI_RT_UBOX_M2U_REG supported on:                                            */
/*      SPRA0 (0x21e05140)                                                      */
/*      SPRB0 (0x21e05140)                                                      */
/*      SPRHBM (0x21e05140)                                                     */
/*      SPRC0 (0x21e05140)                                                      */
/*      SPRMCC (0x21e05140)                                                     */
/*      SPRUCC (0x21e05140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Intel UPI Routing Table
*/


#define QPI_RT_UBOX_M2U_REG 0x13520140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 qpiroutingtable : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit 0 corresponds to NodeID0 Bit 1 corresponds
                               to NodeID1 ... Value of 0 means Intel UPI 0
                               Value of 1 means Intel UPI 1
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} QPI_RT_UBOX_M2U_STRUCT;

/* R2PGNCTRL_UBOX_M2U_REG supported on:                                         */
/*      SPRA0 (0x21e05148)                                                      */
/*      SPRB0 (0x21e05148)                                                      */
/*      SPRHBM (0x21e05148)                                                     */
/*      SPRC0 (0x21e05148)                                                      */
/*      SPRMCC (0x21e05148)                                                     */
/*      SPRUCC (0x21e05148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe General Control Register
*/


#define R2PGNCTRL_UBOX_M2U_REG 0x13520148

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 qpi_rt_sel_ndr_drs : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, R2PCIe routes NDR and DRS
                               packets destined to remote sockets, using the
                               QPI_RT. When this bit is clear, R2PCIe routes
                               NDR and DRS packets destined to remote sockets,
                               using the Intel UPI Link_ID field sent by
                               IIO/Ubox along with these packet.
                            */
    UINT32 rsvd_2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PGNCTRL_UBOX_M2U_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 qpi_rt_sel_ndr_drs : 8;

                            /* Bits[8:1], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, R2PCIe routes NDR and DRS
                               packets destined to remote sockets, using the
                               QPI_RT. When this bit is clear, R2PCIe routes
                               NDR and DRS packets destined to remote sockets,
                               using the Intel UPI Link_ID field sent by
                               IIO/Ubox along with these packet. Bit 1 for
                               node-id 0, bit 2 for node-id 1, bit 3 for node-
                               id 2, bit 4 for node-id 3, bit 5 for node-id 4,
                               bit 6 for node-id 5, bit 7 for node-id 6, bit 8
                               for node-id 7.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PGNCTRL_UBOX_M2U_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* R2DEFEATURE_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e05158)                                                      */
/*      SPRB0 (0x21e05158)                                                      */
/*      SPRHBM (0x21e05158)                                                     */
/*      SPRC0 (0x21e05158)                                                      */
/*      SPRMCC (0x21e05158)                                                     */
/*      SPRUCC (0x21e05158)                                                     */
/* Register default value on SPRA0: 0x00000200                                  */
/* Register default value on SPRB0: 0x00000200                                  */
/* Register default value on SPRHBM: 0x00000200                                 */
/* Register default value on SPRC0: 0x00000200                                  */
/* Register default value on SPRMCC: 0x00000200                                 */
/* Register default value on SPRUCC: 0x00000200                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe defeature register
*/


#define R2DEFEATURE_UBOX_M2U_REG 0x13520158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 9;

                            /* Bits[8:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0ncbiio2ubxinitialcred : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000001*/

                            /* Initial credit for request from IIO to Ubox */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2DEFEATURE_UBOX_M2U_STRUCT;














/* R2GLERRCFG_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x21e05250)                                                      */
/*      SPRB0 (0x21e05250)                                                      */
/*      SPRHBM (0x21e05250)                                                     */
/*      SPRC0 (0x21e05250)                                                      */
/*      SPRMCC (0x21e05250)                                                     */
/*      SPRUCC (0x21e05250)                                                     */
/* Register default value on SPRA0: 0x000066E0                                  */
/* Register default value on SPRB0: 0x000066E0                                  */
/* Register default value on SPRHBM: 0x000066E0                                 */
/* Register default value on SPRC0: 0x000066E0                                  */
/* Register default value on SPRMCC: 0x000066E0                                 */
/* Register default value on SPRUCC: 0x000066E0                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe global viral/fatal error configuration
*/


#define R2GLERRCFG_UBOX_M2U_REG 0x13520250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 zero3 : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /* Zero 3 */
    UINT32 resetglobalfatalerror : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global fatal error status. */
    UINT32 resetglobalviral : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global viral status. */
    UINT32 maskuboxviralin : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Set 1 to block viral status of Ubox */
    UINT32 maskuboxfatalerrorin : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block fatal error status of Ubox to
                               global fatal status
                            */
    UINT32 maskiiofatalerrorin : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block fatal error status from IIO to
                               global fatal status
                            */
    UINT32 zero1 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Zero 1 */
    UINT32 maskiioviralout : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block Vrial status from global viral
                               status to IIO
                            */
    UINT32 maskiioviralin : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block Vrial status from IIO to global
                               viral status
                            */
    UINT32 zero2 : 2;

                            /* Bits[12:11], Access Type=RO, default=0x00000000*/

                            /* Zero 2 */
    UINT32 maskr2fatalerror : 2;

                            /* Bits[14:13], Access Type=RW/P, default=0x00000003*/

                            /*
                               Set 1 to block Set 1 to block from R2PCIE Error
                               to global fatal error status
                            */
    UINT32 localerrorstatus : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Read only R2PCIe error status */
    UINT32 viralstatustoiio : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Read only to IIO viral status */
    UINT32 viralstatusfromiio : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* Read only from IIO viral status */
    UINT32 fatalstatusfromiio : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* Read only from IIO fatal status */
    UINT32 viralstatusfromubox : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Read only from Ubox viral status */
    UINT32 fatalstatusfromubox : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Read only from Ubox fatal status */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2GLERRCFG_UBOX_M2U_STRUCT;

/* R2PINGERRLOG0_UBOX_M2U_REG supported on:                                     */
/*      SPRA0 (0x21e05260)                                                      */
/*      SPRB0 (0x21e05260)                                                      */
/*      SPRHBM (0x21e05260)                                                     */
/*      SPRC0 (0x21e05260)                                                      */
/*      SPRMCC (0x21e05260)                                                     */
/*      SPRUCC (0x21e05260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Ingress error log 0
*/


#define R2PINGERRLOG0_UBOX_M2U_REG 0x13520260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak0bgfcrdoverflow : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK BGF 0 BGF credit overflow */
    UINT32 ak1bgfcrdoverflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK BGF 1 BGF credit overflow */
    UINT32 blbgfcrdoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL BGF credit overflow */
    UINT32 ubxcboncbcrdoverflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ubox Cbo NCB credit overflow */
    UINT32 ubxcboncscrdoverflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ubox Cbo NCS credit overflow */
    UINT32 ubxqpincbcrdoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ubox Intel UPI NCB credit overflow */
    UINT32 ubxqpincscrdoverflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ubox Intel UPI NCS credit overflow */
    UINT32 iioidicrdoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IIO IDI credit overflow */
    UINT32 iioncbcrdoverflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IIO NCB credit overflow */
    UINT32 iioncscrdoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IIO NCS credit overflow */
    UINT32 parerring0 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ingress BL data parity error - Agent 0 */
    UINT32 parerring0injdone : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Inidcating parity error injection is done. S/W
                               need clear this bit if it wants to inject
                               another error
                            */
    UINT32 ubxiioncbcreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IIO NCB credit overflow */
    UINT32 ubxiioncscreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IIO NCS credit overflow */
    UINT32 ivbgfcrdoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IV BGF credit overflow */
    UINT32 ivsnpcrdoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IV Snoop credit overflow */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PINGERRLOG0_UBOX_M2U_STRUCT;

/* R2EGRERRLOG_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e05268)                                                      */
/*      SPRB0 (0x21e05268)                                                      */
/*      SPRHBM (0x21e05268)                                                     */
/*      SPRC0 (0x21e05268)                                                      */
/*      SPRMCC (0x21e05268)                                                     */
/*      SPRUCC (0x21e05268)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Egress error log
*/


#define R2EGRERRLOG_UBOX_M2U_REG 0x13520268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 akcegress_write_to_valid_entry : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 write to valid entry */
    UINT32 akcegress_overflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 overflow */
    UINT32 akegress0_write_to_valid_entry : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 write to occupied entry */
    UINT32 akegress1_write_to_valid_entry : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 1 write to occupied entry */
    UINT32 blegress0_write_to_valid_entry : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 0 write to occupied entry */
    UINT32 blegress1_write_to_valid_entry : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 1 write to occupied entry */
    UINT32 adegress0_write_to_valid_entry : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 0 write to occupied entry */
    UINT32 adegress1_write_to_valid_entry : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 1 write to occupied entry */
    UINT32 akegress0_overflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 buffer overflow */
    UINT32 akegress1_overflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 1 buffer overflow */
    UINT32 blegress0_overflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 0 buffer overflow */
    UINT32 blegress1_overflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 1 buffer overflow */
    UINT32 adegress0_overflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 0 buffer overflow */
    UINT32 adegress1_overflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 1 buffer overflow */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2EGRERRLOG_UBOX_M2U_STRUCT;

/* R2PINGERRMSK0_UBOX_M2U_REG supported on:                                     */
/*      SPRA0 (0x21e05280)                                                      */
/*      SPRB0 (0x21e05280)                                                      */
/*      SPRHBM (0x21e05280)                                                     */
/*      SPRC0 (0x21e05280)                                                      */
/*      SPRMCC (0x21e05280)                                                     */
/*      SPRUCC (0x21e05280)                                                     */
/* Register default value on SPRA0: 0x00001800                                  */
/* Register default value on SPRB0: 0x00001800                                  */
/* Register default value on SPRHBM: 0x00001800                                 */
/* Register default value on SPRC0: 0x00001800                                  */
/* Register default value on SPRMCC: 0x00001800                                 */
/* Register default value on SPRUCC: 0x00001800                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Ingress error mask register 0
*/


#define R2PINGERRMSK0_UBOX_M2U_REG 0x13520280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak0bgfcrderrmsk : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK 0 BGF credit overflow error */
    UINT32 ak1bgfcrderrmsk : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK 1 BGF credit overflow error */
    UINT32 blbgfcrderrmsk : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL BGF credit overflow error */
    UINT32 ubxcboncbcrderrmsk : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* Mask Ubox Cbo NCB credit overflow error */
    UINT32 ubxcboncscrderrmsk : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* Mask Ubox Cbo NCS credit overflow error */
    UINT32 ubxqpincbcrderrmsk : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Mask Ubox Intel UPI NCB credit overflow error */
    UINT32 ubxqpincscrderrmsk : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* Mask Ubox Intel UPI NCS credit overflow error */
    UINT32 iioidicrderrmsk : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Mask IIO IDI credit overflow error */
    UINT32 iioncbcrderrmsk : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* Mask IIO NCB credit overflow error */
    UINT32 iioncscrderrmsk : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* Mask IIO NCS credit overflow error */
    UINT32 parerring0msk : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL ingress data parity error - Agent 0 */
    UINT32 ubxiioncbcrediterrmsk : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* IIO NCB overflow error */
    UINT32 ubxiioncscrediterrmsk : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* IIO NCS overflow error */
    UINT32 rsvd : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ivbgfcrderrmsk : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* IV BGF credit overflow error */
    UINT32 ivsnpcrderrmsk : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* IV Snoop credit overflow error */
    UINT32 ingparerrinjentry : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               6b111111: Par error injection happens to next
                               Ingress Write; otherwise, Inject error the the
                               ingress entry specified by this field
                            */
    UINT32 ingparerrinj : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen to an ingress entry write sepcified by
                               R2PINGERRMSK0.IngParErrInjEntry. Once injection
                               is done, R2PINGERRLOG0.ParErrIng0InjDone will be
                               set. S/W need clear both bits if it wants to do
                               another injection
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PINGERRMSK0_UBOX_M2U_STRUCT;

/* R2PINGERRMSK1_UBOX_M2U_REG supported on:                                     */
/*      SPRA0 (0x21e05284)                                                      */
/*      SPRB0 (0x21e05284)                                                      */
/*      SPRHBM (0x21e05284)                                                     */
/*      SPRC0 (0x21e05284)                                                      */
/*      SPRMCC (0x21e05284)                                                     */
/*      SPRUCC (0x21e05284)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0   */
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2ING/UBOX_M2ING/CFG_R2PCIE_F0  */
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Ingress error mask register 1
*/


#define R2PINGERRMSK1_UBOX_M2U_REG 0x13520284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ingparerrinj : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen to an ingress entry write sepcified by
                               R2PINGERRMSK1. IngParErrInjEntrboth bits if it
                               wants to do another injectiony. Once injection
                               is done, R2PINGERRLOG0.ParErrIng0InjDone will be
                               set. S/W need clear both bits if it wants to do
                               another injection
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PINGERRMSK1_UBOX_M2U_STRUCT;

/* R2EGRERRMSK_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e05288)                                                      */
/*      SPRB0 (0x21e05288)                                                      */
/*      SPRHBM (0x21e05288)                                                     */
/*      SPRC0 (0x21e05288)                                                      */
/*      SPRMCC (0x21e05288)                                                     */
/*      SPRUCC (0x21e05288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2EGR/UBOX_M2EGR/CFG_R2PCIE_F0_M2EGR*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Error Mask Register
*/


#define R2EGRERRMSK_UBOX_M2U_REG 0x13520288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 akcegress_overflow : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Mask AKC Egress Agent 0 overflow error */
    UINT32 akcegress_write_to_valid_entry : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AK Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 akegress0_write_to_valid_entry : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AK Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 akegress1_write_to_valid_entry : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AK Egress Agent 1 write to occupied entry
                               error
                            */
    UINT32 blegress0_write_to_valid_entry : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask BL Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 blegress1_write_to_valid_entry : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask BL Egress Agent 1 write to occupied entry
                               error
                            */
    UINT32 adegress0_write_to_valid_entry : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AD Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 adegress1_write_to_valid_entry : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AD Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 akegress0_overflow : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 0 overflow error */
    UINT32 akegress1_overflow : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 1 overflow error */
    UINT32 blegress0_overflow : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL Egress Agent 0 overflow error */
    UINT32 blegress1_overflow : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 1 overflow error */
    UINT32 adegress0_overflow : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Mask AD Egress Agent 0 overflow error */
    UINT32 adegress1_overflow : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Mask AD Egress Agent 1 overflow error */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2EGRERRMSK_UBOX_M2U_STRUCT;




/* OS_W_CP_N0_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x21e052d0)                                                      */
/*      SPRB0 (0x21e052d0)                                                      */
/*      SPRHBM (0x21e052d0)                                                     */
/*      SPRC0 (0x21e052d0)                                                      */
/*      SPRMCC (0x21e052d0)                                                     */
/*      SPRUCC (0x21e052d0)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N0_UBOX_M2U_REG 0x135202D0

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

} OS_W_CP_N0_UBOX_M2U_STRUCT;

/* OS_W_CP_N1_UBOX_M2U_REG supported on:                                        */
/*      SPRA0 (0x21e052d4)                                                      */
/*      SPRB0 (0x21e052d4)                                                      */
/*      SPRHBM (0x21e052d4)                                                     */
/*      SPRC0 (0x21e052d4)                                                      */
/*      SPRMCC (0x21e052d4)                                                     */
/*      SPRUCC (0x21e052d4)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N1_UBOX_M2U_REG 0x135202D4

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

} OS_W_CP_N1_UBOX_M2U_STRUCT;

/* OS_W_RAC_N0_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e052d8)                                                      */
/*      SPRB0 (0x21e052d8)                                                      */
/*      SPRHBM (0x21e052d8)                                                     */
/*      SPRC0 (0x21e052d8)                                                      */
/*      SPRMCC (0x21e052d8)                                                     */
/*      SPRUCC (0x21e052d8)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N0_UBOX_M2U_REG 0x135202D8

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

} OS_W_RAC_N0_UBOX_M2U_STRUCT;

/* OS_W_RAC_N1_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e052dc)                                                      */
/*      SPRB0 (0x21e052dc)                                                      */
/*      SPRHBM (0x21e052dc)                                                     */
/*      SPRC0 (0x21e052dc)                                                      */
/*      SPRMCC (0x21e052dc)                                                     */
/*      SPRUCC (0x21e052dc)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N1_UBOX_M2U_REG 0x135202DC

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

} OS_W_RAC_N1_UBOX_M2U_STRUCT;

/* OS_W_WAC_N0_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e052e8)                                                      */
/*      SPRB0 (0x21e052e8)                                                      */
/*      SPRHBM (0x21e052e8)                                                     */
/*      SPRC0 (0x21e052e8)                                                      */
/*      SPRMCC (0x21e052e8)                                                     */
/*      SPRUCC (0x21e052e8)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N0_UBOX_M2U_REG 0x135202E8

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

} OS_W_WAC_N0_UBOX_M2U_STRUCT;

/* OS_W_WAC_N1_UBOX_M2U_REG supported on:                                       */
/*      SPRA0 (0x21e052ec)                                                      */
/*      SPRB0 (0x21e052ec)                                                      */
/*      SPRHBM (0x21e052ec)                                                     */
/*      SPRC0 (0x21e052ec)                                                      */
/*      SPRMCC (0x21e052ec)                                                     */
/*      SPRUCC (0x21e052ec)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRB0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRHBM Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRC0 Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRMCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* SPRUCC Register File:    sprsp_top/ubox/UBOX_M2UTL/UBOX_M2UTL/CFG_R2PCIE_F0_M2UTL*/
/* Struct generated from SPRA0 BDF: 30_0_5                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N1_UBOX_M2U_REG 0x135202EC

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

} OS_W_WAC_N1_UBOX_M2U_STRUCT;

#endif /* _UBOX_M2U_h */
