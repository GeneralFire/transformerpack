
/** @file
  MSM_DFX.h

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


#ifndef _MSM_DFX_h
#define _MSM_DFX_h
#include <Base.h>

/* PCI_CFG_DEVICE_VENDOR_ID_DFD_MSM_DFX_REG supported on:                       */
/*      SPRA0 (0x2081a000)                                                      */
/*      SPRB0 (0x2081a000)                                                      */
/*      SPRHBM (0x2081a000)                                                     */
/*      SPRC0 (0x2081a000)                                                      */
/*      SPRMCC (0x2081a000)                                                     */
/*      SPRUCC (0x2081a000)                                                     */
/* Register default value on SPRA0: 0x09A88086                                  */
/* Register default value on SPRB0: 0x09A88086                                  */
/* Register default value on SPRHBM: 0x09A88086                                 */
/* Register default value on SPRC0: 0x09A88086                                  */
/* Register default value on SPRMCC: 0x09A88086                                 */
/* Register default value on SPRUCC: 0x09A88086                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Device ID and Vendor ID Registers
*/


#define PCI_CFG_DEVICE_VENDOR_ID_DFD_MSM_DFX_REG 0x17220000

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

                            /* Bits[31:16], Access Type=RO, default=0x000009A8*/

                            /*
                               Device ID. This field identifies the particular
                               device. This identifier is allocated by the
                               vendor
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_DEVICE_VENDOR_ID_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG supported on:                         */
/*      SPRA0 (0x2081a004)                                                      */
/*      SPRB0 (0x2081a004)                                                      */
/*      SPRHBM (0x2081a004)                                                     */
/*      SPRC0 (0x2081a004)                                                      */
/*      SPRMCC (0x2081a004)                                                     */
/*      SPRUCC (0x2081a004)                                                     */
/* Register default value on SPRA0: 0x00100006                                  */
/* Register default value on SPRB0: 0x00100004                                  */
/* Register default value on SPRHBM: 0x00100004                                 */
/* Register default value on SPRC0: 0x00100004                                  */
/* Register default value on SPRMCC: 0x00100004                                 */
/* Register default value on SPRUCC: 0x00100004                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Status and Command registers
*/


#define PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG 0x17220004

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
                               acceses.
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

} PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_CLASS_REVISION_DFD_MSM_DFX_REG supported on:                         */
/*      SPRA0 (0x2081a008)                                                      */
/*      SPRB0 (0x2081a008)                                                      */
/*      SPRHBM (0x2081a008)                                                     */
/*      SPRC0 (0x2081a008)                                                      */
/*      SPRMCC (0x2081a008)                                                     */
/*      SPRUCC (0x2081a008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Class Code and Revision ID registers
*/


#define PCI_CFG_CLASS_REVISION_DFD_MSM_DFX_REG 0x17220008

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

} PCI_CFG_CLASS_REVISION_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_HEADER_CACHELINE_DFD_MSM_DFX_REG supported on:                       */
/*      SPRA0 (0x2081a00c)                                                      */
/*      SPRB0 (0x2081a00c)                                                      */
/*      SPRHBM (0x2081a00c)                                                     */
/*      SPRC0 (0x2081a00c)                                                      */
/*      SPRMCC (0x2081a00c)                                                     */
/*      SPRUCC (0x2081a00c)                                                     */
/* Register default value on SPRA0: 0x00800000                                  */
/* Register default value on SPRB0: 0x00800000                                  */
/* Register default value on SPRHBM: 0x00800000                                 */
/* Register default value on SPRC0: 0x00800000                                  */
/* Register default value on SPRMCC: 0x00800000                                 */
/* Register default value on SPRUCC: 0x00800000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* BIST, Header, Latency Timer, Cache Line Size Configuration Registers. 
*/


#define PCI_CFG_HEADER_CACHELINE_DFD_MSM_DFX_REG 0x1722000C

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
                               the device has multiple functions. <MSM is a
                               single function>
                            */
    UINT32 bist : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* This register controls BIST. (Unimplemented) */

  } Bits;
  UINT32 Data;

} PCI_CFG_HEADER_CACHELINE_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR0_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a010)                                                      */
/*      SPRB0 (0x2081a010)                                                      */
/*      SPRHBM (0x2081a010)                                                     */
/*      SPRC0 (0x2081a010)                                                      */
/*      SPRMCC (0x2081a010)                                                     */
/*      SPRUCC (0x2081a010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register0 <need to fix memory space>.
*/


#define PCI_CFG_BAR0_DFD_MSM_DFX_REG 0x17220010

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
                               32-bit address range.
                            */
    UINT32 prefetch : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable. When set, address space is
                               prefetchable. Reads to this space have no side
                               effects.
                            */
    UINT32 base_addr_ro : 15;

                            /* Bits[18:4], Access Type=RW, default=0x00000000*/

                            /*
                               Base Address Read Only. These read only bits are
                               part of the Base Address and define an aperture
                               size of 512 KB
                            */
    UINT32 base_addr : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Base Address. This field defines base address
                               bits 31:14 for the device's MMIO space. (32 bit
                               bar)
                            */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR0_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR1_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a014)                                                      */
/*      SPRB0 (0x2081a014)                                                      */
/*      SPRHBM (0x2081a014)                                                     */
/*      SPRC0 (0x2081a014)                                                      */
/*      SPRMCC (0x2081a014)                                                     */
/*      SPRUCC (0x2081a014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register1.
*/


#define PCI_CFG_BAR1_DFD_MSM_DFX_REG 0x17220014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base_addr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address. Reserved */

  } Bits;
  UINT32 Data;

} PCI_CFG_BAR1_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR2_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a018)                                                      */
/*      SPRB0 (0x2081a018)                                                      */
/*      SPRHBM (0x2081a018)                                                     */
/*      SPRC0 (0x2081a018)                                                      */
/*      SPRMCC (0x2081a018)                                                     */
/*      SPRUCC (0x2081a018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register2.
*/


#define PCI_CFG_BAR2_DFD_MSM_DFX_REG 0x17220018

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

} PCI_CFG_BAR2_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR3_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a01c)                                                      */
/*      SPRB0 (0x2081a01c)                                                      */
/*      SPRHBM (0x2081a01c)                                                     */
/*      SPRC0 (0x2081a01c)                                                      */
/*      SPRMCC (0x2081a01c)                                                     */
/*      SPRUCC (0x2081a01c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register3.
*/


#define PCI_CFG_BAR3_DFD_MSM_DFX_REG 0x1722001C

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

} PCI_CFG_BAR3_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR4_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a020)                                                      */
/*      SPRB0 (0x2081a020)                                                      */
/*      SPRHBM (0x2081a020)                                                     */
/*      SPRC0 (0x2081a020)                                                      */
/*      SPRMCC (0x2081a020)                                                     */
/*      SPRUCC (0x2081a020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register4.
*/


#define PCI_CFG_BAR4_DFD_MSM_DFX_REG 0x17220020

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

} PCI_CFG_BAR4_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_BAR5_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a024)                                                      */
/*      SPRB0 (0x2081a024)                                                      */
/*      SPRHBM (0x2081a024)                                                     */
/*      SPRC0 (0x2081a024)                                                      */
/*      SPRMCC (0x2081a024)                                                     */
/*      SPRUCC (0x2081a024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Base Address Register5.
*/


#define PCI_CFG_BAR5_DFD_MSM_DFX_REG 0x17220024

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

} PCI_CFG_BAR5_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_SUBSYSTEM_VENDOR_DFD_MSM_DFX_REG supported on:                       */
/*      SPRA0 (0x2081a02c)                                                      */
/*      SPRB0 (0x2081a02c)                                                      */
/*      SPRHBM (0x2081a02c)                                                     */
/*      SPRC0 (0x2081a02c)                                                      */
/*      SPRMCC (0x2081a02c)                                                     */
/*      SPRUCC (0x2081a02c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Defines the Subsystem ID and Subsystem Vendor ID. 
*/


#define PCI_CFG_SUBSYSTEM_VENDOR_DFD_MSM_DFX_REG 0x1722002C

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

} PCI_CFG_SUBSYSTEM_VENDOR_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_EXPANSION_ROM_DFD_MSM_DFX_REG supported on:                          */
/*      SPRA0 (0x2081a030)                                                      */
/*      SPRB0 (0x2081a030)                                                      */
/*      SPRHBM (0x2081a030)                                                     */
/*      SPRC0 (0x2081a030)                                                      */
/*      SPRMCC (0x2081a030)                                                     */
/*      SPRUCC (0x2081a030)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Expansion ROM base address and size information.
*/


#define PCI_CFG_EXPANSION_ROM_DFD_MSM_DFX_REG 0x17220030

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

} PCI_CFG_EXPANSION_ROM_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_CAPABILITIES_DFD_MSM_DFX_REG supported on:                           */
/*      SPRA0 (0x2081a034)                                                      */
/*      SPRB0 (0x2081a034)                                                      */
/*      SPRHBM (0x2081a034)                                                     */
/*      SPRC0 (0x2081a034)                                                      */
/*      SPRMCC (0x2081a034)                                                     */
/*      SPRUCC (0x2081a034)                                                     */
/* Register default value on SPRA0: 0x00000090                                  */
/* Register default value on SPRB0: 0x00000090                                  */
/* Register default value on SPRHBM: 0x00000090                                 */
/* Register default value on SPRC0: 0x00000090                                  */
/* Register default value on SPRMCC: 0x00000090                                 */
/* Register default value on SPRUCC: 0x00000090                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Pointer to the first Expanded Capabilities structure.
*/


#define PCI_CFG_CAPABILITIES_DFD_MSM_DFX_REG 0x17220034

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

} PCI_CFG_CAPABILITIES_DFD_MSM_DFX_STRUCT;

/* PCI_CFG_INTERRUPT_DFD_MSM_DFX_REG supported on:                              */
/*      SPRA0 (0x2081a03c)                                                      */
/*      SPRB0 (0x2081a03c)                                                      */
/*      SPRHBM (0x2081a03c)                                                     */
/*      SPRC0 (0x2081a03c)                                                      */
/*      SPRMCC (0x2081a03c)                                                     */
/*      SPRUCC (0x2081a03c)                                                     */
/* Register default value on SPRA0: 0x000000FF                                  */
/* Register default value on SPRB0: 0x000000FF                                  */
/* Register default value on SPRHBM: 0x000000FF                                 */
/* Register default value on SPRC0: 0x000000FF                                  */
/* Register default value on SPRMCC: 0x000000FF                                 */
/* Register default value on SPRUCC: 0x000000FF                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Interrupt Line and Pin configuration.
*/


#define PCI_CFG_INTERRUPT_DFD_MSM_DFX_REG 0x1722003C

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

} PCI_CFG_INTERRUPT_DFD_MSM_DFX_STRUCT;

/* PCIE_CFG_PXPCAPID_DISC_DFD_MSM_DFX_REG supported on:                         */
/*      SPRA0 (0x81a090)                                                        */
/*      SPRB0 (0x81a090)                                                        */
/*      SPRHBM (0x81a090)                                                       */
/*      SPRC0 (0x81a090)                                                        */
/*      SPRMCC (0x81a090)                                                       */
/*      SPRUCC (0x81a090)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability ID Register
*/


#define PCIE_CFG_PXPCAPID_DISC_DFD_MSM_DFX_REG 0x17200090

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

} PCIE_CFG_PXPCAPID_DISC_DFD_MSM_DFX_STRUCT;

/* PCIE_CFG_PXPNXTPTR_DFD_MSM_DFX_REG supported on:                             */
/*      SPRA0 (0x81a091)                                                        */
/*      SPRB0 (0x81a091)                                                        */
/*      SPRHBM (0x81a091)                                                       */
/*      SPRC0 (0x81a091)                                                        */
/*      SPRMCC (0x81a091)                                                       */
/*      SPRUCC (0x81a091)                                                       */
/* Register default value on SPRA0: 0x000000E0                                  */
/* Register default value on SPRB0: 0x000000E0                                  */
/* Register default value on SPRHBM: 0x000000E0                                 */
/* Register default value on SPRC0: 0x000000E0                                  */
/* Register default value on SPRMCC: 0x000000E0                                 */
/* Register default value on SPRUCC: 0x000000E0                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability Next Pointer 
*/


#define PCIE_CFG_PXPNXTPTR_DFD_MSM_DFX_REG 0x17200091

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

} PCIE_CFG_PXPNXTPTR_DFD_MSM_DFX_STRUCT;

/* PCIE_CFG_PXPCAP_MSM_DFD_MSM_DFX_REG supported on:                            */
/*      SPRA0 (0x1081a092)                                                      */
/*      SPRB0 (0x1081a092)                                                      */
/*      SPRHBM (0x1081a092)                                                     */
/*      SPRC0 (0x1081a092)                                                      */
/*      SPRMCC (0x1081a092)                                                     */
/*      SPRUCC (0x1081a092)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRHBM: 0x00000002                                 */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Capability Register 
*/


#define PCIE_CFG_PXPCAP_MSM_DFD_MSM_DFX_REG 0x17210092

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

} PCIE_CFG_PXPCAP_MSM_DFD_MSM_DFX_STRUCT;

/* PCIE_CFG_DEV_CAP_DFD_MSM_DFX_REG supported on:                               */
/*      SPRA0 (0x2081a094)                                                      */
/*      SPRB0 (0x2081a094)                                                      */
/*      SPRHBM (0x2081a094)                                                     */
/*      SPRC0 (0x2081a094)                                                      */
/*      SPRMCC (0x2081a094)                                                     */
/*      SPRUCC (0x2081a094)                                                     */
/* Register default value on SPRA0: 0x00008002                                  */
/* Register default value on SPRB0: 0x00008002                                  */
/* Register default value on SPRHBM: 0x00008002                                 */
/* Register default value on SPRC0: 0x00008002                                  */
/* Register default value on SPRMCC: 0x00008002                                 */
/* Register default value on SPRUCC: 0x00008002                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Capability
*/


#define PCIE_CFG_DEV_CAP_DFD_MSM_DFX_REG 0x17220094

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

} PCIE_CFG_DEV_CAP_DFD_MSM_DFX_STRUCT;

/* PCIE_DEVCTRL_CFG_DFD_MSM_DFX_REG supported on:                               */
/*      SPRA0 (0x1081a098)                                                      */
/*      SPRB0 (0x1081a098)                                                      */
/*      SPRHBM (0x1081a098)                                                     */
/*      SPRC0 (0x1081a098)                                                      */
/*      SPRMCC (0x1081a098)                                                     */
/*      SPRUCC (0x1081a098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Control 
*/


#define PCIE_DEVCTRL_CFG_DFD_MSM_DFX_REG 0x17210098

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

} PCIE_DEVCTRL_CFG_DFD_MSM_DFX_STRUCT;

/* PCIE_DEVSTS_CFG_DFD_MSM_DFX_REG supported on:                                */
/*      SPRA0 (0x1081a09a)                                                      */
/*      SPRB0 (0x1081a09a)                                                      */
/*      SPRHBM (0x1081a09a)                                                     */
/*      SPRC0 (0x1081a09a)                                                      */
/*      SPRMCC (0x1081a09a)                                                     */
/*      SPRUCC (0x1081a09a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Status 
*/


#define PCIE_DEVSTS_CFG_DFD_MSM_DFX_REG 0x1721009A

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

} PCIE_DEVSTS_CFG_DFD_MSM_DFX_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_DEVCAP2_CFG_DFD_MSM_DFX_REG supported on:                               */
/*      SPRB0 (0x2081a0b4)                                                      */
/*      SPRHBM (0x2081a0b4)                                                     */
/*      SPRC0 (0x2081a0b4)                                                      */
/*      SPRMCC (0x2081a0b4)                                                     */
/*      SPRUCC (0x2081a0b4)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRB0 BDF: 8_3_2                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Capability 2
*/


#define PCIE_DEVCAP2_CFG_DFD_MSM_DFX_REG 0x172200B4

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

} PCIE_DEVCAP2_CFG_DFD_MSM_DFX_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* PCIE_DEVSTS2_CFG_DFD_MSM_DFX_REG supported on:                               */
/*      SPRB0 (0x1081a0b8)                                                      */
/*      SPRHBM (0x1081a0b8)                                                     */
/*      SPRC0 (0x1081a0b8)                                                      */
/*      SPRMCC (0x1081a0b8)                                                     */
/*      SPRUCC (0x1081a0b8)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRB0 BDF: 8_3_2                                       */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCI Express Device Control 2 (DEVCTRL2)
*/


#define PCIE_DEVSTS2_CFG_DFD_MSM_DFX_REG 0x172100B8

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

} PCIE_DEVSTS2_CFG_DFD_MSM_DFX_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* PCIE_PMECAP_DFD_MSM_DFX_REG supported on:                                    */
/*      SPRA0 (0x2081a0e0)                                                      */
/*      SPRB0 (0x2081a0e0)                                                      */
/*      SPRHBM (0x2081a0e0)                                                     */
/*      SPRC0 (0x2081a0e0)                                                      */
/*      SPRMCC (0x2081a0e0)                                                     */
/*      SPRUCC (0x2081a0e0)                                                     */
/* Register default value on SPRA0: 0x00030001                                  */
/* Register default value on SPRB0: 0x00030001                                  */
/* Register default value on SPRHBM: 0x00030001                                 */
/* Register default value on SPRC0: 0x00030001                                  */
/* Register default value on SPRMCC: 0x00030001                                 */
/* Register default value on SPRUCC: 0x00030001                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* PCIE  power management Capability 
*/


#define PCIE_PMECAP_DFD_MSM_DFX_REG 0x172200E0

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

} PCIE_PMECAP_DFD_MSM_DFX_STRUCT;

/* PCIE_PMECSR_DFD_MSM_DFX_REG supported on:                                    */
/*      SPRA0 (0x2081a0e4)                                                      */
/*      SPRB0 (0x2081a0e4)                                                      */
/*      SPRHBM (0x2081a0e4)                                                     */
/*      SPRC0 (0x2081a0e4)                                                      */
/*      SPRMCC (0x2081a0e4)                                                     */
/*      SPRUCC (0x2081a0e4)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRB0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRHBM Security PolicyGroup: PCIE_SPEC                                       */
/* SPRC0 Security PolicyGroup: PCIE_SPEC                                        */
/* SPRMCC Security PolicyGroup: PCIE_SPEC                                       */
/* SPRUCC Security PolicyGroup: PCIE_SPEC                                       */
/* Power Management Control and Status Register 
*/


#define PCIE_PMECSR_DFD_MSM_DFX_REG 0x172200E4

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

} PCIE_PMECSR_DFD_MSM_DFX_STRUCT;

/* PCIE_EXT_CAP_DFD_MSM_DFX_REG supported on:                                   */
/*      SPRA0 (0x2081a100)                                                      */
/*      SPRB0 (0x2081a100)                                                      */
/*      SPRHBM (0x2081a100)                                                     */
/*      SPRC0 (0x2081a100)                                                      */
/*      SPRMCC (0x2081a100)                                                     */
/*      SPRUCC (0x2081a100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRB0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRHBM Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRC0 Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2      */
/* SPRMCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* SPRUCC Register File:    sprsp_top/oobmsm_gpsb/oobmsm_reg/CFG_MSM_FUNC_2     */
/* Struct generated from SPRA0 BDF: 8_3_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Power Management Capabilities Register.
*/


#define PCIE_EXT_CAP_DFD_MSM_DFX_REG 0x17220100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* PCIE Extend Capability ID */
    UINT32 cap_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /* The Capability Version. */
    UINT32 next_cap_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /* The next capability offset. */

  } Bits;
  UINT32 Data;

} PCIE_EXT_CAP_DFD_MSM_DFX_STRUCT;

#endif /* _MSM_DFX_h */
