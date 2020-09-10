
/** @file
  SMEE_TME.h

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


#ifndef _SMEE_TME_h
#define _SMEE_TME_h
#include <Base.h>

/* TME_CMI_N0_SMEE_TME_REG supported on:                                        */
/*      SPRA0 (0x20005000)                                                      */
/*      SPRB0 (0x20005000)                                                      */
/*      SPRHBM (0x20005000)                                                     */
/*      SPRC0 (0x20005000)                                                      */
/*      SPRMCC (0x20005000)                                                     */
/*      SPRUCC (0x20005000)                                                     */
/* Register default value on SPRA0: 0x00083D09                                  */
/* Register default value on SPRB0: 0x00083D09                                  */
/* Register default value on SPRHBM: 0x00083D09                                 */
/* Register default value on SPRC0: 0x00083D09                                  */
/* Register default value on SPRMCC: 0x00083D09                                 */
/* Register default value on SPRUCC: 0x00083D09                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for CMI related fields
*/


#define TME_CMI_N0_SMEE_TME_REG 0x1C125000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 req_fab_credits_in : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000009*/

                            /* CMI request fabric credits in */
    UINT32 rsp_fab_credits_in : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000008*/

                            /*
                               CMI fabric credits for responses coming in from
                               mcchan
                            */
    UINT32 rd_cpl_fab_input_credits : 6;

                            /* Bits[15:10], Access Type=RW, default=0x0000000F*/

                            /* CMI rd_cpl input fabric credits */
    UINT32 rsp_fab_input_credits : 5;

                            /* Bits[20:16], Access Type=RW, default=0x00000008*/

                            /* CMI response input fabric credits */
    UINT32 credit_config_done : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Credits in this CR have been initialized by
                               software
                            */
    UINT32 rsp_ism_credits_initialized : 1;

                            /* Bits[22:22], Access Type=RW/V, default=0x00000000*/

                            /* Responder ISM credits have been initialized */
    UINT32 req_ism_credits_initialized : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /* Requestor ISM credits have been initialized */
    UINT32 ism_req_idle_timeout_enable : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Enable bit for ISM IDLE_REQ timeout forward
                               progress feature
                            */
    UINT32 ism_req_idle_timeout_error_signal : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Enable bit to assert an error when ISM IDLE_REQ
                               timeout threshold is reached
                            */
    UINT32 ism_req_idle_timeout : 6;

                            /* Bits[31:26], Access Type=RW, default=0x00000000*/

                            /* ISM IDLE_REQ timeout feature threshold */

  } Bits;
  UINT32 Data;

} TME_CMI_N0_SMEE_TME_STRUCT;

/* TME_CMI_N1_SMEE_TME_REG supported on:                                        */
/*      SPRA0 (0x20005004)                                                      */
/*      SPRB0 (0x20005004)                                                      */
/*      SPRHBM (0x20005004)                                                     */
/*      SPRC0 (0x20005004)                                                      */
/*      SPRMCC (0x20005004)                                                     */
/*      SPRUCC (0x20005004)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for CMI related fields
*/


#define TME_CMI_N1_SMEE_TME_REG 0x1C125004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ism_req_idle_timeout : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /* ISM IDLE_REQ timeout feature threshold */
    UINT32 cmd_only_rdb_vc_refund : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               When 0, RDB structure credit is refunded when
                               RDB msg tracker entry is deallocated after all
                               message components are sent. When 1, RDB
                               structure credit is refunded after the command
                               is issued out of the RDB.
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_CMI_N1_SMEE_TME_STRUCT;

/* TME_RDB_N0_SMEE_TME_REG supported on:                                        */
/*      SPRA0 (0x20005008)                                                      */
/*      SPRB0 (0x20005008)                                                      */
/*      SPRHBM (0x20005008)                                                     */
/*      SPRC0 (0x20005008)                                                      */
/*      SPRMCC (0x20005008)                                                     */
/*      SPRUCC (0x20005008)                                                     */
/* Register default value on SPRA0: 0x425294AC                                  */
/* Register default value on SPRB0: 0x425294AC                                  */
/* Register default value on SPRHBM: 0x425294AC                                 */
/* Register default value on SPRC0: 0x425294AC                                  */
/* Register default value on SPRMCC: 0x425294AC                                 */
/* Register default value on SPRUCC: 0x425294AC                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for RDB structures
*/


#define TME_RDB_N0_SMEE_TME_REG 0x1C125008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdb_shared_cnt : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000000C*/

                            /* Initial number of shared credits for RDB */
    UINT32 rd_cpl_vc0_floor : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000005*/

                            /*
                               Initial number of floor credits for VC0 rd_cpl
                               credit pool
                            */
    UINT32 rd_cpl_vc1_floor : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000005*/

                            /*
                               Initial number of floor credits for VC1 rd_cpl
                               credit pool
                            */
    UINT32 rd_cpl_vc2_floor : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000005*/

                            /*
                               Initial number of floor credits for VC2 rd_cpl
                               credit pool
                            */
    UINT32 rd_cpl_vc3_floor : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000005*/

                            /*
                               Initial number of floor credits for VC3 rd_cpl
                               credit pool
                            */
    UINT32 rd_cpl_arb_weight : 5;

                            /* Bits[29:25], Access Type=RW, default=0x00000001*/

                            /* WRR weight for non-bypassed read completions */
    UINT32 byp_rd_cpl_arb_weight : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000001*/

                            /* WRR weight for bypassed read completions */

  } Bits;
  UINT32 Data;

} TME_RDB_N0_SMEE_TME_STRUCT;

/* TME_RDB_N1_SMEE_TME_REG supported on:                                        */
/*      SPRA0 (0x2000500c)                                                      */
/*      SPRB0 (0x2000500c)                                                      */
/*      SPRHBM (0x2000500c)                                                     */
/*      SPRC0 (0x2000500c)                                                      */
/*      SPRMCC (0x2000500c)                                                     */
/*      SPRUCC (0x2000500c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for RDB structures
*/


#define TME_RDB_N1_SMEE_TME_REG 0x1C12500C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 byp_rd_cpl_arb_weight : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000001*/

                            /* WRR weight for bypassed read completions */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_RDB_N1_SMEE_TME_STRUCT;

/* TME_SMEE_TME_REG supported on:                                               */
/*      SPRA0 (0x20005010)                                                      */
/*      SPRB0 (0x20005010)                                                      */
/*      SPRHBM (0x20005010)                                                     */
/*      SPRC0 (0x20005010)                                                      */
/*      SPRMCC (0x20005010)                                                     */
/*      SPRUCC (0x20005010)                                                     */
/* Register default value on SPRA0: 0x01FFF932                                  */
/* Register default value on SPRB0: 0x03FFF932                                  */
/* Register default value on SPRHBM: 0x03FFF932                                 */
/* Register default value on SPRC0: 0x07FFF932                                  */
/* Register default value on SPRMCC: 0x07FFF932                                 */
/* Register default value on SPRUCC: 0x07FFF932                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* General TME configuration
*/


#define TME_SMEE_TME_REG 0x1C125010

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_cmd_data_spacing : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000002*/

                            /* rd_cpl cmd-to-data spacing */
    UINT32 req_output_credits : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000013*/

                            /*
                               Output buffer request credits. Unused on wave 3
                               products and beyond.
                            */
    UINT32 disable_clk_gating : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Disables power-related clock gating */
    UINT32 use_rd_cpl_fab_buffer : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Use TME rd_cpl fabric buffer when TME is not
                               active
                            */
    UINT32 delay_wrptl_cmd_puts : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Implement HSD 1909164765 fix and delay CMI wr
                               credit puts to M2M used for wrptl flow
                            */
    UINT32 credit_misc_error_enable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               crediting errors
                            */
    UINT32 credit_over_sub_error_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded
                            */
    UINT32 credit_cfg_over_sub_error_enable : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded due to incorrect firmware
                               credit programming
                            */
    UINT32 rf_parity_error_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected RF parity
                               errors
                            */
    UINT32 overflow_error_enable : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO overflow
                               conditions
                            */
    UINT32 underflow_error_enable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO underflow
                               conditions
                            */
    UINT32 misc_error_enable : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               error conditions
                            */
    UINT32 mac_error_enable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected integrity
                               failure errors
                            */
    UINT32 tdx_error_enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 tem_error_enable : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TEM errors */
    UINT32 bypass_enable_vec : 3;

                            /* Bits[24:22], Access Type=RW, default=0x00000007*/

                            /*
                               Enable various hardware bypass mechanisms to
                               improve latency. Bit 0: enable CMI rd_cpl_cmd
                               valid to feed directly into the decryptor valid
                               input. Bit 1: Enable non-decrypt completion data
                               to bypass directly from completion fabric buffer
                               out to CMI. Bit 2: Enable decryptor data output
                               to bypass RDB and drive directly to CMI.
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_SMEE_TME_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_cmd_data_spacing : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000002*/

                            /* rd_cpl cmd-to-data spacing */
    UINT32 req_output_credits : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000013*/

                            /*
                               Output buffer request credits. Unused on wave 3
                               products and beyond.
                            */
    UINT32 disable_clk_gating : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Disables power-related clock gating */
    UINT32 use_rd_cpl_fab_buffer : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Use TME rd_cpl fabric buffer when TME is not
                               active
                            */
    UINT32 delay_wrptl_cmd_puts : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Implement HSD 1909164765 fix and delay CMI wr
                               credit puts to M2M used for wrptl flow
                            */
    UINT32 credit_misc_error_enable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               crediting errors
                            */
    UINT32 credit_over_sub_error_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded
                            */
    UINT32 credit_cfg_over_sub_error_enable : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded due to incorrect firmware
                               credit programming
                            */
    UINT32 rf_parity_error_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected RF parity
                               errors
                            */
    UINT32 overflow_error_enable : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO overflow
                               conditions
                            */
    UINT32 underflow_error_enable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO underflow
                               conditions
                            */
    UINT32 misc_error_enable : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               error conditions
                            */
    UINT32 mac_error_enable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected integrity
                               failure errors
                            */
    UINT32 tdx_error_enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 tem_error_enable : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TEM errors */
    UINT32 key_poison_error_enable : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 bypass_enable_vec : 3;

                            /* Bits[25:23], Access Type=RW, default=0x00000007*/

                            /*
                               Enable various hardware bypass mechanisms to
                               improve latency. Bit 0: enable CMI rd_cpl_cmd
                               valid to feed directly into the decryptor valid
                               input. Bit 1: Enable non-decrypt completion data
                               to bypass directly from completion fabric buffer
                               out to CMI. Bit 2: Enable decryptor data output
                               to bypass RDB and drive directly to CMI.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_SMEE_TME_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_cmd_data_spacing : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000002*/

                            /* rd_cpl cmd-to-data spacing */
    UINT32 req_output_credits : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000013*/

                            /*
                               Output buffer request credits. Unused on wave 3
                               products and beyond.
                            */
    UINT32 disable_clk_gating : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Disables power-related clock gating */
    UINT32 use_rd_cpl_fab_buffer : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Use TME rd_cpl fabric buffer when TME is not
                               active
                            */
    UINT32 delay_wrptl_cmd_puts : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Implement HSD 1909164765 fix and delay CMI wr
                               credit puts to M2M used for wrptl flow
                            */
    UINT32 credit_misc_error_enable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               crediting errors
                            */
    UINT32 credit_over_sub_error_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded
                            */
    UINT32 credit_cfg_over_sub_error_enable : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded due to incorrect firmware
                               credit programming
                            */
    UINT32 rf_parity_error_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected RF parity
                               errors
                            */
    UINT32 overflow_error_enable : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO overflow
                               conditions
                            */
    UINT32 underflow_error_enable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO underflow
                               conditions
                            */
    UINT32 misc_error_enable : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               error conditions
                            */
    UINT32 mac_error_enable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected integrity
                               failure errors
                            */
    UINT32 tdx_error_enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 tem_error_enable : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TEM errors */
    UINT32 key_poison_error_enable : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 bypass_enable_vec : 3;

                            /* Bits[25:23], Access Type=RW, default=0x00000007*/

                            /*
                               Enable various hardware bypass mechanisms to
                               improve latency. Bit 0: enable CMI rd_cpl_cmd
                               valid to feed directly into the decryptor valid
                               input. Bit 1: Enable non-decrypt completion data
                               to bypass directly from completion fabric buffer
                               out to CMI. Bit 2: Enable decryptor data output
                               to bypass RDB and drive directly to CMI.
                            */
    UINT32 engine_sdc_error_enable : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Enable signaling of detected SDC errors in the
                               encrypt, decrypt, and tweak security engines
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_SMEE_TME_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_cmd_data_spacing : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000002*/

                            /* rd_cpl cmd-to-data spacing */
    UINT32 req_output_credits : 5;

                            /* Bits[8:4], Access Type=RW, default=0x00000013*/

                            /*
                               Output buffer request credits. Unused on wave 3
                               products and beyond.
                            */
    UINT32 disable_clk_gating : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Disables power-related clock gating */
    UINT32 use_rd_cpl_fab_buffer : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Use TME rd_cpl fabric buffer when TME is not
                               active
                            */
    UINT32 delay_wrptl_cmd_puts : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Implement HSD 1909164765 fix and delay CMI wr
                               credit puts to M2M used for wrptl flow
                            */
    UINT32 credit_misc_error_enable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               crediting errors
                            */
    UINT32 credit_over_sub_error_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded
                            */
    UINT32 credit_cfg_over_sub_error_enable : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected over
                               subscription crediting errors where structure
                               capacity is exceeded due to incorrect firmware
                               credit programming
                            */
    UINT32 rf_parity_error_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected RF parity
                               errors
                            */
    UINT32 overflow_error_enable : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO overflow
                               conditions
                            */
    UINT32 underflow_error_enable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected FIFO underflow
                               conditions
                            */
    UINT32 misc_error_enable : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected miscellaneous
                               error conditions
                            */
    UINT32 mac_error_enable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Enable MCA signaling for detected integrity
                               failure errors
                            */
    UINT32 tdx_error_enable : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TDX errors */
    UINT32 tem_error_enable : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Enable signaling of detected TEM errors */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_SMEE_TME_STRUCT;

/* PRMRR_SMEE_TME_REG supported on:                                             */
/*      SPRA0 (0x20005018)                                                      */
/*      SPRB0 (0x20005018)                                                      */
/*      SPRHBM (0x20005018)                                                     */
/*      SPRC0 (0x20005018)                                                      */
/*      SPRMCC (0x20005018)                                                     */
/*      SPRUCC (0x20005018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* General TME configuration
*/


#define PRMRR_SMEE_TME_REG 0x1C125018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 secured : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* prmrr secured bit */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PRMRR_SMEE_TME_STRUCT;





































































































































































































































































































































































































































































































































/* TME_PERSISTENT1_BASE_SMEE_TME_REG supported on:                              */
/*      SPRA0 (0x20005830)                                                      */
/*      SPRB0 (0x20005830)                                                      */
/*      SPRHBM (0x20005830)                                                     */
/*      SPRC0 (0x20005830)                                                      */
/*      SPRMCC (0x20005830)                                                     */
/*      SPRUCC (0x20005830)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Offset to be subtracted from AD system address
*/


#define TME_PERSISTENT1_BASE_SMEE_TME_REG 0x1C125830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Base physical address to be subtracted from AD
                               system address to generate tweak
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_PERSISTENT1_BASE_SMEE_TME_STRUCT;

/* TME_PERSISTENT1_LIMIT_SMEE_TME_REG supported on:                             */
/*      SPRA0 (0x20005838)                                                      */
/*      SPRB0 (0x20005838)                                                      */
/*      SPRHBM (0x20005838)                                                     */
/*      SPRC0 (0x20005838)                                                      */
/*      SPRMCC (0x20005838)                                                     */
/*      SPRUCC (0x20005838)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Mask used to identify AD system address
*/


#define TME_PERSISTENT1_LIMIT_SMEE_TME_REG 0x1C125838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the bits that must match
                               ADBASE in order to qualify as an AD memory range
                               access
                            */
    UINT32 rsvd : 9;

                            /* Bits[30:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 range_enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable base/mask check for AD */

  } Bits;
  UINT32 Data;

} TME_PERSISTENT1_LIMIT_SMEE_TME_STRUCT;

/* TME_PERSISTENT2_BASE_SMEE_TME_REG supported on:                              */
/*      SPRA0 (0x20005840)                                                      */
/*      SPRB0 (0x20005840)                                                      */
/*      SPRHBM (0x20005840)                                                     */
/*      SPRC0 (0x20005840)                                                      */
/*      SPRMCC (0x20005840)                                                     */
/*      SPRUCC (0x20005840)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Offset to be subtracted from AD system address
*/


#define TME_PERSISTENT2_BASE_SMEE_TME_REG 0x1C125840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 base : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               Base physical address to be subtracted from AD
                               system address to generate tweak
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_PERSISTENT2_BASE_SMEE_TME_STRUCT;

/* TME_PERSISTENT2_LIMIT_SMEE_TME_REG supported on:                             */
/*      SPRA0 (0x20005848)                                                      */
/*      SPRB0 (0x20005848)                                                      */
/*      SPRHBM (0x20005848)                                                     */
/*      SPRC0 (0x20005848)                                                      */
/*      SPRMCC (0x20005848)                                                     */
/*      SPRUCC (0x20005848)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* Mask used to identify AD system address
*/


#define TME_PERSISTENT2_LIMIT_SMEE_TME_REG 0x1C125848

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 limit : 22;

                            /* Bits[21:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the bits that must match
                               ADBASE in order to qualify as an AD memory range
                               access
                            */
    UINT32 rsvd : 9;

                            /* Bits[30:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 range_enable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable base/mask check for AD */

  } Bits;
  UINT32 Data;

} TME_PERSISTENT2_LIMIT_SMEE_TME_STRUCT;

/* TME_PERSISTENT_ADDR_IDS_SMEE_TME_REG supported on:                           */
/*      SPRA0 (0x20005850)                                                      */
/*      SPRB0 (0x20005850)                                                      */
/*      SPRHBM (0x20005850)                                                     */
/*      SPRC0 (0x20005850)                                                      */
/*      SPRMCC (0x20005850)                                                     */
/*      SPRUCC (0x20005850)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* IDs to be appended to AD/AD-WB tweak input address
*/


#define TME_PERSISTENT_ADDR_IDS_SMEE_TME_REG 0x1C125850

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_id : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* ID used to indicate AD access */
    UINT32 adwb_id : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000000*/

                            /* ID used to indicate AD-WB access */
    UINT32 apped_id : 12;

                            /* Bits[15:4], Access Type=RW, default=0x00000000*/

                            /*
                               ID to append to AD/AD-WB tweak input address.
                               This ID should be unique for each socket.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_PERSISTENT_ADDR_IDS_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_RD_POLICY_N0_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x20005858)                                                      */
/*      SPRB0 (0x20005858)                                                      */
/*      SPRHBM (0x20005858)                                                     */
/*      SPRC0 (0x20005858)                                                      */
/*      SPRMCC (0x20005858)                                                     */
/*      SPRUCC (0x20005858)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define TME_SAI_LB_MCHECK_W_RD_POLICY_N0_SMEE_TME_REG 0x1C125858

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

} TME_SAI_LB_MCHECK_W_RD_POLICY_N0_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_RD_POLICY_N1_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x2000585c)                                                      */
/*      SPRB0 (0x2000585c)                                                      */
/*      SPRHBM (0x2000585c)                                                     */
/*      SPRC0 (0x2000585c)                                                      */
/*      SPRMCC (0x2000585c)                                                     */
/*      SPRUCC (0x2000585c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the read access to registers in LB_MCHECK_W security policy group. As indicated by the _W in the policy name, the read access for this particular policy includes ALL supported SAI mnemonics. That is, read access is permitted to all IPs for registers in this group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define TME_SAI_LB_MCHECK_W_RD_POLICY_N1_SMEE_TME_REG 0x1C12585C

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

} TME_SAI_LB_MCHECK_W_RD_POLICY_N1_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_WR_POLICY_N0_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x20005860)                                                      */
/*      SPRB0 (0x20005860)                                                      */
/*      SPRHBM (0x20005860)                                                     */
/*      SPRC0 (0x20005860)                                                      */
/*      SPRMCC (0x20005860)                                                     */
/*      SPRUCC (0x20005860)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define TME_SAI_LB_MCHECK_W_WR_POLICY_N0_SMEE_TME_REG 0x1C125860

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

} TME_SAI_LB_MCHECK_W_WR_POLICY_N0_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_WR_POLICY_N1_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x20005864)                                                      */
/*      SPRB0 (0x20005864)                                                      */
/*      SPRHBM (0x20005864)                                                     */
/*      SPRC0 (0x20005864)                                                      */
/*      SPRMCC (0x20005864)                                                     */
/*      SPRUCC (0x20005864)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to registers in LB_MCHECK_W security policy group. SAI bits corresponding to pcode/ucode/Intel Dfx are read only to prevent BIOS from removing access for them.
*/


#define TME_SAI_LB_MCHECK_W_WR_POLICY_N1_SMEE_TME_REG 0x1C125864

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

} TME_SAI_LB_MCHECK_W_WR_POLICY_N1_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_CR_POLICY_N0_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x20005868)                                                      */
/*      SPRB0 (0x20005868)                                                      */
/*      SPRHBM (0x20005868)                                                     */
/*      SPRC0 (0x20005868)                                                      */
/*      SPRMCC (0x20005868)                                                     */
/*      SPRUCC (0x20005868)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define TME_SAI_LB_MCHECK_W_CR_POLICY_N0_SMEE_TME_REG 0x1C125868

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

} TME_SAI_LB_MCHECK_W_CR_POLICY_N0_SMEE_TME_STRUCT;

/* TME_SAI_LB_MCHECK_W_CR_POLICY_N1_SMEE_TME_REG supported on:                  */
/*      SPRA0 (0x2000586c)                                                      */
/*      SPRB0 (0x2000586c)                                                      */
/*      SPRHBM (0x2000586c)                                                     */
/*      SPRC0 (0x2000586c)                                                      */
/*      SPRMCC (0x2000586c)                                                     */
/*      SPRUCC (0x2000586c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* This register configures the write access to RAC, WAC and CP registers for LB_MCHECK_W security policy group. This policy group is expected to be used for registers that had LB attribute in prior art and need to be checked by MCHECK. BIOS is in the policy group for this register (BOOT and SMM). Bits corresponding to trusted sources (pcode, ucode, Intel Dfx) are made read-only in the CP/RAC/WAC to prevent BIOS from accidentally/maliciously removing access to the trusted sources
*/


#define TME_SAI_LB_MCHECK_W_CR_POLICY_N1_SMEE_TME_REG 0x1C12586C

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

} TME_SAI_LB_MCHECK_W_CR_POLICY_N1_SMEE_TME_STRUCT;


/* TME_CMI_QDEPTH_N0_SMEE_TME_REG supported on:                                 */
/*      SPRA0 (0x20005878)                                                      */
/*      SPRB0 (0x20005878)                                                      */
/*      SPRHBM (0x20005878)                                                     */
/*      SPRC0 (0x20005878)                                                      */
/*      SPRMCC (0x20005878)                                                     */
/*      SPRUCC (0x20005878)                                                     */
/* Register default value on SPRA0: 0x20000000                                  */
/* Register default value on SPRB0: 0x20000000                                  */
/* Register default value on SPRHBM: 0x20000000                                 */
/* Register default value on SPRC0: 0x20000000                                  */
/* Register default value on SPRMCC: 0x20000000                                 */
/* Register default value on SPRUCC: 0x20000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for CMI qdepth related fields
*/


#define TME_CMI_QDEPTH_N0_SMEE_TME_REG 0x1C125878

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_qdepth_shared_credits : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Shared rd_cpl qdepth credits */
    UINT32 rd_cpl_qdepth_vc_valid_vec : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               qdepth valid bit vector denoting which VCs
                               should honor qdepth for read completion
                               crediting
                            */
    UINT32 vc0_rd_cpl_qdepth_credits : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* VC0 read completion qdepth credits */
    UINT32 vc1_rd_cpl_qdepth_credits : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* VC1 read completion qdepth credits */
    UINT32 vc2_rd_cpl_qdepth_credits : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* VC2 read completion qdepth credits */
    UINT32 vc3_rd_cpl_qdepth_credits : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* VC3 read completion qdepth credits */
    UINT32 rd_cpl_qdepth_to_peer : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000002*/

                            /*
                               This is the rd_cpl qdepth value that will be
                               exchanged with mcchan. This defaults to the
                               depth of the RDB and should only be set to
                               values less than this for debug/testing
                               purposes.
                            */

  } Bits;
  UINT32 Data;

} TME_CMI_QDEPTH_N0_SMEE_TME_STRUCT;

/* TME_CMI_QDEPTH_N1_SMEE_TME_REG supported on:                                 */
/*      SPRA0 (0x2000587c)                                                      */
/*      SPRB0 (0x2000587c)                                                      */
/*      SPRHBM (0x2000587c)                                                     */
/*      SPRC0 (0x2000587c)                                                      */
/*      SPRMCC (0x2000587c)                                                     */
/*      SPRUCC (0x2000587c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Configuration register for CMI qdepth related fields
*/


#define TME_CMI_QDEPTH_N1_SMEE_TME_REG 0x1C12587C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_cpl_qdepth_to_peer : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000001*/

                            /*
                               This is the rd_cpl qdepth value that will be
                               exchanged with mcchan. This defaults to the
                               depth of the RDB and should only be set to
                               values less than this for debug/testing
                               purposes.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_CMI_QDEPTH_N1_SMEE_TME_STRUCT;

/* TME_STATUS_N0_SMEE_TME_REG supported on:                                     */
/*      SPRA0 (0x20005880)                                                      */
/*      SPRB0 (0x20005880)                                                      */
/*      SPRHBM (0x20005880)                                                     */
/*      SPRC0 (0x20005880)                                                      */
/*      SPRMCC (0x20005880)                                                     */
/*      SPRUCC (0x20005880)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register holds various fields relating to the current TME status
*/


#define TME_STATUS_N0_SMEE_TME_REG 0x1C125880

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 non_tdx_cmd_tdx_error_seen : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               When this bit is set, it means that a TDX bit
                               mismatch was detected for a non-TDX read command
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TME_STATUS_N0_SMEE_TME_STRUCT;

/* TME_STATUS_N1_SMEE_TME_REG supported on:                                     */
/*      SPRA0 (0x20005884)                                                      */
/*      SPRB0 (0x20005884)                                                      */
/*      SPRHBM (0x20005884)                                                     */
/*      SPRC0 (0x20005884)                                                      */
/*      SPRMCC (0x20005884)                                                     */
/*      SPRUCC (0x20005884)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register holds various fields relating to the current TME status
*/


#define TME_STATUS_N1_SMEE_TME_REG 0x1C125884

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

} TME_STATUS_N1_SMEE_TME_STRUCT;


/* CRDEFEATURE1_SMEE_SMEE_TME_REG supported on:                                 */
/*      SPRA0 (0x10005890)                                                      */
/*      SPRB0 (0x10005890)                                                      */
/*      SPRHBM (0x10005890)                                                     */
/*      SPRC0 (0x10005890)                                                      */
/*      SPRMCC (0x10005890)                                                     */
/*      SPRUCC (0x10005890)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* spare
*/


#define CRDEFEATURE1_SMEE_SMEE_TME_REG 0x1C115890

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

} CRDEFEATURE1_SMEE_SMEE_TME_STRUCT;

/* CRDEFEATURE3_SMEE_SMEE_TME_REG supported on:                                 */
/*      SPRA0 (0x10005898)                                                      */
/*      SPRB0 (0x10005898)                                                      */
/*      SPRHBM (0x10005898)                                                     */
/*      SPRC0 (0x10005898)                                                      */
/*      SPRMCC (0x10005898)                                                     */
/*      SPRUCC (0x10005898)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRB0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRHBM Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRC0 Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                 */
/* SPRMCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* SPRUCC Register File:    sprsp_top/tme_reg[0]/tme/MEM_TME_CSR                */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* spare
*/


#define CRDEFEATURE3_SMEE_SMEE_TME_REG 0x1C115898

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

} CRDEFEATURE3_SMEE_SMEE_TME_STRUCT;
#endif /* _SMEE_TME_h */
