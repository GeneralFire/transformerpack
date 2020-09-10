
/** @file
  SPK_MAIN.h

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


#ifndef _SPK_MAIN_h
#define _SPK_MAIN_h
#include <Base.h>

/* SPK_GLOBAL_CTL_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f40)                                                      */
/*      SPRB0 (0x20058f40)                                                      */
/*      SPRHBM (0x20058f40)                                                     */
/*      SPRC0 (0x20058f40)                                                      */
/*      SPRMCC (0x20058f40)                                                     */
/*      SPRUCC (0x20058f40)                                                     */
/* Register default value on SPRA0: 0x00000208                                  */
/* Register default value on SPRB0: 0x00000208                                  */
/* Register default value on SPRHBM: 0x00000208                                 */
/* Register default value on SPRC0: 0x00000208                                  */
/* Register default value on SPRMCC: 0x00000208                                 */
/* Register default value on SPRUCC: 0x00000208                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register controls the global logic in Sierra Peak and contains some of the functionality present in NPK's MSCnCTL register.
*/


#define SPK_GLOBAL_CTL_SPK_MAIN_REG 0x20028F40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, this instance of Sierra Peak is
                               enabled. Note that this is the master enable
                               signal. This should generally be the last bit to
                               be set when programming SPK (except if SPK is to
                               be enabled as the result of a trigger response.)
                               When set, the agent sharing the mesh stop with
                               SPK will alter its behavior.
                            */
    UINT32 wrap_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Memory Wrap Enabled 0: no wrap 1: wrap enabled
                               This bit enables wrapping within the single
                               memory block.
                            */
    UINT32 capture_done : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When set, forces Sierra Peak to stop accepting
                               new transactions from the DTF (either by
                               dropping or stopping to return credits to the
                               upstream arbiter, depending on the setting of
                               DROP_DATA), and then drain flush all buffers.
                               Note that stopping is not guaranteed to be on
                               any type of packet boundary, however SPK will
                               not stop on a cycle where the debug trace fabric
                               EOF=0.
                            */
    UINT32 drop_data : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               When set, Sierra Peak will drop all incoming DTF
                               data when SPK is not enabled (as indicated by
                               either SPK_GLOBAL_CTL.CAPTURE_DONE = 1 or
                               SPK_GLOBAL_STS.TRIG_STOP = 1 or
                               SPK_GLOBAL_CTL.EN = 1) When clear, Sierra Peak
                               will absorb data into its buffers and wait until
                               it is able to egress lines.
                            */
    UINT32 force_idle : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When set, forces Sierra Peak's to flush all
                               buffers, state machines, refund outstanding
                               credits, etc. and go to an idle state. Note that
                               this will not cause a credit re-initialization
                               with the upstream DTF arbiter.
                            */
    UINT32 trig_stop_en : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               When set, Sierra Peak will initiate a capture
                               done event in hardware after the number of lines
                               contained in SPK_GLOBAL_POSTSTORE have been
                               written to memory. The flow is identical to the
                               flow initiated by writing the CAPTURE_DONE bit.
                               Note that this bit is effectively the ARM bit
                               for this trigger, and has no effect if
                               SPK_GLOBAL_STS.TRIG_STOP=1.
                            */
    UINT32 trig_start_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               When set, Sierra Peak will begin claiming
                               transactions from the DTF on the next packet
                               where debug trace fabric EOF=1. Because SPK has
                               no idea on any type of framing occuring within
                               the debug trace fabric data, it will also assert
                               a sync event upstream. Note that this usage
                               model should be used with DROP_DATA=1 [it is
                               nonsensical to use it with DROP_DATA=0]. Further
                               note that this bit is effectively the ARM bit
                               for this trigger and has no effect if
                               SPK_GLOBAL_STS.TRIG_START=1. The global enable
                               bit (SPK_GLOBAL_CTL.EN) must be set to have the
                               trigger start take effect. However, the
                               TRIG_START_EN must be programmed prior to EN
                               being set in order to keep SPK from immediately
                               starting.
                            */
    UINT32 inject_rate : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Determines the rate at which Sierra Peak will
                               inject into the agent IP -- once per N cycles,
                               where INJECT_RATE is the value of N. Bit clear
                               means N is 2. Bit set means N is 4. This value
                               is sent directly to the Agent IP logic to avoid
                               programming values in multiple places. Must be
                               programmed while SPK_CTL_AGL.EN is zero.
                            */
    UINT32 drop_slots_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable dropping of any spk_early_bl_valid or
                               spk_early_ad_valid slots when SPK is not
                               injecting into the ADD/DROP port. This gives
                               additional BW back to the agent.
                            */
    UINT32 stp22en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               STPv2.2 Enable: Clearing this bit will disable
                               all STP stream features that are specific to
                               MIPI STPv2.2 (including the STPv2.2 VER and
                               FREQ40 packets) and will run Sierra Peak in
                               STPv2.1 compatibility mode.
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_GLOBAL_CTL_SPK_MAIN_STRUCT;

/* SPK_GLOBAL_STS_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f44)                                                      */
/*      SPRB0 (0x20058f44)                                                      */
/*      SPRHBM (0x20058f44)                                                     */
/*      SPRC0 (0x20058f44)                                                      */
/*      SPRMCC (0x20058f44)                                                     */
/*      SPRUCC (0x20058f44)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register provides status information from global control logic in Sierra Peak and is similar to MSCnSTS register in NPK.
*/


#define SPK_GLOBAL_STS_SPK_MAIN_REG 0x20028F44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idle : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, indicates that all internal buffers
                               inside of Sierra Peak are empty and all credits
                               have been returned to the upstream debug trace
                               fabric arbiter. Note that this does not ensure
                               that there are transactions inflight to the
                               Sierra Peak upstream on the debug trace fabric.
                               Note that this does not ensure that all memory
                               writes from Sierra Peak have completed in
                               memory; rather it means that Sierra Peak has
                               completed sending all memory transactions.
                            */
    UINT32 wrap_stat : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               0: memory wrap did not occur (that is, the write
                               pointer never wrapped around to zero.) Trace
                               data should be reconstructed starting from the
                               start of the memory buffer. 1: memory wrap
                               occurred. This is set when
                               SPK_GLOBAL_CTL.WRAP_EN = 1 and SPK__SIZE*4k
                               bytes or more are written to the memory buffer.
                               Bit will be cleared until SPK_GLOBAL_CTL.EN=0 or
                               reset is asserted.
                            */
    UINT32 trig_stop : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set, indicates Sierra Peak has received a
                               trigger response to stop operation and has
                               executed its post-trigger storage. This bit is
                               cleared by software. Note, however, if this bit
                               is cleared (e.g., by clearing
                               SPK_GLOBAL_CTL.TRIG_START_EN) and SPK is not
                               stopped in some other fashion, SPK will resume
                               to capture trace.
                            */
    UINT32 trig_start : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set, indicates Sierra Peak has received a
                               trigger response to start operation. This bit is
                               cleared by software.
                            */
    UINT32 dib_drop_count : 8;

                            /* Bits[11:4], Access Type=RW/V, default=0x00000000*/

                            /*
                               When non-zero, indicates that the DIB dropped an
                               incoming DTF transaction. This is an 8-bit
                               saturating counter. This field is cleared by
                               software. This bit counts valid cycles on the
                               DTF interface input and is intended to be used
                               for debug of SPK itself.
                            */
    UINT32 ad_crd_avail : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Status bit indicates that credit on AD Add
                               interface to CMS is available.
                            */
    UINT32 bl_crd_avail : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               Status bit indicates that credit on BL Add
                               interface to CMS is available.
                            */
    UINT32 ak_rsp_avail : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               Status bit indicates that AK Responses from CMS
                               are available.
                            */
    UINT32 mlb_empty : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               Status bit indicates that the Memory Line Buffer
                               is empty.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_GLOBAL_STS_SPK_MAIN_STRUCT;

/* SPK_GLOBAL_TRIG_POSTSTORE_SPK_MAIN_REG supported on:                         */
/*      SPRA0 (0x20058f48)                                                      */
/*      SPRB0 (0x20058f48)                                                      */
/*      SPRHBM (0x20058f48)                                                     */
/*      SPRC0 (0x20058f48)                                                      */
/*      SPRMCC (0x20058f48)                                                     */
/*      SPRUCC (0x20058f48)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* 
	This register contains the number of memory lines to capture after receiving a stop trigger event.
	
*/


#define SPK_GLOBAL_TRIG_POSTSTORE_SPK_MAIN_REG 0x20028F48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 count : 16;

                            /* Bits[15:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This value contains the number of lines to be
                               stored following Sierra Peak receiving a stop
                               trigger response. Stop on trigger must be
                               enabled (SPK_GLOBAL_CTL.TRIG_STOP_EN=1). Note
                               that this value contains the number of lines to
                               be stored by this Sierra Peak instance; there is
                               no correlation to the number of memory lines
                               stored by other trace aggregators in the system.
                            */
    UINT32 count_remain : 16;

                            /* Bits[31:16], Access Type=RW/V, default=0x00000000*/

                            /*
                               When counting down in a post-store operation,
                               this field contains the number of lines
                               remaining to be stored. This field may be
                               save/restored if necessary.
                            */

  } Bits;
  UINT32 Data;

} SPK_GLOBAL_TRIG_POSTSTORE_SPK_MAIN_STRUCT;

/* SPK_CLOCK_CONTROL_SPK_MAIN_REG supported on:                                 */
/*      SPRA0 (0x20058f4c)                                                      */
/*      SPRB0 (0x20058f4c)                                                      */
/*      SPRHBM (0x20058f4c)                                                     */
/*      SPRC0 (0x20058f4c)                                                      */
/*      SPRMCC (0x20058f4c)                                                     */
/*      SPRUCC (0x20058f4c)                                                     */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register contains the clock gate overrides and PM block overrides.
*/


#define SPK_CLOCK_CONTROL_SPK_MAIN_REG 0x20028F4C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srcblk_req_dly_timer_cfc : 10;

                            /* Bits[9:0], Access Type=RW/P, default=0x00000080*/

                            /*
                               <p>Source Blocking Control: Number of uclk
                               cycles to delay sending Vcfc req to SPK. Feature
                               enabled via SRCBLK_DLY_REQ_CFC.</p>
                            */
    UINT32 srcblk_bypass_en_cfc : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>Source Blocking Control: Bypass source block
                               aggregation for Vcfc power domain by asserting
                               block_ack_cfc without waiting for subIP
                               responses.</p>
                            */
    UINT32 srcblk_timeout_en_cfc : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>Source Blocking Control: Enable timeout for
                               asserting block_ack_cfc (1024 uclk cycles after
                               req).</p>
                            */
    UINT32 srcblk_dis_ack_cfc : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               <p>Source Blocking Control: Never assert
                               block_ack_cfc. (Intended for use with force ack
                               bit: SRCBLK_FORCE_ACK_CFC)</p>
                            */
    UINT32 srcblk_force_ack_cfc : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>Source Blocking Control: Setting this bit
                               forces block_ack_cfc to assert if
                               SRCBLK_DIS_ACK_CFC is enabled.</p>
                            */
    UINT32 srcblk_dly_req_cfc : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>Source Blocking Control: Delay sending req to
                               SPK. Delay duration determined by value in
                               timer_reg in dtf_clk cycles.</p>
                            */
    UINT32 srcblk_ack_status_cfc : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>Source Blocking Status: Current state of
                               pm_block_ack_UnnnH&nbsp;(output of SPK to PMA
                               logic).</p>
                            */
    UINT32 srcblk_final_ack_status_cfc : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>Source Blocking Status: Current state of
                               pmblockack&nbsp;(state of SRCBLK_DIS_ACK_CFC
                               before the defeature gating)</p>
                            */
    UINT32 srcblk_no_dtf_data_drop : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>When set, do not drop data on DTF interface
                               (likely will hang DTF fabric if used.)</p>
                            */
    UINT32 srcblk_no_dtf_sync : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>When set, do not send upstream sync when
                               fsta_pm_block_req is deasserted after SPK has
                               asserted asta_pm_block_ack.</p>
                            */
    UINT32 rsvd : 12;

                            /* Bits[30:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_gate_ovrd : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit allows for the override of gated clocks
                               in Sierra Peak. Intended for debug use only
                            */

  } Bits;
  UINT32 Data;

} SPK_CLOCK_CONTROL_SPK_MAIN_STRUCT;

/* SPK_LBAR_SPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20058f50)                                                      */
/*      SPRB0 (0x20058f50)                                                      */
/*      SPRHBM (0x20058f50)                                                     */
/*      SPRC0 (0x20058f50)                                                      */
/*      SPRMCC (0x20058f50)                                                     */
/*      SPRUCC (0x20058f50)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* 
	Specifies the lower 32b of the base address for the memory block to be used by this instance. 
	
*/


#define SPK_LBAR_SPK_MAIN_REG 0x20028F50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_address : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the lower 32b of the starting address
                               of the trace buffer to which the AGL will store
                               its data. The value programmed in this field is
                               shifted left by 12 bits to create the physical
                               address [43:12]. Due to this shift, the starting
                               address is restricted to start on a 4-kbyte
                               boundary.
                            */

  } Bits;
  UINT32 Data;

} SPK_LBAR_SPK_MAIN_STRUCT;

/* SPK_UBAR_SPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20058f54)                                                      */
/*      SPRB0 (0x20058f54)                                                      */
/*      SPRHBM (0x20058f54)                                                     */
/*      SPRC0 (0x20058f54)                                                      */
/*      SPRMCC (0x20058f54)                                                     */
/*      SPRUCC (0x20058f54)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* 
	Specifies the upper 20b of the base address for the memory block to be used by this instance. 
	
*/


#define SPK_UBAR_SPK_MAIN_REG 0x20028F54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_address : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the upper 20b of the starting address
                               of the trace buffer to which the AGL will store
                               its data. The value programmed in this field is
                               shifted left by 12 bits to create the physical
                               address [63:44]. Due to this shift, the starting
                               address is restricted to start on a 4-kbyte
                               boundary.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_UBAR_SPK_MAIN_STRUCT;

/* SPK_SIZE_SPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20058f58)                                                      */
/*      SPRB0 (0x20058f58)                                                      */
/*      SPRHBM (0x20058f58)                                                     */
/*      SPRC0 (0x20058f58)                                                      */
/*      SPRMCC (0x20058f58)                                                     */
/*      SPRUCC (0x20058f58)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* 
	Specifies the size (in 4k increments) of the memory block to be used by this instance. 
	
*/


#define SPK_SIZE_SPK_MAIN_REG 0x20028F58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 size_of_mem_buf : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the size of the system memory buffer
                               for the address logic in terms of 4kB pages
                               (bits 43:12). By shifting the value in this
                               field left 12 bits, the result is this field
                               multiplied by 4 kBytes. A value of 0h is
                               undefined.
                            */

  } Bits;
  UINT32 Data;

} SPK_SIZE_SPK_MAIN_STRUCT;

/* SPK_LMWP_SPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20058f5c)                                                      */
/*      SPRB0 (0x20058f5c)                                                      */
/*      SPRHBM (0x20058f5c)                                                     */
/*      SPRC0 (0x20058f5c)                                                      */
/*      SPRMCC (0x20058f5c)                                                     */
/*      SPRUCC (0x20058f5c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* 
	The lower 32 bits of the current write pointer for the Address Generation Logic (AGL).
	
*/


#define SPK_LMWP_SPK_MAIN_REG 0x20028F5C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lower_memwr_ptr : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Address Generation Logic Lower Memory Write
                               Pointer
                            */

  } Bits;
  UINT32 Data;

} SPK_LMWP_SPK_MAIN_STRUCT;

/* SPK_UMWP_SPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20058f60)                                                      */
/*      SPRB0 (0x20058f60)                                                      */
/*      SPRHBM (0x20058f60)                                                     */
/*      SPRC0 (0x20058f60)                                                      */
/*      SPRMCC (0x20058f60)                                                     */
/*      SPRUCC (0x20058f60)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* 
	The upper 32 bits of the current write pointer for the Address Generation Logic (AGL).
	
*/


#define SPK_UMWP_SPK_MAIN_REG 0x20028F60

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upper_memwr_ptr : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Address Generation Logic Upper Memory Write
                               Pointer
                            */

  } Bits;
  UINT32 Data;

} SPK_UMWP_SPK_MAIN_STRUCT;

/* SPK_TIMESTAMP_FREQ_OVR_SPK_MAIN_REG supported on:                            */
/*      SPRA0 (0x20058f68)                                                      */
/*      SPRB0 (0x20058f68)                                                      */
/*      SPRHBM (0x20058f68)                                                     */
/*      SPRC0 (0x20058f68)                                                      */
/*      SPRMCC (0x20058f68)                                                     */
/*      SPRUCC (0x20058f68)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* 
    This register is analogous to NPK's FREQ_OVR (GTH_FREQ) register.  This register's value come from the sampling of a strap (SPK_TIMESTAMP_FREQ) at the deassertion of reset. This should not be overridden by software in a normal case. If it is to be overridden, it must be done while tracing is disabled.
	
*/


#define SPK_TIMESTAMP_FREQ_OVR_SPK_MAIN_REG 0x20028F68

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ts_counter_freq : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Timestamp counter frequency. Defined to be
                               crystal clock frequency (in Hertz) * 128.
                            */

  } Bits;
  UINT32 Data;

} SPK_TIMESTAMP_FREQ_OVR_SPK_MAIN_STRUCT;

/* DTFERR1_SPK_MAIN_REG supported on:                                           */
/*      SPRA0 (0x20058f6c)                                                      */
/*      SPRB0 (0x20058f6c)                                                      */
/*      SPRHBM (0x20058f6c)                                                     */
/*      SPRC0 (0x20058f6c)                                                      */
/*      SPRMCC (0x20058f6c)                                                     */
/*      SPRUCC (0x20058f6c)                                                     */
/* Register default value on SPRA0: 0x00000480                                  */
/* Register default value on SPRB0: 0x00000480                                  */
/* Register default value on SPRHBM: 0x00000480                                 */
/* Register default value on SPRC0: 0x00000480                                  */
/* Register default value on SPRMCC: 0x00000480                                 */
/* Register default value on SPRUCC: 0x00000480                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports how many packets were received from DTF with protocol&nbsp;violations.
*/


#define DTFERR1_SPK_MAIN_REG 0x20028F6C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_cnt : 8;

                            /* Bits[7:0], Access Type=RW/1C/V, default=0x00000080*/

                            /*
                               <p>This field stores the number of DTF packets
                               received&nbsp;with DTF protocol violations.
                               See&nbsp;section 3.2.8.10.</p>
                            */
    UINT32 dtf_errcnt_of : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               <p>This&nbsp;bit is set if/when
                               the&nbsp;DTF_ERR_CNT value exceeds 255,&nbsp;and
                               will remain set until explicitly&nbsp;cleared
                               with a write (write 1).</p>
                            */
    UINT32 dtf_debug : 1;

                            /* Bits[9:9], Access Type=RW/V, default=0x00000000*/

                            /*
                               <p>Turns the error&nbsp;registers into a
                               &#8220;latch next DTF&nbsp;transaction&#8221;
                               set of registers. This is&nbsp;useful for debug.
                               When this bit is&nbsp;set, the DTFERRn registers
                               will&nbsp;latch the next incoming
                               DTF&nbsp;transaction. Hardware will
                               clear&nbsp;this bit when a transaction
                               has&nbsp;been latched. Since software
                               can&nbsp;also clear this bit, no
                               software&nbsp;should rely on this bit
                               for&nbsp;determining if a transaction
                               has&nbsp;been latched into the registers.</p>
                            */
    UINT32 dtf_debug_sts : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000001*/

                            /*
                               <p>When set,&nbsp;indicates a packet from DTF
                               was&nbsp;latched into the DTFERRn registers.This
                               bit is set when DTF_DEBUG is&nbsp;set, and the
                               dtfActivesignal&nbsp;asserts. Software can
                               &nbsp;differentiate&nbsp;between a valid packet
                               and an&nbsp;error packet using this bit and
                               the&nbsp;DTF_ERR bit. If DTF_ERR is
                               set,&nbsp;the packet was received with
                               at&nbsp;least one error; if
                               only&nbsp;DTF_DEBUG_STS is set,
                               the&nbsp;transaction had no errors)</p>
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR1_SPK_MAIN_STRUCT;

/* DTFERR2_SPK_MAIN_REG supported on:                                           */
/*      SPRA0 (0x20058f70)                                                      */
/*      SPRB0 (0x20058f70)                                                      */
/*      SPRHBM (0x20058f70)                                                     */
/*      SPRC0 (0x20058f70)                                                      */
/*      SPRMCC (0x20058f70)                                                     */
/*      SPRUCC (0x20058f70)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports how many packets were received from DTF with protocol&nbsp;violations.
*/


#define DTFERR2_SPK_MAIN_REG 0x20028F70

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtfcerr : 8;

                            /* Bits[7:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               <p>This field stores the&nbsp;Channel ID number
                               of first&nbsp;packet with DTF
                               protocol&nbsp;violation. See
                               section&nbsp;3.2.8.10.</p>
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dtfmerr : 8;

                            /* Bits[23:16], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               <p>This field stores the Master ID number of
                               first&nbsp;packet with DTF
                               protocol&nbsp;violation.</p>
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR2_SPK_MAIN_STRUCT;

/* DTFERR3_SPK_MAIN_REG supported on:                                           */
/*      SPRA0 (0x20058f74)                                                      */
/*      SPRB0 (0x20058f74)                                                      */
/*      SPRHBM (0x20058f74)                                                     */
/*      SPRC0 (0x20058f74)                                                      */
/*      SPRMCC (0x20058f74)                                                     */
/*      SPRUCC (0x20058f74)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports how many packets were received from DTF with protocol&nbsp;violations.
*/


#define DTFERR3_SPK_MAIN_REG 0x20028F74

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_type_err : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>This bit&nbsp;field holds the actual value
                               seen&nbsp;on the type input when a
                               DTF&nbsp;protocol error violation was seen.</p>
                            */
    UINT32 dtf_err : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               <p>This bit is set&nbsp;when a DTF packet is
                               received&nbsp;with a protocol violation. This
                               bit is&nbsp;present in this register so
                               that&nbsp;software can detect an error
                               and&nbsp;capture some of the relevant
                               data&nbsp;in a single register.&nbsp;Writing a 1
                               to this register will rearm the error latching
                               circuitry for&nbsp;DTFERR registers except
                               DTFERR1&nbsp;(the DTF Error counter
                               register).&nbsp;The DTFERR register values
                               will&nbsp;not be updated again until
                               the&nbsp;DTF_ERR bit is set.</p>
                            */
    UINT32 dtferr_clearall : 1;

                            /* Bits[9:9], Access Type=RW/V, default=0x00000000*/

                            /*
                               <p>This bit provides a single&nbsp;place for
                               software to clear all&nbsp;DTFErr related
                               registers&nbsp;(specifically, this is added to
                               clear&nbsp;the DTF error counter from
                               within&nbsp;the DTFERR2 register).
                               Software&nbsp;should write a 1 to this bit to
                               clear&nbsp;all DTF Error related registers.
                               This&nbsp;bit is self-clearing, and will
                               always&nbsp;read as 0.</p>
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR3_SPK_MAIN_STRUCT;

/* DTFERR4_SPK_MAIN_REG supported on:                                           */
/*      SPRA0 (0x20058f78)                                                      */
/*      SPRB0 (0x20058f78)                                                      */
/*      SPRHBM (0x20058f78)                                                     */
/*      SPRC0 (0x20058f78)                                                      */
/*      SPRMCC (0x20058f78)                                                     */
/*      SPRUCC (0x20058f78)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports how many packets were received from DTF with protocol&nbsp;violations.
*/


#define DTFERR4_SPK_MAIN_REG 0x20028F78

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_datalo : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>This field stores the lower 32b of Data Bus
                               for DTF packet in error.</p>
                            */

  } Bits;
  UINT32 Data;

} DTFERR4_SPK_MAIN_STRUCT;

/* DTFERR5_SPK_MAIN_REG supported on:                                           */
/*      SPRA0 (0x20058f7c)                                                      */
/*      SPRB0 (0x20058f7c)                                                      */
/*      SPRHBM (0x20058f7c)                                                     */
/*      SPRC0 (0x20058f7c)                                                      */
/*      SPRMCC (0x20058f7c)                                                     */
/*      SPRUCC (0x20058f7c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports how many packets were received from DTF with protocol&nbsp;violations.
*/


#define DTFERR5_SPK_MAIN_REG 0x20028F7C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_datahi : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               <p>This field stores the high 32b of Data Bus
                               for DTF packet in error.&nbsp;</p>
                            */

  } Bits;
  UINT32 Data;

} DTFERR5_SPK_MAIN_STRUCT;

/* SPK_SNC_BASE_1_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f80)                                                      */
/*      SPRB0 (0x20058f80)                                                      */
/*      SPRHBM (0x20058f80)                                                     */
/*      SPRC0 (0x20058f80)                                                      */
/*      SPRMCC (0x20058f80)                                                     */
/*      SPRUCC (0x20058f80)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register 1
*/


#define SPK_SNC_BASE_1_SPK_MAIN_REG 0x20028F80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC Base address [45:30].&nbsp;Since there are
                               64 possible MeshIDs, each agent's LUT is&nbsp;a
                               64 entry table indexed by MeshID.
                            */
    UINT32 twolm_mask : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /* 2LM_Mask[45:40] */
    UINT32 hi_twolm_mask : 6;

                            /* Bits[27:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               2LM_Mask [51:46]&nbsp;(SPR only, any core that
                               only&nbsp;supports 46 address bits can treat
                               this as&nbsp;reserved)
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_BASE_1_SPK_MAIN_STRUCT;

/* SPK_SNC_BASE_2_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f84)                                                      */
/*      SPRB0 (0x20058f84)                                                      */
/*      SPRHBM (0x20058f84)                                                     */
/*      SPRC0 (0x20058f84)                                                      */
/*      SPRMCC (0x20058f84)                                                     */
/*      SPRUCC (0x20058f84)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register 2
*/


#define SPK_SNC_BASE_2_SPK_MAIN_REG 0x20028F84

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC Base address [45:30].&nbsp;This is the 1G
                               aligned base&nbsp;address of Cluster 1, which in
                               turn indicates the&nbsp;limit of Cluster 0.
                            */
    UINT32 rsvd1 : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* RSVD1 */
    UINT32 twolm_mask : 12;

                            /* Bits[28:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               2LM_Mask[39:28], For CNX TwoLM_Mask[28] is
                               always 0
                            */
    UINT32 rsvd2 : 3;

                            /* Bits[31:29], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_BASE_2_SPK_MAIN_STRUCT;

/* SPK_SNC_BASE_3_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f88)                                                      */
/*      SPRB0 (0x20058f88)                                                      */
/*      SPRHBM (0x20058f88)                                                     */
/*      SPRC0 (0x20058f88)                                                      */
/*      SPRMCC (0x20058f88)                                                     */
/*      SPRUCC (0x20058f88)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register 3
*/


#define SPK_SNC_BASE_3_SPK_MAIN_REG 0x20028F88

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC Base address [45:30].&nbsp;This is the base
                               1G aligned&nbsp;address of Cluster N, which in
                               turn indicates the&nbsp;limit of Cluster N-1.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_BASE_3_SPK_MAIN_STRUCT;

/* SPK_SNC_BASE_4_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f8c)                                                      */
/*      SPRB0 (0x20058f8c)                                                      */
/*      SPRHBM (0x20058f8c)                                                     */
/*      SPRC0 (0x20058f8c)                                                      */
/*      SPRMCC (0x20058f8c)                                                     */
/*      SPRUCC (0x20058f8c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register 4
*/


#define SPK_SNC_BASE_4_SPK_MAIN_REG 0x20028F8C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC Base address [45:30].&nbsp;This is the base
                               1G aligned&nbsp;address of Cluster N, which in
                               turn indicates the&nbsp;limit of Cluster N-1.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_BASE_4_SPK_MAIN_STRUCT;

/* SPK_SNC_BASE_5_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f90)                                                      */
/*      SPRB0 (0x20058f90)                                                      */
/*      SPRHBM (0x20058f90)                                                     */
/*      SPRC0 (0x20058f90)                                                      */
/*      SPRMCC (0x20058f90)                                                     */
/*      SPRUCC (0x20058f90)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register 5
*/


#define SPK_SNC_BASE_5_SPK_MAIN_REG 0x20028F90

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC Base address [45:30].&nbsp;This is the base
                               1G aligned&nbsp;address of Cluster N, which in
                               turn indicates the&nbsp;limit of Cluster N-1.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_BASE_5_SPK_MAIN_STRUCT;

/* SPK_SNC_UPPER_BASE_SPK_MAIN_REG supported on:                                */
/*      SPRA0 (0x20058f94)                                                      */
/*      SPRB0 (0x20058f94)                                                      */
/*      SPRHBM (0x20058f94)                                                     */
/*      SPRC0 (0x20058f94)                                                      */
/*      SPRMCC (0x20058f94)                                                     */
/*      SPRUCC (0x20058f94)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Base SNC Configuration Register for Upper bits of Base Address registers (not needed until SPR&nbsp;time frame).
*/


#define SPK_SNC_UPPER_BASE_SPK_MAIN_REG 0x20028F94

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upper_base_1 : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /* SNC base address&nbsp;[51:46] for first cluster */
    UINT32 upper_base_2 : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000000*/

                            /* SNC base address[51:46] for second cluster */
    UINT32 upper_base_3 : 6;

                            /* Bits[17:12], Access Type=RW/P, default=0x00000000*/

                            /* SNC base address[51:46] for third cluster */
    UINT32 upper_base_4 : 6;

                            /* Bits[23:18], Access Type=RW/P, default=0x00000000*/

                            /* SNC base address[51:46] for fourth cluster */
    UINT32 upper_base_5 : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               <p>SNC base address[51:46] for fifth cluster</p>
                               <p>Note: There is no fifth cluster, but this
                               register is&nbsp;<br> still needed because the
                               base address of the fifth&nbsp;<br> cluster sets
                               the limit address for the forth cluster.</p>
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000000*/

                            /* RSVD */

  } Bits;
  UINT32 Data;

} SPK_SNC_UPPER_BASE_SPK_MAIN_STRUCT;

/* SPK_SNC_CONFIG_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058f98)                                                      */
/*      SPRB0 (0x20058f98)                                                      */
/*      SPRHBM (0x20058f98)                                                     */
/*      SPRC0 (0x20058f98)                                                      */
/*      SPRMCC (0x20058f98)                                                     */
/*      SPRUCC (0x20058f98)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Primary SNC Control Register
*/


#define SPK_SNC_CONFIG_SPK_MAIN_REG 0x20028F98

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 full_snc_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable for SNC.&nbsp;This will always be set to
                               0 in the&nbsp;core copy of this register,
                               because the core on&nbsp;10nm is only ever
                               expected to produce the SNC&nbsp;indication (see
                               below). The uncore copy, on the&nbsp;other hand,
                               may have this set to 1.
                            */
    UINT32 snc_ind_enable : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               SNC range check enable.Produces an&nbsp;SNC_ID,
                               and an SNLocal indication, but the&nbsp;CHA_ID
                               from the hash is not modified.
                            */
    UINT32 num_clusters : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /* Number of SNC Clusters */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_SNC_CONFIG_SPK_MAIN_STRUCT;

/* SPK_UNCORE_SNC_CONFIG_SPK_MAIN_REG supported on:                             */
/*      SPRA0 (0x20058f9c)                                                      */
/*      SPRB0 (0x20058f9c)                                                      */
/*      SPRHBM (0x20058f9c)                                                     */
/*      SPRC0 (0x20058f9c)                                                      */
/*      SPRMCC (0x20058f9c)                                                     */
/*      SPRUCC (0x20058f9c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Primary UNCORE SNC Control Register.
*/


#define SPK_UNCORE_SNC_CONFIG_SPK_MAIN_REG 0x20028F9C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_cha_per_cluster : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /* Num CHA Per Cluster/Base CHA for Cluster 0 */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_1 : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 1 */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_2 : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 2 */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 base_cha_cluster_3 : 7;

                            /* Bits[30:24], Access Type=RW/P, default=0x00000000*/

                            /* Base CHA for Cluster 3 */
    UINT32 rsvd_4 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_UNCORE_SNC_CONFIG_SPK_MAIN_STRUCT;

/* SPK_UMA_CLUSTER_SPK_MAIN_REG supported on:                                   */
/*      SPRA0 (0x10058fa0)                                                      */
/*      SPRB0 (0x10058fa0)                                                      */
/*      SPRHBM (0x10058fa0)                                                     */
/*      SPRC0 (0x10058fa0)                                                      */
/*      SPRMCC (0x10058fa0)                                                     */
/*      SPRUCC (0x10058fa0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Primary register to control and configure UMA Clustering.
*/


#define SPK_UMA_CLUSTER_SPK_MAIN_REG 0x20018FA0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 uma_cluster_enable : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable for UMA clustering.&nbsp;Overrides
                               FULL_SNC_ENABLE and&nbsp;SNC_IND_ENABLE if set.
                            */
    UINT16 defeature_xor : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit for XOR defeature mode. By default,
                               UMA mode clustering uses
                               Addr[26:25]^Addr[18:17]^Addr[12:11]^Addr[9:8].
                               But when this bit is set to 1, Addr[12:11] are
                               used.
                            */
    UINT16 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} SPK_UMA_CLUSTER_SPK_MAIN_STRUCT;







/* SPK_SMCR0_SPK_MAIN_REG supported on:                                         */
/*      SPRA0 (0x10058fc0)                                                      */
/*      SPRB0 (0x10058fc0)                                                      */
/*      SPRHBM (0x10058fc0)                                                     */
/*      SPRC0 (0x10058fc0)                                                      */
/*      SPRMCC (0x10058fc0)                                                     */
/*      SPRUCC (0x10058fc0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* 
    This register is analogous to NPK's SMCR0 register, with support for a single port.
	
*/


#define SPK_SMCR0_SPK_MAIN_REG 0x20018FC0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 syncf0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Sync Packet Frequency. Specifies the number of
                               datasets between maintenance packets. A value of
                               0 (zero) turns off maintenance packet generation
                               for the port. However, an initial maintenance
                               packet will be sent at the start of the trace
                               and when the memory buffer wraps so that
                               software can successfully reconstruct the trace
                               data.
                            */

  } Bits;
  UINT16 Data;

} SPK_SMCR0_SPK_MAIN_STRUCT;

/* SPK_TIMESTAMP_FREQ_OVR_UP_SPK_MAIN_REG supported on:                         */
/*      SPRA0 (0x20058fc4)                                                      */
/*      SPRB0 (0x20058fc4)                                                      */
/*      SPRHBM (0x20058fc4)                                                     */
/*      SPRC0 (0x20058fc4)                                                      */
/*      SPRMCC (0x20058fc4)                                                     */
/*      SPRUCC (0x20058fc4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is analogous to NPK's FREQ_OVR_UP (GTH_FREQ) register. This register's value come from the sampling of a strap (SPK_TIMESTAMP_FREQ) at the deassertion of reset. This should not be overridden by software in a normal case. If it is to be overridden, it must be done while tracing is disabled.
*/


#define SPK_TIMESTAMP_FREQ_OVR_UP_SPK_MAIN_REG 0x20028FC4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ts_counter_freq_up : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Upper 8bits of the Timestamp counter frequency.
                               Defined to be crystal clock frequency (in Hertz)
                               * 128.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPK_TIMESTAMP_FREQ_OVR_UP_SPK_MAIN_STRUCT;

/* SPK_OS_W_CP_LO_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058fc8)                                                      */
/*      SPRB0 (0x20058fc8)                                                      */
/*      SPRHBM (0x20058fc8)                                                     */
/*      SPRC0 (0x20058fc8)                                                      */
/*      SPRMCC (0x20058fc8)                                                     */
/*      SPRUCC (0x20058fc8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define SPK_OS_W_CP_LO_SPK_MAIN_REG 0x20028FC8

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

} SPK_OS_W_CP_LO_SPK_MAIN_STRUCT;

/* SPK_OS_W_CP_HI_SPK_MAIN_REG supported on:                                    */
/*      SPRA0 (0x20058fcc)                                                      */
/*      SPRB0 (0x20058fcc)                                                      */
/*      SPRHBM (0x20058fcc)                                                     */
/*      SPRC0 (0x20058fcc)                                                      */
/*      SPRMCC (0x20058fcc)                                                     */
/*      SPRUCC (0x20058fcc)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define SPK_OS_W_CP_HI_SPK_MAIN_REG 0x20028FCC

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

} SPK_OS_W_CP_HI_SPK_MAIN_STRUCT;

/* SPK_OS_W_RAC_LO_SPK_MAIN_REG supported on:                                   */
/*      SPRA0 (0x20058fd0)                                                      */
/*      SPRB0 (0x20058fd0)                                                      */
/*      SPRHBM (0x20058fd0)                                                     */
/*      SPRC0 (0x20058fd0)                                                      */
/*      SPRMCC (0x20058fd0)                                                     */
/*      SPRUCC (0x20058fd0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define SPK_OS_W_RAC_LO_SPK_MAIN_REG 0x20028FD0

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

} SPK_OS_W_RAC_LO_SPK_MAIN_STRUCT;

/* SPK_OS_W_RAC_HI_SPK_MAIN_REG supported on:                                   */
/*      SPRA0 (0x20058fd4)                                                      */
/*      SPRB0 (0x20058fd4)                                                      */
/*      SPRHBM (0x20058fd4)                                                     */
/*      SPRC0 (0x20058fd4)                                                      */
/*      SPRMCC (0x20058fd4)                                                     */
/*      SPRUCC (0x20058fd4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define SPK_OS_W_RAC_HI_SPK_MAIN_REG 0x20028FD4

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

} SPK_OS_W_RAC_HI_SPK_MAIN_STRUCT;

/* SPK_OS_W_WAC_LO_SPK_MAIN_REG supported on:                                   */
/*      SPRA0 (0x20058fd8)                                                      */
/*      SPRB0 (0x20058fd8)                                                      */
/*      SPRHBM (0x20058fd8)                                                     */
/*      SPRC0 (0x20058fd8)                                                      */
/*      SPRMCC (0x20058fd8)                                                     */
/*      SPRUCC (0x20058fd8)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define SPK_OS_W_WAC_LO_SPK_MAIN_REG 0x20028FD8

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

} SPK_OS_W_WAC_LO_SPK_MAIN_STRUCT;

/* SPK_OS_W_WAC_HI_SPK_MAIN_REG supported on:                                   */
/*      SPRA0 (0x20058fdc)                                                      */
/*      SPRB0 (0x20058fdc)                                                      */
/*      SPRHBM (0x20058fdc)                                                     */
/*      SPRC0 (0x20058fdc)                                                      */
/*      SPRMCC (0x20058fdc)                                                     */
/*      SPRUCC (0x20058fdc)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRB0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRHBM Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRC0 Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK         */
/* SPRMCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* SPRUCC Register File:    sprsp_top/spk_reg[0]/spk_main_crnode/MEM_SPK        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define SPK_OS_W_WAC_HI_SPK_MAIN_REG 0x20028FDC

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

} SPK_OS_W_WAC_HI_SPK_MAIN_STRUCT;






#endif /* _SPK_MAIN_h */
