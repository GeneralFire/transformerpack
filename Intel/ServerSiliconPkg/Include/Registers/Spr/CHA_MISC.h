
/** @file
  CHA_MISC.h

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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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
/* BOOT_W                                                                       */
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
/* LB_MCHECK_CHA_W                                                              */
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


#ifndef _CHA_MISC_h
#define _CHA_MISC_h
#include <Base.h>

/* VID_UNICAST_GROUP0_CHA_MISC_REG supported on:                                */
/*      SPRA0 (0x11f50000)                                                      */
/*      SPRB0 (0x11f50000)                                                      */
/*      SPRHBM (0x11f50000)                                                     */
/*      SPRC0 (0x11f50000)                                                      */
/*      SPRMCC (0x11f50000)                                                     */
/*      SPRUCC (0x11f50000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_UNICAST_GROUP0_CHA_MISC_REG 0x00010000

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

} VID_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DID_UNICAST_GROUP0_CHA_MISC_REG supported on:                                */
/*      SPRA0 (0x11f50002)                                                      */
/*      SPRB0 (0x11f50002)                                                      */
/*      SPRHBM (0x11f50002)                                                     */
/*      SPRC0 (0x11f50002)                                                      */
/*      SPRMCC (0x11f50002)                                                     */
/*      SPRUCC (0x11f50002)                                                     */
/* Register default value on SPRA0: 0x0000324D                                  */
/* Register default value on SPRB0: 0x0000324D                                  */
/* Register default value on SPRHBM: 0x0000324D                                 */
/* Register default value on SPRC0: 0x0000324D                                  */
/* Register default value on SPRMCC: 0x0000324D                                 */
/* Register default value on SPRUCC: 0x0000324D                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_UNICAST_GROUP0_CHA_MISC_REG 0x00010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000324D*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PCICMD_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x11f50004)                                                      */
/*      SPRB0 (0x11f50004)                                                      */
/*      SPRHBM (0x11f50004)                                                     */
/*      SPRC0 (0x11f50004)                                                      */
/*      SPRMCC (0x11f50004)                                                     */
/*      SPRUCC (0x11f50004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_UNICAST_GROUP0_CHA_MISC_REG 0x00010004

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

} PCICMD_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PCISTS_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x11f50006)                                                      */
/*      SPRB0 (0x11f50006)                                                      */
/*      SPRHBM (0x11f50006)                                                     */
/*      SPRC0 (0x11f50006)                                                      */
/*      SPRMCC (0x11f50006)                                                     */
/*      SPRUCC (0x11f50006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_UNICAST_GROUP0_CHA_MISC_REG 0x00010006

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

} PCISTS_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* RID_CCR_UNICAST_GROUP0_CHA_MISC_REG supported on:                            */
/*      SPRA0 (0x21f50008)                                                      */
/*      SPRB0 (0x21f50008)                                                      */
/*      SPRHBM (0x21f50008)                                                     */
/*      SPRC0 (0x21f50008)                                                      */
/*      SPRMCC (0x21f50008)                                                     */
/*      SPRUCC (0x21f50008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_UNICAST_GROUP0_CHA_MISC_REG 0x00020008

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

} RID_CCR_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* CLSR_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x1f5000c)                                                       */
/*      SPRB0 (0x1f5000c)                                                       */
/*      SPRHBM (0x1f5000c)                                                      */
/*      SPRC0 (0x1f5000c)                                                       */
/*      SPRMCC (0x1f5000c)                                                      */
/*      SPRUCC (0x1f5000c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_UNICAST_GROUP0_CHA_MISC_REG 0x0000000C

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

} CLSR_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PLAT_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x1f5000d)                                                       */
/*      SPRB0 (0x1f5000d)                                                       */
/*      SPRHBM (0x1f5000d)                                                      */
/*      SPRC0 (0x1f5000d)                                                       */
/*      SPRMCC (0x1f5000d)                                                      */
/*      SPRUCC (0x1f5000d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_UNICAST_GROUP0_CHA_MISC_REG 0x0000000D

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

} PLAT_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* HDR_UNICAST_GROUP0_CHA_MISC_REG supported on:                                */
/*      SPRA0 (0x1f5000e)                                                       */
/*      SPRB0 (0x1f5000e)                                                       */
/*      SPRHBM (0x1f5000e)                                                      */
/*      SPRC0 (0x1f5000e)                                                       */
/*      SPRMCC (0x1f5000e)                                                      */
/*      SPRUCC (0x1f5000e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_UNICAST_GROUP0_CHA_MISC_REG 0x0000000E

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

} HDR_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* BIST_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x1f5000f)                                                       */
/*      SPRB0 (0x1f5000f)                                                       */
/*      SPRHBM (0x1f5000f)                                                      */
/*      SPRC0 (0x1f5000f)                                                       */
/*      SPRMCC (0x1f5000f)                                                      */
/*      SPRUCC (0x1f5000f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_UNICAST_GROUP0_CHA_MISC_REG 0x0000000F

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

} BIST_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* SVID_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x11f5002c)                                                      */
/*      SPRB0 (0x11f5002c)                                                      */
/*      SPRHBM (0x11f5002c)                                                     */
/*      SPRC0 (0x11f5002c)                                                      */
/*      SPRMCC (0x11f5002c)                                                     */
/*      SPRUCC (0x11f5002c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_UNICAST_GROUP0_CHA_MISC_REG 0x0001002C

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

} SVID_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* SDID_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x11f5002e)                                                      */
/*      SPRB0 (0x11f5002e)                                                      */
/*      SPRHBM (0x11f5002e)                                                     */
/*      SPRC0 (0x11f5002e)                                                      */
/*      SPRMCC (0x11f5002e)                                                     */
/*      SPRUCC (0x11f5002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_UNICAST_GROUP0_CHA_MISC_REG 0x0001002E

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

} SDID_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* CAPPTR_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x1f50034)                                                       */
/*      SPRB0 (0x1f50034)                                                       */
/*      SPRHBM (0x1f50034)                                                      */
/*      SPRC0 (0x1f50034)                                                       */
/*      SPRMCC (0x1f50034)                                                      */
/*      SPRUCC (0x1f50034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_UNICAST_GROUP0_CHA_MISC_REG 0x00000034

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

} CAPPTR_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* INTL_UNICAST_GROUP0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x1f5003c)                                                       */
/*      SPRB0 (0x1f5003c)                                                       */
/*      SPRHBM (0x1f5003c)                                                      */
/*      SPRC0 (0x1f5003c)                                                       */
/*      SPRMCC (0x1f5003c)                                                      */
/*      SPRUCC (0x1f5003c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_UNICAST_GROUP0_CHA_MISC_REG 0x0000003C

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

} INTL_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* INTPIN_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x1f5003d)                                                       */
/*      SPRB0 (0x1f5003d)                                                       */
/*      SPRHBM (0x1f5003d)                                                      */
/*      SPRC0 (0x1f5003d)                                                       */
/*      SPRMCC (0x1f5003d)                                                      */
/*      SPRUCC (0x1f5003d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_UNICAST_GROUP0_CHA_MISC_REG 0x0000003D

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

} INTPIN_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* MINGNT_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x1f5003e)                                                       */
/*      SPRB0 (0x1f5003e)                                                       */
/*      SPRHBM (0x1f5003e)                                                      */
/*      SPRC0 (0x1f5003e)                                                       */
/*      SPRMCC (0x1f5003e)                                                      */
/*      SPRUCC (0x1f5003e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_UNICAST_GROUP0_CHA_MISC_REG 0x0000003E

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

} MINGNT_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* MAXLAT_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x1f5003f)                                                       */
/*      SPRB0 (0x1f5003f)                                                       */
/*      SPRHBM (0x1f5003f)                                                      */
/*      SPRC0 (0x1f5003f)                                                       */
/*      SPRMCC (0x1f5003f)                                                      */
/*      SPRUCC (0x1f5003f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_UNICAST_GROUP0_CHA_MISC_REG 0x0000003F

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

} MAXLAT_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PXPCAP_UNICAST_GROUP0_CHA_MISC_REG supported on:                             */
/*      SPRA0 (0x21f50040)                                                      */
/*      SPRB0 (0x21f50040)                                                      */
/*      SPRHBM (0x21f50040)                                                     */
/*      SPRC0 (0x21f50040)                                                      */
/*      SPRMCC (0x21f50040)                                                     */
/*      SPRUCC (0x21f50040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_UNICAST_GROUP0_CHA_MISC_REG 0x00020040

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

} PXPCAP_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PCIE_RSVD0_UNICAST_GROUP0_CHA_MISC_REG supported on:                         */
/*      SPRA0 (0x21f50044)                                                      */
/*      SPRB0 (0x21f50044)                                                      */
/*      SPRHBM (0x21f50044)                                                     */
/*      SPRC0 (0x21f50044)                                                      */
/*      SPRMCC (0x21f50044)                                                     */
/*      SPRUCC (0x21f50044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_UNICAST_GROUP0_CHA_MISC_REG 0x00020044

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

} PCIE_RSVD0_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* PCIE_RSVD1_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50048)                                                      */
/*      SPRB0 (0x21f50048)                                                      */
/*      SPRHBM (0x21f50048)                                                     */
/*      SPRC0 (0x21f50048)                                                      */
/*      SPRMCC (0x21f50048)                                                     */
/*      SPRUCC (0x21f50048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020048

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

} PCIE_RSVD1_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD1_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f5004c)                                                      */
/*      SPRB0 (0x21f5004c)                                                      */
/*      SPRHBM (0x21f5004c)                                                     */
/*      SPRC0 (0x21f5004c)                                                      */
/*      SPRMCC (0x21f5004c)                                                     */
/*      SPRUCC (0x21f5004c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_UNICAST_GROUP0_N1_CHA_MISC_REG 0x0002004C

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

} PCIE_RSVD1_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD2_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50050)                                                      */
/*      SPRB0 (0x21f50050)                                                      */
/*      SPRHBM (0x21f50050)                                                     */
/*      SPRC0 (0x21f50050)                                                      */
/*      SPRMCC (0x21f50050)                                                     */
/*      SPRUCC (0x21f50050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020050

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

} PCIE_RSVD2_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD2_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50054)                                                      */
/*      SPRB0 (0x21f50054)                                                      */
/*      SPRHBM (0x21f50054)                                                     */
/*      SPRC0 (0x21f50054)                                                      */
/*      SPRMCC (0x21f50054)                                                     */
/*      SPRUCC (0x21f50054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_UNICAST_GROUP0_N1_CHA_MISC_REG 0x00020054

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

} PCIE_RSVD2_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD3_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50058)                                                      */
/*      SPRB0 (0x21f50058)                                                      */
/*      SPRHBM (0x21f50058)                                                     */
/*      SPRC0 (0x21f50058)                                                      */
/*      SPRMCC (0x21f50058)                                                     */
/*      SPRUCC (0x21f50058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020058

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

} PCIE_RSVD3_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD3_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f5005c)                                                      */
/*      SPRB0 (0x21f5005c)                                                      */
/*      SPRHBM (0x21f5005c)                                                     */
/*      SPRC0 (0x21f5005c)                                                      */
/*      SPRMCC (0x21f5005c)                                                     */
/*      SPRUCC (0x21f5005c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_UNICAST_GROUP0_N1_CHA_MISC_REG 0x0002005C

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

} PCIE_RSVD3_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD4_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50060)                                                      */
/*      SPRB0 (0x21f50060)                                                      */
/*      SPRHBM (0x21f50060)                                                     */
/*      SPRC0 (0x21f50060)                                                      */
/*      SPRMCC (0x21f50060)                                                     */
/*      SPRUCC (0x21f50060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020060

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

} PCIE_RSVD4_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD4_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50064)                                                      */
/*      SPRB0 (0x21f50064)                                                      */
/*      SPRHBM (0x21f50064)                                                     */
/*      SPRC0 (0x21f50064)                                                      */
/*      SPRMCC (0x21f50064)                                                     */
/*      SPRUCC (0x21f50064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_UNICAST_GROUP0_N1_CHA_MISC_REG 0x00020064

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

} PCIE_RSVD4_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD5_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50068)                                                      */
/*      SPRB0 (0x21f50068)                                                      */
/*      SPRHBM (0x21f50068)                                                     */
/*      SPRC0 (0x21f50068)                                                      */
/*      SPRMCC (0x21f50068)                                                     */
/*      SPRUCC (0x21f50068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020068

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

} PCIE_RSVD5_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD5_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f5006c)                                                      */
/*      SPRB0 (0x21f5006c)                                                      */
/*      SPRHBM (0x21f5006c)                                                     */
/*      SPRC0 (0x21f5006c)                                                      */
/*      SPRMCC (0x21f5006c)                                                     */
/*      SPRUCC (0x21f5006c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_UNICAST_GROUP0_N1_CHA_MISC_REG 0x0002006C

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

} PCIE_RSVD5_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD6_UNICAST_GROUP0_N0_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50070)                                                      */
/*      SPRB0 (0x21f50070)                                                      */
/*      SPRHBM (0x21f50070)                                                     */
/*      SPRC0 (0x21f50070)                                                      */
/*      SPRMCC (0x21f50070)                                                     */
/*      SPRUCC (0x21f50070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_UNICAST_GROUP0_N0_CHA_MISC_REG 0x00020070

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

} PCIE_RSVD6_UNICAST_GROUP0_N0_CHA_MISC_STRUCT;

/* PCIE_RSVD6_UNICAST_GROUP0_N1_CHA_MISC_REG supported on:                      */
/*      SPRA0 (0x21f50074)                                                      */
/*      SPRB0 (0x21f50074)                                                      */
/*      SPRHBM (0x21f50074)                                                     */
/*      SPRC0 (0x21f50074)                                                      */
/*      SPRMCC (0x21f50074)                                                     */
/*      SPRUCC (0x21f50074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_UNICAST_GROUP0_N1_CHA_MISC_REG 0x00020074

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

} PCIE_RSVD6_UNICAST_GROUP0_N1_CHA_MISC_STRUCT;

/* PCIE_RSVD7_UNICAST_GROUP0_CHA_MISC_REG supported on:                         */
/*      SPRA0 (0x21f50078)                                                      */
/*      SPRB0 (0x21f50078)                                                      */
/*      SPRHBM (0x21f50078)                                                     */
/*      SPRC0 (0x21f50078)                                                      */
/*      SPRMCC (0x21f50078)                                                     */
/*      SPRUCC (0x21f50078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_UNICAST_GROUP0_CHA_MISC_REG 0x00020078

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

} PCIE_RSVD7_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* SADDBGMM0_CHA_MISC_REG supported on:                                         */
/*      SPRA0 (0x21f500a0)                                                      */
/*      SPRB0 (0x21f500a0)                                                      */
/*      SPRHBM (0x21f500a0)                                                     */
/*      SPRC0 (0x21f500a0)                                                      */
/*      SPRMCC (0x21f500a0)                                                     */
/*      SPRUCC (0x21f500a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SAD debug MM0
*/


#define SADDBGMM0_CHA_MISC_REG 0x000200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_37_6 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Mask field for address[37:6] */

  } Bits;
  UINT32 Data;

} SADDBGMM0_CHA_MISC_STRUCT;

/* SADDBGMM1_CHA_MISC_REG supported on:                                         */
/*      SPRA0 (0x21f500a4)                                                      */
/*      SPRB0 (0x21f500a4)                                                      */
/*      SPRHBM (0x21f500a4)                                                     */
/*      SPRC0 (0x21f500a4)                                                      */
/*      SPRMCC (0x21f500a4)                                                     */
/*      SPRUCC (0x21f500a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SAD debug MM1
*/


#define SADDBGMM1_CHA_MISC_REG 0x000200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_51_38 : 14;

                            /* Bits[13:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask field for address[51:38]. Mask bits that
                               are 1 correspond to address bits that are not
                               matched.
                            */
    UINT32 match_23_6 : 18;

                            /* Bits[31:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match field for address[23:6]. If bit i in the
                               mask is 1, the corresponding bit in the match
                               field should be 0.
                            */

  } Bits;
  UINT32 Data;

} SADDBGMM1_CHA_MISC_STRUCT;

/* SADDBGMM2_CHA_MISC_REG supported on:                                         */
/*      SPRA0 (0x21f500a8)                                                      */
/*      SPRB0 (0x21f500a8)                                                      */
/*      SPRHBM (0x21f500a8)                                                     */
/*      SPRC0 (0x21f500a8)                                                      */
/*      SPRMCC (0x21f500a8)                                                     */
/*      SPRUCC (0x21f500a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* SAD debug MM2
*/


#define SADDBGMM2_CHA_MISC_REG 0x000200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_51_24 : 28;

                            /* Bits[27:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match field for address[51:24]. If bit i in the
                               mask is 1, the corresponding bit in the match
                               field should be 0.
                            */
    UINT32 errinjaddrvalid : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set to 1 for CLFLUSH address match along with PA
                               Addr match on ERR Injection.
                            */
    UINT32 errinjopcodevalid : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set to 1 for CLFLUSH opcode match along with PA
                               Addr match on ERR Injection.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SADDBGMM2_CHA_MISC_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* FZM_CAPABILITY_UNICAST_GROUP0_CHA_MISC_REG supported on:                     */
/*      SPRB0 (0x21f500b0)                                                      */
/*      SPRHBM (0x21f500b0)                                                     */
/*      SPRC0 (0x21f500b0)                                                      */
/*      SPRMCC (0x21f500b0)                                                     */
/*      SPRUCC (0x21f500b0)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRB0 BDF: 31_10_0                                     */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This register effectively implements a scratchpad for BIOS to record which memory region, as defined by the general purpose DRAM rules, is fast-zero capable.
*/


#define FZM_CAPABILITY_UNICAST_GROUP0_CHA_MISC_REG 0x000200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_vector : 16;

                            /* Bits[15:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               BIOS may use this field, one bit per general-
                               purpose DRAM rule, to record which memory
                               regions are fast-zero capable. (Hardware does
                               not read or write this register field.)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FZM_CAPABILITY_UNICAST_GROUP0_CHA_MISC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */


/* PXPENHCAP_UNICAST_GROUP0_CHA_MISC_REG supported on:                          */
/*      SPRA0 (0x21f50100)                                                      */
/*      SPRB0 (0x21f50100)                                                      */
/*      SPRHBM (0x21f50100)                                                     */
/*      SPRC0 (0x21f50100)                                                      */
/*      SPRMCC (0x21f50100)                                                     */
/*      SPRUCC (0x21f50100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_UNICAST_GROUP0_CHA_MISC_REG 0x00020100

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

} PXPENHCAP_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* HA_AD_CREDITS_CHA_MISC_REG supported on:                                     */
/*      SPRA0 (0x21f50108)                                                      */
/*      SPRB0 (0x21f50108)                                                      */
/*      SPRHBM (0x21f50108)                                                     */
/*      SPRC0 (0x21f50108)                                                      */
/*      SPRMCC (0x21f50108)                                                     */
/*      SPRUCC (0x21f50108)                                                     */
/* Register default value on SPRA0: 0x00111111                                  */
/* Register default value on SPRB0: 0x00111111                                  */
/* Register default value on SPRHBM: 0x00111111                                 */
/* Register default value on SPRC0: 0x00111111                                  */
/* Register default value on SPRMCC: 0x00111111                                 */
/* Register default value on SPRUCC: 0x00111111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register contains HA AD Credit info.
*/


#define HA_AD_CREDITS_CHA_MISC_REG 0x00020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_vna_port0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT0. MC0. */
    UINT32 ad_vna_port1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT1. MC1. */
    UINT32 ad_vna_port2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT2. MC2. */
    UINT32 ad_vna_port3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT3. MC3. */
    UINT32 ad_vna_port4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT4. MC4. */
    UINT32 ad_vna_port5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT5. MC5. */
    UINT32 disadvnaisocres : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable AD VNA Isoc credit reservation. One AD
                               VNA credit is reserved for ISOC in ISOC enabled
                               configurations if this bit is not set.
                            */
    UINT32 rsvd_25 : 4;

                            /* Bits[28:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_AD_CREDITS_CHA_MISC_STRUCT;

/* HA_AD_CREDITS1_CHA_MISC_REG supported on:                                    */
/*      SPRA0 (0x21f5010c)                                                      */
/*      SPRB0 (0x21f5010c)                                                      */
/*      SPRHBM (0x21f5010c)                                                     */
/*      SPRC0 (0x21f5010c)                                                      */
/*      SPRMCC (0x21f5010c)                                                     */
/*      SPRUCC (0x21f5010c)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register contains HA AD Credit info.
*/


#define HA_AD_CREDITS1_CHA_MISC_REG 0x0002010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_vna_port6 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT6. MC6. */
    UINT32 ad_vna_port7 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT7. MC7. */
    UINT32 ad_vna_port8 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT8. MC8. */
    UINT32 ad_vna_port9 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT9. MC9. */
    UINT32 ad_vna_port10 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT10. MC10. */
    UINT32 ad_vna_port11 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT11. MC11. */
    UINT32 ad_vna_port12 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT12. MC12. */
    UINT32 ad_vna_port13 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT13. MC13. */

  } Bits;
  UINT32 Data;

} HA_AD_CREDITS1_CHA_MISC_STRUCT;

/* HA_BL_CREDITS_CHA_MISC_REG supported on:                                     */
/*      SPRA0 (0x21f50110)                                                      */
/*      SPRB0 (0x21f50110)                                                      */
/*      SPRHBM (0x21f50110)                                                     */
/*      SPRC0 (0x21f50110)                                                      */
/*      SPRMCC (0x21f50110)                                                     */
/*      SPRUCC (0x21f50110)                                                     */
/* Register default value on SPRA0: 0x00222222                                  */
/* Register default value on SPRB0: 0x00222222                                  */
/* Register default value on SPRHBM: 0x00222222                                 */
/* Register default value on SPRC0: 0x00222222                                  */
/* Register default value on SPRMCC: 0x00222222                                 */
/* Register default value on SPRUCC: 0x00222222                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register contains HA AD Credit info.
*/


#define HA_BL_CREDITS_CHA_MISC_REG 0x00020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT0. MC0. */
    UINT32 bl_vna_port1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT1. MC1. */
    UINT32 bl_vna_port2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT2. MC2 */
    UINT32 bl_vna_port3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT3. MC3 */
    UINT32 bl_vna_port4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT4. MC4 */
    UINT32 bl_vna_port5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT5. MC5 */
    UINT32 disblvnaisocres : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Disable BL VNA Isoc credit reservation */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_BL_CREDITS_CHA_MISC_STRUCT;

/* HA_BL_CREDITS1_CHA_MISC_REG supported on:                                    */
/*      SPRA0 (0x21f50114)                                                      */
/*      SPRB0 (0x21f50114)                                                      */
/*      SPRHBM (0x21f50114)                                                     */
/*      SPRC0 (0x21f50114)                                                      */
/*      SPRMCC (0x21f50114)                                                     */
/*      SPRUCC (0x21f50114)                                                     */
/* Register default value on SPRA0: 0x22222222                                  */
/* Register default value on SPRB0: 0x22222222                                  */
/* Register default value on SPRHBM: 0x22222222                                 */
/* Register default value on SPRC0: 0x22222222                                  */
/* Register default value on SPRMCC: 0x22222222                                 */
/* Register default value on SPRUCC: 0x22222222                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register contains HA AD Credit info.
*/


#define HA_BL_CREDITS1_CHA_MISC_REG 0x00020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port6 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT6. MC6. */
    UINT32 bl_vna_port7 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT7. MC7. */
    UINT32 bl_vna_port8 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT8. MC8. */
    UINT32 bl_vna_port9 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT9. MC9. */
    UINT32 bl_vna_port10 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT10. MC10. */
    UINT32 bl_vna_port11 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT11. MC11. */
    UINT32 bl_vna_port12 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT12. MC12. */
    UINT32 bl_vna_port13 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT13. MC13. */

  } Bits;
  UINT32 Data;

} HA_BL_CREDITS1_CHA_MISC_STRUCT;

/* LLCERRINJ_CHA_MISC_REG supported on:                                         */
/*      SPRA0 (0x21f50118)                                                      */
/*      SPRB0 (0x21f50118)                                                      */
/*      SPRHBM (0x21f50118)                                                     */
/*      SPRC0 (0x21f50118)                                                      */
/*      SPRMCC (0x21f50118)                                                     */
/*      SPRUCC (0x21f50118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This CSR is for read-side DATA Array Error Injection
*/


#define LLCERRINJ_CHA_MISC_REG 0x00020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inj3bdataerr : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This is used to flip the triple bit data error
                               indication.
                            */
    UINT32 errinjmode : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               if set to 1, it will be once-only injection
                               mode.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LLCERRINJ_CHA_MISC_STRUCT;

#ifdef SPRA0_HOST
/* MLCTRACE_PTR_N0_CHA_MISC_REG supported on:                                   */
/*      SPRA0 (0x21f50120)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* This register contains MLC trace pointer config required in eADR flow
*/


#define MLCTRACE_PTR_N0_CHA_MISC_REG 0x00020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pa_stolen_mem : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Starting address pointer (PA[51:6]) of the
                               stolen memory allocated for this slice of CHA
                               LLC to store MLC content
                            */

  } Bits;
  UINT32 Data;

} MLCTRACE_PTR_N0_CHA_MISC_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MLCTRACE_PTR_N1_CHA_MISC_REG supported on:                                   */
/*      SPRA0 (0x21f50124)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* This register contains MLC trace pointer config required in eADR flow
*/


#define MLCTRACE_PTR_N1_CHA_MISC_REG 0x00020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pa_stolen_mem : 14;

                            /* Bits[13:0], Access Type=RW, default=0x00000000*/

                            /*
                               Starting address pointer (PA[51:6]) of the
                               stolen memory allocated for this slice of CHA
                               LLC to store MLC content
                            */
    UINT32 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mesh_stop_id : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* MC Mesh Stop ID at which the pointer resides */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 channel_id : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               channel ID of the MC channel at which the
                               pointer resides
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MLCTRACE_PTR_N1_CHA_MISC_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* LLCTRACE_PTR_N0_CHA_MISC_REG supported on:                                   */
/*      SPRA0 (0x21f50128)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* This register contains LLC trace pointer config required in eADR flow
*/


#define LLCTRACE_PTR_N0_CHA_MISC_REG 0x00020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pa_stolen_mem : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Starting address pointer (PA[51:6]) of the
                               stolen memory allocated for this slice of CHA
                               LLC to store LLC content
                            */

  } Bits;
  UINT32 Data;

} LLCTRACE_PTR_N0_CHA_MISC_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* LLCTRACE_PTR_N1_CHA_MISC_REG supported on:                                   */
/*      SPRA0 (0x21f5012c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* This register contains LLC trace pointer config required in eADR flow
*/


#define LLCTRACE_PTR_N1_CHA_MISC_REG 0x0002012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pa_stolen_mem : 14;

                            /* Bits[13:0], Access Type=RW, default=0x00000000*/

                            /*
                               Starting address pointer (PA[51:6]) of the
                               stolen memory allocated for this slice of CHA
                               LLC to store LLC content
                            */
    UINT32 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mesh_stop_id : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* MC Mesh Stop ID at which the pointer resides */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 channel_id : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               channel ID of the MC channel at which the
                               pointer resides
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LLCTRACE_PTR_N1_CHA_MISC_STRUCT;
#endif /* (SPRA0_HOST) */

/* FZM_BITS_CHA_MISC_REG supported on:                                          */
/*      SPRA0 (0x21f50130)                                                      */
/*      SPRB0 (0x21f50130)                                                      */
/*      SPRHBM (0x21f50130)                                                     */
/*      SPRC0 (0x21f50130)                                                      */
/*      SPRMCC (0x21f50130)                                                     */
/*      SPRUCC (0x21f50130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* bit vector with two fields. each has 1 bit per DRAM rule for tracking if each DRAM rule has ever observed a RD or a WR. The first field tracks the reads. The second field tracks the writes.
every UPI and IDI access that hits in one of the DRAM_RULE_N registers must increment one of the other of these two bits.
*/


#define FZM_BITS_CHA_MISC_REG 0x00020130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wr_bit_vector : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               one bit per DRAM rule. These bits map to the 16
                               DRAM_RULE_N registers. When the bit is 0, it
                               means this DRAM rule has not yet seen any write
                               accesses. When the bit is 1, it means this DRAM
                               rule has seen a write access. This field is read
                               only, and can be updated only internally by the
                               CHA when a write access with an address that
                               matches the corresponding DRAM rule allocates a
                               TOR entry.
                            */
    UINT32 rd_bit_vector : 16;

                            /* Bits[31:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               one bit per DRAM rule. These bits map to the 16
                               DRAM_RULE_N registers. When the bit is 0, it
                               means this DRAM rule has not yet seen any read
                               accesses. When the bit is 1, it means this DRAM
                               rule has seen a read access. This field is read
                               only, and can be updated only internally by the
                               CHA when a read access with an address that
                               matches the corresponding DRAM rule allocates a
                               TOR entry.
                            */

  } Bits;
  UINT32 Data;

} FZM_BITS_CHA_MISC_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG supported on:                           */
/*      SPRB0 (0x21f50134)                                                      */
/*      SPRHBM (0x21f50134)                                                     */
/*      SPRC0 (0x21f50134)                                                      */
/*      SPRMCC (0x21f50134)                                                     */
/*      SPRUCC (0x21f50134)                                                     */
/* Register default value on SPRB0: 0x0000FFF7                                  */
/* Register default value on SPRHBM: 0x0000FFF7                                 */
/* Register default value on SPRC0: 0x0001FFF7                                  */
/* Register default value on SPRMCC: 0x0001FFF7                                 */
/* Register default value on SPRUCC: 0x0001FFF7                                 */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRB0 BDF: 31_10_0                                     */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register controls the critical entry priority feature for CHA ingress queues. A transaction from an ingress queue slot may win entry into the CHA processing pipeline, but the pipeline may need to reject the transaction. In a worst-case scenario, a transaction may get rejected multiple times in a row, such that a younger transaction makes it through the pipeline first, causing the older, previously rejected transaction to get rejected even more often and thereby expereince excessive processing latency. The feature controlled by this register tracks how often a transaction gets rejected, and if the rejection count exceeds a threshold, the transaction is then considered critical. Critical transactions then get priority access to ensure that older/critical transactions make more rapid progress compared to younger ones.
*/


#define INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG 0x00020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 prq_critical_threshold : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000007*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by posted request queue
                               (PRQ) entries. Rejections are tracked per queue
                               entry, and once the rejection count for an entry
                               exceeds the threshold, the entry is considered
                               critical and will be given priority access to
                               the pipeline. Non-critical entries are
                               reconsidered only once no critical entries
                               remain. Setting this field to its maximum value,
                               all bits 1, effectively disables the critical
                               entry priority feature for PRQ.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ismq_critical_threshold : 4;

                            /* Bits[7:4], Access Type=RW, default=0x0000000F*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by ingress subsequent
                               message queue (ISMQ) entries. Rejections are
                               tracked per queue entry, and once the rejection
                               count for an entry exceeds the threshold, the
                               entry is considered critical and will be given
                               priority access to the pipeline. Non-critical
                               entries are reconsidered only once no critical
                               entries remain, unless a deadlock risk is
                               detected. (See also
                               ISMQ_Deadlock_Avoid_Threshold field.) Setting
                               this field to its maximum value, all bits 1,
                               effectively disables the critical entry priority
                               feature for ISMQ.)
                            */
    UINT32 ismq_deadlock_avoid_threshold : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /*
                               Interdependencies may exist between entries of
                               the ingress subsequent message queue (ISMQ). As
                               a result, if only critical ISMQ entries are
                               allowed access to the pipeline, such a critical
                               entry may not make progress if it depends on a
                               non-critical entry. To avoid deadlocks in such
                               situations, if ISMQ critical-entry processing
                               experiences more consecutive rejections than
                               defined by this threshold field, a deadlock
                               avoidance mechanism is activated that gives non-
                               critical entries temporary access to the
                               pipeline. Setting this field to its maximum
                               value, all bits 1, effectively disables the
                               deadlock avoidance feature.
                            */
    UINT32 rsvd_16 : 12;

                            /* Bits[27:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 prq_disable_on_starve : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the PRQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 prq_disable_unconditional : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for PRQ to become deactivated
                               regardless of other conditions.
                            */
    UINT32 ismq_disable_on_starve : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the ISMQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 ismq_disable_unconditional : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for ISMQ to become
                               deactivated regardless of other conditions.
                            */

  } Bits;
  UINT32 Data;

} INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG supported on:                           */
/*      SPRB0 (0x21f50134)                                                      */
/*      SPRHBM (0x21f50134)                                                     */
/*      SPRC0 (0x21f50134)                                                      */
/*      SPRMCC (0x21f50134)                                                     */
/*      SPRUCC (0x21f50134)                                                     */
/* Register default value on SPRB0: 0x0000FFF7                                  */
/* Register default value on SPRHBM: 0x0000FFF7                                 */
/* Register default value on SPRC0: 0x0001FFF7                                  */
/* Register default value on SPRMCC: 0x0001FFF7                                 */
/* Register default value on SPRUCC: 0x0001FFF7                                 */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRB0 BDF: 31_10_0                                     */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register controls the critical entry priority feature for CHA ingress queues. A transaction from an ingress queue slot may win entry into the CHA processing pipeline, but the pipeline may need to reject the transaction. In a worst-case scenario, a transaction may get rejected multiple times in a row, such that a younger transaction makes it through the pipeline first, causing the older, previously rejected transaction to get rejected even more often and thereby expereince excessive processing latency. The feature controlled by this register tracks how often a transaction gets rejected, and if the rejection count exceeds a threshold, the transaction is then considered critical. Critical transactions then get priority access to ensure that older/critical transactions make more rapid progress compared to younger ones.
*/


#define INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG 0x00020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 prq_critical_threshold : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000007*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by posted request queue
                               (PRQ) entries. Rejections are tracked per queue
                               entry, and once the rejection count for an entry
                               exceeds the threshold, the entry is considered
                               critical and will be given priority access to
                               the pipeline. Non-critical entries are
                               reconsidered only once no critical entries
                               remain. Setting this field to its maximum value,
                               all bits 1, effectively disables the critical
                               entry priority feature for PRQ.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ismq_critical_threshold : 4;

                            /* Bits[7:4], Access Type=RW, default=0x0000000F*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by ingress subsequent
                               message queue (ISMQ) entries. Rejections are
                               tracked per queue entry, and once the rejection
                               count for an entry exceeds the threshold, the
                               entry is considered critical and will be given
                               priority access to the pipeline. Non-critical
                               entries are reconsidered only once no critical
                               entries remain, unless a deadlock risk is
                               detected. (See also
                               ISMQ_Deadlock_Avoid_Threshold field.) Setting
                               this field to its maximum value, all bits 1,
                               effectively disables the critical entry priority
                               feature for ISMQ.)
                            */
    UINT32 ismq_deadlock_avoid_threshold : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /*
                               Interdependencies may exist between entries of
                               the ingress subsequent message queue (ISMQ). As
                               a result, if only critical ISMQ entries are
                               allowed access to the pipeline, such a critical
                               entry may not make progress if it depends on a
                               non-critical entry. To avoid deadlocks in such
                               situations, if ISMQ critical-entry processing
                               experiences more consecutive rejections than
                               defined by this threshold field, a deadlock
                               avoidance mechanism is activated that gives non-
                               critical entries temporary access to the
                               pipeline. Setting this field to its maximum
                               value, all bits 1, effectively disables the
                               deadlock avoidance feature.
                            */
    UINT32 ismq_deadlock_avoid_blocks_ingress : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               When the ISMQ critical priority logic enters
                               deadlock avoidance mode, such that even non-
                               critical ISMQ entries are temporarily admitted
                               to the pipeline, a situation may arise where an
                               ISMQ entry that had a dependency into a critical
                               entry succcessfully completes but is then
                               immediatley replaced by a new entry from another
                               ingress queue (e.g. IRQ) that then also causes a
                               dependency into the critical entry. As a result,
                               the critical entry still cannot make progress
                               event after the deadlock avoidance action. To
                               help increase the effectiveness of the deadlock
                               avoidance pass, that is to avoid the need for
                               multiple deadlock avoidance passes to get one
                               stuck critical entry unstuck, new traffic from
                               the non-ISMQ ingress queues may be blocked
                               during deadlock avoidance. With this bit set to
                               1, the other ingress queues are so blocked. With
                               this bit at 0, other queues may inject new
                               messages into the pipeline during ISMQ deadlock
                               avoidance.
                            */
    UINT32 rsvd_17 : 11;

                            /* Bits[27:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 prq_disable_on_starve : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the PRQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 prq_disable_unconditional : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for PRQ to become deactivated
                               regardless of other conditions.
                            */
    UINT32 ismq_disable_on_starve : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the ISMQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 ismq_disable_unconditional : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for ISMQ to become
                               deactivated regardless of other conditions.
                            */

  } Bits;
  UINT32 Data;

} INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG supported on:                           */
/*      SPRB0 (0x21f50134)                                                      */
/*      SPRHBM (0x21f50134)                                                     */
/*      SPRC0 (0x21f50134)                                                      */
/*      SPRMCC (0x21f50134)                                                     */
/*      SPRUCC (0x21f50134)                                                     */
/* Register default value on SPRB0: 0x0000FFF7                                  */
/* Register default value on SPRHBM: 0x0000FFF7                                 */
/* Register default value on SPRC0: 0x0001FFF7                                  */
/* Register default value on SPRMCC: 0x0001FFF7                                 */
/* Register default value on SPRUCC: 0x0001FFF7                                 */
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRB0 BDF: 31_10_0                                     */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register controls the critical entry priority feature for CHA ingress queues. A transaction from an ingress queue slot may win entry into the CHA processing pipeline, but the pipeline may need to reject the transaction. In a worst-case scenario, a transaction may get rejected multiple times in a row, such that a younger transaction makes it through the pipeline first, causing the older, previously rejected transaction to get rejected even more often and thereby expereince excessive processing latency. The feature controlled by this register tracks how often a transaction gets rejected, and if the rejection count exceeds a threshold, the transaction is then considered critical. Critical transactions then get priority access to ensure that older/critical transactions make more rapid progress compared to younger ones.
*/


#define INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_REG 0x00020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 prq_critical_threshold : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000007*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by posted request queue
                               (PRQ) entries. Rejections are tracked per queue
                               entry, and once the rejection count for an entry
                               exceeds the threshold, the entry is considered
                               critical and will be given priority access to
                               the pipeline. Non-critical entries are
                               reconsidered only once no critical entries
                               remain. Setting this field to its maximum value,
                               all bits 1, effectively disables the critical
                               entry priority feature for PRQ.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ismq_critical_threshold : 4;

                            /* Bits[7:4], Access Type=RW, default=0x0000000F*/

                            /*
                               Threshold for number of consecutive pipeline
                               rejections experienced by ingress subsequent
                               message queue (ISMQ) entries. Rejections are
                               tracked per queue entry, and once the rejection
                               count for an entry exceeds the threshold, the
                               entry is considered critical and will be given
                               priority access to the pipeline. Non-critical
                               entries are reconsidered only once no critical
                               entries remain, unless a deadlock risk is
                               detected. (See also
                               ISMQ_Deadlock_Avoid_Threshold field.) Setting
                               this field to its maximum value, all bits 1,
                               effectively disables the critical entry priority
                               feature for ISMQ.)
                            */
    UINT32 ismq_deadlock_avoid_threshold : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /*
                               Interdependencies may exist between entries of
                               the ingress subsequent message queue (ISMQ). As
                               a result, if only critical ISMQ entries are
                               allowed access to the pipeline, such a critical
                               entry may not make progress if it depends on a
                               non-critical entry. To avoid deadlocks in such
                               situations, if ISMQ critical-entry processing
                               experiences more consecutive rejections than
                               defined by this threshold field, a deadlock
                               avoidance mechanism is activated that gives non-
                               critical entries temporary access to the
                               pipeline. Setting this field to its maximum
                               value, all bits 1, effectively disables the
                               deadlock avoidance feature.
                            */
    UINT32 rsvd_16 : 12;

                            /* Bits[27:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 prq_disable_on_starve : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the PRQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 prq_disable_unconditional : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for PRQ to become deactivated
                               regardless of other conditions.
                            */
    UINT32 ismq_disable_on_starve : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               In addition to the critical priority mechanism,
                               the ISMQ has a legacy anti-starvation mode. The
                               critical priority and anti-starvation mechanisms
                               should interact without problems -- in fact,
                               with critical priority the trigger condition for
                               anti-starvation should largely be avoided -- but
                               if interaction problems arise, setting this bit
                               to 1 will cause the critical priority mechanism
                               to be deactivated if anti-starvation mode
                               becomes active.
                            */
    UINT32 ismq_disable_unconditional : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 causes the critical
                               priority mechanism for ISMQ to become
                               deactivated regardless of other conditions.
                            */

  } Bits;
  UINT32 Data;

} INGRESS_CRIT_ENTRY_PRIO_CHA_MISC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HA_BL_CREDITS2_CHA_MISC_REG supported on:                                    */
/*      SPRHBM (0x21f50138)                                                     */
/*      SPRC0 (0x21f50138)                                                      */
/*      SPRMCC (0x21f50138)                                                     */
/*      SPRUCC (0x21f50138)                                                     */
/* Register default value on SPRHBM: 0x00000022                                 */
/* Register default value on SPRC0: 0x00000022                                  */
/* Register default value on SPRMCC: 0x00000022                                 */
/* Register default value on SPRUCC: 0x00000022                                 */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRHBM BDF: 31_10_0                                    */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This Register contains HA BL credits to IAL MEM Controller
*/


#define HA_BL_CREDITS2_CHA_MISC_REG 0x00020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_port14 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT6. IALMEM6. */
    UINT32 bl_vna_port15 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000002*/

                            /* BL VNA count for PORT7. IALMEM7. */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_BL_CREDITS2_CHA_MISC_STRUCT;
#endif /* (SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* HA_AD_CREDITS2_CHA_MISC_REG supported on:                                    */
/*      SPRHBM (0x21f5013c)                                                     */
/*      SPRC0 (0x21f5013c)                                                      */
/*      SPRMCC (0x21f5013c)                                                     */
/*      SPRUCC (0x21f5013c)                                                     */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRHBM BDF: 31_10_0                                    */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This Register contains HA AD credits to IAL MEM Controller
*/


#define HA_AD_CREDITS2_CHA_MISC_REG 0x0002013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_vna_port14 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT6. IALMEM6. */
    UINT32 ad_vna_port15 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* AD VNA count for PORT7. IALMEM7. */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HA_AD_CREDITS2_CHA_MISC_STRUCT;
#endif /* (SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* H0_CH_ROUTE_TABLE_0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f50180)                                                      */
/*      SPRB0 (0x21f50180)                                                      */
/*      SPRHBM (0x21f50180)                                                     */
/*      SPRC0 (0x21f50180)                                                      */
/*      SPRMCC (0x21f50180)                                                     */
/*      SPRUCC (0x21f50180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into the memory channels on the die.
The Channel_ID values specify the channel ID within the target memory controller. This value is sent to the memory controller on the mesh wires to the memory controller.
There are three pairs of instances of this register. One pair for RT0, one for RT1, and one for RT2. Within each pair, the H0 version is for targetting the left half of the die. The H1 version is for targetting the right half of the die for the corresponding route table.
*/


#define H0_CH_ROUTE_TABLE_0_CHA_MISC_REG 0x00020180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H0_CH_ROUTE_TABLE_0_CHA_MISC_STRUCT;

/* H0_TGT_ROUTE_TABLE_0_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f50184)                                                      */
/*      SPRB0 (0x21f50184)                                                      */
/*      SPRHBM (0x21f50184)                                                     */
/*      SPRC0 (0x21f50184)                                                      */
/*      SPRMCC (0x21f50184)                                                     */
/*      SPRUCC (0x21f50184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into memory controllers on the die.
The Target_ID values specify the target memory controller for each of the 8 route table entries. These will determine which memory controller mesh stop is targetted.

There are two pairs of instances of this register. The first pair if for route table 0. The second par is for route table 1. Within each pair, the H0 version is for targetting the left half of the die and the H1 version is for targetting the right half of the die.
*/


#define H0_TGT_ROUTE_TABLE_0_CHA_MISC_REG 0x00020184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H0_TGT_ROUTE_TABLE_0_CHA_MISC_STRUCT;

/* H1_CH_ROUTE_TABLE_0_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f50198)                                                      */
/*      SPRB0 (0x21f50198)                                                      */
/*      SPRHBM (0x21f50198)                                                     */
/*      SPRC0 (0x21f50198)                                                      */
/*      SPRMCC (0x21f50198)                                                     */
/*      SPRUCC (0x21f50198)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into the memory channels on the die.
The Channel_ID values specify the channel ID within the target memory controller. This value is sent to the memory controller on the mesh wires to the memory controller.
There are three pairs of instances of this register. One pair for RT0, one for RT1, and one for RT2. Within each pair, the H0 version is for targetting the left half of the die. The H1 version is for targetting the right half of the die for the corresponding route table.
*/


#define H1_CH_ROUTE_TABLE_0_CHA_MISC_REG 0x00020198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H1_CH_ROUTE_TABLE_0_CHA_MISC_STRUCT;

/* H1_TGT_ROUTE_TABLE_0_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f5019c)                                                      */
/*      SPRB0 (0x21f5019c)                                                      */
/*      SPRHBM (0x21f5019c)                                                     */
/*      SPRC0 (0x21f5019c)                                                      */
/*      SPRMCC (0x21f5019c)                                                     */
/*      SPRUCC (0x21f5019c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into memory controllers on the die.
The Target_ID values specify the target memory controller for each of the 8 route table entries. These will determine which memory controller mesh stop is targetted.

There are two pairs of instances of this register. The first pair if for route table 0. The second par is for route table 1. Within each pair, the H0 version is for targetting the left half of the die and the H1 version is for targetting the right half of the die.
*/


#define H1_TGT_ROUTE_TABLE_0_CHA_MISC_REG 0x0002019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H1_TGT_ROUTE_TABLE_0_CHA_MISC_STRUCT;

/* H0_CH_ROUTE_TABLE_1_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f501b0)                                                      */
/*      SPRB0 (0x21f501b0)                                                      */
/*      SPRHBM (0x21f501b0)                                                     */
/*      SPRC0 (0x21f501b0)                                                      */
/*      SPRMCC (0x21f501b0)                                                     */
/*      SPRUCC (0x21f501b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into the memory channels on the die.
The Channel_ID values specify the channel ID within the target memory controller. This value is sent to the memory controller on the mesh wires to the memory controller.
There are three pairs of instances of this register. One pair for RT0, one for RT1, and one for RT2. Within each pair, the H0 version is for targetting the left half of the die. The H1 version is for targetting the right half of the die for the corresponding route table.
*/


#define H0_CH_ROUTE_TABLE_1_CHA_MISC_REG 0x000201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H0_CH_ROUTE_TABLE_1_CHA_MISC_STRUCT;

/* H0_TGT_ROUTE_TABLE_1_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f501b4)                                                      */
/*      SPRB0 (0x21f501b4)                                                      */
/*      SPRHBM (0x21f501b4)                                                     */
/*      SPRC0 (0x21f501b4)                                                      */
/*      SPRMCC (0x21f501b4)                                                     */
/*      SPRUCC (0x21f501b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into memory controllers on the die.
The Target_ID values specify the target memory controller for each of the 8 route table entries. These will determine which memory controller mesh stop is targetted.

There are two pairs of instances of this register. The first pair if for route table 0. The second par is for route table 1. Within each pair, the H0 version is for targetting the left half of the die and the H1 version is for targetting the right half of the die.
*/


#define H0_TGT_ROUTE_TABLE_1_CHA_MISC_REG 0x000201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H0_TGT_ROUTE_TABLE_1_CHA_MISC_STRUCT;

/* H1_CH_ROUTE_TABLE_1_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f501c8)                                                      */
/*      SPRB0 (0x21f501c8)                                                      */
/*      SPRHBM (0x21f501c8)                                                     */
/*      SPRC0 (0x21f501c8)                                                      */
/*      SPRMCC (0x21f501c8)                                                     */
/*      SPRUCC (0x21f501c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into the memory channels on the die.
The Channel_ID values specify the channel ID within the target memory controller. This value is sent to the memory controller on the mesh wires to the memory controller.
There are three pairs of instances of this register. One pair for RT0, one for RT1, and one for RT2. Within each pair, the H0 version is for targetting the left half of the die. The H1 version is for targetting the right half of the die for the corresponding route table.
*/


#define H1_CH_ROUTE_TABLE_1_CHA_MISC_REG 0x000201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H1_CH_ROUTE_TABLE_1_CHA_MISC_STRUCT;

/* H1_TGT_ROUTE_TABLE_1_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f501cc)                                                      */
/*      SPRB0 (0x21f501cc)                                                      */
/*      SPRHBM (0x21f501cc)                                                     */
/*      SPRC0 (0x21f501cc)                                                      */
/*      SPRMCC (0x21f501cc)                                                     */
/*      SPRUCC (0x21f501cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* This is the route table struct for tracking the mapping of the SAD registers into memory controllers on the die.
The Target_ID values specify the target memory controller for each of the 8 route table entries. These will determine which memory controller mesh stop is targetted.

There are two pairs of instances of this register. The first pair if for route table 0. The second par is for route table 1. Within each pair, the H0 version is for targetting the left half of the die and the H1 version is for targetting the right half of the die.
*/


#define H1_TGT_ROUTE_TABLE_1_CHA_MISC_REG 0x000201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H1_TGT_ROUTE_TABLE_1_CHA_MISC_STRUCT;

/* H0_TGT_ROUTE_TABLE_2_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f501e0)                                                      */
/*      SPRB0 (0x21f501e0)                                                      */
/*      SPRHBM (0x21f501e0)                                                     */
/*      SPRC0 (0x21f501e0)                                                      */
/*      SPRMCC (0x21f501e0)                                                     */
/*      SPRUCC (0x21f501e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2 version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H0_TGT_ROUTE_TABLE_2_CHA_MISC_REG 0x000201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H0_TGT_ROUTE_TABLE_2_CHA_MISC_STRUCT;

/* H0_CH_ROUTE_TABLE_2_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f501e4)                                                      */
/*      SPRB0 (0x21f501e4)                                                      */
/*      SPRHBM (0x21f501e4)                                                     */
/*      SPRC0 (0x21f501e4)                                                      */
/*      SPRMCC (0x21f501e4)                                                     */
/*      SPRUCC (0x21f501e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2 version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H0_CH_ROUTE_TABLE_2_CHA_MISC_REG 0x000201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H0_CH_ROUTE_TABLE_2_CHA_MISC_STRUCT;

/* H1_TGT_ROUTE_TABLE_2_CHA_MISC_REG supported on:                              */
/*      SPRA0 (0x21f501f8)                                                      */
/*      SPRB0 (0x21f501f8)                                                      */
/*      SPRHBM (0x21f501f8)                                                     */
/*      SPRC0 (0x21f501f8)                                                      */
/*      SPRMCC (0x21f501f8)                                                     */
/*      SPRUCC (0x21f501f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2 version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H1_TGT_ROUTE_TABLE_2_CHA_MISC_REG 0x000201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H1_TGT_ROUTE_TABLE_2_CHA_MISC_STRUCT;

/* H1_CH_ROUTE_TABLE_2_CHA_MISC_REG supported on:                               */
/*      SPRA0 (0x21f501fc)                                                      */
/*      SPRB0 (0x21f501fc)                                                      */
/*      SPRHBM (0x21f501fc)                                                     */
/*      SPRC0 (0x21f501fc)                                                      */
/*      SPRMCC (0x21f501fc)                                                     */
/*      SPRUCC (0x21f501fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2 version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H1_CH_ROUTE_TABLE_2_CHA_MISC_REG 0x000201FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_id_0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 0
                            */
    UINT32 channel_id_1 : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 1
                            */
    UINT32 channel_id_2 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 2
                            */
    UINT32 channel_id_3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 3
                            */
    UINT32 channel_id_4 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 4
                            */
    UINT32 channel_id_5 : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 5
                            */
    UINT32 channel_id_6 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 6
                            */
    UINT32 channel_id_7 : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into the memory
                               channels on the die This is for channel 7
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} H1_CH_ROUTE_TABLE_2_CHA_MISC_STRUCT;

/* H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG supported on:                            */
/*      SPRA0 (0x21f50210)                                                      */
/*      SPRB0 (0x21f50210)                                                      */
/*      SPRHBM (0x21f50210)                                                     */
/*      SPRC0 (0x21f50210)                                                      */
/*      SPRMCC (0x21f50210)                                                     */
/*      SPRUCC (0x21f50210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2lm version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG 0x00020210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_STRUCT;

/* H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG supported on:                            */
/*      SPRA0 (0x21f50228)                                                      */
/*      SPRB0 (0x21f50228)                                                      */
/*      SPRHBM (0x21f50228)                                                     */
/*      SPRC0 (0x21f50228)                                                      */
/*      SPRMCC (0x21f50228)                                                     */
/*      SPRUCC (0x21f50228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* For the route table 2lm version of this register, only the first 4 fields have meaning because the RT2 table has only 4 entries.
*/


#define H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG 0x00020228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 target_id_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 0th target memory
                               controller
                            */
    UINT32 target_id_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 1st target memory
                               controller
                            */
    UINT32 target_id_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 2nd target memory
                               controller
                            */
    UINT32 target_id_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 3rd target memory
                               controller
                            */
    UINT32 target_id_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 4th target memory
                               controller
                            */
    UINT32 target_id_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 5th target memory
                               controller
                            */
    UINT32 target_id_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 6th target memory
                               controller
                            */
    UINT32 target_id_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               This is the route table struct for tracking the
                               mapping of the SAD registers into memory
                               controllers on the die for the 7th target memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_0_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50300)                                                      */
/*      SPRB0 (0x21f50300)                                                      */
/*      SPRHBM (0x21f50300)                                                     */
/*      SPRC0 (0x21f50300)                                                      */
/*      SPRMCC (0x21f50300)                                                     */
/*      SPRUCC (0x21f50300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_0_CHA_MISC_REG 0x00020300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_0_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f50304)                                                      */
/*      SPRB0 (0x21f50304)                                                      */
/*      SPRHBM (0x21f50304)                                                     */
/*      SPRC0 (0x21f50304)                                                      */
/*      SPRMCC (0x21f50304)                                                     */
/*      SPRUCC (0x21f50304)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_0_CHA_MISC_REG 0x00020304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_0_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_1_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50308)                                                      */
/*      SPRB0 (0x21f50308)                                                      */
/*      SPRHBM (0x21f50308)                                                     */
/*      SPRC0 (0x21f50308)                                                      */
/*      SPRMCC (0x21f50308)                                                     */
/*      SPRUCC (0x21f50308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_1_CHA_MISC_REG 0x00020308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_1_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_1_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f5030c)                                                      */
/*      SPRB0 (0x21f5030c)                                                      */
/*      SPRHBM (0x21f5030c)                                                     */
/*      SPRC0 (0x21f5030c)                                                      */
/*      SPRMCC (0x21f5030c)                                                     */
/*      SPRUCC (0x21f5030c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_1_CHA_MISC_REG 0x0002030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_1_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_2_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50310)                                                      */
/*      SPRB0 (0x21f50310)                                                      */
/*      SPRHBM (0x21f50310)                                                     */
/*      SPRC0 (0x21f50310)                                                      */
/*      SPRMCC (0x21f50310)                                                     */
/*      SPRUCC (0x21f50310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_2_CHA_MISC_REG 0x00020310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_2_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_2_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f50314)                                                      */
/*      SPRB0 (0x21f50314)                                                      */
/*      SPRHBM (0x21f50314)                                                     */
/*      SPRC0 (0x21f50314)                                                      */
/*      SPRMCC (0x21f50314)                                                     */
/*      SPRUCC (0x21f50314)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_2_CHA_MISC_REG 0x00020314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_2_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_3_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50318)                                                      */
/*      SPRB0 (0x21f50318)                                                      */
/*      SPRHBM (0x21f50318)                                                     */
/*      SPRC0 (0x21f50318)                                                      */
/*      SPRMCC (0x21f50318)                                                     */
/*      SPRUCC (0x21f50318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_3_CHA_MISC_REG 0x00020318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_3_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_3_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f5031c)                                                      */
/*      SPRB0 (0x21f5031c)                                                      */
/*      SPRHBM (0x21f5031c)                                                     */
/*      SPRC0 (0x21f5031c)                                                      */
/*      SPRMCC (0x21f5031c)                                                     */
/*      SPRUCC (0x21f5031c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_3_CHA_MISC_REG 0x0002031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_3_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_4_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50320)                                                      */
/*      SPRB0 (0x21f50320)                                                      */
/*      SPRHBM (0x21f50320)                                                     */
/*      SPRC0 (0x21f50320)                                                      */
/*      SPRMCC (0x21f50320)                                                     */
/*      SPRUCC (0x21f50320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_4_CHA_MISC_REG 0x00020320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_4_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_4_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f50324)                                                      */
/*      SPRB0 (0x21f50324)                                                      */
/*      SPRHBM (0x21f50324)                                                     */
/*      SPRC0 (0x21f50324)                                                      */
/*      SPRMCC (0x21f50324)                                                     */
/*      SPRUCC (0x21f50324)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_4_CHA_MISC_REG 0x00020324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_4_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_5_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50328)                                                      */
/*      SPRB0 (0x21f50328)                                                      */
/*      SPRHBM (0x21f50328)                                                     */
/*      SPRC0 (0x21f50328)                                                      */
/*      SPRMCC (0x21f50328)                                                     */
/*      SPRUCC (0x21f50328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_5_CHA_MISC_REG 0x00020328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_5_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_5_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f5032c)                                                      */
/*      SPRB0 (0x21f5032c)                                                      */
/*      SPRHBM (0x21f5032c)                                                     */
/*      SPRC0 (0x21f5032c)                                                      */
/*      SPRMCC (0x21f5032c)                                                     */
/*      SPRUCC (0x21f5032c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_5_CHA_MISC_REG 0x0002032C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_5_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_6_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50330)                                                      */
/*      SPRB0 (0x21f50330)                                                      */
/*      SPRHBM (0x21f50330)                                                     */
/*      SPRC0 (0x21f50330)                                                      */
/*      SPRMCC (0x21f50330)                                                     */
/*      SPRUCC (0x21f50330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_6_CHA_MISC_REG 0x00020330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_6_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_6_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f50334)                                                      */
/*      SPRB0 (0x21f50334)                                                      */
/*      SPRHBM (0x21f50334)                                                     */
/*      SPRC0 (0x21f50334)                                                      */
/*      SPRMCC (0x21f50334)                                                     */
/*      SPRUCC (0x21f50334)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_6_CHA_MISC_REG 0x00020334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_6_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_7_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50338)                                                      */
/*      SPRB0 (0x21f50338)                                                      */
/*      SPRHBM (0x21f50338)                                                     */
/*      SPRC0 (0x21f50338)                                                      */
/*      SPRMCC (0x21f50338)                                                     */
/*      SPRUCC (0x21f50338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_7_CHA_MISC_REG 0x00020338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_7_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_7_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f5033c)                                                      */
/*      SPRB0 (0x21f5033c)                                                      */
/*      SPRHBM (0x21f5033c)                                                     */
/*      SPRC0 (0x21f5033c)                                                      */
/*      SPRMCC (0x21f5033c)                                                     */
/*      SPRUCC (0x21f5033c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_7_CHA_MISC_REG 0x0002033C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_7_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_8_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50340)                                                      */
/*      SPRB0 (0x21f50340)                                                      */
/*      SPRHBM (0x21f50340)                                                     */
/*      SPRC0 (0x21f50340)                                                      */
/*      SPRMCC (0x21f50340)                                                     */
/*      SPRUCC (0x21f50340)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_8_CHA_MISC_REG 0x00020340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_8_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_8_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f50344)                                                      */
/*      SPRB0 (0x21f50344)                                                      */
/*      SPRHBM (0x21f50344)                                                     */
/*      SPRC0 (0x21f50344)                                                      */
/*      SPRMCC (0x21f50344)                                                     */
/*      SPRUCC (0x21f50344)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_8_CHA_MISC_REG 0x00020344

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_8_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_9_CHA_MISC_REG supported on:                    */
/*      SPRA0 (0x21f50348)                                                      */
/*      SPRB0 (0x21f50348)                                                      */
/*      SPRHBM (0x21f50348)                                                     */
/*      SPRC0 (0x21f50348)                                                      */
/*      SPRMCC (0x21f50348)                                                     */
/*      SPRUCC (0x21f50348)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_9_CHA_MISC_REG 0x00020348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_9_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_9_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f5034c)                                                      */
/*      SPRB0 (0x21f5034c)                                                      */
/*      SPRHBM (0x21f5034c)                                                     */
/*      SPRC0 (0x21f5034c)                                                      */
/*      SPRMCC (0x21f5034c)                                                     */
/*      SPRUCC (0x21f5034c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_9_CHA_MISC_REG 0x0002034C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_9_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_10_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50350)                                                      */
/*      SPRB0 (0x21f50350)                                                      */
/*      SPRHBM (0x21f50350)                                                     */
/*      SPRC0 (0x21f50350)                                                      */
/*      SPRMCC (0x21f50350)                                                     */
/*      SPRUCC (0x21f50350)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_10_CHA_MISC_REG 0x00020350

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_10_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_10_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f50354)                                                      */
/*      SPRB0 (0x21f50354)                                                      */
/*      SPRHBM (0x21f50354)                                                     */
/*      SPRC0 (0x21f50354)                                                      */
/*      SPRMCC (0x21f50354)                                                     */
/*      SPRUCC (0x21f50354)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_10_CHA_MISC_REG 0x00020354

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_10_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_11_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50358)                                                      */
/*      SPRB0 (0x21f50358)                                                      */
/*      SPRHBM (0x21f50358)                                                     */
/*      SPRC0 (0x21f50358)                                                      */
/*      SPRMCC (0x21f50358)                                                     */
/*      SPRUCC (0x21f50358)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_11_CHA_MISC_REG 0x00020358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_11_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_11_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f5035c)                                                      */
/*      SPRB0 (0x21f5035c)                                                      */
/*      SPRHBM (0x21f5035c)                                                     */
/*      SPRC0 (0x21f5035c)                                                      */
/*      SPRMCC (0x21f5035c)                                                     */
/*      SPRUCC (0x21f5035c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_11_CHA_MISC_REG 0x0002035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_11_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_12_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50360)                                                      */
/*      SPRB0 (0x21f50360)                                                      */
/*      SPRHBM (0x21f50360)                                                     */
/*      SPRC0 (0x21f50360)                                                      */
/*      SPRMCC (0x21f50360)                                                     */
/*      SPRUCC (0x21f50360)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_12_CHA_MISC_REG 0x00020360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_12_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_12_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f50364)                                                      */
/*      SPRB0 (0x21f50364)                                                      */
/*      SPRHBM (0x21f50364)                                                     */
/*      SPRC0 (0x21f50364)                                                      */
/*      SPRMCC (0x21f50364)                                                     */
/*      SPRUCC (0x21f50364)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_12_CHA_MISC_REG 0x00020364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_12_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_13_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50368)                                                      */
/*      SPRB0 (0x21f50368)                                                      */
/*      SPRHBM (0x21f50368)                                                     */
/*      SPRC0 (0x21f50368)                                                      */
/*      SPRMCC (0x21f50368)                                                     */
/*      SPRUCC (0x21f50368)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_13_CHA_MISC_REG 0x00020368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_13_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_13_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f5036c)                                                      */
/*      SPRB0 (0x21f5036c)                                                      */
/*      SPRHBM (0x21f5036c)                                                     */
/*      SPRC0 (0x21f5036c)                                                      */
/*      SPRMCC (0x21f5036c)                                                     */
/*      SPRUCC (0x21f5036c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_13_CHA_MISC_REG 0x0002036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_13_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_14_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50370)                                                      */
/*      SPRB0 (0x21f50370)                                                      */
/*      SPRHBM (0x21f50370)                                                     */
/*      SPRC0 (0x21f50370)                                                      */
/*      SPRMCC (0x21f50370)                                                     */
/*      SPRUCC (0x21f50370)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_14_CHA_MISC_REG 0x00020370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_14_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_14_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f50374)                                                      */
/*      SPRB0 (0x21f50374)                                                      */
/*      SPRHBM (0x21f50374)                                                     */
/*      SPRC0 (0x21f50374)                                                      */
/*      SPRMCC (0x21f50374)                                                     */
/*      SPRUCC (0x21f50374)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_14_CHA_MISC_REG 0x00020374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_14_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG_UNICAST_GROUP0_15_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50378)                                                      */
/*      SPRB0 (0x21f50378)                                                      */
/*      SPRHBM (0x21f50378)                                                     */
/*      SPRC0 (0x21f50378)                                                      */
/*      SPRMCC (0x21f50378)                                                     */
/*      SPRUCC (0x21f50378)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* <p>These registers are used to map out any memory coherent memory region which spans local socket memory or which interleave across multiple remote sockets. This includes any locally mapped NUMA ranges, as well as any UMA memory ranges.</p>
<p>Each instance&nbsp;of DRAM_RULE_CFG has a corresponding INTERLEAVE_LIST_CFG CR, denoting the targets for the specific decoder.</p>
*/


#define DRAM_RULE_CFG_UNICAST_GROUP0_15_CHA_MISC_REG 0x00020378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /* Enable for this DRAM rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               DRAM rule interleave mode. This value determines
                               which 3 bits of the Address should be used to
                               index the Interleave list 00: Address bits
                               {8,7,6}. [64B] 01: Address bits {10,9,8}. [256B]
                               10: Address bits {14,13,12}. [4K] 11: Address
                               bits {15,14,13}. [8K]
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Sets the memory type for the remote DRAM rule
                               according to the following encoding. 00 -
                               Coherent DRAM 01 - Memory Mapped CFG 10 - Low
                               Bandwidth Coherent DRAM 11 - High Bandwidth
                               Coherent DRAM
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies whether or not this address range is
                               cacheable in near memory.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               This correspond to Addr[51:26] of the DRAM rule
                               top limit address. Must be strickly greater then
                               previous rule, even if this rule is disabled,
                               unless this rule and all following rules are
                               disabled. Lower limit is the previous rule (or 0
                               if this is the first rule)
                            */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG_UNICAST_GROUP0_15_CHA_MISC_STRUCT;

/* INTERLEAVE_LIST_CFG_UNICAST_GROUP0_15_CHA_MISC_REG supported on:             */
/*      SPRA0 (0x21f5037c)                                                      */
/*      SPRB0 (0x21f5037c)                                                      */
/*      SPRHBM (0x21f5037c)                                                     */
/*      SPRC0 (0x21f5037c)                                                      */
/*      SPRMCC (0x21f5037c)                                                     */
/*      SPRUCC (0x21f5037c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Interleave list target.  Encoding is as follows:
            4b0xyz  -  target is a remote socket with NodeId=3bxyz
            4b1xyz  -  target is a local memory controller 

When the target is a local memory controller, the LSB (z) indicates the target half (H0/H1) within the local socket i.e. whether the H0 RT is used or the H1 RT is used
The 2 bits 'xy' are used in case of single-way interleaving. These bits are then used to index into a Route Table (RT1/RT2) of any half (H0/H1) to decode any of 4 possible targets
*/


#define INTERLEAVE_LIST_CFG_UNICAST_GROUP0_15_CHA_MISC_REG 0x0002037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 0. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 1. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 2. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 3. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 4. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 5. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 6. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RW/L, default=0x00000008*/

                            /*
                               Interleave list target 7. Encoding is as
                               follows: 4b0xyz - target is a remote socket with
                               NodeId=3bxyz 4b1xyz - target is a local memory
                               controller
                            */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST_CFG_UNICAST_GROUP0_15_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_0_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50380)                                                      */
/*      SPRB0 (0x21f50380)                                                      */
/*      SPRHBM (0x21f50380)                                                     */
/*      SPRC0 (0x21f50380)                                                      */
/*      SPRMCC (0x21f50380)                                                     */
/*      SPRUCC (0x21f50380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_0_CHA_MISC_REG 0x00020380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_0_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_1_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50384)                                                      */
/*      SPRB0 (0x21f50384)                                                      */
/*      SPRHBM (0x21f50384)                                                     */
/*      SPRC0 (0x21f50384)                                                      */
/*      SPRMCC (0x21f50384)                                                     */
/*      SPRUCC (0x21f50384)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_1_CHA_MISC_REG 0x00020384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_1_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_2_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50388)                                                      */
/*      SPRB0 (0x21f50388)                                                      */
/*      SPRHBM (0x21f50388)                                                     */
/*      SPRC0 (0x21f50388)                                                      */
/*      SPRMCC (0x21f50388)                                                     */
/*      SPRUCC (0x21f50388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_2_CHA_MISC_REG 0x00020388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_2_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_3_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f5038c)                                                      */
/*      SPRB0 (0x21f5038c)                                                      */
/*      SPRHBM (0x21f5038c)                                                     */
/*      SPRC0 (0x21f5038c)                                                      */
/*      SPRMCC (0x21f5038c)                                                     */
/*      SPRUCC (0x21f5038c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_3_CHA_MISC_REG 0x0002038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_3_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_4_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50390)                                                      */
/*      SPRB0 (0x21f50390)                                                      */
/*      SPRHBM (0x21f50390)                                                     */
/*      SPRC0 (0x21f50390)                                                      */
/*      SPRMCC (0x21f50390)                                                     */
/*      SPRUCC (0x21f50390)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_4_CHA_MISC_REG 0x00020390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_4_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_5_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50394)                                                      */
/*      SPRB0 (0x21f50394)                                                      */
/*      SPRHBM (0x21f50394)                                                     */
/*      SPRC0 (0x21f50394)                                                      */
/*      SPRMCC (0x21f50394)                                                     */
/*      SPRUCC (0x21f50394)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_5_CHA_MISC_REG 0x00020394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_5_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_6_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f50398)                                                      */
/*      SPRB0 (0x21f50398)                                                      */
/*      SPRHBM (0x21f50398)                                                     */
/*      SPRC0 (0x21f50398)                                                      */
/*      SPRMCC (0x21f50398)                                                     */
/*      SPRUCC (0x21f50398)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_6_CHA_MISC_REG 0x00020398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_6_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_7_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f5039c)                                                      */
/*      SPRB0 (0x21f5039c)                                                      */
/*      SPRHBM (0x21f5039c)                                                     */
/*      SPRC0 (0x21f5039c)                                                      */
/*      SPRMCC (0x21f5039c)                                                     */
/*      SPRUCC (0x21f5039c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_7_CHA_MISC_REG 0x0002039C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_7_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_8_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f503a0)                                                      */
/*      SPRB0 (0x21f503a0)                                                      */
/*      SPRHBM (0x21f503a0)                                                     */
/*      SPRC0 (0x21f503a0)                                                      */
/*      SPRMCC (0x21f503a0)                                                     */
/*      SPRUCC (0x21f503a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_8_CHA_MISC_REG 0x000203A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_8_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_9_CHA_MISC_REG supported on:                   */
/*      SPRA0 (0x21f503a4)                                                      */
/*      SPRB0 (0x21f503a4)                                                      */
/*      SPRHBM (0x21f503a4)                                                     */
/*      SPRC0 (0x21f503a4)                                                      */
/*      SPRMCC (0x21f503a4)                                                     */
/*      SPRUCC (0x21f503a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_9_CHA_MISC_REG 0x000203A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_9_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_10_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503a8)                                                      */
/*      SPRB0 (0x21f503a8)                                                      */
/*      SPRHBM (0x21f503a8)                                                     */
/*      SPRC0 (0x21f503a8)                                                      */
/*      SPRMCC (0x21f503a8)                                                     */
/*      SPRUCC (0x21f503a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_10_CHA_MISC_REG 0x000203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_10_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_11_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503ac)                                                      */
/*      SPRB0 (0x21f503ac)                                                      */
/*      SPRHBM (0x21f503ac)                                                     */
/*      SPRC0 (0x21f503ac)                                                      */
/*      SPRMCC (0x21f503ac)                                                     */
/*      SPRUCC (0x21f503ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_11_CHA_MISC_REG 0x000203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_11_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_12_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503b0)                                                      */
/*      SPRB0 (0x21f503b0)                                                      */
/*      SPRHBM (0x21f503b0)                                                     */
/*      SPRC0 (0x21f503b0)                                                      */
/*      SPRMCC (0x21f503b0)                                                     */
/*      SPRUCC (0x21f503b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_12_CHA_MISC_REG 0x000203B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_12_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_13_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503b4)                                                      */
/*      SPRB0 (0x21f503b4)                                                      */
/*      SPRHBM (0x21f503b4)                                                     */
/*      SPRC0 (0x21f503b4)                                                      */
/*      SPRMCC (0x21f503b4)                                                     */
/*      SPRUCC (0x21f503b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_13_CHA_MISC_REG 0x000203B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_13_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_14_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503b8)                                                      */
/*      SPRB0 (0x21f503b8)                                                      */
/*      SPRHBM (0x21f503b8)                                                     */
/*      SPRC0 (0x21f503b8)                                                      */
/*      SPRMCC (0x21f503b8)                                                     */
/*      SPRUCC (0x21f503b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_14_CHA_MISC_REG 0x000203B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_14_CHA_MISC_STRUCT;

/* DRAM_RULE_CFG2_UNICAST_GROUP0_15_CHA_MISC_REG supported on:                  */
/*      SPRA0 (0x21f503bc)                                                      */
/*      SPRB0 (0x21f503bc)                                                      */
/*      SPRHBM (0x21f503bc)                                                     */
/*      SPRC0 (0x21f503bc)                                                      */
/*      SPRMCC (0x21f503bc)                                                     */
/*      SPRUCC (0x21f503bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* These registers corresponding to each DRAM rule define some settings for the address range being snoop or not, or being a part of IAL memory.
*/


#define DRAM_RULE_CFG2_UNICAST_GROUP0_15_CHA_MISC_REG 0x000203BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snoopy_range_enable : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, if set indicates that the address
                               range covered by this DRAM_RULE is snoopy
                            */
    UINT32 cxl_accelerator_mem : 1;

                            /* Bits[1:1], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set to 1 for this particular DRAM
                               rule, indicates that the rule's memory space
                               maps to a caching accelerator device connected
                               via a Compute Express Link (CXL). (Setting this
                               bit is mutually exclusive with cxl_mem_expander.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 durable_mem_en : 1;

                            /* Bits[2:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               When 0, this bit indicates that this DRAM rule
                               pertains to volatile memory, if set to 1, it
                               indicates durable memory
                            */
    UINT32 cxl_mem_expander : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit, when set for this particular DRAM Rule
                               indicates that the rule's memory space maps to a
                               (non-caching) memory expander connected via a
                               Compute Express Link (CXL). (Setting this bit is
                               mutually exclusive with cxl_accelerator_mem.
                               Only one of cxl_accelerator_mem or
                               cxl_mem_expander may be set, or both may be
                               zero.) (CXL was previously referred to as Intel
                               Accelerator Link or IAL.)
                            */
    UINT32 nxm : 1;

                            /* Bits[4:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Indicates this rule defines NXM (non-existant
                               memory) - can be hot-added later.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE_CFG2_UNICAST_GROUP0_15_CHA_MISC_STRUCT;

/* DRAM_H0_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503c0)                                                      */
/*      SPRB0 (0x21f503c0)                                                      */
/*      SPRHBM (0x21f503c0)                                                     */
/*      SPRC0 (0x21f503c0)                                                      */
/*      SPRMCC (0x21f503c0)                                                     */
/*      SPRUCC (0x21f503c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H0_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503c4)                                                      */
/*      SPRB0 (0x21f503c4)                                                      */
/*      SPRHBM (0x21f503c4)                                                     */
/*      SPRC0 (0x21f503c4)                                                      */
/*      SPRMCC (0x21f503c4)                                                     */
/*      SPRUCC (0x21f503c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503c8)                                                      */
/*      SPRB0 (0x21f503c8)                                                      */
/*      SPRHBM (0x21f503c8)                                                     */
/*      SPRC0 (0x21f503c8)                                                      */
/*      SPRMCC (0x21f503c8)                                                     */
/*      SPRUCC (0x21f503c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503cc)                                                      */
/*      SPRB0 (0x21f503cc)                                                      */
/*      SPRHBM (0x21f503cc)                                                     */
/*      SPRC0 (0x21f503cc)                                                      */
/*      SPRMCC (0x21f503cc)                                                     */
/*      SPRUCC (0x21f503cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H0_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503d0)                                                      */
/*      SPRB0 (0x21f503d0)                                                      */
/*      SPRHBM (0x21f503d0)                                                     */
/*      SPRC0 (0x21f503d0)                                                      */
/*      SPRMCC (0x21f503d0)                                                     */
/*      SPRUCC (0x21f503d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H0_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503d4)                                                      */
/*      SPRB0 (0x21f503d4)                                                      */
/*      SPRHBM (0x21f503d4)                                                     */
/*      SPRC0 (0x21f503d4)                                                      */
/*      SPRMCC (0x21f503d4)                                                     */
/*      SPRUCC (0x21f503d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503d8)                                                      */
/*      SPRB0 (0x21f503d8)                                                      */
/*      SPRHBM (0x21f503d8)                                                     */
/*      SPRC0 (0x21f503d8)                                                      */
/*      SPRMCC (0x21f503d8)                                                     */
/*      SPRUCC (0x21f503d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503dc)                                                      */
/*      SPRB0 (0x21f503dc)                                                      */
/*      SPRHBM (0x21f503dc)                                                     */
/*      SPRC0 (0x21f503dc)                                                      */
/*      SPRMCC (0x21f503dc)                                                     */
/*      SPRUCC (0x21f503dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H0_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503e0)                                                      */
/*      SPRB0 (0x21f503e0)                                                      */
/*      SPRHBM (0x21f503e0)                                                     */
/*      SPRC0 (0x21f503e0)                                                      */
/*      SPRMCC (0x21f503e0)                                                     */
/*      SPRUCC (0x21f503e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H0_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H0_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503e4)                                                      */
/*      SPRB0 (0x21f503e4)                                                      */
/*      SPRHBM (0x21f503e4)                                                     */
/*      SPRC0 (0x21f503e4)                                                      */
/*      SPRMCC (0x21f503e4)                                                     */
/*      SPRUCC (0x21f503e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H0_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H0_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503e8)                                                      */
/*      SPRB0 (0x21f503e8)                                                      */
/*      SPRHBM (0x21f503e8)                                                     */
/*      SPRC0 (0x21f503e8)                                                      */
/*      SPRMCC (0x21f503e8)                                                     */
/*      SPRUCC (0x21f503e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 0 through 9. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
110- 8 way
*/


#define DRAM_H1_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_0 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 0
                            */
    UINT32 mode_1 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 1
                            */
    UINT32 mode_2 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 2
                            */
    UINT32 mode_3 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 3
                            */
    UINT32 mode_4 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 4
                            */
    UINT32 mode_5 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 5
                            */
    UINT32 mode_6 : 3;

                            /* Bits[20:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 6
                            */
    UINT32 mode_7 : 3;

                            /* Bits[23:21], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 7
                            */
    UINT32 mode_8 : 3;

                            /* Bits[26:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 8
                            */
    UINT32 mode_9 : 3;

                            /* Bits[29:27], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rule 9
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_H1_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:              */
/*      SPRA0 (0x21f503ec)                                                      */
/*      SPRB0 (0x21f503ec)                                                      */
/*      SPRHBM (0x21f503ec)                                                     */
/*      SPRC0 (0x21f503ec)                                                      */
/*      SPRMCC (0x21f503ec)                                                     */
/*      SPRUCC (0x21f503ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies the decode mode for the memory targetted by route table 0 and route table 1 on one side of the die for DRAM rules 10 through 15. H0 version of the register covers the left half, the H1 version of the register covers the right half. The RT0 version  of the register covers route table 0, and the RT1 version of the register covers route table 1. This mode helps generating the index that is used to finally index into the route tables.

For RT0 and RT1,  modes 100 and 111 are unsupported values.
Supported values are
000- Single target
001- 2-way
010- 3-way (modulo 3)
011- 4-way
101- 6-way
110- 8-way
          
       
*/


#define DRAM_H1_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mode_10 : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 10
                            */
    UINT32 mode_11 : 3;

                            /* Bits[5:3], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 11
                            */
    UINT32 mode_12 : 3;

                            /* Bits[8:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 12
                            */
    UINT32 mode_13 : 3;

                            /* Bits[11:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 13
                            */
    UINT32 mode_14 : 3;

                            /* Bits[14:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 14
                            */
    UINT32 mode_15 : 3;

                            /* Bits[17:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies the decode mode for the memory
                               targetted by route table 0 and route table 1 on
                               one side of the die for DRAM rules 15
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_H1_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_MC_CHANNEL_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:                */
/*      SPRA0 (0x21f503f0)                                                      */
/*      SPRB0 (0x21f503f0)                                                      */
/*      SPRHBM (0x21f503f0)                                                     */
/*      SPRC0 (0x21f503f0)                                                      */
/*      SPRMCC (0x21f503f0)                                                     */
/*      SPRUCC (0x21f503f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Specifies which route table each DRAM_RULE will use to determine its target ch_ID
00 - means the target memory channel comes from route table 0
01 - means the target memory channel comes from route table 1
10 - means the target memory channel comes from route table 2
11-Reserved
There are 16 fields for each of the 24 dram rules. Field mc_channel_N specifies the mechanism for determining the channel_ID for dram rule N.
*/


#define DRAM_MC_CHANNEL_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_channel_0 : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 0 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_1 : 2;

                            /* Bits[3:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 1 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_2 : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 2 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_3 : 2;

                            /* Bits[7:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 3 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_4 : 2;

                            /* Bits[9:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 4 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_5 : 2;

                            /* Bits[11:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 5 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_6 : 2;

                            /* Bits[13:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 6 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_7 : 2;

                            /* Bits[15:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 7 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_8 : 2;

                            /* Bits[17:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 8 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_9 : 2;

                            /* Bits[19:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 9 will use
                               to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_10 : 2;

                            /* Bits[21:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 10 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_11 : 2;

                            /* Bits[23:22], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 11 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_12 : 2;

                            /* Bits[25:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 12 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_13 : 2;

                            /* Bits[27:26], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 13 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_14 : 2;

                            /* Bits[29:28], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 14 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */
    UINT32 mc_channel_15 : 2;

                            /* Bits[31:30], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE 15 will
                               use to determine its target ch_ID 0 - means the
                               target memory channel comes from route table 0 1
                               - means the target memory channel comes from
                               route table 1 2 - means the target memory
                               channel comes from route table 2 3 - rsvd There
                               are 16 fields for each of the 16 dram rules.
                               Field mc_channel_N specifies the mechanism for
                               determining the channel_ID for dram rule N.
                            */

  } Bits;
  UINT32 Data;

} DRAM_MC_CHANNEL_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_MC_TARGET_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:                 */
/*      SPRA0 (0x21f503f4)                                                      */
/*      SPRB0 (0x21f503f4)                                                      */
/*      SPRHBM (0x21f503f4)                                                     */
/*      SPRC0 (0x21f503f4)                                                      */
/*      SPRMCC (0x21f503f4)                                                     */
/*      SPRUCC (0x21f503f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Contains a set of 2 bit fields which individually specify for each DRAM rule which route table output will be used to determine that rule's MC target.
00 - means the target memory channel comes from route table 0
01 - means the target memory channel comes from route table 1
10 - means the target memory channel comes from route table 2
11-means the target memory channel comes from route table 2LM
There are 16 fields for each of the 24 dram rules. Field mc_channel_N specifies the mechanism for determining the channel_ID for dram rule N.
*/


#define DRAM_MC_TARGET_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_target_0 : 2;

                            /* Bits[1:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_0 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_1 : 2;

                            /* Bits[3:2], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_1 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_2 : 2;

                            /* Bits[5:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_2 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_3 : 2;

                            /* Bits[7:6], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_3 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_4 : 2;

                            /* Bits[9:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_4 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_5 : 2;

                            /* Bits[11:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_5 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_6 : 2;

                            /* Bits[13:12], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_6 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_7 : 2;

                            /* Bits[15:14], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_7 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_8 : 2;

                            /* Bits[17:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_8 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_9 : 2;

                            /* Bits[19:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_9 will use
                               to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_10 : 2;

                            /* Bits[21:20], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_10 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_11 : 2;

                            /* Bits[23:22], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_11 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_12 : 2;

                            /* Bits[25:24], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_12 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_13 : 2;

                            /* Bits[27:26], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_13 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_14 : 2;

                            /* Bits[29:28], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_14 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */
    UINT32 mc_target_15 : 2;

                            /* Bits[31:30], Access Type=RW/L, default=0x00000000*/

                            /*
                               Specifies which route table DRAM_RULE_15 will
                               use to determine its target MC_ID 00 - means the
                               target memory channel comes from route table 0
                               01 - means the target memory channel comes from
                               route table 1 10 - means the target memory
                               channel comes from route table 2 11-means the
                               target memory channel comes from route table 2LM
                            */

  } Bits;
  UINT32 Data;

} DRAM_MC_TARGET_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;

/* DRAM_GLOBAL_INTERLEAVE_CFG_UNICAST_GROUP0_CHA_MISC_REG supported on:         */
/*      SPRA0 (0x21f503f8)                                                      */
/*      SPRB0 (0x21f503f8)                                                      */
/*      SPRHBM (0x21f503f8)                                                     */
/*      SPRC0 (0x21f503f8)                                                      */
/*      SPRMCC (0x21f503f8)                                                     */
/*      SPRUCC (0x21f503f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRB0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRHBM Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRC0 Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRMCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* SPRUCC Register File:    sprsp_top/CHA_MULTI/SAD/CFG_CBO_UNICAST_GROUP0_0x000_0x2ff*/
/* Struct generated from SPRA0 BDF: 31_10_0                                     */
/* SPRA0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRB0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRHBM Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRC0 Security PolicyGroup: LB_MCHECK_CHA_W                                  */
/* SPRMCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* SPRUCC Security PolicyGroup: LB_MCHECK_CHA_W                                 */
/* Contains the base interleave granularity for each of the dram interleaves for all route tables- RT0,RT1,RT2
*/


#define DRAM_GLOBAL_INTERLEAVE_CFG_UNICAST_GROUP0_CHA_MISC_REG 0x000203F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt0_interleave_shift : 3;

                            /* Bits[2:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT0 route table indexing
                               logic 0 - use [8:6] when in a power of two mode.
                               Use [51:6] as input to the mod3 for 3-way
                               interleave modes. 1 - reserved 2 - use [10:8]
                               when in a power of two mode. Use [51:8] as input
                               to the mod3 for 3-way interleave modes. this is
                               the 256B interleave mode without XOR 3 -
                               reserved 4 - reserved 5 - reserved 6 - use
                               [14:12] when in a power of two mode. Use [51:12]
                               as input to the mod3 for 3-way interleave modes
                               7 - use the following bit-wise XOR as the index
                               [10:8] ^ [16:14] ^ [24:22] in power of two mode.
                               Use {51:11, [10:8] ^ [16:14] ^ [24:22]} as input
                               to the mod3 for 3-way interleave modes. This is
                               the perf recommended 256B-XOR interleave mode
                               for power of 2.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt1_interleave_shift : 3;

                            /* Bits[6:4], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT1 route table indexing
                               logic 0 use [8:6] when in a power of two mode.
                               Use [51:6] as input to the mod3 for 3-way
                               interleave modes. 1 reserved 2 use [10:8] when
                               in a power of two mode. Use [51:8] as input to
                               the mod3 for 3-way interleave modes. this is the
                               256B interleave mode without XOR 3 reserved 4
                               reserved 5 reserved 6 - use [14:12] when in a
                               power of two mode. Use [51:12] as input to the
                               mod3 for 3-way interleave modes 7 - use the
                               following bit-wise XOR as the index [10:8] ^
                               [16:14] ^ [24:22] in power of two mode. Use
                               {51:11, [10:8] ^ [16:14] ^ [24:22]} as input to
                               the mod3 for 3-way interleave modes. This is the
                               perf recommended 256B-XOR interleave mode for
                               power of 2.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt2_interleave_shift : 4;

                            /* Bits[11:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Number of bits to shift the physical address
                               before feeding into the RT2 route table indexing
                               logic. 0 reserved 1 reserved 2 use [9:8] when in
                               power of two mode. Use [51:8] as input to the
                               mod3 for 3-way interleave modes (used in
                               mirroring modes) 3 reserved 4 reserved 5
                               reserved 6 use [13:12] when in power of 2 mode.
                               Use [51:12] as input to the mod3 for 3-way
                               interleave modes. (Standard 4KB page granular,
                               low BW memory interleave mode) 7-reserved
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_GLOBAL_INTERLEAVE_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT;


#endif /* _CHA_MISC_h */
