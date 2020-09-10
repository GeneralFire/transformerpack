
/** @file
  MC_GLOBAL.h

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
/* CPGC                                                                         */
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
/* CPGC                                                                         */
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
/* CPGC                                                                         */
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
/* CPGC                                                                         */
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
/* CPGC                                                                         */
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
/* CPGC                                                                         */
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


#ifndef _MC_GLOBAL_h
#define _MC_GLOBAL_h
#include <Base.h>


/* CRDEFEATURE1_DFX_MC_GLOBAL_REG supported on:                                 */
/*      SPRA0 (0x10002008)                                                      */
/*      SPRB0 (0x10002008)                                                      */
/*      SPRHBM (0x10002008)                                                     */
/*      SPRC0 (0x10002008)                                                      */
/*      SPRMCC (0x10002008)                                                     */
/*      SPRUCC (0x10002008)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* spare
*/


#define CRDEFEATURE1_DFX_MC_GLOBAL_REG 0x07212008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 spare : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* spare */

  } Bits;
  UINT16 Data;

} CRDEFEATURE1_DFX_MC_GLOBAL_STRUCT;



















/* RCOMP_GLOBALCTRL1_MC_GLOBAL_REG supported on:                                */
/*      SPRA0 (0x2000224c)                                                      */
/*      SPRB0 (0x2000224c)                                                      */
/*      SPRHBM (0x2000224c)                                                     */
/*      SPRC0 (0x2000224c)                                                      */
/*      SPRMCC (0x2000224c)                                                     */
/*      SPRUCC (0x2000224c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* RCOMP Global Control for BIOS -
This register is used to control CH0/1/2 Logic
*/


#define RCOMP_GLOBALCTRL1_MC_GLOBAL_REG 0x0722224C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               RCOMP start via message channel control for
                               bios. RCOMP start only triggered when the
                               register bit output is changing from 0 -> 1 MC
                               will not be responsible for clearing this bit.
                               MC already provides feedback to bias when Rcomp
                               is done via first_rcomp_done bit field.
                            */
    UINT32 dis_rcomp : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Disable rcomp */
    UINT32 rcomp_in_progress : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /* rcomp in progress */
    UINT32 first_rcomp_done : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               First RCOMP DONE status, does not distinguish
                               the source of the first rcomp trigger
                            */
    UINT32 chnl_disabled : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit 2 of this three bit field is unused in MC.
                               Indicates if the channel has been disabled by
                               BIOS or by fuse, if yes set to 1. Bit1 - chnl1,
                               Bit0 - chnl0 Used for overriding Acks from
                               individual disabled chnls during PM aggregation.
                               Per chnl bit value should match
                               new_imc_fuse_download.chn_disable_mc0 ||
                               mcmtr.chn_disable
                            */
    UINT32 chnl_unpopulated : 3;

                            /* Bits[9:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit 2 of this field is unused in MC. Set a bit
                               of this field to 1 if the corresponding chnl is
                               unpopulated. Bit1 - chnl1, Bit0 - chnl0 Used for
                               identifying unpopulated chnls and overriding Ack
                               during PM aggregation. Per chnl bit value should
                               logically match ~(dimmmtr_0.dimm_pop ||
                               dimmmtr_1.dimm_pop || McDdrtCfg.Slot0 ||
                               McDdrtCfg.Slot1)
                            */
    UINT32 eadr_per_chnl_disable : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               One bit per channel indicating whether that
                               channel has eADR actions to do. Value should
                               match the inverted value of NmCaching Cfg 2lm
                               enable of each channel. If the bit is 1 eADR
                               will not be sent to that Channel and aggregation
                               logic will do an Auto Ack.
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT;

/* CPGC_GLOBALCTRL_MC_GLOBAL_REG supported on:                                  */
/*      SPRA0 (0x20002250)                                                      */
/*      SPRB0 (0x20002250)                                                      */
/*      SPRHBM (0x20002250)                                                     */
/*      SPRC0 (0x20002250)                                                      */
/*      SPRMCC (0x20002250)                                                     */
/*      SPRUCC (0x20002250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CPGC                                             */
/* SPRB0 Security PolicyGroup: CPGC                                             */
/* SPRHBM Security PolicyGroup: CPGC                                            */
/* SPRC0 Security PolicyGroup: CPGC                                             */
/* SPRMCC Security PolicyGroup: CPGC                                            */
/* SPRUCC Security PolicyGroup: CPGC                                            */
/* CPGC Global Control -
This register is used to control CPGC CH0/1/2 Logic
*/


#define CPGC_GLOBALCTRL_MC_GLOBAL_REG 0x07222250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_test : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Setting to 1 will immediately initiate a
                               transition to Loopback.Pattern on all channels
                               that have their global_control bit set. This
                               field is cleared when all channels transition to
                               Loopback.Marker from loopback.Pattern for
                               channels that have their global_control bit set.
                               If channels are not expected to transition to
                               Loopback.Marker at the same time (because of
                               programming or other differences) then
                               start_test bit will not clear by itself. To
                               start the test again, BIOS will need to clear
                               this bit field followed by setting it to 1
                               again.
                            */
    UINT32 stop_test : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               Setting Global_Stop_Test to 1 will force a
                               transition to Loopback.Marker on all channels
                               that have their Global_Control bit set.
                               Global_Stop_Test will immediately clear to 0
                               after forcing a transition to Loopback.Marker on
                               all channels that have their Global_Control bit
                               set.
                            */
    UINT32 clear_errs : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               Setting Global_Clear_Errors to 1 will
                               immediately clear all error status registers
                               (the same effect as asserting assert the
                               Local_Clear_Errors) on any channels that have
                               their Global Control bit is set.
                               Global_Clear_Errors will immediately be reset to
                               0 after all Channels_Err Status Registers are
                               cleared that have their Global Control bit Set
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 stop_test_on_any_err : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Setting Global_Stop_Test_On_Any_Error to 1 will
                               immediately force a transition to
                               Loopback.Marker after any error on any channels
                               (indicated by a no zero value in
                               Channel_Error_Status) that also have their
                               Global Control bit set.
                            */
    UINT32 use_bl4_col_addr : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Currently Address bits [2:0] of the Column Addr
                               are hard tied to zero, Setting this bit will
                               enable bit control over Addres bit [2] as well
                               thus enabling BL4 mode full addressing.
                            */
    UINT32 cpgc_credits : 6;

                            /* Bits[11:6], Access Type=RW, default=0x00000000*/

                            /*
                               Programmable credits for CPGC Multi Credit
                               Scheme - 6d-zero: Use hardcoded defaults of 6d16
                               6d-nonzero: Programmable credits for CPGC Multi-
                               credit scheme
                            */
    UINT32 rsvd_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cpgc_crdt_wait : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               When enabled, CPGC sequencer will wait for all
                               credits to be returned before moving to the next
                               sub-sequence. Usage of this de-feature bit is -
                               limited to WAIT states between two sub-
                               sequences. - limited to when CPGC is using
                               multi-credit scheme.
                            */
    UINT32 multiwr_singlerd_mode : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               when set, CPGC writes are allowed multiple
                               credits, but CPGC reads will only use 1 credit.
                            */
    UINT32 ddrt_addr_mode : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               3b per channel vector that should be set for
                               DDRT. If this bit is set then - Sequencer uses
                               BL4 addressing mode for adddresses to DDRT
                               scheduler
                            */
    UINT32 rsvd_18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CPGC_GLOBALCTRL_MC_GLOBAL_STRUCT;

/* CPGC_GLOBALERR_MC_GLOBAL_REG supported on:                                   */
/*      SPRA0 (0x20002254)                                                      */
/*      SPRB0 (0x20002254)                                                      */
/*      SPRHBM (0x20002254)                                                     */
/*      SPRC0 (0x20002254)                                                      */
/*      SPRMCC (0x20002254)                                                     */
/*      SPRUCC (0x20002254)                                                     */
/* Register default value on SPRA0: 0x00070000                                  */
/* Register default value on SPRB0: 0x00070000                                  */
/* Register default value on SPRHBM: 0x00070000                                 */
/* Register default value on SPRC0: 0x00070000                                  */
/* Register default value on SPRMCC: 0x00070000                                 */
/* Register default value on SPRUCC: 0x00070000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CPGC                                             */
/* SPRB0 Security PolicyGroup: CPGC                                             */
/* SPRHBM Security PolicyGroup: CPGC                                            */
/* SPRC0 Security PolicyGroup: CPGC                                             */
/* SPRMCC Security PolicyGroup: CPGC                                            */
/* SPRUCC Security PolicyGroup: CPGC                                            */
/* CPGC Global Err -
This register is used to globally monitor all CPGC CH# Logic.
*/


#define CPGC_GLOBALERR_MC_GLOBAL_REG 0x07222254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_err_status_0 : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel_Error_Status_0 corresponds to Channel 0.
                               When set this indicated Channel 0 has at least
                               one Data (Data_Error_Status) or ECC
                               (Ecc_Error_Status) error status bit set.
                               Channel_Error_Status_0 will be cleared when
                               setting Local_Clear_Errors or (Global_Control
                               and Global_Clear_Errors) .
                            */
    UINT32 channel_err_status_1 : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel_Error_Status_1 corresponds to Channel 1.
                               When set this indicated Channel 1 has at least
                               one Data (Data_Error_Status) or ECC
                               (Ecc_Error_Status) error status bit set.
                               Channel_Error_Status_1 will be cleared when
                               setting Local_Clear_Errors or (Global_Control
                               and Global_Clear_Errors) .
                            */
    UINT32 channel_err_status_2 : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel_Error_Status_2 corresponds to Channel 2.
                               When set this indicated Channel 2 has at least
                               one Data (Data_Error_Status) or ECC
                               (Ecc_Error_Status) error status bit set.
                               Channel_Error_Status_2 will be cleared when
                               setting Local_Clear_Errors or (Global_Control
                               and Global_Clear_Errors) .
                            */
    UINT32 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 channel_test_done_status_0 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000001*/

                            /*
                               Channel_Test_Done_Status_0 corresponds to
                               Channel 0 and is set in Loopback.Marker and
                               cleared in Loopback.Pattern
                            */
    UINT32 channel_test_done_status_1 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000001*/

                            /*
                               Channel_Test_Done_Status_1 corresponds to
                               Channel 1 and is set in Loopback.Marker and
                               cleared in Loopback.Pattern
                            */
    UINT32 channel_test_done_status_2 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000001*/

                            /*
                               Channel_Test_Done_Status_2 corresponds to
                               Channel 2 and is set in Loopback.Marker and
                               cleared in Loopback.Pattern
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CPGC_GLOBALERR_MC_GLOBAL_STRUCT;


/* MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG supported on:                       */
/*      SPRA0 (0x20002260)                                                      */
/*      SPRB0 (0x20002260)                                                      */
/*      SPRHBM (0x20002260)                                                     */
/*      SPRC0 (0x20002260)                                                      */
/*      SPRMCC (0x20002260)                                                     */
/*      SPRUCC (0x20002260)                                                     */
/* Register default value on SPRA0: 0x10004987                                  */
/* Register default value on SPRB0: 0x10004987                                  */
/* Register default value on SPRHBM: 0x10004987                                 */
/* Register default value on SPRC0: 0x10004987                                  */
/* Register default value on SPRMCC: 0x10004987                                 */
/* Register default value on SPRUCC: 0x10004987                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* mcdfxmisc register to hold CMI credit configuration
*/


#define MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG 0x07222260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 req_fab_credits : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000007*/

                            /* req_fab_credits */
    UINT32 rsp_fab_credits : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000000C*/

                            /* rsp_fab_credits */
    UINT32 rd_cpl_fab_credits : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000012*/

                            /* rd_cpl_fab_credits */
    UINT32 cmi_trace_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* CMI tracing enabled */
    UINT32 rsvd : 12;

                            /* Bits[27:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_ism_active_from_parent : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /* force_ism_active_from_parent */
    UINT32 rsp_ism_credits_initialized : 1;

                            /* Bits[29:29], Access Type=RW/V, default=0x00000000*/

                            /* credits initialized */
    UINT32 req_ism_credits_initialized : 1;

                            /* Bits[30:30], Access Type=RW/V, default=0x00000000*/

                            /* credits initialized */
    UINT32 credit_init_done : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* credit_init_done */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_CREDIT_CONFIG_1_MC_GLOBAL_REG supported on:                       */
/*      SPRA0 (0x20002264)                                                      */
/*      SPRB0 (0x20002264)                                                      */
/*      SPRHBM (0x20002264)                                                     */
/*      SPRC0 (0x20002264)                                                      */
/*      SPRMCC (0x20002264)                                                     */
/*      SPRUCC (0x20002264)                                                     */
/* Register default value on SPRA0: 0x10004987                                  */
/* Register default value on SPRB0: 0x10004987                                  */
/* Register default value on SPRHBM: 0x10004987                                 */
/* Register default value on SPRC0: 0x10004987                                  */
/* Register default value on SPRMCC: 0x10004987                                 */
/* Register default value on SPRUCC: 0x10004987                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* mcdfxmisc register to hold CMI credit configuration
*/


#define MCMISC_CMI_CREDIT_CONFIG_1_MC_GLOBAL_REG 0x07222264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 req_fab_credits : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000007*/

                            /* req_fab_credits */
    UINT32 rsp_fab_credits : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000000C*/

                            /* rsp_fab_credits */
    UINT32 rd_cpl_fab_credits : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000012*/

                            /* rd_cpl_fab_credits */
    UINT32 cmi_trace_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* CMI tracing enabled */
    UINT32 rsvd : 12;

                            /* Bits[27:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_ism_active_from_parent : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /* force_ism_active_from_parent */
    UINT32 rsp_ism_credits_initialized : 1;

                            /* Bits[29:29], Access Type=RW/V, default=0x00000000*/

                            /* credits initialized */
    UINT32 req_ism_credits_initialized : 1;

                            /* Bits[30:30], Access Type=RW/V, default=0x00000000*/

                            /* credits initialized */
    UINT32 credit_init_done : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* credit_init_done */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_CREDIT_CONFIG_1_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_STALL_CONFIG_0_N0_MC_GLOBAL_REG supported on:                     */
/*      SPRA0 (0x20002270)                                                      */
/*      SPRB0 (0x20002270)                                                      */
/*      SPRHBM (0x20002270)                                                     */
/*      SPRC0 (0x20002270)                                                      */
/*      SPRMCC (0x20002270)                                                     */
/*      SPRUCC (0x20002270)                                                     */
/* Register default value on SPRA0: 0xAAAAAAAA                                  */
/* Register default value on SPRB0: 0xAAAAAAAA                                  */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* Register default value on SPRC0: 0xAAAAAAAA                                  */
/* Register default value on SPRMCC: 0xAAAAAAAA                                 */
/* Register default value on SPRUCC: 0xAAAAAAAA                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Controls random CMI stall assertion behavior
*/


#define MCMISC_CMI_STALL_CONFIG_0_N0_MC_GLOBAL_REG 0x07222270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scf_req_stall_assert_seed : 10;

                            /* Bits[9:0], Access Type=RW, default=0x000002AA*/

                            /*
                               SCF request stall assert LFSR seed. Must have at
                               least one bit set in active width.
                            */
    UINT32 scf_req_stall_delay_seed : 10;

                            /* Bits[19:10], Access Type=RW, default=0x000002AA*/

                            /*
                               SCF request stall delay LFSR seed. Must have at
                               least one bit set in active width.
                            */
    UINT32 mcchan_rsp_stall_assert_seed : 10;

                            /* Bits[29:20], Access Type=RW, default=0x000002AA*/

                            /*
                               MCCHAN response stall assert LFSR seed. Must
                               have at least one bit set in active width.
                            */
    UINT32 mcchan_rsp_stall_delay_seed : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000002*/

                            /*
                               MCCHAN response stall delay LFSR seed. Must have
                               at least one bit set in active width.
                            */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_STALL_CONFIG_0_N0_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_STALL_CONFIG_0_N1_MC_GLOBAL_REG supported on:                     */
/*      SPRA0 (0x20002274)                                                      */
/*      SPRB0 (0x20002274)                                                      */
/*      SPRHBM (0x20002274)                                                     */
/*      SPRC0 (0x20002274)                                                      */
/*      SPRMCC (0x20002274)                                                     */
/*      SPRUCC (0x20002274)                                                     */
/* Register default value on SPRA0: 0x00AAAAAA                                  */
/* Register default value on SPRB0: 0x00AAAAAA                                  */
/* Register default value on SPRHBM: 0x00AAAAAA                                 */
/* Register default value on SPRC0: 0x00AAAAAA                                  */
/* Register default value on SPRMCC: 0x00AAAAAA                                 */
/* Register default value on SPRUCC: 0x00AAAAAA                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Controls random CMI stall assertion behavior
*/


#define MCMISC_CMI_STALL_CONFIG_0_N1_MC_GLOBAL_REG 0x07222274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcchan_rsp_stall_delay_seed : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000AA*/

                            /*
                               MCCHAN response stall delay LFSR seed. Must have
                               at least one bit set in active width.
                            */
    UINT32 scf_req_stall_assert_reg_width : 4;

                            /* Bits[11:8], Access Type=RW, default=0x0000000A*/

                            /* Width for scf request assert LFSR */
    UINT32 scf_req_stall_delay_reg_width : 4;

                            /* Bits[15:12], Access Type=RW, default=0x0000000A*/

                            /* Width for scf request delay LFSR */
    UINT32 mcchan_rsp_stall_assert_reg_width : 4;

                            /* Bits[19:16], Access Type=RW, default=0x0000000A*/

                            /* Width for mcchan response assert LFSR */
    UINT32 mcchan_rsp_stall_delay_reg_width : 4;

                            /* Bits[23:20], Access Type=RW, default=0x0000000A*/

                            /* Width for mcchan response delay LFSR */
    UINT32 stall_for_stop_req : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Stall outgoing CMI traffic when mcchan asserts
                               stop request
                            */
    UINT32 rsvd : 4;

                            /* Bits[28:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scf_req_stall_enable : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Bit to enable random scf req stall injection */
    UINT32 mcchan_rsp_stall_enable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Bit to enable random mcchan rsp stall injection */
    UINT32 reg_vld : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Valid to load register fields */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_STALL_CONFIG_0_N1_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_STALL_CONFIG_1_N0_MC_GLOBAL_REG supported on:                     */
/*      SPRA0 (0x20002278)                                                      */
/*      SPRB0 (0x20002278)                                                      */
/*      SPRHBM (0x20002278)                                                     */
/*      SPRC0 (0x20002278)                                                      */
/*      SPRMCC (0x20002278)                                                     */
/*      SPRUCC (0x20002278)                                                     */
/* Register default value on SPRA0: 0xAAAAAAAA                                  */
/* Register default value on SPRB0: 0xAAAAAAAA                                  */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* Register default value on SPRC0: 0xAAAAAAAA                                  */
/* Register default value on SPRMCC: 0xAAAAAAAA                                 */
/* Register default value on SPRUCC: 0xAAAAAAAA                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Controls random CMI stall assertion behavior
*/


#define MCMISC_CMI_STALL_CONFIG_1_N0_MC_GLOBAL_REG 0x07222278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scf_req_stall_assert_seed : 10;

                            /* Bits[9:0], Access Type=RW, default=0x000002AA*/

                            /*
                               SCF request stall assert LFSR seed. Must have at
                               least one bit set in active width.
                            */
    UINT32 scf_req_stall_delay_seed : 10;

                            /* Bits[19:10], Access Type=RW, default=0x000002AA*/

                            /*
                               SCF request stall delay LFSR seed. Must have at
                               least one bit set in active width.
                            */
    UINT32 mcchan_rsp_stall_assert_seed : 10;

                            /* Bits[29:20], Access Type=RW, default=0x000002AA*/

                            /*
                               MCCHAN response stall assert LFSR seed. Must
                               have at least one bit set in active width.
                            */
    UINT32 mcchan_rsp_stall_delay_seed : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000002*/

                            /*
                               MCCHAN response stall delay LFSR seed. Must have
                               at least one bit set in active width.
                            */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_STALL_CONFIG_1_N0_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_STALL_CONFIG_1_N1_MC_GLOBAL_REG supported on:                     */
/*      SPRA0 (0x2000227c)                                                      */
/*      SPRB0 (0x2000227c)                                                      */
/*      SPRHBM (0x2000227c)                                                     */
/*      SPRC0 (0x2000227c)                                                      */
/*      SPRMCC (0x2000227c)                                                     */
/*      SPRUCC (0x2000227c)                                                     */
/* Register default value on SPRA0: 0x00AAAAAA                                  */
/* Register default value on SPRB0: 0x00AAAAAA                                  */
/* Register default value on SPRHBM: 0x00AAAAAA                                 */
/* Register default value on SPRC0: 0x00AAAAAA                                  */
/* Register default value on SPRMCC: 0x00AAAAAA                                 */
/* Register default value on SPRUCC: 0x00AAAAAA                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Controls random CMI stall assertion behavior
*/


#define MCMISC_CMI_STALL_CONFIG_1_N1_MC_GLOBAL_REG 0x0722227C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcchan_rsp_stall_delay_seed : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000AA*/

                            /*
                               MCCHAN response stall delay LFSR seed. Must have
                               at least one bit set in active width.
                            */
    UINT32 scf_req_stall_assert_reg_width : 4;

                            /* Bits[11:8], Access Type=RW, default=0x0000000A*/

                            /* Width for scf request assert LFSR */
    UINT32 scf_req_stall_delay_reg_width : 4;

                            /* Bits[15:12], Access Type=RW, default=0x0000000A*/

                            /* Width for scf request delay LFSR */
    UINT32 mcchan_rsp_stall_assert_reg_width : 4;

                            /* Bits[19:16], Access Type=RW, default=0x0000000A*/

                            /* Width for mcchan response assert LFSR */
    UINT32 mcchan_rsp_stall_delay_reg_width : 4;

                            /* Bits[23:20], Access Type=RW, default=0x0000000A*/

                            /* Width for mcchan response delay LFSR */
    UINT32 stall_for_stop_req : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Stall outgoing CMI traffic when mcchan asserts
                               stop request
                            */
    UINT32 rsvd : 4;

                            /* Bits[28:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scf_req_stall_enable : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Bit to enable random scf req stall injection */
    UINT32 mcchan_rsp_stall_enable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Bit to enable random mcchan rsp stall injection */
    UINT32 reg_vld : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Valid to load register fields */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_STALL_CONFIG_1_N1_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_RD_N0_MC_GLOBAL_REG supported on:                         */
/*      SPRA0 (0x20002290)                                                      */
/*      SPRB0 (0x20002290)                                                      */
/*      SPRHBM (0x20002290)                                                     */
/*      SPRC0 (0x20002290)                                                      */
/*      SPRMCC (0x20002290)                                                     */
/*      SPRUCC (0x20002290)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for reads from all channels hooked to this dfxmisc.  It counts ddr reads for ddr, and hbm reads for hbm.
*/


#define PMON_FREE_RUN_CNTR_RD_N0_MC_GLOBAL_REG 0x07222290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_RD_N0_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_RD_N1_MC_GLOBAL_REG supported on:                         */
/*      SPRA0 (0x20002294)                                                      */
/*      SPRB0 (0x20002294)                                                      */
/*      SPRHBM (0x20002294)                                                     */
/*      SPRC0 (0x20002294)                                                      */
/*      SPRMCC (0x20002294)                                                     */
/*      SPRUCC (0x20002294)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for reads from all channels hooked to this dfxmisc.  It counts ddr reads for ddr, and hbm reads for hbm.
*/


#define PMON_FREE_RUN_CNTR_RD_N1_MC_GLOBAL_REG 0x07222294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_RD_N1_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_WR_N0_MC_GLOBAL_REG supported on:                         */
/*      SPRA0 (0x20002298)                                                      */
/*      SPRB0 (0x20002298)                                                      */
/*      SPRHBM (0x20002298)                                                     */
/*      SPRC0 (0x20002298)                                                      */
/*      SPRMCC (0x20002298)                                                     */
/*      SPRUCC (0x20002298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_FREE_RUN_CNTR_WR_N0_MC_GLOBAL_REG 0x07222298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_WR_N0_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_WR_N1_MC_GLOBAL_REG supported on:                         */
/*      SPRA0 (0x2000229c)                                                      */
/*      SPRB0 (0x2000229c)                                                      */
/*      SPRHBM (0x2000229c)                                                     */
/*      SPRC0 (0x2000229c)                                                      */
/*      SPRMCC (0x2000229c)                                                     */
/*      SPRUCC (0x2000229c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_FREE_RUN_CNTR_WR_N1_MC_GLOBAL_REG 0x0722229C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_WR_N1_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_RD_DDRT_N0_MC_GLOBAL_REG supported on:                    */
/*      SPRA0 (0x200022a0)                                                      */
/*      SPRB0 (0x200022a0)                                                      */
/*      SPRHBM (0x200022a0)                                                     */
/*      SPRC0 (0x200022a0)                                                      */
/*      SPRMCC (0x200022a0)                                                     */
/*      SPRUCC (0x200022a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for reads from all channels hooked to this dfxmisc.  It counts ddrt reads for ddr, and is unconnected for hbm.
*/


#define PMON_FREE_RUN_CNTR_RD_DDRT_N0_MC_GLOBAL_REG 0x072222A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_RD_DDRT_N0_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_RD_DDRT_N1_MC_GLOBAL_REG supported on:                    */
/*      SPRA0 (0x200022a4)                                                      */
/*      SPRB0 (0x200022a4)                                                      */
/*      SPRHBM (0x200022a4)                                                     */
/*      SPRC0 (0x200022a4)                                                      */
/*      SPRMCC (0x200022a4)                                                     */
/*      SPRUCC (0x200022a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for reads from all channels hooked to this dfxmisc.  It counts ddrt reads for ddr, and is unconnected for hbm.
*/


#define PMON_FREE_RUN_CNTR_RD_DDRT_N1_MC_GLOBAL_REG 0x072222A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_RD_DDRT_N1_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_WR_DDRT_N0_MC_GLOBAL_REG supported on:                    */
/*      SPRA0 (0x200022a8)                                                      */
/*      SPRB0 (0x200022a8)                                                      */
/*      SPRHBM (0x200022a8)                                                     */
/*      SPRC0 (0x200022a8)                                                      */
/*      SPRMCC (0x200022a8)                                                     */
/*      SPRUCC (0x200022a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddrt writes for ddr, and is unconnected for hbm.
*/


#define PMON_FREE_RUN_CNTR_WR_DDRT_N0_MC_GLOBAL_REG 0x072222A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_WR_DDRT_N0_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_WR_DDRT_N1_MC_GLOBAL_REG supported on:                    */
/*      SPRA0 (0x200022ac)                                                      */
/*      SPRB0 (0x200022ac)                                                      */
/*      SPRHBM (0x200022ac)                                                     */
/*      SPRC0 (0x200022ac)                                                      */
/*      SPRMCC (0x200022ac)                                                     */
/*      SPRUCC (0x200022ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddrt writes for ddr, and is unconnected for hbm.
*/


#define PMON_FREE_RUN_CNTR_WR_DDRT_N1_MC_GLOBAL_REG 0x072222AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_WR_DDRT_N1_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_DCLK_N0_MC_GLOBAL_REG supported on:                       */
/*      SPRA0 (0x200022b0)                                                      */
/*      SPRB0 (0x200022b0)                                                      */
/*      SPRHBM (0x200022b0)                                                     */
/*      SPRC0 (0x200022b0)                                                      */
/*      SPRMCC (0x200022b0)                                                     */
/*      SPRUCC (0x200022b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for dclk
*/


#define PMON_FREE_RUN_CNTR_DCLK_N0_MC_GLOBAL_REG 0x072222B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_DCLK_N0_MC_GLOBAL_STRUCT;

/* PMON_FREE_RUN_CNTR_DCLK_N1_MC_GLOBAL_REG supported on:                       */
/*      SPRA0 (0x200022b4)                                                      */
/*      SPRB0 (0x200022b4)                                                      */
/*      SPRHBM (0x200022b4)                                                     */
/*      SPRC0 (0x200022b4)                                                      */
/*      SPRMCC (0x200022b4)                                                     */
/*      SPRUCC (0x200022b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for dclk
*/


#define PMON_FREE_RUN_CNTR_DCLK_N1_MC_GLOBAL_REG 0x072222B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_FREE_RUN_CNTR_DCLK_N1_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N0_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022b8)                                                      */
/*      SPRB0 (0x200022b8)                                                      */
/*      SPRHBM (0x200022b8)                                                     */
/*      SPRC0 (0x200022b8)                                                      */
/*      SPRMCC (0x200022b8)                                                     */
/*      SPRUCC (0x200022b8)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access to registers in OS_W security policy group.
*/


#define MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N0_MC_GLOBAL_REG 0x072222B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N0_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N1_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022bc)                                                      */
/*      SPRB0 (0x200022bc)                                                      */
/*      SPRHBM (0x200022bc)                                                     */
/*      SPRC0 (0x200022bc)                                                      */
/*      SPRMCC (0x200022bc)                                                     */
/*      SPRUCC (0x200022bc)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access to registers in OS_W security policy group.
*/


#define MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N1_MC_GLOBAL_REG 0x072222BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} MCDFXMISC_SAI_OS_W_PG_RD_POLICY_N1_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N0_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022c0)                                                      */
/*      SPRB0 (0x200022c0)                                                      */
/*      SPRHBM (0x200022c0)                                                     */
/*      SPRC0 (0x200022c0)                                                      */
/*      SPRMCC (0x200022c0)                                                     */
/*      SPRUCC (0x200022c0)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to registers in OS_W security policy group.
*/


#define MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N0_MC_GLOBAL_REG 0x072222C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N0_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N1_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022c4)                                                      */
/*      SPRB0 (0x200022c4)                                                      */
/*      SPRHBM (0x200022c4)                                                     */
/*      SPRC0 (0x200022c4)                                                      */
/*      SPRMCC (0x200022c4)                                                     */
/*      SPRUCC (0x200022c4)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to registers in OS_W security policy group.
*/


#define MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N1_MC_GLOBAL_REG 0x072222C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W policy
                               group, based on the value from each agent's 6bit
                               SAI field.
                            */

  } Bits;
  UINT32 Data;

} MCDFXMISC_SAI_OS_W_PG_WR_POLICY_N1_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N0_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022c8)                                                      */
/*      SPRB0 (0x200022c8)                                                      */
/*      SPRHBM (0x200022c8)                                                     */
/*      SPRC0 (0x200022c8)                                                      */
/*      SPRMCC (0x200022c8)                                                     */
/*      SPRUCC (0x200022c8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N0_MC_GLOBAL_REG 0x072222C8

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

} MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N0_MC_GLOBAL_STRUCT;

/* MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N1_MC_GLOBAL_REG supported on:               */
/*      SPRA0 (0x200022cc)                                                      */
/*      SPRB0 (0x200022cc)                                                      */
/*      SPRHBM (0x200022cc)                                                     */
/*      SPRC0 (0x200022cc)                                                      */
/*      SPRMCC (0x200022cc)                                                     */
/*      SPRUCC (0x200022cc)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC
*/


#define MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N1_MC_GLOBAL_REG 0x072222CC

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

} MCDFXMISC_SAI_OS_W_PG_CR_POLICY_N1_MC_GLOBAL_STRUCT;

/* MC_PA_BLOCK_CFG_0_N0_MC_GLOBAL_REG supported on:                             */
/*      SPRA0 (0x200022d0)                                                      */
/*      SPRB0 (0x200022d0)                                                      */
/*      SPRHBM (0x200022d0)                                                     */
/*      SPRC0 (0x200022d0)                                                      */
/*      SPRMCC (0x200022d0)                                                     */
/*      SPRUCC (0x200022d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* MC protocol aware configuration register for ADL
*/


#define MC_PA_BLOCK_CFG_0_N0_MC_GLOBAL_REG 0x072222D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dso0_channel0_input_select : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Dso0 Channel 0 input select: '00:CMI Request
                               Channel0 '01:CMI Request Channel1 '10: CMI
                               Completion Channel0 '11:CMI Completion Channel1
                            */
    UINT32 dso0_channel1_input_select : 4;

                            /* Bits[5:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Dso0 Channel 1 input select: '0000:CMI
                               completion channel0 '0001:CMI completion
                               channel1 '0010:CMI completion read data channel0
                               '0011:CMI completion read data channel1
                               '0100:CMI request write data channel0 '0101:CMI
                               request write data channel1 '0110:CMI response
                               channel0 '0111:CMI response channel1 '1000:CMI
                               late completion channel0 '1001:CMI late
                               completion channel1
                            */
    UINT32 dso0_channel2_input_select : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Dso0 Channel 2 input select: '000:CMI Request
                               Channel0 '001:CMI Request Channel1 '010:CMI
                               Completion Channel0 '011:CMI Completion Channel1
                               '100:Upstream DRA Channel 0 '101:Upstream DRA
                               Channel 1
                            */
    UINT32 dso0_channel3_input_select : 4;

                            /* Bits[12:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Dso0 Channel 3 input select: '0000:CMI
                               completion channel0 '0001:CMI completion
                               channel1 '0010:CMI completion read data channel0
                               '0011:CMI completion read data channel1
                               '0100:CMI request write data channel0 '0101:CMI
                               request write data channel1 '0110:CMI response
                               channel0 '0111:CMI response channel1 '1000:CMI
                               late completion channel0 '1001:CMI late
                               completion channel1 '1010:CMI Cmd channel0
                               '1011:CMI Cmd channel1 '1100:Upstream DRA
                               Channel 0 '1101:Upstream DRA Channel 1
                            */
    UINT32 cmi_channel0_cmd_write_data_select : 4;

                            /* Bits[16:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI data select for channel0 write data. '0000:
                               write_data[15:0] '0001: write_data[31:16] '0010:
                               write_data[47:32] '0011: write_data[63:48]
                               '0100: write_data[79:64] '0101:
                               write_data[95:80] '0110: write_data[111:96]
                               '0111: write_data[127:112] '1000:
                               write_data[143:128] '1001: write_data[159:144]
                               '1010: write_data[175:160] '1011:
                               write_data[191:176] '1100: write_data[207:192]
                               '1101: write_data[223:208] '1110:
                               write_data[239:224] '1111: write_data[255:240]
                            */
    UINT32 cmi_channel1_cmd_write_data_select : 4;

                            /* Bits[20:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI data select for channel1 write data. '0000:
                               write_data[15:0] '0001: write_data[31:16] '0010:
                               write_data[47:32] '0011: write_data[63:48]
                               '0100: write_data[79:64] '0101:
                               write_data[95:80] '0110: write_data[111:96]
                               '0111: write_data[127:112] '1000:
                               write_data[143:128] '1001: write_data[159:144]
                               '1010: write_data[175:160] '1011:
                               write_data[191:176] '1100: write_data[207:192]
                               '1101: write_data[223:208] '1110:
                               write_data[239:224] '1111: write_data[255:240]
                            */
    UINT32 cmi_channel0_cpl_read_data_select : 4;

                            /* Bits[24:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI data select for channel0 rd data. '0000:
                               rd_data[15:0] '0001: rd_data[31:16] '0010:
                               rd_data[47:32] '0011: rd_data[63:48] '0100:
                               rd_data[79:64] '0101: rd_data[95:80] '0110:
                               rd_data[111:96] '0111: rd_data[127:112] '1000:
                               rd_data[143:128] '1001: rd_data[159:144] '1010:
                               rd_data[175:160] '1011: rd_data[191:176] '1100:
                               rd_data[207:192] '1101: rd_data[223:208] '1110:
                               rd_data[239:224] '1111: rd_data[255:240]
                            */
    UINT32 cmi_channel1_cpl_read_data_select : 4;

                            /* Bits[28:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI data select for channel1 rd data. '0000:
                               rd_data[15:0] '0001: rd_data[31:16] '0010:
                               rd_data[47:32] '0011: rd_data[63:48] '0100:
                               rd_data[79:64] '0101: rd_data[95:80] '0110:
                               rd_data[111:96] '0111: rd_data[127:112] '1000:
                               rd_data[143:128] '1001: rd_data[159:144] '1010:
                               rd_data[175:160] '1011: rd_data[191:176] '1100:
                               rd_data[207:192] '1101: rd_data[223:208] '1110:
                               rd_data[239:224] '1111: rd_data[255:240]
                            */
    UINT32 dso1_channel0_input_select : 2;

                            /* Bits[30:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               selects between channel0 and channel 1 of the
                               spid data buses coming into this block from the
                               two schedulers and the dedup data '00: channel0
                               spid '01: channel1 spid '10: channel0 DRA '11:
                               channel1 DRA
                            */
    UINT32 channel_address_select : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI address select 2'b00: begin
                               spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[21:6]; end
                               2'b01: begin spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[37:22]; end
                               2'b10: begin spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[44:29]; end
                               2'b11: begin spr_cmi_req_to_dso[i].req_address =
                               {cmiReqCmdIfc111[i].cmd.req_address[44:37],cmiRe
                               qCmdIfc111[i].cmd.req_address[13:6]};
                            */

  } Bits;
  UINT32 Data;

} MC_PA_BLOCK_CFG_0_N0_MC_GLOBAL_STRUCT;

/* MC_PA_BLOCK_CFG_0_N1_MC_GLOBAL_REG supported on:                             */
/*      SPRA0 (0x200022d4)                                                      */
/*      SPRB0 (0x200022d4)                                                      */
/*      SPRHBM (0x200022d4)                                                     */
/*      SPRC0 (0x200022d4)                                                      */
/*      SPRMCC (0x200022d4)                                                     */
/*      SPRUCC (0x200022d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* MC protocol aware configuration register for ADL
*/


#define MC_PA_BLOCK_CFG_0_N1_MC_GLOBAL_REG 0x072222D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 channel_address_select : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMI address select 2'b00: begin
                               spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[21:6]; end
                               2'b01: begin spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[37:22]; end
                               2'b10: begin spr_cmi_req_to_dso[i].req_address =
                               cmiReqCmdIfc111[i].cmd.req_address[44:29]; end
                               2'b11: begin spr_cmi_req_to_dso[i].req_address =
                               {cmiReqCmdIfc111[i].cmd.req_address[44:37],cmiRe
                               qCmdIfc111[i].cmd.req_address[13:6]};
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_PA_BLOCK_CFG_0_N1_MC_GLOBAL_STRUCT;









/* CPGC_GLOBAL_SYNC_MC_GLOBAL_REG supported on:                                 */
/*      SPRA0 (0x20002300)                                                      */
/*      SPRB0 (0x20002300)                                                      */
/*      SPRHBM (0x20002300)                                                     */
/*      SPRC0 (0x20002300)                                                      */
/*      SPRMCC (0x20002300)                                                     */
/*      SPRUCC (0x20002300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CPGC                                             */
/* SPRB0 Security PolicyGroup: CPGC                                             */
/* SPRHBM Security PolicyGroup: CPGC                                            */
/* SPRC0 Security PolicyGroup: CPGC                                             */
/* SPRMCC Security PolicyGroup: CPGC                                            */
/* SPRUCC Security PolicyGroup: CPGC                                            */
/* CPGC Global Synchronous Control - This register is used to Synchronize CPGC start events across tiles with +- 2 dclk accuracy.
BIOS can read the counter_val, add an offset to it, and write value into trigger_val with trigger_arm and trigger_enable set.
*/


#define CPGC_GLOBAL_SYNC_MC_GLOBAL_REG 0x07222300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 trigger_val : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               This value is compared with a running counter
                               counter_val. If sync trigger_arm bit is set,
                               then a trigger will be issued for a match. If
                               sync trigger_enable is set, then a CPGC Global
                               start_test will be set by hardware.
                            */
    UINT32 trigger_arm : 1;

                            /* Bits[10:10], Access Type=RW/V, default=0x00000000*/

                            /*
                               This arms the synchronous trigger. This bit is
                               cleared after a trigger event (volatile).
                            */
    UINT32 trigger_enable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               This bit enables synchronous trigger start
                               feature. CPGC Global start_test will be set when
                               a trigger is issued.
                            */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counter_val : 10;

                            /* Bits[25:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is a read-only running counter that is
                               approximately is synchronized between memory
                               subsystem tiles. This counter counts USync
                               events which are 120ns apart
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CPGC_GLOBAL_SYNC_MC_GLOBAL_STRUCT;


/* PMON_RPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_REG supported on:                        */
/*      SPRA0 (0x20002318)                                                      */
/*      SPRB0 (0x20002318)                                                      */
/*      SPRHBM (0x20002318)                                                     */
/*      SPRC0 (0x20002318)                                                      */
/*      SPRMCC (0x20002318)                                                     */
/*      SPRUCC (0x20002318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_RPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_REG 0x07222318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_RPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_STRUCT;

/* PMON_RPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_REG supported on:                        */
/*      SPRA0 (0x2000231c)                                                      */
/*      SPRB0 (0x2000231c)                                                      */
/*      SPRHBM (0x2000231c)                                                     */
/*      SPRC0 (0x2000231c)                                                      */
/*      SPRMCC (0x2000231c)                                                     */
/*      SPRUCC (0x2000231c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_RPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_REG 0x0722231C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_RPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_STRUCT;

/* PMON_WPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_REG supported on:                        */
/*      SPRA0 (0x20002320)                                                      */
/*      SPRB0 (0x20002320)                                                      */
/*      SPRHBM (0x20002320)                                                     */
/*      SPRC0 (0x20002320)                                                      */
/*      SPRMCC (0x20002320)                                                     */
/*      SPRUCC (0x20002320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_WPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_REG 0x07222320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_WPQ_ACTIVE_CYCLES_N0_MC_GLOBAL_STRUCT;

/* PMON_WPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_REG supported on:                        */
/*      SPRA0 (0x20002324)                                                      */
/*      SPRB0 (0x20002324)                                                      */
/*      SPRHBM (0x20002324)                                                     */
/*      SPRC0 (0x20002324)                                                      */
/*      SPRMCC (0x20002324)                                                     */
/*      SPRUCC (0x20002324)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This reg is a free running pmon counter for writes from all channels hooked to this dfxmisc.  It counts ddr writes for ddr, and hbm writes for hbm.
*/


#define PMON_WPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_REG 0x07222324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 countervalue : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_WPQ_ACTIVE_CYCLES_N1_MC_GLOBAL_STRUCT;

/* BGF_PMON_CONFIG_0_MC_GLOBAL_REG supported on:                                */
/*      SPRA0 (0x20002328)                                                      */
/*      SPRB0 (0x20002328)                                                      */
/*      SPRHBM (0x20002328)                                                     */
/*      SPRC0 (0x20002328)                                                      */
/*      SPRMCC (0x20002328)                                                     */
/*      SPRUCC (0x20002328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the hardware to count events based upon the desired BGF and event 
*/


#define BGF_PMON_CONFIG_0_MC_GLOBAL_REG 0x07222328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bgf_event : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: idle, no counting, 1: BGF inserts, 2: BGF
                               deallocations, 3: monitor the current size of
                               the selected BGF, 4: count each cycle that the
                               selected BGF is full, 5: count each cycle that
                               the selected BGF is empty
                            */
    UINT32 bgf_select_vec : 11;

                            /* Bits[13:3], Access Type=RW, default=0x00000000*/

                            /*
                               BGF bit vector to select which BGF to monitor.
                               SPR encodings as follows. 0: REQ_CMD, 1:
                               REQ_DATA, 2: PRODUCER_FLOW_CONTROL, 3: M2M_MISC,
                               RD_CPL_CMD, 5: RD_CPL_DATA, 6:
                               CONSUMER_FLOW_CONTROL, 7: RSP, 8: RD_CPL_LATE,
                               9: MCCHAN_MISC, 10: ANY_BGF, 11:
                               ANY_REQUEST_BGF, 12: ANY_RESPONSE_BGF. ATS
                               encodings as follows. 0: REQ_CMD, 1: REQ_DATA,
                               2: PRODUCER_FLOW_CONTROL, 3: RD_CPL_CMD, 4:
                               RD_CPL_DATA, 5: CONSUMER_FLOW_CONTROL, 6: RSP,
                               7: ANY_BGF, 8: ANY_REQUEST_BGF, 9:
                               ANY_RESPONSE_BGF
                            */
    UINT32 reset_count : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* reset the counter */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BGF_PMON_CONFIG_0_MC_GLOBAL_STRUCT;

/* BGF_PMON_CONFIG_1_MC_GLOBAL_REG supported on:                                */
/*      SPRA0 (0x2000232c)                                                      */
/*      SPRB0 (0x2000232c)                                                      */
/*      SPRHBM (0x2000232c)                                                     */
/*      SPRC0 (0x2000232c)                                                      */
/*      SPRMCC (0x2000232c)                                                     */
/*      SPRUCC (0x2000232c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the hardware to count events based upon the desired BGF and event 
*/


#define BGF_PMON_CONFIG_1_MC_GLOBAL_REG 0x0722232C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bgf_event : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: idle, no counting, 1: BGF inserts, 2: BGF
                               deallocations, 3: monitor the current size of
                               the selected BGF, 4: count each cycle that the
                               selected BGF is full, 5: count each cycle that
                               the selected BGF is empty
                            */
    UINT32 bgf_select_vec : 11;

                            /* Bits[13:3], Access Type=RW, default=0x00000000*/

                            /*
                               BGF bit vector to select which BGF to monitor.
                               SPR encodings as follows. 0: REQ_CMD, 1:
                               REQ_DATA, 2: PRODUCER_FLOW_CONTROL, 3: M2M_MISC,
                               RD_CPL_CMD, 5: RD_CPL_DATA, 6:
                               CONSUMER_FLOW_CONTROL, 7: RSP, 8: RD_CPL_LATE,
                               9: MCCHAN_MISC, 10: ANY_BGF, 11:
                               ANY_REQUEST_BGF, 12: ANY_RESPONSE_BGF. ATS
                               encodings as follows. 0: REQ_CMD, 1: REQ_DATA,
                               2: PRODUCER_FLOW_CONTROL, 3: RD_CPL_CMD, 4:
                               RD_CPL_DATA, 5: CONSUMER_FLOW_CONTROL, 6: RSP,
                               7: ANY_BGF, 8: ANY_REQUEST_BGF, 9:
                               ANY_RESPONSE_BGF
                            */
    UINT32 reset_count : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* reset the counter */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BGF_PMON_CONFIG_1_MC_GLOBAL_STRUCT;

/* BGF_PMON_COUNTER_0_MC_GLOBAL_REG supported on:                               */
/*      SPRA0 (0x20002330)                                                      */
/*      SPRB0 (0x20002330)                                                      */
/*      SPRHBM (0x20002330)                                                     */
/*      SPRC0 (0x20002330)                                                      */
/*      SPRMCC (0x20002330)                                                     */
/*      SPRUCC (0x20002330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON counter controlled by mcmisc_bgf_pmon_config
*/


#define BGF_PMON_COUNTER_0_MC_GLOBAL_REG 0x07222330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 value : 28;

                            /* Bits[27:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BGF_PMON_COUNTER_0_MC_GLOBAL_STRUCT;

/* BGF_PMON_COUNTER_1_MC_GLOBAL_REG supported on:                               */
/*      SPRA0 (0x20002334)                                                      */
/*      SPRB0 (0x20002334)                                                      */
/*      SPRHBM (0x20002334)                                                     */
/*      SPRC0 (0x20002334)                                                      */
/*      SPRMCC (0x20002334)                                                     */
/*      SPRUCC (0x20002334)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON counter controlled by mcmisc_bgf_pmon_config
*/


#define BGF_PMON_COUNTER_1_MC_GLOBAL_REG 0x07222334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 value : 28;

                            /* Bits[27:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the current value of the counter. */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BGF_PMON_COUNTER_1_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_ISM_CONFIG_0_MC_GLOBAL_REG supported on:                          */
/*      SPRA0 (0x20002338)                                                      */
/*      SPRB0 (0x20002338)                                                      */
/*      SPRHBM (0x20002338)                                                     */
/*      SPRC0 (0x20002338)                                                      */
/*      SPRMCC (0x20002338)                                                     */
/*      SPRUCC (0x20002338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* mcdfxmisc register to hold CMI credit configuration
*/


#define MCMISC_CMI_ISM_CONFIG_0_MC_GLOBAL_REG 0x07222338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idle_req_timeout : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the IDLE_REQ timeout count. If the ISM
                               responder has not reached IDLE within this time,
                               after the requestor has entered IDLE_REQ, the
                               responder will transition to the IDLE_NAK state
                               to break the deadlock and give the requestor
                               another chance to revert back to ACTIVE.
                            */
    UINT32 rsvd : 14;

                            /* Bits[29:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_req_timeout_signal_error : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls whether the mcdfxmisc
                               responder ISM will signal an error when an idle
                               req timeout is detected
                            */
    UINT32 idle_req_timeout_enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls whether the mcdfxmisc
                               responder ISM will generate an IDLE_REQ NAK
                               message when the programmed idle_req_timeout
                               counter expires.
                            */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_ISM_CONFIG_0_MC_GLOBAL_STRUCT;

/* MCMISC_CMI_ISM_CONFIG_1_MC_GLOBAL_REG supported on:                          */
/*      SPRA0 (0x2000233c)                                                      */
/*      SPRB0 (0x2000233c)                                                      */
/*      SPRHBM (0x2000233c)                                                     */
/*      SPRC0 (0x2000233c)                                                      */
/*      SPRMCC (0x2000233c)                                                     */
/*      SPRUCC (0x2000233c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRB0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRHBM Register File:    sprsp_top/mcmisc_top[4]/mcmisc/MEM_MCMISC_CSR       */
/* SPRC0 Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR        */
/* SPRMCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* SPRUCC Register File:    sprsp_top/mcmisc_top[0]/mcmisc/MEM_MCMISC_CSR       */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* mcdfxmisc register to hold CMI credit configuration
*/


#define MCMISC_CMI_ISM_CONFIG_1_MC_GLOBAL_REG 0x0722233C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idle_req_timeout : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the IDLE_REQ timeout count. If the ISM
                               responder has not reached IDLE within this time,
                               after the requestor has entered IDLE_REQ, the
                               responder will transition to the IDLE_NAK state
                               to break the deadlock and give the requestor
                               another chance to revert back to ACTIVE.
                            */
    UINT32 rsvd : 14;

                            /* Bits[29:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_req_timeout_signal_error : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls whether the mcdfxmisc
                               responder ISM will signal an error when an idle
                               req timeout is detected
                            */
    UINT32 idle_req_timeout_enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls whether the mcdfxmisc
                               responder ISM will generate an IDLE_REQ NAK
                               message when the programmed idle_req_timeout
                               counter expires.
                            */

  } Bits;
  UINT32 Data;

} MCMISC_CMI_ISM_CONFIG_1_MC_GLOBAL_STRUCT;
#endif /* _MC_GLOBAL_h */
