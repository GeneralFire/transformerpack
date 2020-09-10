
/** @file
  M3KTI_MAIN.h

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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
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


#ifndef _M3KTI_MAIN_h
#define _M3KTI_MAIN_h
#include <Base.h>

/* VID_M3KTI_MAIN_REG supported on:                                             */
/*      SPRA0 (0x11e28000)                                                      */
/*      SPRB0 (0x11e28000)                                                      */
/*      SPRHBM (0x11e28000)                                                     */
/*      SPRC0 (0x11e28000)                                                      */
/*      SPRMCC (0x11e28000)                                                     */
/*      SPRUCC (0x11e4a000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_M3KTI_MAIN_REG 0x0A010000

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

} VID_M3KTI_MAIN_STRUCT;

/* DID_0_M3KTI_MAIN_REG supported on:                                           */
/*      SPRA0 (0x11e28002)                                                      */
/*      SPRB0 (0x11e28002)                                                      */
/*      SPRHBM (0x11e28002)                                                     */
/*      SPRC0 (0x11e28002)                                                      */
/*      SPRMCC (0x11e28002)                                                     */
/*      SPRUCC (0x11e4a002)                                                     */
/* Register default value on SPRA0: 0x00003245                                  */
/* Register default value on SPRB0: 0x00003245                                  */
/* Register default value on SPRHBM: 0x00003245                                 */
/* Register default value on SPRC0: 0x00003245                                  */
/* Register default value on SPRMCC: 0x00003245                                 */
/* Register default value on SPRUCC: 0x00003245                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_0_M3KTI_MAIN_REG 0x0A010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003245*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_0_M3KTI_MAIN_STRUCT;

/* PCICMD_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x11e28004)                                                      */
/*      SPRB0 (0x11e28004)                                                      */
/*      SPRHBM (0x11e28004)                                                     */
/*      SPRC0 (0x11e28004)                                                      */
/*      SPRMCC (0x11e28004)                                                     */
/*      SPRUCC (0x11e4a004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_0_M3KTI_MAIN_REG 0x0A010004

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

} PCICMD_0_M3KTI_MAIN_STRUCT;

/* PCISTS_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x11e28006)                                                      */
/*      SPRB0 (0x11e28006)                                                      */
/*      SPRHBM (0x11e28006)                                                     */
/*      SPRC0 (0x11e28006)                                                      */
/*      SPRMCC (0x11e28006)                                                     */
/*      SPRUCC (0x11e4a006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_0_M3KTI_MAIN_REG 0x0A010006

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

} PCISTS_0_M3KTI_MAIN_STRUCT;

/* RID_CCR_0_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e28008)                                                      */
/*      SPRB0 (0x21e28008)                                                      */
/*      SPRHBM (0x21e28008)                                                     */
/*      SPRC0 (0x21e28008)                                                      */
/*      SPRMCC (0x21e28008)                                                     */
/*      SPRUCC (0x21e4a008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_0_M3KTI_MAIN_REG 0x0A020008

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

} RID_CCR_0_M3KTI_MAIN_STRUCT;

/* CLSR_0_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x1e2800c)                                                       */
/*      SPRB0 (0x1e2800c)                                                       */
/*      SPRHBM (0x1e2800c)                                                      */
/*      SPRC0 (0x1e2800c)                                                       */
/*      SPRMCC (0x1e2800c)                                                      */
/*      SPRUCC (0x1e4a00c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_0_M3KTI_MAIN_REG 0x0A00000C

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

} CLSR_0_M3KTI_MAIN_STRUCT;

/* PLAT_0_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x1e2800d)                                                       */
/*      SPRB0 (0x1e2800d)                                                       */
/*      SPRHBM (0x1e2800d)                                                      */
/*      SPRC0 (0x1e2800d)                                                       */
/*      SPRMCC (0x1e2800d)                                                      */
/*      SPRUCC (0x1e4a00d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_0_M3KTI_MAIN_REG 0x0A00000D

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

} PLAT_0_M3KTI_MAIN_STRUCT;

/* HDR_0_M3KTI_MAIN_REG supported on:                                           */
/*      SPRA0 (0x1e2800e)                                                       */
/*      SPRB0 (0x1e2800e)                                                       */
/*      SPRHBM (0x1e2800e)                                                      */
/*      SPRC0 (0x1e2800e)                                                       */
/*      SPRMCC (0x1e2800e)                                                      */
/*      SPRUCC (0x1e4a00e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_0_M3KTI_MAIN_REG 0x0A00000E

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

} HDR_0_M3KTI_MAIN_STRUCT;

/* BIST_0_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x1e2800f)                                                       */
/*      SPRB0 (0x1e2800f)                                                       */
/*      SPRHBM (0x1e2800f)                                                      */
/*      SPRC0 (0x1e2800f)                                                       */
/*      SPRMCC (0x1e2800f)                                                      */
/*      SPRUCC (0x1e4a00f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_0_M3KTI_MAIN_REG 0x0A00000F

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

} BIST_0_M3KTI_MAIN_STRUCT;

/* SVID_M3KTI_MAIN_REG supported on:                                            */
/*      SPRA0 (0x11e2802c)                                                      */
/*      SPRB0 (0x11e2802c)                                                      */
/*      SPRHBM (0x11e2802c)                                                     */
/*      SPRC0 (0x11e2802c)                                                      */
/*      SPRMCC (0x11e2802c)                                                     */
/*      SPRUCC (0x11e4a02c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_M3KTI_MAIN_REG 0x0A01002C

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

} SVID_M3KTI_MAIN_STRUCT;

/* SDID_0_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x11e2802e)                                                      */
/*      SPRB0 (0x11e2802e)                                                      */
/*      SPRHBM (0x11e2802e)                                                     */
/*      SPRC0 (0x11e2802e)                                                      */
/*      SPRMCC (0x11e2802e)                                                     */
/*      SPRUCC (0x11e4a02e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_0_M3KTI_MAIN_REG 0x0A01002E

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

} SDID_0_M3KTI_MAIN_STRUCT;

/* CAPPTR_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x1e28034)                                                       */
/*      SPRB0 (0x1e28034)                                                       */
/*      SPRHBM (0x1e28034)                                                      */
/*      SPRC0 (0x1e28034)                                                       */
/*      SPRMCC (0x1e28034)                                                      */
/*      SPRUCC (0x1e4a034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_0_M3KTI_MAIN_REG 0x0A000034

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

} CAPPTR_0_M3KTI_MAIN_STRUCT;

/* INTL_0_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x1e2803c)                                                       */
/*      SPRB0 (0x1e2803c)                                                       */
/*      SPRHBM (0x1e2803c)                                                      */
/*      SPRC0 (0x1e2803c)                                                       */
/*      SPRMCC (0x1e2803c)                                                      */
/*      SPRUCC (0x1e4a03c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_0_M3KTI_MAIN_REG 0x0A00003C

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

} INTL_0_M3KTI_MAIN_STRUCT;

/* INTPIN_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x1e2803d)                                                       */
/*      SPRB0 (0x1e2803d)                                                       */
/*      SPRHBM (0x1e2803d)                                                      */
/*      SPRC0 (0x1e2803d)                                                       */
/*      SPRMCC (0x1e2803d)                                                      */
/*      SPRUCC (0x1e4a03d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_0_M3KTI_MAIN_REG 0x0A00003D

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

} INTPIN_0_M3KTI_MAIN_STRUCT;

/* MINGNT_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x1e2803e)                                                       */
/*      SPRB0 (0x1e2803e)                                                       */
/*      SPRHBM (0x1e2803e)                                                      */
/*      SPRC0 (0x1e2803e)                                                       */
/*      SPRMCC (0x1e2803e)                                                      */
/*      SPRUCC (0x1e4a03e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_0_M3KTI_MAIN_REG 0x0A00003E

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

} MINGNT_0_M3KTI_MAIN_STRUCT;

/* MAXLAT_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x1e2803f)                                                       */
/*      SPRB0 (0x1e2803f)                                                       */
/*      SPRHBM (0x1e2803f)                                                      */
/*      SPRC0 (0x1e2803f)                                                       */
/*      SPRMCC (0x1e2803f)                                                      */
/*      SPRUCC (0x1e4a03f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_0_M3KTI_MAIN_REG 0x0A00003F

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

} MAXLAT_0_M3KTI_MAIN_STRUCT;

/* PXPCAP_0_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x21e28040)                                                      */
/*      SPRB0 (0x21e28040)                                                      */
/*      SPRHBM (0x21e28040)                                                     */
/*      SPRC0 (0x21e28040)                                                      */
/*      SPRMCC (0x21e28040)                                                     */
/*      SPRUCC (0x21e4a040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_0_M3KTI_MAIN_REG 0x0A020040

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

} PXPCAP_0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD0_0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28044)                                                      */
/*      SPRB0 (0x21e28044)                                                      */
/*      SPRHBM (0x21e28044)                                                     */
/*      SPRC0 (0x21e28044)                                                      */
/*      SPRMCC (0x21e28044)                                                     */
/*      SPRUCC (0x21e4a044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_0_M3KTI_MAIN_REG 0x0A020044

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

} PCIE_RSVD0_0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD1_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28048)                                                      */
/*      SPRB0 (0x21e28048)                                                      */
/*      SPRHBM (0x21e28048)                                                     */
/*      SPRC0 (0x21e28048)                                                      */
/*      SPRMCC (0x21e28048)                                                     */
/*      SPRUCC (0x21e4a048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_N0_M3KTI_MAIN_REG 0x0A020048

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

} PCIE_RSVD1_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD1_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e2804c)                                                      */
/*      SPRB0 (0x21e2804c)                                                      */
/*      SPRHBM (0x21e2804c)                                                     */
/*      SPRC0 (0x21e2804c)                                                      */
/*      SPRMCC (0x21e2804c)                                                     */
/*      SPRUCC (0x21e4a04c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_0_N1_M3KTI_MAIN_REG 0x0A02004C

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

} PCIE_RSVD1_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD2_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28050)                                                      */
/*      SPRB0 (0x21e28050)                                                      */
/*      SPRHBM (0x21e28050)                                                     */
/*      SPRC0 (0x21e28050)                                                      */
/*      SPRMCC (0x21e28050)                                                     */
/*      SPRUCC (0x21e4a050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_N0_M3KTI_MAIN_REG 0x0A020050

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

} PCIE_RSVD2_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD2_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28054)                                                      */
/*      SPRB0 (0x21e28054)                                                      */
/*      SPRHBM (0x21e28054)                                                     */
/*      SPRC0 (0x21e28054)                                                      */
/*      SPRMCC (0x21e28054)                                                     */
/*      SPRUCC (0x21e4a054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_0_N1_M3KTI_MAIN_REG 0x0A020054

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

} PCIE_RSVD2_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD3_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28058)                                                      */
/*      SPRB0 (0x21e28058)                                                      */
/*      SPRHBM (0x21e28058)                                                     */
/*      SPRC0 (0x21e28058)                                                      */
/*      SPRMCC (0x21e28058)                                                     */
/*      SPRUCC (0x21e4a058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_N0_M3KTI_MAIN_REG 0x0A020058

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

} PCIE_RSVD3_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD3_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e2805c)                                                      */
/*      SPRB0 (0x21e2805c)                                                      */
/*      SPRHBM (0x21e2805c)                                                     */
/*      SPRC0 (0x21e2805c)                                                      */
/*      SPRMCC (0x21e2805c)                                                     */
/*      SPRUCC (0x21e4a05c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_0_N1_M3KTI_MAIN_REG 0x0A02005C

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

} PCIE_RSVD3_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD4_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28060)                                                      */
/*      SPRB0 (0x21e28060)                                                      */
/*      SPRHBM (0x21e28060)                                                     */
/*      SPRC0 (0x21e28060)                                                      */
/*      SPRMCC (0x21e28060)                                                     */
/*      SPRUCC (0x21e4a060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_N0_M3KTI_MAIN_REG 0x0A020060

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

} PCIE_RSVD4_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD4_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28064)                                                      */
/*      SPRB0 (0x21e28064)                                                      */
/*      SPRHBM (0x21e28064)                                                     */
/*      SPRC0 (0x21e28064)                                                      */
/*      SPRMCC (0x21e28064)                                                     */
/*      SPRUCC (0x21e4a064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_0_N1_M3KTI_MAIN_REG 0x0A020064

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

} PCIE_RSVD4_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD5_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28068)                                                      */
/*      SPRB0 (0x21e28068)                                                      */
/*      SPRHBM (0x21e28068)                                                     */
/*      SPRC0 (0x21e28068)                                                      */
/*      SPRMCC (0x21e28068)                                                     */
/*      SPRUCC (0x21e4a068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_N0_M3KTI_MAIN_REG 0x0A020068

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

} PCIE_RSVD5_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD5_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e2806c)                                                      */
/*      SPRB0 (0x21e2806c)                                                      */
/*      SPRHBM (0x21e2806c)                                                     */
/*      SPRC0 (0x21e2806c)                                                      */
/*      SPRMCC (0x21e2806c)                                                     */
/*      SPRUCC (0x21e4a06c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_0_N1_M3KTI_MAIN_REG 0x0A02006C

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

} PCIE_RSVD5_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD6_0_N0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28070)                                                      */
/*      SPRB0 (0x21e28070)                                                      */
/*      SPRHBM (0x21e28070)                                                     */
/*      SPRC0 (0x21e28070)                                                      */
/*      SPRMCC (0x21e28070)                                                     */
/*      SPRUCC (0x21e4a070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_N0_M3KTI_MAIN_REG 0x0A020070

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

} PCIE_RSVD6_0_N0_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD6_0_N1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28074)                                                      */
/*      SPRB0 (0x21e28074)                                                      */
/*      SPRHBM (0x21e28074)                                                     */
/*      SPRC0 (0x21e28074)                                                      */
/*      SPRMCC (0x21e28074)                                                     */
/*      SPRUCC (0x21e4a074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_0_N1_M3KTI_MAIN_REG 0x0A020074

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

} PCIE_RSVD6_0_N1_M3KTI_MAIN_STRUCT;

/* PCIE_RSVD7_0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28078)                                                      */
/*      SPRB0 (0x21e28078)                                                      */
/*      SPRHBM (0x21e28078)                                                     */
/*      SPRC0 (0x21e28078)                                                      */
/*      SPRMCC (0x21e28078)                                                     */
/*      SPRUCC (0x21e4a078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_0_M3KTI_MAIN_REG 0x0A020078

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

} PCIE_RSVD7_0_M3KTI_MAIN_STRUCT;

/* M3KKRT_M3KTI_MAIN_REG supported on:                                          */
/*      SPRA0 (0x21e28080)                                                      */
/*      SPRB0 (0x21e28080)                                                      */
/*      SPRHBM (0x21e28080)                                                     */
/*      SPRC0 (0x21e28080)                                                      */
/*      SPRMCC (0x21e28080)                                                     */
/*      SPRUCC (0x21e4a080)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M3KKRT
*/


#define M3KKRT_M3KTI_MAIN_REG 0x0A020080

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 port0routeskt : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               8-deep indexed by DNID. The output is 2-bits
                               each indicating target Port. Encoding is:
                               00-Intel UPI Port 0, 01-Intel UPI port 1,
                               10-Intel UPI Port 2, 11-Local Socket Note: It is
                               illegal to program port source to target itself.
                            */
    UINT32 port1routeskt : 16;

                            /* Bits[31:16], Access Type=RW, default=0x0000FFFF*/

                            /*
                               8-deep indexed by DNID. The output is 2-bits
                               each indicating target Port. Encoding is:
                               00-Intel UPI Port 0, 01-Intel UPI port 1,
                               10-Intel UPI Port 2, 11-Local Socket Note: It is
                               illegal to program port source to target itself.
                            */

  } Bits;
  UINT32 Data;

} M3KKRT_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 port0routeskt : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               8-deep indexed by DNID. The output is 2-bits
                               each indicating target Port. Encoding is:
                               00-Intel UPI Port 0, 01-Intel UPI port 1,
                               10-Intel UPI Port 2, 11-Local Socket Note: It is
                               illegal to program port source to target itself.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KKRT_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 port0routeskt : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               8-deep indexed by DNID. The output is 2-bits
                               each indicating target Port. Encoding is:
                               00-Intel UPI Port 0, 01-Intel UPI port 1,
                               10-Intel UPI Port 2, 11-Local Socket Note: It is
                               illegal to program port source to target itself.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KKRT_M3KTI_MAIN_STRUCT;

/* M3KRTE0CR_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e28084)                                                      */
/*      SPRB0 (0x21e28084)                                                      */
/*      SPRHBM (0x21e28084)                                                     */
/*      SPRC0 (0x21e28084)                                                      */
/*      SPRMCC (0x21e28084)                                                     */
/*      SPRUCC (0x21e4a084)                                                     */
/* Register default value on SPRA0: 0x15400540                                  */
/* Register default value on SPRB0: 0x15400540                                  */
/* Register default value on SPRHBM: 0x15400540                                 */
/* Register default value on SPRC0: 0x15400540                                  */
/* Register default value on SPRMCC: 0x15400540                                 */
/* Register default value on SPRUCC: 0x15400540                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KRTE0CR
*/


#define M3KRTE0CR_M3KTI_MAIN_REG 0x0A020084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_rt_ad : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /* VNA Route Through Credits For AD Messages */
    UINT32 vn0_rt_snp : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through SNP Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_req : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through REQ Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_rsp_ad : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through RSP Credits for AD Messages
                               Value also applies for VN1 credits when VN1 is
                               enabled.
                            */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vna_rt_bl : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /* VNA Route Through Credits For BL Messages */
    UINT32 vn0_rt_rsp_bl : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through RSP Credits For BL Messages
                               Value also applies for VN1 credits when VN1 is
                               enabled.
                            */
    UINT32 vn0_rt_ncs : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through NCS Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_ncb : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through NCB Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_wb : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through WB Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KRTE0CR_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KRTE1CR_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e28088)                                                      */
/*      SPRB0 (0x21e28088)                                                      */
/*      SPRHBM (0x21e28088)                                                     */
/*      SPRC0 (0x21e28088)                                                      */
/*      SPRMCC (0x21e28088)                                                     */
/* Register default value on SPRA0: 0x05500150                                  */
/* Register default value on SPRB0: 0x05500150                                  */
/* Register default value on SPRHBM: 0x05500150                                 */
/* Register default value on SPRC0: 0x05500150                                  */
/* Register default value on SPRMCC: 0x05500150                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3KRTE1CR
*/


#define M3KRTE1CR_M3KTI_MAIN_REG 0x0A020088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_rt_ad : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* VNA Route Through Credits For AD Messages */
    UINT32 vn0_rt_snp : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through SNP Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_req : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through REQ Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_rsp_ad : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through RSP Credits for AD Messages
                               Value also applies for VN1 credits when VN1 is
                               enabled.
                            */
    UINT32 rsvd : 6;

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vna_rt_bl : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* VNA Route Through Credits For BL Messages */
    UINT32 vn0_rt_rsp_bl : 2;

                            /* Bits[21:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through RSP Credits For BL Messages
                               Value also applies for VN1 credits when VN1 is
                               enabled.
                            */
    UINT32 vn0_rt_ncs : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through NCS Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_ncb : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through NCB Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 vn0_rt_wb : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               VN0 Route Through WB Credits Value also applies
                               for VN1 credits when VN1 is enabled.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KRTE1CR_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* M3KEGRCTRL0_M3KTI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x21e28090)                                                      */
/*      SPRB0 (0x21e28090)                                                      */
/*      SPRHBM (0x21e28090)                                                     */
/*      SPRC0 (0x21e28090)                                                      */
/*      SPRMCC (0x21e28090)                                                     */
/*      SPRUCC (0x21e4a090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KEGRCTRL0
*/


#define M3KEGRCTRL0_M3KTI_MAIN_REG 0x0A020090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rpq_vna : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Shared Credits to RPQ in all CHA. RPQ size is
                               24. 3 RSVD RPQ per enabled Intel UPI Port. The
                               remander are substraction of the reserved should
                               be split evenly between all active Intel UPI
                               ports (and programmed into this register).
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pre_mode : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               PRE Mode encoding for D2C messages. 0: 1-hop
                               encoding of PRE[4:3] 1: 2-hop encoding of
                               PRE[4:3] D2C are only supported from directly
                               connected socket, so only expected usage case
                               for 2-hop encoding is for Node Controller
                               topologies.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KEGRCTRL0_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KEGRCTRL1_M3KTI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x21e28094)                                                      */
/*      SPRB0 (0x21e28094)                                                      */
/*      SPRHBM (0x21e28094)                                                     */
/*      SPRC0 (0x21e28094)                                                      */
/*      SPRMCC (0x21e28094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3KEGRCTRL1
*/


#define M3KEGRCTRL1_M3KTI_MAIN_REG 0x0A020094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rpq_vna : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Shared Credits to RPQ in all CHA. RPQ size is
                               24. 3 RSVD RPQ per enabled Intel UPI Port. The
                               remander are substraction of the reserved should
                               be split evenly between all active Intel UPI
                               ports (and programmed into this register).
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pre_mode : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               PRE Mode encoding for D2C messages. 0: 1-hop
                               encoding of PRE[4:3] 1: 2-hop encoding of
                               PRE[4:3] D2C are only supported from directly
                               connected socket, so only expected usage case
                               for 2-hop encoding is for Node Controller
                               topologies.
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KEGRCTRL1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* M3KPCIR0CR_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e28098)                                                      */
/*      SPRB0 (0x21e28098)                                                      */
/*      SPRHBM (0x21e28098)                                                     */
/*      SPRC0 (0x21e28098)                                                      */
/*      SPRMCC (0x21e28098)                                                     */
/*      SPRUCC (0x21e4a098)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KPCIR0CR
*/


#define M3KPCIR0CR_M3KTI_MAIN_REG 0x0A020098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio0_ncb : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* IIO0 NCB Credits. */
    UINT32 iio1_ncb : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* IIO1 NCB Credits. */
    UINT32 iio2_ncb : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* IIO2 NCB Credits. */
    UINT32 iio3_ncb : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* IIO3 NCB Credits. */
    UINT32 iio4_ncb : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* IIO4 NCB Credits. */
    UINT32 iio5_ncb : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* IIO5 NCB Credits. */
    UINT32 iio6_ncb : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000001*/

                            /* IIO6 NCB Credits. */
    UINT32 ubox_ncb : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000001*/

                            /* UBox NCB Credits. */

  } Bits;
  UINT32 Data;

} M3KPCIR0CR_M3KTI_MAIN_STRUCT;

/* M3KPCIR1CR_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e2809c)                                                      */
/*      SPRB0 (0x21e2809c)                                                      */
/*      SPRHBM (0x21e2809c)                                                     */
/*      SPRC0 (0x21e2809c)                                                      */
/*      SPRMCC (0x21e2809c)                                                     */
/*      SPRUCC (0x21e4a09c)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KPCIR1CR
*/


#define M3KPCIR1CR_M3KTI_MAIN_REG 0x0A02009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio0_ncs : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* IIO0 NCS Credits */
    UINT32 iio1_ncs : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* IIO1 NCS Credits. */
    UINT32 iio2_ncs : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* IIO2 NCS Credits. */
    UINT32 iio3_ncs : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* IIO3 NCS Credits. */
    UINT32 iio4_ncs : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* IIO4 NCS Credits. */
    UINT32 iio5_ncs : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000001*/

                            /* IIO5 NCS Credits. */
    UINT32 iio6_ncs : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000001*/

                            /* IIO6 NCS Credits. */
    UINT32 ubox_ncs : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000001*/

                            /* UBox NCS Credits. */

  } Bits;
  UINT32 Data;

} M3KPCIR1CR_M3KTI_MAIN_STRUCT;

/* M3KCTRL_M3KTI_MAIN_REG supported on:                                         */
/*      SPRA0 (0x21e280a0)                                                      */
/*      SPRB0 (0x21e280a0)                                                      */
/*      SPRHBM (0x21e280a0)                                                     */
/*      SPRC0 (0x21e280a0)                                                      */
/*      SPRMCC (0x21e280a0)                                                     */
/*      SPRUCC (0x21e4a0a0)                                                     */
/* Register default value on SPRA0: 0x02002000                                  */
/* Register default value on SPRB0: 0x02002000                                  */
/* Register default value on SPRHBM: 0x02002000                                 */
/* Register default value on SPRC0: 0x02002000                                  */
/* Register default value on SPRMCC: 0x02002000                                 */
/* Register default value on SPRUCC: 0x02002000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3KCTRL
*/


#define M3KCTRL_M3KTI_MAIN_REG 0x0A0200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 socketid : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Reflects the NodeID[2:0] assgined to the socket. */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_8 : 4;

                            /* Bits[11:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d2cthreshold : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000002*/

                            /*
                               4b value indicating how many txns must be in the
                               BL Egress before a D2c message is spawned (if
                               requested from the Agent). This is needed to
                               allow BL bandwidth injection to scale down to 1
                               BL packet per message under load instead of 2.
                               Value of 9 and above means disable D2c and a
                               value of 0 is not allowed Note: this value is
                               per Cacheline (2 pumps) in BL Egress Expected
                               value programmed by BIOS is 4
                            */
    UINT32 rsvd_16 : 8;

                            /* Bits[23:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 meshcreditpush : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mesh Credit Push. When set all VNA credits from
                               one of the 2 Intel UPI ports will be pushed to
                               the adjacent M2PCI agent to reduce AK ring
                               congestions. This bit should only be set on
                               M3UPI with Intel UPI 0 and Intel UPI 1
                               connected. It should only be enabled if both
                               Intel UPI 0 and Intel UPI 1 are enabled. Note:
                               that pushing of credits always occurs from
                               physical port 1, whos definition is based on Si
                               layout and may not correspond to Intel UPI port
                               1 on the package.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KCTRL_M3KTI_MAIN_STRUCT;

/* M3SNPFANOUT_P0_M3KTI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x21e280a4)                                                      */
/*      SPRB0 (0x21e280a4)                                                      */
/*      SPRHBM (0x21e280a4)                                                     */
/*      SPRC0 (0x21e280a4)                                                      */
/*      SPRMCC (0x21e280a4)                                                     */
/*      SPRUCC (0x21e4a0a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3SNPFANOUT_P0
*/


#define M3SNPFANOUT_P0_M3KTI_MAIN_REG 0x0A0200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fanouttable : 24;

                            /* Bits[23:0], Access Type=RW, default=0x00000000*/

                            /*
                               Table is a 8-deep x 3-bit wide indicating Snoop
                               Fanout Destination. Index - Result Bits 0 - 2:0
                               1 - 5:3 2 - 8:6 3 - 11:9 4 - 14:12 5 - 17:15 6 -
                               20:18 7 - 23:21 Result bits indicate Fanout
                               Target: 0 - Port 0 1 - Port 1 2 - Port 2 It is
                               not legal to set a bit corresponding to the
                               source (same rule as Routing Table)
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3SNPFANOUT_P0_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3SNPFANOUT_P1_M3KTI_MAIN_REG supported on:                                  */
/*      SPRA0 (0x21e280a8)                                                      */
/*      SPRB0 (0x21e280a8)                                                      */
/*      SPRHBM (0x21e280a8)                                                     */
/*      SPRC0 (0x21e280a8)                                                      */
/*      SPRMCC (0x21e280a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M3SNPFANOUT_P1
*/


#define M3SNPFANOUT_P1_M3KTI_MAIN_REG 0x0A0200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fanouttable : 24;

                            /* Bits[23:0], Access Type=RW, default=0x00000000*/

                            /*
                               Table is a 8-deep x 3-bit wide indicating Snoop
                               Fanout Destination. Index - Result Bits 0 - 2:0
                               1 - 5:3 2 - 8:6 3 - 11:9 4 - 14:12 5 - 17:15 6 -
                               20:18 7 - 23:21 Result bits indicate Fanout
                               Target: 0 - Port 0 1 - Port 1 2 - Port 2 It is
                               not legal to set a bit corresponding to the
                               source (same rule as Routing Table)
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3SNPFANOUT_P1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KTXTRH_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x21e280ac)                                                      */
/*      SPRB0 (0x21e280ac)                                                      */
/*      SPRHBM (0x21e280ac)                                                     */
/*      SPRC0 (0x21e280ac)                                                      */
/*      SPRMCC (0x21e280ac)                                                     */
/* Register default value on SPRA0: 0xF0000000                                  */
/* Register default value on SPRB0: 0xF0000000                                  */
/* Register default value on SPRHBM: 0xF0000000                                 */
/* Register default value on SPRC0: 0xF0000000                                  */
/* Register default value on SPRMCC: 0xF0000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* M3KTxTRH
*/


#define M3KTXTRH_M3KTI_MAIN_REG 0x0A0200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txqcreditsreserved0duringl0 : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved0 when in L0 mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 0 (perf mode)
                            */
    UINT32 txqcreditsreserved1duringl0 : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved1 when in L0 mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 1 (Power saving mode1)
                            */
    UINT32 txqcreditsreserved2duringl0 : 7;

                            /* Bits[20:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved2 when in L0 mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 2 (Power saving mode2)
                            */
    UINT32 txqcreditsreserved3duringl0 : 7;

                            /* Bits[27:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved3 when in L0 mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 3 (Power saving mode3)
                            */
    UINT32 bgfmaxcreditcount : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x0000000F*/

                            /*
                               For normal operation, the domain crossing FIFO
                               between the Intel UPI ingress logic and link
                               layer has 16 entries. This field may be used to
                               artificially limit the size of this FIFO. Legal
                               programming values: 0-15, corresponding credit
                               values: 1-16
                            */

  } Bits;
  UINT32 Data;

} M3KTXTRH_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KTXTRH1_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e280b0)                                                      */
/*      SPRB0 (0x21e280b0)                                                      */
/*      SPRHBM (0x21e280b0)                                                     */
/*      SPRC0 (0x21e280b0)                                                      */
/*      SPRMCC (0x21e280b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* M3KTxTRH1
*/


#define M3KTXTRH1_M3KTI_MAIN_REG 0x0A0200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txqcreditsreserved0duringl0p : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved0 when in L0p mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 0 (perf mode)
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txqcreditsreserved1duringl0p : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved1 when in L0p mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 1 (Power saving mode1)
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txqcreditsreserved2duringl0p : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved2 when in L0p mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 2 (Power saving mode2)
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txqcreditsreserved3duringl0p : 7;

                            /* Bits[30:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               TxQ Credit Reserved3 when in L0p mode. The
                               Reserved 0/1/2/3 register set is selected by PCU
                               through virtual wires to PMA . PowerModeSelect
                               value 3 (Power saving mode3)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KTXTRH1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* M3GLERRCFG_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e280b4)                                                      */
/*      SPRB0 (0x21e280b4)                                                      */
/*      SPRHBM (0x21e280b4)                                                     */
/*      SPRC0 (0x21e280b4)                                                      */
/*      SPRMCC (0x21e280b4)                                                     */
/*      SPRUCC (0x21e4a0b4)                                                     */
/* Register default value on SPRA0: 0x1003E7E0                                  */
/* Register default value on SPRB0: 0x1003E7E0                                  */
/* Register default value on SPRHBM: 0x1003E7E0                                 */
/* Register default value on SPRC0: 0x1003E7E0                                  */
/* Register default value on SPRMCC: 0x1003E7E0                                 */
/* Register default value on SPRUCC: 0x1003E7E0                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3GLERRCFG
*/


#define M3GLERRCFG_M3KTI_MAIN_REG 0x0A0200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 resetglobalfatalerror : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global fatal error status. */
    UINT32 resetglobalviral : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global viral status. */
    UINT32 maskm3fatalerror : 4;

                            /* Bits[8:5], Access Type=RW, default=0x0000000F*/

                            /*
                               Set 1 to block fatal error status of M3UPI to
                               global fatal status. Bit[5]:Ingress
                               MCERR[Agent0], Bit[6]:Ingress MCERR[Agent1],
                               Bit[7]:Egress MCERR[Agent0], Bit[8]:Egress
                               MCERR[Agent1],
                            */
    UINT32 maskktifatalerrorin : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block fatal error status from Intel UPI
                               to global fatal status
                            */
    UINT32 rsvd : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maskktiviralout : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block viral status from global viral
                               status to Intel UPI
                            */
    UINT32 maskktiiviralin : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000003*/

                            /*
                               Set 1 to block viral status from Intel UPI to
                               global viral status
                            */
    UINT32 maskpcufatalerrorout : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /* Set 1 to block fatal to PCU */
    UINT32 rsvd_18 : 11;

                            /* Bits[28:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3GLERRCFG_M3KTI_MAIN_STRUCT;

/* M3KIGRVNSEL0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e280b8)                                                      */
/*      SPRB0 (0x21e280b8)                                                      */
/*      SPRHBM (0x21e280b8)                                                     */
/*      SPRC0 (0x21e280b8)                                                      */
/*      SPRMCC (0x21e280b8)                                                     */
/*      SPRUCC (0x21e4a0b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3KIGRVNSEL0
*/


#define M3KIGRVNSEL0_M3KTI_MAIN_REG 0x0A0200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 igrvnswtchtable0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This table assigns the locally generated Intel
                               UPI packets to VN0 or VN1 dependant on DNID.
                               Table is indexed based on DNID[2:0] and output
                               in VN0 or VN1. Bit - DNID[2:0] - VN 0 - 0x0 -
                               VN0 or VN1 1 - 0x1 - VN0 or VN1 ... 7 - 0x7 -
                               VN0 or VN1 Note: in past products this able also
                               assigned Route-Through traffic VN, but that
                               functionality was removed in SKX.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KIGRVNSEL0_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KIGRVNSEL1_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e280bc)                                                      */
/*      SPRB0 (0x21e280bc)                                                      */
/*      SPRHBM (0x21e280bc)                                                     */
/*      SPRC0 (0x21e280bc)                                                      */
/*      SPRMCC (0x21e280bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* M3KIGRVNSEL1
*/


#define M3KIGRVNSEL1_M3KTI_MAIN_REG 0x0A0200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 igrvnswtchtable1 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This table assigns the locally generated Intel
                               UPI packets to VN0 or VN1 dependant on DNID.
                               Table is indexed based on DNID[2:0] and output
                               in VN0 or VN1. Bit - DNID[2:0] - VN 0 - 0x0 -
                               VN0 or VN1 1 - 0x1 - VN0 or VN1 ... 7 - 0x7 -
                               VN0 or VN1 Note: in past products this able also
                               assigned Route-Through traffic VN, but that
                               functionality was removed in the processor
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KIGRVNSEL1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */



/* M3EFQPRIVC_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e280c8)                                                      */
/*      SPRB0 (0x21e280c8)                                                      */
/*      SPRHBM (0x21e280c8)                                                     */
/*      SPRC0 (0x21e280c8)                                                      */
/*      SPRMCC (0x21e280c8)                                                     */
/*      SPRUCC (0x21e4a0c8)                                                     */
/* Register default value on SPRA0: 0x00152600                                  */
/* Register default value on SPRB0: 0x00152600                                  */
/* Register default value on SPRHBM: 0x00152600                                 */
/* Register default value on SPRC0: 0x00152600                                  */
/* Register default value on SPRMCC: 0x00152600                                 */
/* Register default value on SPRUCC: 0x00161801                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3EFQPrivC
*/


#define M3EFQPRIVC_M3KTI_MAIN_REG 0x0A0200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 6;

                            /* Bits[6:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adflowqueuedepth : 7;

                            /* Bits[13:7], Access Type=RW/P/L, default=0x0000004C*/

                            /*
                               Through this field, the depth of the AD FlowQ
                               queue (shared across all message classes and
                               virtual networks) may be artifically limited for
                               DFx purposes. AD credits allocated to UPI Agent
                               must be relected to match.
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blflowqueuedepth : 6;

                            /* Bits[20:15], Access Type=RW/P/L, default=0x0000002A*/

                            /*
                               Through this field, the depth of the BL FlowQ
                               queue (shared across all message classes and
                               virtual networks) may be artifically limited for
                               DFx purposes. BL Credits allocated to from UPI
                               Agent must be adjusted to match this value.
                            */
    UINT32 rsvd_21 : 5;

                            /* Bits[25:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3EFQPRIVC_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3EFQPRIVC1_M3KTI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x21e280cc)                                                      */
/*      SPRB0 (0x21e280cc)                                                      */
/*      SPRHBM (0x21e280cc)                                                     */
/*      SPRC0 (0x21e280cc)                                                      */
/*      SPRMCC (0x21e280cc)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* M3EFQPrivC1
*/


#define M3EFQPRIVC1_M3KTI_MAIN_REG 0x0A0200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bgfmaxcreditcount : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               For normal operation, the domain crossing FIFO
                               between the UPI ingress logic and link layer has
                               12 entries. This field may be used to
                               artificially limit the size of this FIFO.
                            */
    UINT32 rsvd_4 : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3EFQPRIVC1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */


/* M3EFQCTRL_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e280d4)                                                      */
/*      SPRB0 (0x21e280d4)                                                      */
/*      SPRHBM (0x21e280d4)                                                     */
/*      SPRC0 (0x21e280d4)                                                      */
/*      SPRMCC (0x21e280d4)                                                     */
/*      SPRUCC (0x21e4a0d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3EFQCTRL
*/


#define M3EFQCTRL_M3KTI_MAIN_REG 0x0A0200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pe_mode : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* If set, we copy PE bit to and from DNID[3]. */
    UINT32 rsvd_1 : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3EFQCTRL_M3KTI_MAIN_STRUCT;



/* M3KPRECTRL_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e280e0)                                                      */
/*      SPRB0 (0x21e280e0)                                                      */
/*      SPRHBM (0x21e280e0)                                                     */
/*      SPRC0 (0x21e280e0)                                                      */
/*      SPRMCC (0x21e280e0)                                                     */
/*      SPRUCC (0x21e4a0e0)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3KPRECTRL
*/


#define M3KPRECTRL_M3KTI_MAIN_REG 0x0A0200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mini_sad_mode : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mini SAD Index Mode 0x0 - {pa[7] snc_id[0]
                               pa[6]} Used by EP, and 8-SMI3 EX, for legacy 1LM
                               0x1 - {pa[9] snc_id[0] pa[8]} Used by EP, and
                               8-SMI3 EX, for 1LM 0x2 - {pa[13] snc_id[0]
                               pa[12]} Used by EP, and 8-SMI3 EX, for 2LM 0x4 -
                               {snc_id[1] snc_id[0] pa[6]} 0x5 - {snc_id[1]
                               snc_id[0] pa[8]} 0x6 - {snc_id[1] snc_id[0]
                               pa[12]} index bits are [2:0]
                            */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remspecrdthreshold : 4;

                            /* Bits[8:5], Access Type=RW/P, default=0x00000002*/

                            /*
                               4b value indicating the AD egress credit
                               threshold that is used to determine if we can
                               spawn a remote speculative read message to
                               memory. The value indicates the number of AD
                               txns that must be in the AD egress before
                               allowing a remote speculative read spawn. Value
                               of 9 and above disables spawning prefetches and
                               a value of 0 and 1 are not allowed
                            */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disslot1pfwhenslot0pktvalid : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable the bug fix for b305779 (revert back to
                               A0 behavior)
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KPRECTRL_M3KTI_MAIN_STRUCT;

/* M3KPRETL_M3KTI_MAIN_REG supported on:                                        */
/*      SPRA0 (0x21e280e4)                                                      */
/*      SPRB0 (0x21e280e4)                                                      */
/*      SPRHBM (0x21e280e4)                                                     */
/*      SPRC0 (0x21e280e4)                                                      */
/*      SPRMCC (0x21e280e4)                                                     */
/*      SPRUCC (0x21e4a0e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3KPRETL
*/


#define M3KPRETL_M3KTI_MAIN_REG 0x0A0200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mini_sad_targets : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               8 Targets x 4-bit each Targets Selected based
                               3-bit Mini_SAD_Mode indexings. 4-bit result is
                               encoded to indicate SMI3 or iMC target: 0x0 -
                               iMC0 0x1 - iMC1 0x2-to-0x7 - SMI Port0 to SMI
                               Port5 0x8 to 0x9 - SMI Port6 to SMI Port 7 0x9
                               to 0xF - RSVD (if encoding set no prefetch is
                               generated) Note: There is no Plan to use mixed
                               SMI3 and iMC at the same time in EP. Index to
                               bit map: 0 - bits[3:0] 1 - bit[7:4] etc.
                            */

  } Bits;
  UINT32 Data;

} M3KPRETL_M3KTI_MAIN_STRUCT;

/* M3KUBOXCTL_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e280e8)                                                      */
/*      SPRB0 (0x21e280e8)                                                      */
/*      SPRHBM (0x21e280e8)                                                     */
/*      SPRC0 (0x21e280e8)                                                      */
/*      SPRMCC (0x21e280e8)                                                     */
/*      SPRUCC (0x21e4a0e8)                                                     */
/* Register default value on SPRA0: 0x00FF8FF8                                  */
/* Register default value on SPRB0: 0x00FF8FF8                                  */
/* Register default value on SPRHBM: 0x00FF8FF8                                 */
/* Register default value on SPRC0: 0x00FF8FF8                                  */
/* Register default value on SPRMCC: 0x00FF8FF8                                 */
/* Register default value on SPRUCC: 0x00FF8FF8                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3KPRETL
*/


#define M3KUBOXCTL_M3KTI_MAIN_REG 0x0A0200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtidmatchforuboxp2p : 12;

                            /* Bits[11:0], Access Type=RW/P, default=0x00000FF8*/

                            /* Match register for UBOX RTID for P2P. */
    UINT32 rtidmaskforuboxp2p : 12;

                            /* Bits[23:12], Access Type=RW/P, default=0x00000FF8*/

                            /* Mask register for UBOX RTID for P2P. */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KUBOXCTL_M3KTI_MAIN_STRUCT;

/* M3KLL_TX_CRD_P0_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e280f0)                                                      */
/*      SPRB0 (0x21e280f0)                                                      */
/*      SPRHBM (0x21e280f0)                                                     */
/*      SPRC0 (0x21e280f0)                                                      */
/*      SPRMCC (0x21e280f0)                                                     */
/*      SPRUCC (0x21e4a0f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KLL_TX_CRD_P0
*/


#define M3KLL_TX_CRD_P0_M3KTI_MAIN_REG 0x0A0200F0

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_crd : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* VNA Credits Available for use on Tx */
    UINT32 vn0_req_crd : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Request Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_snp_crd : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Snoop Credits Available for use on Tx 0 - 0
                               Credits 1 - 1+ Credits
                            */
    UINT32 vn0_rsp_crd : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Response Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_wb_crd : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Write Back Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_ncb_crd : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_ncs_crd : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Non-Coherent Standard Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_req_crd : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Request Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_snp_crd : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Snoop Credits Available for use on Tx 0 - 0
                               Credits 1 - 1+ Credits
                            */
    UINT32 vn1_rsp_crd : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Response Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_wb_crd : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Write Back Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_ncb_crd : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_ncs_crd : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Non-Coherent Standard Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KLL_TX_CRD_P0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_vna_crd : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* BL VNA Credits Available for use on Tx */
    UINT32 ad_vn0_req_crd : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 Request Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vn0_snp_crd : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 Snoop Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn0_rsp_crd : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 Response Credits Available for use on Tx
                               0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn0_wb_crd : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 Write Back Credits Available for use on
                               Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn0_ncb_crd : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn0_ncs_crd : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 Non-Coherent Standard Credits Available
                               for use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vn1_req_crd : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 Request Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vn1_snp_crd : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 Snoop Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn1_rsp_crd : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 Response Credits Available for use on Tx
                               0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn1_wb_crd : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 Write Back Credits Available for use on
                               Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn1_ncb_crd : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 bl_vn1_ncs_crd : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 Non-Coherent Standard Credits Available
                               for use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vn0_rsp_crd : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 Response Credits Available for use on Tx
                               0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vn1_rsp_crd : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 Response Credits Available for use on Tx
                               0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 ad_vna_crd : 8;

                            /* Bits[29:22], Access Type=RO/V, default=0x00000000*/

                            /* AD VNA Credits */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KLL_TX_CRD_P0_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KLL_TX_CRD_P1_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e280f4)                                                      */
/*      SPRB0 (0x21e280f4)                                                      */
/*      SPRHBM (0x21e280f4)                                                     */
/*      SPRC0 (0x21e280f4)                                                      */
/*      SPRMCC (0x21e280f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3KLL_TX_CRD_P1
*/


#define M3KLL_TX_CRD_P1_M3KTI_MAIN_REG 0x0A0200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_crd : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* VNA Credits Available for use on Tx */
    UINT32 vn0_req_crd : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Request Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_snp_crd : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Snoop Credits Available for use on Tx 0 - 0
                               Credits 1 - 1+ Credits
                            */
    UINT32 vn0_rsp_crd : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Response Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_wb_crd : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Write Back Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_ncb_crd : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn0_ncs_crd : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN0 Non-Coherent Standard Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_req_crd : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Request Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_snp_crd : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Snoop Credits Available for use on Tx 0 - 0
                               Credits 1 - 1+ Credits
                            */
    UINT32 vn1_rsp_crd : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Response Credits Available for use on Tx 0 -
                               0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_wb_crd : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Write Back Credits Available for use on Tx 0
                               - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_ncb_crd : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Non-Coherent Bypass Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 vn1_ncs_crd : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               VN1 Non-Coherent Standard Credits Available for
                               use on Tx 0 - 0 Credits 1 - 1+ Credits
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KLL_TX_CRD_P1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#ifdef SPRUCC_HOST
/* M3KLL_TX_CRD_P0_1_M3KTI_MAIN_REG supported on:                               */
/*      SPRUCC (0x21e4a0f4)                                                     */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRUCC BDF: 30_9_2                                     */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KLL_TX_CRD_P0_1
*/


#define M3KLL_TX_CRD_P0_1_M3KTI_MAIN_REG 0x0A0200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak_vna_crd : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* AK VNA Credits Available for use on Tx */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KLL_TX_CRD_P0_1_M3KTI_MAIN_STRUCT;
#endif /* (SPRUCC_HOST) */

/* PXPENHCAP_0_M3KTI_MAIN_REG supported on:                                     */
/*      SPRA0 (0x21e28100)                                                      */
/*      SPRB0 (0x21e28100)                                                      */
/*      SPRHBM (0x21e28100)                                                     */
/*      SPRC0 (0x21e28100)                                                      */
/*      SPRMCC (0x21e28100)                                                     */
/*      SPRUCC (0x21e4a100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_0_M3KTI_MAIN_REG 0x0A020100

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

} PXPENHCAP_0_M3KTI_MAIN_STRUCT;




/* M3EGRERRMSK0123_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28118)                                                      */
/*      SPRB0 (0x21e28118)                                                      */
/*      SPRHBM (0x21e28118)                                                     */
/*      SPRC0 (0x21e28118)                                                      */
/*      SPRMCC (0x21e28118)                                                     */
/*      SPRUCC (0x21e4a118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3EGRERRMSK0123
*/


#define M3EGRERRMSK0123_M3KTI_MAIN_REG 0x0A020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 akflowqunderflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* AK flow Underflow mask */
    UINT32 akflowqoverflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* AK flow Overflow mask */
    UINT32 blflowqunderflowmask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL flow underflow mask */
    UINT32 blflowqoverflowmask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL flow overflow mask */
    UINT32 adflowqunderflowmask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* AD flow underflow mask */
    UINT32 adflowqoverflowmask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* AD flow overflow mask */
    UINT32 blm2pncscreditunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL NCS credit underflow mask */
    UINT32 blm2pncbcreditunderflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ncb credit underflow mask */
    UINT32 blm2pncscreditoverflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL ncs credit overflow mask */
    UINT32 blm2pncbcreditoverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL ncb credit overflow mask */
    UINT32 blrtvn1port1creditunderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL VN1 Port 1 credit underflow mask */
    UINT32 blrtvn1port1creditoverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL VN1 Port 1 credit overflow mask */
    UINT32 blrtvn1port20creditunderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL VN1 Port20 Credit underflow mask */
    UINT32 blrtvn1port20creditoverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL VN1 Port20 credit overflow mask */
    UINT32 blrtvn0port1creditunderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL Vn0 port1 credit underflow mask */
    UINT32 blrtvn0port1creditoverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL VN0 Port 1 credit overflow mask */
    UINT32 blrtvn0port20creditunderflowmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* BL VN0 Port20 Credit underflow mask */
    UINT32 blrtvn0port20creditoverflowmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* BL VN0 Port20 credit overflow mask */
    UINT32 adrtvn1port1creditunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* AD VN1 Port 1 credit underflow mask */
    UINT32 adrtvn1port1creditoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* AD VN1 Port 1 credit overflow mask */
    UINT32 adrtvn1port20creditunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD VN1 Port20 Credit underflow mask */
    UINT32 adrtvn1port20creditoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD VN1 Port20 credit overflow mask */
    UINT32 adrtvn0port1creditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* AD Vn0 port1 credit underflow mask */
    UINT32 adrtvn0port1creditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* AD VN0 Port 1 credit overflow mask */
    UINT32 adrtvn0port20creditunderflowmask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* AD VN0 Port20 Credit underflow mask */
    UINT32 adrtvn0port20creditoverflowmask : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* AD VN0 Port20 credit overflow mask */
    UINT32 d2kcmpfifounderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* FIFO underflow mask */
    UINT32 d2kcmpfifooverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* FIFO overflow mask */
    UINT32 cmscreditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* CMS credit underflow mask */
    UINT32 cmscreditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* CMS credit overflow mask */
    UINT32 adchacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* AD CHA credit underflow mask */
    UINT32 adchacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* AD CHA credit overflow mask */

  } Bits;
  UINT32 Data;

} M3EGRERRMSK0123_M3KTI_MAIN_STRUCT;

/* M3EGRERRMSK4567_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e2811c)                                                      */
/*      SPRB0 (0x21e2811c)                                                      */
/*      SPRHBM (0x21e2811c)                                                     */
/*      SPRC0 (0x21e2811c)                                                      */
/*      SPRMCC (0x21e2811c)                                                     */
/*      SPRUCC (0x21e4a11c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3EGRERRMSK4567
*/


#define M3EGRERRMSK4567_M3KTI_MAIN_REG 0x0A02011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 akflowqunderflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* AK flow underflow mask */
    UINT32 akflowqoverflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* AK flow overflow mask */
    UINT32 blflowqunderflowmask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL underflow mask */
    UINT32 blflowqoverflowmask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL flow overflow mask */
    UINT32 adflowqunderflowmask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* AD flow underflow mask */
    UINT32 adflowqoverflowmask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* AD flow overflow mask */
    UINT32 blm2pncscreditunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL ncs credit underflow mask */
    UINT32 blm2pncbcreditunderflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ncb credit underflow mask */
    UINT32 blm2pncscreditoverflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL ncs credit overflow mask */
    UINT32 blm2pncbcreditoverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL ncb credit overflow mask */
    UINT32 blrtvn1port0creditunderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL vn1 port0 credit underflowmask */
    UINT32 blrtvn1port0creditoverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL vn1 port0 credit overflow mask */
    UINT32 blrtvn1port2creditunderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL vn1 port2 credit underflow mask */
    UINT32 blrtvn1port2creditoverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL vn1 port2 credit overflow mask */
    UINT32 blrtvn0port0creditunderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL vn0 port0 credit underflow mask */
    UINT32 blrtvn0port0creditoverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL vn0 port0 credit overflow mask */
    UINT32 blrtvn0port2creditunderflowmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* BL vn0 port2 credit underflow mask */
    UINT32 blrtvn0port2creditoverflowmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* BL vn0 port2 credit overflow mask */
    UINT32 adrtvn1port0creditunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* AD vn1 port0 credit underflow mask */
    UINT32 adrtvn1port0creditoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* AD vn1 port0 credit overflow mask */
    UINT32 adrtvn1port2creditunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD vn1 port2 credit underflow mask */
    UINT32 adrtvn1port2creditoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD vn1 port2 credit overflow mask */
    UINT32 adrtvn0port0creditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* AD vn0 port0 credit underflow mask */
    UINT32 adrtvn0port0creditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* AD vn0 port0credit overflow mask */
    UINT32 adrtvn0port2creditunderflowmask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* AD port2 credit underflow mask */
    UINT32 adrtvn0port2creditoverflowmask : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* AD port2 credit overflow mask */
    UINT32 d2kcmpfifounderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Cmp FIFO underflow mask */
    UINT32 d2kcmpfifooverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* CMP fifo overflow mask */
    UINT32 cmscreditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* CMS credut inderflow mask */
    UINT32 cmscreditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* CMS credit overflow mask */
    UINT32 adchacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* AD CHA credit underflow mask */
    UINT32 adchacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* AD CHA credit overflow mask */

  } Bits;
  UINT32 Data;

} M3EGRERRMSK4567_M3KTI_MAIN_STRUCT;

/* M3EGRERRLOG0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28120)                                                      */
/*      SPRB0 (0x21e28120)                                                      */
/*      SPRHBM (0x21e28120)                                                     */
/*      SPRC0 (0x21e28120)                                                      */
/*      SPRMCC (0x21e28120)                                                     */
/*      SPRUCC (0x21e4a120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG0
*/


#define M3EGRERRLOG0_M3KTI_MAIN_REG 0x0A020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 d2kcmpfifounderflow : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp FIFO underflow mask */
    UINT32 d2kcmpfifooverflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp FIFO overflow mask */
    UINT32 akcmscreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK CMS credit underflow */
    UINT32 blcmscreditunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL CMS credit underflow */
    UINT32 adcmscreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD cms credit underflow */
    UINT32 akcmscreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK CMS credit overflow */
    UINT32 blcmscreditoverflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL CMS credit overflow */
    UINT32 adcmscreditoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD cms credit overflow */
    UINT32 adchasliceidcreditunderflow : 5;

                            /* Bits[12:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Slice ID credit underflow */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adchawbcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Wb credit underflow */
    UINT32 adchasnpcreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA snoop credit underflow */
    UINT32 adchareqcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA request credit underflow */
    UINT32 adchashrcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Shr credit underflow */
    UINT32 adchasliceidcreditoverflow : 5;

                            /* Bits[24:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Slice ID credit overflow */
    UINT32 rsvd_25 : 3;

                            /* Bits[27:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adchawbcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Wb credit overflow */
    UINT32 adchasnpcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Snoop credit overflow */
    UINT32 adchareqcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA request credit overflow */
    UINT32 adchashrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG0_M3KTI_MAIN_STRUCT;

/* M3EGRERRLOG1_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28124)                                                      */
/*      SPRB0 (0x21e28124)                                                      */
/*      SPRHBM (0x21e28124)                                                     */
/*      SPRC0 (0x21e28124)                                                      */
/*      SPRMCC (0x21e28124)                                                     */
/*      SPRUCC (0x21e4a124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG1
*/


#define M3EGRERRLOG1_M3KTI_MAIN_REG 0x0A020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti0parerrhdrrfing : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity Error RF ingress */
    UINT32 kti0parerre2ebeing : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error BE ingress */
    UINT32 kti0parerre2edataing : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error data ingress */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adrtvn1port1rspcreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD VN1 Port1 Rsp credit underflow */
    UINT32 adrtvn1port1snpcreditunderflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD VN1 Port1 Snoop Credit underflow */
    UINT32 adrtvn1port1reqcreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn1 Port1 Request credit underflow */
    UINT32 adrtvn1port20rspcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn1 port 20 rsp credit underflow */
    UINT32 adrtvn1port20snpcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn1 port20 snoop credit underflow */
    UINT32 adrtvn1port20reqcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port20 request credit underflow */
    UINT32 adrtvn1port1rspcreditoverflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port1 rsp credit overflow */
    UINT32 adrtvn1port1snpcreditoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port1 snoop credit overflow */
    UINT32 adrtvn1port1reqcreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port1 request cresit overflow */
    UINT32 adrtvn1port20rspcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port20 Rsp credit overflow */
    UINT32 adrtvn1port20snpcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port20 snoop credit overflow */
    UINT32 adrtvn1port20reqcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port20 request credit overflow */
    UINT32 adrtvn0port1rspcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port1 rsp credit underflow */
    UINT32 adrtvn0port1snpcreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port1 snoop credit underflow */
    UINT32 adrtvn0port1reqcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port1 request credit underflow */
    UINT32 adrtport1shrcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port1 shr credit underflow */
    UINT32 adrtvn0port20rspcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD VN0 port20 rsp credit underflow */
    UINT32 adrtvn0port20snpcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port20 snoop credit underflow */
    UINT32 adrtvn0port20reqcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port20 request credit underflow */
    UINT32 adrtport20shrcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port20 shr credit underflow */
    UINT32 adrtvn0port1rspcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD VN0 port1 rsp credit overflow */
    UINT32 adrtvn0port1snpcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn0 port1 shr credit overflow */
    UINT32 adrtvn0port1reqcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn0 port1 request credit overflow */
    UINT32 adrtport1shrcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port1 shr credit overflow */
    UINT32 adrtvn0port20rspcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port20 rsp credit overflow */
    UINT32 adrtvn0port20snpcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port20 snoop credit overflow */
    UINT32 adrtvn0port20reqcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port20 request credit overflow */
    UINT32 adrtport20shrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port 20 shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG1_M3KTI_MAIN_STRUCT;

/* M3EGRERRLOG2_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28128)                                                      */
/*      SPRB0 (0x21e28128)                                                      */
/*      SPRHBM (0x21e28128)                                                     */
/*      SPRC0 (0x21e28128)                                                      */
/*      SPRMCC (0x21e28128)                                                     */
/*      SPRUCC (0x21e4a128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG2
*/


#define M3EGRERRLOG2_M3KTI_MAIN_REG 0x0A020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rbtfifounderflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* FIFO underflow mask */
    UINT32 rbtfifooverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* FIFO overflow mask */
    UINT32 blrtvn1port20rspcreditunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port20 rsp credit underflow */
    UINT32 blrtvn1port1ncscreditoverflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port1 ncs credit overflow */
    UINT32 blrtvn1port1ncbcreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port1 ncb credit overflow */
    UINT32 blrtvn1port1wbcreditoverflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port1 Wb credit overflow */
    UINT32 blrtvn1port1rspcreditoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port1 Rsp credit overflow */
    UINT32 blrtvn1port20ncscreditoverflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port20 NCS credit overflow */
    UINT32 blrtvn1port20ncbcreditoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port20 NCB credit overflow */
    UINT32 blrtvn1port20wbcreditoverflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL VN1 port20 wb credit overflow */
    UINT32 blrtvn1port20rspcreditoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL VN1 port20 rsp credit overflow */
    UINT32 blrtvn0port1ncscreditunderflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL VN0 port1 NCS credit underflow */
    UINT32 blrtvn0port1ncbcreditunderflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn0 port1 ncb credit underflow */
    UINT32 blrtvn0port1wbcreditunderflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port1 wb credit underflow */
    UINT32 blrtvn0port1rspcreditunderflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port1 rsp credit underflow */
    UINT32 blrtport1shrcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port1 shr credit underflow */
    UINT32 blrtvn0port20ncscreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port20 ncs credit underflow */
    UINT32 blrtvn0port20ncbcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port20 ncb credit underflow */
    UINT32 blrtvn0port20wbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port20 wb credit underflow */
    UINT32 blrtvn0port20rspcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL VN0 port20 Rsp credit underflow */
    UINT32 blrtport20shrcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port20 shr credit underflow */
    UINT32 blrtvn0port1ncscreditoverflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn0 port1 NCS credit overflow */
    UINT32 blrtvn0port1ncbcreditoverflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn0 port1 NCB credit overflow */
    UINT32 blrtvn0port1wbcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port1 wb credit overflow */
    UINT32 blrtvn0port1rspcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port1 rsp credit overflow */
    UINT32 blrtport1shrcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port1 shr credit overflow */
    UINT32 blrtvn0port20ncscreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn0 port20 NCs credit overflow */
    UINT32 blrtvn0port20ncbcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn0 port20 NCb credit overflow */
    UINT32 blrtvn0port20wbcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port20 wb credit overflow */
    UINT32 blrtvn0port20rspcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port20 Rsp credit overflow */
    UINT32 blrtport20shrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port20 shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG2_M3KTI_MAIN_STRUCT;

/* M3EGRERRLOG3_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e2812c)                                                      */
/*      SPRB0 (0x21e2812c)                                                      */
/*      SPRHBM (0x21e2812c)                                                     */
/*      SPRC0 (0x21e2812c)                                                      */
/*      SPRMCC (0x21e2812c)                                                     */
/*      SPRUCC (0x21e4a12c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG3
*/


#define M3EGRERRLOG3_M3KTI_MAIN_REG 0x0A02012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bgfcreditunderflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BGF credit underflow */
    UINT32 bgfcreditoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bgf credit overflow */
    UINT32 akflowqunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK flow underflow */
    UINT32 akflowqoverflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK flow overflow */
    UINT32 blflowqmcunderflow : 3;

                            /* Bits[7:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL flow MC underflow */
    UINT32 blflowqunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL flow underflow mask */
    UINT32 blflowqoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL flow overflow mask */
    UINT32 adflowqmcunderflow : 3;

                            /* Bits[12:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD flow MC underflow */
    UINT32 adflowqunderflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD flow underflow */
    UINT32 adflowqoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD flow overflow */
    UINT32 blm2pidcreditunderflow : 3;

                            /* Bits[17:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ID credit underflow */
    UINT32 blm2pidcreditoverflow : 3;

                            /* Bits[20:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ID credit overflow */
    UINT32 blm2pncscreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL NCS credit underflow mask */
    UINT32 blm2pncbcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncb credit overflow */
    UINT32 blm2pncscreditoverflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncs credit overflow */
    UINT32 blm2pncbcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncb credit overflow */
    UINT32 blrtvn1port1ncscreditunderflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port1 NCS credit underflow */
    UINT32 blrtvn1port1ncbcreditunderflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port1 ncb credit underflow */
    UINT32 blrtvn1port1wbcreditunderflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port1 wb credit underflow */
    UINT32 blrtvn1port1rspcreditunderflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port1 rsp credit underflowe */
    UINT32 blrtvn1port20ncscreditunderflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Vn1 port20 NCS credit underflow */
    UINT32 blrtvn1port20ncbcreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port20 ncb credit underflow */
    UINT32 blrtvn1port20wbcreditunderflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port20 wb credit underflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG3_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3EGRERRLOG4_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28130)                                                      */
/*      SPRB0 (0x21e28130)                                                      */
/*      SPRHBM (0x21e28130)                                                     */
/*      SPRC0 (0x21e28130)                                                      */
/*      SPRMCC (0x21e28130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG4
*/


#define M3EGRERRLOG4_M3KTI_MAIN_REG 0x0A020130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 d2kcmpfifounderflow : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp Fifo underflow */
    UINT32 d2kcmpfifooverflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp Fifo overflow */
    UINT32 akcmscreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK CMS credit underflow */
    UINT32 blcmscreditunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL CMS credit underflow */
    UINT32 adcmscreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CMS credit underflow */
    UINT32 akcmscreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK CMS credit overflow */
    UINT32 blcmscreditoverflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL CMS credit overflow */
    UINT32 adcmscreditoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CMS credit overflow */
    UINT32 adchasliceidcreditunderflow : 5;

                            /* Bits[12:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA slice id credit underflow */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adchawbcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Wb credit underflow */
    UINT32 adchasnpcreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA snoop credit underflow */
    UINT32 adchareqcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA req credit underflow */
    UINT32 adchashrcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Shr credit underflow */
    UINT32 adchasliceidcreditoverflow : 5;

                            /* Bits[24:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Slice ID credit overflow */
    UINT32 rsvd_25 : 3;

                            /* Bits[27:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adchawbcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA wb credit overflow */
    UINT32 adchasnpcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA Snoop credit overflow */
    UINT32 adchareqcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA req credit underflow */
    UINT32 adchashrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD CHA shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG4_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3EGRERRLOG5_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28134)                                                      */
/*      SPRB0 (0x21e28134)                                                      */
/*      SPRHBM (0x21e28134)                                                     */
/*      SPRC0 (0x21e28134)                                                      */
/*      SPRMCC (0x21e28134)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG5
*/


#define M3EGRERRLOG5_M3KTI_MAIN_REG 0x0A020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti1parerrhdrrfing : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity Error RF ingress */
    UINT32 kti1parerre2ebeing : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error BE ingress */
    UINT32 kti1parerre2edataing : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error data ingress */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 adrtvn1port0rspcreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port0 rsp credit underflow */
    UINT32 adrtvn1port0snpcreditunderflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port0 snoop credit underflow */
    UINT32 adrtvn1port0reqcreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port0 request credit underflow */
    UINT32 adrtvn1port2rspcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port2 rsp credit underflow */
    UINT32 adrtvn1port2snpcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port2 snp credit underflow */
    UINT32 adrtvn1port2reqcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn1 port2 request credit underflow */
    UINT32 adrtvn1port0rspcreditoverflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Vn1 port0 rsp credit overflow */
    UINT32 adrtvn1port0snpcreditoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn1 port0 snoop credit overflow */
    UINT32 adrtvn1port0reqcreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port0 request credit overflow */
    UINT32 adrtvn1port2rspcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn1 port2 rsp credit overflow */
    UINT32 adrtvn1port2snpcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn1 port2 snoop credit overflow */
    UINT32 adrtvn1port2reqcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn1 port2 req credit overflow */
    UINT32 adrtvn0port0rspcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port0 rsp credit underflow */
    UINT32 adrtvn0port0snpcreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port0 snoop credit underflow */
    UINT32 adrtvn0port0reqcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port0 request credit underflow */
    UINT32 adrtport0shrcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad port0 shr credit underflow */
    UINT32 adrtvn0port2rspcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad port2 rsp credit underflow */
    UINT32 adrtvn0port2snpcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port2 snoop credit underflow */
    UINT32 adrtvn0port2reqcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port2 request credit underflow */
    UINT32 adrtport2shrcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port2 shr credit underflow */
    UINT32 adrtvn0port0rspcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port0 rsp credit overflow */
    UINT32 adrtvn0port0snpcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port0 snoop credit overflow */
    UINT32 adrtvn0port0reqcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port0 request credit overflow */
    UINT32 adrtport0shrcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port0 shr credit overflow */
    UINT32 adrtvn0port2rspcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port2 rsp credit overflow */
    UINT32 adrtvn0port2snpcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ad vn0 port2 snoop credit overflow */
    UINT32 adrtvn0port2reqcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD vn0 port2 request credit overflow */
    UINT32 adrtport2shrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD port2 shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG5_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3EGRERRLOG6_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28138)                                                      */
/*      SPRB0 (0x21e28138)                                                      */
/*      SPRHBM (0x21e28138)                                                     */
/*      SPRC0 (0x21e28138)                                                      */
/*      SPRMCC (0x21e28138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG6
*/


#define M3EGRERRLOG6_M3KTI_MAIN_REG 0x0A020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rbtfifounderflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* FIFO underflow */
    UINT32 rbtfifooverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* FIFO overflow */
    UINT32 blrtvn1port2rspcreditunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 rsp credit underflow */
    UINT32 blrtvn1port0ncscreditoverflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 ncs credit overflow */
    UINT32 blrtvn1port0ncbcreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 ncb credit overflow */
    UINT32 blrtvn1port0wbcreditoverflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 wb credit overflow */
    UINT32 blrtvn1port0rspcreditoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 rsp credit overflow */
    UINT32 blrtvn1port2ncscreditoverflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 ncs credit overflow */
    UINT32 blrtvn1port2ncbcreditoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 ncb credit overflow */
    UINT32 blrtvn1port2wbcreditoverflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 wb credit overflow */
    UINT32 blrtvn1port2rspcreditoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 rsp credit overflow */
    UINT32 blrtvn0port0ncscreditunderflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 ncs credit underflow */
    UINT32 blrtvn0port0ncbcreditunderflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl vn0 port0 ncb credit underflow */
    UINT32 blrtvn0port0wbcreditunderflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 wb credit underflow */
    UINT32 blrtvn0port0rspcreditunderflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 rsp credit underflow */
    UINT32 blrtport0shrcreditunderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port0 shr credit underflow */
    UINT32 blrtvn0port2ncscreditunderflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 ncs credit underflow */
    UINT32 blrtvn0port2ncbcreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 ncb credit underflow */
    UINT32 blrtvn0port2wbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 wb credit underflow */
    UINT32 blrtvn0port2rspcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 rsp credit underflow */
    UINT32 blrtport2shrcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port2 shr credit underflow */
    UINT32 blrtvn0port0ncscreditoverflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 ncs credit overflow */
    UINT32 blrtvn0port0ncbcreditoverflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 ncbcredit overflow */
    UINT32 blrtvn0port0wbcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 wb credit overflow */
    UINT32 blrtvn0port0rspcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port0 rsp credit overflow */
    UINT32 blrtport0shrcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port0 shr credit overflow */
    UINT32 blrtvn0port2ncscreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 ncs credit overflow */
    UINT32 blrtvn0port2ncbcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 port2 ncb credit overflow */
    UINT32 blrtvn0port2wbcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 wb credit overflow */
    UINT32 blrtvn0port2rspcreditoverflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn0 rsp credit overflow */
    UINT32 blrtport2shrcreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL port2 shr credit overflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG6_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3EGRERRLOG7_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e2813c)                                                      */
/*      SPRB0 (0x21e2813c)                                                      */
/*      SPRHBM (0x21e2813c)                                                     */
/*      SPRC0 (0x21e2813c)                                                      */
/*      SPRMCC (0x21e2813c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3EGRERRLOG7
*/


#define M3EGRERRLOG7_M3KTI_MAIN_REG 0x0A02013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bgfcreditunderflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BGF credit underflow */
    UINT32 bgfcreditoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BGF credit overflow */
    UINT32 akflowqunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK underflow */
    UINT32 akflowqoverflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK overflow */
    UINT32 blflowqmcunderflow : 3;

                            /* Bits[7:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL MC underflow */
    UINT32 blflowqunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL underflow */
    UINT32 blflowqoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL overflow */
    UINT32 adflowqmcunderflow : 3;

                            /* Bits[12:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD MC underflow */
    UINT32 adflowqunderflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD underflow */
    UINT32 adflowqoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD overflow */
    UINT32 blm2pidcreditunderflow : 3;

                            /* Bits[17:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ID credit underflow */
    UINT32 blm2pidcreditoverflow : 3;

                            /* Bits[20:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Id credit underflow */
    UINT32 blm2pncscreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncs credit underflow */
    UINT32 blm2pncbcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncb credit underflow */
    UINT32 blm2pncscreditoverflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncs credit overflow */
    UINT32 blm2pncbcreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ncb credit overflow */
    UINT32 blrtvn1port0ncscreditunderflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 ncs credit underflow */
    UINT32 blrtvn1port0ncbcreditunderflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 ncb credit underflow */
    UINT32 blrtvn1port0wbcreditunderflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl vn1 port0 wb credit underflow */
    UINT32 blrtvn1port0rspcreditunderflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port0 rsp credit underflow */
    UINT32 blrtvn1port2ncscreditunderflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 ncs credit underflow */
    UINT32 blrtvn1port2ncbcreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 ncb credit underflow */
    UINT32 blrtvn1port2wbcreditunderflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL vn1 port2 wb credit underflow */

  } Bits;
  UINT32 Data;

} M3EGRERRLOG7_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */


/* M3KINGCTL_M3KTI_MAIN_REG supported on:                                       */
/*      SPRA0 (0x21e28144)                                                      */
/*      SPRB0 (0x21e28144)                                                      */
/*      SPRHBM (0x21e28144)                                                     */
/*      SPRC0 (0x21e28144)                                                      */
/*      SPRMCC (0x21e28144)                                                     */
/*      SPRUCC (0x21e4a144)                                                     */
/* Register default value on SPRA0: 0x10260844                                  */
/* Register default value on SPRB0: 0x10260844                                  */
/* Register default value on SPRHBM: 0x10260844                                 */
/* Register default value on SPRC0: 0x10260844                                  */
/* Register default value on SPRMCC: 0x10260844                                 */
/* Register default value on SPRUCC: 0x00000844                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KINGCTL
*/


#define M3KINGCTL_M3KTI_MAIN_REG 0x0A020144

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 adbypassexitthrqueued : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x1, then
                               idle bypass mode is exited (or never started)
                               when N (or more) flits are queued for transfer
                               to the link layer, where N is the value of this
                               field.
                            */
    UINT32 adbypassexitthrprior : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x2, then
                               idle bypass mode is exited after N consecutive
                               bypasses, where N is the value of this field. (A
                               single cycle without a bypass resets the
                               consecutive bypass counter.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 validtxcyclemode : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               These bits control how the stall injector
                               considers the clock ratio between the UPI
                               ingress and the UPI link layer logic, that is
                               when cycles may be considered valid or open for
                               transmission of flits from UPI ingress to UPI
                               link layer. Under typical conditions, the UPI
                               ingress logic runs at a faster clock frequency
                               than the link layer. Different settings of this
                               field (and its companions) will make different
                               tradeoffs between message latency and flit
                               packing efficiency, especially for the first few
                               flits after the pipeline transitions from idle
                               to busy state. The FIFO at the clock domain
                               crossing between ingress and link layer will not
                               overflow regardless of this setting; eventually
                               traffic will always be throttled to prevent such
                               overflow. This mode selector controls how
                               traffic flow is modulated beyond simply avoiding
                               FIFO overflows. 0x0 = flit generation is limited
                               only by the number of FIFO entries, so flits
                               will be generated as quickly as possible until
                               the FIFO between the UPI ingress and UPI link
                               layer is completely filled; with sustained
                               traffic, the FIFO will remain at or near its
                               full condition 0x1 = at all times, traffic from
                               UPI ingress to UPI link layer is modulated by a
                               clock ratio-based rate matching signal; with
                               sustained traffic, one or two flits may be in
                               the FIFO at any given time; idle latency may not
                               be optimal 0x2 = flits may be sent to the domain
                               crossing FIFO in a burst until the number of
                               flits in the FIFO reaches a threshold and while
                               at that threshold, traffic is modulated by the
                               ratio-based rate matching signal; idle latency
                               should be minimal 0x3 = flits will be sent to
                               the domain crossing FIFO in a burst until a
                               threshold of consecutive flits is reached and
                               after reaching that threshold, traffic is
                               modulated by the ratio-based rate matching
                               signal; a single idle cycle will reset the
                               consecutive flit count; idle latency should be
                               minimal
                            */
    UINT32 idlebypassexitmodeselector : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field controls when AD messages may no
                               longer use the idle bypass path and start
                               traveling through the standard pipeline instead.
                               0x0 = bypass will only stop if primary resources
                               (e.g. a credit pool) have run out 0x1 = bypass
                               will stop (or never start) depending on the
                               number of flits currently queued for transfer
                               from ingress to the link layer 0x2 = bypass will
                               stop depending on the number of bypasses
                               performed immediately prior to the current
                               message
                            */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flitsinbgfifothresholdunnnh : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000006*/

                            /*
                               If the valid transmit cycle mode is set to 0x2,
                               then flits may be sent from UPI ingress to UPI
                               link layer in a burst as long as the number of
                               flits queued in the ingress-to-link domain
                               crossing FIFO does not exceed the value of this
                               field.
                            */
    UINT32 priorflittxthreshold : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000002*/

                            /*
                               If the valid transmit cycle mode is set to 0x3,
                               then flits may be sent from UPI ingress to UPI
                               link layer in a burst until N flits have been
                               transmitted consecutively, where N is the value
                               of this field. (A single idle cycle resets the
                               consecutive flit counter.)
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 slot2blocktimer : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000010*/

                            /*
                               Timer value for delaying slot2 blocking from UPI
                               Agent request. When UPI Agent will requests slot
                               2 for LLCRD, this value delays immediate
                               blocking of slot 2 scheduling for x clock cycles
                               (Uclk). The reason for delaying is to reduce
                               frequency of forced LLCRD messages that can
                               reduce performance, and to prevent back-to-back
                               slot2 blocking for LLCRD.
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KINGCTL_M3KTI_MAIN_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 adbypassexitthrqueued : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x1, then
                               idle bypass mode is exited (or never started)
                               when N (or more) flits are queued for transfer
                               to the link layer, where N is the value of this
                               field.
                            */
    UINT32 adbypassexitthrprior : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x2, then
                               idle bypass mode is exited after N consecutive
                               bypasses, where N is the value of this field. (A
                               single cycle without a bypass resets the
                               consecutive bypass counter.)
                            */
    UINT32 rsvd : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 validtxcyclemode : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               These bits control how the stall injector
                               considers the clock ratio between the UPI
                               ingress and the UPI link layer logic, that is
                               when cycles may be considered valid or open for
                               transmission of flits from UPI ingress to UPI
                               link layer. Under typical conditions, the UPI
                               ingress logic runs at a faster clock frequency
                               than the link layer. Different settings of this
                               field (and its companions) will make different
                               tradeoffs between message latency and flit
                               packing efficiency, especially for the first few
                               flits after the pipeline transitions from idle
                               to busy state. The FIFO at the clock domain
                               crossing between ingress and link layer will not
                               overflow regardless of this setting; eventually
                               traffic will always be throttled to prevent such
                               overflow. This mode selector controls how
                               traffic flow is modulated beyond simply avoiding
                               FIFO overflows. 0x0 = flit generation is limited
                               only by the number of FIFO entries, so flits
                               will be generated as quickly as possible until
                               the FIFO between the UPI ingress and UPI link
                               layer is completely filled; with sustained
                               traffic, the FIFO will remain at or near its
                               full condition 0x1 = at all times, traffic from
                               UPI ingress to UPI link layer is modulated by a
                               clock ratio-based rate matching signal; with
                               sustained traffic, one or two flits may be in
                               the FIFO at any given time; idle latency may not
                               be optimal 0x2 = flits may be sent to the domain
                               crossing FIFO in a burst until the number of
                               flits in the FIFO reaches a threshold and while
                               at that threshold, traffic is modulated by the
                               ratio-based rate matching signal; idle latency
                               should be minimal 0x3 = flits will be sent to
                               the domain crossing FIFO in a burst until a
                               threshold of consecutive flits is reached and
                               after reaching that threshold, traffic is
                               modulated by the ratio-based rate matching
                               signal; a single idle cycle will reset the
                               consecutive flit count; idle latency should be
                               minimal
                            */
    UINT32 idlebypassexitmodeselector : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field controls when AD messages may no
                               longer use the idle bypass path and start
                               traveling through the standard pipeline instead.
                               0x0 = bypass will only stop if primary resources
                               (e.g. a credit pool) have run out 0x1 = bypass
                               will stop (or never start) depending on the
                               number of flits currently queued for transfer
                               from ingress to the link layer 0x2 = bypass will
                               stop depending on the number of bypasses
                               performed immediately prior to the current
                               message
                            */
    UINT32 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flitsinbgfifothresholdunnnh : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000006*/

                            /*
                               If the valid transmit cycle mode is set to 0x2,
                               then flits may be sent from UPI ingress to UPI
                               link layer in a burst as long as the number of
                               flits queued in the ingress-to-link domain
                               crossing FIFO does not exceed the value of this
                               field.
                            */
    UINT32 priorflittxthreshold : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000002*/

                            /*
                               If the valid transmit cycle mode is set to 0x3,
                               then flits may be sent from UPI ingress to UPI
                               link layer in a burst until N flits have been
                               transmitted consecutively, where N is the value
                               of this field. (A single idle cycle resets the
                               consecutive flit counter.)
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 slot2blocktimer : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000010*/

                            /*
                               Timer value for delaying slot2 blocking from UPI
                               Agent request. When UPI Agent will requests slot
                               2 for LLCRD, this value delays immediate
                               blocking of slot 2 scheduling for x clock cycles
                               (Uclk). The reason for delaying is to reduce
                               frequency of forced LLCRD messages that can
                               reduce performance, and to prevent back-to-back
                               slot2 blocking for LLCRD.
                            */
    UINT32 disablewaitforpump1 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Chicken bit to disable store-and-forward for
                               both pumps. Setting this bit disables the fix
                               done for feature 1607151968
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KINGCTL_M3KTI_MAIN_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 adbypassexitthrqueued : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x1, then
                               idle bypass mode is exited (or never started)
                               when N (or more) flits are queued for transfer
                               to the link layer, where N is the value of this
                               field.
                            */
    UINT32 adbypassexitthrprior : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x2, then
                               idle bypass mode is exited after N consecutive
                               bypasses, where N is the value of this field. (A
                               single cycle without a bypass resets the
                               consecutive bypass counter.)
                            */
    UINT32 rsvd : 3;

                            /* Bits[10:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idlebypassexitmodeselector : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field controls when AD messages may no
                               longer use the idle bypass path and start
                               traveling through the standard pipeline instead.
                               0x0 = bypass will only stop if primary resources
                               (e.g. a credit pool) have run out 0x1 = bypass
                               will stop (or never start) depending on the
                               number of flits currently queued for transfer
                               from ingress to the link layer 0x2 = bypass will
                               stop depending on the number of bypasses
                               performed immediately prior to the current
                               message
                            */
    UINT32 rsvd_13 : 17;

                            /* Bits[29:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disablewaitforpump1 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Chicken bit to disable store-and-forward for
                               both pumps. Setting this bit disables the fix
                               done for feature 1607151968
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KINGCTL_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 adbypassexitthrqueued : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x1, then
                               idle bypass mode is exited (or never started)
                               when N (or more) flits are queued for transfer
                               to the link layer, where N is the value of this
                               field.
                            */
    UINT32 adbypassexitthrprior : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               If the AD bypass exit mode is set to 0x2, then
                               idle bypass mode is exited after N consecutive
                               bypasses, where N is the value of this field. (A
                               single cycle without a bypass resets the
                               consecutive bypass counter.)
                            */
    UINT32 rsvd : 3;

                            /* Bits[10:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idlebypassexitmodeselector : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field controls when AD messages may no
                               longer use the idle bypass path and start
                               traveling through the standard pipeline instead.
                               0x0 = bypass will only stop if primary resources
                               (e.g. a credit pool) have run out 0x1 = bypass
                               will stop (or never start) depending on the
                               number of flits currently queued for transfer
                               from ingress to the link layer 0x2 = bypass will
                               stop depending on the number of bypasses
                               performed immediately prior to the current
                               message
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KINGCTL_M3KTI_MAIN_STRUCT;

/* M3EGRERRMSK8_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28148)                                                      */
/*      SPRB0 (0x21e28148)                                                      */
/*      SPRHBM (0x21e28148)                                                     */
/*      SPRC0 (0x21e28148)                                                      */
/*      SPRMCC (0x21e28148)                                                     */
/*      SPRUCC (0x21e4a148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3EGRERRMSK8
*/


#define M3EGRERRMSK8_M3KTI_MAIN_REG 0x0A020148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti0parerrhdrrfingmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Par Error RF ingress mask */
    UINT32 kti0parerre2ebeingmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* Par error E2EBE ingress mask */
    UINT32 kti0parerre2edataingmask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Par error E2E Data ingress mask */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0bgfcrdunderflowmask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* BGF credit underflow mask */
    UINT32 kti0bgfcrdoverflowmask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* BGF credit overflow mask */
    UINT32 kti0rbtfifounderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* Rbt fifo underflow mask */
    UINT32 kti0rbtfifooverflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Rbt fifo overflow mask */
    UINT32 rsvd_8 : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti1parerrhdrrfingmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Par Error RF ingress mask */
    UINT32 kti1parerre2ebeingmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Par error E2EBE ingress mask */
    UINT32 kti1parerre2edataingmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* Par error E2E Data ingress mask */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti1bgfcrdunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* BGF credit underflow mask */
    UINT32 kti1bgfcrdoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* BGF credit overflow mask */
    UINT32 kti1rbtfifounderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Rbt fifo underflow mask */
    UINT32 kti1rbtfifooverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Rbt fifo overflow mask */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3EGRERRMSK8_M3KTI_MAIN_STRUCT;


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KLL_LDCL_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e28154)                                                      */
/*      SPRB0 (0x21e28154)                                                      */
/*      SPRHBM (0x21e28154)                                                     */
/*      SPRC0 (0x21e28154)                                                      */
/*      SPRMCC (0x21e28154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3KLL_LDCL
*/


#define M3KLL_LDCL_M3KTI_MAIN_REG 0x0A020154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disableflitslot1 : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               By default, all flit slots may be used. If this
                               bit is set to 1, no messages will be placed into
                               flit slot 1. (In some cases, a single message
                               occupies flit slots 0 and 1. Such a message is
                               technically a slot 0 message and the associated
                               use of slot 1 is always allowed and not disabled
                               by this bit.)
                            */
    UINT32 disableflitslot2 : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               By default, all flit slots may be used. If this
                               bit is set to 1, no protocol messages will be
                               placed into flit slot 2. (The link layer may
                               still use flit slot 2 for credit return
                               messages.)
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KLL_LDCL_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* M3INGERRMASK0_M3KTI_MAIN_REG supported on:                                   */
/*      SPRA0 (0x21e28158)                                                      */
/*      SPRB0 (0x21e28158)                                                      */
/*      SPRHBM (0x21e28158)                                                     */
/*      SPRC0 (0x21e28158)                                                      */
/*      SPRMCC (0x21e28158)                                                     */
/*      SPRUCC (0x21e4a158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3INGERRMASK0
*/


#define M3INGERRMASK0_M3KTI_MAIN_REG 0x0A020158

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blp1pcmpbufoverflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* BL Cmp buffer overflow mask */
    UINT32 blp1pcmpbufunderflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* BL cmp buffer underflow mask */
    UINT32 blpumperrorsecondnot1mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL Pump error second not 1 mask */
    UINT32 blpumperrorfirstnot0mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL pump error first not 0 mask */
    UINT32 llresettxnotidlemask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* LL reset TX not idle mask */
    UINT32 llresetrxnotidlemask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* LL reset Rx not idle mask */
    UINT32 blingrlistdeferbufunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow mask */
    UINT32 blingrlistdeferbufoverflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow mask */
    UINT32 bld2kmarkerfifounderflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker FIFO underflow mask */
    UINT32 bld2kmarkerfifooverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker fifo overflow mask */
    UINT32 bld2kinfofifounderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 bld2kinfofifooverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 blcrdtypefifounderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL credit type fifo underflow mask */
    UINT32 blcrdtypefifooverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL Credit type fifo overflow mask */
    UINT32 blsrcidfifounderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL src ID fifo underflow mask */
    UINT32 blsrcidfifooverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL src id fifo overflow mask */
    UINT32 txqcrdunderflowmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Tx credit underflow mask */
    UINT32 txqcrdoverflowmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Tx credit overflow mask */
    UINT32 blingressunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress underflow mask */
    UINT32 blingressoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress overflow mask */
    UINT32 adingressunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress underflow mask */
    UINT32 adingressoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress overflow mask */
    UINT32 d2kcmpcreditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit underflow mask */
    UINT32 d2kcmpcreditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit overflow mask */
    UINT32 bgfcreditunderflowmask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Bgf credit underflow mask */
    UINT32 bgfcreditoverflowmask : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Bgf credit overflow mask */
    UINT32 remotevn1creditunderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn1 credit underflow mask */
    UINT32 remotevn1creditoverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn1 credit overflow mask */
    UINT32 remotevn0creditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn0 credit underflow mask */
    UINT32 remotevn0creditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn0 credit overflow mask */
    UINT32 remotevnacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* Remote vna credit underflow mask */
    UINT32 remotevnacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Remote vna credit overflow mask */

  } Bits;
  UINT32 Data;

} M3INGERRMASK0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blp1pcmpbufoverflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* BL Cmp buffer overflow mask */
    UINT32 blp1pcmpbufunderflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* BL cmp buffer underflow mask */
    UINT32 blpumperrorsecondnot1mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL Pump error second not 1 mask */
    UINT32 blpumperrorfirstnot0mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL pump error first not 0 mask */
    UINT32 rsvd : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingrlistdeferbufunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow mask */
    UINT32 blingrlistdeferbufoverflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow mask */
    UINT32 bld2kmarkerfifounderflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker FIFO underflow mask */
    UINT32 bld2kmarkerfifooverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker fifo overflow mask */
    UINT32 bld2kinfofifounderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 bld2kinfofifooverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 blcrdtypefifounderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL credit type fifo underflow mask */
    UINT32 blcrdtypefifooverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL Credit type fifo overflow mask */
    UINT32 blsrcidfifounderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL src ID fifo underflow mask */
    UINT32 blsrcidfifooverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL src id fifo overflow mask */
    UINT32 rsvd_16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingressunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress underflow mask */
    UINT32 blingressoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress overflow mask */
    UINT32 adingressunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress underflow mask */
    UINT32 adingressoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress overflow mask */
    UINT32 d2kcmpcreditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit underflow mask */
    UINT32 d2kcmpcreditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit overflow mask */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remoteblvn1creditunderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vn1 credit underflow mask */
    UINT32 remoteblvn1creditoverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vn1 credit overflow mask */
    UINT32 remoteblvn0creditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vn0 credit underflow mask */
    UINT32 remoteblvn0creditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vn0 credit overflow mask */
    UINT32 remoteblvnacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vna credit underflow mask */
    UINT32 remoteblvnacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Remote BL vna credit overflow mask */

  } Bits;
  UINT32 Data;

} M3INGERRMASK0_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blp1pcmpbufoverflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* BL Cmp buffer overflow mask */
    UINT32 blp1pcmpbufunderflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* BL cmp buffer underflow mask */
    UINT32 blpumperrorsecondnot1mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL Pump error second not 1 mask */
    UINT32 blpumperrorfirstnot0mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL pump error first not 0 mask */
    UINT32 rsvd : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingrlistdeferbufunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow mask */
    UINT32 blingrlistdeferbufoverflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow mask */
    UINT32 bld2kmarkerfifounderflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker FIFO underflow mask */
    UINT32 bld2kmarkerfifooverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker fifo overflow mask */
    UINT32 bld2kinfofifounderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 bld2kinfofifooverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 blcrdtypefifounderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL credit type fifo underflow mask */
    UINT32 blcrdtypefifooverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL Credit type fifo overflow mask */
    UINT32 blsrcidfifounderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL src ID fifo underflow mask */
    UINT32 blsrcidfifooverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL src id fifo overflow mask */
    UINT32 rsvd_16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingressunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress underflow mask */
    UINT32 blingressoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress overflow mask */
    UINT32 adingressunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress underflow mask */
    UINT32 adingressoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress overflow mask */
    UINT32 d2kcmpcreditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit underflow mask */
    UINT32 d2kcmpcreditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit overflow mask */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3INGERRMASK0_M3KTI_MAIN_STRUCT;

/* M3INGERRMASK1_M3KTI_MAIN_REG supported on:                                   */
/*      SPRA0 (0x21e2815c)                                                      */
/*      SPRB0 (0x21e2815c)                                                      */
/*      SPRHBM (0x21e2815c)                                                     */
/*      SPRC0 (0x21e2815c)                                                      */
/*      SPRMCC (0x21e2815c)                                                     */
/*      SPRUCC (0x21e4a15c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* M3INGERRMASK1
*/


#define M3INGERRMASK1_M3KTI_MAIN_REG 0x0A02015C

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blp1pcmpbufoverflowmask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* BL cmp buffer overflow mask */
    UINT32 blp1pcmpbufunderflowmask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* BL compare buffer underflow mask */
    UINT32 blpumperrorsecondnot1mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* BL pump error second not1 mask */
    UINT32 blpumperrorfirstnot0mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* BL pump error first not0 mask */
    UINT32 llresettxnotidlemask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* LL reset tx not idle mask */
    UINT32 llresetrxnotidlemask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* LL reset Rx not idle mask */
    UINT32 blingrlistdeferbufunderflowmask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow mask */
    UINT32 blingrlistdeferbufoverflowmask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow mask */
    UINT32 bld2kmarkerfifounderflowmask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BL marker fifo underflow mask */
    UINT32 bld2kmarkerfifooverflowmask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* BL Marker fifo overflow mask */
    UINT32 bld2kinfofifounderflowmask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo underflow mask */
    UINT32 bld2kinfofifooverflowmask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* BL info fifo overflow mask */
    UINT32 blcrdtypefifounderflowmask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* BL credit type fifo underflow mask */
    UINT32 blcrdtypefifooverflowmask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* BL type fifo overflow mask */
    UINT32 blsrcidfifounderflowmask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* BL source id fifo underflow mask */
    UINT32 blsrcidfifooverflowmask : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* BL source id fifo overflow mask */
    UINT32 txqcrdunderflowmask : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Tx credit underflow mask */
    UINT32 txqcrdoverflowmask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Tx credit overflow mask */
    UINT32 blingressunderflowmask : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress underflow mask */
    UINT32 blingressoverflowmask : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* BL ingress overflow mask */
    UINT32 adingressunderflowmask : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress underflow mask */
    UINT32 adingressoverflowmask : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* AD ingress overflow mask */
    UINT32 d2kcmpcreditunderflowmask : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit underflow mask */
    UINT32 d2kcmpcreditoverflowmask : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Compare credit overflow mask */
    UINT32 bgfcreditunderflowmask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Bgf credit underflow mask */
    UINT32 bgfcreditoverflowmask : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Bgf credit overflow mask */
    UINT32 remotevn1creditunderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn1 credit underflow mask */
    UINT32 remotevn1creditoverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn1 credit overflow mask */
    UINT32 remotevn0creditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn0 credit underflow mask */
    UINT32 remotevn0creditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Remote vn0 credit overflow mask */
    UINT32 remotevnacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* Remote vna credit underflow mask */
    UINT32 remotevnacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Remote vna credit overflow mask */

  } Bits;
  UINT32 Data;

} M3INGERRMASK1_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 24;

                            /* Bits[23:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remoteakvnacreditunderflowmask : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Remote AK VNA Credit Underflow Mask */
    UINT32 remoteakvnacreditoverflowmask : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Remote AK VNA Credit Overflow Mask */
    UINT32 remoteadvn1creditunderflowmask : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vn1 credit underflow mask */
    UINT32 remoteadvn1creditoverflowmask : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vn1 credit overflow mask */
    UINT32 remoteadvn0creditunderflowmask : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vn0 credit underflow mask */
    UINT32 remoteadvn0creditoverflowmask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vn0 credit overflow mask */
    UINT32 remoteadvnacreditunderflowmask : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vna credit underflow mask */
    UINT32 remoteadvnacreditoverflowmask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Remote AD vna credit overflow mask */

  } Bits;
  UINT32 Data;

} M3INGERRMASK1_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

/* M3INGERRLOG0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28160)                                                      */
/*      SPRB0 (0x21e28160)                                                      */
/*      SPRHBM (0x21e28160)                                                     */
/*      SPRC0 (0x21e28160)                                                      */
/*      SPRMCC (0x21e28160)                                                     */
/*      SPRUCC (0x21e4a160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3INGERRLOG0
*/


#define M3INGERRLOG0_M3KTI_MAIN_REG 0x0A020160

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d2kcmpcreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp Credit underflow */
    UINT32 d2kcmpcreditoverflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp credit overflow */
    UINT32 bgfcreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bgf credit underflow */
    UINT32 bgfcreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bgf credit overflow */
    UINT32 remotevn1ncscreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit underflow */
    UINT32 remotevn1ncbcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit underflow */
    UINT32 remotevn1wbcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit underflow */
    UINT32 remotevn1rspcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit underflow */
    UINT32 remotevn1snpcreditunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit underflow */
    UINT32 remotevn1reqcreditunderflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rquest credit underflow */
    UINT32 remotevn1ncscreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit overflow */
    UINT32 remotevn1ncbcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit overflow */
    UINT32 remotevn1wbcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit overflow */
    UINT32 remotevn1rspcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit overflow */
    UINT32 remotevn1snpcreditoverflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit overflow */
    UINT32 remotevn1reqcreditoverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 request credit overflow */
    UINT32 remotevn0ncscreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit underflow */
    UINT32 remotevn0ncbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit underflow */
    UINT32 remotevn0wbcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit underflow */
    UINT32 remotevn0rspcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit underflow */
    UINT32 remotevn0snpcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit underflow */
    UINT32 remotevn0reqcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit underflow */
    UINT32 remotevn0ncscreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit overflow */
    UINT32 remotevn0ncbcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit overflow */
    UINT32 remotevn0wbcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit overflow */
    UINT32 remotevn0rspcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit overflow */
    UINT32 remotevn0snpcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit overflow */
    UINT32 remotevn0reqcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit overflow */
    UINT32 remotevnacreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vna credit underflow */
    UINT32 remotevnacreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vna credit overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d2kcmpcreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp Credit underflow */
    UINT32 d2kcmpcreditoverflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp credit overflow */
    UINT32 rsvd_4 : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remotevn1ncscreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit underflow */
    UINT32 remotevn1ncbcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit underflow */
    UINT32 remotevn1wbcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit underflow */
    UINT32 remotevn1rspcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit underflow */
    UINT32 remotevn1snpcreditunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit underflow */
    UINT32 remotevn1reqcreditunderflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rquest credit underflow */
    UINT32 remotevn1ncscreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit overflow */
    UINT32 remotevn1ncbcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit overflow */
    UINT32 remotevn1wbcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit overflow */
    UINT32 remotevn1rspcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit overflow */
    UINT32 remotevn1snpcreditoverflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit overflow */
    UINT32 remotevn1reqcreditoverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 request credit overflow */
    UINT32 remotevn0ncscreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit underflow */
    UINT32 remotevn0ncbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit underflow */
    UINT32 remotevn0wbcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit underflow */
    UINT32 remotevn0rspcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit underflow */
    UINT32 remotevn0snpcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit underflow */
    UINT32 remotevn0reqcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit underflow */
    UINT32 remotevn0ncscreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit overflow */
    UINT32 remotevn0ncbcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit overflow */
    UINT32 remotevn0wbcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit overflow */
    UINT32 remotevn0rspcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit overflow */
    UINT32 remotevn0snpcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit overflow */
    UINT32 remotevn0reqcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit overflow */
    UINT32 remoteblvnacreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote BL vna credit underflow */
    UINT32 remoteblvnacreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote BL vna credit overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG0_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d2kcmpcreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp Credit underflow */
    UINT32 d2kcmpcreditoverflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cmp credit overflow */
    UINT32 rsvd_4 : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remotevn1ncscreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit underflow */
    UINT32 remotevn1ncbcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit underflow */
    UINT32 remotevn1wbcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit underflow */
    UINT32 remotevn1rspcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit underflow */
    UINT32 remotevn1snpcreditunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit underflow */
    UINT32 remotevn1reqcreditunderflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rquest credit underflow */
    UINT32 remotevn1ncscreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit overflow */
    UINT32 remotevn1ncbcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit overflow */
    UINT32 remotevn1wbcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit overflow */
    UINT32 remotevn1rspcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit overflow */
    UINT32 remotevn1snpcreditoverflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit overflow */
    UINT32 remotevn1reqcreditoverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 request credit overflow */
    UINT32 remotevn0ncscreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit underflow */
    UINT32 remotevn0ncbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit underflow */
    UINT32 remotevn0wbcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit underflow */
    UINT32 remotevn0rspcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit underflow */
    UINT32 remotevn0snpcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit underflow */
    UINT32 remotevn0reqcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit underflow */
    UINT32 remotevn0ncscreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit overflow */
    UINT32 remotevn0ncbcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit overflow */
    UINT32 remotevn0wbcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit overflow */
    UINT32 remotevn0rspcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit overflow */
    UINT32 remotevn0snpcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit overflow */
    UINT32 remotevn0reqcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit overflow */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3INGERRLOG0_M3KTI_MAIN_STRUCT;

/* M3INGERRLOG1_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28164)                                                      */
/*      SPRB0 (0x21e28164)                                                      */
/*      SPRHBM (0x21e28164)                                                     */
/*      SPRC0 (0x21e28164)                                                      */
/*      SPRMCC (0x21e28164)                                                     */
/*      SPRUCC (0x21e4a164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3INGERRLOG1
*/


#define M3INGERRLOG1_M3KTI_MAIN_REG 0x0A020164

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blp1pcmpbufoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer overflow */
    UINT32 blp1pcmpbufunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer underflow */
    UINT32 blpumperrorsecondnot1 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Pump error second not 1 */
    UINT32 blpumperrorfirstnot0 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl pump error first not 0 */
    UINT32 llresetwhiletxqcreditbusy : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* LL reset while tx credit busy */
    UINT32 llresetwhileslotnotidle : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* LL reset while slot not idle */
    UINT32 llresetwhilelistnotempty : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* LL reset while list not empty */
    UINT32 llresetrxnotidle : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* LL reset Rx not idle */
    UINT32 blingrlistdeferbufunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow */
    UINT32 blingrlistdeferbufoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow */
    UINT32 bld2kmarkerfifounderflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo underflow */
    UINT32 bld2kmarkerfifooverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo overflow */
    UINT32 bld2kinfofifounderflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo underflow */
    UINT32 bld2kinfofifooverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo overflow */
    UINT32 blcrdtypefifounderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL credit type fifo underflow */
    UINT32 blcrdtypefifooverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl credit type fifo overflow */
    UINT32 blsrcidfifounderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo underflow */
    UINT32 blsrcidfifooverflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo overflow */
    UINT32 txqcrdunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Tx credit underflow */
    UINT32 txqcrdoverflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Tx credit overflow */
    UINT32 blingressvcidunderflow : 3;

                            /* Bits[24:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress VCID underflow */
    UINT32 blingressunderflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress underflow */
    UINT32 blingressoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress overflow */
    UINT32 adingressvcidunderflow : 3;

                            /* Bits[29:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress VCID underflow */
    UINT32 adingressunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress underflow */
    UINT32 adingressoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG1_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blp1pcmpbufoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer overflow */
    UINT32 blp1pcmpbufunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer underflow */
    UINT32 blpumperrorsecondnot1 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Pump error second not 1 */
    UINT32 blpumperrorfirstnot0 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl pump error first not 0 */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingrlistdeferbufunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow */
    UINT32 blingrlistdeferbufoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow */
    UINT32 bld2kmarkerfifounderflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo underflow */
    UINT32 bld2kmarkerfifooverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo overflow */
    UINT32 bld2kinfofifounderflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo underflow */
    UINT32 bld2kinfofifooverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo overflow */
    UINT32 blcrdtypefifounderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL credit type fifo underflow */
    UINT32 blcrdtypefifooverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl credit type fifo overflow */
    UINT32 blsrcidfifounderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo underflow */
    UINT32 blsrcidfifooverflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo overflow */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingressvcidunderflow : 3;

                            /* Bits[24:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress VCID underflow */
    UINT32 blingressunderflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress underflow */
    UINT32 blingressoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress overflow */
    UINT32 adingressvcidunderflow : 3;

                            /* Bits[29:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress VCID underflow */
    UINT32 adingressunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress underflow */
    UINT32 adingressoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG1_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blp1pcmpbufoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer overflow */
    UINT32 blp1pcmpbufunderflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL compare buffer underflow */
    UINT32 blpumperrorsecondnot1 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Pump error second not 1 */
    UINT32 blpumperrorfirstnot0 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl pump error first not 0 */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingrlistdeferbufunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Ingress list defer buffer underflow */
    UINT32 blingrlistdeferbufoverflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress list defer buffer overflow */
    UINT32 bld2kmarkerfifounderflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo underflow */
    UINT32 bld2kmarkerfifooverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL marker fifo overflow */
    UINT32 bld2kinfofifounderflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo underflow */
    UINT32 bld2kinfofifooverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL info fifo overflow */
    UINT32 blcrdtypefifounderflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL credit type fifo underflow */
    UINT32 blcrdtypefifooverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bl credit type fifo overflow */
    UINT32 blsrcidfifounderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo underflow */
    UINT32 blsrcidfifooverflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL src id fifo overflow */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 blingressvcidunderflow : 3;

                            /* Bits[24:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress VCID underflow */
    UINT32 blingressunderflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress underflow */
    UINT32 blingressoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL ingress overflow */
    UINT32 adingressvcidunderflow : 3;

                            /* Bits[29:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress VCID underflow */
    UINT32 adingressunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress underflow */
    UINT32 adingressoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD ingress overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG1_M3KTI_MAIN_STRUCT;

/* M3INGERRLOG2_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28168)                                                      */
/*      SPRB0 (0x21e28168)                                                      */
/*      SPRHBM (0x21e28168)                                                     */
/*      SPRC0 (0x21e28168)                                                      */
/*      SPRMCC (0x21e28168)                                                     */
/*      SPRUCC (0x21e4a168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3INGERRLOG2
*/


#define M3INGERRLOG2_M3KTI_MAIN_REG 0x0A020168

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d2kcmpcreditunderflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Compare credit underflow */
    UINT32 d2kcmpcreditoverflow : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Compare credit overflow */
    UINT32 bgfcreditunderflow : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bgf credit underflow */
    UINT32 bgfcreditoverflow : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Bgf credit overflow */
    UINT32 remotevn1ncscreditunderflow : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit underflow */
    UINT32 remotevn1ncbcreditunderflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit underflow */
    UINT32 remotevn1wbcreditunderflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit underflow */
    UINT32 remotevn1rspcreditunderflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit underflow */
    UINT32 remotevn1snpcreditunderflow : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit underflow */
    UINT32 remotevn1reqcreditunderflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 request credit underflow */
    UINT32 remotevn1ncscreditoverflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncs credit overflow */
    UINT32 remotevn1ncbcreditoverflow : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 ncb credit overflow */
    UINT32 remotevn1wbcreditoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 wb credit overflow */
    UINT32 remotevn1rspcreditoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 rsp credit overflow */
    UINT32 remotevn1snpcreditoverflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 snoop credit overflow */
    UINT32 remotevn1reqcreditoverflow : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn1 request credit overflow */
    UINT32 remotevn0ncscreditunderflow : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit underflow */
    UINT32 remotevn0ncbcreditunderflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit underflow */
    UINT32 remotevn0wbcreditunderflow : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit underflow */
    UINT32 remotevn0rspcreditunderflow : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit underflow */
    UINT32 remotevn0snpcreditunderflow : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit underflow */
    UINT32 remotevn0reqcreditunderflow : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit underflow */
    UINT32 remotevn0ncscreditoverflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncs credit overflow */
    UINT32 remotevn0ncbcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 ncb credit overflow */
    UINT32 remotevn0wbcreditoverflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 wb credit overflow */
    UINT32 remotevn0rspcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 rsp credit overflow */
    UINT32 remotevn0snpcreditoverflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 snoop credit overflow */
    UINT32 remotevn0reqcreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vn0 request credit overflow */
    UINT32 remotevnacreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vna credit underflow */
    UINT32 remotevnacreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote vna credit overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG2_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 24;

                            /* Bits[23:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 remotevn1adrspcreditunderflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote VN1 AD Rsp Credit Underflow */
    UINT32 remotevn1adrspcreditoverflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote VN1 AD Rsp Credit Overflow */
    UINT32 remotevn0adrspcreditunderflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote VN0 AD Rsp Credit Underflow */
    UINT32 remotevn0adrspcreditoverflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote VN0 AD Rsp Credit Overflow */
    UINT32 remoteakvnacreditunderflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote AK VNA Credit Underflow */
    UINT32 remoteakvnacreditoverflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote AK VNA Credit Overflow */
    UINT32 remoteadvnacreditunderflow : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote AD VNA Credit Underflow */
    UINT32 remoteadvnacreditoverflow : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Remote AD VNA Credit Overflow */

  } Bits;
  UINT32 Data;

} M3INGERRLOG2_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3INGERRLOG3_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e2816c)                                                      */
/*      SPRB0 (0x21e2816c)                                                      */
/*      SPRHBM (0x21e2816c)                                                     */
/*      SPRC0 (0x21e2816c)                                                      */
/*      SPRMCC (0x21e2816c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* M3INGERRLOG3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e28170)                                                      */
/*      SPRB0 (0x21e28170)                                                      */
/*      SPRHBM (0x21e28170)                                                     */
/*      SPRC0 (0x21e28170)                                                      */
/*      SPRMCC (0x21e28170)                                                     */
/*      SPRUCC (0x21e4a170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3INGDBGREMCRE0KTI0
*/


#define M3INGDBGREMCRE0KTI0_M3KTI_MAIN_REG 0x0A020170

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti0remotevnacreditreferencecount : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its VNA credit allocation for
                               the remote socket via initial credit returns
                               transmitted by the remote socket. However, for
                               error detection (e.g. credit overflow) to work
                               and for clock gating to be more efficient, the
                               ingress logic needs an independent reference for
                               what the total credit VNA count is supposed to
                               be. The value of this field is this reference.
                               Once this field is programmed, the value needs
                               to be activated by toggling the
                               LoadVnaCreditReference bit.
                            */
    UINT32 kti0remotevn0reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0rspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0rspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE0KTI0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti0remoteblvnacreditreferencecount : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its VNA credit allocation for
                               the remote socket via initial credit returns
                               transmitted by the remote socket. However, for
                               error detection (e.g. credit overflow) to work
                               and for clock gating to be more efficient, the
                               ingress logic needs an independent reference for
                               what the total credit VNA count is supposed to
                               be. The value of this field is this reference.
                               Once this field is programmed, the value needs
                               to be activated by toggling the
                               LoadVnaCreditReference bit.
                            */
    UINT32 kti0remotevn0reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0blrspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0blrspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE0KTI0_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0remotevn0reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0remotevn0wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn0ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn0ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE0KTI0_M3KTI_MAIN_STRUCT;

/* M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e28174)                                                      */
/*      SPRB0 (0x21e28174)                                                      */
/*      SPRHBM (0x21e28174)                                                     */
/*      SPRC0 (0x21e28174)                                                      */
/*      SPRMCC (0x21e28174)                                                     */
/*      SPRUCC (0x21e4a174)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3INGDBGREMCRE1KTI0
*/


#define M3INGDBGREMCRE1KTI0_M3KTI_MAIN_REG 0x0A020174

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti0txqcreditreference : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000040*/

                            /*
                               Programmable TXQ credits in m2upi ingress.
                               Defaults to 64.
                            */
    UINT32 kti0loadremotevnacreditreference : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnaCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnaCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1rspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1rspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE1KTI0_M3KTI_MAIN_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 7;

                            /* Bits[6:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0loadremoteblvnacreditreference : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemBLVnaCreditRefValue field, this bit needs to
                               be switched from 0 to 1 to 0. (If this bit is
                               left at 1, then any change in
                               RemVnaCreditRefValue may cause undesired
                               behavior.)
                            */
    UINT32 kti0remotevn1reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1blrspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1blrspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemBLVnxCreditRefValue field, this bit needs to
                               be switched from 0 to 1 to 0. (If this bit is
                               left at 1, then any change in
                               RemVnxCreditRefValue may cause undesired
                               behavior.)
                            */
    UINT32 kti0remotevn1wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE1KTI0_M3KTI_MAIN_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0remotevn1reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0remotevn1wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti0remotevn1ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti0loadremotevn1ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE1KTI0_M3KTI_MAIN_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3INGDBGREMCRE0KTI1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e28178)                                                      */
/*      SPRB0 (0x21e28178)                                                      */
/*      SPRHBM (0x21e28178)                                                     */
/*      SPRC0 (0x21e28178)                                                      */
/*      SPRMCC (0x21e28178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* M3INGDBGREMCRE0KTI1
*/


#define M3INGDBGREMCRE0KTI1_M3KTI_MAIN_REG 0x0A020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti1remotevnacreditreferencecount : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its VNA credit allocation for
                               the remote socket via initial credit returns
                               transmitted by the remote socket. However, for
                               error detection (e.g. credit overflow) to work
                               and for clock gating to be more efficient, the
                               ingress logic needs an independent reference for
                               what the total credit VNA count is supposed to
                               be. The value of this field is this reference.
                               Once this field is programmed, the value needs
                               to be activated by toggling the
                               LoadVnaCreditReference bit.
                            */
    UINT32 kti1loadremotevnacreditreference : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnaCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnaCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0rspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0rspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn0ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn0ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE0KTI1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#ifdef SPRUCC_HOST
/* M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG supported on:                             */
/*      SPRUCC (0x21e4a178)                                                     */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRUCC BDF: 30_9_2                                     */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* M3INGDBGREMCRE2KTI0
*/


#define M3INGDBGREMCRE2KTI0_M3KTI_MAIN_REG 0x0A020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti0remoteakvnacreditreferencecount : 8;

                            /* Bits[13:6], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Remote AK Vna Credit Reference Count */
    UINT32 kti0loadremoteakvnacreditreference : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Load Remote AK Vna Credit Reference */
    UINT32 kti0remoteadvnacreditreferencecount : 8;

                            /* Bits[22:15], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Remote AD Vna Credit Reference Count */
    UINT32 kti0loadremoteadvnacreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Load Remote AD Vna Credit Reference */
    UINT32 kti0remotevn1adrspcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Remote Vn1 AD Rsp Credit Reference Count */
    UINT32 kti0loadremotevn1adrspcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Load Remote Vn1 AD Rsp Credit Reference */
    UINT32 kti0remotevn0adrspcreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Remote Vn0 AD Rsp Credit Reference Count */
    UINT32 kti0loadremotevn0adrspcreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Kti0 Load Remote Vn0 AD Rsp Credit Reference */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE2KTI0_M3KTI_MAIN_STRUCT;
#endif /* (SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3INGDBGREMCRE1KTI1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e2817c)                                                      */
/*      SPRB0 (0x21e2817c)                                                      */
/*      SPRHBM (0x21e2817c)                                                     */
/*      SPRC0 (0x21e2817c)                                                      */
/*      SPRMCC (0x21e2817c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* M3INGDBGREMCRE1KTI1
*/


#define M3INGDBGREMCRE1KTI1_M3KTI_MAIN_REG 0x0A02017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti1remotevn1reqcreditreferencecount : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1reqcreditreference : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn1snpcreditreferencecount : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1snpcreditreference : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn1rspcreditreferencecount : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1rspcreditreference : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn1wbcreditreferencecount : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1wbcreditreference : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn1ncbcreditreferencecount : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1ncbcreditreference : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */
    UINT32 kti1remotevn1ncscreditreferencecount : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               For most aspects of normal operation, the UPI
                               ingress logic gets its non-VNA credit allocation
                               (i.e. credits for each of VN0/VN1 crossed with
                               REQ/SNP/RSP/WB/NCS/NCB) for the remote socket
                               via initial credit returns transmitted by the
                               remote socket. However, for error detection
                               (e.g. credit overflow) to work and for clock
                               gating to be more efficient, the ingress logic
                               needs an independent reference for what the
                               maximum credit non-VNA count is supposed to be.
                               The value of this field is this reference. Once
                               this field is programmed, the value needs to be
                               activated by toggling the LoadVnxCreditReference
                               bit. (The same reference value is applied to
                               each of the 12 non-VNA credit pools.)
                            */
    UINT32 kti1loadremotevn1ncscreditreference : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               To activate a value programmed into the
                               RemVnxCreditRefValue field, this bit needs to be
                               switched from 0 to 1 to 0. (If this bit is left
                               at 1, then any change in RemVnxCreditRefValue
                               may cause undesired behavior.)
                            */

  } Bits;
  UINT32 Data;

} M3INGDBGREMCRE1KTI1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3INGERRINJ1_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e28184)                                                      */
/*      SPRB0 (0x21e28184)                                                      */
/*      SPRHBM (0x21e28184)                                                     */
/*      SPRC0 (0x21e28184)                                                      */
/*      SPRMCC (0x21e28184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* M3INGERRINJ1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */








/* M3KUTLPRIVC0_M3KTI_MAIN_REG supported on:                                    */
/*      SPRA0 (0x21e281d0)                                                      */
/*      SPRB0 (0x21e281d0)                                                      */
/*      SPRHBM (0x21e281d0)                                                     */
/*      SPRC0 (0x21e281d0)                                                      */
/*      SPRMCC (0x21e281d0)                                                     */
/*      SPRUCC (0x21e4a1d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KUtlPrivC0
*/


#define M3KUTLPRIVC0_M3KTI_MAIN_REG 0x0A0201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msgchepckgatedis : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Message channl End point clock gating disable. */
    UINT32 crittercsrckgatedis : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* CSR register clock gating disable. */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KUTLPRIVC0_M3KTI_MAIN_STRUCT;



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KINGRSPGROUPAAGT1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e281dc)                                                      */
/*      SPRB0 (0x21e281dc)                                                      */
/*      SPRHBM (0x21e281dc)                                                     */
/*      SPRC0 (0x21e281dc)                                                      */
/*      SPRMCC (0x21e281dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* M3KINGRSPGROUPAAGT1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KINGRSPGROUPBAGT1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e281e0)                                                      */
/*      SPRB0 (0x21e281e0)                                                      */
/*      SPRHBM (0x21e281e0)                                                     */
/*      SPRC0 (0x21e281e0)                                                      */
/*      SPRMCC (0x21e281e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* M3KINGRSPGROUPBAGT1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KEGRRSPGROUPAAGT1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e281ec)                                                      */
/*      SPRB0 (0x21e281ec)                                                      */
/*      SPRHBM (0x21e281ec)                                                     */
/*      SPRC0 (0x21e281ec)                                                      */
/*      SPRMCC (0x21e281ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* M3KEGRRSPGROUPAAGT1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* M3KEGRRSPGROUPBAGT1_M3KTI_MAIN_REG supported on:                             */
/*      SPRA0 (0x21e281f0)                                                      */
/*      SPRB0 (0x21e281f0)                                                      */
/*      SPRHBM (0x21e281f0)                                                     */
/*      SPRC0 (0x21e281f0)                                                      */
/*      SPRMCC (0x21e281f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* M3KEGRRSPGROUPBAGT1
*/


#define M3KEGRRSPGROUPBAGT1_M3KTI_MAIN_REG 0x0A0201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 14;

                            /* Bits[13:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsprestblockcrdretagt1 : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000000*/

                            /* RspRestBlockCrdRetAgt1 */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KEGRRSPGROUPBAGT1_M3KTI_MAIN_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */








/* M2UPI_OS_W_CP_LO_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e2821c)                                                      */
/*      SPRB0 (0x21e2821c)                                                      */
/*      SPRHBM (0x21e2821c)                                                     */
/*      SPRC0 (0x21e2821c)                                                      */
/*      SPRMCC (0x21e2821c)                                                     */
/*      SPRUCC (0x21e4a21c)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_CP LOWER 32bits
*/


#define M2UPI_OS_W_CP_LO_M3KTI_MAIN_REG 0x0A02021C

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

} M2UPI_OS_W_CP_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_OS_W_CP_HI_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e28220)                                                      */
/*      SPRB0 (0x21e28220)                                                      */
/*      SPRHBM (0x21e28220)                                                     */
/*      SPRC0 (0x21e28220)                                                      */
/*      SPRMCC (0x21e28220)                                                     */
/*      SPRUCC (0x21e4a220)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_CP UPPER 32bits
*/


#define M2UPI_OS_W_CP_HI_M3KTI_MAIN_REG 0x0A020220

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

} M2UPI_OS_W_CP_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_OS_W_RAC_LO_M3KTI_MAIN_REG supported on:                               */
/*      SPRA0 (0x21e28224)                                                      */
/*      SPRB0 (0x21e28224)                                                      */
/*      SPRHBM (0x21e28224)                                                     */
/*      SPRC0 (0x21e28224)                                                      */
/*      SPRMCC (0x21e28224)                                                     */
/*      SPRUCC (0x21e4a224)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_RAC LOWER 32bits
*/


#define M2UPI_OS_W_RAC_LO_M3KTI_MAIN_REG 0x0A020224

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

} M2UPI_OS_W_RAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_OS_W_RAC_HI_M3KTI_MAIN_REG supported on:                               */
/*      SPRA0 (0x21e28228)                                                      */
/*      SPRB0 (0x21e28228)                                                      */
/*      SPRHBM (0x21e28228)                                                     */
/*      SPRC0 (0x21e28228)                                                      */
/*      SPRMCC (0x21e28228)                                                     */
/*      SPRUCC (0x21e4a228)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_RAC UPPER 32bits
*/


#define M2UPI_OS_W_RAC_HI_M3KTI_MAIN_REG 0x0A020228

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

} M2UPI_OS_W_RAC_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_OS_W_WAC_LO_M3KTI_MAIN_REG supported on:                               */
/*      SPRA0 (0x21e2822c)                                                      */
/*      SPRB0 (0x21e2822c)                                                      */
/*      SPRHBM (0x21e2822c)                                                     */
/*      SPRC0 (0x21e2822c)                                                      */
/*      SPRMCC (0x21e2822c)                                                     */
/*      SPRUCC (0x21e4a22c)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_WAC LOWER 32bits
*/


#define M2UPI_OS_W_WAC_LO_M3KTI_MAIN_REG 0x0A02022C

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

} M2UPI_OS_W_WAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_OS_W_WAC_HI_M3KTI_MAIN_REG supported on:                               */
/*      SPRA0 (0x21e28230)                                                      */
/*      SPRB0 (0x21e28230)                                                      */
/*      SPRHBM (0x21e28230)                                                     */
/*      SPRC0 (0x21e28230)                                                      */
/*      SPRMCC (0x21e28230)                                                     */
/*      SPRUCC (0x21e4a230)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M2UPI_OS_W_WAC UPPER 32bits
*/


#define M2UPI_OS_W_WAC_HI_M3KTI_MAIN_REG 0x0A020230

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

} M2UPI_OS_W_WAC_HI_M3KTI_MAIN_STRUCT;



















/* M2UPI_DFX_CP_LO_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28294)                                                      */
/*      SPRB0 (0x21e28294)                                                      */
/*      SPRHBM (0x21e28294)                                                     */
/*      SPRC0 (0x21e28294)                                                      */
/*      SPRMCC (0x21e28294)                                                     */
/*      SPRUCC (0x21e4a294)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_CP LOWER 32bits
*/


#define M2UPI_DFX_CP_LO_M3KTI_MAIN_REG 0x0A020294

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

} M2UPI_DFX_CP_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_DFX_CP_HI_M3KTI_MAIN_REG supported on:                                 */
/*      SPRA0 (0x21e28298)                                                      */
/*      SPRB0 (0x21e28298)                                                      */
/*      SPRHBM (0x21e28298)                                                     */
/*      SPRC0 (0x21e28298)                                                      */
/*      SPRMCC (0x21e28298)                                                     */
/*      SPRUCC (0x21e4a298)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_CP UPPER 32bits
*/


#define M2UPI_DFX_CP_HI_M3KTI_MAIN_REG 0x0A020298

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

} M2UPI_DFX_CP_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_DFX_RAC_LO_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e2829c)                                                      */
/*      SPRB0 (0x21e2829c)                                                      */
/*      SPRHBM (0x21e2829c)                                                     */
/*      SPRC0 (0x21e2829c)                                                      */
/*      SPRMCC (0x21e2829c)                                                     */
/*      SPRUCC (0x21e4a29c)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_RAC LOWER 32bits
*/


#define M2UPI_DFX_RAC_LO_M3KTI_MAIN_REG 0x0A02029C

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

} M2UPI_DFX_RAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_DFX_RAC_HI_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e282a0)                                                      */
/*      SPRB0 (0x21e282a0)                                                      */
/*      SPRHBM (0x21e282a0)                                                     */
/*      SPRC0 (0x21e282a0)                                                      */
/*      SPRMCC (0x21e282a0)                                                     */
/*      SPRUCC (0x21e4a2a0)                                                     */
/* Register default value on SPRA0: 0x20000500                                  */
/* Register default value on SPRB0: 0x20000500                                  */
/* Register default value on SPRHBM: 0x20000500                                 */
/* Register default value on SPRC0: 0x20000500                                  */
/* Register default value on SPRMCC: 0x20000500                                 */
/* Register default value on SPRUCC: 0x20000500                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_RAC UPPER 32bits
*/


#define M2UPI_DFX_RAC_HI_M3KTI_MAIN_REG 0x0A0202A0

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

} M2UPI_DFX_RAC_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_DFX_WAC_LO_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e282a4)                                                      */
/*      SPRB0 (0x21e282a4)                                                      */
/*      SPRHBM (0x21e282a4)                                                     */
/*      SPRC0 (0x21e282a4)                                                      */
/*      SPRMCC (0x21e282a4)                                                     */
/*      SPRUCC (0x21e4a2a4)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_WAC LOWER 32bits
*/


#define M2UPI_DFX_WAC_LO_M3KTI_MAIN_REG 0x0A0202A4

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

} M2UPI_DFX_WAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_DFX_WAC_HI_M3KTI_MAIN_REG supported on:                                */
/*      SPRA0 (0x21e282a8)                                                      */
/*      SPRB0 (0x21e282a8)                                                      */
/*      SPRHBM (0x21e282a8)                                                     */
/*      SPRC0 (0x21e282a8)                                                      */
/*      SPRMCC (0x21e282a8)                                                     */
/*      SPRUCC (0x21e4a2a8)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* M2UPI_DFX_WAC UPPER 32bits
*/


#define M2UPI_DFX_WAC_HI_M3KTI_MAIN_REG 0x0A0202A8

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

} M2UPI_DFX_WAC_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_CP_LO_M3KTI_MAIN_REG supported on:                         */
/*      SPRA0 (0x21e282ac)                                                      */
/*      SPRB0 (0x21e282ac)                                                      */
/*      SPRHBM (0x21e282ac)                                                     */
/*      SPRC0 (0x21e282ac)                                                      */
/*      SPRMCC (0x21e282ac)                                                     */
/*      SPRUCC (0x21e4a2ac)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_CP LOWER 32bits
*/


#define M2UPI_LB_MCHECK_W_CP_LO_M3KTI_MAIN_REG 0x0A0202AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

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

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

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

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

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

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} M2UPI_LB_MCHECK_W_CP_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_CP_HI_M3KTI_MAIN_REG supported on:                         */
/*      SPRA0 (0x21e282b0)                                                      */
/*      SPRB0 (0x21e282b0)                                                      */
/*      SPRHBM (0x21e282b0)                                                     */
/*      SPRC0 (0x21e282b0)                                                      */
/*      SPRMCC (0x21e282b0)                                                     */
/*      SPRUCC (0x21e4a2b0)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_CP UPPER 32bits
*/


#define M2UPI_LB_MCHECK_W_CP_HI_M3KTI_MAIN_REG 0x0A0202B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

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

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to WAC, RAC and CP registers, based on
                               the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} M2UPI_LB_MCHECK_W_CP_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_RAC_LO_M3KTI_MAIN_REG supported on:                        */
/*      SPRA0 (0x21e282b4)                                                      */
/*      SPRB0 (0x21e282b4)                                                      */
/*      SPRHBM (0x21e282b4)                                                     */
/*      SPRC0 (0x21e282b4)                                                      */
/*      SPRMCC (0x21e282b4)                                                     */
/*      SPRUCC (0x21e4a2b4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_RAC LOWER 32bits
*/


#define M2UPI_LB_MCHECK_W_RAC_LO_M3KTI_MAIN_REG 0x0A0202B4

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

} M2UPI_LB_MCHECK_W_RAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_RAC_HI_M3KTI_MAIN_REG supported on:                        */
/*      SPRA0 (0x21e282b8)                                                      */
/*      SPRB0 (0x21e282b8)                                                      */
/*      SPRHBM (0x21e282b8)                                                     */
/*      SPRC0 (0x21e282b8)                                                      */
/*      SPRMCC (0x21e282b8)                                                     */
/*      SPRUCC (0x21e4a2b8)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_RAC UPPER 32bits
*/


#define M2UPI_LB_MCHECK_W_RAC_HI_M3KTI_MAIN_REG 0x0A0202B8

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

} M2UPI_LB_MCHECK_W_RAC_HI_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_REG supported on:                        */
/*      SPRA0 (0x21e282bc)                                                      */
/*      SPRB0 (0x21e282bc)                                                      */
/*      SPRHBM (0x21e282bc)                                                     */
/*      SPRC0 (0x21e282bc)                                                      */
/*      SPRMCC (0x21e282bc)                                                     */
/*      SPRUCC (0x21e4a2bc)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_WAC LOWER 32bits
*/


#define M2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_REG 0x0A0202BC

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

} M2UPI_LB_MCHECK_W_WAC_LO_M3KTI_MAIN_STRUCT;

/* M2UPI_LB_MCHECK_W_WAC_HI_M3KTI_MAIN_REG supported on:                        */
/*      SPRA0 (0x21e282c0)                                                      */
/*      SPRB0 (0x21e282c0)                                                      */
/*      SPRHBM (0x21e282c0)                                                     */
/*      SPRC0 (0x21e282c0)                                                      */
/*      SPRMCC (0x21e282c0)                                                     */
/*      SPRUCC (0x21e4a2c0)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* M2UPI_LB_MCHECK_W_WAC UPPER 32bits
*/


#define M2UPI_LB_MCHECK_W_WAC_HI_M3KTI_MAIN_REG 0x0A0202C0

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

} M2UPI_LB_MCHECK_W_WAC_HI_M3KTI_MAIN_STRUCT;

/* M3KPCIR2CR_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e282d0)                                                      */
/*      SPRB0 (0x21e282d0)                                                      */
/*      SPRHBM (0x21e282d0)                                                     */
/*      SPRC0 (0x21e282d0)                                                      */
/*      SPRMCC (0x21e282d0)                                                     */
/*      SPRUCC (0x21e4a2d0)                                                     */
/* Register default value on SPRA0: 0x00011111                                  */
/* Register default value on SPRB0: 0x00011111                                  */
/* Register default value on SPRHBM: 0x00011111                                 */
/* Register default value on SPRC0: 0x00011111                                  */
/* Register default value on SPRMCC: 0x00011111                                 */
/* Register default value on SPRUCC: 0x00011111                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KPCIR2CR
*/


#define M3KPCIR2CR_M3KTI_MAIN_REG 0x0A0202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio7_ncb : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* IIO7 NCB Credits */
    UINT32 iio8_ncb : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* IIO8 NCB Credits. */
    UINT32 iio9_ncb : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* IIO9 NCB Credits. */
    UINT32 iio10_ncb : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* IIO10 NCB Credits. */
    UINT32 iio11_ncb : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* IIO11 NCB Credits. */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KPCIR2CR_M3KTI_MAIN_STRUCT;

/* M3KPCIR3CR_M3KTI_MAIN_REG supported on:                                      */
/*      SPRA0 (0x21e282d4)                                                      */
/*      SPRB0 (0x21e282d4)                                                      */
/*      SPRHBM (0x21e282d4)                                                     */
/*      SPRC0 (0x21e282d4)                                                      */
/*      SPRMCC (0x21e282d4)                                                     */
/*      SPRUCC (0x21e4a2d4)                                                     */
/* Register default value on SPRA0: 0x00011111                                  */
/* Register default value on SPRB0: 0x00011111                                  */
/* Register default value on SPRHBM: 0x00011111                                 */
/* Register default value on SPRC0: 0x00011111                                  */
/* Register default value on SPRMCC: 0x00011111                                 */
/* Register default value on SPRUCC: 0x00011111                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0       */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/rcconfig/CFG_R3QPI_F0      */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/rcconfig/CFG_R3QPI_F0           */
/* Struct generated from SPRA0 BDF: 30_5_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* M3KPCIR3CR
*/


#define M3KPCIR3CR_M3KTI_MAIN_REG 0x0A0202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio7_ncs : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /* IIO7 NCS Credits */
    UINT32 iio8_ncs : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* IIO8 NCS Credits. */
    UINT32 iio9_ncs : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000001*/

                            /* IIO9 NCS Credits. */
    UINT32 iio10_ncs : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /* IIO10 NCS Credits. */
    UINT32 iio11_ncs : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* IIO11 NCS Credits. */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M3KPCIR3CR_M3KTI_MAIN_STRUCT;


#endif /* _M3KTI_MAIN_h */
