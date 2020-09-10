
/** @file
  MSM_OOB.h

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
/* PCIE_SPEC                                                                    */
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
/* PCIE_SPEC                                                                    */
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
/* PCIE_SPEC                                                                    */
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
/* PCIE_SPEC                                                                    */
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
/* PCIE_SPEC                                                                    */
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
/* PCIE_SPEC                                                                    */
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


#ifndef _MSM_OOB_h
#define _MSM_OOB_h
#include <Base.h>

/* PCI_CFG_DEVICE_VENDOR_ID_MSM_OOB_REG supported on:                           */
/*      SPRA0 (0x20818000)                                                      */
/*      SPRB0 (0x20818000)                                                      */
/*      SPRHBM (0x20818000)                                                     */
/*      SPRC0 (0x20818000)                                                      */
/*      SPRMCC (0x20818000)                                                     */
/*      SPRUCC (0x20818000)                                                     */
/* Register default value on SPRA0: 0x09A68086                                  */
/* Register default value on SPRB0: 0x09A68086                                  */
/* Register default value on SPRHBM: 0x09A68086                                 */
/* Register default value on SPRC0: 0x09A68086                                  */
/* Register default value on SPRMCC: 0x09A68086                                 */
/* Register default value on SPRUCC: 0x09A68086                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Device ID and Vendor ID Registers
*/


#define PCI_CFG_DEVICE_VENDOR_ID_MSM_OOB_REG 0x17020000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vendor_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               Vendor ID. This field identifies the
                               manufacturer of the device. Valid vendor
                               identifiers are allocated by the PCI SIG to
                               ensure uniqueness.
                            */
    UINT32 device_id : 16;

                            /* Bits[31:16], Access Type=RO, default=0x000009A6*/

                            /*
                               Device ID. This field identifies the particular
                               device. This identifier is allocated by the
                               vendor
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_DEVICE_VENDOR_ID_MSM_OOB_STRUCT;

/* PCI_CFG_STATUS_COMMAND_MSM_OOB_REG supported on:                             */
/*      SPRA0 (0x20818004)                                                      */
/*      SPRB0 (0x20818004)                                                      */
/*      SPRHBM (0x20818004)                                                     */
/*      SPRC0 (0x20818004)                                                      */
/*      SPRMCC (0x20818004)                                                     */
/*      SPRUCC (0x20818004)                                                     */
/* Register default value on SPRA0: 0x00100006                                  */
/* Register default value on SPRB0: 0x00100000                                  */
/* Register default value on SPRHBM: 0x00100000                                 */
/* Register default value on SPRC0: 0x00100000                                  */
/* Register default value on SPRMCC: 0x00100000                                 */
/* Register default value on SPRUCC: 0x00100000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Status and Command registers
*/


#define PCI_CFG_STATUS_COMMAND_MSM_OOB_REG 0x17020004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_space_ena : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               IO Space Enable. IO Space access is not
                               supported. Setting this bit will not enable IO
                               Space access.
                            */
    UINT32 memory_space_ena : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Memory Space Enable. Controls the device's
                               response to Memory Space accesses. A value of 0
                               disabled the device response. A value of 1
                               allows the device to respond to Memory Space
                               accesses.
                            */
    UINT32 bus_master_ena : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Bus Master Enable. Controls the ability of a PCI
                               Express Endpoint to issue Memory and I/O
                               Read/Write Requests. When this bit is Set, the
                               PCI Express Function is allowed to issue Memory
                               or I/O Requests. When this bit is Clear, the PCI
                               Express Function is not allowed to issue any
                               Memory or I/O Requests. Note that as MSI/MSI-X
                               interrupt Messages are in-band memory writes,
                               setting the Bus Master Enable bit to 0 disables
                               MSI/MSI-X interrupt Messages as well. Requests
                               other than Memory or I/O Requests are not
                               controlled by this bit.
                            */
    UINT32 special_cycle_ena : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate. Does not apply,
                               hardwired to 0.
                            */
    UINT32 mem_wrt_inval : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate. Does not apply,
                               hardwired to 0.
                            */
    UINT32 vga_palette_snoop : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               VGA Palette Snoop. Does not apply, hardwired to
                               0
                            */
    UINT32 parity_error_resp : 1;

                            /* Bits[6:6], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Parity Error Response. This bit controls the
                               logging of poisoned TLPs in the Master Data
                               Parity Error bit in the Status register.
                            */
    UINT32 idsel_step : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               IDSEL Stepping/Wait Cycle Control- Does not
                               apply, hardwired to 0.
                            */
    UINT32 serr_enable : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SERR# Enable. When Set, this bit enables
                               reporting of Non-fatal and Fatal errors detected
                               by the Function to the Root Complex. Note that
                               errors are reported if enabled either through
                               this bit or through the PCI Express specific
                               bits in the Device Control register
                            */
    UINT32 fast_b2b_trans_ena : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back Transactions Enable- Does not
                               apply, must be hardwired to 0.
                            */
    UINT32 interrupt_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Disable Controls the ability of a PCI
                               Express Function to generate INTx interrupts.
                               When Set, Functions are prevented from asserting
                               INTx interrupts. Any INTx emulation interrupts
                               already asserted by the Function must be
                               deasserted when this bit is Set.
                            */
    UINT32 reserved_18_11 : 8;

                            /* Bits[18:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 interrupt_status : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Status. When Set, indicates that an
                               INTx emulation interrupt is pending internally
                               in the Function.
                            */
    UINT32 capabilities_list : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Capabilities List Indicates the presence of an
                               Extended Capability list item. Hardwired to 1.
                            */
    UINT32 is66mhz_capable : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               66 MHz Capable. Does not apply, must be
                               hardwired to 0.
                            */
    UINT32 reserved_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 fast_b2b_trans_cap : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back Transactions Capable- Does not
                               apply, must be hardwired to 0.
                            */
    UINT32 mas_data_par_err : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Master Data Parity Error. This bit is Set by an
                               Endpoint Function if the Parity Error Response
                               bit in the Command register is 1 and either of
                               the following two conditions occurs: Endpoint
                               receives a Poisoned Completion Endpoint
                               transmits a Poisoned Request
                            */
    UINT32 devsel_timing : 2;

                            /* Bits[26:25], Access Type=RO, default=0x00000000*/

                            /*
                               DEVSEL Timing- Does not apply, hardwired to
                               3'b00
                            */
    UINT32 sig_target_abort : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled Target Abort. This bit is Set when a
                               Function completes a Posted or NonPosted Request
                               as a Completer Abort error.
                            */
    UINT32 rec_target_abort : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Target Abort. This bit is Set when a
                               Requester receives a Completion with Completer
                               Abort Completion Status.
                            */
    UINT32 rec_master_abort : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Master Abort. This bit is Set when a
                               Requester receives a Completion with Unsupported
                               Request Completion Status.
                            */
    UINT32 sig_system_error : 1;

                            /* Bits[30:30], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled System Error. This bit is Set when a
                               Function sends an ERR_FATAL or ERR_NONFATAL
                               Message, and the SERR# Enable bit in the Command
                               register is 1.
                            */
    UINT32 parity_error : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Detected Parity Error. This bit is Set by a
                               Function whenever it receives a Poisoned TLP,
                               regardless of the state the Parity Error
                               Response bit in the Command Register.
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_STATUS_COMMAND_MSM_OOB_STRUCT;

/* PCI_CFG_CLASS_REVISION_MSM_OOB_REG supported on:                             */
/*      SPRA0 (0x20818008)                                                      */
/*      SPRB0 (0x20818008)                                                      */
/*      SPRHBM (0x20818008)                                                     */
/*      SPRC0 (0x20818008)                                                      */
/*      SPRMCC (0x20818008)                                                     */
/*      SPRUCC (0x20818008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Class Code and Revision ID registers
*/


#define PCI_CFG_CLASS_REVISION_MSM_OOB_REG 0x17020008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Revision ID. This register specifies a device
                               specific revision identifier. The value is
                               chosen by the vendor. This field should be
                               viewed as a vendor defined extension to the
                               Device ID.
                            */
    UINT32 class_code : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00088000*/

                            /*
                               Class Code. The Class Code register is used to
                               identify the generic function of the device and,
                               in some cases, a specific register level
                               programming interface.
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_CLASS_REVISION_MSM_OOB_STRUCT;

/* PCI_CFG_HEADER_CACHELINE_MSM_OOB_REG supported on:                           */
/*      SPRA0 (0x2081800c)                                                      */
/*      SPRB0 (0x2081800c)                                                      */
/*      SPRHBM (0x2081800c)                                                     */
/*      SPRC0 (0x2081800c)                                                      */
/*      SPRMCC (0x2081800c)                                                     */
/*      SPRUCC (0x2081800c)                                                     */
/* Register default value on SPRA0: 0x00800000                                  */
/* Register default value on SPRB0: 0x00800000                                  */
/* Register default value on SPRHBM: 0x00800000                                 */
/* Register default value on SPRC0: 0x00800000                                  */
/* Register default value on SPRMCC: 0x00800000                                 */
/* Register default value on SPRUCC: 0x00800000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIST, Header, Latency Timer, Cache Line Size Configuration Registers. 
*/


#define PCI_CFG_HEADER_CACHELINE_MSM_OOB_REG 0x1702000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_line_size : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Cache Line Size. Specifies the system cacheline
                               size in units of DWORDs. No impact to PCI.
                            */
    UINT32 mas_latency_tim : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Master Latency Timer. Does not apply, hardwired
                               to 0.
                            */
    UINT32 header_type : 7;

                            /* Bits[22:16], Access Type=RO, default=0x00000000*/

                            /* Header Type. Does not apply, hardwired to 0. */
    UINT32 device_type : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               Device Type. This bit is used to identify a
                               multi-function device. If the bit is 0, then the
                               device is single function.If the bit is 1, then
                               the device has multiple functions.
                            */
    UINT32 bist : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* This register controls BIST. */

  } Bits;
  UINT32 Data;

} PCI_CFG_HEADER_CACHELINE_MSM_OOB_STRUCT;

/* PCI_CFG_BAR0_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818010)                                                      */
/*      SPRB0 (0x20818010)                                                      */
/*      SPRHBM (0x20818010)                                                     */
/*      SPRC0 (0x20818010)                                                      */
/*      SPRMCC (0x20818010)                                                     */
/*      SPRUCC (0x20818010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register 0
*/


#define PCI_CFG_BAR0_MSM_OOB_REG 0x17020010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem_space : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space. When clear (zero), BAR defines a
                               memory space address range.
                            */
    UINT32 bar_type : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000000*/

                            /*
                               BAR Type. When set to 2'b-0, BAR defines a
                               32-bit address range and is used in conjunction
                               with BAR1
                            */
    UINT32 prefetch : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable. When set, address space is
                               prefetchable. Reads to this space have no side
                               effects.
                            */
    UINT32 base_addr_ro : 9;

                            /* Bits[12:4], Access Type=RW, default=0x00000000*/

                            /* Sets aperature .. 128 byte memory space */
    UINT32 base_addr : 19;

                            /* Bits[31:13], Access Type=RW, default=0x00000000*/

                            /* Base Address. Defines the lower base address */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR0_MSM_OOB_STRUCT;

/* PCI_CFG_BAR1_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818014)                                                      */
/*      SPRB0 (0x20818014)                                                      */
/*      SPRHBM (0x20818014)                                                     */
/*      SPRC0 (0x20818014)                                                      */
/*      SPRMCC (0x20818014)                                                     */
/*      SPRUCC (0x20818014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register1.
*/


#define PCI_CFG_BAR1_MSM_OOB_REG 0x17020014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR1_MSM_OOB_STRUCT;

/* PCI_CFG_BAR2_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818018)                                                      */
/*      SPRB0 (0x20818018)                                                      */
/*      SPRHBM (0x20818018)                                                     */
/*      SPRC0 (0x20818018)                                                      */
/*      SPRMCC (0x20818018)                                                     */
/*      SPRUCC (0x20818018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register2.
*/


#define PCI_CFG_BAR2_MSM_OOB_REG 0x17020018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address. reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR2_MSM_OOB_STRUCT;

/* PCI_CFG_BAR3_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x2081801c)                                                      */
/*      SPRB0 (0x2081801c)                                                      */
/*      SPRHBM (0x2081801c)                                                     */
/*      SPRC0 (0x2081801c)                                                      */
/*      SPRMCC (0x2081801c)                                                     */
/*      SPRUCC (0x2081801c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register3.
*/


#define PCI_CFG_BAR3_MSM_OOB_REG 0x1702001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address. reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR3_MSM_OOB_STRUCT;

/* PCI_CFG_BAR4_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818020)                                                      */
/*      SPRB0 (0x20818020)                                                      */
/*      SPRHBM (0x20818020)                                                     */
/*      SPRC0 (0x20818020)                                                      */
/*      SPRMCC (0x20818020)                                                     */
/*      SPRUCC (0x20818020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register4.
*/


#define PCI_CFG_BAR4_MSM_OOB_REG 0x17020020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address. reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR4_MSM_OOB_STRUCT;

/* PCI_CFG_BAR5_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818024)                                                      */
/*      SPRB0 (0x20818024)                                                      */
/*      SPRHBM (0x20818024)                                                     */
/*      SPRC0 (0x20818024)                                                      */
/*      SPRMCC (0x20818024)                                                     */
/*      SPRUCC (0x20818024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register5.
*/


#define PCI_CFG_BAR5_MSM_OOB_REG 0x17020024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address. reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR5_MSM_OOB_STRUCT;

/* PCI_CFG_SUBSYSTEM_VENDOR_MSM_OOB_REG supported on:                           */
/*      SPRA0 (0x2081802c)                                                      */
/*      SPRB0 (0x2081802c)                                                      */
/*      SPRHBM (0x2081802c)                                                     */
/*      SPRC0 (0x2081802c)                                                      */
/*      SPRMCC (0x2081802c)                                                     */
/*      SPRUCC (0x2081802c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Defines the Subsystem ID and Subsystem Vendor ID. 
*/


#define PCI_CFG_SUBSYSTEM_VENDOR_MSM_OOB_REG 0x1702002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sub_vendor_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               Specifies the Subsystem Vendor ID assigned by
                               PCI SIG.
                            */
    UINT32 subsystem_id : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*
                               Specifies the Subsystem ID assigned by device
                               manufacturer.
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_SUBSYSTEM_VENDOR_MSM_OOB_STRUCT;

/* PCI_CFG_EXPANSION_ROM_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x20818030)                                                      */
/*      SPRB0 (0x20818030)                                                      */
/*      SPRHBM (0x20818030)                                                     */
/*      SPRC0 (0x20818030)                                                      */
/*      SPRMCC (0x20818030)                                                     */
/*      SPRUCC (0x20818030)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Expansion ROM base address and size information.
*/


#define PCI_CFG_EXPANSION_ROM_MSM_OOB_REG 0x17020030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Expanstion ROM Enable. When set to 0, Expansion
                               ROM is disabled.
                            */
    UINT32 reserved_10_1 : 10;

                            /* Bits[10:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 base_addr : 21;

                            /* Bits[31:11], Access Type=RO, default=0x00000000*/

                            /* Expansion ROM base address. */

  } Bits;
  UINT32 Data;

} PCI_CFG_EXPANSION_ROM_MSM_OOB_STRUCT;

/* PCI_CFG_CAPABILITIES_MSM_OOB_REG supported on:                               */
/*      SPRA0 (0x20818034)                                                      */
/*      SPRB0 (0x20818034)                                                      */
/*      SPRHBM (0x20818034)                                                     */
/*      SPRC0 (0x20818034)                                                      */
/*      SPRMCC (0x20818034)                                                     */
/*      SPRUCC (0x20818034)                                                     */
/* Register default value on SPRA0: 0x00000090                                  */
/* Register default value on SPRB0: 0x00000090                                  */
/* Register default value on SPRHBM: 0x00000090                                 */
/* Register default value on SPRC0: 0x00000090                                  */
/* Register default value on SPRMCC: 0x00000090                                 */
/* Register default value on SPRUCC: 0x00000090                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Pointer to the first Expanded Capabilities structure.
*/


#define PCI_CFG_CAPABILITIES_MSM_OOB_REG 0x17020034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000090*/

                            /*
                               Capabilities Pointer. Pointer to the Expanded
                               Capabilities structure.
                            */
    UINT32 reserved_31_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_CAPABILITIES_MSM_OOB_STRUCT;

/* PCI_CFG_INTERRUPT_MSM_OOB_REG supported on:                                  */
/*      SPRA0 (0x2081803c)                                                      */
/*      SPRB0 (0x2081803c)                                                      */
/*      SPRHBM (0x2081803c)                                                     */
/*      SPRC0 (0x2081803c)                                                      */
/*      SPRMCC (0x2081803c)                                                     */
/*      SPRUCC (0x2081803c)                                                     */
/* Register default value on SPRA0: 0x000000FF                                  */
/* Register default value on SPRB0: 0x000000FF                                  */
/* Register default value on SPRHBM: 0x000000FF                                 */
/* Register default value on SPRC0: 0x000000FF                                  */
/* Register default value on SPRMCC: 0x000000FF                                 */
/* Register default value on SPRUCC: 0x000000FF                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Interrupt Line and Pin configuration.
*/


#define PCI_CFG_INTERRUPT_MSM_OOB_REG 0x1702003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interrupt_line : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000FF*/

                            /*
                               Interrupt Line. Specifies the IRQx line
                               signaling for PCI. When set to 0xFF, interrupt
                               line signaling is not used.
                            */
    UINT32 interrupt_pin : 3;

                            /* Bits[10:8], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Pin. Specifies the INTx pin signaling
                               for PCI. When set to 0x1, INTA in signaled.
                            */
    UINT32 reserved_31_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_INTERRUPT_MSM_OOB_STRUCT;

/* PCIE_CFG_PXPCAPID_MSM_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x818090)                                                        */
/*      SPRB0 (0x818090)                                                        */
/*      SPRHBM (0x818090)                                                       */
/*      SPRC0 (0x818090)                                                        */
/*      SPRMCC (0x818090)                                                       */
/*      SPRUCC (0x818090)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability ID Register
*/


#define PCIE_CFG_PXPCAPID_MSM_MSM_OOB_REG 0x17000090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*
                               Assigned by PCI-SIG for PCI Express capability
                               ID.
                            */

  } Bits;
  UINT8 Data;

} PCIE_CFG_PXPCAPID_MSM_MSM_OOB_STRUCT;

/* PCIE_CFG_PXPNXTPTR_MSM_MSM_OOB_REG supported on:                             */
/*      SPRA0 (0x818091)                                                        */
/*      SPRB0 (0x818091)                                                        */
/*      SPRHBM (0x818091)                                                       */
/*      SPRC0 (0x818091)                                                        */
/*      SPRMCC (0x818091)                                                       */
/*      SPRUCC (0x818091)                                                       */
/* Register default value on SPRA0: 0x000000E0                                  */
/* Register default value on SPRB0: 0x000000E0                                  */
/* Register default value on SPRHBM: 0x000000E0                                 */
/* Register default value on SPRC0: 0x000000E0                                  */
/* Register default value on SPRMCC: 0x000000E0                                 */
/* Register default value on SPRUCC: 0x000000E0                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability Next Pointer 
*/


#define PCIE_CFG_PXPNXTPTR_MSM_MSM_OOB_REG 0x17000091

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 next_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000E0*/

                            /* Next Capability is the PCI PM Capability. */

  } Bits;
  UINT8 Data;

} PCIE_CFG_PXPNXTPTR_MSM_MSM_OOB_STRUCT;

/* PCIE_CFG_PXPCAP_MSM_MSM_OOB_REG supported on:                                */
/*      SPRA0 (0x10818092)                                                      */
/*      SPRB0 (0x10818092)                                                      */
/*      SPRHBM (0x10818092)                                                     */
/*      SPRC0 (0x10818092)                                                      */
/*      SPRMCC (0x10818092)                                                     */
/*      SPRUCC (0x10818092)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRHBM: 0x00000002                                 */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability Register 
*/


#define PCIE_CFG_PXPCAP_MSM_MSM_OOB_REG 0x17010092

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capability_version : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               PCI Express Capability is Compliant with Version
                               2.0 of the PCI Express Spec.
                            */
    UINT16 device_port_type : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /*
                               This field identifies the type of device. It is
                               set to 0100 for all the Express ports.
                            */
    UINT16 slot_implemented : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               1: Indicates that the PCI Express link
                               associated with the port is connected to a slot.
                               0: Indicates no slot is connected to this port.
                               Notes: This bit is is set by BIOS.
                            */
    UINT16 interrupt_message_number : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Applies to Root Ports. This field indicates the
                               interrupt message number that is generated for
                               Power Management/Hot Plug/Bandwidth-change
                               events. When there are more than one MSI
                               interrupt Number allocated for the root port MSI
                               interrupts, this register field is required to
                               contain the offset between the base Message Data
                               and the MSI Message that is generated when there
                               are these change interrupts. IIO assigns the
                               first vector for these change events and so this
                               field is set to 0.
                            */
    UINT16 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCIE_CFG_PXPCAP_MSM_MSM_OOB_STRUCT;

/* PCIE_CFG_DEV_CAP_MSM_MSM_OOB_REG supported on:                               */
/*      SPRA0 (0x20818094)                                                      */
/*      SPRB0 (0x20818094)                                                      */
/*      SPRHBM (0x20818094)                                                     */
/*      SPRC0 (0x20818094)                                                      */
/*      SPRMCC (0x20818094)                                                     */
/*      SPRUCC (0x20818094)                                                     */
/* Register default value on SPRA0: 0x00008002                                  */
/* Register default value on SPRB0: 0x00008002                                  */
/* Register default value on SPRHBM: 0x00008002                                 */
/* Register default value on SPRC0: 0x00008002                                  */
/* Register default value on SPRMCC: 0x00008002                                 */
/* Register default value on SPRUCC: 0x00008002                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Capability
*/


#define PCIE_CFG_DEV_CAP_MSM_MSM_OOB_REG 0x17020094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_payload_size_supported : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /* Supports 512B payloads on PCI Express Ports. */
    UINT32 phantom_functions_supported : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /* IIO does not support phantom functions. */
    UINT32 extended_tag_field_supported : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* extended tag fueld supported */
    UINT32 endpoint_l0s_acceptable_latency : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the endpoint acceptable latency (not
                               applicable).
                            */
    UINT32 endpoint_l1_acceptable_latency : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the endpoint acceptable latency (not
                               applicable).
                            */
    UINT32 attention_button_present : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Not Supported */
    UINT32 attention_indicator_present : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Not Supported */
    UINT32 power_indicator_present_on_device : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to Root Ports or integrated
                               devices
                            */
    UINT32 role_based_error_reporting : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /* IIO supports Role Based Error Reporting. */
    UINT32 rsvd : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 captured_slot_power_limit_value : 8;

                            /* Bits[25:18], Access Type=RO, default=0x00000000*/

                            /* Captured slot power limit value. */
    UINT32 captured_slot_power_limit_scale : 2;

                            /* Bits[27:26], Access Type=RO, default=0x00000000*/

                            /* Captured slot power limit scale. */
    UINT32 func_level_reset : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /* Function level Reset */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCIE_CFG_DEV_CAP_MSM_MSM_OOB_STRUCT;

/* PCIE_DEVCTRL_CFG_MSM_MSM_OOB_REG supported on:                               */
/*      SPRA0 (0x10818098)                                                      */
/*      SPRB0 (0x10818098)                                                      */
/*      SPRHBM (0x10818098)                                                     */
/*      SPRC0 (0x10818098)                                                      */
/*      SPRMCC (0x10818098)                                                     */
/*      SPRUCC (0x10818098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Control 
*/


#define PCIE_DEVCTRL_CFG_MSM_MSM_OOB_REG 0x17010098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 correctable_error_reporting_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the reporting of correctable errors
                               that IIO detects on the PCI Express/DMI
                               interface 0: Reporting of link Correctable error
                               detected by the port is disabled 1: Reporting of
                               link Correctable error detected by port is
                               enabled Refer to PCI Express Base Specification,
                               Revision 2.0 for complete details of how this
                               bit is used in conjunction with other bits to
                               report errors. This bit is not used to control
                               the reporting of other internal component
                               correctable errors (at the port unit) in any
                               way.
                            */
    UINT16 non_fatal_error_reporting_enable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the reporting of non-fatal errors that
                               IIO detects on the PCI Express interface. 0:
                               Reporting of Non Fatal error detected by device
                               is disabled 1: Reporting of Non Fatal error
                               detected by device is enabled Refer to PCI
                               Express Base Specification, Revision 2.0 for
                               complete details of how this bit is used in
                               conjunction with other bits to report errors.
                               This bit is not used to control the reporting of
                               other internal component uncorrectable non-fatal
                               errors (at the port unit) in any way.
                            */
    UINT16 fatal_error_reporting_enable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the reporting of fatal errors that IIO
                               detects on the PCI Express/DMI interface. 0:
                               Reporting of Fatal error detected by device is
                               disabled 1: Reporting of Fatal error detected by
                               device is enabled Refer to PCI Express Base
                               Specification, Revision 2.0 for complete details
                               of how this bit is used in conjunction with
                               other bits to report errors. This bit is not
                               used to control the reporting of other internal
                               component uncorrectable fatal errors (at the
                               port unit) in any way.
                            */
    UINT16 unsupported_request_reporting_enable : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* unsupported request reporting enable */
    UINT16 enable_relaxed_ordering : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCIe Root Ports since they
                               never set 'Relaxed Ordering' bit as a requester.
                               This bit has no impact on forwarding of relaxed
                               ordering attribute on peer requests.
                            */
    UINT16 max_payload_size : 3;

                            /* Bits[7:5], Access Type=RO, default=0x00000000*/

                            /*
                               000: 128B max payload size 001: 256B max payload
                               size 010: 512B max payload size others: alias to
                               128B IIO can receive packets equal to the size
                               set by this field. IIO generate read completions
                               as large as the value set by this field. IIO
                               generates memory writes of max 64B.
                            */
    UINT16 extended_tag_field_enable : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Enables the Root Port to generate 8-bit tags. If
                               disabled, the Root Port generates 5-bit tags.
                            */
    UINT16 phantom_functions_enable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable, since IIO never uses phantom
                               functions as a requestor.
                            */
    UINT16 auxiliary_power_management_enable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* auxiliary power management enable */
    UINT16 enable_no_snoop : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCIe Root Ports since they
                               never set the 'No Snoop' bit for transactions
                               they originate (not forwarded from peer) to PCI
                               Express/DMI. This bit has no impact on
                               forwarding of NoSnoop attribute on peer
                               requests.
                            */
    UINT16 max_read_request_size : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               PCI Express ports do not generate requests
                               greater than 64B and this field is RO.
                            */
    UINT16 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCIE_DEVCTRL_CFG_MSM_MSM_OOB_STRUCT;

/* PCIE_DEVSTS_CFG_MSM_MSM_OOB_REG supported on:                                */
/*      SPRA0 (0x1081809a)                                                      */
/*      SPRB0 (0x1081809a)                                                      */
/*      SPRHBM (0x1081809a)                                                     */
/*      SPRC0 (0x1081809a)                                                      */
/*      SPRMCC (0x1081809a)                                                     */
/*      SPRUCC (0x1081809a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Status 
*/


#define PCIE_DEVSTS_CFG_MSM_MSM_OOB_REG 0x1701009A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 correctable_error_detected : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit gets set if a correctable error is
                               detected by the root port. Errors are logged in
                               this register regardless of whether error
                               reporting is enabled or not in the PCI Express
                               Device Control register. 1: correctable errors
                               detected 0: No correctable errors detected
                            */
    UINT16 non_fatal_error_detected : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit gets set if a non-fatal uncorrectable
                               error is detected by the Root Port. Errors are
                               logged in this register regardless of whether
                               error reporting is enabled or not in the Device
                               Control register. 1: Non Fatal errors detected
                               0: No non-Fatal Errors detected
                            */
    UINT16 fatal_error_detected : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that a fatal (uncorrectable)
                               error is detected by the root port. Errors are
                               logged in this register regardless of whether
                               error reporting is enabled or not in the Device
                               Control register. 1: Fatal errors detected 0: No
                               Fatal errors detected
                            */
    UINT16 unsupported_request_detected : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that the port detected an
                               Unsupported Request. Errors are logged in this
                               register regardless of whether error reporting
                               is enabled or not in the Device Control
                               Register. 1: Unsupported Request detected at the
                               device/port. These unsupported requests are NP
                               requests inbound that the Root Port port
                               received and it detected them as unsupported
                               requests (e.g. address decoding failures that
                               the Root Port detected on a packet, receiving
                               inbound lock reads, BME bit is clear etc.). 0:
                               No unsupported request detected by the port This
                               bit is not set on peer2peer completions with UR
                               status that are forwarded by the port to the
                               PCIe link.
                            */
    UINT16 aux_power_detected : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* aux power detected */
    UINT16 transactions_pending : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Does not apply to Root/DMI ports, i.e. bit
                               hardwired to 0 for these devices.
                            */
    UINT16 rsvd : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCIE_DEVSTS_CFG_MSM_MSM_OOB_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_LINKCAPABILITIES_MSM_OOB_REG supported on:                              */
/*      SPRB0 (0x2081809c)                                                      */
/*      SPRHBM (0x2081809c)                                                     */
/*      SPRC0 (0x2081809c)                                                      */
/*      SPRMCC (0x2081809c)                                                     */
/*      SPRUCC (0x2081809c)                                                     */
/* Register default value on SPRB0: 0x00000C09                                  */
/* Register default value on SPRHBM: 0x00000C09                                 */
/* Register default value on SPRC0: 0x00000C09                                  */
/* Register default value on SPRMCC: 0x00000C09                                 */
/* Register default value on SPRUCC: 0x00000C09                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRB0 BDF: 8_3_0                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Link capabilities register identifies the link specific capabilities. 
*/


#define PCIE_LINKCAPABILITIES_MSM_OOB_REG 0x1702009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_link_speed : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000001*/

                            /*
                               Max Link Speed This field indicates the maximum
                               Link speed of the associated Port. The encoded
                               value specifies a bit location in the Supported
                               Link Speeds Vector (in the Link Capabilities 2
                               register) that corresponds to the maximum Link
                               speed. Defined encodings are: 0001b Supported
                               Link Speeds Vector field bit 0 0010b Supported
                               Link Speeds Vector field bit 1 0011b Supported
                               Link Speeds Vector field bit 2 0100b Supported
                               Link Speeds Vector field bit 3 0101b Supported
                               Link Speeds Vector field bit 4 0110b Supported
                               Link Speeds Vector field bit 5 0111b Supported
                               Link Speeds Vector field bit 6
                            */
    UINT32 max_link_width : 7;

                            /* Bits[9:3], Access Type=RO/V, default=0x00000001*/

                            /*
                               Maximum Link Width This field indicates the
                               maximum Link width (xN corresponding to N Lanes)
                               implemented by the component. This value is
                               permitted to exceed the number of Lanes routed
                               to the slot (Downstream Port), adapter connector
                               (Upstream Port), or in the case of component-to-
                               component connections, the actual wired
                               connection width. Defined encodings are: 00
                               0001b x1 00 0010b x2 00 0100b x4 00 1000b x8 00
                               1100b x12 01 0000b x16 10 0000b x32
                            */
    UINT32 aspm : 2;

                            /* Bits[11:10], Access Type=RO/V, default=0x00000003*/

                            /*
                               Active State Power Management (ASPM) Support
                               This field indicates the level of ASPM supported
                               on the given PCI Express Link. See Section 5.4.1
                               for ASPM support requirements. Defined encodings
                               are: 00b No ASPM Support 01b L0s Supported 10b
                               L1 Supported 11b L0s and L1 Supported
                            */
    UINT32 l0_exit_latency : 3;

                            /* Bits[14:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               L0s Exit Latency This field indicates the L0s
                               exit latency for the given PCI Express Link. The
                               value reported indicates the length of time this
                               Port requires to complete transition from L0s to
                               L0. If L0s is not supported, the value is
                               undefined; however, see the Implementation Note
                               Potential Issues With Legacy Software When L0s
                               is Not Supported in Section 5.4.1.1 for the
                               recommended value. Defined encodings are: 000b
                               Less than 64 ns 001b 64 ns to less than 128 ns
                               010b 128 ns to less than 256 ns 011b 256 ns to
                               less than 512 ns 100b 512 ns to less than 1 s
                               101b 1 s to less than 2 s 110b 2 s-4 s 111b More
                               than 4 s
                            */
    UINT32 l1_exit_latency : 3;

                            /* Bits[17:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               L1 Exit Latency This field indicates the L1 exit
                               latency for the given PCI Express Link. The
                               value reported indicates the length of time this
                               Port requires to complete transition from ASPM
                               Defined encodings are: 000b Less than 64 ns 001b
                               64 ns to less than 128 ns 010b 128 ns to less
                               than 256 ns 011b 256 ns to less than 512 ns 100b
                               512 ns to less than 1 s 101b 1 s to less than 2
                               s 110b 2 s-4 s 111b More than 4 s
                            */
    UINT32 clock_power_mgmt : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Clock Power Management For Upstream Ports, a
                               value of 1b in this bit indicates that the
                               component tolerates the removal of any reference
                               clock(s) via the clock request (CLKREQ#)
                               mechanism when the Link is in the L1 and L2/L3
                               Ready Link states. A value of 0b indicates the
                               component does not have this capability and that
                               reference clock(s) must not be removed in these
                               Link states. L1 PM Substates defines other
                               semantics for the CLKREQ# signal, which are
                               managed independently of Clock Power Management.
                               This Capability is applicable only in form
                               factors that support clock request (CLKREQ#)
                               capability. For a Multi-Function Device
                               associated with an Upstream Port, each Function
                               indicates its capability independently. Power
                               Management configuration software must only
                               permit reference clock removal if all Functions
                               of the Multi-Function Device indicate a 1b in
                               this bit. For ARI Devices, all Functions must
                               indicate the same value in this bit. For
                               Downstream Ports, this bit must be hardwired to
                               0b.
                            */
    UINT32 surprise_down_err_reporting : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               Surprise Down Error Reporting Capable For a
                               Downstream Port, this bit must be Set if the
                               component supports the optional capability of
                               detecting and reporting a Surprise Down error
                               condition. For Upstream Ports and components
                               that do not support this optional capability,
                               this bit must be hardwired to 0b.
                            */
    UINT32 data_link_active_reporting_en : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               Data Link Layer Link Active Reporting Capable
                               For a Downstream Port, this bit must be
                               hardwired to 1b if the component supports the
                               optional capability of reporting the DL_Active
                               state of the Data Link Control and Management
                               State Machine. For a hot-plug capable Downstream
                               Port (as indicated by the Hot-Plug Capable bit
                               of the Slot Capabilities register) or a
                               Downstream Port that supports Link speeds
                               greater than 5.0 GT/s, this bit must be
                               hardwired to 1b. For Upstream Ports and
                               components that do not support this optional
                               capability, this bit must be hardwired to 0b
                            */
    UINT32 link_bw_notification_cap : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               Link Bandwidth Notification Capability A value
                               of 1b indicates support for the Link Bandwidth
                               Notification status and interrupt mechanisms.
                               This capability is required for all Root Ports
                               and Switch Downstream Ports supporting Links
                               wider than x1 and/or multiple Link speeds. This
                               field is not applicable and is Reserved for
                               Endpoints, PCI Express to PCI/PCI-X bridges, and
                               Upstream Ports of Switches. Functions that do
                               not implement the Link Bandwidth Notification
                               Capability must hardwire this bit to 0b.
                            */
    UINT32 aspm_optional_compliance : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /*
                               ASPM Optionality Compliance This bit must be set
                               to 1b in all Functions. Components implemented
                               against certain earlier versions of this
                               specification will have this bit set to 0b.
                               Software is permitted to use the value of this
                               bit to help determine whether to enable ASPM or
                               whether to run ASPM compliance tests.
                            */
    UINT32 rsvd : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 portnumber : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port Number This field indicates the PCI Express
                               Port number for the given PCI Express Link.
                            */

  } Bits;
  UINT32 Data;

} PCIE_LINKCAPABILITIES_MSM_OOB_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_LINK_CONTROL_MSM_OOB_REG supported on:                                  */
/*      SPRB0 (0x208180a0)                                                      */
/*      SPRHBM (0x208180a0)                                                     */
/*      SPRC0 (0x208180a0)                                                      */
/*      SPRMCC (0x208180a0)                                                     */
/*      SPRUCC (0x208180a0)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRB0 BDF: 8_3_0                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Link capabilities register identifies the link specific capabilities. 
*/


#define PCIE_LINK_CONTROL_MSM_OOB_REG 0x170200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 aspm_ctrl : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Active State Power Management (ASPM) Control
                               This field controls the level of ASPM enabled on
                               the given PCI Express Link. See Section 5.4.1.3
                               for requirements on when and how to enable ASPM.
                               Defined encodings are: 00b Disabled 01bL0s Entry
                               Enabled 10bL1 Entry Enabled 11bL0s and L1 Entry
                               Enabled Note: L0s Entry Enabled enables the
                               Transmitter to enter L0s. If L0s is supported,
                               the Receiver must be capable of entering L0s
                               even when the Transmitter is disabled from
                               entering L0s (00b or 10b). ASPM L1 must be
                               enabled by software in the Upstream component on
                               a Link prior to enabling ASPM L1 in the
                               Downstream component on that Link. When
                               disabling ASPM L1, software must disable ASPM L1
                               in the Downstream component on a Link prior to
                               disabling ASPM L1 in the Upstream component on
                               that Link. ASPM L1 must only be enabled on the
                               Downstream component if both components on a
                               Link support ASPM L1
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 read_compl_boundary : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Root Ports: Read Completion Boundary (RCB)
                               Indicates the RCB value for the Root Port. Refer
                               to Section 2.3.1.1 for the definition of the
                               parameter RCB. Defined encodings are: 0b 64 byte
                               1b 128 byte This bit is hardwired for a Root
                               Port and returns its RCB support capabilities.
                               Endpoints and Bridges: Read Completion Boundary
                               (RCB) Optionally Set by configuration software
                               to indicate the RCB value of the Root Port
                               Upstream from the Endpoint or Bridge. Refer to
                               Section 2.3.1.1 for the definition of the
                               parameter RCB. Defined encodings are: 0b 64 byte
                               1b 128 byte Configuration software must only Set
                               this bit if the Root Port Upstream from the
                               Endpoint or Bridge reports an RCB value of 128
                               bytes (a value of 1b in the Read Completion
                               Boundary bit). Default value of this bit is 0b.
                               Functions that do not implement this feature
                               must hardwire the bit to 0b
                            */
    UINT32 link_disable : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Link Disable This bit disables the Link by
                               directing the LTSSM to the Disabled state when
                               Set; this bit is Reserved on Endpoints, PCI
                               Express to PCI/PCI-X bridges, and Upstream Ports
                               of Switches. Writes to this bit are immediately
                               reflected in the value read from the bit,
                               regardless of actual Link state. After clearing
                               this bit, software must honor timing
                               requirements defined in Section 6.6.1 with
                               respect to the first Configuration Read
                               following a Conventional Reset.
                            */
    UINT32 retrain_link : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Retrain Link A write of 1b to this bit initiates
                               Link retraining by directing the Physical Layer
                               LTSSM to the Recovery state. If the LTSSM is
                               already in Recovery or Configuration, re-
                               entering Recovery is permitted but not required.
                               If the Port is in DPC when a write of 1b to this
                               bit occurs, the result is undefined. Reads of
                               this bit always return 0b. It is permitted to
                               write 1b to this bit while simultaneously
                               writing modified values to other fields in this
                               register. If the LTSSM is not already in
                               Recovery or Configuration, the resulting Link
                               training must use the modified values. If the
                               LTSSM is already in Recovery or Configuration,
                               the modified values are not required to affect
                               the Link training that's already in progress.
                               This bit is not applicable and is Reserved for
                               Endpoints, PCI Express to PCI/PCI-X bridges, and
                               Upstream Ports of Switches
                            */
    UINT32 common_clk_config : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Common Clock Configuration When Set, this bit
                               indicates that this component and the component
                               at the opposite end of this Link are operating
                               with a distributed common reference clock. A
                               value of 0b indicates that this component and
                               the component at the opposite end of this Link
                               are operating with asynchronous reference clock.
                               For non-ARI Multi-Function Devices, software
                               must program the same value for this bit in all
                               Functions. If not all Functions are Set, then
                               the component must as a whole assume that its
                               reference clock is not common with the Upstream
                               component. For ARI Devices, Common Clock
                               Configuration is determined solely by the
                               setting in Function 0. The settings in the other
                               Functions always return whatever value software
                               programmed for each, but otherwise are ignored
                               by the component. Components utilize this common
                               clock configuration information to report the
                               correct L0s and L1 Exit Latencies. After
                               changing the value in this bit in both
                               components on a Link, software must trigger the
                               Link to retrain by writing a 1b to the Retrain
                               Link bit of the Downstream Port.
                            */
    UINT32 extended_synch : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Extended Synch When Set, this bit forces the
                               transmission of additional Ordered Sets when
                               exiting the L0s state (see Section 4.2.4.5) and
                               when in the Recovery state (see Section
                               4.2.6.4.1). This mode provides external devices
                               (e.g., logic analyzers) monitoring the Link time
                               to achieve bit and Symbol lock before the Link
                               enters the L0 state and resumes communication.
                               For Multi-Function Devices if any Function has
                               this bit Set, then the component must transmit
                               the additional Ordered Sets when exiting L0s or
                               when in Recovery.
                            */
    UINT32 clock_pm_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Clock Power Management Applicable only
                               for Upstream Ports and with form factors that
                               support a Clock Request (CLKREQ#) mechanism,
                               this bit operates as follows: 0bClock power
                               management is disabled and device must hold
                               CLKREQ# signal low. 1bWhen this bit is Set, the
                               device is permitted to use CLKREQ# signal to
                               power manage Link clock according to protocol
                               defined in appropriate form factor
                               specification. For a non-ARI Multi-Function
                               Device, power-managementconfiguration software
                               must only Set this bit if all Functions of the
                               Multi-Function Device indicate a 1b in the Clock
                               Power Management bit of the Link Capabilities
                               register. The component is permitted to use the
                               CLKREQ# signal to power manage Link clock only
                               if this bit is Set for all Functions. For ARI
                               Devices, Clock Power Management is enabled
                               solely by the setting in Function 0. The
                               settings in the other Functions always return
                               whatever value software programmed for each, but
                               otherwise are ignored by the component. The
                               CLKREQ# signal may also be controlled via the L1
                               PM Substates mechanism. Such control is not
                               affected by the setting of this bit. Downstream
                               Ports and components that do not support Clock
                               Power Management (as indicated by a 0b value in
                               the Clock Power Management bit of the Link
                               Capabilities register) must hardwire this bit to
                               0b
                            */
    UINT32 hw_auto_width_disable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Hardware Autonomous Width Disable When Set, this
                               bit disables hardware from changing the Link
                               width for reasons other than attempting to
                               correct unreliable Link operation by reducing
                               Link width. For a Multi-Function Device
                               associated with an Upstream Port, the bit in
                               Function 0 is of type RW, and only Function 0
                               controls the components Link behavior.
                            */
    UINT32 link_bw_mgmt_int_en : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Link Bandwidth Management Interrupt Enable When
                               Set, this bit enables the generation of an
                               interrupt to indicate that the Link Bandwidth
                               Management Status bit has been Set. This bit is
                               not applicable and is Reserved for Endpoints,
                               PCI Express-to-PCI/PCI-X bridges, and Upstream
                               Ports of Switches
                            */
    UINT32 link_auto_bw_mgmt_en : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Link Autonomous Bandwidth Interrupt Enable When
                               Set, this bit enables the generation of an
                               interrupt to indicate that the Link Autonomous
                               Bandwidth Status bit has been Set.
                            */
    UINT32 reserved_15_12 : 4;

                            /* Bits[15:12], Access Type=RO, default=0x00000000*/

                            /* This is a reserved register. */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCIE_LINK_CONTROL_MSM_OOB_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_DEVCAP2_CFG_MSM_MSM_OOB_REG supported on:                               */
/*      SPRB0 (0x208180b4)                                                      */
/*      SPRHBM (0x208180b4)                                                     */
/*      SPRC0 (0x208180b4)                                                      */
/*      SPRMCC (0x208180b4)                                                     */
/*      SPRUCC (0x208180b4)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRB0 BDF: 8_3_0                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Capability 2
*/


#define PCIE_DEVCAP2_CFG_MSM_MSM_OOB_REG 0x170200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmpltovalsup : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /*
                               Completion Timeout Values Supported This field
                               indicates device support for the optional
                               Completion Timeout programmability mechanism.
                               This mechanism allows system software to modify
                               the Completion Timeout range. Bits are one-hot
                               encoded and set according to the table below to
                               show timeout value ranges supported. A device
                               that supports the optional capability of
                               Completion Timeout Programmability must set at
                               least two bits. Four time values ranges are
                               defined: Range A: 50us to 10ms Range B: 10ms to
                               250ms Range C: 250ms to 4s Range D: 4s to 64s
                               Bits are set according to table below to show
                               timeout value ranges supported. 0000b:
                               Completions Timeout programming not supported --
                               values are fixed in the range 50us to 50ms.
                               0001b: Range A 0010b: Range B 0011b: Range A & B
                               0110b: Range B & C 0111b: Range A, B, & C 1110b:
                               Range B, C D 1111b: Range A, B, C & D All other
                               values are reserved. IIO supports timeout values
                               up to 10ms-64s.
                            */
    UINT32 cmpltodissup : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Completion Timeout Disable Supported */
    UINT32 ari_en : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Alternative RID Interpretation Capable This bit
                               is set to 1b indicating the port supports this
                               capability.
                            */
    UINT32 atomicroutsup : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT32 atomic32bcompsup : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* 32-bit AtomicOp Completer Supported */
    UINT32 atomic64bcompsup : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* 64-bit AtomicOp Completer Supported */
    UINT32 atomic128bcascompsup : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* 128-bit CAS Completer Supported */
    UINT32 rsvd : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ltr_en : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               A value of 1b indicates support for the optional
                               Latency Tolerance Reporting (LTR) mechanism
                               capability.
                            */
    UINT32 tph_completer_supported : 2;

                            /* Bits[13:12], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the support for TLP Processing Hints.
                               IIO does not support the extended TPH header.
                               00: TPH and Extended TPH Completer not
                               supported. 01: TPH Completer supported; Extended
                               TPH Completer not supported. 10: Reserved. 11:
                               Both TPH and Extended TPH Completer supported.
                            */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCIE_DEVCAP2_CFG_MSM_MSM_OOB_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_DEVCTRL2_CFG_MSM_MSM_OOB_REG supported on:                              */
/*      SPRB0 (0x108180b8)                                                      */
/*      SPRHBM (0x108180b8)                                                     */
/*      SPRC0 (0x108180b8)                                                      */
/*      SPRMCC (0x108180b8)                                                     */
/*      SPRUCC (0x108180b8)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRB0 BDF: 8_3_0                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Control 2 (DEVCTRL2)
*/


#define PCIE_DEVCTRL2_CFG_MSM_MSM_OOB_REG 0x170100B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 compltoval : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Completion Timeout Value on NP Tx that IIO
                               issues on PCIe/DMI In Devices that support
                               Completion Timeout programmability, this field
                               allows system software to modify the Completion
                               Timeout range. The following encodings and
                               corresponding timeout ranges are defined: 0000b
                               = 10ms to 50ms 0001b = Reserved (IIO aliases to
                               0000b) 0010b = Reserved (IIO aliases to 0000b)
                               0101b = 16ms to 55ms 0110b = 65ms to 210ms 1001b
                               = 260ms to 900ms 1010b = 1s to 3.5s 1101b = 4s
                               to 13s 1110b = 17s to 64s When software selects
                               17s to 64s range, CTOCTRL further controls the
                               timeout value within that range. For all other
                               ranges selected by OS, the timeout value within
                               that range is fixed in IIO hardware. Software
                               can change this field while there is active
                               traffic in the root port. This value will also
                               be used to control PME_TO_ACK Timeout. That is
                               this field sets the timeout value for receiving
                               a PME_TO_ACK message after a PME_TURN_OFF
                               message has been transmitted. The PME_TO_ACK
                               Timeout has meaning only if bit 6 of MISCCTRLSTS
                               register is set to a 1b. This is not used in NTB
                               mode for the internal endpoint.
                            */
    UINT16 compltodis : 1;

                            /* Bits[4:4], Access Type=RW/V, default=0x00000000*/

                            /*
                               Completion Timeout Disable When set to 1b, this
                               bit disables the Completion Timeout mechanism
                               for all NP tx that IIO issues on the PCIE/DMI
                               link. When 0b, completion timeout is enabled.
                               Software can change this field while there is
                               active traffic in the root/DMI port. This is not
                               used in NTB mode for the internal endpoint.
                            */
    UINT16 ari : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Alternative RID Interpretation Enable Applies
                               only to root ports. When set to 1b, ARI is
                               enabled for the Root Port.
                            */
    UINT16 atomicreqen : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 atomicegressblock : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 rsvd : 2;

                            /* Bits[9:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 ltr_en : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Enables processing of LTR messages. */
    UINT16 rsvd_11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCIE_DEVCTRL2_CFG_MSM_MSM_OOB_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* PCIE_PMECAP_MSM_MSM_OOB_REG supported on:                                    */
/*      SPRA0 (0x208180e0)                                                      */
/*      SPRB0 (0x208180e0)                                                      */
/*      SPRHBM (0x208180e0)                                                     */
/*      SPRC0 (0x208180e0)                                                      */
/*      SPRMCC (0x208180e0)                                                     */
/*      SPRUCC (0x208180e0)                                                     */
/* Register default value on SPRA0: 0x00030001                                  */
/* Register default value on SPRB0: 0x00030001                                  */
/* Register default value on SPRHBM: 0x00030001                                 */
/* Register default value on SPRC0: 0x00030001                                  */
/* Register default value on SPRMCC: 0x00030001                                 */
/* Register default value on SPRUCC: 0x00030001                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCIE  power management Capability 
*/


#define PCIE_PMECAP_MSM_MSM_OOB_REG 0x170200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /* Assigned by PCI-SIG for PM Capability ID. */
    UINT32 next_capability_pointer : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* This is the last capability in the chain. */
    UINT32 version : 3;

                            /* Bits[18:16], Access Type=RO, default=0x00000003*/

                            /*
                               This field is set to 3h (PM 1.2 compliant) as
                               version number.
                            */
    UINT32 pme_clock : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               This field is hardwired to 0h as it does not
                               apply to PCI Express.
                            */
    UINT32 rsvd : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 device_specific_initialization : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* device specific initilization */
    UINT32 aux_current : 3;

                            /* Bits[24:22], Access Type=RO, default=0x00000000*/

                            /*
                               The aux current for the endpoint (not
                               applicable).
                            */
    UINT32 d1_support : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* IIO does not support power management state D1. */
    UINT32 d2_support : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /* IIO does not support power management state D2. */
    UINT32 pme_support : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /*
                               Bits 31, 30 and 27 must be set to 1 for PCI-PCI
                               bridge structures representing ports on root
                               complexes.
                            */

  } Bits;
  UINT32 Data;

} PCIE_PMECAP_MSM_MSM_OOB_STRUCT;

/* PCIE_PMECSR_MSM_MSM_OOB_REG supported on:                                    */
/*      SPRA0 (0x208180e4)                                                      */
/*      SPRB0 (0x208180e4)                                                      */
/*      SPRHBM (0x208180e4)                                                     */
/*      SPRC0 (0x208180e4)                                                      */
/*      SPRMCC (0x208180e4)                                                     */
/*      SPRUCC (0x208180e4)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Power Management Control and Status Register 
*/


#define PCIE_PMECSR_MSM_MSM_OOB_REG 0x170200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 power_state : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This 2-bit field is used to determine the
                               current power state of the function and to set a
                               new power state as well. 00: D0 01: D1 (not
                               supported) 10: D2 (not supported) 11: D3_hot If
                               Software tries to write 01 or 10 to this field,
                               the power state does not change from the
                               existing power state (which is either D0 or
                               D3hot) and nor do these bits change value. All
                               devices will respond to only Type 0
                               configuration transactions (targeting the
                               device's configuration space itself) when in
                               D3hot state (root port will not forward Type 1/0
                               transactions to the downstream link) and will
                               not respond to memory/IO transactions (i.e.
                               D3hot state is equivalent to MSE/IOSE bits being
                               clear) as target and will not generate any
                               memory/IO/configuration transactions as
                               initiator on the primary bus (messages are still
                               allowed to pass through).
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 no_soft_reset : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               Indicates the port does not reset its registers
                               when transitioning from D3hot to D0.
                            */
    UINT32 rsvd_4 : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pme_enable : 1;

                            /* Bits[8:8], Access Type=RO/P, default=0x00000000*/

                            /*
                               When set, a virtual PM_PME message is generated
                               internally on an enabled PCI Express Hot-Plug
                               event. This virtual PM_PME message then sets the
                               appropriate bits in the ROOTSTS register (which
                               can then trigger an interrupt).
                            */
    UINT32 data_select : 4;

                            /* Bits[12:9], Access Type=RO, default=0x00000000*/

                            /* The Data select control signal. */
    UINT32 data_scale : 2;

                            /* Bits[14:13], Access Type=RO, default=0x00000000*/

                            /* The data_scale indicator bit. */
    UINT32 pme_status : 1;

                            /* Bits[15:15], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               This field is independent of the PME Enable bit
                               and indicates a PM_PME message (either from the
                               link or internally from within that Root Port)
                               was received at the port.
                            */
    UINT32 rsvd_16 : 6;

                            /* Bits[21:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 b2_b3_support : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /* Indicates b2_b3_support for the endpoint. */
    UINT32 bus_power_clock_control_enable : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /* The bus power clock control enable. */
    UINT32 data : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Not used by IIO. */

  } Bits;
  UINT32 Data;

} PCIE_PMECSR_MSM_MSM_OOB_STRUCT;

/* ERRCAPHDR_MSM_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x20818100)                                                      */
/*      SPRB0 (0x20818100)                                                      */
/*      SPRHBM (0x20818100)                                                     */
/*      SPRC0 (0x20818100)                                                      */
/*      SPRMCC (0x20818100)                                                     */
/*      SPRUCC (0x20818100)                                                     */
/* Register default value on SPRA0: 0x1D010001                                  */
/* Register default value on SPRB0: 0x00010001                                  */
/* Register default value on SPRHBM: 0x00010001                                 */
/* Register default value on SPRC0: 0x00010001                                  */
/* Register default value on SPRMCC: 0x00010001                                 */
/* Register default value on SPRUCC: 0x00010001                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Advanced Error Reporting Extended Capability Header 
*/


#define ERRCAPHDR_MSM_MSM_OOB_REG 0x17020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pci_express_extended_cap_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               Assigned by PCI-SIG for AER Extendeded
                               Capability ID.
                            */
    UINT32 capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Set to 1h for this version of the PCI Express
                               logic
                            */
    UINT32 next_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000001D0*/

                            /*
                               Next Capability is the Error Injection
                               Capability.
                            */

  } Bits;
  UINT32 Data;

} ERRCAPHDR_MSM_MSM_OOB_STRUCT;

/* UNCERSTS_MSM_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818104)                                                      */
/*      SPRB0 (0x20818104)                                                      */
/*      SPRHBM (0x20818104)                                                     */
/*      SPRC0 (0x20818104)                                                      */
/*      SPRMCC (0x20818104)                                                     */
/*      SPRUCC (0x20818104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Uncorrectable Error Status 
*/


#define UNCERSTS_MSM_MSM_OOB_REG 0x17020104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_link_protocol_error_status : 1;

                            /* Bits[4:4], Access Type=RW/1C/P, default=0x00000000*/

                            /* Data link protocol error status. */
    UINT32 surprise_down_error_status : 1;

                            /* Bits[5:5], Access Type=RW/1C/P, default=0x00000000*/

                            /* Surprise down error status. */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poisoned_tlp_status : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates poisoned tlp status. */
    UINT32 flow_control_protocol_error_status : 1;

                            /* Bits[13:13], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates flow control protocol error status. */
    UINT32 completion_time_out_status : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates completion time out status. */
    UINT32 completer_abort_status : 1;

                            /* Bits[15:15], Access Type=RW/1C/P, default=0x00000000*/

                            /* Completer abort status indicator. */
    UINT32 unexpected_completion_status : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates unexpected completion status. */
    UINT32 receiver_buffer_overflow_status : 1;

                            /* Bits[17:17], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates receiver buffer overflow status. */
    UINT32 malformed_tlp_status : 1;

                            /* Bits[18:18], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates malformed tlp status. */
    UINT32 ecrc_error_status : 1;

                            /* Bits[19:19], Access Type=RW/1C/P, default=0x00000000*/

                            /* The ECRC Error Status indicator. */
    UINT32 received_an_unsupported_request : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that a received an unsupported
                               request.
                            */
    UINT32 acs_violation_status : 1;

                            /* Bits[21:21], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates ACS violation status. */
    UINT32 uncer_internal_err : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates uncorrectable internal error. */
    UINT32 rsvd_23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison_egress_block_status : 1;

                            /* Bits[26:26], Access Type=RW/1C/P, default=0x00000000*/

                            /* A poison egress block status. */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCERSTS_MSM_MSM_OOB_STRUCT;

/* UNCERRMSK_MSM_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x20818108)                                                      */
/*      SPRB0 (0x20818108)                                                      */
/*      SPRHBM (0x20818108)                                                     */
/*      SPRC0 (0x20818108)                                                      */
/*      SPRMCC (0x20818108)                                                     */
/*      SPRUCC (0x20818108)                                                     */
/* Register default value on SPRA0: 0x04400000                                  */
/* Register default value on SPRB0: 0x04400000                                  */
/* Register default value on SPRHBM: 0x04400000                                 */
/* Register default value on SPRC0: 0x04400000                                  */
/* Register default value on SPRMCC: 0x04400000                                 */
/* Register default value on SPRUCC: 0x04400000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Uncorrectable Error Mask 
*/


#define UNCERRMSK_MSM_MSM_OOB_REG 0x17020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_link_layer_protocol_error_mask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask register for data link layer protocol
                               error.
                            */
    UINT32 surprise_down_error_mask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for surprise down error. */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poisoned_tlp_mask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for poisoned tlp mask. */
    UINT32 flow_control_protocol_error_mask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for flow control protocol error. */
    UINT32 completion_time_out_mask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for completion timeout. */
    UINT32 completer_abort_mask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for completion abort. */
    UINT32 unexpected_completion_mask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for unexpected completion. */
    UINT32 receiver_buffer_overflow_mask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for receiver buffer overflow. */
    UINT32 malformed_tlp_mask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* The malformed tlp mask. */
    UINT32 ecrc_error_mask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for ecrc errors. */
    UINT32 unsupported_request_error_mask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for unsupported request errors. */
    UINT32 acs_violation_mask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* Mask register for acs violations. */
    UINT32 uncer_internal_err : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* Mask register for uncorrectable internal errors. */
    UINT32 rsvd_23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison_egress_block : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /* Mask register for poison egress block. */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCERRMSK_MSM_MSM_OOB_STRUCT;

/* UNCERRSEV_MSM_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x2081810c)                                                      */
/*      SPRB0 (0x2081810c)                                                      */
/*      SPRHBM (0x2081810c)                                                     */
/*      SPRC0 (0x2081810c)                                                      */
/*      SPRMCC (0x2081810c)                                                     */
/*      SPRUCC (0x2081810c)                                                     */
/* Register default value on SPRA0: 0x00462030                                  */
/* Register default value on SPRB0: 0x00462030                                  */
/* Register default value on SPRHBM: 0x00462030                                 */
/* Register default value on SPRC0: 0x00462030                                  */
/* Register default value on SPRMCC: 0x00462030                                 */
/* Register default value on SPRUCC: 0x00462030                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Uncorrectable Error Severity 
*/


#define UNCERRSEV_MSM_MSM_OOB_REG 0x1702010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_link_protocol_error_severity : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* The data link protocol error severity. */
    UINT32 surprise_down_error_severity : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Surprise down error severity. */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poisoned_tlp_severity : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* The poisoned tlp severity. */
    UINT32 flow_control_protocol_error_severity : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* The flow control protocol error severity. */
    UINT32 completion_time_out_severity : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* The completion time out severity. */
    UINT32 completer_abort_severity : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* The completer abort severity. */
    UINT32 unexpected_completion_severity : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* The unexpected completion severity. */
    UINT32 receiver_buffer_overflow_severity : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /* The receiver buffer overflow severity. */
    UINT32 malformed_tlp_severity : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* The malformed tlp severity. */
    UINT32 ecrc_error_severity : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* The ecrc error severity. */
    UINT32 unsupported_request_error_severity : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* An unsupported request error severity. */
    UINT32 acs_violation_severity : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* The acs violation severity bit. */
    UINT32 uncer_internal_sev : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* The uncorrectable internal severity. */
    UINT32 rsvd_23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison_egress_block_sev : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* The poison egress block severity. */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCERRSEV_MSM_MSM_OOB_STRUCT;

/* CORERRSTS_MSM_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x20818110)                                                      */
/*      SPRB0 (0x20818110)                                                      */
/*      SPRHBM (0x20818110)                                                     */
/*      SPRC0 (0x20818110)                                                      */
/*      SPRMCC (0x20818110)                                                     */
/*      SPRUCC (0x20818110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Correctable Error Status
*/


#define CORERRSTS_MSM_MSM_OOB_REG 0x17020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 receiver_error_status : 1;

                            /* Bits[0:0], Access Type=RW/1C/P, default=0x00000000*/

                            /* Correctable Error Status bit for Receiver error. */
    UINT32 rsvd : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bad_tlp_status : 1;

                            /* Bits[6:6], Access Type=RW/1C/P, default=0x00000000*/

                            /* Correctable Error Status bit for bad tlp. */
    UINT32 bad_dllp_status : 1;

                            /* Bits[7:7], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates the bad dllp error status. */
    UINT32 replay_num_rollover_status : 1;

                            /* Bits[8:8], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for replay num
                               rollover.
                            */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 replay_timer_time_out_status : 1;

                            /* Bits[12:12], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for replay timer
                               timeout.
                            */
    UINT32 advisory_non_fatal_error_status : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for advisory non-
                               fatal error.
                            */
    UINT32 corrected_internal_error_status : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Correctable Error Status bit for internal error. */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORERRSTS_MSM_MSM_OOB_STRUCT;

/* CORRERRRMSK_MSM_MSM_OOB_REG supported on:                                    */
/*      SPRA0 (0x20818114)                                                      */
/*      SPRB0 (0x20818114)                                                      */
/*      SPRHBM (0x20818114)                                                     */
/*      SPRC0 (0x20818114)                                                      */
/*      SPRMCC (0x20818114)                                                     */
/*      SPRUCC (0x20818114)                                                     */
/* Register default value on SPRA0: 0x00006000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Correctable Error Mask
*/


#define CORRERRRMSK_MSM_MSM_OOB_REG 0x17020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 receiver_error_mask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Mask bit for receiver error. */
    UINT32 rsvd_1 : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bad_tlp_mask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* Mask bit for bad tlp error. */
    UINT32 bad_dllp_mask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Mask bit for bad dllp mask. */
    UINT32 replay_num_rollover_mask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* Mask bit for replay num rollover. */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 replay_timer_time_out_mask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Mask bit for replay timer time_out. */
    UINT32 advisory_non_fatal_error_mask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* Mask bit for advisory non-fatal error. */
    UINT32 corrected_internal_error_mask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* corrected internal error mask */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORRERRRMSK_MSM_MSM_OOB_STRUCT;

/* ERRCAP_MSM_OOB_REG supported on:                                             */
/*      SPRA0 (0x20818118)                                                      */
/*      SPRB0 (0x20818118)                                                      */
/*      SPRHBM (0x20818118)                                                     */
/*      SPRC0 (0x20818118)                                                      */
/*      SPRMCC (0x20818118)                                                     */
/*      SPRUCC (0x20818118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Advanced Error Capabilities and Control Register 
*/


#define ERRCAP_MSM_OOB_REG 0x17020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_error_pointer : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The First Error Pointer is a read-only register
                               that identifies the bit position of the first
                               unmasked error reported in the Uncorrectable
                               Error register. In case of two errors happening
                               at the same time, fatal error gets precedence
                               over non-fatal, in terms of being reported as
                               first error. This field is rearmed to capture
                               new errors when the status bit indicated by this
                               field is cleared by software.
                            */
    UINT32 ecrc_generation_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               BIOS must set this bit to allow software to
                               recognize that this Root Port supports
                               generation of ECRC on transmitted packets.
                            */
    UINT32 ecrc_generation_enable : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               0: ECRC generation is disabled 1: ECRC
                               generation is enabled
                            */
    UINT32 ecrc_check_capable : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               BIOS must set this bit to allow software to
                               recognize that this Root Port supports checking
                               of ECRC on received packets.
                            */
    UINT32 ecrc_check_enable : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               0: ECRC checking is disabled 1: ECRC checking is
                               enabled
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRCAP_MSM_OOB_STRUCT;

/* HDRLOG0_MSM_OOB_REG supported on:                                            */
/*      SPRA0 (0x2081811c)                                                      */
/*      SPRB0 (0x2081811c)                                                      */
/*      SPRHBM (0x2081811c)                                                     */
/*      SPRC0 (0x2081811c)                                                      */
/*      SPRMCC (0x2081811c)                                                     */
/*      SPRUCC (0x2081811c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Header Log 0 
*/


#define HDRLOG0_MSM_OOB_REG 0x1702011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0 Logs the first DWORD of
                               the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} HDRLOG0_MSM_OOB_STRUCT;

/* HDRLOG1_MSM_OOB_REG supported on:                                            */
/*      SPRA0 (0x20818120)                                                      */
/*      SPRB0 (0x20818120)                                                      */
/*      SPRHBM (0x20818120)                                                     */
/*      SPRC0 (0x20818120)                                                      */
/*      SPRMCC (0x20818120)                                                     */
/*      SPRUCC (0x20818120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Header Log 0 
*/


#define HDRLOG1_MSM_OOB_REG 0x17020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0 Logs the first DWORD of
                               the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} HDRLOG1_MSM_OOB_STRUCT;

/* HDRLOG2_MSM_OOB_REG supported on:                                            */
/*      SPRA0 (0x20818124)                                                      */
/*      SPRB0 (0x20818124)                                                      */
/*      SPRHBM (0x20818124)                                                     */
/*      SPRC0 (0x20818124)                                                      */
/*      SPRMCC (0x20818124)                                                     */
/*      SPRUCC (0x20818124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Header Log 0 
*/


#define HDRLOG2_MSM_OOB_REG 0x17020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0 Logs the first DWORD of
                               the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} HDRLOG2_MSM_OOB_STRUCT;

/* HDRLOG3_MSM_OOB_REG supported on:                                            */
/*      SPRA0 (0x20818128)                                                      */
/*      SPRB0 (0x20818128)                                                      */
/*      SPRHBM (0x20818128)                                                     */
/*      SPRC0 (0x20818128)                                                      */
/*      SPRMCC (0x20818128)                                                     */
/*      SPRUCC (0x20818128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Header Log 0 
*/


#define HDRLOG3_MSM_OOB_REG 0x17020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0 Logs the first DWORD of
                               the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} HDRLOG3_MSM_OOB_STRUCT;

/* SOCKET_BUS_RANGE_MSM_OOB_REG supported on:                                   */
/*      SPRA0 (0x20818144)                                                      */
/*      SPRB0 (0x20818144)                                                      */
/*      SPRHBM (0x20818144)                                                     */
/*      SPRC0 (0x20818144)                                                      */
/*      SPRMCC (0x20818144)                                                     */
/*      SPRUCC (0x20818144)                                                     */
/* Register default value on SPRA0: 0x0000FF00                                  */
/* Register default value on SPRB0: 0x0000FF00                                  */
/* Register default value on SPRHBM: 0x0000FF00                                 */
/* Register default value on SPRC0: 0x0000FF00                                  */
/* Register default value on SPRMCC: 0x0000FF00                                 */
/* Register default value on SPRUCC: 0x0000FF00                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Indicates which BUS range is supported by the socket. By default we assume all buses.
*/


#define SOCKET_BUS_RANGE_MSM_OOB_REG 0x17020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_bus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* indicates the start bus allocated to the socket */
    UINT32 end_bus : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /* indicates the end bus allocated to the socket */
    UINT32 segment_id : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* The segment allocated to socket. */
    UINT32 reserved_30_24 : 7;

                            /* Bits[30:24], Access Type=RO, default=0x00000000*/

                            /* reserved fields */
    UINT32 valid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates that the entries in the socket bus
                               table are valid
                            */

  } Bits;
  UINT32 Data;

} SOCKET_BUS_RANGE_MSM_OOB_STRUCT;

/* MSM_UCODE_MSR_64_MSM_OOB_REG supported on:                                   */
/*      SPRA0 (0x20818148)                                                      */
/*      SPRB0 (0x20818148)                                                      */
/*      SPRHBM (0x20818148)                                                     */
/*      SPRC0 (0x20818148)                                                      */
/*      SPRMCC (0x20818148)                                                     */
/*      SPRUCC (0x20818148)                                                     */
/* Register default value on SPRA0: 0x00022000                                  */
/* Register default value on SPRB0: 0x00022000                                  */
/* Register default value on SPRHBM: 0x00022000                                 */
/* Register default value on SPRC0: 0x00022000                                  */
/* Register default value on SPRMCC: 0x00022000                                 */
/* Register default value on SPRUCC: 0x00022000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Down Stream Control Register - Controls whether write requests can be sent to devices
*/


#define MSM_UCODE_MSR_64_MSM_OOB_REG 0x17020148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dwn_stream_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for all
                               Agents
                            */
    UINT32 reserved_7_1 : 7;

                            /* Bits[7:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 peci : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               PECI Agent
                            */
    UINT32 pecioversmbus : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               PECI over SMBUS Agent
                            */
    UINT32 me : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable Downstream Access for WrPCIConfig for ME */
    UINT32 ie : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Enable Downstream Access for WrPCIConfig for IE */
    UINT32 generic : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               generic Agent
                            */
    UINT32 dfx_red : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               Debug RED Agent
                            */
    UINT32 espi : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* RESERVED */
    UINT32 dfx_green : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               Debug GREEN Agent
                            */
    UINT32 dfx_orange : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               Debug ORANGE Agent
                            */
    UINT32 dfx_redm : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Enable Downstream Access for WrPCIConfig for
                               Debug REDM Agent
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reserved_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /* NO Lock bit required as controlled by BIOS */

  } Bits;
  UINT32 Data;

} MSM_UCODE_MSR_64_MSM_OOB_STRUCT;

/* BIOS_CRASH_CTRL_MSM_OOB_REG supported on:                                    */
/*      SPRA0 (0x2081814c)                                                      */
/*      SPRB0 (0x2081814c)                                                      */
/*      SPRHBM (0x2081814c)                                                     */
/*      SPRC0 (0x2081814c)                                                      */
/*      SPRMCC (0x2081814c)                                                     */
/*      SPRUCC (0x2081814c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register allows for enabling and disabling the Crash Log Feature.
*/


#define BIOS_CRASH_CTRL_MSM_OOB_REG 0x1702014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crash_log_disable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field disables the ability to perform
                               crashlog. 0: Crashlog Enabled. 1: Crashlog
                               Disabled.
                            */
    UINT32 reserved_31_1 : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* This is a reserved field. */

  } Bits;
  UINT32 Data;

} BIOS_CRASH_CTRL_MSM_OOB_STRUCT;

/* MSM_PCI_PECI_BIOS_MSM_OOB_REG supported on:                                  */
/*      SPRA0 (0x20818150)                                                      */
/*      SPRB0 (0x20818150)                                                      */
/*      SPRHBM (0x20818150)                                                     */
/*      SPRC0 (0x20818150)                                                      */
/*      SPRMCC (0x20818150)                                                     */
/*      SPRUCC (0x20818150)                                                     */
/* Register default value on SPRA0: 0x0803FF80                                  */
/* Register default value on SPRB0: 0x0803FF80                                  */
/* Register default value on SPRHBM: 0x0803FF80                                 */
/* Register default value on SPRC0: 0x0803FF80                                  */
/* Register default value on SPRMCC: 0x0803FF80                                 */
/* Register default value on SPRUCC: 0x0803FF80                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register provides BIOS Control of PECI Module trust settings.
*/


#define MSM_PCI_PECI_BIOS_MSM_OOB_REG 0x17020150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 peci_wake : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* PCI Wake. Wake system in Package C6 */
    UINT32 peci_trustmode : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Trust mode. 0x0 = Untrusted 0x1 = Trusted
                               0x10/0x11 = Use Agent Trust Register
                            */
    UINT32 pci_config_compl : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               PCI Config Complete. Indicates that Bios has
                               completed BUS enumeration and assignment. All
                               OOBMSM registers are set to their correct values
                            */
    UINT32 reserved_6_4 : 3;

                            /* Bits[6:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 disableooberrorreporting : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               This bit enables the reporting to IEH of errors
                               generated by OOB Agents.
                            */
    UINT32 peci_agt_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /* Enabled Legacy PECI */
    UINT32 peci_smbus_agt_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /* Enable PECI over SMBUS */
    UINT32 me_agt_en : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /* The ME Agent is Always Enabled. */
    UINT32 ie_agt_en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Enabled IE access to PECI */
    UINT32 generic_agt_en : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /* Enable Generic Agent */
    UINT32 dfx_red_agt_en : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /* Intel Debug Agent is Always Enabled */
    UINT32 espi_agt_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /* Enable the eSPI_AGENT. */
    UINT32 dfx_green_agt_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Enable Customer Debug Agent to use PECI */
    UINT32 dfx_redmanu_agt_en : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /* Intel Debug Agent is always enabled. */
    UINT32 dfx_orange_agt_en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /* Enable Customer Extended Debug Agent to use PECI */
    UINT32 reserved_21_18 : 4;

                            /* Bits[21:18], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 peci_agt_trust : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 peci_smbus_agt_trust : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 me_agt_trust : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 ie_agt_trust : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 generic_agt_trust : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 dfx_red_agt_trust : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000001*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 espi_agt_trust : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 dfx_green_agt_trust : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 dfx_redmanu_agt_trust : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */
    UINT32 dfx_orange_agt_trust : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               PECI Agent Trust Mode. Determines on a per agent
                               basis whether the agent is trusted or not. This
                               register is filtered by a Pcode configurable
                               register.
                            */

  } Bits;
  UINT32 Data;

} MSM_PCI_PECI_BIOS_MSM_OOB_STRUCT;

/* MCTP_EID_CTRL_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x20818154)                                                      */
/*      SPRB0 (0x20818154)                                                      */
/*      SPRHBM (0x20818154)                                                     */
/*      SPRC0 (0x20818154)                                                      */
/*      SPRMCC (0x20818154)                                                     */
/*      SPRUCC (0x20818154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* MCTP EID Control Register
*/


#define MCTP_EID_CTRL_MSM_OOB_REG 0x17020154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msm_eid : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Default EID for OOBMSM ... SegmentID is the same
                               as ITCCTRL_23.cfg_myseg, The segmentID width is
                               determine by ITCCTRL_23[35:34] ... The segid is
                               place in this register [7:7-2^width-1][eid]. The
                               default eid which is the remaining bits will be
                               replaced by ME on assignment. ME should be
                               providing the correct segmentID with an EID
                               assignment.
                            */
    UINT32 reserved_31_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCTP_EID_CTRL_MSM_OOB_STRUCT;


/* MSM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_REG supported on:                       */
/*      SPRA0 (0x20818160)                                                      */
/*      SPRB0 (0x20818160)                                                      */
/*      SPRHBM (0x20818160)                                                     */
/*      SPRC0 (0x20818160)                                                      */
/*      SPRMCC (0x20818160)                                                     */
/*      SPRUCC (0x20818160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Sets the Base Address for the NVDIMM Mailbox
*/


#define MSM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_REG 0x17020160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* We need manually add the Lock capability in RTL */

  } Bits;
  UINT32 Data;

} MSM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_STRUCT;

/* MSM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_REG supported on:                       */
/*      SPRA0 (0x20818164)                                                      */
/*      SPRB0 (0x20818164)                                                      */
/*      SPRHBM (0x20818164)                                                     */
/*      SPRC0 (0x20818164)                                                      */
/*      SPRMCC (0x20818164)                                                     */
/*      SPRUCC (0x20818164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Sets the Base Address for the NVDIMM Mailbox
*/


#define MSM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_REG 0x17020164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* We need manually add the Lock capability in RTL */

  } Bits;
  UINT32 Data;

} MSM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_STRUCT;

/* MSM_NVDIMM_MAILBOX_N0_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x20818168)                                                      */
/*      SPRB0 (0x20818168)                                                      */
/*      SPRHBM (0x20818168)                                                     */
/*      SPRC0 (0x20818168)                                                      */
/*      SPRMCC (0x20818168)                                                     */
/*      SPRUCC (0x20818168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Calculates the mailbox offset for a particular mailbox
*/


#define MSM_NVDIMM_MAILBOX_N0_MSM_OOB_REG 0x17020168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interleave_ways : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* Number of Interleave ways */
    UINT32 interleave_size : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               The number of contiguous bytes assigned to each
                               CR DIMM. THis is a compact encoding (log of
                               interleve size to base 2). ie interleave_size
                               =log(actual_interleave_value)
                            */
    UINT32 reserved : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} MSM_NVDIMM_MAILBOX_N0_MSM_OOB_STRUCT;

/* MSM_NVDIMM_MAILBOX_N1_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x2081816c)                                                      */
/*      SPRB0 (0x2081816c)                                                      */
/*      SPRHBM (0x2081816c)                                                     */
/*      SPRC0 (0x2081816c)                                                      */
/*      SPRMCC (0x2081816c)                                                     */
/*      SPRUCC (0x2081816c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Calculates the mailbox offset for a particular mailbox
*/


#define MSM_NVDIMM_MAILBOX_N1_MSM_OOB_REG 0x1702016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 presence : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               32 bit Bitmap indicating which DIMMs in the
                               interleave set are present (each bit represents
                               a DIMM in the interleave set. 1'b0 = NotPresent
                               1'b0 = Present
                            */

  } Bits;
  UINT32 Data;

} MSM_NVDIMM_MAILBOX_N1_MSM_OOB_STRUCT;

/* UNCERSTS_OOB_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x20818170)                                                      */
/*      SPRB0 (0x20818170)                                                      */
/*      SPRHBM (0x20818170)                                                     */
/*      SPRC0 (0x20818170)                                                      */
/*      SPRMCC (0x20818170)                                                     */
/*      SPRUCC (0x20818170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Uncorrectable Error Status 
*/


#define UNCERSTS_OOB_MSM_OOB_REG 0x17020170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_link_protocol_error_status : 1;

                            /* Bits[4:4], Access Type=RW/1C/P, default=0x00000000*/

                            /* Data link protocol error status. */
    UINT32 surprise_down_error_status : 1;

                            /* Bits[5:5], Access Type=RW/1C/P, default=0x00000000*/

                            /* Surprise down error status. */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poisoned_tlp_status : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates poisoned tlp status. */
    UINT32 flow_control_protocol_error_status : 1;

                            /* Bits[13:13], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates flow control protocol error status. */
    UINT32 completion_time_out_status : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates completion time out status. */
    UINT32 completer_abort_status : 1;

                            /* Bits[15:15], Access Type=RW/1C/P, default=0x00000000*/

                            /* Completer abort status indicator. */
    UINT32 unexpected_completion_status : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates unexpected completion status. */
    UINT32 receiver_buffer_overflow_status : 1;

                            /* Bits[17:17], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates receiver buffer overflow status. */
    UINT32 malformed_tlp_status : 1;

                            /* Bits[18:18], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates malformed tlp status. */
    UINT32 ecrc_error_status : 1;

                            /* Bits[19:19], Access Type=RW/1C/P, default=0x00000000*/

                            /* The ECRC Error Status indicator. */
    UINT32 received_an_unsupported_request : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that a received an unsupported
                               request.
                            */
    UINT32 acs_violation_status : 1;

                            /* Bits[21:21], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates ACS violation status. */
    UINT32 uncer_internal_err : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Indicates uncorrectable internal error. */
    UINT32 rsvd_23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison_egress_block_status : 1;

                            /* Bits[26:26], Access Type=RW/1C/P, default=0x00000000*/

                            /* A poison egress block status. */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCERSTS_OOB_MSM_OOB_STRUCT;

/* CORERRSTS_OOB_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x20818174)                                                      */
/*      SPRB0 (0x20818174)                                                      */
/*      SPRHBM (0x20818174)                                                     */
/*      SPRC0 (0x20818174)                                                      */
/*      SPRMCC (0x20818174)                                                     */
/*      SPRUCC (0x20818174)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Correctable Error Status
*/


#define CORERRSTS_OOB_MSM_OOB_REG 0x17020174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 receiver_error_status : 1;

                            /* Bits[0:0], Access Type=RW/1C/P, default=0x00000000*/

                            /* Correctable Error Status bit for Receiver error. */
    UINT32 rsvd : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bad_tlp_status : 1;

                            /* Bits[6:6], Access Type=RW/1C/P, default=0x00000000*/

                            /* Correctable Error Status bit for bad tlp. */
    UINT32 bad_dllp_status : 1;

                            /* Bits[7:7], Access Type=RW/1C/P, default=0x00000000*/

                            /* Indicates the bad dllp error status. */
    UINT32 replay_num_rollover_status : 1;

                            /* Bits[8:8], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for replay num
                               rollover.
                            */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 replay_timer_time_out_status : 1;

                            /* Bits[12:12], Access Type=RW/1C/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for replay timer
                               timeout.
                            */
    UINT32 advisory_non_fatal_error_status : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Correctable Error Status bit for advisory non-
                               fatal error.
                            */
    UINT32 corrected_internal_error_status : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Correctable Error Status bit for internal error. */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORERRSTS_OOB_MSM_OOB_STRUCT;

/* MCTP_BUSCTRL_SEGID_GLBL_MSM_OOB_REG supported on:                            */
/*      SPRA0 (0x20818178)                                                      */
/*      SPRB0 (0x20818178)                                                      */
/*      SPRHBM (0x20818178)                                                     */
/*      SPRC0 (0x20818178)                                                      */
/*      SPRMCC (0x20818178)                                                     */
/*      SPRUCC (0x20818178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIOS Control of Setting up the mapping of BUSID to MCTP SEGID
*/


#define MCTP_BUSCTRL_SEGID_GLBL_MSM_OOB_REG 0x17020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mctp_segid_size : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Remote Bus Range 0 Target Node ID */
    UINT32 rsvd : 25;

                            /* Bits[27:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reserved_31_3 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved for configuration of MCTP */

  } Bits;
  UINT32 Data;

} MCTP_BUSCTRL_SEGID_GLBL_MSM_OOB_STRUCT;

/* MCTP_BUSCTRL_SEGID_CTRL_MSM_OOB_REG supported on:                            */
/*      SPRA0 (0x2081817c)                                                      */
/*      SPRB0 (0x2081817c)                                                      */
/*      SPRHBM (0x2081817c)                                                     */
/*      SPRC0 (0x2081817c)                                                      */
/*      SPRMCC (0x2081817c)                                                     */
/*      SPRUCC (0x2081817c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIOS Control of Setting up the mapping of BUSID to MCTP SEGID
*/


#define MCTP_BUSCTRL_SEGID_CTRL_MSM_OOB_REG 0x1702017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mctp_segid_size : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Remote Bus Range 0 Target Node ID */
    UINT32 rsvd : 25;

                            /* Bits[27:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reserved_31_3 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved for configuration of MCTP */

  } Bits;
  UINT32 Data;

} MCTP_BUSCTRL_SEGID_CTRL_MSM_OOB_STRUCT;

/* SMM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_REG supported on:                       */
/*      SPRA0 (0x20818180)                                                      */
/*      SPRB0 (0x20818180)                                                      */
/*      SPRHBM (0x20818180)                                                     */
/*      SPRC0 (0x20818180)                                                      */
/*      SPRMCC (0x20818180)                                                     */
/*      SPRUCC (0x20818180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Sets the Base Address for the NVDIMM Mailbox
*/


#define SMM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_REG 0x17020180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* We need manually add the Lock capability in RTL */

  } Bits;
  UINT32 Data;

} SMM_NVDIMM_MAILBOX_OFFSET_N0_MSM_OOB_STRUCT;

/* SMM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_REG supported on:                       */
/*      SPRA0 (0x20818184)                                                      */
/*      SPRB0 (0x20818184)                                                      */
/*      SPRHBM (0x20818184)                                                     */
/*      SPRC0 (0x20818184)                                                      */
/*      SPRMCC (0x20818184)                                                     */
/*      SPRUCC (0x20818184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Sets the Base Address for the NVDIMM Mailbox
*/


#define SMM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_REG 0x17020184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* We need manually add the Lock capability in RTL */

  } Bits;
  UINT32 Data;

} SMM_NVDIMM_MAILBOX_OFFSET_N1_MSM_OOB_STRUCT;

/* SMM_NVDIMM_MAILBOX_N0_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x20818188)                                                      */
/*      SPRB0 (0x20818188)                                                      */
/*      SPRHBM (0x20818188)                                                     */
/*      SPRC0 (0x20818188)                                                      */
/*      SPRMCC (0x20818188)                                                     */
/*      SPRUCC (0x20818188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Calculates the mailbox offset for a particular mailbox
*/


#define SMM_NVDIMM_MAILBOX_N0_MSM_OOB_REG 0x17020188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interleave_ways : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* Number of Interleave ways */
    UINT32 interleave_size : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               The number of contiguous bytes assigned to each
                               CR DIMM. THis is a compact encoding (log of
                               interleve size to base 2). ie interleave_size
                               =log(actual_interleave_value)
                            */
    UINT32 reserved : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} SMM_NVDIMM_MAILBOX_N0_MSM_OOB_STRUCT;

/* SMM_NVDIMM_MAILBOX_N1_MSM_OOB_REG supported on:                              */
/*      SPRA0 (0x2081818c)                                                      */
/*      SPRB0 (0x2081818c)                                                      */
/*      SPRHBM (0x2081818c)                                                     */
/*      SPRC0 (0x2081818c)                                                      */
/*      SPRMCC (0x2081818c)                                                     */
/*      SPRUCC (0x2081818c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Calculates the mailbox offset for a particular mailbox
*/


#define SMM_NVDIMM_MAILBOX_N1_MSM_OOB_REG 0x1702018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 presence : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               32 bit Bitmap indicating which DIMMs in the
                               interleave set are present (each bit represents
                               a DIMM in the interleave set. 1'b0 = NotPresent
                               1'b0 = Present
                            */

  } Bits;
  UINT32 Data;

} SMM_NVDIMM_MAILBOX_N1_MSM_OOB_STRUCT;

/* CPUBUSNO_MSM_OOB_REG supported on:                                           */
/*      SPRA0 (0x20818190)                                                      */
/*      SPRB0 (0x20818190)                                                      */
/*      SPRHBM (0x20818190)                                                     */
/*      SPRC0 (0x20818190)                                                      */
/*      SPRMCC (0x20818190)                                                     */
/*      SPRUCC (0x20818190)                                                     */
/* Register default value on SPRA0: 0x03020100                                  */
/* Register default value on SPRB0: 0x03020100                                  */
/* Register default value on SPRHBM: 0x03020100                                 */
/* Register default value on SPRC0: 0x03020100                                  */
/* Register default value on SPRMCC: 0x03020100                                 */
/* Register default value on SPRUCC: 0x03020100                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO_MSM_OOB_REG 0x17020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 0 */
    UINT32 rootbuss1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000001*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 1 */
    UINT32 rootbuss2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000002*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 2 */
    UINT32 rootbuss3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000003*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 3 */

  } Bits;
  UINT32 Data;

} CPUBUSNO_MSM_OOB_STRUCT;

/* CPUBUSNO1_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x20818194)                                                      */
/*      SPRB0 (0x20818194)                                                      */
/*      SPRHBM (0x20818194)                                                     */
/*      SPRC0 (0x20818194)                                                      */
/*      SPRMCC (0x20818194)                                                     */
/*      SPRUCC (0x20818194)                                                     */
/* Register default value on SPRA0: 0x07060504                                  */
/* Register default value on SPRB0: 0x07060504                                  */
/* Register default value on SPRHBM: 0x07060504                                 */
/* Register default value on SPRC0: 0x07060504                                  */
/* Register default value on SPRMCC: 0x07060504                                 */
/* Register default value on SPRUCC: 0x07060504                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO1_MSM_OOB_REG 0x17020194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss4 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000004*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 4 */
    UINT32 rootbuss5 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000005*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 5 */
    UINT32 rootbuss6 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000006*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 6 */
    UINT32 rootbuss7 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000007*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 7 */

  } Bits;
  UINT32 Data;

} CPUBUSNO1_MSM_OOB_STRUCT;

/* CPUBUSNO2_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x20818198)                                                      */
/*      SPRB0 (0x20818198)                                                      */
/*      SPRHBM (0x20818198)                                                     */
/*      SPRC0 (0x20818198)                                                      */
/*      SPRMCC (0x20818198)                                                     */
/*      SPRUCC (0x20818198)                                                     */
/* Register default value on SPRA0: 0x0B0A0908                                  */
/* Register default value on SPRB0: 0x0B0A0908                                  */
/* Register default value on SPRHBM: 0x0B0A0908                                 */
/* Register default value on SPRC0: 0x0B0A0908                                  */
/* Register default value on SPRMCC: 0x0B0A0908                                 */
/* Register default value on SPRUCC: 0x0B0A0908                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO2_MSM_OOB_REG 0x17020198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss8 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000008*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 8 */
    UINT32 rootbuss9 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000009*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 9 */
    UINT32 rootbuss10 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000000A*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 10. */
    UINT32 rootbuss11 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x0000000B*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 11 */

  } Bits;
  UINT32 Data;

} CPUBUSNO2_MSM_OOB_STRUCT;

/* CPUBUSNO3_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x2081819c)                                                      */
/*      SPRB0 (0x2081819c)                                                      */
/*      SPRHBM (0x2081819c)                                                     */
/*      SPRC0 (0x2081819c)                                                      */
/*      SPRMCC (0x2081819c)                                                     */
/*      SPRUCC (0x2081819c)                                                     */
/* Register default value on SPRA0: 0x0F0E0D0C                                  */
/* Register default value on SPRB0: 0x0F0E0D0C                                  */
/* Register default value on SPRHBM: 0x0F0E0D0C                                 */
/* Register default value on SPRC0: 0x0F0E0D0C                                  */
/* Register default value on SPRMCC: 0x0F0E0D0C                                 */
/* Register default value on SPRUCC: 0x0F0E0D0C                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO3_MSM_OOB_REG 0x1702019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss12 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x0000000C*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 12 */
    UINT32 rootbuss13 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x0000000D*/

                            /* Base Root bus for Physical Mesh2IOSF Stack 13 */
    UINT32 rootbuss14 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000000E*/

                            /* Base Root bus for Physical Mesh2IOSF Stack S14 */
    UINT32 rootbuss15 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x0000000F*/

                            /* Base Root bus for Physical Mesh2IOSF Stack S15 */

  } Bits;
  UINT32 Data;

} CPUBUSNO3_MSM_OOB_STRUCT;

/* CPUBUSNO_VALID_MSM_OOB_REG supported on:                                     */
/*      SPRA0 (0x208181a0)                                                      */
/*      SPRB0 (0x208181a0)                                                      */
/*      SPRHBM (0x208181a0)                                                     */
/*      SPRC0 (0x208181a0)                                                      */
/*      SPRMCC (0x208181a0)                                                     */
/*      SPRUCC (0x208181a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Indicates which BUS are configured and enabled.
*/


#define CPUBUSNO_VALID_MSM_OOB_REG 0x170201A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en_rootbuss0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Root Bus S0 Configured */
    UINT32 en_rootbuss1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Root Bus S1 Configured */
    UINT32 en_rootbuss2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Root Bus S2 Configured */
    UINT32 en_rootbuss3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Root Bus S3 Configured */
    UINT32 en_rootbuss4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Root Bus S4 Configured */
    UINT32 en_rootbuss5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Root Bus S5 Configured */
    UINT32 en_rootbuss6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Root Bus S6 Configured */
    UINT32 en_rootbuss7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Root Bus S7 Configured */
    UINT32 en_rootbuss8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Root Bus S8 Configured */
    UINT32 en_rootbuss9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Root Bus S9 Configured */
    UINT32 en_rootbuss10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Root Bus S10 Configured */
    UINT32 en_rootbuss11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Root Bus S11 Configured */
    UINT32 en_rootbuss12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Root Bus S12 Configured */
    UINT32 en_rootbuss13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Root Bus S13 Configured */
    UINT32 en_rootbuss14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Root Bus S15 Configured */
    UINT32 en_rootbuss16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* Root Bus S16 Configured */
    UINT32 en_rootbuss17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Root Bus S18 Configured */
    UINT32 en_rootbuss19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Root Bus S19 Configured */
    UINT32 en_rootbuss20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Root Bus S20 Configured */
    UINT32 en_rootbuss21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Root Bus S22 Configured */
    UINT32 en_rootbuss23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /* Root Bus S23 Configured */
    UINT32 en_rootbuss24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* Root Bus S26 Configured */
    UINT32 en_rootbuss27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* Root Bus S14 Configured */
    UINT32 en_rootbuss29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Root Bus S29 Configured */
    UINT32 en_rootbusu0 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Root Bus U0 Configured */
    UINT32 en_rootbusu1 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Root Bus U1 Configured */

  } Bits;
  UINT32 Data;

} CPUBUSNO_VALID_MSM_OOB_STRUCT;

/* FABRIC_ERROR_STS_MSM_OOB_REG supported on:                                   */
/*      SPRA0 (0x208181a8)                                                      */
/*      SPRB0 (0x208181a8)                                                      */
/*      SPRHBM (0x208181a8)                                                     */
/*      SPRC0 (0x208181a8)                                                      */
/*      SPRMCC (0x208181a8)                                                     */
/*      SPRUCC (0x208181a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* BIOS Crash Control Register
*/


#define FABRIC_ERROR_STS_MSM_OOB_REG 0x170201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fabric_0_parity_err : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Fabric 0 parity error (gpsb) */
    UINT32 fabric_1_parity_err : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Fabric 0 parity error (pmsb) */
    UINT32 inpkgc : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates whether we were in package C or not
                               when error occurred.
                            */
    UINT32 reserved_31_3 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FABRIC_ERROR_STS_MSM_OOB_STRUCT;

/* MCTP_ALLOW_0_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x208181b0)                                                      */
/*      SPRB0 (0x208181b0)                                                      */
/*      SPRHBM (0x208181b0)                                                     */
/*      SPRC0 (0x208181b0)                                                      */
/*      SPRMCC (0x208181b0)                                                     */
/*      SPRUCC (0x208181b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Allows for an OEM to define the allowed device to access MCTP Services from OOBMSM
*/


#define MCTP_ALLOW_0_MSM_OOB_REG 0x170201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Post Enumerated Bus Number */
    UINT32 device : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /* The devices Device Number */
    UINT32 function : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000000*/

                            /* The devices PCI Function */
    UINT32 rsvd : 15;

                            /* Bits[30:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 entry_enabled : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Indicates the entry is enabled */

  } Bits;
  UINT32 Data;

} MCTP_ALLOW_0_MSM_OOB_STRUCT;

/* MCTP_ALLOW_1_MSM_OOB_REG supported on:                                       */
/*      SPRA0 (0x208181b4)                                                      */
/*      SPRB0 (0x208181b4)                                                      */
/*      SPRHBM (0x208181b4)                                                     */
/*      SPRC0 (0x208181b4)                                                      */
/*      SPRMCC (0x208181b4)                                                     */
/*      SPRUCC (0x208181b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Allows for an OEM to define the allowed device to access MCTP Services from OOBMSM
*/


#define MCTP_ALLOW_1_MSM_OOB_REG 0x170201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bus : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Post Enumerated Bus Number */
    UINT32 device : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /* The devices Device Number */
    UINT32 function : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000000*/

                            /* The devices PCI Function */
    UINT32 rsvd : 15;

                            /* Bits[30:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 entry_enabled : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Indicates the entry is enabled */

  } Bits;
  UINT32 Data;

} MCTP_ALLOW_1_MSM_OOB_STRUCT;

/* CRASHLOG_CTRL_MSM_OOB_REG supported on:                                      */
/*      SPRA0 (0x208181b8)                                                      */
/*      SPRB0 (0x208181b8)                                                      */
/*      SPRHBM (0x208181b8)                                                     */
/*      SPRC0 (0x208181b8)                                                      */
/*      SPRMCC (0x208181b8)                                                     */
/*      SPRUCC (0x208181b8)                                                     */
/* Register default value on SPRA0: 0x000000FA                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRHBM: 0x00000092                                 */
/* Register default value on SPRC0: 0x00000082                                  */
/* Register default value on SPRMCC: 0x00000082                                 */
/* Register default value on SPRUCC: 0x00000082                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used by IA firmware to control the crashlog feature
*/


#define CRASHLOG_CTRL_MSM_OOB_REG 0x170201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 crashlogdiscovery : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /* Indicates that crashlog exists in the system. */
    UINT32 disablecrashlog : 1;

                            /* Bits[2:2], Access Type=RW/V/P, default=0x00000000*/

                            /* This bit Disables automated crashlog */
    UINT32 manualtrigger : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000001*/

                            /* Force a crashlog to take place. */
    UINT32 consumecrashlog : 1;

                            /* Bits[4:4], Access Type=RW/V, default=0x00000001*/

                            /*
                               Indicates that crashlog has been Consumed by an
                               agent.
                            */
    UINT32 collectonallresets : 1;

                            /* Bits[5:5], Access Type=RW/V/P, default=0x00000001*/

                            /* Automatically collect the crashlog on a reset */
    UINT32 enabletriggeronce : 1;

                            /* Bits[6:6], Access Type=RW/V/P, default=0x00000001*/

                            /*
                               Enable the trigger to occur only once so that
                               data is not collected on a subsequent reset.
                               When this bit set, crashlog will only execute
                               once and not be collected on the next reset
                            */
    UINT32 rearmtrigger : 1;

                            /* Bits[7:7], Access Type=RW/V/P, default=0x00000001*/

                            /*
                               This is used in conjunction with trigger once.
                               Must re-arm the trigger to be able to trigger
                               again with trigger once command.
                            */
    UINT32 disablecorecrashlog : 1;

                            /* Bits[8:8], Access Type=RW/V/P, default=0x00000000*/

                            /* This bit Disables automated crashlog for cores */
    UINT32 disabletorcrashlog : 1;

                            /* Bits[9:9], Access Type=RW/V/P, default=0x00000000*/

                            /* This bit Disables automated crashlog for ToR. */
    UINT32 disableuncorecrashlog : 1;

                            /* Bits[10:10], Access Type=RW/V/P, default=0x00000000*/

                            /* This bit Disables automated crashlog for ToR. */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CRASHLOG_CTRL_MSM_OOB_STRUCT;

/* CPUBUSNO4_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x208181c0)                                                      */
/*      SPRB0 (0x208181c0)                                                      */
/*      SPRHBM (0x208181c0)                                                     */
/*      SPRC0 (0x208181c0)                                                      */
/*      SPRMCC (0x208181c0)                                                     */
/*      SPRUCC (0x208181c0)                                                     */
/* Register default value on SPRA0: 0x13121110                                  */
/* Register default value on SPRB0: 0x13121110                                  */
/* Register default value on SPRHBM: 0x13121110                                 */
/* Register default value on SPRC0: 0x13121110                                  */
/* Register default value on SPRMCC: 0x13121110                                 */
/* Register default value on SPRUCC: 0x13121110                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO4_MSM_OOB_REG 0x170201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss16 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000010*/

                            /* Root bus for Stack 16 */
    UINT32 rootbuss17 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000011*/

                            /* Base Root bus for Stack 17 */
    UINT32 rootbuss18 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000012*/

                            /* Base Root bus for Stack 18 */
    UINT32 rootbuss19 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000013*/

                            /* Base Root bus for Stack 19 */

  } Bits;
  UINT32 Data;

} CPUBUSNO4_MSM_OOB_STRUCT;

/* CPUBUSNO5_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x208181c4)                                                      */
/*      SPRB0 (0x208181c4)                                                      */
/*      SPRHBM (0x208181c4)                                                     */
/*      SPRC0 (0x208181c4)                                                      */
/*      SPRMCC (0x208181c4)                                                     */
/*      SPRUCC (0x208181c4)                                                     */
/* Register default value on SPRA0: 0x17161514                                  */
/* Register default value on SPRB0: 0x17161514                                  */
/* Register default value on SPRHBM: 0x17161514                                 */
/* Register default value on SPRC0: 0x17161514                                  */
/* Register default value on SPRMCC: 0x17161514                                 */
/* Register default value on SPRUCC: 0x17161514                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO5_MSM_OOB_REG 0x170201C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss20 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000014*/

                            /* Root bus for Stack 20 */
    UINT32 rootbuss21 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000015*/

                            /* Base Root bus for Stack 21 */
    UINT32 rootbuss22 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000016*/

                            /* Base Root bus for Stack 22 */
    UINT32 rootbuss23 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000017*/

                            /* Base Root bus for Stack 23 */

  } Bits;
  UINT32 Data;

} CPUBUSNO5_MSM_OOB_STRUCT;

/* CPUBUSNO6_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x208181c8)                                                      */
/*      SPRB0 (0x208181c8)                                                      */
/*      SPRHBM (0x208181c8)                                                     */
/*      SPRC0 (0x208181c8)                                                      */
/*      SPRMCC (0x208181c8)                                                     */
/*      SPRUCC (0x208181c8)                                                     */
/* Register default value on SPRA0: 0x1B1A1918                                  */
/* Register default value on SPRB0: 0x1B1A1918                                  */
/* Register default value on SPRHBM: 0x1B1A1918                                 */
/* Register default value on SPRC0: 0x1B1A1918                                  */
/* Register default value on SPRMCC: 0x1B1A1918                                 */
/* Register default value on SPRUCC: 0x1B1A1918                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO6_MSM_OOB_REG 0x170201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss24 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000018*/

                            /* Root bus for Stack 24 */
    UINT32 rootbuss25 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000019*/

                            /* Base Root bus for Stack 25 */
    UINT32 rootbuss26 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000001A*/

                            /* Base Root bus for Stack 26 */
    UINT32 rootbuss27 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x0000001B*/

                            /* Base Root bus for Stack 27 */

  } Bits;
  UINT32 Data;

} CPUBUSNO6_MSM_OOB_STRUCT;

/* CPUBUSNO7_MSM_OOB_REG supported on:                                          */
/*      SPRA0 (0x208181cc)                                                      */
/*      SPRB0 (0x208181cc)                                                      */
/*      SPRHBM (0x208181cc)                                                     */
/*      SPRC0 (0x208181cc)                                                      */
/*      SPRMCC (0x208181cc)                                                     */
/*      SPRUCC (0x208181cc)                                                     */
/* Register default value on SPRA0: 0x1F1E1D1C                                  */
/* Register default value on SPRB0: 0x1F1E1D1C                                  */
/* Register default value on SPRHBM: 0x1F1E1D1C                                 */
/* Register default value on SPRC0: 0x1F1E1D1C                                  */
/* Register default value on SPRMCC: 0x1F1E1D1C                                 */
/* Register default value on SPRUCC: 0x1F1E1D1C                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_0     */
/* Struct generated from SPRA0 BDF: 8_3_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configs the PECI services to be able correctly access devices on the fabric post enumeration for PCIConfig and EndPointCOnfig Commands. The set of registers maps the pre-enumerated value used internally by the PECI Service to the allocated bus number. These register mimic the configuration done in Mesh and UBOX.
*/


#define CPUBUSNO7_MSM_OOB_REG 0x170201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rootbuss28 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x0000001C*/

                            /* Root bus for Stack 28 */
    UINT32 rootbuss29 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x0000001D*/

                            /* Base Root bus for Stack 29 */
    UINT32 rootbusu0 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000001E*/

                            /* Base Root bus for Stack U0 */
    UINT32 rootbusu1 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x0000001F*/

                            /* Base Root bus for Stack U1 */

  } Bits;
  UINT32 Data;

} CPUBUSNO7_MSM_OOB_STRUCT;
#endif /* _MSM_OOB_h */
