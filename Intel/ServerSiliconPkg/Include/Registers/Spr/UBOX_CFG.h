
/** @file
  UBOX_CFG.h

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


#ifndef _UBOX_CFG_h
#define _UBOX_CFG_h
#include <Base.h>

/* VID_0_0_0_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x11e00000)                                                      */
/*      SPRB0 (0x11e00000)                                                      */
/*      SPRHBM (0x11e00000)                                                     */
/*      SPRC0 (0x11e00000)                                                      */
/*      SPRMCC (0x11e00000)                                                     */
/*      SPRUCC (0x11e00000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_0_0_0_UBOX_CFG_REG 0x13010000

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

} VID_0_0_0_UBOX_CFG_STRUCT;

/* DID_0_0_0_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x11e00002)                                                      */
/*      SPRB0 (0x11e00002)                                                      */
/*      SPRHBM (0x11e00002)                                                     */
/*      SPRC0 (0x11e00002)                                                      */
/*      SPRMCC (0x11e00002)                                                     */
/*      SPRUCC (0x11e00002)                                                     */
/* Register default value on SPRA0: 0x00003250                                  */
/* Register default value on SPRB0: 0x00003250                                  */
/* Register default value on SPRHBM: 0x00003250                                 */
/* Register default value on SPRC0: 0x00003250                                  */
/* Register default value on SPRMCC: 0x00003250                                 */
/* Register default value on SPRUCC: 0x00003250                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_0_0_0_UBOX_CFG_REG 0x13010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00003250*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_0_0_0_UBOX_CFG_STRUCT;

/* PCICMD_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x11e00004)                                                      */
/*      SPRB0 (0x11e00004)                                                      */
/*      SPRHBM (0x11e00004)                                                     */
/*      SPRC0 (0x11e00004)                                                      */
/*      SPRMCC (0x11e00004)                                                     */
/*      SPRUCC (0x11e00004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_0_0_0_UBOX_CFG_REG 0x13010004

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

} PCICMD_0_0_0_UBOX_CFG_STRUCT;

/* PCISTS_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x11e00006)                                                      */
/*      SPRB0 (0x11e00006)                                                      */
/*      SPRHBM (0x11e00006)                                                     */
/*      SPRC0 (0x11e00006)                                                      */
/*      SPRMCC (0x11e00006)                                                     */
/*      SPRUCC (0x11e00006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_0_0_0_UBOX_CFG_REG 0x13010006

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

} PCISTS_0_0_0_UBOX_CFG_STRUCT;

/* RID_CCR_0_0_0_UBOX_CFG_REG supported on:                                     */
/*      SPRA0 (0x21e00008)                                                      */
/*      SPRB0 (0x21e00008)                                                      */
/*      SPRHBM (0x21e00008)                                                     */
/*      SPRC0 (0x21e00008)                                                      */
/*      SPRMCC (0x21e00008)                                                     */
/*      SPRUCC (0x21e00008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_0_0_0_UBOX_CFG_REG 0x13020008

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

} RID_CCR_0_0_0_UBOX_CFG_STRUCT;

/* CLSR_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x1e0000c)                                                       */
/*      SPRB0 (0x1e0000c)                                                       */
/*      SPRHBM (0x1e0000c)                                                      */
/*      SPRC0 (0x1e0000c)                                                       */
/*      SPRMCC (0x1e0000c)                                                      */
/*      SPRUCC (0x1e0000c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_0_0_0_UBOX_CFG_REG 0x1300000C

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

} CLSR_0_0_0_UBOX_CFG_STRUCT;

/* PLAT_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x1e0000d)                                                       */
/*      SPRB0 (0x1e0000d)                                                       */
/*      SPRHBM (0x1e0000d)                                                      */
/*      SPRC0 (0x1e0000d)                                                       */
/*      SPRMCC (0x1e0000d)                                                      */
/*      SPRUCC (0x1e0000d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_0_0_0_UBOX_CFG_REG 0x1300000D

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

} PLAT_0_0_0_UBOX_CFG_STRUCT;

/* HDR_0_0_0_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x1e0000e)                                                       */
/*      SPRB0 (0x1e0000e)                                                       */
/*      SPRHBM (0x1e0000e)                                                      */
/*      SPRC0 (0x1e0000e)                                                       */
/*      SPRMCC (0x1e0000e)                                                      */
/*      SPRUCC (0x1e0000e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_0_0_0_UBOX_CFG_REG 0x1300000E

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

} HDR_0_0_0_UBOX_CFG_STRUCT;

/* BIST_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x1e0000f)                                                       */
/*      SPRB0 (0x1e0000f)                                                       */
/*      SPRHBM (0x1e0000f)                                                      */
/*      SPRC0 (0x1e0000f)                                                       */
/*      SPRMCC (0x1e0000f)                                                      */
/*      SPRUCC (0x1e0000f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_0_0_0_UBOX_CFG_REG 0x1300000F

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

} BIST_0_0_0_UBOX_CFG_STRUCT;

/* SVID_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x11e0002c)                                                      */
/*      SPRB0 (0x11e0002c)                                                      */
/*      SPRHBM (0x11e0002c)                                                     */
/*      SPRC0 (0x11e0002c)                                                      */
/*      SPRMCC (0x11e0002c)                                                     */
/*      SPRUCC (0x11e0002c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_0_0_0_UBOX_CFG_REG 0x1301002C

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

} SVID_0_0_0_UBOX_CFG_STRUCT;

/* SDID_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x11e0002e)                                                      */
/*      SPRB0 (0x11e0002e)                                                      */
/*      SPRHBM (0x11e0002e)                                                     */
/*      SPRC0 (0x11e0002e)                                                      */
/*      SPRMCC (0x11e0002e)                                                     */
/*      SPRUCC (0x11e0002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_0_0_0_UBOX_CFG_REG 0x1301002E

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

} SDID_0_0_0_UBOX_CFG_STRUCT;

/* CAPPTR_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x1e00034)                                                       */
/*      SPRB0 (0x1e00034)                                                       */
/*      SPRHBM (0x1e00034)                                                      */
/*      SPRC0 (0x1e00034)                                                       */
/*      SPRMCC (0x1e00034)                                                      */
/*      SPRUCC (0x1e00034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_0_0_0_UBOX_CFG_REG 0x13000034

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

} CAPPTR_0_0_0_UBOX_CFG_STRUCT;

/* INTL_0_0_0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x1e0003c)                                                       */
/*      SPRB0 (0x1e0003c)                                                       */
/*      SPRHBM (0x1e0003c)                                                      */
/*      SPRC0 (0x1e0003c)                                                       */
/*      SPRMCC (0x1e0003c)                                                      */
/*      SPRUCC (0x1e0003c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_0_0_0_UBOX_CFG_REG 0x1300003C

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

} INTL_0_0_0_UBOX_CFG_STRUCT;

/* INTPIN_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x1e0003d)                                                       */
/*      SPRB0 (0x1e0003d)                                                       */
/*      SPRHBM (0x1e0003d)                                                      */
/*      SPRC0 (0x1e0003d)                                                       */
/*      SPRMCC (0x1e0003d)                                                      */
/*      SPRUCC (0x1e0003d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_0_0_0_UBOX_CFG_REG 0x1300003D

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

} INTPIN_0_0_0_UBOX_CFG_STRUCT;

/* MINGNT_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x1e0003e)                                                       */
/*      SPRB0 (0x1e0003e)                                                       */
/*      SPRHBM (0x1e0003e)                                                      */
/*      SPRC0 (0x1e0003e)                                                       */
/*      SPRMCC (0x1e0003e)                                                      */
/*      SPRUCC (0x1e0003e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_0_0_0_UBOX_CFG_REG 0x1300003E

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

} MINGNT_0_0_0_UBOX_CFG_STRUCT;

/* MAXLAT_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x1e0003f)                                                       */
/*      SPRB0 (0x1e0003f)                                                       */
/*      SPRHBM (0x1e0003f)                                                      */
/*      SPRC0 (0x1e0003f)                                                       */
/*      SPRMCC (0x1e0003f)                                                      */
/*      SPRUCC (0x1e0003f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_0_0_0_UBOX_CFG_REG 0x1300003F

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

} MAXLAT_0_0_0_UBOX_CFG_STRUCT;

/* PXPCAP_0_0_0_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x21e00040)                                                      */
/*      SPRB0 (0x21e00040)                                                      */
/*      SPRHBM (0x21e00040)                                                     */
/*      SPRC0 (0x21e00040)                                                      */
/*      SPRMCC (0x21e00040)                                                     */
/*      SPRUCC (0x21e00040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_0_0_0_UBOX_CFG_REG 0x13020040

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

} PXPCAP_0_0_0_UBOX_CFG_STRUCT;

/* PCIE_RSVD0_0_0_0_UBOX_CFG_REG supported on:                                  */
/*      SPRA0 (0x21e00044)                                                      */
/*      SPRB0 (0x21e00044)                                                      */
/*      SPRHBM (0x21e00044)                                                     */
/*      SPRC0 (0x21e00044)                                                      */
/*      SPRMCC (0x21e00044)                                                     */
/*      SPRUCC (0x21e00044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_0_0_0_UBOX_CFG_REG 0x13020044

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

} PCIE_RSVD0_0_0_0_UBOX_CFG_STRUCT;

/* PCIE_RSVD1_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00048)                                                      */
/*      SPRB0 (0x21e00048)                                                      */
/*      SPRHBM (0x21e00048)                                                     */
/*      SPRC0 (0x21e00048)                                                      */
/*      SPRMCC (0x21e00048)                                                     */
/*      SPRUCC (0x21e00048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_0_N0_UBOX_CFG_REG 0x13020048

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

} PCIE_RSVD1_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD1_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e0004c)                                                      */
/*      SPRB0 (0x21e0004c)                                                      */
/*      SPRHBM (0x21e0004c)                                                     */
/*      SPRC0 (0x21e0004c)                                                      */
/*      SPRMCC (0x21e0004c)                                                     */
/*      SPRUCC (0x21e0004c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_0_0_N1_UBOX_CFG_REG 0x1302004C

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

} PCIE_RSVD1_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD2_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00050)                                                      */
/*      SPRB0 (0x21e00050)                                                      */
/*      SPRHBM (0x21e00050)                                                     */
/*      SPRC0 (0x21e00050)                                                      */
/*      SPRMCC (0x21e00050)                                                     */
/*      SPRUCC (0x21e00050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_0_N0_UBOX_CFG_REG 0x13020050

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

} PCIE_RSVD2_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD2_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00054)                                                      */
/*      SPRB0 (0x21e00054)                                                      */
/*      SPRHBM (0x21e00054)                                                     */
/*      SPRC0 (0x21e00054)                                                      */
/*      SPRMCC (0x21e00054)                                                     */
/*      SPRUCC (0x21e00054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_0_0_N1_UBOX_CFG_REG 0x13020054

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

} PCIE_RSVD2_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD3_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00058)                                                      */
/*      SPRB0 (0x21e00058)                                                      */
/*      SPRHBM (0x21e00058)                                                     */
/*      SPRC0 (0x21e00058)                                                      */
/*      SPRMCC (0x21e00058)                                                     */
/*      SPRUCC (0x21e00058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_0_N0_UBOX_CFG_REG 0x13020058

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

} PCIE_RSVD3_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD3_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e0005c)                                                      */
/*      SPRB0 (0x21e0005c)                                                      */
/*      SPRHBM (0x21e0005c)                                                     */
/*      SPRC0 (0x21e0005c)                                                      */
/*      SPRMCC (0x21e0005c)                                                     */
/*      SPRUCC (0x21e0005c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_0_0_N1_UBOX_CFG_REG 0x1302005C

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

} PCIE_RSVD3_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD4_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00060)                                                      */
/*      SPRB0 (0x21e00060)                                                      */
/*      SPRHBM (0x21e00060)                                                     */
/*      SPRC0 (0x21e00060)                                                      */
/*      SPRMCC (0x21e00060)                                                     */
/*      SPRUCC (0x21e00060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_0_N0_UBOX_CFG_REG 0x13020060

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

} PCIE_RSVD4_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD4_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00064)                                                      */
/*      SPRB0 (0x21e00064)                                                      */
/*      SPRHBM (0x21e00064)                                                     */
/*      SPRC0 (0x21e00064)                                                      */
/*      SPRMCC (0x21e00064)                                                     */
/*      SPRUCC (0x21e00064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_0_0_N1_UBOX_CFG_REG 0x13020064

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

} PCIE_RSVD4_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD5_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00068)                                                      */
/*      SPRB0 (0x21e00068)                                                      */
/*      SPRHBM (0x21e00068)                                                     */
/*      SPRC0 (0x21e00068)                                                      */
/*      SPRMCC (0x21e00068)                                                     */
/*      SPRUCC (0x21e00068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_0_N0_UBOX_CFG_REG 0x13020068

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

} PCIE_RSVD5_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD5_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e0006c)                                                      */
/*      SPRB0 (0x21e0006c)                                                      */
/*      SPRHBM (0x21e0006c)                                                     */
/*      SPRC0 (0x21e0006c)                                                      */
/*      SPRMCC (0x21e0006c)                                                     */
/*      SPRUCC (0x21e0006c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_0_0_N1_UBOX_CFG_REG 0x1302006C

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

} PCIE_RSVD5_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD6_0_0_0_N0_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00070)                                                      */
/*      SPRB0 (0x21e00070)                                                      */
/*      SPRHBM (0x21e00070)                                                     */
/*      SPRC0 (0x21e00070)                                                      */
/*      SPRMCC (0x21e00070)                                                     */
/*      SPRUCC (0x21e00070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_0_N0_UBOX_CFG_REG 0x13020070

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

} PCIE_RSVD6_0_0_0_N0_UBOX_CFG_STRUCT;

/* PCIE_RSVD6_0_0_0_N1_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e00074)                                                      */
/*      SPRB0 (0x21e00074)                                                      */
/*      SPRHBM (0x21e00074)                                                     */
/*      SPRC0 (0x21e00074)                                                      */
/*      SPRMCC (0x21e00074)                                                     */
/*      SPRUCC (0x21e00074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_0_0_N1_UBOX_CFG_REG 0x13020074

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

} PCIE_RSVD6_0_0_0_N1_UBOX_CFG_STRUCT;

/* PCIE_RSVD7_0_0_0_UBOX_CFG_REG supported on:                                  */
/*      SPRA0 (0x21e00078)                                                      */
/*      SPRB0 (0x21e00078)                                                      */
/*      SPRHBM (0x21e00078)                                                     */
/*      SPRC0 (0x21e00078)                                                      */
/*      SPRMCC (0x21e00078)                                                     */
/*      SPRUCC (0x21e00078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_0_0_0_UBOX_CFG_REG 0x13020078

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

} PCIE_RSVD7_0_0_0_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00080)                                                      */
/*      SPRB0 (0x21e00080)                                                      */
/*      SPRHBM (0x21e00080)                                                     */
/*      SPRC0 (0x21e00080)                                                      */
/*      SPRMCC (0x21e00080)                                                     */
/*      SPRUCC (0x21e00080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_REG 0x13020080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00084)                                                      */
/*      SPRB0 (0x21e00084)                                                      */
/*      SPRHBM (0x21e00084)                                                     */
/*      SPRC0 (0x21e00084)                                                      */
/*      SPRMCC (0x21e00084)                                                     */
/*      SPRUCC (0x21e00084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG 0x13020084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00088)                                                      */
/*      SPRB0 (0x21e00088)                                                      */
/*      SPRHBM (0x21e00088)                                                     */
/*      SPRC0 (0x21e00088)                                                      */
/*      SPRMCC (0x21e00088)                                                     */
/*      SPRUCC (0x21e00088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_REG 0x13020088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e0008c)                                                      */
/*      SPRB0 (0x21e0008c)                                                      */
/*      SPRHBM (0x21e0008c)                                                     */
/*      SPRC0 (0x21e0008c)                                                      */
/*      SPRMCC (0x21e0008c)                                                     */
/*      SPRUCC (0x21e0008c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_REG 0x1302008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00090)                                                      */
/*      SPRB0 (0x21e00090)                                                      */
/*      SPRHBM (0x21e00090)                                                     */
/*      SPRC0 (0x21e00090)                                                      */
/*      SPRMCC (0x21e00090)                                                     */
/*      SPRUCC (0x21e00090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG 0x13020090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_STRUCT;

/* INTCONTROL_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x21e00094)                                                      */
/*      SPRB0 (0x21e00094)                                                      */
/*      SPRHBM (0x21e00094)                                                     */
/*      SPRC0 (0x21e00094)                                                      */
/*      SPRMCC (0x21e00094)                                                     */
/*      SPRUCC (0x21e00094)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Interrupt Configuration Register
*/


#define INTCONTROL_UBOX_CFG_REG 0x13020094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xapicen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Set this bit if you would like extended XAPIC
                               configuration to be used. This bit can be
                               written directly, and can also be updated using
                               XTPR messages
                            */
    UINT32 forcex2apic : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Write: 1: Forces the system to move into X2APIC
                               Mode. 0: No affect
                            */
    UINT32 rsvd : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clastchksmpmod : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               0: Disable checking for Logical_APICID[31:0]
                               being non-zero when sampling flat/ cluster mode
                               bit in the IntPrioUpd message as part of setting
                               bit 1 in this register 1: Enable the above
                               checking
                            */
    UINT32 logfltclustmod : 1;

                            /* Bits[17:17], Access Type=RW/V, default=0x00000000*/

                            /*
                               Set by bios to indicate if the OS is running
                               logical flat or logical cluster mode. This bit
                               can also be updated by IntPrioUpd messages. This
                               bit reflects the setup of the filter at any
                               given time. 0 - flat, 1 - cluster.
                            */
    UINT32 logflatclustovren : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               0 : IA32 Logical Flat or Cluster Mode bit is
                               locked as Read only bit. 1 : IA32 Logical Flat
                               or Cluster Mode bit may be written by SW, values
                               written by xTPR update are ignored. For one time
                               override of the IA32 Logical Flat or Cluster
                               Mode value, return this bit to its default state
                               after the bit is changed. Leaving this bit as 1
                               will prevent automatic update of the filter.
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} INTCONTROL_UBOX_CFG_STRUCT;

/* UBOXERRCTL_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x21e00098)                                                      */
/*      SPRB0 (0x21e00098)                                                      */
/*      SPRHBM (0x21e00098)                                                     */
/*      SPRC0 (0x21e00098)                                                      */
/*      SPRMCC (0x21e00098)                                                     */
/*      SPRUCC (0x21e00098)                                                     */
/* Register default value on SPRA0: 0x81FF7FC0                                  */
/* Register default value on SPRB0: 0x81FF7FC0                                  */
/* Register default value on SPRHBM: 0x81FF7FC0                                 */
/* Register default value on SPRC0: 0x81FF7FC0                                  */
/* Register default value on SPRMCC: 0x81FF7FC0                                 */
/* Register default value on SPRUCC: 0x81FF7FC0                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Ubox per-error enable mask. If masked, a given Ubox error will not be signaled via SMI and will not be logged. 
*/


#define UBOXERRCTL_UBOX_CFG_REG 0x13020098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poisonmask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Mask SMI generation on receiving poison in Ubox. */
    UINT32 unsupportedmask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask SMI generation on receiving unsupported
                               opcodes.
                            */
    UINT32 cfgrdaddrmisalignedmask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask SMI generation on receiving MMCFG read
                               address misalignment.
                            */
    UINT32 cfgwraddrmisalignedmask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask SMI generation on receiving MMCFG write
                               address misalignment.
                            */
    UINT32 smitimeoutmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask SMI generation on receiving SMI Timeout.
                               (Mask on default)
                            */
    UINT32 smitimeoutbasesizeselect : 2;

                            /* Bits[8:7], Access Type=RW/P, default=0x00000003*/

                            /*
                               Selecting the size of the bit base counter 00->6
                               bits, 01->15 bits, 10->20 bits, 11->24 bits
                            */
    UINT32 smitimeoutcountmatch : 6;

                            /* Bits[14:9], Access Type=RW/P, default=0x0000003F*/

                            /* The value to match the time out counter [9:4] */
    UINT32 masterlocktimeoutmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask SMI generation on receiving Master Lock
                               Timeout.
                            */
    UINT32 masterlocktimeoutbasesizeselect : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000003*/

                            /*
                               Selecting the size of the bit base counter 00->6
                               bits, 01->15 bits, 10->20 bits, 11->24 bits
                            */
    UINT32 masterlocktimeoutcountmatch : 6;

                            /* Bits[23:18], Access Type=RW/P, default=0x0000003F*/

                            /* The value to match the time out counter [9:4] */
    UINT32 uboxerrortopcumask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to mask direct MCA from Ubox local error
                               to PCU
                            */
    UINT32 rsvd_25 : 6;

                            /* Bits[30:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nmibiosenable : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set to 1 to enable signalling of pin-based NMI
                               to BIOS
                            */

  } Bits;
  UINT32 Data;

} UBOXERRCTL_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_UBOXERRSTS2_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e000a0)                                                      */
/*      SPRB0 (0x21e000a0)                                                      */
/*      SPRHBM (0x21e000a0)                                                     */
/*      SPRC0 (0x21e000a0)                                                      */
/*      SPRMCC (0x21e000a0)                                                     */
/*      SPRUCC (0x21e000a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is error status register in the Ubox and covers most of the interrupt related errors
*/


#define NCEVENTS_CR_UBOXERRSTS2_UBOX_CFG_REG 0x130200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cfgrdaddrmisalignedsmm : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               MMCFG Read Address Misaligment received by Ubox
                               - SMM
                            */
    UINT32 cfgwraddrmisalignedsmm : 1;

                            /* Bits[1:1], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               MMCFG Write Address Misaligment received by Ubox
                               - SMM
                            */
    UINT32 misalignedmmiordsmm : 1;

                            /* Bits[2:2], Access Type=RW/V/P, default=0x00000000*/

                            /* SMM MMIO Rd misalignment error detected by Ubox */
    UINT32 misalignedmmiowrsmm : 1;

                            /* Bits[3:3], Access Type=RW/V/P, default=0x00000000*/

                            /* SMM MMIO Wr misalignment error detected by Ubox */
    UINT32 misalignedmmiordnonsmm : 1;

                            /* Bits[4:4], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Non MMIO Cfg Rd misalignment error detected by
                               Ubox
                            */
    UINT32 misalignedmmiowrnonsmm : 1;

                            /* Bits[5:5], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Non MMIO Cfg Wr misalignment error detected by
                               Ubox
                            */
    UINT32 gpsbparityerror : 1;

                            /* Bits[6:6], Access Type=RW/V/P, default=0x00000000*/

                            /* GPSB parity error detected by Ubox */
    UINT32 saierror : 1;

                            /* Bits[7:7], Access Type=RW/V/P, default=0x00000000*/

                            /* SAI error detected by Ubox */
    UINT32 semaphoreerror : 1;

                            /* Bits[8:8], Access Type=RW/V/P, default=0x00000000*/

                            /* Semaphore error detected by Ubox */
    UINT32 akegresswritetovalidentry : 1;

                            /* Bits[9:9], Access Type=RW/V/P, default=0x00000000*/

                            /* AK Egress Agent write to occupied entry error */
    UINT32 blegresswritetovalidentry : 1;

                            /* Bits[10:10], Access Type=RW/V/P, default=0x00000000*/

                            /* BL Egress Agent write to occupied entry error */
    UINT32 akcegresswritetovalidentry : 1;

                            /* Bits[11:11], Access Type=RW/V/P, default=0x00000000*/

                            /* AKC Egress Agent write to occupied entry error */
    UINT32 akegressoverflow : 1;

                            /* Bits[12:12], Access Type=RW/V/P, default=0x00000000*/

                            /* AK Egress overflow error */
    UINT32 blegressoverflow : 1;

                            /* Bits[13:13], Access Type=RW/V/P, default=0x00000000*/

                            /* BL Egress overflow error */
    UINT32 akcegressoverflow : 1;

                            /* Bits[14:14], Access Type=RW/V/P, default=0x00000000*/

                            /* AKC Egress overflow error */
    UINT32 chatouboxoverflowncb : 1;

                            /* Bits[15:15], Access Type=RW/V/P, default=0x00000000*/

                            /* CHA to Ubox Overflow on NCB error */
    UINT32 blingressoverflow : 1;

                            /* Bits[16:16], Access Type=RW/V/P, default=0x00000000*/

                            /* CHA to Ubox Overflow on NCS error */
    UINT32 upitouboxoverflowncb : 1;

                            /* Bits[17:17], Access Type=RW/V/P, default=0x00000000*/

                            /* UPI to Ubox Overflow on NCB error */
    UINT32 upitouboxoverflowncs : 1;

                            /* Bits[18:18], Access Type=RW/V/P, default=0x00000000*/

                            /* UPI to Ubox Overflow on NCS error */
    UINT32 ingressparityerror : 1;

                            /* Bits[19:19], Access Type=RW/V/P, default=0x00000000*/

                            /* M2Ubox Ingress Parity error */
    UINT32 ms2iosftouboxoverflowncb : 1;

                            /* Bits[20:20], Access Type=RW/V/P, default=0x00000000*/

                            /* MS2IOSF to Ubox Overflow on NCB error */
    UINT32 ms2iosftouboxoverflowncs : 1;

                            /* Bits[21:21], Access Type=RW/V/P, default=0x00000000*/

                            /* MS2IOSF to Ubox Overflow on NCS error */
    UINT32 sgx_doorbell_error : 1;

                            /* Bits[22:22], Access Type=RW/V/P, default=0x00000000*/

                            /* LTDoorbell Error to UboxFatalErrors in NCU */
    UINT32 reserved : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved bits */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_UBOXERRSTS2_UBOX_CFG_STRUCT;

/* IERRLOGGINGREG_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e000a4)                                                      */
/*      SPRB0 (0x21e000a4)                                                      */
/*      SPRHBM (0x21e000a4)                                                     */
/*      SPRC0 (0x21e000a4)                                                      */
/*      SPRMCC (0x21e000a4)                                                     */
/*      SPRUCC (0x21e000a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/*  IERR first/second logging error.
*/


#define IERRLOGGINGREG_UBOX_CFG_REG 0x130200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 firstierrsrcid : 10;

                            /* Bits[9:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the first
                               IERR
                            */
    UINT32 firstierrsrcvalid : 1;

                            /* Bits[10:10], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the FirstIerrSrcID is valid */
    UINT32 firstierrsrcfromcore : 1;

                            /* Bits[11:11], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the FirstIerrSrcID is from a C2U */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 secondierrsrcid : 10;

                            /* Bits[25:16], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the second
                               IERR
                            */
    UINT32 secondierrsrcvalid : 1;

                            /* Bits[26:26], Access Type=RW/V/P, default=0x00000000*/

                            /* set to 1 if the SecondIerrSrcID is valid */
    UINT32 secondierrsrcfromcore : 1;

                            /* Bits[27:27], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the SecondIerrSrcID is from a C2U */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IERRLOGGINGREG_UBOX_CFG_STRUCT;

/* MCERRLOGGINGREG_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e000a8)                                                      */
/*      SPRB0 (0x21e000a8)                                                      */
/*      SPRHBM (0x21e000a8)                                                     */
/*      SPRC0 (0x21e000a8)                                                      */
/*      SPRMCC (0x21e000a8)                                                     */
/*      SPRUCC (0x21e000a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/*  MCERR first/second logging error.
*/


#define MCERRLOGGINGREG_UBOX_CFG_REG 0x130200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 firstmcerrsrcid : 10;

                            /* Bits[9:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the first
                               MCERR
                            */
    UINT32 firstmcerrsrcvalid : 1;

                            /* Bits[10:10], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to 1 if the FirstSrcID is valid. */
    UINT32 firstmcerrsrcfromcore : 1;

                            /* Bits[11:11], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the FirstMCErrSrcID is from a C2U */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 secondmcerrsrcid : 10;

                            /* Bits[25:16], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the second
                               MCeRR
                            */
    UINT32 secondmcerrsrcvalid : 1;

                            /* Bits[26:26], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to 1 if the SecondMCerrSrcID is valid. */
    UINT32 secondmcerrsrcfromcore : 1;

                            /* Bits[27:27], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the SecondMCerrSrcID is from a C2U */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCERRLOGGINGREG_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_INTCONTROL1_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e000ac)                                                      */
/*      SPRB0 (0x21e000ac)                                                      */
/*      SPRHBM (0x21e000ac)                                                     */
/*      SPRC0 (0x21e000ac)                                                      */
/*      SPRMCC (0x21e000ac)                                                     */
/*      SPRUCC (0x21e000ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Configuration Register
*/


#define NCEVENTS_CR_INTCONTROL1_UBOX_CFG_REG 0x130200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdrmodsel : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Selects the redirection mode used for MSI
                               interrupts with lowest-priority delivery mode.
                               The folowing schemes are used : 000 : Fixed
                               Priority - select the first enabled APIC in the
                               cluster. 001: Redirect last - last vector
                               selected (applicable only in extended mode) 010
                               : Hash Vector - select the first enabled APIC in
                               round robin manner starting form the hash of the
                               vector number. default: Fixed Priority
                            */
    UINT32 hashmodctr : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates the hash mode control for the
                               interrupt control. Select the hush function for
                               the Vector based Hash Mode interrupt redirection
                               control : 000 select bits 7:4/5:4 for vector
                               cluster/flat algorithm 001 select bits 6:3/4:3
                               010 select bits 4:1/2:1 011 select bits 3:0/1:0
                               other - reserved
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_INTCONTROL1_UBOX_CFG_STRUCT;

/* EMCA_CORE_CSMI_LOG_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e000b0)                                                      */
/*      SPRB0 (0x21e000b0)                                                      */
/*      SPRHBM (0x21e000b0)                                                     */
/*      SPRC0 (0x21e000b0)                                                      */
/*      SPRMCC (0x21e000b0)                                                     */
/*      SPRUCC (0x21e000b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled a CSMI to ubox via the MCLK message. Enabled on the B0 and later stepping of the processor.
*/


#define EMCA_CORE_CSMI_LOG_UBOX_CFG_REG 0x130200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 0 */
    UINT32 core1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 1 */
    UINT32 core2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 2 */
    UINT32 core3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 3 */
    UINT32 core4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 4 */
    UINT32 core5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 5 */
    UINT32 core6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 6 */
    UINT32 core7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 7 */
    UINT32 core8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 8 */
    UINT32 core9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 9 */
    UINT32 core10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 10 */
    UINT32 core11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 11 */
    UINT32 core12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 12 */
    UINT32 core13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 13 */
    UINT32 core14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 14 */
    UINT32 core15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 15 */
    UINT32 core16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 16 */
    UINT32 core17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 17 */
    UINT32 core18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 18 */
    UINT32 core19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 19 */
    UINT32 core20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 20 */
    UINT32 core21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 21 */
    UINT32 core22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 22 */
    UINT32 core23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 23 */
    UINT32 core24 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 24 */
    UINT32 core25 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 25 */
    UINT32 core26 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 26 */
    UINT32 core27 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 27 */
    UINT32 core28 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 28 */
    UINT32 core29 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 29 */
    UINT32 core30 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 30 */
    UINT32 core31 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 31 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_CSMI_LOG_UBOX_CFG_STRUCT;

/* EMCA_CORE_CSMI_LOG_CFG1_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e000b4)                                                      */
/*      SPRB0 (0x21e000b4)                                                      */
/*      SPRHBM (0x21e000b4)                                                     */
/*      SPRC0 (0x21e000b4)                                                      */
/*      SPRMCC (0x21e000b4)                                                     */
/*      SPRUCC (0x21e000b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled a CSMI to ubox via the MCLK message. Enabled on the B0 and later stepping of the processor.
*/


#define EMCA_CORE_CSMI_LOG_CFG1_UBOX_CFG_REG 0x130200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core32 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 32 */
    UINT32 core33 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 33 */
    UINT32 core34 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 34 */
    UINT32 core35 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 35 */
    UINT32 core36 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 36 */
    UINT32 core37 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 37 */
    UINT32 core38 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 38 */
    UINT32 core39 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 39 */
    UINT32 core40 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 40 */
    UINT32 core41 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 41 */
    UINT32 core42 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 42 */
    UINT32 core43 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 43 */
    UINT32 core44 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core45 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core46 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core47 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core48 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core49 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core50 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core51 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core52 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core53 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core54 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core55 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core56 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core57 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core58 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core59 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core60 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core61 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core62 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */
    UINT32 core63 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* CSMI received indicator for core 44 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_CSMI_LOG_CFG1_UBOX_CFG_STRUCT;

/* EMCA_CORE_MSMI_LOG_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e000b8)                                                      */
/*      SPRB0 (0x21e000b8)                                                      */
/*      SPRHBM (0x21e000b8)                                                     */
/*      SPRC0 (0x21e000b8)                                                      */
/*      SPRMCC (0x21e000b8)                                                     */
/*      SPRUCC (0x21e000b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled an MSMI to ubox via the MCLK message. Enabled on the B0 and later stepping of the processor.
*/


#define EMCA_CORE_MSMI_LOG_UBOX_CFG_REG 0x130200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 0 */
    UINT32 core1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 1 */
    UINT32 core2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 2 */
    UINT32 core3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 3 */
    UINT32 core4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 4 */
    UINT32 core5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 5 */
    UINT32 core6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 6 */
    UINT32 core7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 7 */
    UINT32 core8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 8 */
    UINT32 core9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 9 */
    UINT32 core10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 10 */
    UINT32 core11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 11 */
    UINT32 core12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 12 */
    UINT32 core13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 13 */
    UINT32 core14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 14 */
    UINT32 core15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 15 */
    UINT32 core16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 16 */
    UINT32 core17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 17 */
    UINT32 core18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 18 */
    UINT32 core19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 19 */
    UINT32 core20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 20 */
    UINT32 core21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 21 */
    UINT32 core22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 22 */
    UINT32 core23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 23 */
    UINT32 core24 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 24 */
    UINT32 core25 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 25 */
    UINT32 core26 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 26 */
    UINT32 core27 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 27 */
    UINT32 core28 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 28 */
    UINT32 core29 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 29 */
    UINT32 core30 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 30 */
    UINT32 core31 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 31 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_MSMI_LOG_UBOX_CFG_STRUCT;

/* EMCA_CORE_MSMI_LOG_CFG1_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e000bc)                                                      */
/*      SPRB0 (0x21e000bc)                                                      */
/*      SPRHBM (0x21e000bc)                                                     */
/*      SPRC0 (0x21e000bc)                                                      */
/*      SPRMCC (0x21e000bc)                                                     */
/*      SPRUCC (0x21e000bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled an MSMI to ubox via the MCLK message. Enabled on the B0 and later stepping of the processor.
*/


#define EMCA_CORE_MSMI_LOG_CFG1_UBOX_CFG_REG 0x130200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core32 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 32 */
    UINT32 core33 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 33 */
    UINT32 core34 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 34 */
    UINT32 core35 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 35 */
    UINT32 core36 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 36 */
    UINT32 core37 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 37 */
    UINT32 core38 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 38 */
    UINT32 core39 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 39 */
    UINT32 core40 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 40 */
    UINT32 core41 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 41 */
    UINT32 core42 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 42 */
    UINT32 core43 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 43 */
    UINT32 core44 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core45 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core46 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core47 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core48 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core49 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core50 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core51 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core52 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core53 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core54 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core55 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core56 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core57 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core58 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core59 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core60 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core61 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core62 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core63 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_MSMI_LOG_CFG1_UBOX_CFG_STRUCT;

/* CPUNODEID_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e000c0)                                                      */
/*      SPRB0 (0x21e000c0)                                                      */
/*      SPRHBM (0x21e000c0)                                                     */
/*      SPRC0 (0x21e000c0)                                                      */
/*      SPRMCC (0x21e000c0)                                                     */
/*      SPRUCC (0x21e000c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Node ID Configuration Register
*/


#define CPUNODEID_UBOX_CFG_REG 0x130200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lclnodeid : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Node Id of the local Socket */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 locknodeid : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /* NodeId of the lock master */
    UINT32 rsvd_8 : 2;

                            /* Bits[9:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lgcnodeid : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /* NodeID of the legacy socket */
    UINT32 nodectrlid : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000000*/

                            /*
                               Node ID of the Node Controller. Set by the BIOS.
                               (EX processor only)
                            */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CPUNODEID_UBOX_CFG_STRUCT;

/* CPUENABLE_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e000c4)                                                      */
/*      SPRB0 (0x21e000c4)                                                      */
/*      SPRHBM (0x21e000c4)                                                     */
/*      SPRC0 (0x21e000c4)                                                      */
/*      SPRMCC (0x21e000c4)                                                     */
/*      SPRUCC (0x21e000c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Indicates which CPU is enabled
*/


#define CPUENABLE_UBOX_CFG_REG 0x130200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nodeiden : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Bit mask to indicate which node_id is enabled. */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Valid bit to indicate that the register has been
                               initialized.
                            */

  } Bits;
  UINT32 Data;

} CPUENABLE_UBOX_CFG_STRUCT;

/* BCASTLIST_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e000cc)                                                      */
/*      SPRB0 (0x21e000cc)                                                      */
/*      SPRHBM (0x21e000cc)                                                     */
/*      SPRC0 (0x21e000cc)                                                      */
/*      SPRMCC (0x21e000cc)                                                     */
/*      SPRUCC (0x21e000cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Broadcast lists
*/


#define BCASTLIST_UBOX_CFG_REG 0x130200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intlist : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Broadcast list for interrupts and PMReq in an
                               hot-add capable system
                            */
    UINT32 stopstartlist : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Broadcast list for StopReq/StartReq in a hot-add
                               capable system
                            */
    UINT32 misclist : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Broadcast list for WbInvdAck/InvdAck/EOI in a
                               hot-add capable system
                            */
    UINT32 intpriolist : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Broadcast list for IntPrioUpd in a hot-add
                               capable system
                            */

  } Bits;
  UINT32 Data;

} BCASTLIST_UBOX_CFG_STRUCT;

/* LOCKCONTROL_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e000d0)                                                      */
/*      SPRB0 (0x21e000d0)                                                      */
/*      SPRHBM (0x21e000d0)                                                     */
/*      SPRC0 (0x21e000d0)                                                      */
/*      SPRMCC (0x21e000d0)                                                     */
/*      SPRUCC (0x21e000d0)                                                     */
/* Register default value on SPRA0: 0x00000043                                  */
/* Register default value on SPRB0: 0x00000043                                  */
/* Register default value on SPRHBM: 0x00000043                                 */
/* Register default value on SPRC0: 0x00000043                                  */
/* Register default value on SPRMCC: 0x00000043                                 */
/* Register default value on SPRUCC: 0x00000043                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Lock Control registers
*/


#define LOCKCONTROL_UBOX_CFG_REG 0x130200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lockdisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Whether Locks are enabled in the system or not */
    UINT32 lockdelay : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000001*/

                            /*
                               Delay between SysLock This may be used to
                               prevent starvation on frequent Lock usage. 000 -
                               0x0 001 - 0x200 (1.2us) 010 - 0x1000 (10us) 011
                               - 0x2000 (20us) 100 - 0x4000 (40us) 101 - 0x8000
                               (80 us) 110 - 0x10000 (160 us) 111 - 0x20000
                               (320 us)
                            */
    UINT32 compatibilitymode : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Enable Compatibility Mode - disabled since no
                               longer supported
                            */
    UINT32 splitlockopt : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               This bit enables the optimized split lock flow
                               which removes Stopreq3/Stopreq4,
                               Startreq2/Startreq3 from the lock sequence
                            */
    UINT32 skippculk : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Setting this bit removes the Ubox PCU handshake
                               during stopreq and startreq only for splitlocks
                            */
    UINT32 splitstopreqiagt : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit will sequence Stopreq between
                               Ia and GT. Needed for Servers which have GT
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCKCONTROL_UBOX_CFG_STRUCT;

/* GIDNIDMAP_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e000d4)                                                      */
/*      SPRB0 (0x21e000d4)                                                      */
/*      SPRHBM (0x21e000d4)                                                     */
/*      SPRC0 (0x21e000d4)                                                      */
/*      SPRMCC (0x21e000d4)                                                     */
/*      SPRUCC (0x21e000d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Mapping between group id and nodeid
*/


#define GIDNIDMAP_UBOX_CFG_REG 0x130200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nodeid0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Node Id for group 0 */
    UINT32 nodeid1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Node Id for group Id 1 */
    UINT32 nodeid2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Node Id for group Id 2 */
    UINT32 nodeid3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Node Id for group 3 */
    UINT32 nodeid4 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* Node Id for group id 4 */
    UINT32 nodeid5 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /* Node Id for group 5 */
    UINT32 nodeid6 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /* Node Id for group 6 */
    UINT32 nodeid7 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /* NodeId for group id 7 */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GIDNIDMAP_UBOX_CFG_STRUCT;

/* EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG supported on:                         */
/*      SPRA0 (0x21e000d8)                                                      */
/*      SPRB0 (0x21e000d8)                                                      */
/*      SPRHBM (0x21e000d8)                                                     */
/*      SPRC0 (0x21e000d8)                                                      */
/*      SPRMCC (0x21e000d8)                                                     */
/*      SPRUCC (0x21e000d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* When this bit set the ubox will convert IERRs generated by the core to an MSMIs to allow firmware first model. In addition the Ubox will signal the PCU to assert the MSMI socket pin
*/


#define EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG 0x130200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 encoreierrtomsmi : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               setting this bit enables the Ubox to convert an
                               IERR from the core to an MSMI. Additionally the
                               Ubox will send a NCU_PCU_MSG to the PCU with the
                               IERR and MSMI bit asserted.
                            */
    UINT32 enrmcatormsmi : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               setting this bit enables Ubox to convert a rMCA
                               to rMSMI. This bit has no effect if the rMCA
                               feature is disabled.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e000dc)                                                      */
/*      SPRB0 (0x21e000dc)                                                      */
/*      SPRHBM (0x21e000dc)                                                     */
/*      SPRC0 (0x21e000dc)                                                      */
/*      SPRMCC (0x21e000dc)                                                     */
/*      SPRUCC (0x21e000dc)                                                     */
/* Register default value on SPRA0: 0x003FFFFC                                  */
/* Register default value on SPRB0: 0x003FFFFC                                  */
/* Register default value on SPRHBM: 0x003FFFFC                                 */
/* Register default value on SPRC0: 0x003FFFFC                                  */
/* Register default value on SPRMCC: 0x003FFFFC                                 */
/* Register default value on SPRUCC: 0x003FFFFC                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Ubox per-error enable mask. If masked, a given Ubox error will 
    	    not be signaled via SMI and will not be logged.
*/


#define NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG 0x130200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 misalignedcfgrdsmmmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask error on SMM MMCFG Read in Ubox - MSCOD
                               0x8002
                            */
    UINT32 misalignedcfgwrsmmmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask error on SMM MMCFG Read in Ubox - MSCOD
                               0x8003
                            */
    UINT32 misalignedmmiordsmmmask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask error on SMM MMIO Read in Ubox - MSCOD
                               0x8006
                            */
    UINT32 misalignedmmiowrsmmmask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask error on SMM MMCFG Write in Ubox - MSCOD
                               0x8007
                            */
    UINT32 misalignedmmiordnonsmmmask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask error on SMM MMIO Read in Ubox - MSCOD
                               0x8008
                            */
    UINT32 misalignedmmiowrnonsmmmask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask error on SMM MMIO Write in Ubox - MSCOD
                               0x8009
                            */
    UINT32 gpsbparityerrormask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* Mask GPSB Parity Error in Ubox - MSCOD 0x800C */
    UINT32 saierrormask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask SMI generation on SAI Error in Ubox - MSCOD
                               0x800D
                            */
    UINT32 semaphoreerrormask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask SMI generation on Semaphore Error in Ubox -
                               MSCOD 0x800E
                            */
    UINT32 akegresswritetovalidentrymask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask AK Egress Agent wirte to occuped entry
                               error - MSCOD 0x800F
                            */
    UINT32 blegresswritetovalidentrymask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask BL Egress Agent wirte to occuped entry
                               error - MSCOD 0x8010
                            */
    UINT32 akcegresswritetovalidentrymask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask AKC Egress Agent wirte to occuped entry
                               error - MSCOD 0x8011
                            */
    UINT32 akegressoverflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* Mask AK Egress overflow error - MSCOD 0x8012 */
    UINT32 blegressoverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* Mask BL Egress overflow error - MSCOD 0x8013 */
    UINT32 akcegressoverflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* Mask AKC Egress overflow error - MSCOD 0x8014 */
    UINT32 chatouboxoverflowncbmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask CHA to Ubox Overlfow on NCB error - MSCOD
                               0x8015
                            */
    UINT32 blingressoverflowmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask CHA to Ubox Overlfow on NCS error - MSCOD
                               0x8016
                            */
    UINT32 upitouboxoverflowncbmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask UPI to Ubox Overlfow on NCB error - MSCOD
                               0x8017
                            */
    UINT32 upitouboxoverflowncsmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask UPI to Ubox Overlfow on NCS error - MSCOD
                               0x8018
                            */
    UINT32 ingressparityerrormask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /* MaskM2Ubox Ingress Parity Error - MSCOD 0x8019 */
    UINT32 ms2iosftouboxoverflowncbmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask MS2IOSF to Ubox Overflow on NCB error -
                               MSCOD 0x801A
                            */
    UINT32 ms2iosftouboxoverflowncsmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask MS2IOSF to Ubox Overflow on NCS error -
                               MSCOD 0x801B
                            */
    UINT32 enable_io_mca : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* BIOS enable to IOMCA. Moved from IIO to Ubox */
    UINT32 enable_pcc_eq0_sev1 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Enable PCC=0 for Severity-1 errors. '0:
                               disabled, '1 - enabled
                            */
    UINT32 mca2ieh_enable : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Enable forwarding MCA messages to IEH. Needed
                               for Cooperlake.
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_STRUCT;

/* CORECOUNT_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e000e0)                                                      */
/*      SPRB0 (0x21e000e0)                                                      */
/*      SPRHBM (0x21e000e0)                                                     */
/*      SPRC0 (0x21e000e0)                                                      */
/*      SPRMCC (0x21e000e0)                                                     */
/*      SPRUCC (0x21e000e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reflection of the LTCount2 register
*/


#define CORECOUNT_UBOX_CFG_REG 0x130200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 corecount : 10;

                            /* Bits[9:0], Access Type=RO/V, default=0x00000000*/

                            /* Reflection of the LTCount2 uCR */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORECOUNT_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG supported on:                            */
/*      SPRA0 (0x21e000e4)                                                      */
/*      SPRB0 (0x21e000e4)                                                      */
/*      SPRHBM (0x21e000e4)                                                     */
/*      SPRC0 (0x21e000e4)                                                      */
/*      SPRMCC (0x21e000e4)                                                     */
/*      SPRUCC (0x21e000e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is error status register in the Ubox and covers most of the interrupt related errors
*/


#define NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_REG 0x130200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /* reserved bits */
    UINT32 poisonrsvd : 1;

                            /* Bits[2:2], Access Type=RW/V/P, default=0x00000000*/

                            /* Ubox received a poisoned transaction */
    UINT32 unsupportedopcode : 1;

                            /* Bits[3:3], Access Type=RW/V/P, default=0x00000000*/

                            /* Unsupported opcode received by Ubox */
    UINT32 cfgrdaddrmisaligned : 1;

                            /* Bits[4:4], Access Type=RW/V/P, default=0x00000000*/

                            /* MMCFG Read Address Misaligment received by Ubox */
    UINT32 cfgwraddrmisaligned : 1;

                            /* Bits[5:5], Access Type=RW/V/P, default=0x00000000*/

                            /* MMCFG Write Address Misaligment received by Ubox */
    UINT32 smitimeout : 1;

                            /* Bits[6:6], Access Type=RW/V/P, default=0x00000000*/

                            /* SMI Timeout received by Ubox */
    UINT32 masterlocktimeout : 1;

                            /* Bits[7:7], Access Type=RW/V/P, default=0x00000000*/

                            /* Master Lock Timeout received by Ubox */
    UINT32 reserved1 : 9;

                            /* Bits[16:8], Access Type=RO, default=0x00000000*/

                            /* reserved */
    UINT32 sideband_tracker_error : 1;

                            /* Bits[17:17], Access Type=RW/V/P, default=0x00000000*/

                            /* Sideband tracker error */
    UINT32 sideband_tracker_timeout : 8;

                            /* Bits[25:18], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               One hot per tracker - Tracker TimeOut. This
                               error occurs when any NP request doesn't receive
                               response in 4K cycles. The event is SV use and
                               logging only, not signaling as Ubox error.
                            */
    UINT32 reserved2 : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* Reserved bits */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_UBOXERRSTS_UBOX_CFG_STRUCT;

/* UBOXERRMISC2_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x21e000e8)                                                      */
/*      SPRB0 (0x21e000e8)                                                      */
/*      SPRHBM (0x21e000e8)                                                     */
/*      SPRC0 (0x21e000e8)                                                      */
/*      SPRMCC (0x21e000e8)                                                     */
/*      SPRUCC (0x21e000e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logging of additional error information. 
*/


#define UBOXERRMISC2_UBOX_CFG_REG 0x130200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address_40_9 : 32;

                            /* Bits[31:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               address[40:9](inbound BL ring) logged at event
                               of poison reception/unsupport opcode/misaligned
                               CFG access.
                            */

  } Bits;
  UINT32 Data;

} UBOXERRMISC2_UBOX_CFG_STRUCT;

/* UBOXERRMISC_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e000ec)                                                      */
/*      SPRB0 (0x21e000ec)                                                      */
/*      SPRHBM (0x21e000ec)                                                     */
/*      SPRC0 (0x21e000ec)                                                      */
/*      SPRMCC (0x21e000ec)                                                     */
/*      SPRUCC (0x21e000ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logging of additional error information. 
*/


#define UBOXERRMISC_UBOX_CFG_REG 0x130200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res_opcode : 4;

                            /* Bits[3:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Opcode(inbound BL ring) logged at event of
                               poison reception/unsupport opcode/misaligned CFG
                               access.
                            */
    UINT32 rtid : 11;

                            /* Bits[14:4], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               RTID(inbound BL ring) logged at event of poison
                               reception/unsupport opcode/misaligned CFG
                               access.
                            */
    UINT32 rnid : 6;

                            /* Bits[20:15], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               RNID(inbound BL ring) logged at event of poison
                               reception/unsupport opcode/misaligned CFG
                               access.
                            */
    UINT32 src : 2;

                            /* Bits[22:21], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Source(inbound BL ring) logged at event of
                               poison reception/unsupport opcode/misaligned CFG
                               access.
                            */
    UINT32 address_8_0 : 9;

                            /* Bits[31:23], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               address[8:0](inbound BL ring) logged at event of
                               poison reception/unsupport opcode/misaligned CFG
                               access.
                            */

  } Bits;
  UINT32 Data;

} UBOXERRMISC_UBOX_CFG_STRUCT;

/* UBOXERRMISC3_UBOX_CFG_REG supported on:                                      */
/*      SPRA0 (0x21e000f4)                                                      */
/*      SPRB0 (0x21e000f4)                                                      */
/*      SPRHBM (0x21e000f4)                                                     */
/*      SPRC0 (0x21e000f4)                                                      */
/*      SPRMCC (0x21e000f4)                                                     */
/*      SPRUCC (0x21e000f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logging of additional error information. 
*/


#define UBOXERRMISC3_UBOX_CFG_REG 0x130200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_41 : 11;

                            /* Bits[10:0], Access Type=RW/V/P, default=0x00000000*/

                            /* BL request address[51:41] */
    UINT32 rtid_11 : 1;

                            /* Bits[11:11], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               RTID[11] since SPR increased RTID width to 12b.
                               Adding it here since ErrMisc was maxed out
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UBOXERRMISC3_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e000f8)                                                      */
/*      SPRB0 (0x21e000f8)                                                      */
/*      SPRHBM (0x21e000f8)                                                     */
/*      SPRC0 (0x21e000f8)                                                      */
/*      SPRMCC (0x21e000f8)                                                     */
/*      SPRUCC (0x21e000f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG 0x130200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e000fc)                                                      */
/*      SPRB0 (0x21e000fc)                                                      */
/*      SPRHBM (0x21e000fc)                                                     */
/*      SPRC0 (0x21e000fc)                                                      */
/*      SPRMCC (0x21e000fc)                                                     */
/*      SPRUCC (0x21e000fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG 0x130200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_STRUCT;

/* PXPENHCAP_0_0_0_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00100)                                                      */
/*      SPRB0 (0x21e00100)                                                      */
/*      SPRHBM (0x21e00100)                                                     */
/*      SPRC0 (0x21e00100)                                                      */
/*      SPRMCC (0x21e00100)                                                     */
/*      SPRUCC (0x21e00100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_0_0_0_UBOX_CFG_REG 0x13020100

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

} PXPENHCAP_0_0_0_UBOX_CFG_STRUCT;

/* UBOX_CTS_ERR_MASK_UBOX_CFG_REG supported on:                                 */
/*      SPRA0 (0x21e00108)                                                      */
/*      SPRB0 (0x21e00108)                                                      */
/*      SPRHBM (0x21e00108)                                                     */
/*      SPRC0 (0x21e00108)                                                      */
/*      SPRMCC (0x21e00108)                                                     */
/*      SPRUCC (0x21e00108)                                                     */
/* Register default value on SPRA0: 0x000FFFFF                                  */
/* Register default value on SPRB0: 0x000FFFFF                                  */
/* Register default value on SPRHBM: 0x000FFFFF                                 */
/* Register default value on SPRC0: 0x000FFFFF                                  */
/* Register default value on SPRMCC: 0x000FFFFF                                 */
/* Register default value on SPRUCC: 0x000FFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is the mask register to filter Ubox errors sending to CTS. If the mask bits in UBOXErrCtl_CFG are set, the mask bits of this register are dont care and no error is sent.
*/


#define UBOX_CTS_ERR_MASK_UBOX_CFG_REG 0x13020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 maskunsupportopcode : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by unsupported
                               opcode received in UBox.
                            */
    UINT32 maskpoisonreception : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by poison
                               received in UBox.
                            */
    UINT32 maskcfgrdaddrmisalign : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by CFG read
                               address misalignment in UBox.
                            */
    UINT32 maskcfgwraddrmisalign : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by CFG write
                               address misalignment in UBox.
                            */
    UINT32 masksmitimeout : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by SMI timeout
                               in UBox.
                            */
    UINT32 maskmasterlocktimeout : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by master lock
                               timeout in UBox.
                            */
    UINT32 maskm2mcerror : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by strobe MC
                               error from M2PCIE.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maskuboxierr : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by IERR message
                               received in UBox.
                            */
    UINT32 maskuboxumc : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by UMC message
                               received in UBox.
                            */
    UINT32 maskuboxmsmi : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by MSMI message
                               received in UBox.
                            */
    UINT32 maskpcuproxyierr : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by PCU proxy of
                               IERR in UBox.
                            */
    UINT32 maskpcuproxyumc : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by PCU proxy of
                               UMC in UBox.
                            */
    UINT32 maskpcuproxymsmi : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block CTS error updated by PCU proxy of
                               MSMI in UBox.
                            */
    UINT32 maskextviral : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by CATERR pin
                               assertion
                            */
    UINT32 maskmsgchtrkerr : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by Message Channel
                               Tracker in UBox
                            */
    UINT32 maskmsgchtrktimeout : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by Message Channel
                               Tracker in UBox
                            */
    UINT32 masksmisrcumc : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by SMI from UMC in
                               UBox
                            */
    UINT32 masksmisrcimc : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by SMI from iMC in
                               UBox
                            */
    UINT32 masksmidelivld : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to CTS error updated by SMI delivery
                               status valid in UBox
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UBOX_CTS_ERR_MASK_UBOX_CFG_STRUCT;

/* UBOX_EVENT_ERR_SEL_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e0010c)                                                      */
/*      SPRB0 (0x21e0010c)                                                      */
/*      SPRHBM (0x21e0010c)                                                     */
/*      SPRC0 (0x21e0010c)                                                      */
/*      SPRMCC (0x21e0010c)                                                     */
/*      SPRUCC (0x21e0010c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is the select register to choose the error event to be sent out as triggers for CTS or MBP.
*/


#define UBOX_EVENT_ERR_SEL_UBOX_CFG_REG 0x1302010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 triggersel0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit0. 0 : PCU UMC 1 : CTS
                               Trigger selected by UBOX_CTS_ERR_MASK_CFG 2 :
                               IIO Error[0]
                            */
    UINT32 rsvd : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 triggersel1 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit1. 0 : PCU IERR 1 : CTS
                               Trigger selected by UBOX_CTS_ERR_MASK_CFG 2 :
                               IIO Error[1]
                            */
    UINT32 rsvd_6 : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 triggersel2 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit2. 0 : MSMI 1 : CTS Trigger
                               selected by UBOX_CTS_ERR_MASK_CFG 2 : IIO
                               Error[2]
                            */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 triggersel3 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit3. 0 : CSMI 1 : CTS Trigger
                               selected by UBOX_CTS_ERR_MASK_CFG 2 : IIO
                               Error[0]
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 triggersel4 : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit4. 0 : CMC 1 : CTS Trigger
                               selected by UBOX_CTS_ERR_MASK_CFG 2 : IIO
                               Error[1]
                            */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 triggersel5 : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               Select which error event to output to CTS or MBP
                               through Trigger Bit5. 0 : PMON Overflow 1 : CTS
                               Trigger selected by UBOX_CTS_ERR_MASK_CFG 2 :
                               IIO Error[2]
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UBOX_EVENT_ERR_SEL_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG supported on:                             */
/*      SPRA0 (0x21e00110)                                                      */
/*      SPRB0 (0x21e00110)                                                      */
/*      SPRHBM (0x21e00110)                                                     */
/*      SPRC0 (0x21e00110)                                                      */
/*      SPRMCC (0x21e00110)                                                     */
/*      SPRUCC (0x21e00110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logs the source of SMI
*/


#define NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG 0x13020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mainbandmeshid : 7;

                            /* Bits[6:0], Access Type=RW/V/P, default=0x00000000*/

                            /* Mainband Mesh ID for SMI source */
    UINT32 mainbandvalid : 1;

                            /* Bits[7:7], Access Type=RW/V/P, default=0x00000000*/

                            /* Valid bit for Mainband Mesh ID */
    UINT32 sidebandportid : 10;

                            /* Bits[17:8], Access Type=RW/V/P, default=0x00000000*/

                            /* Sideband Logical Port ID for SMI source */
    UINT32 sidebandvalid : 1;

                            /* Bits[18:18], Access Type=RW/V/P, default=0x00000000*/

                            /* Valid bit for Sideband Port ID */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_SMISRCLOG_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_SMISRCCLR_UBOX_CFG_REG supported on:                             */
/*      SPRA0 (0x21e00114)                                                      */
/*      SPRB0 (0x21e00114)                                                      */
/*      SPRHBM (0x21e00114)                                                     */
/*      SPRC0 (0x21e00114)                                                      */
/*      SPRMCC (0x21e00114)                                                     */
/*      SPRUCC (0x21e00114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Register for BIOS to clear SMISRCLOG information
*/


#define NCEVENTS_CR_SMISRCCLR_UBOX_CFG_REG 0x13020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 7;

                            /* Bits[6:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mainbandvalidclr : 1;

                            /* Bits[7:7], Access Type=RW/V/P, default=0x00000000*/

                            /* Valid bit for Mainband Mesh ID clear */
    UINT32 rsvd_8 : 10;

                            /* Bits[17:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sidebandvalidclr : 1;

                            /* Bits[18:18], Access Type=RW/V/P, default=0x00000000*/

                            /* Valid bit for Sideband Port ID clear */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_SMISRCCLR_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD7_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00120)                                                      */
/*      SPRB0 (0x21e00120)                                                      */
/*      SPRHBM (0x21e00120)                                                     */
/*      SPRC0 (0x21e00120)                                                      */
/*      SPRMCC (0x21e00120)                                                     */
/*      SPRUCC (0x21e00120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD7_UBOX_CFG_REG 0x13020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD7_UBOX_CFG_STRUCT;

/* BIOSSTICKYLOCKBYPASSSCRATCHPAD8_UBOX_CFG_REG supported on:                   */
/*      SPRA0 (0x21e00124)                                                      */
/*      SPRB0 (0x21e00124)                                                      */
/*      SPRHBM (0x21e00124)                                                     */
/*      SPRC0 (0x21e00124)                                                      */
/*      SPRMCC (0x21e00124)                                                     */
/*      SPRUCC (0x21e00124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS events.
*/


#define BIOSSTICKYLOCKBYPASSSCRATCHPAD8_UBOX_CFG_REG 0x13020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Usage - checkpoints to see where in the process
                               the new socket is at.
                            */

  } Bits;
  UINT32 Data;

} BIOSSTICKYLOCKBYPASSSCRATCHPAD8_UBOX_CFG_STRUCT;

/* PXPENHCAP_UBOX_CFG_REG supported on:                                         */
/*      SPRA0 (0x21e00128)                                                      */
/*      SPRB0 (0x21e00128)                                                      */
/*      SPRHBM (0x21e00128)                                                     */
/*      SPRC0 (0x21e00128)                                                      */
/*      SPRMCC (0x21e00128)                                                     */
/*      SPRUCC (0x21e00128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_UBOX_CFG_REG 0x13020128

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

} PXPENHCAP_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_DELAY_0_N0_UBOX_CFG_REG supported on:                       */
/*      SPRA0 (0x21e00130)                                                      */
/*      SPRB0 (0x21e00130)                                                      */
/*      SPRHBM (0x21e00130)                                                     */
/*      SPRC0 (0x21e00130)                                                      */
/*      SPRMCC (0x21e00130)                                                     */
/*      SPRUCC (0x21e00130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_MBE_DELAY_0_N0_UBOX_CFG_REG 0x13020130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_DELAY_0_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_DELAY_0_N1_UBOX_CFG_REG supported on:                       */
/*      SPRA0 (0x21e00134)                                                      */
/*      SPRB0 (0x21e00134)                                                      */
/*      SPRHBM (0x21e00134)                                                     */
/*      SPRC0 (0x21e00134)                                                      */
/*      SPRMCC (0x21e00134)                                                     */
/*      SPRUCC (0x21e00134)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_MBE_DELAY_0_N1_UBOX_CFG_REG 0x13020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_4 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_5 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_6 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_7 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_DELAY_0_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_DELAY_1_N0_UBOX_CFG_REG supported on:                       */
/*      SPRA0 (0x21e00138)                                                      */
/*      SPRB0 (0x21e00138)                                                      */
/*      SPRHBM (0x21e00138)                                                     */
/*      SPRC0 (0x21e00138)                                                      */
/*      SPRMCC (0x21e00138)                                                     */
/*      SPRUCC (0x21e00138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_MBE_DELAY_1_N0_UBOX_CFG_REG 0x13020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_8 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_9 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_DELAY_1_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_DELAY_1_N1_UBOX_CFG_REG supported on:                       */
/*      SPRA0 (0x21e0013c)                                                      */
/*      SPRB0 (0x21e0013c)                                                      */
/*      SPRHBM (0x21e0013c)                                                     */
/*      SPRC0 (0x21e0013c)                                                      */
/*      SPRMCC (0x21e0013c)                                                     */
/*      SPRUCC (0x21e0013c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_MBE_DELAY_1_N1_UBOX_CFG_REG 0x1302013C

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

} NCEVENTS_CFG_MBE_DELAY_1_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_TIMEWINDOW_0_N0_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00140)                                                      */
/*      SPRB0 (0x21e00140)                                                      */
/*      SPRHBM (0x21e00140)                                                     */
/*      SPRC0 (0x21e00140)                                                      */
/*      SPRMCC (0x21e00140)                                                     */
/*      SPRUCC (0x21e00140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_MBE_TIMEWINDOW_0_N0_UBOX_CFG_REG 0x13020140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_TIMEWINDOW_0_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_TIMEWINDOW_0_N1_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00144)                                                      */
/*      SPRB0 (0x21e00144)                                                      */
/*      SPRHBM (0x21e00144)                                                     */
/*      SPRC0 (0x21e00144)                                                      */
/*      SPRMCC (0x21e00144)                                                     */
/*      SPRUCC (0x21e00144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_MBE_TIMEWINDOW_0_N1_UBOX_CFG_REG 0x13020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_4 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_5 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_6 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_7 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_TIMEWINDOW_0_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_TIMEWINDOW_1_N0_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00148)                                                      */
/*      SPRB0 (0x21e00148)                                                      */
/*      SPRHBM (0x21e00148)                                                     */
/*      SPRC0 (0x21e00148)                                                      */
/*      SPRMCC (0x21e00148)                                                     */
/*      SPRUCC (0x21e00148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_MBE_TIMEWINDOW_1_N0_UBOX_CFG_REG 0x13020148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_8 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_9 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_MBE_TIMEWINDOW_1_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_MBE_TIMEWINDOW_1_N1_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e0014c)                                                      */
/*      SPRB0 (0x21e0014c)                                                      */
/*      SPRHBM (0x21e0014c)                                                     */
/*      SPRC0 (0x21e0014c)                                                      */
/*      SPRMCC (0x21e0014c)                                                     */
/*      SPRUCC (0x21e0014c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_MBE_TIMEWINDOW_1_N1_UBOX_CFG_REG 0x1302014C

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

} NCEVENTS_CFG_MBE_TIMEWINDOW_1_N1_UBOX_CFG_STRUCT;

/* UBOX_GL_ERR_CFG_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00150)                                                      */
/*      SPRB0 (0x21e00150)                                                      */
/*      SPRHBM (0x21e00150)                                                     */
/*      SPRC0 (0x21e00150)                                                      */
/*      SPRMCC (0x21e00150)                                                     */
/*      SPRUCC (0x21e00150)                                                     */
/* Register default value on SPRA0: 0x01FC0000                                  */
/* Register default value on SPRB0: 0x01FC0000                                  */
/* Register default value on SPRHBM: 0x01FC0000                                 */
/* Register default value on SPRC0: 0x01FC0000                                  */
/* Register default value on SPRMCC: 0x01FC0000                                 */
/* Register default value on SPRUCC: 0x01FC0000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the viral/global fatal error config/status register in the Ubox. The mask bits in UBOXErrCtl_CFG are set, the mask bits of this register are dont care and no viral/fatal error are signaled.
*/


#define UBOX_GL_ERR_CFG_UBOX_CFG_REG 0x13020150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 statusuboxfatalerror : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Read only global fatal error status in UBox (UMC
                               or IERR)
                            */
    UINT32 statusuboxviral : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* Read only global viralr status in UBox */
    UINT32 resetuboxfatalerror : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear Ubox fatal error status. */
    UINT32 resetuboxviral : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear Ubox viral status. */
    UINT32 maskuboxumc : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block fatal error status updated by UMC
                               message received in UBox.
                            */
    UINT32 maskuboxierr : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block fatal error status updated by
                               IERR message received in UBox.
                            */
    UINT32 maskunsupportopcode : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by unsupported opcode received in UBox.
                            */
    UINT32 maskpoisonreception : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by poison received in UBox.
                            */
    UINT32 maskcfgrdaddrmisalign : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by CFG read address misalignment in UBox.
                            */
    UINT32 maskcfgwraddrmisalign : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by CFG write address misalignment in UBox.
                            */
    UINT32 masksmitimeout : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by SMI timeout in UBox.
                            */
    UINT32 maskmasterlocktimeout : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by master lock timeout in UBox.
                            */
    UINT32 maskpcuproxyierr : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by PCU proxy of IERR in UBox.
                            */
    UINT32 maskpcuproxyumc : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by PCU proxy of UMC in UBox.
                            */
    UINT32 maskpcuproxymsmi : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block viral[0], fatal[1] status updated
                               by PCU proxy of UMC in UBox.
                            */
    UINT32 maskextviral : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Set 1 to block viral status updated by CATERR
                               pin assertion
                            */
    UINT32 maskuboxmsmi : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block fatal error status updated by
                               MSMI message received in UBox.
                            */
    UINT32 maskieherr2 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Set 1 to block fatal error status updated by
                               MSMI message received in UBox.
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UBOX_GL_ERR_CFG_UBOX_CFG_STRUCT;

/* NCEVENTS_CR_GL_ERR_CFG1_UBOX_CFG_REG supported on:                           */
/*      SPRA0 (0x21e00158)                                                      */
/*      SPRB0 (0x21e00158)                                                      */
/*      SPRHBM (0x21e00158)                                                     */
/*      SPRC0 (0x21e00158)                                                      */
/*      SPRMCC (0x21e00158)                                                     */
/*      SPRUCC (0x21e00158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Bits of this register mask global fatal signaling for various errors in the Ubox
*/


#define NCEVENTS_CR_GL_ERR_CFG1_UBOX_CFG_REG 0x13020158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 maskmmiordaddrmisalign : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskmmiowraddrmisalign : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskgpsbparityerror : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 masksaierror : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 masksemaphoreerror : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskakegresswritevalidentry : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskblegresswritevalidentry : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskakcegresswritevalidentry : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskakegressoverflow : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskblegressoverflow : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskakcegressoverflow : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskchancboverflow : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskchancsoverflow : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskupincboverflow : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskupincsoverflow : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskingressparityerror : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskm2incboverflow : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 maskm2incsoverflow : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* When set, masks this error */
    UINT32 masksgxdoorbellerror : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* This is for masking the SGX Doorbell Error */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CR_GL_ERR_CFG1_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_CP_N0_UBOX_CFG_REG supported on:                                 */
/*      SPRA0 (0x21e00160)                                                      */
/*      SPRB0 (0x21e00160)                                                      */
/*      SPRHBM (0x21e00160)                                                     */
/*      SPRC0 (0x21e00160)                                                      */
/*      SPRMCC (0x21e00160)                                                     */
/*      SPRUCC (0x21e00160)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N0_UBOX_CFG_REG 0x13020160

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

} LB_MCHECK_W_CP_N0_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_CP_N1_UBOX_CFG_REG supported on:                                 */
/*      SPRA0 (0x21e00164)                                                      */
/*      SPRB0 (0x21e00164)                                                      */
/*      SPRHBM (0x21e00164)                                                     */
/*      SPRC0 (0x21e00164)                                                      */
/*      SPRMCC (0x21e00164)                                                     */
/*      SPRUCC (0x21e00164)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define LB_MCHECK_W_CP_N1_UBOX_CFG_REG 0x13020164

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

} LB_MCHECK_W_CP_N1_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_RAC_N0_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e00168)                                                      */
/*      SPRB0 (0x21e00168)                                                      */
/*      SPRHBM (0x21e00168)                                                     */
/*      SPRC0 (0x21e00168)                                                      */
/*      SPRMCC (0x21e00168)                                                     */
/*      SPRUCC (0x21e00168)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N0_UBOX_CFG_REG 0x13020168

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

} LB_MCHECK_W_RAC_N0_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_RAC_N1_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e0016c)                                                      */
/*      SPRB0 (0x21e0016c)                                                      */
/*      SPRHBM (0x21e0016c)                                                     */
/*      SPRC0 (0x21e0016c)                                                      */
/*      SPRMCC (0x21e0016c)                                                     */
/*      SPRUCC (0x21e0016c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_RAC_N1_UBOX_CFG_REG 0x1302016C

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

} LB_MCHECK_W_RAC_N1_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_WAC_N0_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e00170)                                                      */
/*      SPRB0 (0x21e00170)                                                      */
/*      SPRHBM (0x21e00170)                                                     */
/*      SPRC0 (0x21e00170)                                                      */
/*      SPRMCC (0x21e00170)                                                     */
/*      SPRUCC (0x21e00170)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N0_UBOX_CFG_REG 0x13020170

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

} LB_MCHECK_W_WAC_N0_UBOX_CFG_STRUCT;

/* LB_MCHECK_W_WAC_N1_UBOX_CFG_REG supported on:                                */
/*      SPRA0 (0x21e00174)                                                      */
/*      SPRB0 (0x21e00174)                                                      */
/*      SPRHBM (0x21e00174)                                                     */
/*      SPRC0 (0x21e00174)                                                      */
/*      SPRMCC (0x21e00174)                                                     */
/*      SPRUCC (0x21e00174)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define LB_MCHECK_W_WAC_N1_UBOX_CFG_REG 0x13020174

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

} LB_MCHECK_W_WAC_N1_UBOX_CFG_STRUCT;

/* OS_W_CP_N0_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x21e00178)                                                      */
/*      SPRB0 (0x21e00178)                                                      */
/*      SPRHBM (0x21e00178)                                                     */
/*      SPRC0 (0x21e00178)                                                      */
/*      SPRMCC (0x21e00178)                                                     */
/*      SPRUCC (0x21e00178)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N0_UBOX_CFG_REG 0x13020178

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

} OS_W_CP_N0_UBOX_CFG_STRUCT;

/* OS_W_CP_N1_UBOX_CFG_REG supported on:                                        */
/*      SPRA0 (0x21e0017c)                                                      */
/*      SPRB0 (0x21e0017c)                                                      */
/*      SPRHBM (0x21e0017c)                                                     */
/*      SPRC0 (0x21e0017c)                                                      */
/*      SPRMCC (0x21e0017c)                                                     */
/*      SPRUCC (0x21e0017c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define OS_W_CP_N1_UBOX_CFG_REG 0x1302017C

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

} OS_W_CP_N1_UBOX_CFG_STRUCT;

/* OS_W_RAC_N0_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e00180)                                                      */
/*      SPRB0 (0x21e00180)                                                      */
/*      SPRHBM (0x21e00180)                                                     */
/*      SPRC0 (0x21e00180)                                                      */
/*      SPRMCC (0x21e00180)                                                     */
/*      SPRUCC (0x21e00180)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N0_UBOX_CFG_REG 0x13020180

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

} OS_W_RAC_N0_UBOX_CFG_STRUCT;

/* OS_W_RAC_N1_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e00184)                                                      */
/*      SPRB0 (0x21e00184)                                                      */
/*      SPRHBM (0x21e00184)                                                     */
/*      SPRC0 (0x21e00184)                                                      */
/*      SPRMCC (0x21e00184)                                                     */
/*      SPRUCC (0x21e00184)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N1_UBOX_CFG_REG 0x13020184

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

} OS_W_RAC_N1_UBOX_CFG_STRUCT;

/* OS_W_WAC_N0_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e00188)                                                      */
/*      SPRB0 (0x21e00188)                                                      */
/*      SPRHBM (0x21e00188)                                                     */
/*      SPRC0 (0x21e00188)                                                      */
/*      SPRMCC (0x21e00188)                                                     */
/*      SPRUCC (0x21e00188)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N0_UBOX_CFG_REG 0x13020188

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

} OS_W_WAC_N0_UBOX_CFG_STRUCT;

/* OS_W_WAC_N1_UBOX_CFG_REG supported on:                                       */
/*      SPRA0 (0x21e0018c)                                                      */
/*      SPRB0 (0x21e0018c)                                                      */
/*      SPRHBM (0x21e0018c)                                                     */
/*      SPRC0 (0x21e0018c)                                                      */
/*      SPRMCC (0x21e0018c)                                                     */
/*      SPRUCC (0x21e0018c)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N1_UBOX_CFG_REG 0x1302018C

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

} OS_W_WAC_N1_UBOX_CFG_STRUCT;







/* UBOXCSMITHRES4_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001a8)                                                      */
/*      SPRB0 (0x21e001a8)                                                      */
/*      SPRHBM (0x21e001a8)                                                     */
/*      SPRC0 (0x21e001a8)                                                      */
/*      SPRMCC (0x21e001a8)                                                     */
/*      SPRUCC (0x21e001a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES4_UBOX_CFG_REG 0x130201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES4_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES5_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001ac)                                                      */
/*      SPRB0 (0x21e001ac)                                                      */
/*      SPRHBM (0x21e001ac)                                                     */
/*      SPRC0 (0x21e001ac)                                                      */
/*      SPRMCC (0x21e001ac)                                                     */
/*      SPRUCC (0x21e001ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES5_UBOX_CFG_REG 0x130201AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES5_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES6_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001b0)                                                      */
/*      SPRB0 (0x21e001b0)                                                      */
/*      SPRHBM (0x21e001b0)                                                     */
/*      SPRC0 (0x21e001b0)                                                      */
/*      SPRMCC (0x21e001b0)                                                     */
/*      SPRUCC (0x21e001b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES6_UBOX_CFG_REG 0x130201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES6_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES7_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001b4)                                                      */
/*      SPRB0 (0x21e001b4)                                                      */
/*      SPRHBM (0x21e001b4)                                                     */
/*      SPRC0 (0x21e001b4)                                                      */
/*      SPRMCC (0x21e001b4)                                                     */
/*      SPRUCC (0x21e001b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES7_UBOX_CFG_REG 0x130201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES7_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES8_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001b8)                                                      */
/*      SPRB0 (0x21e001b8)                                                      */
/*      SPRHBM (0x21e001b8)                                                     */
/*      SPRC0 (0x21e001b8)                                                      */
/*      SPRMCC (0x21e001b8)                                                     */
/*      SPRUCC (0x21e001b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES8_UBOX_CFG_REG 0x130201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES8_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES9_UBOX_CFG_REG supported on:                                    */
/*      SPRA0 (0x21e001bc)                                                      */
/*      SPRB0 (0x21e001bc)                                                      */
/*      SPRHBM (0x21e001bc)                                                     */
/*      SPRC0 (0x21e001bc)                                                      */
/*      SPRMCC (0x21e001bc)                                                     */
/*      SPRUCC (0x21e001bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES9_UBOX_CFG_REG 0x130201BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES9_UBOX_CFG_STRUCT;

/* EMCA_CORE_RMSMI_LOG_UBOX_CFG_REG supported on:                               */
/*      SPRA0 (0x21e001c0)                                                      */
/*      SPRB0 (0x21e001c0)                                                      */
/*      SPRHBM (0x21e001c0)                                                     */
/*      SPRC0 (0x21e001c0)                                                      */
/*      SPRMCC (0x21e001c0)                                                     */
/*      SPRUCC (0x21e001c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled a rMSMI to ubox via C2U
*/


#define EMCA_CORE_RMSMI_LOG_UBOX_CFG_REG 0x130201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 0 */
    UINT32 core1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 1 */
    UINT32 core2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 2 */
    UINT32 core3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 3 */
    UINT32 core4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 4 */
    UINT32 core5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 5 */
    UINT32 core6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 6 */
    UINT32 core7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 7 */
    UINT32 core8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 8 */
    UINT32 core9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 9 */
    UINT32 core10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 10 */
    UINT32 core11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 11 */
    UINT32 core12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 12 */
    UINT32 core13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 13 */
    UINT32 core14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 14 */
    UINT32 core15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 15 */
    UINT32 core16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 16 */
    UINT32 core17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 17 */
    UINT32 core18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 18 */
    UINT32 core19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 19 */
    UINT32 core20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 20 */
    UINT32 core21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 21 */
    UINT32 core22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 22 */
    UINT32 core23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 23 */
    UINT32 core24 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 24 */
    UINT32 core25 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 25 */
    UINT32 core26 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 26 */
    UINT32 core27 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 27 */
    UINT32 core28 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 28 */
    UINT32 core29 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 29 */
    UINT32 core30 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 30 */
    UINT32 core31 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 31 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_RMSMI_LOG_UBOX_CFG_STRUCT;

/* EMCA_CORE_RMSMI_LOG_CFG1_UBOX_CFG_REG supported on:                          */
/*      SPRA0 (0x21e001c4)                                                      */
/*      SPRB0 (0x21e001c4)                                                      */
/*      SPRHBM (0x21e001c4)                                                     */
/*      SPRC0 (0x21e001c4)                                                      */
/*      SPRMCC (0x21e001c4)                                                     */
/*      SPRUCC (0x21e001c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This is a log of which cores have signalled a MSMI to ubox via C2U
*/


#define EMCA_CORE_RMSMI_LOG_CFG1_UBOX_CFG_REG 0x130201C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 core32 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 32 */
    UINT32 core33 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 33 */
    UINT32 core34 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 34 */
    UINT32 core35 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 35 */
    UINT32 core36 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 36 */
    UINT32 core37 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 37 */
    UINT32 core38 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 38 */
    UINT32 core39 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 39 */
    UINT32 core40 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 40 */
    UINT32 core41 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 41 */
    UINT32 core42 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 42 */
    UINT32 core43 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 43 */
    UINT32 core44 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 44 */
    UINT32 core45 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 45 */
    UINT32 core46 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 46 */
    UINT32 core47 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 47 */
    UINT32 core48 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 48 */
    UINT32 core49 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 49 */
    UINT32 core50 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 50 */
    UINT32 core51 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 51 */
    UINT32 core52 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 52 */
    UINT32 core53 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 53 */
    UINT32 core54 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 54 */
    UINT32 core55 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 55 */
    UINT32 core56 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 56 */
    UINT32 core57 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 57 */
    UINT32 core58 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 58 */
    UINT32 core59 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 59 */
    UINT32 core60 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 60 */
    UINT32 core61 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 61 */
    UINT32 core62 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 62 */
    UINT32 core63 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /* MSMI received indicator for core 63 */

  } Bits;
  UINT32 Data;

} EMCA_CORE_RMSMI_LOG_CFG1_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES12_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001c8)                                                      */
/*      SPRB0 (0x21e001c8)                                                      */
/*      SPRHBM (0x21e001c8)                                                     */
/*      SPRC0 (0x21e001c8)                                                      */
/*      SPRMCC (0x21e001c8)                                                     */
/*      SPRUCC (0x21e001c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES12_UBOX_CFG_REG 0x130201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES12_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES13_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001d0)                                                      */
/*      SPRB0 (0x21e001d0)                                                      */
/*      SPRHBM (0x21e001d0)                                                     */
/*      SPRC0 (0x21e001d0)                                                      */
/*      SPRMCC (0x21e001d0)                                                     */
/*      SPRUCC (0x21e001d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES13_UBOX_CFG_REG 0x130201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES13_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES14_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001d4)                                                      */
/*      SPRB0 (0x21e001d4)                                                      */
/*      SPRHBM (0x21e001d4)                                                     */
/*      SPRC0 (0x21e001d4)                                                      */
/*      SPRMCC (0x21e001d4)                                                     */
/*      SPRUCC (0x21e001d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES14_UBOX_CFG_REG 0x130201D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES14_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES15_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001d8)                                                      */
/*      SPRB0 (0x21e001d8)                                                      */
/*      SPRHBM (0x21e001d8)                                                     */
/*      SPRC0 (0x21e001d8)                                                      */
/*      SPRMCC (0x21e001d8)                                                     */
/*      SPRUCC (0x21e001d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES15_UBOX_CFG_REG 0x130201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES15_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES16_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001dc)                                                      */
/*      SPRB0 (0x21e001dc)                                                      */
/*      SPRHBM (0x21e001dc)                                                     */
/*      SPRC0 (0x21e001dc)                                                      */
/*      SPRMCC (0x21e001dc)                                                     */
/*      SPRUCC (0x21e001dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES16_UBOX_CFG_REG 0x130201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES16_UBOX_CFG_STRUCT;

/* RMCERRLOGGINGREG_UBOX_CFG_REG supported on:                                  */
/*      SPRA0 (0x21e001e0)                                                      */
/*      SPRB0 (0x21e001e0)                                                      */
/*      SPRHBM (0x21e001e0)                                                     */
/*      SPRC0 (0x21e001e0)                                                      */
/*      SPRMCC (0x21e001e0)                                                     */
/*      SPRUCC (0x21e001e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/*  rMCERR first/second logging error.
*/


#define RMCERRLOGGINGREG_UBOX_CFG_REG 0x130201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 firstrmcerrsrcid : 10;

                            /* Bits[9:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the first
                               rMCErr
                            */
    UINT32 firstrmcerrsrcvalid : 1;

                            /* Bits[10:10], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the FirstrMCErrSrcID is valid */
    UINT32 firstrmcerrsrcfromcore : 1;

                            /* Bits[11:11], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the FirstrMCErrSrcID is from a C2U */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 secondrmcerrsrcid : 10;

                            /* Bits[25:16], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Logical Port ID of the end point with the second
                               rMCErr
                            */
    UINT32 secondrmcerrsrcvalid : 1;

                            /* Bits[26:26], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the SecondrMCErrSrcID is valid */
    UINT32 secondrmcerrsrcfromcore : 1;

                            /* Bits[27:27], Access Type=RW/V/P, default=0x00000000*/

                            /* Set to '1 if the SecondrMCErrSrcID is from a C2U */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RMCERRLOGGINGREG_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES19_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001e8)                                                      */
/*      SPRB0 (0x21e001e8)                                                      */
/*      SPRHBM (0x21e001e8)                                                     */
/*      SPRC0 (0x21e001e8)                                                      */
/*      SPRMCC (0x21e001e8)                                                     */
/*      SPRUCC (0x21e001e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES19_UBOX_CFG_REG 0x130201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES19_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES20_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001ec)                                                      */
/*      SPRB0 (0x21e001ec)                                                      */
/*      SPRHBM (0x21e001ec)                                                     */
/*      SPRC0 (0x21e001ec)                                                      */
/*      SPRMCC (0x21e001ec)                                                     */
/*      SPRUCC (0x21e001ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES20_UBOX_CFG_REG 0x130201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES20_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES21_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001f0)                                                      */
/*      SPRB0 (0x21e001f0)                                                      */
/*      SPRHBM (0x21e001f0)                                                     */
/*      SPRC0 (0x21e001f0)                                                      */
/*      SPRMCC (0x21e001f0)                                                     */
/*      SPRUCC (0x21e001f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES21_UBOX_CFG_REG 0x130201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES21_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES22_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001f4)                                                      */
/*      SPRB0 (0x21e001f4)                                                      */
/*      SPRHBM (0x21e001f4)                                                     */
/*      SPRC0 (0x21e001f4)                                                      */
/*      SPRMCC (0x21e001f4)                                                     */
/*      SPRUCC (0x21e001f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES22_UBOX_CFG_REG 0x130201F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES22_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES23_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001f8)                                                      */
/*      SPRB0 (0x21e001f8)                                                      */
/*      SPRHBM (0x21e001f8)                                                     */
/*      SPRC0 (0x21e001f8)                                                      */
/*      SPRMCC (0x21e001f8)                                                     */
/*      SPRUCC (0x21e001f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES23_UBOX_CFG_REG 0x130201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES23_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES24_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e001fc)                                                      */
/*      SPRB0 (0x21e001fc)                                                      */
/*      SPRHBM (0x21e001fc)                                                     */
/*      SPRC0 (0x21e001fc)                                                      */
/*      SPRMCC (0x21e001fc)                                                     */
/*      SPRUCC (0x21e001fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES24_UBOX_CFG_REG 0x130201FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES24_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES25_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00200)                                                      */
/*      SPRB0 (0x21e00200)                                                      */
/*      SPRHBM (0x21e00200)                                                     */
/*      SPRC0 (0x21e00200)                                                      */
/*      SPRMCC (0x21e00200)                                                     */
/*      SPRUCC (0x21e00200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES25_UBOX_CFG_REG 0x13020200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES25_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES26_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00204)                                                      */
/*      SPRB0 (0x21e00204)                                                      */
/*      SPRHBM (0x21e00204)                                                     */
/*      SPRC0 (0x21e00204)                                                      */
/*      SPRMCC (0x21e00204)                                                     */
/*      SPRUCC (0x21e00204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES26_UBOX_CFG_REG 0x13020204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES26_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES27_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00208)                                                      */
/*      SPRB0 (0x21e00208)                                                      */
/*      SPRHBM (0x21e00208)                                                     */
/*      SPRC0 (0x21e00208)                                                      */
/*      SPRMCC (0x21e00208)                                                     */
/*      SPRUCC (0x21e00208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES27_UBOX_CFG_REG 0x13020208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES27_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES28_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e0020c)                                                      */
/*      SPRB0 (0x21e0020c)                                                      */
/*      SPRHBM (0x21e0020c)                                                     */
/*      SPRC0 (0x21e0020c)                                                      */
/*      SPRMCC (0x21e0020c)                                                     */
/*      SPRUCC (0x21e0020c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES28_UBOX_CFG_REG 0x1302020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES28_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES29_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00210)                                                      */
/*      SPRB0 (0x21e00210)                                                      */
/*      SPRHBM (0x21e00210)                                                     */
/*      SPRC0 (0x21e00210)                                                      */
/*      SPRMCC (0x21e00210)                                                     */
/*      SPRUCC (0x21e00210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES29_UBOX_CFG_REG 0x13020210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES29_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES30_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00214)                                                      */
/*      SPRB0 (0x21e00214)                                                      */
/*      SPRHBM (0x21e00214)                                                     */
/*      SPRC0 (0x21e00214)                                                      */
/*      SPRMCC (0x21e00214)                                                     */
/*      SPRUCC (0x21e00214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES30_UBOX_CFG_REG 0x13020214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES30_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES31_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00218)                                                      */
/*      SPRB0 (0x21e00218)                                                      */
/*      SPRHBM (0x21e00218)                                                     */
/*      SPRC0 (0x21e00218)                                                      */
/*      SPRMCC (0x21e00218)                                                     */
/*      SPRUCC (0x21e00218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES31_UBOX_CFG_REG 0x13020218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES31_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES10_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00220)                                                      */
/*      SPRB0 (0x21e00220)                                                      */
/*      SPRHBM (0x21e00220)                                                     */
/*      SPRC0 (0x21e00220)                                                      */
/*      SPRMCC (0x21e00220)                                                     */
/*      SPRUCC (0x21e00220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES10_UBOX_CFG_REG 0x13020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES10_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES11_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00224)                                                      */
/*      SPRB0 (0x21e00224)                                                      */
/*      SPRHBM (0x21e00224)                                                     */
/*      SPRC0 (0x21e00224)                                                      */
/*      SPRMCC (0x21e00224)                                                     */
/*      SPRUCC (0x21e00224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES11_UBOX_CFG_REG 0x13020224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES11_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES17_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e00228)                                                      */
/*      SPRB0 (0x21e00228)                                                      */
/*      SPRHBM (0x21e00228)                                                     */
/*      SPRC0 (0x21e00228)                                                      */
/*      SPRMCC (0x21e00228)                                                     */
/*      SPRUCC (0x21e00228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES17_UBOX_CFG_REG 0x13020228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES17_UBOX_CFG_STRUCT;

/* UBOXCSMITHRES18_UBOX_CFG_REG supported on:                                   */
/*      SPRA0 (0x21e0022c)                                                      */
/*      SPRB0 (0x21e0022c)                                                      */
/*      SPRHBM (0x21e0022c)                                                     */
/*      SPRC0 (0x21e0022c)                                                      */
/*      SPRMCC (0x21e0022c)                                                     */
/*      SPRUCC (0x21e0022c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* CSMI thresholding register
*/


#define UBOXCSMITHRES18_UBOX_CFG_REG 0x1302022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bios_ce_threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* threshold value for cmsi */
    UINT32 match_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* csmi enable */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} UBOXCSMITHRES18_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N0_UBOX_CFG_REG supported on:             */
/*      SPRA0 (0x21e00300)                                                      */
/*      SPRB0 (0x21e00300)                                                      */
/*      SPRHBM (0x21e00300)                                                     */
/*      SPRC0 (0x21e00300)                                                      */
/*      SPRMCC (0x21e00300)                                                     */
/*      SPRUCC (0x21e00300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N0_UBOX_CFG_REG 0x13020300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N1_UBOX_CFG_REG supported on:             */
/*      SPRA0 (0x21e00304)                                                      */
/*      SPRB0 (0x21e00304)                                                      */
/*      SPRHBM (0x21e00304)                                                     */
/*      SPRC0 (0x21e00304)                                                      */
/*      SPRMCC (0x21e00304)                                                     */
/*      SPRUCC (0x21e00304)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N1_UBOX_CFG_REG 0x13020304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_4 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_5 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_6 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_7 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_0_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N0_UBOX_CFG_REG supported on:             */
/*      SPRA0 (0x21e00308)                                                      */
/*      SPRB0 (0x21e00308)                                                      */
/*      SPRHBM (0x21e00308)                                                     */
/*      SPRC0 (0x21e00308)                                                      */
/*      SPRMCC (0x21e00308)                                                     */
/*      SPRUCC (0x21e00308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N0_UBOX_CFG_REG 0x13020308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_timewindow_8 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 mbe_timewindow_9 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Time Window */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N1_UBOX_CFG_REG supported on:             */
/*      SPRA0 (0x21e0030c)                                                      */
/*      SPRB0 (0x21e0030c)                                                      */
/*      SPRHBM (0x21e0030c)                                                     */
/*      SPRC0 (0x21e0030c)                                                      */
/*      SPRMCC (0x21e0030c)                                                     */
/*      SPRUCC (0x21e0030c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Time Window
*/


#define NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N1_UBOX_CFG_REG 0x1302030C

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

} NCEVENTS_CFG_3DXP_MBE_TIMEWINDOW_1_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_DELAY_0_N0_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00310)                                                      */
/*      SPRB0 (0x21e00310)                                                      */
/*      SPRHBM (0x21e00310)                                                     */
/*      SPRC0 (0x21e00310)                                                      */
/*      SPRMCC (0x21e00310)                                                     */
/*      SPRUCC (0x21e00310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_3DXP_MBE_DELAY_0_N0_UBOX_CFG_REG 0x13020310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_DELAY_0_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_DELAY_0_N1_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00314)                                                      */
/*      SPRB0 (0x21e00314)                                                      */
/*      SPRHBM (0x21e00314)                                                     */
/*      SPRC0 (0x21e00314)                                                      */
/*      SPRMCC (0x21e00314)                                                     */
/*      SPRUCC (0x21e00314)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_3DXP_MBE_DELAY_0_N1_UBOX_CFG_REG 0x13020314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_4 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_5 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_6 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_7 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_DELAY_0_N1_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_DELAY_1_N0_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e00318)                                                      */
/*      SPRB0 (0x21e00318)                                                      */
/*      SPRHBM (0x21e00318)                                                     */
/*      SPRC0 (0x21e00318)                                                      */
/*      SPRMCC (0x21e00318)                                                     */
/*      SPRUCC (0x21e00318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_3DXP_MBE_DELAY_1_N0_UBOX_CFG_REG 0x13020318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mbe_delay_8 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 mbe_delay_9 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Platform QoS Memory BW Enforcement Delay */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NCEVENTS_CFG_3DXP_MBE_DELAY_1_N0_UBOX_CFG_STRUCT;

/* NCEVENTS_CFG_3DXP_MBE_DELAY_1_N1_UBOX_CFG_REG supported on:                  */
/*      SPRA0 (0x21e0031c)                                                      */
/*      SPRB0 (0x21e0031c)                                                      */
/*      SPRHBM (0x21e0031c)                                                     */
/*      SPRC0 (0x21e0031c)                                                      */
/*      SPRMCC (0x21e0031c)                                                     */
/*      SPRUCC (0x21e0031c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRB0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRHBM Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRC0 Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0         */
/* SPRMCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* SPRUCC Register File:    sprsp_top/ubox/ncevents/ncevents/CFG_UBOX_F0        */
/* Struct generated from SPRA0 BDF: 30_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Platform QoS Memory BW Enforcement Delay
*/


#define NCEVENTS_CFG_3DXP_MBE_DELAY_1_N1_UBOX_CFG_REG 0x1302031C

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

} NCEVENTS_CFG_3DXP_MBE_DELAY_1_N1_UBOX_CFG_STRUCT;
#endif /* _UBOX_CFG_h */
